// codebreaker/exact/carve.hpp -- typed C++ of codebreaker.exact.carve
//
// carve.py -- where the program's own functions begin and end.
//
//     python -m codebreaker exact PROJECT carve [--write] [-v]
//
// The program's own code is what lies before the library part (exact/library.py), less the kit's
// objects linked first. In it a function begins
//
//   - where the compiler's prologue stands (the toolchain profile's "carve": "prologues", byte
//     patterns as hex regular expressions; one whose group 1 is a near call's displacement counts only
//     when the call goes to the routine "prologue_call" names - C 3.00's __chkstk)
//   - where a frame is set up (push bp / mov bp,sp) right after a return or a pad: a function compiled
//     without the stack probe (-Gs), or by hand
//   - where a near call of the code goes (the code disassembled from each start to the next)
//
// and ends where the next begins (or the program's own code ends). A start the project knows better
// can be added or taken away: project "starts_add" / "starts_drop" (image offsets).
//
//     carve::functions(project) -> [(start, length, why)]
// src/FUNCS (--write): one start a line, the length after it.
//
// Typing notes:
//   * `prj` is the real project::Project (exact/project.hpp); the library stage is asked through its library adapter.
//   * The prologue patterns are bytes regular expressions in the Python; here they are run as str regular expressions over
//     the image read as latin-1 (a byte is a character, '.' any of them with re.S), which finds the same places.
//   * x86::body needs the 16-bit decoder installed with x86::set_decoder.
//   * main() prints to `out` (std::cout by default).
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::carve {

// (lo, hi, [(a, b) ranges inside that are not the program's own: the kit's objects]); mp: the library stage's answer (None: ask it)
std::tuple<int64_t, int64_t, py::list<std::tuple<int64_t, int64_t>>> region(project::Project& prj, const library::MapP& mp = nullptr);

// [(start, length, why)]
py::list<std::tuple<int64_t, int64_t, py::str>> functions(project::Project& prj, bool verbose = false);

// a pattern of hex bytes ("55 8b ec (?:33c0|b8....) e8 (....)") as a regex over latin-1 text: two hex digits a byte, '..' any
// byte, the regex's own ( ) | ? * + left as they are
py::str _hexrx(const py::str& rx);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::carve
