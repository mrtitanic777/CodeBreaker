// codebreaker/exact/bench.cpp -- typed C++ of codebreaker.exact.bench (see bench.hpp).

#include "codebreaker/exact/bench.hpp"

#include <algorithm>

#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/draft/package.hpp"

namespace codebreaker::exact::bench {

namespace {

using py::bytes;
using py::str;

str first(const std::string& s, int64_t n) { return str(s).slice(0, n); }   // str(e)[:n]

bool is_hex(const str& x) {
    for (char32_t c : x.u32()) {
        if (!((c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f') || (c >= U'A' && c <= U'F'))) return false;
    }
    return true;
}

}  // namespace

py::list<std::tuple<int64_t, str, str>> run(project::Project& prj, const std::optional<py::list<int64_t>>& want, bool show, std::ostream& out) {
    auto tc = prj.toolchain();
    const auto& inverse = project::profile_parts(*tc).inverse;
    if (!inverse) throw py::KeyError("'inverse'");
    auto dv = project::as_draft(prj);
    std::shared_ptr<draft::InverseModel> inv = dv->load_inverse_model(*inverse);
    py::dict<int64_t, str> names = draft::_names(*dv);
    const bytes& img = prj.image;
    py::dict<int64_t, int64_t> fs;
    for (const auto& f : carve::functions(prj)) fs.set(std::get<0>(f), std::get<1>(f));
    py::dict<int64_t, str> have;
    for (const auto& s : prj.sources()) {
        if (fs.contains(std::get<0>(s))) have.set(std::get<0>(s), std::get<1>(s));
    }
    py::list<int64_t> todo = want && !want->empty() ? *want : py::list<int64_t>::from(have.keys());
    py::list<std::tuple<int64_t, str, str>> rows;
    for (int64_t a : py::sorted(todo)) {
        if (!fs.contains(a)) continue;
        int64_t ln = fs.at(a);
        py::list<std::tuple<str, py::dict<str, int64_t>>> texts;
        try {
            texts = draft::model_texts(*inv, img.slice(a, a + ln), py::percent(str("f%05x"), a), a);
        } catch (const py::Exception& e) {
            rows.append(std::make_tuple(a, str("declined: ") + first(e.what(), 80), str("")));
            continue;
        }
        str src, v;
        try {
            std::tie(src, v) = dv->prove_texts(*inv, texts, a, ln, names);       // (a pass that fails on one function: noted, not fatal)
        } catch (const py::Exception& e) {
            src = str("Traceback (most recent call last):\n") + str(e.type()) + str(": ") + str(e.what()) + str("\n");
            v = py::percent(str("failed: %s: %s"), str(e.type()), first(e.what(), 80));
        }
        rows.append(std::make_tuple(a, v, src));
        if (show) {
            out << py::percent(str("== %05x %d bytes: %s\n%s"), a, ln, v, src).utf8() << "\n";
            if (have.contains(a)) {
                py::file f = py::open(have.at(a), str("r"), str("latin1"));
                out << (str("-- the source:\n") + f.read_text()).utf8() << "\n";
                f.close();
            }
        }
    }
    return rows;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    py::list<int64_t> want;
    for (int64_t k = 0; k < argv.size(); ++k) {
        const str& x = argv[k];
        if (k > 0 && argv[k - 1] == "--out") continue;      // (the file's name is not an address, whatever it is made of)
        if (!x.empty() && !x.startswith(str("-")) && is_hex(x)) want.append(py::int_parse(x, 16));
    }
    auto rows = run(prj, want.empty() ? std::nullopt : std::optional<py::list<int64_t>>(want), argv.contains(str("--show")), out);
    py::dict<str, int64_t> tally;
    for (const auto& r : rows) {
        str k = py::split(std::get<1>(r), str(":"))[0];
        tally[k] = tally.get(k, 0) + 1;
    }
    auto items = tally.items();
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
    if (argv.contains(str("--out"))) {
        py::file f = py::open(argv[argv.index(str("--out")) + 1], str("w"));
        for (const auto& r : rows) f.write(py::percent(str("%05x %s\n"), std::get<0>(r), std::get<1>(r)));
        f.close();
    }
    std::u32string parts;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i) parts += U", ";
        parts += py::percent(str("%d %s"), std::get<1>(items[i]), std::get<0>(items[i])).u32();
    }
    out << py::percent(str("%d functions: %s"), rows.size(), str(parts)).utf8() << "\n";
    return 0;
}

}  // namespace codebreaker::exact::bench
