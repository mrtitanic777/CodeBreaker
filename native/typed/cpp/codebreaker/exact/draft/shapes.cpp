// codebreaker/exact/draft/shapes.cpp -- typed C++ from codebreaker.exact.draft.shapes (see shapes.hpp).
#include "codebreaker/exact/draft/shapes.hpp"

#include <algorithm>
#include <functional>
#include <set>
#include <tuple>
#include <variant>
#include <vector>

#include "py_os.hpp"

namespace codebreaker::exact::draft::shapes {

namespace {

using py::str;
using Lines = py::list<str>;
using OLines = py::list<std::optional<str>>;
using Match = py::re::Match;
using OMatch = std::optional<Match>;
using text::_balanced;
using text::_block_end;
using text::_conj;
using text::_else_ifs;
using text::_in_loop;
using text::_live_after;
using text::_not;
using text::_opener;
using text::_openers;
using text::_plain_store;
using text::_strip;
using text::_top_split;
using text::_truth;
using text::split_body;

str operator""_s(const char* p, size_t n) { return str(std::string(p, n)); }

// (cond, then, else, end) of the if at line k as a record
struct IP {
    str cond;
    Lines then;
    std::optional<Lines> els;
    int64_t end;
};

std::optional<IP> parts(const Lines& lines, int64_t k) {
    auto p = text::_if_parts(lines, k);
    if (!p) return std::nullopt;
    return IP{std::get<0>(*p), std::get<1>(*p), std::get<2>(*p), std::get<3>(*p)};
}

Lines split_nl(const str& t) { return py::split(t, "\n"_s); }
str join_nl(const Lines& l) { return py::join("\n"_s, l); }

// "\n".join(x for x in lines if x is not None)
str join_opt(const OLines& l) {
    Lines out;
    for (int64_t i = 0; i < l.size(); ++i)
        if (l[i]) out.append(*l[i]);
    return join_nl(out);
}

OLines opt_of(const Lines& l) {
    OLines out;
    for (int64_t i = 0; i < l.size(); ++i) out.append(l[i]);
    return out;
}

// len(l) - len(l.lstrip()): the indentation of a line
int64_t indent_of(const str& l) { return l.size() - l.lstrip().size(); }

// bool(x.strip())
bool nb(const str& x) { return !x.strip().empty(); }

// re.match(r"\s*", l).group(0)
str lead(const str& l) { return py::re::match(R"re(\s*)re"_s, l)->group(0); }

// x[4:] if x.startswith(ind + "    ") else x
str dedent4(const str& x, const str& ind) {
    return x.startswith(ind + "    ") ? x.slice_from(4) : x;
}

str wpat(const str& w) { return py::percent(R"re(\b%s\b)re"_s, w); }
bool wsearch(const str& w, const str& s) { return py::re::search(wpat(w), s).has_value(); }
int64_t wcount(const str& w, const str& s) { return py::re::findall(wpat(w), s).size(); }
// re.sub(r"\bW\b", lambda _: lit, s, count)
str wsub_lit(const str& w, const str& lit, const str& s, int64_t count = 0) {
    return py::re::sub(wpat(w), [&](const Match&) -> str { return lit; }, s, count);
}
// re.sub(r"\bW\b", repl, s, count) with repl a template
str wsub_tpl(const str& w, const str& repl, const str& s, int64_t count = 0) {
    return py::re::sub(wpat(w), repl, s, count);
}

bool one_of(const str& s, std::initializer_list<const char*> xs) {
    for (const char* x : xs)
        if (s == x) return true;
    return false;
}

str esc(const str& s) { return py::re::escape(s); }

// `a or b` for a pass giving back Optional[str]
str or_str(const std::optional<str>& a, const str& b) { return (a && !a->empty()) ? *a : b; }

// next((x for x in reversed(ls) if x.strip()), "")
str last_nonblank(const Lines& ls) {
    for (int64_t i = ls.size() - 1; i >= 0; --i)
        if (nb(ls[i])) return ls[i];
    return str("");
}

// lines.index(x, start)
int64_t index_from(const Lines& lines, const str& x, int64_t start) {
    for (int64_t i = start; i < lines.size(); ++i)
        if (lines[i] == x) return i;
    throw py::ValueError("value is not in list");
}

const str CASELABELRE = R"re(\s*(?:case [^:]+|default):)re"_s;

void uchar_field(const str& ptr, int64_t off) {
    text::UCHAR.add(std::variant<std::tuple<str, int64_t>, str>(std::in_place_index<0>, std::make_tuple(ptr, off)));
}
void uchar_global(const str& g) {
    text::UCHAR.add(std::variant<std::tuple<str, int64_t>, str>(std::in_place_index<1>, g));
}

}  // namespace

// if (C) { r = 1; } else { r = 0; } - a comparison's value (the compiler's 1, then 0 unless it holds): r = C
py::str compare_values(const py::str& text_in) {
    str text = text_in;
    // (a byte given one or the other, then widened with zeros: a char's ?: - an int's would be given in ax)
    text = py::re::sub(
        R"re((?m)^([ \t]*)if \((.*)\) \{\n\s*al = (-?\d+);\n\s*\} else \{\n\s*al = (-?\d+);\n\s*\}\n\s*ah = 0;$)re"_s,
        [&](const Match& m) -> str {
            return py::percent("%sax = ((unsigned char)(%s ? %s : %s));"_s, m.group(1),
                               _strip(py::percent("(%s)"_s, m.group(2))), m.group(3), m.group(4));
        },
        text);
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), c = m.group(2), r = m.group(3), a = m.group(4), b = m.group(5);
        if (a == "1" && b == "0")
            return py::percent("%s%s = %s;"_s, ind, r, c.startswith("("_s) ? c : py::percent("(%s)"_s, c));
        if (a == "0" && b == "1") return py::percent("%s%s = !(%s);"_s, ind, r, c);
        return m.group(0);
    };
    return py::re::sub(
        R"re((?m)^([ \t]*)if \((.*)\) \{\n\s*(ax|al|bx|cx|dx) = ([01]);\n\s*\} else \{\n\s*\3 = ([01]);\n\s*\}$)re"_s,
        one, text);
}

// a || or && taken as a value - the compiler's 1/0 behind the tests - read back:
//   if (A0) { if (B) { L: r = 1; } else { r = 0; } } else { goto L; }   ->  r = !A0 || B
//   if (A) { if (B) { r = 1; } else { r = 0; } } else { r = 0; }        ->  r = A && B
py::str logical_values(const py::str& text_in) {
    str text = text_in;
    text = py::re::sub(
        R"re((?m)^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(L_\w+):\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*goto \4;\n\s*\}$)re"_s,
        [&](const Match& m) -> str {
            return py::percent("%s%s = %s || %s;"_s, m.group(1), m.group(5), _not(m.group(2)), _truth(m.group(3)));
        },
        text);
    // (if (A) { if (B) { r = 1; } else { L: r = 0; } } else { goto L; }  ->  r = A && B)
    text = py::re::sub(
        R"re((?m)^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*(L_\w+):\n\s*\4 = 0;\n\s*\}\n\s*\} else \{\n\s*goto \5;\n\s*\}$)re"_s,
        [&](const Match& m) -> str {
            return py::percent("%s%s = %s && %s;"_s, m.group(1), m.group(4), _truth(m.group(2)), _truth(m.group(3)));
        },
        text);
    auto and_ = [&](const Match& m) -> str {
        str b = m.group(4);
        // (the temporaries the second test reads)
        for (const auto& t : py::re::compile(R"re((\w+) = ([^;\n]+);)re"_s).findall_groups<str, str>(m.group(3))) {
            str t1 = std::get<1>(t);
            b = py::re::sub(wpat(std::get<0>(t)),
                            [&](const Match&) -> str {
                                return !py::re::fullmatch(R"re(\w+)re"_s, t1) ? py::percent("(%s)"_s, t1) : t1;
                            },
                            b);
        }
        return py::percent("%s%s = %s && %s;"_s, m.group(1), m.group(5), _truth(m.group(2)), _truth(b));
    };
    text = py::re::sub(
        R"re((?m)^([ \t]*)if (\(.*\)) \{\n((?:\s*(?:ax|bx|cx|dx|si|di) = [^;\n]+;\n)*)\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*\5 = 0;\n\s*\}$)re"_s,
        and_, text);
    return text;
}

// C ? A : B - one test's two values; a value that is itself a test of the same two values joins the test
// (C ? (C2 ? X : Y) : X is (!C || C2) ? X : Y; C ? X : (C2 ? X : Y) is (C || C2) ? X : Y)
py::str _ternary(const py::str& c, const py::str& a, const py::str& b) {
    for (int i = 0; i < 2; ++i) {
        const str& inner = i == 0 ? a : b;
        const str& other = i == 0 ? b : a;
        bool or_not = i == 0;   // join == "or-not"
        auto m = py::re::fullmatch(R"re(\((.*) \? (.*) : (.*)\))re"_s, inner);
        if (m && _balanced(m->group(1)) && _balanced(m->group(2)) && m->group(2) == other) {
            str first = or_not ? _truth(_not("("_s + c + ")"_s)) : _truth("("_s + c + ")"_s);
            return py::percent("%s ? %s : %s"_s, _conj("||"_s, first, _truth("("_s + m->group(1) + ")"_s)), m->group(2),
                               m->group(3));
        }
        if (m && _balanced(m->group(1)) && _balanced(m->group(2)) && m->group(3) == other) {   // (C ? (C2 ? X : Y) : Y:
            str first = or_not ? _truth("("_s + c + ")"_s) : _truth(_not("("_s + c + ")"_s));   //  C && C2 ? X : Y)
            return py::percent("%s ? %s : %s"_s, _conj("&&"_s, first, _truth("("_s + m->group(1) + ")"_s)), m->group(2),
                               m->group(3));
        }
    }
    return py::percent("%s ? %s : %s"_s, _truth(c), a, b);
}

// a register stepped to test it - inc al; jne - read back as the compare it is: R = (X + K); if ((R..) == 0)
// -> if ((X == -K)) (a byte's: & 0xff)
py::str step_tests(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str x = m.group(3), op = m.group(4), k = m.group(5), head = m.group(6), rel = m.group(7), tail = m.group(8);
        int64_t kk = py::int_from_str(k);
        int64_t v = op == "+" ? -kk : kk;
        return py::percent("%s%s%s %s %d))%s"_s, head, ""_s, x, rel, v, tail);
    };
    return py::re::sub(
        R"re((?m)^([ \t]*)([abcd][lx]) = \((.+) ([+-]) (\d+)\);\n([ \t]*(?:if|while) \(\()\(?\2(?: & 0xff)?\)? (==|!=) 0\)\)(.*)$)re"_s,
        one, text);
}

// a register given one value or another by a test, then read once by the next statement - C's ?: read back:
//     if (C) { r = A; } else { r = B; } X = ..r..;   ->   X = ..(C ? A : B)..;
// (the arms give it nothing else, and the statement after reads it no more)
py::str conditional_values(const py::str& text_in) {
    str text = text_in;
    const str REG = R"re((ax|bx|cx|dx|si|di|al|bl|cl|dl))re"_s;
    const str RX = py::percent(
        R"re((?m)^([ \t]*)if (\(.*\)) \{\n\s*%s = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\s*\}\n(\s*)(.*)$)re"_s, REG);
    const str kw = R"re((?:if|while|for|switch|\}|L_\w+:))re"_s;
    str cur;   // (m.string of the substitution under way)
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), c = m.group(2), r = m.group(3), a = m.group(4), b = m.group(5), ind2 = m.group(6),
            nxt = m.group(7);
        if (ind2 != ind || wcount(r, nxt) != 1 || py::re::match(kw, nxt)) return m.group(0);
        str rest = cur.slice_from(m.end());
        if (wsearch(r, py::re::split(py::percent(R"re(\n\s*%s = (?![^;\n]*\b%s\b))re"_s, r, r), "\n"_s + rest)[0]) &&
            !one_of(r, {"si", "di"}))
            return m.group(0);   // (read again later: not this way)
        if (one_of(r, {"si", "di"})) return m.group(0);   // (a register variable: given a value it keeps)
        str val = _ternary(c, a, b);
        if (r.endswith("l"_s))   // (a byte register's: the value a char)
            return ind + wsub_lit(r, py::percent("(char)(%s)"_s, val), nxt, 1);
        return ind + wsub_lit(r, py::percent("(%s)"_s, val), nxt, 1);
    };
    // (the value given first and given again when a test holds - r = A; if (C) { r = B; }: C ? B : A)
    const str PRE = R"re((?m)^([ \t]*)(ax|bx|cx|dx) = ([^;\n]+);\n\1if (\(.*\)) \{\n\s*\2 = ([^;\n]+);\n\1\}\n\1(.*)$)re"_s;
    cur = text;
    auto first = [&](const Match& m) -> str {
        str ind = m.group(1), r = m.group(2), a = m.group(3), c = m.group(4), b = m.group(5), nxt = m.group(6);
        if (wcount(r, nxt) != 1 || wsearch(r, c) || py::re::match(kw, nxt) ||
            _live_after(opt_of(split_nl(cur.slice_from(m.end()))), -1, r))
            return m.group(0);
        return ind + wsub_lit(r, py::percent("(%s)"_s, _ternary(c, b, a)), nxt, 1);
    };
    text = py::re::sub(PRE, first, text);
    // (nested: an arm that is itself such a test - `else { if (C2) { r = B; } else { r = D; } }` - one value
    //  first, `r = (C2 ? B : D);`, then the outer test)
    const str INNER =
        R"re((?m)^([ \t]*)if (\(.*\)) \{\n\s*(ax|bx|cx|dx) = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\1\}$)re"_s;
    while (true) {
        str t = py::re::sub(INNER,
                            [&](const Match& m) -> str {
                                return py::percent("%s%s = (%s);"_s, m.group(1), m.group(3),
                                                   _ternary(m.group(2), m.group(4), m.group(5)));
                            },
                            text);
        cur = t;
        str t2 = py::re::sub(RX, one, t);
        t = t2;
        if (t == text) return t;
        text = t;
    }
}

// if (C) { r = E1; } else { ..; r = E2; } return r;  ->  if (C) return E1; ..; return E2; (each branch ends
// the function: the compiler jumps from the first to the end)
py::str branch_returns(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)if (\(.*\)) \{\s*$)re"_s, lines[k]);
        if (!m || k + 3 >= lines.size()) continue;
        str ind = m->group(1);
        auto a = py::re::match(R"re(\s*(ax|al|bx|cx|dx|si|di) = (.*);\s*$)re"_s, lines[k + 1]);
        if (!a || lines[k + 2] != ind + "} else {"_s) continue;
        str reg = a->group(1);
        std::optional<int64_t> close;
        for (int64_t j = k + 3; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { close = j; break; }
        if (!close || *close + 1 >= lines.size() ||
            !py::re::fullmatch(py::percent(R"re(\s*return %s;\s*)re"_s, reg), lines[*close + 1]))
            continue;
        auto last = py::re::match(py::percent(R"re(\s*%s = (.*);\s*$)re"_s, reg), lines[*close - 1]);
        if (!last) continue;
        Lines body;
        for (const str& x : lines.slice(k + 3, *close - 1)) body.append(dedent4(x, ind));
        Lines nw;
        nw.append(py::percent("%sif %s"_s, ind, m->group(2)));
        nw.append(py::percent("%s    return %s;"_s, ind, a->group(2)));
        nw.extend(body);
        nw.append(py::percent("%sreturn %s;"_s, ind, last->group(1)));
        return branch_returns(join_nl(lines.slice(0, k) + nw + lines.slice_from(*close + 2)));
    }
    return text;
}

// a switch whose cases are one run of values - LO..HI - the compiler tests as a range (cmp LO, jb; cmp HI,
// jbe): R = E; if ((R >= LO)) { if ((R > HI)) { D } else { C } } else { D }  ->
// switch (E) { default: D case LO: .. case HI: C }
py::str range_switches(const py::str& text) {
    Lines lines = split_nl(text);
    auto dd = [](const Lines& ls) {
        Lines out;
        for (const str& l : ls) out.append(l.startswith("    "_s) ? l.slice_from(4) : l);
        return out;
    };
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        str a1, a2, a3;   // (the indentation, the register or variable tested, the expression it holds)
        bool have_v = false;
        auto a = py::re::fullmatch(R"re(([ \t]*)(ax|bx|cx|dx) = (.+);)re"_s, lines[k]);
        if (a) {
            a1 = a->group(1), a2 = a->group(2), a3 = a->group(3);
        } else {   // (or a variable the code loads for the tests: RANGES)
            auto v = py::re::fullmatch(R"re(([ \t]*)if \(\(([\w\->]+) >= (\d+)\)\) \{)re"_s, lines[k]);
            if (!v) continue;
            bool any_lo = false;
            for (const auto& r : text::RANGES)
                if (std::get<0>(r) == py::int_from_str(v->group(3))) any_lo = true;
            if (!any_lo) continue;
            a1 = v->group(1), a2 = v->group(2), a3 = v->group(2);
            have_v = true;
        }
        std::optional<IP> pp = parts(lines, !have_v ? k + 1 : k);
        if (!pp || !pp->els) continue;
        IP p = *pp;
        auto c1 = py::re::fullmatch(py::percent(R"re(\(\(%s >= (\d+)\)\))re"_s, a2), "("_s + p.cond + ")"_s);
        if (c1 && !p.then.empty() &&
            py::re::fullmatch(py::percent(R"re(\s*(?:ax|bx|cx|dx) = %s;)re"_s, esc(a2)), p.then[0])) {
            p.then = p.then.slice_from(1);   // (the selector loaded again for the next test: nothing)
        }
        std::optional<IP> qq = (c1 && !p.then.empty()) ? parts(p.then, 0) : std::nullopt;
        if (!qq || qq->end != p.then.size()) continue;
        IP q = *qq;
        if (!q.els) q.els = Lines();   // (the range's own arm empty: only a break)
        auto c2 = py::re::fullmatch(py::percent(R"re(\(\(%s (>|<=) (\d+)\)\))re"_s, a2), "("_s + q.cond + ")"_s);
        if (c2 && c2->group(1) == "<=") {   // (the range's own arm first: the same, turned about)
            Lines t1 = q.then;
            q.then = *q.els;
            q.els = t1;
        }
        std::optional<Match> g_;
        if (c2 && p.els->size() == 1) g_ = py::re::fullmatch(R"re(\s*goto (L_\w+);)re"_s, (*p.els)[0]);
        if (g_ && !dd(q.then).empty() && dd(q.then)[0].strip() == g_->group(1) + ":"_s) {   // (the other way out a goto into it:
            Lines body_;                                                                       //  the same code)
            for (const str& l : dd(q.then))
                if (l.strip() != g_->group(1) + ":"_s) body_.append(l);
            if (!py::re::search(py::percent(R"re(\bgoto %s;)re"_s, g_->group(1)),
                                join_nl(lines.slice(0, k) + lines.slice_from(p.end)))) {
                p.els = body_;
                Lines nt;
                for (const str& l : body_) nt.append(nb(l) ? "    "_s + l : l);
                q.then = nt;
            }
        }
        if (!c2 || !(dd(q.then) == *p.els)) continue;
        int64_t lo = py::int_from_str(c1->group(1)), hi = py::int_from_str(c2->group(2));
        if (!(0 <= lo && lo <= hi) || hi - lo > 16) continue;
        str ind = a1;
        Lines dflt;
        dflt.append(ind + "default:"_s);
        dflt.extend(*p.els);
        dflt.append(ind + "    break;"_s);
        Lines cases;
        for (int64_t v = lo; v <= hi; ++v) cases.append(py::percent("%scase %d:"_s, ind, v));
        cases.extend(dd(*q.els));
        cases.append(ind + "    break;"_s);
        bool first = false;   // (the code shows the cases' arm first: no default - what the rest do follows the
        for (const auto& r : text::RANGES)   //  switch, the cases leaving)
            if (std::get<0>(r) == lo && std::get<1>(r) == hi && std::get<2>(r)) first = true;
        Lines ends;
        for (const str& l : dd(*q.els))
            if (nb(l)) ends.append(l);
        str after("");
        for (const str& l : lines.slice_from(p.end))
            if (nb(l)) { after = l; break; }
        auto r_ = py::re::fullmatch(R"re(\s*return (\w+);)re"_s, after);
        bool swallow = false;
        if (first && r_ && !ends.empty() &&
            py::re::fullmatch(py::percent(R"re(\s*%s = [^;]+;)re"_s, r_->group(1)), ends[-1])) {
            swallow = true;   // all(... for l in p[2][-1:]): the last line of the default arm, if any
            if (!p.els->empty()) {
                const str& l = (*p.els)[-1];
                bool ok1 = py::re::fullmatch(py::percent(R"re(\s*(?:%s = [^;]+;|))re"_s, r_->group(1)), l).has_value();
                swallow = ok1 || !nb(l);
            }
        }
        if (swallow) {
            auto sub_ = [&](const Lines& ls) {
                Lines out;
                for (const str& l : ls)
                    out.append(py::re::sub(py::percent(R"re(^(\s*)%s = (.*);$)re"_s, r_->group(1)), R"re(\1return \2;)re"_s, l));
                return out;
            };
            Lines nw;
            nw.append(py::percent("%sswitch (%s) {"_s, ind, a3));
            nw.extend(sub_(cases.slice(0, -1)));
            nw.append(ind + "}"_s);
            nw.extend(sub_(dd(*p.els)));
            lines.set_slice(k, p.end, nw);
            std::optional<int64_t> k2;
            for (int64_t j = k + nw.size(); j < lines.size(); ++j)
                if (nb(lines[j])) { k2 = j; break; }
            if (!k2) throw py::StopIteration();
            lines.pop(*k2);   // (the return after: each way returns its own)
            return range_switches(join_nl(lines));
        }
        Lines nw;
        if (first && !ends.empty() && py::re::match(R"re(\s*return\b)re"_s, ends[-1])) {
            nw.append(py::percent("%sswitch (%s) {"_s, ind, a3));
            nw.extend(cases.slice(0, -1));
            nw.append(ind + "}"_s);
            nw.extend(dd(*p.els));
        } else {
            nw.append(py::percent("%sswitch (%s) {"_s, ind, a3));
            nw.extend(first ? cases + dflt : dflt + cases);
            nw.append(ind + "}"_s);
        }
        lines.set_slice(k, p.end, nw);
        return range_switches(join_nl(lines));
    }
    return text;
}

