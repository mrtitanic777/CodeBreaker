#include "cb/source/pe_writer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <iterator>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/pe_functions.hpp"
#include "cb/source/assembly.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::pe_writer {
namespace {

namespace l2 = translate::l2;
namespace xf = x86_flat;

constexpr std::string_view kReader = "pe_writer";
constexpr std::size_t kLongestInstruction = 15;
constexpr std::size_t kImportDirectory = 1;       // the import directory's index among the data directories
constexpr std::uint32_t kImportByOrdinal = 0x80000000;   // a lookup-table entry with this bit holds an ordinal, not an RVA
constexpr std::size_t kImportDescriptor = 20;            // bytes in an import descriptor
constexpr std::size_t kEntryPointField = 16;             // where AddressOfEntryPoint lies in the optional header
constexpr std::size_t kOptionalHeaderAt = 24;            // where the optional header lies after the new header's offset

// A flow ends at one of these.
bool ends_flow(std::string_view m) {
    return m == "ret" || m == "retf" || m == "jmp" || m == "hlt" || m == "iretd" || m == "iret" || m == "ud2" || m.starts_with("int3");
}

// The macros that write a register-to-register instruction in the direction NASM does not: width 1 is a 32-bit register, 2 a 16-bit one (it
// takes the operand-size prefix in 32-bit code), 0 a byte register.
const std::vector<std::string>& prelude() {
    static const std::vector<std::string> lines = [] {
        std::vector<std::string> p = {"", "; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one",
                                      "; Microsoft's compiler writes; NASM itself writes the other"};
        struct Group {
            std::array<std::string_view, 8> names;
            int width;
        };
        const std::array<Group, 3> groups = {{{{"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"}, 1},
                                              {{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}, 2},
                                              {{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}, 0}}};
        for (const Group& g : groups) {
            for (std::size_t k = 0; k < g.names.size(); ++k) p.push_back(std::format("%assign _r_{} {}", g.names[k], k));
            for (std::size_t k = 0; k < g.names.size(); ++k) p.push_back(std::format("%assign _w_{} {}", g.names[k], g.width));
        }
        for (const char* line : {"%macro _rr 3", "  %if _w_%2 == 2", "    db 0x66", "  %endif", "  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3",
                                 "%endmacro"}) {
            p.emplace_back(line);
        }
        for (const xf::RegisterPairOp& op : xf::register_pair_ops()) p.push_back(std::format("%define {}_ _rr 0x{:02x},", op.mnemonic, op.opcode));
        return p;
    }();
    return lines;
}

std::string indented(const std::string& line) { return "    " + line; }

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// `text` with the first number `v` (as `0x..`) that stands alone replaced by `with`: not after a word character, a `+` (or, `after_bracket`, a
// `[`), and followed by a non-word character (or, `at_end`, by the end). Nothing when there is none, or when the replacement spells it as it was.
std::optional<std::string> replace_number(std::string_view text, std::uint32_t v, bool after_bracket, bool at_end, std::string_view with) {
    const std::string number = std::format("0x{:x}", v);
    for (std::size_t at = text.find(number); at != std::string_view::npos; at = text.find(number, at + 1)) {
        if (at > 0) {
            const char before = text[at - 1];
            if (is_word(before) || before == '+' || (after_bracket && before == '[')) continue;
        }
        const std::size_t end = at + number.size();
        if (at_end ? end != text.size() : (end < text.size() && is_word(text[end]))) continue;
        std::string out(text.substr(0, at));
        out += with;
        out += text.substr(end);
        if (out == text) return std::nullopt;
        return out;
    }
    return std::nullopt;
}

// The text inside the first pair of brackets of an operand, with where the pair lies.
struct Brackets {
    std::size_t open = 0, close = 0;
};
std::optional<Brackets> find_brackets(std::string_view ops) {
    const std::size_t open = ops.find('[');
    if (open == std::string_view::npos) return std::nullopt;
    const std::size_t close = ops.find(']', open + 1);
    if (close == std::string_view::npos) return std::nullopt;
    return Brackets{open, close};
}

// `line`, then spaces to column 40, then `; mnemonic operands` (the operands may be none).
std::string annotated(std::string line, const xf::Insn& insn) {
    const std::size_t pad = line.size() < 40 ? 40 - line.size() : 1;
    return line + std::string(pad, ' ') + "; " + std::string(text::trim(insn.mnemonic + " " + insn.op_str));
}

// The name of a section in the source: its own where NASM can take it; apart from NASM's own names and from the stored sections'.
std::string section_name_in_source(std::string_view name, std::size_t index, bool stored) {
    std::string n(name);
    for (char& c : n) {
        if (!is_word(c) && c != '.') c = '_';
    }
    if (n.empty()) n = std::format("sec{}", index);
    // (NASM's own .bss is never stored in the file; a stored section of that name is named apart, as are the two parts that are not sections)
    if (n == "hdr" || n == "tail" || n == ".bss") n = std::string(text::trim(n, ".")) + "_";
    // (and NASM's .text, .data are always stored)
    if (!stored) n = std::string(text::trim(n, ".")) + "_nobits";
    return n;
}

// The label an import address table slot goes by: `__imp_<name>`, a name made of the module and the ordinal where it has none.
std::string import_label(std::string_view module, const formats::pe::ImportSymbol& symbol) {
    std::string name;
    if (symbol.name && !symbol.name->empty()) {
        name = *symbol.name;
    } else {
        name = std::format("{}_{}", text::sanitize_identifier(module), symbol.ordinal.value_or(0));
    }
    for (char& c : name) {
        if (!is_word(c) && c != '@' && c != '?' && c != '$') c = '_';
    }
    return "__imp_" + name;
}

std::uint32_t u32_at(ByteView data, std::size_t at) {
    io::ByteReader reader(data, kReader);
    reader.seek(at);
    return reader.u32();
}

}  // namespace

