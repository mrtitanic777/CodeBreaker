// codebreaker/port/sanitize.cpp -- typed C++ port of codebreaker.port.sanitize (see sanitize.hpp).
//
// sanitize.py -- what the port does wrong only when it runs: found by AddressSanitizer on the verify cases, mended.
//
// An old program may write past the end of an array where its own linker put the next variable - harmless there
// (what it overwrote it did not read again, or read as it meant to), memory of something else here. The compiler
// cannot see it; a run can. The port is built with -fsanitize=address, every verify case run, and each overflow the
// sanitizer reports becomes a rule of the port:
//
//   a global array written (or read) past its end   the array given room for what the original reached
//   a table read before its start (t[k - 0x101],    what the old linker had put there, by the program's data
//   k at the end of the input: -1)                  segment's places (SYMBOLS): that variable read instead

#include "codebreaker/port/sanitize.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "codebreaker/base/ctext.hpp"

namespace codebreaker::port::sanitize {

using base::ctext::blank;
using py::dict;
using py::list;
using py::set;
using py::str;

namespace {

str S(const char* s) { return str(s); }
// the Python's "%..." % args
template <class... A> str F(const char* f, const A&... a) { return py::percent(str(f), a...); }
str esc(const str& s) { return py::re::escape(s); }
// re.sub(r"\s+", " ", x)
str ws1(const str& x) { return py::re::sub(S(R"(\s+)"), S(" "), x); }
// the escaped name's `\ ` made `\s+` (the Python's .replace(r"\ ", r"\s+"))
str spaces_free(const str& escaped) { return escaped.replace(S("\\ "), S("\\s+")); }
// x.replace(a, b, 1)
str replace1(const str& x, const str& a, const str& b) {
    int64_t at = x.find(a);
    if (at < 0) return x;
    return x.slice(0, at) + b + x.slice(at + a.size(), x.size());
}
// code points a..b of text, as t[a:b]
str sl(const str& s, int64_t a, int64_t b) { return s.slice(a, b); }
int64_t to_int(const str& s) { return py::int_parse(s, 10); }
str stars_of(int64_t n) { return str(std::string(static_cast<size_t>(std::max<int64_t>(n, 0)), '*')); }

// the lines of `text` (text.split("\n"))
list<str> lines_of(const str& text) { return py::split(text, S("\n")); }

// any(r[:2] == (f, v) for r in np_rules), and with the kind: any(r[:2] == (f, v) and r[3:] == (kind,) ...)
bool has_rule(const pre_ansi::Port& port, const str& f, const str& v) {
    for (const pre_ansi::NpRule& r : port.np_rules)
        if (r.file == f && r.var == v) return true;
    return false;
}
bool has_rule_kind(const pre_ansi::Port& port, const str& f, const str& v, const char* kind) {
    for (const pre_ansi::NpRule& r : port.np_rules)
        if (r.file == f && r.var == v && r.kind && *r.kind == kind) return true;
    return false;
}
// port.np_rules = port.np_rules + more  (a new list: the old one is left as it was)
void add_rules(pre_ansi::Port& port, const std::vector<pre_ansi::NpRule>& more) {
    list<pre_ansi::NpRule> n = port.np_rules.copy();
    for (const auto& r : more) n.append(r);
    port.np_rules = n;
}

// fnmatch.fnmatchcase after ntpath.normcase on both sides (Windows): lower case, / as \ .
bool match_from(const std::u32string& n, size_t i, const std::u32string& p, size_t j) {
    while (j < p.size()) {
        char32_t c = p[j];
        if (c == U'*') {
            while (j < p.size() && p[j] == U'*') j++;
            if (j == p.size()) return true;
            for (size_t k = i; k <= n.size(); k++)
                if (match_from(n, k, p, j)) return true;
            return false;
        }
        if (c == U'?') {
            if (i >= n.size()) return false;
            i++;
            j++;
            continue;
        }
        if (c == U'[') {
            size_t k = j + 1;
            if (k < p.size() && p[k] == U'!') k++;
            if (k < p.size() && p[k] == U']') k++;
            while (k < p.size() && p[k] != U']') k++;
            if (k >= p.size()) {                            // (no closing bracket: the bracket itself)
                if (i >= n.size() || n[i] != U'[') return false;
                i++;
                j++;
                continue;
            }
            if (i >= n.size()) return false;
            size_t a = j + 1;
            bool neg = false;
            if (p[a] == U'!') { neg = true; a++; }
            bool hit = false;
            for (size_t x = a; x < k; x++) {
                if (x + 2 < k && p[x + 1] == U'-') {
                    if (p[x] <= n[i] && n[i] <= p[x + 2]) hit = true;
                    x += 2;
                } else if (p[x] == n[i]) {
                    hit = true;
                }
            }
            if (hit == neg) return false;
            i++;
            j = k + 1;
            continue;
        }
        if (i >= n.size() || n[i] != c) return false;
        i++;
        j++;
    }
    return i == n.size();
}
bool fnmatch_(const str& name, const str& pat) {
    auto norm = [](const str& s) { return s.lower().replace(S("/"), S("\\")).u32(); };
    return match_from(norm(name), 0, norm(pat), 0);
}

bool has_magic(const str& s) {
    for (char32_t c : s.u32())
        if (c == U'*' || c == U'?' || c == U'[') return true;
    return false;
}
bool is_hidden(const str& s) { return s.startswith(S(".")); }

// glob._glob1 / _glob0 and _iglob
list<str> glob1(const str& dirname, const str& pattern) {
    list<str> names;
    try {
        names = py::os::listdir(dirname.empty() ? S(".") : dirname);
    } catch (const py::OSError&) {
        return list<str>();
    }
    list<str> out;
    for (const str& n : names)
        if ((!is_hidden(n) || is_hidden(pattern)) && fnmatch_(n, pattern)) out.append(n);
    return out;
}
list<str> glob0(const str& dirname, const str& basename) {
    list<str> out;
    if (!basename.empty()) {
        if (py::os::path::exists(py::os::path::join(dirname, basename))) out.append(basename);
    } else if (py::os::path::isdir(dirname)) {
        out.append(basename);
    }
    return out;
}
list<str> iglob(const str& pathname) {
    list<str> out;
    auto [dirname, basename] = py::os::path::split(pathname);
    if (!has_magic(pathname)) {
        if (!basename.empty()) {
            if (py::os::path::exists(pathname)) out.append(pathname);
        } else if (py::os::path::isdir(dirname)) {
            out.append(pathname);
        }
        return out;
    }
    if (dirname.empty()) {
        for (const str& n : glob1(S("."), basename)) out.append(n);
        return out;
    }
    list<str> dirs;
    if (dirname != pathname && has_magic(dirname)) dirs = iglob(dirname);
    else dirs.append(dirname);
    for (const str& d : dirs) {
        list<str> names = has_magic(basename) ? glob1(d, basename) : glob0(d, basename);
        for (const str& n : names) out.append(py::os::path::join(d, n));
    }
    return out;
}

}  // namespace

// glob.glob(pattern)
list<str> glob(const str& pattern) { return iglob(pattern); }

const py::re::Pattern GLOBAL(R"PY(AddressSanitizer: global-buffer-overflow.*?\n(?P<rw>READ|WRITE) of size (?P<sz>\d+).*?is located (?P<after>\d+) bytes after global variable '(?P<var>\w+)'.*?of size (?P<size>\d+))PY",
                             py::re::S);

const py::re::Pattern BEFORE(R"PY(AddressSanitizer: global-buffer-overflow.*?\n(?P<rw>READ) of size (?P<sz>\d+).*?\n\s*#0 0x[0-9a-fA-F]+ in \w+ (?P<path>[^\n]*?):(?P<line>\d+))PY",
                             py::re::S);
const py::re::Pattern BEFORE_VAR(R"PY(is located (?P<before>\d+) bytes before global variable '(?P<var>\w+)'.*?of size (?P<size>\d+))PY", 0);

const dict<str, std::tuple<int64_t, int64_t>> C_SIZES = {
    {S("char"), {1, 1}}, {S("signed char"), {1, 1}}, {S("unsigned char"), {1, 1}}, {S("int16_t"), {2, 2}},
    {S("uint16_t"), {2, 2}}, {S("short"), {2, 2}}, {S("int32_t"), {4, 4}}, {S("uint32_t"), {4, 4}}, {S("int"), {4, 4}},
    {S("unsigned"), {4, 4}}, {S("long"), {4, 4}}, {S("float"), {4, 4}}, {S("double"), {8, 8}}, {S("intptr_t"), {8, 8}},
    {S("int64_t"), {8, 8}}};

// {member: (offset, size)} and the size of a structure of today's C (x64: pointers 8, natural alignment) - of
// its members' text (the port's: int16_t and the like); (None, None) when a member is not known
std::tuple<std::optional<dict<str, std::tuple<int64_t, int64_t>>>, std::optional<int64_t>> c_layout(const str& body) {
    using Out = std::tuple<std::optional<dict<str, std::tuple<int64_t, int64_t>>>, std::optional<int64_t>>;
    int64_t off = 0, align = 1;
    dict<str, std::tuple<int64_t, int64_t>> out;
    for (const str& d : py::split(blank(body), S(";"))) {
        str ds = d.strip();
        auto r = !ds.empty() ? pre_ansi::declarators(ds) : std::nullopt;
        if (!r) continue;
        str base = py::re::sub(S(R"(\s+)"), S(" "), py::re::sub(S(R"(\b(register|const|volatile|signed)\b(?= \w))"), S(""), std::get<0>(*r))).strip();
        for (const auto& nd : std::get<1>(*r)) {
            const str& nm = std::get<0>(nd);
            const str& dcl = std::get<1>(nd);
            int64_t n = 1;
            for (const str& x : py::re::findall(S(R"(\[\s*(\d+)\s*\])"), dcl)) n *= to_int(x);
            int64_t sz, al;
            if (replace1(dcl, nm, S("")).contains(S("*"))) {
                sz = 8;
                al = 8;
            } else if (C_SIZES.contains(base)) {
                std::tie(sz, al) = C_SIZES.at(base);
            } else {
                return Out{std::nullopt, std::nullopt};
            }
            off = py::floordiv(off + al - 1, al) * al;
            out[nm] = {off, sz * n};
            off += sz * n;
            align = std::max(align, al);
        }
    }
    return Out{out, py::floordiv(off + align - 1, align) * align};
}

const py::re::Pattern INDEX(R"PY((?P<path>[^\s:]+(?::[\\/][^\s:]+)?):(?P<line>\d+):(?P<col>\d+): runtime error: index (?P<idx>-\d+) out of bounds for type '(?P<type>[^']*)')PY", 0);

// a read before a table's start: what the original read there (the variable its linker put there) - a rule
// for the line; -> why, or None
std::optional<str> before_start(pre_ansi::Port& port, const dict<str, str>& out, const str& case_, const str& text) {
    const dict<str, int64_t>& ds = port.ds;
    auto ix = INDEX.search(text);
    std::optional<int64_t> given;
    str var, f, line;
    int64_t before = 0, ln = 0;
    if (ix) {                                               // (the bounds check's report: the row itself)
        f = py::os::path::basename(ix->group(1));
        ln = to_int(ix->group(2));
        if (!out.contains(f)) return std::nullopt;
        line = lines_of(out.at(f))[ln - 1];
        list<py::re::Match> names;
        for (const auto& x : py::re::finditer(S(R"(\b(\w+)\s*\[)"), line))
            if (ds.contains(x.group(1))) names.append(x);
        if (names.empty()) return std::nullopt;
        int64_t col = to_int(ix->group(3)) - 1;
        const py::re::Match* best = &names[0];
        int64_t bestd = std::abs(names[0].start() - col);
        for (int64_t k = 1; k < names.size(); k++) {
            int64_t dd = std::abs(names[k].start() - col);
            if (dd < bestd) {
                bestd = dd;
                best = &names[k];
            }
        }
        var = best->group(1);
        given = to_int(ix->group(4));
        before = 0;
    } else {
        auto m = BEFORE.search(text);
        auto v = BEFORE_VAR.search(text);
        if (!m || !v || !ds.contains(v->group(2))) return std::nullopt;
        var = v->group(2);
        before = to_int(v->group(1));
        f = py::os::path::basename(m->group(3));
        ln = to_int(m->group(4));
        if (!out.contains(f)) return std::nullopt;
        line = lines_of(out.at(f))[ln - 1];
    }
    auto x = py::re::search(S(R"(\b)") + esc(var) + S(R"(\s*\[([^\[\]]+)\]\s*\.\s*(\w+))"), line);
    if (!x || py::re::search(S(R"(\+\+|--|[^=!<>]=[^=])"), x->group(1))) return std::nullopt;
    str field = x->group(2);
    std::optional<str> decl;
    for (const auto& kv : out.items()) {                    // (the table's structure and its rows)
        const str& t = std::get<1>(kv);
        auto d = py::re::search(S(R"(\b(struct\s+\w+)\s+)") + esc(var) + S(R"(\s*\[[^\]]*\]\s*=\s*\{)"), blank(t));
        if (d) {
            decl = d->group(1);
            break;
        }
    }
    if (!decl) {
        for (const auto& kv : out.items()) {
            auto d = py::re::search(S(R"(\b(struct\s+\w+)\s+)") + esc(var) + S(R"(\s*\[)"), blank(std::get<1>(kv)));
            if (d) {
                decl = d->group(1);
                break;
            }
        }
    }
    if (!decl) return std::nullopt;
    str st = ws1(*decl);
    std::optional<str> body;
    for (const auto& kv : out.items()) {
        const str& t = std::get<1>(kv);
        auto b = py::re::search(S(R"(\b)") + spaces_free(esc(st)) + S(R"(\s*\{([^{}]*)\})"), blank(t));
        if (b) {
            body = sl(t, b->start(1), b->end(1));
            break;
        }
    }
    std::optional<dict<str, std::tuple<int64_t, int64_t>>> lay;
    std::optional<int64_t> esz;
    if (body && !body->empty()) std::tie(lay, esz) = c_layout(*body);
    const asm16::Layouts& old = port.old_layouts_for(f);
    const asm16::Layout* olay = old.get(st);
    if (!lay || lay->empty() || !lay->contains(field) || !olay || !olay->has(field)) return std::nullopt;
    int64_t idx;
    int64_t lay_off = std::get<0>(lay->at(field));
    if (given) {
        idx = *given;
    } else {
        int64_t pos = -before;                              // (the place read, from the table's start, today)
        idx = py::floordiv(pos - lay_off, *esz);            // (the row: below the first)
        if (idx * *esz + lay_off != pos) return std::nullopt;   // (not that member of a row: another read)
    }
    int64_t addr = ds.at(var) + idx * olay->size + olay->find(field)->off;   // (that row's member in the old data segment)
    bool have_owner = false;
    int64_t owner_a = 0;
    str owner_n;
    for (const auto& kv : ds.items()) {                     // (max over (address, name) pairs)
        int64_t a = std::get<1>(kv);
        if (a > addr) continue;
        if (!have_owner || a > owner_a || (a == owner_a && std::get<0>(kv) > owner_n)) {
            have_owner = true;
            owner_a = a;
            owner_n = std::get<0>(kv);
        }
    }
    if (!have_owner) return std::nullopt;                   // (before every variable the program has: no variable to read)
    int64_t o = addr - owner_a;
    str w = owner_n;
    std::optional<str> wexpr;
    for (const auto& kv : out.items()) {                    // (what is there: of the variable's own type)
        const str& t = std::get<1>(kv);
        auto d = py::re::search(S(R"(\b(struct\s+\w+)\s+)") + esc(w) + S(R"(\s*\[)"), blank(t));
        if (d) {
            str wst = ws1(d->group(1));                     // (declared where it is read now, its header too)
            port.extra_decls.setdefault(f, set<str>()).add(F("extern %s %s[];", wst, w));
            std::vector<str> hs;
            for (const auto& hk : out.items()) {
                const str& h = std::get<0>(hk);
                if (h.endswith(S(".h")) && py::re::search(S(R"(\b)") + spaces_free(esc(wst)) + S(R"(\s*\{)"), blank(std::get<1>(hk))))
                    hs.push_back(h);
            }
            if (!hs.empty()) {
                list<str>& need = port.need_headers.setdefault(f, list<str>());
                if (!need.contains(hs[0])) need.append(hs[0]);
            }
            const asm16::Layout* wl = old.get(ws1(d->group(1)));
            if (wl) {
                int64_t e = py::floordiv(o, wl->size), rem = py::mod(o, wl->size);
                std::optional<str> mem;
                for (const auto& mk : wl->members)
                    if (mk.second.off == rem) {
                        mem = mk.first;
                        break;
                    }
                if (mem) wexpr = F("%s[%d].%s", w, e, *mem);
            }
            break;
        }
    }
    if (!wexpr && o == 0) wexpr = w;
    if (!wexpr) return std::nullopt;
    std::optional<std::tuple<str, int64_t>> o_;
    if (port.line_origin.contains(f)) o_ = port.line_origin.at(f).get(ln);
    str nw = sl(line, 0, x->start()) + F("((%s) == %d ? %s : %s)", x->group(1).strip(), idx, *wexpr, x->group(0)) + sl(line, x->end(), line.size());
    port.line_fixes[o_ ? std::make_tuple(f, std::get<0>(*o_), std::get<1>(*o_)) : std::make_tuple(f, line, int64_t(-1))] = nw;
    str why = F("%s:%d %s[%d].%s (case '%s'): before the table - the original read %s there", f, ln, var, idx, field, case_, *wexpr);
    port.mended[S("a table read before its start (as the original did: its linker's variable there): that variable read")].append(why);
    return why;
}

// where clang's sanitizer runtime is, beside the compiler (Windows: a DLL the program needs on its path)
std::optional<str> runtime_dir(const str& cc) {
    str root = py::os::path::dirname(py::os::path::dirname(py::os::path::abspath(cc)));
    std::vector<str> found;
    for (const str& p : glob(py::os::path::join(py::os::path::join(py::os::path::join(py::os::path::join(py::os::path::join(root, S("lib")), S("clang")), S("*")), S("lib")), S("windows"))))
        found.push_back(p);
    std::sort(found.begin(), found.end(), [](const str& a, const str& b) { return a > b; });   // (sorted, reverse)
    for (const str& p : found)
        if (!glob(py::os::path::join(p, S("clang_rt.asan_dynamic*.dll"))).empty()) return p;
    return std::nullopt;
}

const py::re::Pattern LOW(R"PY(access-violation on unknown address 0x0*(?P<addr>[0-9a-fA-F]{1,2})\b.*?\n(?:.*\n)*?\s*#\d+ 0x[0-9a-fA-F]+ in (?P<fn>\w+) (?P<path>[^\n]*?):(?P<line>\d+))PY", 0);

// a read at a small address (below 100h) on a line that reads p[i]->m: p's words small numbers at times - on
// DOS a read of the null segment there; each p[...]-> of the file read through dos_np; -> why, or None
std::optional<str> low_reads(pre_ansi::Port& port, const dict<str, str>& out, const str& case_, const str& text) {
    auto m = LOW.search(text);
    if (!m) return std::nullopt;
    str f = py::os::path::basename(m->group(3));
    if (!out.contains(f)) return std::nullopt;
    str addr = m->group(1);
    int64_t lno = to_int(m->group(4));
    list<str> all = lines_of(out.at(f));
    str line = lno <= all.size() ? all[lno - 1] : S("");
    const str& of = out.at(f);
    for (const str& v : py::re::findall(S(R"((?<![\w.>])(\w+)\s*\[[^\[\]]*\]\s*->)"), line)) {
        if (has_rule(port, f, v)) continue;
        auto d = py::re::search(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*\*\s*)") + esc(v) + S(R"(\b|\b((?:struct|union)\s+\w+)\s*\*\s*)") + esc(v) + S(R"(\s*\[)"), of);
        if (!d) continue;
        std::optional<str> g1 = d->group_opt(1);
        str typ = ((g1 && !g1->empty()) ? *g1 : d->group(2)) + S(" *");
        add_rules(port, {pre_ansi::NpRule{f, v, typ, std::nullopt}});
        str why = F("%s: %s[...] a small number at times (read at %sh, case '%s'): read through the null segment there", f, v, addr, case_);
        port.mended[S("a table's word read as a pointer, a small number at times (DOS read the null segment there): dos_np")].append(why);
        return why;
    }
    bool chains_on = pre_ansi::rule(S("np_chains"));
    if (chains_on) {
        // p->next->prev = ..: p was read through - the NULL its link, p->next, the list's last's
        for (const auto& g : py::re::Pattern(S(R"((?<![\w.>])(?:\(\s*\([^()]*\)\s*dos_np\(\s*(\w+)\s*\)\s*\)|(\w+))\s*->\s*(\w+)\s*->)"), 0)
                                 .findall_groups<str, str, str>(line)) {
            const str& a_ = std::get<0>(g);
            const str& b_ = std::get<1>(g);
            str v = !a_.empty() ? a_ : b_;
            str mem = std::get<2>(g);
            if (has_rule_kind(port, f, mem, "mem")) continue;
            auto tm = py::re::search(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*)") + esc(v) + S(R"(\s*[;,=)])"), of);
            if (!tm) continue;
            dict<str, str> texts = port.files.copy();       // (the definition the file sees: its own, its headers')
            for (const auto& kv : out.items())
                if (!std::get<1>(kv).empty()) texts[std::get<0>(kv)] = std::get<1>(kv);
            set<str> seen;
            std::vector<str> todo = {f};
            list<str> mine;
            while (!todo.empty()) {
                str x = todo.front();
                todo.erase(todo.begin());
                if (seen.contains(x) || !texts.contains(x)) continue;
                seen.add(x);
                mine.append(texts.at(x));
                for (const str& inc : py::re::findall(S(R"PY(#\s*include\s*"([^"]+)")PY"), texts.at(x))) todo.push_back(inc);
            }
            str rx_ = S(R"(\b)") + spaces_free(esc(tm->group(1))) + S(R"(\s*\{([^{}]*)\})");
            auto body = py::re::search(rx_, py::join(S("\n"), mine));
            if (!body) {
                list<str> all_texts;
                for (const auto& kv : texts.items()) all_texts.append(std::get<1>(kv));
                body = py::re::search(rx_, py::join(S("\n"), all_texts));
            }
            std::optional<py::re::Match> mt;
            if (body)
                mt = py::re::search(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*\s*)*)") + esc(mem) + S(R"(\s*[;,])"), body->group(1));
            if (!mt) continue;
            add_rules(port, {pre_ansi::NpRule{f, mem, mt->group(1) + S(" *"), S("mem")}});
            str why = F("%s: %s->%s a NULL at times (at %sh, case '%s'): through the null segment there", f, v, mem, addr, case_);
            port.mended[S("a pointer read through when NULL (DOS read the null segment there): dos_np")].append(why);
            return why;
        }
        // ((q = f06df2(p))->kind: the value given back NULL at the list's end
        for (const str& v : py::re::findall(S(R"(\(\s*(\w+)\s*=\s*\w+\s*\([^()]*\)\s*\)\s*->)"), line)) {
            if (has_rule_kind(port, f, v, "assign")) continue;
            auto d = py::re::search(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*\s*)*)") + esc(v) + S(R"(\s*[;,=)])"), of);
            if (!d) continue;
            std::vector<pre_ansi::NpRule> more = {pre_ansi::NpRule{f, v, d->group(1) + S(" *"), S("assign")}};
            if (!has_rule_kind(port, f, v, "var")) more.push_back(pre_ansi::NpRule{f, v, d->group(1) + S(" *"), S("var")});   // (and q->op after it)
            add_rules(port, more);
            str why = F("%s: %s as given a NULL at times (at %sh, case '%s'): read through the null segment there", f, v, addr, case_);
            port.mended[S("a pointer read through when NULL (DOS read the null segment there): dos_np")].append(why);
            return why;
        }
    }
    for (const str& v : py::re::findall(S(R"((?<![\w.>])(\w+)\s*->)"), line)) {   // (l->op, l a NULL there: as DOS read it)
        if (has_rule(port, f, v)) continue;
        auto d = py::re::search(S(R"(\b((?:struct|union)\s+\w+)\s*\*\s*)") + esc(v) + S(R"(\s*[;,=)])"), of);
        if (!d) continue;
        add_rules(port, {pre_ansi::NpRule{f, v, d->group(1) + S(" *"), S("var")}});
        str why = F("%s: %s a NULL at times (read at %sh, case '%s'): read through the null segment there", f, v, addr, case_);
        port.mended[S("a pointer read through when NULL (DOS read the null segment there): dos_np")].append(why);
        return why;
    }
    return std::nullopt;
}

// the rules from the sanitizer's reports; -> what was mended ([(variable, why)])
list<std::tuple<str, str>> mend(pre_ansi::Port& port, const dict<str, str>& out, const list<std::tuple<str, str>>& reports) {
    list<std::tuple<str, str>> done;
    auto in_done = [&](const str& why) {
        for (const auto& cw : done)
            if (std::get<1>(cw) == why) return true;
        return false;
    };
    for (const auto& report : reports) {
        const str& case_ = std::get<0>(report);
        const str& text = std::get<1>(report);
        bool found = false;
        std::optional<str> why = low_reads(port, out, case_, text);
        if (why) {
            done.append({case_, *why});
            continue;
        }
        for (const auto& ix : INDEX.finditer(text)) {       // (each index the bounds check reported: one a read
            why = before_start(port, out, case_, ix.group(0));   // before a table's start, perhaps)
            if (why && !in_done(*why)) {
                done.append({case_, *why});
                found = true;
            }
        }
        if (found) continue;
        if (text.contains(S("bytes before global variable"))) {
            str head = py::split(text, S("bytes before global variable"))[0];
            str tail = head.size() > 400 ? sl(head, head.size() - 400, head.size()) : head;   // ([-400:])
            if (!tail.contains(S("bytes after global variable"))) {
                why = before_start(port, out, case_, text);
                if (why) {
                    done.append({case_, *why});
                    continue;
                }
            }
        }
        for (const auto& m : GLOBAL.finditer(text)) {
            str var = m.group(4);
            str rw = m.group(1);
            int64_t sz = to_int(m.group(2)), after = to_int(m.group(3)), size = to_int(m.group(5));
            if (!port.reach.contains(var) && port.joins_next(var)) {
                list<str> files_texts;
                for (const auto& kv : port.files.items()) files_texts.append(std::get<1>(kv));
                auto elems = py::re::search(S(R"((?m)^\s*(?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned)\s+)") + esc(var) +
                                                S(R"(\s*\[[^\]]*\]\s*=\s*\{([^}]*)\})"),
                                            py::join(S("\n"), files_texts));
                int64_t n = 0;
                if (elems)
                    for (const str& x : py::split(elems->group(1), S(",")))
                        if (!x.strip().empty()) n++;
                if (n) {                                    // (read into the variable after it: one table, as the
                    int64_t elem = py::floordiv(size, n);   //  original's linker made them - its elements)
                    port.reach[var] = py::floordiv(size + after + sz - 1, elem);
                    str why2 = F("%s[%d] %s (case '%s'): the table went on into the variables after it - one table", var, port.reach.at(var), rw.lower(), case_);
                    port.mended[S("a table read past its end into the variables after it (a run showed it): one table")].append(why2);
                    done.append({var, why2});
                    continue;
                }
            }
            if (port.bss.contains(var) && port.join_bss(var)) {
                str why2 = F("%s %s past its end (case '%s'): the BSS variable after it, one array with it", var, rw.lower(), case_);
                port.mended[S("a BSS array gone past into the one after it (the old layout's): one array, their names places in it")].append(why2);
                done.append({var, why2});
                continue;
            }
            int64_t decl = 0;
            for (const auto& kv : out.items()) {            // (its definition: not a declaration of it, extern)
                auto d = py::re::search(S(R"((?m)^\s*(?!extern\b)(?:static\s+)?(?:[\w ]*?\b)?(?:char|short|int|long|unsigned|signed|float|double|struct\s+\w+|union\s+\w+|\w+_t)\b[\s*]+)") +
                                            esc(var) + S(R"(\s*\[\s*(\d+)\s*\])"),
                                        std::get<1>(kv));
                if (d) {
                    decl = to_int(d->group(1));
                    break;
                }
            }
            bool sized = true;
            if (!decl) {                                    // (char s[] = "...": its size the string's)
                for (const auto& kv : out.items()) {
                    if (py::re::search(S(R"((?m)^\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+)") + esc(var) + S(R"(\s*\[\s*\]\s*=)"), std::get<1>(kv))) {
                        decl = size;
                        sized = false;
                        break;
                    }
                }
            }
            if (!decl || port.grown.get(var, 0) > decl) continue;   // (grown already, this round)
            int64_t elem = std::max<int64_t>(1, py::floordiv(size, decl));   // (a declaration larger than the size reported: one byte an element)
            int64_t extra = -py::floordiv(-(after + sz), elem);     // (math.ceil((after + sz) / elem))
            port.grown[var] = decl + extra;
            if (sized) {
                port.decl_rules.append(pre_ansi::DeclRule(
                    S(R"((?m)^(\s*(?:extern\s+|static\s+)?(?:[\w ]*?\b)?(?:char|short|int|long|unsigned|signed|float|double|struct\s+\w+|union\s+\w+|\w+_t)\b[\s*]+)") +
                        esc(var) + S(R"(\s*\[)\s*)") + str(std::to_string(decl)) + S(R"(\s*\])"),
                    S(R"(\g<1>)") + str(std::to_string(decl + extra)) + S("]")));
            } else {
                port.decl_rules.append(pre_ansi::DeclRule(
                    S(R"((?m)^(\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+)") + esc(var) + S(R"(\s*\[)\s*\])"),
                    S(R"(\g<1>)") + str(std::to_string(decl + extra)) + S("]")));
            }
            str why2 = F("%s[%d] %s (case '%s'): the original reached into the variable its linker put after it", var, decl + extra - 1,
                         rw == "WRITE" ? "written" : "read", case_);
            port.mended[S("array reached past its end (as the original did, harmlessly there): room made for it")].append(why2);
            done.append({var, why2});
        }
    }
    return done;
}

}  // namespace codebreaker::port::sanitize
