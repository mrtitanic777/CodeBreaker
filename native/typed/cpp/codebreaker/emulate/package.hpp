// codebreaker/emulate/package.hpp -- typed C++ port of codebreaker/emulate/__init__.py.
//
// emulate -- run a piece of the original (or of a rebuild) and record what it did.
//
// One runner per kind of code, all in this package, each returning the one run record (emulate/run.py: Run) --
// how the run ended, the result registers, the memory it wrote, the calls and device accesses out of it in
// order, how the stack moved, the registers it had to keep, which of its instructions ran:
//
//     kind       runner (each platform's emulator.run)   the code it runs
//     pe         emulator.recorded (trace_function)       a function of a 32-bit Windows module
//     ne         windows16.recorded (probe)               a function of a 16-bit Windows module
//     le         driver.recorded (run)                    an entry point of a virtual device driver (flat ring 0)
//     powerpc    powerpc.recorded (run)                   a function of a PowerPC image
//     nes ...    m6502 / z80 / sm83 / m68k .recorded      a function of a console or home-computer program
//
//     auto [module, function] = emulate::runner("pe");     // -> ("codebreaker.emulate.emulator", "recorded")
//
// Typing notes:
//   * The Python's runner(kind) imports the module and returns its function. Each runner has its own typed
//     signature in C++ (emulate/emulator.hpp, windows16.hpp, powerpc.hpp, m6502.hpp ...), so no one type holds them
//     all: runner() answers the (module, function) pair the platform registry names (platforms::runners()), and the
//     caller calls the typed function of that module.
//   * RUNNERS is `_runners()` taken when first used (the Python takes it at import).
//   * runner(kind) of a kind that is not a platform's name raises KeyError, as RUNNERS[kind] does.
#pragma once

#include <tuple>

#include "py.hpp"

namespace codebreaker::emulate {

// {platform name: (module, function)} of every platform whose code CodeBreaker can run
py::dict<py::str, std::tuple<py::str, py::str>> _runners();

// each platform's runner (codebreaker/platforms)
const py::dict<py::str, std::tuple<py::str, py::str>>& RUNNERS();

// The function that runs code of this kind (the kinds are verify/reconstruct.py's): its (module, name).
std::tuple<py::str, py::str> runner(const py::str& kind);

}  // namespace codebreaker::emulate
