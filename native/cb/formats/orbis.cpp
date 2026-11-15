// cb-unity: alone
#include "cb/formats/orbis.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::orbis {

namespace {

constexpr std::string_view kReader = "orbis";
constexpr std::uint64_t kMaxFile = 256ull << 20;
constexpr std::size_t kMaxSegments = 256, kMaxSymbols = 1u << 20, kMaxRelocations = 1u << 22;

constexpr std::uint16_t kEtExec = 2, kEtDyn = 3, kEtSceExec = 0xFE00, kEtSceExecAslr = 0xFE10, kEtSceDynamic = 0xFE18;

// The dynamic tags, plain and Sony's.
constexpr std::int64_t kDtNeeded = 1, kDtPltRelSz = 2, kDtHash = 4, kDtStrTab = 5, kDtSymTab = 6, kDtRela = 7, kDtRelaSz = 8, kDtStrSz = 10, kDtInit = 12,
                       kDtJmpRel = 0x17, kDtInitArray = 0x19, kDtInitArraySz = 0x1b, kDtGnuHash = 0x6ffffef5;
constexpr std::int64_t kSceImportModule = 0x6100000F, kSceImportLib = 0x61000015, kSceJmpRel = 0x61000029, kScePltRelSz = 0x6100002d, kSceRela = 0x6100002f,
                       kSceRelaSz = 0x61000031, kSceStrTab = 0x61000035, kSceStrSz = 0x61000037, kSceSymTab = 0x61000039, kSceSymTabSz = 0x6100003f;

template <class T>
T at(ByteView d, std::uint64_t off) {
    return get<T>(d, off, kReader);
}

std::string c_string(ByteView d, std::uint64_t off, std::size_t limit = 512) {
    std::string out;
    for (std::size_t k = 0; k < limit && off + k < d.size() && d[off + k] != 0; ++k) out += static_cast<char>(d[off + k]);
    return out;
}

bool plain_elf_head(ByteView h) {
    return h.size() >= 64 && h[0] == 0x7F && h[1] == 'E' && h[2] == 'L' && h[3] == 'F' && h[4] == 2 && h[5] == 1 && try_get<std::uint16_t>(h, 18).value_or(0) == kMachineX86_64;
}

bool good_type(std::uint16_t t) { return t == kEtExec || t == kEtDyn || t == kEtSceExec || t == kEtSceExecAslr || t == kEtSceDynamic; }

// The base 64 alphabet an Orbis symbol name's library number uses.
int base64_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '-') return 63;
    return -1;
}

std::vector<Segment> read_segments(ByteView d) {
    const std::uint64_t phoff = at<std::uint64_t>(d, 32);
    const std::uint16_t entsize = at<std::uint16_t>(d, 54), count = at<std::uint16_t>(d, 56);
    if (count > kMaxSegments) throw FormatError(kReader, 56, "too many program headers");
    if (count != 0 && entsize < 56) throw FormatError(kReader, 54, "a program header entry is smaller than an Elf64_Phdr");
    std::vector<Segment> out;
    for (std::uint16_t k = 0; k < count; ++k) {
        const std::uint64_t o = phoff + static_cast<std::uint64_t>(k) * entsize;
        Segment s;
        s.type = at<std::uint32_t>(d, o);
        s.flags = at<std::uint32_t>(d, o + 4);
        s.offset = at<std::uint64_t>(d, o + 8);
        s.address = at<std::uint64_t>(d, o + 16);
        s.file_size = at<std::uint64_t>(d, o + 32);
        s.memory_size = at<std::uint64_t>(d, o + 40);
        s.align = at<std::uint64_t>(d, o + 48);
        out.push_back(s);
    }
    return out;
}

Symbol read_symbol(ByteView d, std::uint64_t table, std::uint64_t index, std::uint64_t strings, std::uint64_t string_end) {
    const std::uint64_t o = table + index * 24;
    Symbol s;
    const std::uint32_t name = at<std::uint32_t>(d, o);
    s.info = at<std::uint8_t>(d, o + 4);
    s.section = at<std::uint16_t>(d, o + 6);
    s.value = at<std::uint64_t>(d, o + 8);
    s.size = at<std::uint64_t>(d, o + 16);
    if (strings + name < string_end) s.name = c_string(d, strings + name);
    return s;
}

void add_function(Program& p, std::set<std::uint64_t>& seen, const Symbol& s) {
    if (!s.defined() || !s.function() || s.value == 0 || s.name.empty() || !p.in_code(s.value) || !seen.insert(s.value).second) return;
    p.functions.push_back(s);
}