// `return r;` at the end, every way to it giving r a value last (an if's arms, nested): each of those
// a return of its value, the last return gone - the compiler's jumps to the one exit read back
py::str push_returns(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 2; ++k) {   // (an if's arm ending r = ax, r a register variable: r's)
        auto m = py::re::fullmatch(R"re(([ \t]*)ax = ([^;\n]+);)re"_s, lines[k]);
        if (!m ||
            (lines[k + 1] != py::percent("%s%s = ax;"_s, m->group(1), "si"_s) &&
             lines[k + 1] != py::percent("%s%s = ax;"_s, m->group(1), "di"_s)) ||
            !py::re::fullmatch(R"re([ \t]*\}(?: else \{)?)re"_s, lines[k + 2]))
            continue;
        int64_t ind = m->group(1).size() - 4;
        std::optional<int64_t> op;
        for (int64_t j = k - 1; j >= 0; --j)
            if (nb(lines[j]) && indent_of(lines[j]) == ind) { op = j; break; }
        if (!op || !py::re::match(R"re([ \t]*(?:if \(.*\) \{|\} else \{|else \{)$)re"_s, lines[*op])) continue;
        str r = lines[k + 1].strip().slice(0, 2);
        str l0 = py::percent("%s%s = %s;"_s, m->group(1), r, m->group(2));
        str l1 = py::percent("%sax = %s;"_s, m->group(1), r);
        lines[k] = l0;
        lines[k + 1] = l1;
    }
    lines = _else_ifs(lines);
    std::optional<int64_t> end;
    for (int64_t k = 0; k < lines.size(); ++k)
        if (py::re::fullmatch(R"re(    return (ax|bx|cx|dx|si|di);)re"_s, lines[k])) end = k;
    if (!end || *end + 1 >= lines.size() || lines[*end + 1] != "}") return text;
    const int64_t endi = *end;
    str reg = py::re::fullmatch(R"re(\s*return (\w+);)re"_s, lines[endi])->group(1);
    const str regassign = py::percent(R"re(\s*%s = [^;]+;)re"_s, reg);
    std::set<int64_t> breaks;   // (a switch's breaks after the values become returns)
    int64_t leaves = 0;
    using Idx = std::optional<std::vector<int64_t>>;
    std::function<Idx(int64_t, int64_t)> tail;
    tail = [&](int64_t lo, int64_t hi) -> Idx {   // (the statements lines[lo:hi] end giving reg a value:
        int64_t k = hi - 1;                        //  [indexes of those assignments], or None)
        while (k >= lo && !nb(lines[k])) k -= 1;
        if (k < lo) return std::nullopt;
        if (py::re::fullmatch(regassign, lines[k])) return std::vector<int64_t>{k};
        if (py::re::fullmatch(R"re(\s*return\b[^;]*;)re"_s, lines[k]) && k != endi) {
            leaves += 1;   // (an arm returning its own value: nothing to give)
            return std::vector<int64_t>{};
        }
        if (lines[k].strip() != "}") return std::nullopt;
        std::optional<int64_t> sw;
        for (int64_t j = k - 1; j >= lo; --j)
            if (nb(lines[j]) && !py::re::match(R"re(\s*(?:case [^:]+|default):$)re"_s, lines[j]) &&
                indent_of(lines[j]) == indent_of(lines[k])) {
                sw = j;
                break;
            }
        if (sw && py::re::match(R"re(\s*switch \(.*\) \{$)re"_s, lines[*sw])) {   // (a switch each arm of which gives
            std::vector<int64_t> got, arm;                                         //  reg and breaks, a default among them)
            bool ok_ = true;
            for (int64_t j = *sw + 1; j <= k; ++j) {
                if (py::re::match(R"re(\s*(?:case [^:]+|default):$)re"_s, lines[j]) || j == k) {
                    if (!arm.empty()) {
                        std::vector<int64_t> idx;
                        for (int64_t x : arm)
                            if (nb(lines[x])) idx.push_back(x);
                        Idx sub;
                        if (idx.size() >= 2 && lines[idx.back()].strip() == "break;" &&
                            lines[idx[idx.size() - 2]].strip() == "}")
                            sub = tail(idx[0], idx.back());   // (an arm ending in an if-tree giving it)
                        if (idx.size() >= 2 && lines[idx.back()].strip() == "break;" &&
                            py::re::fullmatch(regassign, lines[idx[idx.size() - 2]])) {
                            got.push_back(idx[idx.size() - 2]);
                            breaks.insert(idx.back());
                        } else if (sub && !sub->empty()) {
                            got.insert(got.end(), sub->begin(), sub->end());
                            breaks.insert(idx.back());
                        } else {
                            ok_ = false;
                        }
                    }
                    arm.clear();
                } else {
                    arm.push_back(j);
                }
            }
            if (ok_ && !got.empty()) return got;   // (no default: the function falls off its end there,
            return std::nullopt;                   //  as the source does)
        }
        std::optional<int64_t> start;
        for (int64_t j = k - 1; j >= lo; --j)
            if (py::re::match(R"re(\s*if \(.*\) \{$)re"_s, lines[j]) && indent_of(lines[j]) == indent_of(lines[k])) {
                start = j;
                break;
            }
        if (!start) return std::nullopt;
        auto p = parts(lines, *start);
        if (!p || p->end != k + 1 || !p->els) return std::nullopt;
        int64_t then_lo = *start + 1;
        int64_t then_hi = then_lo + p->then.size();
        Idx a = tail(then_lo, then_hi);
        Idx b = tail(then_hi + 1, k);
        if (a && b) {
            std::vector<int64_t> r = *a;
            r.insert(r.end(), b->begin(), b->end());
            return r;
        }
        return std::nullopt;
    };
    Idx ks = tail(0, endi);
    if (!ks || ks->empty()) {   // (a switch last, each arm giving it: case ..: return X;)
        int64_t k = endi - 1;
        if (lines[k].strip() == "}") {
            str ind = lines[k].slice(0, lines[k].size() - lines[k].lstrip().size());
            std::optional<int64_t> st;
            for (int64_t j = k - 1; j >= 0; --j)
                if (lines[j].startswith(ind + "switch ("_s) && lines[j].endswith("{"_s)) { st = j; break; }
            if (st) {
                std::vector<int64_t> arm;
                bool ok = true;
                std::vector<std::pair<int64_t, std::optional<int64_t>>> rets;
                for (int64_t j = *st + 1; j <= k; ++j) {
                    str l = lines[j];
                    if (py::re::match(R"re(\s*(?:case [^:]+|default):$)re"_s, l) || j == k) {
                        if (!arm.empty()) {
                            std::vector<int64_t> idx;
                            for (int64_t x : arm)
                                if (nb(lines[x])) idx.push_back(x);
                            if (idx.size() >= 2 && lines[idx.back()].strip() == "break;" &&
                                py::re::fullmatch(regassign, lines[idx[idx.size() - 2]])) {
                                rets.push_back({idx[idx.size() - 2], idx.back()});
                            } else if (!idx.empty() && py::re::fullmatch(regassign, lines[idx.back()]) && j == k) {
                                rets.push_back({idx.back(), std::nullopt});
                            } else if (!idx.empty() && j != k &&
                                       !py::re::match(R"re(\s*(?:break|continue|return|goto)\b)re"_s, lines[idx.back()])) {
                                Lines joined;
                                for (int64_t x : idx) joined.append(lines[x]);
                                if (wsearch(reg, py::join(" "_s, joined))) ok = false;
                                // else: an arm falling into the next: that one returns
                            } else {
                                ok = false;
                            }
                        }
                        arm.clear();
                    } else {
                        arm.push_back(j);
                    }
                }
                if (ok && !rets.empty()) {
                    std::set<int64_t> drop;
                    for (const auto& ab : rets) {
                        lines[ab.first] = py::re::sub(py::percent(R"re(^(\s*)%s = (.*);$)re"_s, reg), R"re(\1return \2;)re"_s,
                                                      lines[ab.first]);
                        if (ab.second) drop.insert(*ab.second);
                    }
                    Lines kept;
                    for (int64_t j = 0; j < lines.size(); ++j)
                        if (!drop.count(j) && j != endi) kept.append(lines[j]);
                    return join_nl(kept);
                }
                // (each `r = V; break;` anywhere in the arms, outside loops of their own, a return of V; when no
                //  break is left and every arm ends in a return or falls on, the last return is gone)
                std::vector<std::pair<int64_t, std::optional<int64_t>>> spans;
                for (int64_t j = *st + 1; j < k; ++j)
                    if (py::re::match(R"re(\s*(?:while|for|do|switch)\b.*\{$)re"_s, lines[j]))
                        spans.push_back({j, _block_end(lines, j)});
                std::vector<int64_t> pairs;
                for (int64_t j = *st + 1; j < k - 1; ++j) {
                    if (py::re::fullmatch(regassign, lines[j]) && lines[j + 1].strip() == "break;") {
                        bool inside = false;
                        for (const auto& sp : spans) {
                            int64_t bb = (sp.second && *sp.second != 0) ? *sp.second : k;
                            if (sp.first < j && j < bb) inside = true;
                        }
                        if (!inside) pairs.push_back(j);
                    }
                }
                if (!pairs.empty()) {
                    OLines nw = opt_of(lines);
                    for (int64_t j : pairs) {
                        nw[j] = py::re::sub(py::percent(R"re(^(\s*)%s = (.*);$)re"_s, reg), R"re(\1return \2;)re"_s, *nw[j]);
                        nw[j + 1] = std::nullopt;
                    }
                    Lines rest;
                    for (int64_t j = *st + 1; j < k && j < nw.size(); ++j)
                        if (nw[j]) rest.append(*nw[j]);
                    bool left = false;
                    for (const str& x : rest)
                        if (x.strip() == "break;") left = true;
                    if (!left && wsearch(reg, join_nl(rest))) left = true;
                    str last = last_nonblank(rest);
                    if (!left && py::re::match(R"re(\s*return\b)re"_s, last)) {
                        nw[endi] = std::nullopt;
                        return join_opt(nw);
                    }
                }
            }
        }
        return text;
    }
    if (static_cast<int64_t>(ks->size()) + leaves < 2) return text;
    OLines out = opt_of(lines);
    for (int64_t k : *ks)
        out[k] = py::re::sub(py::percent(R"re(^(\s*)%s = (.*);$)re"_s, reg), R"re(\1return \2;)re"_s, *out[k]);
    out[endi] = std::nullopt;
    for (int64_t b_ : breaks) out[b_] = std::nullopt;
    return join_opt(out);
}

// if (A) { ..; r = E1; } else if (B) { ..; r = E2; } .. return r;  -> each arm returns its value (with no
// last else the function falls off its end there, as its source did)
py::str chain_returns(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)if \(.*\) \{\s*$)re"_s, lines[k]);
        if (!m) continue;
        str ind = m->group(1);
        std::vector<int64_t> arms_end;   // (the closing line of each arm)
        int64_t j = k + 1;
        while (j < lines.size()) {
            str x = lines[j];
            if ((x.startswith(ind + "} else if ("_s) && x.endswith("{"_s)) || x == ind + "} else {"_s) {
                arms_end.push_back(j);
            } else if (x == ind + "}"_s) {
                arms_end.push_back(j);
                break;
            } else if (x.startswith(ind) && !x.startswith(ind + " "_s) && nb(x)) {
                break;
            }
            j += 1;
        }
        if (arms_end.empty() || lines[arms_end.back()] != ind + "}"_s || arms_end.back() + 1 >= lines.size()) continue;
        auto ret = py::re::fullmatch(R"re(\s*return (ax|bx|cx|dx|si|di);\s*)re"_s, lines[arms_end.back() + 1]);
        if (!ret || arms_end.size() < 2) continue;
        str reg = ret->group(1);
        std::vector<int64_t> lasts;
        for (int64_t e : arms_end) lasts.push_back(e - 1);
        bool all_ = true;
        for (int64_t q : lasts)
            if (!py::re::fullmatch(py::percent(R"re(\s*%s = .+;\s*)re"_s, reg), lines[q])) { all_ = false; break; }
        if (!all_) continue;
        OLines out = opt_of(lines);
        for (int64_t q : lasts)
            out[q] = py::re::sub(py::percent(R"re(^(\s*)%s = (.+);\s*$)re"_s, reg), R"re(\1return \2;)re"_s, *out[q]);
        out[arms_end.back() + 1] = std::nullopt;
        return chain_returns(join_opt(out));
    }
    return text;
}

// al = E; ah = 0;  - a byte widened with zeros: ax = E, E an unsigned char (a global so declared, a field of a
// struct so laid out)
py::str zero_extends(const py::str& text_in) {
    OLines lines = opt_of(split_nl(text_in));
    auto vor = [&](int64_t i) -> str { return lines[i] ? *lines[i] : str(""); };
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        auto m = py::re::match(R"re((\s*)([abcd])[lx] = (.+);\s*$)re"_s, vor(k));   // (al, bl.. - or the word the model wrote)
        if (m && py::re::fullmatch(py::percent(R"re(\s*%sh = 0;\s*)re"_s, m->group(2)), vor(k + 1))) {
            str e = m->group(3);
            auto mk = py::re::fullmatch(py::percent(R"re(\(%sl & (\d+)\))re"_s, m->group(2)), e);   // (a byte masked first: al = X; al = (al & K))
            if (mk && k > 0 && lines[k - 1]) {
                auto p_ = py::re::fullmatch(py::percent(R"re(\s*%sl = (.+);)re"_s, m->group(2)), *lines[k - 1]);
                if (p_) {
                    e = py::percent("(%s & %s)"_s, p_->group(1), mk->group(1));
                    lines[k - 1] = std::nullopt;
                    auto f0 = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, p_->group(1));
                    if (f0) uchar_field(f0->group(1), py::int_from_str(f0->group(2)));
                }
            }
            lines[k] = py::percent("%s%sx = %s;"_s, m->group(1), m->group(2), e);
            lines[k + 1] = std::nullopt;
            auto g = py::re::fullmatch(R"re((g_[0-9a-f]{1,4}))re"_s, e);
            auto f = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, e);   // (through a field too: its own struct's)
            if (g) uchar_global(py::percent("g_%04x"_s, py::int_from_str(g->group(1).slice_from(2), 16)));
            else if (f) uchar_field(f->group(1), py::int_from_str(f->group(2)));
        }
    }
    str text = join_opt(lines);
    // (a byte stored, then widened with zeros and stored again: y = x = e, x an unsigned char)
    auto chained = [&](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), e = m.group(3), y = m.group(5);
        auto g = py::re::fullmatch(R"re(g_([0-9a-f]{1,4}))re"_s, x);
        auto f = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, x);
        if (g) uchar_global(py::percent("g_%04x"_s, py::int_from_str(g->group(1), 16)));
        else if (f) uchar_field(f->group(1), py::int_from_str(f->group(2)));
        return py::percent("%s%s = %s = %s;"_s, ind, y, x, e);
    };
    return py::re::sub(R"re((?m)^([ \t]*)([^=;\n]+?) = ([^;\n]+);\n[ \t]*([abcd])h = 0;\n[ \t]*([^=;\n]+?) = \4x;$)re"_s, chained, text);
}

// al given a byte while ah is still the 0 an earlier widening left (no call, no ah between): ax read then
// is that byte widened - al = X; R = ax  ->  R = X (X then an unsigned char)
py::str high_zero(const py::str& text) {
    OLines lines = opt_of(split_nl(text));
    bool zero = false;
    for (int64_t k = 0; k < lines.size(); ++k) {
        if (!lines[k]) continue;
        str l = *lines[k];
        if (py::re::fullmatch(R"re(\s*ah = 0;)re"_s, l)) {
            zero = true;
            continue;
        }
        if (py::re::search(R"re(\w\(|\bah\b|^\s*ax = |\b(?:if|while|for|switch)\b|[{}]|L_\w+:)re"_s, l)) {
            zero = !py::re::fullmatch(R"re(\s*\w+ = ax;)re"_s, l) ? false : zero;
            if (py::re::search(R"re(\w\(|\bah\b|^\s*ax = |[{}]|L_\w+:)re"_s, l)) zero = false;
            continue;
        }
        auto m = py::re::fullmatch(R"re((\s*)al = (.+);)re"_s, l);
        if (zero && m && k + 1 < lines.size()) {
            auto n = py::re::fullmatch(R"re((\s*)(\w+) = ax;)re"_s, *lines[k + 1]);
            if (n) {
                lines[k] = py::percent("%s%s = %s;"_s, n->group(1), n->group(2), m->group(2));
                lines[k + 1] = std::nullopt;
                auto f = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, m->group(2));
                if (f) uchar_field(f->group(1), py::int_from_str(f->group(2)));
            }
        }
    }
    return join_opt(lines);
}

// al = E; ... ax read before ax or al is given again, al never read: the byte widened for its use (cbw) -
// in C a char is an int where it is used: ax = E
py::str widen_bytes(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)(al|bl|cl|dl) = (.*);\s*$)re"_s, lines[k]);
        if (!m) continue;
        str b8 = m->group(2);
        str w = b8.slice(0, 1) + "x"_s;
        for (const str& x : lines.slice_from(k + 1)) {
            if (wsearch(b8, x)) break;
            if (wsearch(w, x)) {
                if (!py::re::match(py::percent(R"re(\s*%s = )re"_s, w), x))
                    lines[k] = py::percent("%s%s = %s;"_s, m->group(1), w, m->group(3));
                break;
            }
        }
    }
    return join_nl(lines);
}

