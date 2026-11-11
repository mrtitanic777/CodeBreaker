// codebreaker/port/pre_ansi.cpp -- typed C++ port of codebreaker.port.pre_ansi, PART 1: the rules, the text helpers, the
// rewriting functions of lines 1-715 of the Python and Port.__init__ (see pre_ansi.hpp for the typing notes).
//
// pre_ansi.py -- C of before the standard made C a compiler of today takes, the meaning kept.
//
// The C that CodeBreaker recovers from a program of the 1980s is what its authors wrote: function definitions in the
// old style (`f(a, b) char *a; {`), no prototypes, an int that is 16 bits, a long that is 32, structures whose bytes
// are laid out in memory pages and files by literal sizes, a member name used on whatever pointer is at hand, the C
// library of the program's own (its stdio, printf, malloc) among the sources. Each of these is mended here, the same
// way for every program:
//
//   definitions    the old-style definition becomes a prototyped one; a function that gives no value is void
//   prototypes     one header (proto.h) declares every function of the program; the old `extern f();` lines go
//   widths         long is int32_t everywhere (what it was); in a structure, and in a pointer cast that reads
//                  the bytes of a buffer, int and unsigned are int16_t and uint16_t - the layout the bytes have
//   formats        a %ld / %lx of a long, now an int32_t, is %d / %x
//   shared data    a variable a header defines (every file had its own, the linker made them one) is declared
//                  extern there and defined once, in data.c
//   runtime        the program's own C library (the modules that define fopen, malloc, _main...) and its headers
//                  are left out: the C library of today stands in; what was no standard call (DOS's handles, the
//                  keyboard line) the platform layer gives (platform/dosrt.h)
//
// and then the compiler's own complaints, in a loop: a member used on a pointer to something else is reached
// through a cast to the structure that has it; a function whose value is used after all gives an int again; a
// function called with more or fewer arguments than it has keeps the old-style definition (its calls unchecked,
// as they were). Every change is noted for the report, with the line.

#include "codebreaker/port/pre_ansi.hpp"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <map>
#include <unordered_map>

#include "codebreaker/base/ctext.hpp"
#include "json.hpp"
#include "value.hpp"

