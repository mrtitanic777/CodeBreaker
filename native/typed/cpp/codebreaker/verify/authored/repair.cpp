// codebreaker/verify/authored/repair.cpp -- typed C++ port of codebreaker.verify.authored.repair (see repair.hpp).

#include "codebreaker/verify/authored/repair.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <thread>
#include <unordered_map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/authored/shape.hpp"
#include "codebreaker/verify/lanes/powerpc.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::authored::repair {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace eng = codebreaker::engine::native_engine;

str S(const char* s) { return str(s); }

const str _ID = S("(?:[A-Za-z_]\\w*|0[xX][0-9a-fA-F]+|\\d+)");
const str _NAME = S("[A-Za-z_]\\w*");

// an expression that is `a op b` and nothing more: it begins after ( = , ; { or `return`, and ends at ) ; or , -- so no neighbour's precedence can
// take an operand away.  Groups: 1 pre, 2 a, 3 op, 4 b, 5 post.
const py::re::Pattern& PAIR() {
    static const py::re::Pattern p = py::re::compile(S("(?P<pre>(?:[(,;{]|(?<![=!<>])=)\\s*|\\breturn\\s+)(?P<a>") + _ID +
                                                     S(")\\s*(?P<op>==|!=|<=|>=|[-+*&|^<>])\\s*(?P<b>") + _ID + S(")(?P<post>\\s*[);,])"));
    return p;
}
// a statement that only steps a variable: x++;  ++x;  x += 1;  x = x + 1;  Groups: 1 pre, 2 x1, 3 o1, 4 o2, 5 x2, 6 x3, 7 o3, 8 post.
const py::re::Pattern& STEP() {
    static const py::re::Pattern p = py::re::compile(S("(?P<pre>[;{}]\\s*|\\belse\\s+)(?:(?P<x1>") + _NAME + S(")\\s*(?P<o1>\\+\\+|--)|(?P<o2>\\+\\+|--)\\s*(?P<x2>") + _NAME +
                                                     S(")|(?P<x3>") + _NAME + S(")\\s*(?P<o3>[-+])=\\s*1)(?P<post>\\s*[;)])"));
    return p;
}
// x = x op y;   Groups: 1 pre, 2 x, 3 op, 4 y, 5 post.
const py::re::Pattern& LONG() {
    static const py::re::Pattern p = py::re::compile(S("(?P<pre>[;{}]\\s*|\\belse\\s+)(?P<x>") + _NAME + S(")\\s*=\\s*(?P=x)\\s*(?P<op>[-+*&|^])\\s*(?P<y>") + _ID +
                                                     S(")(?P<post>\\s*[;)])"));
    return p;
}
// x op= y;   Groups: 1 pre, 2 x, 3 op, 4 y, 5 post.
const py::re::Pattern& SHORT() {
    static const py::re::Pattern p = py::re::compile(S("(?P<pre>[;{}]\\s*|\\belse\\s+)(?P<x>") + _NAME + S(")\\s*(?P<op>[-+*&|^])=\\s*(?P<y>") + _ID +
                                                     S(")(?P<post>\\s*[;)])"));
    return p;
}
const py::re::Pattern& DECL() {
    static const py::re::Pattern p = py::re::compile(S("^[A-Za-z_][\\w ]*\\**\\s*[A-Za-z_]\\w*(\\s*\\[[^\\]]*\\])?\\s*;$"));
    return p;
}

// the operator the operands of a commutative operator swap to; "" for one that does not commute
str commutative(const str& op) {
    if (op == S("+") || op == S("*") || op == S("&") || op == S("|") || op == S("^") || op == S("==") || op == S("!=")) return op;
    if (op == S("<")) return S(">");
    if (op == S(">")) return S("<");
    if (op == S("<=")) return S(">=");
    if (op == S(">=")) return S("<=");
    return S("");
}

str flip(const str& op) {
    if (op == S("<")) return S(">");
    if (op == S(">")) return S("<");
    if (op == S("<=")) return S(">=");
    if (op == S(">=")) return S("<=");
    return op;   // == and !=
}

str count_text(int64_t n) { return py::to_str(n); }

Masked masked_(const match::ByteMasked& c) { return Masked(std::get<0>(c), match::Mask(std::get<1>(c))); }

// difflib.SequenceMatcher(None, a, b, autojunk=False): the number of elements in its matching blocks
int64_t matching_total(const std::vector<int64_t>& a, const std::vector<int64_t>& b) {
    std::unordered_map<int64_t, std::vector<int64_t>> b2j;
    for (int64_t j = 0; j < static_cast<int64_t>(b.size()); ++j) b2j[b[static_cast<size_t>(j)]].push_back(j);
    auto longest = [&](int64_t alo, int64_t ahi, int64_t blo, int64_t bhi) {
        int64_t besti = alo, bestj = blo, bestsize = 0;
        std::unordered_map<int64_t, int64_t> j2len;
        for (int64_t i = alo; i < ahi; ++i) {
            std::unordered_map<int64_t, int64_t> next;
            auto it = b2j.find(a[static_cast<size_t>(i)]);
            if (it != b2j.end())
                for (int64_t j : it->second) {
                    if (j < blo) continue;
                    if (j >= bhi) break;
                    auto p = j2len.find(j - 1);
                    const int64_t k = (p == j2len.end() ? 0 : p->second) + 1;
                    next[j] = k;
                    if (k > bestsize) {
                        besti = i - k + 1;
                        bestj = j - k + 1;
                        bestsize = k;
                    }
                }
            j2len = std::move(next);
        }
        return std::make_tuple(besti, bestj, bestsize);
    };
    int64_t total = 0;
    std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t>> queue{{0, static_cast<int64_t>(a.size()), 0, static_cast<int64_t>(b.size())}};
    while (!queue.empty()) {
        auto [alo, ahi, blo, bhi] = queue.back();
        queue.pop_back();
        auto [i, j, k] = longest(alo, ahi, blo, bhi);
        if (k) {
            total += k;
            if (alo < i && blo < j) queue.emplace_back(alo, i, blo, j);
            if (i + k < ahi && j + k < bhi) queue.emplace_back(i + k, ahi, j + k, bhi);
        }
    }
    return total;
}

