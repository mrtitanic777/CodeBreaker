// codebreaker/port/asm16.cpp -- typed C++ port of codebreaker.port.asm16 (see asm16.hpp).
//
// asm16.py -- a program's own 16-bit assembly language, made assembly language for a 64-bit machine.
//
// The routines a program wrote in assembly language stay in assembly language: every instruction, label, loop and string
// operation as it was, the arithmetic on values in the 16 and 8 bits it had. What a 64-bit machine changes is changed and
// nothing else:
//
//   pointers         a register that holds a pointer at that place (it was loaded from a pointer of the C program, is an
//                    address, or comes from one) is used whole (rsi for si); one that holds a value keeps its 16 or 8 bits;
//                    a value used as an index is widened into a scratch register (r11) first
//   structures       [bx+14] is a member of the C program's structure bx points to: its place in the 64-bit layout
//   segments         ds, es and their loads go (one flat memory); les/lds are loads of a pointer; far is near
//   calls            inside the module the old way, on the stack, each slot 8 bytes; a routine the C program calls gets an
//                    entry of the C of today (its arguments in rcx, rdx, r8, r9) that pushes them as the old caller did,
//                    cdecl or pascal, and keeps the registers the C of today keeps
//   DOS              INT 21h is a call of the platform layer (dos_int21), the registers in a block
//
// The kinds of the registers are followed through each routine (a pass to a fixed point over its jumps and calls); the
// C program's types come from its headers - the old layout of each structure (16-bit pointers, far ones 4 bytes) from
// the source as it was, the new one as the port lays it out.

#include "codebreaker/port/asm16.hpp"

#include <algorithm>

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/port/pre_ansi.hpp"
#include "codebreaker/translate/backends/asm.hpp"
#include "codebreaker/translate/l2.hpp"