// The static symbol table of a plain ELF, when its section headers are in the file.
void read_static_functions(Program& p, std::set<std::uint64_t>& seen) {
    const ByteView d = p.file;
    const auto shoff = try_get<std::uint64_t>(d, 40);
    const auto entsize = try_get<std::uint16_t>(d, 58), count = try_get<std::uint16_t>(d, 60);
    if (!shoff || !entsize || !count || *shoff == 0 || *entsize < 64 || *count == 0 || *count > 4096) return;
    for (std::uint16_t k = 0; k < *count; ++k) {
        const std::uint64_t o = *shoff + static_cast<std::uint64_t>(k) * *entsize;
        const auto type = try_get<std::uint32_t>(d, o + 4), link = try_get<std::uint32_t>(d, o + 40);
        const auto off = try_get<std::uint64_t>(d, o + 24), size = try_get<std::uint64_t>(d, o + 32);
        if (!type || *type != 2 || !off || !size || !link || *link >= *count) continue;   // SHT_SYMTAB
        const std::uint64_t lo = *shoff + static_cast<std::uint64_t>(*link) * *entsize;
        const auto strings = try_get<std::uint64_t>(d, lo + 24), string_size = try_get<std::uint64_t>(d, lo + 32);
        if (!strings || !string_size || *off > d.size() || *size > d.size() - *off || *strings > d.size() || *string_size > d.size() - *strings) continue;
        for (std::uint64_t i = 1; i < *size / 24 && i < kMaxSymbols; ++i) add_function(p, seen, read_symbol(d, *off, i, *strings, *strings + *string_size));
    }
}

