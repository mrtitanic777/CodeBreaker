// codebreaker/invert/msc6_walk.hpp -- C++17 port of codebreaker.invert.msc6_walk.
//
// msc6_walk.py -- generalized whole-function MSC 6.0a inverse: a symbol-map-driven statement walker
// (returns, if-blocks recursed, far calls, long/word global stores), declining at the first unmodeled
// byte.  C++ (native/invert/msc6_small.cc; docs/NATIVE.md); this calls its entry point (cbinv::msc6_walk).
//
// Typing notes:
//   * invert() answers the Python's (text, meta) or (None, why) as the record Inverted: `text` is empty
//     when the model declines, and then `why` is the reason; `meta` is the dict {"stmts", "globals"}.
//   * syms is {offset: (name, 'long'|'word')}.
//   * The Python's `native_invert` answered a walk with unbalanced JSON (the closing brace of the answer was
//     missing), so invert() raised json.JSONDecodeError on every function it read; fixed in
//     native/invert/msc6_small.cc and so here (reported in _bugs/LIST.md).
//   * `main` is the module's `if __name__ == "__main__"` regression: the Cleanup function through the walker,
//     recompiled by Microsoft C 6.0a; it returns the process's exit status.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::msc6_walk {

struct Meta {
    int64_t stmts = 0;
    py::dict<py::str, py::str> globals;   // name -> 'long' | 'word'  (the model's own words: "long" or "int")
};

struct Inverted {
    std::optional<py::str> text;   // the C text; empty when the model declined
    Meta meta;                     // (with text)
    py::str why;                   // (without text) the reason it declined
};

Inverted invert(const py::bytes& code, const py::dict<int64_t, std::tuple<py::str, py::str>>& syms, const py::str& callee = py::str("G"));

int64_t main();

}  // namespace codebreaker::invert::msc6_walk