// r = E(*p); p = p + n; X uses r once - the load and the step are *p++ in its place (n: the size it reads)
py::str post_increments(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        str l = lines[k];
        auto m = py::re::match(R"re((\s*)(ax|bx|cx|dx|si|di|al|bl|cl|dl|t\d+) = (.*);\s*$)re"_s, l);   // (a byte register too: a char;
        if (m && k + 2 < lines.size()) {                                                              //  the model's temporary)
            str reg = m->group(2), e = m->group(3);
            auto st = py::re::match(R"re(\s*(\w+) = \1 \+ (\d)\s*;\s*$)re"_s, lines[k + 1]);
            if (!st) st = py::re::match(R"re(\s*(\w+)\+\+()\s*;\s*$)re"_s, lines[k + 1]);
            if (st) {
                str p = st->group(1);
                str n_ = st->group_opt(2).value_or(str(""));
                int64_t n = n_.empty() ? 1 : py::int_from_str(n_);   // (p++: a step of one)
                auto deref = py::re::fullmatch(py::percent(R"re(\*\((?:unsigned )?(char|int) \*\)%s|\*%s)re"_s, p, p), e);
                std::optional<int64_t> size;
                if (deref && deref->group_opt(1) && *deref->group_opt(1) == "char") size = 1;
                else if (deref && deref->group_opt(1) && *deref->group_opt(1) == "int") size = 2;
                str nxt = lines[k + 2];
                str rest = join_nl(lines.slice_from(k + 3));
                if (deref && (!size || *size == n) && wcount(reg, nxt) == 1 &&
                    !wsearch(reg, py::re::split(py::percent(R"re(\n\s*%s = )re"_s, reg), rest)[0]) && !wsearch(p, nxt)) {
                    out.append(wsub_tpl(reg, py::percent("%s++"_s, e), nxt, 1));
                    k += 3;
                    continue;
                }
                // (r = p; p = p + 1; X reads r once: p's old value - an index stepped, a[n++])
                if (e == p && n == 1 && (one_of(reg, {"ax", "bx", "cx", "dx"}) || reg.startswith("t"_s)) &&
                    wcount(reg, nxt) == 1 && !wsearch(p, nxt) && !_live_after(opt_of(lines), k + 2, reg)) {
                    out.append(wsub_tpl(reg, py::percent("%s++"_s, p), nxt, 1));
                    k += 3;
                    continue;
                }
            }
            auto sd = py::re::match(R"re(\s*(\w+) = \1 - 1\s*;\s*$)re"_s, lines[k + 1]);
            if (!sd) sd = py::re::match(R"re(\s*(\w+)--\s*;\s*$)re"_s, lines[k + 1]);
            if (sd && e == sd->group(1) && (one_of(reg, {"ax", "bx", "cx", "dx"}) || reg.startswith("t"_s))) {   // (and p--: the
                str nxt = lines[k + 2];                                                                          //  old value)
                if (wcount(reg, nxt) == 1 && !wsearch(e, nxt) && !_live_after(opt_of(lines), k + 2, reg)) {
                    out.append(wsub_tpl(reg, py::percent("%s--"_s, e), nxt, 1));
                    k += 3;
                    continue;
                }
            }
        }
        out.append(l);
        k += 1;
    }
    return join_nl(out);
}

// r = E0; for (;;) { v = r; .. r = E1; continue; .. } - a register carried round a loop into a variable at its
// top (the model's phi): v itself - v = E0 before, v = E1 at each way round, the tests of r tests of v
py::str loop_registers(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 1; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)(?:for \(;;\)|while \(1\)) \{)re"_s, lines[k]);
        if (!m) continue;
        std::optional<int64_t> e = _block_end(lines, k);
        std::optional<Match> f;
        if (e && *e != 0) f = py::re::fullmatch(R"re([ \t]*(\w+) = (ax|bx|cx|dx);)re"_s, lines[k + 1]);
        auto pre = py::re::fullmatch(R"re(([ \t]*)(ax|bx|cx|dx) = ([^;]+);)re"_s, lines[k - 1]);
        if (!f || !pre || pre->group(2) != f->group(2)) continue;
        str v = f->group(1), r = f->group(2);
        Lines body = lines.slice(k + 2, *e);
        bool ok = true;
        for (int64_t j = 0; j < body.size(); ++j) {   // (r read only as v's value; given only before going round)
            if (py::re::match(py::percent(R"re([ \t]*%s = )re"_s, r), body[j])) {
                if (!(j + 1 < body.size() && body[j + 1].strip() == "continue;")) ok = false;
            }
        }
        if (!ok || _live_after(opt_of(lines), *e, r)) continue;
        Lines nw;
        nw.append(pre->group(1) + py::percent("%s = %s;"_s, v, pre->group(3)));
        nw.append(lines[k]);
        for (const str& x : body) nw.append(wsub_tpl(r, v, x));
        nw.extend(lines.slice(*e, *e + 1));
        return loop_registers(join_nl(lines.slice(0, k - 1) + nw + lines.slice_from(*e + 1)));
    }
    return text;
}

// x = K; if/while (.. x ..) at once - the test of what x was just given: of K (a variant: the compiler
// compares with the register it still holds K in)
std::optional<py::str> const_tests(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), k = m.group(3), kw = m.group(4), test = m.group(5);
        if (wcount(esc(x), test) != 1) return m.group(0);
        return py::percent("%s%s = %s;\n%s%s (%s) {"_s, ind, x, k, ind, kw, wsub_tpl(esc(x), k, test));
    };
    str t = py::re::sub(R"re((?m)^([ \t]*)(\w+) = (-?\d+);\n\1(if|while) \((.*)\) \{$)re"_s, one, text);
    Lines lines = split_nl(t);   // (and on past statements not giving it, to the first test of it in the block or one inside)
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)(\w+) = (-?\d+);)re"_s, lines[k]);
        if (!m) continue;
        str x = m->group(2), kv = m->group(3);
        int64_t ind = m->group(1).size();
        for (int64_t j = k + 1; j < lines.size(); ++j) {
            str y = lines[j];
            if ((nb(y) && indent_of(y) < ind) || py::re::match(R"re(\s*(?:L_\w+:|\}\s*else))re"_s, y)) break;
            if (py::re::search(py::percent(R"re((?<![\w>.])%s\s*(?:=[^=]|\+\+|--))re"_s, x), y) ||
                py::re::match(R"re(\s*(?:while|for|do)\b)re"_s, y))
                break;
            auto tm = py::re::fullmatch(R"re((\s*)if \((.*)\) \{)re"_s, y);
            if (tm && wsearch(x, tm->group(2))) {
                if (wcount(x, tm->group(2)) == 1)
                    lines[j] = py::percent("%sif (%s) {"_s, tm->group(1), wsub_tpl(x, kv, tm->group(2)));
                break;
            }
            if (wsearch(x, y)) break;
        }
    }
    t = join_nl(lines);
    if (t != text) return t;
    return std::nullopt;
}

// in a loop: if (C) { S; continue; } else { R } (R leaving: return, break) last in its body - the leaving
// test first: if (!C) { R } S (a variant, with its for when S ends by a step)
std::optional<py::str> loop_guards(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)(?:while \(.*\)|for \(.*\)) \{)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!e) continue;
        str i4 = m->group(1) + "    "_s;
        std::optional<int64_t> j0;
        for (int64_t j = k + 1; j < *e; ++j)
            if (lines[j].startswith(i4 + "if ("_s) && lines[j].endswith("{"_s)) j0 = j;
        std::optional<IP> p = j0 ? parts(lines, *j0) : std::nullopt;
        if (!p || !p->els || p->end != *e) continue;
        Lines th, el;
        for (const str& x : p->then)
            if (nb(x)) th.append(x);
        for (const str& x : *p->els)
            if (nb(x)) el.append(x);
        if (th.empty() || th[-1].strip() != "continue;" || el.empty() ||
            !py::re::match(R"re(\s*(?:return\b|break;))re"_s, el[-1]))
            continue;
        str neg = _truth(_not(py::percent("(%s)"_s, p->cond)));
        Lines nw;
        nw.append(py::percent("%sif (%s) {"_s, i4, neg));
        nw.extend(*p->els);
        nw.append(i4 + "}"_s);
        for (const str& x : th.slice(0, -1)) nw.append(x.slice_from(4));
        str t = join_nl(lines.slice(0, *j0) + nw + lines.slice_from(*e));
        return or_str(for_loops(t), t);
    }
    return std::nullopt;
}

// for (;;) { if (C) break; S } - the loop's test first: while (!C) { S }
py::str first_breaks(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)for \(;;\) \{)re"_s, lines[k]);
        std::optional<Match> b;
        if (m) b = py::re::fullmatch(R"re([ \t]*if (\(.*\)) break;)re"_s, lines[k + 1]);
        if (!b || !_balanced(b->group(1))) continue;
        str cond = _truth(_not(b->group(1)));
        Lines one;
        one.append(py::percent("%swhile %s {"_s, m->group(1),
                               (cond.startswith("("_s) && _balanced(cond.slice(1, -1))) ? cond : py::percent("(%s)"_s, cond)));
        lines.set_slice(k, k + 2, one);
        return first_breaks(join_nl(lines));
    }
    return text;
}

// while (C) { if (D) { S } else { break; } } - the loop's own test the two: while (C && D) { S } (and the
// other way about, if (D) { break; } S: while (C && !D) { S })
std::optional<py::str> loop_tests(const py::str& text) {
    Lines lines = split_nl(text);
    auto only = [](const Lines& ls, const char* what) {
        Lines nbs;
        for (const str& x : ls)
            if (nb(x)) nbs.append(x.strip());
        return nbs.size() == 1 && nbs[0] == what;
    };
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)while (\(.*\)) \{)re"_s, lines[k]);
        if (!m) continue;
        std::optional<int64_t> e = _block_end(lines, k);
        if (!e || lines[*e].strip() != "}") continue;
        std::optional<IP> p = parts(lines, k + 1);
        if (!p || p->end != *e) continue;
        str ind = m->group(1);
        Lines body;
        str d;
        if (p->els && only(*p->els, "break;")) {
            body = p->then;
            d = !p->cond.startswith("("_s) ? py::percent("(%s)"_s, p->cond) : p->cond;
        } else if (!p->els && only(p->then, "break;")) {
            continue;   // (if (D) break; and nothing after: no body to keep)
        } else {
            continue;
        }
        str cond = _conj("&&"_s, _truth(m->group(2)), _truth(d));
        Lines nw;
        nw.append(py::percent("%swhile (%s) {"_s, ind, cond));
        for (const str& x : body) nw.append(x.startswith(ind + "        "_s) ? x.slice_from(4) : x);
        nw.append(py::percent("%s}"_s, ind));
        str t = join_nl(lines.slice(0, k) + nw + lines.slice_from(*e + 1));
        return or_str(for_loops(t), t);   // (and its step a for's, when it has one)
    }
    return std::nullopt;
}

// goto L; with L: in another arm, a few plain statements to that arm's end after it - the compiler's one
// copy of a tail two arms share: the statements in place of the goto (the value it carried given just before)
std::optional<py::str> goto_tails(const py::str& text, int64_t most) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)goto (L_\w+);)re"_s, lines[k]);
        if (!m) continue;
        std::optional<int64_t> at;
        for (int64_t j = 0; j < lines.size(); ++j)
            if (lines[j].strip() == m->group(2) + ":"_s) { at = j; break; }
        bool fall = false;
        if (at && *at > k) {
            fall = true;
            for (const str& x : lines.slice(k + 1, *at))
                if (!(!nb(x) || x.strip().endswith(":"_s))) { fall = false; break; }
        }
        if (!at || fall) continue;   // (a goto that only falls there: fall_gotos')
        int64_t ind = indent_of(lines[*at]);
        Lines tail;
        bool tail_none = false;
        int64_t j = *at + 1;
        while (j < lines.size() && nb(lines[j]) && indent_of(lines[j]) == ind) {
            str x = lines[j].strip();
            if (x.endswith("{"_s) || (x.endswith(":"_s) && !py::re::fullmatch(R"re(L_\w+:)re"_s, x)) ||
                py::re::match(R"re((?:case |default))re"_s, x)) {
                tail_none = true;
                break;
            }
            if (py::re::fullmatch(R"re(L_\w+:)re"_s, x)) {
                j += 1;
                continue;
            }
            tail.append(x);
            j += 1;
        }
        if (tail_none || tail.empty() || tail.size() > most || j >= lines.size() || indent_of(lines[j]) >= ind) continue;
        bool jumpy = false;
        for (const str& x : tail)
            if (py::re::match(R"re((?:goto|break|continue|return)\b)re"_s, x)) jumpy = true;
        if (jumpy) continue;
        Lines rep;
        for (const str& x : tail) rep.append(m->group(1) + x);
        lines.set_slice(k, k + 1, rep);
        str t = join_nl(lines);
        py::set<str> used;
        for (const str& u : py::re::findall(R"re(goto (L_\w+);)re"_s, t)) used.add(u);
        Lines keep;
        for (const str& x : split_nl(t)) {
            bool drop = py::re::fullmatch(R"re([ \t]*(L_\w+):)re"_s, x) && !used.contains(x.strip().slice(0, -1));
            if (!drop) keep.append(x);
        }
        return join_nl(keep);
    }
    return std::nullopt;
}

// ax = E; r = ax; switch (ax) - the selector an assignment: switch (r = E); and F(..); if (.. ax ..) - a call
// whose value the next test reads: the call in the test; ((x + 1) == 0) as (x == -1)
py::str call_tests(const py::str& text_in) {
    str text = py::re::sub(R"re((?m)^([ \t]*)ax = ([^;\n]+);\n\1(\w+) = ax;\n\1switch \(ax\) \{$)re"_s,
                           [&](const Match& m) -> str {
                               return py::percent("%sswitch (%s = %s) {"_s, m.group(1), m.group(3), m.group(2));
                           },
                           text_in);
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), call = m.group(2), test = m.group(3);
        if (wcount("ax"_s, test) != 1) return m.group(0);
        return py::percent("%sif %s {"_s, ind, wsub_lit("ax"_s, call, test));
    };
    text = py::re::sub(R"re((?m)^([ \t]*)([A-Za-z_]\w*\([^;\n]*\));\n\1if (\(.*\)) \{$)re"_s, one, text);
    return py::re::sub(R"re(\(\(([^()]+|[A-Za-z_]\w*\([^()]*\)) \+ 1\) (==|!=) 0\))re"_s, R"re((\1 \2 -1))re"_s, text);
}

// a switch arm ending with what the next arm is, word for word, and leaves (return, break): that tail gone -
// it falls into the next (the compiler's one copy, the model's two)
py::str fall_dups(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)switch \(.*\) \{)re"_s, lines[k]);
        if (!m) continue;
        str ind = m->group(1);
        std::optional<int64_t> end;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!end) continue;
        const str lab = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
        std::vector<int64_t> heads;
        for (int64_t j = k + 1; j < *end; ++j)
            if (py::re::fullmatch(lab, lines[j])) heads.push_back(j);
        std::vector<std::pair<int64_t, int64_t>> arms;
        for (size_t i = 0; i < heads.size(); ++i) {
            int64_t stop = i + 1 < heads.size() ? heads[i + 1] : *end;
            int64_t j = heads[i];
            while (j + 1 < stop && py::re::fullmatch(lab, lines[j + 1])) j += 1;
            if (j + 1 < stop) arms.push_back({j + 1, stop});
        }
        for (size_t i = 0; i + 1 < arms.size(); ++i) {
            int64_t a0 = arms[i].first, a1 = arms[i].second, b0 = arms[i + 1].first, b1 = arms[i + 1].second;
            int64_t nb_ = b1 - b0;
            Lines body_b = lines.slice(b0, b1);
            if (body_b.empty() || !py::re::match(R"re(\s*(?:return\b|break;))re"_s, body_b[-1]) || a1 - a0 <= nb_ ||
                !(lines.slice(a1 - nb_, a1) == body_b))
                continue;
            bool lbl = false;
            for (const str& x : body_b)
                if (py::re::fullmatch(R"re(\s*L_\w+:)re"_s, x)) lbl = true;
            if (lbl) continue;
            lines.set_slice(a1 - nb_, a1, Lines());
            return fall_dups(join_nl(lines));
        }
    }
    return text;
}

// goto L; where what comes after it, past the ends of ifs and a switch's case labels, is L: itself - the
// goto only falls there (a case falling into the next): gone, and L with it when nothing else goes there
py::str fall_gotos(const py::str& text) {
    OLines lines = opt_of(split_nl(text));
    auto orempty = [&](int64_t i) -> str { return lines[i] ? *lines[i] : str(""); };
    auto blanked = [&]() {
        Lines out;
        for (int64_t i = 0; i < lines.size(); ++i) out.append(orempty(i));
        return out;
    };
    auto opener = [&](int64_t j) -> std::optional<int64_t> {   // (the line opening the block line j closes)
        int64_t ind = indent_of(*lines[j]);
        for (int64_t q = j - 1; q >= 0; --q)
            if (lines[q] && nb(*lines[q]) && indent_of(*lines[q]) == ind && lines[q]->rstrip().endswith("{"_s)) return q;
        return std::nullopt;
    };
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            auto m = py::re::fullmatch(R"re([ \t]*goto (L_\w+);)re"_s, orempty(k));
            if (!m) continue;
            int64_t j = k + 1;
            while (j < lines.size()) {
                str x = orempty(j).strip();
                if (!lines[j] || x.empty()) {
                    j += 1;
                    continue;
                }
                if (x == "}") {
                    std::optional<int64_t> q = opener(j);
                    if (!q || !py::re::match(R"re([ \t]*(?:if \(.*\)|\} else|else(?: if \(.*\))?) \{$)re"_s, *lines[*q])) break;
                    j += 1;
                    continue;
                }
                if (py::re::fullmatch(R"re(case [^:]+:|default:)re"_s, x)) {
                    j += 1;
                    continue;
                }
                break;
            }
            if (j < lines.size() && orempty(j).strip() == m->group(1) + ":"_s) {
                lines[k] = std::nullopt;
                changed = true;
                break;
            }
            auto after = [&](int64_t q) -> std::optional<int64_t> {   // (where the flow goes from line q on: past ends of ifs
                while (q < lines.size()) {                             //  and the else arms beside them, to a statement)
                    str x = orempty(q).strip();
                    if (!lines[q] || x.empty() || py::re::fullmatch(R"re(L_\w+:)re"_s, x)) {
                        q += 1;
                    } else if (x == "}") {
                        std::optional<int64_t> o = opener(q);
                        if (!o || !py::re::match(R"re([ \t]*(?:if \(.*\)|\} else|else) \{$)re"_s, *lines[*o])) return std::nullopt;
                        q += 1;
                    } else if (x == "} else {") {
                        std::optional<int64_t> be = _block_end(blanked(), q);
                        q = (be && *be != 0) ? *be : lines.size();
                    } else {
                        return q;
                    }
                }
                return std::nullopt;
            };
            std::optional<int64_t> at;
            for (int64_t q = 0; q < lines.size(); ++q)
                if (lines[q] && lines[q]->strip() == m->group(1) + ":"_s) { at = q; break; }
            if (at) {
                std::optional<int64_t> a1 = after(k + 1);
                if (a1 && a1 == after(*at + 1)) {
                    lines[k] = std::nullopt;   // (the goto goes where its own way goes anyway)
                    changed = true;
                    break;
                }
            }
            // (if (C) { goto L; } else { S } with L just after: if (!C) { S }, falling into L)
            std::optional<Match> c;
            if (k) c = py::re::fullmatch(R"re(([ \t]*)if (\(.*\)) \{)re"_s, orempty(k - 1));
            if (c && k + 1 < lines.size() && lines[k + 1] && *lines[k + 1] == c->group(1) + "} else {"_s) {
                std::optional<int64_t> e = _block_end(blanked(), k + 1);
                if (e && lines[*e]->strip() == "}") {
                    j = *e + 1;
                    while (j < lines.size() && py::re::fullmatch(R"re([ \t]*(?:case [^:]+|default):)re"_s, orempty(j))) j += 1;
                    if (j < lines.size() && orempty(j).strip() == m->group(1) + ":"_s) {
                        str nn = _strip(_not(c->group(2)));
                        lines[k - 1] = py::percent("%sif (%s) {"_s, c->group(1),
                                                   nn.startswith("("_s) ? nn : py::percent("(%s)"_s, _not(c->group(2))));
                        lines[k] = std::nullopt;
                        lines[k + 1] = std::nullopt;
                        changed = true;
                        break;
                    }
                }
            }
        }
    }
    py::set<str> used;
    for (const str& u : py::re::findall(R"re(goto (L_\w+);)re"_s, join_opt(lines))) used.add(u);
    Lines keep;
    for (int64_t i = 0; i < lines.size(); ++i) {
        if (!lines[i]) continue;
        bool drop = py::re::fullmatch(R"re([ \t]*(L_\w+):)re"_s, *lines[i]) && !used.contains(lines[i]->strip().slice(0, -1));
        if (!drop) keep.append(*lines[i]);
    }
    return join_nl(keep);
}

// if ((f(..) != 0)) { .. return ..; } then the arm's end, the function's value read after: ax is that call's
// value, 0 there - ax = 0; written (the model's value left in the register)
py::str tested_zero(const py::str& text) {
    Lines lines = split_nl(text);
    if (!py::re::search(R"re((?m)^[ \t]*return ax;$)re"_s, text)) return text;
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(([ \t]*)if \(\(([A-Za-z_]\w*\(.*\)) != 0\)\) \{)re"_s, lines[k]);
        std::optional<IP> p = m ? parts(lines, k) : std::nullopt;
        if (!p || p->els || p->end >= lines.size()) continue;
        Lines th;
        for (const str& x : p->then)
            if (nb(x)) th.append(x);
        if (th.empty() || !py::re::match(R"re(\s*return\b)re"_s, th[-1]) ||
            !py::re::fullmatch(R"re([ \t]*\}(?: else \{)?)re"_s, lines[p->end]))
            continue;
        lines.insert(p->end, m->group(1) + "ax = 0;"_s);
        return tested_zero(join_nl(lines));
    }
    return text;
}