// the instructions as one list of ids (both lists of one comparison use one numbering)
void ids_of(const Insns& x, const Insns& y, std::vector<int64_t>& ix, std::vector<int64_t>& iy) {
    std::map<std::string, int64_t> ids;
    auto id = [&](const std::string& token) {
        auto it = ids.find(token);
        if (it == ids.end()) it = ids.emplace(token, static_cast<int64_t>(ids.size())).first;
        return it->second;
    };
    auto tokens = [&](const Insns& v, std::vector<int64_t>& out) {
        if (std::holds_alternative<py::list<py::bytes>>(v)) {
            for (const py::bytes& b : std::get<py::list<py::bytes>>(v)) out.push_back(id("b" + b.raw()));
        } else {
            for (int64_t w : std::get<py::list<int64_t>>(v)) out.push_back(id("w" + std::to_string(w)));
        }
    };
    tokens(x, ix);
    tokens(y, iy);
}

int64_t insns_size(const Insns& v) {
    return std::holds_alternative<py::list<py::bytes>>(v) ? static_cast<int64_t>(std::get<py::list<py::bytes>>(v).size())
                                                           : static_cast<int64_t>(std::get<py::list<int64_t>>(v).size());
}

}  // namespace

void log_print(const str& line) { py::print(line); }

std::tuple<str, py::list<str>, str> _split(const str& source) {
    auto [head, brace, body] = source.partition(S("{"));
    if (brace.empty()) return {source, py::list<str>(), S("")};
    py::list<str> lines = py::split(body, S("\n"));
    py::list<str> decls;
    int64_t i = 0;
    const int64_t n = static_cast<int64_t>(lines.size());
    while (i < n) {
        const str s = lines[i].strip();
        if (s.empty() || DECL().match(s)) {
            if (!s.empty()) decls.append(lines[i]);
            ++i;
        } else {
            break;
        }
    }
    py::list<str> rest;
    for (int64_t k = i; k < n; ++k) rest.append(lines[k]);
    return {head + S("{"), decls, py::join(S("\n"), rest)};
}

int64_t _frame(const py::bytes& code, int64_t bits) {
    if (bits == BITS_PPC) {
        const int64_t n = std::min<int64_t>(code.size(), 48);
        for (int64_t k = 0; k < n - 3; k += 4) {
            int64_t w = 0;
            for (int64_t b = 0; b < 4; ++b) w = (w << 8) | code[k + b];
            if ((w >> 16) == 0x9421) return (-(w & 0xFFFF)) & 0xFFFF;   // stwu r1,-N(r1)
        }
        return 0;
    }
    const match::Decoder& md = match::decoder(bits == 32 ? 32 : 16);
    const str sp = bits == 32 ? S("esp,") : S("sp,");
    int64_t k = 0;
    for (const match::Insn& i : md(code.slice(0, 32), 0)) {
        if (i.mnemonic == S("sub") && i.op_str.startswith(sp)) {
            try {
                return py::int_parse(py::split(i.op_str, S(","))[1].strip(), 0);
            } catch (const py::ValueError&) {
                return 0;
            }
        }
        if (k >= 6 || !(i.mnemonic == S("push") || i.mnemonic == S("mov"))) break;
        ++k;
    }
    return 0;
}

Insns instructions(const Masked& built, int64_t bits) {
    const py::bytes code = std::get<0>(built);
    if (bits == BITS_PPC) {
        const py::list<int64_t>& mask = std::get<py::list<int64_t>>(std::get<1>(built));
        py::list<int64_t> out;
        for (int64_t i = 0; i < code.size() - 3; i += 4) {
            int64_t w = 0;
            for (int64_t b = 0; b < 4; ++b) w = (w << 8) | code[i + b];
            out.append((w & ~mask[i / 4]) & 0xFFFFFFFFLL);
        }
        return out;
    }
    const py::bytes& mask = std::get<py::bytes>(std::get<1>(built));
    const match::Decoder& md = match::decoder(bits == 32 ? 32 : 16);
    py::list<py::bytes> out;
    int64_t at = 0;
    while (at < code.size()) {
        int64_t n = 0;
        for (const match::Insn& i : md(code.slice_from(at), at)) {
            std::string one;
            for (int64_t k = i.address; k < i.address + i.size; ++k) one.push_back(mask[k] ? '\0' : static_cast<char>(code[k]));
            out.append(py::bytes(one));
            n = i.address + i.size;
        }
        if (n <= at) {   // not an instruction: one byte, and on
            out.append(code.slice(at, at + 1));
            n = at + 1;
        }
        at = n;
    }
    return out;
}

Score score(const Masked& built, const Masked& original, int64_t bits, const std::optional<Insns>& target) {
    str v;
    int64_t d = 0, t = 0;
    if (bits == BITS_PPC) {
        auto r = match::distance(built, original, S("powerpc"));
        v = std::get<0>(r), d = std::get<1>(r), t = std::get<2>(r);
    } else {
        auto r = match::distance(built, original);
        v = std::get<0>(r), d = std::get<1>(r), t = std::get<2>(r);
    }
    if (!d || !bits) return {std::make_tuple(d ? d : 0, d), Distance(v, d, t)};
    const Insns mine = instructions(built, bits);
    const Insns theirs = target ? *target : instructions(original, bits);
    std::vector<int64_t> a, b;
    ids_of(mine, theirs, a, b);
    const int64_t same = matching_total(a, b);
    return {std::make_tuple(std::max(insns_size(mine), insns_size(theirs)) - same + 1, d), Distance(v, d, t)};
}

