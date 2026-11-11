// codebreaker/port/pre_ansi_p2.cpp -- typed C++ port of codebreaker.port.pre_ansi, PART 2: the Port methods
// agree_globals ... _globals (Python lines 886-2145). See pre_ansi.hpp for the typing notes.

#include <algorithm>
#include <cstdio>
#include <functional>

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

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
// s[:at].count("\n")
int64_t nl_before(const str& s, int64_t at) { return sl(s, 0, at).count(S("\n")); }
// s.replace(a, b, 1)
str replace1(const str& s, const str& a, const str& b) {
    int64_t i = s.find(a);
    if (i < 0) return s;
    return sl(s, 0, i) + b + s.slice_from(i + a.size());
}
// the strings of a list as the Python's text.split("\n")
list<str> lines_of(const str& t) { return py::split(t, S("\n")); }
str unlines(const list<str>& l) { return py::join(S("\n"), l); }
// sorted(edits, reverse=True)
void sort_desc(std::vector<Edit3>& e) { std::sort(e.begin(), e.end(), [](const Edit3& a, const Edit3& b) { return b < a; }); }
void apply_desc(str& t, std::vector<Edit3> e) {
    sort_desc(e);
    for (const auto& [a, b, n] : e) t = sl(t, 0, a) + n + t.slice_from(b);
}
// int(x, 0)
int64_t int0(const str& x) { return py::int_parse(x, 0); }
// the regex of a literal number: 0x.. or decimal
bool fullm(const char* pat, const str& s) { return static_cast<bool>(py::re::fullmatch(S(pat), s)); }
str fmt_int(int64_t v) { return py::to_str(v); }
str hex_lower(int64_t v, int width) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%0*llx", width, static_cast<unsigned long long>(v));
    return str(buf);
}
str hex_plain(int64_t v) { return hex_lower(v, 1); }
bool ends_star(const str& t) { return t.endswith(S("*")); }
// "" if the type ends in * else " "
str sp_for(const str& t) { return ends_star(t) ? S("") : S(" "); }
bool starts_any(const str& s, std::initializer_list<const char*> ps) {
    for (const char* p : ps) if (s.startswith(S(p))) return true;
    return false;
}
// {struct S *p ...}: the structure each pointer variable of a code is declared to point to
dict<str, str> ptr_of_map(const str& code) {
    dict<str, str> out;
    for (const auto& m : py::re::finditer(S(R"PY(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])PY"), code))
        out.set(m.group(2), ws1(m.group(1)));
    return out;
}
// the Python's `-ish` helper: x in d for a dict<str,...>
template <class V> bool has(const dict<str, V>& d, const str& k) { return d.contains(k); }

}  // namespace

// ================================================================ agree_globals
// extern unsigned g; in one file, extern char *g; in another (the old linker did not mind): a pointer in
// every file - and the number that was the top of the segment (0xffff), the top of memory now
void Port::agree_globals() {
    if (_agreed) return;
    _agreed = true;
    py::re::Pattern decl = py::re::compile(
        S(R"PY((?m)^(\s*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+)\s*;)PY"));
    dict<str, str> ptrs;
    for (const auto& [n, t] : files.items()) {
        for (const auto& m : py::re::finditer(
                 S(R"PY((?m)^\s*extern\s+((?:const\s+)?(?:struct\s+\w+|union\s+\w+|\w+)\s*\*+)\s*(\w+)\s*;)PY"), blank(t)))
            ptrs.setdefault(m.group(2), replace1(replace1(ws1(m.group(1)), S(" *"), S("*")), S("*"), S(" *")));
        if (rule(S("agree_ptr_lists"))) {
            // (extern char *a, *b;)
            for (const auto& m : py::re::finditer(
                     S(R"PY((?m)^\s*extern\s+((?:struct\s+\w+|union\s+\w+|(?:unsigned\s+)?char|int))\s+(\*\s*\w+(?:\s*,\s*\*\s*\w+)+)\s*;)PY"),
                     blank(t)))
                for (const auto& w : py::split(m.group(2), S(",")))
                    ptrs.setdefault(w.replace(S("*"), S("")).strip(), ws1(m.group(1)) + S(" *"));
        }
    }
    set<str> ext_ptrs = set<str>::from(ptrs.keys());   // (what files declare extern: the program's variables)
    for (const auto& [n, t] : files.items()) {          // (and its definition's: FILE *g = 0;)
        for (const auto& m : py::re::finditer(
                 S(R"PY((?m)^((?:const\s+)?(?:struct\s+\w+|union\s+\w+|(?!static\b|extern\b|return\b)\w+)\s*\*+)\s*(\w+)\s*(?:=[^;]*)?;)PY"),
                 blank(t)))
            ptrs.setdefault(m.group(2), replace1(replace1(ws1(m.group(1)), S(" *"), S("*")), S("*"), S(" *")));
    }
    agree_notes = list<str>();
    py::re::Pattern many = py::re::compile(
        S(R"PY((?m)(?:^|(?<=;))([ \t]*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+(?:\s*,\s*\w+)+)\s*;)PY"));
    for (const auto& n : py::sorted(files.keys())) {   // (extern int f, g; - g a pointer: a line of its own)
        str t = files.at(n);
        auto ms = many.finditer(blank(t));
        for (int64_t i = ms.size() - 1; i >= 0; --i) {
            const Match& m = ms[i];
            list<str> names;
            for (const auto& w : py::split(m.group(3), S(","))) names.append(w.strip());
            bool any = false;
            for (const auto& w : names) any = any || ptrs.contains(w);
            if (!any) continue;
            list<str> rest, parts;
            for (const auto& w : names) if (!ptrs.contains(w)) rest.append(w);
            if (rest.size()) parts.append(m.group(1) + m.group(2) + py::join(S(", "), rest));
            for (const auto& w : names) if (ptrs.contains(w)) parts.append(m.group(1) + m.group(2) + w);
            str nw = py::join(S(";\n"), parts) + S(";");
            t = sl(t, 0, m.start()) + nw + t.slice_from(m.end());
        }
        files.set(n, t);
    }
    for (const auto& n : py::sorted(files.keys())) {
        str t = files.at(n);
        list<str> names;
        for (const auto& m : decl.finditer(blank(t))) if (ptrs.contains(m.group(3))) names.append(m.group(3));
        for (const auto& name : names) {
            str typ = ptrs.at(name);
            t = py::re::sub(S(R"PY((?m)^(\s*extern\s+)(?:unsigned|int|short|unsigned\s+int)\s+)PY") + esc(name) + S(R"PY(\s*;)PY"),
                            [&](const Match& m) { return m.group(1) + typ + sp_for(typ) + name + S(";"); }, t);
            t = py::re::sub(S(R"PY(\b()PY") + esc(name) + S(R"PY(\s*=\s*)(?:0[xX][fF]{4}|65535)\b)PY"),
                            [&](const Match& m) { return m.group(1) + S("(") + typ + S(")UINTPTR_MAX"); }, t);
            agree_notes.append(py::percent(S("%s (%s)"), n, name));
        }
        set<int64_t> heads_;   // (an old-style definition's parameter lines - int v; between f(v) and its { - are no
        if (rule(S("agree_ptr_defs")) && n.endswith(S(".c"))) {   //  definitions)
            for (const auto& d : definitions(lines_of(t), lines_of(blank(t))))
                for (int64_t k = std::get<1>(d); k < std::get<2>(d) + 1; ++k) heads_.add(k);
        }
        if (rule(S("agree_ptr_defs"))) {
            auto ms = py::re::finditer(S(R"PY((?m)^(?:unsigned\s+int|unsigned|int|short)\s+(\w+)\s*(=\s*0\s*)?;)PY"), blank(t));
            for (int64_t i = ms.size() - 1; i >= 0; --i) {
                const Match& m = ms[i];
                // (and its definition: int g_0c6a = 0; - extern char *g_0c6a; in the files that use it)
                str name = m.group(1);
                if (ext_ptrs.contains(name) && !heads_.contains(nl_before(blank(t), m.start())) &&
                    !py::re::search(S(R"PY(\b)PY") + esc(name) + S(R"PY(\s*\()PY"), sl(blank(t), m.start(), m.end()))) {
                    str typ = ptrs.at(name);
                    t = sl(t, 0, m.start()) + typ + sp_for(typ) + name + S(";") +
                        (m.group_opt(2) ? S(" /* (= 0) */") : S("")) + t.slice_from(m.end());
                    agree_notes.append(py::percent(S("%s (%s)"), n, name));
                }
            }
        }
        files.set(n, t);
    }
    dict<str, str> longs;   // (extern long g_0cb0; - its definition int g_0cb0 = 0; and int g_0cb2 = 0;, the two
    if (rule(S("agree_longs"))) {   //  words of the long, the old data's layout: the long its definition)
        for (const auto& t : files.values())
            for (const auto& m : py::re::finditer(S(R"PY((?m)^\s*extern\s+((?:unsigned\s+)?long)\s+([\w\s,]+);)PY"), blank(t)))
                for (const auto& w : py::split(m.group(2), S(",")))
                    if (fullm(R"PY(\s*\w+\s*)PY", w)) longs.setdefault(w.strip(), m.group(1));
    }
    for (const auto& n : py::sorted(files.keys())) {
        str t = files.at(n);
        str code = blank(t);
        std::vector<Edit3> edits;
        for (const auto& m : py::re::finditer(
                 S(R"PY((?m)^((?:unsigned\s+int|unsigned|int|short))\s+(\w+)\s*(=\s*([^;,]+))?;([^\n]*))PY"), code)) {
            str nm = m.group(2);
            if (!longs.contains(nm)) continue;
            str lo = m.group_opt(4) ? m.group(4).strip() : S("0");
            auto hi_m = py::re::match(
                S(R"PY(\n(?:unsigned\s+int|unsigned|int|short)\s+(g_[0-9a-f]{4})\s*(?:=\s*([^;,]+))?;[^\n]*)PY"),
                code.slice_from(m.end()));
            std::optional<str> nxt;
            if (fullm(R"PY(g_[0-9a-f]{4})PY", nm)) nxt = S("g_") + hex_lower(py::int_parse(nm.slice_from(2), 16) + 2, 4);
            if (!fullm(R"PY(-?(?:0[xX][0-9a-fA-F]+|\d+))PY", lo)) continue;
            int64_t val = int0(lo) & 0xffff;
            int64_t end = m.end();
            bool other = false;
            if (hi_m && nxt && hi_m->group(1) == *nxt) {
                other = false;
                for (const auto& [k_, o] : files.items())
                    if (k_ != n && py::re::search(S(R"PY(\b)PY") + *nxt + S(R"PY(\b)PY"), blank(o))) other = true;
            }
            if (hi_m && nxt && hi_m->group(1) == *nxt && !other) {
                str hv = (hi_m->group_opt(2) ? *hi_m->group_opt(2) : S("0")).strip();
                if (fullm(R"PY(-?(?:0[xX][0-9a-fA-F]+|\d+))PY", hv)) {
                    val |= (int0(hv) & 0xffff) << 16;   // (the high word, no other file's: in the long now)
                    end = m.end() + hi_m->end();
                }
            }
            str typ = !m.group(1).contains(S("unsigned")) ? longs.at(nm) : S("unsigned long");
            edits.emplace_back(m.start(), end,
                               typ + S(" ") + nm + S(" = ") + (val > 0xffff ? S("0x") + hex_plain(val) + S("L") : fmt_int(int0(lo))) +
                                   S(";") + m.group(5));
        }
        for (int64_t i = static_cast<int64_t>(edits.size()) - 1; i >= 0; --i) {
            const auto& [a, b, nw] = edits[i];
            t = sl(t, 0, a) + nw + t.slice_from(b);
        }
        if (!edits.empty()) {
            files.set(n, t);
            list<str> names;
            for (const auto& e : edits) names.append(py::re::search(S(R"PY(\s(\w+)\s=)PY"), std::get<2>(e))->group(1));
            long_notes.append(py::percent(S("%s (%s)"), n, py::join(S(", "), names)));
        }
    }
}

