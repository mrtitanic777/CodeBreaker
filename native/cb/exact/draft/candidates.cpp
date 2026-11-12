#include "cb/exact/draft/candidates.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <map>
#include <set>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/candidates_internal.hpp"
#include "cb/exact/draft/shapes.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::draft::candidates {

using namespace inner;

namespace {

// How long a variant list may grow before the beam gets it: nothing limits it here, the prover does.
constexpr std::uint32_t kDataPlaces = 0x10000;   // (the data and the BSS after it: any place a pointer is compared with)
constexpr std::size_t kCharParamBases = 4;       // (the first variants a value given twice is chained in)
constexpr std::size_t kAddressBases = 2;         // (the first variants a number that is a place in the data is tried in)
constexpr std::size_t kUnsignedGlobalBases = 3;  // (the first localized sources a compared global is made unsigned in)

// `-?(0x[0-9a-f]+|\d+)` as Python reads a number with base 0 (no octal): the value, or none.
std::optional<std::int64_t> cand_number(sv s) {
    bool negative = false;
    if (s.starts_with("-")) {
        negative = true;
        s.remove_prefix(1);
    } else if (s.starts_with("+")) {
        s.remove_prefix(1);
    }
    std::optional<std::int64_t> v;
    if (s.starts_with("0x") || s.starts_with("0X")) v = ct::parse_int<std::int64_t>(s.substr(2), 16);
    else if (s.size() > 1 && s[0] == '0') v = std::nullopt;   // (a leading zero: no number)
    else v = ct::parse_int<std::int64_t>(s, 10);
    if (!v) return std::nullopt;
    return negative ? -*v : *v;
}

// The first index at which the variants hold `v` (a variant that came twice is inserted after its first place).
std::size_t cand_index_of(const std::vector<std::string>& variants, const std::string& v) {
    return static_cast<std::size_t>(std::ranges::find(variants, v) - variants.begin());
}

}  // namespace

std::string IntDecl::rest() const {
    std::vector<std::string> all = before;
    all.insert(all.end(), after.begin(), after.end());
    return ct::join(all, ", ");
}

std::optional<IntDecl> inner::int_decl_parts(sv line, sv x) {
    IntDecl out;
    sv rest = line;
    std::size_t blanks = 0;
    while (blanks < rest.size() && (rest[blanks] == ' ' || rest[blanks] == '\t')) ++blanks;
    out.indent = str(rest.substr(0, blanks));
    rest.remove_prefix(blanks);
    if (!rest.starts_with("int ") || !rest.ends_with(";")) return std::nullopt;
    rest = rest.substr(4, rest.size() - 5);
    std::vector<std::string> names;
    for (const sv item : ct::split(rest, ", ")) {
        if (item.empty() || !std::ranges::all_of(item, is_word)) return std::nullopt;
        names.emplace_back(item);
    }
    // (the pattern's greedy list of names before x finds the last x)
    for (std::size_t i = names.size(); i-- > 0;) {
        if (names[i] != x) continue;
        out.before.assign(names.begin(), names.begin() + static_cast<std::ptrdiff_t>(i));
        out.after.assign(names.begin() + static_cast<std::ptrdiff_t>(i) + 1, names.end());
        return out;
    }
    return std::nullopt;
}

std::map<std::uint32_t, std::string> data_names_of(const SymbolTable& symbols) {
    std::map<std::uint32_t, std::string> out;
    for (const auto& [name, place] : symbols) {
        if (place.kind != SymKind::Data || name.find(':') != std::string::npos || place.address < 0 || place.address > 0xFFFFFFFFll) continue;
        if (name.empty() || !std::ranges::all_of(name, is_word)) continue;
        sv bare = strip_left(name, "_");
        if (bare == std::format("g_{:04x}", place.address)) continue;
        out[static_cast<std::uint32_t>(place.address)] = str(bare);   // (the later name of a place wins)
    }
    return out;
}

// ---- the bytes the code reads ---------------------------------------------------------------------------------

