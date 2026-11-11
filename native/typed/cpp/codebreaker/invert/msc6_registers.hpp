// codebreaker/invert/msc6_registers.hpp -- C++17 port of codebreaker.invert.msc6_registers.
//
// msc6_registers.py -- register-value-tracking inverse for MSC 6.0a (the piece register
// variables need, beyond the template walker). A small abstract interpreter tracks each
// register's symbolic C-expression through the instruction stream, recovers the enregistered
// locals (SI/DI) and the value-numbering idiom (`push ax; mov si,ax` = one value both passed
// AND kept live in a var), and emits C; and the counted for-loop with a register counter.
// Verified byte-exact via the oracle (relocs masked). Declines at the first unmodeled instruction.
//
// Both are C++ (native/invert/msc6_registers.cc; docs/NATIVE.md); this calls their entry points
// (cbinv::msc6_invert_reg, cbinv::msc6_invert_loop) directly.
//
//     wt inverse msc6reg     # runs the non-loop register-var self-test
//
// Typing notes:
//   * Both answer the Python's (text, meta) or (None, reason) as the record Inverted: `text` is empty when the
//     model declines, and then `why` is the reason.
//   * `meta` is a dict whose keys depend on the function: invert_reg gives saved, regvars, stmts; invert_loop
//     gives counter, limit, body_calls: the record Meta, each key the other function lacks an empty optional.
//   * imports is {call index: name}.
#pragma once

#include <optional>

#include "py.hpp"

namespace codebreaker::invert::msc6_registers {

struct Meta {
    std::optional<py::list<py::str>> saved;
    std::optional<py::list<py::str>> regvars;
    std::optional<py::list<py::str>> stmts;
    std::optional<py::str> counter;
    std::optional<py::str> limit;
    std::optional<py::list<py::str>> body_calls;
};

struct Inverted {
    std::optional<py::str> text;
    Meta meta;
    py::str why;
};

// code:bytes (a whole function) -> (c_source, meta) or (None, reason).  imports: {call index: name}.
Inverted invert_reg(const py::bytes& code, const py::dict<int64_t, py::str>& imports = py::dict<int64_t, py::str>());

// Recover the MSC 6.0a counted-for-loop with a register counter:
//     di = param ; si = 0 ; or di,di; jle skip ; L: BODY ; inc si; cmp si,di; jl L; skip: return si
// -> `for (i = 0; i < a; i++) BODY; return i;`  Declines otherwise.
Inverted invert_loop(const py::bytes& code, const py::dict<int64_t, py::str>& imports = py::dict<int64_t, py::str>());

void _selftest();

}  // namespace codebreaker::invert::msc6_registers