// for (;;) { [x = E;] if (C1) continue; .. if (Cn) { leave } [else { continue; }] } - a loop of tests going
// round while one fails: while (!((x = E) .. && !C1 && .. Cn)) ; then leave (a return of a number, a break)
py::str continue_loops(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    auto eq = [](const Lines& a, std::initializer_list<const char*> b) {
        if (a.size() != static_cast<int64_t>(b.size())) return false;
        int64_t i = 0;
        for (const char* x : b)
            if (a[i++] != x) return false;
        return true;
    };
    while (k < lines.size()) {
        auto m = py::re::fullmatch(R"re(([ \t]*)for \(;;\) \{)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!m || !e || lines[*e].strip() != "}") {
            k += 1;
            continue;
        }
        Lines body;
        for (const str& x : lines.slice(k + 1, *e))
            if (nb(x)) body.append(x.strip());
        std::optional<Match> first;
        if (!body.empty()) first = py::re::fullmatch(R"re((\w+) = ([^;=]+);)re"_s, body[0]);
        int64_t q = first ? 1 : 0;
        Lines parts_;
        std::optional<str> leave;
        bool ok = true;
        while (q < body.size()) {
            auto c = py::re::fullmatch(R"re(if (\(.*\)) \{)re"_s, body[q]);
            auto c1 = py::re::fullmatch(R"re(if (\(.*\)) continue;)re"_s, body[q]);
            if (c1) {
                parts_.append(_truth(_not(c1->group(1))));
                q += 1;
                continue;
            }
            if (c && eq(body.slice(q + 1, q + 3), {"continue;", "}"})) {
                parts_.append(_truth(_not(c->group(1))));
                q += 3;
                continue;
            }
            if (c && q + 2 < body.size() && py::re::fullmatch(R"re((?:return -?\d+|break);)re"_s, body[q + 1]) &&
                (eq(body.slice_from(q + 2), {"}"}) || eq(body.slice_from(q + 2), {"} else {", "continue;", "}"}))) {
                parts_.append(_truth(c->group(1)));
                leave = body[q + 1];
                q = body.size();
                break;
            }
            ok = false;
            break;
        }
        if (!ok || !leave || parts_.size() < 2) {
            k += 1;
            continue;
        }
        if (first) {
            str x = first->group(1), v = first->group(2);
            str p0 = parts_[0];
            if (wcount(x, p0) != 1) {
                k += 1;
                continue;
            }
            parts_[0] = wsub_lit(x, py::percent("(%s = %s)"_s, x, v), p0, 1);
        }
        Lines conds;
        for (const str& p : parts_)
            conds.append((py::re::fullmatch(R"re(!?[\w\->.\[\]]+|!?\(.*\))re"_s, p) && _balanced(p)) ? p : py::percent("(%s)"_s, p));
        str cond = py::join(" && "_s, conds);
        str ind = m->group(1);
        Lines nw;
        nw.append(py::percent("%swhile (!(%s))"_s, ind, cond));
        nw.append(py::percent("%s    ;"_s, ind));
        if (*leave != "break;") nw.append(py::percent("%s%s"_s, ind, *leave));
        lines.set_slice(k, *e + 1, nw);
        k += nw.size();
    }
    return join_nl(lines);
}

// for (;;) { [x = E;] if (C1) break; .. if (Cn) { break; } [else { continue; }] } - a loop of tests alone
// (its value given first): while ((x = E) && !C1 .. && !Cn) ; - the first test x's zero
py::str empty_loops(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    auto eq = [](const Lines& a, std::initializer_list<const char*> b) {
        if (a.size() != static_cast<int64_t>(b.size())) return false;
        int64_t i = 0;
        for (const char* x : b)
            if (a[i++] != x) return false;
        return true;
    };
    while (k < lines.size()) {
        auto m = py::re::match(R"re(([ \t]*)for \(;;\) \{$)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!m || !e || lines[*e].strip() != "}") {
            k += 1;
            continue;
        }
        Lines body;
        for (const str& x : lines.slice(k + 1, *e)) body.append(x.strip());
        Lines conds;
        std::optional<std::tuple<str, str>> first;
        int64_t q = 0;
        std::optional<Match> a;
        if (!body.empty()) a = py::re::fullmatch(R"re((\w+) = ([^;=]+);)re"_s, body[0]);
        if (a) {
            first = std::make_tuple(a->group(1), a->group(2));
            q = 1;
        }
        bool ok = true;
        while (q < body.size()) {
            auto t = py::re::fullmatch(R"re(if (\(.*\)) break;)re"_s, body[q]);
            if (t) {
                conds.append(t->group(1));
                q += 1;
                continue;
            }
            t = py::re::fullmatch(R"re(if (\(.*\)) \{)re"_s, body[q]);
            if (t && eq(body.slice(q + 1, q + 3), {"break;", "}"})) {
                conds.append(t->group(1));
                q += 3;
                continue;
            }
            if (t && eq(body.slice(q + 1, q + 5), {"break;", "} else {", "continue;", "}"})) {
                conds.append(t->group(1));
                q += 5;
                continue;
            }
            if (body[q] == "continue;" && q == body.size() - 1) {
                q += 1;
                continue;
            }
            ok = false;
            break;
        }
        if (!ok && first && body.size() > 2 &&
            py::re::fullmatch(py::percent(R"re(if \(\((%s == 0)\)\) break;|if \(!%s\) break;)re"_s, std::get<0>(*first), std::get<0>(*first)),
                              body[1])) {
            // (for (;;) { x = E; if (!x) break; S } - the value tested first, a body after: while ((x = E)) { S })
            Lines one;
            one.append(py::percent("%swhile ((%s = %s)) {"_s, m->group(1), std::get<0>(*first), std::get<1>(*first)));
            lines.set_slice(k, k + 3, one);
            k += 1;
            continue;
        }
        bool unbalanced = false;
        for (const str& c : conds)
            if (!_balanced(c)) unbalanced = true;
        if (!ok || conds.empty() || unbalanced) {
            k += 1;
            continue;
        }
        Lines gs;
        for (const str& c : conds) gs.append(_truth(_not(c)));
        if (first) {
            str x = std::get<0>(*first), v = std::get<1>(*first);
            str c0 = _strip(conds[0]);
            if (c0 == py::percent("(%s == 0)"_s, x) || c0 == py::percent("!%s"_s, x)) {
                gs[0] = py::percent("(%s = %s)"_s, x, v);
            } else {
                k += 1;
                continue;
            }
        }
        Lines parts_;
        for (const str& c : gs)
            parts_.append((py::re::fullmatch(R"re([\w\->.\[\]]+|\(.*\))re"_s, c) &&
                           _balanced(c.startswith("("_s) ? c.slice(1, -1) : c))
                              ? c
                              : py::percent("(%s)"_s, c));
        str g = py::join(" && "_s, parts_);
        Lines nw;
        nw.append(py::percent("%swhile (%s)"_s, m->group(1), g));
        nw.append(py::percent("%s    ;"_s, m->group(1)));
        lines.set_slice(k, *e + 1, nw);
        k += 2;
    }
    return join_nl(lines);
}

// M = E; x = M; - a value stored and read straight back from where it went (E a call's: no other way to
// have it twice): x = M = E, the compiler's chained store (the value kept in the register)
py::str stored_reads(const py::str& text) {
    return py::re::sub(R"re((?m)^([ \t]*)(\w+(?:->f\d+)+) = ([^;\n]*\w\([^;\n]*\));\n\1(\w+) = \2;$)re"_s,
                       [&](const Match& m) -> str {
                           return py::percent("%s%s = %s = %s;"_s, m.group(1), m.group(4), m.group(2), m.group(3));
                       },
                       text);
}

// r = E; x = r; if ((r != 0)) - a value stored, then tested in the register still holding it: the
// assignment tested, if ((x = E)) (== 0: if (!(x = E))) - r read no more
py::str stored_tests(const py::str& text) {
    OLines lines = opt_of(split_nl(text));
    auto vor = [&](int64_t i) -> str { return lines[i] ? *lines[i] : str(""); };
    for (int64_t k = 0; k < lines.size() - 2; ++k) {
        auto m = py::re::match(R"re(([ \t]*)(ax|al|bx|cx|dx) = ([^;]+);$)re"_s, vor(k));
        if (!m) continue;
        str ind = m->group(1), r = m->group(2), e = m->group(3);
        auto a = py::re::match(py::percent(R"re(%s(\w+(?:->f\d+)*|\*\w+) = %s;$)re"_s, ind, r), vor(k + 1));
        auto t = py::re::match(py::percent(R"re(%s(if|while) \(\(%s ([!=]=) 0\)\)(.*)$)re"_s, ind, r), vor(k + 2));
        if (!a || !t || wsearch(r, a->group(1) + e + t->group(3)) || _live_after(lines, k + 2, r)) continue;
        str v = py::percent("(%s = %s)"_s, a->group(1), e);
        lines[k] = py::percent("%s%s (%s)%s"_s, ind, t->group(1),
                               t->group(2) == "!=" ? py::percent("(%s)"_s, v.slice(1, -1)) : "!"_s + v, t->group(3));
        lines[k + 1] = std::nullopt;
        lines[k + 2] = std::nullopt;
    }
    return join_opt(lines);
}

// the returns as a source with early exits writes them (a variant): an else after an arm that returns
// written after the if; a switch's default arm first, every path of it returning and every other arm too,
// written after the switch; a return reached at once again by an arm's end (through ifs' ends only) gone
std::optional<py::str> flat_returns(const py::str& text) {
    auto hsb = split_body(text);
    str head = std::get<0>(hsb), sep = std::get<1>(hsb), body = std::get<2>(hsb);
    if (sep.empty()) return std::nullopt;
    Lines lines = split_nl(body);
    Lines seen = lines.copy();
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {   // (an else after an arm that returns)
            str l = lines[k];
            if (l.strip() != "} else {") continue;
            str prev = last_nonblank(lines.slice(0, k));
            str ind = l.slice(0, l.size() - l.lstrip().size());
            if (!py::re::fullmatch(py::percent(R"re(%s    return\b[^;]*;)re"_s, esc(ind)), prev)) continue;
            std::optional<int64_t> op = _opener(lines, k);
            if (!op || !py::re::match(R"re(\s*if \(.*\) \{$)re"_s, lines[*op])) continue;
            std::optional<IP> p = parts(lines, *op);
            if (!p || !p->els) continue;
            Lines nw = lines.slice(0, k);
            nw.append(ind + "}"_s);
            for (const str& x : *p->els) nw.append(dedent4(x, ind));
            nw.extend(lines.slice_from(p->end));
            lines = nw;
            changed = true;
            break;
        }
        if (changed) continue;
        for (int64_t k = 0; k < lines.size(); ++k) {   // (a default arm first, every path returning)
            str l = lines[k];
            auto m = py::re::match(R"re((\s*)switch \(.*\) \{$)re"_s, l);
            if (!m) continue;
            str ind = m->group(1);
            std::optional<int64_t> end;
            for (int64_t j = k + 1; j < lines.size(); ++j)
                if (lines[j] == ind + "}"_s) { end = j; break; }
            if (!end || lines[k + 1] != ind + "default:"_s) continue;
            const str lab = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
            std::vector<int64_t> labels;
            for (int64_t j = k + 1; j < *end; ++j)
                if (py::re::fullmatch(lab, lines[j])) labels.push_back(j);
            std::vector<std::pair<int64_t, int64_t>> arms;
            for (size_t i = 0; i < labels.size(); ++i) arms.push_back({labels[i], i + 1 < labels.size() ? labels[i + 1] : *end});
            std::vector<str> ends;
            for (const auto& ab : arms) ends.push_back(last_nonblank(lines.slice(ab.first + 1, ab.second)));
            bool all_ret = true;
            for (const str& e : ends)
                if (!e.empty() && !py::re::fullmatch(py::percent(R"re(%s    return\b[^;]*;)re"_s, esc(ind)), e)) all_ret = false;
            bool any_brk = false;
            for (const str& x : lines.slice(k + 1, *end))
                if (py::re::match(R"re(\s*(?:break|continue);)re"_s, x)) any_brk = true;
            if (arms.size() < 2 || ends[0].empty() || !all_ret || any_brk) continue;
            int64_t d0 = arms[0].first, d1 = arms[0].second;
            Lines nw = lines.slice(0, k + 1);
            nw.extend(lines.slice(d1, *end + 1));
            for (const str& x : lines.slice(d0 + 1, d1)) nw.append(dedent4(x, ind));
            nw.extend(lines.slice_from(*end + 1));
            lines = nw;
            changed = true;
            break;
        }
        if (changed) continue;
        for (int64_t k = 0; k < lines.size(); ++k) {   // (a return the arm's end reaches at once again)
            auto m = py::re::fullmatch(R"re(\s*(return\b[^;]*;))re"_s, lines[k]);
            if (!m) continue;
            int64_t j = k + 1;
            bool ok = true;
            while (j < lines.size() && lines[j].strip() == "}") {
                std::optional<int64_t> op = _opener(lines, j);
                if (!op || !py::re::match(R"re(\s*(?:if \(.*\) \{|\} else \{)$)re"_s, lines[*op])) {
                    ok = false;
                    break;
                }
                j += 1;
            }
            if (ok && j > k + 1 && j < lines.size() && lines[j].strip() == m->group(1) &&
                !(lines[k - 1].strip() == "{" || lines[k - 1].strip() == "} else {") &&
                !py::re::match(R"re(\s*(?:if|else)\b.*\{$)re"_s, lines[k - 1])) {
                lines = lines.slice(0, k) + lines.slice_from(k + 1);
                changed = true;
                break;
            }
        }
    }
    if (lines == seen) return std::nullopt;
    return head + sep + join_nl(lines);
}

// switch (x) { default: L: S .. case K: .. goto L; .. } - the default arm the code after the switch (every
// way through S leaving, the other arms jumping to it): default: break; the gotos breaks, S after the switch
py::str switch_follow(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)switch \(.*\) \{$)re"_s, lines[k]);
        if (!m || k + 2 >= lines.size() || lines[k + 1] != m->group(1) + "default:"_s) continue;
        str ind = m->group(1);
        auto lab = py::re::fullmatch(py::percent(R"re(%s    (L_\w+):)re"_s, esc(ind)), lines[k + 2]);
        std::optional<int64_t> end;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!lab || !end) continue;
        const str casere = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
        std::optional<int64_t> nxt;
        for (int64_t j = k + 2; j < *end; ++j)
            if (py::re::fullmatch(casere, lines[j])) { nxt = j; break; }
        if (!nxt) continue;
        Lines body = lines.slice(k + 3, *nxt);
        str last = last_nonblank(body);
        std::vector<int64_t> gotos;
        for (int64_t j = *nxt; j < *end; ++j)
            if (lines[j].strip() == py::percent("goto %s;"_s, lab->group(1))) gotos.push_back(j);
        bool has_break = false;
        for (const str& x : body)
            if (py::re::fullmatch(py::percent(R"re(%s    break;)re"_s, esc(ind)), x)) has_break = true;
        bool in_loop = false;
        for (int64_t j : gotos)
            if (_in_loop(lines, k, j, ind.size())) in_loop = true;
        if (gotos.empty() || !py::re::fullmatch(py::percent(R"re(%s    return\b[^;]*;)re"_s, esc(ind)), last) || has_break ||
            in_loop)
            continue;
        Lines nw = lines.copy();
        for (int64_t j : gotos)
            nw[j] = nw[j].replace(py::percent("goto %s;"_s, lab->group(1)), "break;"_s);
        Lines rest = nw.slice(*nxt, *end + 1);
        Lines after;
        for (const str& x : body) after.append(dedent4(x, ind));
        Lines out = nw.slice(0, k + 1);
        out.append(ind + "default:"_s);
        out.append(ind + "    break;"_s);
        out.extend(rest);
        out.extend(after);
        out.extend(nw.slice_from(*end + 1));
        str t = join_nl(out);
        if (py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab->group(1)), t)) {
            Lines o2 = nw.slice(0, k + 1);
            o2.append(ind + "default:"_s);
            o2.append(ind + "    break;"_s);
            o2.extend(rest);
            o2.append(ind + lab->group(1) + ":"_s);
            o2.extend(after);
            o2.extend(nw.slice_from(*end + 1));
            t = join_nl(o2);
        }
        return switch_follow(t);
    }
    return text;
}

// switch (..) { .. case K: return V; } - the arms that only return written first (a variant: the compiler
// lays a switch's arms out from the source's order its own way)
std::optional<py::str> return_cases_first(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)switch \(.*\) \{$)re"_s, lines[k]);
        if (!m) continue;
        str ind = m->group(1);
        std::optional<int64_t> end;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!end) continue;
        const str lab = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
        std::vector<int64_t> labels;
        for (int64_t j = k + 1; j < *end; ++j)
            if (py::re::fullmatch(lab, lines[j])) labels.push_back(j);
        if (labels.empty() || labels[0] != k + 1) continue;
        std::vector<std::pair<int64_t, int64_t>> groups;   // (arms: their labels together, then the statements)
        size_t j = 0;
        while (j < labels.size()) {
            size_t a = j;
            while (j + 1 < labels.size() && labels[j + 1] == labels[j] + 1) j += 1;
            int64_t b = j + 1 < labels.size() ? labels[j + 1] : *end;
            groups.push_back({labels[a], b});
            j += 1;
        }
        const str retonly = py::percent(R"re(\s*(?:case [^:]+|default):|%s    return(?: -?\d+)?;)re"_s, esc(ind));
        std::vector<size_t> rets;
        for (size_t gi = 0; gi < groups.size(); ++gi) {
            bool any_stmt = false;
            for (const str& x : lines.slice(groups[gi].first, groups[gi].second))
                if (nb(x) && !py::re::fullmatch(CASELABELRE, x)) { any_stmt = true; break; }
            bool all_ret = true;
            for (const str& x : lines.slice(groups[gi].first, groups[gi].second))
                if (nb(x) && !py::re::fullmatch(retonly, x)) all_ret = false;
            if (any_stmt && all_ret) rets.push_back(gi);
        }
        if (rets.empty() || rets[0] == 0 || rets.size() == groups.size()) continue;
        bool falls = false;
        for (size_t gi : rets) {
            if (gi == 0) continue;   // (g in prev: not the first)
            Lines pv = lines.slice(groups[gi - 1].first, groups[gi - 1].second);
            if (!py::re::match(R"re(\s*(?:break|continue|return\b|goto\b))re"_s, last_nonblank(pv))) falls = true;
        }
        if (falls) continue;   // (an arm falling into it: not moved)
        Lines nw = lines.slice(0, k + 1);
        for (size_t gi : rets) nw.extend(lines.slice(groups[gi].first, groups[gi].second));
        for (size_t gi = 0; gi < groups.size(); ++gi) {
            bool is_ret = std::find(rets.begin(), rets.end(), gi) != rets.end();
            if (!is_ret) nw.extend(lines.slice(groups[gi].first, groups[gi].second));
        }
        nw.extend(lines.slice_from(*end));
        return join_nl(nw);
    }
    return std::nullopt;
}

