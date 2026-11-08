// codebreaker/exact/batch.cpp -- typed C++ of codebreaker.exact.batch (see batch.hpp).
#include "codebreaker/exact/batch.hpp"

#include <algorithm>
#include <vector>

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/exact/omf.hpp"

namespace codebreaker::exact::batch {

const py::set<py::str> KEYWORDS = py::set<py::str>::from(std::vector<py::str>{
    py::str("extern"), py::str("int"), py::str("char"), py::str("long"), py::str("short"), py::str("unsigned"),
    py::str("signed"), py::str("struct"), py::str("union"), py::str("double"), py::str("float"),
    py::str("register"), py::str("static"), py::str("void"), py::str("auto"), py::str("typedef"),
    py::str("enum"), py::str("sizeof"), py::str("return"), py::str("if"), py::str("else"), py::str("while"),
    py::str("for"), py::str("do"), py::str("switch"), py::str("case"), py::str("default"), py::str("break"),
    py::str("continue"), py::str("goto")});
const py::re::Pattern SUFFIX = py::re::compile(py::str("__Q\\d+$"));

std::optional<std::tuple<py::str, py::str>> split(const py::str& src) {
    py::list<py::str> pre, body;
    for (const py::str& l : py::split(src, py::str("\n"))) {
        py::str s = l.strip();
        if (py::len(body) == 0 &&
            (s.startswith(py::str("#include")) ||
             py::re::fullmatch(py::str("/\\* \\w+: [^*]*\\*/"), s).has_value() || s.empty())) {
            pre.append(l);
            continue;
        }
        if (s.startswith(py::str("#"))) return std::nullopt;
        body.append(l);
    }
    py::str b = base::ctext::blank(py::join(py::str("\n"), body), true, true);
    if (py::re::findall(py::str("\\{"), b).size() != py::re::findall(py::str("\\}"), b).size() ||
        py::re::findall(py::str("\\("), b).size() != py::re::findall(py::str("\\)"), b).size())
        return std::nullopt;   // (a text that does not hold together: its errors would run on into the next one's)
    py::list<py::str> kept;
    for (const py::str& x : pre)
        if (!x.strip().empty()) kept.append(x);
    return std::make_tuple(py::join(py::str("\n"), kept), py::join(py::str("\n"), body));
}

py::set<py::str> own_names(const py::str& body) {
    py::set<py::str> names;
    auto m = py::re::compile(py::str("(?m)^(?!extern\\b|struct\\b|static\\b)(?:[\\w *]+\\n)?([A-Za-z_]\\w*)\\("))
                 .search(body);
    if (m) names.add(m->group(1));
    for (const py::str& l0 : py::re::findall(py::str("(?m)^extern\\b([^;]*);"), body)) {
        py::str l = py::re::sub(py::str("\\bstruct\\s+\\w+"), py::str(" "), l0);
        for (const py::str& n : py::re::findall(py::str("([A-Za-z_]\\w*)\\s*(?=[(\\[,);]|$)"), l))
            if (!KEYWORDS.contains(n)) names.add(n);
    }
    for (const py::str& n : py::re::findall(py::str("\\bstruct\\s+(\\w+)\\s*\\{"), body)) names.add(n);
    return names;
}

py::str renamed(const py::str& body, int64_t k) {
    py::set<py::str> names = own_names(body);
    if (names.empty()) return body;
    std::vector<py::str> esc;
    for (const py::str& n : names) esc.push_back(py::re::escape(n));
    std::stable_sort(esc.begin(), esc.end(),
                     [](const py::str& a, const py::str& b) { return py::len(a) > py::len(b); });   // key=len, reverse=True
    py::re::Pattern rx = py::re::compile(py::str("\\b(") + py::join(py::str("|"), esc) + py::str(")\\b"));
    py::str out;
    for (const py::str& piece : py::re::split(py::str("(\"(?:[^\"\\\\]|\\\\.)*\")"), body)) {   // (not inside a string)
        if (piece.startswith(py::str("\""))) {
            out = out + piece;
        } else {
            out = out + rx.sub([k](const py::re::Match& m) { return m.group(1) + py::str("__Q") + py::to_str(k); }, piece);
        }
    }
    return out;
}

std::optional<std::tuple<py::str, py::list<std::tuple<int64_t, int64_t>>, py::list<int64_t>>> build(
    const py::list<py::str>& srcs) {
    std::vector<std::optional<std::tuple<py::str, py::str>>> parts;
    for (const py::str& s : srcs) parts.push_back(batch::split(s));
    std::vector<int64_t> ok;
    for (size_t i = 0; i < parts.size(); i++)
        if (parts[i]) ok.push_back(static_cast<int64_t>(i));
    if (ok.size() < 2) return std::nullopt;
    py::str pre = std::get<0>(*parts[static_cast<size_t>(ok[0])]);
    std::vector<int64_t> same;
    for (int64_t i : ok)
        if (std::get<0>(*parts[static_cast<size_t>(i)]) == pre) same.push_back(i);
    ok = same;
    if (ok.size() < 2) return std::nullopt;
    py::list<py::str> lines;
    if (!pre.empty())
        for (const py::str& x : py::split(pre, py::str("\n"))) lines.append(x);
    py::list<std::tuple<int64_t, int64_t>> spans;
    py::list<int64_t> idx;
    for (size_t k = 0; k < ok.size(); k++) {
        int64_t i = ok[k];
        py::list<py::str> b = py::split(renamed(std::get<1>(*parts[static_cast<size_t>(i)]), static_cast<int64_t>(k)), py::str("\n"));
        int64_t first = py::len(lines) + 1;
        for (const py::str& x : b) lines.append(x);
        lines.append(py::str(""));
        spans.append(std::make_tuple(first, py::len(lines)));
        idx.append(i);
    }
    return std::make_tuple(py::join(py::str("\n"), lines) + py::str("\n"), spans, idx);
}

py::set<int64_t> error_lines(const py::str& con) {
    py::set<int64_t> out;
    for (const auto& m : py::re::finditer(py::str("\\.C\\((\\d+)\\)\\s*:\\s*(?!warning)"), con, py::re::I))
        out.add(py::int_parse(m.group(1), 10));   // (3.00 names only warnings)
    return out;
}

py::dict<int64_t, std::tuple<py::bytes, int64_t>> compile_many(const Project& prj, Toolchain& tc,
                                                               const py::list<py::str>& srcs, int64_t rounds) {
    py::dict<int64_t, std::tuple<py::bytes, int64_t>> none;
    std::vector<int64_t> want;
    for (int64_t i = 0; i < py::len(srcs); i++) want.push_back(i);
    for (int64_t round = 0; round < rounds; round++) {
        py::list<py::str> sub;
        for (int64_t i : want) sub.append(srcs[i]);
        auto b = build(sub);
        if (!b) return none;
        const py::str& text = std::get<0>(*b);
        const auto& spans = std::get<1>(*b);
        const auto& ok = std::get<2>(*b);
        std::vector<int64_t> idx;
        for (int64_t i : ok) idx.push_back(want[static_cast<size_t>(i)]);
        auto [obj, con] = tc.compile(text.encode("latin1", "replace"), prj.headers(), prj.include, prj.switches);
        if (obj) {
            py::bytes renamed_obj = omf::rename_symbols(*obj, [](const py::str& n) { return SUFFIX.sub(py::str(""), n); });
            py::dict<int64_t, std::tuple<py::bytes, int64_t>> out;
            for (size_t k = 0; k < idx.size(); k++) out.set(idx[k], std::make_tuple(renamed_obj, static_cast<int64_t>(k)));
            return out;
        }
        py::set<int64_t> bad = error_lines(con);
        if (bad.empty()) return none;
        std::vector<bool> drop(idx.size(), false);
        bool any = false;
        for (size_t k = 0; k < idx.size(); k++) {
            auto [lo, hi] = spans[static_cast<int64_t>(k)];
            for (int64_t l : bad)
                if (lo <= l && l <= hi) {
                    drop[k] = true;
                    any = true;
                    break;
                }
        }
        if (!any) return none;
        want.clear();
        for (size_t k = 0; k < idx.size(); k++)
            if (!drop[k]) want.push_back(idx[k]);
        if (want.size() < 2) return none;
    }
    return none;
}

}  // namespace codebreaker::exact::batch