str _blank(const str& text) { return base::ctext::blank(text, true); }

int64_t _depth(const str& shadow, int64_t at) {
    int64_t open = 0, close = 0;
    const str head = shadow.slice(0, at);
    for (const str& c : head) {
        if (c == S("(")) ++open;
        else if (c == S(")")) ++close;
    }
    return open - close;
}

py::list<std::tuple<int64_t, int64_t, str>> _tests(const str& shadow, const str& body) {
    py::list<std::tuple<int64_t, int64_t, str>> out;
    static const py::re::Pattern head = py::re::compile(S("\\b(?:if|while)\\s*\\("));
    static const py::re::Pattern call = py::re::compile(S("[A-Za-z_]\\w*\\s*\\("));
    const int64_t n = shadow.size();
    for (const py::re::Match& m : head.finditer(shadow)) {
        const int64_t i = m.end();
        int64_t depth = 1, j = i;
        while (j < n && depth) {
            depth += (shadow[j] == S("(")) - (shadow[j] == S(")"));
            ++j;
        }
        if (depth) continue;
        const str inner = shadow.slice(i, j - 1);
        std::vector<std::pair<int64_t, str>> ops;
        int64_t d = 0, k = 0;
        bool plain = true;
        const int64_t ni = inner.size();
        while (k < ni) {
            const str c = inner[k], two = inner.slice(k, k + 2);
            const bool two_in_assign = two == S("++") || two == S("--") || two == S("+=") || two == S("-=") || two == S("*=") || two == S("/=") ||
                                       two == S("|=") || two == S("&=") || two == S("^=") || two == S("%=");
            if (c == S("(") || c == S("[")) {
                ++d;
            } else if (c == S(")") || c == S("]")) {
                --d;
            } else if (two_in_assign) {
                plain = false;
            } else if (d == 0 && (two == S("&&") || two == S("||") || c == S("?") || c == S(","))) {
                plain = false;
            } else if (two == S("<<") || two == S(">>") || two == S("->")) {
                ++k;
            } else if (two == S("==") || two == S("!=") || two == S("<=") || two == S(">=")) {
                if (d == 0) ops.emplace_back(k, two);
                ++k;
            } else if (c == S("<") || c == S(">")) {
                if (d == 0) ops.emplace_back(k, c);
            } else if (c == S("=")) {
                plain = false;
            } else if (d == 0 && (c == S("&") || c == S("|") || c == S("^"))) {
                plain = false;   // binds looser than a comparison
            }
            ++k;
        }
        if (!plain || ops.size() != 1 || call.search(inner)) continue;
        const int64_t at = ops[0].first;
        const str op = ops[0].second;
        const str left = body.slice(i, i + at).strip(), right = body.slice(i + at + op.size(), j - 1).strip();
        if (!left.empty() && !right.empty() && left != right) out.append(std::make_tuple(i, j - 1, py::percent(S("%s %s %s"), right, flip(op), left)));
    }
    return out;
}

std::optional<str> _toggle_register(const str& line) {
    const str s = line.strip();
    if (s.contains(S("[")) || s.startswith(S("static ")) || s.startswith(S("extern ")) || !DECL().match(s)) return std::nullopt;
    const str indent = line.slice(0, line.size() - line.lstrip().size());
    const str other = s.startswith(S("register ")) ? s.slice_from(static_cast<int64_t>(std::string("register ").size())) : S("register ") + s;
    py::list<str> parts = py::split(other);
    if (parts.empty()) throw py::IndexError("list index out of range");
    const str last = parts[static_cast<int64_t>(parts.size()) - 1];
    if ((last == S("register") || last == S("int;")) && parts.size() == 1) return std::nullopt;
    return indent + other;
}

