// codebreaker/exact/names.hpp -- COMPAT SHIM over cb/exact/names.hpp (docs/CPP_DESIGN.md 6.3), for the old layer's callers (exact/seed). Deleted with its last py::
// consumer. Conversion and forwarding only: the naming is native/cb/exact/names.hpp, over the cb project the old Project holds (`impl_`).
//
// names.py -- a program's functions and data named after another version of it that has names.
//
//     python -m codebreaker exact PROJECT names --from OTHER [--write] [-v]
//
// Each function's instructions are read with the places masked (call and jump targets, the addresses of data, the words the linker fills): a function whose
// masked code is the same as one function of OTHER's takes its name; from a pair, the calls each makes at the same instruction name each other's callees; a name
// is not given twice. OTHER's names: its src/SYMBOLS and its "place_by" and "names" files.
//
//     names::carry(project, other) -> Carried {names, dnames, pairs, dpairs}
//
// What differs from the port it replaces: main() prints to stdout (its `out` is not used); the data pairs and pairs are dicts in the order they were found.
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "cb/exact/names.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::names {

namespace nn = ::cb::exact::names;

// ({code address: name}, {DS offset: name}) the project has; starts: where functions begin (none: carve's, else the places named)
inline std::tuple<py::dict<int64_t, py::str>, py::dict<int64_t, py::str>> known(project::Project& prj, const std::optional<py::set<int64_t>>& starts = std::nullopt) {
    std::optional<std::set<int64_t>> given;
    if (starts) {
        given.emplace();
        for (int64_t a : *starts) given->insert(a);
    }
    const nn::Known k = nn::known(*prj.impl_, given);
    py::dict<int64_t, py::str> code, data;
    for (const auto& [a, n] : k.code) code.set(a, py::str(n));
    for (const auto& [a, n] : k.data) data.set(a, py::str(n));
    return {code, data};
}

// [(address, length)]
inline py::list<std::tuple<int64_t, int64_t>> functions(project::Project& prj) {
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& [a, n] : nn::functions(*prj.impl_)) out.append(std::make_tuple(a, n));
    return out;
}

// (masked instruction, call target or None, data place or None)
using Row = std::tuple<py::str, std::optional<int64_t>, std::optional<int64_t>>;
inline py::list<Row> read(project::Project& prj, int64_t a, int64_t ln) {
    py::list<Row> out;
    for (const nn::Insn& i : nn::read(*prj.impl_, a, ln)) out.append(Row{py::str(i.masked), i.call, i.data});
    return out;
}

struct Carried {
    py::dict<int64_t, std::tuple<py::str, py::str>> names;      // {address: (name, how)}
    py::dict<int64_t, std::tuple<py::str, py::str>> dnames;     // {DS offset: (name, how)}
    py::dict<int64_t, int64_t> pairs;                           // this program's place -> the other's (code)
    py::dict<int64_t, int64_t> dpairs;                          // (data)
};

inline Carried carry(project::Project& prj, project::Project& other) {
    const nn::Carried c = nn::carry(*prj.impl_, *other.impl_);
    Carried out;
    for (const auto& [a, n] : c.code) out.names.set(a, std::make_tuple(py::str(n.name), py::str(n.how)));
    for (const auto& [a, n] : c.data) out.dnames.set(a, std::make_tuple(py::str(n.name), py::str(n.how)));
    for (const auto& [a, b] : c.pairs) out.pairs.set(a, b);
    for (const auto& [a, b] : c.data_pairs) out.dpairs.set(a, b);
    return out;
}

inline int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& = std::cout) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    return nn::run(*prj.impl_, args);
}

}  // namespace codebreaker::exact::names
