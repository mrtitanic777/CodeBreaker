// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/ppc_elf.hpp"

#include "formats/deflate.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ppc_elf {

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

// The headers of the file as it stands (no inflating).
Elf parse_headers(ByteView data) {
    if (!is_ppc_elf(data)) throw FormatError(kReader, 0, "not a 32-bit big-endian PowerPC ELF executable");
    Elf elf;
    elf.data.assign(data.begin(), data.end());
    elf.os_abi = data[7];
    elf.abi_version = data[8];
    elf.type = be16(data, 16);
    elf.machine = be16(data, 18);
    elf.version = be32(data, 20);
    elf.entry = be32(data, 24);
    elf.phoff = be32(data, 28);
    elf.shoff = be32(data, 32);
    elf.flags = be32(data, 36);
    elf.ehsize = be16(data, 40);
    elf.phentsize = be16(data, 42);
    elf.phnum = be16(data, 44);
    elf.shentsize = be16(data, 46);
    elf.shnum = be16(data, 48);
    elf.shstrndx = be16(data, 50);
    elf.wiiu = elf.os_abi == kOsAbiCafe || elf.type == kTypeRpl;
    if (elf.phnum != 0) {
        if (elf.phentsize != kProgramHeaderSize || static_cast<std::uint64_t>(elf.phoff) + std::uint64_t{elf.phnum} * kProgramHeaderSize > data.size())
            throw FormatError(kReader, 28, "the program header table is outside the file");
        for (unsigned i = 0; i < elf.phnum; ++i) {
            const std::size_t at = elf.phoff + i * kProgramHeaderSize;
            elf.program_headers.push_back({be32(data, at), be32(data, at + 4), be32(data, at + 8), be32(data, at + 12), be32(data, at + 16), be32(data, at + 20), be32(data, at + 24), be32(data, at + 28)});
        }
    }
    if (elf.shnum != 0) {
        if (elf.shentsize != kSectionHeaderSize || static_cast<std::uint64_t>(elf.shoff) + std::uint64_t{elf.shnum} * kSectionHeaderSize > data.size())
            throw FormatError(kReader, 32, "the section header table is outside the file");
        for (unsigned i = 0; i < elf.shnum; ++i) {
            const std::size_t at = elf.shoff + i * kSectionHeaderSize;
            SectionHeader s;
            s.name_offset = be32(data, at);
            s.type = be32(data, at + 4);
            s.flags = be32(data, at + 8);
            s.address = be32(data, at + 12);
            s.offset = be32(data, at + 16);
            s.size = be32(data, at + 20);
            s.link = be32(data, at + 24);
            s.info = be32(data, at + 28);
            s.align = be32(data, at + 32);
            s.entsize = be32(data, at + 36);
            if (s.type != kTypeNobits && s.type != 0 && static_cast<std::uint64_t>(s.offset) + s.size > data.size())
                throw FormatError(kReader, at, std::format("section {} lies outside the file", i));
            elf.sections.push_back(std::move(s));
        }
        if (elf.shstrndx < elf.sections.size()) {
            const SectionHeader& strings = elf.sections[elf.shstrndx];
            if (strings.type != kTypeNobits && !(strings.flags & kFlagDeflated)) {
                const ByteView table = ByteView(elf.data).subspan(strings.offset, strings.size);
                for (SectionHeader& s : elf.sections) {
                    if (s.name_offset >= table.size()) continue;
                    const auto end = std::find(table.begin() + s.name_offset, table.end(), std::uint8_t{0});
                    s.name.assign(reinterpret_cast<const char*>(table.data() + s.name_offset), static_cast<std::size_t>(end - (table.begin() + s.name_offset)));
                }
            }
        }
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
