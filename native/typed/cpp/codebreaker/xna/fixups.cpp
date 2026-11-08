// codebreaker/xna/fixups.cpp -- C++ port of codebreaker.xna.fixups.

#include "codebreaker/xna/fixups.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::xna::fixups {

const py::str _LVALUE(
    "(?:[A-Za-z_]\\w*(?:\\s*\\[[^\\[\\]]*\\])?)(?:\\s*\\.\\s*[A-Za-z_]\\w*(?:\\s*\\[[^\\[\\]]*\\])?)*");

const py::re::Pattern _REF_CAST(py::str("\\(\\s*[A-Za-z_][\\w.]*\\s*\\)\\s*\\(\\s*ref\\s+(") + _LVALUE + py::str(")\\s*\\)"), 0);

const py::re::Pattern _CTOR(
    py::str("(?P<base>base|this)\\._002Ector\\s*\\("
            "|\\(\\([A-Za-z_][A-Za-z0-9_.]*\\)this\\)\\._002Ector\\s*\\("
            "|\\(?(?P<var>[A-Za-z_][A-Za-z0-9_]*)\\)?\\._002Ector\\s*\\("),
    0);

std::tuple<py::str, int64_t> ref_casts(const py::str& text0) {
    py::str text = text0;
    int64_t total = 0;
    bool first = true;
    py::str prev;
    while (first || prev != text) {  // nested: ((IDisposable)(Enumerator)(ref e))
        first = false;
        prev = text;
        int64_t n;
        std::tie(text, n) = _REF_CAST.subn(py::str("\\1"), text);
        total += n;
    }
    return std::make_tuple(text, total);
}

std::tuple<std::optional<py::str>, int64_t> _matching(const py::str& text, int64_t open_paren) {
    int64_t depth = 0;
    const int64_t stop = py::len(text);
    for (int64_t i = open_paren; i < stop; ++i) {
        if (text[i] == "(") {
            depth += 1;
        } else if (text[i] == ")") {
            depth -= 1;
            if (depth == 0) {
                return std::make_tuple(std::optional<py::str>(text.slice(open_paren + 1, i)), i + 1);
            }
        }
    }
    return std::tuple<std::optional<py::str>, int64_t>{std::nullopt, -1};
}

int64_t _enclosing_brace(const py::str& text, int64_t pos) {
    int64_t depth = 0;
    for (int64_t i = pos - 1; i > -1; --i) {
        if (text[i] == "}") {
            depth += 1;
        } else if (text[i] == "{") {
            if (depth == 0) {
                return i;
            }
            depth -= 1;
        }
    }
    return -1;
}

std::optional<py::str> _declared_type(const py::str& text, const py::str& var, int64_t before) {
    py::re::Pattern rx(py::str("(?m)^[ \\t]*([A-Za-z_][A-Za-z0-9_<>,.\\[\\] ]*?)[ \\t]+") + py::re::escape(var) +
                           py::str("[ \\t]*(?:=(?!=)|;)"),
                       0);
    // (finditer(text, 0, before): the text as if it ended at `before`; a negative endpos is 0)
    const py::str upto = before < 0 ? py::str("") : text.slice(0, before);
    py::list<py::re::Match> hits = rx.finditer(upto);
    if (hits.empty()) return std::nullopt;
    return hits[-1].group(1).strip();
}

