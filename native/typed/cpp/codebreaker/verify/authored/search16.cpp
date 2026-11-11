// codebreaker/verify/authored/search16.cpp -- typed C++ of codebreaker.verify.authored.search16 (see search16.hpp).

#include "codebreaker/verify/authored/search16.hpp"

#include <cmath>
#include <set>
#include <string>
#include <unordered_map>

#include "codebreaker/verify/verdicts.hpp"

namespace codebreaker::verify::authored::search16 {

namespace V = codebreaker::verify::verdicts;

namespace {
// (speed: the regex engine costs a few microseconds a call, so a line that cannot match a pattern is not offered to it.
//  Each test below is a necessary condition of the pattern named: no result changes.)
bool has(const py::str& s, char32_t c) { return s.u32().find(c) != std::u32string::npos; }
bool may_decl(const py::str& s) { return has(s, U';'); }                                // _DECL, _STMT end in ';'
bool may_compare(const py::str& s) { return has(s, U'<') || has(s, U'>'); }             // _COMPARE
bool may_left(const py::str& s) { return has(s, U'(') && may_compare(s); }              // _LEFT
bool may_if(const py::str& s) { return has(s, U'{') && s.contains(py::str("if (")); }   // _IF
bool may_assign(const py::str& s) { return has(s, U'=') && has(s, U';'); }              // _SELF, _COMPOUND
bool may_commute(const py::str& s) { return has(s, U'('); }                             // _COMMUTE

// What the patterns say of one line, found when first asked and kept (a pure function of the line's text).  A search
// rewrites a text a line or two at a time and looks at it again next round, so most lines were seen before; the
// regex engine, a few microseconds a call, is the cost of a rewrite.
struct LineInfo {
    bool decl_known = false, stmt_known = false, self_known = false, compound_known = false, if_known = false;
    bool compare_known = false, left_known = false, commute_known = false;
    std::optional<py::re::Match> decl, self, compound, ifm;
    bool stmt = false;
    std::vector<py::re::Match> compare, left, commute;
};
// (bounded: the cache is emptied between calls, never in the middle of one)
void trim_cache();
const std::optional<py::re::Match>& decl_of(const py::str& line);        // _DECL.match(line)
bool stmt_of(const py::str& line);                                       // _STMT.match(line) is not None
const std::optional<py::re::Match>& self_of(const py::str& line);        // _SELF.match(line)
const std::optional<py::re::Match>& compound_of(const py::str& line);    // _COMPOUND.match(line)
const std::optional<py::re::Match>& if_of(const py::str& line);          // _IF.match(line)
const std::vector<py::re::Match>& compare_of(const py::str& line);       // list(_COMPARE.finditer(line))
const std::vector<py::re::Match>& left_of(const py::str& line);          // list(_LEFT.finditer(line))
const std::vector<py::re::Match>& commute_of(const py::str& line);       // list(_COMMUTE.finditer(line))
}  // namespace

// ------------------------------------------------------------------------------------------ the rewrites
const py::re::Pattern& _DECL() {
    static const py::re::Pattern p = py::re::compile(
        py::str(R"re(^(\s+)((?:unsigned |signed )?\w+)(\s+\**\s*)(\w+)(\s*(?:\[[^\]]*\])?\s*;)\s*$)re"));
    return p;
}

const py::dict<py::str, py::str>& _SIGN() {
    static const py::dict<py::str, py::str> d = {
        {py::str("int1"), py::str("uint1")},      {py::str("uint1"), py::str("int1")},      {py::str("int2"), py::str("uint2")},
        {py::str("uint2"), py::str("int2")},      {py::str("int4"), py::str("uint4")},      {py::str("uint4"), py::str("int4")},
        {py::str("xunknown2"), py::str("int2")},  {py::str("xunknown4"), py::str("int4")},  {py::str("xunknown1"), py::str("int1")}};
    return d;
}

const py::re::Pattern& _COMPARE() {
    static const py::re::Pattern p = py::re::compile(py::str(R"re((?<![<>=!])(<=|>=|<|>)\s*(-?)(0x[0-9a-fA-F]+|\d+)\b)re"));
    return p;
}

namespace {

// int(text, 0) for the text the patterns match: Python refuses a decimal with a leading zero ("012") when the base is 0.
int64_t int_base0(const py::str& text) {
    if (text.contains(py::str("0x"))) return py::int_from_str(text, 16);
    if (py::len(text) > 1 && text[0] == "0" && !text.strip(py::str("0")).empty())
        throw py::ValueError(py::percent("invalid literal for int() with base 0: %r", text));
    return py::int_from_str(text, 10);
}

// "-0x%x" % -v if v < 0 else "0x%x" % v
py::str hexconst(int64_t v) {
    uint64_t a = v < 0 ? 0 - static_cast<uint64_t>(v) : static_cast<uint64_t>(v);
    static const char* digits = "0123456789abcdef";
    std::string s;
    do {
        s.insert(s.begin(), digits[a & 15]);
        a >>= 4;
    } while (a);
    return py::str((v < 0 ? "-0x" : "0x") + s);
}


}  // namespace

py::str _compare(const py::re::Match& m) {
    py::str op = m.group(1), neg = m.group(2), num = m.group(3);
    int64_t v = neg.empty() ? int_base0(num) : -int_base0(num);
    if (op == "<=") { op = py::str("<"); v = v + 1; }
    else if (op == "<") { op = py::str("<="); v = v - 1; }
    else if (op == ">=") { op = py::str(">"); v = v - 1; }
    else { op = py::str(">="); v = v + 1; }
    return op + py::str(" ") + hexconst(v);
}

const py::re::Pattern& _LEFT() {
    static const py::re::Pattern p =
        py::re::compile(py::str(R"re(\(\s*(-?)(0x[0-9a-fA-F]+|\d+)\s*(<=|>=|<|>)\s*([\w.\[\]]+)\s*\))re"));
    return p;
}

py::str _left(const py::re::Match& m) {
    py::str neg = m.group(1), num = m.group(2), op = m.group(3), var = m.group(4);
    int64_t v = neg.empty() ? int_base0(num) : -int_base0(num);
    if (op == "<") { op = py::str(">="); v = v + 1; }
    else if (op == "<=") { op = py::str(">"); v = v - 1; }
    else if (op == ">") { op = py::str("<="); v = v - 1; }
    else { op = py::str("<"); v = v + 1; }
    return py::str("(") + var + py::str(" ") + op + py::str(" ") + hexconst(v) + py::str(")");
}

const py::re::Pattern& _IF() {
    static const py::re::Pattern p = py::re::compile(py::str(R"re(^(\s*)if \((.*)\) \{\s*$)re"));
    return p;
}

py::str _negate(const py::str& cond) {
    std::optional<py::re::Match> m = py::re::fullmatch(py::str(R"re(\s*([^()&|!<>=]+?)\s*(==|!=|<=|>=|<|>)\s*([^()&|!<>=]+?)\s*)re"), cond);
    if (m) {
        py::str op = m->group(2);
        py::str neg = op == "==" ? py::str("!=") : op == "!=" ? py::str("==") : op == "<" ? py::str(">=")
                    : op == ">=" ? py::str("<") : op == ">" ? py::str("<=") : py::str(">");
        return m->group(1) + py::str(" ") + neg + py::str(" ") + m->group(3);
    }
    return py::str("!(") + cond + py::str(")");
}

const py::re::Pattern& _STMT() {
    static const py::re::Pattern p = py::re::compile(py::str(
        R"re(^(\s+)(?!return\b|if\b|else\b|while\b|for\b|do\b|switch\b|case\b|default\b|goto\b|break\b|continue\b)[^{};]*;\s*$)re"));
    return p;
}
const py::re::Pattern& _SELF() {
    static const py::re::Pattern p = py::re::compile(py::str(R"re(^(\s*)([\w.\[\]]+) = \2 (\+|-|\*|&|\||\^|<<|>>) (.+);\s*$)re"));
    return p;
}
const py::re::Pattern& _COMPOUND() {
    static const py::re::Pattern p = py::re::compile(py::str(R"re(^(\s*)([\w.\[\]]+) (\+|-|\*|&|\||\^|<<|>>)= (.+);\s*$)re"));
    return p;
}
const py::re::Pattern& _COMMUTE() {
    static const py::re::Pattern p =
        py::re::compile(py::str(R"re(\(([\w.]+) (==|!=|\+|\*|&|\|) ([\w.]+|0x[0-9a-fA-F]+|\d+)\))re"));
    return p;
}

namespace {

std::unordered_map<std::u32string, LineInfo>& cache_() {
    thread_local std::unordered_map<std::u32string, LineInfo> c;
    return c;
}
LineInfo& info(const py::str& line) { return cache_()[line.u32()]; }
void trim_cache() {
    if (cache_().size() > 20000) cache_().clear();
}

const std::optional<py::re::Match>& decl_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.decl_known) {
        if (may_decl(line)) i.decl = _DECL().match(line);
        i.decl_known = true;
    }
    return i.decl;
}
bool stmt_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.stmt_known) {
        i.stmt = may_decl(line) && _STMT().match(line);
        i.stmt_known = true;
    }
    return i.stmt;
}
const std::optional<py::re::Match>& self_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.self_known) {
        if (may_assign(line)) i.self = _SELF().match(line);
        i.self_known = true;
    }
    return i.self;
}
const std::optional<py::re::Match>& compound_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.compound_known) {
        if (may_assign(line)) i.compound = _COMPOUND().match(line);
        i.compound_known = true;
    }
    return i.compound;
}
const std::optional<py::re::Match>& if_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.if_known) {
        if (may_if(line)) i.ifm = _IF().match(line);
        i.if_known = true;
    }
    return i.ifm;
}
const std::vector<py::re::Match>& compare_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.compare_known) {
        if (may_compare(line))
            for (const py::re::Match& m : _COMPARE().finditer(line)) i.compare.push_back(m);
        i.compare_known = true;
    }
    return i.compare;
}
const std::vector<py::re::Match>& left_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.left_known) {
        if (may_left(line))
            for (const py::re::Match& m : _LEFT().finditer(line)) i.left.push_back(m);
        i.left_known = true;
    }
    return i.left;
}
const std::vector<py::re::Match>& commute_of(const py::str& line) {
    LineInfo& i = info(line);
    if (!i.commute_known) {
        if (may_commute(line))
            for (const py::re::Match& m : _COMMUTE().finditer(line)) i.commute.push_back(m);
        i.commute_known = true;
    }
    return i.commute;
}