// switch (x) { case K: .. f(ax) .. } - an arm reading the register the selector was compared in before
// anything gives it a value (an arm only the dispatch enters: the first, or after one that leaves): x itself
py::str selector_reads(const py::str& text) {
    Lines lines = split_nl(text);
    const str NUL(std::u32string(1, U'\0'));
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)switch \((.+)\) \{$)re"_s, lines[k]);
        if (!m || py::re::search(R"re(\w\s*\(|\+\+|--|[^=!<>]=[^=])re"_s, m->group(2)) ||
            py::re::search(R"re(\b(?:ax|al|ah)\b)re"_s, m->group(2)))
            continue;
        str ind = m->group(1), sel = m->group(2);
        int64_t j = k + 1;
        bool entered = true;   // (entered: only the dispatch reaches this arm)
        const str casere = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
        const str stopre = py::percent(R"re(%s(?:case [^:]+|default):|%s\})re"_s, esc(ind), esc(ind));
        while (j < lines.size() && lines[j] != ind + "}"_s) {
            str x = lines[j];
            if (py::re::fullmatch(casere, x)) {
                str prev = lines[k];
                for (int64_t i = j - 1; i >= k + 1; --i)
                    if (nb(lines[i])) { prev = lines[i]; break; }
                entered = py::re::fullmatch(CASELABELRE, prev)
                              ? entered
                              : (prev == lines[k] ||
                                 py::re::fullmatch(R"re(\s*(?:break|continue|return\b.*|goto \w+);)re"_s, prev).has_value());
                bool ok = entered;
                j += 1;
                auto bsel = py::re::fullmatch(R"re(\*(\w+))re"_s, sel);   // (a byte the selector: its al)
                while (ok && j < lines.size() && !py::re::fullmatch(stopre, lines[j])) {
                    str y = lines[j];
                    if (bsel && py::re::fullmatch(py::percent(R"re(\s*%s = %s \+ 1;)re"_s, bsel->group(1), bsel->group(1)), y) &&
                        j + 1 < lines.size() && wcount("al"_s, lines[j + 1]) == 1 && !wsearch(bsel->group(1), lines[j + 1])) {
                        lines[j + 1] = wsub_tpl("al"_s, py::percent("*%s++"_s, bsel->group(1)), lines[j + 1]);   // (read, then stepped:
                        lines[j] = NUL;                                                                        //  *p++)
                        break;
                    }
                    if (bsel && wcount("al"_s, y) == 1 && !py::re::search(R"re(\b(?:ax|ah)\b|\bal = )re"_s, y)) {
                        lines[j] = wsub_tpl("al"_s, sel, y);
                        break;
                    }
                    if (py::re::match(R"re(\s*L_\w+:)re"_s, y) || py::re::search(R"re(\b(?:al|ah)\b)re"_s, y)) break;
                    if (wsearch("ax"_s, y)) {
                        auto w = py::re::match(R"re((\s*ax = )(.*)$)re"_s, y);
                        if (w && !wsearch("ax"_s, w->group(2))) break;   // (given a value first: not the selector's)
                        if (w) lines[j] = w->group(1) + wsub_tpl("ax"_s, sel, w->group(2));
                        else if (py::re::match(R"re(\s*(?:if|while|switch|for)\b)re"_s, y) && !py::re::search(R"re(\w\s*\()re"_s, y)) break;
                        else lines[j] = wsub_tpl("ax"_s, sel, y);
                        break;
                    }
                    if (py::re::search(R"re(\w\s*\(|[{}])re"_s, y)) break;   // (a call, a block: the register may hold anything)
                    j += 1;
                }
                continue;
            }
            j += 1;
        }
    }
    Lines keep;
    for (const str& l : lines)
        if (l != NUL) keep.append(l);
    return join_nl(keep);
}

// [the text with one int field of a struct unsigned] - a variant each, for the fields shifted right or
// compared (<, <=, >, >=): the compiler's shr and unsigned jumps say unsigned
py::list<py::str> unsigned_fields(const py::str& text, int64_t most) {
    Lines out;
    struct Fld {
        str tag;
        int64_t a, b;
        std::optional<str> n;
    };
    std::vector<Fld> fields;
    for (const Match& s : py::re::finditer(R"re(struct (\w+) \{[^}]*\})re"_s, text)) {
        for (const Match& f : py::re::finditer(R"re((?<![\w ])int f(\d+);|(?<=[{;] )int f(\d+);)re"_s, s.group(0)))
            fields.push_back({s.group(1), s.start() + f.start(), s.start() + f.end(), f.group_opt(1)});
    }
    for (size_t i = 0; i < fields.size() && i < 16; ++i) {
        const Fld& fl = fields[i];
        str n = (fl.n && !fl.n->empty()) ? *fl.n : py::re::search(R"re(\d+)re"_s, text.slice(fl.a, fl.b))->group(0);
        Lines ptrs = py::re::findall(py::percent(R"re(struct %s \*\s*(\w+))re"_s, fl.tag), text);
        ptrs.extend(py::re::findall(py::percent(R"re(struct %s (\w+)\[)re"_s, fl.tag), text));
        bool any_ = false;
        for (const str& p : ptrs)
            if (py::re::search(py::percent(R"re(\b%s(?:\[[^\]]*\])?(?:->|\.)f%s\b\)? (?:>>|[<>]=?(?![<>])))re"_s, esc(p), n), text)) {
                any_ = true;
                break;
            }
        if (!any_) continue;
        out.append(text.slice(0, fl.a) + py::percent("unsigned f%s;"_s, n) + text.slice_from(fl.b));
        if (out.size() >= most) break;
    }
    return out;
}

// [the text with two next statements swapped] - assignments neither of which reads or gives what the other
// gives, no call in either (a variant each: the order the model wrote two independent stores in may not be
// the source's)
py::list<py::str> adjacent_swaps(const py::str& text, int64_t most) {
    Lines lines = split_nl(text);
    Lines out;
    const str rx = R"re(([ \t]*)([\w\->.\[\]*]+) = ([^;]+);$)re"_s;
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        auto a = py::re::fullmatch(rx, lines[k]);
        auto b = py::re::fullmatch(rx, lines[k + 1]);
        if (!a || !b || a->group(1) != b->group(1) || py::re::search(R"re(\w\s*\()re"_s, a->group(3) + b->group(3))) continue;
        str la = py::re::sub(R"re(->.*|\[.*)re"_s, ""_s, a->group(2)).lstrip("*"_s);
        str lb = py::re::sub(R"re(->.*|\[.*)re"_s, ""_s, b->group(2)).lstrip("*"_s);
        if (wsearch(esc(la), lines[k + 1]) || wsearch(esc(lb), lines[k])) continue;
        Lines nw = lines.copy();
        nw[k] = lines[k + 1];
        nw[k + 1] = lines[k];
        out.append(join_nl(nw));
        if (out.size() >= most) break;
    }
    return out;
}

// ((int *)((char *)p + N))[E] - words from N bytes into what an int *p points to: p[E + N/2] (a variant: the
// compiler folds the constant into the place, [bx+si+N])
py::str word_offsets(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str p = m.group(1), e = m.group(3);
        int64_t n = py::int_from_str(m.group(2));
        if (n % 2) return m.group(0);
        if (py::re::search(py::percent(R"re((?m)^[ \t]*(?:register )?int \*%s;)re"_s, esc(p)), text))
            return py::percent("%s[%s + %d]"_s, p, e, py::floordiv(n, 2));
        return py::percent("((int *)%s)[%s + %d]"_s, p, e, py::floordiv(n, 2));   // (another pointer: through an int * of it)
    };
    return py::re::sub(R"re(\(\(int \*\)\(\(char \*\)(\w+) \+ (\d+)\)\)\[([^\[\]]+)\])re"_s, one, text);
}

// *(int *)buf = E; - a buffer's first word given: its first byte (a variant: the store's width the model
// left to the type it guessed)
py::str byte_heads(const py::str& text) {
    return py::re::sub(R"re((?m)^([ \t]*)\*\(int \*\)(\w+) = ([^;]+);$)re"_s, R"re(\1\2[0] = \3;)re"_s, text);
}

// if (C) { A; T } else { T } - both arms ending in the same statements (the compiler's one copy of them,
// jumped to): if (C) { A } T (the else's alone the same: if (!C) { B } T)
py::str common_tails(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<IP> p = parts(lines, k);
        if (!p || !p->els) {
            k += 1;
            continue;
        }
        Lines a = p->then.copy(), b = p->els->copy();
        int64_t n = 0;
        while (n < std::min(a.size(), b.size()) && a[a.size() - 1 - n] == b[b.size() - 1 - n] && nb(a[a.size() - 1 - n])) n += 1;
        bool bad = false;
        if (n) {
            for (const str& x : a.slice_from(a.size() - n))
                if (py::re::match(R"re(\s*(?:L_\w+:|case\b|default:|\}|else\b))re"_s, x) || x.rstrip().endswith("{"_s)) bad = true;
        }
        if (!n || bad) {
            k += 1;
            continue;
        }
        str ind = lead(lines[k]);
        Lines tail;
        for (const str& x : a.slice_from(a.size() - n)) tail.append(dedent4(x, ind));
        Lines ra = a.slice(0, a.size() - n), rb = b.slice(0, b.size() - n);
        bool ra_any = false, rb_any = false;
        for (const str& x : ra) if (nb(x)) ra_any = true;
        for (const str& x : rb) if (nb(x)) rb_any = true;
        if (!ra_any && !rb_any) {
            k += 1;
            continue;
        }
        Lines nw;
        if (!rb_any) {
            nw.append(lines[k]);
            nw.extend(ra);
            nw.append(ind + "}"_s);
        } else if (!ra_any) {
            str neg = _not(py::percent("(%s)"_s, p->cond));
            nw.append(py::percent("%sif %s {"_s, ind,
                                  (neg.startswith("("_s) && _balanced(neg.slice(1, -1))) ? neg : py::percent("(%s)"_s, neg)));
            nw.extend(rb);
            nw.append(ind + "}"_s);
        } else {
            nw.append(lines[k]);
            nw.extend(ra);
            nw.append(ind + "} else {"_s);
            nw.extend(rb);
            nw.append(ind + "}"_s);
        }
        lines.set_slice(k, p->end, nw + tail);
        k += 1;
    }
    return join_nl(lines);
}

// if (C) { S; A } else { S; B } - both arms beginning with the same statement, no call in it and nothing C
// reads given by it: S; if (C) { A } else { B } (the model's copy of what the compiler did before the test)
py::str common_heads(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<IP> p = parts(lines, k);
        if (!p || !p->els) {
            k += 1;
            continue;
        }
        Lines a, b;
        for (const str& x : p->then) if (nb(x)) a.append(x);
        for (const str& x : *p->els) if (nb(x)) b.append(x);
        std::optional<Match> m;
        if (!a.empty() && !b.empty() && a[0].strip() == b[0].strip())
            m = py::re::fullmatch(R"re(\s*([\w\->.\[\]*]+) = ([^;]+);)re"_s, a[0]);
        if (!m || py::re::search(R"re(\w\s*\(|\+\+|--)re"_s, a[0]) ||
            wsearch(esc(py::re::sub(R"re(->.*|\[.*)re"_s, ""_s, m->group(1)).lstrip("*"_s)), p->cond)) {
            k += 1;
            continue;
        }
        str ind = lead(lines[k]);
        int64_t ia = p->then.index(a[0]), ib = p->els->index(b[0]);
        Lines nw;
        nw.append(ind + a[0].strip());
        nw.append(lines[k]);
        nw.extend(p->then.slice(0, ia));
        nw.extend(p->then.slice_from(ia + 1));
        nw.append(ind + "} else {"_s);
        nw.extend(p->els->slice(0, ib));
        nw.extend(p->els->slice_from(ib + 1));
        nw.append(ind + "}"_s);
        lines.set_slice(k, p->end, nw);
        k += 2;
    }
    return join_nl(lines);
}

// t = A; v = F(B); .. (G(t) op v) - two calls an operator joins, the compiler's order: the right one
// first into a temporary, the left one's argument kept from before it: (G(A) op F(B)) - a variant
std::optional<py::str> call_pairs(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), t = m.group(2), a = m.group(3), v = m.group(4), f = m.group(5), rest = m.group(6);
        auto g = py::re::search(py::percent(R"re(\b(\w+)\(%s\) ([-+*&|^]) %s\b)re"_s, t, v), rest);
        int64_t nv = wcount(v, text);
        if (!g || (nv != 3 && nv != 2)) return m.group(0);
        str nw = rest.slice(0, g->start()) + py::percent("%s(%s) %s %s"_s, g->group(1), a, g->group(2), f) + rest.slice_from(g->end());
        return ind + nw;
    };
    str t = py::re::sub(R"re((?m)^([ \t]*)(t\d+) = ([^;\n]+);\n[ \t]*(v\d+) = (\w+\([^;\n]*\));\n[ \t]*(.*)$)re"_s, one, text);
    if (t == text) return std::nullopt;
    for (const str& v : py::re::findall(R"re((?m)^[ \t]*int (v\d+);$)re"_s, t))   // (the temporary's local: gone)
        if (wcount(v, t) == 1) t = py::re::sub(py::percent(R"re((?m)^[ \t]*int %s;\n)re"_s, v), ""_s, t);
    for (const str& x : py::re::findall(R"re((?m)^[ \t]*int ((?:t\d+, )*t\d+);$)re"_s, t)) {
        Lines left;
        for (const str& y : py::split(x, ", "_s))
            if (wcount(y, t) > 1) left.append(y);
        t = py::re::sub(py::percent(R"re((?m)^([ \t]*)int %s;$)re"_s, esc(x)),
                        left.empty() ? str("") : py::percent(R"re(\1int %s;)re"_s, py::join(", "_s, left)), t);
    }
    return py::re::sub(R"re(\n\n+)re"_s, "\n"_s, t);
}

// *(C ? A : B) = E; - one store the compiler shares between two ways to its place: if (C) *A = E; else
// *B = E; (A, B: `*X` read as the field X->f0 when they end in one)
py::str lvalue_choices(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), inner = m.group(2), e = m.group(3);
        Lines ps_ = _top_split(inner, "?"_s);
        if (ps_.size() != 2) return m.group(0);
        str c = ps_[0].strip();
        Lines ab = _top_split(ps_[1], ":"_s);
        if (ab.size() != 2 || py::re::search(R"re(\w\s*\(|\+\+|--)re"_s, e)) return m.group(0);
        str a = ab[0].strip(), b = ab[1].strip();
        c = c.startswith("("_s) ? c : py::percent("(%s)"_s, c);
        return py::percent("%sif %s {\n%s    *%s = %s;\n%s} else {\n%s    *%s = %s;\n%s}"_s, ind, c, ind, a, e, ind, ind, b, e, ind);
    };
    return py::re::sub(R"re((?m)^([ \t]*)\*\((.+\?.+:.+)\) = ([^;]+);$)re"_s, one, text);
}

// L: S; goto H; inside a loop H: while (..) { .. } - the code a goto in the loop jumps to, the loop's own
// last statements (S then the next test): S at the end of the loop's body, `goto L;` where it was (a
// variant: the source's `nomatch: ;` before the step, the model's copy at the first jump to it)
std::optional<py::str> label_sinks(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t i = 0; i < lines.size(); ++i) {
        auto m = py::re::fullmatch(R"re((\s*)(L_\w+):)re"_s, lines[i]);
        if (!m) continue;
        str ind = m->group(1), lab = m->group(2);
        int64_t j = i + 1;
        Lines body;
        while (j < lines.size() && lines[j].startswith(ind) && nb(lines[j]) && !lines[j].startswith(ind + "}"_s) &&
               !py::re::fullmatch(R"re(\s*L_\w+:)re"_s, lines[j])) {
            body.append(lines[j]);
            j += 1;
        }
        if (ind.size() < 8 || body.empty() || j >= lines.size() || !lines[j].startswith(ind.slice(0, -4) + "}"_s))
            continue;   // (the block's last statements, inside an arm)
        auto g = py::re::fullmatch(R"re(\s*goto (L_\w+);)re"_s, body[-1]);
        bool bad = false;
        for (const str& x : body)
            if (x.rstrip().endswith("{"_s) || x.strip().startswith("}"_s)) bad = true;
        if (!g || bad) continue;
        str h = g->group(1);
        std::optional<int64_t> hj;
        for (int64_t k = 0; k < i; ++k)
            if (lines[k].strip() == h + ":"_s) { hj = k; break; }
        if (!hj || !py::re::match(R"re(\s*while \(.*\) \{$)re"_s, lines[*hj + 1])) continue;
        str hind = lead(lines[*hj + 1]);
        std::optional<int64_t> end;
        for (int64_t k = *hj + 2; k < lines.size(); ++k)
            if (lines[k] == hind + "}"_s) { end = k; break; }
        if (!end || !(*hj + 1 < i && i < *end)) continue;
        Lines moved;
        moved.append(hind + "    "_s + lab + ":"_s);
        for (const str& x : body.slice(0, -1)) moved.append(hind + "    "_s + x.strip());
        Lines nw = lines.slice(0, i);
        nw.append(ind + py::percent("goto %s;"_s, lab));
        nw.extend(lines.slice(j, *end));
        nw.extend(moved);
        nw.extend(lines.slice_from(*end));
        str t = join_nl(nw);
        if (!py::re::search(py::percent(R"re(\bgoto %s;)re"_s, h), t))
            t = py::re::sub(py::percent(R"re((?m)^[ \t]*%s:\n)re"_s, h), ""_s, t);
        return t;
    }
    return std::nullopt;
}

// a function ending in a loop that never ends, returning a variable from inside it: the return at the end,
// jumped to (goto out; .. out: return x;) - a variant: the code the compiler lays after the loop
std::optional<py::str> shared_exit(const py::str& text) {
    auto hsb = split_body(text);
    str head = std::get<0>(hsb), sep = std::get<1>(hsb), body = std::get<2>(hsb);
    if (sep.empty()) return std::nullopt;
    Lines lines = split_nl(body);
    std::optional<int64_t> last;
    for (int64_t k = lines.size() - 1; k >= 0; --k)
        if (nb(lines[k])) { last = k; break; }
    if (!last || lines[*last].strip() != "}") return std::nullopt;
    std::optional<int64_t> prev;
    for (int64_t k = *last - 1; k >= 0; --k)
        if (nb(lines[k])) { prev = k; break; }
    if (!prev || py::re::match(R"re(\s*return\b)re"_s, lines[*prev])) return std::nullopt;
    std::vector<int64_t> rets;
    for (int64_t k = 0; k < lines.size(); ++k)
        if (py::re::fullmatch(R"re(\s+return [a-z]\w*;)re"_s, lines[k]) && indent_of(lines[k]) > 4) rets.push_back(k);
    if (rets.empty()) return std::nullopt;
    int64_t k = rets.back();
    str v = lines[k].strip();
    lines[k] = py::re::sub(R"re(return [a-z]\w*;)re"_s, "goto out_;"_s, lines[k]);
    Lines ins;
    ins.append("out_:"_s);
    ins.append("    "_s + v);
    lines.set_slice(*last, *last, ins);
    return head + sep + join_nl(lines);
}

// bx = E; bx = (bx << 1) + P; .. *bx .. - a word element through a pointer P held in the data (not an
// array there): *(P + E), P a pointer to words
py::str pointer_index(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), e = m.group(2), p = m.group(3), rest = m.group(4);
        if (py::re::findall(R"re(\*bx\b)re"_s, rest).size() != 1 || py::re::search(R"re(\bbx\b)re"_s, rest.replace("*bx"_s, ""_s)))
            return m.group(0);
        text::PTRSIZE[p.startswith("g_"_s) ? py::percent("g_%04x"_s, py::int_from_str(p.slice_from(2), 16)) : p] = 2;
        return ind + rest.replace("*bx"_s, py::percent("*(%s + %s)"_s, p, py::re::fullmatch(R"re([\w\->.]+)re"_s, e) ? e : py::percent("(%s)"_s, e)));
    };
    return py::re::sub(R"re((?m)^([ \t]*)bx = (.+);\n[ \t]*bx = \(bx << 1\) \+ (g_[0-9a-f]+|[a-z]|v\d+);\n[ \t]*(.*)$)re"_s, one, text);
}

// [switch (x) { .. case K: return V; .. } with that case an if before it] - `if (x == K) return V;` the
// compiler compares first and merges into the switch's tests (variants: as it returns, and a bare return)
py::list<py::str> case_guards(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    for (int64_t k = 0; k < lines.size(); ++k) {
        str l = lines[k];
        auto m = py::re::match(R"re((\s*)switch \((.+)\) \{$)re"_s, l);
        if (!m) continue;
        str ind = m->group(1), sel = m->group(2);
        std::optional<int64_t> end;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!end) continue;
        for (int64_t j = k + 1; j < *end - 1; ++j) {
            auto c = py::re::fullmatch(py::percent(R"re(%scase (-?\w+):)re"_s, esc(ind)), lines[j]);
            auto r = py::re::fullmatch(py::percent(R"re(%s    (return(?: [^;]+)?;))re"_s, esc(ind)), lines[j + 1]);
            str prev = lines[j - 1];
            if (!c || !r || py::re::fullmatch(CASELABELRE, prev) ||
                (j + 2 < *end && !py::re::fullmatch(CASELABELRE, lines[j + 2]) && lines[j + 2] != ind + "}"_s))
                continue;
            if (!py::re::match(R"re(\s*(?:break|continue|return\b|goto\b))re"_s, prev) && prev != l) continue;   // (an arm falling into it: not alone)
            Lines body = lines.slice(0, k) + lines.slice(k, j) + lines.slice_from(j + 2);
            std::vector<str> rets = {r->group(1), "return;"_s};
            std::sort(rets.begin(), rets.end());
            rets.erase(std::unique(rets.begin(), rets.end()), rets.end());
            for (const str& ret : rets) {
                str g = py::percent("%sif ((%s == %s)) %s"_s, ind, sel, c->group(1), ret);
                Lines nw = body.copy();
                nw.insert(k, g);
                out.append(join_nl(nw));
            }
            break;
        }
    }
    return out;
}