std::string_view usage() {
    return R"(pe_writer -- a 32-bit Windows program as assembly source that builds back into it, byte for byte.

    pe-source <program> [--out DIR] [--stub SOURCE.ASM] [--keep-stub]

The whole file is one NASM source assembled flat (-f bin): one section for the headers and one
for every section of the image, each placed at its file offset (start=) and at its address
(vstart=), so a label's value is the address the program sees. Nothing the loader patches is a
number: every place the base-relocation table names is written as the label it points at, the
import table's addresses are written as `label - IMAGE`, and the entry point as the label of the
entry. Calls and jumps go to labels.

Code is what is reached from the entry point, from the starts of functions, and from every
relocated pointer into code (function pointers, the tables of a switch), each path stopping at
an instruction that would cut through a relocated place; what is not reached is data.

An instruction is written as its mnemonic only where the assembler turns that text back into the
original's bytes: every round assembles the whole file, reads where each instruction went (the
assembler's map of labels) and compares its bytes; one that differs, or that the assembler
rejects, is written as its bytes (with its mnemonic beside it) in the next round, until nothing
differs. The DOS program in front of the headers (a stub) is either kept as the original's bytes
or, given --stub, assembled from its own source (dos-source) and included.
)";
}

// ---- reading the program ---------------------------------------------------------------------------------------------------------

Image::Image(formats::pe::Image pe, std::string file_name) : pe_(std::move(pe)), file_name_(std::move(file_name)) {
    base_ = pe_.header().image_base;
    headers_ = pe_.optional_header().headers_size;
    const std::span<const formats::pe::Section> table = pe_.sections();
    if (table.empty()) throw FormatError(kReader, 0, "the program has no sections");
    std::size_t stored_end = 0;
    for (std::size_t i = 0; i < table.size(); ++i) {
        const formats::pe::Section& s = table[i];
        Section g;
        g.name = s.name;
        g.virtual_size = s.virtual_size;
        g.virtual_address = s.virtual_address;
        g.raw_size = s.raw_size;
        g.raw_ptr = s.raw_ptr;
        g.characteristics = s.characteristics;
        g.va = base_ + s.virtual_address;
        const Va next = i + 1 < table.size() ? base_ + table[i + 1].virtual_address : g.va + std::max(s.virtual_size, s.raw_size);
        g.extent = next >= g.va ? next - g.va : 0;   // (sections out of order: the later one is not reached by an address)
        g.stored = s.raw_ptr != 0;
        g.code = s.is_code();
        g.nasm_name = section_name_in_source(s.name, i, g.stored);
        sections_.push_back(std::move(g));
        stored_end = std::max<std::size_t>(stored_end, static_cast<std::size_t>(s.raw_ptr) + s.raw_size);
    }
    tail_ = data().size() > stored_end ? data().size() - stored_end : 0;

    for (const std::uint64_t rva : pe_.base_relocs()) {
        if (const auto at = pe_.file_offset(rva)) sites_[base_ + rva] = u32_at(data(), static_cast<std::size_t>(*at));
    }
    for (const formats::pe::ImportModule& module : pe_.import_table(false)) {
        for (const formats::pe::ImportSymbol& symbol : module.symbols) imports_[base_ + symbol.slot_rva] = import_label(module.module, symbol);
    }
    rva_fields_ = import_rva_fields();
    decode_code();
}

