// cb-unity: alone
#include "formats/jaguar.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::jaguar {

namespace {

constexpr std::string_view kReader = "jaguar";
constexpr std::uint16_t kCoffMagic = 0x0150;   // the 68000's, big-endian
constexpr std::size_t kCoffHeader = 20;
constexpr std::size_t kSectionHeader = 40;
constexpr std::uint32_t kStypBss = 0x80;

std::uint32_t be32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) << 24 | static_cast<std::uint32_t>(d[at + 1]) << 16 | static_cast<std::uint32_t>(d[at + 2]) << 8 | d[at + 3];
}
std::uint16_t be16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] << 8 | d[at + 1]); }

bool is_coff(ByteView d) { return d.size() >= kCoffHeader && be16(d, 0) == kCoffMagic && be16(d, 2) != 0 && be16(d, 2) <= 32; }

Program parse_coff(ByteView d) {
    Program p;
    p.kind = Kind::Coff;
    p.file.assign(d.begin(), d.end());
    const std::size_t sections = be16(d, 2);
    const std::size_t optional_header = be16(d, 16);
    if (optional_header < 28) throw FormatError(kReader, 16, "a COFF without the a.out header (no entry address)");
    p.entry = be32(d, kCoffHeader + 16);   // a.out: magic, tsize, dsize, bsize, entry
    const std::size_t first = kCoffHeader + optional_header;
    if (first + sections * kSectionHeader > d.size()) throw FormatError(kReader, first, "the section headers are cut short");
    for (std::size_t i = 0; i < sections; ++i) {
        const std::size_t h = first + i * kSectionHeader;
        Segment s;
        s.name.assign(reinterpret_cast<const char*>(d.data()) + h, 8);
        s.name.resize(std::strlen(s.name.c_str()));
        s.address = be32(d, h + 12);   // s_vaddr
        s.size = be32(d, h + 16);
        s.offset = be32(d, h + 20);
        const std::uint32_t flags = be32(d, h + 36);
        if (s.size == 0 || s.offset == 0 || (flags & kStypBss) != 0) continue;
        if (static_cast<std::uint64_t>(s.offset) + s.size > d.size()) throw FormatError(kReader, h, std::format("section {} runs past the end of the file", s.name));
        if (static_cast<std::uint64_t>(s.address) + s.size > kMemorySize) throw FormatError(kReader, h, std::format("section {} is outside the 24-bit address space", s.name));
        p.segments.push_back(std::move(s));
    }
    if (p.segments.empty()) throw FormatError(kReader, first, "no section holds bytes");
    if (!p.offset(p.entry)) throw FormatError(kReader, kCoffHeader, std::format("the entry address ${:06X} is in no section", p.entry));
    return p;
}

Program parse_cartridge(ByteView d) {
    if (d.size() < kHeaderAt + 8 || d.size() > kRomMax) throw FormatError(kReader, 0, std::format("{} bytes is not a Jaguar cartridge (up to 6 MB, with the header at $400)", d.size()));
    Program p;
    p.kind = Kind::Cartridge;
    p.file.assign(d.begin(), d.end());
    p.header_flags = be32(d, kHeaderAt);
    p.entry = be32(d, kHeaderAt + 4);
    p.segments.push_back({kRomBase, 0, static_cast<std::uint32_t>(d.size()), "ROM"});
    if (p.entry % 2 != 0 || !p.offset(p.entry)) throw FormatError(kReader, kHeaderAt + 4, std::format("the start address ${:08X} is not in the ROM", p.entry));
    return p;
}

}  // namespace

const Segment* Program::segment_at(std::uint32_t address) const {
    for (const Segment& s : segments) {
        if (address >= s.address && address - s.address < s.size) return &s;
    }
    return nullptr;
}

std::optional<std::uint32_t> Program::offset(std::uint32_t address) const {
    const Segment* s = segment_at(address);
    if (!s) return std::nullopt;
    return s->offset + (address - s->address);
}

Bytes Program::memory() const {
    Bytes mem(kMemorySize, 0);
    for (const Segment& s : segments) std::copy_n(file.begin() + s.offset, s.size, mem.begin() + s.address);
    return mem;
}

std::vector<std::uint32_t> Program::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::vector<std::uint32_t> all;
    std::set<std::uint32_t> seen;
    for (const Segment& s : segments) {
        std::set<std::uint32_t> starts;
        if (entry >= s.address && entry - s.address < s.size) starts.insert(entry);
        if (starts.empty()) continue;
        for (const std::uint32_t a : find_functions(mem, std::move(starts), decode, {.begin = s.address, .end = s.address + s.size, .even_targets_only = true})) {
            if (seen.insert(a).second) all.push_back(a);
        }
    }
    std::sort(all.begin(), all.end());
    return all;
}

Program parse(ByteView data) {
    if (is_coff(data)) return parse_coff(data);
    return parse_cartridge(data);
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool cart_name = has_extension(path, {".j64", ".jag", ".rom"});
    const bool coff_name = has_extension(path, {".cof", ".abs", ".jag"});
    if (!cart_name && !coff_name) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderAt + 8);
    if (!head) return false;
    if (coff_name && is_coff(*head)) return true;
    if (!cart_name) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size < kHeaderAt + 8 || *size > kRomMax || head->size() < kHeaderAt + 8) return false;
    const std::uint32_t start = be32(*head, kHeaderAt + 4);
    return start % 2 == 0 && start >= kRomBase + kHeaderAt + 8 && start < kRomBase + *size;
}

}  // namespace cb::formats::jaguar
