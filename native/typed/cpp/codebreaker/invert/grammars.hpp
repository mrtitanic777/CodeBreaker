// codebreaker/invert/grammars.hpp -- SHIM over cb/invert/grammars.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
#pragma once

#include <algorithm>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/invert/grammars.hpp"

namespace codebreaker::invert::grammars {

// A grammar's context: None, or a dict (only its "nargs" is read).
using Ctx = std::optional<py::dict<py::str, int64_t>>;

inline cb::invert::grammars::Shape shape_of(const Ctx& ctx) {
    cb::invert::grammars::Shape shape;
    if (ctx) shape.nargs = static_cast<std::size_t>(std::max<int64_t>(0, ctx->get(py::str("nargs"), 3)));
    return shape;
}

inline py::list<py::str> g_thunk(const Ctx& ctx) { return cb::compat::to_py(cb::invert::grammars::thunk(shape_of(ctx))); }
inline py::list<py::str> g_bufread(const Ctx& ctx) { return cb::compat::to_py(cb::invert::grammars::bufread(shape_of(ctx))); }

using Grammar = py::list<py::str> (*)(const Ctx&);
inline const py::list<Grammar> GRAMMARS = [] {
    py::list<Grammar> g;
    g.append(&g_thunk);
    g.append(&g_bufread);
    return g;
}();

}  // namespace codebreaker::invert::grammars