Image Image::open(const std::filesystem::path& program) { return Image(formats::pe::Image::open(program), fs::to_text(program.filename())); }

ByteView Image::bytes(std::size_t lo, std::size_t hi) const {
    // (the file may end inside what a table says: what is there is all there is)
    const ByteView all = data();
    lo = std::min(lo, all.size());
    hi = std::min(std::max(hi, lo), all.size());
    return all.subspan(lo, hi - lo);
}

// ---- what is where ---------------------------------------------------------------------------------------------------------------

const Section* Image::section_of(Va va) const {
    for (const Section& s : sections_) {
        if (s.va <= va && va < s.va + s.extent) return &s;
    }
    return nullptr;
}

std::optional<std::size_t> Image::offset(Va va) const {
    if (base_ <= va && va < base_ + headers_) return static_cast<std::size_t>(va - base_);
    const Section* s = section_of(va);
    if (s != nullptr && s->stored && va - s->va < s->raw_size) return static_cast<std::size_t>(s->raw_ptr + (va - s->va));
    return std::nullopt;
}

std::optional<Va> Image::va_of_offset(std::size_t offset) const {
    for (const Section& s : sections_) {
        if (s.stored && s.raw_ptr <= offset && offset < static_cast<std::size_t>(s.raw_ptr) + s.raw_size) return s.va + (offset - s.raw_ptr);
    }
    return std::nullopt;
}

bool Image::in_code(Va va) const {
    return std::ranges::any_of(sections_, [&](const Section& s) { return s.code && s.stored && s.va <= va && va < s.va + s.raw_size; });
}

// The RVA fields of the import directory (the descriptors, the lookup and the address tables), by their file offsets.
std::map<std::size_t, std::uint32_t> Image::import_rva_fields() const {
    std::map<std::size_t, std::uint32_t> out;
    const formats::pe::DataDirectory directory = pe_.directory(kImportDirectory);
    if (directory.rva == 0) return out;
    const auto first = pe_.file_offset(directory.rva);
    if (!first) return out;
    const ByteView d = data();
    for (std::size_t k = 0; *first + kImportDescriptor * (k + 1) <= d.size(); ++k) {
        const std::size_t record = static_cast<std::size_t>(*first) + kImportDescriptor * k;
        const std::uint32_t lookup = u32_at(d, record), name = u32_at(d, record + 12), address_table = u32_at(d, record + 16);
        if (lookup == 0 && name == 0 && address_table == 0) break;
        for (const auto& [at, v] : {std::pair{record, lookup}, std::pair{record + 12, name}, std::pair{record + 16, address_table}}) {
            if (v != 0) out[at] = v;
        }
        for (const std::uint32_t table : {lookup, address_table}) {
            const auto start = table != 0 ? pe_.file_offset(table) : std::nullopt;
            if (!start) continue;
            for (std::size_t j = 0; *start + 4 * (j + 1) <= d.size(); ++j) {
                const std::uint32_t v = u32_at(d, static_cast<std::size_t>(*start) + 4 * j);
                if (v == 0) break;
                if ((v & kImportByOrdinal) == 0) out[static_cast<std::size_t>(*start) + 4 * j] = v;
            }
        }
    }
    return out;
}

// ---- code ------------------------------------------------------------------------------------------------------------------------

