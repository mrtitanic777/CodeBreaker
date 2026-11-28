// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/ppc_elf.hpp"

#include "formats/deflate.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ppc_elf {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "ppc_elf";
constexpr std::size_t kMaxSection = std::size_t{512} << 20;

std::uint16_t be16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] << 8 | b[at + 1]); }
std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}
void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 24);
    b[at + 1] = static_cast<std::uint8_t>(v >> 16);
    b[at + 2] = static_cast<std::uint8_t>(v >> 8);
    b[at + 3] = static_cast<std::uint8_t>(v);
}

// The zlib stream inflated to exactly `expected` bytes; FormatError otherwise.
Bytes inflate_exact(ByteView in, std::size_t expected, std::size_t at) {
    deflate::Inflated r = deflate::inflate_exact(in, expected);
    if (r.status != deflate::Status::end || r.data.size() != expected) throw FormatError(kReader, at, "a compressed section does not inflate to its size");
    return std::move(r.data);
}

std::uint32_t u32_of(const eng::Node& n, std::string_view field) { return static_cast<std::uint32_t>(n.u(field)); }

// The headers of the file as it stands (no inflating). The layout is elf.fmt's (read big-endian: elfbe.fmt); that the
// tables and the sections lie in the file is this reader's rule.
Elf parse_headers(ByteView data) {
    if (!is_ppc_elf(data)) throw FormatError(kReader, 0, "not a 32-bit big-endian PowerPC ELF executable");
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    const eng::Tree tree = eng::builtin().read("elfbe", data, options);
    const eng::Node& e = tree.root.get("elf");
    const eng::Node& id = e.get("ident");
    const eng::Node& h = e.get("header");
    Elf elf;
    elf.data.assign(data.begin(), data.end());
    elf.os_abi = static_cast<std::uint8_t>(id.u("os_abi"));
    elf.abi_version = static_cast<std::uint8_t>(id.u("abi_version"));
    elf.type = static_cast<std::uint16_t>(h.u("type"));
    elf.machine = static_cast<std::uint16_t>(h.u("machine"));
    elf.version = u32_of(h, "version");
    elf.entry = u32_of(h, "entry");
    elf.phoff = u32_of(h, "phoff");
    elf.shoff = u32_of(h, "shoff");
    elf.flags = u32_of(h, "flags");
    elf.ehsize = static_cast<std::uint16_t>(h.u("ehsize"));
    elf.phentsize = static_cast<std::uint16_t>(h.u("phentsize"));
    elf.phnum = static_cast<std::uint16_t>(h.u("phnum"));
    elf.shentsize = static_cast<std::uint16_t>(h.u("shentsize"));
    elf.shnum = static_cast<std::uint16_t>(h.u("shnum"));
    elf.shstrndx = static_cast<std::uint16_t>(h.u("shstrndx"));
    elf.wiiu = elf.os_abi == kOsAbiCafe || elf.type == kTypeRpl;
    if (elf.phnum != 0) {
        if (elf.phentsize != kProgramHeaderSize || static_cast<std::uint64_t>(elf.phoff) + std::uint64_t{elf.phnum} * kProgramHeaderSize > data.size())
            throw FormatError(kReader, 28, "the program header table is outside the file");
        for (const eng::Node& p : e.get("segments").children)
            elf.program_headers.push_back({u32_of(p, "type"), u32_of(p, "offset"), u32_of(p, "vaddr"), u32_of(p, "paddr"), u32_of(p, "filesz"), u32_of(p, "memsz"),
                                           u32_of(p, "flags"), u32_of(p, "align")});
    }
    if (elf.shnum != 0) {
        if (elf.shentsize != kSectionHeaderSize || static_cast<std::uint64_t>(elf.shoff) + std::uint64_t{elf.shnum} * kSectionHeaderSize > data.size())
            throw FormatError(kReader, 32, "the section header table is outside the file");
        const eng::Node& sections = e.get("sections");
        for (std::size_t i = 0; i < sections.children.size(); ++i) {
            const eng::Node& n = sections.children[i];
            SectionHeader s{u32_of(n, "name_offset"), u32_of(n, "type"), u32_of(n, "flags"), u32_of(n, "address"), u32_of(n, "offset"), u32_of(n, "size"),
                            u32_of(n, "link"), u32_of(n, "info"), u32_of(n, "align"), u32_of(n, "entsize"), {}};
            if (s.type != kTypeNobits && s.type != 0 && static_cast<std::uint64_t>(s.offset) + s.size > data.size())
                throw FormatError(kReader, n.offset, std::format("section {} lies outside the file", i));
            elf.sections.push_back(std::move(s));
        }
        // the names, unless the string table is itself stored compressed (they are read again once it is inflated)
        const eng::Node* names = e.find("names");
        if (names && !(elf.sections[elf.shstrndx].flags & kFlagDeflated))
            for (std::size_t i = 0; i < elf.sections.size(); ++i)
                if (const eng::Node* t = names->children[i].child("text")) elf.sections[i].name = t->text.substr(0, t->text.find('\0'));
    }
    return elf;
}

