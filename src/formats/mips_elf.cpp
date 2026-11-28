// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/mips_elf.hpp"

#include <algorithm>
#include <optional>

#include "core/error.hpp"
#include "formats/format_builtin.hpp"

namespace cb::formats::mips_elf {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "mips_elf";

std::uint16_t le16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | b[at + 1] << 8); }
std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
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

namespace {

// The file's structures through elf.fmt (the symbol tables when `symbols`).
eng::Tree read_tree(ByteView data, bool symbols = false) {
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    if (symbols) options.parts.insert("symbols");
    return eng::builtin().read("elf", data, options);
}

std::uint32_t u32_of(const eng::Node& n, std::string_view field) { return static_cast<std::uint32_t>(n.u(field)); }

// A name the tree holds, up to its terminator (empty when it has none).
std::string text_of(const eng::Node* n) {
    if (!n) return {};
    return n->text.substr(0, n->text.find('\0'));
}

}  // namespace

Elf parse(ByteView data, std::uint16_t machine) {
    if (!probe_header(data, machine)) throw FormatError(kReader, 0, machine == kMachineMips ? "not a 32-bit little-endian MIPS ELF file" : "not a 32-bit little-endian ELF file of the machine asked for");
    // The layout is elf.fmt's; that the tables lie in the file with entries of this format's size is this reader's rule.
    const eng::Tree tree = read_tree(data);
    const eng::Node& e = tree.root.get("elf");
    const eng::Node& h = e.get("header");
    Elf elf;
    elf.data.assign(data.begin(), data.end());
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
    if (elf.phnum != 0 && (elf.phentsize != kProgramHeaderSize || static_cast<std::uint64_t>(elf.phoff) + static_cast<std::uint64_t>(elf.phnum) * kProgramHeaderSize > data.size()))
        throw FormatError(kReader, elf.phoff, "the program header table is not inside the file");
    if (const eng::Node* segments = e.find("segments"))
        for (const eng::Node& p : segments->children)
            elf.program_headers.push_back({u32_of(p, "type"), u32_of(p, "offset"), u32_of(p, "vaddr"), u32_of(p, "paddr"), u32_of(p, "filesz"), u32_of(p, "memsz"),
                                           u32_of(p, "flags"), u32_of(p, "align")});
    if (elf.shnum != 0 && (elf.shentsize != kSectionHeaderSize || static_cast<std::uint64_t>(elf.shoff) + static_cast<std::uint64_t>(elf.shnum) * kSectionHeaderSize > data.size()))
        throw FormatError(kReader, elf.shoff, "the section header table is not inside the file");
    const eng::Node* names = e.find("names");
    if (const eng::Node* sections = e.find("sections"))
        for (std::size_t i = 0; i < sections->children.size(); ++i) {
            const eng::Node& n = sections->children[i];
            SectionHeader s{u32_of(n, "name_offset"), u32_of(n, "type"), u32_of(n, "flags"), u32_of(n, "address"), u32_of(n, "offset"), u32_of(n, "size"),
                            u32_of(n, "link"), u32_of(n, "info"), u32_of(n, "align"), u32_of(n, "entsize"), {}};
            if (names) s.name = text_of(names->children[i].child("text"));
            elf.sections.push_back(std::move(s));
        }
    return elf;
}

std::vector<Symbol> Elf::symbols() const {
    std::vector<Symbol> out;
    const eng::Tree tree = read_tree(data, true);
    const eng::Node* tables = tree.root.get("elf").find("symbols");
    if (!tables) return out;
    for (const eng::Node& table : tables->children) {
        const eng::Node* entries = table.child("entries");
        if (!entries) continue;
        for (const eng::Node& e : entries->children) {
            Symbol sym;
            sym.name = text_of(e.child("name"));
            sym.value = u32_of(e, "value");
            sym.size = u32_of(e, "size");
            sym.type = static_cast<std::uint8_t>(e.u("type"));
            sym.bind = static_cast<std::uint8_t>(e.u("bind"));
            sym.section = static_cast<std::uint16_t>(e.u("section"));
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
