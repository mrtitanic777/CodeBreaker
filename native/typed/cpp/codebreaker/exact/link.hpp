// codebreaker/exact/link.hpp -- typed C++ of codebreaker.exact.link
//
// link.py -- the program linked from its sources by the period linker, and compared with the original file.
//
//     python -m codebreaker exact PROJECT link [-v] [--keep DIR] [--map]
//
// What goes in, in this order:
//     src/FIRSTOBJ   the kit's objects linked before the program's own (one a line)
//     src/MODULES    the program's modules (exact/modules.py): compiled whole, a function not yet the
//                    program's made a stand-in of its bytes (exact/standin.py); NAME.OBJ: the kit's
//     src/BSS        the variables without a value at their places: "range LO HI", "hole ADDR LEN" (a
//                    library module's own _BSS between), "name ADDR"; an object before the library
//                    part, one after each hole
//     src/CRTLIB     the library's modules in the program's order (exact/library.py): those the rest
//                    needs are linked as objects; RAW lines are the program's bytes
//     src/LINKOPT    the linker's switches;  src/EXEMOD  the kit's EXEMOD run after, one line each
//
// The linker and the files it is run with are the toolchain profile's "link" part:
//     {"kit": kit, "run": "disk3/LINK.EXE", "dir": kit folder of libraries and objects,
//      "libs": {model: [library files]}, "exemod": "disk3/EXEMOD.EXE", "psp": paragraph}
//
// The result is compared with the original: header, relocation table, load image; where they part,
// the first difference is put to the stage that makes that part (a function's code, the data, the
// library's part, the variables without a value, the header).
//
// Typing notes:
//   * `prj` is the real project::Project; the stages it asks (modules, library, standin, recut, linkwhole) are given the
//     adapters of the project (project::as_modules, as_library, as_standin, as_recut, as_linkwhole).  standin::rebuild
//     takes the 16-bit decoder, which is project::decoder16() (install it with project::set_decoder16).
//   * The Python's module attributes `bss_objects.after` and `crt.conflicts` (answers left on the functions) are fields
//     of the structs BssObjects and CrtResult.
//   * `LINK`'s profile part is the struct project::LinkSpec (project.hpp); SystemExit(msg) is py::SystemExit.
//   * Everything the stages print goes to `out` (std::cout by default); `quiet` calls print nothing.
//   * diagnose() takes the original file or none (the packed case: ours is held to the unpacked image).
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::link {

using Objects = py::list<std::tuple<py::str, py::bytes>>;     // [(name, object module bytes)]

// the toolchain and its linker part (SystemExit when the profile has none)
std::tuple<std::shared_ptr<toolchain::Toolchain>, project::LinkSpec> _lp(project::Project& prj);

// a file of the kit the linker part names
py::str _kitfile(const std::shared_ptr<toolchain::Toolchain>& tc, const project::LinkSpec& lk, const py::str& path);

// ([(name, bytes)] of the program's own modules, [stand-ins])
std::tuple<Objects, py::list<py::str>> objects(project::Project& prj, bool verbose = false, std::ostream& out = std::cout);

// where the program's own code ends: the first code place src/CRTLIB names, else the end carve finds
int64_t _library_start(project::Project& prj);

Objects first_objects(project::Project& prj);

struct BssObjects {
    Objects objects;
    bool after = false;                     // the communal names are in a segment of their own, after the library
};
BssObjects bss_objects(project::Project& prj);

struct CrtResult {
    Objects modules;                        // the library's modules the rest needs, in the program's order
    py::list<py::str> missing;              // names left unresolved
    py::list<py::str> conflicts;            // where the program has one routine and ours names two
};
// the library's modules the rest needs, as objects in the program's order (src/CRTLIB); names left unresolved
CrtResult crt(project::Project& prj, const Objects& objs);

py::str dosname(const py::str& n);

// -> (the linked file or None, LINK's words, its map)
std::tuple<std::optional<py::bytes>, py::str, py::str> run_link(project::Project& prj, const Objects& objs, const std::optional<py::str>& keep = std::nullopt,
                                                                 const py::str& opts = py::str(""));

// (the 14 header words, the load image, the relocations [(offset, segment)])
std::tuple<py::list<int64_t>, py::bytes, py::list<std::tuple<int64_t, int64_t>>> parts(const py::bytes& exe);

extern const py::list<py::str> FIELDS;

// where ours parts from the original, and which stage makes that part (ref: none for a packed original)
py::list<py::str> diagnose(project::Project& prj, const std::optional<py::bytes>& ref, const py::bytes& exe);

// where b runs ahead of or behind a: [lines] 'DS:XXXX on, ours is +N' at each change
py::list<py::str> drift(const py::bytes& a, const py::bytes& b, int64_t step = 2, int64_t span = 12, int64_t reach = 160);

// the linker's switches (src/LINKOPT)
py::str opts_of(project::Project& prj);

struct Built {
    std::optional<py::bytes> exe;           // the linked file or None
    py::str console;                        // LINK's words
    py::str map;
    py::list<py::str> stand;                // the stand-ins
};
Built build(project::Project& prj, const std::optional<py::str>& keep = std::nullopt, bool verbose = false, bool quiet = false,
            const std::optional<py::str>& opts = std::nullopt, std::ostream& out = std::cout);

// the data stand-ins' lengths fitted by the link (project.json "data_sizes", "data_pads", kept) -> the lines of what was done
py::list<py::str> fit(project::Project& prj, int64_t rounds = 8);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::link
