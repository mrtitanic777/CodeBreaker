// codebreaker/analyze/origin.hpp -- SHIM over cb/analyze/origin.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a verdict and a result as py::
// records, module16 over a path, the Counter-shaped summary, `main`) for the ports that still use it (build/module). Conversion and
// forwarding only; deleted with its last py:: consumer.
//
// module16 opens the module, makes the scratch folder the Python made (build::watcom::_scratch) and describes it with the old layer's knowledge
// and the policy the environment says. What the old module had besides and nobody outside used (judge over the old decoder, _linker16, _judge's
// helpers) is not here.
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "cb/analyze/origin.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::origin {

namespace no = ::cb::analyze::origin;

// collections.Counter as an ordered {key: count}
using Counter = py::dict<py::str, int64_t>;
// Counter.most_common(n): the n commonest keys with their counts (all when n is absent), ties in insertion order
inline py::list<std::tuple<py::str, int64_t>> most_common(const Counter& c, const std::optional<int64_t>& n = std::nullopt) {
    std::vector<std::tuple<py::str, int64_t>> items = c.items();
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
    py::list<std::tuple<py::str, int64_t>> out;
    for (size_t i = 0; i < items.size() && (!n || static_cast<int64_t>(i) < *n); i++) out.append(items[i]);
    return out;
}

// The verdict on one function.
struct Origin {
    int64_t va = 0;
    py::str name;
    py::str language = py::str("c");     // "c" | "assembly"
    py::list<py::str> evidence;          // why assembly (one piece makes it so)
    py::list<py::str> compiler;          // the C compiler's fingerprints

    Origin() = default;
    Origin(int64_t va_, const py::str& name_) : va(va_), name(name_) {}
    py::str repr() const {
        no::Origin n;
        n.name = name.utf8();
        n.language = language == py::str("assembly") ? no::Language::Assembly : no::Language::C;
        for (const py::str& e : evidence.vec()) n.evidence.push_back(e.utf8());
        for (const py::str& e : compiler.vec()) n.compiler.push_back(e.utf8());
        return ::cb::compat::to_py(n.describe());
    }
};

struct Result {
    py::dict<int64_t, Origin> functions;
    std::tuple<Counter, Counter> summary() const {   // (the languages counted, the first evidence's kind counted)
        Counter n, why;
        for (const Origin& o : functions.values()) {
            n.set(o.language, n.get(o.language, int64_t(0)) + 1);
            if (!o.evidence.empty()) {
                const std::string e = o.evidence[0].utf8();
                const py::str k = py::str(e.substr(0, e.find(" (")));
                why.set(k, why.get(k, int64_t(0)) + 1);
            }
        }
        return {n, why};
    }
};

// Every function of a 16-bit module, judged. `more`: {va: name} of functions found besides the ones the module's own tables give.
inline Result module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more = std::nullopt) {
    return ::cb::compat::guard([&] {
        const py::str work = build::watcom::_scratch(py::list<py::str>{py::str("origin"), py::os::path::basename(path).lower()});
        decompile::describe::shim_detail::OldKnowledge knowledge;
        std::map<int64_t, std::string> extra;
        if (more) {
            for (const auto& kv : more->items()) extra[std::get<0>(kv)] = std::get<1>(kv).utf8();
        }
        const no::Result r = no::module16(::cb::formats::ne::Module::open_file(std::filesystem::path(path.utf8())), std::filesystem::path(work.utf8()), knowledge,
                                          more ? &extra : nullptr, ::cb::decompile::describe::Policy::from_environment());
        Result out;
        for (const auto& [va, o] : r.functions) {
            Origin old(va, ::cb::compat::to_py(o.name));
            old.language = py::str(std::string(no::to_text(o.language)));
            for (const std::string& e : o.evidence) old.evidence.append(py::str(e));
            for (const std::string& e : o.compiler) old.compiler.append(py::str(e));
            out.functions.set(va, old);
        }
        return out;
    });
}
inline Result _module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more = std::nullopt) { return module16(path, more); }

// python -m codebreaker origin <module>   which functions were written in assembly
inline int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        std::cout << "python -m codebreaker origin <module>   which functions were written in assembly\n";
        return 2;
    }
    const py::str path = py::os::path::isfile(argv[0]) ? argv[0] : *base::corpus::retail(argv[0]);
    const Result r = module16(path);
    no::Result n;
    for (const auto& kv : r.functions.items()) {
        no::Origin o;
        o.va = std::get<0>(kv);
        o.name = std::get<1>(kv).name.utf8();
        o.language = std::get<1>(kv).language == py::str("assembly") ? no::Language::Assembly : no::Language::C;
        for (const py::str& e : std::get<1>(kv).evidence.vec()) o.evidence.push_back(e.utf8());
        for (const py::str& e : std::get<1>(kv).compiler.vec()) o.compiler.push_back(e.utf8());
        n.functions.emplace(o.va, std::move(o));
    }
    bool list = false;
    for (const py::str& a : argv.vec()) list = list || a == py::str("--list");
    std::cout << no::report(py::os::path::basename(path).utf8(), n, list);
    return 0;
}

}  // namespace codebreaker::analyze::origin
