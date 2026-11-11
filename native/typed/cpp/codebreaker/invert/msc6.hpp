// codebreaker/invert/msc6.hpp -- C++17 port of codebreaker.invert.msc6.
//
// msc6.py -- the Microsoft C 6.0a model: one recursive pass that runs register-value tracking,
// control-flow recovery (if/else, do-while, short-circuit and materialised conditions) and the
// statement templates (calls far, near, cdecl and through a COM vtable; far pointers; longs; globals;
// stack locals) together over a whole function, and declines at the first instruction it does not
// model.  It reads the frames of Microsoft C 6.0a and of Visual C++ 1.52c (mov ax,ds and near-loadds
// prologues, the lea / sub bp teardowns).
//
// The model is C++ (native/invert/msc6.cc; docs/NATIVE.md, "the compiler models"); this calls its
// entry point (cbinv::msc6_invert) directly.
//
//     python -m codebreaker invert --tool msc6      # the self-test against the real compiler
//
// Typing notes:
//   * invert() answers the Python's (text, meta) or (None, the reason it declined) as the record Inverted:
//     `text` is empty when the model declines, and then `why` is the reason.
//   * meta is the dict {calls, regvars, globals, cdecl, has_nearcall, nears}: the record Meta.
//   * imports {k: name} names the k-th far call, syms {offset: (name, 'long'|'word')} the DGROUP variables
//     (None: none), params [(name, 'int'|'far'|'long')] the signature (None: read from the body and retf N),
//     nearmap {offset: name} the same-segment callees (None: none), func_base the function's offset in its segment.
//   * _selftest prints its report with py::print and needs Microsoft C 6.0a (invert/compilers).
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::msc6 {

struct Meta {
    int64_t calls = 0;
    py::list<py::str> regvars;
    py::dict<py::str, py::str> globals;
    bool cdecl_ = false;   // "cdecl" (a macro of the MSVC headers)
    bool has_nearcall = false;
    py::list<py::str> nears;
};

struct Inverted {
    std::optional<py::str> text;
    Meta meta;
    py::str why;
};

Inverted invert(const py::bytes& code, const py::dict<int64_t, py::str>& imports = py::dict<int64_t, py::str>(),
                const std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>>& syms = std::nullopt,
                const py::str& rtype = py::str("int"),
                const std::optional<py::list<std::tuple<py::str, py::str>>>& params = std::nullopt,
                const std::optional<py::dict<int64_t, py::str>>& nearmap = std::nullopt, int64_t func_base = 0);

void _selftest();

}  // namespace codebreaker::invert::msc6