// whether a table's definition is followed in its file by one of the same type (what an index past its
// end reached, in the original)
bool Port::joins_next(const str& var) {
    py::re::Pattern defn = py::re::compile(
        S(R"PY((?m)^((?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned))\s+)PY") + esc(var) +
        S(R"PY(\s*\[\s*\d*\s*\]\s*=[^;]*;[^\n]*\n\1\s+\w+\s*(?:\[\s*\d*\s*\])?\s*=)PY"));
    for (const auto& t : files.values())
        if (defn.search(t)) return true;
    return false;
}

// char *pool_alloc() { ... malloc(36) ... }, its callers (struct node *)pool_alloc(): the program's own
// allocator of blocks of one old size, for structures - blocks of the structures' size now
void Port::fixed_allocators() {
    if (_fixed) return;
    _fixed = true;
    const asm16::Layouts& old = old_layouts();
    dict<str, set<str>> users;
    for (const auto& [n, t] : files.items())
        for (const auto& m : py::re::finditer(S(R"PY(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*\(\s*\))PY"), blank(t)))
            users.setdefault(m.group(2), set<str>()).add(ws1(m.group(1)));
    for (const auto& n : py::sorted(files.keys())) {
        str t = files.at(n);
        str code = blank(t);
        for (const auto& d : definitions_any(lines_of(t), lines_of(code))) {
            const str& f = std::get<0>(d);
            int64_t j = std::get<2>(d), k = std::get<3>(d);
            list<str> structs;
            if (users.contains(f))
                for (const auto& x : users.at(f)) if (old.contains(x)) structs.append(x);
            structs = py::sorted(structs);
            if (!structs.size()) continue;
            list<str> lines = lines_of(t);
            list<str> clines = lines_of(code);
            for (int64_t x = j; x < k + 1; ++x) {
                auto m = py::re::search(S(R"PY(\b\w*alloc\w*\s*\(\s*(0[xX][0-9a-fA-F]+|\d+)\s*\))PY"), clines[x]);
                if (!m) continue;
                int64_t size = int0(m->group(1));
                int64_t big = 0;
                bool first = true;
                for (const auto& st_ : structs) {
                    int64_t z = old.at(st_).size;
                    big = first ? z : std::max(big, z);
                    first = false;
                }
                if (size < big) continue;   // (not the structures' blocks)
                str expr = S("sizeof(") + structs[0] + S(")");
                for (int64_t q = 1; q < structs.size(); ++q) {
                    const str& st_ = structs[q];
                    expr = S("(sizeof(") + st_ + S(") > ") + expr + S(" ? sizeof(") + st_ + S(") : ") + expr + S(")");
                }
                int64_t rest = size - big;
                if (rest) expr = expr + S(" + ") + fmt_int(rest);
                auto [a, b] = m->span(1);
                lines[x] = sl(lines[x], 0, a) + expr + lines[x].slice_from(b);
                alloc_notes.append(py::percent(S("%s:%d (%s: %d for %s)"), n, x + 1, f, size, py::join(S(", "), structs)));
            }
            t = unlines(lines);
        }
        files.set(n, t);
    }
}

// if (x >= g_16ec) - x a struct xstate * into g_123c[200], g_16ec the variable the old linker put right
// after g_123c (another type's): the comparison was with g_123c's end - (g_123c + 200)
void Port::array_bounds() {
    if (_bounds_done) return;
    _bounds_done = true;
    dict<str, int64_t> dsl = ds.copy();
    if (dsl.empty()) return;
    for (const auto& t : originals.values())   // (a variable named by its place, g_748e: there)
        for (const auto& m : py::re::finditer(S(R"PY(\bg_([0-9a-f]{4})\b)PY"), t))
            dsl.setdefault(S("g_") + m.group(1), py::int_parse(m.group(1), 16));
    const asm16::Layouts& old = old_layouts();
    // (name: (base type, elements, old element size))
    dict<str, std::tuple<str, int64_t, int64_t>> arrays;
    for (const auto& t : originals.values()) {
        str code = blank(t);
        list<str> cl = lines_of(code);
        list<int64_t> dps = depths(cl);
        for (int64_t i = 0; i < cl.size(); ++i) {
            const str& l = cl[i];
            if (dps[i] || starts_any(l.lstrip(), {"extern", "#", "typedef"})) continue;
            auto m = py::re::match(
                S(R"PY(\s*(?:static\s+)?((?:struct|union)\s+\w+|(?:unsigned\s+)?\w+)\s+(\*?)\s*(\w+)\s*\[\s*(\d+)\s*\]\s*[;=])PY"), l);
            if (!m || !dsl.contains(m->group(3))) continue;
            str base = ws1(m->group(1));
            int64_t esz;
            if (!m->group(2).empty()) esz = 2;
            else if (old.contains(base)) esz = old.at(base).size;
            else esz = OLD_SCALARS.get(base, 0);
            if (esz) arrays.set(m->group(3), {base + (!m->group(2).empty() ? S(" *") : S("")), py::int_parse(m->group(4)), esz});
        }
    }
    dict<int64_t, list<str>> starts;
    for (const auto& [nm, at] : dsl.items()) starts.setdefault(at, list<str>()).append(nm);
    dict<str, std::tuple<str, int64_t, int64_t>> tables = arrays.copy();   // (and a table only declared, extern int
    for (const auto& t : originals.values()) {                              //  g_748e[] - in the BSS: as many as up to
        for (const auto& l : lines_of(blank(t))) {                          //  the next variable)
            auto m = py::re::match(S(R"PY(\s*extern\s+((?:unsigned\s+)?\w+)\s+(.*);)PY"), l);
            if (!m || m->group(1) == S("struct") || m->group(1) == S("union")) continue;
            str base = ws1(m->group(1));
            int64_t esz = OLD_SCALARS.get(base, 0);
            for (const auto& mm : py::re::finditer(S(R"PY((?:^|,)\s*(\w+)\s*\[\s*\])PY"), m->group(2))) {
                str nm = mm.group(1);
                if (!esz || tables.contains(nm) || !dsl.contains(nm)) continue;
                std::optional<int64_t> nxt;
                for (int64_t v : dsl.values()) if (v > dsl.at(nm) && (!nxt || v < *nxt)) nxt = v;
                if (nxt && (*nxt - dsl.at(nm)) % esz == 0) tables.set(nm, {base, (*nxt - dsl.at(nm)) / esz, esz});
            }
        }
    }
    auto same_ = [](const str& u) {
        str n = ws1(u).strip();
        if (n == S("int16_t") || n == S("short")) return S("int");
        if (n == S("uint16_t") || n == S("unsigned int")) return S("unsigned");
        if (n == S("int8_t")) return S("char");
        return n;
    };
    auto tabs = tables.items();
    std::sort(tabs.begin(), tabs.end(), [](const auto& x, const auto& y) { return std::get<0>(x) < std::get<0>(y); });
    for (const auto& [a, tv] : tabs) {
        const auto& [base, cnt, esz] = tv;
        list<str> after;
        if (starts.contains(dsl.at(a) + cnt * esz))
            for (const auto& b : starts.at(dsl.at(a) + cnt * esz)) if (b != a) after.append(b);
        for (const auto& b : after) {
            std::optional<str> bbase = arrays.contains(b) ? std::optional<str>(std::get<0>(arrays.at(b))) : std::nullopt;
            if (bbase && *bbase == base) continue;   // (one type: the tables' join does it)
            for (const auto& n : py::sorted(files.keys())) {
                str t = files.at(n);
                str code = blank(t);
                std::vector<Edit3> edits;
                str pat = S(R"PY((?<![\w.>])([\w.\->]+)\s*(>=|<=|<|>)\s*)PY") + esc(b) + S(R"PY(\b(?!\s*[\[.(])|(?<![\w.>]))PY") + esc(b) +
                          S(R"PY(\b(?!\s*[\[.(])\s*(>=|<=|<|>)\s*([\w.\->]+))PY");
                for (const auto& m : py::re::finditer(pat, code)) {
                    str v = m.group_opt(1) ? *m.group_opt(1) : *m.group_opt(4);
                    list<str> words_;
                    for (const auto& w : py::re::finditer(S(R"PY(\w+)PY"), v)) words_.append(w.group());
                    if (!words_.size()) continue;   // (a -> or . alone: no name)
                    str last = words_[-1];
                    std::optional<str> pt;
                    if (fullm(R"PY(\w+)PY", v)) pt = pointee_of(t, last);
                    if ((!pt || pt->empty()) && fullm(R"PY(\w+)PY", v)) {   // (or a number's: int *g_0524)
                        auto sm = py::re::search(
                            S(R"PY(\b((?:unsigned\s+)?(?:int|short|char|long|unsigned|u?int(?:8|16|32)_t))\s*\*\s*(?:\w+\s*,\s*\*?\s*)*)PY") +
                                esc(v) + S(R"PY(\b\s*[;,)=])PY"),
                            code);
                        pt = sm ? std::optional<str>(sm->group(1)) : std::nullopt;
                    }
                    if (pt && !pt->empty() && same_(*pt) == same_(base.replace(S(" *"), S("")))) {
                        int64_t bs = m.start(), be = m.end();
                        str seg = sl(code, bs, be);
                        int64_t k = m.group_opt(1) && !m.group(1).empty() ? seg.rfind(b) : seg.find(b);
                        edits.emplace_back(bs + k, bs + k + b.size(), S("(") + a + S(" + ") + fmt_int(cnt) + S(")"));
                    }
                }
                apply_desc(t, edits);
                if (!edits.empty()) {
                    if (!py::re::search(S(R"PY(\b)PY") + esc(a) + S(R"PY(\b)PY"), code))   // (the table named here: declared)
                        extra_decls.setdefault(n, set<str>())
                            .add(S("extern ") + base.replace(S(" *"), S("")) + (base.endswith(S("*")) ? S(" *") : S(" ")) + a +
                                 S("[") + fmt_int(cnt) + S("];"));
                    files.set(n, t);
                    notes[S("a pointer into a table compared with the variable the old linker put after it: the table's end")]
                        .append(py::percent(S("%s (%s: %s + %d)"), n, b, a, cnt));
                }
            }
        }
    }
}