// The file with its compressed sections inflated, every later offset moved by what the sections grew.
Bytes inflated_file(const Elf& elf) {
    struct Replacement {
        std::size_t index;
        std::uint32_t old_offset, old_size;
        Bytes data;
    };
    std::vector<Replacement> changes;
    for (std::size_t i = 0; i < elf.sections.size(); ++i) {
        const SectionHeader& s = elf.sections[i];
        if (!(s.flags & kFlagDeflated) || s.type == kTypeNobits) continue;
        if (s.size < 4 + 2) throw FormatError(kReader, s.offset, "a compressed section is too short");
        const std::size_t plain = be32(elf.data, s.offset);
        if (plain > kMaxSection) throw FormatError(kReader, s.offset, "a compressed section claims an absurd size");
        changes.push_back({i, s.offset, s.size, inflate_exact(ByteView(elf.data).subspan(s.offset + 4, s.size - 4), plain, s.offset)});
    }
    std::ranges::sort(changes, {}, &Replacement::old_offset);
    // what moves: an offset is shifted by what the sections that end at or before it grew
    const auto shifted = [&](std::uint32_t offset) {
        std::int64_t delta = 0;
        for (const Replacement& c : changes)
            if (static_cast<std::uint64_t>(c.old_offset) + c.old_size <= offset) delta += static_cast<std::int64_t>(c.data.size()) - c.old_size;
        return static_cast<std::uint32_t>(offset + delta);
    };
    Bytes out;
    std::size_t at = 0;
    for (const Replacement& c : changes) {
        if (c.old_offset < at) throw FormatError(kReader, c.old_offset, "compressed sections overlap");
        out.insert(out.end(), elf.data.begin() + static_cast<std::ptrdiff_t>(at), elf.data.begin() + c.old_offset);
        out.insert(out.end(), c.data.begin(), c.data.end());
        at = static_cast<std::size_t>(c.old_offset) + c.old_size;
    }
    out.insert(out.end(), elf.data.begin() + static_cast<std::ptrdiff_t>(at), elf.data.end());
    // the headers say where things are now
    if (elf.shnum != 0) {
        const std::size_t table = shifted(elf.shoff);
        put32(out, 32, static_cast<std::uint32_t>(table));
        for (std::size_t i = 0; i < elf.sections.size(); ++i) {
            const SectionHeader& s = elf.sections[i];
            const std::size_t h = table + i * kSectionHeaderSize;
            const auto change = std::ranges::find(changes, i, &Replacement::index);
            if (change != changes.end()) {
                put32(out, h + 8, s.flags & ~kFlagDeflated);
                put32(out, h + 16, shifted(s.offset));
                put32(out, h + 20, static_cast<std::uint32_t>(change->data.size()));
            } else if (s.type != kTypeNobits && s.type != 0) {
                put32(out, h + 16, shifted(s.offset));
            }
        }
    }
    if (elf.phnum != 0) {
        const std::size_t table = shifted(elf.phoff);
        put32(out, 28, static_cast<std::uint32_t>(table));
        for (unsigned i = 0; i < elf.phnum; ++i) put32(out, table + i * kProgramHeaderSize + 4, shifted(elf.program_headers[i].offset));
    }
    return out;
}

// Does the code hold a `lis rD, 0xCDxx` (the Wii Hollywood registers)?
bool touches_wii(ByteView code) {
    for (std::size_t at = 0; at + 4 <= code.size(); at += 4) {
        const std::uint32_t w = be32(code, at);
        if ((w & 0xFC1F0000) != 0x3C000000) continue;
        const std::uint32_t imm = w & 0xFFFF;
        if (imm >= 0xCD00 && imm <= 0xCDFF) return true;
    }
    return false;
}

}  // namespace

bool is_ppc_elf(ByteView head) {
    if (head.size() < kHeaderSize) return false;
    if (head[0] != 0x7F || head[1] != 'E' || head[2] != 'L' || head[3] != 'F') return false;
    if (head[4] != 1 || head[5] != 2) return false;   // 32-bit, big-endian
    const std::uint16_t type = be16(head, 16), machine = be16(head, 18);
    return machine == kMachinePpc && (type == kTypeExec || type == kTypeRpl);
}

Elf parse(ByteView data) {
    Elf elf = parse_headers(data);
    std::size_t deflated = 0;
    for (const SectionHeader& s : elf.sections) deflated += (s.flags & kFlagDeflated) != 0 && s.type != kTypeNobits;
    if (deflated == 0) return elf;
    const Bytes plain = inflated_file(elf);
    Elf out = parse_headers(plain);
    out.inflated_sections = deflated;
    return out;
}

Elf read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    return head && is_ppc_elf(*head);
}

std::vector<ppc_segment::Segment> Elf::segments() const {
    std::vector<ppc_segment::Segment> out;
    for (const SectionHeader& s : sections) {
        if (!(s.flags & kFlagAlloc) || s.type == kTypeNobits || s.type == 0 || s.size == 0) continue;
        if (s.address == 0) continue;
        out.push_back({s.name, s.address, s.offset, s.size, s.size, (s.flags & kFlagExec) != 0});
    }
    if (out.empty()) {
        for (const ProgramHeader& p : program_headers) {
            if (p.type != 1 || p.filesz == 0) continue;
            out.push_back({std::format("segment{}", out.size()), p.vaddr, p.offset, p.filesz, p.memsz, (p.flags & 1) != 0});
        }
    }
    return out;
}

dol::Console Elf::console() const {
    for (const ppc_segment::Segment& s : segments()) {
        if (s.address >= 0x90000000 && s.address < 0xA0000000) return dol::Console::Wii;
        if (s.code && touches_wii(ByteView(data).subspan(s.offset, s.size))) return dol::Console::Wii;
    }
    return dol::Console::GameCube;
}

}  // namespace cb::formats::ppc_elf