namespace codebreaker::port::pre_ansi {

using base::ctext::blank;
using base::ctext::sub_code;
using py::dict;
using py::list;
using py::set;
using py::str;

namespace {

using Match = py::re::Match;
using Edit3 = std::tuple<int64_t, int64_t, str>;   // (start, end, new)

str S(const char* s) { return str(s); }
str sl(const str& s, int64_t a, int64_t b) { return s.slice(a, b); }
str esc(const str& s) { return py::re::escape(s); }
// re.sub(r"\s+", " ", x)
str ws1(const str& x) { return py::re::sub(S(R"PY(\s+)PY"), S(" "), x); }
// code.count("\n", 0, at)
int64_t nl_before(const str& s, int64_t at) { return sl(s, 0, at).count(S("\n")); }
list<str> lines_of(const str& t) { return py::split(t, S("\n")); }
str unlines(const list<str>& l) { return py::join(S("\n"), l); }
bool fullm(const char* pat, const str& s) { return static_cast<bool>(py::re::fullmatch(S(pat), s)); }
// "" if the type ends in * else " "
str sp_for(const str& t) { return t.endswith(S("*")) ? S("") : S(" "); }
// (c in "([") - (c in ")]")
int64_t bal(char32_t c) { return (c == U'(' || c == U'[') - (c == U')' || c == U']'); }
// the characters of a text at the Python index (negative from the end)
char32_t chr_at(const str& s, int64_t i) { return s.at_(i < 0 ? i + s.size() : i); }

std::string trim_ws_lead(const std::string& s) { return s; }

// the arguments of a call that begins after its '(' at `from`: [(start, end)] as the Python's loops make them
list<std::tuple<int64_t, int64_t>> spans_of(const str& code, int64_t from) {
    list<std::tuple<int64_t, int64_t>> spans;
    int64_t i = from, d = 1, start = from;
    while (i < code.size() && d) {
        char32_t c = code.at_(i);
        d += bal(c);
        if ((c == U',' && d == 1) || d == 0) {
            spans.append({start, i});
            start = i + 1;
        }
        i += 1;
    }
    return spans;
}

str int_text(int64_t v) { return py::to_str(v); }
// int(x, 0) for the "0x.." or decimal numbers of the Python; a leading-zero decimal as int(x)
int64_t int_lit(const str& x) { return fullm(R"PY(0\d+)PY", x) ? py::int_parse(x, 10) : py::int_parse(x, 0); }

}  // namespace

// ================================================================ constants

const set<str> KEYWORDS = set<str>::from(list<str>{
    S("auto"), S("break"), S("case"), S("char"), S("const"), S("continue"), S("default"), S("do"), S("double"),
    S("else"), S("enum"), S("extern"), S("float"), S("for"), S("goto"), S("if"), S("int"), S("long"), S("register"),
    S("return"), S("short"), S("signed"), S("sizeof"), S("static"), S("struct"), S("switch"), S("typedef"),
    S("union"), S("unsigned"), S("void"), S("volatile"), S("while")});
const set<str> STDINT = set<str>::from(list<str>{S("int8_t"), S("uint8_t"), S("int16_t"), S("uint16_t"), S("int32_t"),
                                                  S("uint32_t"), S("intptr_t"), S("uintptr_t"), S("size_t")});
const set<str> TYPEWORDS = set<str>::from(list<str>{
    S("int"), S("char"), S("long"), S("short"), S("unsigned"), S("signed"), S("float"), S("double"), S("void"),
    S("struct"), S("union"), S("enum"), S("register"), S("static"), S("extern"), S("auto"), S("const"),
    S("volatile"), S("FILE"), S("int16_t"), S("uint16_t"), S("int32_t"), S("uint32_t"), S("int8_t"), S("uint8_t"),
    S("intptr_t"), S("uintptr_t")});
const set<str> PRINTF = set<str>::from(list<str>{S("printf"), S("fprintf"), S("sprintf")});

// the rules MS C 3.00's P3 brought (its pools, size tables, views, NULL links), each by a name: those another
// program's port does not keep are off here until they do. CB_PORT_RULES turns rules on or off over these defaults,
// comma separated (+rule or rule on, -rule off; +all / -all every one) - a regression's bisection
const set<str> RULES_OFF = set<str>();

// whether the port's rule `name` runs (RULES_OFF, then CB_PORT_RULES in its order)
bool rule(const str& name) {
    bool on = !RULES_OFF.contains(name);
    const char* env = std::getenv("CB_PORT_RULES");
    str rules = env ? str(env) : S("");
    for (const str& w : py::split(rules.replace(S(" "), S("")), S(","))) {
        if (w == name || w == S("+") + name || w == S("+all")) {
            on = true;
        } else if (w == S("-") + name || w == S("-all")) {
            on = false;
        }
    }
    return on;
}

// ---------------------------------------------------------------- the profile (runtimes/NAME.json)

Profile Profile::load(const str& path) {
    py::V doc;
    {
        py::file f = py::open(path, S("r"));
        doc = py::json_parse(f.read_text().utf8());
    }
    auto strs = [](const py::V& v) {
        list<str> out;
        if (v.is_list()) for (const py::V& x : v.seq()) out.append(str(x.str()));
        return out;
    };
    Profile p;
    if (doc.get("name").is_str()) p.name = str(doc.get("name").str());
    p.libc = strs(doc.get("libc"));
    p.runtime_defines = strs(doc.get("runtime_defines"));
    p.runtime_headers = strs(doc.get("runtime_headers"));
    p.runtime_internals = strs(doc.get("runtime_internals"));
    const py::V& plat = doc.get("platform");
    if (plat.get("header").is_str()) p.platform_header = str(plat.get("header").str());
    p.platform_calls = strs(plat.get("calls"));
    if (plat.get("renames").is_dict())
        for (const auto& kv : plat.get("renames").items()) p.platform_renames.set(str(kv.first.str()), str(kv.second.str()));
    const py::V& w = doc.get("widths");
    if (w.get("pointer").is_int()) p.widths_pointer = w.get("pointer").as_int();
    if (doc.get("int_storage").is_int()) p.int_storage = doc.get("int_storage").as_int();
    if (doc.get("printf_hex_upper").is_bool()) p.printf_hex_upper = doc.get("printf_hex_upper").as_int() != 0;
    if (doc.get("struct_align").is_int()) p.struct_align = doc.get("struct_align").as_int();
    if (doc.get("keywords_removed").is_list()) p.keywords_removed = strs(doc.get("keywords_removed"));
    if (doc.get("arg_order").is_str()) p.arg_order = str(doc.get("arg_order").str());
    return p;
}

// ================================================================ reading the text
// (blank and sub_code, what is code apart from its strings and comments: base/ctext)

// the brace depth at the start of each line
list<int64_t> depths(const list<str>& code_lines) {
    int64_t d = 0;
    list<int64_t> out;
    for (const str& l : code_lines) {
        out.append(d);
        d += l.count(S("{")) - l.count(S("}"));
    }
    return out;
}

list<str> split_top(const str& s, const str& sep) {
    list<str> parts;
    int64_t d = 0;
    str cur;
    for (const str& c : s) {
        if (c == S("(") || c == S("[")) {
            d += 1;
        } else if (c == S(")") || c == S("]")) {
            d -= 1;
        }
        if (c == sep && d == 0) {
            parts.append(cur);
            cur = S("");
        } else {
            cur += c;
        }
    }
    parts.append(cur);
    return parts;
}

std::optional<std::tuple<str, list<std::tuple<str, str>>>> declarators(const str& decl, const set<str>& types) {
    list<str> parts = split_top(decl.strip().rstrip(S(";")));
    str first = parts[0];
    list<Match> toks = py::re::finditer(S(R"PY([A-Za-z_]\w*)PY"), first);
    std::optional<Match> name;
    for (int64_t i = 0; i < toks.size(); i++) {
        const Match& m = toks[i];
        if (types.contains(m.group()) ||
            (i && (toks[i - 1].group() == S("struct") || toks[i - 1].group() == S("union") ||
                   toks[i - 1].group() == S("enum")))) {
            continue;
        }
        name = m;
        break;
    }
    if (!name) {
        return std::nullopt;
    }
    int64_t start = name->start();
    while (start && (first.at_(start - 1) == U' ' || first.at_(start - 1) == U'\t' || first.at_(start - 1) == U'*' ||
                     first.at_(start - 1) == U'(')) {
        start -= 1;
    }
    while (start < name->start() && (first.at_(start) == U' ' || first.at_(start) == U'\t')) {
        start += 1;
    }
    str base = sl(first, 0, start).strip();
    list<std::tuple<str, str>> out;
    out.append({name->group(), first.slice_from(start).strip()});
    for (int64_t i = 1; i < parts.size(); i++) {
        const str& p = parts[i];
        std::optional<Match> m = py::re::search(S(R"PY([A-Za-z_]\w*)PY"), p);
        if (!m) {
            return std::nullopt;
        }
        out.append({m->group(), p.strip()});
    }
    return std::tuple<str, list<std::tuple<str, str>>>{base, out};
}

std::optional<std::tuple<str, list<std::tuple<str, str>>>> declarators(const str& decl) {
    return declarators(decl, TYPEWORDS);
}

// ---------------------------------------------------------------- one file

// a parameter declared an array - T a[] or T a[][3]: the pointer it is (T *a, T (*a)[3]), which a prototype
// can name before T is complete
str array_param(const str& t, const str& n) {
    std::optional<Match> m = py::re::fullmatch(
        S(R"PY((.*?)\b)PY") + esc(n) + S(R"PY(\s*\[[^\]]*\]((?:\s*\[[^\]]*\])*)\s*)PY"), t);
    if (!m) {
        return t;
    }
    str rest = m->group(2).strip();
    return !rest.empty() ? m->group(1) + S("(*") + n + S(")") + rest : m->group(1) + S("*") + n;
}

str Fn::proto(bool names) const {
    (void)names;
    list<str> ps_;
    for (const auto& [n, t] : params) ps_.append(array_param(t, n));
    str ps = py::join(S(", "), ps_);
    if (ps.empty()) ps = S("void");   // (T a[][3]: the pointer it is)
    str r = void_ ? S("void") : (!ret.empty() ? ret : S("int"));
    str sep = r.endswith(S("*")) ? S("") : S(" ");
    str st = static_ ? S("static ") : S("");
    if (varargs) {
        return st + r + sep + name + S("(") + ps + S(", ...)");
    }
    if (loose) {
        return st + r + sep + name + S("()");
    }
    return st + r + sep + name + S("(") + ps + S(")");
}

namespace {
const py::re::Pattern& head_rx() {
    static const py::re::Pattern p = py::re::compile(
        S(R"PY(^([\w\s*]*?)([A-Za-z_]\w*)\s*\(([^()]*)\)\s*$)PY"));
    return p;
}
}  // namespace

// the old-style function definitions of a file: (Fn, first line, the line of its '{', its last line)
list<Definition> definitions(const list<str>& lines, const list<str>& code) {
    list<Definition> out;
    list<int64_t> ds = depths(code);
    int64_t i = 0;
    while (i < lines.size()) {
        std::optional<Match> m;
        if (ds[i] == 0 && !code[i].lstrip().startswith(S("#"))) {
            m = head_rx().match(code[i].rstrip());
        }
        if (!m || KEYWORDS.contains(m->group(2))) {
            i += 1;
            continue;
        }
        // not all(w in TYPEWORDS or w == "*" ...) and not re.fullmatch(...)
        bool all_type = true;
        for (const str& w : py::re::findall(S(R"PY(\w+|\*)PY"), m->group(1))) {
            if (w == S("struct") || w == S("union")) continue;
            if (!(TYPEWORDS.contains(w) || w == S("*"))) {
                all_type = false;
                break;
            }
        }
        if (!all_type && !fullm(R"PY(\s*(?:static\s+)?(?:struct|union)\s+\w+\s*\**\s*)PY", m->group(1))) {
            i += 1;
            continue;
        }
        list<str> params;
        for (const str& p0 : py::split(m->group(3), S(","))) {
            str p = p0.strip();
            if (!p.empty()) params.append(p);
        }
        bool all_ident = true;
        for (const str& p : params) {
            if (!fullm(R"PY([A-Za-z_]\w*)PY", p)) {
                all_ident = false;
                break;
            }
        }
        if (!all_ident || (params.size() == 1 && params[0] == S("void"))) {
            i += 1;
            continue;
        }
        int64_t first = i;
        if (m->group(1).strip().empty() && i && ds[i - 1] == 0 &&
            fullm((R"PY(\s*(?:static\s+)?(?:(?:unsigned|signed|long|short|int|char|void|float|double|struct\s+\w+|)PY"
                   R"PY(union\s+\w+)\s*)+\**\s*|\s*(?:static\s+)?[A-Z_][A-Z0-9_]*\s*\*+\s*)PY"),
                  code[i - 1]) &&
            !code[i - 1].strip().empty()) {   // (or a type of the library's: FILE *)
            first = i - 1;                    // (the type on the line before: char *\nf(a))
        }
        int64_t j = i + 1;
        list<str> decls;
        while (j < lines.size() && !code[j].strip().startswith(S("{"))) {
            str s = code[j].strip();
            if (!s.empty() && !s.endswith(S(";"))) break;
            if (!s.empty()) decls.append(s);
            j += 1;
        }
        if (j >= lines.size() || !code[j].strip().startswith(S("{"))) {
            i += 1;
            continue;
        }
        dict<str, str> types;
        for (const str& d : decls) {
            auto r = declarators(d);
            if (!r) continue;
            str base = std::get<0>(*r);
            if (py::re::sub(S(R"PY(\b(?:register|auto|static|extern|const|volatile)\b)PY"), S(""), base).strip().empty()) {
                base = (base + S(" int")).strip();   // (register op, asg; - old C's int, unsaid)
            }
            for (const auto& [n, dcl] : std::get<1>(*r)) types.set(n, (base + S(" ") + dcl).strip());
        }
        str ret = first == i ? m->group(1).strip() : code[first].strip();
        bool stat = static_cast<bool>(py::re::match(S(R"PY(static\b)PY"), ret));
        ret = py::re::sub(S(R"PY(^static\s*)PY"), S(""), ret).strip();
        list<std::tuple<str, str>> fparams;
        for (const str& p : params) fparams.append({p, types.get(p, S("int ") + p)});
        FnP fn = std::make_shared<Fn>(m->group(2), ret, fparams, stat, i + 1);
        for (const str& p : params)   // (parameters it never declared: ints by default)
            if (!types.contains(p)) fn->implicit.add(p);
        int64_t k = j, d = 0;   // (the body: to its closing brace)
        while (k < lines.size()) {
            d += code[k].count(S("{")) - code[k].count(S("}"));
            if (d == 0) break;
            k += 1;
        }
        out.append({fn, first, j, k});
        i = k + 1;
    }
    return out;
}

// long as int32_t; int and unsigned in a structure, and in a pointer cast, as int16_t and uint16_t; with
// int16, every int and unsigned (the arithmetic that rotates a word, or counts on its wrapping round, as it was)
std::tuple<str, str> widths(str text, str code, Notes& notes, const str& fname, bool int16) {
    std::vector<std::tuple<int64_t, int64_t, std::optional<str>>> edits;   // (start, end, new)

    auto sub = [&](const char* rx, const char* nw, int64_t lo, std::optional<int64_t> hi, const char* why) {
        str region = hi ? sl(code, lo, *hi) : code.slice_from(lo);
        for (const Match& m : py::re::finditer(S(rx), region)) {
            int64_t a = lo + m.start(), b = lo + m.end();
            bool hit = false;
            for (const auto& [s, e, x] : edits) {
                if (a < e && s < b) {
                    hit = true;
                    break;
                }
            }
            if (!hit) {
                edits.push_back({a, b, S(nw)});
                if (why) {
                    notes[S(why)].append(fname + S(":") + int_text(nl_before(code, a) + 1));
                }
            }
        }
    };
    for (const Match& m : py::re::finditer(S(R"PY(\b(?:struct|union)\s*\w*\s*\{)PY"), code)) {   // (structures: the bytes' layout)
        int64_t d = 0, k = m.end() - 1;
        while (k < code.size()) {
            char32_t c = code.at_(k);
            d += (c == U'{') - (c == U'}');
            if (d == 0) break;
            k += 1;
        }
        sub(R"PY(\bunsigned\s+short(?:\s+int)?\b|\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)PY", "uint16_t",
            m.end(), k, "struct member unsigned -> uint16_t (the structure's bytes as they were)");
        sub(R"PY((?<!unsigned )(?<!signed )\b(?:short\s+int|short|int)\b(?!\w))PY", "int16_t", m.end(), k,
            "struct member int -> int16_t (the structure's bytes as they were)");
    }
    sub(R"PY(\(\s*unsigned(?:\s+int)?\s*(?=\*+\s*\)))PY", "(uint16_t ", 0, std::nullopt,
        "pointer cast (unsigned *) -> (uint16_t *): a word of a buffer");
    sub(R"PY(\(\s*int\s*(?=\*+\s*\)))PY", "(int16_t ", 0, std::nullopt,
        "pointer cast (int *) -> (int16_t *): a word of a buffer");
    sub(R"PY(\bunsigned\s+long(?:\s+int)?\b|\blong\s+unsigned(?:\s+int)?\b)PY", "uint32_t", 0, std::nullopt,
        "unsigned long -> uint32_t");
    sub(R"PY(\blong(?:\s+int)?\b)PY", "int32_t", 0, std::nullopt, "long -> int32_t (32 bits, as it was)");
    sub(R"PY(\bunsigned\s+short(?:\s+int)?\b)PY", "uint16_t", 0, std::nullopt, "short -> int16_t");
    sub(R"PY(\bshort(?:\s+int)?\b)PY", "int16_t", 0, std::nullopt, "short -> int16_t");
    if (int16) {   // (every int as wide as it was: a store truncates as the 8086's did; main's own are the system's)
        std::vector<std::tuple<int64_t, int64_t>> guards;
        for (const Match& m : py::re::finditer(S(R"PY((?m)^.*\bmain\s*\(.*$)PY"), code)) {
            edits.push_back({m.start(), m.start(), std::nullopt});
            edits.push_back({m.start(), m.end(), std::nullopt});
        }
        sub(R"PY(\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)PY", "uint16_t", 0, std::nullopt,
            "unsigned -> uint16_t (16 bits, as it was)");
        sub(R"PY((?<!unsigned )(?<!signed )\bint\b)PY", "int16_t", 0, std::nullopt, "int -> int16_t (16 bits, as it was)");
        std::vector<std::tuple<int64_t, int64_t, std::optional<str>>> kept;
        for (const auto& e : edits)
            if (std::get<2>(e)) kept.push_back(e);
        edits = kept;
    }
    std::vector<Edit3> real;
    for (const auto& [a, b, nw] : edits) real.push_back({a, b, *nw});
    std::sort(real.begin(), real.end(), [](const Edit3& x, const Edit3& y) { return y < x; });
    for (const auto& [a, b, nw] : real) {
        text = sl(text, 0, a) + nw + text.slice_from(b);
        code = sl(code, 0, a) + nw + code.slice_from(b);
    }
    return {text, code};
}

// the argument of a %ld %lx %lu in printf's family (an int32_t now, or a long of the C library's, as
// ftell's) given as (long): what the format says, on every system
str formats(str text, const str& code, Notes& notes, const str& fname, bool int16, bool hex_upper) {
    std::vector<Edit3> edits;
    str alt = py::join(S("|"), PRINTF);
    for (const Match& m : py::re::finditer(S(R"PY(\b(?:)PY") + alt + S(R"PY()\s*\()PY"), code)) {
        list<std::tuple<int64_t, int64_t>> spans = spans_of(code, m.end());   // (the call's arguments)
        int64_t fmt = -1;
        for (int64_t k = 0; k < spans.size(); k++) {
            auto [a, b] = spans[k];
            if (sl(code, a, b).strip().startswith(S("\""))) {
                fmt = k;
                break;
            }
        }
        if (fmt < 0) continue;
        auto [a, b] = spans[fmt];
        str lit = py::re::sub(S(R"PY((%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?)[FN](?=l?[spndiouxXc]))PY"), S(R"PY(\1)PY"), sl(text, a, b));
        if (lit != sl(text, a, b)) {   // (%Fs, %Np: Microsoft's far and near - one memory now)
            notes[S("%Fs / %Np -> %s / %p (Microsoft's printf: a far or near pointer; one flat memory now)")].append(
                fname + S(":") + int_text(nl_before(code, a) + 1));
        }
        if (hex_upper) {   // (the old printf's %x was in capitals)
            str up = py::re::sub(S(R"PY((%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?l?)x)PY"), S(R"PY(\1X)PY"), lit);
            if (up != lit) {
                notes[S("%x -> %X (the old printf gave its hexadecimal in capitals)")].append(
                    fname + S(":") + int_text(nl_before(code, a) + 1));
            }
            lit = up;
        }
        if (lit != sl(text, a, b)) {
            edits.push_back({a, b, lit});
        }
        int64_t k = fmt;
        for (const Match& cm : py::re::finditer(S(R"PY(%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?(l?)([diouxXcsp%]))PY"), lit)) {
            str lng = cm.group(1), conv = cm.group(2);
            if (conv == S("%")) continue;
            k += 1;
            if (k >= spans.size()) break;
            auto [x, y] = spans[k];
            str arg = sl(text, x, y).strip();
            str seg = sl(text, x, y);
            str lead = sl(seg, 0, seg.size() - seg.lstrip().size());
            str simple = (fullm(R"PY([\w.\->\[\]]+|\w+\([^()]*\))PY", arg)) ? arg : S("(") + arg + S(")");
            if (!lng.empty() && !arg.startswith(S("(long)"))) {
                edits.push_back({x, y, lead + S("(long)") + simple});
                notes[S("argument of a %l. format given as (long) (a long is int32_t here)")].append(
                    fname + S(":") + int_text(nl_before(code, x) + 1));
            } else if (int16 && lng.empty() && (conv == S("o") || conv == S("u") || conv == S("x") || conv == S("X")) &&
                       !arg.startswith(S("(uint16_t)"))) {
                edits.push_back({x, y, lead + S("(uint16_t)") + simple});   // (16 bits shown, as its printf showed them)
                notes[S("argument of a %x / %u format given as (uint16_t) (16 bits, as the old printf showed)")].append(
                    fname + S(":") + int_text(nl_before(code, x) + 1));
            }
        }
    }
    std::sort(edits.begin(), edits.end(), [](const Edit3& x, const Edit3& y) { return y < x; });
    for (const auto& [x, y, nw] : edits) {
        text = sl(text, 0, x) + nw + text.slice_from(y);
    }
    return text;
}

// ================================================================ the rewriting functions

// ({allocator: {(struct, old size)}}: of struct_sizes)
dict<str, set<std::tuple<str, int64_t>>> POOLS;
// (a cast of type words alone)
const str ARGCAST = S(R"PY((?:\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)\s*)?)PY");
const str ALLOC_PATTERN = S(R"PY(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w*alloc\w*)\s*\()PY");
const str CASTCALL_PATTERN = S(R"PY(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\()PY");
const py::re::Pattern ALLOC = py::re::compile(ALLOC_PATTERN);
const py::re::Pattern CASTCALL = py::re::compile(CASTCALL_PATTERN);
const list<str> MEMFNS = list<str>{S("memset"), S("memcpy"), S("memmove"), S("memcmp"), S("movbytes"), S("fillb"),
                                   S("far_memcpy"), S("_fmemcpy"), S("_fmemset")};
const str GLOBAL_DECL = S(R"PY((?m)((?:^|;)\s*(?:extern\s+|static\s+|register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*)%s\s*((?:,\s*\w+\s*)*);)PY");
const str LOCAL_DECL = S(R"PY((?m)^(\s*(?:register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*)%s\s*((?:,\s*\w+\s*)*);)PY");

// whether a variable is computed with as a number somewhere (n /= 2, n * v, n << 1, n & 7, n % k)
bool used_as_number(const str& code, const str& var) {
    str v = esc(var);
    return static_cast<bool>(
               py::re::search(S(R"PY(\b)PY") + v + S(R"PY(\s*(?:[*/%^|]=|<<=|>>=|[/%]|<<|>>|\*(?!\s*\))|&(?!&)|\|(?!\|)))PY"), code)) ||
           static_cast<bool>(py::re::search(S(R"PY((?:[\w)\]]\s*(?:[*/%]|<<|>>)|[*/%]=|<<=|>>=)\s*)PY") + v + S(R"PY(\b)PY"), code)) ||
           static_cast<bool>(py::re::search(
               S(R"PY(\(?\s*\b)PY") + v + S(R"PY(\s*(?:[+\-]=?[^;)]*)?\)?\s*(?:[<>]=?)\s*-?\s*(?:[1-9]|0\b))PY"), code)) ||
           static_cast<bool>(py::re::search(S(R"PY(\b)PY") + v + S(R"PY(\s*[+\-]\s*\w+\s*\)\s*[&|])PY"), code)) ||   // (compared with a number; (off + n) & ~n)
           static_cast<bool>(py::re::search(S(R"PY(\[\s*(?:\+\+|--)?\s*)PY") + v + S(R"PY(\s*(?:\+\+|--)?\s*\])PY"), code));   // (an index)
}

// the functions of a text, new style and then old: ((name, first line, '{' line, last line), ...) - a text
// seen before: as it was (one parse a text, not one a complaint)
list<std::tuple<str, int64_t, int64_t, int64_t>> function_map(const str& text) {
    static std::unordered_map<str, list<std::tuple<str, int64_t, int64_t, int64_t>>, py::str_hash> cache;
    auto hit = cache.find(text);
    if (hit != cache.end()) {
        return hit->second;
    }
    str code = blank(text);
    list<str> lines = lines_of(text), clines = lines_of(code);
    list<std::tuple<str, int64_t, int64_t, int64_t>> out;
    for (const auto& d : definitions_any(lines, clines)) out.append(d);
    for (const auto& [f, s, j, k] : definitions(lines, clines)) out.append({f->name, s, j, k});
    if (cache.size() >= 4096) cache.clear();
    cache.emplace(text, out);
    return out;
}

// (name, first line, '{' line, last line) of the function line ln (1-based) is in - None
std::optional<std::tuple<str, int64_t, int64_t, int64_t>> enclosing(const str& text, int64_t ln) {
    for (const auto& d : function_map(text)) {   // (new style first; an old-style one, kept so)
        if (std::get<1>(d) <= ln - 1 && ln - 1 <= std::get<3>(d)) {
            return d;
        }
    }
    return std::nullopt;
}

// whether a function (definitions_any's) declares var itself: a parameter or a local
bool declares_local(const str& text, const std::tuple<str, int64_t, int64_t, int64_t>& fn, const str& var) {
    list<str> lines = lines_of(blank(text)).slice(std::get<1>(fn), std::get<3>(fn) + 1);
    return static_cast<bool>(py::re::search(
        S(R"PY(\b(?:u?int16_t|int|unsigned|intptr_t)\s+(?:[^;(){}]*,\s*)?)PY") + esc(var) + S(R"PY(\b\s*[;,)=])PY"), unlines(lines)));
}

// the variables of one function that hold pointers, declared pointers there alone
str apply_local_retypes(const str& text, const str& fname,
                        const dict<std::tuple<str, str, str>, str>& retypes) {
    std::vector<std::tuple<str, str, str>> mine;   // (function, variable, type)
    for (const auto& [key, t] : retypes.items()) {
        if (std::get<0>(key) == fname) mine.push_back({std::get<1>(key), std::get<2>(key), t});
    }
    if (mine.empty()) {
        return text;
    }
    list<str> lines = lines_of(text);
    list<str> code = lines_of(blank(text));
    std::vector<DefinitionAny> defs;
    for (const auto& d : definitions_any(lines, code)) defs.push_back(d);
    for (const auto& [f, s, j, k] : definitions(lines, code)) defs.push_back({f->name, s, j, k});
    std::stable_sort(defs.begin(), defs.end(), [](const DefinitionAny& a, const DefinitionAny& b) {
        return -std::get<1>(a) < -std::get<1>(b);   // (sorted by key=-first line)
    });
    for (const auto& d : defs) {   // (new style and old)
        for (const auto& [fn, var, t] : mine) {
            if (fn != std::get<0>(d)) continue;
            str body = unlines(lines.slice(std::get<1>(d), std::get<3>(d) + 1));
            str v = esc(var);
            body = py::re::sub(S(R"PY((?m)^(\s*(?:register\s+)?)(?:u?int16_t|int|unsigned)\s+)PY") + v + S(R"PY(\s*;)PY"),
                               [&](const Match& m) { return m.group(1) + t + sp_for(t) + var + S(";"); }, body);
            body = py::re::sub(py::percent(LOCAL_DECL, v), split_decl(var, t), body);
            body = py::re::sub(S(R"PY(\((?:u?int16_t|int|unsigned)\s+)PY") + v + S(R"PY(\s*([,)]))PY"),   // (a parameter of the prototype)
                               [&](const Match& m) { return S("(") + t + sp_for(t) + var + m.group(1); }, body);
            body = py::re::sub(S(R"PY(,\s*(?:u?int16_t|int|unsigned)\s+)PY") + v + S(R"PY(\s*([,)]))PY"),
                               [&](const Match& m) { return S(", ") + t + sp_for(t) + var + m.group(1); }, body);
            lines.set_slice(std::get<1>(d), std::get<3>(d) + 1, lines_of(body));
        }
    }
    return unlines(lines);
}

str split_decl::operator()(const Match& m) const {
    list<str> rest;
    for (const str& x : py::split(( m.group(3) + m.group(4)).replace(S(","), S(" ")))) {
        if (!x.strip().empty()) rest.append(x.strip());
    }
    str lead = m.group(1);
    str out = !rest.empty() ? lead + m.group(2) + S(" ") + py::join(S(", "), rest) + S("; ") : lead;
    str cls;
    if (!rest.empty()) {   // (its storage: so)
        list<str> ws;
        for (const str& w : py::split(lead)) if (w == S("extern") || w == S("static")) ws.append(w);
        cls = py::join(S(" "), ws);
    }
    return out + (!cls.empty() ? cls + S(" ") : S("")) + typ + sp_for(typ) + name + S(";");
}

// memset(g_seg_assume, 0, 24): the bytes of a global array given as the number it had - sizeof it now
str mem_sizes(str text, const dict<str, int64_t>& olds, const dict<str, int64_t>& elems, Notes& notes, const str& fname) {
    str code = blank(text);
    std::vector<Edit3> edits;
    for (const Match& m : py::re::finditer(S(R"PY(\b()PY") + py::join(S("|"), MEMFNS) + S(R"PY()\s*\()PY"), code)) {
        list<std::tuple<int64_t, int64_t>> args = spans_of(code, m.end());
        list<str> names;
        for (const auto& [a, b] : args) {
            auto x = py::re::fullmatch(S(R"PY(\s*&?\s*(\w+)\s*)PY"), sl(code, a, b));
            if (x && olds.contains(x->group(1))) names.append(x->group(1));
        }
        for (const auto& [a, b] : args) {
            auto lit = py::re::fullmatch(S(R"PY(\s*(0[xX][0-9a-fA-F]+|\d+)\s*)PY"), sl(code, a, b));
            if (!lit) continue;
            int64_t n = int_lit(lit->group(1));
            for (const str& nm : names) {
                if (olds.at(nm) == n && n > 1) {
                    std::optional<int64_t> e = elems.get(nm);   // (as many elements as it had, of today's size: the
                    str nw = (e && *e && n % *e == 0)           //  declaration may say no size)
                                 ? int_text(n / *e) + S(" * sizeof(") + nm + S("[0])")
                                 : S("sizeof(") + nm + S(")");
                    edits.push_back({a + lit->start(1), a + lit->end(1), nw});
                    notes[S("the bytes of a global array given as the number it had: sizeof it now")].append(
                        fname + S(":") + int_text(nl_before(code, a) + 1) + S(" (") + nm + S(": ") + int_text(n) + S(")"));
                    break;
                }
            }
        }
    }
    std::sort(edits.begin(), edits.end(), [](const Edit3& x, const Edit3& y) { return y < x; });
    for (const auto& [a, b, nw] : edits) text = sl(text, 0, a) + nw + text.slice_from(b);
    return text;
}

// p = alloc(n + 8); p[i + 8] = c; b = (struct blk *)p - a structure with its bytes after a head (a trailing
// array): the head's old length as the number it was - offsetof(struct blk, its array) now
str header_offsets(str text, const asm16::Layouts& old, Notes& notes, const str& fname) {
    list<str> lines = lines_of(text);
    list<str> code = lines_of(blank(text));
    for (const auto& [name, s_, j_, k_] : definitions_any(lines, code)) {
        (void)name;
        (void)j_;
        str body = unlines(code.slice(s_, k_ + 1));
        std::vector<std::tuple<str, str>> heads;
        for (const Match& m : py::re::finditer(
                 S(R"PY(\(\s*(struct\s+\w+)\s*\*\s*\)\s*(\w+)\b(?!\s*(?:\(|->|\[)))PY"), body))
            heads.push_back({m.group(1), m.group(2)});
        for (const Match& m : py::re::finditer(S(R"PY(\b(struct\s+\w+)\s*\*\s*(\w+)\s*[;,])PY"), body)) {   // (b = p, b a structure's pointer)
            for (const Match& a_ : py::re::finditer(S(R"PY(\b)PY") + esc(m.group(2)) + S(R"PY(\s*=\s*(\w+)\s*;)PY"), body))
                heads.push_back({m.group(1), a_.group(1)});
        }
        for (const auto& [st0, v] : heads) {
            str st_ = ws1(st0);
            const asm16::Layout* lay = old.get(st_);
            if (!lay) continue;
            std::vector<std::tuple<int64_t, str>> mems;
            int64_t last = 0;
            bool any = false;
            for (const auto& [k, x] : lay->members) {
                if (x.decl.contains(S("["))) mems.push_back({x.off, k});
                last = any ? std::max(last, x.off) : x.off;
                any = true;
            }
            if (mems.empty()) continue;
            auto [off, mem] = *std::max_element(mems.begin(), mems.end());
            if (off < 4 || off < last) continue;   // (its last member, the bytes after the head)
            str n_ = S(R"PY((?<![\w.]))PY") + int_text(off) + S(R"PY((?![\w.]))PY");
            str what = S("offsetof(") + st_ + S(", ") + mem + S(")");
            list<str> pats = {S(R"PY(\b)PY") + esc(v) + S(R"PY(\s*\[[^\]\n]*)PY"),
                              S(R"PY(\b)PY") + esc(v) + S(R"PY(\s*=\s*(?:\([^()]*\)\s*)?\w+\s*\([^;\n]*)PY"),
                              S(R"PY(\b)PY") + esc(v) + S(R"PY(\s*\+\s*)PY")};
            for (int64_t x = s_; x <= k_; x++) {
                str c = code[x], l = lines[x];
                for (const str& pt : pats) {
                    list<Match> ms = py::re::finditer(pt + n_, c);
                    for (int64_t q = ms.size() - 1; q >= 0; q--) {
                        const Match& mm = ms[q];
                        int64_t e = mm.end();
                        int64_t b_ = e - int_text(off).size();
                        if (sl(c, b_, e) == int_text(off)) {
                            l = sl(l, 0, b_) + what + l.slice_from(e);
                            c = sl(c, 0, b_) + what + c.slice_from(e);
                        }
                    }
                }
                if (l != lines[x]) {
                    lines[x] = l;
                    code[x] = c;
                    notes[S("a structure's head given as the bytes it was (its array after it): offsetof")].append(
                        fname + S(":") + int_text(x + 1) + S(" (") + what + S(")"));
                }
            }
        }
    }
    return unlines(lines);
}

// the member a structure's old layout has at the offset n (not its start, not its end) - where a block of n
// bytes, a prefix of the structure, ended then; or None
std::optional<str> prefix_member(const asm16::Layout* lay, int64_t n) {
    if (!lay || n <= 0 || n >= lay->size) {
        return std::nullopt;
    }
    for (const auto& [k, v] : lay->members) {
        if (v.off == n) return k;
    }
    return std::nullopt;
}

// (struct S *)malloc(n + 29): the recovered source gave a structure's size as the number it was (the bytes the
// same) - now sizeof(struct S) and the rest: a structure of today's layout is longer
str struct_sizes(str text, const asm16::Layouts& old, Notes& notes, const str& fname,
                 const dict<str, set<int64_t>>& allocs) {
    const str LIT_RX = S(R"PY(\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*)PY");
    str code = blank(text);
    dict<str, str> ptr_of;   // (p = f04fb5(0, 10); p a struct tcell *: as if the call were cast to it)
    for (const Match& m : py::re::finditer(S(R"PY(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])PY"), code))
        ptr_of.setdefault(m.group(2), ws1(m.group(1)));
    std::vector<std::tuple<int64_t, str>> ins;
    for (const Match& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(([^()]*)\))PY"), code)) {
        std::optional<str> st_ = ptr_of.get(m.group(1));
        if (!st_ || !old.contains(*st_) || KEYWORDS.contains(m.group(2)) || m.group(2).contains(S("alloc"))) continue;
        list<int64_t> lits;
        list<str> tab;
        for (const str& a_ : py::split(m.group(3), S(","))) {
            if (py::re::fullmatch(LIT_RX, a_)) lits.append(py::int_parse(a_.strip(), 0));
            if (fullm(R"PY(\s*[A-Za-z_]\w*\s*)PY", a_) &&
                py::re::search(S(R"PY(\b)PY") + esc(a_.strip()) + S(R"PY(\s*=\s*\w+\s*\[[^\]]*\]\s*\.\s*\w+\s*;)PY"), code))
                tab.append(a_);
        }
        const asm16::Layout* lay = old.get(*st_);
        bool size_in = lits.contains(lay->size);
        bool second = false;
        if (!size_in && allocs.contains(m.group(2))) {
            int64_t pm = 0;
            for (int64_t n_ : lits) if (prefix_member(lay, n_)) pm++;
            second = pm == 1 || (tab.size() == 1 && lits.empty());
        }
        if (size_in || second) {
            ins.push_back({m.start(2), S("(") + *st_ + S(" *)")});   // (its size, or a prefix's: s = f(1, 10), 10 where a
        }                                                            //  member of it began)
    }
    for (const Match& m : py::re::finditer(
             S(R"PY(\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(\s*([A-Za-z_]\w*)\s*\(([^()]*)\)\s*\))PY"), code)) {
        std::optional<str> st_ = ptr_of.get(m.group(1));   // (t = f08e60(f0a42b(3, 22)); t a struct pnode *: the
        if (!st_ || !old.contains(*st_) || KEYWORDS.contains(m.group(3)) || m.group(3).contains(S("alloc"))) continue;   // block made, then
        bool any = false;                                   //  filled - the inner call's block of its old size)
        for (const str& a_ : py::split(m.group(4), S(","))) {
            if (py::re::fullmatch(LIT_RX, a_) && py::int_parse(a_.strip(), 0) == old.at(*st_).size) {
                any = true;
                break;
            }
        }
        if (any) ins.push_back({m.start(3), S("(") + *st_ + S(" *)")});
    }
    std::sort(ins.begin(), ins.end());
    for (int64_t q = static_cast<int64_t>(ins.size()) - 1; q >= 0; q--) {
        auto [at, cast] = ins[q];
        text = sl(text, 0, at) + cast + text.slice_from(at);
    }
    code = blank(text);
    std::vector<Edit3> pedits;   // ((struct sym **)f(1, 22): a table of pointers by its bytes then: two each - its pointers' now)
    for (const Match& m : py::re::finditer(
             S(R"PY(\(\s*((?:struct|union)\s+\w+|\w+)\s*\*\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\()PY"), code)) {
        if (KEYWORDS.contains(m.group(2)) || MEMFNS.contains(m.group(2))) continue;
        list<std::tuple<int64_t, int64_t>> args = spans_of(code, m.end());
        std::vector<std::tuple<int64_t, int64_t>> nums;
        for (const auto& [a_, b_] : args)
            if (fullm(R"PY(\s*[1-9]\d*\s*)PY", sl(code, a_, b_))) nums.push_back({a_, b_});
        if (nums.empty()) continue;
        auto [a_, b_] = nums.back();
        int64_t n_ = py::int_parse(sl(code, a_, b_), 10);
        if (n_ % 2 == 0) {
            pedits.push_back({a_, b_, S(" ") + int_text(n_ / 2) + S(" * sizeof(") + m.group(1) + S(" *)")});
            notes[S("a table of pointers given by its bytes then (two a pointer): as many of today's")].append(
                fname + S(":") + int_text(nl_before(code, a_) + 1) + S(" (") + m.group(1) + S(" **: ") + int_text(n_) + S(")"));
        }
    }
    std::sort(pedits.begin(), pedits.end(), [](const Edit3& x, const Edit3& y) { return y < x; });
    for (const auto& [a_, b_, nw] : pedits) text = sl(text, 0, a_) + nw + text.slice_from(b_);
    code = blank(text);
    std::vector<Edit3> edits;
    for (const Match& m : CASTCALL.finditer(code)) {
        str s = ws1(m.group(1));
        if (!old.contains(s)) continue;
        list<std::tuple<int64_t, int64_t>> args = spans_of(code, m.end());
        if (args.empty()) continue;
        const int64_t osize = old.at(s).size;
        const asm16::Layout* olay = old.get(s);
        if (!py::re::search(S("alloc"), m.group(2))) {   // (a routine of the program's that gives it: one of its
            list<std::tuple<int64_t, int64_t>> whole;     //  arguments the old size itself)
            for (const auto& [a_, b_] : args) {
                str seg = sl(code, a_, b_);
                if (py::re::fullmatch(LIT_RX, seg) && py::int_parse(seg.strip(), 0) == osize) whole.append({a_, b_});
            }
            std::optional<set<int64_t>> kn_;
            if (allocs.contains(m.group(2))) kn_ = allocs.at(m.group(2));
            if (whole.size() > 1 && kn_ && kn_->size() == 1) {
                int64_t mn = *kn_->begin();
                for (int64_t q : *kn_) mn = std::min(mn, q);
                if (mn < args.size() && whole.contains(args[mn])) {
                    whole = list<std::tuple<int64_t, int64_t>>{args[mn]};   // (f03e86(4, 4): the one its size is given at)
                }
            }
            if (whole.size() != 1 && !allocs.contains(m.group(2))) {
                continue;   // (a part's size: only to an allocator's call)
            }
            if (whole.size() != 1) {
                list<std::tuple<int64_t, int64_t>> tabled;   // (s = f(far, sz), sz = g_11a8[kind].n: a size a table
                for (const auto& [a_, b_] : args) {           //  of old byte counts gives, by kind - the structure whole
                    str seg = sl(code, a_, b_);               //  now: no part of it is read past the kind's own part)
                    if (fullm(R"PY(\s*[A-Za-z_]\w*\s*)PY", seg) &&
                        py::re::search(S(R"PY(\b)PY") + esc(seg.strip()) + S(R"PY(\s*=\s*\w+\s*\[[^\]]*\]\s*\.\s*\w+\s*;)PY"), code))
                        tabled.append({a_, b_});
                }
                bool anylit = false;
                for (const auto& [a_, b_] : args)
                    if (py::re::fullmatch(LIT_RX, sl(code, a_, b_))) anylit = true;
                if (tabled.size() == 1 && !anylit) {
                    auto [a_, b_] = tabled[0];
                    str seg = sl(code, a_, b_);
                    str lead = sl(seg, 0, seg.size() - seg.lstrip().size());
                    edits.push_back({a_, b_, lead + S("sizeof(") + s + S(")")});
                    notes[S("a structure's block of a size a table of old byte counts gives (a part of it, by kind): "
                            "the whole structure now")]
                        .append(fname + S(":") + int_text(nl_before(code, a_) + 1) + S(" (") + s + S(": ") + seg.strip() + S(")"));
                    continue;
                }
                int64_t a_, b_;   // (the place its size was given at, where known)
                std::optional<set<int64_t>> at_;
                if (allocs.contains(m.group(2))) at_ = allocs.at(m.group(2));
                if (at_ && at_->size() == 1 && *at_->begin() < args.size()) {
                    std::tie(a_, b_) = args[*at_->begin()];
                } else {
                    std::tie(a_, b_) = args[-1];   // (its size the allocator's last argument: a block of
                }
                str seg = sl(code, a_, b_);
                std::vector<Match> lits;
                for (const Match& x : py::re::finditer(S(R"PY((?<![\w.])(0[xX][0-9a-fA-F]+|[1-9]\d*)(?![\w.]))PY"), seg)) {
                    if (!py::re::search(S(R"PY((?:[=!<>]=|[<>])\s*$)PY"), sl(seg, 0, x.start())) &&   // (a compared one: no size)
                        !py::re::match(S(R"PY(\s*(?:[=!<>]=|[<>]))PY"), seg.slice_from(x.end())))
                        lits.push_back(x);
                }
                bool ok = !lits.empty();
                bool anyp = false;
                for (const Match& x : lits) {
                    int64_t v = py::int_parse(x.group(1), 0);
                    if (!(v == osize || prefix_member(olay, v))) ok = false;
                    if (prefix_member(olay, v)) anyp = true;
                }
                if (ok && anyp) {   // the structure whole now: the program stepped a word past its part at times
                    for (int64_t q = static_cast<int64_t>(lits.size()) - 1; q >= 0; q--)   // (a label's num over sa), harmless in the old pool
                        edits.push_back({a_ + lits[q].start(), a_ + lits[q].end(), S("sizeof(") + s + S(")")});
                    auto cm = py::re::match(S(R"PY(\s*\(\s*(?:unsigned\s+|signed\s+)?char\s*\))PY"), seg);
                    if (cm) {   // ((char)(...): a byte held the old size - not now)
                        edits.push_back({a_ + cm->start() + seg.size() - seg.lstrip().size(), a_ + cm->end(), S("")});
                    }
                    notes[S("a block of a structure's first members, given as the bytes they were: the whole structure")]
                        .append(fname + S(":") + int_text(nl_before(code, a_) + 1) + S(" (") + s + S(": ") + seg.strip() + S(")"));
                }
                continue;
            }
            args = whole;
        }
        auto [a, b] = (m.group(2) != S("realloc")) ? args[0] : args[args.size() - 1];
        str seg = sl(code, a, b);
        list<Match> nums = py::re::finditer(S(R"PY((?<![\w.])(0[xX][0-9a-fA-F]+|\d+)(?![\w.]))PY"), seg);
        if (nums.empty()) continue;
        int64_t bestv = 0;
        int64_t bi = -1;
        for (int64_t q = 0; q < nums.size(); q++) {
            int64_t v = int_lit(nums[q].group(1));
            if (bi < 0 || v > bestv) {   // (max: the first of equals)
                bestv = v;
                bi = q;
            }
        }
        const Match& x = nums[bi];
        int64_t n = int_lit(x.group(1)), size = osize;
        if (n < size / 2) continue;   // (a count, not the structure)
        int64_t rest = n - size;
        str nw = S("sizeof(") + s + S(")") + (rest > 0 ? S(" + ") + int_text(rest) : rest < 0 ? S(" - ") + int_text(-rest) : S(""));
        edits.push_back({a + x.start(), a + x.end(), nw});
        if (m.group(2) != S("malloc") && m.group(2) != S("calloc") && m.group(2) != S("realloc") &&
            m.group(2) != S("_fmalloc") && n == size && py::re::search(S("alloc"), m.group(2))) {
            POOLS.setdefault(m.group(2), set<std::tuple<str, int64_t>>()).add({s, n});   // (the program's own allocator: given this size)
        }
        notes[S("a structure's size given as the number it had (16-bit layout): sizeof and the rest")].append(
            fname + S(":") + int_text(nl_before(code, a) + 1) + S(" (") + s + S(": ") + int_text(n) + S(")"));
    }
    std::sort(edits.begin(), edits.end(), [](const Edit3& x, const Edit3& y) { return y < x; });
    for (const auto& [a, b, nw] : edits) text = sl(text, 0, a) + nw + text.slice_from(b);
    code = blank(text);
    std::map<int64_t, std::tuple<int64_t, str>> pool_edits;
    for (const Match& m : py::re::finditer(S(R"PY(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*;)PY"), code)) {
        str s = ws1(m.group(1)), pool = m.group(2);   // (a record carved from a pool: p = (struct S *)pool;
        if (!old.contains(s)) continue;                //  pool += size - its size the old one's)
        auto bump = py::re::search(S(R"PY(\b)PY") + esc(pool) + S(R"PY(\s*\+=\s*(\w+)\s*;)PY"), sl(code, m.end(), m.end() + 400));
        if (!bump) continue;
        str var = bump->group(1);
        int64_t start = code.rfind(S("\n{"), 0, m.start());
        const asm16::Layout* lay = old.get(s);
        str region = sl(code, start, m.start());
        bool broke = false;
        for (const Match& a : py::re::finditer(S(R"PY(\b)PY") + esc(var) + S(R"PY(\s*=\s*(\d+)\s*;)PY"), region)) {
            int64_t n = py::int_parse(a.group(1), 10);
            if (2 <= n && n <= lay->size) {
                int64_t x = start + a.start(1);
                pool_edits[x] = {x + a.group(1).size(), S("sizeof(") + s + S(")")};
                notes[S("a record carved from a pool by the old size of its structure (part of it): the whole "
                        "structure's size now, so that none reaches into the next")]
                    .append(fname + S(":") + int_text(nl_before(code, x) + 1) + S(" (") + s + S(": ") + int_text(n) + S(")"));
                broke = true;
                break;
            }
        }
        if (!broke) {   // (size = extra[kind] + n + 0x1c: the old common part, what the kind had
            set<int64_t> offs;
            for (const auto& [k, v] : lay->members) offs.add(v.off);
            offs.add(lay->size);
            for (const Match& a : py::re::finditer(S(R"PY(\b)PY") + esc(var) + S(R"PY(\s*=\s*([^;=]+);)PY"), region)) {
                list<str> terms, lits;
                for (const str& t : py::re::split(S(R"PY(\+)PY"), a.group(1))) terms.append(t.strip());
                for (const str& t : terms) if (fullm(R"PY(0[xX][0-9a-fA-F]+|\d+)PY", t)) lits.append(t);
                if (lits.size() != 1 || !offs.contains(py::int_parse(lits[0], 0)) || py::int_parse(lits[0], 0) < 2 ||
                    a.group(1).contains(S("-")))
                    continue;
                list<str> keep;
                for (const str& t : terms)
                    if (!lits.contains(t) && !fullm(R"PY(\w+\s*\[[^\]]*\])PY", t)) keep.append(t);
                int64_t x = start + a.start(1), e = start + a.end(1);
                list<str> parts = {S("sizeof(") + s + S(")")};
                parts.extend(keep);
                pool_edits[x] = {e, py::join(S(" + "), parts)};
                notes[S("a record carved from a pool by the old sizes of its parts (the common part, what its "
                        "kind had past it): the whole structure now, so that none reaches into the next")]
                    .append(fname + S(":") + int_text(nl_before(code, x) + 1) + S(" (") + s + S(": ") + a.group(1).strip() + S(")"));
                break;
            }
        }
    }
    for (auto it = pool_edits.rbegin(); it != pool_edits.rend(); ++it) {
        int64_t x = it->first;
        auto& [e, nw] = it->second;
        text = sl(text, 0, x) + nw + text.slice_from(e);
    }
    return text;
}

