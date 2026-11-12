// codebreaker/translate/backends/cpp17.hpp -- typed C++ port of codebreaker.translate.backends.cpp17.
//
// cpp17.py -- the translator's C++17 back end, for its Python callers: a thin shim over the native one.
//
// The back end is C++ (native/l2/src/cpp17_*.cc; docs/NATIVE.md step 5, docs/TRANSLATOR_DESIGN.md):
// ONE C++17 emitter, with the source language's rules chosen by the unit's language (Unit.lang) --
// 'python' on native/ported/include/py.hpp, 'java' (or none) on codebreaker/translate/runtime/cpp17/jrt.hpp;
// 'csharp', 'c' and 'cpp' have a place and decline until their rules are written. This module only hands
// the L2 over (l2.to_json) and the text back, until its callers move to C++ themselves:
//
//     write_unit(unit)       a Python module (the Python front end, docs/PYTHON_FRONTEND.md) -> (header,
//                            source, {function: (first, last) source line})
//     program(units, main)   a whole Java program from L2 -> its C++ text (the modern forms C++17 has no word
//                            for are lowered first: translate/desugar.py)
//     (the transpiler's C++17 bodies are cb/translate/backends/cpp03 with Dialect::Cpp17: this module no longer forwards them)
//
// Both raise Decline for what the back end cannot write.
//
// Typing notes:
//   * A unit is an ir::NodeP (the IR tree; to_json is l2::to_json). `main` is the main class's name or None.
//   * RUNTIME and API_MAPS are found from this source file's place in the tree
//     (<repo>/native/typed/cpp/codebreaker/translate/backends/cpp17.cpp -> <repo>/codebreaker/...).
//   * _native() (the check that the library loads) has nothing to check, the library being linked in.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::cpp17 {

namespace ir = codebreaker::translate::ir;

extern const py::str RUNTIME;
extern const py::list<py::str> FLAGS;
extern const py::str API_MAPS;

// The C++17 back end does not write this unit (the reason says why).
using Decline = translate::native_l2::Cpp17Decline;   // (the library raises it for the C++17 back end)

py::str header_path(const py::str& module);
py::str source_path(const py::str& module);

// An ir.Unit of the Python front end -> (header text, source text, {function: (first, last) source line})
// -- the lines say where each function's body is in the source, so a compiler's error can be pinned on it.
std::tuple<py::str, py::str, py::dict<py::str, std::tuple<int64_t, int64_t>>> write_unit(
    const ir::NodeP& unit, const std::optional<py::str>& banner = std::nullopt);

// The C++17 text of a whole Java program (its L2 units), or raise Decline.
py::str program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main = std::nullopt);


}  // namespace codebreaker::translate::backends::cpp17
