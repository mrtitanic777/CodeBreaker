// cb-unity: alone
#include "formats/vita.hpp"

#include "formats/deflate.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::vita {

namespace {

constexpr std::string_view kReader = "vita";
constexpr std::uint32_t kPtLoad = 1;

std::uint16_t le16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] | d[at + 1] << 8); }
std::uint32_t le32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) | static_cast<std::uint32_t>(d[at + 1]) << 8 | static_cast<std::uint32_t>(d[at + 2]) << 16 | static_cast<std::uint32_t>(d[at + 3]) << 24;
}
std::uint64_t le64(ByteView d, std::size_t at) { return le32(d, at) | static_cast<std::uint64_t>(le32(d, at + 4)) << 32; }
void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}
void put16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}
std::uint32_t align16(std::uint32_t v) { return (v + 15) & ~15u; }

bool is_self(ByteView h) { return h.size() >= 0x60 && h[0] == 'S' && h[1] == 'C' && h[2] == 'E' && h[3] == 0 && le32(h, 4) == 3; }
bool is_sony_arm_elf(ByteView h) {
    if (h.size() < 52 || h[0] != 0x7F || h[1] != 'E' || h[2] != 'L' || h[3] != 'F' || h[4] != 1 || h[5] != 1 || le16(h, 18) != kMachineArm) return false;
    const std::uint16_t type = le16(h, 16);
    return type >= 0xFE00 && type <= 0xFE04;
}

Bytes inflate_exact(ByteView in, std::size_t expected, std::size_t at) {
    deflate::Inflated r = deflate::inflate_exact(in, expected);
    if (r.status != deflate::Status::end || r.data.size() != expected) throw FormatError(kReader, at, "a compressed segment does not inflate to its size");
    return std::move(r.data);
}

struct Phdr {
    std::uint32_t type, offset, vaddr, filesz, memsz, flags;
};

// What the program's module information says: the name, the entries and the imports (pointers are read where they point in the image).
void read_module_info(Program& p, std::uint32_t entry_field) {
    const std::size_t seg_index = entry_field >> 30;
    if (seg_index >= p.segments.size()) return;
    const Segment& home = p.segments[seg_index];
    const std::uint32_t offset = entry_field & 0x3FFFFFFFu;
    const ByteView image = p.image;
    const std::size_t at = static_cast<std::size_t>(home.image_offset) + offset;
    if (offset + 76 > home.size || at + 76 > image.size()) return;
    p.module_name = std::string(reinterpret_cast<const char*>(image.data()) + at + 4, 27).c_str();
    // a pointer inside the module information is an offset in the home segment; one inside an import is an address (or a segment-tagged offset)
    const auto in_home = [&](std::uint32_t v) -> std::optional<std::size_t> {
        const std::uint32_t o = v & 0x3FFFFFFFu;
        return o < home.size ? std::optional<std::size_t>(home.image_offset + o) : std::nullopt;
    };
    const auto resolve = [&](std::uint32_t v) -> std::optional<std::size_t> {
        if (const auto o = p.offset_of(v)) return *o;
        const std::size_t seg = v >> 30;
        if (seg < p.segments.size() && (v & 0x3FFFFFFFu) < p.segments[seg].size) return p.segments[seg].image_offset + (v & 0x3FFFFFFFu);
        return std::nullopt;
    };
    for (const std::size_t field : {68u, 72u}) {
        const std::uint32_t v = le32(image, at + field);
        if (v != 0 && v != 0xFFFFFFFFu && (v & 0x3FFFFFFFu) < home.size) p.entries.push_back(home.address + (v & 0x3FFFFFFEu) + (v & 1));
    }
    const auto top = in_home(le32(image, at + 44)), end = in_home(le32(image, at + 48));
    if (!top || !end) return;
    for (std::size_t pos = *top; pos + 52 <= *end && pos + 52 <= image.size();) {
        const std::uint16_t size = le16(image, pos);
        if (size < 0x34) break;
        const std::uint16_t functions = le16(image, pos + 6);
        const std::uint32_t library_nid = le32(image, pos + 16);
        const auto name = resolve(le32(image, pos + 20));
        const auto nids = resolve(le32(image, pos + 28)), stubs = resolve(le32(image, pos + 32));
        std::string library = library_name(library_nid);
        if (library.empty() && name) library = std::string(reinterpret_cast<const char*>(image.data()) + *name, std::min<std::size_t>(image.size() - *name, 64)).c_str();
        if (library.empty()) library = std::format("lib_{:08X}", library_nid);
        for (unsigned k = 0; nids && stubs && k < functions && *nids + 4 * (k + 1) <= image.size() && *stubs + 4 * (k + 1) <= image.size(); ++k) {
            Import i;
            i.library = library;
            i.library_nid = library_nid;
            i.nid = le32(image, *nids + 4 * k);
            i.name = function_name(i.nid);
            i.stub = le32(image, *stubs + 4 * k) & ~1u;
            p.imports.push_back(std::move(i));
        }
        pos += size;
    }
}

Program build(ByteView elf_header, const std::vector<Phdr>& phdrs, const std::vector<Bytes>& data, bool self) {
    Program p;
    p.self = self;
    p.elf_type = le16(elf_header, 16);
    p.elf_entry = le32(elf_header, 24);
    p.elf_flags = le32(elf_header, 36);
    for (std::size_t k = 0; k < phdrs.size(); ++k) {
        if (phdrs[k].type != kPtLoad) continue;
        Segment s;
        s.image_offset = align16(static_cast<std::uint32_t>(p.image.size()));
        p.image.resize(s.image_offset, 0);
        s.size = static_cast<std::uint32_t>(data[k].size());
        s.address = phdrs[k].vaddr;
        s.memory_size = phdrs[k].memsz;
        s.flags = phdrs[k].flags;
        p.image.insert(p.image.end(), data[k].begin(), data[k].end());
        p.segments.push_back(s);
    }
    if (p.segments.empty()) throw FormatError(kReader, 0, "the program has no loadable segment");
    read_module_info(p, p.elf_entry);
    return p;
}

