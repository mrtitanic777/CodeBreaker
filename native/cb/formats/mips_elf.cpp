// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/mips_elf.hpp"

#include <algorithm>
#include <optional>

#include "cb/core/error.hpp"

namespace cb::formats::mips_elf {

namespace {

constexpr std::string_view kReader = "mips_elf";

std::uint16_t le16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | b[at + 1] << 8); }
std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

std::string c_string(ByteView data, std::size_t at, std::size_t limit) {
    std::string s;
    for (std::size_t i = at; i < limit && i < data.size() && data[i] != 0; ++i) s += static_cast<char>(data[i]);
    return s;
}

constexpr std::uint32_t kPtLoad = 1;

}  // namespace

bool probe_header(ByteView head, std::uint16_t machine) {
    return head.size() >= kHeaderSize && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 1 && head[5] == 1 && le16(head, 18) == machine;
}

bool probe_header(ByteView head) { return probe_header(head, kMachineMips); }

std::uint32_t machine_of(std::uint32_t flags) {
    const std::uint32_t m = flags & kMachineMask;
    return m == kMach5900 || m == kMachAllegrex ? m : 0;
}

Elf parse(ByteView data) { return parse(data, kMachineMips); }

Elf parse(ByteView data, std::uint16_t machine) {
    if (!probe_header(data, machine)) throw FormatError(kReader, 0, machine == kMachineMips ? "not a 32-bit little-endian MIPS ELF file" : "not a 32-bit little-endian ELF file of the machine asked for");
    Elf elf;
    elf.data.assign(data.begin(), data.end());
    elf.type = le16(data, 16);
    elf.machine = le16(data, 18);
    elf.version = le32(data, 20);
    elf.entry = le32(data, 24);
    elf.phoff = le32(data, 28);
    elf.shoff = le32(data, 32);
    elf.flags = le32(data, 36);
    elf.ehsize = le16(data, 40);
    elf.phentsize = le16(data, 42);
    elf.phnum = le16(data, 44);
    elf.shentsize = le16(data, 46);
    elf.shnum = le16(data, 48);
    elf.shstrndx = le16(data, 50);
    if (elf.phnum != 0 && (elf.phentsize != kProgramHeaderSize || static_cast<std::uint64_t>(elf.phoff) + static_cast<std::uint64_t>(elf.phnum) * kProgramHeaderSize > data.size()))
        throw FormatError(kReader, elf.phoff, "the program header table is not inside the file");
    for (std::size_t i = 0; i < elf.phnum; ++i) {
        const std::size_t at = elf.phoff + i * kProgramHeaderSize;
        ProgramHeader p;
        p.type = le32(data, at);
        p.offset = le32(data, at + 4);
        p.vaddr = le32(data, at + 8);
        p.paddr = le32(data, at + 12);
        p.filesz = le32(data, at + 16);
        p.memsz = le32(data, at + 20);
        p.flags = le32(data, at + 24);
        p.align = le32(data, at + 28);
        elf.program_headers.push_back(p);
    }
    if (elf.shnum != 0 && (elf.shentsize != kSectionHeaderSize || static_cast<std::uint64_t>(elf.shoff) + static_cast<std::uint64_t>(elf.shnum) * kSectionHeaderSize > data.size()))
        throw FormatError(kReader, elf.shoff, "the section header table is not inside the file");
    for (std::size_t i = 0; i < elf.shnum; ++i) {
        const std::size_t at = elf.shoff + i * kSectionHeaderSize;
        SectionHeader s;
        s.name_offset = le32(data, at);
        s.type = le32(data, at + 4);
        s.flags = le32(data, at + 8);
        s.address = le32(data, at + 12);
        s.offset = le32(data, at + 16);
        s.size = le32(data, at + 20);
        s.link = le32(data, at + 24);
        s.info = le32(data, at + 28);
        s.align = le32(data, at + 32);
        s.entsize = le32(data, at + 36);
        elf.sections.push_back(s);
    }
    if (elf.shstrndx < elf.sections.size()) {
        const SectionHeader& names = elf.sections[elf.shstrndx];
        if (names.type != kTypeNobits && static_cast<std::uint64_t>(names.offset) + names.size <= data.size())
            for (SectionHeader& s : elf.sections)
                if (s.name_offset < names.size) s.name = c_string(data, names.offset + s.name_offset, static_cast<std::size_t>(names.offset) + names.size);
    }
    return elf;
}

std::vector<Symbol> Elf::symbols() const {
    std::vector<Symbol> out;
    for (const SectionHeader& s : sections) {
        if (s.type != kTypeSymtab || s.entsize != 16 || s.link >= sections.size()) continue;
        const SectionHeader& strings = sections[s.link];
        if (strings.type == kTypeNobits || static_cast<std::uint64_t>(strings.offset) + strings.size > data.size()) continue;
        if (static_cast<std::uint64_t>(s.offset) + s.size > data.size()) continue;
        for (std::size_t at = s.offset; at + 16 <= static_cast<std::size_t>(s.offset) + s.size; at += 16) {
            Symbol sym;
            const std::uint32_t name_at = le32(data, at);
            sym.value = le32(data, at + 4);
            sym.size = le32(data, at + 8);
            sym.type = data[at + 12] & 15;
            sym.bind = data[at + 12] >> 4;
            sym.section = le16(data, at + 14);
            if (name_at != 0 && name_at < strings.size) sym.name = c_string(data, strings.offset + name_at, static_cast<std::size_t>(strings.offset) + strings.size);
            out.push_back(std::move(sym));
        }
    }
    return out;
}

std::optional<std::uint32_t> Elf::address_of(std::uint32_t offset) const {
    for (const ProgramHeader& p : program_headers)
        if (p.type == kPtLoad && offset >= p.offset && offset - p.offset < p.filesz) return p.vaddr + (offset - p.offset);
    return std::nullopt;
}

std::optional<std::uint32_t> Elf::offset_of(std::uint32_t address) const {
    for (const ProgramHeader& p : program_headers)
        if (p.type == kPtLoad && address >= p.vaddr && address - p.vaddr < p.filesz) return p.offset + (address - p.vaddr);
    return std::nullopt;
}

std::optional<ByteView> Elf::bytes_at(std::uint32_t address, std::size_t size) const {
    for (const ProgramHeader& p : program_headers)
        if (p.type == kPtLoad && address >= p.vaddr && address - p.vaddr < p.filesz) {
            const std::size_t room = p.filesz - (address - p.vaddr);
            const std::size_t at = p.offset + (address - p.vaddr);
            if (size > room || at + size > data.size()) return std::nullopt;
            return ByteView(data).subspan(at, size);
        }
    return std::nullopt;
}

std::optional<std::string> Elf::string_at(std::uint32_t address) const {
    std::string out;
    for (std::uint32_t a = address; out.size() < 256; ++a) {
        const auto b = bytes_at(a, 1);
        if (!b) return std::nullopt;
        if ((*b)[0] == 0) return out;
        out += static_cast<char>((*b)[0]);
    }
    return std::nullopt;
}

std::optional<std::uint32_t> Elf::word_at(std::uint32_t address) const {
    const auto b = bytes_at(address, 4);
    if (!b) return std::nullopt;
    return le32(*b, 0);
}

const SectionHeader* Elf::section(const std::string& name) const {
    for (const SectionHeader& s : sections)
        if (s.name == name) return &s;
    return nullptr;
}

}  // namespace cb::formats::mips_elf