void Image::decode_code() {
    const platforms::Decoder& decoder = decoders_.get(platforms::Arch::X86, 32, true);
    std::vector<Va> seeds;
    if (pe_.header().entry_rva != 0) seeds.push_back(base_ + pe_.header().entry_rva);
    std::set<Va> found;
    for (const formats::pe_functions::Function& f : formats::pe_functions::function_spans(pe_)) found.insert(f.start);
    seeds.insert(seeds.end(), found.begin(), found.end());
    const std::vector<Va> entries = seeds;   // (the entry and the functions found: each a function)
    std::set<Va> relocated_targets;
    for (const auto& [at, value] : sites_) {
        if (in_code(value)) relocated_targets.insert(value);
    }
    seeds.insert(seeds.end(), relocated_targets.begin(), relocated_targets.end());

    std::vector<Va> work;   // (taken from the end)
    for (const Va a : seeds) {
        if (in_code(a)) work.push_back(a);
    }
    while (!work.empty()) {
        Va a = work.back();
        work.pop_back();
        while (in_code(a) && !insns_.contains(a)) {
            if (covered_.contains(a)) break;   // into the middle of one already decoded
            const std::optional<std::size_t> o = offset(a);
            if (!o) break;
            const std::optional<xf::Insn> one = xf::decode(decoder, bytes(*o, *o + kLongestInstruction), static_cast<std::uint32_t>(a));
            if (!one || !fits(*one) || !in_code(a + one->size - 1)) break;
            bool overlap = false;
            for (std::uint32_t k = 0; k < one->size && !overlap; ++k) overlap = covered_.contains(a + k);
            if (overlap) break;
            insns_[a] = *one;
            for (std::uint32_t k = 0; k < one->size; ++k) covered_[a + k] = a;
            const std::optional<std::uint32_t> target = xf::branch_target(*one);
            if (target && in_code(*target)) work.push_back(*target);
            if (ends_flow(one->mnemonic)) break;
            a += one->size;
        }
    }
    // where each function begins, for the fragment each is in the file's L2 tree: those and every place a near call lands, where an
    // instruction was found
    for (const Va a : entries) {
        if (insns_.contains(a)) starts_.insert(a);
    }
    for (const auto& [at, insn] : insns_) {
        if (insn.mnemonic != "call") continue;
        if (const auto target = xf::branch_target(insn); target && insns_.contains(*target)) starts_.insert(*target);
    }
}

// Every relocated place inside the instruction is its whole 4-byte displacement or immediate.
bool Image::fits(const xf::Insn& insn) const {
    for (std::uint32_t k = 0; k < insn.size; ++k) {
        if (!sites_.contains(insn.address + k)) continue;
        const bool displacement = k == insn.disp_offset && insn.disp_size == 4;
        const bool immediate = k == insn.imm_offset && insn.imm_size == 4;
        if (!displacement && !immediate) return false;
        if (xf::is_branch(insn.mnemonic) && k == insn.imm_offset) return false;   // a relative operand is never relocated
    }
    return true;
}

// ---- names -----------------------------------------------------------------------------------------------------------------------

// The addresses that need a label: what relocated places, import fields and branches name.
std::set<Va> Image::wanted() const {
    std::set<Va> want;
    for (const auto& [at, value] : sites_) want.insert(value);
    for (const auto& [at, rva] : rva_fields_) want.insert(base_ + rva);
    for (const auto& [at, insn] : insns_) {
        if (const auto target = xf::branch_target(insn)) want.insert(*target);
    }
    if (pe_.header().entry_rva != 0) want.insert(base_ + pe_.header().entry_rva);
    return want;
}

std::string Image::name(Va va) const {
    if (const auto slot = imports_.find(va); slot != imports_.end()) return slot->second;
    return insns_.contains(va) ? std::format("L{:x}", va) : std::format("D{:x}", va);
}

// How the source spells an address: its label, a label plus a distance, or IMAGE plus one.
std::string Image::ref(Va va) const {
    if (const auto p = placed_.find(va); p != placed_.end()) return p->second;
    if (const auto start = covered_.find(va); start != covered_.end()) {   // inside an instruction
        const auto label = placed_.find(start->second);
        CB_ASSERT(label != placed_.end(), "an instruction something points into has a label");
        return std::format("{}+{}", label->second, va - start->second);
    }
    if ((base_ <= va && va < base_ + headers_) || section_of(va) != nullptr) return std::format("IMAGE+0x{:x}", va - base_);
    return std::format("0x{:x}", va & 0xFFFFFFFF);   // (outside the image: a fixed address)
}

// ---- source ----------------------------------------------------------------------------------------------------------------------