py::list<int64_t> locals_(const py::list<py::str>& lines) {
    int64_t start = -1;
    for (int64_t k = 0; k < py::len(lines); k++) {
        if (lines[k].strip() == "{") {
            start = k;
            break;
        }
    }
    py::list<int64_t> out;
    if (start < 0) return out;
    for (int64_t k = start + 1; k < py::len(lines); k++) {
        if (decl_of(lines[k])) {
            out.append(k);
        } else if (!lines[k].strip().empty()) {
            break;
        }
    }
    return out;
}

py::list<std::tuple<int64_t, int64_t, int64_t>> if_else_(const py::list<py::str>& lines) {
    py::list<std::tuple<int64_t, int64_t, int64_t>> out;
    int64_t n = py::len(lines);
    for (int64_t k = 0; k < n; k++) {
        const std::optional<py::re::Match>& m = if_of(lines[k]);
        if (!m) continue;
        py::str ind = m->group(1);
        int64_t close = -1;
        for (int64_t j = k + 1; j < n; j++) {
            if (lines[j] == ind + "}") {
                close = j;
                break;
            }
        }
        if (close < 0 || close + 1 >= n || lines[close + 1] != ind + "else {") continue;
        int64_t end = -1;
        for (int64_t j = close + 2; j < n; j++) {
            if (lines[j] == ind + "}") {
                end = j;
                break;
            }
        }
        if (end >= 0) out.append(std::make_tuple(k, close + 1, end));
    }
    return out;
}