std::optional<ByteFields> byte_fields(const MarkingModel* model, ByteView code) {
    if (model == nullptr) return std::nullopt;
    const std::optional<std::string> marked = model->marked_c(code);
    if (!marked) return std::nullopt;
    ByteFields out;
    static const Regex byte_field(R"re(BYTE\(\s*\(?(\w+)->f(\d+)\)?\s*\))re");
    byte_field.for_each_match(*marked, [&](const RegexMatch& m) {
        out.sites.insert(Site{str(m[1]), to_int(m[2])});
        return true;
    });
    static const Regex byte_deref(R"re(BYTE\(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)?\))re");   // (*p a byte: a char *)
    byte_deref.for_each_match(*marked, [&](const RegexMatch& m) {
        out.sites.insert(Site{str(m[1]), 0});
        return true;
    });
    // (and from the code itself: a byte read or written through a register at an offset - whatever the draft names the
    // pointer)
    const std::vector<x86::Insn> insns = x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()), false);
    for (std::size_t k = 0; k + 4 < insns.size(); ++k) {   // (a register loaded and tested against a range: a switch's)
        const x86::Insn &load = insns[k], &lo_insn = insns[k + 1], &below = insns[k + 2], &hi_insn = insns[k + 3], &above = insns[k + 4];
        std::optional<std::string> lo;
        if (lo_insn.mnemonic == "or" && lo_insn.op_str == "ax, ax") lo = "0";
        else if (lo_insn.mnemonic == "cmp") lo = lo_insn.op_str.substr(std::min<std::size_t>(4, lo_insn.op_str.size()));
        if (load.mnemonic == "mov" && load.op_str.starts_with("ax, ") && lo && hi_insn.mnemonic == "cmp" && lo_insn.op_str.starts_with("ax, ") &&
            hi_insn.op_str.starts_with("ax, ") && (below.mnemonic == "jl" || below.mnemonic == "jb") &&
            (above.mnemonic == "jg" || above.mnemonic == "ja" || above.mnemonic == "jle" || above.mnemonic == "jbe")) {
            const auto low = cand_number(*lo), high = cand_number(hi_insn.op_str.substr(4));
            if (low && high) out.ranges.insert(dt::Range{*low, *high, above.mnemonic == "jg" || above.mnemonic == "ja"});   // (jg/ja to the default: the cases' code first)
        }
    }
    static const Regex zero_load(R"re(al, byte ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\])re");
    for (std::size_t k = 0; k < insns.size(); ++k) {   // (a slot's byte loaded and widened with zeros: unsigned)
        if (insns[k].mnemonic != "mov") continue;
        const auto z = zero_load.full_match(insns[k].op_str);
        if (!z) continue;
        bool widened = false;
        for (std::size_t j = k + 1; j < std::min(insns.size(), k + 4); ++j) {
            const x86::Insn& x = insns[j];
            widened = widened || ((x.op_str == "ah, ah" || x.op_str == "ah, 0") && (x.mnemonic == "sub" || x.mnemonic == "xor" || x.mnemonic == "mov"));
        }
        if (widened) out.frame_slots.insert(FrameSlot{((*z)[1] == "+" ? 1 : -1) * *cand_number((*z)[2]), SlotWidth::ZeroExtended});
    }
    static const Regex indexed_byte(R"re(byte ptr \[(?:bx|si|di) \+ (0x[0-9a-f]+|\d+)\])re");
    static const Regex width_at(R"re((byte|word) ptr \[(bx|si|di)(?: \+ (0x[0-9a-f]+|\d+))?\])re");
    static const Regex global_byte(R"re(byte ptr \[(0x[0-9a-f]+|\d+)\])re");
    static const Regex frame_width(R"re((byte|word) ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\])re");
    for (const x86::Insn& i : insns) {
        if (const auto m = indexed_byte.search(i.op_str)) {
            const std::int64_t offset = *cand_number((*m)[1]);
            out.sites.insert(Site{offset < 0x100 ? "*" : "data", offset});   // (a byte in the data indexed by a register)
        }
        width_at.for_each_match(i.op_str, [&](const RegexMatch& m) {   // (each width at each offset through each register: an offset read both ways is two structs')
            const std::int64_t offset = m.has(3) ? *cand_number(m[3]) : 0;
            if (offset < 0x100) out.sites.insert(Site{str(m[2]) + (m[1] == "byte" ? "*" : "w"), offset});
            return true;
        });
        if (const auto g = global_byte.search(i.op_str)) {   // (a global read or written a byte at a time - never a word: a word's low byte)
            const std::string word = "word ptr [" + str((*g)[1]) + "]";
            if (!std::ranges::any_of(insns, [&](const x86::Insn& x) { return x.op_str.find(word) != std::string::npos; }))
                out.globals.insert(std::format("g_{:04x}", *cand_number((*g)[1])));
        }
        if (const auto f = frame_width.search(i.op_str)) {   // (a frame slot by its width)
            out.frame_slots.insert(FrameSlot{((*f)[2] == "+" ? 1 : -1) * *cand_number((*f)[3]), (*f)[1] == "byte" ? SlotWidth::Byte : SlotWidth::Word});
        }
    }
    return out;
}

// ---- the frame --------------------------------------------------------------------------------------------------

std::string frame_chars(std::string_view text_in, const State& state) {
    std::map<std::int64_t, std::set<SlotWidth>> slots;
    for (const FrameSlot& s : state.frame_slots) slots[s.offset].insert(s.width);
    std::vector<std::pair<std::string, std::string>> chars;   // (name, type)
    for (const auto& [off, widths] : slots) {
        std::set<SlotWidth> plain = widths;
        plain.erase(SlotWidth::ZeroExtended);
        if (plain != std::set<SlotWidth>{SlotWidth::Byte}) continue;
        std::string name;
        if (off < 0) name = std::format("v{}", -off);
        else if (off >= 4 && off % 2 == 0) name = std::string(1, static_cast<char>('a' + (off - 4) / 2));
        else continue;
        chars.emplace_back(name, widths.contains(SlotWidth::ZeroExtended) ? "unsigned char" : "char");   // (widened with zeros where it is read: unsigned)
    }
    std::string text = str(text_in);
    for (const auto& [x, ty] : chars) {
        const std::string e = re_escape(x);
        const Regex lone(R"re((?m)^([ \t]*)int )re" + e + ";$");
        text = lone.replace_all(text, "$1" + ty + " " + x + ";");
        text = map_lines(text, [&](const std::string& line) {
            const auto parts = int_decl_parts(line, x);
            if (!parts) return line;
            return parts->indent + "int " + parts->rest() + ";\n" + parts->indent + ty + " " + x + ";";
        });
        if (ty != "char") {
            const Regex char_decl(R"re((?m)^([ \t]*)char )re" + e + ";$");
            text = char_decl.replace_all(text, "$1" + ty + " " + x + ";");
        }
    }
    return text;
}

std::optional<std::string> buffers(std::string_view text_in) {
    std::string text = str(text_in);
    static const Regex frame_place(R"re(\bbp - (\d+)\b)re");
    std::set<std::int64_t> places;
    frame_place.for_each_match(text, [&](const RegexMatch& m) {
        places.insert(to_int(m[1]));
        return true;
    });
    for (const std::int64_t n : places) {   // (the frame's place bp-N: a local there, in place of the filler that stood for it)
        const Regex local(R"re((?m)^[ \t]*int v)re" + std::to_string(n) + ";");
        if (!local.search(text)) {
            static const Regex filler(R"re((?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n)re");
            std::optional<RegexMatch> last;
            filler.for_each_match(text, [&](const RegexMatch& m) {
                last = m;
                return true;
            });
            if (!last) continue;
            text = text.substr(0, last->start()) + str(last->group(1)) + "int v" + std::to_string(n) + ";\n" + text.substr(last->end());
        }
        const Regex place(R"re(\bbp - )re" + std::to_string(n) + R"re(\b)re");
        text = place.replace_all(text, "&v" + std::to_string(n));
    }
    static const Regex int_local(R"re((?m)^[ \t]*int v(\d+);)re");
    std::set<std::int64_t> locals;
    int_local.for_each_match(text, [&](const RegexMatch& m) {
        locals.insert(to_int(m[1]));
        return true;
    });
    std::vector<std::int64_t> taken;
    for (const std::int64_t n : locals) {
        const Regex address(R"re(&v)re" + std::to_string(n) + R"re(\b)re");
        if (address.search(text)) taken.push_back(n);
    }
    if (taken.empty()) return std::nullopt;
    for (const std::int64_t n : taken) {
        std::int64_t below = 0;
        for (const std::int64_t m : locals)
            if (m < n) below = std::max(below, m);
        const std::int64_t size = n - below;
        const std::string v = "v" + std::to_string(n);
        const Regex decl(R"re((?m)^([ \t]*)int )re" + v + ";");
        text = decl.replace_all(text, "$1char " + v + "[" + std::to_string(size) + "];");
        const Regex address(R"re(&)re" + v + R"re(\b)re");
        text = address.replace_all(text, v);
        const Regex store(R"re((?m)^([ \t]*))re" + v + " = ");
        text = store.replace_all(text, "$1*(int *)" + v + " = ");
    }
    return text;
}

