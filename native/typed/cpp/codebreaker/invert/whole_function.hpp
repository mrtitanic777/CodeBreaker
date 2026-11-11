// codebreaker/invert/whole_function.hpp -- C++17 port of codebreaker.invert.whole_function.
//
// whole_function.py -- whole-function templates: a structural matcher over the instructions and an
// emitter that fills the holes, so a recurring shape is read once and reused on every module.  The
// compiler then proves each candidate exact or rejects it.
//
// Template so far: the pure forwarding thunk (loadds prologue; pushes of zeros and [bp+off] params;
// push cs; call a sibling; [add sp,N]; the epilogue), in both argument orders.  C++
// (native/invert/msc6_helpers.cc; docs/NATIVE.md); this calls its entry point (cbinv::whole_function_candidates).
//
//     auto c = whole_function::synthesize(func_bytes);     // -> candidate C str, or None
//
// Typing notes:
//   * candidates() is the Python's generator: here the whole list of (template, candidate C), in order.
//   * synthesize's `verify(func_bytes, c)` answers the dict {'exact': bool} the Python reads with .get("exact");
//     an empty std::function is verify=None (the first candidate is taken).
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::whole_function {

// (template, candidate C) for `func_bytes` from every matching template.
py::list<std::tuple<py::str, py::str>> candidates(const py::bytes& func_bytes);

using Verify = std::function<py::dict<py::str, bool>(const py::bytes&, const py::str&)>;

// The first candidate C that `verify(func_bytes, c)` accepts ({'exact': True}), or -- with no
// verifier -- the first candidate.
std::optional<py::str> synthesize(const py::bytes& func_bytes, const Verify& verify = nullptr);

}  // namespace codebreaker::invert::whole_function