py::list<int64_t> statement_swaps_(const py::list<py::str>& lines, int64_t body) {
    py::list<int64_t> out;
    for (int64_t k = body; k < py::len(lines) - 1; k++) {
        const py::str& a = lines[k];
        const py::str& b = lines[k + 1];
        // (the cheap tests first: all of them must hold)
        if (py::len(a) - py::len(a.lstrip()) == py::len(b) - py::len(b.lstrip()) && (a + b).contains(py::str(" = ")) &&
            stmt_of(a) && !decl_of(a) && stmt_of(b) && !decl_of(b))
            out.append(k);
    }
    return out;
}

}  // namespace

py::list<int64_t> _locals(const py::list<py::str>& lines) {
    trim_cache();
    return locals_(lines);
}

py::list<std::tuple<int64_t, int64_t, int64_t>> _if_else(const py::list<py::str>& lines) {
    trim_cache();
    return if_else_(lines);
}

py::list<int64_t> _statement_swaps(const py::list<py::str>& lines, int64_t body) {
    trim_cache();
    return statement_swaps_(lines, body);
}

std::optional<py::str> rewrite(const py::str& text, Random& rng) {
    trim_cache();
    py::list<py::str> lines = py::split(text, py::str("\n"));
    py::list<int64_t> decls = locals_(lines);
    py::list<py::str> kinds;
    if (py::len(decls) > 1) kinds.append(py::str("order"));
    bool any_sign = false;
    for (int64_t k : decls)
        if (_SIGN().contains(decl_of(lines[k])->group(2))) any_sign = true;
    if (any_sign) kinds.append(py::str("sign"));
    int64_t body = 0;
    for (int64_t k = 0; k < py::len(lines); k++) {
        if (lines[k].strip() == "{") {
            body = k;
            break;
        }
    }
    // (line, match, which: 0 _compare / 1 _left); the matches are the cache's
    std::vector<std::tuple<int64_t, const py::re::Match*, int>> compares;
    for (int64_t k = body; k < py::len(lines); k++)
        for (const py::re::Match& m : compare_of(lines[k])) compares.emplace_back(k, &m, 0);
    for (int64_t k = body; k < py::len(lines); k++)
        for (const py::re::Match& m : left_of(lines[k])) compares.emplace_back(k, &m, 1);
    if (!compares.empty()) {
        kinds.append(py::str("compare"));
        kinds.append(py::str("compare"));
    }
    py::list<std::tuple<int64_t, int64_t, int64_t>> branches = if_else_(lines);
    if (!branches.empty()) {
        kinds.append(py::str("branch"));
        kinds.append(py::str("branch"));
    }
    py::list<int64_t> swaps = statement_swaps_(lines, body + 1);
    if (!swaps.empty()) kinds.append(py::str("swap"));
    py::list<int64_t> selfs;
    for (int64_t k = body; k < py::len(lines); k++)
        if (self_of(lines[k]) || compound_of(lines[k])) selfs.append(k);
    if (!selfs.empty()) kinds.append(py::str("assign"));
    std::vector<std::tuple<int64_t, const py::re::Match*>> commutes;
    for (int64_t k = body; k < py::len(lines); k++)
        for (const py::re::Match& m : commute_of(lines[k])) commutes.emplace_back(k, &m);
    if (!commutes.empty()) kinds.append(py::str("commute"));
    if (kinds.empty()) return std::nullopt;
    py::str kind = rng.choice(kinds);
    if (kind == "swap") {                                // two statements in the other order
        int64_t k = rng.choice(swaps);
        py::str t = lines[k];
        lines[k] = lines[k + 1];
        lines[k + 1] = t;
    } else if (kind == "assign") {                       // `x = x + y` <-> `x += y`, and `x += 1` <-> `x++`
        int64_t k = rng.choice(selfs);
        const std::optional<py::re::Match>& m = self_of(lines[k]);
        if (m) {
            lines[k] = m->group(1) + m->group(2) + py::str(" ") + m->group(3) + py::str("= ") + m->group(4) + py::str(";");
        } else {
            const std::optional<py::re::Match>& c = compound_of(lines[k]);
            py::str ind = c->group(1), v = c->group(2), op = c->group(3), rhs = c->group(4);
            py::str r = rhs.strip();
            if ((op == "+" || op == "-") && (r == "1" || r == "0x1") && rng.random() < 0.5) {
                lines[k] = ind + v + op + op + py::str(";");
            } else {
                lines[k] = ind + v + py::str(" = ") + v + py::str(" ") + op + py::str(" ") + rhs + py::str(";");
            }
        }
    } else if (kind == "commute") {                      // `(a == b)` -> `(b == a)`
        std::tuple<int64_t, const py::re::Match*> pick = commutes[static_cast<size_t>(rng._randbelow(static_cast<int64_t>(commutes.size())))];
        int64_t k = std::get<0>(pick);
        const py::re::Match& m = *std::get<1>(pick);
        lines[k] = lines[k].slice(0, m.start()) + py::str("(") + m.group(3) + py::str(" ") + m.group(2) + py::str(" ") + m.group(1) +
                   py::str(")") + lines[k].slice_from(m.end());
    } else if (kind == "branch") {                       // `if (c) {A} else {B}` -> `if (!c) {B} else {A}`
        std::tuple<int64_t, int64_t, int64_t> pick = rng.choice(branches);
        int64_t k = std::get<0>(pick), e = std::get<1>(pick), end = std::get<2>(pick);
        const std::optional<py::re::Match>& m = if_of(lines[k]);
        std::vector<py::str> out;
        for (int64_t i = 0; i < k; i++) out.push_back(lines[i]);
        out.push_back(m->group(1) + py::str("if (") + _negate(m->group(2)) + py::str(") {"));
        for (int64_t i = e + 1; i < end; i++) out.push_back(lines[i]);             // b = lines[e + 1:end]
        out.push_back(m->group(1) + py::str("}"));
        out.push_back(m->group(1) + py::str("else {"));
        for (int64_t i = k + 1; i < e - 1; i++) out.push_back(lines[i]);           // a = lines[k + 1:e - 1]
        out.push_back(m->group(1) + py::str("}"));
        for (int64_t i = end + 1; i < py::len(lines); i++) out.push_back(lines[i]);
        lines = py::list<py::str>(out);
    } else if (kind == "compare") {
        std::tuple<int64_t, const py::re::Match*, int> pick = compares[static_cast<size_t>(rng._randbelow(static_cast<int64_t>(compares.size())))];
        int64_t k = std::get<0>(pick);
        const py::re::Match& m = *std::get<1>(pick);
        py::str how = std::get<2>(pick) == 0 ? _compare(m) : _left(m);
        lines[k] = lines[k].slice(0, m.start()) + how + lines[k].slice_from(m.end());
    } else if (kind == "order") {
        py::list<int64_t> ab = rng.sample(decls, 2);
        py::str t = lines[ab[0]];
        lines[ab[0]] = lines[ab[1]];
        lines[ab[1]] = t;
    } else {
        py::list<int64_t> signed_;
        for (int64_t k : decls)
            if (_SIGN().contains(decl_of(lines[k])->group(2))) signed_.append(k);
        int64_t k = rng.choice(signed_);
        const std::optional<py::re::Match>& m = decl_of(lines[k]);
        lines[k] = m->group(1) + _SIGN().at(m->group(2)) + m->group(3) + m->group(4) + m->group(5);
    }
    return py::join(py::str("\n"), lines);
}

