// codebreaker/port/pre_ansi_p5.cpp -- typed C++ port of codebreaker.port.pre_ansi, PART 5 (Python lines 4262-5436):
// call_at, ctype, block_heads, whole_word_stores, file_internals, stdio_inits, word_copies, word_unions,
// old_c_ways, numbered_places, agree_wide, widen_member, stored_wide, passed_on, union_member_for, visible_tags, mend.
//
// Typing notes:
//   * A "complaint of the compiler" (the dicts port/__main__.py builds) is the record Diag.
//   * Port.retyped is keyed by a variable's name, or for a local by the tuple (file, function, variable): here a
//     local's key is the text retyped_key(file, function, variable) (it is read only by mend).
//   * Port.decl_rules' replacement is a template text or a split_decl: DeclRule (apply_decl_rule runs one).
//   * Port.need_headers is {file: [headers]}: the Python's set semantics (add) are "append unless there".
//   * The Python `mend` names a groupdict of the complaint's pattern: here the names of the pattern that matched
//     (SAYS, searched again as said() does) are looked up by `gname`.

#include "codebreaker/port/pre_ansi.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/ctext.hpp"

namespace codebreaker::port::pre_ansi {

using py::list;
using py::set;
using py::str;
using Match = py::re::Match;
using codebreaker::base::ctext::blank;
using codebreaker::base::ctext::sub_code;

namespace {

str S(const char* s) { return str(s); }

// (the Python's `x in (a, b, ...)` over strings)
bool starts_any(const str& s, std::initializer_list<const char*> ps) {
    for (const char* p : ps) {
        if (s.startswith(str(p))) return true;
    }
    return false;
}

// l[:len(l) - len(l.lstrip())]
str lead_ws(const str& s) { return s.slice(0, s.size() - s.lstrip().size()); }

// s.replace(a, b, 1)
str replace_first(const str& s, const str& a, const str& b) {
    int64_t at = s.find(a);
    if (at < 0) return s;
    return s.slice(0, at) + b + s.slice_from(at + a.size());
}

// notes[why].append(where)  (a defaultdict(list))
void note(Notes& notes, const char* why, const str& where) { notes.setdefault(str(why), list<str>()).append(where); }
void note(Notes& notes, const str& why, const str& where) { notes.setdefault(why, list<str>()).append(where); }

// code.count("\n", 0, at)
int64_t nl_before(const str& code, int64_t at) { return code.slice(0, at).count(S("\n")); }

using Edit = std::tuple<int64_t, int64_t, str>;

// edits applied last to first: `for a, b, new in sorted(edits, reverse=True)`
str apply_edits_desc(str text, std::vector<Edit> edits) {
    std::sort(edits.begin(), edits.end(), [](const Edit& x, const Edit& y) { return y < x; });
    for (const auto& e : edits) text = text.slice(0, std::get<0>(e)) + std::get<2>(e) + text.slice_from(std::get<1>(e));
    return text;
}

std::optional<Match> re_search(const char* p, const str& s) { return py::re::search(str(p), s); }
std::optional<Match> re_match(const char* p, const str& s) { return py::re::match(str(p), s); }
std::optional<Match> re_full(const char* p, const str& s) { return py::re::fullmatch(str(p), s); }

str pct(const str& fmt, const str& a) { return py::percent(fmt, a); }

// the `{` after a position: the Python's brace walk `d += {"{": 1, "}": -1}.get(c, 0)`; -> index of the match
int64_t brace_end(const str& code, int64_t from_open) {
    int64_t d = 0, k = from_open;
    while (k < code.size()) {
        char32_t c = code.at_(k);
        d += (c == '{') - (c == '}');
        if (d == 0) break;
        k++;
    }
    return k;
}

}  // namespace

// ---------------------------------------------------------------- Port.decl_rules

str apply_decl_rule(const DeclRule& r, const str& text) {
    if (r.fn.has_value()) {
        const split_decl& f = *r.fn;
        return py::re::sub(r.pattern, [&](const Match& m) { return f(m); }, text);
    }
    return py::re::sub(r.pattern, r.repl, text);
}

str retyped_key(const str& file, const str& function, const str& variable) {
    return file + S("\x01") + function + S("\x01") + variable;
}

// ---------------------------------------------------------------- call_at, ctype

std::optional<CallAt> call_at(const str& line, int64_t col) {
    std::optional<CallAt> best;
    for (const Match& m : py::re::finditer(S(R"RX(\b([A-Za-z_]\w*)\s*\()RX"), line)) {
        if (KEYWORDS.contains(m.group(1))) continue;
        std::optional<list<Span>> spans = call_args(line.slice_from(m.start()), m.group(1), true);
        if (!spans.has_value() || spans->empty()) continue;
        int64_t k = 0;
        for (const Span& sp : *spans) {
            int64_t a = std::get<0>(sp) + m.start(), b = std::get<1>(sp) + m.start();
            if (a <= col && col <= b) {
                if (!best.has_value() || b - a < std::get<1>(std::get<2>(*best)) - std::get<0>(std::get<2>(*best)))
                    best = CallAt{m.group(1), k, Span{a, b}};
            }
            k++;
        }
    }
    return best;
}

str ctype(const str& t_in, const set<str>& tags) {
    str t = t_in.replace(S("__cdecl "), S("")).replace(S("__cdecl"), S(""));
    t = py::re::sub(S(R"RX(\s*\(aka '[^']*'\))RX"), S(""), t);
    t = py::re::sub(S(R"RX((?<!struct )(?<!union )\b(\w+)\b)RX"),
                    [&](const Match& m) { return tags.contains(m.group(1)) ? S("struct ") + m.group(1) : m.group(1); },
                    t);
    return py::re::sub(S(R"RX(\s+)RX"), S(" "), t).strip();
}

// ---------------------------------------------------------------- block_heads

str block_heads(str t, Notes& notes, const str& fname, const set<str>& ptr2) {
    str code = blank(t);
    std::vector<Edit> edits;
    set<int64_t> heads;
    str ref = S(R"RX((?:\((?:[^()]|\([^()]*\))*\)|[\w.\->]+))RX");
    for (const Match& m : py::re::finditer(S(R"RX(\(\s*char\s*\*\s*\)\s*()RX") + ref + S(R"RX()\s*\+\s*(\d+)\b)RX"), code)) {
        str opd = m.group(1);
        int64_t n = py::int_parse(m.group(2));
        list<str> last = py::re::findall(S(R"RX(\w+)RX"), py::re::sub(S(R"RX(\(\s*char\s*\*\s*\*\s*\))RX"), S(""), opd));
        bool ok = py::re::sub(S(R"RX(\s+)RX"), S(" "), opd).replace(S("( char"), S("(char")).contains(S("char **")) ||
                  (!last.empty() && ptr2.contains(last[-1])) ||
                  py::re::search(S(R"RX(\(\s*char\s*\*\s*\*\s*\))RX"), opd).has_value();
        if (n < 2 || n % 2 || !ok) continue;
        edits.push_back({m.start(2), m.end(2), py::percent(S("DOS_HEAD(%d)"), n)});
        heads.add(n);
    }
    for (const Match& m : py::re::finditer(S(R"RX(\{([^{}]*)\})RX"), code)) {   // ({ (char **)g, 1016, g + 4, ... }: a pool's first block)
        str row = m.group(1);
        for (const Match& c : py::re::finditer(S(R"RX(\(\s*char\s*\*\s*\*\s*\)\s*(\w+))RX"), row)) {
            for (const Match& x : py::re::finditer(S(R"RX(\b)RX") + py::re::escape(c.group(1)) + S(R"RX(\s*\+\s*(\d+)\b)RX"), row)) {
                int64_t n = py::int_parse(x.group(1));
                if (n >= 2 && n % 2 == 0) {
                    edits.push_back({m.start(1) + x.start(1), m.start(1) + x.end(1), py::percent(S("DOS_HEAD(%d)"), n)});
                    heads.add(n);
                }
            }
        }
    }
    for (int64_t n : heads) {                               // (and the blocks given: their heads as long)
        for (const Match& m : py::re::finditer(
                 py::percent(S(R"RX(\b(?:malloc|calloc|alloc|getmem|sbrk)\s*\(([^;()]*\S)\s*\+\s*(%d)\s*\))RX"), n), code))
            edits.push_back({m.start(2), m.end(2), py::percent(S("DOS_HEAD(%d)"), n)});
    }
    std::sort(edits.begin(), edits.end(), [](const Edit& x, const Edit& y) { return y < x; });
    set<int64_t> seen;
    int64_t out = 0;
    for (const auto& e : edits) {
        if (seen.contains(std::get<0>(e))) continue;
        seen.add(std::get<0>(e));
        t = t.slice(0, std::get<0>(e)) + std::get<2>(e) + t.slice_from(std::get<1>(e));
        out++;
    }
    if (out) {
        note(notes,
             "a block chained by its first word, its bytes after a head of the old size: the head DOS_HEAD "
             "(the link a pointer of today's)",
             py::percent(S("%s (%d)"), fname, out));
    }
    return t;
}

// ---------------------------------------------------------------- whole_word_stores

str whole_word_stores(const str& t, Notes& notes, const str& fname) {
    list<str> lines = py::split(t, S("\n")), code = py::split(blank(t), S("\n"));
    for (const DefinitionAny& d : definitions_any(lines, code)) {
        int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
        str body = py::join(S("\n"), code.slice(s_, k_ + 1));
        set<str> wide = set<str>::from(py::re::findall(S(R"RX(\bintptr_t\s+(\w+)\s*[,)=])RX"), body));
        for (const str& dl : py::re::findall(S(R"RX(\bintptr_t\s+([^;(){}]*);)RX"), body)) {   // (intptr_t n, v, pool;)
            for (const str& x : py::split(dl, S(",")))
                wide.add(py::re::match(S(R"RX(\s*\**\s*(\w*))RX"), x)->group(1));
        }
        if (wide.empty()) continue;
        py::re::Pattern rx(S(R"RX(\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*=\s*(\w+)\s*;)RX"), 0);
        for (int64_t x = j_; x <= k_; x++) {
            std::optional<Match> m = rx.search(code[x]);
            if (m && wide.contains(m->group(2))) {
                lines[x] = lines[x].slice(0, m->start()) + S("*(intptr_t *)") + m->group(1) + S(" = ") + m->group(2) +
                           S(";") + lines[x].slice_from(m->end());
                note(notes,
                     "a word stored through (int *) of a value that may be a pointer (intptr_t): stored whole",
                     py::percent(S("%s:%d"), fname, x + 1));
            }
        }
    }
    return py::join(S("\n"), lines);
}

// ---------------------------------------------------------------- file_internals, stdio_inits

str file_internals(str t, Notes& notes, const str& fname) {
    str code = blank(t);
    std::vector<Edit> edits;
    for (const Match& m : py::re::finditer(
             S(R"RX(([A-Za-z_][\w.]*(?:->\w+|\[[^\]]*\])*)\s*->\s*_(file|flag)\b(?!\s*(?:[-+*/|&^]?=(?!=)|\+\+|--)))RX"), code))
        edits.push_back({m.start(), m.end(), py::percent(m.group(2) == "file" ? S("fileno(%s)") : S("dos_fflag(%s)"), m.group(1))});
    std::reverse(edits.begin(), edits.end());
    for (const auto& e : edits) t = t.slice(0, std::get<0>(e)) + std::get<2>(e) + t.slice_from(std::get<1>(e));
    if (!edits.empty()) {
        note(notes,
             "the old library's FILE read inside (today's keeps it to itself): fileno, its end-of-file and "
             "error flags (dos_fflag)",
             py::percent(S("%s (%d)"), fname, static_cast<int64_t>(edits.size())));
    }
    return t;
}

str stdio_inits(str t, Notes& notes, const str& fname) {
    if (!fname.endswith(S(".c"))) return t;
    std::vector<std::tuple<str, str>> sets;
    t = py::re::sub(S(R"RX((?m)^((?:static\s+)?)(FILE\s*\*\s*)(\w+)\s*=\s*(stdin|stdout|stderr)\s*;(.*)$)RX"),
                    [&](const Match& m) {
                        sets.emplace_back(m.group(3), m.group(4));
                        note(notes,
                             "a variable given stdin, stdout or stderr where it is defined (not a constant now): set before "
                             "main",
                             py::percent(S("%s (%s)"), fname, m.group(3)));
                        return py::percent(S("%sFILE *%s;%s"), m.group(1), m.group(3), m.group(5));
                    },
                    t);
    for (const auto& vw : sets) {
        t = t.rstrip(S("\n")) + py::percent(S("\nDOS_INIT(dos_init_%s) { %s = %s; }\n"), std::get<0>(vw), std::get<0>(vw),
                                            std::get<1>(vw));
    }
    return t;
}

// ---------------------------------------------------------------- word_copies, word_unions

str word_copies(str t, Notes& notes, const str& fname, const set<str>& copied) {
    if (copied.empty()) return t;
    str code = blank(t);
    std::vector<Edit> edits;
    for (const Match& um : py::re::finditer(S(R"RX(\bunion\s*\w*\s*\{)RX"), code)) {
        int64_t k = brace_end(code, um.end() - 1);
        str body = code.slice(um.end(), k);
        str flat = py::re::sub(S(R"RX(\b(?:struct|union)\s*\w*\s*\{[^{}]*\})RX"),
                               [](const Match& x) { return py::repeat(S(" "), x.group(0).size()); }, body);
        if (!py::re::search(S(R"RX(\*\s*\w+\s*;)RX"), flat)) continue;   // (no pointer in it: a number is a number)
        for (const Match& st : py::re::finditer(S(R"RX((?m)^([ \t]*)(?:u?int16_t|short|int|unsigned)\s+(\w+)\s*;)RX"), flat)) {
            if (copied.contains(st.group(2))) {
                int64_t a = um.end() + st.start(), b = um.end() + st.end();
                edits.push_back({a, b, py::percent(S("%sintptr_t %s;"), st.group(1), st.group(2))});
                note(notes,
                     "an int of a union of pointers, copied from one to another (the word it was: a pointer "
                     "too): intptr_t",
                     py::percent(S("%s (%s)"), fname, st.group(2)));
            }
        }
    }
    return apply_edits_desc(t, edits);
}

str word_unions(const str& t, Notes& notes, const str& fname, const set<str>& olds) {
    return py::re::sub(S(R"RX(\bunion(\s+\w+)?\s*\{([^{}]*)\})RX"),
                       [&](const Match& m) {
                           str body = m.group(2);
                           list<str> decls, ptrs, ints;
                           for (const str& d : py::split(body, S(";"))) {
                               if (!d.strip().empty()) decls.append(d.strip());
                           }
                           for (const str& d : decls) {
                               if (d.contains(S("*"))) ptrs.append(d);
                           }
                           for (const str& d : decls) {
                               if (py::re::fullmatch(S(R"RX((?:u?int16_t|short|int|unsigned)\s+\w+)RX"), d)) ints.append(d);
                           }
                           std::optional<str> g1 = m.group_opt(1);
                           if (ptrs.empty() || ints.size() != 1 || ints.size() + ptrs.size() != decls.size() ||
                               olds.contains(g1.value_or(S("")).strip()))
                               return m.group(0);
                           str nm = py::split(ints[0])[-1];
                           note(notes, "a union of a number and pointers (one 16-bit word then): the number intptr_t",
                                py::percent(S("%s (%s)"), fname, nm));
                           return replace_first(m.group(0), ints[0], S("intptr_t ") + nm);
                       },
                       t);
}

// ---------------------------------------------------------------- old_c_ways

void old_c_ways(py::dict<str, str>& out, Notes& notes) {
    set<str> held;                                          // (*(int16_t *)&p->size = dos_ph(..): a handle kept in
    for (const str& t : out.values()) {                     //  the word over two bytes - read as a pointer: what it
        for (const Match& m : py::re::finditer(
                 S(R"RX(\*\s*\(\s*int16_t\s*\*\s*\)\s*&\s*[^=;]*?->\s*(\w+)\s*=\s*dos_ph\s*\()RX"), blank(t)))
            held.add(m.group(1));                           //  stands for)
    }
    if (!held.empty()) {
        list<str> esc;
        for (const str& h : py::sorted(held)) esc.append(py::re::escape(h));
        py::re::Pattern hx(S(R"RX(\(\s*((?:struct|union)\s+\w+\s*\*|char\s*\*)\s*\)\s*\(\s*(\*\s*\(\s*int16_t\s*\*\s*\)\s*&)RX") +
                               S(R"RX(\s*[\w>\-.\s()*]*?->\s*(?:)RX") + py::join(S("|"), esc) + S(R"RX())\s*\))RX"),
                           0);
        for (const auto& kv : out.items()) {
            const str& n = std::get<0>(kv);
            const str& t = std::get<1>(kv);
            if (!t.empty() && n.endswith(S(".c"))) {
                str t2 = sub_code(t, hx, [&](const Match& m) {
                    return py::percent(S("(%s)dos_hp(%s)"), m.group(1), t.slice(m.start(2), m.end(2)));
                });
                if (t2 != t) {
                    out[n] = t2;
                    note(notes, "a handle kept in a word over two bytes, read as a pointer: what it stands for (dos_hp)", n);
                }
            }
        }
    }
    py::re::Pattern sx(S(R"RX((?m)^((?:static\s+)?(?:unsigned\s+)?char\s*\*\s*\w+\s*=\s*)("(?:[^"\\\n]|\\.)*")\s*;)RX"), 0);
    py::re::Pattern tx(S(R"RX(\btime\s*\(\s*(?:\(\s*(?:time_t|long|int32_t)\s*\*\s*\)\s*)?&\s*(\w+)\s*\))RX"), 0);
    for (const auto& kv : out.items()) {
        const str& n = std::get<0>(kv);
        str t = std::get<1>(kv);
        if (t.empty() || !n.endswith(S(".c"))) continue;
        str code = blank(t);
        list<int64_t> ds = depths(py::split(code, S("\n")));
        std::vector<Edit> edits;
        for (const Match& m : sx.finditer(code)) {
            if (ds[nl_before(code, m.start())] == 0)
                edits.push_back({m.start(2), m.end(2), S("(char []){") + t.slice(m.start(2), m.end(2)) + S("}")});
        }
        for (const Match& m : tx.finditer(code)) {
            if (py::re::search(S(R"RX(\b(?:long|int32_t|uint32_t)\s+(?:\w+\s*,\s*)*)RX") + py::re::escape(m.group(1)) + S(R"RX(\b)RX"),
                               code))
                edits.push_back({m.start(), m.end(), S("(") + m.group(1) + S(" = (int32_t)time(NULL))")});
        }
        t = apply_edits_desc(t, edits);
        if (!edits.empty()) {
            out[n] = t;
            note(notes,
                 "what old C allowed that today's does not: a string's text written to (an array of its own), "
                 "time() into a long (today's time_t 8 bytes)",
                 py::percent(S("%s (%d)"), n, static_cast<int64_t>(edits.size())));
        }
    }
}

// ---------------------------------------------------------------- numbered_places

void numbered_places(py::dict<str, str>& out, const list<str>& extra, Notes& notes) {
    py::dict<str, str> decls;                               // (name: its declaration, as the definition says it)
    list<str> texts = list<str>::from(out.values());
    texts.append(py::join(S("\n"), extra));
    for (const str& t : texts) {
        str code = blank(t);
        list<str> cl = py::split(code, S("\n"));
        list<int64_t> ds = depths(cl);
        int64_t i = 0;
        for (const str& l : cl) {
            std::optional<Match> m = py::re::match(
                S(R"RX(\s*(?:static\s+)?((?:(?:struct|union)\s+\w+|(?:unsigned\s+)?\w+)\s*\**\s*(g_[0-9a-f]{4}))RX") +
                    S(R"RX(\s*(?:\[[^\]]*\])?)\s*(?:=|;))RX"),
                l);
            if (m && ds[i] == 0 && !starts_any(l.lstrip(), {"extern", "typedef", "#"}))
                decls.setdefault(m->group(2), py::re::sub(S(R"RX(\s+)RX"), S(" "), m->group(1)).strip());
            i++;
        }
    }
    if (decls.empty()) return;
    str cast = S(R"RX(\(\s*(?:(?:struct|union)\s+\w+|\w+)\s*\*+\s*\)\s*)RX");
    for (const auto& kv : out.items()) {
        const str& n = std::get<0>(kv);
        str t = std::get<1>(kv);
        if (!n.endswith(S(".c")) || t.empty()) continue;
        str code = blank(t);
        std::vector<Edit> edits;
        set<str> used;
        for (const Match& m : py::re::finditer(cast + S(R"RX((\((?:[^()]*)\)|0x[0-9a-fA-F]{4}\b))RX"), code)) {
            int64_t seg_at = m.start(1);
            for (const Match& h : py::re::finditer(S(R"RX(\b0x([0-9a-fA-F]{4})\b)RX"), m.group(1))) {
                str nm = S("g_") + h.group(1).lower();
                if (decls.contains(nm)) {
                    edits.push_back({seg_at + h.start(), seg_at + h.end(), nm});
                    used.add(nm);
                }
            }
        }
        if (edits.empty()) continue;
        t = apply_edits_desc(t, edits);
        list<str> need;
        for (const str& nm : py::sorted(used)) {
            if (!py::re::search(S(R"RX((?m)^[^\n(]*\b)RX") + nm + S(R"RX(\b[^\n(]*;)RX"), blank(t))) need.append(nm);
        }
        if (!need.empty()) {
            list<str> lines = py::split(t, S("\n"));
            int64_t at = -1;
            int64_t i = 0;
            for (const str& l : lines) {
                if (l.startswith(S("#include"))) at = std::max(at, i);
                i++;
            }
            at += 1;
            int64_t off = 0;
            for (const str& nm : need) lines.insert(at + off++, S("extern ") + decls.at(nm) + S(";"));
            t = py::join(S("\n"), lines);
        }
        out[n] = t;
        note(notes, "a variable's place given as its number (the old data segment's): the variable",
             py::percent(S("%s (%s)"), n, py::join(S(", "), py::sorted(used))));
    }
}

// ---------------------------------------------------------------- agree_wide

void agree_wide(py::dict<str, str>& out, const list<str>& extra, Notes& notes) {
    set<str> wide;
    bool more = rule(S("agree_wide_defs"));                 // (definitions and tables' places too)
    list<str> texts = list<str>::from(out.values());
    texts.append(py::join(S("\n"), extra));
    for (const str& t : texts) {
        str code = blank(t);
        list<str> cl = py::split(code, S("\n"));
        list<int64_t> ds = depths(cl);
        int64_t i = 0;
        for (const str& l : cl) {
            std::optional<Match> m = py::re::match(S(R"RX(\s*(?:extern\s+|static\s+)?intptr_t\s+([^;()]*);)RX"), l);
            if (ds[i] == 0 && m) {
                for (const str& d : py::split(m->group(1), S(","))) {
                    if (py::re::fullmatch(S(R"RX(\s*\w+\s*)RX"), d)) wide.add(d.strip());
                }
            }
            i++;
        }
        if (more) {
            for (const str& w : py::re::findall(S(R"RX(\(\s*intptr_t\s*\*\s*\)\s*&\s*(\w+)\b(?!\s*(?:\[|\.|->)))RX"), code))
                wide.add(w);                                //  (a table's { "-FPc", (int *)&g_0cbc }: the place a word of
        }                                                   //  any row's width is written to - the variable that)
    }
    if (wide.empty()) return;
    py::re::Pattern rx(S(R"RX((?m)^([ \t]*(?:extern\s+)?)((?:u?int16_t|int|short|unsigned))\s+([\w\s,]+);)RX"), 0);
    py::re::Pattern rd(S(R"RX((?m)^([ \t]*(?:static\s+)?)((?:u?int16_t|int|short|unsigned))\s+(\w+)(\s*=[^;{}]*;))RX"), 0);
    py::re::Pattern rn(S(R"RX((?m)^([ \t]*(?:static\s+)?)((?:u?int16_t|int|short|unsigned))\s+(\w+)\s*;)RX"), 0);
    py::re::Pattern ra(S(R"RX(([{,]\s*)&\s*(\w+)\s*(?=[,}]))RX"), 0);
    for (const auto& kv : out.items()) {
        const str& n = std::get<0>(kv);
        str t = std::get<1>(kv);
        if (t.empty()) continue;
        str code = blank(t);
        list<str> cl = py::split(code, S("\n"));
        list<int64_t> ds = depths(cl);
        std::set<Edit> edits;
        for (const Match& m : rx.finditer(code)) {
            int64_t line = nl_before(code, m.start());
            if (ds[line] || !m.group(1).contains(S("extern"))) continue;
            list<str> names, hit, rest;
            for (const str& w : py::split(m.group(3), S(","))) names.append(w.strip());
            for (const str& w : names) (wide.contains(w) ? hit : rest).append(w);
            if (hit.empty()) continue;
            str a = !rest.empty() ? py::percent(S("%s%s %s; "), m.group(1), m.group(2), py::join(S(", "), rest)) : m.group(1);
            str b = rest.empty() ? py::percent(S("intptr_t %s;"), py::join(S(", "), hit))
                                 : py::percent(S("extern intptr_t %s;"), py::join(S(", "), hit));
            edits.insert({m.start(), m.end(), a + b});
        }
        if (more) {
            for (const Match& m : rd.finditer(code)) {      // (its definition: int16_t g_0cbc = 0;)
                int64_t line = nl_before(code, m.start());
                if (ds[line] == 0 && wide.contains(m.group(3))) edits.insert({m.start(2), m.end(2), S("intptr_t")});
            }
            for (const Match& m : rn.finditer(code)) {      // (or without a value: int16_t g_0cbc;)
                int64_t line = nl_before(code, m.start());
                if (ds[line] == 0 && wide.contains(m.group(3)) && !m.group(1).contains(S("extern")))
                    edits.insert({m.start(2), m.end(2), S("intptr_t")});
            }
            for (const Match& m : ra.finditer(code)) {      // (and given as the place of a 16-bit one too - { 'a', 1, &g_0c92 }
                int64_t line = nl_before(code, m.start());  //  - a file's table's, not a call's)
                std::optional<int64_t> top;
                for (int64_t j = line; j >= 0; j--) {
                    if (ds[j] == 0) { top = j; break; }
                }
                if (wide.contains(m.group(2)) && top.has_value() &&
                    py::re::search(S(R"RX(=\s*\{)RX"), cl[*top]))
                    edits.insert({m.start() + m.group(1).size(), m.start() + m.group(1).size(), S("(void *)")});
            }
        }
        for (auto it = edits.rbegin(); it != edits.rend(); ++it)
            t = t.slice(0, std::get<0>(*it)) + std::get<2>(*it) + t.slice_from(std::get<1>(*it));
        if (!edits.empty()) {
            out[n] = t;
            note(notes,
                 "a variable intptr_t in some files (it holds a pointer), a number of 16 bits in another: intptr_t in all",
                 n);
        }
    }
}

// ---------------------------------------------------------------- widen_member, stored_wide, passed_on

std::optional<str> widen_member(const str& text, const str& tag, const str& mem) {
    str code = blank(text);
    std::optional<Match> m = py::re::search(S(R"RX(\bstruct\s+)RX") + py::re::escape(tag) + S(R"RX(\s*\{)RX"), code);
    if (!m) return std::nullopt;
    int64_t end = code.find(S("}"), m->end());
    if (end < 0) return std::nullopt;
    for (const Match& d : py::re::finditer(S(R"RX((?m)\b((?:u?int16_t|int|short|unsigned))\s+([\w\s,]+?)\s*;)RX"),
                                           code.slice(m->end(), end))) {
        list<str> names;
        for (const str& x : py::split(d.group(2), S(","))) names.append(x.strip());
        bool plain = true;
        for (const str& x : names) plain = plain && py::re::fullmatch(S(R"RX(\w+)RX"), x).has_value();
        if (!names.contains(mem) || !plain) continue;
        int64_t a = m->end() + d.start(), b = m->end() + d.end();
        if (text.slice(b, text.find(S("\n"), b)).contains(S("(a handle"))) return std::nullopt;
        int64_t k = names.index(mem);                       // (in their order: an initializer gives them so)
        list<str> parts;
        if (k) parts.append(py::percent(S("%s %s;"), d.group(1), py::join(S(", "), names.slice(0, k))));
        parts.append(S("intptr_t ") + mem + S(";"));
        if (k + 1 < names.size())
            parts.append(py::percent(S("%s %s;"), d.group(1), py::join(S(", "), names.slice_from(k + 1))));
        return text.slice(0, a) + py::join(S(" "), parts) + text.slice_from(b);
    }
    return std::nullopt;
}

bool stored_wide(Port& port, py::dict<str, str>& out) {
    set<std::tuple<str, str>> wide;                         // ((tag, member)) given an intptr_t parameter
    for (const auto& kv : out.items()) {
        const str& f = std::get<0>(kv);
        const str& t = std::get<1>(kv);
        if (!f.endswith(S(".c"))) continue;
        list<str> lines = py::split(t, S("\n")), code = py::split(blank(t), S("\n"));
        for (const DefinitionAny& d : definitions_any(lines, code)) {
            const str& name = std::get<0>(d);
            int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
            FnP me = port.fns.get(name);
            if (!me) continue;
            str head = py::join(S(" "), code.slice(s_, j_ + 1));   // (the definition's own say, as the port has it now)
            list<str> ps;
            for (const auto& p : me->params) {
                if (py::re::fullmatch(S(R"RX(intptr_t\s+\w+)RX"), std::get<1>(p).strip())) ps.append(std::get<0>(p));
            }
            ps.extend(py::re::findall(S(R"RX(\bintptr_t\s+(\w+)\s*[,);])RX"), head));
            str body = py::join(S("\n"), code.slice(j_, k_ + 1));
            for (const str& pn : ps) {
                for (const Match& m : py::re::finditer(S(R"RX(\b(\w+)\s*->\s*(\w+)\s*=\s*)RX") + py::re::escape(pn) + S(R"RX(\s*;)RX"), body)) {
                    set<str> tags = set<str>::from(py::re::findall(
                        S(R"RX(\bstruct\s+(\w+)\s*\*\s*)RX") + py::re::escape(m.group(1)) + S(R"RX(\s*[;,=)])RX"), blank(t)));
                    if (tags.size() == 1) wide.add({*tags.begin(), m.group(2)});
                }
            }
        }
        if (rule(S("stored_wide_cast"))) {
            for (const Match& m : py::re::finditer(S(R"RX((?<![\w.>])(\w+)\s*->\s*(\w+)\s*=\s*\(\s*intptr_t\s*\))RX"), blank(t))) {   // (p->s6 =
                set<str> tags = set<str>::from(py::re::findall(                          // (int)name: a pointer the old code kept in a member - its width)
                    S(R"RX(\bstruct\s+(\w+)\s*\*\s*(?:\w+\s*,\s*\*?\s*)*)RX") + py::re::escape(m.group(1)) + S(R"RX(\s*[;,=)])RX"),
                    blank(t)));
                if (tags.size() == 1) wide.add({*tags.begin(), m.group(2)});
            }
        }
    }
    bool changed = false;
    set<str> done;
    std::vector<std::tuple<str, str>> wide_sorted(wide.begin(), wide.end());
    std::sort(wide_sorted.begin(), wide_sorted.end());
    for (const auto& tm : wide_sorted) {                   // (in the port's own text: each round is made from it)
        const str& tag = std::get<0>(tm);
        const str& mem = std::get<1>(tm);
        for (const auto& kv : port.files.items()) {
            const str& f = std::get<0>(kv);
            const str& t = std::get<1>(kv);
            if (t.empty()) continue;
            std::optional<str> new_ = widen_member(t, tag, mem);
            if (new_) {
                port.files[f] = *new_;
                if (out.contains(f) && !out.at(f).empty()) {
                    std::optional<str> new2 = widen_member(out.at(f), tag, mem);
                    if (new2) out[f] = *new2;
                }
                note(port.mended,
                     "a member given an intptr_t parameter (the pointer it carried kept there): intptr_t",
                     py::percent(S("%s (struct %s: %s)"), f, tag, mem));
                changed = true;
                done.add(mem);
            }
        }
    }
    set<str> mems;
    for (const auto& tm : wide) mems.add(std::get<1>(tm));
    for (const auto& kv : out.items()) {                   // (and taken out again: v = s->v; return v;)
        const str& f = std::get<0>(kv);
        if (!f.endswith(S(".c")) || mems.empty()) continue;
        list<str> lines = py::split(std::get<1>(kv), S("\n")), code = py::split(blank(std::get<1>(kv)), S("\n"));
        std::vector<DefinitionAny> defs;
        for (const DefinitionAny& d : definitions_any(lines, code)) defs.push_back(d);
        std::stable_sort(defs.begin(), defs.end(),
                         [](const DefinitionAny& a, const DefinitionAny& b) { return std::get<1>(a) > std::get<1>(b); });
        for (const DefinitionAny& d : defs) {
            const str& name = std::get<0>(d);
            int64_t s_ = std::get<1>(d), j_ = std::get<2>(d), k_ = std::get<3>(d);
            FnP me = port.fns.get(name);
            if (!me) continue;
            str rt = (me->ret.empty() ? S("int") : me->ret).strip();
            if (!(rt == "int" || rt == "int16_t" || rt == "short" || rt == "unsigned" || rt == "uint16_t")) continue;
            str body = py::join(S("\n"), code.slice(j_, k_ + 1));
            set<str> rets = set<str>::from(py::re::findall(S(R"RX(\breturn\s+(\w+)\s*;)RX"), body));
            if (rets.size() != 1 || py::re::search(S(R"RX(\breturn\s+[^\w\s;]|\breturn\s+\w+\s*[^\w\s;])RX"), body)) continue;
            str v = *rets.begin();
            list<str> esc;
            for (const str& m_ : mems) esc.append(py::re::escape(m_));
            if (!py::re::search(S(R"RX(\b)RX") + py::re::escape(v) + S(R"RX(\s*=\s*\w+\s*->\s*(?:)RX") + py::join(S("|"), esc) + S(R"RX()\s*;)RX"),
                                body))
                continue;
            for (int64_t x = j_; x <= k_; x++) {
                str nw = py::re::sub(S(R"RX(^(\s*)(?:u?int16_t|int|short|unsigned)(\s+)RX") + py::re::escape(v) + S(R"RX(\s*;))RX"),
                                     S(R"RX(\1intptr_t\2)RX"), lines[x]);
                if (nw != lines[x]) lines[x] = nw;
            }
            me->ret = S("intptr_t");
            me->ret_set = true;
            port.local_retypes[std::make_tuple(f, name, v)] = S("intptr_t");
            for (int64_t x = s_; x <= j_; x++) {
                lines[x] = py::re::sub(S(R"RX(^(\s*(?:static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+)RX") + py::re::escape(name) + S(R"RX(\s*\())RX"),
                                       S(R"RX(\1intptr_t\2)RX"), lines[x]);
            }
            out[f] = py::join(S("\n"), lines);
            for (const auto& gk : out.items()) {           // (its declarations)
                const str& g = std::get<0>(gk);
                const str& tg = std::get<1>(gk);
                if (!tg.empty() && g != f) {
                    str ng = py::re::sub(S(R"RX((?m)^(\s*(?:extern\s+|static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+)RX") +
                                             py::re::escape(name) + S(R"RX(\s*\())RX"),
                                         S(R"RX(\1intptr_t\2)RX"), tg);
                    if (ng != tg) out[g] = ng;
                }
            }
            lines = py::split(out.at(f), S("\n"));
            note(port.mended, "a function giving back what a member kept of an intptr_t parameter: intptr_t",
                 py::percent(S("%s (%s: %s)"), f, name, v));
            changed = true;
        }
    }
    return changed;
}

bool passed_on(Port& port, py::dict<str, str>& out) {
    py::dict<str, set<int64_t>> wide;
    for (const auto& kv : port.fns.items()) {
        set<int64_t> ks;
        int64_t k = 0;
        for (const auto& p : std::get<1>(kv)->params) {
            if (py::re::fullmatch(S(R"RX(intptr_t\s+\w+)RX"), std::get<1>(p).strip())) ks.add(k);
            k++;
        }
        if (!ks.empty()) wide.set(std::get<0>(kv), ks);
    }
    if (wide.empty()) return false;
    list<str> esc;
    for (const str& n : py::sorted(list<str>::from(wide.keys()))) esc.append(py::re::escape(n));
    py::re::Pattern pat(S(R"RX(\b()RX") + py::join(S("|"), esc) + S(R"RX()\s*\()RX"), 0);
    bool changed = false;
    for (const auto& kv : out.items()) {
        const str& f = std::get<0>(kv);
        const str& t = std::get<1>(kv);
        if (!f.endswith(S(".c"))) continue;
        list<str> lines = py::split(t, S("\n")), code = py::split(blank(t), S("\n"));
        for (const DefinitionAny& d : definitions_any(lines, code)) {
            const str& name = std::get<0>(d);
            int64_t j_ = std::get<2>(d), k_ = std::get<3>(d);
            FnP me = port.fns.get(name);
            if (!me) continue;
            py::dict<str, int64_t> mine;
            int64_t i = 0;
            for (const auto& p : me->params) {
                const str& pn = std::get<0>(p);
                if (py::re::fullmatch(S(R"RX((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+)RX") + py::re::escape(pn),
                                      std::get<1>(p).strip()))
                    mine.set(pn, i);
                i++;
            }
            if (mine.empty()) continue;
            for (int64_t x = j_; x <= k_; x++) {
                for (const Match& m : pat.finditer(code[x])) {
                    std::optional<list<Span>> sp = call_args(code[x].slice_from(m.start()), m.group(1));
                    list<Span> spans = sp ? *sp : list<Span>();
                    int64_t k = 0;
                    for (const Span& s : spans) {
                        str arg = code[x].slice(m.start() + std::get<0>(s), m.start() + std::get<1>(s)).strip();
                        if (wide.at(m.group(1)).contains(k) && mine.contains(arg)) {
                            int64_t pi = mine.pop(arg);
                            me->params[pi] = std::make_tuple(arg, S("intptr_t ") + arg);
                            port.param_types[std::make_tuple(me->name, pi)] = std::get<1>(me->params[pi]);
                            note(port.mended,
                                 "integer parameter given on to one that holds a pointer (it carried one): intptr_t",
                                 py::percent(S("%s:%d (%s, %s)"), f, x + 1, me->name, arg));
                            changed = true;
                        }
                        k++;
                    }
                }
            }
        }
    }
    return changed;
}

// ---------------------------------------------------------------- union_member_for, visible_tags

std::optional<str> union_member_for(const py::dict<str, str>& out, const str& union_, const str& typ) {
    str tag = py::split(union_)[-1];
    std::optional<str> body;
    for (const str& t : out.values()) {
        std::optional<Match> m = py::re::search(S(R"RX(\bunion\s+)RX") + py::re::escape(tag) + S(R"RX(\s*\{)RX"), blank(t));
        if (m) {
            int64_t d = 1, k = m->end();
            str c = blank(t);
            while (k < c.size() && d) {
                char32_t ch = c.at_(k);
                d += (ch == '{') - (ch == '}');
                k++;
            }
            body = c.slice(m->end(), k - 1);
            break;
        }
    }
    if (!body) return std::nullopt;
    str flat = py::re::sub(S(R"RX(\{[^{}]*\})RX"), S(""), *body);   // (its own members: not those of a structure in it)
    std::vector<std::tuple<str, str>> mems;
    for (const str& decl : py::split(flat, S(";"))) {
        auto r = decl.strip().empty() ? decltype(declarators(decl))() : declarators(decl.strip());
        if (r) {
            const auto& ds = std::get<1>(*r);
            const str& base = std::get<0>(*r);
            if (ds.size() == 1 && !base.strip().empty() &&
                (!base.strip().startswith(std::make_tuple("struct", "union")) ||
                 py::re::match(S(R"RX((?:struct|union)\s+\w+$)RX"), base.strip()) ||
                 std::get<1>(ds[0]).contains(S("*")))) {
                const str& nm = std::get<0>(ds[0]);
                const str& dcl = std::get<1>(ds[0]);
                mems.emplace_back(nm, (py::re::sub(S(R"RX(\s+)RX"), S(" "), base).strip() + S(" ") + dcl.replace(nm, S("")))
                                          .replace(S(" "), S("")));
            }
        }
    }
    str t = typ.replace(S(" "), S("")).replace(S("const"), S(""));
    static const std::map<std::string, std::string> kinds = {
        {"long", "int32_t"}, {"longint", "int32_t"}, {"int", "int32_t"}, {"unsignedint", "uint32_t"},
        {"unsigned", "uint32_t"}, {"short", "int16_t"}, {"shortint", "int16_t"}, {"shortunsignedint", "uint16_t"},
        {"unsignedshort", "uint16_t"}, {"float", "double"}, {"unsignedlong", "uint32_t"}};   // (the compiler's words: today's int is 32 bits)
    auto kt = kinds.find(t.utf8());
    if (kt != kinds.end()) t = str(kt->second);
    for (const auto& nm_mt : mems) {
        if (std::get<1>(nm_mt) == t) return std::get<0>(nm_mt);
    }
    auto first_of = [&](std::function<bool(const str&)> pred) -> std::optional<str> {
        for (const auto& nm_mt : mems) {
            if (pred(std::get<1>(nm_mt))) return std::get<0>(nm_mt);
        }
        return std::nullopt;
    };
    if (t.contains(S("*"))) {
        std::optional<str> r = first_of([](const str& mt) { return mt == "char*"; });
        return r ? r : first_of([](const str& mt) { return mt.contains(S("*")); });
    }
    if (t == "double" || t == "float" || t == "longdouble")
        return first_of([](const str& mt) { return mt == "double" || mt == "float"; });
    if (t == "int32_t" || t == "uint32_t" || t == "longlong" || t == "int64_t" || t == "intptr_t")
        return first_of([](const str& mt) { return mt == "int32_t" || mt == "uint32_t"; });
    return first_of([](const str& mt) {
        return mt == "int16_t" || mt == "uint16_t" || mt == "char" || mt == "unsignedchar";
    });
}

set<str> visible_tags(const py::dict<str, str>& out, const str& f) {
    set<str> seen, tags;
    list<str> todo;
    todo.append(f);
    while (!todo.empty()) {
        str n = todo.pop();
        if (seen.contains(n) || !out.contains(n)) continue;
        seen.add(n);
        str t = blank(out.at(n));
        tags.update(py::re::findall(S(R"RX(\b(?:struct|union)\s+(\w+)\s*\{)RX"), t));
        todo.extend(py::re::findall(S(R"RX(#\s*include\s*"([^"]+)")RX"), out.at(n)));
    }
    return tags;
}

// ---------------------------------------------------------------- mend

namespace {

// the complaint's kind, and the named groups of the pattern that said it (the Python's `kind, m = said(text)`,
// `m.groupdict()`)
struct SaidNamed {
    std::optional<str> kind;
    std::optional<Match> m;
    std::vector<std::pair<std::string, int>> names;
    std::optional<str> get(const std::string& k) const {
        if (!m) return std::nullopt;
        for (const auto& nm : names) {
            if (nm.first == k) return m->group_opt(nm.second);
        }
        return std::nullopt;
    }
};

SaidNamed said_named(const str& text) {
    for (const auto& kp : SAYS) {
        for (const str& p : std::get<1>(kp)) {
            py::re::Pattern pat(p, 0);
            std::optional<Match> m = pat.search(text);
            if (m) {
                SaidNamed r;
                r.kind = std::get<0>(kp);
                r.m = m;
                r.names = pat.rx().names();
                return r;
            }
        }
    }
    return SaidNamed();
}

const char* EQ_RX = R"RX((?<![=!<>+\-*/%&|^])=(?!=))RX";

// the matches of `=` (not ==, <=, +=...) on a line, nearest to col first
std::vector<Match> eq_near(const str& l, int64_t col) {
    std::vector<Match> v;
    for (const Match& x : py::re::finditer(str(EQ_RX), l)) v.push_back(x);
    std::stable_sort(v.begin(), v.end(), [&](const Match& a, const Match& b) {
        int64_t da = a.start() > col ? a.start() - col : col - a.start();
        int64_t db = b.start() > col ? b.start() - col : col - b.start();
        return da < db;
    });
    return v;
}

// the end of the value that starts at a: to a `;` or `,` at depth 0 (brackets: ([ or, with braces, ([{)
int64_t value_end(const str& l, int64_t a, bool braces) {
    int64_t b = a, dd = 0;
    while (b < l.size() && !(dd == 0 && (l.at_(b) == ';' || l.at_(b) == ','))) {
        char32_t c = l.at_(b);
        dd += (c == '(' || c == '[' || (braces && c == '{')) - (c == ')' || c == ']' || (braces && c == '}'));
        if (dd < 0) break;
        b++;
    }
    return b;
}

void need_header(Port& port, const str& f, const str& h) {
    list<str> cur = port.need_headers.setdefault(f, list<str>());
    if (!cur.contains(h)) cur.append(h);
}

bool is_int_word(const str& s) {
    return py::re::fullmatch(S(R"RX((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+\w+)RX"), s.strip()).has_value();
}

}  // namespace

bool mend(Port& port, py::dict<str, str>& out, const list<Diag>& diags) {
    std::optional<str> all_;
    auto all_code = [&]() -> const str& {                   // (every file's code, as one text: once a round)
        if (!all_) {
            list<str> parts;
            for (const str& t_ : out.values()) {
                if (!t_.empty()) parts.append(blank(t_));
            }
            all_ = py::join(S("\n"), parts);
        }
        return *all_;
    };
    py::dict<str, set<str>> vis;
    auto visible = [&](const str& f_) -> set<str> {         // (the tags a file sees: once a file a round)
        if (!vis.contains(f_)) vis.set(f_, visible_tags(out, f_));
        return vis.at(f_);
    };
    bool changed_ = passed_on(port, out);
    if (stored_wide(port, out)) changed_ = true;
    Notes& notes = port.mended;
    py::dict<str, list<str>> mem = members(list<str>::from(out.values()));
    set<str> tags;
    for (const auto& kv : mem) {
        (void)kv;
    }
    for (const str& k : mem.keys()) {
        for (const str& s : mem.at(k)) tags.add(py::split(s)[1]);
    }
    bool changed = false;
    py::dict<std::tuple<str, int64_t>, list<Diag>> by_line;
    for (const Diag& d : diags) by_line.setdefault(std::make_tuple(d.file, d.line), list<Diag>()).append(d);
    for (const auto& blk : by_line.items()) {
        const str& f = std::get<0>(std::get<0>(blk));
        int64_t ln = std::get<1>(std::get<0>(blk));
        std::vector<Diag> ds;
        for (const Diag& dd_ : std::get<1>(blk)) ds.push_back(dd_);
        if (!out.contains(f)) continue;
        list<str> lines = py::split(out.at(f), S("\n"));
        if (ln > lines.size()) continue;
        str l = lines[ln - 1];
        str orig = l;
        std::stable_sort(ds.begin(), ds.end(), [](const Diag& a, const Diag& b) {   // (right to left: a mend moves what follows it)
            return a.col.value_or(0) > b.col.value_or(0);
        });
        for (const Diag& d : ds) {
            SaidNamed sm = said_named(d.text);
            std::optional<str> kind = sm.kind;
            if (!kind && d.text.contains(S("incompatible"))) {
                kind = S("other");                          // (only the a = b = 0 below may mend it)
                sm.m.reset();
            }
            if (!kind) continue;
            int64_t col = std::max<int64_t>(d.col.value_or(1) - 1, 0);
            auto g = [&](const char* k) -> std::optional<str> {
                std::optional<str> v = sm.get(k);
                if (v && !v->empty() && py::re::search(S(R"RX(\b__\w+)RX"), *v)) {   // (a name of one C library's own - __p_sig_fn_t: what
                    std::optional<Match> aka = py::re::search(                     //  it is, every compiler's)
                        S("'") + py::re::escape(*v) + S(R"RX(' \{aka '([^']*)'\})RX"), d.text);
                    if (aka) return aka->group(1);
                }
                return v;
            };
            auto gs = [&](const char* k) -> str { return g(k).value_or(S("")); };
            auto gt = [&](const char* k) -> bool { std::optional<str> v = g(k); return v && !v->empty(); };
            const str& K = *kind;
            // ---- the first chain
            if (K == "libname") {
                str fn = gs("fn");
                if (f == "proto.h" && port.fns.contains(fn) && !port.renames.contains(fn)) {
                    port.renames[fn] = fn + S("_");         // (proto.h agrees with the definitions: what it meets is the library)
                    note(notes, "function named as one of the C library of today: renamed with _ after",
                         py::percent(S("%s (%s)"), port.fns.at(fn)->file.value_or(S("")), fn));
                    changed = true;
                }
            } else if (K == "argtype") {
                std::optional<CallAt> at = call_at(l, col);
                if (gt("fn")) {                             // (gcc names the function and the argument)
                    std::optional<list<Span>> spans = call_args(l, gs("fn"), true);
                    int64_t k = py::int_parse(gs("n")) - 1;
                    if (spans && !spans->empty() && k < spans->size()) at = CallAt{gs("fn"), k, (*spans)[k]};
                }
                FnP fn = at ? port.fns.get(std::get<0>(*at)) : FnP();
                if ((at && (d.text + g("how").value_or(S(""))).contains(S("pointer from integer"))) ||
                    (at && d.text.contains(S("integer to pointer")))) {
                    int64_t a = std::get<0>(std::get<2>(*at)), b = std::get<1>(std::get<2>(*at));   // (a word of 16 bits laid over two bytes - *(int *)&s->sa -
                    str arg = l.slice(a, b).strip();                                                //  given as a pointer: it holds a handle; the pointer)
                    if (py::re::match(S(R"RX(\(?\s*\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\))RX"), arg)) {
                        str lead = lead_ws(l.slice(a, b));
                        l = l.slice(0, a) + lead + S("dos_hp(") + arg + S(")") + l.slice_from(b);
                        note(notes,
                             "a word of two bytes given as a pointer (a pointer kept there in two bytes): the handle "
                             "it holds, the pointer",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                        continue;
                    }
                }
                std::optional<Match> want;                  // (the first note that says "expected '")
                for (const str& x : d.notes) {
                    if (x.contains(S("expected '"))) {
                        want = py::re::search(S(R"RX(expected '([^']*)'(?: \{aka '[^']*'\})? but argument is of type)RX"), x);
                        break;
                    }
                }
                if (!fn && at && want) {                    // (a parameter of the C library's: what it takes)
                    str t = py::re::sub(S(R"RX(\bconst\s+)RX"), S(""), ctype(want->group(1), tags));   // (the variable is written too)
                    if (py::re::search(S(R"RX(\b__\w+)RX"), t)) {   // (the library's own name for it: what it is)
                        std::optional<Match> aka;
                        for (const str& x : d.notes) {
                            if (x.contains(S("aka"))) {
                                aka = py::re::search(S(R"RX(expected '[^']*' \{aka '([^']*)'\})RX"), x);
                                break;
                            }
                        }
                        if (aka) t = ctype(aka->group(1), tags);
                    }
                    int64_t a = std::get<0>(std::get<2>(*at)), b = std::get<1>(std::get<2>(*at));
                    str arg = l.slice(a, b).strip();
                    std::optional<Match> v = py::re::fullmatch(S(R"RX((?:[\w\]\[]+(?:->|\.))*(\w+))RX"), arg);
                    if (v && (port.fns.contains(v->group(1)) || port.asm_protos.contains(v->group(1)) || t.contains(S("(*)"))))
                        v.reset();                          // (a function, or a function's pointer: a cast)
                    auto fn_here = enclosing(out.get(f, S("")), ln);
                    if (v) {
                        bool starred = false;
                        for (const str& t_ : out.values()) {
                            if (py::re::search(S(R"RX(\*\s*)RX") + py::re::escape(v->group(1)) + S(R"RX(\s*[;,)=\[])RX"), blank(t_))) {
                                starred = true;
                                break;
                            }
                        }
                        if (!starred || (fn_here && declares_local(out.at(f), *fn_here, v->group(1))))
                            v.reset();                      // (an int that held a pointer, there: a cast)
                    }
                    if (v && t.contains(S("*")) && !py::re::match(CAST, arg) &&
                        !py::re::match(S(R"RX(\s*(?:const\s+)?void\s*\*)RX"), t)) {
                        // (a variable or member the recovered source typed loosely - the bytes did not say: it is
                        // what the library takes, so it is declared one)
                        str name = v->group(1);
                        if (!port.retyped.contains(name)) {
                            port.retyped[name] = t;
                            port.decl_rules.append(DeclRule(
                                S(R"RX((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:const\s+)?(?:u?int16_t|int|unsigned|char|void|u?int32_t)\s*\*\s*)RX") +
                                    py::re::escape(name) + S(R"RX(\s*;)RX"),
                                py::percent(S(R"RX(\g<1>%s%s%s;)RX"), t, t.endswith(S("*")) ? S("") : S(" "), name)));
                            note(notes,
                                 "variable or member given to the C library as its type (the recovered source "
                                 "had it loosely typed): declared so",
                                 py::percent(S("%s:%d (%s, %s)"), f, ln, name, t));
                            changed = true;
                        }
                    } else if (!py::re::match(CAST, arg)) {
                        l = l.slice(0, a) + (l.slice(a, a + 1) == " " ? S(" ") : S("")) + S("(") + t + S(")") + arg + l.slice_from(b);
                        note(notes, "argument of another type than the C library's parameter: a cast",
                             py::percent(S("%s:%d (%s)"), f, ln, std::get<0>(*at)));
                        changed = true;
                    }
                    continue;
                }
                if (!fn || std::get<1>(*at) >= fn->params.size()) continue;
                str n = std::get<0>(fn->params[std::get<1>(*at)]);
                str decl = std::get<1>(fn->params[std::get<1>(*at)]);
                int64_t a = std::get<0>(std::get<2>(*at)), b = std::get<1>(std::get<2>(*at));
                str arg = l.slice(a, b).strip();
                if (arg.startswith(S("(intptr_t)")) || arg.endswith(S("!= 0"))) continue;   // (mended already)
                str how = gs("how");
                std::optional<Match> derefs = py::re::search(
                    S(R"RX(\*\s*\+*\s*)RX") + py::re::escape(n) + S(R"RX(\b|\b)RX") + py::re::escape(n) +
                        S(R"RX(\s*(?:\+\+|--)?\s*(?:\[|->))RX"),
                    fn->body);
                if (how.contains(S("incompatible pointer")) && port.given_params.contains(std::make_tuple(fn->name, std::get<1>(*at))) &&
                    !decl.contains(S("void *")) && !derefs) {   // (callers give it pointers of more than one type, as
                    int64_t k = std::get<1>(*at);               //  the int it was took them all: any pointer)
                    fn->params[k] = std::make_tuple(n, S("void *") + n);
                    port.param_types[std::make_tuple(fn->name, k)] = std::get<1>(fn->params[k]);
                    note(notes, "parameter the old definition left an int, given pointers of several types: void *",
                         py::percent(S("%s:%d (%s, %s)"), f, ln, fn->name, n));
                    changed = true;
                    continue;
                }
                str given_t = gs("frm");
                if (!gt("frm")) {
                    given_t = S("");
                    for (const str& y : d.notes) {
                        std::optional<Match> x_ = py::re::search(S(R"RX(but argument is of type '([^']*)')RX"), y);
                        if (x_) { given_t = x_->group(1); break; }
                    }
                }
                std::optional<Match> word_out = py::re::fullmatch(
                    S(R"RX((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s*\*\s*)RX") + py::re::escape(n), decl.strip());
                if (how.contains(S("incompatible pointer")) && word_out &&
                    (py::re::search(S(R"RX(\*\s*\*\s*$)RX"), given_t) ||
                     py::re::fullmatch(S(R"RX(intptr_t\s*\*)RX"), given_t.strip()))) {
                    int64_t k = std::get<1>(*at);           // (an int * the old function stored a pointer through -
                    fn->params[k] = std::make_tuple(n, S("intptr_t *") + n);   //  a word was either: a place that holds either whole)
                    port.param_types[std::make_tuple(fn->name, k)] = std::get<1>(fn->params[k]);
                    note(notes, "int * parameter given the place of a pointer (it stores one there): intptr_t *",
                         py::percent(S("%s:%d (%s, %s)"), f, ln, fn->name, n));
                    changed = true;
                    continue;
                }
                if (how.contains(S("incompatible pointer")) &&
                    py::re::fullmatch(S(R"RX(intptr_t\s*\*\s*)RX") + py::re::escape(n), decl.strip())) {
                    if (py::re::search(S(R"RX(\*\s*\*\s*$)RX"), given_t)) continue;   // (a pointer's place: an intptr_t's, the same 8 bytes)
                    std::optional<Match> v_ = py::re::fullmatch(S(R"RX(&\s*(\w+))RX"), arg);
                    auto fn_at = enclosing(out.get(f, S("")), ln);
                    if (v_ && fn_at && py::re::fullmatch(S(R"RX((?:u?int16_t|short|int|unsigned)\s*\*)RX"), given_t.strip()) &&
                        declares_local(out.at(f), *fn_at, v_->group(1))) {
                        port.local_retypes[std::make_tuple(f, std::get<0>(*fn_at), v_->group(1))] = S("intptr_t");   // (the word it gives: whole)
                        note(notes, "a local's place given for one a pointer is stored in: the local intptr_t",
                             py::percent(S("%s:%d (%s, %s)"), f, ln, std::get<0>(*fn_at), v_->group(1)));
                        changed = true;
                        continue;
                    }
                }
                str new_, why;
                if (how.contains(S("from pointer")) || how.contains(S("pointer to integer")) ||
                    (gt("frm") && gs("frm").contains(S("*")) && !g("to").value_or(S("*")).contains(S("*")))) {
                    std::optional<Match> given;             // (the first note that says "but argument is of type")
                    for (const str& x : d.notes) {
                        if (x.contains(S("but argument is of type"))) {
                            given = py::re::search(S(R"RX(but argument is of type '([^']*\*)')RX"), x);
                            break;
                        }
                    }
                    list<str> libc_sorted = py::sorted(port.profile.libc);
                    std::optional<Match> as_ptr = py::re::search(
                        S(R"RX(\*\s*)RX") + py::re::escape(n) + S(R"RX(\b|\b)RX") + py::re::escape(n) +
                            S(R"RX(\s*(?:\[|->)|\b(?:)RX") + py::join(S("|"), libc_sorted) + S(R"RX()\s*\(\s*)RX") +
                            py::re::escape(n) + S(R"RX(\s*[,)])RX"),
                        fn->body);                          // (used as a pointer there: deref, index, a library's)
                    if (given && !used_as_number(fn->body, n) &&
                        (fn->implicit.contains(n) ||
                         (as_ptr && py::re::fullmatch(S(R"RX((?:register\s+)?(?:int|unsigned|short|int16_t|uint16_t)\s+)RX") +
                                                          py::re::escape(n),
                                                      decl.strip()))) &&
                        !fn->loose) {
                        int64_t k = std::get<1>(*at);       // (a parameter the old definition never declared - an
                        str t_ = ctype(given->group(1), tags);   //  int by default: what the callers give it)
                        fn->params[k] = std::make_tuple(n, t_ + (t_.endswith(S("*")) ? S("") : S(" ")) + n);
                        port.param_types[std::make_tuple(fn->name, k)] = std::get<1>(fn->params[k]);
                        port.given_params.add(std::make_tuple(fn->name, k));
                        note(notes, "parameter the old definition left undeclared (an int): the pointer its callers give",
                             py::percent(S("%s:%d (%s, %s: %s)"), f, ln, fn->name, n, t_));
                        changed = true;
                        continue;
                    }
                    if (truthy(fn->body, n)) {
                        new_ = arg + S(" != 0");
                        why = S("pointer given for a parameter that is only tested: p != 0");
                    } else if (py::re::search(S(R"RX(\bintptr_t\b)RX"), decl)) {   // (the parameter holds either: the pointer given whole)
                        new_ = S("(intptr_t)(") + arg + S(")");
                        why = S("pointer given for a parameter that holds either: (intptr_t)");
                    } else {                                // (an int that carried a pointer - 16 bits both, then:
                        int64_t k = std::get<1>(*at);       //  the parameter holds either whole now)
                        fn->params[k] = std::make_tuple(n, py::re::sub(
                            S(R"RX(^(?:register\s+)?(?:u?int16_t|int|unsigned|short)\b)RX"),
                            [](const Match& mm) { return mm.group(0).replace(py::split(mm.group(0))[-1], S("intptr_t")); },
                            decl));
                        port.param_types[std::make_tuple(fn->name, k)] = std::get<1>(fn->params[k]);
                        note(notes, "integer parameter given a pointer (both were 16 bits): intptr_t, to hold either",
                             py::percent(S("%s:%d (%s, %s)"), f, ln, fn->name, n));
                        changed = true;
                        continue;
                    }
                } else {
                    str t = gt("to") ? ctype(gs("to"), tags) : abstract(decl, n);
                    Notes scratch;
                    t = std::get<0>(widths(t, blank(t), scratch, f, port.int16));
                    if (arg.replace(S(" "), S("")).startswith(S("(") + t.replace(S(" "), S("")) + S(")")))
                        continue;                           // (cast so already: the complaint is another's)
                    bool simple = py::re::fullmatch(S(R"RX([\w.\->\[\]]+|\w+\([^()]*\)|&?[\w.\->\[\]]+)RX"), arg).has_value();
                    new_ = S("(") + t + S(")") + (simple ? arg : S("(") + arg + S(")"));
                    why = S("argument of another type than the parameter (as old C took it): a cast");
                }
                l = l.slice(0, a) + (l.slice(a, a + 1) == " " ? S(" ") : S("")) + new_ + l.slice_from(b);
                note(notes, why, py::percent(S("%s:%d (%s, argument %d)"), f, ln, fn->name, std::get<1>(*at) + 1));
                changed = true;
            } else if (K == "fnptr") {
                std::optional<Match> x = py::re::match(S(R"RX(\s*(\w+)\s*=\s*(\w+)\s*;)RX"), l);
                std::optional<Match> at = py::re::match(S(R"RX(\w+)RX"), l.slice_from(col));
                FnP fn = at ? port.fns.get(at->group(0)) : FnP();
                if (fn && fn->void_ && !ctype(gs("to")).startswith(S("void"))) {
                    fn->void_ = false;
                    fn->keep_int = true;                    // (kept with functions that give an int: so does it)
                    note(notes, "function put where functions giving an int are (a table, a pointer): int",
                         py::percent(S("%s:%d (%s)"), f, ln, fn->name));
                    changed = true;
                } else if (at && port.fns.contains(at->group(0)) && !x && gt("to") && d.text.contains(S("initialization"))) {
                    str t = ctype(gs("to"), tags);          // (an element of a table of them: as the table's)
                    l = l.slice(0, col) + S("(") + t + S(")") + l.slice_from(col);
                    note(notes, "function put in a table of functions of another prototype: a cast",
                         py::percent(S("%s:%d (%s)"), f, ln, at->group(0)));
                    changed = true;
                } else if (x && port.fns.contains(x->group(2)) && port.fptr_mixed.contains(x->group(1)) && gt("to")) {
                    str t = ctype(gs("to"), tags);          // (given functions of several prototypes: each made the
                    l = l.slice(0, x->start(2)) + S("(") + t + S(")") + l.slice_from(x->start(2));   // pointer's own type)
                    note(notes, "pointer given functions of several prototypes: each cast to the pointer's type",
                         py::percent(S("%s:%d (%s)"), f, ln, x->group(2)));
                    changed = true;
                } else if (x && port.fns.contains(x->group(2))) {
                    str t = ctype(gs("frm"), tags);
                    if (!t.contains(S("*)"))) t = replace_first(t, S("("), S("(*)("));   // (clang names the function's type, not its pointer's)
                    t = py::re::sub(S(R"RX(\(\s*\*\s*\))RX"), S("(*)"), t);
                    port.fptr.setdefault(x->group(1), set<str>()).add(t);
                    changed = true;
                } else {
                    kind = S("assign");                     // (no function of the program given: a cast)
                }
            }
            const std::optional<str> K2 = kind;
            // ---- 0 given to several of different types at once (a = b = 0): to each
            std::optional<Match> chain = py::re::fullmatch(
                S(R"RX((\s*)((?:[\w.\->\[\]]+\s*=\s*){2,})(0|\(\w[\w\s*]*\)\s*0)\s*;(.*))RX"), l);
            if (chain && (*K2 == "assign" || *K2 == "intptr" || *K2 == "callptr" || *K2 == "fnptr" ||
                          d.text.contains(S("incompatible")))) {
                list<str> lhs, parts;
                for (const str& x : py::split(chain->group(2), S("="))) {
                    if (!x.strip().empty()) lhs.append(x.strip());
                }
                for (const str& x : lhs) parts.append(x + S(" = 0"));
                l = chain->group(1) + py::join(S(", "), parts) + S(";") + chain->group(4);
                note(notes, "0 given to several of different types at once (a = b = 0): to each", py::percent(S("%s:%d"), f, ln));
                changed = true;
                kind.reset();
            }
            // ---- the second chain
            if (!kind) continue;
            const str& K3 = *kind;
            if (K3 == "assign") {
                std::vector<Match> eq = eq_near(l, col);
                if (!eq.empty()) {
                    int64_t a = eq[0].end();
                    int64_t b = value_end(l, a, true);      // (a member of a compound literal: to its })
                    while (b > a && l.at_(b - 1) == ' ') b--;   // (the value: not the space before its end)
                    str rhs = l.slice(a, b).strip();
                    str t = ctype(gs("to"), tags);
                    std::optional<Match> c = py::re::match(CAST, rhs);
                    if (c && c->end() < rhs.size() &&
                        rhs.slice(0, c->end()).replace(S(" "), S("")) != S("(") + t.replace(S(" "), S("")) + S(")")) {
                        rhs = rhs.slice_from(c->end()).strip();   // (a cast of an earlier round, to another type: replaced)
                        c.reset();
                    }
                    if (!rhs.empty() && !c) {
                        bool simple = py::re::fullmatch(S(R"RX([\w.\->\[\]]+|\w+\(.*\))RX"), rhs).has_value();
                        l = l.slice(0, a) + S(" (") + t + S(")") + (simple ? rhs : S("(") + rhs + S(")")) + l.slice_from(b);
                        note(notes, "pointer given to a pointer of another type (as old C took it): a cast",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                    }
                }
            } else if (K3 == "retptr") {                    // (a function of old C's int that gives a pointer)
                list<str> above = py::split(out.at(f), S("\n")).slice(0, ln);
                for (const str& x : py::reversed(above)) {
                    std::optional<Match> dm = py::re::match(S(R"RX(^[\w\s*]*?\b(\w+)\s*\([^;]*\)\s*$)RX"), x);
                    if (dm && port.fns.contains(dm->group(1))) {
                        FnP fn = port.fns.at(dm->group(1));
                        str t = ctype(gs("frm"), tags);
                        if (fn->ret.empty() || fn->ret == "int" || fn->ret == "int16_t") {
                            fn->ret = t;
                            fn->void_ = false;
                            fn->keep_int = true;
                            note(notes, "function of old C's int that gives a pointer: declared to give it",
                                 py::percent(S("%s (%s, %s)"), f, fn->name, t));
                            changed = true;
                        }
                        break;
                    }
                }
            } else if (K3 == "retint" || K3 == "retmix") {  // (a function given a pointer type by its callers that
                auto fn_at = enclosing(out.get(f, S("")), ln);   //  gives an int - an offset; or pointers of more than
                FnP fn = fn_at ? port.fns.get(std::get<0>(*fn_at)) : FnP();   // one type: as the int it was gave them all)
                if (fn && K3 == "retint" && !fn->int_ret) {
                    fn->ret = S("int");
                    fn->void_ = false;
                    fn->keep_int = true;
                    fn->int_ret = true;
                    fn->ret_set = true;
                    note(notes,
                         "function that gives an integer though its callers take it for a pointer (16 bits both "
                         "then): int, the callers cast",
                         py::percent(S("%s (%s)"), f, fn->name));
                    changed = true;
                } else if (fn && K3 == "retmix" && fn->ret != "void *") {
                    fn->ret = S("void *");
                    fn->void_ = false;
                    fn->keep_int = true;
                    fn->ret_set = true;
                    note(notes, "function that gives pointers of several types (old C's int): void *",
                         py::percent(S("%s (%s)"), f, fn->name));
                    changed = true;
                }
            } else if (K3 == "callptr") {                   // (a pointer from a function declared int)
                std::vector<Match> near = eq_near(l, col);
                std::optional<Match> cm = near.empty() ? std::nullopt
                                                       : py::re::match(S(R"RX(\s*\(*\s*(\w+)\s*\()RX"), l.slice_from(near[0].end()));
                std::optional<str> name = cm ? std::optional<str>(cm->group(1)) : std::nullopt;   // (the call right after the = complained of)
                str t = ctype(gs("to"), tags);
                if (name && port.fns.contains(*name) && port.fns.at(*name)->int_ret) {
                    int64_t a = near[0].end();              // (it gives an integer: what the old pointer was)
                    int64_t b = value_end(l, a, false);
                    str rhs = l.slice(a, b).strip();
                    if (!rhs.empty() && !py::re::match(CAST, rhs)) {
                        l = l.slice(0, a) + S(" (") + t + S(")(intptr_t)(") + rhs + S(")") + l.slice_from(b);
                        note(notes, "integer given to a pointer (as old C took it): a cast", py::percent(S("%s:%d"), f, ln));
                        changed = true;
                    }
                } else if (name && port.fns.contains(*name) &&
                           (port.fns.at(*name)->ret.empty() || port.fns.at(*name)->ret == "int" ||
                            port.fns.at(*name)->ret == "int16_t")) {
                    FnP fn = port.fns.at(*name);
                    fn->ret = t;
                    fn->void_ = false;
                    fn->keep_int = true;
                    note(notes, "function of old C's int that gives a pointer: declared to give it",
                         py::percent(S("%s:%d (%s, %s)"), f, ln, *name, t));
                    changed = true;
                } else if (!name || (!port.fns.contains(*name) && !port.asm_protos.contains(*name))) {
                    std::vector<Match> eq = eq_near(l, col);
                    if (!eq.empty()) {                      // (no call: the value cast, as old C took it)
                        int64_t a = eq[0].end();
                        int64_t b = value_end(l, a, false);
                        str rhs = l.slice(a, b).strip();
                        if (!rhs.empty() && !py::re::match(CAST, rhs)) {
                            l = l.slice(0, a) + S(" (") + t + S(")(") + rhs + S(")") + l.slice_from(b);
                            note(notes, "integer given to a pointer (as old C took it): a cast", py::percent(S("%s:%d"), f, ln));
                            changed = true;
                        }
                    }
                } else if (port.asm_protos.contains(*name) && !port.asm_protos.at(*name).startswith(t)) {
                    port.asm_protos[*name] = t + (t.endswith(S("*")) ? S("") : S(" ")) + *name + S("();");
                    note(notes, "routine of the assembly language that gives a pointer: declared to give it",
                         py::percent(S("%s:%d (%s, %s)"), f, ln, *name, t));
                    changed = true;
                }
            } else if (K3 == "neg") {
                // x == -1 of an unsigned: 16 bits, the 8086 compared it unsigned (0xFFFF); here the uint16_t is made
                // an int first and the test fails - the constant given the operand's type keeps the meaning
                std::optional<str> t = gt("t") ? std::optional<str>(ctype(gs("t"))) : std::nullopt;
                std::optional<Match> best;
                for (const Match& x : py::re::finditer(
                         S(R"RX((?P<a>[\w\]\)]+)\s*(?P<op>[=!<>]=|[<>])\s*(?P<k>-\s*\d+)\b|)RX") +
                             S(R"RX((?P<k2>-\s*\d+)\s*(?P<op2>[=!<>]=|[<>])\s*(?P<b>[\w\[(]+))RX"), l)) {
                    auto dist = [&](const Match& mm) { return mm.start() > col ? mm.start() - col : col - mm.start(); };
                    if (!best || dist(x) < dist(*best)) best = x;
                }
                if (best) {
                    // groups: 1 a, 2 op, 3 k, 4 k2, 5 op2, 6 b
                    std::optional<str> ga = best->group_opt(1), gb = best->group_opt(6);
                    str opnd = (ga && !ga->empty()) ? *ga : gb.value_or(S(""));
                    str name = py::re::sub(S(R"RX(\[.*)RX"), S(""), opnd).strip(S("()"));
                    std::optional<Match> decl = py::re::search(
                        S(R"RX(\b(u?int16_t|uint8_t|unsigned char)\b[^;]*\b)RX") + py::re::escape(name) + S(R"RX(\b)RX"), out.at(f));
                    str tt = t ? *t : (decl ? decl->group(1) : S("uint16_t"));   // (a byte's test was always false there too)
                    if (tt == "uint16_t" || tt == "unsigned short") {
                        std::optional<str> gk = best->group_opt(3);
                        int64_t kg = (gk && !gk->empty()) ? 3 : 4;
                        auto [a, b] = best->span(kg);
                        l = l.slice(0, a) + S("(uint16_t)") + best->group(kg).replace(S(" "), S("")) + l.slice_from(b);
                        note(notes,
                             "negative constant compared with a 16-bit unsigned (unsigned there, an int here): "
                             "(uint16_t)-1",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                    }
                }
            } else if (K3 == "intptr") {
                int64_t eq_ = l.find(S("="), std::max<int64_t>(0, col - 1));
                if (eq_ > 0 && l.slice(eq_ + 1, eq_ + 2) != "=" &&
                    py::re::search(S(R"RX(\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*&[^=]*$)RX"), l.slice(0, eq_))) {
                    int64_t end_ = l.find(S(";"), eq_);     // (*(int *)&s->sa = str: a pointer kept in a word laid
                    str rhs_ = end_ > eq_ ? l.slice(eq_ + 1, end_).strip() : S("");   //  over two bytes - its handle there)
                    if (!rhs_.empty() && !rhs_.startswith(S("dos_ph("))) {
                        l = l.slice(0, eq_ + 1) + S(" dos_ph(") + rhs_ + S(")") + l.slice_from(end_);
                        note(notes,
                             "a pointer kept in a word of two bytes laid over others (*(int *)&s->sa): its handle "
                             "(dos_ph)",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                        continue;
                    }
                }
                std::optional<Match> x = py::re::match(S(R"RX(\s*(\w+)\s*=)RX"), l);
                str t = ctype(gs("frm"), tags);
                if (!x) {                                   // (if ((fp = fopen(..)) == 0): the name before the =)
                    int64_t eqat = l.find(S("="), std::max<int64_t>(0, col - 1));
                    std::optional<Match> vm = eqat > 0 ? py::re::search(S(R"RX((?<![.>])\b(\w+)\s*$)RX"), l.slice(0, eqat)) : std::nullopt;
                    auto fn_at = enclosing(out.get(f, S("")), ln);
                    if (vm && fn_at && declares_local(out.at(f), *fn_at, vm->group(1))) {
                        x = vm;
                    } else if (vm && eqat > 0 && l.slice(eqat + 1, eqat + 2) != "=" &&
                               (gs("to") + d.text).contains(S("intptr_t")) && rule(S("mend_intptr_assign"))) {
                        int64_t depth = 0, k_ = eqat + 1;   // (if (!(g_0c58 = fopen(..))), g_0c58 made intptr_t
                        while (k_ < l.size()) {             //  already: the value there its intptr_t - to the
                            char32_t ch_ = l.at_(k_);       //  ')' that closes the assignment)
                            if (ch_ == '(' || ch_ == '[') {
                                depth++;
                            } else if (ch_ == ')' || ch_ == ']') {
                                if (depth == 0) break;
                                depth--;
                            } else if ((ch_ == ',' || ch_ == ';') && depth == 0) {
                                break;
                            }
                            k_++;
                        }
                        str rhs_ = l.slice(eqat + 1, k_).strip();
                        if (!rhs_.empty() && !rhs_.startswith(S("(intptr_t)"))) {
                            l = l.slice(0, eqat + 1) + S(" (intptr_t)(") + rhs_ + S(")") + l.slice_from(k_);
                            note(notes, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                                 py::percent(S("%s:%d"), f, ln));
                            changed = true;
                        }
                        continue;
                    }
                }
                if (!x) {                                   // (a member given a pointer's value - a = p = 0: the
                    int64_t eq = l.find(S("="), std::max<int64_t>(0, col - 1));   //  number it was, the pointer's bits)
                    std::optional<Match> mm = eq > 0 ? py::re::search(S(R"RX((?<![\w.>])(\w+)\s*(?:->|\.)\s*(\w+)\s*$)RX"), l.slice(0, eq))
                                                     : std::nullopt;
                    if (mm && l.slice(eq + 1, eq + 2) != "=") {   // (x->node = n, int node: the member a pointer's width)
                        list<str> tg = py::re::findall(
                            S(R"RX(\bstruct\s+(\w+)\s*\*?\s*)RX") + py::re::escape(mm->group(1)) + S(R"RX(\s*[;,=)\[])RX"),
                            blank(out.get(f, S(""))));
                        bool done_ = false;
                        if (set<str>::from(tg).size() == 1) {
                            for (const auto& hk : port.files.items()) {
                                const str& h_ = std::get<0>(hk);
                                const str& ht = std::get<1>(hk);
                                std::optional<Match> sm = py::re::search(
                                    S(R"RX(\bstruct\s+)RX") + py::re::escape(tg[0]) + S(R"RX(\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+)RX") +
                                        py::re::escape(mm->group(2)) + S(R"RX(\s*;)RX"),
                                    blank(ht));
                                if (sm) {
                                    port.files[h_] = ht.slice(0, sm->start(1)) + S("intptr_t") + ht.slice_from(sm->end(1));
                                    if (out.contains(h_) && !out.at(h_).empty()) {
                                        std::optional<Match> s2 = py::re::search(
                                            S(R"RX(\bstruct\s+)RX") + py::re::escape(tg[0]) +
                                                S(R"RX(\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+)RX") +
                                                py::re::escape(mm->group(2)) + S(R"RX(\s*;)RX"),
                                            blank(out.at(h_)));
                                        if (s2) out[h_] = out.at(h_).slice(0, s2->start(1)) + S("intptr_t") + out.at(h_).slice_from(s2->end(1));
                                    }
                                    note(notes, "a member given a pointer (an int then, a pointer's width): intptr_t",
                                         py::percent(S("%s (struct %s: %s)"), h_, tg[0], mm->group(2)));
                                    changed = done_ = true;
                                }
                            }
                        }
                        if (done_) continue;
                    }
                    int64_t end = l.find(S(";"), eq);
                    str rhs = (eq >= 0 && end > eq) ? l.slice(eq + 1, end).strip() : S("");
                    if (!rhs.empty() && !rhs.startswith(S("(intptr_t)")) && l.slice(eq + 1, eq + 2) != "=") {
                        l = l.slice(0, eq + 1) + S(" (intptr_t)(") + rhs + S(")") + l.slice_from(end);
                        note(notes, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                    }
                    continue;
                }
                str var = x->group(1);
                auto fn_at = enclosing(out.get(f, S("")), ln);   // (the function the complaint is in, and whether the
                bool local = fn_at && declares_local(out.at(f), *fn_at, var);   //  variable is one of its own)
                str key_ = local ? retyped_key(f, std::get<0>(*fn_at), var) : var;
                bool wide_ = gs("to").contains(S("intptr_t")) || d.text.contains(S("long long"));
                if (port.retyped.contains(key_) && !wide_) continue;
                str scope = local ? py::join(S("\n"), py::split(blank(out.at(f)), S("\n")).slice(std::get<1>(*fn_at), std::get<3>(*fn_at) + 1))
                                  : all_code();
                if (wide_ || used_as_number(scope, var)) {  // (a number too: the pointer's bits given to it, a cast)
                    int64_t eqat = l.find(S("="), std::max<int64_t>(0, col - 1));
                    int64_t a = eqat + 1;
                    int64_t b = value_end(l, a, false);
                    str rhs = l.slice(a, b).strip();
                    if (eqat > 0 && !rhs.empty() && !rhs.startswith(S("(intptr_t)"))) {
                        l = l.slice(0, a) + S(" (intptr_t)(") + rhs + S(")") + l.slice_from(b);
                        note(notes, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                             py::percent(S("%s:%d"), f, ln));
                        changed = true;
                    }
                    continue;
                }
                if (!local) {
                    bool any = false;
                    for (const str& t_ : out.values()) {
                        if (py::re::search(S(R"RX((?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+(?:[^;(){}\n]*,\s*)?)RX") +
                                               py::re::escape(var) + S(R"RX(\s*[;,=\[])RX"),
                                           blank(t_))) {
                            any = true;
                            break;
                        }
                    }
                    if (!any) continue;                     // (no variable of the program's by that name: not a guess)
                }
                port.retyped[key_] = t;                     // (the variable holds a pointer: it is declared one)
                if (local) {                                // (there alone: another function's may be a number)
                    port.local_retypes[std::make_tuple(f, std::get<0>(*fn_at), var)] = t;
                } else {
                    str v = py::re::escape(var);
                    port.decl_rules.append(DeclRule(
                        S(R"RX((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)\s+)RX") + v +
                            S(R"RX((\s*=\s*0)?\s*;)RX"),
                        py::percent(S(R"RX(\g<1>%s%s%s\g<2>;)RX"), t, t.endswith(S("*")) ? S("") : S(" "), var)));
                    port.decl_rules.append(DeclRule(py::percent(GLOBAL_DECL, v), split_decl(var, t)));   // (extern int a, v, b;)
                }
                note(notes, "integer variable that holds a pointer (the 8086's were both 16 bits): declared a pointer",
                     py::percent(S("%s:%d (%s, %s)"), f, ln, var, t));
                changed = true;
            } else if (K3 == "ptrcast") {                   // ((int)p: a pointer as a word, 16 bits both then - the
                py::re::Pattern cp(S(R"RX(\(\s*(?:u?int16_t|int|short|unsigned|unsigned\s+int)\s*\))RX"), 0);
                std::optional<Match> x = cp.match(l.slice_from(col));   //  whole of it now)
                std::optional<int64_t> at_;
                int64_t xlen = 0;
                if (!x) {
                    list<Match> x_ = cp.finditer(l.slice(0, col + 1));
                    if (!x_.empty()) { x = x_[-1]; at_ = x->start(); }
                    xlen = x ? x->end() - x->start() : 0;
                } else {
                    at_ = col;
                    xlen = x->end() - x->start();
                }
                if (!x) continue;
                l = l.slice(0, *at_) + S("(intptr_t)") + l.slice_from(*at_ + xlen);
                note(notes, "a pointer cast to an int (both 16 bits then): to intptr_t, the whole of it",
                     py::percent(S("%s:%d"), f, ln));
                changed = true;
            } else if (K3 == "incomplete") {                // (a structure given by value, its header not here)
                std::optional<str> tag = g("to");
                if (!tag || tag->empty()) {
                    tag.reset();
                    std::optional<CallAt> at = call_at(l, col);
                    FnP fn = at ? port.fns.get(std::get<0>(*at)) : FnP();
                    if (fn && std::get<1>(*at) < fn->params.size()) {
                        std::optional<Match> x = py::re::search(S(R"RX(\b(?:struct|union)\s+\w+)RX"),
                                                                std::get<1>(fn->params[std::get<1>(*at)]));
                        if (x) tag = x->group(0);
                    }
                }
                if (!tag) continue;
                str tag_rx = py::re::escape(*tag).replace(S("\\ "), S("\\s+")).replace(S(" "), S("\\s+"));
                list<str> hs;
                for (const auto& hk : out.items()) {
                    if (std::get<0>(hk).endswith(S(".h")) && py::re::search(S(R"RX(\b)RX") + tag_rx + S(R"RX(\s*\{)RX"), blank(std::get<1>(hk))))
                        hs.append(std::get<0>(hk));
                }
                if (hs.size() == 1 && !port.need_headers.get(f, list<str>()).contains(hs[0])) {
                    need_header(port, f, hs[0]);
                    note(notes, "a structure given by value where its header was not included: included",
                         py::percent(S("%s:%d (%s)"), f, ln, hs[0]));
                    changed = true;
                }
            } else if (K3 == "unionset") {                  // (u = a member's value: that member of it set)
                str to = gs("to"), frm = gs("frm");
                int64_t eq = l.find(S("="), std::max<int64_t>(0, col - 1));
                if (eq < 0 || l.slice(eq + 1, eq + 2) == "=") continue;
                int64_t a = eq + 1;
                int64_t b = value_end(l, a, true);
                str rhs = l.slice(a, b).strip();
                std::optional<str> mname = union_member_for(out, to, frm);
                if (rhs.empty() || !mname || rhs.startswith(S("(") + to + S(")"))) continue;
                l = l.slice(0, a) + S(" (") + to + S("){ .") + *mname + S(" = ") + rhs + S(" }") + l.slice_from(b);
                note(notes, "a union given a member's value (old C copied the bytes): the union, that member set",
                     py::percent(S("%s:%d (.%s)"), f, ln, *mname));
                changed = true;
            } else if (K3 == "unionarg") {                  // (a union given by value: old C pushed a member's
                std::optional<str> to = g("to");            //  value, its bytes)
                if (!to || to->empty()) {
                    to.reset();
                    for (const str& y : d.notes) {
                        std::optional<Match> x_ = py::re::search(S(R"RX(expected '(union \w+)')RX"), y);
                        if (x_) { to = x_->group(1); break; }
                    }
                }
                std::optional<str> frm = g("frm");
                if (!frm || frm->empty()) {
                    frm.reset();
                    for (const str& y : d.notes) {
                        std::optional<Match> x_ = py::re::search(S(R"RX(argument is of type '([^']*)')RX"), y);
                        if (x_) { frm = x_->group(1); break; }
                    }
                }
                if (!to || !frm) continue;
                std::optional<CallAt> at = call_at(l, col);
                if (gt("fn")) {
                    std::optional<list<Span>> spans = call_args(l, gs("fn"), true);
                    int64_t k = py::int_parse(gs("n")) - 1;
                    if (spans && !spans->empty() && k < spans->size()) at = CallAt{gs("fn"), k, (*spans)[k]};
                }
                if (!at) continue;
                int64_t a = std::get<0>(std::get<2>(*at)), b = std::get<1>(std::get<2>(*at));
                str arg = l.slice(a, b).strip();
                if (arg.startswith(S("(") + *to + S(")"))) continue;
                std::optional<str> mname = union_member_for(out, *to, *frm);
                bool zero = py::re::fullmatch(S(R"RX(0[lL]?)RX"), arg).has_value();
                if (!mname && !zero) continue;
                str lead = lead_ws(l.slice(a, b));
                bool plain_zero = (zero && !mname) || (zero && *frm == "int");
                l = l.slice(0, a) + lead +
                    (plain_zero ? S("(") + *to + S("){ 0 }") : S("(") + *to + S("){ .") + mname.value_or(S("None")) + S(" = ") + arg + S(" }")) +
                    l.slice_from(b);
                if (!visible(f).contains(py::split(*to)[-1])) {   // (the union, complete where it is made)
                    for (const auto& hk : out.items()) {
                        if (std::get<0>(hk).endswith(S(".h")) &&
                            py::re::search(S(R"RX(\bunion\s+)RX") + py::re::escape(py::split(*to)[-1]) + S(R"RX(\s*\{)RX"),
                                           blank(std::get<1>(hk)))) {
                            need_header(port, f, std::get<0>(hk));
                            break;
                        }
                    }
                }
                note(notes,
                     "a union given by value as one of its members' values (old C pushed the bytes): the union, "
                     "that member set",
                     py::percent(S("%s:%d (%s, .%s)"), f, ln, std::get<0>(*at), mname.value_or(S("None"))));
                changed = true;
            } else if (K3 == "seqpt") {                     // (f(op, (n -= 2) == 0, n): the old compiler's order
                str v = gs("v");                            //  of the arguments, written out)
                str order = port.profile.arg_order;
                std::optional<Match> x = py::re::search(
                    S(R"RX(\(\s*()RX") + v + S(R"RX(\s*(?:[-+*/%&|^]|<<|>>)=\s*[^()=,;]+|(?:\+\+|--)\s*)RX") + v + S(R"RX()\s*\))RX"), l);
                std::optional<Match> st_ = py::re::match(S(R"RX((\s*)(return\s+)?)RX"), l);
                if (!x || order != "left" || l.rstrip().endswith(std::make_tuple("{", ")")) ||
                    py::re::match(S(R"RX(\s*(?:if|while|for|switch)\b)RX"), l) || !l.rstrip().endswith(S(";")))
                    continue;
                if (py::re::search(S(R"RX(\b)RX") + v + S(R"RX(\b)RX"), l.slice(st_->end(), x->start()))) continue;   // (read before it: an order of its own)
                str eff = x->group(1);
                str body = l.slice(0, x->start()) + v + l.slice_from(x->end());
                str ind = st_->group(1), ret = st_->group_opt(2).value_or(S(""));
                str rest = body.slice_from(st_->end()).rstrip();
                rest = rest.slice(0, rest.size() - 1);
                l = !ret.empty() ? py::percent(S("%s%s(%s, %s);"), ind, ret, eff, rest) : py::percent(S("%s%s, %s;"), ind, eff, rest);
                note(notes,
                     "an argument changes a variable another reads (their order unsaid in C): the old compiler's "
                     "order, left to right, written out",
                     py::percent(S("%s:%d (%s)"), f, ln, v));
                changed = true;
            } else if (K3 == "intcast") {                   // (an int made a pointer: it held one - 16 bits both)
                std::optional<Match> x = py::re::match(
                    S(R"RX(\(\s*(?:(?:struct|union)\s+\w+|\w+)[\w\s]*\*+\s*\)\s*(?:\(\s*(\w+)\s*\)|(\w+)))RX"), l.slice_from(col));
                if (!x || py::re::match(S(R"RX(\s*(?:\(|->|\.|\[))RX"), l.slice_from(col + x->end()))) continue;
                str var = x->group_opt(1) && !x->group_opt(1)->empty() ? *x->group_opt(1) : x->group_opt(2).value_or(S(""));
                auto fn_at = enclosing(out.get(f, S("")), ln);
                FnP fn = fn_at ? port.fns.get(std::get<0>(*fn_at)) : FnP();
                std::optional<int64_t> k;
                if (fn) {
                    int64_t i = 0;
                    for (const auto& p : fn->params) {
                        if (std::get<0>(p) == var) { k = i; break; }
                        i++;
                    }
                }
                if (k) {
                    if (py::re::fullmatch(S(R"RX((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+)RX") + py::re::escape(var),
                                          std::get<1>(fn->params[*k]).strip())) {
                        fn->params[*k] = std::make_tuple(var, S("intptr_t ") + var);
                        port.param_types[std::make_tuple(fn->name, *k)] = std::get<1>(fn->params[*k]);
                        note(notes, "integer parameter made a pointer (it held one: both were 16 bits): intptr_t",
                             py::percent(S("%s:%d (%s, %s)"), f, ln, fn->name, var));
                        changed = true;
                    }
                    std::vector<std::tuple<str, std::vector<str>>> calls_;   // (the others of the line: f(.., (char *)a) && f(.., b) -
                    for (const Match& cm : py::re::finditer(S(R"RX(\b(\w+)\s*\()RX"), l)) {   //  one complaint a line - in the same place of a
                        std::optional<list<Span>> sp = call_args(l.slice_from(cm.start()), cm.group(1));   // call of the same function)
                        std::vector<str> as_;
                        if (sp) {
                            for (const Span& s : *sp) as_.push_back(l.slice(cm.start() + std::get<0>(s), cm.start() + std::get<1>(s)).strip());
                        }
                        calls_.emplace_back(cm.group(1), as_);
                    }
                    str cast_ = S(R"RX((?:\(\s*(?:(?:struct|union)\s+\w+|\w+)[\w\s]*\*+\s*\)\s*)?\(?\s*(\w+)\s*\)?)RX");
                    set<std::tuple<str, int64_t>> places;
                    for (const auto& c : calls_) {
                        int64_t i_ = 0;
                        for (const str& a_ : std::get<1>(c)) {
                            std::optional<Match> fm = py::re::fullmatch(cast_, a_);
                            if (fm && fm->group(1) == var) places.add(std::make_tuple(std::get<0>(c), i_));
                            i_++;
                        }
                    }
                    set<str> others;
                    for (const auto& c : calls_) {
                        int64_t i_ = 0;
                        for (const str& a_ : std::get<1>(c)) {
                            std::optional<Match> fm = py::re::fullmatch(cast_, a_);
                            if (places.contains(std::make_tuple(std::get<0>(c), i_)) && fm) others.add(fm->group(1));
                            i_++;
                        }
                    }
                    for (const str& y : others) {
                        std::optional<int64_t> k2;
                        int64_t i = 0;
                        for (const auto& p : fn->params) {
                            if (std::get<0>(p) == y && std::get<0>(p) != var) { k2 = i; break; }
                            i++;
                        }
                        if (k2 && is_int_word(std::get<1>(fn->params[*k2]))) {
                            str pn2 = std::get<0>(fn->params[*k2]);
                            fn->params[*k2] = std::make_tuple(pn2, S("intptr_t ") + pn2);
                            port.param_types[std::make_tuple(fn->name, *k2)] = std::get<1>(fn->params[*k2]);
                            note(notes, "integer parameter made a pointer (it held one: both were 16 bits): intptr_t",
                                 py::percent(S("%s:%d (%s, %s)"), f, ln, fn->name, pn2));
                            changed = true;
                        }
                    }
                    continue;
                }
                bool local = fn_at && declares_local(out.at(f), *fn_at, var);
                str key_ = local ? retyped_key(f, std::get<0>(*fn_at), var) : var;
                if (port.retyped.contains(key_)) continue;
                if (local) {
                    port.retyped[key_] = S("intptr_t");
                    port.local_retypes[std::make_tuple(f, std::get<0>(*fn_at), var)] = S("intptr_t");
                } else {
                    bool any = false;
                    for (const str& t_ : out.values()) {
                        if (py::re::search(S(R"RX((?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+)RX") + py::re::escape(var) + S(R"RX(\s*;)RX"),
                                           blank(t_))) {
                            any = true;
                            break;
                        }
                    }
                    if (!any) continue;
                    port.retyped[key_] = S("intptr_t");
                    port.decl_rules.append(DeclRule(
                        S(R"RX((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)\s+)RX") + py::re::escape(var) + S(R"RX(\s*;)RX"),
                        S(R"RX(\g<1>intptr_t )RX") + var + S(";")));
                }
                note(notes, "integer variable made a pointer (it held one: both were 16 bits): intptr_t",
                     py::percent(S("%s:%d (%s)"), f, ln, var));
                changed = true;
            } else if (K3 == "member") {
                std::optional<str> name = g("m");
                std::vector<Match> xs;
                for (const Match& x : py::re::finditer(S(R"RX(->\s*(\w+))RX"), l)) {
                    if (!name || name->empty() || x.group(1) == *name) xs.push_back(x);
                }
                std::stable_sort(xs.begin(), xs.end(), [&](const Match& a, const Match& b) {
                    int64_t da = a.start() > col ? a.start() - col : col - a.start();
                    int64_t db = b.start() > col ? b.start() - col : col - b.start();
                    return da < db;
                });
                set<str> seen_ = visible(f);                // (of the structures the file sees: the one that has it)
                for (size_t xi = 0; xi < xs.size() && xi < 1; xi++) {
                    const Match& x = xs[xi];
                    list<str> cands = mem.contains(x.group(1)) ? mem.at(x.group(1)) : list<str>();
                    set<str> in_h;
                    for (const auto& hk : out.items()) {
                        if (!std::get<0>(hk).endswith(S(".h"))) continue;
                        for (const str& tg : py::re::findall(S(R"RX(\b(?:struct|union)\s+(\w+)\s*\{)RX"), blank(std::get<1>(hk)))) in_h.add(tg);
                    }
                    std::optional<str> s;
                    for (const str& c : cands) {
                        if (!c.empty() && seen_.contains(py::split(c)[-1])) { s = c; break; }
                    }
                    if (!s) {
                        for (const str& c : cands) {
                            if (!c.empty() && in_h.contains(py::split(c)[-1])) { s = c; break; }   // (a header's: it can be included)
                        }
                    }
                    if (!s) continue;
                    int64_t o = object_before(l, x.start());
                    str obj = l.slice(o, x.start()).strip();
                    if (obj.startswith(S("((") + *s + S(" *)"))) continue;
                    l = l.slice(0, o) + S("((") + *s + S(" *)") + obj + S(")") + l.slice_from(x.start());
                    note(notes,
                         "member of a structure used on another pointer (as old C allowed): a cast to the "
                         "structure that has it",
                         py::percent(S("%s:%d (%s)"), f, ln, x.group(1)));
                    changed = true;
                }
            } else if (K3 == "void") {
                for (const Match& x : py::re::finditer(S(R"RX(\b(\w+)\s*\()RX"), l)) {
                    FnP fn = port.fns.get(x.group(1));
                    if (fn && fn->void_) {
                        fn->void_ = false;
                        fn->keep_int = true;
                        note(notes, "function whose value is used after all: int (it was what the register held)",
                             py::percent(S("%s:%d (%s)"), f, ln, fn->name));
                        changed = true;
                    }
                }
            } else if (K3 == "arity") {
                list<str> names;
                if (gt("fn")) {
                    names.append(gs("fn"));
                } else {
                    for (const Match& x : py::re::finditer(S(R"RX(\b(\w+)\s*\()RX"), l)) names.append(x.group(1));
                }
                for (const str& n : names) {
                    FnP fn = port.fns.get(n);
                    std::optional<list<Span>> sp = fn ? call_args(l, n) : std::nullopt;
                    if (fn && !fn->loose && !fn->varargs && sp) {
                        int64_t cnt = 0;
                        for (const Span& s : *sp) {
                            if (!l.slice(std::get<0>(s), std::get<1>(s)).strip().empty()) cnt++;
                        }
                        if (cnt != fn->params.size()) {
                            fn->loose = true;
                            note(notes,
                                 "function called with other numbers of arguments than it has: old-style definition "
                                 "kept (its calls unchecked, as they were)",
                                 py::percent(S("%s:%d (%s)"), f, ln, n));
                            changed = true;
                        }
                    }
                }
            }
        }
        if (l != orig) {                                    // (kept as a rule: the next round's text has the line)
            std::optional<std::tuple<str, int64_t>> o;
            if (port.line_origin.contains(f) && port.line_origin.at(f).contains(ln)) o = port.line_origin.at(f).at(ln);
            port.line_fixes[o ? std::make_tuple(f, std::get<0>(*o), std::get<1>(*o)) : std::make_tuple(f, orig, int64_t(-1))] = l;
        }
    }
    for (const auto& kv : port.fptr.items()) {              // (a pointer to functions of one prototype: given it)
        const str& var = std::get<0>(kv);
        const set<str>& types = std::get<1>(kv);
        if (types.size() > 1 && !port.fptr_mixed.contains(var)) {
            port.fptr_mixed.add(var);
            changed = true;
        }
        if (types.size() == 1 && !port.fptr_done.contains(var)) {
            str t = *types.begin();
            port.decl_rules.append(DeclRule(
                S(R"RX(\b(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\(\s*\*\s*)RX") + var + S(R"RX(\s*\)\s*\(\s*\))RX"),
                replace_first(t, S("(*)"), S("(*") + var + S(")"))));
            port.fptr_done.add(var);
            note(notes, "pointer to functions declared with the prototype of those it is given",
                 py::percent(S("%s (%s)"), var, t));
            changed = true;
        }
    }
    return changed || changed_;
}

}  // namespace codebreaker::port::pre_ansi
