// codebreaker/verify/measure.cpp -- typed C++ port of codebreaker.verify.measure (see measure.hpp).

#include "codebreaker/verify/measure.hpp"

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::measure {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace eng = codebreaker::engine::native_engine;

// a store cell as the text the Python's str() gives it (None -> "None")
str show(const eng::Cell& c) {
    if (std::holds_alternative<std::monostate>(c)) return str("None");
    return eng::cell_str(c);
}

// collections.Counter's most_common(): by count, the first seen first among equals
struct Tally {
    std::vector<std::pair<str, int64_t>> items;
    void add(const str& key) {
        for (auto& kv : items)
            if (kv.first == key) {
                ++kv.second;
                return;
            }
        items.emplace_back(key, 1);
    }
    std::vector<std::pair<str, int64_t>> most_common(size_t n = static_cast<size_t>(-1)) const {
        std::vector<std::pair<str, int64_t>> v = items;
        std::stable_sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        if (v.size() > n) v.resize(n);
        return v;
    }
};

}  // namespace

const str DOC = str(
    "measure.py -- reconstruct and judge one module and print the tally. Nothing is written to the\n"
    "source tree.\n"
    "\n"
    "    python -m codebreaker measure <module> [--no-engine]\n"
    "\n"
    "The decompile, build and judge are steps of the engine (engine/lanes.py) by default: only what\n"
    "changed since the last measure is made again. --no-engine (or CB_ENGINE=0) runs the old direct path.\n"
    "\n"
    "This is the number to watch while changing the decompiler or the verifier.\n");

int64_t main(const py::list<str>& argv_in) {
    py::list<str> argv = argv_in;
    if (argv.contains(str("--engine"))) py::os::environ_::set(str("CB_ENGINE"), str("1"));   // (the engine is the default; kept for scripts)
    if (argv.contains(str("--no-engine"))) py::os::environ_::set(str("CB_ENGINE"), str("0"));   // (the old direct path)
    py::list<str> rest;
    for (const str& a : argv)
        if (a != str("--engine") && a != str("--no-engine")) rest.append(a);
    argv = rest;
    if (argv.empty()) {
        py::print(DOC);
        return 2;
    }
    const str given = argv[0];
    std::optional<str> path = py::os::path::isdir(given) ? std::optional<str>(given) : base::corpus::retail(given);   // a folder: a Java program
    if (!path || path->empty()) throw py::SystemExit(py::percent(str("no module %s in the corpus"), py::repr(given)));
    auto fs = store::functions::open();
    auto [sha, digest] = reconstruct::run(*path, fs);
    auto rows = fs->candidates(sha, digest);
    Tally why, bad;
    for (const auto& r : rows) {
        const str verdict = std::holds_alternative<str>(r.verdict) ? std::get<str>(r.verdict) : str("");
        if (verdict == V::DIVERGENT) why.add(show(r.reason));
        if (verdict == V::NO_COMPILE) {
            str reason = std::holds_alternative<str>(r.reason) ? std::get<str>(r.reason) : str("");   // (r["reason"] or "")
            bad.add(reason.slice(0, 60));
        }
    }
    if (!why.items.empty()) {
        py::list<str> parts;
        for (const auto& kv : why.most_common()) parts.append(py::percent(str("%s %d"), kv.first, kv.second));
        py::print(str("unverified by cause: ") + py::join(str(", "), parts));
    }
    if (!bad.items.empty()) {
        py::list<str> parts;
        for (const auto& kv : bad.most_common(4)) parts.append(py::percent(str("%s (%d)"), kv.first, kv.second));
        py::print(str("does not compile: ") + py::join(str("; "), parts));
    }
    return 0;
}

}  // namespace codebreaker::verify::measure