// An instruction's lines: its mnemonic in NASM's spelling, or its bytes with the relocated places and branch distances as expressions (`raw`).
std::vector<std::string> Image::text(const xf::Insn& insn, bool raw) const {
    const Va a = insn.address;
    const ByteView b = insn.raw();
    const std::optional<std::uint32_t> target = xf::branch_target(insn);
    std::vector<std::uint32_t> inside;   // the offsets in the instruction where a relocation patches
    for (std::uint32_t k = 0; k < insn.size; ++k) {
        if (sites_.contains(a + k)) inside.push_back(k);
    }
    if (!raw) {
        if (const std::optional<std::string> t = nasm_text(insn, target, inside); t && !t->empty()) return {*t};
    }
    std::vector<std::string> out;
    std::uint32_t k = 0;
    const std::uint32_t n = insn.size;
    while (k < n) {
        if (std::ranges::find(inside, k) != inside.end()) {
            out.push_back("dd " + ref(sites_.at(a + k)));
            k += 4;
        } else if (target && k == insn.imm_offset) {
            CB_ASSERT(insn.imm_size == 4 || insn.imm_size == 2 || insn.imm_size == 1, "a branch's displacement is 1, 2 or 4 bytes");
            const char* directive = insn.imm_size == 4 ? "dd" : insn.imm_size == 2 ? "dw" : "db";
            out.push_back(std::format("{} {}-($+{})", directive, ref(*target), insn.imm_size));
            k += insn.imm_size;
        } else {
            std::uint32_t stop = n;
            for (const std::uint32_t x : inside) {
                if (x > k) stop = std::min(stop, x);
            }
            if (target && insn.imm_offset > k) stop = std::min<std::uint32_t>(stop, insn.imm_offset);
            out.push_back("db " + xf::db_bytes(b, k, stop));
            k = stop;
        }
    }
    out.front() = annotated(out.front(), insn);
    return out;
}

// The instruction in NASM's spelling, or nothing where only its bytes will do.
std::optional<std::string> Image::nasm_text(const xf::Insn& insn, const std::optional<std::uint32_t>& target,
                                            const std::vector<std::uint32_t>& inside) const {
    static const Regex scaled_index(R"(\[(\w\w:)?(e\w\w\*[248]))");
    static const Regex base_and_index(R"([a-z]{2,3}\s*[+-])");
    const std::string& m = insn.mnemonic;
    if (m == "ljmp" || m == "lcall") return std::nullopt;
    if (xf::is_string_op(m)) {
        for (const std::uint8_t p : insn.prefix) {
            if (xf::is_segment_prefix(p)) return std::nullopt;
        }
        return m;
    }
    if (target) {
        if (m == "call") return insn.imm_size == 4 ? std::optional<std::string>("call " + ref(*target)) : std::nullopt;
        if (m == "loop" || m == "loope" || m == "loopne" || m == "jecxz" || m == "jcxz") return m + " " + ref(*target);
        return std::format("{} {} {}", m, insn.imm_size == 1 ? "short" : "near", ref(*target));
    }
    if (const std::optional<std::string> pair = xf::register_pair_text(insn)) return pair;
    // (NASM would split an index that has a scale into a base and an index: `nosplit` keeps it one register)
    std::string ops = xf::nasm_operands(scaled_index.replace_all(insn.op_str, "[$1nosplit $2"));
    for (const std::uint32_t k : inside) {
        const std::uint32_t v = sites_.at(insn.address + k);
        const std::string label = ref(v);
        if (k == insn.disp_offset) {
            const std::optional<Brackets> br = find_brackets(ops);
            if (!br) return std::nullopt;
            const std::string_view group = std::string_view(ops).substr(br->open, br->close - br->open + 1);
            const std::optional<std::string> inner = replace_number(group, v, false, false, label);
            if (!inner) return std::nullopt;
            ops = ops.substr(0, br->open) + *inner + ops.substr(br->close + 1);
        } else {
            const std::optional<std::string> replaced = replace_number(ops, v, true, true, (m == "push" ? "dword " : "") + label);
            if (!replaced) return std::nullopt;
            ops = *replaced;
        }
    }
    if ((insn.imm_size == 2 || insn.imm_size == 4) && !(std::ranges::find(inside, insn.imm_offset) != inside.end()) && xf::is_arith(m)) {
        if (const auto n = xf::trailing_number(ops)) {
            const std::int64_t short_form_from = insn.imm_size == 2 ? 0xFF80 : 0xFFFFFF80;
            if ((-128 <= n->value && n->value <= 127) || n->value >= short_form_from) {
                ops.insert(n->start, insn.imm_size == 2 ? "strict word " : "strict dword ");
            }
        }
    }
    if (insn.disp_size == 4 && !(std::ranges::find(inside, insn.disp_offset) != inside.end())) {
        if (const std::optional<Brackets> br = find_brackets(ops)) {
            const std::string_view group = std::string_view(ops).substr(br->open + 1, br->close - br->open - 1);
            if (base_and_index.test(group)) ops = ops.substr(0, br->open) + "[dword " + std::string(group) + "]" + ops.substr(br->close + 1);
        }
    }
    return std::string(text::trim(m + " " + ops));
}

flat_rounds::Source<Va> Image::write(const std::set<Va>& raw, bool final) { return source(raw, !final); }

