// cb-unity: alone
#include "formats/threedo.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::threedo {

namespace {

constexpr std::string_view kReader = "threedo";

std::uint32_t be32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) << 24 | static_cast<std::uint32_t>(d[at + 1]) << 16 | static_cast<std::uint32_t>(d[at + 2]) << 8 | d[at + 3];
}

std::string text_of(ByteView d, std::size_t at, std::size_t length) {
    std::string s(reinterpret_cast<const char*>(d.data()) + at, length);
    const std::size_t end = s.find('\0');
    if (end != std::string::npos) s.resize(end);
    return s;
}

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// Does the file read as an AIF: word 3 a BL or a NOP, word 4 the exit SWI, the areas' sizes inside the file?
bool looks_like_aif(ByteView d) {
    if (d.size() < kAifHeader + 4) return false;
    const std::uint32_t entry = be32(d, 12), exit = be32(d, 16);
    if (entry != 0xE1A00000 && (entry >> 24) != 0xEB) return false;
    if ((exit >> 24) != 0xEF) return false;
    const std::uint64_t areas = static_cast<std::uint64_t>(be32(d, 20)) + be32(d, 24);
    return areas != 0 && areas <= d.size() + 0x1000;
}

Aif parse_aif(ByteView d) {
    Aif a;
    const std::uint32_t entry = be32(d, 12);
    if ((entry >> 24) == 0xEB) {
        const std::int32_t offset = static_cast<std::int32_t>(entry << 8) >> 6;   // the 24-bit word offset, times four
        a.entry = static_cast<std::uint32_t>(12 + 8 + offset);
    }
    a.read_only = be32(d, 20);
    a.read_write = be32(d, 24);
    a.debug = be32(d, 28);
    a.zero_init = be32(d, 32);
    a.base = be32(d, 40);
    a.mode = be32(d, 48);
    return a;
}

}  // namespace

bool is_volume_header(ByteView sector) {
    return sector.size() >= 7 && sector[0] == 0x01 && sector[1] == 0x5A && sector[2] == 0x5A && sector[3] == 0x5A && sector[4] == 0x5A && sector[5] == 0x5A &&
           sector[6] == 0x01;
}

Bytes Program::native() const {
    const ByteView img = image();
    Bytes out(img.size() + (4 - img.size() % 4) % 4, 0);
    std::copy(img.begin(), img.end(), out.begin());
    for (std::size_t i = 0; i < out.size(); i += 4) std::reverse(out.begin() + static_cast<std::ptrdiff_t>(i), out.begin() + static_cast<std::ptrdiff_t>(i) + 4);
    return out;
}

std::vector<std::uint32_t> Program::functions(const Decoder& decode) const {
    const Bytes mem = native();
    std::set<std::uint32_t> starts;
    if (aif.entry < mem.size()) starts.insert(aif.entry);
    return find_functions(mem, std::move(starts), decode, {.begin = 0, .end = static_cast<std::uint32_t>(mem.size()), .even_targets_only = true});
}

Program parse(ByteView data) {
    Program p;
    if (is_volume_header(data)) {
        if (data.size() < 100) throw FormatError(kReader, 0, "the volume header is cut short");
        p.kind = Kind::Disc;
        p.file.assign(data.begin(), data.end());
        p.label = text_of(data, 39, 32);
        const std::uint32_t block = be32(data, 75);
        const std::uint32_t root_blocks = be32(data, 87);
        const std::uint32_t root_copy = be32(data, 99);
        if (block == 0 || block > 0x10000) throw FormatError(kReader, 75, std::format("block size {}", block));
        std::uint64_t at = static_cast<std::uint64_t>(root_copy) * block;
        if (at >= data.size()) throw FormatError(kReader, 99, "the root directory is past the end of the image");
        // walk the root directory's blocks, entry by entry
        std::uint32_t seen = 0;
        bool last = false;
        while (!last && seen++ < std::max<std::uint32_t>(root_blocks, 1)) {
            if (at + 20 > data.size()) break;
            const std::uint32_t next = be32(data, at), first_free = be32(data, at + 12), first = be32(data, at + 16);
            std::uint64_t entry_at = at + first;
            while (entry_at + 72 <= data.size() && entry_at < at + std::min<std::uint64_t>(first_free, block)) {
                Entry e;
                e.flags = be32(data, entry_at);
                e.type = be32(data, entry_at + 8);
                e.block_size = be32(data, entry_at + 12);
                e.byte_count = be32(data, entry_at + 16);
                e.name = text_of(data, entry_at + 32, 32);
                const std::uint32_t copies = be32(data, entry_at + 64);
                e.first_block = be32(data, entry_at + 68);
                p.root.push_back(e);
                entry_at += 68 + 4 * (static_cast<std::uint64_t>(std::min<std::uint32_t>(copies, 64)) + 1);
                if (e.flags & 0x80000000u) last = true;
                if (e.flags & 0x40000000u) break;
            }
            if (next == 0xFFFFFFFFu || next == 0) break;
            at = static_cast<std::uint64_t>(next) * block;
            if (at >= data.size()) break;
        }
        const auto found = std::find_if(p.root.begin(), p.root.end(), [](const Entry& e) { return !e.directory() && lower(e.name) == "launchme"; });
        if (found == p.root.end()) throw FormatError(kReader, 0, "the disc's root directory has no LaunchMe");
        const std::uint64_t start = static_cast<std::uint64_t>(found->first_block) * (found->block_size ? found->block_size : block);
        if (start + found->byte_count > data.size()) throw FormatError(kReader, start, "LaunchMe runs past the end of the image");
        p.name = found->name;
        p.image_offset = static_cast<std::uint32_t>(start);
        p.image_size = found->byte_count;
    } else {
        p.kind = Kind::Aif;
        p.file.assign(data.begin(), data.end());
        p.image_size = static_cast<std::uint32_t>(data.size());
    }
    if (!looks_like_aif(p.image())) throw FormatError(kReader, p.image_offset, "not an AIF executable (word 4 is not the exit SWI)");
    p.aif = parse_aif(p.image());
    if (p.aif.entry >= p.image_size) throw FormatError(kReader, p.image_offset + 12, "the entry is outside the image");
    return p;
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool iso = has_extension(path, {".iso", ".bin", ".img"});
    const std::string name = lower(path.filename().string());
    const bool aif = has_extension(path, {".aif"}) || name == "launchme";
    if (!iso && !aif) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, kSector);
    if (!head) return false;
    if (iso && is_volume_header(*head)) return true;
    return aif && looks_like_aif(*head);
}

}  // namespace cb::formats::threedo
