// codebreaker/exact/seed.hpp -- COMPAT SHIM over cb/exact/seed.hpp (docs/CPP_DESIGN.md 6.3), for the old layer's caller (exact/run). Deleted with its last py::
// consumer. Conversion and forwarding only: the seeding is native/cb/exact/seed.hpp, over the cb project the old Project holds (`impl_`).
//
// seed.py -- a program's functions given the sources of the same functions in another version of it.
//
//     python -m codebreaker exact PROJECT seed --from OTHER [--write] [-v]
//
// What differs from the port it replaces: main() prints to stdout (its `out` is not used); the "seeded from" comment writes the path with '/'.
#pragma once

#include <cstdint>
#include <iostream>

#include "cb/exact/seed.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::seed {

namespace ss = ::cb::exact::seed;

// {this program's address: the source file's text}
inline py::dict<int64_t, py::str> seeds(project::Project& prj, project::Project& other) {
    py::dict<int64_t, py::str> out;
    for (const auto& [a, text] : ss::seeds(*prj.impl_, *other.impl_)) out.set(a, py::str(text));
    return out;
}

inline int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& = std::cout) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    return ss::run(*prj.impl_, args);
}

}  // namespace codebreaker::exact::seed