py::list<std::tuple<str, str>> _knobs(const str& source, int64_t target_frame, const std::optional<int64_t>& draft_frame) {
    py::list<std::tuple<str, str>> out;
    auto yield = [&](const str& name, const str& text) { out.append(std::make_tuple(name, text)); };
    auto [head, decls, body] = _split(source);
    const str decl_text = py::join(S("\n"), decls);
    const str top = head + (decls.empty() ? S("") : S("\n") + decl_text) + S("\n");
    // frame: the draft keeps the original's locals in one block with spare room at its end; the room is made what the original's frame says it was
    static const py::re::Pattern spare_rx = py::re::compile(S("uint1 _rest\\[(\\d+)\\]; \\} _f;"));
    auto spare = spare_rx.search(source);
    if (spare && target_frame && draft_frame && *draft_frame != target_frame) {
        const int64_t room = py::int_parse(spare->group(1)) - (*draft_frame - target_frame);
        if (room >= 0) {
            const str nw = room ? py::percent(S("uint1 _rest[%d]; } _f;"), room) : S("} _f;");
            yield(py::percent(S("frame%d"), room), source.slice(0, spare->start()) + nw + source.slice_from(spare->end()));
        }
    }
    // pad: the original reserves more stack than the draft -- an unused local of the difference
    if (target_frame && draft_frame && target_frame > *draft_frame) {
        const int64_t gap = target_frame - *draft_frame;
        const str pad = py::percent(S("  char codebreaker_unused[%d];"), gap);
        yield(py::percent(S("pad%d"), gap), head + S("\n") + pad + (decls.empty() ? S("") : S("\n") + decl_text) + S("\n") + body);
    }
    // order: two of the leading locals change places (the frame is laid out in declaration order; any order is some changes of two, and the search
    // goes on from each)
    const int64_t lead = std::min<int64_t>(decls.size(), MAX_LOCALS);
    int64_t n = 0;
    for (int64_t i = 0; i < lead; ++i)
        for (int64_t j = i + 1; j < lead; ++j, ++n) {
            py::list<str> moved = decls.copy();
            const str t = moved[i];
            moved[i] = moved[j];
            moved[j] = t;
            bool same = true;   // (moved != decls)
            for (int64_t k = 0; k < static_cast<int64_t>(moved.size()); ++k)
                if (moved[k] != decls[k]) same = false;
            if (!same) yield(py::percent(S("order%d"), n), head + S("\n") + py::join(S("\n"), moved) + S("\n") + body);
        }
    // register: a local kept in a register, or not (the compilers of the time did as they were told)
    for (int64_t i = 0; i < std::min<int64_t>(decls.size(), MAX_LOCALS); ++i) {
        auto other = _toggle_register(decls[i]);
        if (other) {
            py::list<str> v;
            for (int64_t k = 0; k < static_cast<int64_t>(decls.size()); ++k) v.append(k == i ? *other : decls[k]);
            yield(py::percent(S("register%d"), i), head + S("\n") + py::join(S("\n"), v) + S("\n") + body);
        }
    }
    // regparam: the same for an argument, where each is declared on a line of its own (f(a) int a; {)
    const int64_t close = head.rfind(S(")"));
    if (close >= 0) {
        py::list<str> lines = py::split(head.slice_from(close + 1), S("\n"));
        for (int64_t i = 0; i < static_cast<int64_t>(lines.size()); ++i) {
            std::optional<str> other = lines[i].strip().empty() ? std::nullopt : _toggle_register(lines[i]);
            if (other) {
                py::list<str> v;
                for (int64_t k = 0; k < static_cast<int64_t>(lines.size()); ++k) v.append(k == i ? *other : lines[k]);
                yield(py::percent(S("regparam%d"), i), head.slice(0, close + 1) + py::join(S("\n"), v) + (decls.empty() ? S("") : S("\n") + decl_text) + S("\n") + body);
            }
        }
    }
    const str shadow = _blank(body);
    // swap: the operands of a commutative operator the other way round; a < b as b > a
    {
        int64_t j = 0;
        for (const py::re::Match& m : PAIR().finditer(shadow)) {
            if (commutative(m.group(3)).empty() || m.group(2) == m.group(4)) continue;
            if (j >= MAX_SITES) break;
            const str nw = m.group(1) + m.group(4) + S(" ") + commutative(m.group(3)) + S(" ") + m.group(2) + m.group(5);
            yield(py::percent(S("swap%d"), j), top + body.slice(0, m.start()) + nw + body.slice_from(m.end()));
            ++j;
        }
    }
    // switch: a chain of `if (v == A) .. else if (v == B) ..` as a switch on v
    {
        int64_t j = 0;
        for (const auto& e : shape::switches(body, shadow)) {
            if (j >= MAX_SITES) break;
            yield(py::percent(S("switch%d"), j), top + body.slice(0, std::get<0>(e)) + std::get<2>(e) + body.slice_from(std::get<1>(e)));
            ++j;
        }
    }
    // fold: `t = f(..);` and at once `if (t == N)`, as `if (f(..) == N)`
    {
        int64_t j = 0;
        for (const auto& e : shape::folds(decls, body, shadow)) {
            if (j >= MAX_SITES) break;
            yield(py::percent(S("fold%d"), j), top + body.slice(0, std::get<0>(e)) + std::get<2>(e) + body.slice_from(std::get<1>(e)));
            ++j;
        }
    }
    // return: `r = x;` that only the final `return r;` follows, as `return x;`
    if (auto early = shape::returns(head, decls, body, shadow); early && !early->empty()) yield(S("return"), top + *early);
    // test: the two sides of the one comparison an `if` or `while` makes, the other way round
    {
        int64_t j = 0;
        for (const auto& e : _tests(shadow, body)) {
            if (j >= MAX_SITES) break;
            yield(py::percent(S("test%d"), j), top + body.slice(0, std::get<0>(e)) + std::get<2>(e) + body.slice_from(std::get<1>(e)));
            ++j;
        }
    }
    // step: the ways of writing a statement that adds or takes one
    {
        int64_t k_site = 0;
        for (const py::re::Match& m : STEP().finditer(shadow)) {
            if (k_site >= MAX_SITES) break;
            const int64_t d = _depth(shadow, m.start());
            const str post = m.group(8).strip();
            if ((post == S(";") && d != 0) || (post == S(")") && d != 1)) continue;   // the condition of a for: its value is used
            const str x = !m.group(2).empty() ? m.group(2) : !m.group(5).empty() ? m.group(5) : m.group(6);
            const str o = !m.group(3).empty() ? m.group(3) : !m.group(4).empty() ? m.group(4) : m.group(7);
            const str sign = o.slice(0, 1);
            const str forms[3] = {x + sign + sign, sign + sign + x, py::percent(S("%s %s= 1"), x, sign)};
            const int64_t mine = !m.group(2).empty() ? 0 : !m.group(5).empty() ? 1 : 2;
            for (int64_t k = 0; k < 3; ++k)
                if (k != mine)
                    yield(py::percent(S("step%d.%d"), k_site, k), top + body.slice(0, m.start()) + m.group(1) + forms[k] + m.group(8) + body.slice_from(m.end()));
            ++k_site;
        }
    }
    // assign: x = x + y  and  x += y
    {
        int64_t k_site = 0;
        for (int which = 0; which < 2; ++which) {
            const py::re::Pattern& rx = which == 0 ? LONG() : SHORT();
            for (const py::re::Match& m : rx.finditer(shadow)) {
                if (k_site >= MAX_SITES) break;
                const int64_t d = _depth(shadow, m.start());
                const str post = m.group(5).strip();
                if ((post == S(";") && d != 0) || (post == S(")") && d != 1)) continue;
                const str x = m.group(2), op = m.group(3), y = m.group(4);
                const str form = which == 0 ? x + S(" ") + op + S("= ") + y : x + S(" = ") + x + S(" ") + op + S(" ") + y;
                yield(py::percent(S("assign%d"), k_site), top + body.slice(0, m.start()) + m.group(1) + form + m.group(5) + body.slice_from(m.end()));
                ++k_site;
            }
        }
    }
    return out;
}