// ---- register variables -------------------------------------------------------------------------------------------

std::string register_variables(std::string_view text, const std::vector<std::string>& regs, const RegisterParams& regparam) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    std::string head = split.head, body = split.body;
    std::vector<std::string> rdecl;   // (declared in the order of their registers: the compiler gives the first declared the first)
    std::vector<std::pair<std::string, int>> params(regparam.entries().begin(), regparam.entries().end());
    const auto rank = [&](const std::string& r) {
        const auto at = std::ranges::find(regs, r);
        return at == regs.end() ? std::size_t{9} : static_cast<std::size_t>(at - regs.begin());
    };
    std::ranges::stable_sort(params, [&](const auto& a, const auto& b) { return rank(a.first) < rank(b.first); });
    for (const auto& [r, ix] : params) {
        const std::string p(1, static_cast<char>('a' + ix));
        body = replace_word(body, r, p);
        const std::string e = re_escape(p);
        const Regex through(R"re(\*\s*\(?\s*(?:\+\+|--)?\s*)re" + e + R"re(\b|\b)re" + e + R"re(\s*(?:\+\+|--)?\s*\[)re");
        const bool ptr = through.search(body).has_value();
        Lines lines = split_nl(head);
        std::vector<std::optional<std::string>> kept;   // (out of the plain declaration of the parameters)
        static const Regex int_line(R"re((\s*)int (.*);)re");
        for (const std::string& l : lines) {
            const auto m = int_line.full_match(l);
            if (m && has_word((*m)[2], p)) {
                std::vector<std::string> rest;
                for (const sv x : ct::split((*m)[2], ',')) {
                    const sv t = ct::trim(x);
                    if (!t.empty() && t != p) rest.emplace_back(t);
                }
                if (rest.empty()) kept.push_back(std::nullopt);
                else kept.push_back(str((*m)[1]) + "int " + ct::join(rest, ", ") + ";");
            } else {
                kept.push_back(l);
            }
        }
        Lines survivors;
        for (const auto& l : kept)
            if (l) survivors.push_back(*l);
        head = join_nl(survivors);
        const Regex twice(R"re(\*\(\*)re" + e + R"re(\)|\*\*)re" + e + R"re(\b)re");
        const Regex arrow(R"re(\b)re" + e + "->");
        const bool pp = twice.search(body) && !arrow.search(body);   // (read through twice: a pointer to pointers)
        rdecl.push_back(std::string("register ") + (pp ? "char **" : ptr ? "char *" : "int ") + p + ";");
    }
    if (!rdecl.empty()) {
        std::string trimmed = head;
        while (trimmed.ends_with("\n")) trimmed.pop_back();
        head = trimmed + "\n" + ct::join(rdecl, "\n") + "\n";
        static const Regex signature(R"re((?m)^(\w+)\(([\w, ]*)\)\s*$)re");   // (the parameters in the signature, all of them)
        if (const auto sig = signature.search(head)) {
            std::vector<std::string> have;
            for (const sv x : ct::split((*sig)[2], ','))
                if (!ct::trim(x).empty()) have.emplace_back(ct::trim(x));
            std::set<std::string> wanted(have.begin(), have.end());
            for (const auto& [r, ix] : params) wanted.insert(std::string(1, static_cast<char>('a' + ix)));
            const std::vector<std::string> want(wanted.begin(), wanted.end());
            if (want != have) head = head.substr(0, sig->start()) + str((*sig)[1]) + "(" + ct::join(want, ", ") + ")" + head.substr(sig->end());
        }
    }
    std::vector<std::string> used;
    for (const std::string& r : regs)
        if (has_word(body, r)) used.push_back(r);
    if (used.empty()) return head + split.brace + body;
    std::vector<std::string> decls;
    std::set<std::string> pointer_names;   // (pointer parameters)
    static const Regex char_pointer(R"re((?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);)re");
    char_pointer.for_each_match(head, [&](const RegexMatch& m) {
        pointer_names.insert(str(m[1]));
        return true;
    });
    for (const std::string& r : used) {
        const std::string e = re_escape(r);
        const Regex starred(R"re(\*\s*)re" + e + R"re(\b|\b)re" + e + R"re(\s*\[)re");
        bool ptr = starred.search(body).has_value();
        for (const std::string& p : pointer_names) {   // (given a pointer's value)
            const Regex given(R"re((?m)^[ \t]*)re" + e + " = " + re_escape(p) + ";");
            ptr = ptr || given.search(body).has_value();
        }
        const Regex twice(R"re(\*\(\*)re" + e + R"re(\)|\*\*)re" + e + R"re(\b)re");
        const Regex arrow(R"re(\b)re" + e + "->");
        const bool pp = twice.search(body) && !arrow.search(body);
        decls.push_back(std::string("    register ") + (pp ? "char **" : ptr ? "char *" : "int ") + "r_" + r + ";");
        body = replace_word(body, r, "r_" + r);
    }
    return head + "{\n" + ct::join(decls, "\n") + body;
}