// ================================================================ word_stacks
// int *sp; ... sp[1] = out; fp = (int *)sp[2]; - words that held pointers as well as numbers (a stack of
// frames, a list of results): the words a pointer's width - intptr_t - and every int * and int array they
// are one with (given to each other, compared, passed on to a parameter)
void Port::word_stacks() {
    if (_stacks_done) return;
    _stacks_done = true;
    using Key = std::tuple<str, std::optional<str>, str>;   // (file, function or None, name)
    const str INT = S(R"PY((?:register\s+)?(?:unsigned\s+|signed\s+)?(?:int|short|unsigned))PY");
    set<str> ptrs;                // (names declared pointers anywhere, at the top)
    dict<str, list<str>> arrays;  // (an int array at a file's top: where it is)
    for (const auto& [n, t] : files.items()) {
        str code = blank(t);
        list<str> cl = lines_of(code);
        list<int64_t> dd = depths(cl);
        for (int64_t i = 0; i < cl.size(); ++i) {
            const str& l = cl[i];
            if (dd[i]) continue;
            for (const auto& m : py::re::finditer(S(R"PY(\*\s*(\w+)\s*(?=[;,=\[)]))PY"), l))
                if (!sl(l, 0, m.start()).contains(S("(")) || l.contains(S("(*"))) ptrs.add(m.group(1));
            auto m = py::re::match(S(R"PY(\s*(?:extern\s+|static\s+)?)PY") + INT + S(R"PY(\s+(\w+)\s*\[[^\]]*\]\s*(?:=[^;]*)?;)PY"), l);
            if (m) arrays.setdefault(m->group(1), list<str>()).append(n);
        }
    }
    dict<Key, Key> parent;
    auto find = [&](Key x) {
        while (parent.setdefault(x, x) != x) {
            Key g = parent.at(parent.at(x));
            parent.set(x, g);
            x = parent.at(x);
        }
        return x;
    };
    auto unite = [&](const Key& a, const Key& b) {
        Key fa = find(a), fb = find(b);
        parent.set(fa, fb);
    };
    set<Key> seeds;
    dict<Key, bool> nodes;                                 // (node: (file, function or None, name))
    dict<std::tuple<str, int64_t>, Key> params;
    dict<str, std::tuple<str, list<str>>> defs;
    auto first_of = [](const Match& x) { return x.group_opt(1) ? *x.group_opt(1) : *x.group_opt(2); };
    const str inner = S(R"PY((?:^|,)\s*(?:\*\s*(\w+)|(\w+)\s*\[[^\]]*\])\s*(?=,|$))PY");
    for (const auto& n : py::sorted(files.keys())) {       // (the definitions first)
        if (!n.endswith(S(".c"))) continue;
        str t = files.at(n);
        list<str> lines = lines_of(t), code = lines_of(blank(t));
        for (const auto& d : definitions(lines, code)) {
            const FnP& f = std::get<0>(d);
            int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
            str head = unlines(code.slice(s_, j_));
            str body = unlines(code.slice(j_, k_ + 1));
            list<str> names;
            for (const auto& [pn, pt] : f->params) names.append(pn);
            dict<str, Key> mine;                           // (an int * of the function's: its node)
            for (const auto& dm : py::re::finditer(S(R"PY((?m)^\s*)PY") + INT + S(R"PY(\s+([^;(){}]*);)PY"), head + S("\n") + body))
                for (const auto& x : py::re::finditer(inner, dm.group(1))) {
                    str nm = first_of(x);
                    mine.set(nm, Key(n, f->name, nm));
                }
            defs.set(f->name, {n, names});
            for (int64_t k = 0; k < names.size(); ++k)
                if (mine.contains(names[k])) params.set({f->name, k}, mine.at(names[k]));
            list<str> bl;
            for (const auto& l : lines_of(body))
                if (l.strip().endswith(S(";")) &&
                    py::re::match(S(R"PY(\s*(?:register\s+)?(?:struct|union|char|int|unsigned|long|short)\b)PY"), l))
                    bl.append(l);
            set<str> local_ptrs;
            for (const auto& m : py::re::finditer(S(R"PY(\*\s*(\w+)\s*[;,=)\[])PY"), head + S("\n") + unlines(bl)))
                local_ptrs.add(m.group(1));
            auto node = [&](const str& v) -> std::optional<Key> {
                if (mine.contains(v)) return mine.at(v);
                if (arrays.contains(v)) return Key(S(""), std::nullopt, v);
                return std::nullopt;
            };
            set<str> cand = set<str>::from(mine.keys());
            for (const auto& a : arrays.keys())
                if (py::re::search(S(R"PY(\b)PY") + esc(a) + S(R"PY(\b)PY"), body)) cand.add(a);
            for (const auto& v : cand) nodes.set(*node(v), true);
            for (const auto& m : py::re::finditer(
                     S(R"PY((?:\b(\w+)\s*\[[^\]]*\]|\*\s*(\w+)\s*(?:\+\+|--)?)\s*=(?!=)\s*([^;]*);)PY"), body)) {
                auto v = node(first_of(m));
                str e = m.group(3).strip();
                if (v && (fullm(R"PY(&\s*\w+.*|".*)PY", e) ||
                          (fullm(R"PY(\w+)PY", e) && (local_ptrs.contains(e) || mine.contains(e) || ptrs.contains(e)))))
                    seeds.add(*v);   // (a pointer kept in a word of it)
            }
            for (const auto& m : py::re::finditer(S(R"PY(\(\s*(?:struct\s+\w+|\w+)\s*\*+\s*\)\s*(\w+)\s*\[)PY"), body))
                if (auto v = node(m.group(1))) seeds.add(*v);   // (a word of it read as a pointer)
            for (const auto& m : py::re::finditer(S(R"PY(\(\s*(?:struct\s+\w+|\w+)\s*\*\s*\*\s*\)\s*(\w+)\b(?!\s*\[))PY"), body))
                if (auto v = node(m.group(1))) seeds.add(*v);   // (it, taken as pointers)
            for (const auto& m : py::re::finditer(
                     S(R"PY(\b(\w+)\s*(?:=(?!=)|[<>]=?|==|!=)\s*(?:\(\s*\w+\s*\*\s*\)\s*)?(\w+)\b(?!\s*[\[(]))PY"), body)) {
                auto a = node(m.group(1)), b = node(m.group(2));
                if (a && b) unite(*a, *b);   // (one of them given the other, or compared)
            }
            str INT2 = INT.replace(S(R"PY((?:register\s+)?)PY"), S(""));
            for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*\(\s*)PY") + INT2 + S(R"PY(\s*\*\s*\)\s*\w+\s*\[)PY"), body))
                if (auto v = node(m.group(1))) seeds.add(*v);   // (fp = (int *)sp[2]: a pointer kept in a word)
        }
    }
    for (const auto& n : py::sorted(files.keys())) {       // (a call: its arguments one with the parameters)
        if (!n.endswith(S(".c"))) continue;
        str t = files.at(n);
        list<str> lines = lines_of(t), code = lines_of(blank(t));
        for (const auto& d : definitions(lines, code)) {
            const FnP& f = std::get<0>(d);
            int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
            str body = unlines(code.slice(j_, k_ + 1));
            dict<str, Key> mine;
            for (const auto& dm : py::re::finditer(S(R"PY((?m)^\s*)PY") + INT + S(R"PY(\s+([^;(){}]*);)PY"), unlines(code.slice(s_, k_ + 1))))
                for (const auto& x : py::re::finditer(inner, dm.group(1))) {
                    str nm = first_of(x);
                    mine.set(nm, Key(n, f->name, nm));
                }
            for (const auto& [g, gv] : defs.items()) {
                for (const auto& c : py::re::finditer(S(R"PY((?<![\w.>]))PY") + esc(g) + S(R"PY(\s*\()PY"), body)) {
                    auto sp_ = call_args(sl(body, c.start(), c.start() + 400), g, false);
                    list<Span> sps = sp_ ? *sp_ : list<Span>();
                    for (int64_t k = 0; k < sps.size(); ++k) {
                        auto [a, b] = sps[k];
                        str arg = sl(body, c.start() + a, c.start() + b).strip();
                        if (params.contains({g, k}) && fullm(R"PY(\w+)PY", arg)) {
                            std::optional<Key> v;
                            if (mine.contains(arg)) v = mine.at(arg);
                            else if (arrays.contains(arg)) v = Key(S(""), std::nullopt, arg);
                            if (v) unite(*v, params.at({g, k}));
                        }
                    }
                }
            }
        }
    }
    set<Key> wide;
    for (const auto& x : seeds) wide.add(find(x));
    set<Key> chosen;
    {
        std::vector<Key> universe = parent.keys();
        for (const auto& k : nodes.keys()) universe.push_back(k);
        for (const auto& k : seeds) universe.push_back(k);
        for (const auto& x : universe)
            if (wide.contains(find(x))) chosen.add(x);
    }
    if (chosen.empty()) return;
    std::vector<Key> order(chosen.begin(), chosen.end());
    std::sort(order.begin(), order.end(), [](const Key& a, const Key& b) {
        return std::make_tuple(std::get<0>(a), std::get<1>(a).value_or(S("")), std::get<2>(a)) <
               std::make_tuple(std::get<0>(b), std::get<1>(b).value_or(S("")), std::get<2>(b));
    });
    const str why = S("words that held pointers too (a stack, a list of results): intptr_t");
    for (const auto& [n, fname, v] : order) {
        if (!fname) {                                      // (an array: its definition and every extern of it)
            if (arrays.contains(v))
                for (const auto& f : arrays.at(v)) {
                    str t = files.at(f);
                    t = py::re::sub(S(R"PY((?m)^(\s*(?:extern\s+|static\s+)?))PY") + INT + S(R"PY((\s+)PY") + esc(v) + S(R"PY(\s*\[))PY"),
                                    S(R"PY(\1intptr_t\2)PY"), t);
                    files.set(f, t);
                }
            notes[why].append(v);
            continue;
        }
        str t = files.at(n);
        list<str> lines = lines_of(t), code = lines_of(blank(t));
        for (const auto& d : definitions(lines, code)) {
            const FnP& f = std::get<0>(d);
            int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
            if (f->name != *fname) continue;
            for (int64_t x = s_; x < k_ + 1; ++x) {
                auto m = py::re::match(S(R"PY((\s*(?:register\s+)?)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s+)(.*;.*)$)PY"), lines[x]);
                str me_ = S(R"PY(\*\s*)PY") + esc(v) + S(R"PY(|)PY") + esc(v) + S(R"PY(\s*\[[^\]]*\])PY");
                if (!m) continue;
                str g3 = m->group(3);
                list<str> parts = split_top(py::split(g3, S(";"))[0], S(","));
                bool anyd = false;
                for (const auto& d_ : parts) anyd = anyd || py::re::fullmatch(me_, d_.strip());
                if (!anyd) continue;
                list<str> decls;
                for (const auto& d_ : parts) decls.append(d_.strip());
                str rest = g3.slice_from(py::split(g3, S(";"))[0].size());
                list<str> keep;
                for (const auto& d_ : decls) if (!py::re::fullmatch(me_, d_)) keep.append(d_);
                str mine_d;
                for (const auto& d_ : decls) if (py::re::fullmatch(me_, d_)) { mine_d = d_; break; }
                str base = sl(lines[x], m->group(1).size(), lines[x].find(m->group(2), m->group(1).size()));
                str nw = m->group(1) + S("intptr_t ") + mine_d + S(";");
                if (keep.size()) nw = m->group(1) + base + S(" ") + py::join(S(", "), keep) + S("; intptr_t ") + mine_d + S(";");
                lines[x] = nw + rest.slice_from(1);
                break;
            }
            // (fp = (int *)sp[2]: an intptr_t * taken out)
            str body = unlines(lines.slice(j_, k_ + 1));
            body = py::re::sub(S(R"PY((\b)PY") + esc(v) + S(R"PY(\s*=\s*\(\s*)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s*\*\s*\)))PY"),
                               S(R"PY(\1intptr_t\2)PY"), body);
            lines.set_slice(j_, k_ + 1, lines_of(body));
        }
        files.set(n, unlines(lines));
        notes[why].append(py::percent(S("%s (%s: %s)"), n, *fname, v));
    }
}