// `extern T f(), g();` lines of functions the program defines or the C library has: gone (proto.h and the
// library's headers declare them); a variable on the same line is kept - unless the platform layer declares it
// (what the old runtime's assembler defined, as ipnCore)
str old_declarations(const str& text, const str& code, const set<str>& known, Notes& notes, const str& fname,
                     const list<str>& platform) {
    list<str> lines = lines_of(text), clines = lines_of(code);
    list<str> jl, jc;   // (extern f(), g(),\n h(); - one declaration: one line)
    int64_t i = 0;
    while (i < lines.size()) {
        str s = clines[i].strip();
        if (s.startswith(S("extern")) && !s.endswith(S(";")) && !s.contains(S("{"))) {
            int64_t k = i + 1;
            while (k < lines.size() && k - i < 12 && !clines[k].strip().endswith(S(";")) && !clines[k].contains(S("{"))) k += 1;
            if (k < lines.size() && k - i < 12 && !clines[k].contains(S("{")) &&
                !py::join(S(""), clines.slice(i, k + 1)).contains(S("#"))) {
                list<str> parts_l = {lines[i].rstrip()}, parts_c = {clines[i].rstrip()};
                for (int64_t q = i + 1; q < k + 1; q++) {
                    parts_l.append(lines[q].strip());
                    parts_c.append(clines[q].strip());
                }
                jl.append(py::join(S(" "), parts_l));
                jc.append(py::join(S(" "), parts_c));
                i = k + 1;
                continue;
            }
        }
        jl.append(lines[i]);
        jc.append(clines[i]);
        i += 1;
    }
    list<str> out;
    for (int64_t q = 0; q < jl.size(); q++) {
        const str& l = jl[q];
        const str& c = jc[q];
        str s = c.strip();
        bool plat = false;
        if (s.startswith(S("extern"))) {
            for (const str& p : platform) {
                if (py::re::search(S(R"PY(\b)PY") + esc(p) + S(R"PY(\b)PY"), s)) {
                    plat = true;
                    break;
                }
            }
        }
        std::optional<std::tuple<str, list<std::tuple<str, str>>>> r;
        if (s.endswith(S(";")) && (s.contains(S("()")) || plat) && !s.contains(S("="))) r = declarators(s);
        str base = r ? std::get<0>(*r) : S("");
        bool sub_ok = false;
        if (r && !base.empty()) {
            sub_ok = true;
            for (const str& w : py::split(py::re::sub(S(R"PY(\b(?:struct|union)\s+\w+)PY"), S(""), base)))
                if (!TYPEWORDS.contains(w)) sub_ok = false;
        }
        if (!r || base.empty() || !sub_ok) {
            out.append(l);
            continue;
        }
        const auto& ds = std::get<1>(*r);
        list<str> keep;
        for (const auto& [n, d] : ds) {
            if (!((d.endswith(S("()")) && known.contains(n)) || platform.contains(n))) keep.append(d);
        }
        // (the Python's `n not in platform`, with `d.endswith("()") and n in known` as the other half)
        if (keep.size() == ds.size()) {
            out.append(l);
            continue;
        }
        notes[S("old declaration removed (proto.h / the C library declare it)")].append(fname + S(":") + int_text(out.size() + 1));
        if (!keep.empty()) {
            str ind = sl(l, 0, l.size() - l.lstrip().size());
            out.append(ind + base + S(" ") + py::join(S(", "), keep) + S(";"));
        }
    }
    return unlines(out);
}

