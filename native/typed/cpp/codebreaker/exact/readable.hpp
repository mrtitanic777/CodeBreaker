// codebreaker/exact/readable.hpp -- C++ port of codebreaker.exact.readable.
//
// readable.py -- the sources with the names SYMBOLS has put in for the engine's address-names, proven to change nothing.
//
//     python -m codebreaker exact PROJECT readable [--write]
//
// The sources the engine writes name a function fXXXXX and a variable g_XXXX after where they are. Where src/SYMBOLS
// has a name for that place (the names stage carries them from another version; a person may write one), each
// source file gets it in place of the address-name: a name of C code `_name` is `name` in C. The files are compiled
// again, every file into the ledger, and the change is kept only if no function that was identical stops being so
// (the names of functions and variables are the linker's; a local's name, which can move it in the frame, is left).
// Without --write the renaming is only tried and reported.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::exact::readable {

// An address-name's place: ("f", address of a function) or ("g", offset of a variable).
using Place = std::tuple<py::str, int64_t>;
// {address-name: name}
using Names = py::dict<Place, py::str>;
// project.symbols(): {name: (type "C" | "D" | "S" | "?", address)}
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;
// {(file, function): verdict}
using Counts = py::dict<std::tuple<py::str, py::str>, py::str>;

// One row of the ledger (ledger.run): file, function, address (None: no place), length, verdict, notes, toolchains.
struct Row {
    py::str file;
    py::str name;
    std::optional<int64_t> address;
    int64_t length = 0;
    py::str verdict;
    py::list<py::str> notes;
    py::list<py::str> by;
};

// The part of exact.project.Project and exact.ledger that main() uses. The Python module takes the Project
// object itself; here it is this interface, which the real Project (and a test double) implements.
class Project {
public:
    virtual ~Project() = default;
    virtual py::str dir() const = 0;                                      // prj.dir
    virtual py::str src() const = 0;                                      // prj.src
    virtual Symbols symbols() const = 0;                                  // prj.symbols()
    virtual py::list<std::tuple<int64_t, py::str>> sources() const = 0;   // prj.sources(): [(address, path)]
    virtual py::list<Row> ledger_run() const = 0;                         // ledger.run(prj)
    // projmod.Project(prj.dir, override={"src": src})
    virtual std::unique_ptr<Project> with_src(const py::str& src) const = 0;
};

// The address-name: a function fXXXXX (group 1, maybe _fXXXXX) or a variable g_XXXX (group 2).
const py::re::Pattern& ADDRNAME();

// {address-name: name} - from SYMBOLS, for the places it has a real name for
Names mapping(const Symbols& symbols);
Names mapping(const Project& prj);

// the text with each address-name that has a name renamed - unless the name is in the text already, as
// something else
py::str rename(const py::str& text, const Names& names);

// {(file, function): verdict} of the ledger's rows
Counts counts(const py::list<Row>& rows);
Counts counts(const Project& prj);

// The command: 0 kept (or nothing to do), 1 not kept / no names. Prints its report to stdout.
int64_t main(const Project& prj, const py::list<py::str>& argv);

}  // namespace codebreaker::exact::readable
