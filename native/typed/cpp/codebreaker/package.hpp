// codebreaker/package.hpp -- typed C++ from codebreaker/__init__.py (the package `codebreaker`).
//
// CodeBreaker -- reconstruct readable, compilable source from binaries, and prove it behaves
// like the original.
//
//     base/        where things are; settings; spreading work over cores; progress
//     formats/     reading executables (PE, NE, LE, CAB)
//     disasm/      instructions and function boundaries
//     emulate/     running original and rebuilt code in an emulator and recording what it does
//     decompile/   getting C out of a function (our decompiler)
//     recover/     what a function's behaviour says about it: arguments, structures, globals
//     symbols/     names and prototypes from headers, exports and symbol files
//     verify/      building a reconstruction and judging it against the original
//     source/      writing the reconstructed module and the scoreboard
//     targets/     what is specific to one thing being reconstructed (win95)
//     ui/          the progress window
//
// The package's own code is its import: `settings.export_for_carried_over_code()`.  Here it is init(), which the
// program's start (or a caller that wants the environment filled in) calls (Python's import runs it once; a second call
// changes nothing more, as every variable it sets is then set).
//
// Typing notes:
//   * settings.export_for_carried_over_code() is not in the C++ runtime (py_settings.hpp has get(); settings.state() is
//     written locally in several ports): it is export_for_carried_over_code() below, with the Python's behaviour: each of
//     the three variables not already set (or set empty) takes its setting's value when that is not empty, and
//     CB_RESULTS_DB is set to <state>/results.sqlite when it is not set.
#pragma once

namespace codebreaker::package {

// The modules carried over from the previous layout read their locations from environment
// variables. Fill in the ones not already set, from the settings. Goes away as each is changed
// to ask `settings` directly.
void export_for_carried_over_code();

// What importing the package does: export_for_carried_over_code().
void init();

}  // namespace codebreaker::package
