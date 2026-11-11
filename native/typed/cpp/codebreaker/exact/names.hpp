// codebreaker/exact/names.hpp -- typed C++ of codebreaker.exact.names
//
// names.py -- a program's functions and data named after another version of it that has names.
//
//     python -m codebreaker exact PROJECT names --from OTHER [--write] [-v]
//
// Most of a program's functions are the same source in its neighbouring versions; their code is the same but
// for where things are. Each function's instructions are read with the places masked - call and jump targets,
// the addresses of data, the words the linker fills - so that code that moved is still the same code:
//
//   1. a function whose masked code is the same as one function of OTHER's (and of no other function of
//      either) is that function: it takes its name
//   2. from a pair, the calls each makes at the same instruction name each other's callees (a function whose
//      own code changed is still found, by who calls it); the same for the data words at the same places
//   3. a name OTHER has that this program already gives another function is not given twice
//
// OTHER's names: its src/SYMBOLS (C code, D data) and its "place_by" file (name fXXXXX / g_XXXX). --write puts the
// names into this project's src/SYMBOLS (a name it has already is kept); without it they are listed.
//
//     names::carry(project, other) -> Carried {names, dnames, pairs, dpairs}
//
// Typing notes:
//   * Python's carry() answers ({address: (name, how)}, {DS offset: (name, how)}) and leaves `carry.pairs`, `carry.dpairs`
//     (this program's place -> the other's) as attributes of the function for exact/seed.py; here they are the fields of
//     the Carried it returns.
//   * A function's instructions read with places masked are `Row`s (masked text, call target, data place).
//   * main() prints to `out` (std::cout by default).
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/exact/project.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::names {

extern const py::str ADDR;        // r"0x[0-9a-f]{3,}"
extern const py::str CODE_NAME;   // r"(?:f|P)([0-9a-fA-F]{4,5})"
extern const py::str DATA_NAME;   // r"(?:g_|G)([0-9a-fA-F]{1,4})"

// ({code address: name}, {DS offset: name}) the project has: its SYMBOLS (a place of no kind is code where a function begins),
// its "place_by" file and its "names" file - lines NAME fXXXXX|g_XXXX, or PXXXXX|GXXXX NAME.  starts: where functions begin
// (none: carve's, else the places named)
std::tuple<py::dict<int64_t, py::str>, py::dict<int64_t, py::str>> known(project::Project& prj,
                                                                         const std::optional<py::set<int64_t>>& starts = std::nullopt);

// [(address, length)]
py::list<std::tuple<int64_t, int64_t>> functions(project::Project& prj);

// (masked instruction, call target or None, data place or None)
using Row = std::tuple<py::str, std::optional<int64_t>, std::optional<int64_t>>;
py::list<Row> read(project::Project& prj, int64_t a, int64_t ln);

struct Carried {
    py::dict<int64_t, std::tuple<py::str, py::str>> names;      // {address: (name, how)}
    py::dict<int64_t, std::tuple<py::str, py::str>> dnames;     // {DS offset: (name, how)}
    py::dict<int64_t, int64_t> pairs;                           // this program's place -> the other's (code)
    py::dict<int64_t, int64_t> dpairs;                          // (data)
};
Carried carry(project::Project& prj, project::Project& other);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::names