std::optional<std::string> register_slots(std::string_view text, bool register_locals_have_slots) {
    if (!register_locals_have_slots) return std::nullopt;
    static const Regex register_local(R"re((?m)^[ \t]*register [^;(]*\br_\w+;$)re");
    std::size_t n_regs = 0;
    register_local.for_each_match(text, [&](const RegexMatch&) {
        ++n_regs;
        return true;
    });
    static const Regex filler(R"re((?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n)re");
    std::optional<RegexMatch> last;
    filler.for_each_match(text, [&](const RegexMatch& m) {
        last = m;
        return true;
    });
    if (n_regs == 0 || !last) return std::nullopt;
    static const Regex next_word(R"re([ \t]*(\S+))re");
    const auto next = next_word.match(text.substr(last->end()));
    static const Regex declaration(R"re((?:register|int|char|long|unsigned|struct|short)\b)re");
    if (!next || declaration.match((*next)[1])) return std::nullopt;
    const std::int64_t n = to_int(last->group(3)) - 2 * static_cast<std::int64_t>(n_regs);   // (the trailing one, after the last local)
    return str(text.substr(0, last->start())) + (n > 0 ? std::format("{}char {}[{}];\n", last->group(1), last->group(2), n) : std::string()) +
           str(text.substr(last->end()));
}

std::vector<std::string> unsigned_locals(std::string_view text, std::size_t most) {
    std::vector<std::string> out;
    static const Regex int_local(R"re((?m)^[ \t]*int (v\d+);$)re");
    std::vector<std::string> names;
    int_local.for_each_match(text, [&](const RegexMatch& m) {
        names.push_back(str(m[1]));
        return names.size() < most;
    });
    for (const std::string& name : names) {
        const std::string e = re_escape(name);
        const Regex compared(R"re((?:[<>]=? )re" + e + R"re(\b|\b)re" + e + R"re( [<>]=?)(?![<>]))re");
        if (!compared.search(text)) continue;
        const Regex decl(R"re((?m)^([ \t]*)int )re" + e + ";$");
        out.push_back(decl.replace_all(text, "$1unsigned " + name + ";"));
    }
    return out;
}

std::optional<std::string> registers_last(std::string_view text, bool register_locals_have_slots) {
    if (!register_locals_have_slots) return std::nullopt;
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    const Lines lines = split_nl(split.body);
    std::size_t k = 0;
    while (k < lines.size() && (blank(lines[k]) || ct::trim(lines[k]) == "{")) ++k;
    std::size_t j = k;
    static const Regex declaration(R"re([ \t]*(?:register |unsigned |int |char |long |short |struct \w+ )[^;(=]*;)re");
    while (j < lines.size() && declaration.full_match(lines[j])) ++j;
    Lines decl(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(j));
    Lines regs, rest;
    for (const std::string& x : decl) (ct::trim(x).starts_with("register ") ? regs : rest).push_back(x);
    Lines reordered = rest;
    reordered.insert(reordered.end(), regs.begin(), regs.end());
    if (regs.empty() || rest.empty() || decl == reordered) return std::nullopt;
    Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
    out.insert(out.end(), reordered.begin(), reordered.end());
    out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(j), lines.end());
    return split.head + split.brace + join_nl(out);
}

// ---- strings in the data ------------------------------------------------------------------------------------------

namespace {

// data given a string (`g = "..";`, `g[i] = "..";`) declared int: a char * (an array of them)
std::string cand_string_decls(std::string text) {
    static const Regex given(R"re(\b(g_[0-9a-f]{4})(\[[^\]]*\])? = ")re");
    std::vector<std::string> names;
    given.for_each_match(text, [&](const RegexMatch& m) {
        names.push_back(str(m[1]));
        return true;
    });
    for (const std::string& g : names) {
        const Regex extern_int(R"re((?m)^extern int ((?:\w+(?:\[\])?, )*))re" + g + R"re((\[\])?((?:, \w+(?:\[\])?)*);$)re");
        text = sub_all(extern_int, text, [&](const RegexMatch& d) {
            std::vector<std::string> rest;
            for (const sv part : {d.group(1), d.group(3)}) {
                std::string s(part);
                while (s.starts_with(",") || s.starts_with(" ")) s.erase(0, 1);
                while (s.ends_with(",") || s.ends_with(" ")) s.pop_back();
                if (!s.empty()) rest.push_back(s);
            }
            std::string joined = ct::join(rest, ", ");
            return (joined.empty() ? std::string() : "extern int " + joined + ";\n") + "extern char *" + g + str(d.group(2)) + ";";
        });
    }
    return text;
}

// A C string literal's text of bytes.
std::string cand_c_string(ByteView s) {
    std::string out = "\"";
    for (const std::uint8_t c : s) {
        if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else if (c == '"' || c == '\\') {
            out += '\\';
            out += static_cast<char>(c);
        } else if (c >= 32 && c < 127) out += static_cast<char>(c);
        else out += std::format("\\{:03o}", c);
    }
    return out + "\"";
}

}  // namespace

std::vector<std::string> string_variants(const Settings& settings, std::string_view text) {
    ByteView ds;
    if (settings.data_segment <= settings.image.size()) ds = settings.image.subspan(settings.data_segment);
    static const Regex number(R"re((?<![\w.])(\d{2,5})(?![\w.]))re");
    const std::size_t brace = text.find('{');
    const sv body = brace == npos ? text : text.substr(brace + 1);
    std::set<std::int64_t> nums;
    number.for_each_match(body, [&](const RegexMatch& m) {
        nums.insert(to_int(m[1]));
        return true;
    });
    std::vector<std::pair<std::int64_t, std::string>> cands;
    for (const std::int64_t v : nums) {
        if (!(0 < v && static_cast<std::size_t>(v) < ds.size() - 1 && ds[static_cast<std::size_t>(v)] >= 32 && ds[static_cast<std::size_t>(v)] < 127 && ds[static_cast<std::size_t>(v) - 1] == 0)) continue;
        std::size_t e = static_cast<std::size_t>(v);
        while (e < ds.size() && ds[e] != 0) ++e;
        if (e == ds.size()) continue;   // (no end: no string - the original cut the last byte off such a run)
        const ByteView s = ds.subspan(static_cast<std::size_t>(v), e - static_cast<std::size_t>(v));
        if (!s.empty() && s.size() < 200 && std::ranges::all_of(s, [](std::uint8_t c) { return (c >= 32 && c < 127) || c == 9 || c == 10 || c == 13; }))
            cands.emplace_back(v, cand_c_string(s));
    }
    const auto place = [](const std::string& t, std::int64_t v, const std::string& lit) {
        const Regex number_at(R"re((?<![\w.]))re" + std::to_string(v) + R"re((?![\w.]))re");
        return number_at.replace_all(t, std::function<std::string(const RegexMatch&)>([&](const RegexMatch&) { return lit; }));
    };
    std::vector<std::string> out;
    for (const auto& [v, lit] : cands) out.push_back(cand_string_decls(place(str(text), v, lit)));   // (each alone, then all together)
    if (cands.size() > 1) {
        std::string t = str(text);
        for (const auto& [v, lit] : cands) t = place(t, v, lit);
        out.push_back(cand_string_decls(t));
    }
    return out;
}

