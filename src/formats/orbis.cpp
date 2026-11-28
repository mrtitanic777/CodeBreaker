// cb-unity: alone
#include "formats/orbis.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"
#include "formats/read.hpp"

namespace cb::formats::orbis {

namespace eng = cb::formats::engine;

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

// A read of `data` through orbis.fmt from the struct `root`.
eng::Tree orbis_tree(ByteView data, std::string_view root, std::map<std::string, std::int64_t> vars = {}, bool sections = false) {
    eng::ReadOptions options;
    options.root = std::string(root);
    options.vars = std::move(vars);
    if (sections) options.parts.insert("sections");
    options.keep_source = false;   // only read from
    return eng::builtin().read("orbis", data, options);
}

// A field the description reads with `try`: its value when it was in the file.
std::optional<std::uint64_t> orbis_field(const eng::Node& n, std::string_view field) {
    const eng::Node* f = n.child(field);
    if (!f || f->kind != eng::Kind::Int) return std::nullopt;
    return f->value;
}

// The program headers of an Elf64 tree (`d` is what it was read from: an entry not wholly in it is the file cut short).
std::vector<Segment> read_segments(const eng::Node& elf, ByteView d) {
    const eng::Node& header = elf.get("header");
    const std::uint64_t phoff = header.u("phoff"), entsize = header.u("phentsize"), count = header.u("phnum");
    if (count > kMaxSegments) throw FormatError(kReader, 56, "too many program headers");
    if (count != 0 && entsize < 56) throw FormatError(kReader, 54, "a program header entry is smaller than an Elf64_Phdr");
    std::vector<Segment> out;
    const std::vector<eng::Node>& table = elf.get("segments").children;
    for (std::uint64_t k = 0; k < count; ++k) {
        const eng::Node* e = table[k].child("entry");
        const std::uint64_t o = phoff + k * entsize;
        if (!e) throw TruncatedError(kReader, o, 56, o < d.size() ? d.size() - o : 0);
        Segment s;
        s.type = static_cast<std::uint32_t>(e->u("type"));
        s.flags = static_cast<std::uint32_t>(e->u("flags"));
        s.offset = e->u("offset");
        s.address = e->u("vaddr");
        s.file_size = e->u("filesz");
        s.memory_size = e->u("memsz");
        s.align = e->u("align");
        out.push_back(s);
    }
    return out;
}

// A symbol table entry; its name is the string at strings + its name offset, when that is before string_end.
Symbol make_symbol(ByteView d, const eng::Node& e, std::uint64_t strings, std::uint64_t string_end) {
    Symbol s;
    const std::uint64_t name = e.u("name_offset");
    s.info = static_cast<std::uint8_t>(e.u("info"));
    s.section = static_cast<std::uint16_t>(e.u("section"));
    s.value = e.u("value");
    s.size = e.u("size");
    if (strings + name < string_end) s.name = c_string(d, strings + name);
    return s;
}

void add_function(Program& p, std::set<std::uint64_t>& seen, const Symbol& s) {
    if (!s.defined() || !s.function() || s.value == 0 || s.name.empty() || !p.in_code(s.value) || !seen.insert(s.value).second) return;
    p.functions.push_back(s);
}

// The static symbol tables of a plain ELF, when its section headers are in the file.
void read_static_functions(Program& p, std::set<std::uint64_t>& seen) {
    const ByteView d = p.file;
    const eng::Tree tree = orbis_tree(d, "Elf64", {}, true);
    const eng::Node& header = tree.root.get("header");
    const std::uint64_t count = header.u("shnum");
    if (header.u("shoff") == 0 || header.u("shentsize") < 64 || count == 0 || count > 4096) return;
    const std::vector<eng::Node>& sections = tree.root.get("sections").children;
    for (std::uint64_t k = 0; k < count; ++k) {
        const eng::Node& sh = sections[k].get("entry");
        const auto type = orbis_field(sh, "type"), link = orbis_field(sh, "link");
        const auto off = orbis_field(sh, "offset"), size = orbis_field(sh, "size");
        if (!type || *type != 2 || !off || !size || !link || *link >= count) continue;   // SHT_SYMTAB
        const eng::Node& strings_sh = sections[*link].get("entry");
        const auto strings = orbis_field(strings_sh, "offset"), string_size = orbis_field(strings_sh, "size");
        if (!strings || !string_size || *off > d.size() || *size > d.size() - *off || *strings > d.size() || *string_size > d.size() - *strings) continue;
        const std::uint64_t entries = std::min<std::uint64_t>(*size / 24, kMaxSymbols);
        if (entries < 2) continue;
        const eng::Tree table = orbis_tree(d, "Statics", {{"at", static_cast<std::int64_t>(*off)}, {"count", static_cast<std::int64_t>(entries)}});
        const std::vector<eng::Node>& symbols = table.root.get("symbols").children;
        for (std::size_t i = 1; i < symbols.size(); ++i) add_function(p, seen, make_symbol(d, symbols[i], *strings, *strings + *string_size));
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
    // the entries start before the segment's end (an end past the file is the file's)
    const std::uint64_t dyn_end = std::min<std::uint64_t>(dyn->offset + dyn->file_size, d.size());
    const eng::Tree dynamic = orbis_tree(d, "Dynamic", {{"dyn_at", static_cast<std::int64_t>(dyn->offset)}, {"dyn_end", static_cast<std::int64_t>(dyn_end)}});
    for (const eng::Node& e : dynamic.root.get("entries").children) {
        const std::int64_t t = e.i("tag");
        const std::uint64_t v = e.u("value");
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

    // the tables, where the tags place them (a place past the file, or one a signed offset cannot say, holds nothing)
    const auto var = [&](std::uint64_t at) { return at < d.size() ? static_cast<std::int64_t>(at) : std::int64_t{-1}; };
    const std::uint64_t symbols_at = symbols != 0 && strings != 0 ? symbols : 0;
    const eng::Tree tables = orbis_tree(d, "Tables", {{"sym_at", symbols_at != 0 ? var(symbols_at) : 0}, {"sym_count", static_cast<std::int64_t>(symbol_count)},
                                                      {"rela_at", rela != 0 ? var(rela) : 0}, {"rela_count", static_cast<std::int64_t>(rela_count)},
                                                      {"jmprel_at", jmprel != 0 ? var(jmprel) : 0}, {"jmprel_count", static_cast<std::int64_t>(jmprel_count)}});
    if (const eng::Node* table = tables.root.child("symbols"))
        for (const eng::Node& e : table->children) p.symbols.push_back(make_symbol(d, e, strings, string_end));
    for (const std::string_view name : {"rela", "jmprel"})
        if (const eng::Node* table = tables.root.child(name))
            for (const eng::Node& e : table->children) {
                Relocation r;
                r.offset = e.u("offset");
                const std::uint64_t info = e.u("info");
                r.type = static_cast<std::uint32_t>(info);
                r.symbol = static_cast<std::uint32_t>(info >> 32);
                r.addend = e.i("addend");
                p.relocations.push_back(r);
            }

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

// The layout is orbis.fmt's; what the types, the counts and the places mean is this reader's.
Program build(Bytes file, bool self) {
    Program p;
    p.file = std::move(file);
    const ByteView d = p.file;
    if (!plain_elf_head(d)) throw FormatError(kReader, 0, "not an x86-64 ELF64 file");
    const eng::Tree tree = orbis_tree(d, "Elf64");
    const eng::Node& header = tree.root.get("header");
    p.type = static_cast<std::uint16_t>(header.u("type"));
    if (!good_type(p.type)) throw FormatError(kReader, 16, std::format("ELF type 0x{:X} is not an executable or a shared object", p.type));
    p.entry = header.u("entry");
    p.segments = read_segments(tree.root, d);
    p.self = self;
    read_dynamic(p);
    p.container = self ? "fake SELF" : p.orbis ? "OELF" : "ELF";
    return p;
}

// ---- the SELF ---------------------------------------------------------------------------------------------------

// The ELF a fake SELF holds, rebuilt: its header and program headers as the SELF copies them, each segment at its file offset.
Program from_self(ByteView d) {
    const eng::Tree tree = orbis_tree(d, "Self");
    const eng::Node& self = tree.root;
    const auto count = orbis_field(self, "entry_count");
    if (!count) throw TruncatedError(kReader, 0x18, 2, d.size() > 0x18 ? d.size() - 0x18 : 0);
    const std::uint64_t entries = *count;
    if (entries == 0 || entries > 1024) throw FormatError(kReader, 0x18, "a SELF with no segment entries (or too many)");
    const std::uint64_t elf_at = 0x20 + entries * 0x20;
    if (elf_at + 0x40 > d.size()) throw TruncatedError(kReader, elf_at, 0x40, d.size() > elf_at ? d.size() - elf_at : 0);
    const ByteView head = d.subspan(elf_at);
    if (!plain_elf_head(head)) throw FormatError(kReader, elf_at, "the SELF does not hold an x86-64 ELF header");
    const eng::Node& header = self.get("header");
    const std::uint64_t phnum = header.u("phnum");
    if (phnum > kMaxSegments || header.u("phoff") != 0x40 || header.u("phentsize") != 56)
        throw FormatError(kReader, elf_at, "the ELF header inside the SELF is not the layout CodeBreaker reads");
    const std::uint64_t ext_at = (elf_at + 0x40 + phnum * 56 + 15) & ~15ull;
    const auto paid = orbis_field(self, "paid"), program_type = orbis_field(self, "program_type");
    if (!program_type || *program_type != 1)
        throw FormatError(kReader, ext_at, std::format("the SELF's program type is {} (1 is fake): a retail or system SELF is signed and encrypted, which CodeBreaker does not decrypt", program_type.value_or(0)));

    Bytes elf(0x40 + static_cast<std::size_t>(phnum) * 56);
    if (head.size() < elf.size()) throw TruncatedError(kReader, elf_at, elf.size(), head.size());
    std::copy_n(head.begin(), elf.size(), elf.begin());
    const std::vector<Segment> segments = read_segments(orbis_tree(ByteView(elf), "Elf64").root, ByteView(elf));
    std::uint64_t size = elf.size();
    struct Part {
        std::uint64_t at, offset, bytes;
    };
    std::vector<Part> parts;
    const std::vector<eng::Node>& table = self.get("entries").children;
    for (std::uint64_t k = 0; k < entries; ++k) {
        const std::uint64_t o = 0x20 + k * 0x20;
        if (k >= table.size()) throw TruncatedError(kReader, o, 24, o < d.size() ? d.size() - o : 0);
        const eng::Node& e = table[k];
        const std::uint64_t props = e.u("properties"), offset = e.u("offset"), bytes = e.u("size");
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