flat_rounds::Source<Va> Image::source(const std::set<Va>& raw, bool mark_all) {
    std::set<Va> want = wanted();
    if (mark_all) {
        for (const auto& entry : insns_) want.insert(entry.first);
    }
    placed_.clear();
    // a label can stand at an instruction's start or anywhere in data
    for (const Va va : want) {
        if ((insns_.contains(va) || !covered_.contains(va)) && (section_of(va) != nullptr || (base_ <= va && va < base_ + headers_))) {
            placed_[va] = name(va);
        }
    }
    std::set<Va> pointed_into;
    for (const Va va : want) {
        if (const auto start = covered_.find(va); start != covered_.end() && !insns_.contains(va)) pointed_into.insert(start->second);
    }
    for (const Va start : pointed_into) placed_[start] = name(start);

    l2::Assembly out({}, "nasm", file_name_);   // (the file as one L2 tree, as it is written)
    std::map<std::size_t, Va> lines_of;
    out.append(std::format("; {} -- assembled with NASM: nasm -f bin -o {} this file", file_name_, file_name_));
    out.append("bits 32");
    out.append(std::format("IMAGE equ 0x{:x}", base_));
    out.extend(prelude());
    out.part("hdr", "headers", translate::ir::OriginVa{file_name_, base_}, headers_);
    out.append("");
    out.append(std::format("section hdr start=0 vstart=0x{:x} align=1", base_));
    if (stub_) {
        out.append(std::format("incbin \"{}\"", fs::to_text(*stub_)));
        write_data(out, base_ + fs::file_size(*stub_), base_ + headers_, true);
    } else {
        write_data(out, base_, base_ + headers_, true);
    }

    // what the file holds between the headers and the sections, or between two sections: as it is
    std::map<std::string, std::pair<std::size_t, std::size_t>> gaps;
    {
        std::vector<const Section*> in_file;
        for (const Section& s : sections_) {
            if (s.stored) in_file.push_back(&s);
        }
        std::ranges::stable_sort(in_file, {}, &Section::raw_ptr);
        std::size_t ends = headers_;
        for (const Section* s : in_file) {
            if (s->raw_ptr > ends) gaps[s->nasm_name] = {ends, s->raw_ptr};
            ends = std::max<std::size_t>(ends, static_cast<std::size_t>(s->raw_ptr) + s->raw_size);
        }
    }
    for (const Section& s : sections_) {
        out.part(s.nasm_name, "section", translate::ir::OriginVa{file_name_, s.va}, s.stored ? std::optional<std::uint64_t>(s.raw_size) : std::nullopt);
        out.append("");
        if (const auto gap = gaps.find(s.nasm_name); gap != gaps.end()) {
            out.append(std::format("section gap_{} start=0x{:x} vstart=0 align=1", text::trim(s.nasm_name, "."), gap->second.first));
            for (const std::string& x : assembly::data_lines(bytes(gap->second.first, gap->second.second))) out.append(indented(x));
            out.append("");
        }
        if (!s.stored) {
            out.append(std::format("section {} nobits vstart=0x{:x}", s.nasm_name, s.va));
            write_nobits(out, s);
            continue;
        }
        out.append(std::format("section {} progbits start=0x{:x} vstart=0x{:x} align=1", s.nasm_name, s.raw_ptr, s.va));
        Va a = s.va;
        const Va end = s.va + s.raw_size;
        std::vector<Va> mine;   // the function starts in this section, ascending
        for (const Va start : starts_) {
            if (a <= start && start < end) mine.push_back(start);
        }
        std::map<Va, Va> ends;   // (a function runs to the next one)
        for (std::size_t k = 0; k < mine.size(); ++k) ends[mine[k]] = k + 1 < mine.size() ? mine[k + 1] : end;
        while (a < end) {
            if (const auto found = insns_.find(a); found != insns_.end()) {
                const xf::Insn& insn = found->second;
                const auto label = placed_.find(a);
                if (const auto fe = ends.find(a); fe != ends.end()) {   // (each function a fragment of its own)
                    out.part(label != placed_.end() && !label->second.empty() ? label->second : name(a), "function",
                             translate::ir::OriginVa{file_name_, a}, fe->second - a);
                }
                if (label != placed_.end()) out.append(label->second + ":");
                for (const std::string& t : text(insn, raw.contains(a))) {
                    lines_of[out.lines().size() + 1] = a;
                    out.append(indented(t));
                }
                a += insn.size;
            } else {
                Va stop = a + 1;
                while (stop < end && !insns_.contains(stop)) ++stop;
                write_data(out, a, stop, false);
                a = stop;
            }
        }
        // labels in the part of the section the file does not store (zeros the loader adds): from its end
        for (auto label = placed_.lower_bound(end); label != placed_.end() && label->first < s.va + s.extent; ++label) {
            out.append(std::format("{} equ $+0x{:x}", label->second, label->first - end));
        }
    }
    if (tail_ != 0) {
        out.part("tail", "after the sections", std::nullopt, tail_);
        out.append("");
        out.append(std::format("section tail start=0x{:x} vstart=0 align=1", data().size() - tail_));
        for (const std::string& x : assembly::data_lines(bytes(data().size() - tail_, data().size()))) out.append(indented(x));
    }
    out.append("");
    assembly_ = out;
    return {image_writer::text(out), std::move(lines_of)};
}

