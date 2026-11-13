// codebreaker/exact/carve.hpp -- COMPAT SHIM over cb/exact/carve.hpp (docs/CPP_DESIGN.md 6.3), for the old layer's callers (exact/project's old adapter, bench,
// link, names, seed). Deleted with its last py:: consumer. Conversion and forwarding only: the carve is native/cb/exact/carve.hpp, over the cb project the old
// Project holds (`impl_`).
//
// carve.py -- where the program's own functions begin and end.
//
//     python -m codebreaker exact PROJECT carve [--write] [-v]
//
// The program's own code is what lies before the library part (exact/library.py), less the kit's objects linked first. In it a function begins where the
// compiler's prologue stands, where a frame is set up right after a return or a pad, and where a near call of the code goes; it ends where the next begins (or
// the program's own code ends). A start the project knows better can be added or taken away: project "starts_add" / "starts_drop" (image offsets).
//
//     carve::functions(project) -> [(start, length, why)]
// src/FUNCS (--write): one start a line, the length after it.
//
// What differs from the port it replaces: the library stage's answer (`mp`) is not taken -- the carve asks for its own; `_hexrx` gives a pattern in cb::Regex's
// syntax (\xHH for a byte), which py::re reads the same; main() prints to stdout (its `out` is not used).
#pragma once

#include <cstdint>
#include <iostream>
#include <tuple>

#include "cb/exact/carve.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::carve {

namespace nc = ::cb::exact::carve;

// (lo, hi, [(a, b) ranges inside that are not the program's own: the kit's objects])
inline std::tuple<int64_t, int64_t, py::list<std::tuple<int64_t, int64_t>>> region(project::Project& prj, const library::MapP& = nullptr) {
    const nc::Region r = nc::region(*prj.impl_);
    py::list<std::tuple<int64_t, int64_t>> skip;
    for (const auto& [a, b] : r.skip) skip.append(std::make_tuple(a, b));
    return {r.lo, r.hi, skip};
}

// [(start, length, why)]
inline py::list<std::tuple<int64_t, int64_t, py::str>> functions(project::Project& prj, bool = false) {
    py::list<std::tuple<int64_t, int64_t, py::str>> out;
    for (const nc::Function& f : nc::functions(*prj.impl_)) out.append(std::make_tuple(int64_t(f.address), int64_t(f.length), py::str(f.why)));
    return out;
}

// a pattern of hex bytes as a regex over latin-1 text
inline py::str _hexrx(const py::str& rx) { return py::str(nc::hex_pattern(rx.utf8())); }

inline int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& = std::cout) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    return nc::run(*prj.impl_, args);
}

}  // namespace codebreaker::exact::carve
