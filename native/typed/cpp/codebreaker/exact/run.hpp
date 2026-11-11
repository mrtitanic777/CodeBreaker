// codebreaker/exact/run.hpp -- typed C++ of codebreaker.exact.run
//
// run.py -- exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).
//
//     python -m codebreaker exact PROJECT STAGE [options]
//     python -m codebreaker exact --toolchains             the toolchain profiles there are
//     python -m codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]
//                                                           one file compiled, its functions listed
//
// PROJECT is a folder with a project.json (codebreaker/exact/project.hpp). The stages are the Python's STAGES, each a
// typed port: ledger, library, carve, modules, link, bss, data, draft, repair, names, readable, seed, identify, bench.
//
// Typing notes:
//   * Python finds a stage by importlib.import_module(STAGES[stage]).main(prj, argv); here a stage is a function
//     `Stage(project, argv, out)` found in a table: the typed ports' own mains, each given the adapter of the project it
//     reads (project::as_ledger, as_library, as_data, as_identify, as_readable, as_draft; carve, link, bss, names, seed,
//     bench take the project itself).  register_stage replaces one (a program that has another stage to run).
//   * The stages that print through the Python's print() (ledger, data, readable, draft ...) print to std::cout; those with an
//     `out` of their own are given the one main() was given.  main() with std::cout (the default) is the Python's.
//   * The 16-bit decoder (project::set_decoder16, x86::set_decoder) and the toolchains' plugins (mspascal, plm86, xenix_c
//     register_plugin) are installed by the program, as the Python imports them.
//   * The Python's __doc__ (what -h prints) is DOC.
#pragma once

#include <cstdint>
#include <functional>
#include <iostream>

#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::run {

extern const py::str DOC;
// the stage names, in the order of the Python's STAGES
const py::list<py::str>& stage_names();

using Stage = std::function<int64_t(project::Project&, const py::list<py::str>&, std::ostream&)>;
void register_stage(const py::str& name, const Stage& fn);

int64_t main(const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::run
