// codebreaker/verify/measure.hpp -- typed C++ of codebreaker.verify.measure.
//
// measure.py -- reconstruct and judge one module and print the tally. Nothing is written to the
// source tree.
//
//     python -m codebreaker measure <module> [--no-engine]
//
// The decompile, build and judge are steps of the engine (engine/lanes.py) by default: only what
// changed since the last measure is made again. --no-engine (or CB_ENGINE=0) runs the old direct path.
//
// This is the number to watch while changing the decompiler or the verifier.
//
// Typing notes:
//   * main(argv) takes the arguments after the command name (a list of str) and returns the exit status; what it
//     prints goes to stdout (py::print); a module that is not in the corpus is py::SystemExit.
//   * The function store and the lanes are the typed ports' (store::functions::open, reconstruct::run); the lanes
//     must be registered before main() is called (engine/lanes_wiring.hpp).
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::verify::measure {

extern const py::str DOC;

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::verify::measure
