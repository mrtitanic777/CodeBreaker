// codebreaker/invert/grammars.hpp -- C++ from codebreaker.invert.grammars.
#pragma once

#include <optional>

#include "py.hpp"

namespace codebreaker::invert::grammars {

// A grammar's context: None, or a dict (only its "nargs" is read). The Python takes any dict; every
// value a caller passes is an int, so the typed form is dict[str, int].
using Ctx = std::optional<py::dict<py::str, int64_t>>;

// The Python grammars are generators; each is finite and pure, so they are returned whole, in the order
// the generator yields them.
py::list<py::str> g_thunk(const Ctx& ctx);
py::list<py::str> g_bufread(const Ctx& ctx);

using Grammar = py::list<py::str> (*)(const Ctx&);
extern const py::list<Grammar> GRAMMARS;   // [g_thunk, g_bufread]

}  // namespace codebreaker::invert::grammars