py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                       const Score& score, int64_t rounds, int64_t seed, const Log& log) {
    Random rng(seed);
    py::dict<py::str, std::tuple<py::str, Verdict>> best;
    py::dict<py::str, Group> group;
    for (const auto& kv : near.items()) {
        const py::str& fn = std::get<0>(kv);
        const Near& n = std::get<1>(kv);
        best[fn] = std::make_tuple(std::get<0>(n), std::get<1>(n));
        group[fn] = std::get<2>(n);
    }
    for (int64_t r = 0; r < rounds; r++) {
        py::dict<py::str, py::str> tries;
        for (const auto& kv : best.items()) {
            const py::str& fn = std::get<0>(kv);
            const py::str& t = std::get<0>(std::get<1>(kv));
            const Verdict& v = std::get<1>(std::get<1>(kv));
            if (std::get<0>(v) == V::IDENTICAL) continue;
            std::optional<py::str> nw = rewrite(t, rng);
            if (nw && *nw != t) tries[fn] = *nw;
        }
        if (tries.empty()) break;
        int64_t gained = 0;
        std::set<Group> groups;
        for (const auto& kv : tries.items()) groups.insert(group.at(std::get<0>(kv)));
        for (const Group& g : groups) {
            py::dict<py::str, py::str> mine;
            for (const auto& kv : tries.items())
                if (group.at(std::get<0>(kv)) == g) mine[std::get<0>(kv)] = std::get<1>(kv);
            for (const auto& kv : compile_group(g, mine).items()) {
                const py::str& fn = std::get<0>(kv);
                const Verdict& v = std::get<1>(kv);
                if (score(v) > score(std::get<1>(best.at(fn)))) {
                    best[fn] = std::make_tuple(mine.at(fn), v);
                    gained += 1;
                }
            }
        }
        if (log) {
            int64_t identical = 0;
            for (const auto& kv : best.items())
                if (std::get<0>(std::get<1>(std::get<1>(kv))) == V::IDENTICAL) identical++;
            log(py::percent("  round %d: %d rewrites, %d nearer, %d identical", r + 1, py::len(tries), gained, identical));
        }
    }
    return best;
}

py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                       const Score& score, int64_t rounds) {
    return search(near, compile_group, score, rounds, 1995, [](const py::str& s) { py::print(s); });
}

}  // namespace codebreaker::verify::authored::search16