// common_heads, then the ?: values it leaves and the registers and temporaries folded (a variant: the
// compiler may have done the statement before the test, or in each arm)
std::optional<py::str> heads_variant(const py::str& text) {
    str h = common_heads(text);
    if (h != text) return temporaries(fold_registers(fold_registers(conditional_values(h))));
    return std::nullopt;
}

// if (C1) { A; T } else if (C2) { B; T } else { return R; } - every way on ending in the same tail T (a
// return last), the others leaving: if (C1) { A } else if (C2) { B } else { return R; } T - the compiler's
// one copy of T, jumped to
std::optional<py::str> chain_tails(const py::str& text) {
    Lines lines = split_nl(text);
    auto leaves = [](const Lines& ls) -> bool {
        if (ls.empty()) return false;
        Lines nbs;
        for (const str& x : ls) if (nb(x)) nbs.append(x);
        if (nbs.empty()) throw py::IndexError("list index out of range");
        return py::re::match(R"re(\s*(?:return\b|goto\b|break;|continue;))re"_s, nbs[-1]).has_value();
    };
    using Arm = std::optional<std::pair<Lines, bool>>;
    std::function<Arm(const Lines&, const Lines&)> strip;
    strip = [&](const Lines& arm, const Lines& t) -> Arm {   // (the arm without the tail t - its lines stripped of
        Lines body = arm.copy();                              //  their indentation - or None)
        while (!body.empty() && !nb(body[-1])) body.pop();
        if (body.size() >= t.size()) {
            Lines stripped;
            for (const str& x : body.slice_from(body.size() - t.size())) stripped.append(x.strip());
            if (stripped == t) return std::make_pair(body.slice(0, body.size() - t.size()), true);
        }
        std::optional<IP> p = !body.empty() ? parts(body, 0) : std::nullopt;
        if (p && p->end == body.size() && p->els) {
            Arm a = strip(p->then, t);
            Arm b = strip(*p->els, t);
            str ind = lead(body[0]);
            if (a && b && (a->second || b->second) && (a->second || leaves(p->then)) && (b->second || leaves(*p->els))) {
                Lines nw;
                nw.append(body[0]);
                nw.extend(a->first);
                nw.append(ind + "} else {"_s);
                nw.extend(b->first);
                nw.append(ind + "}"_s);
                return std::make_pair(nw, true);
            }
        }
        if (leaves(body)) return std::make_pair(body, false);
        return std::nullopt;
    };
    for (int64_t k = 0; k < lines.size(); ++k) {
        std::optional<IP> p = parts(lines, k);
        if (!p || !p->els) continue;
        Lines th;
        for (const str& x : p->then) if (nb(x)) th.append(x);
        struct Best {
            Lines t;
            Arm a, b;
        };
        std::optional<Best> best;
        for (int64_t n = 1; n < 7; ++n) {   // (the longest tail every way shares)
            if (th.size() < n || !py::re::match(R"re(\s*return\b)re"_s, th[-1])) break;
            Lines t;
            for (const str& x : th.slice_from(th.size() - n)) t.append(x.strip());
            bool stop = false;
            for (const str& x : t)
                if (x.endswith("{"_s) || x.startswith("}"_s) || py::re::match(R"re((?:L_\w+:|case\b|default:))re"_s, x)) stop = true;
            if (stop) break;
            Arm a = strip(p->then, t);
            Arm b = strip(*p->els, t);
            if (a && b && b->second) best = Best{t, a, b};
        }
        if (best) {
            str ind = lead(lines[k]);
            Lines nw;
            nw.append(lines[k]);
            nw.extend(best->a->first);
            nw.append(ind + "} else {"_s);
            nw.extend(best->b->first);
            nw.append(ind + "}"_s);
            for (const str& x : best->t) nw.append(ind + x);
            return join_nl(lines.slice(0, k) + nw + lines.slice_from(p->end));
        }
    }
    return std::nullopt;
}

// goto L; ... L: return x; - a jump to a plain return (the compiler's one copy of the return, the others
// jumping to it): the return in the goto's place, the label gone when nothing else jumps to it
py::str goto_returns(const py::str& text) {
    py::dict<str, str> rets;
    for (const auto& t : py::re::compile(R"re((?m)^[ \t]*(L_\w+):\n[ \t]*(return(?: [\w]+| -?\d+)?;)$)re"_s).findall_groups<str, str>(text))
        rets.set(std::get<0>(t), std::get<1>(t));
    if (rets.empty()) return text;
    str out = py::re::sub(R"re((?m)^([ \t]*)goto (L_\w+);$)re"_s,
                          [&](const Match& m) -> str { return rets.contains(m.group(2)) ? m.group(1) + rets.at(m.group(2)) : m.group(0); },
                          text);
    for (const str& lab : rets)
        if (!py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab), out))
            out = py::re::sub(py::percent(R"re((?m)^[ \t]*%s:\n)re"_s, lab), ""_s, out);
    return out;
}

// a char local read in a test as (int)c (a variant: the compiler widens it, cbw, before comparing - or
// compares the byte, as the source asked)
std::optional<py::str> char_widens(const py::str& text) {
    Lines names = py::re::findall(R"re((?m)^[ \t]*char (v\d+|[a-z]|r_\w+);)re"_s, text);
    if (names.empty()) return std::nullopt;
    Lines escaped;
    for (const str& n : names) escaped.append(esc(n));
    py::re::Pattern rx = py::re::compile(py::percent(R"re((?<![\w])(?<!\(char\))(%s)\b(?!\s*=[^=]))re"_s, py::join("|"_s, escaped)));
    Lines out;
    for (const str& l0 : split_nl(text)) {
        str l = l0;
        auto m = py::re::match(R"re((\s*)(if|while|for|switch) \((.*)\)( \{| break;| continue;| return [^;]*;)?$)re"_s, l);
        if (m)
            l = m->group(1) + m->group(2) + " ("_s + rx.sub(R"re((int)\1)re"_s, m->group(3)) + ")"_s + m->group_opt(4).value_or(str(""));
        out.append(l);
    }
    str t = join_nl(out);
    if (t != text) return t;
    return std::nullopt;
}

// a continue last in the last statement of a loop's body (an if's arm closing it): nothing - the loop goes on
// there anyway (the compiler falls into the test; a continue is a jump to it)
py::str tail_continues(const py::str& text) {
    Lines lines = split_nl(text);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            str l = lines[k];
            if (l.strip() != "continue;" || k + 1 >= lines.size()) continue;
            int64_t j = k + 1;   // (only closing braces after it, to the loop's own)
            while (j < lines.size() && lines[j].strip() == "}") {
                std::optional<int64_t> op = _opener(lines, j);
                if (!op) break;
                if (py::re::match(R"re(\s*(?:while|for)\b.*\{$)re"_s, lines[*op])) {
                    if (j > k + 1) {   // (inside an if at the body's end, not the body's own)
                        lines.pop(k);
                        changed = true;
                    }
                    break;
                }
                if (!py::re::match(R"re(\s*(?:if \(.*\) \{|\} else \{)$)re"_s, lines[*op])) break;
                j += 1;
            }
            if (changed) break;
        }
    }
    return join_nl(lines);
}

// for (;;) { x = E; if (C(x)) break; S } - the value tested as it is given: while (!C((x = E))) { S }
std::optional<py::str> assign_loops(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 2; ++k) {
        auto m = py::re::fullmatch(R"re((\s*)for \(;;\) \{)re"_s, lines[k]);
        if (!m) continue;
        str ind = m->group(1);
        auto a = py::re::fullmatch(py::percent(R"re(%s    ([a-z]|v\d+|r_\w+) = ([^;]+);)re"_s, esc(ind)), lines[k + 1]);
        auto b = py::re::fullmatch(py::percent(R"re(%s    if \((.+)\) break;)re"_s, esc(ind)), lines[k + 2]);
        if (!a || !b || wcount(a->group(1), b->group(1)) != 1) continue;
        std::optional<int64_t> end;
        for (int64_t j = k + 3; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!end || *end == 0) continue;   // (body = lines[k+3:end] if end else None)
        bool cont = false;
        for (const str& x : lines.slice(k + 3, *end))
            if (x.strip() == "continue;" && indent_of(x) == ind.size() + 4) cont = true;
        if (cont) continue;
        str cond = wsub_tpl(a->group(1), py::percent("(%s = %s)"_s, a->group(1), a->group(2)), b->group(1), 1);
        str neg = _not(py::percent("(%s)"_s, cond));
        Lines one;
        one.append(py::percent("%swhile %s {"_s, ind, (neg.startswith("("_s) && _balanced(neg.slice(1, -1))) ? neg : py::percent("(%s)"_s, neg)));
        lines.set_slice(k, k + 3, one);
        return join_nl(lines);
    }
    return std::nullopt;
}

// .. r = A; L: X = r; } .. } else { .. r = B; goto L; } - one store the compiler shares between the two arms of
// an if (the jump to it from the other): each arm its own, X = A and X = B (both arms ending there)
py::str store_joins(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t i = 1; i < lines.size() - 1; ++i) {
        auto m = py::re::fullmatch(R"re(\s*(L_\w+):)re"_s, lines[i]);
        std::optional<Match> st, a;
        if (m) st = py::re::fullmatch(R"re((\s*)([\w\->.\[\]]+) = (ax|al|bx|cx|dx);)re"_s, lines[i + 1]);
        if (st) a = py::re::fullmatch(py::percent(R"re(\s*%s = ([^;]+);)re"_s, st->group(3)), lines[i - 1]);
        if (!a) continue;
        str lab = m->group(1);
        std::vector<int64_t> gs;
        for (int64_t j = 0; j < lines.size(); ++j)
            if (lines[j].strip() == py::percent("goto %s;"_s, lab)) gs.push_back(j);
        if (gs.size() != 1) continue;
        int64_t g = gs[0];
        auto b = py::re::fullmatch(py::percent(R"re(\s*%s = ([^;]+);)re"_s, st->group(3)), lines[g - 1]);
        if (!b) continue;
        py::list<int64_t> ol = _openers(lines, i), og = _openers(lines, g);
        bool common = false;
        for (int64_t x : ol)
            if (og.contains(x)) { common = true; break; }
        if (!common) continue;   // (the innermost block around both)
        // (the two ways: the arms of one if below that block; after each store only blocks closing to it)
        auto closes_to = [&](int64_t k) -> int64_t {
            int64_t j = k + 1;
            while (j < lines.size() && (lines[j].strip() == "}" || lines[j].strip() == "} else {")) {
                if (lines[j].strip() == "} else {") return j;
                j += 1;
            }
            return j;
        };
        std::optional<int64_t> el;
        for (int64_t j = std::min(i, g); j < std::max(i, g); ++j) {
            if (lines[j].strip() != "} else {") continue;
            std::optional<int64_t> o = _opener(lines, j);
            if (o && (ol.contains(*o) || og.contains(*o))) { el = j; break; }
        }
        if (!el) continue;
        int64_t ends[2] = {closes_to(i + 1), closes_to(g)};
        bool all_ = true;
        for (int64_t e : ends) {
            bool first_ = lines[e].strip() == "} else {" && e == *el;
            if (first_) continue;
            std::optional<int64_t> o = _opener(lines, *el);
            if (!o) throw py::TypeError("list indices must be integers or slices, not NoneType");
            bool second = e < lines.size() && indent_of(lines[e]) <= indent_of(lines[*o]);
            if (!second) { all_ = false; break; }
        }
        if (!all_) continue;
        str ind_s = st->group(1), ind_g = lead(lines[g]);
        Lines one;
        one.append(py::percent("%s%s = %s;"_s, ind_g, st->group(2), b->group(1)));
        lines.set_slice(g - 1, g + 1, one);
        if (g < i) i -= 1;
        Lines two;
        two.append(py::percent("%s%s = %s;"_s, ind_s, st->group(2), a->group(1)));
        lines.set_slice(i - 1, i + 2, two);
        return store_joins(join_nl(lines));
    }
    return text;
}

// L: for (;;) { for (;;) { S } break; } - an outer loop only restarting the inner one (a goto L in S): the
// inner loop alone, the gotos its continues (the model's reading of a loop entered at a shared clean-up)
py::str loop_collapse(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 3; ++k) {
        auto m = py::re::fullmatch(R"re((\s*)(L_\w+):)re"_s, lines[k]);
        if (!m || lines[k + 1] != m->group(1) + "for (;;) {"_s || lines[k + 2] != m->group(1) + "    for (;;) {"_s) continue;
        str ind = m->group(1), lab = m->group(2);
        std::optional<int64_t> e_in;
        for (int64_t j = k + 3; j < lines.size(); ++j)
            if (lines[j] == ind + "    }"_s) { e_in = j; break; }
        if (!e_in || *e_in + 2 >= lines.size() || lines[*e_in + 1].strip() != "break;" || lines[*e_in + 2] != ind + "}"_s) continue;
        Lines body = lines.slice(k + 3, *e_in);
        std::vector<int64_t> gotos;
        for (int64_t j = 0; j < body.size(); ++j)
            if (body[j].strip() == py::percent("goto %s;"_s, lab)) gotos.push_back(j);
        auto in_inner_loop = [&](int64_t j) -> bool {   // (a loop of its own around the goto: a continue would be
            int64_t want = indent_of(body[j]);          //  that loop's - a switch is no loop for a continue)
            for (int64_t q = j - 1; q >= 0; --q) {
                const str& x = body[q];
                int64_t d = indent_of(x);
                if (nb(x) && d < want && x.rstrip().endswith("{"_s)) {
                    if (py::re::match(R"re(\s*(?:while|for|do)\b)re"_s, x) || x.strip() == "do {") return true;
                    want = d;
                }
            }
            return false;
        };
        bool any_inner = false;
        for (int64_t j : gotos)
            if (in_inner_loop(j)) any_inner = true;
        if (gotos.empty() || py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab), join_nl(lines.slice(0, k) + lines.slice_from(*e_in))) ||
            any_inner)
            continue;
        for (int64_t j : gotos) body[j] = body[j].replace(py::percent("goto %s;"_s, lab), "continue;"_s);
        Lines nw;
        nw.append(ind + "for (;;) {"_s);
        for (const str& x : body) nw.append(x.startswith(ind + "        "_s) ? x.slice_from(4) : x);
        nw.append(ind + "}"_s);
        return join_nl(lines.slice(0, k) + nw + lines.slice_from(*e_in + 3));
    }
    return text;
}

// switch (x) { default: S continue; case K: break; } last in a loop's body - one value passed over: if (x != K)
// { S } (the compiler's one compare either way)
py::str switch_ifs(const py::str& text_in) {
    str text = text_in;
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), sel = m.group(2), body = m.group(3), k = m.group(4), close = m.group(5);
        if (lead(close).size() >= ind.size()) return m.group(0);
        return py::percent("%sif ((%s != %s)) {\n%s%s}\n%s"_s, ind, sel, k, body, ind, close);
    };
    text = py::re::sub(
        R"re((?m)^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1    continue;\n\1case (-?\w+):\n\1    break;\n\1\}\n(\1[ \t]*\}|[ \t]*\}))re"_s,
        one, text);
    // (switch (x) { case K: break; default: S } last: the value passed over first - if (x == K) continue; S)
    auto two_impl = [&](const str& ind, const str& sel, const str& k, const str& body, const str& close,
                        const str& whole) -> str {
        if (lead(close).size() >= ind.size() || py::re::search(py::percent(R"re((?m)^%s    break;$)re"_s, esc(ind)), body))
            return whole;
        str dd;
        for (const str& x : py::split(body.rstrip("\n"_s), "\n"_s))
            dd += x.startswith(ind + "    "_s) ? x.slice_from(4) + "\n"_s : x + "\n"_s;
        return py::percent("%sif ((%s == %s)) continue;\n%s%s"_s, ind, sel, k, dd, close);
    };
    text = py::re::sub(
        R"re((?m)^([ \t]*)switch \((.+)\) \{\n\1case (-?\w+):\n\1    break;\n\1default:\n((?:\1    .*\n)+?)\1\}\n([ \t]*\}))re"_s,
        [&](const Match& m) -> str {
            return two_impl(m.group(1), m.group(2), m.group(3), m.group(4), m.group(5), m.group(0));
        },
        text);
    // (the same, the default first: switch (x) { default: S case K: break; } - S falls into the break)
    return py::re::sub(
        R"re((?m)^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1case (-?\w+):\n\1    break;\n\1\}\n([ \t]*\}))re"_s,
        [&](const Match& m) -> str {
            return two_impl(m.group(1), m.group(2), m.group(4), m.group(3), m.group(5), m.group(0));
        },
        text);
}

// if (C) { r = A; L: V = r; S } else { r = B; goto L; } - the value of a ?: the compiler gives in each arm,
// the two meeting at the store: V = C ? A : B; S
py::str ternary_joins(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<IP> p = parts(lines, k);
        if (!p || !p->els || p->then.size() < 3) {
            k += 1;
            continue;
        }
        auto a = py::re::fullmatch(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re"_s, p->then[0]);
        std::optional<Match> lab, st;
        if (a) lab = py::re::fullmatch(R"re(\s*(L_\w+):)re"_s, p->then[1]);
        if (lab) st = py::re::fullmatch(py::percent(R"re(\s*([\w\->.\[\]]+) = %s;)re"_s, a->group(2)), p->then[2]);
        Lines el;
        for (const str& x : *p->els) if (nb(x)) el.append(x.strip());
        if (!st || el.size() != 2) {
            k += 1;
            continue;
        }
        auto b = py::re::fullmatch(py::percent(R"re(%s = ([^;]+);)re"_s, a->group(2)), el[0]);
        if (!b || el[1] != py::percent("goto %s;"_s, lab->group(1)) ||
            py::re::findall(py::percent(R"re(\bgoto %s;)re"_s, lab->group(1)), join_nl(lines)).size() != 1) {
            k += 1;
            continue;
        }
        str ind = lead(lines[k]);
        str c = p->cond.startswith("("_s) ? p->cond : py::percent("(%s)"_s, p->cond);
        Lines rest;
        for (const str& x : p->then.slice_from(3)) rest.append(dedent4(x, ind));
        Lines nw;
        nw.append(py::percent("%s%s = (%s ? %s : %s);"_s, ind, a->group(2), c, a->group(3), b->group(1)));   // (the register
        nw.append(py::percent("%s%s = %s;"_s, ind, st->group(1), a->group(2)));                              //  still holds it)
        nw.extend(rest);
        lines.set_slice(k, p->end, nw);
    }
    return join_nl(lines);
}

// V = ax; .. f(.., ax) - the register read again while it still holds what was stored from it (no write to
// it between, no call, no label - a way in from elsewhere): V there
py::str stored_values(const py::str& text_in) {
    // (inc al; jne: a byte compared with -1 - ((r + 1) & 0xff) == 0 is (char)r == -1)
    str text = py::re::sub(R"re(\(\(\((\w+) \+ 1\) & 0xff\) ([!=]=) 0\))re"_s, R"re(((char)\1 \2 -1))re"_s, text_in);
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::fullmatch(R"re(\s*([a-z]|v\d+|g_[0-9a-f]+|r_\w+) = (ax|al);)re"_s, lines[k]);
        if (!m) continue;
        str v = m->group(1), r = m->group(2);
        py::re::Pattern rx = py::re::compile(r == "ax" ? R"re(\b(?:ax|al|ah)\b)re"_s : R"re(\b(?:al)\b)re"_s);
        for (int64_t j = k + 1; j < lines.size(); ++j) {
            str y = lines[j];
            if (py::re::match(R"re(\s*L_\w+:)re"_s, y) || py::re::match(R"re(\s*(?:case\b|default:|\} else\b))re"_s, y)) break;
            auto w = py::re::match(R"re((\s*)(?:ax|al|ah) = (.*)$)re"_s, y);
            if (w) {   // (written: its value read first, then no more)
                if (rx.search(w->group(2)) && r == "ax")
                    lines[j] = y.slice(0, y.size() - w->group(2).size()) + wsub_tpl("ax"_s, v, w->group(2));
                break;
            }
            if (rx.search(y)) {
                if (r == "ax" && py::re::search(R"re(\b(?:al|ah)\b)re"_s, y)) break;
                lines[j] = wsub_tpl(r, v, y);
            }
            bool calls = false;
            for (const str& c : py::re::findall(R"re(\b([A-Za-z_]\w*)\s*\()re"_s, y))
                if (!one_of(c, {"if", "while", "for", "switch", "sizeof", "return"})) calls = true;
            if (calls || py::re::search(R"re(\(\*[^()]+\)\s*\()re"_s, y) ||
                py::re::search(py::percent(R"re(\b%s\s*=[^=])re"_s, esc(v)), y) ||
                py::re::match(R"re(\s*(?:return|break|continue|goto)\b)re"_s, y))
                break;   // (a call - it gives ax - or V given again: done)
        }
    }
    return join_nl(lines);
}