// A section the file does not store: space, with a label where one is wanted.
void Image::write_nobits(l2::Assembly& out, const Section& s) const {
    Va a = s.va;
    const Va end = s.va + s.extent;
    for (auto label = placed_.lower_bound(a); label != placed_.end() && label->first < end; ++label) {
        if (label->first > a) out.append(std::format("    resb 0x{:x}", label->first - a));
        out.append(label->second + ":");
        a = label->first;
    }
    if (end > a) out.append(std::format("    resb 0x{:x}", end - a));
}

// Bytes from `from` to `to`: labels where they stand, relocated places and RVA fields as expressions, the rest as `db` lines. `header`: the
// bytes are the headers' (the entry point is written as the label of the entry).
void Image::write_data(l2::Assembly& out, Va from, Va to, bool header) const {
    const std::uint32_t entry = pe_.header().entry_rva;
    const Va entry_field = base_ + pe_.header().e_lfanew + kOptionalHeaderAt + kEntryPointField;
    std::set<Va> cuts;
    for (auto p = placed_.lower_bound(from); p != placed_.end() && p->first < to; ++p) cuts.insert(p->first);
    for (auto s = sites_.lower_bound(from); s != sites_.end() && s->first < to; ++s) cuts.insert(s->first);
    if (header && entry != 0) cuts.insert(entry_field);
    std::map<Va, std::uint32_t> rva_at;   // the RVA fields that lie in this stretch, by address
    for (const auto& [file_offset, rva] : rva_fields_) {
        if (const auto va = va_of_offset(file_offset); va && from <= *va && *va < to) rva_at[*va] = rva;
    }
    for (const auto& entry_at : rva_at) cuts.insert(entry_at.first);

    Va pos = from;
    std::vector<Va> ordered(cuts.begin(), cuts.end());
    ordered.push_back(to);
    for (const Va c : ordered) {
        const auto label = placed_.find(c);
        if (c < pos) {   // inside the dword just written
            if (label != placed_.end()) out.append(std::format("{} equ $-{}", label->second, pos - c));
            continue;
        }
        if (c > pos) {
            const std::optional<std::size_t> o = offset(pos);
            if (!o) throw FormatError(kReader, pos, "bytes to write lie outside the file's stored part");
            for (const std::string& x : assembly::data_lines(bytes(*o, *o + (c - pos)))) out.append(indented(x));
            pos = c;
        }
        if (c == to) break;
        if (label != placed_.end()) out.append(label->second + ":");
        if (const auto site = sites_.find(c); site != sites_.end() && c + 4 <= to) {
            out.append("    dd " + ref(site->second));
            pos = c + 4;
        } else if (const auto field = rva_at.find(c); field != rva_at.end() && c + 4 <= to) {
            out.append(std::format("    dd {}-IMAGE", ref(base_ + field->second)));
            pos = c + 4;
        } else if (header && entry != 0 && c == entry_field) {
            out.append(std::format("    dd {}-IMAGE                          ; the entry point", ref(base_ + entry)));
            pos = c + 4;
        }
    }
}

