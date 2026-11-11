// codebreaker/exact/bss.hpp -- typed C++ of codebreaker.exact.bss
//
// bss.py -- the program's variables that begin without a value, each at its place (src/BSS).
//
//     python -m codebreaker exact PROJECT bss [--write]
//
// They lie from the end of the file's data (the load image ends there) to the stack's segment. In the
// sources they were plain definitions the compiler hands the linker as communal names, which the linker
// places in the order of its own table (a hash of the names) - ours are not the original names, so each
// is given its place outright (exact/link.py makes an object of them). What is there:
//
//   - the program's own: the data places SYMBOLS learnt from the identical functions, in the range; and
//     the sources' names for data SYMBOLS has no place for (a variable only functions not yet identical
//     use): the engine names data g_XXXX after its place, DS:XXXX
//   - the library's: its modules' own _BSS segments (a "hole" the linker fills with them, found by the
//     words of their code that point into it), and its communal names (stdio's buffers) by the words of
//     the library's code that name them
//
//     src/BSS:  range LO HI / hole ADDR LEN / name ADDR      (hex)
//
// Typing notes:
//   * propose() answers (edata, stack, names, holes): the struct Proposal; a hole is (place, length, [the modules' names]).
//   * `prj` is the real project::Project; the library stage is asked through its library adapter.
//   * main() prints to `out` (std::cout by default); the Python's print(txt) is the text and one more line end.
#pragma once

#include <cstdint>
#include <iostream>
#include <tuple>

#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::bss {

using Hole = std::tuple<int64_t, int64_t, py::list<py::str>>;

struct Proposal {
    int64_t edata = 0;                       // the range's start (DS offset)
    int64_t stack = 0;                       // and end
    py::dict<py::str, int64_t> names;        // {name: DS offset}
    py::list<Hole> holes;                    // a library module's own _BSS between them
};

Proposal propose(project::Project& prj);

py::list<py::str> lines(project::Project& prj, int64_t edata, int64_t stack, const py::dict<py::str, int64_t>& names, const py::list<Hole>& holes);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::bss
