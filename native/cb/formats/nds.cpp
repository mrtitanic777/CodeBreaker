#include "cb/formats/nds.hpp"

#include <algorithm>
#include <array>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/gba.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::nds {

namespace {

constexpr std::string_view kReader = "nds";
constexpr std::size_t kLogoAt = 0xC0, kLogoCrcAt = 0x15C, kHeaderCrcAt = 0x15E;

std::uint32_t word_at(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 |
           static_cast<std::uint32_t>(b[at + 3]) << 24;
}
std::uint16_t half_at(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | b[at + 1] << 8); }

std::string text_field(ByteView b, std::size_t at, std::size_t n) {
    std::string s;
    for (std::size_t i = 0; i < n; ++i) s += static_cast<char>(b[at + i]);
    while (!s.empty() && s.back() == '\0') s.pop_back();
    return s;
}

Binary binary_at(ByteView b, std::size_t at) { return {word_at(b, at), word_at(b, at + 4), word_at(b, at + 8), word_at(b, at + 12)}; }

// The file names the FNT gives, by file id: "dir/sub/name". Anything out of bounds ends that directory; a directory is visited once.
std::map<std::uint32_t, std::string> file_names(ByteView rom, std::uint32_t fnt_offset, std::uint32_t fnt_size) {
    std::map<std::uint32_t, std::string> names;
    if (fnt_size < 8 || static_cast<std::uint64_t>(fnt_offset) + fnt_size > rom.size()) return names;
    const ByteView fnt = rom.subspan(fnt_offset, fnt_size);
    const unsigned directories = std::min<unsigned>(half_at(fnt, 6), 0x1000);
    std::set<unsigned> seen;
    const auto walk = [&](auto&& self, unsigned dir, const std::string& prefix, unsigned depth) -> void {
        if (dir >= directories || depth > 16 || !seen.insert(dir).second || dir * 8u + 8 > fnt.size()) return;
        std::uint32_t at = word_at(fnt, dir * 8u);
        std::uint32_t id = half_at(fnt, dir * 8u + 4);
        while (at < fnt.size()) {
            const unsigned type = fnt[at++];
            if (type == 0) break;
            const unsigned length = type & 0x7F;
            if (at + length + ((type & 0x80) ? 2u : 0u) > fnt.size()) break;
            const std::string name = std::string(reinterpret_cast<const char*>(fnt.data() + at), length);
            at += length;
            if (type & 0x80) {
                const unsigned child = half_at(fnt, at) & 0xFFF;
                at += 2;
                self(self, child, prefix + name + "/", depth + 1);
            } else {
                names[id++] = prefix + name;
            }
        }
    };
    walk(walk, 0, "", 0);
    return names;
}

}  // namespace