namespace codebreaker::port::asm16 {

namespace asm_ = codebreaker::translate::backends::asm_;
namespace l2 = codebreaker::translate::l2;
using py::str;

namespace {

str S(const char* s) { return str(s); }

// the Python's "%..." % args
template <class... A> str F(const char* f, const A&... a) { return py::percent(str(f), a...); }

template <class... A> str Fv(const char* f, const A&... a) { return F(f, a...); }

// str.split(): the words of a text
py::list<str> words(const str& s) { return py::split(s); }

// the last word of an operand text (`l.args[0].split()[-1]`): IndexError when there is none
str last_word(const str& s) {
    py::list<str> w = words(s);
    if (w.empty()) throw py::IndexError("list index out of range");
    return w[-1];
}

// s.split(None, 1): at most two parts, the second as it is after the first run of spaces
py::list<str> split_ws1(const str& s) {
    const std::u32string& u = s.u32();
    size_t i = 0, n = u.size();
    py::list<str> out;
    while (i < n && str::space_(u[i])) ++i;
    if (i >= n) return out;
    size_t j = i;
    while (j < n && !str::space_(u[j])) ++j;
    out.append(str(u.substr(i, j - i)));
    size_t k = j;
    while (k < n && str::space_(u[k])) ++k;
    if (k < n) out.append(str(u.substr(k)));
    return out;
}

bool in_(const str& x, const py::list<str>& l) { return l.contains(x); }

const std::vector<str>& v16() {
    static const std::vector<str> v = {S("ax"), S("bx"), S("cx"), S("dx"), S("si"), S("di"), S("bp"), S("sp")};
    return v;
}

bool is_r16(const str& r) { return R16.contains(r); }
bool is_r8(const str& r) { return R8.contains(r); }

str lstrip_chars(const str& t, const char* chars) { return t.lstrip(str(chars)); }

str ptr_word(int64_t size) {
    switch (size) {
        case 1: return S("byte ptr ");
        case 2: return S("word ptr ");
        case 4: return S("dword ptr ");
        case 8: return S("qword ptr ");
        case 10: return S("tbyte ptr ");
    }
    throw py::KeyError(std::to_string(size));
}

str stars(int64_t n) { return str(std::string(static_cast<size_t>(std::max<int64_t>(n, 0)), '*')); }

// str.expandtabs(8)
str expandtabs8(const str& s) {
    std::u32string o;
    size_t col = 0;
    for (char32_t c : s.u32()) {
        if (c == '\t') {
            size_t n = 8 - col % 8;
            o.append(n, U' ');
            col += n;
        } else {
            o.push_back(c);
            col = (c == '\n' || c == '\r') ? 0 : col + 1;
        }
    }
    return str(o);
}

}  // namespace

const py::list<str> R16 = {S("ax"), S("bx"), S("cx"), S("dx"), S("si"), S("di"), S("bp"), S("sp")};
const py::dict<str, str> R8 = {{S("al"), S("ax")}, {S("ah"), S("ax")}, {S("bl"), S("bx")}, {S("bh"), S("bx")},
                               {S("cl"), S("cx")}, {S("ch"), S("cx")}, {S("dl"), S("dx")}, {S("dh"), S("dx")}};
const py::dict<str, str> R64 = {{S("ax"), S("rax")}, {S("bx"), S("rbx")}, {S("cx"), S("rcx")}, {S("dx"), S("rdx")},
                                {S("si"), S("rsi")}, {S("di"), S("rdi")}, {S("bp"), S("rbp")}, {S("sp"), S("rsp")}};
const py::set<str> SEG = {S("ds"), S("es"), S("ss"), S("cs")};
const py::set<str> JUMPS = {
    S("jmp"), S("je"), S("jne"), S("jz"), S("jnz"), S("jb"), S("jnb"), S("jae"), S("jnae"), S("ja"), S("jna"), S("jbe"),
    S("jnbe"), S("jc"), S("jnc"), S("jl"), S("jnl"), S("jge"), S("jnge"), S("jg"), S("jng"), S("jle"), S("jnle"), S("js"),
    S("jns"), S("jo"), S("jno"), S("jp"), S("jnp"), S("jpe"), S("jpo"), S("loop"), S("loope"), S("loopne"), S("loopz"),
    S("loopnz"), S("jcxz")};
const py::set<str> STRING = {S("lodsb"), S("lodsw"), S("stosb"), S("stosw"), S("movsb"),
                             S("movsw"), S("cmpsb"), S("cmpsw"), S("scasb"), S("scasw")};
const py::dict<str, int64_t> SIZES = {{S("byte"), 1}, {S("word"), 2}, {S("dword"), 4}, {S("qword"), 8}, {S("tbyte"), 10}};
const py::list<str> R16_CODE = {S("ax"), S("cx"), S("dx"), S("bx"), S("sp"), S("bp"), S("si"), S("di")};
const py::list<str> R8_CODE = {S("al"), S("cl"), S("dl"), S("bl"), S("ah"), S("ch"), S("dh"), S("bh")};
const py::set<str> CALL_WORDS = {S("near"), S("far"), S("huge"), S("pascal"), S("cdecl"), S("fortran"),
                                 S("_near"), S("_far"), S("_pascal"), S("_cdecl")};
const K VAL = K(S("val"));
py::dict<str, str> FIRST_MEMBER;

// ---------------------------------------------------------------- reading the source

str strip_comment(const str& l) {
    const std::u32string& u = l.u32();
    char32_t q = 0;
    for (size_t i = 0; i < u.size(); ++i) {
        char32_t c = u[i];
        if (q) {
            if (c == q) q = 0;
        } else if (c == '\'' || c == '"') {
            q = c;
        } else if (c == ';') {
            return str(u.substr(0, i));
        }
    }
    return l;
}

py::list<str> split_ops(const str& s) {
    py::list<str> out;
    int64_t d = 0;
    std::u32string cur;
    char32_t q = 0;
    for (char32_t c : s.u32()) {
        if (q) {
            cur.push_back(c);
            if (c == q) q = 0;
            continue;
        }
        if (c == '\'' || c == '"') q = c;
        else if (c == '[' || c == '(') ++d;
        else if (c == ']' || c == ')') --d;
        if (c == ',' && d == 0) {
            out.append(str(cur).strip());
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!str(cur).strip().empty()) out.append(str(cur).strip());
    return out;
}

py::list<LineP> parse(const str& text) {
    py::list<LineP> out;
    int64_t no = 0;
    for (const str& raw : py::split(text, S("\n"))) {
        ++no;
        LineP l = std::make_shared<Line>(no, raw);
        str s = strip_comment(raw).strip();
        auto m = py::re::match(S(R"(([A-Za-z_$@?][\w$@?]*)\s*:(?!:)\s*(.*)$)"), s);
        if (m && !py::re::match(S(R"((?i)(es|ds|ss|cs)\s*:)"), s)) {
            l->label = m->group(1);
            s = m->group(2).strip();
        }
        if (s.empty()) {
            out.append(l);
            continue;
        }
        py::list<str> w = split_ws1(s);
        if (w.size() > 1) {
            py::list<str> w1 = split_ws1(w[1]);
            static const py::set<str> kinds = {S("SEGMENT"), S("ENDS"), S("GROUP"), S("PROC"), S("ENDP"), S("LABEL"),
                                               S("EQU"), S("DB"), S("DW"), S("DD"), S("DQ"), S("DT"), S("=")};
            if (kinds.contains(w1[0].upper())) {
                l->directive = Directive{w1[0].upper(), w[0], w1.size() > 1 ? w1[1] : S("")};
                out.append(l);
                continue;
            }
        }
        static const py::set<str> first = {S("DB"), S("DW"), S("DD"), S("DQ"), S("DT"), S("PUBLIC"), S("EXTRN"),
                                           S("ASSUME"), S("TITLE"), S("END"), S("ORG"), S("EVEN"), S("ALIGN"),
                                           S("INCLUDE"), S("NAME"), S("PAGE"), S("SUBTTL"), S(".8086"), S(".186"),
                                           S(".286"), S(".8087")};
        if (first.contains(w[0].upper())) {
            l->directive = Directive{w[0].upper(), std::nullopt, w.size() > 1 ? w[1] : S("")};
            out.append(l);
            continue;
        }
        str op = w[0].lower();
        str rest = w.size() > 1 ? w[1] : S("");
        static const py::set<str> prefixes = {S("rep"), S("repe"), S("repz"), S("repne"), S("repnz")};
        if (prefixes.contains(op)) {
            l->prefix = op;
            py::list<str> w2 = split_ws1(rest);
            op = w2[0].lower();       // (IndexError for a prefix alone, as the Python's w2[0])
            rest = w2.size() > 1 ? w2[1] : S("");
        }
        l->op = op;
        l->args = split_ops(rest);
        out.append(l);
    }
    return out;
}

// {label: (segment, offset)} as the program's own assembler placed them (its listing; the assembler runs under
// CodeBreaker's emulator, nothing of it on the host)
Listing listing_offsets(const str& path, const str& assembler, const str& args) {
    namespace dos = codebreaker::exact::emu::dos;
    str w = py::tempfile::mkdtemp(std::nullopt, S("asm16_"));
    str lst;
    try {
        py::shutil::copy(assembler, py::os::path::join(w, S("ASM.EXE")));
        py::file f = py::open_rb(path);
        py::bytes src = f.read().replace(py::bytes(std::string("\r\n")), py::bytes(std::string("\n")))
                            .replace(py::bytes(std::string("\n")), py::bytes(std::string("\r\n")));
        f.close();
        py::file g = py::open(py::os::path::join(w, S("T.ASM")), S("wb"));
        g.write(src);
        g.close();
        dos::run(py::os::path::join(w, S("ASM.EXE")), w, args, py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false,
                 py::bytes(std::string("PATH=C:\\\0", 9)), std::nullopt, std::nullopt, true);
        py::file h = py::open(py::os::path::join(w, S("T.LST")), S("r"), S("latin1"));
        lst = h.read_text().replace(S("\r"), S(""));
        h.close();
    } catch (...) {
        py::shutil::rmtree(w, true);
        throw;
    }
    py::shutil::rmtree(w, true);
    Listing out;
    std::optional<str> seg;
    for (str l : py::split(lst, S("\n"))) {
        l = expandtabs8(l);                                 // (MASM 5's listing is laid out with tabs)
        auto m = py::re::match(S(R"(^ ([0-9A-F]{4})  ((?:[0-9A-F]{2,4}R? ?)*)\s*(.*)$)"), l);
        if (m && seg && l.size() > 32) {
            int64_t nbytes = 0;
            std::string hex;
            for (const str& t : py::split(m->group(2))) {
                str tt = t.rstrip(S("R"));
                nbytes += tt.size() / 2;
                hex += tt.utf8();
            }
            ListLine ll;
            ll.seg = *seg;
            ll.offset = py::int_parse(m->group(1), 16);
            ll.nbytes = nbytes;
            ll.text = py::re::sub(S(R"(\s+)"), S(" "), l.slice(32, l.size())).strip().slice(0, 24);
            std::string raw;
            for (size_t i = 0; i + 1 < hex.size(); i += 2) raw.push_back(static_cast<char>(std::stoi(hex.substr(i, 2), nullptr, 16)));
            ll.data = py::bytes(raw);
            out.lines.append(ll);
        }
        m = py::re::match(S(R"(^ ([0-9A-F]{4}) {2,}.*?\b([A-Za-z_$@?][\w$@?]*)\s+SEGMENT\b)"), l, py::re::I);
        if (m) {
            seg = m->group(2);
            continue;
        }
        if (py::re::match(S(R"((?i)^ [0-9A-F]{4}\s+[A-Za-z_$@?][\w$@?]*\s+ENDS\b)"), l)) {   // (not a wrapped comment's "ends")
            seg = std::nullopt;
            continue;
        }
        m = py::re::match(S(R"(^ ([0-9A-F]{4})\s+(?:[0-9A-F ]+R?\s+)?\s*([A-Za-z_$@?][\w$@?]*)(?::|\s+LABEL\b))"), l, py::re::I);
        if (m && seg) out.labels.set(m->group(2), std::make_tuple(*seg, py::int_parse(m->group(1), 16)));
    }
    return out;
}

// ---------------------------------------------------------------- operands

std::optional<std::tuple<int64_t, str, py::list<str>>> decode_direct(const py::bytes& bb) {
    const std::string& b = bb.raw();
    auto u = [&](size_t i) { return static_cast<int64_t>(static_cast<unsigned char>(b[i])); };
    if (b.size() >= 4 && (u(0) == 0x88 || u(0) == 0x89 || u(0) == 0x8A || u(0) == 0x8B) && (u(1) & 0xC7) == 0x06) {
        str reg = ((u(0) & 1) ? R16_CODE : R8_CODE)[(u(1) >> 3) & 7];
        str mem = F("%s ptr [0%04xh]", (u(0) & 1) ? "word" : "byte", u(2) | u(3) << 8);
        py::list<str> a;
        if (u(0) & 2) { a.append(reg); a.append(mem); } else { a.append(mem); a.append(reg); }
        return std::make_tuple(int64_t(4), S("mov"), a);
    }
    if (b.size() >= 3 && (u(0) == 0xA0 || u(0) == 0xA1 || u(0) == 0xA2 || u(0) == 0xA3)) {
        str reg = (u(0) & 1) ? S("ax") : S("al");
        str mem = F("%s ptr [0%04xh]", (u(0) & 1) ? "word" : "byte", u(1) | u(2) << 8);
        py::list<str> a;
        if (u(0) < 0xA2) { a.append(reg); a.append(mem); } else { a.append(mem); a.append(reg); }
        return std::make_tuple(int64_t(3), S("mov"), a);
    }
    return std::nullopt;
}

// the module's code segments: of class CODE (Lattice's PROG), named ..TEXT, or what ASSUME puts in CS - itself or
// through its group
py::set<str> code_segments(const py::list<LineP>& lines) {
    py::set<str> out;
    py::dict<str, py::list<str>> groups;
    for (const LineP& l : lines) {
        if (!l->directive) continue;
        const Directive& d = *l->directive;
        if (d.kind == "SEGMENT") {
            auto cls = py::re::search(S("'([^']*)'"), d.rest);
            if ((cls && (cls->group(1).upper() == "CODE" || cls->group(1).upper() == "PROG")) ||
                (d.name && d.name->upper().endswith(S("TEXT"))))
                out.add(*d.name);
        } else if (d.kind == "GROUP") {
            py::list<str> g;
            for (const str& x : py::split(strip_comment(d.rest), S(",")))
                if (!x.strip().empty()) g.append(x.strip());
            groups.set(d.name->upper(), g);
        }
    }
    for (const LineP& l : lines) {
        if (!l->directive) continue;
        const Directive& d = *l->directive;
        if (d.kind == "ASSUME") {
            auto m = py::re::search(S(R"((?i)\bcs\s*:\s*([\w$@?]+))"), d.rest);
            if (m) {
                str name = m->group(1);
                py::list<str> names = groups.get(name.upper(), py::list<str>{name});
                for (const str& n : names)
                    if (n != "NOTHING" && n != "nothing") out.add(n);
            }
        }
    }
    return out;
}

std::optional<int64_t> number(const str& s0) {
    str s = s0.strip();
    auto m = py::re::fullmatch(S(R"(([0-9][0-9a-fA-F]*)[hH])"), s);
    if (m) return py::int_parse(m->group(1), 16);
    if (py::re::fullmatch(S(R"(\d+)"), s)) return py::int_parse(s);
    m = py::re::fullmatch(S(R"(0[xX]([0-9a-fA-F]+))"), s);
    if (m) return py::int_parse(m->group(1), 16);
    return std::nullopt;
}

Operand operand(const str& a, const Equates* equ) {
    str s = a.strip();
    str low = s.lower();
    Operand res;
    if (is_r16(low) || is_r8(low) || SEG.contains(low)) {
        res.kind = S("reg");
        res.text = low;
        return res;
    }
    auto m = py::re::fullmatch(S(R"((?i)offset\s+(?:\w+:)?([\w$@?]+)\s*(?:([+-])\s*(\w+))?)"), s);
    if (m) {
        std::optional<str> g3 = m->group_opt(3);
        int64_t d = 0;
        bool none = false;
        if (g3) {
            auto n = number(*g3);
            if (n) d = *n;
            else none = true;                                // (d = None)
        }
        res.kind = S("off");
        res.text = m->group(1);
        auto g2 = m->group_opt(2);
        if (none && g2 && *g2 == "-") throw py::TypeError("bad operand type for unary -: 'NoneType'");
        res.disp = (g2 && *g2 == "-") ? -d : d;
        res.disp_none = none;
        return res;
    }
    std::optional<int64_t> size;
    m = py::re::match(S(R"((?i)(byte|word|dword|qword|tbyte)\s+ptr\s+(.*)$)"), s);
    if (m) {
        size = SIZES.at(m->group(1).lower());
        s = m->group(2).strip();
    }
    std::optional<str> seg;
    m = py::re::match(S(R"((?i)(es|ds|ss|cs)\s*:\s*(.*)$)"), s);
    if (m) {
        seg = m->group(1).lower();
        s = m->group(2).strip();
    }
    if (s.contains(S("[")) || size.has_value() || seg.has_value()) {   // (size 0 cannot be: SIZES are 1..10)
        py::list<str> regs;
        int64_t disp = 0;
        std::optional<str> sym;
        py::list<str> insides;
        for (const str& g : py::re::findall(S(R"(\[([^\]]*)\])"), s)) insides.append(g);
        str inside = py::join(S(" + "), insides);
        str outside = py::re::sub(S(R"(\[[^\]]*\])"), S(" "), s);
        for (const str& part : py::re::findall(S(R"([+-]?\s*[^+\-\s]+)"), (outside + S(" + ") + inside).replace(S(" "), S("")))) {
            int64_t sign = part.startswith(S("-")) ? -1 : 1;
            str t = part.lstrip(S("+-"));
            if (t.empty()) continue;
            if (is_r16(t.lower())) {
                regs.append(t.lower());
            } else if (number(t)) {
                disp += sign * *number(t);
            } else if (equ && equ->map.contains(t) && number(equ->map.at(t))) {
                disp += sign * *number(equ->map.at(t));
            } else {
                if (sym) throw Asm16Error(F("two symbols in %r", a));
                sym = t;
            }
        }
        res.kind = S("mem");
        res.mem = std::make_shared<Mem>(size, seg, sym, regs, disp, a);
        return res;
    }
    if (number(s) || py::re::fullmatch(S("'.'"), s) || py::re::fullmatch(S(R"(-\s*\w+)"), s)) {
        res.kind = S("imm");
        res.text = s;
        return res;
    }
    if (equ && equ->map.contains(s)) {
        res.kind = S("imm");
        res.text = s;
        return res;
    }
    if (equ && !equ->data.empty() && equ->data.contains(s)) {   // (a variable named bare: the memory it is)
        res.kind = S("mem");
        res.mem = std::make_shared<Mem>(equ->data.at(s), std::nullopt, s, py::list<str>(), 0, a);
        return res;
    }
    res.kind = S("sym");
    res.text = s;
    return res;
}

// ---------------------------------------------------------------- the C program's types

CTypes::CTypes(const py::list<str>& port_texts, const py::list<str>& orig_texts) {
    for (const str& t : port_texts) {
        str code = base::ctext::blank(t);
        py::list<str> cl = py::split(code, S("\n"));
        py::list<int64_t> ds = pre_ansi::depths(cl);
        int64_t i = -1;
        for (const str& l : cl) {
            ++i;
            str s = l.strip();
            if (ds[i] || !s.endswith(S(";")) || s.replace(S("(*"), S("")).contains(S("(")) ||
                s.startswith(std::make_tuple("typedef", "#")))
                continue;
            s = py::re::sub(S(R"(=.*;$)"), S(";"), s);
            auto r = pre_ansi::declarators(s);
            if (!r || std::get<0>(*r).empty()) continue;
            for (const auto& nd : std::get<1>(*r)) {
                if (!vars.contains(std::get<0>(nd)))
                    vars.set(std::get<0>(nd),
                             std::make_tuple(py::re::sub(S(R"(\b(extern|static|register|const|volatile)\b)"), S(""),
                                                         std::get<0>(*r)).strip(),
                                             std::get<1>(nd)));
            }
        }
    }
    old = _layouts(orig_texts, 2, 4);
    new_ = _layouts(port_texts, 8, 8);
    for (const str& t : orig_texts) {                       // (globals whose pointers were far: 4 bytes each)
        for (const auto& d : py::re::finditer(
                 S(R"((?m)^[^;(){}#]*\bfar\s*\*[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*(?:=[^;]*)?;)"), base::ctext::blank(t)))
            far_ptrs.add(d.group(1));
    }
}

// (old, new) bytes of an element of a global array (or of the variable) - None when not known
std::optional<std::tuple<int64_t, int64_t>> CTypes::elem_sizes(const str& name) const {
    if (!vars.contains(name)) return std::nullopt;
    const auto& v = vars.at(name);
    const str& base = std::get<0>(v);
    const str& d = std::get<1>(v);
    str b = py::re::sub(S(R"(\s+)"), S(" "), py::re::sub(S(R"(\b(extern|static|register|const|volatile)\b)"), S(""), base)).strip();
    if (d.contains(S("*"))) return std::make_tuple(far_ptrs.contains(name) ? int64_t(4) : int64_t(2), int64_t(8));
    if (b.startswith(std::make_tuple("struct", "union"))) {
        const Layout* o = old.get(b);
        const Layout* n = new_.get(b);
        if (o && n) return std::make_tuple(o->size, n->size);
        return std::nullopt;
    }
    static const py::dict<str, int64_t> sc = {
        {S("char"), 1}, {S("signed char"), 1}, {S("unsigned char"), 1}, {S("int16_t"), 2}, {S("uint16_t"), 2},
        {S("short"), 2}, {S("int32_t"), 4}, {S("uint32_t"), 4}, {S("long"), 4}, {S("int8_t"), 1}, {S("uint8_t"), 1}};
    if (sc.contains(b) && sc.at(b)) return std::make_tuple(sc.at(b), sc.at(b));
    return std::nullopt;
}

namespace {

// the layout of a structure's (or union's) members: a nested struct or union one member of its size
Layout body_layout(const str& body, bool uni, int64_t ptr, int64_t far, Layouts& out, const str& path);

}  // namespace

Layouts CTypes::_layouts(const py::list<str>& texts, int64_t ptr, int64_t far) {
    Layouts out;
    std::vector<std::pair<str, std::pair<str, bool>>> defs;      // (each tag's first definition: its body)
    auto def_has = [&](const str& k) {
        for (const auto& kv : defs) if (kv.first == k) return true;
        return false;
    };
    for (const str& t : texts) {
        str code = base::ctext::blank(t);
        for (const auto& m : py::re::finditer(S(R"(\b(struct|union)\s+(\w+)\s*\{)"), code)) {
            int64_t d = 0, k = m.end() - 1;
            while (k < code.size()) {
                char32_t ch = code.at_(k);
                d += (ch == '{') ? 1 : (ch == '}') ? -1 : 0;
                if (d == 0) break;
                ++k;
            }
            str key = m.group(1) + S(" ") + m.group(2);
            if (!def_has(key)) defs.push_back({key, {code.slice(m.end(), k), m.group(1) == "union"}});
        }
    }
    for (int round = 0; round < 6; ++round) {                // (again until the sizes settle)
        py::dict<str, Layout> before = out.map.copy();
        for (const auto& kv : defs) out.map.set(kv.first, body_layout(kv.second.first, kv.second.second, ptr, far, out, kv.first));
        if (out.map == before) break;
    }
    return out;
}

namespace {

Layout body_layout(const str& body, bool uni, int64_t ptr, int64_t far, Layouts& out, const str& path) {
    std::vector<str> members;
    int64_t d = 0;
    std::u32string cur;
    for (char32_t c : body.u32()) {                          // (the members: ; at the body's own level)
        d += (c == '{') - (c == '}');
        if (c == ';' && d == 0) {
            members.push_back(str(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    Layout lay;
    int64_t off = 0;
    for (const str& s : members) {
        if (s.strip().empty()) continue;
        auto nest = py::re::match(S(R"(\s*(struct|union)\s*\w*\s*\{(.*)\}\s*(.*)$)"), s, py::re::S);
        if (nest) {                                          // (struct { ... } name: its own layout, one member)
            std::vector<str> names;
            for (const str& x : py::split(nest->group(3), S(",")))
                if (!x.strip().empty()) names.push_back(x.strip());
            Layout inner = body_layout(nest->group(2), nest->group(1) == "union", ptr, far, out,
                                       path + S(".") + (names.empty() ? S("?") : names[0]));
            for (auto& x : names) x = py::re::sub(S(R"(\s*\[.*)"), S(""), x);
            for (const str& nm : names) {                    // (its members, for what is inside it)
                out.inner.set(path + S(".") + nm, std::make_tuple(nest->group(1) == "union", inner));
                out.map.setdefault(path + S(".") + nm, inner);
            }
            bool allptr = true;
            for (const auto& kv : inner.members) if (!kv.second.isptr) { allptr = false; break; }
            for (const str& nm : names) {
                int64_t sz = inner.size;
                if (sz > 1 && off % 2) off += 1;
                Member mem;
                mem.off = uni ? 0 : off;
                mem.size = sz;
                mem.type = nest->group(1);
                mem.decl = nm;
                mem.isptr = allptr;
                lay.set(nm, mem);
                off = uni ? std::max(off, sz) : off + sz;
            }
            continue;
        }
        bool isfar = static_cast<bool>(py::re::search(S(R"(\bfar\b)"), s));
        str s2 = py::re::sub(S(R"(\b(far|near|huge)\b)"), S(""), s);
        auto r = pre_ansi::declarators(s2);
        if (!r) continue;
        for (const auto& nd : std::get<1>(*r)) {
            const str& n = std::get<0>(nd);
            const str& dcl = std::get<1>(nd);
            std::optional<int64_t> szo = pre_ansi::size_of(std::get<0>(*r), dcl, n, isfar ? far : ptr, out);
            int64_t sz = szo ? *szo : 2;
            if (sz > 1 && off % 2) off += 1;
            bool isptr = dcl.replace(n, S("")).contains(S("*")) ;
            // (dcl.replace(n, "", 1): the first occurrence only)
            {
                int64_t at = dcl.find(n);
                str rest = at < 0 ? dcl : dcl.slice(0, at) + dcl.slice(at + n.size(), dcl.size());
                isptr = rest.contains(S("*"));
            }
            Member mem;
            mem.off = uni ? 0 : off;
            mem.size = sz;
            mem.type = std::get<0>(*r).strip();
            mem.decl = dcl;
            mem.isptr = isptr;
            lay.set(n, mem);
            off = uni ? std::max(off, sz) : off + sz;
        }
    }
    lay.size = off + (off > 1 ? off % 2 : 0);
    return lay;
}

}  // namespace

// (base type, declarator) of a C global
std::optional<std::tuple<str, str>> CTypes::var(const str& name) const {
    if (!vars.contains(name)) return std::nullopt;
    return vars.at(name);
}

// the type a pointer variable points to ('struct fcb', 'char') - None when it is no pointer
std::optional<str> CTypes::pointee(const str& name) const {
    if (!vars.contains(name)) return std::nullopt;
    const auto& v = vars.at(name);
    if (std::get<1>(v).contains(S("*"))) return py::re::sub(S(R"(\s+)"), S(" "), std::get<0>(v)).strip();
    return std::nullopt;
}

bool CTypes::is_pointer(const str& name) const {
    return vars.contains(name) && std::get<1>(vars.at(name)).contains(S("*"));
}

namespace {

// str.replace(name, "", 1).count("*")
int64_t stars_after(const str& decl, const str& name) {
    int64_t at = decl.find(name);
    str rest = at < 0 ? decl : decl.slice(0, at) + decl.slice(at + name.size(), decl.size());
    return rest.count(S("*"));
}

}  // namespace

// what the address of the member that begins at an old offset points to - None
std::optional<str> CTypes::member_type(const str& structure, int64_t off_old) const {
    const Layout* o = old.get(structure);
    if (!o) return std::nullopt;
    for (const auto& kv : o->members) {
        const str& name = kv.first;
        const Member& v = kv.second;
        if (v.off != off_old) continue;
        str sub = structure + S(".") + name;
        if (old.inner.contains(sub)) return sub;
        int64_t st = stars_after(v.decl, name);
        return v.type + (st ? S(" ") + stars(st) : S(""));
    }
    return std::nullopt;
}

namespace {

std::optional<Field> field_in(const CTypes& ct, const str& path, const Layout& o, const Layout& n, int64_t off_old) {
    for (int ex = 1; ex >= 0; --ex) {                        // (a member that begins there, else one around it)
        bool exact = ex == 1;
        for (const auto& kv : o.members) {
            const str& name = kv.first;
            const Member& v = kv.second;
            int64_t fo = v.off, fs = v.size;
            const Member* nm = n.find(name);
            str sub = path + S(".") + name;
            if (!(fo <= off_old && off_old < fo + fs && nm) || (exact && fo != off_old && !ct.old.inner.contains(sub)))
                continue;
            if (ct.old.inner.contains(sub) && ct.new_.inner.contains(sub)) {   // (a structure or union inside: within it)
                const auto& oi = ct.old.inner.at(sub);
                const auto& ni = ct.new_.inner.at(sub);
                bool uo = std::get<0>(oi);
                const Layout& io = std::get<1>(oi);
                const Layout& inn = std::get<1>(ni);
                if (uo) {                                    // (a union: its views, the one with a member there)
                    for (const auto& vkv : io.members) {
                        const str& vname = vkv.first;
                        const Member& vv = vkv.second;
                        const Member* nv = inn.find(vname);
                        if (!nv) continue;
                        str vsub = sub + S(".") + vname;
                        if (ct.old.inner.contains(vsub) && ct.new_.inner.contains(vsub)) {
                            auto f = field_in(ct, vsub, std::get<1>(ct.old.inner.at(vsub)), std::get<1>(ct.new_.inner.at(vsub)),
                                              off_old - fo - vv.off);
                            if (f && (std::get<1>(*f) == off_old - fo - vv.off || !exact))
                                return Field{std::get<0>(*f), fo + vv.off + std::get<1>(*f), std::get<2>(*f),
                                             nm->off + nv->off + std::get<3>(*f), std::get<4>(*f), std::get<5>(*f),
                                             std::get<6>(*f)};
                        }
                    }
                    continue;
                }
                auto f = field_in(ct, sub, io, inn, off_old - fo);
                if (f)
                    return Field{std::get<0>(*f), fo + std::get<1>(*f), std::get<2>(*f), nm->off + std::get<3>(*f),
                                 std::get<4>(*f), std::get<5>(*f), std::get<6>(*f)};
                continue;
            }
            int64_t st = stars_after(v.decl, name);
            str to = v.type + (st > 1 ? S(" ") + stars(st - 1) : S(""));
            return Field{name, fo, fs, nm->off, nm->size, v.isptr, to};
        }
    }
    return std::nullopt;
}

}  // namespace

// the member of a structure at an old offset: (name, old offset, old size, new offset, new size, is pointer, its
// type) - None when not known
std::optional<Field> CTypes::field(const str& structure, int64_t off_old) const {
    const Layout* o = old.get(structure);
    const Layout* n = new_.get(structure);
    if (!o || !n) return std::nullopt;
    return field_in(*this, structure, *o, *n, off_old);
}

// ---------------------------------------------------------------- kinds

const py::dict<str, CSig> RUNTIME_C = [] {
    py::dict<str, CSig> d;
    auto sig = [](bool pascal, int64_t asz, const char* akind, const char* ret) {
        CSig s;
        s.pascal = pascal;
        s.args.append(std::make_tuple(asz, str(akind)));
        s.ret = str(ret);
        return s;
    };
    // routines of the old C library the assembly calls: today's C library's, as each was called
    d.set(S("__ffree"), sig(false, 4, "ptr", "void"));
    d.set(S("__fmalloc"), sig(false, 2, "val", "ptr"));
    d.set(S("_free"), sig(false, 2, "ptr", "void"));
    d.set(S("_malloc"), sig(false, 2, "val", "ptr"));
    return d;
}();

// {C name: (pascal, [(the old size of each argument, val|long|ptr|double)], what it returns)} of the functions a C
// program defines - the old way (the arguments' types declared after the head), the macros made of calling words
// (#define NEAR near pascal) as they were
py::dict<str, CSig> c_functions(const py::list<str>& texts) {
    py::dict<str, str> macros;
    for (const str& t : texts) {
        for (const auto& m : py::re::finditer(S(R"((?m)^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$)"), t)) {
            bool sub = true;
            for (const str& w : py::split(m.group(2))) if (!CALL_WORDS.contains(w)) { sub = false; break; }
            if (sub) macros.set(m.group(1), m.group(2));
        }
    }
    py::dict<str, CSig> out;
    for (const str& t : texts) {
        str code = base::ctext::blank(t);
        for (const auto& kv : macros.items())
            code = py::re::sub(S(R"(\b)") + std::get<0>(kv) + S(R"(\b)"), std::get<1>(kv), code);
        py::list<str> lines = py::split(code, S("\n"));
        py::list<int64_t> ds = pre_ansi::depths(lines);
        for (int64_t i = 0; i < lines.size(); ++i) {
            const str& ln = lines[i];
            auto m = py::re::match(S(R"(^([\w\s*]*?)\b([A-Za-z_]\w*)\s*\(([\w\s,]*)\)\s*$)"), ln);
            if (!m || ds[i] != 0 || pre_ansi::KEYWORDS.contains(m->group(2))) continue;
            int64_t j = i + 1;
            py::dict<str, std::tuple<str, str>> decls;
            while (j < lines.size() && !lines[j].strip().startswith(S("{"))) {
                str d = lines[j].strip();
                if (!d.endswith(S(";"))) break;
                auto base = py::re::match(
                    S(R"(((?:\s*\b(?:register|unsigned|signed|long|short|int|char|float|double|struct\s+\w+|union\s+\w+|far|near|huge)\b)+))"),
                    d);
                if (base) {
                    for (const str& dcl : pre_ansi::split_top(d.slice(base->end(), d.size()).rstrip(S(";")))) {
                        auto n = py::re::search(S(R"(\w+)"), dcl);
                        if (n) decls.set(n->group(0), std::make_tuple(base->group(1), dcl));
                    }
                }
                ++j;
            }
            if (j >= lines.size() || !lines[j].strip().startswith(S("{"))) continue;
            str ret = m->group(1);
            py::set<str> words_ = py::set<str>::from(py::split(ret));
            py::list<std::tuple<int64_t, str>> args;
            bool bad = false;
            for (const str& a0 : py::split(m->group(3), S(","))) {
                str a = a0.strip();
                if (a.empty()) continue;
                std::tuple<str, str> bd = decls.get(a, std::make_tuple(S("int"), a));
                const str& base = std::get<0>(bd);
                const str& dcl = std::get<1>(bd);
                if (dcl.contains(S("*")) || dcl.contains(S("["))) {
                    int64_t at = dcl.find(a);
                    str before = at < 0 ? dcl : dcl.slice(0, at);
                    args.append(std::make_tuple(py::re::search(S(R"(\b(far|huge)\b)"), base + before) ? int64_t(4) : int64_t(2), S("ptr")));
                } else if (base.contains(S("double")) || base.contains(S("float"))) {
                    args.append(std::make_tuple(int64_t(8), S("double")));
                } else if (base.contains(S("long"))) {
                    args.append(std::make_tuple(int64_t(4), S("long")));
                } else if (py::re::search(S(R"(\b(struct|union)\b)"), base)) {
                    bad = true;
                    break;
                } else {
                    args.append(std::make_tuple(int64_t(2), S("val")));
                }
            }
            if (bad) continue;
            str rk = ret.contains(S("*")) ? S("ptr")
                     : words_.contains(S("long")) ? S("long")
                     : (words_.contains(S("void")) && !ret.contains(S("*"))) ? S("void") : S("val");
            std::optional<str> to;                           // (what the address it gives points to)
            if (rk == "ptr") {
                py::list<str> ws;
                int64_t star = ret.find(S("*"));
                str head = star < 0 ? ret : ret.slice(0, star);
                for (const str& w : py::re::findall(S(R"(\w+)"), head))
                    if (!CALL_WORDS.contains(w) && w != "static" && w != "extern" && w != "register") ws.append(w);
                str base = py::join(S(" "), ws);
                int64_t st = ret.count(S("*"));
                if (!base.empty()) to = base + (st > 1 ? S(" ") + stars(st - 1) : S(""));
            }
            auto old = out.get(m->group(2));
            if (!old || args.size() >= old->args.size()) {
                CSig s;
                s.pascal = words_.contains(S("pascal"));
                s.args = args;
                s.ret = rk;
                s.to = to;
                out.set(m->group(2), s);
            }
        }
    }
    return out;
}

// the name the ported routine itself has (a routine called from C has the C name for its entry)
str inner_name(const str& label, const py::set<str>& cnames) {
    str n = label.startswith(S("_")) ? label.slice(1, label.size()) : label;
    py::dict<str, str> lower;
    for (const str& x : cnames) lower.set(x.lower(), x);
    str c = lower.get(n.lower(), n);
    return c == label ? S("@") + label : label;
}

std::optional<K> join(const std::optional<K>& a_, const std::optional<K>& b_) {
    if (!a_) return b_;
    const K& a = *a_;
    if (a.kind == "ptr" && b_ && b_->kind == "ptr" && a.to != b_->to && a.to && !a.to->empty() && b_->to && !b_->to->empty()) {
        const K& b = *b_;
        const K* xy[2][2] = {{&a, &b}, {&b, &a}};
        for (auto& p : xy) {
            const K& x = *p[0];
            const K& y = *p[1];
            if (FIRST_MEMBER.contains(*y.to) && FIRST_MEMBER.at(*y.to) == *x.to) {   // (a pointer to a structure whose first
                std::set<Origin> o = a.origin;                                      //  member is a pointer: a pointer to such
                o.insert(b.origin.begin(), b.origin.end());                         //  a pointer too - a chain walked from
                return K(S("ptr"), x.to, 0, 0, o);                                  //  its head's place)
            }
        }
    }
    if (!b_ || a.same_key(*b_)) {
        if (!b_ || std::includes(a.origin.begin(), a.origin.end(), b_->origin.begin(), b_->origin.end())) return a;
        K r = a;
        r.origin.insert(b_->origin.begin(), b_->origin.end());
        return r;
    }
    const K& b = *b_;
    std::set<Origin> o = a.origin;
    o.insert(b.origin.begin(), b.origin.end());
    if (a.kind == "val" && b.kind == "val") return K(S("val"), std::nullopt, 0, 0, o);
    if (a.kind == "ptr" && b.kind == "ptr") return K(S("ptr"), a.to == b.to ? a.to : std::nullopt, 0, 0, o);
    if (a.kind == "val" || b.kind == "val") {
        const K& ok = a.kind != "val" ? a : b;
        K r(S("opq"), ok.to, 0, 0, o);
        r.owner = ok.owner;
        return r;
    }
    return K(S("opq"), std::nullopt, 0, 0, o);
}

bool State::same_key(const State& o) const {
    if (sp_old != o.sp_old || sp_new != o.sp_new || routines != o.routines) return false;
    if (r.size() != o.r.size()) return false;
    for (const auto& kv : r) {
        auto it = o.r.find(kv.first);
        if (it == o.r.end() || !kv.second.same_key(it->second) || kv.second.origin != it->second.origin) return false;
    }
    if (stack.size() != o.stack.size()) return false;
    for (size_t i = 0; i < stack.size(); ++i) {
        if (stack[i].old != o.stack[i].old || stack[i].new_ != o.stack[i].new_) return false;
        if (stack[i].k.has_value() != o.stack[i].k.has_value()) return false;
        if (stack[i].k && !stack[i].k->same_key(*o.stack[i].k)) return false;
    }
    return true;
}

State merge(const std::optional<State>& a_, const State& b) { return merge(a_ ? &*a_ : nullptr, b); }

// whether merge(a, b) has another key than a (what _flow asks), without building it
static bool merge_changes(const State& a, const State& b) {
    for (const auto& kv : b.r) {
        auto ia = a.r.find(kv.first);
        if (ia == a.r.end()) return true;
        const K& ka = ia->second;
        const K& kb = kv.second;
        if (ka.same_key(kb)) {
            if (std::includes(ka.origin.begin(), ka.origin.end(), kb.origin.begin(), kb.origin.end())) continue;
            return true;
        }
        std::optional<K> j = join(std::optional<K>(ka), std::optional<K>(kb));
        if (!j || !j->same_key(ka) || j->origin != ka.origin) return true;
    }
    if (a.stack.size() == b.stack.size()) {
        for (size_t i = 0; i < a.stack.size(); ++i) {
            if (!a.stack[i].k && !b.stack[i].k) continue;
            std::optional<K> j = join(a.stack[i].k, b.stack[i].k);
            if (j.has_value() != a.stack[i].k.has_value()) return true;
            if (j && !j->same_key(*a.stack[i].k)) return true;
        }
    }
    for (const str& r : b.routines)
        if (!a.routines.count(r)) return true;
    return false;
}

// a = merge(a, b), in place (the result merge(&a, b) gives)
void merge_into(State& a, const State& b) {
    for (const auto& kv : b.r) {
        auto ia = a.r.find(kv.first);
        if (ia == a.r.end()) {
            a.r[kv.first] = kv.second;                        // (join(None, b): b)
        } else {
            std::optional<K> j = join(std::optional<K>(ia->second), std::optional<K>(kv.second));
            if (j) ia->second = std::move(*j);
        }
    }
    if (a.stack.size() == b.stack.size()) {
        for (size_t i = 0; i < a.stack.size(); ++i) a.stack[i].k = join(a.stack[i].k, b.stack[i].k);
    }
    a.routines.insert(b.routines.begin(), b.routines.end());
}

State merge(const State* ap, const State& b) {
    if (!ap) return b;
    const State& a = *ap;
    State s = a;
    std::set<str> keys;
    for (const auto& kv : a.r) keys.insert(kv.first);
    for (const auto& kv : b.r) keys.insert(kv.first);
    for (const str& k : keys) {
        auto ia = a.r.find(k);
        auto ib = b.r.find(k);
        std::optional<K> ka = ia == a.r.end() ? std::nullopt : std::optional<K>(ia->second);
        std::optional<K> kb = ib == b.r.end() ? std::nullopt : std::optional<K>(ib->second);
        std::optional<K> j = join(ka, kb);
        if (j) s.r[k] = *j;
    }
    if (a.stack.size() == b.stack.size()) {
        for (size_t i = 0; i < a.stack.size(); ++i) s.stack[i].k = join(a.stack[i].k, b.stack[i].k);
    }
    s.routines = a.routines;
    s.routines.insert(b.routines.begin(), b.routines.end());
    return s;
}

// ---------------------------------------------------------------- the module

// An assembler module's source file as its L2 Module: one MachineFragment, its text as written.
asm_::Document module(const str& name, const str& text) {
    l2::Dict frag = l2::fragment(text, name, S("masm"), std::nullopt, S("hand-written"));
    py::list<l2::Val> units;
    units.append(l2::Val(frag));
    l2::Dict doc = l2::module(units);
    asm_::Document d;
    if (doc.contains(S("node")) && std::holds_alternative<str>(doc.at(S("node")).v)) d.node = std::get<str>(doc.at(S("node")).v);
    py::list<asm_::Unit> us;
    for (const l2::Val& u : std::get<l2::Val::List>(doc.at(S("units")).v)) {
        const l2::Dict& ud = std::get<l2::Dict>(u.v);
        asm_::Unit unit;
        auto gs = [&](const char* k) -> std::optional<str> {
            if (!ud.contains(str(k))) return std::nullopt;
            const l2::Val& v = ud.at(str(k));
            if (std::holds_alternative<str>(v.v)) return std::get<str>(v.v);
            return std::nullopt;
        };
        unit.node = gs("node");
        unit.asm_ = gs("asm");
        unit.name = gs("name");
        unit.text = gs("text");
        unit.syntax = gs("syntax");
        unit.within = ud.contains(S("within")) && !ud.at(S("within")).is_none();
        us.append(unit);
    }
    d.units = us;
    return d;
}

// ---------------------------------------------------------------- Port: reading the module

Port::Port(const asm_::Document& doc_, const CTypes& ctypes, const py::list<str>& cnames, const py::set<str>& pascal_,
           const std::optional<py::dict<str, CSig>>& cfuncs_, const py::list<str>& asm_routines_, bool underscore) {
    doc = doc_;
    lines = parse(asm_::module(doc));
    if (cfuncs_ && !cfuncs_->empty()) cfuncs = *cfuncs_;
    for (const str& st_ : ctypes.old.map.keys()) {          // (the structures that begin with a pointer)
        auto f = ctypes.field(st_, 0);
        if (f && std::get<5>(*f) && std::get<1>(*f) == 0 && std::get<2>(*f) == 2) FIRST_MEMBER.set(st_, std::get<6>(*f) + S(" *"));
    }
    asm_routines = py::set<str>::from(asm_routines_);
    cnames_all = py::set<str>::from(cnames);
    ct = ctypes;
    code_seg = code_segments(lines);
    std::optional<str> seg;
    bool proc_far = false;
    for (const LineP& l : lines) {
        std::optional<Directive> d = l->directive;
        if (d && d->kind == "SEGMENT") {
            seg = d->name;
            continue;
        }
        if (d && d->kind == "ENDS") {
            seg = std::nullopt;
            continue;
        }
        if (d && d->kind == "PUBLIC") {
            for (const str& x : py::split(d->rest, S(",")))
                if (!x.strip().empty()) public_.add(x.strip());
            continue;
        }
        if (d && d->kind == "EXTRN") {
            for (const str& x : py::split(d->rest, S(","))) {
                if (x.contains(S(":"))) {
                    auto parts = py::split(x, S(":"), 1);
                    extrn.set(parts[0].strip(), parts[1].strip().upper());
                }
            }
            continue;
        }
        if (d && (d->kind == "EQU" || d->kind == "=")) {
            equ.map.set(*d->name, d->rest.strip());
            continue;
        }
        if (seg && code_seg.contains(*seg)) {
            l->seg = seg;
            if (d && d->kind == "LABEL") l->label = d->name;
            if (d && d->kind == "PROC") {                    // (NAME PROC [FAR]: the routine's label; its ret far)
                l->label = d->name;
                l->directive = std::nullopt;
                proc_far = static_cast<bool>(py::re::search(S(R"((?i)\bFAR\b)"), d->rest));
            }
            if (d && d->kind == "ENDP") proc_far = false;
            if (l->op && *l->op == "ret" && proc_far) l->op = S("retf");
            if (l->label) {
                labels.set(*l->label, code.size());
                bool seen_ = false;                          // (a label given twice: its place now, in its first place)
                for (auto& kv : label_list) if (kv.first == *l->label) { kv.second = labels.at(*l->label); seen_ = true; }
                if (!seen_) label_list.push_back({*l->label, labels.at(*l->label)});
            }
            if (d && (d->kind == "DB" || d->kind == "DW") && !code.empty() && code[-1]->directive &&
                code[-1]->directive->kind == "DB") {
                auto nb = number(py::split(code[-1]->directive->rest, S(";"))[0]);
                if (nb && (*nb == 0xCA || *nb == 0xC2) && !code[-1]->op) {
                    LineP prev = code[-1];                   // (DB 0CAh, DW n: a return written as its bytes)
                    auto n2 = number(prev->directive->rest);
                    prev->op = (n2 && *n2 == 0xCA) ? S("retf") : S("ret");
                    prev->args = py::list<str>{strip_comment(d->rest).strip()};
                    prev->directive = std::nullopt;
                    continue;
                }
            }
            if (d && d->kind == "DB") {
                auto nb = number(strip_comment(d->rest).strip());
                if (nb && (*nb == 0xCB || *nb == 0xC3)) {
                    l->op = (*nb == 0xCB) ? S("retf") : S("ret");
                    l->args = py::list<str>();
                    l->directive = std::nullopt;
                }
            }
            if (l->op || l->label || (d && (d->kind == "DB" || d->kind == "DW" || d->kind == "DD"))) code.append(l);
        } else if (seg) {
            data.append(std::make_tuple(*seg, l));
        }
    }
    Equates eq;
    eq.map = equ.map;
    for (const auto& kv : extrn.items()) {
        const str& k = std::get<1>(kv);
        if (k == "BYTE" || k == "WORD" || k == "DWORD" || k == "QWORD" || k == "TBYTE") eq.data.set(std::get<0>(kv), SIZES.at(k.lower()));
    }
    for (const auto& sl : data) {
        const LineP& l = std::get<1>(sl);
        if (l->directive && l->directive->name) {
            const str& k = l->directive->kind;
            static const py::dict<str, int64_t> sz = {{S("DB"), 1}, {S("DW"), 2}, {S("DD"), 4}, {S("DQ"), 8}, {S("DT"), 10}};
            if (sz.contains(k)) eq.data.set(*l->directive->name, sz.at(k));
        }
    }
    equ = eq;
    py::dict<str, str> lower;
    for (const str& n : cnames) lower.set(n.lower(), n);
    for (const str& p : public_) {                               // (a public routine: the name the C program calls)
        if (labels.contains(p)) {
            str n = p.startswith(S("_")) ? p.slice(1, p.size()) : p;
            cname.set(p, lower.get(n.lower(), n));
        }
    }
    for (const str& p : cname) {
        if (pascal_.contains(cname.at(p)) || (!p.startswith(S("_")) && underscore)) pascal.add(p);
    }
}

// (label, distance) of a code address in a segment, by the old listing - None when it is none
std::optional<std::tuple<str, int64_t>> Port::code_label(const std::optional<str>& seg, int64_t value) const {
    std::optional<std::pair<str, int64_t>> best;
    for (const str& n : offsets.labels) {
        if (n.startswith(S("@"))) continue;
        const auto& so_ = offsets.labels.at(n);
        const str& s = std::get<0>(so_);
        int64_t o = std::get<1>(so_);
        if (seg && s == *seg && o <= value && labels.contains(n) && (!best || o > best->second)) best = std::make_pair(n, o);
    }
    if (!best) return std::nullopt;
    int64_t nxt = int64_t(1) << 20;
    bool any = false;
    for (const str& n : offsets.labels) {
        const auto& so = offsets.labels.at(n);
        if (!n.startswith(S("@")) && seg && std::get<0>(so) == *seg && std::get<1>(so) > best->second) {
            if (!any || std::get<1>(so) < nxt) nxt = std::get<1>(so);
            any = true;
        }
    }
    if (value >= nxt) return std::nullopt;
    return std::make_tuple(best->first, value - best->second);
}

// each table line of the code: its old offset and length (from the listing, matched in order)
void Port::place_data() {
    const py::list<ListLine>& ls = offsets.lines;
    int64_t k = 0;
    dline = py::dict<int64_t, std::tuple<int64_t, int64_t>>();
    for (int64_t i = 0; i < code.size(); ++i) {
        const LineP& l = code[i];
        if (!(l->directive && (l->directive->kind == "DB" || l->directive->kind == "DW" || l->directive->kind == "DD"))) continue;
        str want = py::re::sub(S(R"(\s+)"), S(" "), strip_comment(l->text)).strip().slice(0, 24);
        for (int64_t j = k; j < ls.size(); ++j) {
            if (ls[j].seg == *l->seg && ls[j].text.slice(0, 16) == want.slice(0, 16)) {
                std::optional<int64_t> nxt;
                for (int64_t q = j + 1; q < ls.size(); ++q)
                    if (ls[q].seg == *l->seg && ls[q].offset > ls[j].offset) { nxt = ls[q].offset; break; }
                dline.set(i, std::make_tuple(ls[j].offset, nxt ? *nxt - ls[j].offset : ls[j].nbytes));   // (its length: to the next line)
                k = j + 1;
                break;
            }
        }
    }
}

// (label, distance) of a number that is an address in a table of the code segment - None when it is none: inside a
// table line, or just before one (a table counted from 1)
std::optional<std::tuple<str, int64_t>> Port::data_address(const str& seg, int64_t v) const {
    std::optional<std::pair<str, int64_t>> best;
    for (const auto& kv : dline.items()) {
        int64_t i = std::get<0>(kv);
        int64_t o = std::get<0>(std::get<1>(kv)), n = std::get<1>(std::get<1>(kv));
        if (!(code[i]->seg && *code[i]->seg == seg)) continue;
        if (o <= v && v < o + std::max<int64_t>(n, 1)) return std::make_tuple(F("@d_%04x", o), v - o);
        if (v < o && o <= v + 128 && (!best || o < best->second)) best = std::make_pair(F("@d_%04x", o), o);   // (a table counted from a base before it)
    }
    if (best) return std::make_tuple(best->first, v - best->second);
    return std::nullopt;
}

// the routines a table of the code segment names (DW place, as the listing places them)
py::set<str> Port::table_targets() const {
    py::set<str> out;
    for (const LineP& l : code) {
        if (l->directive && l->directive->kind == "DW") {
            for (const str& v : split_ops(strip_comment(l->directive->rest))) {
                auto nv = number(v);
                std::optional<std::tuple<str, int64_t>> c = nv ? code_label(l->seg, *nv) : std::nullopt;
                if (c && std::get<1>(*c) == 0) out.add(std::get<0>(*c));
                auto o = py::re::fullmatch(S(R"((?i)offset\s+(?:\w+:)?(\w+))"), v.strip());
                if (o && labels.contains(o->group(1))) out.add(o->group(1));
            }
        }
    }
    return out;
}

// whether register r, given a number at line i, is then used to read the code segment (cs:[r]): the number is an
// address there
bool Port::cs_address(int64_t i, const str& r) const {
    for (int64_t j = i + 1; j < std::min<int64_t>(i + 40, code.size()); ++j) {
        const LineP& l = code[j];
        if (!l->op) continue;
        for (const str& x : l->args)
            if (py::re::search(S(R"((?i)cs\s*:\s*\[[^\]]*\b)") + r + S(R"(\b)"), x)) return true;
        if (!l->args.empty() && (*l->op == "mov" || *l->op == "lea" || *l->op == "pop" || *l->op == "les" || *l->op == "lds") &&
            l->args[0].lower() == r)
            return false;
        if (*l->op == "ret" || *l->op == "retf" || *l->op == "jmp") return false;
    }
    return false;
}

// ---------------------------------------------------------------- Port: the kinds, to a fixed point

State Port::entry_state(const str& routine_) const {
    State s;
    for (const str& r : R16) {
        K k(S("opq"));
        k.origin.insert(Origin{S("in"), routine_, r, 0});
        s.r[r] = k;
    }
    s.r[S("sp")] = K(S("frame"), std::nullopt, 0, 0);
    s.routines.insert(routine_);
    return s;
}

void Port::analyse() {
    before.clear();
    std::deque<int64_t> work;
    for (const str& p : cname.keys()) {                      // (a routine called from outside - the C program, the other
        if (!cnames_all.contains(cname.at(p)) && !used_outside.contains(p) && called.contains(p)) continue;   // modules - or by none: an entry;
        int64_t i = labels.at(p);                            //  else its callers' states are what it is given)
        before[i] = entry_state(p);
        work.push_back(i);
    }
    {
        std::vector<str> tt;
        for (const str& p : table_targets()) tt.push_back(p);
        std::sort(tt.begin(), tt.end());
        for (const str& p : tt) {                            // (a routine called through a table of the code: an entry of
            int64_t i = labels.at(p);                        //  its own, whoever calls it)
            if (!before.count(i)) {
                before[i] = entry_state(p);
                work.push_back(i);
            }
        }
    }
    int64_t seen = 0;
    while (!work.empty()) {
        int64_t i = work.front();
        work.pop_front();
        ++seen;
        if (seen > 200000) throw Asm16Error("the kinds do not settle");
        State st = before.at(i);
        const LineP l = code[i];
        State pre_call;
        py::list<Flow> flow;
        if (l->op) {
            if (*l->op == "call") pre_call = st;             // (the state the callee is entered with: before)
            auto r = step(i, l, std::move(st));
            st = std::move(std::get<0>(r));
            flow = std::get<1>(r);
        } else {
            flow.append(Flow{Flow::Next, S("")});
        }
        std::vector<int64_t> nxt;
        for (const Flow& f : flow) {
            if (f.kind == Flow::Next) {
                nxt.push_back(i + 1);
            } else if (f.kind == Flow::Jump) {
                if (labels.contains(f.label)) nxt.push_back(labels.at(f.label));
            } else if (f.kind == Flow::Call) {
                if (labels.contains(f.label)) {
                    int64_t t = labels.at(f.label);
                    State cs = pre_call;
                    cs.stack.push_back(StackItem{2, 8, K(S("ret"))});   // (the return address)
                    cs.sp_old -= 2;
                    cs.sp_new -= 8;
                    State rel;
                    rel.r = cs.r;
                    for (const str& r : R16) {
                        auto it = cs.r.find(r);
                        if (it == cs.r.end()) continue;
                        const K& k_ = it->second;
                        if (k_.kind == "frame" && r == "bp") {   // (BP still the caller's frame: a local routine of its works on it)
                            FrameOwner owner;
                            if (k_.owner) {
                                owner = *k_.owner;
                            } else {
                                owner.routine = routine(i);
                                for (const auto& x : pre_call.stack) owner.stack.push_back({x.old, x.new_});
                            }
                            K nk(S("frame"), std::nullopt, k_.old, k_.new_);
                            nk.owner = owner;
                            rel.r[r] = nk;
                        } else if (k_.kind == "frame") {
                            rel.r[r] = K(S("opq"));          // (a frame of the caller's is none of the callee's)
                        }
                    }
                    rel.r[S("sp")] = K(S("frame"), std::nullopt, 0, 0);
                    rel.routines.insert(f.label);
                    _flow(t, rel, work);
                }
                nxt.push_back(i + 1);
            }
        }
        for (int64_t j : nxt)
            if (j < code.size()) _flow(j, st, work);
    }
}

void Port::_flow(int64_t j, const State& st, std::deque<int64_t>& work) {
    auto it = before.find(j);
    if (it == before.end()) {
        before[j] = st;
        work.push_back(j);
        return;
    }
    if (merge_changes(it->second, st)) {                      // (new = merge(old, st); new.key() != old.key())
        merge_into(it->second, st);
        work.push_back(j);
    }
}

// [1f62h]: memory at an old address of the data segment, as a number - the variable there (or m)
Mem Port::absolute(const Mem& m) {
    if (m.sym || !m.regs.empty() || m.disp < 0x100 || (m.seg && *m.seg == "cs")) return m;
    std::optional<std::tuple<str, int64_t>> v;
    if (!data_map.empty()) v = data_var(m.disp);
    if (!v) return m;
    data_used.add(std::get<0>(*v));
    return Mem(m.size, std::nullopt, std::get<0>(*v), py::list<str>(), std::get<1>(*v), m.text);
}

// the kind of what a memory operand holds, and its new place: (K, where)
std::tuple<K, std::optional<Field>> Port::kind_of_mem(const Mem& m0, const State& st) {
    Mem m = absolute(m0);
    auto find = [&](const str& r) -> const K* {
        auto it = st.r.find(r);
        return it == st.r.end() ? nullptr : &it->second;
    };
    if (m.sym && m.regs.size() == 1 && ct.is_pointer(c(*m.sym))) {
        auto v = ct.var(c(*m.sym));
        if ((v ? std::get<1>(*v) : S("")).contains(S("["))) {
            const K* k_ = find(m.regs[0]);
            if (!(k_ && k_->wide())) return {K(S("ptr"), ct.pointee(c(*m.sym))), std::nullopt};   // (table[bx]: an element of an array of pointers)
        }
    }
    if (m.sym && m.regs.empty()) {
        str name = c(*m.sym);
        if (own_ptr.count(std::make_tuple(*m.sym, m.disp))) return {K(S("ptr")), std::nullopt};   // (a slot of the module's own data holding an address)
        bool sz = !m.size || *m.size == 2 || *m.size == 4;
        if (ct.is_pointer(name) && sz && m.disp == 0) return {K(S("ptr"), ct.pointee(name)), std::nullopt};
        if (ct.is_pointer(name) && ct.far_ptrs.contains(name) && m.disp == 2 && (!m.size || *m.size == 2))
            return {K(S("seg")), std::nullopt};              // (the segment half of a far pointer variable)
        return {VAL, std::nullopt};
    }
    if (m.regs.size() == 1 && !m.sym) {
        const K* b = find(m.regs[0]);
        if (b && b->kind == "ptr" && b->to && b->to->startswith(S("@ds:"))) return kind_of_mem(ds_mem(m, *b), st);
        if (b && b->kind == "ptr" && b->to && b->to->endswith(S("*")) && m.disp % 2 == 0 && (!m.size || *m.size == 2))
            return {K(S("ptr"), b->to->slice(0, b->to->size() - 1).strip()), std::nullopt};   // (an element of an array of pointers: one of them)
        if (b && b->kind == "ptr" && b->to && b->to->startswith(std::make_tuple("struct", "union")) && !b->to->endswith(S("*"))) {
            auto f = ct.field(*b->to, m.disp);
            if (f) {
                int64_t fo = std::get<1>(*f), fs = std::get<2>(*f);
                bool isptr = std::get<5>(*f);
                if (isptr && m.disp == fo) {
                    str to = py::re::sub(S(R"(\b(far|near|huge|extern|static)\b)"), S(""), std::get<6>(*f)).strip();
                    return {K(S("ptr"), to), f};
                }
                if (isptr && m.disp == fo + 2 && fs == 4) return {K(S("seg")), f};
                return {VAL, f};
            }
        }
        if (b && b->kind == "frame" && b->owner && b->old + m.disp < 0) return {VAL, std::nullopt};   // (a local of the routine whose frame it is)
        if (b && b->kind == "frame" && b->old + m.disp < 0) {
            int64_t e = b->old + m.disp, o = 0;              // (below the entry: what was pushed there, or room)
            for (const auto& it : st.stack) {
                if (o - it.old <= e && e < o) {
                    if (it.k && it.old == 2 && it.new_ == 8) return {*it.k, std::nullopt};
                    return {VAL, std::nullopt};
                }
                o -= it.old;
            }
            return {VAL, std::nullopt};
        }
        if (b && b->kind == "frame") {
            int64_t e = b->old + m.disp;
            std::vector<Rt> rs;
            if (b->owner) rs.push_back(b->owner->routine);
            else for (const str& r_ : st.routines) rs.push_back(r_);
            for (const Rt& r_ : rs) {
                auto pp = param_place(r_);
                if (pp.contains(e) && !pp.at(e).empty() && (!m.size || *m.size == 2 || *m.size == 4))   // (an argument the C program passes a pointer in)
                    return {K(S("ptr"), pp.at(e)), std::nullopt};
            }
            K k(S("opq"));
            k.origin.insert(Origin{S("param"), m.regs[0], S(""), e});
            return {k, std::nullopt};
        }
    }
    return {VAL, std::nullopt};
}

// {old offset from the entry's sp: what the argument there points to} - of the C program's calls
py::dict<int64_t, str> Port::param_place(const Rt& routine_) const {
    py::dict<int64_t, str> out;
    if (!routine_ || !param_types.contains(*routine_)) return out;
    const auto& args = param_types.at(*routine_);
    if (args.empty()) return out;
    int64_t at = retsize(routine_);
    for (const auto& a : args) {
        const auto& to = std::get<1>(a);
        if (to && !to->empty()) out.set(at, *to);
        at += std::get<0>(a);
    }
    return out;
}

// (old, new) size of what a pointer's kind points to - (1, 1) when not known
std::tuple<int64_t, int64_t> Port::element_sizes(const K& k) const {
    str to = k.to ? *k.to : S("");
    if (to.endswith(S("*"))) return {2, 8};
    if (to.startswith(std::make_tuple("struct", "union"))) {
        const Layout* o = ct.old.get(to);
        const Layout* n = ct.new_.get(to);
        if (o && n) return {o->size, n->size};
    }
    return {1, 1};
}

// (name, offset inside it) of an old address of the data segment - None when it is none of them
std::optional<std::tuple<str, int64_t>> Port::data_var(int64_t addr) const {
    int64_t best = -1;
    for (int64_t k = 0; k < data_map.size(); ++k) {
        if (std::get<0>(data_map[k]) <= addr) best = k;
        else break;
    }
    if (best < 0) return std::nullopt;
    int64_t a = std::get<0>(data_map[best]);
    const str& n = std::get<1>(data_map[best]);
    int64_t nxt = best + 1 < data_map.size() ? std::get<0>(data_map[best + 1]) : a + 0x10000;
    if (addr < nxt) return std::make_tuple(n, addr - a);
    return std::nullopt;
}

// the variable whose old address is exactly v (a number of the code) - None
std::optional<str> Port::data_at(std::optional<int64_t> v) const {
    if (!v || *v < 0x100) return std::nullopt;
    for (const auto& x : data_map)
        if (std::get<0>(x) == *v) return std::get<1>(x);
    return std::nullopt;
}

// whether register r, given a number at line i, is then used as the base of a memory operand of the data segment
// (not cs:) before it is given another value
bool Port::ds_base(int64_t i, const str& r) const {
    for (int64_t j = i + 1; j < std::min<int64_t>(i + 60, code.size()); ++j) {
        const LineP& l = code[j];
        if (!l->op) continue;
        for (const str& x : l->args)
            if (py::re::search(S(R"(\[[^\]]*\b)") + r + S(R"(\b)"), x, py::re::I) && !py::re::search(S(R"((?i)cs\s*:)"), x)) return true;
        const str& op = *l->op;
        if (!l->args.empty() && (op == "mov" || op == "lea" || op == "pop" || op == "les" || op == "lds" || op == "xchg") &&
            l->args[0].lower() == r)
            return false;
        if (op == "ret" || op == "retf" || op == "jmp" || op == "call") return false;
    }
    return false;
}

// a memory operand at a register given an old address of the data segment: the variable there
Mem Port::ds_mem(const Mem& m, const K& k) {
    int64_t addr = py::int_parse(k.to->slice(4, k.to->size()), 16) + m.disp;
    auto v = data_var(addr);
    if (!v) throw Asm16Error(F("the data segment's %04xh: no variable known there", addr));
    data_used.add(std::get<0>(*v));
    return Mem(m.size, std::nullopt, std::get<0>(*v), py::list<str>(), std::get<1>(*v), m.text);
}

// a routine outside the module called: ("asm", its name) - another module's routine, called as it was - or ("c", C name,
// (pascal, [(old size, kind)], returns)) - the C program's or the C library's - or None
std::optional<Callee> Port::callee(const str& t) const {
    if (asm_routines.contains(t)) return Callee{S("asm"), inner_name(t, cnames_all), CSig()};
    if (RUNTIME_C.contains(t)) {
        // (the Python's r[0] is the C name, r[1:] the signature: the names are those of the table below)
        static const py::dict<str, str> cn = {{S("__ffree"), S("dos_ffree")}, {S("__fmalloc"), S("dos_fmalloc")},
                                              {S("_free"), S("free")}, {S("_malloc"), S("malloc")}};
        return Callee{S("c"), cn.at(t), RUNTIME_C.at(t)};
    }
    py::dict<str, str> lower;
    for (const str& k : cfuncs.keys()) lower.set(k.lower(), k);
    if (t.startswith(S("_")) && cfuncs.contains(t.slice(1, t.size())) && !cfuncs.at(t.slice(1, t.size())).pascal)
        return Callee{S("c"), t.slice(1, t.size()), cfuncs.at(t.slice(1, t.size()))};
    auto k = lower.get(t.lower());
    if (!t.startswith(S("_")) && k && cfuncs.at(*k).pascal) return Callee{S("c"), *k, cfuncs.at(*k)};
    return std::nullopt;
}

// the old stack's words of a C function's arguments: [(arg index, size, kind, first slot)], the number of slots (the
// lowest address first: pascal pushed the first argument first)
std::tuple<py::list<std::tuple<int64_t, int64_t, str, int64_t>>, int64_t> Port::slots(const CSig& sig) {
    std::vector<int64_t> order;
    for (int64_t j = 0; j < sig.args.size(); ++j) order.push_back(j);
    if (sig.pascal) std::reverse(order.begin(), order.end());
    py::list<std::tuple<int64_t, int64_t, str, int64_t>> out;
    int64_t s = 0;
    for (int64_t j : order) {
        auto sk = sig.args[j];
        out.append(std::make_tuple(j, std::get<0>(sk), std::get<1>(sk), s));
        s += std::get<0>(sk) / 2;
    }
    std::vector<std::tuple<int64_t, int64_t, str, int64_t>> v = out.vec();
    std::sort(v.begin(), v.end());
    return {py::list<std::tuple<int64_t, int64_t, str, int64_t>>(v), s};
}

// the call of a C function from the old code: its arguments, on the stack as the old caller pushed them (8 bytes
// each word), given today's way; what it returns where the old code looks (ax, dx:ax)
py::list<str> Port::c_thunk(const str& name, const CSig& sig) const {
    auto sl = slots(sig);
    const auto& args = std::get<0>(sl);
    int64_t n = std::get<1>(sl);
    static const char* regs[4] = {"rcx", "rdx", "r8", "r9"};
    int64_t extra = std::max<int64_t>(0, args.size() - 4);
    py::list<str> out;
    out.append(S(""));
    out.append(F("; %s from the old code: the arguments from the old stack, %s", name,
                 sig.pascal ? "taken off by it (pascal)" : "left for the caller"));
    out.append(F("@@c_%s PROC", name));
    out.append(S("\tpush\tr15"));
    out.append(S("\tmov\tr15, rsp"));
    out.append(S("\tand\trsp, -16"));
    out.append(F("\tsub\trsp, %d", 32 + 8 * (extra + extra % 2)));
    for (const auto& a : args) {
        int64_t j = std::get<0>(a);
        const str& kind = std::get<2>(a);
        int64_t sl_ = std::get<3>(a);
        str at = F("[r15+%d]", 16 + 8 * sl_);
        str hi = F("[r15+%d]", 16 + 8 * (sl_ + 1));
        str dst = j < 4 ? S(regs[j]) : S("rax");
        static const py::dict<str, str> d32m = {{S("rcx"), S("ecx")}, {S("rdx"), S("edx")}, {S("r8"), S("r8d")},
                                                {S("r9"), S("r9d")}, {S("rax"), S("eax")}};
        str d32 = d32m.at(dst);
        if (kind == "ptr") {
            out.append(F("\tmov\t%s, qword ptr %s", dst, at));
        } else if (kind == "long") {
            out.append(F("\tmovzx\t%s, word ptr %s", d32, at));
            out.append(F("\tmovzx\tr11d, word ptr %s", hi));
            out.append(S("\tshl\tr11d, 16"));
            out.append(F("\tor\t%s, r11d", d32));
        } else if (kind == "double") {
            out.append(F("\tmov\t%s, qword ptr %s", dst, at));
            if (j < 4) out.append(F("\tmovq\txmm%d, %s", j, dst));
        } else {
            out.append(F("\tmovsx\t%s, word ptr %s", dst, at));
        }
        if (j >= 4) out.append(F("\tmov\tqword ptr [rsp+%d], rax", 32 + 8 * (j - 4)));
    }
    out.append(F("\tcall\t%s", name));
    if (sig.ret == "long") {
        out.append(S("\tmov\tedx, eax"));
        out.append(S("\tshr\tedx, 16\t\t; (a long: dx:ax)"));
    }
    out.append(S("\tmov\trsp, r15"));
    out.append(S("\tpop\tr15"));
    out.append(sig.pascal && n ? F("\tret\t%d", 8 * n) : S("\tret"));
    out.append(F("@@c_%s ENDP", name));
    return out;
}

// the C name of an assembler symbol (_x is C's x)
str Port::c(const str& s) const { return s.startswith(S("_")) ? s.slice(1, s.size()) : s; }

namespace {

bool is_reg(const Operand& o, const char* r) { return o.kind == "reg" && o.text == r; }

bool one_of(const str& x, std::initializer_list<const char*> xs) {
    for (const char* y : xs) if (x == y) return true;
    return false;
}

}  // namespace

// the state after an instruction, and where control goes
std::tuple<State, py::list<Flow>> Port::step(int64_t i, const LineP& l, State st) {
    const str& op = *l->op;
    py::list<Operand> a = operands_of(l);
    py::list<Flow> flow;
    flow.append(Flow{Flow::Next, S("")});
    if ((op == "mov" || op == "pop") && !l->args.empty()) {
        str t = l->args[0].strip().lower();
        if (t == "es" || t == "ds") st.r.erase(S("@") + t);   // (another segment: no address's now)
    }

    auto setr = [&](const str& r, const std::optional<K>& k) {
        if (is_r8(r)) {
            auto it = st.r.find(R8.at(r));
            K nk(S("val"));
            nk.origin = (it != st.r.end() ? it->second : VAL).origin;
            st.r[R8.at(r)] = nk;
        } else if (is_r16(r)) {
            if (k) st.r[r] = *k;
            else st.r.erase(r);
        }
    };
    auto stget = [&](const str& r) -> std::optional<K> {
        auto it = st.r.find(r);
        if (it == st.r.end()) return std::nullopt;
        return it->second;
    };
    auto get = [&](const Operand& o) -> std::optional<K> {
        if (o.kind == "reg") {
            if (is_r16(o.text)) return stget(o.text);
            return is_r8(o.text) ? VAL : K(S("seg"));
        }
        if (o.kind == "off") {
            auto v = ct.var(c(o.text));
            return K(S("ptr"), v ? std::optional<str>(std::get<0>(*v)) : std::nullopt);
        }
        if (o.kind == "mem") return std::get<0>(kind_of_mem(*o.mem, st));
        return VAL;
    };

    if (JUMPS.contains(op)) {
        flow = py::list<Flow>();
        flow.append(Flow{Flow::Jump, last_word(l->args[0])});     // (SHORT, NEAR PTR...: the label is the last word)
        if (op != "jmp") flow.append(Flow{Flow::Next, S("")});
        return {std::move(st), flow};
    }
    if (op == "call") {
        str t = last_word(l->args[0]);
        for (const char* r : {"ax", "cx", "dx", "bx"}) {
            if (!labels.contains(t)) {
                if (str(r) != "ax") st.r[str(r)] = VAL;
                else st.r[S("ax")] = K(S("opq"));
            }
        }
        std::optional<Callee> ce;
        if (!labels.contains(t)) ce = callee(t);
        if (ce && ce->kind == "c" && ce->sig.pascal) {         // (pascal: the callee took its arguments off)
            int64_t words_ = std::get<1>(slots(ce->sig));
            while (words_ > 0 && !st.stack.empty()) {
                StackItem it = st.stack.back();
                st.stack.pop_back();
                st.sp_old += it.old;
                st.sp_new += it.new_;
                words_ -= it.old / 2;
            }
        }
        if (ce && ce->kind == "c" && ce->sig.ret == "long") st.r[S("dx")] = VAL;
        if (ce && ce->kind == "c" && ce->sig.ret == "ptr") st.r[S("ax")] = K(S("ptr"), ce->sig.to);   // (the address the C function gives: its type's)
        if (labels.contains(t)) {
            auto after = exit_of(t);
            py::set<str> mods = writes(t);
            if (after) {
                for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "bp"}) {
                    if (!mods.contains(str(r))) continue;       // (what the routine never writes is the caller's still)
                    auto it = after->r.find(str(r));
                    if (it != after->r.end()) {
                        const K& k = it->second;
                        bool inorigin = false;
                        for (const Origin& o : k.origin) if (o.kind == "in") inorigin = true;
                        if (!(!k.origin.empty() && inorigin)) st.r[str(r)] = k;
                    }
                }
            }
        }
        py::list<Flow> fl;
        fl.append(Flow{Flow::Call, t});
        return {std::move(st), fl};
    }
    if (op == "ret" || op == "retf" || op == "retn" || op == "iret") {
        std::vector<Rt> rts;
        if (!st.routines.empty()) for (const str& r : st.routines) rts.push_back(r);      // (the routines whose code this is)
        else rts.push_back(routine(i));
        for (const Rt& rt : rts) {
            exits_new[rt].push_back(st);
            if (op == "retf") far.insert(rt);                // (a far return: a far routine, its arguments 4 on)
        }
        py::list<Flow> fl;
        fl.append(Flow{Flow::Ret, S("")});
        return {std::move(st), fl};
    }
    if (op == "push") {
        std::optional<K> k = (a[0].kind == "reg" || a[0].kind == "mem") ? get(a[0]) : std::optional<K>(VAL);
        bool gone = a[0].kind == "reg" && SEG.contains(a[0].text);   // (a segment register: not pushed any more)
        st.stack.push_back(StackItem{2, gone ? 0 : 8, k});
        st.sp_old -= 2;
        st.sp_new -= gone ? 0 : 8;
        return {std::move(st), flow};
    }
    if (op == "pushf" || op == "pushfw") {
        st.stack.push_back(StackItem{2, 8, VAL});
        st.sp_old -= 2;
        st.sp_new -= 8;
        return {std::move(st), flow};
    }
    if (op == "pop" || op == "popf" || op == "popfw") {
        StackItem item{2, 8, VAL};
        if (!st.stack.empty()) {
            item = st.stack.back();
            st.stack.pop_back();
        }
        st.sp_old += 2;
        st.sp_new += item.new_;
        if (op == "pop" && a[0].kind == "reg") setr(a[0].text, item.k);
        return {std::move(st), flow};
    }
    if (one_of(op, {"mov", "xchg", "lea", "les", "lds"}) && !a.empty() && a[0].kind == "reg") {
        str d = a[0].text;
        if (op == "xchg" && a.size() > 1) {
            auto k0 = get(a[0]);
            auto k1 = get(a[1]);
            setr(d, k1);
            if (a[1].kind == "reg") setr(a[1].text, k0);
            return {std::move(st), flow};
        }
        if (op == "lea") {
            std::optional<str> to;
            if (a[1].kind == "mem" && a[1].mem->regs.size() == 1 && !a[1].mem->sym) {
                auto kb = stget(a[1].mem->regs[0]);
                if (kb && kb->kind == "ptr" && kb->to && kb->to->startswith(std::make_tuple("struct", "union")) && !kb->to->endswith(S("*")))
                    to = ct.member_type(*kb->to, a[1].mem->disp);   // (the address of a member: its type's)
            }
            setr(d, K(S("ptr"), to));
            return {std::move(st), flow};
        }
        if (op == "les" || op == "lds") {
            auto k = get(a[1]);
            setr(d, (k && k->wide()) ? *k : K(S("ptr")));
            K sk(S("seg"));
            sk.origin.insert(Origin{S("segof"), d, S(""), 0});
            st.r[S("@") + op.slice(1, op.size())] = sk;
            return {std::move(st), flow};
        }
        if (a[1].kind == "reg" && (a[1].text == "es" || a[1].text == "ds") && is_r16(d) && st.r.count(S("@") + a[1].text)) {
            std::optional<str> partner;
            for (const Origin& o : st.r.at(S("@") + a[1].text).origin) if (o.kind == "segof") { partner = o.a; break; }
            std::optional<K> pk = partner ? stget(*partner) : std::nullopt;
            if (pk && pk->wide()) {                          // (the segment of an address: the address, for its test against 0)
                setr(d, pk);
                return {std::move(st), flow};
            }
        }
        Operand src = a[1];
        auto nv = [&]() -> std::optional<int64_t> { return number(l->args[1]); };
        if (src.kind == "reg" && src.text == "sp") {
            setr(d, K(S("frame"), std::nullopt, st.sp_old, st.sp_new));
        } else if (SEG.contains(d) || (src.kind == "reg" && SEG.contains(src.text))) {
            if (is_r16(d)) setr(d, K(S("seg")));
        } else if (d == "cx") {
            setr(d, VAL);                                    // (cx addresses nothing on the 8086: a count)
        } else if (src.kind == "imm" && one_of(d, {"bx", "si", "di"}) && nv() &&
                   ((cs_address(i, d) && (data_address(*l->seg, *nv()) || code_label(l->seg, *nv()))) ||
                    (data_address(*l->seg, *nv()) && *nv() >= 0x100))) {
            // (an address register given a number in a table of the code, or just before one: its address)
            setr(d, K(S("ptr"), S("code")));                 // (an address in the code segment: a table)
        } else if (src.kind == "imm" && one_of(d, {"bx", "si", "di"}) && nv() && !data_map.empty() &&
                   [&] { for (const auto& x : data_map) if (std::get<0>(x) == *nv()) return true; return false; }() &&
                   ds_base(i, d)) {
            setr(d, K(S("ptr"), F("@ds:%x", *nv())));        // (an old address of the data segment: a variable's)
        } else {
            std::optional<K> k_ = get(src);
            if (!k_) k_ = VAL;
            setr(d, (k_->kind == "seg" && src.kind == "mem") ? VAL : *k_);   // (a far address's segment: a number)
        }
        return {std::move(st), flow};
    }
    if (op == "sub" && is_reg(a[0], "sp")) {
        int64_t n = number(l->args[1]).value_or(0);
        st.stack.push_back(StackItem{n, n, VAL});
        st.sp_old -= n;
        st.sp_new -= n;
        return {std::move(st), flow};
    }
    if (op == "add" && is_reg(a[0], "sp")) {
        int64_t n = number(l->args[1]).value_or(0);
        st.sp_old += n;
        while (n > 0 && !st.stack.empty()) {
            StackItem item = st.stack.back();
            st.stack.pop_back();
            n -= item.old;
            st.sp_new += item.new_;
        }
        return {std::move(st), flow};
    }
    if (STRING.contains(op) || one_of(op, {"cbw", "cwd", "mul", "imul", "div", "idiv", "lahf"})) {
        if (one_of(op, {"lodsb", "lodsw", "cbw", "lahf"})) st.r[S("ax")] = VAL;
        if (one_of(op, {"cwd", "mul", "imul", "div", "idiv"})) {
            st.r[S("ax")] = VAL;
            st.r[S("dx")] = VAL;
        }
        if (l->prefix || op == "loop") st.r[S("cx")] = VAL;
        return {std::move(st), flow};
    }
    if (op == "int") {
        st.r[S("ax")] = VAL;
        return {std::move(st), flow};
    }
    if (!a.empty() && a[0].kind == "reg" &&
        one_of(op, {"add", "sub", "inc", "dec", "neg", "and", "or", "xor", "adc", "sbb", "not", "shl", "shr", "sal", "sar",
                    "rol", "ror", "rcl", "rcr"})) {
        str d = a[0].text;
        std::optional<K> k = is_r16(d) ? stget(d) : std::optional<K>(VAL);
        if (is_r8(d)) {
            setr(d, VAL);
        } else if (op == "xor" && a.size() > 1 && a[1] == a[0]) {
            setr(d, VAL);
        } else if ((op == "sub" || op == "cmp") && k && k->wide() && a.size() > 1 && a[1].kind == "imm" &&
                   data_at(number(l->args[1]))) {
            if (op == "sub") setr(d, VAL);                   // (an address less a variable's: how far into it)
        } else if ((op == "add" || op == "sub") && k && k->wide()) {
            std::optional<K> s2 = a.size() > 1 ? get(a[1]) : std::optional<K>(VAL);
            if (op == "sub" && s2 && s2->wide()) {
                std::set<Origin> o = k->origin;
                o.insert(s2->origin.begin(), s2->origin.end());
                K r(S("opq"));
                r.origin = o;
                setr(d, (k->kind == "opq" && s2->kind == "opq") ? std::optional<K>(r) : std::optional<K>(VAL));
            } else if (k->kind == "frame") {
                int64_t n = number(l->args[1]).value_or(0);
                K nk(S("frame"), std::nullopt, k->old + (op == "add" ? n : -n), k->new_ + (op == "add" ? n : -n));
                nk.owner = k->owner;
                setr(d, nk);
            } else {
                setr(d, k);
            }
        } else if ((op == "inc" || op == "dec") && k && k->wide()) {
            setr(d, k);
        } else if ((op == "and" || op == "or") && a.size() > 1 && a[1] == a[0]) {
            // (a test of itself)
        } else {
            setr(d, VAL);
        }
        if (op == "add" && a.size() > 1 && k && k->kind == "val") {
            auto s2 = get(a[1]);
            if (s2 && s2->wide() && s2->kind != "frame") setr(d, s2);   // (a value added to a pointer: the pointer)
        }
        return {std::move(st), flow};
    }
    return {std::move(st), flow};
}

// the 16-bit registers a routine (from its label: the lines its jumps reach, and the routines it calls) gives a value -
// a push and pop of one around it not counted
py::set<str> Port::writes(const str& label, py::set<str>* seen_) {
    if (_writes.count(label)) return _writes.at(label);
    py::set<str> own_seen;
    py::set<str>& seen = seen_ ? *seen_ : own_seen;
    if (seen.contains(label) || !labels.contains(label)) return py::set<str>();
    seen.add(label);
    py::set<str> out;
    std::vector<int64_t> todo{labels.at(label)};
    std::set<int64_t> done;
    std::map<str, int64_t> pushed;
    while (!todo.empty()) {
        int64_t i = todo.back();
        todo.pop_back();
        if (done.count(i) || i >= code.size()) continue;
        done.insert(i);
        const LineP& l = code[i];
        std::vector<int64_t> nxt{i + 1};
        if (l->op) {
            const str& op = *l->op;
            str a0 = !l->args.empty() ? l->args[0].lower() : S("");
            str r16 = R8.get(a0, a0);
            if (op == "push" && is_r16(a0)) {
                pushed[a0] += 1;
            } else if (op == "pop" && is_r16(a0)) {
                if (!pushed[a0]) out.add(a0);                // (a pop of what it did not push: a value given)
            } else if (one_of(op, {"mov", "add", "sub", "and", "or", "xor", "adc", "sbb", "inc", "dec", "neg", "not", "shl",
                                   "shr", "sal", "sar", "rol", "ror", "rcl", "rcr", "lea", "les", "lds", "xchg"}) &&
                       is_r16(r16) && !((op == "and" || op == "or") && l->args.size() == 2 && l->args[0].lower() == l->args[1].lower())) {
                out.add(r16);
                if (op == "xchg" && l->args.size() == 2) {
                    str a1 = l->args[1].lower();
                    str r1 = R8.get(a1, a1);
                    if (is_r16(r1)) out.add(r1);
                }
            }
            if (one_of(op, {"lodsb", "lodsw", "cbw", "lahf", "in"})) out.add(S("ax"));
            if (one_of(op, {"cwd", "mul", "imul", "div", "idiv"})) { out.add(S("ax")); out.add(S("dx")); }
            if (STRING.contains(op)) {
                str p4 = op.slice(0, 4);
                if (p4 == "lods") out.add(S("si"));
                else if (p4 == "stos" || p4 == "scas") out.add(S("di"));
                else { out.add(S("si")); out.add(S("di")); }
                if (l->prefix) out.add(S("cx"));
            }
            if (op.startswith(S("loop")) || op == "int") out.add(op.startswith(S("loop")) ? S("cx") : S("ax"));
            if (op == "call") {
                str t = last_word(l->args[0]);
                if (labels.contains(t)) {
                    py::set<str> w = writes(t, &seen);
                    for (const str& x : w) out.add(x);
                } else {
                    for (const char* x : {"ax", "bx", "cx", "dx"}) out.add(str(x));
                }
            }
            if (JUMPS.contains(op)) {
                str t = last_word(l->args[0]);
                if (labels.contains(t)) nxt.push_back(labels.at(t));
                if (op == "jmp") nxt.erase(nxt.begin());
            }
            if (op == "ret" || op == "retf" || op == "retn" || op == "iret") nxt.clear();
        }
        for (int64_t n : nxt) todo.push_back(n);
    }
    _writes[label] = out;
    return out;
}

// the public routine (or the label called) a line belongs to: the nearest such label above it
Rt Port::routine(int64_t i) const {
    Rt best;
    for (const auto& kv : label_list) {
        const str& p = kv.first;
        int64_t j = kv.second;
        if (j <= i && (!best || j > labels.at(*best)) && (cname.contains(p) || called.contains(p))) best = p;
    }
    return best;
}

std::optional<State> Port::exit_of(const str& label) const {
    auto it = exits.find(label);
    const std::vector<State>* ex = (it != exits.end() && !it->second.empty()) ? &it->second : nullptr;
    if (!ex) {
        auto jt = exits_new.find(label);
        if (jt != exits_new.end()) ex = &jt->second;
    }
    if (!ex || ex->empty()) return std::nullopt;
    std::optional<State> s;
    for (const State& e : *ex) s = merge(s, e);
    return s;
}

Port& Port::run() {
    prepare();
    called = py::set<str>();
    for (const LineP& l : code)
        if (l->op && *l->op == "call") called.add(last_word(l->args[0]));
    for (const str& t : table_targets()) called.add(t);
    {
        std::set<Rt> add;
        for (int64_t i = 0; i < code.size(); ++i)
            if (code[i]->op && *code[i]->op == "retf") add.insert(routine(i));
        add.erase(Rt());
        far.insert(add.begin(), add.end());
    }
    for (int rnd = 0; rnd < 4; ++rnd) {                      // (then again, knowing the slots that hold addresses)
        for (int k = 0; k < 4; ++k) {                        // (the callees' exits, and again: each round looks up the round before's, whole)
            exits_new.clear();
            analyse();
            exits = exits_new;
        }
        auto found = own_slots();
        if (found == own_ptr) break;
        own_ptr = found;
    }
    _params();
    return *this;
}

// {label: bytes} of the module's own data (its data segments' labels)
py::dict<str, int64_t> Port::own_data() const {
    py::dict<str, int64_t> out;
    std::optional<str> cur;
    for (const auto& sl : data) {
        const LineP& l = std::get<1>(sl);
        if (l->directive) {
            const Directive& d = *l->directive;
            bool isd = one_of(d.kind, {"DB", "DW", "DD", "DQ", "DT"});
            if (isd && d.name) {
                cur = *d.name;
                out.set(*cur, 0);
            } else if (d.kind == "LABEL") {
                cur = d.name;
                out.set(*cur, 0);
            }
            if (isd && cur) {
                auto m = py::re::match(S(R"(\s*(\d+)\s+DUP)"), d.rest, py::re::I);
                int64_t unit = d.kind == "DB" ? 1 : d.kind == "DW" ? 2 : d.kind == "DD" ? 4 : d.kind == "DQ" ? 8 : 10;
                out[*cur] += (m ? py::int_parse(m->group(1)) : split_ops(strip_comment(d.rest)).size()) * unit;
            }
        }
    }
    return out;
}

// the words of the module's own data that are given an address (a whole register stored there): their (label, offset) -
// those alone, where no other access overlaps them
std::set<std::tuple<str, int64_t>> Port::own_slots() {
    py::dict<str, int64_t> own = own_data();
    std::map<str, std::set<std::tuple<int64_t, int64_t>>> acc;
    std::set<str> none_off;                                  // (an 'off' whose displacement is None: compared, a TypeError)
    std::set<std::tuple<str, int64_t>> ptr;
    for (int64_t i = 0; i < code.size(); ++i) {
        const LineP& l = code[i];
        auto it = before.find(i);
        if (it == before.end() || !l->op) continue;
        const State& st = it->second;
        std::vector<Operand> ops;
        for (const str& x : l->args) ops.push_back(operand(x, &equ));
        for (size_t k_ = 0; k_ < ops.size(); ++k_) {
            const Operand& o = ops[k_];
            if (o.kind == "off" && own.contains(o.text)) {
                if (o.disp_none) none_off.insert(o.text);
                acc[o.text].insert(std::make_tuple(o.disp, int64_t(0)));
            }
            if (o.kind != "mem" || !o.mem->sym || !own.contains(*o.mem->sym) || !o.mem->regs.empty()) continue;
            const Mem& m = *o.mem;
            const Operand* other = ops.size() == 2 ? &ops[1 - k_] : nullptr;
            int64_t size = (m.size && *m.size) ? *m.size : ((other && other->kind == "reg" && is_r8(other->text)) ? 1 : 2);
            acc[*m.sym].insert(std::make_tuple(m.disp, size));
            if ((*l->op == "mov" || *l->op == "xchg") && k_ == 0 && other && other->kind == "reg" && is_r16(other->text)) {
                auto kr = st.r.find(other->text);
                if (kr != st.r.end() && kr->second.wide() && kr->second.kind != "frame" && size == 2) ptr.insert(std::make_tuple(*m.sym, m.disp));
            }
        }
    }
    std::set<std::tuple<str, int64_t>> good;
    for (const auto& sd : ptr) {                             // (none other reaches into the slot)
        const str& sym_ = std::get<0>(sd);
        int64_t d = std::get<1>(sd);
        bool ok = true;
        if (none_off.count(sym_)) throw py::TypeError("'<' not supported between instances of 'NoneType' and 'int'");
        for (const auto& osz : acc[sym_]) {
            int64_t o = std::get<0>(osz), sz = std::get<1>(osz);
            if (!(!(o < d + 2 && d < o + std::max<int64_t>(sz, 1)) || (o == d && (sz == 2 || sz == 0)))) { ok = false; break; }
        }
        if (ok && used_as_address(sym_, d)) good.insert(sd);
    }
    return good;
}

// the evidence that a slot holds an address: what is read from it is then used as one (a register's base, the si or di
// of a string instruction), or it is compared with an address
bool Port::used_as_address(const str& sym_, int64_t d) {
    for (int64_t i = 0; i < code.size(); ++i) {
        const LineP& l = code[i];
        auto it = before.find(i);
        if (it == before.end() || !l->op) continue;
        const State& st = it->second;
        std::vector<Operand> ops;
        for (const str& x : l->args) ops.push_back(operand(x, &equ));
        std::vector<size_t> hit;
        for (size_t k = 0; k < ops.size(); ++k)
            if (ops[k].kind == "mem" && ops[k].mem->sym && *ops[k].mem->sym == sym_ && ops[k].mem->disp == d && ops[k].mem->regs.empty()) hit.push_back(k);
        if (hit.empty()) continue;
        if ((*l->op == "cmp" || *l->op == "sub") && ops.size() == 2 && ops[1 - hit[0]].kind == "reg") {
            const str& rn = ops[1 - hit[0]].text;
            auto k = st.r.find(rn);
            if ((k != st.r.end() && k->second.kind == "ptr") || (is_r16(rn) && address_use(i + 1, rn, 3, *std::make_unique<std::set<int64_t>>())))
                return true;                                 // (compared with an address)
        }
        if (*l->op == "mov" && hit[0] == 1 && ops[0].kind == "reg" && is_r16(ops[0].text)) {
            std::set<int64_t> seen;
            if (address_use(i + 1, ops[0].text, 3, seen)) return true;
        }
    }
    return false;
}

// whether register r, from line j0 on (into the routines called, depth levels), is used as an address before it is
// given another value
bool Port::address_use(int64_t j0, const str& r, int64_t depth, std::set<int64_t>& seen) {
    if (seen.count(j0)) return false;
    seen.insert(j0);
    for (int64_t j = j0; j < std::min<int64_t>(j0 + 60, code.size()); ++j) {
        const LineP& l2 = code[j];
        if (!l2->op) continue;
        for (const str& x : l2->args)
            if (py::re::search(S(R"(\[[^\]]*\b)") + r + S(R"(\b)"), x, py::re::I)) return true;   // (used as a base)
        const str& op = *l2->op;
        if (STRING.contains(op)) {
            str p4 = op.slice(0, 4);
            if ((r == "si" && (p4 == "lods" || p4 == "movs" || p4 == "cmps")) || (r == "di" && (p4 == "stos" || p4 == "movs" || p4 == "cmps" || p4 == "scas")))
                return true;                                 // (the string instruction's address)
        }
        if (op == "call" && depth && labels.contains(last_word(l2->args[0]))) {
            if (address_use(labels.at(last_word(l2->args[0])), r, depth - 1, seen)) return true;
        }
        if (!l2->args.empty() && l2->args[0].lower() == r && one_of(op, {"mov", "lea", "les", "lds", "xor"})) return false;   // (given another value; a pop gives back the one pushed)
        if (op == "ret" || op == "retf" || op == "jmp") return false;
    }
    return false;
}

// an old offset in the module's own data: the new one (the slots that hold addresses 8 bytes now)
int64_t Port::own_place(const str& sym_, int64_t disp) const {
    int64_t n = 0;
    for (const auto& sd : own_ptr)
        if (std::get<0>(sd) == sym_ && std::get<1>(sd) < disp) ++n;
    return disp + 6 * n;
}

// before the kinds: the table lines of the code placed (the listing's offsets); a short jump written as its two bytes
// (DB 74h / DB 18h) the jump again, to the label where the listing puts its target
// the operands of a line (parsed once: they are what they are until prepare() changes the lines)
py::list<Operand> Port::operands_of(const LineP& l) {
    auto it = opcache_.find(l.get());
    if (it != opcache_.end()) return it->second;
    py::list<Operand> a;
    for (const str& x : l->args) a.append(operand(x, &equ));
    opcache_[l.get()] = a;
    return a;
}

Port& Port::prepare() {
    opcache_.clear();
    place_data();
    static const py::dict<int64_t, str> jcc = {{0x70, S("jo")}, {0x71, S("jno")}, {0x72, S("jb")}, {0x73, S("jae")}, {0x74, S("je")},
                                               {0x75, S("jne")}, {0x76, S("jbe")}, {0x77, S("ja")}, {0x78, S("js")}, {0x79, S("jns")},
                                               {0x7C, S("jl")}, {0x7D, S("jge")}, {0x7E, S("jle")}, {0x7F, S("jg")}, {0xEB, S("jmp")}};
    for (int64_t i = 0; i + 1 < code.size(); ++i) {
        const LineP& l = code[i];
        const LineP& n = code[i + 1];
        if (!(l->directive && n->directive && l->directive->kind == "DB" && n->directive->kind == "DB" && dline.contains(i) && !n->label)) continue;
        auto op = number(strip_comment(l->directive->rest).strip());
        auto d = number(strip_comment(n->directive->rest).strip());
        if (!op || !jcc.contains(*op) || !d || !JUMPS.contains(jcc.at(*op))) continue;
        int64_t to = std::get<0>(dline.at(i)) + 2 + (*d >= 128 ? *d - 256 : *d);
        auto c_ = code_label(l->seg, to);
        if (c_ && std::get<1>(*c_) == 0) {
            l->op = jcc.at(*op);
            l->args = py::list<str>{std::get<0>(*c_)};
            l->directive = std::nullopt;
            n->directive = std::nullopt;
            notes.append(F("line %d: a jump written as its bytes - %s %s", l->no, jcc.at(*op), std::get<0>(*c_)));
        }
    }
    std::map<std::pair<str, int64_t>, ListLine> at;
    for (const ListLine& x : offsets.lines) at[std::make_pair(x.seg, x.offset)] = x;
    for (int64_t i = 0; i + 1 < code.size(); ++i) {          // (an instruction written as its bytes, running into the next
        const LineP& l = code[i];                            //  line's: the instruction, the next gone)
        const LineP& n = code[i + 1];
        if (!(l->directive && l->directive->kind == "DB" && dline.contains(i) && n->op && !n->label)) continue;
        std::string db;
        for (const str& x : split_ops(strip_comment(l->directive->rest))) {
            auto nx = number(x);
            if (nx) db.push_back(static_cast<char>(*nx & 0xFF));
        }
        auto itn = at.find(std::make_pair(*l->seg, std::get<0>(dline.at(i)) + static_cast<int64_t>(db.size())));
        if (itn == at.end()) continue;
        const ListLine& nxt = itn->second;
        auto ins = decode_direct(py::bytes(db + nxt.data.raw().substr(0, 4)));
        if (ins && static_cast<int64_t>(db.size()) < std::get<0>(*ins) &&
            std::get<0>(*ins) == static_cast<int64_t>(db.size()) + static_cast<int64_t>(nxt.data.raw().size())) {
            l->op = std::get<1>(*ins);
            l->args = std::get<2>(*ins);
            l->directive = std::nullopt;
            n->op = std::nullopt;
            n->args = py::list<str>();
            notes.append(F("line %d: an instruction written as its bytes, with the next line's - %s %s", l->no,
                           std::get<1>(*ins), py::join(S(", "), std::get<2>(*ins))));
        }
    }
    opcache_.clear();
    return *this;
}

// -- the parameters: where each routine reads its arguments on the stack

// {routine: [(old offset from the entry's sp, old size)]} of the arguments each reads, in order
void Port::_params() {
    std::map<Rt, std::map<int64_t, int64_t>> acc;
    for (int64_t i = 0; i < code.size(); ++i) {
        const LineP& l = code[i];
        auto it = before.find(i);
        if (it == before.end() || !l->op) continue;
        const State& st = it->second;
        for (const str& x : l->args) {
            Operand o = operand(x, &equ);
            if (o.kind != "mem" || o.mem->regs.size() != 1) continue;
            auto kt = st.r.find(o.mem->regs[0]);
            if (kt != st.r.end() && kt->second.kind == "frame") {
                const K& k = kt->second;
                int64_t e = k.old + o.mem->disp;
                Rt r = k.owner ? k.owner->routine : routine(i);
                int64_t size = (o.mem->size && *o.mem->size) ? *o.mem->size : ((*l->op == "les" || *l->op == "lds") ? 4 : 2);
                if (*l->op == "les" || *l->op == "lds") size = 4;
                auto& slot_ = acc[r];
                slot_[e] = std::max(slot_.count(e) ? slot_[e] : 0, size);
            }
        }
    }
    params.clear();
    for (const auto& rd : acc) {
        int64_t rs = retsize(rd.first);
        std::vector<std::tuple<int64_t, int64_t>> out;
        for (const auto& ed : rd.second) {                   // (sorted by offset)
            int64_t e = ed.first;
            if (e < rs) continue;
            if (!out.empty() && e < std::get<0>(out.back()) + std::get<1>(out.back())) continue;   // (the upper half of one read whole)
            out.push_back(std::make_tuple(e, ed.second));
        }
        params[rd.first] = out;
    }
}

int64_t Port::retsize(const Rt& r) const { return far.count(r) ? 4 : 2; }

// an argument's place: (slot number, offset inside it) of an old offset from the entry's sp
std::tuple<int64_t, int64_t> Port::slot(const Rt& r, int64_t e) const {
    auto it = params.find(r);
    if (it != params.end()) {
        for (size_t k = 0; k < it->second.size(); ++k) {
            int64_t o = std::get<0>(it->second[k]), s = std::get<1>(it->second[k]);
            if (o <= e && e < o + s) return {static_cast<int64_t>(k), e - o};
        }
    }
    throw Asm16Error(F("%s: no argument at sp+%d", r, e));
}

// -- the 64-bit text

// the module for ml64: data, an entry of today's C for each routine it calls, the routines
str Port::emit(const py::set<str>& longret) {
    used_int21 = false;
    ptr_args.clear();
    unreached = py::list<int64_t>();                         // (lines no path reaches: said in the report)
    py::list<str> out;
    out.append(S("; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the"));
    out.append(S("; registers that hold addresses used whole, members of structures at their new places, segments gone,"));
    out.append(S("; DOS called through the platform layer."));
    out.append(S("OPTION CASEMAP:NONE"));
    out.append(S(""));
    {
        std::vector<std::pair<str, str>> eqs;
        for (const auto& kv : equ.map.items()) eqs.push_back({std::get<0>(kv), std::get<1>(kv)});
        std::sort(eqs.begin(), eqs.end());
        for (const auto& kv : eqs)
            if (number(kv.second)) out.append(F("%s\tEQU\t%s", kv.first, kv.second));
    }
    py::list<str> ext;
    asm_calls = py::set<str>();
    c_calls = py::dict<str, CSig>();
    {
        std::vector<std::pair<str, str>> ex;
        for (const auto& kv : extrn.items()) ex.push_back({std::get<0>(kv), std::get<1>(kv)});
        std::sort(ex.begin(), ex.end());
        for (const auto& nk : ex) {
            const str& n = nk.first;
            const str& kind = nk.second;
            bool procish = kind == "NEAR" || kind == "FAR" || kind == "PROC";
            if (procish && (asm_routines.contains(n) || callee(n))) continue;   // (called: by its own name, or its C name, below)
            if (aliases.contains(c(n))) {                    // (a place inside another: the other is extern)
                str own = F("EXTERN %s:BYTE", std::get<0>(aliases.at(c(n))));
                if (!ext.contains(own)) ext.append(own);
                continue;
            }
            ext.append(F("EXTERN %s:%s", c(n), procish ? S("PROC") : kind));
        }
    }
    py::list<str> body;
    std::set<str> bases;
    auto dbg = py::os::getenv(S("CB_ASM16_DEBUG"));
    bool debug = dbg && *dbg == "1";                         // (every label public: a debugger names them)
    for (int64_t i = 0; i < code.size(); ++i) {
        const LineP& l = code[i];
        if ((debug || (l->label && public_.contains(*l->label) && labels.contains(*l->label))) && l->label && !l->directive)
            body.append(F("PUBLIC %s", local(*l->label)));   // (the other modules call it as it was)
        if (!bases.count(*l->seg)) {
            bases.insert(*l->seg);
            body.append(F("@base_%s:\t\t\t; (the start of the segment %s: what its tables count from)", *l->seg, *l->seg));
        }
        if (l->directive && one_of(l->directive->kind, {"DB", "DW", "DD", "LABEL"})) {
            for (const str& x : code_data(l)) body.append(x);
            continue;
        }
        if (l->label) body.append(F("%s:", local(*l->label)));
        if (l->op) {
            auto it = before.find(i);
            if (it == before.end()) {
                body.append(F("\t; (never reached) %s", strip_comment(l->text).strip()));
                unreached.append(l->no);
                continue;
            }
            try {
                for (const str& x : instr(i, l, it->second)) body.append(S("\t") + x);
            } catch (const Asm16Error& e) {
                throw Asm16Error(F("line %d (%s): %s", l->no, l->text.strip(), str(e.what())));
            }
        }
    }
    if (used_int21) ext.append(S("EXTERN dos_int21:PROC"));
    {
        std::vector<str> du;
        for (const str& n : data_used) du.push_back(n);
        std::sort(du.begin(), du.end());
        for (const str& n : du) {                            // (variables reached by their old addresses)
            if (!extrn.contains(n) && !public_.contains(n)) {
                str e_ = F("EXTERN %s:BYTE", sym_any(n));
                if (!ext.contains(e_)) ext.append(e_);
            }
        }
        std::vector<str> ac;
        for (const str& n : asm_calls) ac.push_back(n);
        std::sort(ac.begin(), ac.end());
        for (const str& n : ac) ext.append(F("EXTERN %s:PROC", n));
        std::vector<str> cc;
        for (const str& n : c_calls.keys()) cc.push_back(n);
        std::sort(cc.begin(), cc.end());
        for (const str& n : cc) ext.append(F("EXTERN %s:PROC", n));
    }
    py::list<str> wrappers;
    {
        std::vector<str> cc;
        for (const str& n : c_calls.keys()) cc.push_back(n);
        std::sort(cc.begin(), cc.end());
        for (const str& n : cc)
            for (const str& x : c_thunk(n, c_calls.at(n))) wrappers.append(x);
    }
    {
        std::vector<str> ps;
        for (const str& p : cname.keys()) ps.push_back(p);
        std::stable_sort(ps.begin(), ps.end(), [&](const str& x, const str& y) { return labels.at(x) < labels.at(y); });
        for (const str& p : ps)
            for (const str& x : wrapper(p, longret.contains(p))) wrappers.append(x);
    }
    if (used_null) ext.append(S("EXTERN dos_nullseg:BYTE"));
    for (const str& x : ext) out.append(x);
    out.append(S(""));
    out.append(S(".data"));
    if (used_int21) out.append(S("dosregs\tQWORD\t8 DUP (0)\t\t; (the registers given to DOS and back)"));
    for (const str& x : data_text()) out.append(x);
    out.append(S(""));
    out.append(S(".code"));
    for (const str& x : wrappers) out.append(x);
    for (const str& x : body) out.append(x);
    out.append(S(""));
    out.append(S("END"));
    return py::join(S("\n"), out) + S("\n");
}

// a table in the code segment: as it was - a word that is the place of a routine of the segment, that place (counted
// from the segment's start, as the old one was)
py::list<str> Port::code_data(const LineP& l) {
    const str& kind = l->directive->kind;
    const std::optional<str>& name = l->directive->name;
    str rest = strip_comment(l->directive->rest).strip();
    if (kind == "LABEL") {
        py::list<str> r;
        r.append(F("%s\tLABEL\t%s", name, rest));
        return r;
    }
    py::list<str> out;
    if (l->label) out.append(F("%s:", *l->label));
    int64_t idx = -1;
    for (int64_t q = 0; q < code.size(); ++q)
        if (code[q] == l) { idx = q; break; }
    if (idx < 0) throw py::ValueError("x is not in list");
    if (dline.contains(idx)) out.append(F("@d_%04x:", std::get<0>(dline.at(idx))));
    if (kind == "DW") {
        py::list<str> vals;
        for (const str& v : split_ops(rest)) {
            auto nv = number(v);
            std::optional<std::tuple<str, int64_t>> c_ = nv ? code_label(l->seg, *nv) : std::nullopt;
            auto o = py::re::fullmatch(S(R"((?i)offset\s+(?:\w+:)?(\w+))"), v.strip());
            if (o && labels.contains(o->group(1))) {         // (a routine's place, named: from the segment's start)
                vals.append(F("%s - @base_%s", local(o->group(1)), *l->seg));
                continue;
            }
            vals.append((c_ && std::get<1>(*c_) == 0) ? F("%s - @base_%s", std::get<0>(*c_), *l->seg) : v);
        }
        out.append(F("\tDW\t%s", py::join(S(", "), vals)));
        return out;
    }
    out.append(F("\t%s\t%s", kind, rest));
    return out;
}

str Port::local(const str& label) const {
    return (cname.contains(label) && cname.at(label) == label) ? S("@") + label : label;
}

py::list<str> Port::data_text() {
    py::list<str> out;
    for (const auto& sl : data) {
        const LineP& l = std::get<1>(sl);
        if (!l->directive) {
            if (l->label) out.append(F("%s:", *l->label));
            continue;
        }
        const str& kind = l->directive->kind;
        const std::optional<str>& name = l->directive->name;
        if (one_of(kind, {"PUBLIC", "EXTRN", "ASSUME", "TITLE", "END"})) continue;
        bool ispub = name && public_.contains(*name);
        std::optional<str> nm = ispub ? std::optional<str>(c(*name)) : name;
        py::list<str> pb;
        if (ispub) pb.append(F("PUBLIC %s", *nm));
        str rest = strip_comment(l->directive->rest).strip();
        bool dkind = one_of(kind, {"DB", "DW", "DD", "DQ", "DT"});
        bool owned = false;
        if (name) for (const auto& sd : own_ptr) if (std::get<0>(sd) == *name) owned = true;
        if (kind == "LABEL") {
            for (const str& x : pb) out.append(x);
            out.append(F("%s\tLABEL\t%s", nm, rest));
        } else if (dkind && name && owned) {                 // (room for the addresses it holds, 8 bytes each)
            int64_t cnt = 0;
            for (const auto& sd : own_ptr) if (std::get<0>(sd) == *name) ++cnt;
            int64_t grow = 6 * cnt;
            for (const str& x : pb) out.append(x);
            out.append(F("%s\t%s\t%s\t\t; (%d bytes more: %d words of it hold addresses)", *nm, kind, rest, grow, grow / 6));
            out.append(F("\tDB\t%d DUP (0)", grow));
        } else if (dkind) {
            for (const str& x : pb) out.append(x);
            if (kind == "DW" && py::re::search(S(R"((?i)\boffset\b)"), rest)) {
                py::list<str> vals;
                for (const str& v : split_ops(rest)) vals.append(py::re::sub(S(R"((?i)offset\s+(?:\w+:)?)"), S(""), v).strip());
                py::list<str> cv;
                for (const str& v : vals) cv.append(v.startswith(S("_")) ? c(v) : v);
                out.append(F("%s\tDQ\t%s", nm ? *nm : S(""), py::join(S(", "), cv)));
            } else {
                out.append(F("%s\t%s\t%s", nm ? *nm : S(""), kind, rest));
            }
        }
    }
    return out;
}

// the entry of today's C: its arguments (rcx, rdx, r8, r9, then the stack) pushed as the old caller pushed them, the
// registers today's C keeps kept
py::list<str> Port::wrapper(const str& p, bool longret) {
    auto pit = params.find(Rt(p));
    int64_t n = pit == params.end() ? 0 : static_cast<int64_t>(pit->second.size());
    bool pasc = pascal.contains(p);
    static const char* saved[5] = {"rbx", "rsi", "rdi", "rbp", "r15"};
    py::list<str> args_;
    for (int64_t j = 0; j < n; ++j) args_.append(F("a%d", j));
    py::list<str> out;
    out.append(S(""));
    out.append(F("; %s(%s) - %s", cname.at(p), py::join(S(", "), args_),
                 pasc ? "pascal: the first argument pushed first, the routine takes them off" : "the last argument pushed first, taken off after"));
    out.append(F("PUBLIC %s", cname.at(p)));
    out.append(F("%s PROC", cname.at(p)));
    for (const char* r : saved) out.append(F("\tpush\t%s", r));
    static const char* regs[4] = {"rcx", "rdx", "r8", "r9"};
    int64_t pushed = 0;
    std::set<int64_t> ptrs;
    if (pit != params.end()) {
        auto pa = ptr_args.find(Rt(p));
        for (size_t k = 0; k < pit->second.size(); ++k)
            if (pa != ptr_args.end() && pa->second.contains(std::get<0>(pit->second[k]))) ptrs.insert(static_cast<int64_t>(k));
    }
    std::optional<std::vector<std::pair<int64_t, int64_t>>> words_;   // (word k of the old stack: (C argument, its half))
    std::vector<std::tuple<int64_t, std::optional<str>>> cargs;
    if (param_types.contains(p)) for (const auto& x : param_types.at(p)) cargs.push_back(x);
    // (cargs: the sizes and what each points to)
    std::vector<std::pair<int64_t, bool>> csz;
    if (param_types.contains(p)) for (const auto& x : param_types.at(p)) csz.push_back({std::get<0>(x), static_cast<bool>(std::get<1>(x) && !std::get<1>(x)->empty())});
    if (!pasc && !csz.empty()) {
        bool any4 = false;
        int64_t sum = 0;
        for (const auto& x : csz) { if (x.first == 4) any4 = true; sum += x.first == 4 ? 2 : 1; }
        if (any4 && sum == n) {
            std::vector<std::pair<int64_t, int64_t>> w;      // (a long of the C: two words, the low one first)
            for (size_t i = 0; i < csz.size(); ++i) {
                if (csz[i].first == 4 && !csz[i].second) { w.push_back({int64_t(i), 0}); w.push_back({int64_t(i), 1}); }
                else w.push_back({int64_t(i), 0});
            }
            if (static_cast<int64_t>(w.size()) == n) words_ = w;
        }
    }
    for (int64_t k = n - 1; k >= 0; --k) {                   // (slot 0 at the lowest address, as the old stack had it)
        int64_t j, half = 0;
        if (words_) { j = (*words_)[k].first; half = (*words_)[k].second; }
        else j = pasc ? (n - 1 - k) : k;
        str reg_;
        if (j < 4) {
            reg_ = S(regs[j]);
        } else {
            int64_t off = 8 * 5 + 8 * pushed + 8 + 32 + 8 * (j - 4);
            out.append(F("\tmov\tr11, qword ptr [rsp+%d]", off));
            reg_ = S("r11");
        }
        if (half) {                                          // (the high word of a long: its own slot)
            out.append(F("\tmov\tr11, %s", reg_));
            out.append(S("\tshr\tr11, 16"));
            reg_ = S("r11");
        }
        if (ptrs.count(k)) {                                 // (a NULL address read what began the data segment)
            str lab = F("@@null_%s_%d", cname.at(p), k);
            out.append(F("\ttest\t%s, %s", reg_, reg_));
            out.append(F("\tjnz\t%s", lab));
            out.append(F("\tlea\t%s, dos_nullseg", reg_));
            out.append(F("%s:", lab));
            used_null = true;
        }
        out.append(F("\tpush\t%s", reg_));
        ++pushed;
    }
    out.append(F("\tcall\t%s", local(p)));
    if (!pasc && n) out.append(F("\tadd\trsp, %d", 8 * n));
    if (longret) {
        out.append(S("\tmovzx\teax, ax"));
        out.append(S("\tshl\tedx, 16"));
        out.append(S("\tor\teax, edx\t\t; (a long: dx:ax)"));
    }
    for (int r = 4; r >= 0; --r) out.append(F("\tpop\t%s", saved[r]));
    out.append(S("\tcld"));
    out.append(S("\tret"));
    out.append(F("%s ENDP", cname.at(p)));
    return out;
}

// -- one instruction

str Port::reg(const str& r, const State& st, std::optional<bool> wide) const {
    if (is_r8(r)) return r;
    auto it = st.r.find(r);
    bool w = (!wide && it != st.r.end()) ? it->second.wide() : wide.value_or(false);
    return w ? R64.at(r) : r;
}

// the C name of a variable of the program, whether the module declares it or not
str Port::sym_any(const str& s) const {
    str c_ = c(s);
    if (aliases.contains(c_)) return std::get<0>(aliases.at(c_));
    return c_;
}

str Port::sym(const str& s) const {
    if (extrn.contains(s) || public_.contains(s) || data_used.contains(s)) {
        str c_ = c(s);
        if (aliases.contains(c_)) {
            const auto& oo = aliases.at(c_);
            return std::get<1>(oo) ? F("%s+%d", std::get<0>(oo), std::get<1>(oo)) : std::get<0>(oo);
        }
        return c_;
    }
    return s;
}

namespace {

int64_t floordiv_(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
int64_t floormod_(int64_t a, int64_t b) { return a - floordiv_(a, b) * b; }
int64_t bit_length_(int64_t v) { int64_t n = 0; while (v > 0) { ++n; v >>= 1; } return n; }
str signed_(int64_t d) { return d ? F("%+d", d) : S(""); }

}  // namespace

// a memory operand at its new place: text (with its size), the instructions it needs first in pre
str Port::mem(const Mem& m0, const State& st, int64_t size, int64_t i, py::list<str>& pre) {
    str sz = ptr_word(size);
    Mem m = absolute(m0);
    int64_t disp = m.disp;
    std::vector<str> parts;
    std::vector<str> scratch{S("r11"), S("r9")};
    auto find = [&](const str& r) -> const K* {
        auto it = st.r.find(r);
        return it == st.r.end() ? nullptr : &it->second;
    };
    if (m.sym) {
        str name = sym(*m.sym);
        disp = own_place(*m.sym, disp);
        if (m.regs.empty()) return sz + name + signed_(disp);
        pre.append(F("lea\tr10, %s", name));
        parts.push_back(S("r10"));
        std::optional<std::tuple<int64_t, int64_t>> es_;
        if (extrn.contains(*m.sym) || public_.contains(*m.sym)) es_ = ct.elem_sizes(c(*m.sym));
        const K* k_ = m.regs.size() == 1 ? find(m.regs[0]) : nullptr;
        if (es_ && std::get<0>(*es_) != std::get<1>(*es_) && m.regs.size() == 1 && !(k_ && k_->wide())) {
            int64_t so = std::get<0>(*es_), sn = std::get<1>(*es_);   // (an index of the old elements: today's)
            if (so & (so - 1)) throw Asm16Error(F("an index into elements of %d bytes", so));
            pre.append(F("movzx\tr11d, %s", m.regs[0]));
            pre.append(F("imul\tr11, r11, %d", sn));
            pre.append(F("sar\tr11, %d\t\t; (%d bytes an element then, %d now)", bit_length_(so) - 1, so, sn));
            disp = floordiv_(disp, so) * sn + floormod_(disp, so);
            return sz + S("[r10+r11") + signed_(disp) + S("]");
        }
    }
    if (m.regs.size() == 1 && !m.sym) {
        const K* k = find(m.regs[0]);
        if (k && k->kind == "ptr" && k->to && k->to->startswith(S("@ds:"))) return mem(ds_mem(m, *k), st, size, i, pre);
    }
    for (const str& r : m.regs) {
        const K* k = find(r);
        as_address(i, k ? std::optional<K>(*k) : std::nullopt);
        if (k && k->kind == "frame") {
            Rt rt = k->owner ? k->owner->routine : routine(i);
            int64_t e = k->old + disp;
            int64_t nw = 0;
            if (e >= retsize(rt)) {
                auto sl = slot(rt, e);
                nw = 8 + 8 * std::get<0>(sl) + std::get<1>(sl);
            } else if (k->owner) {                           // (the owner's stack, as it was when it called)
                int64_t o_ = 0, n_ = 0;
                bool got = false;
                for (const auto& sn : k->owner->stack) {
                    if (o_ - sn.first <= e && e < o_) {
                        nw = n_ - sn.second + (e - (o_ - sn.first));
                        got = true;
                        break;
                    }
                    o_ -= sn.first;
                    n_ -= sn.second;
                }
                if (!got) throw Asm16Error(F("sp%+d is not on the stack of %s", e, rt));
            } else {
                nw = stack_place(st, e);
            }
            disp = nw - k->new_;
            parts.push_back(R64.at(r));
        } else if (k && k->wide()) {
            if (k->kind == "ptr" && k->to && k->to->endswith(S("*")) && m.regs.size() == 1 && !m.sym) {
                disp = disp * 4;                             // (pointers of 2 bytes, 8 now)
            } else if (k->kind == "ptr" && k->to && k->to->startswith(std::make_tuple("struct", "union")) && m.regs.size() == 1 && !m.sym) {
                auto f = ct.field(*k->to, disp);
                if (f) disp = std::get<3>(*f) + (disp - std::get<1>(*f));
            }
            parts.push_back(R64.at(r));
        } else {
            if (scratch.empty()) throw py::IndexError("pop from empty list");
            str s = scratch.front();
            scratch.erase(scratch.begin());
            pre.append(F("movzx\t%sd, %s", s, r));
            parts.push_back(s);
        }
    }
    py::list<str> pl;
    for (const str& x : parts) pl.append(x);
    return sz + S("[") + py::join(S("+"), pl) + signed_(disp) + S("]");
}

// an argument of the routine used as an address here: noted (its NULL is the old data segment's start)
void Port::as_address(int64_t i, const std::optional<K>& k) {
    if (!k) return;
    for (const Origin& o : k->origin)
        if (o.kind == "param") ptr_args[routine(i)].add(o.n);
}

// an old offset below the entry's sp: its new one (the pushes and the room made, as they are now)
int64_t Port::stack_place(const State& st, int64_t e) const {
    int64_t o = 0, n = 0;
    for (const auto& it : st.stack) {
        if (o - it.old <= e && e < o) return n - it.new_ + (e - (o - it.old));   // (the same byte of the item: a push now 8 bytes)
        o -= it.old;
        n -= it.new_;
    }
    throw Asm16Error(F("sp%+d is not on the stack as it is here", e));
}

// the size the instruction's memory operand is used at, now
int64_t Port::access_size(const LineP& l, const py::list<Operand>& a, const State& st, int64_t which) {
    (void)l;
    const Mem& m = *a[which].mem;
    auto kf = kind_of_mem(m, st);
    const K& k = std::get<0>(kf);
    const std::optional<Field>& f = std::get<1>(kf);
    const Operand* other = a.size() > 1 ? &a[1 - which] : nullptr;
    if (other && other->kind == "reg") {
        const str& r = other->text;
        if (is_r8(r)) return 1;
        auto ko = st.r.find(r);
        if (ko != st.r.end() && ko->second.wide() && ko->second.kind != "frame") {
            bool frame_base = false;
            if (!m.regs.empty()) {
                auto kb = st.r.find(m.regs[0]);
                frame_base = kb != st.r.end() && kb->second.kind == "frame";
            }
            if (k.wide() || (f && std::get<5>(*f)) || frame_base) return 8;
        }
        return 2;
    }
    if (m.size && *m.size) return *m.size;
    return 2;
}

// add/sub sp, n: the bytes it is now (the pushes it takes off are 8 bytes each, room made the same)
int64_t Port::sp_new_bytes(const State& st, int64_t n, const str& op) const {
    if (op == "sub") return n;
    std::vector<StackItem> items = st.stack;
    int64_t left = n, nw = 0;
    while (left > 0 && !items.empty()) {
        StackItem it = items.back();
        items.pop_back();
        left -= it.old;
        nw += it.new_;
    }
    return nw;
}

py::list<str> Port::instr(int64_t i, const LineP& l, const State& st) {
    const str& op = *l->op;
    py::list<Operand> a = operands_of(l);
    py::list<str> pre;
    auto stk = [&](const str& r) -> const K* {
        auto it = st.r.find(r);
        return it == st.r.end() ? nullptr : &it->second;
    };
    // R(r, w)
    auto R = [&](const str& r, std::optional<bool> w = std::nullopt) { return reg(r, st, w); };
    // M(which, size)
    auto M = [&](int64_t which, std::optional<int64_t> size = std::nullopt) {
        int64_t sz = size ? *size : access_size(l, a, st, which);
        return mem(*a[which].mem, st, sz, i, pre);
    };
    auto done = [&](std::initializer_list<str> lines_) {
        py::list<str> out = pre.copy();
        for (const str& x : lines_) out.append(x);
        return out;
    };
    auto one = [&](const str& x) {
        py::list<str> o;
        o.append(x);
        return o;
    };
    auto many = [&](std::initializer_list<str> xs) {
        py::list<str> o;
        for (const str& x : xs) o.append(x);
        return o;
    };
    auto segop = [](const Operand& o) { return o.kind == "reg" && SEG.contains(o.text); };
    str trimmed = strip_comment(l->text).strip();

    if (op == "mov" && a[0].kind == "reg" && is_r16(a[0].text) && a[1].kind == "reg" && (a[1].text == "es" || a[1].text == "ds") &&
        st.r.count(S("@") + a[1].text)) {
        std::optional<str> partner;
        for (const Origin& o : st.r.at(S("@") + a[1].text).origin) if (o.kind == "segof") { partner = o.a; break; }
        const K* pk = partner ? stk(*partner) : nullptr;
        if (pk && pk->wide())
            return one(F("mov\t%s, %s\t\t; (the segment of the address in %s: the address, 0 when it is)", R64.at(a[0].text), R64.at(*partner), *partner));
    }
    if (op == "mov" && (segop(a[0]) || segop(a[1]))) return one(F("; %s\t\t(a segment: one flat memory)", trimmed));
    if ((op == "push" || op == "pop") && segop(a[0])) return one(F("; %s\t\t(a segment: one flat memory)", trimmed));
    std::optional<int64_t> src_at;
    if (op == "push") src_at = 0;
    else if (one_of(op, {"or", "and", "add", "sub", "cmp", "adc", "sbb", "xor", "test"}) && a.size() == 2 && a[0].kind == "reg") src_at = 1;
    if (src_at && a[*src_at].kind == "mem") {                // (the segment half of a far address read: none now, 0 - the
        auto kf = kind_of_mem(*a[*src_at].mem, st);          //  address is whole in the other half's place)
        if (std::get<0>(kf).kind == "seg") {
            if (op == "push") return one(F("push\t0\t\t; (%s: a segment, none now)", trimmed));
            const K* kd_ = stk(a[0].text);
            str d_ = (is_r16(a[0].text) && kd_ && kd_->wide()) ? R64.at(a[0].text) : a[0].text;
            return one(F("%s\t%s, 0\t\t; (%s: a segment, none now)", op, d_, trimmed));
        }
    }
    if (JUMPS.contains(op)) {
        str t = last_word(l->args[0]);
        if (op == "jcxz" || op.startswith(S("loop"))) {      // (a short jump only, and the flags kept: through a jump beside it
            str n = F("@@j%d", i);                           //  to the place, however far)
            return many({S("movzx\tecx, cx"), F("%s\t%s_t", op == "jcxz" ? S("jrcxz") : op, n), F("jmp\t%s_n", n), F("%s_t:", n),
                         F("jmp\t%s", t), F("%s_n:", n)});
        }
        return one(F("%s\t%s", op, t));
    }
    if (op == "call") {
        str t = last_word(l->args[0]);
        if (labels.contains(t)) return one(F("call\t%s", local(t)));
        if (a[0].kind == "mem" && a[0].mem->seg && *a[0].mem->seg == "cs") {   // (through a table of the code: its entries are the
            py::list<str> pre2;                              //  routines' places from the segment's start)
            str where = mem(*a[0].mem, st, 2, i, pre2);
            py::list<str> out = pre2.copy();
            out.append(F("movzx\tr11d, %s", where));
            out.append(F("lea\tr10, @base_%s", *l->seg));
            out.append(S("add\tr11, r10"));
            out.append(S("call\tr11"));
            return out;
        }
        auto ce = callee(t);
        if (ce && ce->kind == "asm") {
            asm_calls.add(ce->name);
            return one(F("call\t%s", ce->name));
        }
        if (ce) {
            c_calls.set(ce->name, ce->sig);
            return one(F("call\t@@c_%s", ce->name));
        }
        throw Asm16Error(F("a call of %s, outside the module (not yet ported)", t));
    }
    if (op == "ret" || op == "retn" || op == "retf") {
        if (!l->args.empty()) {
            Rt rt = routine(i);
            auto nbytes = number(l->args[0]);
            if (!nbytes) throw py::TypeError("unsupported operand type(s) for +: 'int' and 'NoneType'");
            int64_t n = 0;
            auto pit = params.find(rt);
            if (pit != params.end())
                for (const auto& os : pit->second)
                    if (std::get<0>(os) < retsize(rt) + *nbytes) ++n;
            return one(F("ret\t%d", 8 * n));
        }
        return one(S("ret"));
    }
    if (op == "int") {
        auto n = number(l->args[0]);
        if (!n || *n != 0x21) throw Asm16Error(F("int %s", l->args[0]));
        used_int21 = true;
        return many({S("mov\tqword ptr dosregs, rax"), S("mov\tqword ptr dosregs+8, rbx"), S("mov\tqword ptr dosregs+16, rcx"),
                     S("mov\tqword ptr dosregs+24, rdx"), S("mov\tqword ptr dosregs+32, rsi"), S("mov\tqword ptr dosregs+40, rdi"),
                     S("mov\tr15, rsp\t\t; (DOS: the platform layer, called as today's C is)"), S("and\trsp, -16"), S("sub\trsp, 32"),
                     S("lea\trcx, dosregs"), S("call\tdos_int21"), S("mov\trsp, r15"), S("mov\trax, qword ptr dosregs"),
                     S("mov\trbx, qword ptr dosregs+8"), S("mov\trcx, qword ptr dosregs+16"), S("mov\trdx, qword ptr dosregs+24"),
                     S("bt\tqword ptr dosregs+48, 0\t; (the carry: an error)")});
    }
    if (op == "xlat" || op == "xlatb") {                     // (al from the table at bx: the table's address whole)
        const K* k = stk(S("bx"));
        if (!(k && k->wide())) throw Asm16Error("xlat with bx not an address");
        return one(S("xlatb"));
    }
    if (op == "pushf" || op == "pushfw") return one(S("pushfq"));
    if (op == "popf" || op == "popfw") return one(S("popfq"));
    if (op == "push") {
        if (a[0].kind == "reg") return one(F("push\t%s", R64.at(a[0].text)));
        if (a[0].kind == "mem") {
            auto kf = kind_of_mem(*a[0].mem, st);
            if (std::get<0>(kf).wide()) {
                str m8 = M(0, 8);
                return done({S("push\t") + m8});
            }
            str m2 = M(0, 2);
            return done({S("movzx\tr11d, ") + m2, S("push\tr11")});
        }
        if (a[0].kind == "off") return many({F("lea\tr11, %s", sym(a[0].text)), S("push\tr11")});
        return one(F("push\t%s", l->args[0]));
    }
    if (op == "pop") {
        if (a[0].kind == "reg") return one(F("pop\t%s", R64.at(a[0].text)));
        throw Asm16Error("pop to memory");
    }
    if (op == "les" || op == "lds") {
        str m8 = M(1, 8);
        return done({F("mov\t%s, %s", R64.at(a[0].text), m8)});
    }
    if (op == "lea") {
        str m8 = M(1, 8);
        return done({F("lea\t%s, %s", R64.at(a[0].text), m8.slice(str("qword ptr ").size(), m8.size()))});
    }
    if ((op == "mov" || op == "xchg") && a[0].kind == "reg" && a[1].kind == "reg") {
        str d = a[0].text, s = a[1].text;
        if (s == "sp") return one(F("%s\t%s, rsp", op, R64.at(d)));
        if (is_r8(d) || is_r8(s)) return one(F("%s\t%s, %s", op, d, s));
        const K* ks = stk(s);
        const K* kd = stk(d);
        // w = (ks and ks.wide()) or (op == "xchg" and kd and kd.wide()): None, False or True
        std::optional<bool> w;
        bool first = ks && ks->wide();
        if (first) w = true;
        else if (op != "xchg") w = false;
        else if (!kd) w = std::nullopt;
        else w = kd->wide();
        return one(F("%s\t%s, %s", op, R(d, w), R(s, w)));
    }
    if ((op == "mov" || op == "xchg") && a[0].kind == "reg" && a[1].kind == "mem") {
        str d = a[0].text;
        auto kf = kind_of_mem(*a[1].mem, st);
        const K& k = std::get<0>(kf);
        if (k.kind == "seg" && op == "mov" && is_r16(d)) {   // (the segment of a far address read: 0 when the address is 0,
            const Mem& m_ = *a[1].mem;                       //  else not - what it was tested for)
            Mem lo(m_.size, m_.seg, m_.sym, m_.regs, m_.disp - 2, m_.text);
            str where = mem(lo, st, 8, i, pre);
            return done({F("mov\tr11, %s", where), S("test\tr11, r11"),
                         F("setnz\tr11b\t\t; (%s: the segment of an address - 0 when it is 0)", trimmed), F("movzx\te%s, r11b", d)});
        }
        if (k.kind == "seg") return one(F("; %s\t\t(a segment: one flat memory)", trimmed));
        if (is_r8(d)) {
            str m1 = M(1, 1);
            return done({F("%s\t%s, %s", op, d, m1)});
        }
        if (k.wide() && d != "cx") {                         // (cx: a count, whatever the member was declared)
            str m8 = M(1, 8);
            return done({F("%s\t%s, %s", op, R64.at(d), m8)});
        }
        str m2 = M(1, 2);
        return done({F("%s\t%s, %s", op, d, m2)});
    }
    if ((op == "mov" || op == "xchg") && a[0].kind == "mem" && a[1].kind == "reg") {
        str s = a[1].text;
        auto kf = kind_of_mem(*a[0].mem, st);
        const K& k = std::get<0>(kf);
        const std::optional<Field>& f = std::get<1>(kf);
        if (k.kind == "seg") return one(F("; %s\t\t(a segment: one flat memory)", trimmed));
        int64_t size = access_size(l, a, st, 0);
        if (size == 2 && f && std::get<5>(*f) && std::get<2>(*f) == 4 && std::get<4>(*f) == 8 && is_r16(s) && op == "mov") {
            str m8 = M(0, 8);
            return done({F("movzx\tr11d, %s\t\t; (the offset half of a far address: the address now)", s), F("mov\t%s, r11", m8)});
        }
        str ms = M(0, size);
        return done({F("%s\t%s, %s", op, ms, size == 8 ? R64.at(s) : s)});
    }
    if (op == "mov" && a[0].kind == "reg" && a[1].kind == "off") {
        if (a[1].disp_none) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        int64_t dd = own_place(a[1].text, a[1].disp);
        return one(F("lea\t%s, %s", R64.at(a[0].text), sym(a[1].text) + signed_(dd)));
    }
    std::optional<K> k_after;
    if (op == "mov" && a[0].kind == "reg" && a[1].kind == "imm" && i + 1 < code.size()) {
        auto bt = before.find(i + 1);
        if (bt != before.end()) {
            auto it = bt->second.r.find(a[0].text);
            if (it != bt->second.r.end()) k_after = it->second;
        }
    }
    if (k_after && k_after->kind == "ptr" && k_after->to && k_after->to->startswith(S("@ds:"))) {
        auto v = data_var(py::int_parse(k_after->to->slice(4, k_after->to->size()), 16));
        if (!v) throw py::TypeError("'NoneType' object is not subscriptable");
        data_used.add(std::get<0>(*v));
        return one(F("lea\t%s, %s%s\t\t; (%s: the old address of %s)", R64.at(a[0].text), sym(std::get<0>(*v)),
                     std::get<1>(*v) ? F("%+d", std::get<1>(*v)) : S(""), l->args[1], c(std::get<0>(*v))));
    }
    if (k_after && k_after->kind == "ptr" && k_after->to && *k_after->to == "code") {
        auto v = number(l->args[1]);
        if (!v) throw py::TypeError("unsupported operand type(s)");
        auto ld = data_address(*l->seg, *v);
        if (!ld) ld = code_label(l->seg, *v);
        if (!ld) throw py::TypeError("cannot unpack non-iterable NoneType object");
        return one(F("lea\t%s, %s%s\t\t; (%s: an address in the code)", R64.at(a[0].text), std::get<0>(*ld),
                     std::get<1>(*ld) ? F("%+d", std::get<1>(*ld)) : S(""), l->args[1]));
    }
    if (op == "mov" && a[0].kind == "reg" && a[1].kind == "sym" &&
        (py::re::match(S(R"((?i)seg\s)"), a[1].text) || a[1].text.upper() == "DGROUP" || code_seg.contains(a[1].text)))
        return one(F("; %s\t\t(a segment: one flat memory)", trimmed));
    if (op == "mov" && a[0].kind == "reg" && (a[1].kind == "imm" || a[1].kind == "sym")) return one(F("mov\t%s, %s", a[0].text, l->args[1]));
    if (op == "mov" && a[0].kind == "mem" && a[1].kind == "off") {   // (an address stored: the whole of it)
        if (a[1].disp_none) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        int64_t dd = own_place(a[1].text, a[1].disp);
        str m8 = M(0, 8);
        return done({F("lea\tr11, %s", sym(a[1].text) + signed_(dd)), F("mov\t%s, r11", m8)});
    }
    if (op == "mov" && a[0].kind == "mem") {
        auto kf = kind_of_mem(*a[0].mem, st);
        const K& k = std::get<0>(kf);
        const std::optional<Field>& f = std::get<1>(kf);
        int64_t size = (a[0].mem->size && *a[0].mem->size) ? *a[0].mem->size : 2;
        auto n0 = number(l->args[1]);
        if (k.kind == "ptr" && f && std::get<5>(*f) && n0 && *n0 == 0) size = 8;
        str ms = M(0, size);
        return done({F("mov\t%s, %s", ms, l->args[1])});
    }
    if (one_of(op, {"add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb"}) && a[0].kind == "reg") {
        str d = a[0].text;
        if (d == "sp") {
            int64_t n = number(l->args[1]).value_or(0);
            return one(F("%s\trsp, %d", op, sp_new_bytes(st, n, op)));
        }
        const K* kd = is_r16(d) ? stk(d) : nullptr;
        if (op == "xor" && a[1] == a[0] && is_r16(d)) return one(F("xor\te%s, e%s", d, d));
        if (a[1].kind == "imm" && op == "sub" && is_r16(d) && !(kd && kd->wide()) && data_at(number(l->args[1]))) {
            str v = *data_at(number(l->args[1]));            // (an offset less a variable's old address: today's, the low 16
            data_used.add(v);                                //  bits - the same difference)
            return many({F("lea\tr11, %s\t\t; (%s: the old address of %s)", sym(v), l->args[1], c(v)), F("sub\t%s, r11w", d)});
        }
        if (a[1].kind == "imm" && (op == "sub" || op == "cmp" || op == "add") && kd && kd->wide() && data_at(number(l->args[1]))) {
            str v = *data_at(number(l->args[1]));            // (the old address of a variable: its address now)
            data_used.add(v);
            return many({F("lea\tr11, %s\t\t; (%s: the old address of %s)", sym(v), l->args[1], c(v)), F("%s\t%s, r11", op, R64.at(d))});
        }
        if (is_r8(d)) {
            if (a[1].kind == "mem") {
                str m1 = M(1, 1);
                return done({F("%s\t%s, %s", op, d, m1)});
            }
            return one(F("%s\t%s, %s", op, d, l->args[1]));
        }
        Operand src = a[1];
        if (src.kind == "reg") {
            const K* ks = is_r16(src.text) ? stk(src.text) : nullptr;
            if (is_r8(src.text)) return one(F("%s\t%s, %s", op, d, src.text));
            if (kd && kd->wide() && ks && ks->wide()) return one(F("%s\t%s, %s", op, R64.at(d), R64.at(src.text)));
            if (kd && kd->wide() && kd->kind != "frame" && (op == "add" || op == "sub")) {
                auto sn_ = element_sizes(*kd);
                int64_t so = std::get<0>(sn_), sn = std::get<1>(sn_);
                if (so != sn) {                              // (an index into an array: the elements' size now)
                    py::list<str> out_ = many({F("movsx\tr11, %s", src.text), F("imul\tr11, r11, %d", sn)});
                    if ((so & (so - 1)) == 0) out_.append(F("sar\tr11, %d\t\t; (%d bytes an element then, %d now)", bit_length_(so) - 1, so, sn));
                    else throw Asm16Error(F("an index into elements of %d bytes", so));
                    out_.append(F("%s\t%s, r11", op, R64.at(d)));
                    return out_;
                }
                return many({F("movsx\tr11, %s", src.text), F("%s\t%s, r11", op, R64.at(d))});
            }
            if (op == "add" && ks && ks->wide() && ks->kind != "frame" && (!kd || !kd->wide())) {
                auto sn_ = element_sizes(*ks);
                int64_t so = std::get<0>(sn_), sn = std::get<1>(sn_);
                if (so != sn && (so & (so - 1)) == 0)        // (an index of the old elements: today's)
                    return many({F("movzx\te%s, %s", d, d), F("imul\t%s, %s, %d", R64.at(d), R64.at(d), sn),
                                 F("sar\t%s, %d\t\t; (%d bytes an element then, %d now)", R64.at(d), bit_length_(so) - 1, so, sn),
                                 F("add\t%s, %s\t\t; (a count added to an address: the address)", R64.at(d), R64.at(src.text))});
                return many({F("movzx\te%s, %s", d, d), F("add\t%s, %s\t\t; (a count added to an address: the address)", R64.at(d), R64.at(src.text))});
            }
            return one(F("%s\t%s, %s", op, d, src.text));
        }
        if (src.kind == "mem") {
            auto kf = kind_of_mem(*src.mem, st);
            const K& k = std::get<0>(kf);
            if (kd && kd->wide() && k.wide()) {
                str m8 = M(1, 8);
                return done({F("%s\t%s, %s", op, R64.at(d), m8)});
            }
            if (kd && kd->wide() && kd->kind != "frame" && (op == "add" || op == "sub") && is_r16(d)) {
                str where = M(1, 2);                         // (a count from memory added to an address: the whole)
                py::list<str> out = pre.copy();
                out.append(F("movsx\tr8, %s", where));
                out.append(F("%s\t%s, r8", op, R64.at(d)));
                return out;
            }
            str m2 = M(1, 2);
            return done({F("%s\t%s, %s", op, d, m2)});
        }
        if (src.kind == "off") return many({F("lea\tr11, %s", sym(src.text)), F("%s\t%s, r11", op, R64.at(d))});
        if (op == "cmp" && kd && kd->kind == "ptr" && kd->to && *kd->to == "code" && number(l->args[1]) &&
            (data_address(*l->seg, *number(l->args[1])) || code_label(l->seg, *number(l->args[1])))) {
            int64_t v = *number(l->args[1]);
            auto ld = data_address(*l->seg, v);
            if (!ld) ld = code_label(l->seg, v);
            return many({F("lea\tr11, %s%s", std::get<0>(*ld), std::get<1>(*ld) ? F("%+d", std::get<1>(*ld)) : S("")), F("%s\t%s, r11", op, R64.at(d))});
        }
        if (kd && kd->wide() && kd->kind == "ptr") {
            auto v = number(l->args[1]);
            if (v && *v >= 0x8000) *v -= 0x10000;
            return one(F("%s\t%s, %s", op, R64.at(d), v ? py::to_str(*v) : l->args[1]));
        }
        return one(F("%s\t%s, %s", op, d, l->args[1]));
    }
    if (one_of(op, {"add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb"}) && a[0].kind == "mem") {
        int64_t size = (a.size() > 1 && a[1].kind == "reg" && is_r8(a[1].text)) ? 1 : ((a[0].mem->size && *a[0].mem->size) ? *a[0].mem->size : 2);
        auto kf = kind_of_mem(*a[0].mem, st);
        const K* k_ = &std::get<0>(kf);
        if ((op == "add" || op == "sub") && size == 2 && k_ && k_->kind == "ptr") {   // (an address moved on by a count: the whole of it)
            if (a[1].kind == "imm") {
                str m8 = M(0, 8);
                return done({F("%s\t%s, %s", op, m8, l->args[1])});
            }
            if (a[1].kind == "reg" && is_r16(a[1].text)) {
                str where = M(0, 8);                         // (r8: the operand's own place may want r11)
                py::list<str> out = pre.copy();
                out.append(F("movsx\tr8, %s", a[1].text));
                out.append(F("%s\t%s, r8", op, where));
                return out;
            }
        }
        if ((op == "cmp" || op == "test") && size == 2 && a[1].kind == "imm" && number(l->args[1]) && *number(l->args[1]) == 0) {
            auto kf2 = kind_of_mem(*a[0].mem, st);
            if (std::get<0>(kf2).kind == "ptr") size = 8;    // (an address compared with 0: the whole of it)
        }
        if (size == 8) {
            str m8 = M(0, 8);
            return done({F("%s\t%s, 0", op, m8)});
        }
        str ms = M(0, size);
        return done({F("%s\t%s, %s", op, ms, a[1].kind != "reg" ? l->args[1] : a[1].text)});
    }
    if (one_of(op, {"inc", "dec", "neg", "not"}) && a[0].kind == "reg") {
        str d = a[0].text;
        const K* k = is_r16(d) ? stk(d) : nullptr;
        return one(F("%s\t%s", op, (k && k->wide() && (op == "inc" || op == "dec")) ? R64.at(d) : d));
    }
    if (one_of(op, {"inc", "dec", "neg", "not"}) && a[0].kind == "mem") {
        auto kf = kind_of_mem(*a[0].mem, st);
        const K& k_ = std::get<0>(kf);
        const Mem& m_ = *a[0].mem;
        const K* fr = (m_.regs.size() == 1 && !m_.sym) ? stk(m_.regs[0]) : nullptr;
        bool arg_slot = fr && fr->kind == "frame" && k_.wide() &&
                        fr->old + m_.disp >= retsize(fr->owner ? fr->owner->routine : routine(i));
        if ((op == "inc" || op == "dec") && (k_.kind == "ptr" || arg_slot) && ((m_.size && *m_.size) ? *m_.size : 2) == 2) {
            str m8 = M(0, 8);
            return done({F("%s\t%s\t\t; (an address moved on: the whole of it)", op, m8)});
        }
        str ms = M(0, (m_.size && *m_.size) ? *m_.size : 2);
        return done({F("%s\t%s", op, ms)});
    }
    if (one_of(op, {"shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr"})) {
        if (a[0].kind == "mem") {
            str ms = M(0, (a[0].mem->size && *a[0].mem->size) ? *a[0].mem->size : 2);
            return done({F("%s\t%s, %s", op, ms, l->args[1])});
        }
        return one(F("%s\t%s, %s", op, a[0].text, l->args[1]));
    }
    if (one_of(op, {"mul", "imul", "div", "idiv"})) {
        if (a[0].kind == "mem") {
            str ms = M(0, (a[0].mem->size && *a[0].mem->size) ? *a[0].mem->size : 2);
            return done({F("%s\t%s", op, ms)});
        }
        return one(F("%s\t%s", op, a[0].text));
    }
    if (STRING.contains(op)) {
        str p4 = op.slice(0, 4);
        for (const char* rr : {"si", "di"}) {
            str r = rr;
            if ((r == "si" && (p4 == "lods" || p4 == "movs" || p4 == "cmps")) || (r == "di" && (p4 == "stos" || p4 == "movs" || p4 == "cmps" || p4 == "scas"))) {
                const K* k = stk(r);
                as_address(i, k ? std::optional<K>(*k) : std::nullopt);
            }
        }
        if (l->prefix) return many({S("movzx\tecx, cx"), F("%s %s", *l->prefix, op)});
        return one(op);
    }
    if (one_of(op, {"cbw", "cwd", "cld", "std", "stc", "clc", "cmc", "nop", "lahf", "sahf", "cli", "sti"})) return one(op);
    throw Asm16Error(F("no rule for %s", op));
}


}  // namespace codebreaker::port::asm16