// The instructions the assembler did not give back: compared where each one went, its length (to the next instruction's label) and its
// bytes, outside the places whose value depends on where things are (relocated addresses, branch distances).
std::set<Va> Image::differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Va>& raw) {
    std::set<Va> bad;
    const auto label_of = [&](Va a) -> const flat_rounds::Label* {
        const auto name = placed_.find(a);
        if (name == placed_.end()) return nullptr;
        const auto hit = labels.find(name->second);
        return hit == labels.end() ? nullptr : &hit->second;
    };
    for (auto it = insns_.begin(); it != insns_.end(); ++it) {
        const Va a = it->first;
        const xf::Insn& insn = it->second;
        if (raw.contains(a)) continue;
        const flat_rounds::Label* here = label_of(a);
        const Section* s = section_of(a);
        if (here == nullptr || s == nullptr) {
            bad.insert(a);
            continue;
        }
        if (const auto next = std::next(it); next != insns_.end() && next->first == a + insn.size) {
            const flat_rounds::Label* there = label_of(next->first);
            if (there != nullptr && section_of(next->first) == s && static_cast<std::int64_t>(there->address) - static_cast<std::int64_t>(here->address) != insn.size) {
                bad.insert(a);
                continue;
            }
        }
        const std::size_t at = static_cast<std::size_t>(s->raw_ptr + (here->address - s->va));
        const std::optional<std::size_t> original_at = offset(a);
        CB_ASSERT(original_at.has_value(), "an instruction lies in the stored part of a section");
        const ByteView mine = built.subspan(std::min(at, built.size()), std::min<std::size_t>(insn.size, built.size() - std::min(at, built.size())));
        const ByteView original = bytes(*original_at, *original_at + insn.size);
        // (the bytes of a relocated place, and a branch's displacement, depend on where things are)
        std::set<std::size_t> skip;
        for (std::uint32_t k = 0; k < insn.size; ++k) {
            if (sites_.contains(a + k)) skip.insert({k, k + 1, k + 2, k + 3});
        }
        if (xf::branch_target(insn)) {
            for (std::uint32_t k = 0; k < insn.imm_size; ++k) skip.insert(insn.imm_offset + k);
        }
        bool differs = mine.size() != original.size();
        for (std::size_t k = 0; !differs && k < original.size(); ++k) {
            if (!skip.contains(k) && mine[k] != original[k]) differs = true;
        }
        if (differs) bad.insert(a);
    }
    return bad;
}

// ---- the result ------------------------------------------------------------------------------------------------------------------

flat_rounds::FlatReport Result::report() const {
    flat_rounds::FlatReport r;
    r.instructions = instructions;
    r.raw = raw;
    r.rounds = rounds;
    r.same = same;
    r.built = image.has_value();
    r.source = source;
    return r;
}

Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir_, const Options& options) {
    const std::function<void(std::string_view)> say = options.say ? options.say : [](std::string_view) {};
    const std::filesystem::path outdir = std::filesystem::absolute(outdir_);
    fs::make_dirs(outdir);
    Image image = Image::open(program);
    const std::string name = text::lower(fs::to_text(program.filename()));
    std::optional<std::filesystem::path> stub;
    if (std::holds_alternative<SearchStub>(options.stub)) {
        if (options.find_stub) stub = options.find_stub(image.pe().data(), image.pe().header().e_lfanew, outdir, say);
    } else if (const auto* given = std::get_if<std::filesystem::path>(&options.stub)) {
        stub = *given;
    }
    if (stub && stub->empty()) stub.reset();
    image.set_stub(stub);

    const flat_rounds::FlatResult<Va> fr = flat_rounds::flat<Va>(image.pe().data(), image, outdir, name, {.say = say});
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = image.instruction_count();
    r.raw = fr.raw.size();
    return r;
}

int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say) {
    std::vector<std::string> programs;
    std::optional<std::string> out, stub;
    bool keep_stub = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out" || args[i] == "--stub") {
            if (i + 1 >= args.size()) throw UsageError(args[i] + " needs a value");
            std::optional<std::string>& slot = args[i] == "--out" ? out : stub;
            slot = args[++i];
        } else if (args[i] == "--keep-stub") {
            keep_stub = true;
        } else if (!args[i].starts_with("--")) {
            programs.push_back(args[i]);
        }
    }
    if (programs.empty()) {
        say(usage());
        return 2;
    }
    const std::filesystem::path program = fs::from_text(programs.front());
    const std::filesystem::path outdir = out ? fs::from_text(*out) : env.state / "pe_source" / text::lower(fs::to_text(program.stem()));
    Options options{.stub = SearchStub{}, .find_stub = env.find_stub, .say = say};
    if (keep_stub) options.stub = KeepStub{};
    else if (stub) options.stub = std::filesystem::absolute(fs::from_text(*stub));
    const flat_rounds::ReportText report = flat_rounds::report(program, reconstruct(program, outdir, options).report());
    say(text::rtrim(report.text));
    return report.exit_code;
}

}  // namespace cb::source::pe_writer