// ================================================================ merge_tables
// a table the program reads past its end at a constant place (t[7] of a t[3]): it went on into the
// variables defined after it in its file - one table again, those names its elements
void Port::merge_tables() {
    // (and where a run read one past its end: sanitize)
    if (_merged == static_cast<int64_t>(reach.size()) + 1) return;
    _merged = static_cast<int64_t>(reach.size()) + 1;
    dict<str, int64_t> uses;   // (defaultdict(int))
    for (const auto& [n, t] : files.items())
        for (const auto& m : py::re::finditer(S(R"PY(\b([A-Za-z_]\w*)\s*\[\s*(\d+)\s*\])PY"), blank(t)))
            uses[m.group(1)] = std::max(uses.get(m.group(1), 0), py::int_parse(m.group(2)));
    for (const auto& [k_, v_] : reach.items()) uses[k_] = std::max(uses.get(k_, 0), v_);
    py::re::Pattern defn = py::re::compile(
        S(R"PY(^((?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned|intptr_t))\s+(\w+)\s*(\[\s*\d*\s*\])?\s*(?:=\s*(\{[^;{}]*\}|[^;{}]*?)\s*)?;(.*)$)PY"));
    // groups: 1 base, 2 name, 3 arr, 4 val, 5 rest
    auto elems = [&](const Match& m_) -> std::optional<list<str>> {   // (an array without initialiser: its size in zeros)
        if (m_.group_opt(4)) return std::nullopt;
        auto sz = py::re::search(S(R"PY(\d+)PY"), m_.group_opt(3) ? *m_.group_opt(3) : S(""));
        bool arr = m_.group_opt(3) && !m_.group_opt(3)->empty();
        if (sz || !arr) {
            list<str> o;
            int64_t cnt = sz ? py::int_parse(sz->group()) : 1;
            for (int64_t i = 0; i < cnt; ++i) o.append(S("0"));
            return o;
        }
        return std::nullopt;
    };
    auto split_vals = [&](const str& val) {
        list<str> out;
        for (const auto& v : py::split(val.strip(S("{} ")), S(","))) if (!v.strip().empty()) out.append(v.strip());
        return out;
    };
    for (const auto& n : py::sorted(files.keys())) {
        if (!n.endswith(S(".c"))) continue;
        list<str> lines = lines_of(files.at(n));
        int64_t i = 0;
        while (i < lines.size()) {
            auto m = defn.match(lines[i]);
            std::optional<list<str>> vals;
            auto arr_of = [](const Match& x) { return x.group_opt(3) && !x.group_opt(3)->empty(); };
            if (m && arr_of(*m) && !m->group_opt(4)) {
                vals = elems(*m);
            } else if (m && arr_of(*m)) {
                str val = *m->group_opt(4);
                vals = split_vals(val);
                auto sz_ = py::re::search(S(R"PY(\d+)PY"), *m->group_opt(3));   // (int t[2691] = { 0 }: its 2691, the rest 0)
                if (sz_ && vals->size() < py::int_parse(sz_->group()) && !val.lstrip().startswith(S("\"")))
                    for (int64_t q = vals->size(); q < py::int_parse(sz_->group()); ++q) vals->append(S("0"));
                if (val.lstrip().startswith(S("\""))) {    // (a string: its bytes, and the 0 after)
                    vals = std::nullopt;
                    str lit = val.strip();
                    auto size = py::re::search(S(R"PY(\d+)PY"), *m->group_opt(3));
                    int64_t sz = size ? py::int_parse(size->group()) : static_cast<int64_t>(c_string_bytes(lit).raw().size()) + 1;
                    if (uses.get(m->group(2), -1) >= sz)
                        notes[S("a string read past its end at a constant place (not joined: check)")]
                            .append(py::percent(S("%s (%s)"), n, m->group(2)));
                }
            }
            if (!m || !vals || vals->empty() || uses.get(m->group(2), -1) < vals->size()) {
                i += 1;
                continue;
            }
            str base = m->group(1), name = m->group(2);
            using Part = std::tuple<str, int64_t, int64_t, bool>;
            std::vector<Part> parts{{name, 0, vals->size(), true}};
            int64_t j = i + 1;
            while (j < lines.size()) {   // (what follows, of the same type: the table's rest)
                auto m2 = defn.match(lines[j]);
                if (!m2 || m2->group(1) != base) break;
                list<str> v2;
                if (!m2->group_opt(4)) {
                    auto e = elems(*m2);
                    if (!e) break;
                    v2 = *e;
                } else if (arr_of(*m2) && m2->group(4).lstrip().startswith(S("\""))) {
                    str lit = m2->group(4).strip();    // (a string among them: its bytes as elements)
                    std::string bs = c_string_bytes(lit).raw();
                    auto size = py::re::search(S(R"PY(\d+)PY"), *m2->group_opt(3));
                    int64_t want = size ? py::int_parse(size->group()) : static_cast<int64_t>(bs.size()) + 1;
                    for (unsigned char c : bs) v2.append(fmt_int(c));
                    for (int64_t q = static_cast<int64_t>(bs.size()); q < want; ++q) v2.append(S("0"));
                } else {
                    if (arr_of(*m2)) v2 = split_vals(m2->group(4));
                    else v2 = list<str>{m2->group(4).strip()};
                    auto sz_ = py::re::search(S(R"PY(\d+)PY"), m2->group_opt(3) ? *m2->group_opt(3) : S(""));
                    if (sz_ && v2.size() < py::int_parse(sz_->group()))
                        for (int64_t q = v2.size(); q < py::int_parse(sz_->group()); ++q) v2.append(S("0"));
                }
                parts.emplace_back(m2->group(2), vals->size(), v2.size(), arr_of(*m2));
                vals->extend(v2);
                j += 1;
            }
            if (parts.size() == 1) {
                i += 1;
                continue;
            }
            str body = py::join(S(", "), *vals);
            list<str> pn;
            for (const auto& p : parts) pn.append(std::get<0>(p));
            bool nonzero = false;
            for (const auto& v : *vals) nonzero = nonzero || v != S("0");
            list<str> repl;
            if (nonzero)
                repl.append(py::percent(S("%s %s[%d] = { %s };\t/* (one table: %s) */"), base, name, vals->size(), body,
                                        py::join(S(", "), pn)));
            else   // (none given a value: one block of them)
                repl.append(py::percent(S("%s %s[%d];\t/* (one table: %s) */"), base, name, vals->size(), py::join(S(", "), pn)));
            lines.set_slice(i, j, repl);
            list<str> others;
            for (size_t q = 1; q < parts.size(); ++q) {
                const auto& p = parts[q];
                aliases.set(std::get<0>(p),
                            {std::get<3>(p) ? py::percent(S("(&%s[%d])"), name, std::get<1>(p))
                                            : py::percent(S("%s[%d]"), name, std::get<1>(p)),
                             py::percent(S("%s %s[%d];"), base, name, vals->size())});
                others.append(std::get<0>(p));
            }
            table_notes.append(py::percent(S("%s (%s: %s)"), n, name, py::join(S(", "), others)));
            i += 1;
        }
        files.set(n, unlines(lines));
    }
}

// ================================================================ numbered_tables
// f06988(f021ff, 0x3a20), f06988(fn, tab) struct hsym **tab: a table's place in the old data segment given
// as its number - the table there by name (g_3a20); and an int table given where a table of pointers is
// wanted (struct hsym **, or cast to one) holding them - intptr_t, a pointer's width
void Port::numbered_tables() {
    if (_numbered_done) return;
    _numbered_done = true;
    dict<str, std::tuple<str, str>> decls;   // (g_3a20: "int g_3a20[64]", as declared somewhere)
    for (const auto& t : originals.values())
        for (const auto& m : py::re::finditer(
                 S(R"PY((?m)^\s*(?:extern\s+|static\s+)?((?:unsigned\s+)?(?:int|short|unsigned|char))\s+(g_[0-9a-f]{4})\s*\[\s*(\d*)\s*\]\s*[;=,])PY"),
                 blank(t)))
            if (!m.group(3).empty() || !decls.contains(m.group(2))) decls.set(m.group(2), {m.group(1), m.group(3)});
    if (decls.empty()) return;
    dict<str, FnP> defs_;   // (the definitions' parameters: fns is not made yet)
    for (const auto& [n, t] : files.items())
        if (n.endswith(S(".c")))
            for (const auto& d : definitions(lines_of(t), lines_of(blank(t)))) defs_.setdefault(std::get<0>(d)->name, std::get<0>(d));
    dict<str, list<bool>> ptrp;
    for (const auto& [nm_, f] : defs_.items()) {
        list<bool> l;
        for (const auto& [pn, pt] : f->params) l.append(static_cast<bool>(py::re::search(S(R"PY(\*)PY"), pt)));
        ptrp.set(nm_, l);
    }
    dict<str, str> bodies;
    for (const auto& [n, t] : files.items())
        if (n.endswith(S(".c"))) {
            list<str> cl_ = lines_of(blank(t));
            for (const auto& d : definitions(lines_of(t), cl_))
                bodies.setdefault(std::get<0>(d)->name, unlines(cl_.slice(std::get<2>(d), std::get<3>(d) + 1)));
        }
    for (int rep = 0; rep < 3; ++rep) {   // (f067e8(num, tab) { return f067b5(num, tab); } - a parameter given on to a
        for (const auto& [nm_, f] : defs_.items()) {   //  pointer one: a pointer too)
            str bd = bodies.get(nm_, S(""));
            for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*\()PY"), bd)) {
                if (!ptrp.contains(m.group(1)) || m.group(1) == nm_) continue;
                auto sp0 = call_args(sl(bd, m.start(), m.start() + 400), m.group(1), false);
                list<Span> sp = sp0 ? *sp0 : list<Span>();
                for (int64_t k = 0; k < sp.size(); ++k) {
                    auto [a_, b_] = sp[k];
                    str a = sl(bd, m.start() + a_, m.start() + b_).strip();
                    if (k < ptrp.at(m.group(1)).size() && ptrp.at(m.group(1))[k])
                        for (int64_t i_ = 0; i_ < f->params.size(); ++i_)
                            if (std::get<0>(f->params[i_]) == a) ptrp.at(nm_)[i_] = true;
                }
            }
        }
    }
    set<str> wide;
    for (const auto& [n, t0] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        str t = t0;
        str code = blank(t);
        std::vector<Edit3> edits;
        set<str> used;
        str pat;
        if (!ptrp.empty()) {
            list<str> ks;
            for (const auto& k : py::sorted(ptrp.keys())) ks.append(esc(k));
            pat = S(R"PY(\b()PY") + py::join(S("|"), ks) + S(R"PY()\s*\()PY");
        } else {
            pat = S("(?!)");
        }
        for (const auto& m : py::re::finditer(pat, code)) {
            auto sp0 = call_args(sl(code, m.start(), m.start() + 600), m.group(1), false);
            list<Span> sp = sp0 ? *sp0 : list<Span>();
            for (int64_t k = 0; k < sp.size(); ++k) {
                auto [a_, b_] = sp[k];
                if (k >= ptrp.at(m.group(1)).size() || !ptrp.at(m.group(1))[k]) continue;
                str arg = sl(code, m.start() + a_, m.start() + b_);
                for (const auto& h : py::re::finditer(S(R"PY(\b0x([0-9a-fA-F]{4})\b)PY"), arg)) {
                    str nm = S("g_") + h.group(1).lower();
                    if (decls.contains(nm) &&
                        fullm(R"PY(\s*(?:[^?:]*\?\s*)?0x[0-9a-fA-F]{4}\s*(?::\s*0x[0-9a-fA-F]{4}\s*)?)PY", arg)) {
                        int64_t at = m.start() + a_ + h.start();
                        edits.emplace_back(at, at + h.group().size(), nm);
                        used.add(nm);
                    }
                }
            }
        }
        std::sort(edits.begin(), edits.end());
        edits.erase(std::unique(edits.begin(), edits.end()), edits.end());   // (set(edits))
        apply_desc(t, edits);
        if (!edits.empty()) {
            files.set(n, t);
            for (const auto& nm : used)
                if (!py::re::search(S(R"PY(\b)PY") + nm + S(R"PY(\b\s*\[[^\]]*\]\s*[;,=])PY"), blank(t)))
                    extra_decls.setdefault(n, set<str>())
                        .add(S("extern ") + std::get<0>(decls.at(nm)) + S(" ") + nm + S("[") + std::get<1>(decls.at(nm)) + S("];"));
            notes[S("a table's place given as its number (the old data segment's): the table, by name")]
                .append(py::percent(S("%s (%s)"), n, py::join(S(", "), py::sorted(used))));
        }
    }
    for (const auto& [n, t] : files.items()) {   // (an int table where a table of pointers is wanted)
        str code = blank(t);
        for (const auto& nm : decls.keys()) {
            if (!py::re::search(S(R"PY(\b)PY") + nm + S(R"PY(\b)PY"), code)) continue;
            if (py::re::search(S(R"PY(\(\s*(?:(?:struct|union)\s+\w+|\w+)\s*\*\s*\*\s*\)\s*\(?\s*(?:[^()?:]*\?\s*)?(?:\w+\s*:\s*)?)PY") +
                                   nm + S(R"PY(\b)PY"),
                               code))
                wide.add(nm);
            for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*\()PY"), code)) {
                if (!defs_.contains(m.group(1))) continue;
                const FnP& f = defs_.at(m.group(1));
                auto sp0 = call_args(sl(code, m.start(), m.start() + 600), m.group(1), false);
                list<Span> sp = sp0 ? *sp0 : list<Span>();
                for (int64_t k = 0; k < sp.size(); ++k) {
                    auto [a_, b_] = sp[k];
                    if (k < f->params.size() && py::re::search(S(R"PY(\*\s*\*)PY"), std::get<1>(f->params[k])) &&
                        py::re::search(S(R"PY((?:^|[?:]\s*))PY") + nm + S(R"PY(\s*$|^\s*)PY") + nm + S(R"PY(\b\s*(?::|$))PY"),
                                       sl(code, m.start() + a_, m.start() + b_).strip()))
                        wide.add(nm);
                }
            }
        }
    }
    for (const auto& nm : py::sorted(wide)) {
        const str& b0 = std::get<0>(decls.at(nm));
        if (b0 == S("int") || b0 == S("short") || b0 == S("unsigned") || b0 == S("unsigned int")) {
            decl_rules.append(DeclRule(S(R"PY(\b(?:unsigned\s+int|unsigned|short|u?int16_t|int)(\s+)PY") + nm + S(R"PY(\s*\[))PY"),
                                       S(R"PY(intptr_t\1)PY")));
            dict<str, set<str>> rebuilt;
            for (const auto& [f_, ds_] : extra_decls.items()) {
                set<str> s;
                for (const auto& d : ds_) s.add(d.replace(b0 + S(" ") + nm + S("["), S("intptr_t ") + nm + S("[")));
                rebuilt.set(f_, s);
            }
            extra_decls = rebuilt;
            notes[S("an int table holding pointers (given where a table of them is wanted): intptr_t")].append(nm);
        }
    }
}

