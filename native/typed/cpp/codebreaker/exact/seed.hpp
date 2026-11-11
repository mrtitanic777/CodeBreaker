// codebreaker/exact/seed.hpp -- typed C++ of codebreaker.exact.seed
//
// seed.py -- a program's functions given the sources of the same functions in another version of it.
//
//     python -m codebreaker exact PROJECT seed --from OTHER [--write] [-v]
//
// The functions the names stage pairs with OTHER's (the same code but for its places, or found by its calls) are
// mostly the same source. For each pair whose function OTHER has in a source file, this program gets a file of
// its own, src/fADDR.c (ADDR this program's place): every top-level declaration of OTHER's file (its includes,
// macros, types, variables - a function of the file that returns other than an int declared), then the function's
// text. OTHER's address-names in it (fXXXXX, g_XXXX) are made this program's where the pairs say where those are.
// The ledger then says which are this program's bytes; a file that is not is a draft to start from.
//
// A file this project has already is kept. Without --write the files are only counted.
//
// Typing notes:
//   * split() answers (declarations, {function: its text}, {function: the type it returns}): the struct Split.
//   * The names stage's pairs, which the Python reads off `namesmod.carry.pairs`, are the fields of names::Carried.
//   * main() prints to `out` (std::cout by default).
#pragma once

#include <cstdint>
#include <iostream>

#include "codebreaker/exact/names.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::seed {

extern const py::str FUNC;      // the regular expression of a function's head line

struct Split {
    py::list<py::str> decls;                 // the top-level declarations
    py::dict<py::str, py::str> funcs;        // {function name: its text}
    py::dict<py::str, py::str> types;        // {function name: the type it returns} (those that return other than an int)
};

Split split(const py::str& text);

// {this program's address: the source file's text}
py::dict<int64_t, py::str> seeds(project::Project& prj, project::Project& other);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::seed
