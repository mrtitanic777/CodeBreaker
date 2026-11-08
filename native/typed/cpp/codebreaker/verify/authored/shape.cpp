// codebreaker/verify/authored/shape.cpp -- C++ port of codebreaker.verify.authored.shape (see shape.hpp).

#include "codebreaker/verify/authored/shape.hpp"

#include <algorithm>

namespace codebreaker::verify::authored::shape {

const py::str _NUM = R"re((?:-?0x[0-9a-fA-F]+|-?\d+))re";
const py::str _NAME = R"re([A-Za-z_]\w*)re";

namespace {

// int(text, 0) for the text _NUM matches: Python refuses a decimal with a leading zero ("012") when the base is 0.
int64_t int_base0(const py::str& text) {
    if (text.contains(py::str("0x"))) {
        return py::int_from_str(text, 16);
    }
    py::str digits = text.startswith(py::str("-")) ? text.slice_from(1) : text;
    if (py::len(digits) > 1 && digits[0] == "0" && !digits.strip(py::str("0")).empty()) {
        throw py::ValueError(py::percent("invalid literal for int() with base 0: %r", text));
    }
    return py::int_from_str(text, 10);
}

// (pyre caches compiled patterns; a Pattern is a handle to one)
const py::re::Pattern compiled(const py::str& pattern, int64_t flags = 0) { return py::re::compile(pattern, flags); }

}  // namespace

// The index of the bracket that closes the one at `i`, or -1.
int64_t _close(const py::str& shadow, int64_t i, const py::str& pair) {
    int64_t depth = 0;
    for (int64_t j = i; j < py::len(shadow); ++j) {
        if (shadow[j] == pair[0]) {
            depth += 1;
        } else if (shadow[j] == pair[1]) {
            depth -= 1;
            if (depth == 0) {
                return j;
            }
        }
    }
    return -1;
}

// [(start, end)] of the statements between `lo` and `hi`, an if with all its elses as one.
py::list<Span> statements(const py::str& shadow, int64_t lo, int64_t hi) {
    py::list<Span> out;
    int64_t i = lo;
    const py::re::Pattern do_word = compiled(py::str("do\\b"));
    const py::re::Pattern else_word = compiled(py::str("\\s*else\\b"));
    const py::re::Pattern while_word = compiled(py::str("\\s*while\\b"));
    while (i < hi) {
        while (i < hi && shadow[i].isspace()) {
            i += 1;
        }
        if (i >= hi) {
            break;
        }
        int64_t start = i, depth = 0;
        bool loop = do_word.match(shadow.slice(i, hi)).has_value();
        while (i < hi) {
            py::str c = shadow[i];
            if (c == "(") {
                depth += 1;
            } else if (c == ")") {
                depth -= 1;
            } else if (c == ";" && depth == 0) {
                i += 1;
                break;
            } else if (c == "{" && depth == 0) {
                int64_t j = _close(shadow, i);
                if (j < 0 || j >= hi) {
                    return out;  // not well formed: say nothing more
                }
                i = j + 1;
                py::str rest = shadow.slice(i, hi);
                std::optional<py::re::Match> again = else_word.match(rest);
                if (again.has_value()) {
                    i += again->end();
                    continue;
                }
                if (loop && while_word.match(rest).has_value()) {
                    continue;  // do { } while (...);
                }
                break;
            }
            i += 1;
        }
        out.append(Span(start, i));
    }
    return out;
}

// (start, end, text) for each chain `if (v == A) {..} else if (v == B) {..} [else {..}]`:
// the same as a switch. Not when a branch holds a `break` (it would leave the switch instead
// of the loop it was in), nor when a value is tested twice.
py::list<Edit> switches(const py::str& body, const py::str& shadow) {
    py::list<Edit> out;
    const py::re::Pattern head = compiled(py::percent(R"re(\bif \((%s) == (%s)\) \{)re", _NAME, _NUM));
    const py::re::Pattern last_else = compiled(py::str("\\s*else \\{"));
    const py::re::Pattern word_break = compiled(py::str("\\bbreak\\b"));
    for (const py::re::Match& m : head.finditer(shadow)) {
        py::str before = shadow.slice(0, m.start()).rstrip();
        if (before.endswith(py::str("else")) || !py::str(";{}").contains(before.slice(-1, py::none))) {
            continue;  // inside a chain, or not a statement
        }
        py::str v = m.group(1);
        py::list<std::tuple<py::str, int64_t, int64_t>> cases;
        std::optional<std::tuple<int64_t, int64_t>> default_;
        int64_t at = m.start();
        py::str value = m.group(2);
        int64_t brace = m.end() - 1;
        const py::re::Pattern next_case = compiled(py::percent(R"re(\s*else if \(%s == (%s)\) \{)re", py::re::escape(v), _NUM));
        for (;;) {
            int64_t end = _close(shadow, brace);
            if (end < 0) {
                cases = py::list<std::tuple<py::str, int64_t, int64_t>>();
                break;
            }
            cases.append(std::make_tuple(value, brace, end));
            at = end + 1;
            std::optional<py::re::Match> again = next_case.match(shadow.slice_from(at));
            if (again.has_value()) {
                value = again->group(1);
                brace = at + again->end() - 1;
                continue;
            }
            std::optional<py::re::Match> last = last_else.match(shadow.slice_from(at));
            if (last.has_value()) {
                brace = at + last->end() - 1;
                end = _close(shadow, brace);
                if (end < 0) {
                    cases = py::list<std::tuple<py::str, int64_t, int64_t>>();
                    break;
                }
                default_ = std::make_tuple(brace, end);
                at = end + 1;
            }
            break;
        }
        py::list<int64_t> values;
        for (const auto& c : cases) {
            values.append(int_base0(std::get<0>(c)) & 0xFFFFFFFF);
        }
        if (py::len(cases) < 2 || py::set<int64_t>::from(values).size() != py::len(values)) {
            continue;
        }
        if (word_break.search(shadow.slice(m.start(), at)).has_value()) {
            continue;
        }
        py::list<py::str> lines = {py::percent("switch (%s) {", v)};
        for (const auto& c : cases) {
            lines.append(py::percent("case %s:\n{%s}\nbreak;", std::get<0>(c), body.slice(std::get<1>(c) + 1, std::get<2>(c))));
        }
        if (default_.has_value()) {
            lines.append(py::percent("default:\n{%s}", body.slice(std::get<0>(*default_) + 1, std::get<1>(*default_))));
        }
        lines.append(py::str("}"));
        out.append(Edit(m.start(), at, py::join(py::str("\n"), lines)));
    }
    return out;
}

// Collect, for the statement at [start, end), every `r = x;` that is the last thing done
// before control reaches the end of the function.
void _tail(const py::str& body, const py::str& shadow, int64_t start, int64_t end, const py::str& r, const py::list<Edit>& edits) {
    py::str text = shadow.slice(start, end);
    std::optional<py::re::Match> m = compiled(py::percent(R"re(%s = ([^;{}]+);\s*$)re", py::re::escape(r))).match(text);
    if (m.has_value() && !text.contains(py::str("{"))) {
        edits.append(Edit(start, end, py::percent("return %s;", body.slice(start + m->start(1), start + m->end(1)).strip())));
        return;
    }
    if (!compiled(py::str("if\\s*\\(")).match(text).has_value()) {
        return;
    }
    int64_t at = start + text.index(py::str("("));
    int64_t close = _close(shadow, at, py::str("()"));
    const py::re::Pattern else_word = compiled(py::str("\\s*else\\b\\s*"));
    const py::re::Pattern if_open = compiled(py::str("if\\s*\\("));
    while (close >= 0) {
        int64_t brace = close + 1;
        while (brace < end && shadow[brace].isspace()) {
            brace += 1;
        }
        if (brace >= end || shadow[brace] != "{") {
            return;
        }
        int64_t stop = _close(shadow, brace);
        if (stop < 0) {
            return;
        }
        py::list<Span> inner = statements(shadow, brace + 1, stop);
        if (!inner.empty()) {
            _tail(body, shadow, std::get<0>(inner[-1]), std::get<1>(inner[-1]), r, edits);
        }
        std::optional<py::re::Match> rest = else_word.match(shadow.slice(stop + 1, end));
        if (!rest.has_value()) {
            return;
        }
        at = stop + 1 + rest->end();
        if (shadow.slice(at, at + 1) == "{") {  // else { ... }
            stop = _close(shadow, at);
            py::list<Span> inner2 = stop >= 0 ? statements(shadow, at + 1, stop) : py::list<Span>();
            if (!inner2.empty()) {
                _tail(body, shadow, std::get<0>(inner2[-1]), std::get<1>(inner2[-1]), r, edits);
            }
            return;
        }
        std::optional<py::re::Match> nxt = if_open.match(shadow.slice(at, end));  // else if (...)
        if (!nxt.has_value()) {
            return;
        }
        at += nxt->end() - 1;
        close = _close(shadow, at, py::str("()"));
    }
}

// The body with every `r = x;` that only the final `return r;` follows written `return x;`,
// or None when there is none. Only when `r` has the type the function returns: handing a value
// back through a narrower variable cuts it down, and returning it directly would not.
std::optional<py::str> returns(const py::str& head, const py::list<py::str>& decls, const py::str& body, const py::str& shadow) {
    py::list<Span> top = statements(shadow, 0, py::len(shadow.rstrip().rstrip(py::str("}"))));
    if (py::len(top) < 2) {
        return std::nullopt;
    }
    std::optional<py::re::Match> last = compiled(py::percent(R"re(return (%s);\s*$)re", _NAME)).match(shadow.slice(std::get<0>(top[-1]), std::get<1>(top[-1])));
    if (!last.has_value()) {
        return std::nullopt;
    }
    py::str r = last->group(1);
    std::optional<py::re::Match> own = compiled(py::percent(R"re((?m)^\s*(\w+)\s+%s;\s*$)re", py::re::escape(r))).search(py::join(py::str("\n"), decls));
    if (!own.has_value()) {
        own = compiled(py::percent(R"re([(,]\s*(\w+)\s+%s\s*[,)])re", py::re::escape(r))).search(head);
    }
    std::optional<py::str> returned = _returned(head);
    if (!own.has_value() || !returned.has_value() || own->group(1) != *returned) {
        return std::nullopt;
    }
    py::list<Edit> edits;
    _tail(body, shadow, std::get<0>(top[-2]), std::get<1>(top[-2]), r, edits);
    if (edits.empty()) {
        return std::nullopt;
    }
    std::vector<Edit> sorted_edits(edits.vec().begin(), edits.vec().end());
    std::sort(sorted_edits.begin(), sorted_edits.end(), [](const Edit& a, const Edit& b) { return b < a; });  // reverse=True
    py::str result = body;
    for (const Edit& e : sorted_edits) {
        result = result.slice(0, std::get<0>(e)) + std::get<2>(e) + result.slice_from(std::get<1>(e));
    }
    return result;
}

// The type the function returns, from its heading (the word before its convention and name).
std::optional<py::str> _returned(const py::str& head) {
    py::list<py::str> lines;
    for (const py::str& ln : py::split(head, py::str("\n"))) {
        if (ln.contains(py::str("("))) {
            lines.append(ln);
        }
    }
    if (lines.empty()) {
        return std::nullopt;
    }
    py::list<py::str> words;
    for (const py::str& w : py::split(py::split(lines[-1], py::str("("))[0])) {
        if (!w.startswith(py::str("__"))) {
            words.append(w);
        }
    }
    if (py::len(words) >= 2) {
        return words[-2];
    }
    return std::nullopt;
}

// (start, end, text) for each `t = f(...);` followed at once by `if (t <compare> N) {`, where
// that is all `t` is for: written `if (f(...) <compare> N) {`. A decompiler names every value it
// sees; a person tests the call. Only for a variable of the type a function returns (int4), so
// that nothing is cut down or widened on the way, and only when the two lines are its only
// mentions.
py::list<Edit> folds(const py::list<py::str>& decls, const py::str& body, const py::str& shadow) {
    py::list<Edit> out;
    py::set<py::str> plain;
    const py::re::Pattern decl = compiled(py::percent(R"re(\s*int4 (%s);\s*$)re", _NAME));
    for (const py::str& d : decls) {
        std::optional<py::re::Match> m = decl.match(d);
        if (m.has_value()) {
            plain.add(m->group(1));
        }
    }
    // groups: 1 in, 2 t, 3 call, 4 op, 5 n
    const py::re::Pattern line = compiled(py::percent(
        R"re((?m)^(?P<in>[ \t]*)(?P<t>%s) = (?P<call>%s\([^;\n]*\));\n[ \t]*if \((?P=t) (?P<op>==|!=|<=|>=|<|>) (?P<n>%s)\) \{)re",
        _NAME, _NAME, _NUM));
    for (const py::re::Match& m : line.finditer(shadow)) {
        py::str t = m.group(2);
        if (!plain.contains(t) || py::len(compiled(py::percent(R"re(\b%s\b)re", py::re::escape(t))).findall(shadow)) != 2) {
            continue;
        }
        py::str call = body.slice(m.start(3), m.end(3));
        if (_close(shadow, m.start(3) + shadow.slice_from(m.start(3)).index(py::str("(")), py::str("()")) != m.end(3) - 1) {
            continue;  // more than the one call on that line
        }
        out.append(Edit(m.start(), m.end(), py::percent("%sif (%s %s %s) {", m.group(1), call, m.group(4), m.group(5))));
    }
    return out;
}

}  // namespace codebreaker::verify::authored::shape