std::tuple<py::str, int64_t, py::list<std::tuple<int64_t, py::str>>> constructors(const py::str& text0) {
    py::str text = text0;
    py::list<std::tuple<int64_t, py::str>> left;
    if (!text.contains("_002Ector")) {
        return std::make_tuple(text, int64_t(0), left);
    }
    std::vector<std::tuple<int64_t, int64_t, py::str>> edits;
    for (const py::re::Match& m : _CTOR.finditer(text)) {
        const int64_t from = m.end() - 1;
        const int64_t paren = text.find(py::str("("), from);
        if (paren < 0) throw py::ValueError("substring not found");
        std::optional<py::str> args;
        int64_t after;
        std::tie(args, after) = _matching(text, paren);
        if (!args) {
            continue;
        }
        const int64_t line = text.slice(0, m.start()).count(py::str("\n")) + 1;
        const std::optional<py::str> base_g = m.group_opt(1);   // (?P<base>)
        const std::optional<py::str> var_g = m.group_opt(2);    // (?P<var>)
        if ((base_g && !base_g->empty()) || !var_g) {
            // ((Base)this).ctor is the base constructor
            const py::str kind = (base_g && !base_g->empty()) ? *base_g : py::str("base");
            const int64_t brace = _enclosing_brace(text, m.start());
            const int64_t close = brace > 0 ? text.rfind(py::str(")"), 0, brace) : -1;
            if (close <= 0) {
                left.append(std::make_tuple(line, py::str("no constructor parameter list before it")));
                continue;
            }
            edits.emplace_back(close + 1, close + 1, py::str(" : ") + kind + py::str("(") + *args + py::str(")"));
            const int64_t start = text.rfind(py::str("\n"), 0, m.start()) + 1;
            const int64_t end = text.find(py::str("\n"), m.start());
            edits.emplace_back(start, end < 0 ? py::len(text) : end + 1, py::str(""));
        } else {
            const py::str var = *var_g;
            const std::optional<py::str> typ = _declared_type(text, var, m.start());
            if (!typ || typ->empty()) {
                left.append(std::make_tuple(line, py::str("no declaration of ") + var + py::str(" before it")));
                continue;
            }
            edits.emplace_back(m.start(), after, var + py::str(" = new ") + *typ + py::str("(") + *args + py::str(")"));
        }
    }
    int64_t applied = 0;
    int64_t last = py::len(text) + 1;
    std::stable_sort(edits.begin(), edits.end(),
                     [](const auto& x, const auto& y) { return std::get<0>(x) > std::get<0>(y); });
    for (const auto& edit : edits) {
        const int64_t s = std::get<0>(edit);
        const int64_t e = std::get<1>(edit);
        if (e > last) {
            continue;
        }
        text = text.slice(0, s) + std::get<2>(edit) + text.slice(e, py::len(text));
        last = s;
        applied += 1;
    }
    return std::make_tuple(text, applied, left);
}

std::tuple<py::str, int64_t> data_holders(const py::str& text) {
    if (!text.contains("/* Not supported: data(")) {
        return std::make_tuple(text, int64_t(0));
    }
    return py::re::subn(py::str("\\bprivate(\\s+struct\\s+\\w+)"), py::str("internal\\1"), text);
}

Report apply(const py::str& folder) {
    Report report;
    for (const auto& entry : py::os::walk(folder)) {
        const py::str& base = std::get<0>(entry);
        for (const py::str& f : std::get<2>(entry)) {
            if (!f.endswith(".cs")) {
                continue;
            }
            const py::str p = py::os::path::join(base, f);
            // encoding="utf-8-sig": utf-8 without the byte-order mark the file may begin with
            // (the runtime's open() reads "utf-8-sig" as plain utf-8, which keeps the mark).
            py::file fh = py::open(p, py::str("r"), py::str("utf-8"));
            py::str text = fh.read_text();
            fh.close();
            if (!text.empty() && text.at_(0) == U'\uFEFF') text = text.slice(1, py::len(text));
            py::str fixed;
            int64_t n_ref, n_ctor, n_data;
            py::list<std::tuple<int64_t, py::str>> left;
            std::tie(fixed, n_ref) = ref_casts(text);
            std::tie(fixed, n_ctor, left) = constructors(fixed);
            std::tie(fixed, n_data) = data_holders(fixed);
            report.data_holders += n_data;
            report.ref_casts += n_ref;
            report.constructors += n_ctor;
            for (const auto& item : left) {
                report.left.append(std::make_tuple(py::os::path::relpath(p, folder), std::get<0>(item), std::get<1>(item)));
            }
            if (fixed != text) {
                py::file out = py::open(p, py::str("w"), py::str("utf-8"));
                out.write(fixed);
                out.close();
            }
        }
    }
    return report;
}

}  // namespace codebreaker::xna::fixups
