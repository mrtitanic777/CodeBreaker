// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/mips_elf.hpp"

#include <algorithm>
#include <optional>

#include "core/error.hpp"
#include "formats/gen/elf.hpp"

namespace cb::formats::mips_elf {

namespace {

constexpr std::string_view kReader = "mips_elf";

std::uint16_t le16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | b[at + 1] << 8); }
std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

// A name of elf.fmt's (a cstr, its terminator included), up to its terminator (empty when there is none).
std::string text_of(const std::optional<std::string_view>& t) {
    if (!t) return {};
    return std::string(t->substr(0, t->find('\0')));
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

// The layout is elf.fmt's (its generated reader, formats/gen/elf); that the tables lie in the file with entries of this
// format's size is this reader's rule.
Elf parse(ByteView data, std::uint16_t machine) {
    if (!probe_header(data, machine)) throw FormatError(kReader, 0, machine == kMachineMips ? "not a 32-bit little-endian MIPS ELF file" : "not a 32-bit little-endian ELF file of the machine asked for");
    const gen::elf::Elf e = gen::elf::read(data).elf;
    const gen::elf::Header& h = e.header;
    Elf elf;
    elf.data.assign(data.begin(), data.end());
    elf.type = h.type;
    elf.machine = h.machine;
    elf.version = h.version;
    elf.entry = static_cast<std::uint32_t>(h.entry);
    elf.phoff = static_cast<std::uint32_t>(h.phoff);
    elf.shoff = static_cast<std::uint32_t>(h.shoff);
    elf.flags = h.flags;
    elf.ehsize = h.ehsize;
    elf.phentsize = h.phentsize;
    elf.phnum = h.phnum;
    elf.shentsize = h.shentsize;
    elf.shnum = h.shnum;
    elf.shstrndx = h.shstrndx;
    if (elf.phnum != 0 && (elf.phentsize != kProgramHeaderSize || static_cast<std::uint64_t>(elf.phoff) + static_cast<std::uint64_t>(elf.phnum) * kProgramHeaderSize > data.size()))
        throw FormatError(kReader, elf.phoff, "the program header table is not inside the file");
    if (e.segments) {
        elf.program_headers.reserve(e.segments->size());
        for (const gen::elf::Segment& p : *e.segments)
            elf.program_headers.push_back({p.type, static_cast<std::uint32_t>(p.offset), static_cast<std::uint32_t>(p.vaddr), static_cast<std::uint32_t>(p.paddr),
                                           static_cast<std::uint32_t>(p.filesz), static_cast<std::uint32_t>(p.memsz), p.flags, static_cast<std::uint32_t>(p.align)});
    }
    if (elf.shnum != 0 && (elf.shentsize != kSectionHeaderSize || static_cast<std::uint64_t>(elf.shoff) + static_cast<std::uint64_t>(elf.shnum) * kSectionHeaderSize > data.size()))
        throw FormatError(kReader, elf.shoff, "the section header table is not inside the file");
    if (e.sections) {
        elf.sections.reserve(e.sections->size());
        for (std::size_t i = 0; i < e.sections->size(); ++i) {
            const gen::elf::Section& n = (*e.sections)[i];
            SectionHeader s{n.name_offset, n.type, static_cast<std::uint32_t>(n.flags), static_cast<std::uint32_t>(n.address), static_cast<std::uint32_t>(n.offset),
                            static_cast<std::uint32_t>(n.size), n.link, n.info, static_cast<std::uint32_t>(n.align), static_cast<std::uint32_t>(n.entsize), {}};
            if (e.names) s.name = text_of((*e.names)[i].text);
            elf.sections.push_back(std::move(s));
        }
    }
    return elf;
}

std::vector<Symbol> Elf::symbols() const {
    std::vector<Symbol> out;
    gen::elf::Options options;
    options.parts.symbols = true;
    const gen::elf::Elf e = gen::elf::read(data, options).elf;
    if (!e.symbols) return out;
    for (const gen::elf::SymbolTable& table : *e.symbols) {
        if (!table.entries) continue;
        for (const gen::elf::Symbol& n : *table.entries) {
            Symbol sym;
            sym.name = text_of(n.name);
            sym.value = static_cast<std::uint32_t>(n.value);
            sym.size = static_cast<std::uint32_t>(n.size);
            sym.type = static_cast<std::uint8_t>(n.type);
            sym.bind = static_cast<std::uint8_t>(n.bind);
            sym.section = n.section;
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