// ================================================================ size_tables
// struct recfmt g_2096[] = { { "tknNdplbcga", 22 }, ..., { "tukNevrsma", 32 } }; n = g_2096[kind].n;
// p = f03f02(n, 0, pool); f03fc9(p, s, n): a table of the old byte counts of a structure's first members, by
// kind (32 the whole struct psym, 22 up to its member at 16h) - each the same part of today's structure
// (offsetof the member that began there, sizeof the whole)
void Port::size_tables() {
    if (_size_tables_done) return;
    _size_tables_done = true;
    const asm16::Layouts& old = old_layouts();
    const auto& allocs = allocators();
    using TF = std::tuple<str, str>;
    dict<TF, set<str>> uses;   // ((table, field): the structures it gives sizes of)
    for (const auto& [n, t] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        str code = blank(t);
        dict<str, str> ptr_of = ptr_of_map(code);
        dict<str, TF> tv;   // (n = g_2096[s->kind].n;)
        for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*(\w+)\s*\[[^\]]*\]\s*\.\s*(\w+)\s*;)PY"), code))
            tv.set(m.group(1), TF(m.group(2), m.group(3)));
        for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?(\w+)\s*\()PY"), code)) {
            auto st = ptr_of.get(m.group(1));
            if (!st || !old.contains(*st) || !allocs.contains(m.group(2)) || allocs.at(m.group(2)).size() != 1) continue;
            auto sp0 = call_args(sl(code, m.start(2), m.start(2) + 400), m.group(2), false);
            list<Span> sp = sp0 ? *sp0 : list<Span>();
            int64_t k = *allocs.at(m.group(2)).begin();
            if (k >= sp.size()) continue;
            str a = sl(code, m.start(2) + std::get<0>(sp[k]), m.start(2) + std::get<1>(sp[k])).strip();
            auto d = py::re::fullmatch(S(R"PY((\w+)\s*\[[^\]]*\]\s*\.\s*(\w+))PY"), a);
            if (d) uses.setdefault(TF(d->group(1), d->group(2)), set<str>()).add(*st);
            else if (fullm(R"PY(\w+)PY", a) && tv.contains(a)) uses.setdefault(tv.at(a), set<str>()).add(*st);
        }
    }
    auto keys = uses.keys();
    std::sort(keys.begin(), keys.end());
    for (const auto& key : keys) {
        const str& tab = std::get<0>(key);
        const str& field = std::get<1>(key);
        const set<str>& sts = uses.at(key);
        if (sts.size() != 1) continue;
        str st_ = *sts.begin();
        const asm16::Layout& lay = old.at(st_);
        dict<int64_t, str> offs;
        for (const auto& [k, v] : lay.members)
            if (!k.contains(S("."))) offs.set(v.off, k);
        for (const auto& [n, t0] : files.items()) {
            str t = t0;
            str code = blank(t);
            auto dm = py::re::search(S(R"PY((?m)^(?:static\s+)?(struct\s+\w+)\s+)PY") + esc(tab) + S(R"PY(\s*\[[^\]]*\]\s*=\s*\{)PY"), code);
            if (!dm) continue;
            const asm16::Layout* tl = old.get(ws1(dm->group(1)));
            if (!tl) continue;
            int64_t k = 0;
            if (tl->has(field)) {
                for (int64_t q = 0; q < static_cast<int64_t>(tl->members.size()); ++q)
                    if (tl->members[q].first == field) { k = q; break; }
            } else {   // (struct okind g_1010[] defined, extern struct recfmt g_1010[] used: the place by order)
                std::optional<str> ux;
                for (const auto& t2 : files.values()) {
                    auto x_ = py::re::search(S(R"PY(\bextern\s+((?:struct|union)\s+\w+)\s+)PY") + esc(tab) + S(R"PY(\s*\[)PY"), blank(t2));
                    if (x_) { ux = ws1(x_->group(1)); break; }
                }
                const asm16::Layout* ul = ux ? old.get(*ux) : nullptr;
                if (!ul || !ul->has(field)) continue;
                for (int64_t q = 0; q < static_cast<int64_t>(ul->members.size()); ++q)
                    if (ul->members[q].first == field) { k = q; break; }
                if (k >= static_cast<int64_t>(tl->members.size())) continue;
            }
            int64_t end = code.find(S("};"), dm->end());
            std::vector<Edit3> edits;
            for (const auto& e : py::re::finditer(S(R"PY(\{([^{}]*)\})PY"), sl(code, dm->end(), end))) {
                list<str> parts = split_top(e.group(1), S(","));
                if (k >= parts.size()) continue;
                str v = parts[k].strip();
                if (!fullm(R"PY(0[xX][0-9a-fA-F]+|\d+)PY", v) || int0(v) == 0) continue;
                int64_t nv = int0(v);
                std::optional<str> nw;
                if (nv == lay.size) nw = S("sizeof(") + st_ + S(")");
                else if (offs.contains(nv)) nw = S("offsetof(") + st_ + S(", ") + offs.at(nv) + S(")");
                if (!nw) continue;
                int64_t at = dm->end() + e.start(1);
                for (int64_t q = 0; q < k; ++q) at += parts[q].size() + 1;
                int64_t lead = parts[k].size() - parts[k].lstrip().size();
                edits.emplace_back(at + lead, at + lead + v.size(), *nw);
            }
            apply_desc(t, edits);
            if (!edits.empty()) {
                if (!t.contains(S("<stddef.h>"))) t = S("#include <stddef.h>\n") + t;
                files.set(n, t);
                notes_size_tables.append(py::percent(S("%s (%s[].%s: %s)"), n, tab, field, st_));
            }
        }
    }
}

// ================================================================ pool_blocks
// a pooled allocator - if (size > p->left) { ... malloc(p->size + 4) ... p->left = p->size; } - and its
// table of pools, { (char **)g_2ea0, 8, g_2ea0 + 4, (char **)g_2ea0, 8, 1 }: each pool's blocks of a size for
// the old pieces (their pointers two bytes); today's pieces up to four times that - the blocks so many times
// the size, and none left in the first (static, of the old size: it only heads the pool's chain)
void Port::pool_blocks() {
    if (_pools_done) return;
    _pools_done = true;
    const asm16::Layouts& old = old_layouts();
    int64_t ratio = 8 / std::max<int64_t>(1, profile.widths_pointer.value_or(16) / 8);
    dict<str, std::tuple<str, str>> found;   // (struct pool: (its block size member, what is left))
    for (const auto& [n, t] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        list<str> cl_ = lines_of(blank(t));
        for (const auto& d : definitions(lines_of(t), cl_)) {
            int64_t s = std::get<1>(d), j = std::get<2>(d), k = std::get<3>(d);
            str body = unlines(cl_.slice(j, k + 1));
            for (const auto& m : py::re::finditer(S(R"PY(\bmalloc\s*\(\s*(\w+)\s*->\s*(\w+))PY"), body)) {
                str p_ = m.group(1), a_ = m.group(2);
                auto b_ = py::re::search(S(R"PY(\b)PY") + p_ + S(R"PY(\s*->\s*(\w+)\s*=\s*)PY") + p_ + S(R"PY(\s*->\s*)PY") + a_ + S(R"PY(\b)PY"), body);
                auto st_ = py::re::search(S(R"PY(\b(struct\s+\w+)\s*\*\s*)PY") + p_ + S(R"PY(\s*[;,)])PY"), unlines(cl_.slice(s, k + 1)));
                if (b_ && st_ && py::re::search(S(R"PY(>\s*)PY") + p_ + S(R"PY(\s*->\s*)PY") + b_->group(1) + S(R"PY(\b)PY"), body))
                    found.set(ws1(st_->group(1)), {a_, b_->group(1)});
            }
        }
    }
    for (const auto& [st_, ab] : found.items()) {
        const str& a_ = std::get<0>(ab);
        const str& b_ = std::get<1>(ab);
        const asm16::Layout* lay = old.get(st_);
        if (!lay || !lay->has(a_) || !lay->has(b_)) continue;
        int64_t ia = 0, ib = 0;
        for (int64_t q = 0; q < static_cast<int64_t>(lay->members.size()); ++q) {
            if (lay->members[q].first == a_) ia = q;
            if (lay->members[q].first == b_) ib = q;
        }
        list<str> words;
        for (const auto& w : py::split(st_, S(" "))) words.append(esc(w));
        for (const auto& [n, t0] : files.items()) {
            str t = t0;
            str code = blank(t);
            auto dm = py::re::search(S(R"PY((?m)^(?:static\s+)?)PY") + py::join(S(R"PY(\s+)PY"), words) + S(R"PY(\s+\w+\s*\[[^\]]*\]\s*=\s*\{)PY"), code);
            if (!dm) continue;
            int64_t end = code.find(S("};"), dm->end());
            std::vector<Edit3> edits;
            for (const auto& e : py::re::finditer(S(R"PY(\{([^{}]*)\})PY"), sl(code, dm->end(), end))) {
                list<str> parts = split_top(e.group(1), S(","));
                str nv = ia < parts.size() ? parts[ia].strip() : S("");
                if (fullm(R"PY(0[xX][0-9a-fA-F]+|\d+)PY", nv)) {   // (its first block, a BSS array of the old size: as big as the
                    set<str> gs;                                   //  blocks now, a head's room more)
                    for (const auto& g_ : py::re::finditer(S(R"PY(\b(g_[0-9a-f]{4})\b)PY"), e.group(1))) gs.add(g_.group(1));
                    for (const auto& g_ : gs)
                        if (bss.contains(g_)) bss.set(g_, std::max(bss.at(g_), int0(nv) * ratio + 16));
                }
                for (int64_t i_ : {ia, ib}) {
                    if (i_ >= parts.size()) continue;
                    str v = parts[i_].strip();
                    if (!fullm(R"PY(0[xX][0-9a-fA-F]+|\d+)PY", v)) continue;
                    int64_t at = dm->end() + e.start(1);
                    for (int64_t q = 0; q < i_; ++q) at += parts[q].size() + 1;
                    int64_t lead = parts[i_].size() - parts[i_].lstrip().size();
                    edits.emplace_back(at + lead, at + lead + v.size(), fmt_int(int0(v) * ratio));
                }
            }
            apply_desc(t, edits);
            if (!edits.empty()) {
                files.set(n, t);
                notes_pools.append(py::percent(S("%s (%s: %s and %s x%d)"), n, st_, a_, b_, ratio));
            }
        }
    }
}