// r = E; if ((r != K1)) { if ((r == K2)) { L: S } } else { goto L; } - two values to one arm, the compiler's
// compares of a switch: switch (E) { case K1: case K2: S }
py::str two_case_switches(const py::str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 4; ++k) {
        auto a = py::re::fullmatch(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re"_s, lines[k]);
        if (!a) continue;
        str ind = a->group(1), r = a->group(2), e = a->group(3);
        std::optional<IP> p = parts(lines, k + 1);
        std::optional<Match> c1;
        if (p) c1 = py::re::fullmatch(py::percent(R"re(\(\(%s != (-?\d+)\)\))re"_s, r), "("_s + p->cond + ")"_s);
        if (!c1) continue;
        if (!p->els) continue;
        Lines nbs;
        for (const str& x : *p->els) if (nb(x)) nbs.append(x.strip());
        if (nbs.size() > 1) continue;   // ([..][:1] != [..])
        Lines joined_;
        for (const str& x : *p->els) joined_.append(x.strip());
        auto gm = py::re::fullmatch(R"re(goto (L_\w+);)re"_s, py::join(""_s, joined_));
        if (!gm) continue;
        str lab = gm->group(1);
        std::optional<IP> q = !p->then.empty() ? parts(p->then, 0) : std::nullopt;
        std::optional<Match> c2;
        if (q) c2 = py::re::fullmatch(py::percent(R"re(\(\(%s == (-?\d+)\)\))re"_s, r), "("_s + q->cond + ")"_s);
        if (!c2 || q->els || q->end != p->then.size() || q->then.empty() || q->then[0].strip() != lab + ":"_s) continue;
        Lines body;
        for (const str& x : q->then.slice_from(1)) body.append(x.startswith(ind + "        "_s) ? x.slice_from(8) : x);
        bool brk = false;
        for (const str& x : body) if (x.strip() == "break;") brk = true;
        if (py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab), join_nl(lines.slice(0, k) + body + lines.slice_from(p->end))) ||
            brk || wsearch(r, join_nl(body)))
            continue;
        Lines nw;
        nw.append(py::percent("%sswitch (%s) {"_s, ind, (!e.startswith("("_s) || !_balanced(e.slice(1, -1))) ? e : e.slice(1, -1)));
        nw.append(py::percent("%scase %s:"_s, ind, c1->group(1)));
        nw.append(py::percent("%scase %s:"_s, ind, c2->group(1)));
        for (const str& x : body) nw.append(nb(x) ? "    "_s + x : x);
        nw.append(py::percent("%s}"_s, ind));
        lines.set_slice(k, p->end, nw);
        return join_nl(lines);
    }
    return text;
}

// ax = E; X = ax; X' = dx; ax = (ax | dx); if ((ax == 0)) - a long given and tested at once: if ((X = E) == 0)
// (X a long field's, a long of the data's or a local's two words)
py::str long_tests(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), e = m.group(2), lo = m.group(3), hi = m.group(4), op = m.group(5), rest = m.group(6);
        std::optional<str> x = _pair_long(lo, hi);
        if (!x || x->empty() || x->startswith("(long)"_s)) return m.group(0);
        return py::percent("%sif (((%s = %s) %s 0))%s"_s, ind, *x, e, op, rest);
    };
    return py::re::sub(
        R"re((?m)^([ \t]*)ax = ([^;\n]+);\n[ \t]*([\w\->.]+) = ax;\n[ \t]*([\w\->.]+) = dx;\n[ \t]*ax = \(ax \| dx\);\n[ \t]*if \(\(ax ([!=]=) 0\)\)(.*)$)re"_s,
        one, text);
}

// the long two word expressions are, the low and the high (a long field's, a long of the data's, a long
// local's two words, an int and its sign spread), or None
std::optional<py::str> _pair_long(const py::str& lo, const py::str& hi) {
    auto f = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, lo);
    auto g = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, hi);
    if (f && g && f->group(1) == g->group(1) && py::int_from_str(g->group(2)) == py::int_from_str(f->group(2)) + 2) {
        if (!text::LONGINFO.fields) text::LONGINFO.fields = py::set<int64_t>();
        text::LONGINFO.fields->add(py::int_from_str(f->group(2)));
        return py::percent("*(long *)&%s->f%s"_s, f->group(1), f->group(2));
    }
    f = py::re::fullmatch(R"re(g_([0-9a-f]+))re"_s, lo);
    g = py::re::fullmatch(R"re(g_([0-9a-f]+))re"_s, hi);
    if (f && g && py::int_from_str(g->group(1), 16) == py::int_from_str(f->group(1), 16) + 2) {
        if (!text::LONGINFO.globals) text::LONGINFO.globals = py::set<str>();
        text::LONGINFO.globals->add(py::percent("g_%04x"_s, py::int_from_str(f->group(1), 16)));
        return lo;
    }
    f = py::re::fullmatch(R"re(v(\d+))re"_s, lo);
    g = py::re::fullmatch(R"re(v(\d+))re"_s, hi);
    if (f && g && py::int_from_str(g->group(1)) == py::int_from_str(f->group(1)) - 2) return "*(long *)&"_s + lo;
    if (py::re::fullmatch(py::percent(R"re(\(?0 - \(%s >> 15\)\)?)re"_s, esc(lo)), hi) &&
        py::re::fullmatch(R"re([\w\->.\[\]]+)re"_s, lo))
        return "(long)"_s + lo;
    return std::nullopt;
}

// if (C) { .. ax = E; } else { .. ax = L; dx = H; } X = ax; X' = dx; - a long both arms leave in dx:ax,
// stored a word at a time after them (a long field's words, a long of the data's, a long local's): the long
// stored in each arm (E a long already - a double converted, a long cast - or the long L and H are)
py::str sunk_pairs(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<IP> p = parts(lines, k);
        k += 1;
        if (!p || !p->els || p->end + 1 >= lines.size()) continue;
        auto st = py::re::fullmatch(R"re((\s*)([\w\->.]+) = ax;)re"_s, lines[p->end]);
        auto st2 = py::re::fullmatch(R"re(\s*([\w\->.]+) = dx;)re"_s, lines[p->end + 1]);
        std::optional<str> dst;
        if (st && st2) dst = _pair_long(st->group(2), st2->group(1));
        if (!dst || dst->empty() || dst->startswith("(long)"_s)) continue;
        if (_live_after(opt_of(lines), p->end + 1, "ax"_s, R"re(\b(?:ax|al|ah|dx)\b)re"_s)) continue;
        using Val = std::optional<std::pair<str, Lines>>;
        auto value = [&](const Lines& arm) -> Val {   // (the long an arm leaves, and the arm without it)
            Lines body;
            for (const str& x : arm) if (nb(x)) body.append(x);
            if (body.size() >= 2) {
                auto a = py::re::fullmatch(R"re(\s*(ax|dx) = ([^;]+);)re"_s, body[-2]);
                auto b = py::re::fullmatch(R"re(\s*(ax|dx) = ([^;]+);)re"_s, body[-1]);
                if (a && b && a->group(1) != b->group(1)) {
                    const Match& lo = a->group(1) == "ax" ? *a : *b;
                    const Match& hi = a->group(1) == "ax" ? *b : *a;
                    std::optional<str> v = _pair_long(lo.group(2), hi.group(2));
                    if (v && !v->empty()) return std::make_pair(*v, arm.slice(0, arm.index(body[-2])));
                }
            }
            if (!body.empty()) {
                auto e = py::re::fullmatch(R"re(\s*ax = (.+);)re"_s, body[-1]);
                bool dxset = false;
                for (const str& x : body.slice(body.size() >= 3 ? body.size() - 3 : 0, body.size()))
                    if (py::re::match(R"re(\s*dx = )re"_s, x)) dxset = true;
                if (e && py::re::search(R"re(\(double \*\)|\(float \*\)|\(long\)|\(double\))re"_s, e->group(1)) && !dxset)
                    return std::make_pair(e->group(1), arm.slice(0, arm.index(body[-1])));
            }
            return std::nullopt;
        };
        Val a = value(p->then), b = value(*p->els);
        if (!a || !b) continue;
        str ind = st->group(1);
        auto same = [&](const str& v) {
            return v == *dst || v.replace("*(long *)&"_s, ""_s) == dst->replace("*(long *)&"_s, ""_s);
        };
        bool anytxt = false;
        for (const str& x : a->second + b->second) if (nb(x)) anytxt = true;
        Lines nw;
        if (!anytxt && (same(a->first) || same(b->first))) {
            str c = p->cond.startswith("("_s) ? p->cond : py::percent("(%s)"_s, p->cond);   // (one way the long kept: the other's value or it -
            nw.append(ind + py::percent("%s = %s ? %s : %s;"_s, *dst, c, a->first, b->first));   //  a ?:)
        } else {
            nw.append(lines[k - 1]);
            nw.extend(a->second);
            nw.append(ind + py::percent("    %s = %s;"_s, *dst, a->first));
            nw.append(ind + "} else {"_s);
            nw.extend(b->second);
            nw.append(ind + py::percent("    %s = %s;"_s, *dst, b->first));
            nw.append(ind + "}"_s);
        }
        lines.set_slice(k - 1, p->end + 2, nw);
    }
    return join_nl(lines);
}

// if (C) { .. r = E1; } else { .. r = E2; } S(r); - a statement both arms ended with, the compiler's one copy
// of it after them (cross-jumped), the value it reads given in each: S in each arm, S(E1) and S(E2)
py::str sunk_tails(const py::str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        std::optional<IP> p = parts(lines, k);
        k += 1;
        if (!p || !p->els || p->then.empty() || p->els->empty() || p->end >= lines.size()) continue;
        str ind = lead(lines[k - 1]);
        auto a = py::re::fullmatch(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re"_s, p->then[-1]);
        auto b = py::re::fullmatch(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re"_s, (*p->els)[-1]);
        str s = lines[p->end];
        if (!a || !b || a->group(2) != b->group(2) || !s.startswith(ind) ||
            (s.slice(ind.size(), ind.size() + 1).isspace()))
            continue;
        str r = a->group(2);
        std::optional<str> rd = (r == "ax" || r == "al") ? std::optional<str>(R"re(\b(?:ax|al|ah)\b)re"_s) : std::nullopt;
        if (!py::re::fullmatch(R"re(\s*[^;{}]+;)re"_s, s) ||
            py::re::fullmatch(py::percent(R"re(\s*[\w\->.\[\]*]+ = %s;)re"_s, r), s) ||
            py::re::match(R"re(\s*(?:return|goto|break|continue|case|default)\b)re"_s, s) || wcount(r, s) != 1 ||
            _live_after(opt_of(lines), p->end, r, rd))
            continue;
        bool reads = false;
        for (const str& x : p->then.slice(0, -1) + p->els->slice(0, -1))
            if (wsearch(r, x)) reads = true;
        if (reads) continue;
        auto put = [&](const str& e) -> str {
            return py::re::fullmatch(R"re([\w\->.\[\]]+|[\w]+\(.*\))re"_s, e) ? e : py::percent("(%s)"_s, e);
        };
        str ia = a->group(1), ib = b->group(1);
        Lines nw;
        nw.append(lines[k - 1]);
        nw.extend(p->then.slice(0, -1));
        nw.append(ia + wsub_lit(r, put(a->group(3)), s.strip()));
        nw.append(ind + "} else {"_s);
        nw.extend(p->els->slice(0, -1));
        nw.append(ib + wsub_lit(r, put(b->group(3)), s.strip()));
        nw.append(ind + "}"_s);
        lines.set_slice(k - 1, p->end + 1, nw);
    }
    return join_nl(lines);
}

// r = E; A = r; B = r; - r given once and stored twice at once, read no more: the value given twice,
// B = A = E (A stored first, as the compiler stores a chain from the right)
py::str register_chains(const py::str& text) {
    OLines lines = opt_of(split_nl(text));
    auto vor = [&](int64_t i) -> str { return lines[i] ? *lines[i] : str(""); };
    for (int64_t k = 0; k < lines.size() - 2; ++k) {
        auto m = py::re::match(R"re(([ \t]*)(ax|bx|cx|dx) = ([^;]+);$)re"_s, vor(k));
        if (!m) continue;
        str ind = m->group(1), r = m->group(2), e = m->group(3);
        auto a = py::re::match(py::percent(R"re(%s([^;=]+?) = %s;$)re"_s, ind, r), vor(k + 1));
        auto b = py::re::match(py::percent(R"re(%s([^;=]+?) = %s;$)re"_s, ind, r), vor(k + 2));
        if (!a || !b || wsearch(r, a->group(1) + b->group(1) + e) || _live_after(lines, k + 2, r)) continue;
        lines[k] = py::percent("%s%s = %s = %s;"_s, ind, b->group(1), a->group(1), e);
        lines[k + 1] = std::nullopt;
        lines[k + 2] = std::nullopt;
    }
    return join_opt(lines);
}

// do { r = E; [p = p + 1;] M = r; } while (r != 0) - the value stored is what the loop tests: an empty loop
// on the assignment, while (M = E) ; (a string copied, while (*d++ = *s++) ;) - E's pointer stepped as *p++
py::str store_loops(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), reg = m.group(2), e = m.group(3), dst = m.group(5);
        std::optional<str> p = m.group_opt(4);
        if (p && !p->empty()) {
            auto d = py::re::fullmatch(py::percent(R"re(\*\((?:unsigned )?char \*\)%s|\*%s)re"_s, *p, *p), e);
            if (!d || wsearch(*p, dst)) return m.group(0);
            e = py::percent("*%s++"_s, *p);
        }
        if (wsearch(reg, dst + e)) return m.group(0);
        return py::percent("%swhile (%s = %s)\n%s    ;"_s, ind, dst, e, ind);
    };
    return py::re::sub(
        R"re((?m)^([ \t]*)do \{\n[ \t]*(ax|al|bx|bl|cx|cl|dx|dl|t\d+) = ([^;\n]+);\n(?:[ \t]*(\w+) = \4 \+ 1;\n)?[ \t]*([^;\n=]+?) = \2;\n[ \t]*\} while \(\(?\2(?: != 0)?\)?\);)re"_s,
        one, text);
}

// R = X; (R a scratch register, X a variable): the reads of R that follow are X - until R or X is given
// again, a call (which leaves its value in ax), a label or a block's end; the copy then goes
py::str copy_props(const py::str& text) {
    Lines lines = split_nl(text);
    OLines out = opt_of(lines);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto m = py::re::match(R"re((\s*)(ax|bx|cx|dx) = ([A-Za-z_]\w*);\s*$)re"_s, lines[k]);
        if (!m) continue;
        str r = m->group(2), x = m->group(3);
        if (one_of(x, {"ax", "bx", "cx", "dx"})) continue;   // (si, di: register variables - copied as variables)
        int64_t depth = 0;
        for (int64_t j = k + 1; j < lines.size(); ++j) {
            if (!out[j]) continue;   // (a copy dropped before: nothing)
            str y = *out[j];
            depth += y.count("{"_s) - y.count("}"_s);
            if (depth < 0 || py::re::match(R"re(\s*L_\w+:)re"_s, y)) break;
            if (py::re::fullmatch(py::percent(R"re(\s*%s = %s;\s*)re"_s, r, x), y)) {   // (the same copy again: no change)
                out[j] = std::nullopt;
                continue;
            }
            int64_t reads = wcount(r, y);
            auto sets_r = py::re::match(py::percent(R"re(\s*(?:%s|%sl|%sh)\s*=[^=])re"_s, r, r.slice(0, 1), r.slice(0, 1)), y);   // (or a half of it)
            if (reads && !sets_r) out[j] = wsub_tpl(r, x, y);
            if (sets_r || py::re::search(py::percent(R"re(\b%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*%s\b)re"_s, x, x), y) ||
                (r == "ax" && py::re::search(R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\s*\()re"_s, y)))
                break;
        }
        if (!_live_after(out, k, r)) out[k] = std::nullopt;   // (read no more before it is given again: gone)
    }
    return join_opt(out);
}

// X = E; X = X op F; (F not reading X): one expression - X = E op F (the compiler kept the value in X's
// register between the two)
py::str self_updates(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        auto m = py::re::match(R"re((\s*)([A-Za-z_]\w*) = ([^;]+);\s*$)re"_s, lines[k]);
        if (m && k + 1 < lines.size()) {
            str ind = m->group(1), x = m->group(2), e = m->group(3);
            auto u = py::re::match(py::percent(R"re(\s*%s = %s ([-+*&|^]|<<|>>) (.+);\s*$)re"_s, x, x), lines[k + 1]);
            if (!u) u = py::re::match(py::percent(R"re(\s*%s = \(%s ([-+*&|^]|<<|>>) ([^()]+)\);\s*$)re"_s, x, x), lines[k + 1]);   // (or in parentheses)
            if (u && !wsearch(x, e + u->group(2)) && !py::re::match(R"re(\s*(?:if|while|return)\b)re"_s, lines[k])) {
                str f = u->group(2);
                if (py::re::search(R"re( [-+*/%&|^<>]{1,2} )re"_s, e) &&
                    !(e.startswith("("_s) && e.endswith(")"_s) && _balanced(e.slice(1, -1))))
                    e = py::percent("(%s)"_s, e);   // (E an expression of its own: kept one, whatever op)
                lines[k + 1] = py::percent("%s%s = %s %s %s;"_s, ind, x, e, u->group(1), f);
                k += 1;   // (and on: the next step of it too)
                continue;
            }
        }
        out.append(lines[k]);
        k += 1;
    }
    return join_nl(out);
}

// R = X; X = X + 1 (or - 1); then R read once: the value before the step - X++ (X--) in its place
py::str post_steps(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        auto m = py::re::match(R"re((\s*)(ax|bx|cx|dx|al|bl|cl|dl) = ([A-Za-z_]\w*);\s*$)re"_s, lines[k]);
        if (m && k + 2 < lines.size()) {
            str reg = m->group(2), x = m->group(3);
            auto st = py::re::match(py::percent(R"re(\s*%s = %s ([+-]) 1;\s*$)re"_s, x, x), lines[k + 1]);
            str nxt = lines[k + 2];
            str rest = join_nl(lines.slice_from(k + 3));
            if (st && wcount(reg, nxt) == 1 && !wsearch(x, nxt) &&
                !wsearch(reg, py::re::split(py::percent(R"re(\n\s*%s = )re"_s, reg), rest)[0])) {
                out.append(wsub_tpl(reg, x + (st->group(1) == "+" ? "++"_s : "--"_s), nxt, 1));
                k += 3;
                continue;
            }
        }
        out.append(lines[k]);
        k += 1;
    }
    return join_nl(out);
}

// a byte copied and the copy tested (the compiler tests the register it moved it through):
//     [s = s - 1;] *d = *s; [d = d + 1;] if (((*s & 0xff) == 0)) break;  ->  if ((*d++ = *--s) == 0) break;
// (the steps written where they go: --s before, d++ after)
py::str copy_tests(const py::str& text) {
    auto one = [&](const Match& m) -> str {
        str ind = m.group(1), d = m.group(3), s = m.group(4), t = m.group(6);
        std::optional<str> pre = m.group_opt(2), post = m.group_opt(5);
        bool has_pre = pre && !pre->empty(), has_post = post && !post->empty();
        if (t != s || (has_pre && *pre != s)) return m.group(0);
        str src = has_pre ? py::percent("*--%s"_s, s) : py::percent("*%s"_s, s);
        str dst = has_post ? py::percent("*%s++"_s, d) : py::percent("*%s"_s, d);
        return py::percent("%sif (((%s = %s) == 0)) break;"_s, ind, dst, src);
    };
    return py::re::sub(
        R"re((?m)^([ \t]*)(?:(\w+) = \2 - 1;\n\s*)?\*(\w+) = \*(\w+);\n(?:\s*(\3) = \3 \+ 1;\n)?\s*if \(\(\(\*(\w+) & 0xff\) == 0\)\) break;$)re"_s,
        one, text);
}