Program parse_self(ByteView d) {
    const std::uint64_t elf_at = le64(d, 0x40), phdr_at = le64(d, 0x48), info_at = le64(d, 0x58);
    if (elf_at + 52 > d.size() || phdr_at > d.size()) throw FormatError(kReader, 0x40, "the SELF's ELF header is outside the file");
    const ByteView elf = d.subspan(elf_at);
    if (!is_sony_arm_elf(elf)) throw FormatError(kReader, elf_at, "the SELF does not hold a Vita ELF");
    const unsigned count = le16(elf, 44);
    if (phdr_at + 32ull * count > d.size() || info_at + 32ull * count > d.size()) throw FormatError(kReader, phdr_at, "the SELF's tables are outside the file");
    std::vector<Phdr> phdrs;
    std::vector<Bytes> data;
    for (unsigned k = 0; k < count; ++k) {
        const std::size_t at = phdr_at + 32ull * k;
        phdrs.push_back({le32(d, at), le32(d, at + 4), le32(d, at + 8), le32(d, at + 16), le32(d, at + 20), le32(d, at + 24)});
        const std::size_t info = info_at + 32ull * k;
        const std::uint64_t offset = le64(d, info), length = le64(d, info + 8), compression = le64(d, info + 16), encrypted = le64(d, info + 24);
        if (phdrs.back().type != kPtLoad) {
            data.emplace_back();
            continue;
        }
        if (encrypted == 1) throw FormatError(kReader, info, "the SELF's segments are encrypted: not handled (CodeBreaker does not decrypt)");
        if (offset + length > d.size()) throw FormatError(kReader, info, "a segment is outside the file");
        const ByteView raw = d.subspan(offset, length);
        data.push_back(compression == 2 ? inflate_exact(raw, phdrs.back().filesz, offset) : Bytes(raw.begin(), raw.end()));
    }
    return build(elf, phdrs, data, true);
}

Program parse_elf(ByteView d) {
    const unsigned count = le16(d, 44);
    const std::uint32_t phoff = le32(d, 28);
    if (phoff + 32ull * count > d.size()) throw FormatError(kReader, phoff, "the program header table is outside the file");
    std::vector<Phdr> phdrs;
    std::vector<Bytes> data;
    for (unsigned k = 0; k < count; ++k) {
        const std::size_t at = phoff + 32ull * k;
        phdrs.push_back({le32(d, at), le32(d, at + 4), le32(d, at + 8), le32(d, at + 16), le32(d, at + 20), le32(d, at + 24)});
        if (phdrs.back().type != kPtLoad) {
            data.emplace_back();
            continue;
        }
        if (static_cast<std::uint64_t>(phdrs.back().offset) + phdrs.back().filesz > d.size()) throw FormatError(kReader, at, "a segment is outside the file");
        data.emplace_back(d.begin() + phdrs.back().offset, d.begin() + phdrs.back().offset + phdrs.back().filesz);
    }
    return build(d, phdrs, data, false);
}

}  // namespace

std::optional<std::uint32_t> Program::offset_of(std::uint32_t address) const {
    for (const Segment& s : segments)
        if (address >= s.address && address - s.address < s.size) return s.image_offset + (address - s.address);
    return std::nullopt;
}

std::optional<std::uint32_t> Program::address_of(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (offset >= s.image_offset && offset - s.image_offset < s.size) return s.address + (offset - s.image_offset);
    return std::nullopt;
}

bool Program::in_code(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (s.code() && offset >= s.image_offset && offset - s.image_offset < s.size) return true;
    return false;
}

Bytes Program::to_elf(ByteView body) const {
    const std::uint32_t headers = align16(static_cast<std::uint32_t>(52 + 32 * segments.size()));
    Bytes out(headers + body.size(), 0);
    out[0] = 0x7F;
    out[1] = 'E';
    out[2] = 'L';
    out[3] = 'F';
    out[4] = 1;
    out[5] = 1;
    out[6] = 1;
    put16(out, 16, elf_type);
    put16(out, 18, kMachineArm);
    put32(out, 20, 1);
    put32(out, 24, elf_entry);
    put32(out, 28, 52);
    put32(out, 36, elf_flags);
    put16(out, 40, 52);
    put16(out, 42, 32);
    put16(out, 44, static_cast<std::uint32_t>(segments.size()));
    for (std::size_t k = 0; k < segments.size(); ++k) {
        const Segment& s = segments[k];
        const std::size_t at = 52 + 32 * k;
        put32(out, at, kPtLoad);
        put32(out, at + 4, headers + s.image_offset);
        put32(out, at + 8, s.address);
        put32(out, at + 12, s.address);
        put32(out, at + 16, s.size);
        put32(out, at + 20, s.memory_size);
        put32(out, at + 24, s.flags);
        put32(out, at + 28, 0x1000);
    }
    std::copy(body.begin(), body.end(), out.begin() + headers);
    return out;
}

Program parse(ByteView data) {
    if (is_self(data)) return parse_self(data);
    if (is_sony_arm_elf(data)) return parse_elf(data);
    throw FormatError(kReader, 0, "not a Vita SELF or ELF");
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".self", ".velf", ".bin", ".elf", ".suprx"}) ) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 1024);
    return head && (is_self(*head) || is_sony_arm_elf(*head));
}

}  // namespace cb::formats::vita