void read_dynamic(Program& p) {
    const ByteView d = p.file;
    const Segment* dyn = nullptr;
    const Segment* dynlib = nullptr;
    for (const Segment& s : p.segments) {
        if (s.type == kPtDynamic && !dyn) dyn = &s;
        if (s.type == kPtSceDynlibData && !dynlib) dynlib = &s;
    }
    if (!dyn || dyn->offset >= d.size()) return;
    p.orbis = dynlib != nullptr;
    std::map<std::int64_t, std::uint64_t> tag;
    std::vector<std::uint64_t> needed_values;
    std::vector<std::uint64_t> libs;
    for (std::uint64_t o = dyn->offset; o + 16 <= d.size() && o < dyn->offset + dyn->file_size; o += 16) {
        const std::int64_t t = at<std::int64_t>(d, o);
        const std::uint64_t v = at<std::uint64_t>(d, o + 8);
        if (t == 0) break;
        if (t == kDtNeeded) needed_values.push_back(v);
        else if (t == kSceImportLib) libs.push_back(v);
        else if (t == kSceImportModule) needed_values.push_back(v);
        else tag[t] = v;
    }
    const auto get_tag = [&](std::int64_t t) -> std::optional<std::uint64_t> {
        const auto it = tag.find(t);
        return it == tag.end() ? std::nullopt : std::optional<std::uint64_t>(it->second);
    };
    // Where a table is: a virtual address (plain), or an offset inside the dynamic-data segment (Orbis).
    const auto place = [&](std::uint64_t v) -> std::optional<std::uint64_t> {
        if (p.orbis) return dynlib->offset + v;
        return p.offset_of(v);
    };
    std::uint64_t strings = 0, string_end = 0, symbols = 0, symbol_count = 0, rela = 0, rela_count = 0, jmprel = 0, jmprel_count = 0;
    if (const auto s = get_tag(p.orbis ? kSceStrTab : kDtStrTab); s && place(*s)) {
        strings = *place(*s);
        const std::uint64_t size = get_tag(p.orbis ? kSceStrSz : kDtStrSz).value_or(0);
        string_end = std::min<std::uint64_t>(d.size(), strings + (size != 0 ? size : d.size()));
    }
    if (const auto s = get_tag(p.orbis ? kSceSymTab : kDtSymTab); s && place(*s)) {
        symbols = *place(*s);
        if (p.orbis) {
            symbol_count = get_tag(kSceSymTabSz).value_or(0) / 24;
        } else {
            // the table runs to the next table the dynamic section names
            std::uint64_t next = UINT64_MAX;
            for (const std::int64_t t : {kDtStrTab, kDtHash, kDtGnuHash, kDtRela, kDtJmpRel}) {
                if (const auto a = get_tag(t); a && *a > *s) next = std::min(next, *a);
            }
            if (next != UINT64_MAX) symbol_count = (next - *s) / 24;
        }
        symbol_count = std::min<std::uint64_t>(symbol_count, kMaxSymbols);
    }
    if (const auto s = get_tag(p.orbis ? kSceRela : kDtRela); s && place(*s)) {
        rela = *place(*s);
        rela_count = std::min<std::uint64_t>(get_tag(p.orbis ? kSceRelaSz : kDtRelaSz).value_or(0) / 24, kMaxRelocations);
    }
    if (const auto s = get_tag(p.orbis ? kSceJmpRel : kDtJmpRel); s && place(*s)) {
        jmprel = *place(*s);
        jmprel_count = std::min<std::uint64_t>(get_tag(p.orbis ? kScePltRelSz : kDtPltRelSz).value_or(0) / 24, kMaxRelocations);
    }

    for (const std::uint64_t v : needed_values) {
        if (strings == 0) break;
        const std::string name = c_string(d, strings + (p.orbis ? (v & 0xFFFFFFFFull) : v));
        if (!name.empty()) p.needed.push_back(name);
    }
    std::map<std::uint32_t, std::string> library_by_id;   // an Orbis library number -> its name
    for (const std::uint64_t v : libs) {
        if (strings != 0) library_by_id[static_cast<std::uint32_t>(v >> 48)] = c_string(d, strings + (v & 0xFFFFFFFFull));
    }

    if (symbols != 0 && strings != 0) {
        for (std::uint64_t i = 0; i < symbol_count; ++i) {
            if (symbols + (i + 1) * 24 > d.size()) break;
            p.symbols.push_back(read_symbol(d, symbols, i, strings, string_end));
        }
    }
    const auto relocations = [&](std::uint64_t table, std::uint64_t count) {
        for (std::uint64_t i = 0; i < count; ++i) {
            const std::uint64_t o = table + i * 24;
            if (o + 24 > d.size()) break;
            Relocation r;
            r.offset = at<std::uint64_t>(d, o);
            const std::uint64_t info = at<std::uint64_t>(d, o + 8);
            r.type = static_cast<std::uint32_t>(info);
            r.symbol = static_cast<std::uint32_t>(info >> 32);
            r.addend = at<std::int64_t>(d, o + 16);
            p.relocations.push_back(r);
        }
    };
    if (rela != 0) relocations(rela, rela_count);
    if (jmprel != 0) relocations(jmprel, jmprel_count);

    std::set<std::uint64_t> slots;
    for (const Relocation& r : p.relocations) {
        if ((r.type != 1 && r.type != 6 && r.type != 7) || r.symbol == 0 || r.symbol >= p.symbols.size() || p.symbols[r.symbol].defined()) continue;
        if (!slots.insert(r.offset).second) continue;
        const Symbol& s = p.symbols[r.symbol];
        Import im;
        im.slot = r.offset;
        if (p.orbis) {
            // <NID>#<library>#<module>: the library number is a base 64 digit
            const std::size_t first = s.name.find('#');
            im.nid = s.name.substr(0, first);
            im.name = function_name(im.nid);
            if (first != std::string::npos && first + 1 < s.name.size()) {
                const int id = base64_value(s.name[first + 1]);
                if (const auto it = library_by_id.find(static_cast<std::uint32_t>(id < 0 ? 0 : id)); it != library_by_id.end()) im.library = it->second;
            }
        } else {
            im.name = s.name;
            im.nid = nid_of(s.name);
        }
        p.imports.push_back(std::move(im));
    }
    std::ranges::sort(p.imports, {}, &Import::slot);

    // the initialisers: DT_INIT, and the slots of DT_INIT_ARRAY (their value is the relocation's addend, or what the file holds)
    if (const auto init = get_tag(kDtInit); init && *init != 0 && !p.orbis) p.initializers.push_back(*init);
    const auto array = get_tag(kDtInitArray), array_size = get_tag(kDtInitArraySz);
    if (array && array_size && !p.orbis) {
        for (std::uint64_t k = 0; k < *array_size / 8 && k < 4096; ++k) {
            const std::uint64_t slot = *array + k * 8;
            std::optional<std::uint64_t> value;
            for (const Relocation& r : p.relocations) {
                if (r.offset == slot && r.type == 8) value = static_cast<std::uint64_t>(r.addend);
            }
            if (!value) {
                if (const auto o = p.offset_of(slot)) value = try_get<std::uint64_t>(d, *o);
            }
            if (value && *value != 0 && p.in_code(*value)) p.initializers.push_back(*value);
        }
    }

    std::set<std::uint64_t> seen;
    for (const Symbol& s : p.symbols) add_function(p, seen, s);
    if (!p.orbis) read_static_functions(p, seen);
    std::ranges::sort(p.functions, {}, &Symbol::value);
}