// for (;;) { if (C) break; ..} - the loop's test first: while (!C) { ..}
py::str test_first_loops(const py::str& text) {
    OLines lines = opt_of(split_nl(text));
    for (int64_t k = 0; k < lines.size(); ++k) {
        std::optional<Match> m;
        if (lines[k]) m = py::re::match(R"re((\s*)for \(;;\) \{\s*$)re"_s, *lines[k]);
        if (m && k + 1 < lines.size() && lines[k + 1]) {
            auto b = py::re::match(R"re(\s*if (\(.*\)) break;\s*$)re"_s, *lines[k + 1]);
            if (b) {
                lines[k] = py::percent("%swhile (%s) {"_s, m->group(1), _not(b->group(1)));
                lines[k + 1] = std::nullopt;
                continue;
            }
            auto s = py::re::match(R"re(\s*(\w+) = \1 ([+-]) ([12]);\s*$)re"_s, *lines[k + 1]);
            if (!s) s = py::re::match(R"re(\s*(\w+)(\+|-)[+-]()\s*;\s*$)re"_s, *lines[k + 1]);   // (a step, then the test: --p in it;
            std::optional<Match> b2;                                                           //  by 2: words)
            if (s && k + 2 < lines.size() && lines[k + 2] && !lines[k + 2]->empty())
                b2 = py::re::match(R"re(\s*if (\(.*\)) break;\s*$)re"_s, *lines[k + 2]);
            if (b2 && wcount(s->group(1), b2->group(1)) == 1) {
                str op = s->group(2) == "+" ? "++"_s : "--"_s;
                str c = wsub_tpl(s->group(1), op + s->group(1), b2->group(1));
                lines[k] = py::percent("%swhile (%s) {"_s, m->group(1), _truth(_not(c)));
                lines[k + 1] = std::nullopt;
                lines[k + 2] = std::nullopt;
            }
        }
    }
    return join_opt(lines);
}

// r = f(..); LV = r; and r read no more: LV = f(..) (the call's value stored where it goes)
py::str stored_calls(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    const str CALLKW = R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\()re"_s;
    while (k < lines.size()) {
        auto m = py::re::match(py::percent(R"re((\s*)%s = (.*\w\(.*\).*);\s*$)re"_s, text::WREG), lines[k]);   // (a value with a call in it)
        if (m && k + 1 < lines.size()) {
            str reg = m->group(2), call = m->group(3);
            str rd = one_of(reg, {"ax", "bx", "cx", "dx"}) ? py::percent("(?:%s|%sl)"_s, reg, reg.slice(0, 1)) : reg;   // (or its low byte: a char)
            auto st = py::re::match(py::percent(R"re((\s*)(.+?) = %s;\s*$)re"_s, rd), lines[k + 1]);
            auto a_ = py::re::match(py::percent(R"re((\s*%s = )(.*)$)re"_s, reg), lines[k + 1]);   // (its reads there gone - not a value it is given)
            str stripped = a_ ? a_->group(1) + py::re::sub(wpat(rd), ""_s, a_->group(2)) : py::re::sub(wpat(rd), ""_s, lines[k + 1]);
            Lines tmp = lines.slice(0, k + 1);
            tmp.append(stripped);
            tmp.extend(lines.slice_from(k + 2));
            bool later = !_live_after(opt_of(tmp), k, reg, wpat(rd));
            // (read before it is given a value that does not read it, on any path: still wanted)
            if (st && !wsearch(rd, st->group(2)) && later) {
                out.append(py::percent("%s%s = %s;"_s, st->group(1), st->group(2), call));
                k += 2;
                continue;
            }
            if (st && !wsearch(rd, st->group(2)) && py::re::fullmatch(R"re(\w+)re"_s, st->group(2))) {
                str lv = st->group(2);   // (read again after: the variable it was stored in -
                int64_t j = k + 2;       //  until it is given another value, a call, a label)
                Lines fixed = lines.copy();
                while (j < fixed.size() && !py::re::match(py::percent(R"re(\s*(?:%s|%s) = |\s*L_\w+:)re"_s, reg, rd), fixed[j]) &&
                       !py::re::search(R"re(\w\()re"_s, fixed[j]) && !py::re::search(py::percent(R"re(\b%s\s*=[^=])re"_s, lv), fixed[j])) {
                    fixed[j] = wsub_tpl(rd, lv, fixed[j]);
                    j += 1;
                }
                if (!_live_after(opt_of(fixed), k + 1, reg, wpat(rd))) {
                    lines = fixed;
                    out.append(py::percent("%s%s = %s;"_s, st->group(1), lv, call));
                    k += 2;
                    continue;
                }
            }
            str nxt = lines[k + 1];   // (read once, anywhere in the next statement - which
            if (one_of(reg, {"si", "di"})) later = false;   //  (a register variable's value: kept in it)
            if (later && wcount(rd, nxt) == 1 && !py::re::search(R"re(\w\()re"_s, nxt) &&
                !py::re::match(R"re(\s*(?:\}|L_\w+:|else\b))re"_s, nxt) &&
                !py::re::search(py::percent(R"re(\b%s\s*=[^=])re"_s, rd), nxt)) {
                out.append(wsub_lit(rd, call, nxt, 1));   // makes no call of its own:
                k += 2;                                   //  the order stays)
                continue;
            }
            int64_t ncalls = py::re::findall(CALLKW, nxt).size();
            auto ca = py::re::search(
                R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re"_s, nxt);
            str outside = ca ? py::re::sub(py::percent(R"re(^\s*%s = )re"_s, reg), ""_s, nxt.slice(0, ca->start()) + nxt.slice_from(ca->end())) : str("");
            if (later && ncalls == 1 && ca && wcount(rd, ca->group(1)) == 1 && !wsearch(rd, outside) &&
                !py::re::match(R"re(\s*(?:\}|L_\w+:|else\b))re"_s, nxt)) {
                bool byte = wsearch(py::percent("%sl"_s, reg.slice(0, 1)), ca->group(1)) && reg != py::percent("%sl"_s, reg.slice(0, 1));   // (an argument of the one
                str v = byte ? py::percent("(char)(%s)"_s, call) : call;                                                                    //  call there: its value
                str arg = wsub_lit(rd, v, ca->group(1), 1);                                                                                 //  goes first anyway; its
                out.append(nxt.slice(0, ca->start(1)) + arg + nxt.slice_from(ca->end(1)));                                                  //  low byte, a char)
                k += 2;
                continue;
            }
        }
        out.append(lines[k]);
        k += 1;
    }
    return join_nl(out);
}

// a while loop whose body ends by stepping a variable - `X = X + K;`, or `else { X = X + K; continue; }`
// after a test whose other arm leaves - a for loop: for (INIT; C; X = X + K), its first statement the init when
// the line before gives a value to what C tests (a variant: the compiler lays a for out its own way - the step
// where a continue goes)
std::optional<py::str> for_loops(const py::str& text) {
    Lines lines = split_nl(text);
    bool changed = false;
    int64_t k = 0;
    const str ST = R"re(([A-Za-z_]\w*) = \1 [+-] \d+;|[A-Za-z_]\w*(?:\+\+|--);)re"_s;
    while (k < lines.size()) {
        auto m = py::re::match(R"re((\s*)while \((.*)\) \{$)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!m || !e || lines[*e].strip() != "}") {
            k += 1;
            continue;
        }
        str ind = m->group(1);
        str i4 = ind + "    "_s;
        Lines body = lines.slice(k + 1, *e);
        Lines own;
        for (const str& l : body)
            if (l.startswith(i4) && !l.startswith(i4 + " "_s)) own.append(l);
        auto has_cont = [&]() { return own.contains(i4 + "continue;"_s); };
        std::optional<str> step;
        auto stre = [&](const str& pre) { return pre + "(?:"_s + ST + ")"_s; };
        if (!body.empty() && py::re::fullmatch(stre(i4), body[-1]) && !has_cont()) {
            step = body[-1].strip();
            body = body.slice(0, -1);
        } else if (body.size() >= 4 && body[body.size() - 4] == i4 + "} else {"_s && py::re::fullmatch(stre(i4 + "    "_s), body[-3]) &&
                   body[-2] == i4 + "    continue;"_s && body[-1] == i4 + "}"_s && !has_cont() &&
                   py::re::match(R"re(\s*(?:break|return)\b)re"_s, body.size() >= 5 ? body[-5] : str(""))) {
            step = body[-3].strip();
            body = body.slice(0, body.size() - 4);
            body.append(i4 + "}"_s);
        } else {   // (the other way about: if (C) { X = X + K; continue; } else { ..leaves.. })
            std::optional<int64_t> j0;
            for (int64_t j = 0; j < body.size(); ++j)
                if (body[j].startswith(i4 + "if ("_s)) j0 = j;
            std::optional<IP> q = j0 ? parts(body, *j0) : std::nullopt;
            if (q && q->end == body.size() && q->els && !q->els->empty() && q->then.size() == 2 &&
                py::re::fullmatch(stre(i4 + "    "_s), q->then[0]) && q->then[1] == i4 + "    continue;"_s &&
                py::re::match(R"re(\s*(?:break|return)\b)re"_s, (*q->els)[-1]) && !has_cont()) {
                step = q->then[0].strip();
                Lines nb_ = body.slice(0, *j0);
                nb_.append(py::percent("%sif (%s) {"_s, i4, _truth(_not("("_s + q->cond + ")"_s))));
                nb_.extend(*q->els);
                nb_.append(i4 + "}"_s);
                body = nb_;
            } else if (q && q->end == body.size() && q->els && !q->els->empty() && q->els->size() >= 3 &&
                       py::re::fullmatch(stre(i4 + "    "_s), (*q->els)[-2]) && (*q->els)[-1] == i4 + "    continue;"_s &&
                       py::re::match(R"re(\s*(?:break|return)\b)re"_s, q->then[-1]) && !has_cont()) {
                bool anyc = false;
                for (const str& x : q->els->slice(0, -1)) if (x.strip() == "continue;") anyc = true;
                if (!anyc) {   // (if (C) { ..leaves.. } else { S; X = X + K; continue; })
                    step = (*q->els)[-2].strip();
                    Lines nb_ = body.slice(0, *j0 + 1);
                    nb_.extend(q->then);
                    nb_.append(i4 + "}"_s);
                    for (const str& x : q->els->slice(0, -2)) nb_.append(x.slice_from(4));
                    body = nb_;
                }
            }
        }
        if (!step || step->empty()) {
            k += 1;
            continue;
        }
        str init = "";
        int64_t p = k - 1;   // (the init: the line before, or one a few before
        while (p >= 0 && p > k - 4 && py::re::fullmatch(esc(ind) + R"re([A-Za-z_]\w* = [^;]+;)re"_s, lines[p])) {   // with
            auto im = py::re::fullmatch(esc(ind) + R"re(([A-Za-z_]\w*) = ([^;]+);)re"_s, lines[p]);   // nothing between
            str sv = py::re::match(R"re([A-Za-z_]\w*)re"_s, *step)->group(0);   // (the variable stepped)
            bool reads = false;
            for (int64_t j = p + 1; j < k; ++j)
                if (wsearch(sv, lines[j])) reads = true;
            if (im->group(1) == sv && !reads) {   // reading it)
                init = py::percent("%s = %s"_s, im->group(1), im->group(2));
                break;
            }
            p -= 1;
        }
        str head = py::percent("%sfor (%s; %s; %s) {"_s, ind, init, _truth("("_s + m->group(2) + ")"_s), step->rstrip(";"_s));
        Lines nw;
        nw.append(head);
        nw.extend(body);
        nw.append(lines[*e]);
        if (!init.empty()) {
            lines.set_slice(k, *e + 1, nw);
            lines.pop(p);
        } else {
            lines.set_slice(k, *e + 1, nw);
            k += 1;
        }
        changed = true;
    }
    if (changed) return join_nl(lines);
    return std::nullopt;
}

// the model's temporaries (tN: a value it had to keep while it read another - an argument pushed first)
// read once, by the next statement: the value in its place (C's own order of evaluation gives it back); one
// read more often declared an int
py::str temporaries(const py::str& text_in) {
    Lines lines = split_nl(text_in);
    int64_t k = 0;
    while (k < lines.size()) {
        auto m = py::re::match(R"re((\s*)(t\d+) = (.+);$)re"_s, lines[k]);
        if (m) {
            str x = m->group(2);
            std::vector<int64_t> uses;
            for (int64_t j = 0; j < lines.size(); ++j)
                if (j != k && wsearch(x, lines[j]) && !py::re::match(R"re([ \t]*(?:int|char|long|unsigned|register)\b)re"_s, lines[j]))
                    uses.push_back(j);
            int64_t nd = lines.size();
            for (int64_t j = k + 1; j < lines.size(); ++j)
                if (py::re::match(py::percent(R"re(\s*%s = )re"_s, x), lines[j])) { nd = j; break; }
            std::vector<int64_t> mine;
            for (int64_t j : uses)
                if (k < j && j < nd) mine.push_back(j);
            if (mine.size() == 1 && mine[0] == k + 1) {
                bool anyread = false;
                for (int64_t j = nd + 1; j < lines.size(); ++j) {
                    if (std::find(uses.begin(), uses.end(), j) == uses.end()) continue;
                    bool given = false;
                    for (int64_t q = nd; q < j; ++q)
                        if (py::re::match(py::percent(R"re(\s*%s = )re"_s, x), lines[q])) { given = true; break; }
                    if (given) continue;
                    if (wsearch(x, lines[j])) anyread = true;
                }
                if (!anyread) uses = {k + 1};   // (read once before it is given again: that value's)
            }
            if (uses.size() == 1 && uses[0] == k + 1 && wcount(x, lines[k + 1]) == 1) {
                str e = m->group(3);
                lines[k + 1] = py::re::sub(wpat(x),
                                           [&](const Match&) -> str {
                                               return py::re::fullmatch(R"re([\w\[\]\->.]+)re"_s, e) ? e : py::percent("(%s)"_s, e);
                                           },
                                           lines[k + 1]);
                lines.pop(k);
                continue;
            }
        }
        k += 1;
    }
    Lines kept;   // (declared again below: the ones still read)
    for (const str& l : lines)
        if (!py::re::fullmatch(R"re([ \t]*int t\d+(?:, t\d+)*;)re"_s, l)) kept.append(l);
    str text = join_nl(kept);
    int64_t brace = text.find("{"_s);
    str after = brace >= 0 ? text.slice_from(brace + 1) : text;   // text.split("{", 1)[-1]
    std::set<str> left;
    for (const str& t : py::re::findall(R"re(\b(t\d+)\b)re"_s, after)) left.insert(t);
    if (!left.empty()) {
        auto hsb = split_body(text);
        if (!std::get<1>(hsb).empty()) {
            Lines names;
            for (const str& n : left) names.append(n);
            text = std::get<0>(hsb) + std::get<1>(hsb) + py::percent("\n    int %s;"_s, py::join(", "_s, names)) + std::get<2>(hsb);
        }
    }
    return text;
}

// what a model leaves of the machine's registers, folded where the text alone shows how: a register given a
// value no later statement reads (a call's: the call; a load: nothing), a register returned right after it was
// given, a call the model wrote twice (once alone, once into a register)
py::str fold_registers(const py::str& text) {
    Lines lines = split_nl(text);
    Lines out;
    const str NUL(std::u32string(1, U'\0'));
    const str EFFECT = R"re(\w\s*\(|\+\+|--|[^=!<>]=[^=])re"_s;
    for (int64_t k = 0; k < lines.size(); ++k) {
        str l = lines[k];
        auto m = py::re::match(R"re((\s*)(ax|al|bx|cx|dx|si|di|bl|cl|dl|ah|bh|ch|dh) = (.*);\s*$)re"_s, l);
        if (m) {
            str ind = m->group(1), reg = m->group(2), expr = m->group(3);
            str nxt("");
            for (const str& x : lines.slice_from(k + 1))
                if (nb(x)) { nxt = x; break; }
            str later("");
            if (lines.slice_from(k + 1).contains(nxt)) later = join_nl(lines.slice_from(index_from(lines, nxt, k + 1) + 1));
            bool pure = py::re::fullmatch(
                            R"re([A-Za-z_]\w*|-?\d+|[A-Za-z_]\w*(?:->f\d+)+|\*[A-Za-z_]\w*|[A-Za-z_]\w*\[[A-Za-z_0-9]+\])re"_s, expr)
                            .has_value() ||
                        (!py::re::search(EFFECT, expr) && expr.startswith("("_s) && expr.endswith(")"_s));
            if (!pure && py::re::fullmatch(R"re([A-Za-z_][\w\->.]* (?:[-+&|^*]|<<|>>) (?:[A-Za-z_][\w\->.]*|\d+))re"_s, expr)) {
                expr = py::percent("(%s)"_s, expr);   // (a plain operation: in parentheses where it goes)
                pure = true;
            }
            auto upd = py::re::fullmatch(py::percent(R"re((\s*)([a-z]|v\d+|r_\w+|si|di) = \2 ([-+&|^]) %s;\s*)re"_s, reg), nxt);
            bool sidi = one_of(reg, {"si", "di"});
            if (upd && !sidi && py::re::fullmatch(R"re([A-Za-z_]\w*\(.*\))re"_s, expr) &&
                !wsearch(reg, py::re::split(py::percent(R"re(\n\s*%s = )re"_s, reg), later)[0]) &&
                !wsearch(upd->group(2), std::get<0>(expr.partition("("_s)))) {
                int64_t j = index_from(lines, nxt, k + 1);   // (a call's value added to a local, a parameter: the local
                lines[j] = py::percent("%s%s = %s %s %s;"_s, upd->group(1), upd->group(2), upd->group(2), upd->group(3), expr);   // updated by it)
                continue;
            }
            if (!pure && !py::re::search(EFFECT, expr) && py::re::fullmatch(R"re(!?\(.*\))re"_s, expr)) {
                expr = py::percent("(%s)"_s, expr);   // (a test's value: no call, no store)
                pure = true;
            }
            // (a value with no effect: a name, a field, an element, or no call and no store in it)
            Lines names = py::re::findall(R"re([A-Za-z_]\w*)re"_s, expr);
            bool names_any = false;
            for (const str& n : names)
                if (py::re::search(py::percent(R"re(\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b)re"_s, n, n), nxt)) { names_any = true; break; }
            if (pure && !sidi && wcount(reg, nxt) == 1 &&
                (py::re::fullmatch(py::percent(R"re(\s*[A-Za-z_]\w* = %s;\s*)re"_s, reg), nxt) || _plain_store(nxt, reg) || !names_any) &&
                !wsearch(reg, py::re::split(py::percent(R"re(\n\s*%s = )re"_s, reg), later)[0]) &&
                (_plain_store(nxt, reg) ||
                 !py::re::search(py::percent(R"re(\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b)re"_s, esc(expr), esc(expr)), nxt))) {
                int64_t j = index_from(lines, nxt, k + 1);   // (a plain value read once, at once: in its place)
                auto bk = py::re::fullmatch(py::percent(R"re((\s*)%s = (\(%s [-+&|^] [^;]+\));\s*)re"_s, esc(expr), reg), nxt);
                if (bk && one_of(reg, {"al", "bl", "cl", "dl"}))   // (a byte read, changed in a register, put back: the
                    nxt = py::percent("%s%s = (char)%s;"_s, bk->group(1), expr, bk->group(2));   // value cast - else done in place)
                lines[j] = py::re::sub(wpat(reg), [&](const Match&) -> str { return expr; }, nxt);
                continue;
            }
            if (py::re::fullmatch(py::percent(R"re(\s*return %s;\s*)re"_s, reg), nxt) &&
                (!sidi || !py::re::search(R"re(\w\s*\()re"_s, expr))) {
                out.append(py::percent("%sreturn %s;"_s, ind, expr));
                lines[index_from(lines, nxt, k + 1)] = NUL;
                continue;
            }
            bool used = _live_after(opt_of(lines), k, reg, wpat(reg == "ax" ? str("(?:ax|al)") : reg)) ||
                        sidi;   // (a register variable: a loop may read it on its next pass)
            if (!used) {
                auto call = py::re::search(R"re(\b(?!if\b|while\b|sizeof\b)[A-Za-z_]\w*\s*\(|\(\*[^()]+\)\s*\()re"_s, expr);   // (a call: a name, or
                                                                                                                              //  a pointer's, then
                if (call && !out.empty() && out[-1].strip() == expr + ";"_s) continue;                                       //  its arguments)
                if (call) out.append(py::percent("%s%s;"_s, ind, expr));
                continue;   // (a load nothing reads)
            }
        }
        if (l != NUL) out.append(l);
    }
    return join_nl(out);
}

}  // namespace codebreaker::exact::draft::shapes