std::optional<Repaired> repair(const Compile& compile, const str& source, const Masked& original, int64_t bits) {
    std::optional<Masked> built = compile(source);
    if (!built) return std::nullopt;
    std::optional<Insns> target;
    if (bits) target = instructions(original, bits);
    const int64_t target_frame = bits ? _frame(std::get<0>(original), bits) : 0;
    Score sc = score(*built, original, bits, target);
    std::tuple<int64_t, int64_t> s = std::get<0>(sc);
    Distance dist = std::get<1>(sc);
    Repaired best_r;
    best_r.source = source;
    best_r.verdict = std::get<0>(dist);
    best_r.distance = std::get<1>(dist);
    best_r.total = std::get<2>(dist);
    best_r.knobs = S("");
    std::tuple<int64_t, int64_t> best_s = s;
    struct Item {
        std::tuple<int64_t, int64_t> s;
        str text;
        Masked was;
        str path;
    };
    std::vector<Item> frontier{{s, source, *built, S("")}};
    std::set<std::u32string> seen{source.u32()};
    int64_t tried = 1, stale = 0;
    while (std::get<1>(best_s) && tried < MAX_WORK && stale < PATIENCE && !frontier.empty()) {
        std::vector<Item> found;
        for (const Item& it : frontier) {
            std::optional<int64_t> draft_frame;
            if (bits) draft_frame = _frame(std::get<0>(it.was), bits);
            for (const auto& kv : _knobs(it.text, target_frame, draft_frame)) {
                const str& knob = std::get<0>(kv);
                const str& variant = std::get<1>(kv);
                if (seen.count(variant.u32()) || tried >= MAX_WORK) continue;
                seen.insert(variant.u32());
                ++tried;
                std::optional<Masked> now = compile(variant);
                if (!now) continue;
                Score ns = score(*now, original, bits, target);
                const auto& s2 = std::get<0>(ns);
                const Distance& dd = std::get<1>(ns);
                const str path2 = (it.path + S(",") + knob).strip(S(","));
                found.push_back({s2, variant, *now, path2});
                if (s2 < best_s) {
                    best_s = s2;
                    stale = -1;
                    best_r.source = variant;
                    best_r.verdict = std::get<0>(dd);
                    best_r.distance = std::get<1>(dd);
                    best_r.total = std::get<2>(dd);
                    best_r.knobs = path2;
                }
                if (!std::get<1>(dd)) break;
            }
            if (!std::get<1>(best_s)) break;
        }
        ++stale;
        std::stable_sort(found.begin(), found.end(), [](const Item& a, const Item& b) {
            return a.s < b.s || (a.s == b.s && a.path.size() < b.path.size());
        });
        if (found.size() > static_cast<size_t>(BEAM)) found.resize(static_cast<size_t>(BEAM));
        frontier = found;
    }
    best_r.tried = tried;
    return best_r;
}

std::optional<Repaired> repair_function(const str& cl, const py::dict<str, str>& env, const str& header, const str& source, const str& name,
                                        const Masked& original, const str& work, const str& flags) {
    struct Setting {
        str flags, header, label;
    };
    std::vector<Setting> settings{{flags, header, S("")}};
    if (!flags.contains(S("/Oy-"))) settings.push_back({flags + S(" /Oy-"), header, S("/Oy-")});
    const str near = match::needed_header(header, source);
    {
        std::vector<Setting> more;
        for (const Setting& s : settings) more.push_back({s.flags, near, (s.label + S(" own-declarations")).strip()});
        for (const Setting& s : more) settings.push_back(s);
    }
    std::optional<std::tuple<std::tuple<int64_t, int64_t>, str, str, str>> best;
    for (const Setting& st : settings) {
        auto built = match::compile_function(cl, env, st.header, source, name, work, st.flags);
        if (!built) continue;
        const auto s = std::get<0>(score(masked_(*built), original, 32));
        if (!best || s < std::get<0>(*best)) best = std::make_tuple(s, st.flags, st.header, st.label);
        if (!std::get<1>(s)) break;
    }
    if (!best) return std::nullopt;
    const str f = std::get<1>(*best), h = std::get<2>(*best), label = std::get<3>(*best);
    std::optional<Repaired> r = repair(
        [&](const str& text) -> std::optional<Masked> {
            auto c = match::compile_function(cl, env, h, text, name, work, f);
            if (!c) return std::nullopt;
            return masked_(*c);
        },
        source, original, 32);
    if (r && !label.empty()) r->knobs = (py::percent(S("[%s],"), label) + r->knobs).strip(S(","));
    return r;
}