// ================================================================ local_copies
// struct ins { int i0, i2; int kind; ... }; in a file, struct ins { struct ins *next, *prev; int kind; ... }
// in a header: the file's own copy of the program's structure, its words where the other's pointers are (the
// old layout the same) - the header's structure, the file's names of its members the header's
void Port::local_copies() {
    if (_local_copies_done) return;
    _local_copies_done = true;
    dict<str, str> heads;
    for (const auto& [h, t] : files.items())
        if (h.endswith(S(".h")))
            for (const auto& m : py::re::finditer(S(R"PY(\bstruct\s+(\w+)\s*\{)PY"), blank(t))) heads.setdefault(m.group(1), h);
    if (heads.empty()) return;
    list<str> htexts;
    for (const auto& h : py::sorted(files.keys()))
        if (h.endswith(S(".h"))) htexts.append(files.at(h));
    asm16::Layouts hlay = asm16::CTypes::_layouts(htexts, 2, 4);
    for (const auto& n : py::sorted(files.keys())) {
        if (!n.endswith(S(".c"))) continue;
        str t = files.at(n);
        str code = blank(t);
        list<int64_t> ds_ = depths(lines_of(code));
        set<str> done_tags;
        while (true) {   // (one at a time: the text moves under each)
            std::optional<Match> m;
            for (const auto& x : py::re::finditer(S(R"PY((?m)^struct\s+(\w+)\s*\{[^{}]*\}\s*;[ \t]*\n?)PY"), code))
                if (!done_tags.contains(x.group(1))) { m = x; break; }
            if (!m) break;
            str tag = m->group(1);
            done_tags.add(tag);
            if (!heads.contains(tag) || ds_[nl_before(code, m->start())]) continue;
            asm16::Layouts mine_l = asm16::CTypes::_layouts(list<str>{m->group()}, 2, 4);
            const asm16::Layout* mine = mine_l.get(S("struct ") + tag);
            const asm16::Layout* theirs = hlay.get(S("struct ") + tag);
            if (!mine || !theirs || mine->size != theirs->size) continue;
            dict<std::tuple<int64_t, int64_t>, str> at_t;
            for (const auto& [k, v] : theirs->members) at_t.set({v.off, v.size}, k);
            dict<str, str> ren;
            dict<str, std::tuple<str, str>> puns;
            bool ok = true;
            for (const auto& [k, v] : mine->members) {
                if (!at_t.contains({v.off, v.size})) {   // (int size; over char size, i9: the word there)
                    std::vector<std::tuple<int64_t, int64_t, str>> cover;
                    for (const auto& [kk, w] : theirs->members)
                        if (v.off <= w.off && w.off < v.off + v.size) cover.emplace_back(w.off, w.size, kk);
                    std::sort(cover.begin(), cover.end());
                    std::optional<str> ct_;
                    if (v.size == 2) ct_ = S("int16_t");
                    else if (v.size == 4) ct_ = S("int32_t");
                    if (ct_ && cover.size() > 1 && std::get<0>(cover.front()) == v.off &&
                        std::get<0>(cover.back()) + std::get<1>(cover.back()) == v.off + v.size && !v.isptr) {
                        puns.set(k, {std::get<2>(cover.front()), *ct_});
                        continue;
                    }
                    ok = false;
                    break;
                }
                const str& o = at_t.at({v.off, v.size});
                if (o != k) ren.set(k, o);
            }
            if (!ok) continue;
            str nw = sl(t, 0, m->start()) + t.slice_from(m->end());
            for (const auto& [a, b] : ren.items())   // (its uses: ->i0 the header's ->next)
                nw = sub_code(nw, py::re::compile(S(R"PY(((?:->|\.)\s*))PY") + esc(a) + S(R"PY(\b)PY")),
                              [&](const Match& x) { return x.group(1) + b; });
            set<str> mine_vars;
            for (const auto& x : py::re::finditer(S(R"PY(\bstruct\s+)PY") + esc(tag) + S(R"PY(\s*\*\s*(\w+))PY"), blank(nw)))
                mine_vars.add(x.group(1));
            for (const auto& t2 : files.values())   // (and the program's variables of it, the header's)
                for (const auto& vm : py::re::finditer(S(R"PY((?m)^\s*extern\s+struct\s+)PY") + esc(tag) + S(R"PY(\s*\*\s*(\w+))PY"), blank(t2)))
                    mine_vars.add(vm.group(1));
            for (const auto& [a, bc] : puns.items()) {   // (p->size on a struct ins * alone: other structures' ->size are their own)
                if (mine_vars.empty()) break;
                list<str> alts;
                for (const auto& v : py::sorted(mine_vars)) alts.append(esc(v));
                const str& b = std::get<0>(bc);
                const str& ct_ = std::get<1>(bc);
                nw = sub_code(nw, py::re::compile(S(R"PY((?<![\w.>])()PY") + py::join(S("|"), alts) + S(R"PY()\s*->\s*)PY") + esc(a) + S(R"PY(\b)PY")),
                              [&](const Match& x) { return S("(*(") + ct_ + S(" *)&") + x.group(1) + S("->") + b + S(")"); });
            }
            if (!py::re::search(S(R"PY(#\s*include\s*")PY") + esc(heads.at(tag)) + S(R"PY(")PY"), nw)) {
                list<str> lines = lines_of(nw);
                int64_t at = -1;
                for (int64_t i = 0; i < lines.size(); ++i)
                    if (lines[i].startswith(S("#include"))) at = std::max(at, i);
                lines.insert(at + 1, S("#include \"") + heads.at(tag) + S("\""));
                nw = unlines(lines);
            }
            t = nw;
            files.set(n, t);
            code = blank(t);
            ds_ = depths(lines_of(code));
            str tail;
            if (!ren.empty()) {
                list<str> kv;
                for (const auto& [k, v] : ren.items()) kv.append(k + S(" as ") + v);
                tail = S(": ") + py::join(S(", "), kv);
            }
            notes_local_copies.append(py::percent(S("%s (struct %s%s)"), n, tag, tail));
        }
    }
}

// ================================================================ byte_places
// f00002(op, p) char *p; { p[0xa] = 2; *(int *)(p + 6) = 0; } given a struct opnd * by every caller:
// the bytes by their old places - the members there (p[0xa] its o10)
void Port::byte_places() {
    if (_byte_places_done) return;
    _byte_places_done = true;
    const asm16::Layouts& old = old_layouts();
    dict<str, FnP> defs_;
    dict<str, std::tuple<str, int64_t, int64_t>> where;
    for (const auto& [n, t] : files.items())
        if (n.endswith(S(".c")))
            for (const auto& d : definitions(lines_of(t), lines_of(blank(t)))) {
                defs_.setdefault(std::get<0>(d)->name, std::get<0>(d));
                where.setdefault(std::get<0>(d)->name, {n, std::get<1>(d), std::get<3>(d)});
            }
    dict<str, list<int64_t>> cands;
    for (const auto& [nm, f] : defs_.items()) {
        list<int64_t> ks;
        for (int64_t i = 0; i < f->params.size(); ++i)
            if (py::re::fullmatch(S(R"PY((?:register\s+)?(?:unsigned\s+)?char\s*\*\s*\w+)PY"), std::get<1>(f->params[i]).strip())) ks.append(i);
        if (ks.size()) cands.set(nm, ks);
    }
    if (cands.empty()) return;
    using FK = std::tuple<str, int64_t>;
    dict<FK, set<std::optional<str>>> given;   // ((function, place): the structures given there)
    for (const auto& [n, t] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        str code = blank(t);
        dict<str, str> ptr_of = ptr_of_map(code);
        list<str> ks;
        for (const auto& k : py::sorted(cands.keys())) ks.append(esc(k));
        for (const auto& m : py::re::finditer(S(R"PY(\b()PY") + py::join(S("|"), ks) + S(R"PY()\s*\()PY"), code)) {
            const auto& w = where.at(m.group(1));
            int64_t ln = nl_before(code, m.start());
            if (std::get<0>(w) == n && ln <= std::get<1>(w) + 1 && ln >= std::get<1>(w) - 1) continue;   // (its own definition's head)
            auto sp0 = call_args(sl(code, m.start(), m.start() + 400), m.group(1), false);
            list<Span> sp = sp0 ? *sp0 : list<Span>();
            for (int64_t k : cands.at(m.group(1))) {
                if (k < sp.size()) {
                    str a = py::re::sub(S(R"PY(^\(\s*(?:unsigned\s+)?char\s*\*\s*\)\s*)PY"), S(""),
                                        sl(code, m.start() + std::get<0>(sp[k]), m.start() + std::get<1>(sp[k])).strip());
                    given.setdefault(FK(m.group(1), k), set<std::optional<str>>())
                        .add(fullm(R"PY(\w+)PY", a) ? ptr_of.get(a) : std::optional<str>());
                }
            }
        }
    }
    auto gk = given.keys();
    std::sort(gk.begin(), gk.end());
    for (const auto& key : gk) {
        const str& fn = std::get<0>(key);
        int64_t k = std::get<1>(key);
        const auto& sts = given.at(key);
        if (sts.size() != 1 || sts.contains(std::nullopt)) continue;
        str st_ = **sts.begin();
        const asm16::Layout* lay = old.get(st_);
        if (!lay) continue;
        dict<std::tuple<int64_t, int64_t>, str> at;
        for (const auto& [mem, v] : lay->members)
            if (!mem.contains(S(".")) && !v.decl.contains(S("["))) at.set({v.off, v.size}, mem);
        auto [n, s, e] = where.at(fn);
        str pn = std::get<0>(defs_.at(fn)->params[k]);
        list<str> lines = lines_of(files.at(n));
        str body = unlines(lines.slice(s, e + 1));
        const str num = S(R"PY((0[xX][0-9a-fA-F]+|\d+))PY");
        str nw = py::re::sub(S(R"PY(\b)PY") + esc(pn) + S(R"PY(\s*\[\s*)PY") + num + S(R"PY(\s*\])PY"),
                             [&](const Match& m) {
                                 int64_t o = int0(m.group(1));
                                 if (at.contains({o, 1})) return S("((") + st_ + S(" *)") + pn + S(")->") + at.at({o, 1});
                                 return m.group();
                             },
                             body);
        auto word_at = [&](const Match& m) -> str {
            int64_t o = int0(m.group(1));
            if (at.contains({o, 2})) return S("((") + st_ + S(" *)") + pn + S(")->") + at.at({o, 2});
            // (the high word of a long there: its upper half)
            for (const auto& [mem, v] : lay->members)
                if (!mem.contains(S(".")) && v.size == 4 && v.off < o && o < v.off + 4 && !v.isptr)
                    return S("(((int16_t *)&((") + st_ + S(" *)") + pn + S(")->") + mem + S(")[") + fmt_int((o - v.off) / 2) + S("])");
            return m.group();
        };
        nw = py::re::sub(S(R"PY(\*\s*\(\s*(?:int|short|unsigned|int16_t)\s*\*\s*\)\s*\(\s*)PY") + esc(pn) + S(R"PY(\s*\+\s*)PY") + num + S(R"PY(\s*\))PY"),
                         word_at, nw);
        if (nw != body) {
            list<str> all = lines.slice(0, s);
            all.extend(lines_of(nw));
            all.extend(lines.slice(e + 1, lines.size()));
            files.set(n, unlines(all));
            notes_byte_places.append(py::percent(S("%s (%s's %s: %s)"), n, fn, pn, st_));
        }
    }
}

// ================================================================ shared_blocks
// cl = f03f02(0x14, 0, 0), struct cell *cl - 20 bytes, struct fix's old size, its cells cast to struct fix *
// elsewhere: blocks for either - the larger of the two structures now
void Port::shared_blocks() {
    if (_shared_blocks_done) return;
    _shared_blocks_done = true;
    const asm16::Layouts& old = old_layouts();
    const auto& allocs = allocators();
    if (!rule(S("alloc_places"))) return;   // (the Python's allocators() gave a set of names then: not a dict)
    dict<str, list<str>> casts;   // (struct X: the structures its objects are cast to, in the order found)
    auto add_cast = [&](const str& x, const str& s) {
        list<str>& l = casts.setdefault(x, list<str>());
        if (!l.contains(s)) l.append(s);
    };
    for (const auto& [n, t] : files.items()) {
        str code = blank(t);
        dict<str, str> ptr_of = ptr_of_map(code);
        for (const auto& m : py::re::finditer(S(R"PY(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\b(?!\s*\())PY"), code)) {
            auto x = ptr_of.get(m.group(2));
            str s = ws1(m.group(1));
            if (x && *x != s) add_cast(*x, s);
        }
        dict<str, str> rets;   // (f = f03fec(); struct fix *f, struct cell *f03fec(): the same)
        for (const auto& m : py::re::finditer(S(R"PY(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*\()PY"), code)) rets.set(m.group(2), ws1(m.group(1)));
        for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*(\w+)\s*\()PY"), code)) {
            auto s = ptr_of.get(m.group(1));
            auto x = rets.get(m.group(2));
            if (s && x && *s != *x) add_cast(*x, *s);
        }
    }
    for (const auto& [n, t0] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        str t = t0;
        str code = blank(t);
        dict<str, str> ptr_of = ptr_of_map(code);
        std::vector<Edit3> edits;
        for (const auto& m : py::re::finditer(S(R"PY(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?(\w+)\s*\()PY"), code)) {
            auto x = ptr_of.get(m.group(1));
            if (!x || !old.contains(*x) || !allocs.contains(m.group(2)) || allocs.at(m.group(2)).size() != 1) continue;
            auto sp0 = call_args(sl(code, m.start(2), m.start(2) + 400), m.group(2), false);
            list<Span> sp = sp0 ? *sp0 : list<Span>();
            int64_t k = *allocs.at(m.group(2)).begin();
            if (k >= sp.size()) continue;
            int64_t a_ = m.start(2) + std::get<0>(sp[k]), b_ = m.start(2) + std::get<1>(sp[k]);
            str v = sl(code, a_, b_).strip();
            if (!fullm(R"PY(0[xX][0-9a-fA-F]+|\d+)PY", v)) continue;
            int64_t nv = int0(v);
            int64_t xs = old.at(*x).size;
            if (nv <= xs) continue;
            std::vector<str> ss;   // (the largest it was made to hold: 18 of 20)
            if (casts.contains(*x))
                for (const auto& s : casts.at(*x))
                    if (old.contains(s) && xs < old.at(s).size && old.at(s).size <= nv) ss.push_back(s);
            std::stable_sort(ss.begin(), ss.end(), [&](const str& p, const str& q) { return -old.at(p).size < -old.at(q).size; });
            if (ss.empty()) continue;
            int64_t lead = sl(code, a_, b_).size() - sl(code, a_, b_).lstrip().size();
            edits.emplace_back(a_ + lead, a_ + lead + v.size(),
                               S("(sizeof(") + ss[0] + S(") > sizeof(") + *x + S(") ? sizeof(") + ss[0] + S(") : sizeof(") + *x + S("))"));
            notes_shared.append(py::percent(S("%s (%s, %s: %d)"), n, *x, ss[0], nv));
        }
        apply_desc(t, edits);
        if (!edits.empty()) files.set(n, t);
    }
}