// a header's variables without extern (each file that included it had them; the linker made them one):
// declared extern here, -> (text, [definitions for data.c])
std::tuple<str, list<str>> header_data(const str& text, const str& code, const str& fname, Notes& notes) {
    list<str> lines = lines_of(text), clines = lines_of(code);
    list<int64_t> ds = depths(clines);
    list<str> defs;
    for (int64_t i = 0; i < clines.size(); i++) {
        str s = clines[i].strip();
        if (ds[i] || !s.endswith(S(";")) || s.startswith(std::make_tuple(S("#"), S("extern"), S("typedef"), S("static"))) ||
            (s.contains(S("(")) && !py::re::search(S(R"PY(\(\s*\*)PY"), s)) || s.contains(S("{")) || s.contains(S("}")) ||
            s.contains(S("="))) {
            continue;
        }
        auto r = declarators(s);
        if (!r || std::get<0>(*r).empty()) continue;
        defs.append(lines[i].strip());
        lines[i] = py::re::sub(S(R"PY(^(\s*))PY"), S(R"PY(\1extern )PY"), lines[i]);
        notes[S("shared variable of a header: extern there, defined once in data.c")].append(fname + S(":") + int_text(i + 1));
    }
    return {unlines(lines), defs};
}

// ================================================================ the program