std::tuple<int64_t, int64_t, int64_t, int64_t> truth(const Profile& profile, int64_t limit, const Log& log) {
    py::list<Unit> units = profile.units();
    const str work = py::os::path::join(_scratch(), S("truth"));
    Compile compile = [&](const str& text) { return profile.compile(text, work); };
    int64_t n = 0, same = 0, knocked = 0, back = 0;
    const int64_t total = static_cast<int64_t>(units.size());
    for (int64_t ui = 0; ui < (limit ? std::min(limit, total) : total); ++ui) {
        const Unit& u = units[ui];
        ++n;
        auto built = compile(u.source);
        if (!built) throw py::TypeError("'NoneType' object is not subscriptable");
        if (std::get<0>(match::distance(*built, u.original)) != V::IDENTICAL) continue;
        ++same;
        py::list<std::tuple<str, str>> one = _knobs(u.source, 0, std::nullopt);
        std::vector<std::tuple<str, str>> two;
        for (int64_t a = 0; a < std::min<int64_t>(one.size(), 2); ++a) {
            py::list<std::tuple<str, str>> inner = _knobs(std::get<1>(one[a]), 0, std::nullopt);
            const int64_t m = static_cast<int64_t>(inner.size());
            for (int64_t b = std::max<int64_t>(0, m - 2); b < m; ++b) two.emplace_back(std::get<0>(one[a]) + S(",") + std::get<0>(inner[b]), std::get<1>(inner[b]));
        }
        std::vector<std::pair<str, std::vector<std::tuple<str, str>>>> kinds;
        for (const auto& kv : one) {
            const str kind = std::get<0>(kv).rstrip(S("0123456789"));
            auto it = std::find_if(kinds.begin(), kinds.end(), [&](const auto& p) { return p.first == kind; });
            if (it == kinds.end()) {
                kinds.push_back({kind, {}});
                it = kinds.end() - 1;
            }
            it->second.push_back(kv);
        }
        std::vector<std::tuple<str, str>> picks;
        for (const auto& g : kinds)
            for (size_t k = 0; k < std::min<size_t>(2, g.second.size()); ++k) picks.push_back(g.second[k]);
        for (size_t k = 0; k < std::min<size_t>(2, two.size()); ++k) picks.push_back(two[k]);
        for (const auto& pick : picks) {
            const str& knob = std::get<0>(pick);
            const str& moved = std::get<1>(pick);
            auto b = compile(moved);
            if (!b || std::get<1>(match::distance(*b, u.original)) == 0) continue;   // the compiler gives the same bytes either way
            ++knocked;
            std::optional<Repaired> r = repair(compile, moved, u.original, profile.bits);
            const bool ok = r && r->distance == 0;
            back += ok;
            if (log && !ok)
                log(py::percent(S("  %-12s %-16s not brought back: distance %s after %s"), u.name, knob, r ? py::to_str(r->distance) : S("None"),
                                r ? r->knobs : S("None")));
        }
    }
    return {n, same, knocked, back};
}