// ================================================================ word_copies
// *(int *)s = *(int *)p; s, p struct psym * - the first word copied, the chain's link (two bytes then):
// the first member, as wide as it is now
void Port::word_copies() {
    if (_word_copies_done) return;
    _word_copies_done = true;
    const asm16::Layouts& old = old_layouts();
    py::re::Pattern rx = py::re::compile(
        S(R"PY(\*\s*\(\s*(?:unsigned\s+)?(?:int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*=\s*\*\s*\(\s*(?:unsigned\s+)?(?:int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*;)PY"));
    for (const auto& [n, t0] : files.items()) {
        if (!n.endswith(S(".c"))) continue;
        str t = t0;
        str code = blank(t);
        dict<str, str> ptr_of = ptr_of_map(code);
        std::vector<Edit3> edits;
        for (const auto& m : rx.finditer(code)) {
            auto a = ptr_of.get(m.group(1)), b = ptr_of.get(m.group(2));
            if (!a || !b || *a != *b || !old.contains(*a)) continue;
            std::optional<std::tuple<int64_t, str>> first;   // (min of (offset, name))
            for (const auto& [k, v] : old.at(*a).members)
                if (!first || std::make_tuple(v.off, k) < *first) first = std::make_tuple(v.off, k);
            if (!first || std::get<0>(*first) != 0 || old.at(*a).find(std::get<1>(*first))->size != 2) continue;
            edits.emplace_back(m.start(), m.end(),
                               m.group(1) + S("->") + std::get<1>(*first) + S(" = ") + m.group(2) + S("->") + std::get<1>(*first) + S(";"));
        }
        apply_desc(t, edits);
        if (!edits.empty()) {
            files.set(n, t);
            notes_word_copies.append(py::percent(S("%s (%d)"), n, static_cast<int64_t>(edits.size())));
        }
    }
}

