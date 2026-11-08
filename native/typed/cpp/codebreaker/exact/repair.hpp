// codebreaker/exact/repair.hpp -- typed C++ port of codebreaker.exact.repair.
//
// repair.py -- a function that differs from the program by what the comparison names, mended in its source and
// kept only when the compiler proves it.
//
//     python -m codebreaker exact PROJECT repair [--write] [-v]
//
// The ledger's comparison says where a function parts from the program; some of what it says is a fix it can
// try. For each such function the source file is changed, compiled again, and held to the program: a change is
// kept only if the function comes out identical and no function of the file that was identical stops being so.
// Without --write the changes are only reported.
//
//   values each +N   bytes that differ alone, each by the same amount (an immediate): the source names a constant
//                    or an enumeration's member with another value. Tried, in order: the names that end in a
//                    number equal to such a value (an enumeration named by its members' ordinals: DK46) with the
//                    number moved by N - those alone, then every name of that family; a literal of that value
//                    made the program's.
//
// A Pascal program's change is written to the project's own overlay (its first source folder), so the lower
// layers stay what they are for the other programs built from them.
//
// Typing notes:
//   * `prj` is ledger::Project (the members of exact.project.Project that ledger.py reads). The Pascal path reads
//     the project through pascal_project() (pascal_join's view of it); the Python's `_only(prj, name)` -- a copy
//     of the project whose compilands are the one named -- is that view with its compilands cut to the one.
//   * a job (the Python's dict(fn=, at=, path=, dest=, mine=, d=, unit=, check=)) is the struct Job; its check()
//     gives the ledger's rows (the first six fields of each), of which main() reads the function and the verdict.
//   * main() writes what the Python prints to `out`. open(..., encoding="latin1", newline="") is py::file::open
//     with that encoding and newline.
//   * `verbose` is accepted and not used, as in the Python.
#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/exact/ledger.hpp"
#include "codebreaker/exact/pascal_join.hpp"
#include "py.hpp"

namespace codebreaker::exact::repair {

using Row = ledger::Row;

// what a note names: the shift of the values and the places in the program where they differ -> (shift, offsets), or None
std::optional<std::tuple<int64_t, py::list<int64_t>>> _shifts(const py::list<py::str>& notes);

// the source changed so that the values `mine` become mine + d -> [(what, text)]
py::list<std::tuple<py::str, py::str>> candidates(const py::str& text, const py::list<int64_t>& mine, int64_t d);

struct Job {
    std::optional<py::str> fn;
    std::optional<int64_t> at;
    py::str path;
    py::str dest;
    py::list<int64_t> mine;
    int64_t d = 0;
    std::optional<py::str> unit;
    std::function<py::list<Row>()> check;
};

// the project with only the compiland `name` (for pascal_join.run)
pascal_join::Project _only(const pascal_join::Project& prj, const py::str& name);

// the file whose first line declares P<h> (a piece, or one nested in a piece's .d folder), the top layer's
std::optional<py::str> _piece_of(const pascal_join::Compiland& c, const py::str& h);

py::list<Job> _pascal(const ledger::Project& prj, bool verbose);

// the ledger's files (one function a file, or whole files): each differing function the note can mend
py::list<Job> _sourced(const ledger::Project& prj, bool verbose);

int64_t main(const ledger::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::repair