str _scratch() {
    const str path = py::os::path::join(py::settings::get_str(S("state")), S("repair"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

str best_flags(const str& cl, const py::dict<str, str>& env, const str& header, const py::dict<str, str>& texts, const py::list<str>& order,
               const py::dict<int64_t, match::ByteMasked>& originals, const py::dict<str, int64_t>& address, const str& work, int64_t sample) {
    std::vector<str> names;
    for (const str& n : order)
        if (originals.contains(address.at(n))) names.push_back(n);
    const int64_t step = std::max<int64_t>(1, static_cast<int64_t>(names.size()) / sample);
    std::vector<str> chosen;
    for (size_t i = 0; i < names.size() && static_cast<int64_t>(chosen.size()) < sample; i += static_cast<size_t>(step)) chosen.push_back(names[i]);
    int64_t best_score = -1;
    str best = match::FLAG_SETS[0];
    for (const str& flags : match::FLAG_SETS) {
        int64_t got = 0;
        for (const str& n : chosen) {
            auto built = match::compile_function(cl, env, header, texts.at(n), n, work, flags);
            if (built) {
                const str v = std::get<0>(match::distance(masked_(*built), masked_(originals.at(address.at(n)))));
                got += v == V::IDENTICAL ? 3 : v == V::SAME_SIZE ? 1 : 0;
            }
        }
        if (got > best_score) {
            best_score = got;
            best = flags;
        }
    }
    return best;
}

namespace {

str thread_folder(const str& work) {
    const size_t id = std::hash<std::thread::id>()(std::this_thread::get_id());
    return py::os::path::join(work, py::percent(S("t%d"), static_cast<int64_t>(id & 0x7FFFFFFF)));
}

void tally(Counts& counts, const Repaired& r) {
    std::get<0>(counts) += 1;
    if (r.verdict == V::IDENTICAL) std::get<1>(counts) += 1;
    else if (r.verdict == V::SAME_SIZE) std::get<2>(counts) += 1;
    else std::get<3>(counts) += 1;
}

}  // namespace

Counts run_powerpc(const str& module_path, const std::optional<str>& flags_in, const str& work, const std::shared_ptr<FunctionStore>& fs_in, const Log& log,
                   int64_t limit) {
    auto tools = build::functions::powerpc_tools();
    if (!tools) throw py::SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    const str cl = std::get<0>(*tools);
    const str sha = base::corpus::sha(module_path).value_or(S(""));
    auto fs = fs_in ? fs_in : store::functions::open();
    reconstruct::Prepared prepared = lanes::powerpc::prepare(module_path, py::os::path::join(work, S("source")));
    const auto m = std::static_pointer_cast<decompile::c_text::ModulePPC>(std::static_pointer_cast<translate::l2::Made>(prepared.lane));
    const str header = prepared.header;
    const py::dict<str, str> texts = prepared.texts;
    py::dict<str, int64_t> address;
    for (const auto& kv : m->by_va().items()) address.set(std::get<1>(kv), std::get<0>(kv));
    std::vector<int64_t> vas;
    for (const int64_t v : address.values()) vas.push_back(v);
    std::sort(vas.begin(), vas.end());
    py::list<int64_t> sorted_vas(vas);
    const auto& arch = match::ARCH().at(S("powerpc"));
    const py::dict<int64_t, match::WordMasked> originals = std::get<match::OriginalsPPC>(arch.originals)(*m->image, sorted_vas);
    fs->add_module(sha, py::os::path::basename(module_path).lower(), module_path, S("powerpc"));
    {
        py::list<str> keys;
        for (const str& n : address.keys()) keys.append(prepared.key.at(n));
        fs->add_functions(sha, keys);
    }
    str flags;
    if (!flags_in || flags_in->empty()) {
        std::vector<str> candidates;
        for (const str& n : prepared.order)
            if (originals.contains(address.at(n))) candidates.push_back(n);
        std::vector<str> sample;
        for (size_t i = 0; i < candidates.size() && sample.size() < 60; i += 10) sample.push_back(candidates[i]);
        int64_t best_got = -1;
        str best = match::POWERPC_FLAG_SETS[0];
        for (const str& f : match::POWERPC_FLAG_SETS) {
            int64_t got = 0;
            for (const str& n : sample) {
                auto b = match::compile_powerpc(cl, header, texts.at(n), n, py::os::path::join(work, S("flags")), f);
                if (b) {
                    const str v = std::get<0>(match::distance(Masked(std::get<0>(*b), match::Mask(std::get<1>(*b))),
                                                               Masked(std::get<0>(originals.at(address.at(n))), match::Mask(std::get<1>(originals.at(address.at(n))))), S("powerpc")));
                    got += v == V::IDENTICAL ? 3 : v == V::SAME_SIZE ? 1 : 0;
                }
            }
            if (got > best_got || (got == best_got && f.u32() > best.u32())) {   // max((got, f)): the larger tuple
                best_got = got;
                best = f;
            }
        }
        flags = best;
        if (log) log(S("  compiler switches that fit best: ") + flags);
    } else {
        flags = *flags_in;
    }
    py::list<str> todo;
    for (const str& n : prepared.order)
        if (originals.contains(address.at(n))) {
            if (limit && static_cast<int64_t>(todo.size()) >= limit) break;
            todo.append(n);
        }
    auto one = [&](const str& name) -> std::optional<Repaired> {
        const str own = thread_folder(work);
        const auto& o = originals.at(address.at(name));
        return repair(
            [&](const str& text) -> std::optional<Masked> {
                auto b = match::compile_powerpc(cl, header, text, name, own, flags);
                if (!b) return std::nullopt;
                return Masked(std::get<0>(*b), match::Mask(std::get<1>(*b)));
            },
            texts.at(name), Masked(std::get<0>(o), match::Mask(std::get<1>(o))), BITS_PPC);
    };
    auto results = jobs::pool::threads(one, todo, base::parallel::workers());
    Counts counts{0, 0, 0, 0};
    for (int64_t i = 0; i < static_cast<int64_t>(todo.size()); ++i) {
        const str& name = todo[i];
        const std::optional<Repaired>& r = results[i];
        if (!r) continue;
        fs->record_variant(sha, prepared.key.at(name), S("xbox 360 cl ") + flags, r->source, eng::Cell(r->distance), eng::Cell(r->total), r->verdict, r->knobs,
                           eng::Cell(), S(""));
        tally(counts, *r);
        if (log && !r->knobs.empty())
            log(py::percent(S("  %-28s %-9s distance %d/%d  via %s"), name, r->verdict, r->distance, r->total, r->knobs));
    }
    return counts;
}

Counts run(const str& module_path, const std::optional<str>& flags_in, const str& work, const std::shared_ptr<FunctionStore>& fs_in, const Log& log,
           int64_t limit) {
    const str cl = match::compiler();
    if (cl.empty()) throw py::SystemExit("no period compiler (settings.json: period_compiler)");
    const str sha = base::corpus::sha(module_path).value_or(S(""));
    auto fs = fs_in ? fs_in : store::functions::open();
    const py::dict<str, str> env = match::compiler_env(cl);
    reconstruct::Prepared prepared = lanes::win32::prepare(module_path, py::os::path::join(work, S("source")));
    const auto m = std::static_pointer_cast<decompile::c_text::Module32>(prepared.lane);
    py::dict<str, int64_t> address;
    for (const auto& kv : m->by_va().items()) address.set(std::get<1>(kv), std::get<0>(kv));
    std::vector<int64_t> vas;
    for (const int64_t v : address.values()) vas.push_back(v);
    std::sort(vas.begin(), vas.end());
    // (match::original_functions reads the functions' extents through a hook: pe_functions.function_spans, which has its port)
    match::set_function_spans([](const str& path) {
        py::dict<int64_t, int64_t> out;
        for (const auto& kv : formats::pe_functions::function_spans(path).items()) out.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
        return out;
    });
    const py::dict<int64_t, match::ByteMasked> originals = match::original_functions(module_path, py::list<int64_t>(vas));
    py::dict<str, str> behaves;
    for (const auto& r : fs->candidates(sha, std::get<0>(lanes::win32::settings_profile(S("pe"))))) {
        if (std::holds_alternative<str>(r.addr) && std::holds_alternative<str>(r.verdict)) behaves.set(std::get<str>(r.addr), std::get<str>(r.verdict));
    }
    fs->add_module(sha, py::os::path::basename(module_path).lower(), module_path, S("pe"));
    {
        py::list<str> keys;
        for (const int64_t a : address.values()) keys.append(lanes::win32::LANES.at(S("pe")).key(a));
        fs->add_functions(sha, keys);
    }
    // the declarations a period compiler is given: imports as imports, globals as variables
    py::set<str> imports;
    for (const str& i : m->imports) imports.add(i);
    auto [header, texts] = match::period_source(prepared.header, prepared.texts, imports, match::relocation_targets(module_path));
    str flags;
    if (!flags_in || flags_in->empty()) {
        flags = best_flags(cl, env, header, texts, prepared.order, originals, address, py::os::path::join(work, S("flags")));
        if (log) log(S("  compiler switches that fit best: ") + flags);
    } else {
        flags = *flags_in;
    }
    py::list<str> todo;
    for (const str& n : prepared.order)
        if (originals.contains(address.at(n))) {
            if (limit && static_cast<int64_t>(todo.size()) >= limit) break;
            todo.append(n);
        }
    auto one = [&](const str& name) -> std::optional<Repaired> {
        const str own = thread_folder(work);   // a folder per worker
        return repair_function(cl, env, header, texts.at(name), name, masked_(originals.at(address.at(name))), own, flags);
    };
    auto results = jobs::pool::threads(one, todo, base::parallel::workers());
    Counts counts{0, 0, 0, 0};
    for (int64_t i = 0; i < static_cast<int64_t>(todo.size()); ++i) {
        const str& name = todo[i];
        const int64_t va = address.at(name);
        const std::optional<Repaired>& r = results[i];
        if (!r) continue;
        const str key = lanes::win32::LANES.at(S("pe")).key(va);
        fs->record_variant(sha, key, py::os::path::basename(cl).lower() + S(" ") + flags, r->source, eng::Cell(r->distance), eng::Cell(r->total), r->verdict,
                           r->knobs, eng::Cell(), behaves.get(key, S("")));
        tally(counts, *r);
        if (log && !r->knobs.empty())
            log(py::percent(S("  %-28s %-9s distance %d/%d  via %s"), name, r->verdict, r->distance, r->total, r->knobs));
    }
    return counts;
}

const str DOC = S(
    "python -m codebreaker repair <module> [--flags \"/O2 /Gz\"] [--json FILE]\n"
    "\n"
    "Move each function's reconstruction toward the authored bytes with behaviour-preserving\n"
    "rewrites, compiled with a period compiler. Records the variants in the store.");

int64_t main(const py::list<str>& argv, const Periods& periods) {
    if (argv.contains(S("--truth"))) {
        int64_t at = 0;
        for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
            if (argv[i] == S("--truth")) {
                at = i;
                break;
            }
        py::list<str> rest;
        for (int64_t i = at + 1; i < static_cast<int64_t>(argv.size()); ++i) rest.append(argv[i]);
        if (!periods.names || !periods.get) throw py::NotImplementedError("build.period has no typed port");
        if (rest.empty()) throw py::SystemExit(py::percent(S("repair --truth <compiler> [target]   (have: %s)"), py::join(S(", "), periods.names())));
        py::list<str> two;
        for (int64_t i = 0; i < std::min<int64_t>(2, rest.size()); ++i) two.append(rest[i]);
        auto [n, same, knocked, back] = truth(periods.get(two));
        py::print(py::percent(S("%s: %d sources, %d identical as they stand; %d knocked away from the original, %d brought back"), py::join(S(" "), two), n, same,
                              knocked, back));
        return 0;
    }
    py::list<str> args;
    for (const str& a : argv)
        if (!a.startswith(S("--"))) args.append(a);
    std::optional<str> flags;
    if (argv.contains(S("--flags"))) {
        for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
            if (argv[i] == S("--flags")) {
                if (i + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
                flags = argv[i + 1];
                break;
            }
    }
    int64_t limit = 0;
    if (argv.contains(S("--first"))) {
        for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
            if (argv[i] == S("--first")) {
                if (i + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
                limit = py::int_parse(argv[i + 1]);
                break;
            }
    }
    py::list<str> kept;
    for (const str& a : args)
        if (!(flags && a == *flags) && !(argv.contains(S("--first")) && a == py::to_str(limit))) kept.append(a);
    args = kept;
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    std::optional<str> path = py::os::path::isfile(args[0]) ? std::optional<str>(args[0]) : base::corpus::retail(args[0]);
    bool powerpc = args[0].lower().endswith(S(".xex"));
    if (powerpc) {
        path = formats::xex::basefile(args[0]);
    } else if (path && base::corpus::lane(*path) == std::optional<str>(S("pe"))) {
        powerpc = formats::pe::open_pe(*path).machine == 0x01F2;
    }
    if (!path || base::corpus::lane(*path) != std::optional<str>(S("pe")))
        throw py::SystemExit(py::percent(S("%s: a 32-bit Windows module or a PowerPC image is needed"), args[0]));
    const str work = py::os::path::join(py::settings::get_str(S("state")), S("repair"), py::os::path::basename(*path).lower());
    py::os::makedirs(py::os::path::dirname(work), 0777, true);
    auto [n, ident, same, diff] = powerpc ? run_powerpc(*path, flags, work, nullptr, log_print, limit) : run(*path, flags, work, nullptr, log_print, limit);
    py::print(py::percent(S("%s: %d functions repaired toward the original's bytes: %d identical, %d same size, %d different"), py::os::path::basename(*path), n,
                          ident, same, diff));
    return 0;
}

}  // namespace codebreaker::verify::authored::repair