Program build(Bytes file, bool self) {
    Program p;
    p.file = std::move(file);
    const ByteView d = p.file;
    if (!plain_elf_head(d)) throw FormatError(kReader, 0, "not an x86-64 ELF64 file");
    p.type = at<std::uint16_t>(d, 16);
    if (!good_type(p.type)) throw FormatError(kReader, 16, std::format("ELF type 0x{:X} is not an executable or a shared object", p.type));
    p.entry = at<std::uint64_t>(d, 24);
    p.segments = read_segments(d);
    p.self = self;
    read_dynamic(p);
    p.container = self ? "fake SELF" : p.orbis ? "OELF" : "ELF";
    return p;
}

// ---- the SELF ---------------------------------------------------------------------------------------------------

// The ELF a fake SELF holds, rebuilt: its header and program headers as the SELF copies them, each segment at its file offset.
Program from_self(ByteView d) {
    const std::uint16_t entries = at<std::uint16_t>(d, 0x18);
    if (entries == 0 || entries > 1024) throw FormatError(kReader, 0x18, "a SELF with no segment entries (or too many)");
    const std::uint64_t elf_at = 0x20 + static_cast<std::uint64_t>(entries) * 0x20;
    if (elf_at + 0x40 > d.size()) throw TruncatedError(kReader, elf_at, 0x40, d.size() > elf_at ? d.size() - elf_at : 0);
    const ByteView head = d.subspan(elf_at);
    if (!plain_elf_head(head)) throw FormatError(kReader, elf_at, "the SELF does not hold an x86-64 ELF header");
    const std::uint16_t phnum = at<std::uint16_t>(head, 56);
    if (phnum > kMaxSegments || at<std::uint64_t>(head, 32) != 0x40 || at<std::uint16_t>(head, 54) != 56)
        throw FormatError(kReader, elf_at, "the ELF header inside the SELF is not the layout CodeBreaker reads");
    const std::uint64_t ext_at = (elf_at + 0x40 + static_cast<std::uint64_t>(phnum) * 56 + 15) & ~15ull;
    const auto paid = try_get<std::uint64_t>(d, ext_at), program_type = try_get<std::uint64_t>(d, ext_at + 8);
    if (!program_type || *program_type != 1)
        throw FormatError(kReader, ext_at, std::format("the SELF's program type is {} (1 is fake): a retail or system SELF is signed and encrypted, which CodeBreaker does not decrypt", program_type.value_or(0)));

    Bytes elf(0x40 + static_cast<std::size_t>(phnum) * 56);
    if (head.size() < elf.size()) throw TruncatedError(kReader, elf_at, elf.size(), head.size());
    std::copy_n(head.begin(), elf.size(), elf.begin());
    const std::vector<Segment> segments = read_segments(ByteView(elf));
    std::uint64_t size = elf.size();
    struct Part {
        std::uint64_t at, offset, bytes;
    };
    std::vector<Part> parts;
    for (std::uint16_t k = 0; k < entries; ++k) {
        const std::uint64_t o = 0x20 + static_cast<std::uint64_t>(k) * 0x20;
        const std::uint64_t props = at<std::uint64_t>(d, o), offset = at<std::uint64_t>(d, o + 8), bytes = at<std::uint64_t>(d, o + 16);
        if ((props & (1u << 11)) == 0 || (props & (1u << 16)) != 0) continue;   // a data entry has blocks and no digests; the others are meta
        if ((props & 2) != 0 || (props & 8) != 0) throw FormatError(kReader, o, "a SELF segment is encrypted or compressed: CodeBreaker does not decrypt");
        const std::uint64_t index = (props >> 20) & 0xFFF;
        if (index >= segments.size()) throw FormatError(kReader, o, "a SELF entry names a program header that is not there");
        if (offset > d.size() || bytes > d.size() - offset) throw TruncatedError(kReader, offset, bytes, d.size() > offset ? d.size() - offset : 0);
        const Segment& s = segments[index];
        if (bytes > s.file_size || s.offset > kMaxFile || bytes > kMaxFile) throw FormatError(kReader, o, "a SELF segment is larger than its program header says");
        parts.push_back({offset, s.offset, bytes});
        size = std::max(size, s.offset + bytes);
    }
    if (size > kMaxFile) throw FormatError(kReader, 0, "the ELF inside the SELF is larger than CodeBreaker reads");
    elf.resize(size, 0);
    for (const Part& part : parts) std::copy_n(d.begin() + static_cast<std::ptrdiff_t>(part.at), part.bytes, elf.begin() + static_cast<std::ptrdiff_t>(part.offset));
    Program p = build(std::move(elf), true);
    p.program_type = static_cast<std::uint32_t>(*program_type);
    p.paid = paid.value_or(0);
    return p;
}

}  // namespace

