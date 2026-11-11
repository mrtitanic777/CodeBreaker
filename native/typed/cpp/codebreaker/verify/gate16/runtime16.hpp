// codebreaker/verify/gate16/runtime16.hpp -- typed C++ port of codebreaker.verify.gate16.runtime16.
//
// runtime16.py -- which of a 16-bit module's functions are the compiler's run-time library.
//
//     runtime16::identify(originals_by_name)   -> {function: "MDLLCEW.LIB:__aFlmul", ...}
//
// A Windows module built with Microsoft C 8.00c (Visual C++ 1.52c) was linked with that compiler's
// own libraries -- for a medium-model DLL, MDLLCEW / MDLLCAW / MNOCRTW (and LIBW, the import
// library). A function of the module that is one of the library's, byte for byte outside the places
// the linker filled in, is not the module's code: it needs no reconstruction (the library is linked),
// and matching it is no measure of the C. Each routine of each library is set beside the start of
// each function (symbols/libraries reads the libraries); the longest routine that fits names it. A
// far call in a routine is compared as a far call (only its address left out): a short routine
// around one would otherwise fit every small function that makes a call.
//
// settings: msvc152 (the compiler's folder; its LIB folder is read).
//
// Over symbols/libraries/package: functions(path)[1] and fits.
// The setting msvc152 is read with py::settings::get_str (CB_MSVC152, else settings.json, else the defaults).
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/symbols/libraries/package.hpp"

namespace codebreaker::verify::gate16::runtime16 {

using Function = symbols::libraries::Function;

extern const py::list<py::str> LIBRARIES;

// the library routines, once read (None before; tests set it, as the Python tests monkeypatch _routines)
extern std::optional<py::list<Function>> _routines;

// libraries.fits
using symbols::libraries::fits;

// [Function] of the libraries' public routines (symbols/libraries), each named library:name,
// longest first
const py::list<Function>& routines(const py::list<py::str>& names = LIBRARIES);

// {function: library routine} for those of `originals` {name: (bytes, mask)} that are one
py::dict<py::str, py::str> identify(const py::dict<py::str, std::tuple<py::bytes, py::bytes>>& originals);

}  // namespace codebreaker::verify::gate16::runtime16
