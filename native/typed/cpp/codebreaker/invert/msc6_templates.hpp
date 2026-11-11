// codebreaker/invert/msc6_templates.hpp -- C++17 port of codebreaker.invert.msc6_templates.
//
// msc6_templates.py -- the leaf templates of Microsoft C 6.00a (medium model, optimized): T0 the
// loadds far frame, T1 return-constant, T2 return-param, T3 return-global, T4 store-constant, T5 one far
// call, T6 an if-branch of leaves; anything else declined.  C++ (native/invert/msc6_small.cc;
// docs/NATIVE.md); this calls its entry point (cbinv::msc6_templates).
//
// Usage:  python -m codebreaker invert --tool msc6-templates <hexbytes> [--verify]
//
// Typing notes:
//   * invert() answers the Python's (text, meta) or (None, reason) as the record Inverted: `text` is empty when
//     the model declines, and then `why` is the reason.
//   * `meta` is a dict whose keys depend on the template (T0..T4 give nargs, frame, trailing_nop, rtype,
//     globals, template; T5 nargs, call_args, template; T6 nargs, template, lines): the record Meta, each key the
//     template lacks an empty optional.
//   * main(argv) takes sys.argv (the program name first) and returns the exit status (the Python's sys.exit codes:
//     2 without a hex argument, 1 when declined, else 0).
#pragma once

#include <optional>

#include "py.hpp"

namespace codebreaker::invert::msc6_templates {

struct Meta {
    int64_t nargs = 0;
    std::optional<int64_t> frame;
    std::optional<bool> trailing_nop;
    std::optional<py::str> rtype;
    std::optional<py::list<py::str>> globals;
    py::str template_;                                  // "template"
    std::optional<py::list<py::str>> call_args;
    std::optional<py::list<py::str>> lines;
};

struct Inverted {
    std::optional<py::str> text;
    Meta meta;
    py::str why;
};

// the module's docstring (printed by main without arguments)
extern const py::str DOC;

// (C text, meta) for a modeled template, or (None, reason).
Inverted invert(const py::bytes& code);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::msc6_templates