// ---- long arithmetic, the 8087, the frame ------------------------------------------------------------------------

std::string long_helpers(std::string_view text, const std::map<std::string, std::string>& helpers) {
    if (helpers.empty()) return str(text);
    std::vector<std::string> names;
    for (const auto& kv : helpers) names.push_back(re_escape(kv.first));
    const Regex call(R"re(\b()re" + ct::join(names, "|") + R"re()\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re");
    std::set<std::string> unsigned_longs;
    static const Regex field(R"re((\w+(?:->f\d+)*)->f(\d+))re");
    static const Regex before_arg(R"re(\w\($|,$)re");
    static const Regex unsigned_routine(R"re(a?ul)re");
    static const Regex plain_name(R"re(&?\w+)re");
    const std::string source = str(text);
    std::string t = sub_all(call, source, [&](const RegexMatch& m) {
        const std::string& op = helpers.at(str(m[1]));
        std::vector<std::string> args;
        for (const std::string& a : dt::top_commas(m[2])) args.emplace_back(ct::trim(a));
        if (args.size() == 3) {   // (the long operand pushed a word at a time: its words - a field's two - the long there)
            const auto f1 = field.full_match(args[1]), f2 = field.full_match(args[2]);
            if (f1 && f2 && (*f1)[1] == (*f2)[1] && to_int((*f2)[2]) == to_int((*f1)[2]) + 2) args = {args[0], "*(long *)&" + args[1]};
        }
        if (args.size() != 2) return str(m.whole());
        if (unsigned_routine.match(m[1]) && plain_name.full_match(args[0])) unsigned_longs.insert(std::string(strip_left(args[0], "&")));   // (an unsigned one's operand: an unsigned long)
        if (op.ends_with("=") && op != "==" && op != "<=" && op != ">=" && op != "!=") {
            if (!args[0].starts_with("&")) return str(m.whole());
            return "(" + args[0].substr(1) + " " + op + " " + args[1] + ")";
        }
        const std::string v = "(" + args[0] + " " + op + " " + args[1] + ")";
        std::string before(ct::rtrim(sv(source).substr(0, m.start()))), after(ct::ltrim(sv(source).substr(m.end())));
        if (before_arg.search(before) && !after.empty() && (after[0] == ')' || after[0] == ',')) return "(int)" + v;   // (an argument pushed as one word: the long's low word - an int)
        return v;
    });
    for (const std::string& x : unsigned_longs) {
        const Regex decl(R"re((?m)^([ \t]*)(register )?long )re" + re_escape(x) + ";$");
        t = decl.replace_all(t, "$1$2unsigned long " + x + ";");
    }
    static const Regex bare_statement(R"re((?m)^([ \t]*)\((.+ [*/%<>+-]{1,2}= .+)\);$)re");   // (a statement of its own: no parens)
    t = bare_statement.replace_all(t, "$1$2;");
    // (the long it leaves in dx:ax tested at once - `ax = (ax | dx)`: the assignment's value)
    static const Regex tested(R"re((?m)^([ \t]*)(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n\s*ax = \(ax \| dx\);$)re");
    std::string t2 = tested.replace_all(t, "$1ax = ($2);");
    static const Regex do_test(R"re((?m)^[ \t]*(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n(\s*)\} while \(\(\(ax \| dx\) != 0\)\);$)re");   // (a do's test of it)
    t2 = do_test.replace_all(t2, "$2} while ((($1) != 0));");
    return t2 != t ? shapes::fold_registers(t2) : t;
}

std::string fp_conversions(std::string_view text, const std::vector<std::string>& routines, State& state) {
    if (routines.empty()) return str(text);
    std::vector<std::string> names;
    for (const std::string& r : routines) names.push_back(re_escape(r));
    const std::string rx = ct::join(names, "|");
    const std::string arg = R"re(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*))re";
    const Regex long_store(R"re((?m)^([ \t]*)(\w+)->f0 = (?:)re" + rx + R"re()\()re" + arg + R"re(\);\n\s*\2->f2 = dx;$)re");
    std::string t = sub_all(long_store, text, [&](const RegexMatch& m) {
        state.fp.types["*" + str(m[2])] = "long";
        return str(m[1]) + "*" + str(m[2]) + " = " + str(m[3]) + ";";
    });
    const Regex conversion(R"re((?m)^([ \t]*)(.+?) = (?:)re" + rx + R"re()\()re" + arg + R"re(\);$)re");
    return conversion.replace_all(t, "$1$2 = $3;");
}