std::uint16_t crc16(ByteView data) {
    std::uint32_t crc = 0xFFFF;
    for (const std::uint8_t b : data) {
        crc ^= b;
        for (int bit = 0; bit < 8; ++bit) crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
    return static_cast<std::uint16_t>(crc);
}

ByteView Rom::memory(Cpu cpu) const {
    const Binary& b = binary(cpu);
    if (static_cast<std::uint64_t>(b.rom_offset) + b.size > data.size()) return {};
    return ByteView(data).subspan(b.rom_offset, b.size);
}

std::optional<Cpu> Rom::cpu_of(std::uint32_t address) const {
    if (header.arm9.holds(address) && !memory(Cpu::Arm9).empty()) return Cpu::Arm9;
    if (header.arm7.holds(address) && !memory(Cpu::Arm7).empty()) return Cpu::Arm7;
    return std::nullopt;
}

std::vector<std::uint32_t> Rom::functions(Cpu cpu, const Decoder& decode) const {
    const Binary& b = binary(cpu);
    const ByteView mem = memory(cpu);
    std::vector<std::uint32_t> out;
    if (mem.empty()) return out;
    // the decoder reads addresses as offsets into the program, so the entry is given as one (its state bit kept)
    std::set<std::uint32_t> starts;
    if (b.holds(b.entry & ~1u)) starts.insert((b.entry & ~1u) - b.ram_address | (b.entry & 1u));
    const std::vector<std::uint32_t> found = find_functions(mem, std::move(starts), decode, {.begin = 0, .end = static_cast<std::uint32_t>(mem.size())});
    for (const std::uint32_t a : found) out.push_back((a & ~1u) + b.ram_address | (a & 1u));
    return out;
}

std::vector<Region> Rom::regions() const {
    std::vector<Region> out;
    const auto add = [&](Region r) {
        if (r.size == 0 || static_cast<std::uint64_t>(r.offset) + r.size > data.size()) return;
        for (const Region& have : out)
            if (r.offset < have.end() && have.offset < r.end()) return;
        out.push_back(std::move(r));
    };
    const Header& h = header;
    add({Region::Kind::Header, 0, static_cast<std::uint32_t>(kHeaderSize), 0, {}});
    add({Region::Kind::Arm9, h.arm9.rom_offset, h.arm9.size, 0, {}});
    add({Region::Kind::Arm7, h.arm7.rom_offset, h.arm7.size, 0, {}});
    add({Region::Kind::Fnt, h.fnt_offset, h.fnt_size, 0, {}});
    add({Region::Kind::Fat, h.fat_offset, h.fat_size, 0, {}});
    add({Region::Kind::Overlays9, h.ovl9_offset, h.ovl9_size, 0, {}});
    add({Region::Kind::Overlays7, h.ovl7_offset, h.ovl7_size, 0, {}});
    if (h.banner_offset != 0 && static_cast<std::uint64_t>(h.banner_offset) + 4 <= data.size()) {
        const std::uint16_t version = half_at(data, h.banner_offset);
        const std::uint32_t size = version == 1 ? 0x840 : version == 2 ? 0x940 : version == 3 ? 0xA40 : version == 0x103 ? 0x23C0 : 0;
        add({Region::Kind::Banner, h.banner_offset, size, 0, {}});
    }
    if (h.fat_size >= 8 && static_cast<std::uint64_t>(h.fat_offset) + h.fat_size <= data.size()) {
        const std::map<std::uint32_t, std::string> names = file_names(data, h.fnt_offset, h.fnt_size);
        const std::uint32_t count = std::min<std::uint32_t>(h.fat_size / 8, 0x10000);
        for (std::uint32_t id = 0; id < count; ++id) {
            const std::uint32_t start = word_at(data, h.fat_offset + id * 8), end = word_at(data, h.fat_offset + id * 8 + 4);
            if (end < start) continue;
            const auto name = names.find(id);
            add({Region::Kind::File, start, end - start, id, name == names.end() ? std::string() : name->second});
        }
    }
    std::ranges::sort(out, [](const Region& a, const Region& b) { return a.offset < b.offset; });
    return out;
}

Rom parse(ByteView data) {
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, "shorter than the 512-byte header");
    if (data.size() > kMaxRom) throw FormatError(kReader, 0, "larger than 512 MB");
    if (half_at(data, kLogoCrcAt) != kLogoCrc) throw FormatError(kReader, kLogoCrcAt, "not a Nintendo DS ROM (the logo checksum $CF56 is not at $15C)");
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    Header& h = rom.header;
    h.title = text_field(data, 0, 12);
    h.game_code = text_field(data, 0x0C, 4);
    h.maker_code = text_field(data, 0x10, 2);
    h.unit_code = data[0x12];
    h.seed = data[0x13];
    h.capacity = data[0x14];
    h.region = data[0x1D];
    h.version = data[0x1E];
    h.autostart = data[0x1F];
    h.arm9 = binary_at(data, 0x20);
    h.arm7 = binary_at(data, 0x30);
    h.fnt_offset = word_at(data, 0x40);
    h.fnt_size = word_at(data, 0x44);
    h.fat_offset = word_at(data, 0x48);
    h.fat_size = word_at(data, 0x4C);
    h.ovl9_offset = word_at(data, 0x50);
    h.ovl9_size = word_at(data, 0x54);
    h.ovl7_offset = word_at(data, 0x58);
    h.ovl7_size = word_at(data, 0x5C);
    h.banner_offset = word_at(data, 0x68);
    h.used_size = word_at(data, 0x80);
    h.header_size = word_at(data, 0x84);
    h.logo_ok = std::equal(gba::kLogo.begin(), gba::kLogo.end(), data.begin() + kLogoAt);
    h.logo_crc = half_at(data, kLogoCrcAt);
    h.header_crc = half_at(data, kHeaderCrcAt);
    h.computed_header_crc = crc16(data.subspan(0, kHeaderCrcAt));
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    if (!head || head->size() < kHeaderSize) return false;
    if (half_at(*head, kLogoCrcAt) != kLogoCrc) return false;
    return std::equal(gba::kLogo.begin(), gba::kLogo.end(), head->begin() + kLogoAt);
}

}  // namespace cb::formats::nds