// ================================================================ modernize
py::dict<py::str, py::str> Port::modernize() {
    // (the rules of the first round, each by its name)
    if (rule(S("word_copies"))) word_copies();
    if (rule(S("shared_blocks"))) shared_blocks();
    if (rule(S("byte_places"))) byte_places();
    if (rule(S("local_copies"))) local_copies();
    if (rule(S("pool_blocks"))) pool_blocks();
    if (rule(S("size_tables"))) size_tables();
    if (rule(S("numbered_tables"))) numbered_tables();
    array_bounds();
    word_stacks();
    merge_tables();
    fixed_allocators();
    agree_globals();
    notes = Notes();   // (defaultdict(list, {k: list(v) for k, v in self.mended.items()}))
    for (const auto& [k, v] : mended.items()) notes.set(k, v.copy());
    auto add_all = [&](const char* why, const list<str>& xs) {
        for (const auto& x : xs) notes[S(why)].append(x);
    };
    add_all("a variable another file declares a pointer, declared a number here (the old linker did not mind): the pointer; "
            "0xffff, the top of the segment, the top of memory", agree_notes);
    add_all("a structure's first word copied as an int (its chain's link then): the first member, as wide as it is now",
            notes_word_copies);
    add_all("a block of one structure's old size made for another, cast to it elsewhere: the larger of the two now", notes_shared);
    add_all("a view's pointer where the other has a number (one thing cast to both): the number a pointer's width",
            notes_view_wide);
    add_all("a view only one thing cast to both gives, that would read a byte of the other's word: not taken (the other "
            "then a record of bytes)", view_puns_refused);
    add_all("a structure's bytes reached through a char * by their old places: the members that were there",
            notes_byte_places);
    add_all("a file's own copy of the program's structure, its words where the other has pointers (the old layout the "
            "same): the program's, by its names", notes_local_copies);
    add_all("a pooled allocator's blocks of a size for the old pieces: so many times the size (today's pointers), its "
            "first, static blocks as big", notes_pools);
    add_all("a table of a structure's old byte counts (its first members, by kind): the same parts of today's structure "
            "(offsetof, sizeof)", notes_size_tables);
    add_all("a long the other files declare, defined as the two words it was in the old data: the long", long_notes);
    add_all("the program's own allocator of blocks of a structure's old size: the structure's size now", alloc_notes);
    for (const auto& [k_, v_] : union_notes.items())
        for (const auto& x : v_) notes[k_].append(x);
    add_all("member retyped: the source declared it otherwise than the program uses it (--types)", member_fixes);
    add_all("a table read past its end into the variables after it: one table, their names its elements", table_notes);
    for (const auto& [n, c] : notes_init.items())
        notes[S("far / near / pascal and their kind dropped (one flat memory, one way of calling)")]
            .append(py::percent(S("%s (%d)"), n, c));
    data = list<std::tuple<str, str>>();
    set<str> known_lib = set<str>::from(profile.libc) | set<str>::from(profile.platform_calls);
    for (const auto& n : py::sorted(files.keys())) {   // (the definitions first: every name the program has)
        if (n.endswith(S(".c"))) {
            str t = files.at(n);
            for (const auto& d : definitions(lines_of(t), lines_of(blank(t)))) {
                FnP& fp = fns.setdefault(std::get<0>(d)->name, std::get<0>(d));
                fp->file = n;
            }
        }
    }
    bss_views();
    pass_through_params();
    set<str> known = known_lib | set<str>::from(fns.keys()) | set<str>::from(asm_protos.keys());
    dict<str, str> lower;   // (pascal names were made capitals for the linker: a call in capitals and a definition in
    {                       //  small letters were one function)
        list<str> names = list<str>::from(fns.keys());
        names.extend(asm_protos.keys());
        for (const auto& f : names) lower.setdefault(f.lower(), f);
    }
    for (const auto& [n, t] : files.items()) {
        set<str> cs;
        for (const auto& m : py::re::finditer(S(R"PY(\b([A-Za-z_]\w*)\s*\()PY"), blank(t))) cs.add(m.group(1));
        for (const auto& c : cs) {
            auto d = lower.get(c.lower());
            if (d && !d->empty() && *d != c && !known.contains(c) && !renames.contains(c)) {
                renames.set(c, *d);
                notes[S("a pascal name called in other letters (the linker made both capitals): the definition's")]
                    .append(py::percent(S("%s (%s)"), n, c));
            }
        }
    }
    for (const auto& n : py::sorted(files.keys())) {   // (the callers' declarations say what a function gives, where its
        str code = blank(files.at(n));                  //  definition left it old C's int)
        for (const auto& m : py::re::finditer(
                 S(R"PY((?m)^\s*(?:extern\s+)?((?:unsigned\s+|signed\s+)?(?:struct\s+\w+|union\s+\w+|\w+)(?:\s*\*+\s*|\s+))(\w+)\s*\(\s*\)\s*;)PY"),
                 code)) {
            str t = ws1(m.group(1)).strip(), name = m.group(2);
            if (!fns.contains(name)) continue;
            FnP f = fns.at(name);
            if (f->ret.empty() && t != S("int") && t != S("extern") && !starts_any(t, {"extern", "static"}) &&
                (TYPEWORDS.contains(py::split(t)[0]) || py::split(t)[0] == S("struct") || py::split(t)[0] == S("union"))) {
                f->ret = t.strip();
                f->ret_set = true;
                notes[S("what a function of old C's int gives, as its callers declared it")]
                    .append(py::percent(S("%s (%s: %s)"), n, name, f->ret));
            }
        }
    }
    dict<str, str> outd;
    for (const auto& n : py::sorted(files.keys())) {
        str t = files.at(n);
        t = _preprocessor(t, n);
        t = sub_code(t, py::re::compile(S(R"PY((?m)^([ \t]*(?:register|auto|static)\s+)(?=\**\s*\w+\s*(?:\[[^\]]*\]\s*)?(?:[,;=]))(?!(?:unsigned|signed|int|char|short|long|float|double|void|const|volatile|register|static)\b))PY")),
                     [](const Match& m) { return m.group(1) + S("int "); });   // (register i; - old C's int, unsaid)
        str code = blank(t);
        set<str> macro_names;
        for (const auto& m : py::re::finditer(S(R"PY((?m)^\s*#\s*define\s+(\w+)\s+(\w+)\s*$)PY"), t))
            if (known.contains(m.group(2))) macro_names.add(m.group(1));   // (#define yylex f049ab: f049ab's)
        list<str> plat = profile.platform_calls.copy();
        plat.extend(aliases.keys());
        for (const auto& [w, d] : aliases.values()) plat.append(py::re::search(S(R"PY([A-Za-z_]\w*)PY"), w)->group());
        t = old_declarations(t, code, known | macro_names, notes, n, plat);
        if (n.endswith(S(".h"))) {
            auto [t2, defs] = header_data(t, blank(t), n, notes);
            t = t2;
            for (const auto& d : defs) data.append({n, d});
        }
        outd.set(n, t);
    }
    dict<str, str> tags;   // (a structure: the header that defines it - one alone; a tag two headers define is either's:
    set<str> twice;        //  none added)
    for (const auto& [h, t] : outd.items())
        if (h.endswith(S(".h")))
            for (const auto& m : py::re::finditer(S(R"PY(\b(struct|union)\s+(\w+)\s*\{)PY"), blank(t))) {
                str k_ = m.group(1) + S(" ") + m.group(2);
                if (tags.get(k_, h) != h) twice.add(k_);
                tags.setdefault(k_, h);
            }
    for (const auto& k_ : twice) tags.pop(k_, S(""));
    for (const auto& [n, t0] : outd.items()) {
        if (!n.endswith(S(".c"))) continue;
        str t = t0;
        str code = blank(t);
        set<str> own;
        for (const auto& m : py::re::finditer(S(R"PY(\b(struct|union)\s+(\w+)\s*\{)PY"), code)) own.add(m.group(1) + S(" ") + m.group(2));
        set<str> inc;   // (and what those include, in turn)
        list<str> todo_;
        for (const auto& m : py::re::finditer(S(R"PY(#\s*include\s*"([^"]+)")PY"), t)) todo_.append(m.group(1));
        while (todo_.size()) {
            str h_ = todo_.pop();
            if (!inc.contains(h_)) {
                inc.add(h_);
                for (const auto& m : py::re::finditer(S(R"PY(#\s*include\s*"([^"]+)")PY"), outd.get(h_, S(""))))
                    todo_.append(m.group(1));
            }
        }
        set<str> needs;
        set<str> structs_used;
        for (const auto& m : py::re::finditer(S(R"PY(\b(?:struct|union)\s+\w+)PY"), code)) structs_used.add(m.group());
        for (const auto& u : structs_used - own)
            if (tags.contains(u) && !inc.contains(tags.at(u))) needs.add(tags.at(u));
        if (need_headers.contains(n))
            for (const auto& h_ : need_headers.at(n)) if (!inc.contains(h_)) needs.add(h_);
        list<str> need = py::sorted(needs);
        if (need.size()) {   // (the old build gave it the header some other way: after its own - what those need comes first)
            list<str> ls = lines_of(t);
            int64_t at = -1;
            bool any = false;
            for (int64_t i = 0; i < ls.size(); ++i)
                if (py::re::match(S(R"PY(\s*#\s*include\b)PY"), ls[i])) { at = any ? std::max(at, i) : i; any = true; }
            if (!any) throw py::ValueError("max() arg is an empty sequence");
            list<str> ins;
            for (const auto& h : need) ins.append(S("#include \"") + h + S("\""));
            ls.set_slice(at + 1, at + 1, ins);
            t = unlines(ls);
            outd.set(n, t);
            notes[S("header of the structures a file uses, which it did not include: included")]
                .append(py::percent(S("%s (%s)"), n, py::join(S(", "), need)));
        }
        list<str> more;
        if (extra_decls.contains(n))
            for (const auto& d_ : extra_decls.at(n)) if (!t.contains(d_)) more.append(d_);
        more = py::sorted(more);
        if (more.size()) {   // (a variable a mend reads there: declared)
            list<str> ls = lines_of(outd.at(n));
            int64_t at = -1;
            for (int64_t i = 0; i < ls.size(); ++i)
                if (py::re::match(S(R"PY(\s*#\s*include\b)PY"), ls[i])) at = std::max(at, i);
            ls.set_slice(at + 1, at + 1, more);
            outd.set(n, unlines(ls));
        }
    }
    for (const auto& [n, t] : outd.items())
        if (n.endswith(S(".c"))) outd.set(n, _definitions(t, n));
    for (const auto& [n, t0] : outd.items()) {
        str t = t0;
        str code = blank(t);
        std::tie(t, code) = widths(t, code, notes, n, int16);
        t = word_unions(t, notes, n, old_records);
        t = pre_ansi::word_copies(t, notes, n, copied_members());
        t = stdio_inits(t, notes, n);
        t = file_internals(t, notes, n);
        t = whole_word_stores(t, notes, n);
        t = offset_views(t, n);
        t = block_heads(t, notes, n, ptr2_names());
        t = bss_ends(t, n);
        t = formats(t, code, notes, n, int16, profile.printf_hex_upper);
        t = struct_sizes(t, old_layouts_for(n), notes, n, allocators());
        t = header_offsets(t, old_layouts_for(n), notes, n);
        t = mem_sizes(t, old_var_sizes(), old_elem_sizes(), notes, n);
        outd.set(n, t);
    }
    for (const auto& [n, t0] : outd.items()) {   // (the program's allocators: the old sizes they keep their blocks by, the
        str t = t0;                              //  structures' sizes now)
        for (const auto& [fn_, sizes] : POOLS.items()) {
            if (!fns.contains(fn_)) continue;
            const FnP& f = fns.at(fn_);
            if (!f->file || *f->file != n) continue;
            str code = blank(t);
            auto dm = py::re::search(S(R"PY((?m)^[\w\s*]*\b)PY") + esc(fn_) + S(R"PY(\s*\([^;]*\)\s*$)PY"), code);
            if (!dm) continue;
            int64_t body_end = code.find(S("\n}"), dm->end());
            for (const auto& [st_, old_n] : sizes) {
                str seg = sl(t, dm->end(), body_end);
                str nw = py::re::sub(S(R"PY((?<![\w.]))PY") + fmt_int(old_n) + S(R"PY((?![\w.]))PY"), S("sizeof(") + st_ + S(")"), seg);
                if (nw != seg) {
                    t = sl(t, 0, dm->end()) + nw + t.slice_from(body_end);
                    body_end += nw.size() - seg.size();
                    auto h = tags.get(st_);
                    if (h && !h->empty() && !t.contains(S("#include \"") + *h + S("\""))) {   // (the structure's header, for its size)
                        t = replace1(t, S("#include \"port.h\""), S("#include \"port.h\"\n#include \"") + *h + S("\""));
                        body_end += (S("\n#include \"") + *h + S("\"")).size();
                    }
                    notes[S("the program's own allocator keeps its blocks by a structure's old size: its size now")]
                        .append(py::percent(S("%s (%s: %d)"), n, fn_, old_n));
                }
            }
        }
        outd.set(n, t);
    }
    frames(*this, outd);
    loose_calls(outd);
    array_sizes = dict<str, int64_t>();   // (each array the program defines: its elements)
    {
        auto all = outd.items();
        all.emplace_back(S("bss.c"), unlines(bss_definitions()));
        for (const auto& [n, t0] : all) {
            str t = t0;
            for (const auto& rule_ : decl_rules) t = apply_decl_rule(rule_, t);
            str c = blank(t);
            list<str> cl = lines_of(c);
            list<int64_t> ds_ = depths(cl);
            for (int64_t i = 0; i < cl.size(); ++i) {
                const str& l = cl[i];
                if (ds_[i] || starts_any(l.lstrip(), {"extern", "#", "typedef"})) continue;
                int64_t br = l.find(S("["));
                str before = br < 0 ? l : sl(l, 0, br);
                auto m = py::re::match(S(R"PY(\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*(\d+)\s*\]\s*(?:=|;))PY"), l);
                if (m && !before.contains(S("("))) {
                    array_sizes.setdefault(m->group(1), py::int_parse(m->group(2)));
                    continue;
                }
                m = py::re::match(S(R"PY(\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*\]\s*=\s*\{)PY"), l);
                if (m && !before.contains(S("("))) {
                    int64_t at = m->end() - 1;
                    for (int64_t q = 0; q < i; ++q) at += cl[q].size() + 1;
                    int64_t d = 0, k = at;
                    while (k < c.size()) {
                        char32_t ch = c.at_(k);
                        d += ch == U'{' ? 1 : ch == U'}' ? -1 : 0;
                        if (d == 0) break;
                        k += 1;
                    }
                    list<str> rows;   // (its rows: commas outside braces)
                    int64_t dd = 0;
                    str cur_;
                    for (const auto& chs : sl(c, at + 1, k)) {
                        char32_t ch = chs.at_(0);
                        dd += (ch == U'(' || ch == U'{' || ch == U'[') - (ch == U')' || ch == U'}' || ch == U']');
                        if (ch == U',' && dd == 0) {
                            rows.append(cur_);
                            cur_ = S("");
                        } else {
                            cur_ += chs;
                        }
                    }
                    rows.append(cur_);
                    int64_t nrows = 0;
                    for (const auto& x : rows) if (!x.strip().empty()) nrows += 1;
                    if (nrows) array_sizes.setdefault(m->group(1), nrows);
                }
            }
        }
    }
    for (const auto& h : outd.keys()) {   // (every header of the program's: once - port.h and data.c include some of them too)
        if (!h.endswith(S(".h"))) continue;
        if (outd.contains(h) && !outd.at(h).contains(S("DOSRT_ONCE_"))) {
            str g_ = S("DOSRT_ONCE_") + py::re::sub(S(R"PY(\W)PY"), S("_"), h).upper();
            outd.set(h, S("#ifndef ") + g_ + S("\n#define ") + g_ + S("\n") + outd.at(h).rstrip(S("\n")) + S("\n#endif\n"));
        }
    }
    for (const auto& [n, t] : outd.items()) outd.set(n, finish(n, t));
    list<str> bss_ = bss_definitions();
    agree_wide(outd, bss_, notes);
    if (rule(S("numbered_places"))) numbered_places(outd, bss_, notes);
    if (rule(S("old_c_ways"))) old_c_ways(outd, notes);
    out = outd;
    return outd;
}

// ================================================================ loose_calls
// the calls of an old-style definition kept (its calls unchecked, its integer parameters intptr_t): each
// integer given it made an intptr_t - unprototyped, an int 0 leaves the upper half of the 64-bit place what
// it was
void Port::loose_calls(py::dict<py::str, py::str>& out_) {
    dict<str, list<bool>> loose;
    for (const auto& f : fns.values()) {
        str key = renames.get(f->name, f->name);
        if (f->loose) {
            list<bool> l;
            for (const auto& [n_, t] : f->params)
                l.append(static_cast<bool>(py::re::fullmatch(
                    S(R"PY((?:register\s+)?(?:unsigned\s+int|unsigned|short|int|signed|intptr_t)\s+\w+)PY"), t.strip())));
            loose.set(key, l);
        } else if (f->va_slots) {   // (its variable arguments a slot each: every one of them an intptr_t)
            list<bool> l;
            for (int64_t i = 0; i < f->params.size(); ++i) l.append(false);
            for (int i = 0; i < 32; ++i) l.append(true);
            loose.set(key, l);
        }
    }
    if (loose.empty()) return;
    list<str> ks;
    for (const auto& k : py::sorted(loose.keys())) ks.append(esc(k));
    py::re::Pattern pat = py::re::compile(S(R"PY(\b()PY") + py::join(S("|"), ks) + S(R"PY()\s*\()PY"));
    for (const auto& [n, t] : out_.items()) {
        if (!n.endswith(S(".c"))) continue;
        list<str> lines = lines_of(t), code = lines_of(blank(t));
        bool changed = false;
        for (int64_t i = 0; i < code.size(); ++i) {
            const str& c = code[i];
            if (!pat.search(c) || py::re::match(S(R"PY(^\s*(?:static\s+)?[\w\s*]*\b\w+\s*\([\w\s,]*\)\s*$)PY"), c)) continue;   // (the definition's own head)
            str l = lines[i];
            auto ms = pat.finditer(c);
            for (int64_t q = ms.size() - 1; q >= 0; --q) {
                const Match& m = ms[q];
                auto spans = call_args(l.slice_from(m.start()), m.group(1), false);
                if (!spans || spans->empty()) continue;
                const list<bool>& ints = loose.at(m.group(1));
                for (int64_t k = spans->size() - 1; k >= 0; --k) {
                    auto [a_, b_] = (*spans)[k];
                    str arg = sl(l, m.start() + a_, m.start() + b_);
                    if (k >= ints.size() || !ints[k] || arg.strip().empty() || arg.strip().startswith(S("(intptr_t)")) ||
                        arg.strip() == S("...") || py::re::search(S(R"PY(\bchar\s*\*)PY"), arg))   // (a head's own: f(char *s, ...))
                        continue;
                    str lead = sl(arg, 0, arg.size() - arg.lstrip().size());
                    l = sl(l, 0, m.start() + a_) + lead + S("(intptr_t)(") + arg.strip() + S(")") + l.slice_from(m.start() + b_);
                    changed = true;
                }
            }
            if (l != lines[i]) {
                lines[i] = l;
                notes[S("an integer given to an old-style definition kept (unchecked calls): made its intptr_t")]
                    .append(py::percent(S("%s:%d"), n, i + 1));
            }
        }
        if (changed) out_.set(n, unlines(lines));
    }
}

// ================================================================ _globals
// [(line, name, base, shape, initializer or None)] of the one-name definitions at a file's top
list<std::tuple<int64_t, str, str, str, std::optional<str>>> Port::_globals(const str& text) {
    list<str> code = lines_of(blank(text));
    list<int64_t> dd = depths(code);
    list<std::tuple<int64_t, str, str, str, std::optional<str>>> out_;
    for (int64_t i = 0; i < code.size(); ++i) {
        const str& c = code[i];
        str st_ = c.strip();
        if (dd[i] || !st_.endswith(S(";")) || starts_any(st_, {"extern", "typedef", "#"}) ||
            (st_.replace(S("(*"), S("")).contains(S("(")) && !st_.contains(S("="))))
            continue;
        auto m = py::re::match(S(R"PY(([^=]*?)(?:=(.*))?;$)PY"), st_);
        if (!m) continue;
        auto r = declarators(m->group(1).strip());
        if (!r || std::get<1>(*r).size() != 1) continue;
        auto [nm, dcl] = std::get<1>(*r)[0];
        str base = ws1(py::re::sub(S(R"PY(\bstatic\s+)PY"), S(""), std::get<0>(*r))).strip();
        str raw = lines_of(text)[i];
        int64_t eq = c.find(S("=")), semi = c.rfind(S(";"));
        out_.append({i, nm, base, py::re::sub(S(R"PY(\b)PY") + esc(nm) + S(R"PY(\b)PY"), S("@"), dcl).replace(S(" "), S("")),
                     m->group_opt(2) ? std::optional<str>(sl(raw, eq + 1, semi).strip()) : std::nullopt});
    }
    return out_;
}

}  // namespace codebreaker::port::pre_ansi