Port::Port(const dict<str, str>& files_in, const Profile& profile_, const dict<str, str>& members) {
    profile = profile_;
    dict<str, str> files0 = files_in.copy();
    for (const auto& [key, typ] : members.items()) {   // {"struct.member": type}
        int64_t dot = key.find(S("."));
        str st = sl(key, 0, dot), mem = key.slice_from(dot + 1);
        for (const auto& [n, t] : files0.items()) {
            auto m = py::re::search(S(R"PY(\bstruct\s+)PY") + esc(st) + S(R"PY(\s*\{)PY"), t);
            if (!m) continue;
            int64_t end = t.find(S("};"), m->end());
            str body = sl(t, m->end(), end);
            str nw = py::re::Pattern(S(R"PY((?m)^([ \t]*)[^;{}\n]*?\b)PY") + esc(mem) + S(R"PY(\s*;)PY"), 0)
                         .sub_fn([&](const Match& x) { return x.group(1) + typ + sp_for(typ) + mem + S(";"); }, body, 1);
            if (nw != body) {
                files0.set(n, sl(t, 0, m->end()) + nw + t.slice_from(end));
                member_fixes.append(n + S(": struct ") + st + S("'s ") + mem + S(" is ") + typ + S(" (as the program uses it)"));
            }
        }
    }
    originals = files0.copy();   // (the source as it was: the old layouts, far ones)
    for (const auto& [n, t] : files0.items()) {
        std::optional<str> why = _runtime(n, t);
        if (why) {
            left_out.set(n, *why);
        } else {
            files.set(n, t);
        }
    }
    py::re::Pattern stand = py::re::compile(S(R"PY(/\* SAMEFILE \*/\n([^\n{;]*\([^)]*\))\s*\{\s*\})PY"));
    for (const auto& [n, t] : files.items()) {   // (empty stand-ins of functions of the original's same
        str nw = stand.sub_fn(                      //  file, made for the byte-exact build: declarations)
            [](const Match& m) { return S("extern ") + py::re::sub(S(R"PY(\(.*\))PY"), S("()"), m.group(1)).strip() + S(";"); }, t);
        if (nw != t) {
            files.set(n, nw);
            notes[S("an empty stand-in of a function defined elsewhere (for the byte-exact build): declared instead")].append(n);
        }
    }
    set<str> internals = set<str>::from(profile.runtime_internals);
    for (const auto& [n, t] : files.items()) {   // (the old library's own structures, copied into the
        str nw = py::re::sub(S(R"PY((?m)^[ \t]*struct\s+(\w+)\s*\{[^{}]*\}\s*;[^\n]*\n?)PY"),   // program's header - FILE's:
                             [&](const Match& m) { return internals.contains(m.group(1)) ? S("") : m.group(0); }, t);   // today's library's)
        if (nw != t) {
            files.set(n, nw);
            union_notes[S("the old library's own structure, copied into the program: today's library's instead")].append(n);
        }
    }
    {
        asm16::Layouts old_l = asm16::CTypes::_layouts(list<str>::from(originals.values()), 2, 4);
        asm16::Layouts new_l = asm16::CTypes::_layouts(list<str>::from(files.values()), 8, 8);
        dict<str, std::tuple<int64_t, int64_t>> sizes;
        for (const str& k : old_l.map.keys()) {
            if (new_l.contains(k)) sizes.set(k, {old_l.at(k).size, new_l.at(k).size});
        }
        for (const str& n : py::sorted(files)) {   // (unions whose views meet at the old places: so still)
            files.set(n, union_views(files.at(n), union_notes, n, sizes));
        }
    }
    anon_tags();
    view_headers = set<str>();
    struct_views();
    word_columns();
    twin_structs();
    cast_twins();
    fnptr_returns();
    link_words();
    array_tails();
    set<std::tuple<str, str>> casts;   // (struct rfld *)&sym->u.rfl: a structure of its own laid
    for (const str& t : files.values()) {  //  over a view of the union - laid out as the view is
        for (const Match& m : py::re::finditer(
                 S(R"PY(\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*&[^;,)]*?\bu\.(\w+))PY"), blank(t)))
            casts.add({m.group(1), m.group(2)});
    }
    for (const auto& [tag, view] : py::sorted(casts)) {
        std::optional<str> vbody;
        for (const str& t : files.values()) {
            auto vm = py::re::search(S(R"PY(\bstruct\s*(?:\w+\s*)?\{([^{}]*)\}\s*)PY") + esc(view) + S(R"PY(\s*;)PY"), t);   // (tagged or not)
            if (vm && vm->group(1).contains(S("_old"))) vbody = vm->group(1);
        }
        if (!vbody || vbody->empty()) continue;
        for (const auto& [n, t] : files.items()) {
            auto sm = py::re::search(S(R"PY(\bstruct\s+)PY") + esc(tag) + S(R"PY(\s*\{[^{}]*\})PY"), t);
            if (sm) {
                str nw = S("struct ") + tag + S(" {") + *vbody + S("}");
                files.set(n, sl(t, 0, sm->start()) + nw + t.slice_from(sm->end()));
                union_notes[S("a structure laid over a view of a union (by a cast): laid out as the view is")].append(
                    n + S(" (struct ") + tag + S(", u.") + view + S(")"));
            }
        }
    }
    set<std::tuple<str, str>> over;   // (((struct comsym *)sym)->count: a structure that is
    for (const str& t : files.values()) {   //  another's first bytes and then members of it)
        for (const Match& m : py::re::finditer(
                 S(R"PY(\(\s*\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*(\w+)\s*\)\s*->)PY"), blank(t))) {
            auto d_ = py::re::search(S(R"PY(\bstruct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*)PY") + esc(m.group(2)) + S(R"PY(\b)PY"), blank(t));
            if (d_) over.add({m.group(1), S("struct ") + d_->group(1)});
        }
    }
    if (!over.empty()) {
        asm16::Layouts old_l = asm16::CTypes::_layouts(list<str>::from(originals.values()), 2, 4);
        asm16::Layouts new_l = asm16::CTypes::_layouts(list<str>::from(files.values()), 8, 8);
        asm16::CTypes ct_;
        ct_.old = old_l;
        ct_.new_ = new_l;
        for (const auto& [tag, under] : py::sorted(over)) {
            for (const auto& [n, t] : files.items()) {
                auto sm = py::re::search(S(R"PY(\bstruct\s+)PY") + esc(tag) +
                                             S(R"PY(\s*\{\s*char\s+(\w+)\s*\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\]\s*;)PY"),
                                         blank(t));   // (its comments aside: the places are the text's)
                if (!sm) continue;
                int64_t k = py::int_parse(sm->group(2), 0);
                std::optional<asm16::Field> f;
                if (old_l.contains(under)) f = ct_.field(under, k);   // (the member of the other there)
                std::optional<std::tuple<str, int64_t>> best;
                if (f && std::get<1>(*f) == k) best = std::tuple<str, int64_t>{under, std::get<3>(*f)};
                if (best && std::get<1>(*best) != k) {
                    files.set(n, sl(t, 0, sm->start(2)) + int_text(std::get<1>(*best)) + t.slice_from(sm->end(2)));
                    union_notes[S("a structure that is another's first bytes and then members of it: the bytes as many as "
                                  "the other's now")]
                        .append(n + S(" (struct ") + tag + S(" over ") + std::get<0>(*best) + S(": ") + int_text(std::get<1>(*best)) + S(")"));
                }
            }
        }
    }
    py::re::Pattern fp = py::re::compile(
        S(R"PY(FP_SEG\s*\(\s*(\w+)\s*\)\s*=\s*([^;]+);\s*FP_OFF\s*\(\s*\1\s*\)\s*=\s*([^;]+);)PY"));
    for (const auto& [n, t] : files.items()) {   // (a far pointer made of a segment and an offset: the
        str nw = fp.sub_fn(                         //  platform layer gives the place, as DOS had it)
            [](const Match& m) {
                return m.group(1) + S(" = dos_far(") + m.group(2).strip() + S(", ") + m.group(3).strip() + S(");");
            },
            t);
        if (nw != t) {
            files.set(n, nw);
        }
    }
    if (profile.keywords_removed && !profile.keywords_removed->empty()) {   // (far, near, pascal...: the 8086's segments and
        const list<str>& kw = *profile.keywords_removed;                    //  calling conventions, which a flat memory has not)
        set<str> kwset = set<str>::from(kw);
        set<str> mac;   // (a macro of those words alone - #define NEAR near pascal: gone too, and the word where it was used)
        for (const str& t : files.values()) {
            for (const Match& m : py::re::finditer(S(R"PY((?m)^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+([\w \t]+?)[ \t]*$)PY"), t)) {
                bool sub_ok = true;
                for (const str& w : py::split(m.group(2)))
                    if (!kwset.contains(w)) sub_ok = false;
                if (sub_ok) mac.add(m.group(1));
            }
        }
        list<str> macs = py::sorted(mac);
        if (!mac.empty()) {
            py::re::Pattern drx = py::re::compile(S(R"PY((?m)^[ \t]*#[ \t]*define[ \t]+(?:)PY") + py::join(S("|"), macs) +
                                                  S(R"PY()[ \t]+[\w \t]*\n)PY"));
            for (const auto& [n, t] : files.items()) files.set(n, drx.sub(S(""), t));
        }
        list<str> alts;
        for (const str& w : kw) alts.append(esc(w));
        for (const str& w : macs) alts.append(esc(w));
        py::re::Pattern rx = py::re::compile(S(R"PY((?<!->)(?<!\.)\b(?:)PY") + py::join(S("|"), alts) +
                                             S(R"PY()\b(?!\s*[,;=\[)])[ \t]*)PY"));
        //                 (a qualifier: not a name of its own - char near, x3; p->far)
        for (const auto& [n, t] : files.items()) {
            str nw = sub_code(t, rx, [](const Match&) { return S(""); });
            if (nw != t) {
                files.set(n, nw);
                notes_init[n] += rx.finditer(blank(t)).size();
            }
        }
    }
    // self.fns = {}, data = [], line_origin = {}, line_fixes = {}, mended (the loop's), decl_rules = [], fptr, param_types...
    renames = profile.platform_renames.copy();   // (a function whose name the C library has: renamed; the platform's own)
    int16 = profile.int_storage && *profile.int_storage == 16;   // (every int 16 bits; else only where bytes are laid out)
}

std::optional<str> Port::_runtime(const str& name, const str& text) {
    const Profile& p = profile;
    if (name.endswith(S(".h")) && p.runtime_headers.contains(name)) {
        return S("the old C library's header: the C library of today has its own");
    }
    if (name.endswith(S(".c"))) {
        str code = blank(text);
        list<str> got;
        for (const auto& [f, s, j, k] : definitions(lines_of(text), lines_of(code))) {
            if (p.runtime_defines.contains(f->name)) got.append(f->name);
        }
        if (!got.empty()) {
            return S("the old C library (it defines ") + py::join(S(", "), got) + S("): the C library of today stands in");
        }
    }
    return std::nullopt;
}

}  // namespace codebreaker::port::pre_ansi