std::string fp_types(std::string_view text, const FpInfo& fp) {
    if (fp.types.empty() && !fp.ret.has_value()) return str(text);
    if (fp.types.empty() && fp.ret->empty()) return str(text);
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    std::string pre, fn = split.head, body = split.body;
    static const Regex function_start(R"re((?m)^(?:\w+\s*\n)?\w+\()re");   // (the declarations before the function: data's)
    if (const auto m = function_start.search(split.head)) {
        pre = split.head.substr(0, m->start());
        fn = split.head.substr(m->start());
    }
    static const Regex pointee(R"re(\*\(?(\w+)\)?)re");
    static const Regex declaration_line(R"re(\s*(?:register\s+)?(?:int|char|long|unsigned|short|struct|float|double)\b)re");
    for (const auto& [nm, ty] : fp.types) {
        std::string x = nm;
        bool ptr = false;
        if (const auto m = pointee.full_match(nm)) {
            x = str((*m)[1]);
            ptr = true;
        }
        if (x.empty() || !std::ranges::all_of(x, is_word)) continue;
        const std::string declared = ty + " " + (ptr ? "*" : "") + x;
        if (const auto f2 = forms::redeclare(fn, x, declared)) {
            fn = *f2;
            continue;
        }
        Lines lines = split_nl(body);
        bool stopped = false;   // (a local: among the declarations at the top)
        for (std::string& l : lines) {
            if (!blank(l) && !declaration_line.match(l)) {
                stopped = true;
                break;
            }
            if (blank(l)) continue;
            if (const auto l2 = forms::redeclare(l, x, declared)) {
                l = *l2;
                stopped = true;
                break;
            }
        }
        if (stopped) body = join_nl(lines);
        if (x.starts_with("g_")) {
            const Regex extern_int(R"re((?m)^extern int ((?:\w+, )*))re" + re_escape(x) + R"re(((?:, \w+)*);$)re");
            pre = sub_all(extern_int, pre, [&](const RegexMatch& d) {
                std::vector<std::string> rest;
                for (const sv part : {d.group(1), d.group(2)}) {
                    std::string s(part);
                    while (s.starts_with(",") || s.starts_with(" ")) s.erase(0, 1);
                    while (s.ends_with(",") || s.ends_with(" ")) s.pop_back();
                    if (!s.empty()) rest.push_back(s);
                }
                const std::string joined = ct::join(rest, ", ");
                return (joined.empty() ? std::string() : "extern int " + joined + ";\n") + "extern " + declared + ";";
            });
        }
    }
    if (fp.ret && !fp.ret->empty()) {
        static const Regex name_start(R"re((?m)^\w+\()re");
        static const Regex double_line(R"re((?m)^double\s*$)re");
        if (const auto m = name_start.search(fn); m && !double_line.search(fn)) fn = fn.substr(0, m->start()) + *fp.ret + "\n" + fn.substr(m->start());
    }
    return pre + fn + split.brace + body;
}

std::string fill_frame(std::string_view text, std::uint32_t frame_size, bool registers_count, FillerName name) {
    if (frame_size == 0 || text.find('{') == npos) return str(text);
    const dt::BodySplit split = dt::split_body(text);
    std::int64_t size = 0;
    Lines lines = split_nl(split.body);
    std::size_t last = 0;
    std::vector<std::pair<std::size_t, std::int64_t>> gaps;   // (line, bytes): a filler before a local vN the declarations above would put nearer bp than N
    static const Regex declaration(R"re(\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short|struct\s+\w+))\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*)re");
    static const Regex local_v(R"re(v(\d+))re");
    for (std::size_t k = 1; k < lines.size(); ++k) {
        const auto m = declaration.full_match(lines[k]);
        if (!m) {
            if (!blank(lines[k])) break;
            continue;
        }
        last = k;
        if (m->has(1) && !(*m)[1].empty()) {
            size += registers_count ? 2 : 0;
            continue;
        }
        const std::string type(ct::trim((*m)[2]));
        const bool star = !(*m)[3].empty();
        if (type.starts_with("struct") && !star) return str(text);   // (a struct's size the text does not say)
        const std::string last_word(type.substr(type.find_last_of(" \t") == npos ? 0 : type.find_last_of(" \t") + 1));
        const std::int64_t one = star ? 2 : static_cast<std::int64_t>(dt::size_of_type(last_word, 2));
        const bool counted = m->has(5) && !(*m)[5].empty();
        const std::int64_t n = one * (counted ? to_int((*m)[5]) : 1);
        if (const auto v = local_v.full_match((*m)[4]); v && !star && !counted && last_word == "long" && size + n < to_int((*v)[1])) {
            gaps.emplace_back(k, to_int((*v)[1]) - size - n);   // (a long the pair made of vN and the word above it: where vN was)
            size = to_int((*v)[1]) - n;
        }
        size += n + (n & 1);
    }
    for (auto g = gaps.rbegin(); g != gaps.rend(); ++g) {
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(g->first), std::format("    char _pg{}[{}];", g->first, g->second));
        ++last;
    }
    const std::int64_t fs = frame_size;
    if (!gaps.empty() && fs > size) {
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(last + 1), std::format("    char _pf[{}];", fs - size));
        return split.head + split.brace + join_nl(lines);
    }
    static const Regex autos(R"re((?m)^\s+(?:int|char|long|short)\s+v\d+)re");
    if (fs <= size || autos.search(split.body)) return gaps.empty() ? str(text) : split.head + split.brace + join_nl(lines);   // (the model's autos: the frame model lays those out)
    lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(last + 1), std::format("    char {}[{}];", name == FillerName::ByPlace ? std::format("_p{}", size) : std::string("_pf"), fs - size));
    return split.head + split.brace + join_nl(lines);
}

// ---- the stage -------------------------------------------------------------------------------------------------

namespace {

// `&p[N]` and `&p->fN` - the place, p + N (lea).
std::string cand_places_as_sums(std::string text) {
    static const Regex index(R"re(&(\w+)\[(-?)(\d+)\])re");
    text = sub_all(index, text, [](const RegexMatch& m) { return "(" + str(m[1]) + (m[2].empty() ? " + " : " - ") + str(m[3]) + ")"; });
    static const Regex field(R"re(&(\w+)->f(\d+)\b)re");
    return field.replace_all(text, "($1 + $2)");
}

// The text of a pass that may have nothing to say.
void cand_push(std::vector<std::string>& variants, std::optional<std::string> v) {
    if (v && !v->empty()) variants.push_back(std::move(*v));
}

}  // namespace