std::optional<std::uint64_t> Program::offset_of(std::uint64_t address) const {
    for (const Segment& s : segments) {
        if (s.loaded() && address >= s.address && address - s.address < s.file_size && s.offset + (address - s.address) < file.size()) return s.offset + (address - s.address);
    }
    return std::nullopt;
}

std::optional<std::uint64_t> Program::address_of(std::uint64_t offset) const {
    for (const Segment& s : segments) {
        if (s.loaded() && offset >= s.offset && offset - s.offset < s.file_size) return s.address + (offset - s.offset);
    }
    return std::nullopt;
}

bool Program::in_code(std::uint64_t address) const {
    return std::ranges::any_of(segments, [&](const Segment& s) { return s.code() && address >= s.address && address - s.address < s.file_size; });
}

std::string Program::import_at(std::uint64_t address) const {
    const auto it = std::ranges::lower_bound(imports, address, {}, &Import::slot);
    if (it == imports.end() || it->slot != address) return {};
    return it->name.empty() ? it->nid : it->name;
}

std::uint64_t Program::image_end() const {
    std::uint64_t end = 0;
    for (const Segment& s : segments) {
        if (s.loaded()) end = std::max(end, s.address + s.memory_size);
    }
    return end;
}

Kind kind_of(ByteView head) {
    if (plain_elf_head(head)) return Kind::Elf;
    if (head.size() >= 4 && head[0] == 0x4F && head[1] == 0x15 && head[2] == 0x3D && head[3] == 0x1D) return Kind::Fself;
    if (head.size() >= 4 && head[0] == 0x54 && head[1] == 0x14 && head[2] == 0xF5 && head[3] == 0xEE) return Kind::Ps5Self;
    if (head.size() >= 4 && head[0] == 0x7F && head[1] == 'C' && head[2] == 'N' && head[3] == 'T') return Kind::Pkg;
    return Kind::None;
}

Program parse(ByteView data) {
    switch (kind_of(data.first(std::min<std::size_t>(data.size(), 64)))) {
        case Kind::Elf: return build(Bytes(data.begin(), data.end()), false);
        case Kind::Fself: return from_self(data);
        case Kind::Ps5Self: throw FormatError(kReader, 0, "a PlayStation 5 SELF (54 14 F5 EE): recognised, not handled -- CodeBreaker does not decrypt");
        case Kind::Pkg: throw FormatError(kReader, 0, "a PlayStation 4 package (.pkg): recognised, not handled -- its files are inside an encrypted or packed image");
        default: throw FormatError(kReader, 0, "not an x86-64 ELF, a PlayStation 4 SELF or a PlayStation package");
    }
}

Program read(const std::filesystem::path& path) {
    const auto size = size_of(path);
    if (!size) throw NotFoundError(path, 2);
    if (*size > kMaxFile) throw FormatError(kReader, 0, "the file is larger than CodeBreaker reads as a program");
    return parse(fs::read_file(path));
}

Console console_of(const std::filesystem::path& file, const Program& program) {
    if (program.self || program.orbis) return Console::Ps4;
    std::string name = file.filename().string();
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (name.find("ps5") != std::string::npos || name.find("prospero") != std::string::npos) return Console::Ps5;
    return Console::Ps4;
}

bool is_playstation(const Program& program) {
    // An x86-64 ELF is a PlayStation one when it is an Orbis ELF or names the system's libraries (a Linux program names libc.so.6 and a loader).
    return program.orbis || std::ranges::any_of(program.needed, [](const std::string& n) { return n.find("libkernel") != std::string::npos || n.find("libSce") != std::string::npos || n.ends_with(".sprx"); });
}

bool probe(const std::filesystem::path& path, Console console) {
    try {
        const auto head = read_prefix(path, 0, 64);
        if (!head) return false;
        const Kind kind = kind_of(*head);
        switch (kind) {
            case Kind::None: return false;
            case Kind::Pkg: case Kind::Fself: return console == Console::Ps4;   // (an encrypted SELF or a package is the lane's to refuse, with the reason)
            case Kind::Ps5Self: return console == Console::Ps5;
            case Kind::RetailSelf: return console == Console::Ps4;
            case Kind::Elf: break;
        }
        const Program p = read(path);
        return is_playstation(p) && console_of(path, p) == console;
    } catch (const Error&) {
        return false;
    }
}

}  // namespace cb::formats::orbis