std::vector<Candidate> candidates(const Settings& settings, std::string_view source_text, std::uint32_t at, const std::map<std::uint32_t, std::string>& names,
                                  const std::vector<std::string>& regs, const RegisterParams& regparam, const ByteFields& widths,
                                  std::uint32_t frame_size, State& state, text::ProjectCaches& caches) {
    using namespace shapes;
    auto& facts = state.facts;
    const bool framed = settings.frame_order.has_value();   // (MS C 3.00 gives some register locals a slot in the frame)
    const forms::FlatReturns flat = [](std::string_view t) { return shapes::flat_returns(t); };

    std::vector<std::string> base{str(source_text)};
    cand_push(base, void_variant(source_text));
    std::vector<std::string> bases = base;
    for (const std::string& t : base) cand_push(bases, buffers(t));

    std::vector<std::vector<Candidate>> out;
    for (const std::string& base_text : bases) {
        state.struct_pointers.clear();
        facts.word_pointers.clear();
        facts.arrays.clear();
        facts.struct_arrays.clear();
        facts.unsigned_sites.clear();
        facts.unsigned_globals.clear();
        facts.byte_globals = widths.globals;
        facts.streams.clear();
        facts.include.reset();
        facts.include2.reset();
        static const Regex shared_tail(R"re(\btarg(\d+)\b)re");
        std::string t = shared_tail.replace_all(base_text, "t9$1");   // (the model's temporaries of a shared tail: temporaries)
        t = fall_gotos(t);
        t = forms::store_reloads(t);
        t = long_returns(t);
        t = long_pairs(t, state);
        t = forms::char_params(t);
        t = forms::char_stores(t);
        t = forms::case_falls(t);
        t = forms::table_macros(settings.kit, caches, facts, t, settings.table_macros);
        t = zero_extends(high_zero(copy_props(t), facts), facts);
        t = cand_places_as_sums(t);   // (lea: &p[N], &p->fN - the place, p + N)
        {
            const std::optional<std::string> tails = goto_tails(t);
            if (tails && !tails->empty()) t = *tails;
        }
        t = fall_gotos(t);
        t = fall_dups(t);
        t = loop_registers(t);
        t = first_breaks(t);
        t = continue_loops(t);
        t = tested_zero(t);
        t = empty_loops(t);
        t = forms::loop_gotos(t);
        t = range_switches(t, facts);
        t = forms::bool_trees(t);
        t = step_tests(t);
        t = logical_values(t);
        t = conditional_values(t);
        t = push_returns(t);
        t = branch_returns(t);
        t = chain_returns(t);
        t = compare_values(t);
        t = widen_bytes(t);
        t = post_increments(t);
        t = store_loops(t);
        t = fold_registers(stored_calls(t));
        t = stored_calls(fold_registers(push_returns(t)));   // (returns again, as the passes left the arms)
        t = lvalue_choices(t);
        t = selector_reads(t);
        t = post_steps(t);
        t = copy_tests(t);
        t = test_first_loops(t);
        t = pointer_index(t, facts);
        t = forms::word_arrays(t, facts);
        t = register_chains(t);
        t = forms::scaled_index(t);
        t = stored_reads(t);
        t = long_tests(t, facts);
        t = two_case_switches(t);
        t = loop_collapse(t);
        t = switch_ifs(t);
        t = ternary_joins(t);
        t = store_joins(t);
        t = conditional_values(t);
        t = fold_registers(stored_values(t));   // (the ?: values again, the elements folded)
        t = switch_follow(t);
        t = self_updates(t);
        t = forms::guard_gotos(t);
        t = forms::merge_conditions(t);
        t = forms::shared_arms(t);
        t = empty_loops(t);
        t = sunk_pairs(t, facts);
        t = sunk_tails(t);
        t = goto_returns(t);
        t = compare_values(t);
        t = temporaries(t);
        t = push_returns(t);
        t = fold_registers(stored_tests(t));
        std::set<Site> wd = widths.sites;   // (a parameter kept in a register: the bytes read through that register its own)
        for (const auto& [reg, index] : regparam) {
            if (index < 0 || index >= 26) continue;
            for (const Site& s : widths.sites)
                if ((s.pointer == reg || s.pointer == reg + "*") && !widths.sites.contains(Site{reg + "w", s.offset})) wd.insert(Site{std::string(1, static_cast<char>('a' + index)), s.offset});
        }
        t = long_pairs(t, state);
        t = forms::shapes(t);
        t = register_variables(t, regs, regparam);
        t = local_pointers(t);
        t = forms::pointer_flow(t);
        t = structs(t, wd, state);
        static const Regex given_itself(R"re((?m)^[ \t]*(\w+) = \1;\n)re");
        t = given_itself.replace_all(t, "");   // (a register variable given itself: nothing)
        t = long_pairs(t, state);
        t = long_ladders(t);
        t = forms::register_values(t);
        t = lvalue_choices(t);
        t = switch_ifs(t);   // (the longs again, as the passes left them)
        static const Regex frame_place(R"re(\bbp - \d+\b)re");
        if (frame_place.search(t)) {   // (a frame place the passes left as bp - N: the local)
            if (const auto b = buffers(t)) t = *b;
            static const Regex place(R"re(\bbp - (\d+)\b)re");
            static const Regex char_array(R"re((?m)^[ \t]*char v(\d+)\[)re");
            static const Regex declared(R"re((?m)^[ \t]*\w[\w ]*\bv(\d+);)re");
            std::set<std::string> arrays, locals;
            char_array.for_each_match(t, [&](const RegexMatch& m) {
                arrays.insert(str(m[1]));
                return true;
            });
            declared.for_each_match(t, [&](const RegexMatch& m) {
                locals.insert(str(m[1]));
                return true;
            });
            t = sub_all(place, t, [&](const RegexMatch& m) {
                const std::string n = str(m[1]);
                if (arrays.contains(n)) return "v" + n;
                if (locals.contains(n)) return "(char *)&v" + n;
                return str(m.whole());
            });
        }
        t = frame_chars(forms::fn_pointers(t), state);
        const std::string t0 = t;
        t = fill_frame(t0, frame_size, framed);
        const std::string t_by_place = fill_frame(t0, frame_size, framed, FillerName::ByPlace);   // (the filler named by its place: the frame's order lays it out - a variant)

        std::vector<std::string> variants{t};
        if (t_by_place != t) variants.push_back(t_by_place);
        cand_push(variants, forms::store_increments(t));
        cand_push(variants, forms::pre_steps(t));
        cand_push(variants, forms::pre_increments(t, facts));
        cand_push(variants, forms::assign_in_tests(t));
        cand_push(variants, forms::and_returns(t));
        cand_push(variants, forms::inner_registers(t));
        cand_push(variants, for_loops(t));
        cand_push(variants, forms::register_temps(t));
        cand_push(variants, forms::compound_assigns(t));
        cand_push(variants, forms::return_sources(t));
        cand_push(variants, forms::assign_in_args(t));
        cand_push(variants, forms::split_ternaries(t));
        cand_push(variants, forms::default_last(t));
        cand_push(variants, forms::push_tails(t));
        cand_push(variants, forms::high_words(t));
        cand_push(variants, forms::unmasked(t));
        cand_push(variants, forms::no_default(t));
        cand_push(variants, forms::unsigned_registers(t));
        cand_push(variants, forms::empty_cases_first(t));
        cand_push(variants, register_slots(t, framed));
        cand_push(variants, forms::register_copies(t, facts));
        cand_push(variants, forms::lead_filler(t));
        cand_push(variants, forms::scaled_places(t, facts));
        cand_push(variants, forms::shared_tails(t));
        cand_push(variants, forms::guard_returns(t));
        cand_push(variants, forms::default_after(t));
        cand_push(variants, forms::frame_more(t));
        cand_push(variants, loop_tests(t));
        cand_push(variants, forms::switch_breaks(t));
        cand_push(variants, forms::register_reads(t));
        cand_push(variants, forms::compare_results(t));
        cand_push(variants, forms::or_loops(t));
        cand_push(variants, loop_guards(t));
        cand_push(variants, const_tests(t));
        cand_push(variants, shapes::flat_returns(t));
        cand_push(variants, return_cases_first(t));
        cand_push(variants, registers_last(t, framed));
        cand_push(variants, forms::guard_shared(t, flat));
        cand_push(variants, forms::guard_flat(t, flat));
        cand_push(variants, dt::changed([](const std::string& x) { return common_tails(x); }, t));
        cand_push(variants, dt::changed([](const std::string& x) { return byte_heads(x); }, t));
        cand_push(variants, dt::changed([](const std::string& x) { return word_offsets(x); }, t));
        cand_push(variants, chain_tails(t));
        cand_push(variants, heads_variant(t));
        cand_push(variants, shared_exit(t));
        cand_push(variants, call_pairs(t));
        cand_push(variants, label_sinks(t));
        cand_push(variants, assign_loops(t));
        cand_push(variants, char_widens(t));
        cand_push(variants, dt::changed([](const std::string& x) { return tail_continues(x); }, t));
        for (std::string& v : case_guards(t))
            if (!v.empty()) variants.push_back(std::move(v));
        {
            std::optional<std::string> both = forms::pre_steps(t);
            if (both) both = forms::pre_increments(*both, facts);
            cand_push(variants, both);
        }
        {
            const std::vector<std::string> first(variants.begin(), variants.begin() + static_cast<std::ptrdiff_t>(std::min(variants.size(), kCharParamBases)));
            for (const std::string& v : first) {   // (a value given twice: chained)
                if (const auto c = forms::chain_assigns(v); c && !c->empty()) variants.insert(variants.begin() + static_cast<std::ptrdiff_t>(cand_index_of(variants, v)) + 1, *c);
            }
        }
        for (const std::string& v : std::vector<std::string>(variants))   // (a pointer register variable through which words are read: int *, the compiler decides)
            for (std::string& w : forms::pointer_widths(v)) variants.push_back(std::move(w));
        static const Regex bytes_compared(R"re(& 0xff\)|\b[a-z] [<>=!]=? (?:3[2-9]|[4-9]\d|1[01]\d|12[0-6])\b)re");   // (a byte compared, or a char: a char param)
        if (bytes_compared.search(t))
            for (std::string& v : forms::char_param_variants(t)) variants.push_back(std::move(v));
        if (settings.frame_order) {
            const forms::FrameRule& rule = *settings.frame_order;
            for (const std::string& v : std::vector<std::string>(variants)) {   // (the names the frame's order asks for)
                const std::optional<std::string> fn = forms::frame_names(v, rule, forms::FrameOptions{false, framed});
                if (fn) variants.insert(variants.begin() + static_cast<std::ptrdiff_t>(cand_index_of(variants, v)) + 1, *fn);
                if (framed) {   // (the register locals' slots last)
                    const std::optional<std::string> fl = forms::frame_names(v, rule, forms::FrameOptions{true, framed});
                    if (fl && fl != fn) variants.push_back(*fl);
                }
            }
        }
        {
            const std::vector<std::string> first(variants.begin(), variants.begin() + static_cast<std::ptrdiff_t>(std::min(variants.size(), kAddressBases)));
            for (const std::string& v : first) {   // (a number that is a place in the data)
                for (forms::AddressVariant& av : forms::address_variants(v, kDataPlaces)) {
                    facts.arrays.emplace(av.name, "char");
                    variants.push_back(av.text);
                    std::vector<std::string> widened = forms::pointer_widths(av.text);   // (and its pointer stepping words through it)
                    if (!widened.empty()) variants.push_back(std::move(widened.front()));
                }
            }
        }
        std::vector<Candidate> loc;
        for (const std::string& v : variants) loc.push_back(localize(settings, forms::word_constants(v), at, names, state, caches));
        static const Regex compared(R"re([<>=]=?\s*-?\d)re");
        const std::vector<Candidate> head(loc.begin(), loc.begin() + static_cast<std::ptrdiff_t>(std::min(loc.size(), kUnsignedGlobalBases)));
        for (const Candidate& c : head) {   // (a global compared: unsigned, a variant)
            if (!c.registers_left.empty() || !compared.search(c.source)) continue;
            for (std::string& u : forms::unsigned_globals(c.source)) loc.push_back(Candidate{std::move(u), c.registers_left});
        }
        out.push_back(std::move(loc));
    }
    // (the bases' readings taken in turn - each base's first, then each one's second, .. - so that no base's many
    // variants keep another's first readings from the compiler)
    std::vector<Candidate> merged;
    std::size_t longest = 0;
    for (const auto& group : out) longest = std::max(longest, group.size());
    for (std::size_t i = 0; i < longest; ++i)
        for (auto& group : out)
            if (i < group.size()) merged.push_back(std::move(group[i]));
    return merged;
}

}  // namespace cb::exact::draft::candidates
