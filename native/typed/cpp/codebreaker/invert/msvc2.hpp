// codebreaker/invert/msvc2.hpp -- C++17 port of codebreaker.invert.msvc2.
//
// msvc2.py - INVERSE of msvc2model: MSVC 2.0 (win95 C2.EXE) bytes -> byte-exact C.
//
// The 32-bit sibling of early_c.py. Recognises the codegen templates msvc2model emits and recovers
// the source expression, then SELF-VERIFIES: a candidate is accepted only if msvc2model.emit(node)
// reproduces the input bytes exactly (relocation slots -- call targets, global addresses -- masked),
// so every returned C is byte-exact by construction -- never a plausible-but-wrong guess.
// Out-of-class byte sequences are cleanly DECLINED (return None), exactly like the 16-bit engine.
//
// Scope = the modeled forward class: single-`return` __stdcall leaves over int args/consts, chains,
// mixed precedence, comparisons, deref / index, globals, stores, extern & indirect calls, the
// frameless (/Oy) and __fastcall forms, and straight-line statement lists (msvc2_registers.interp).
//
// The inverse is C++ (native/invert/msvc2.cc; docs/NATIVE.md); this calls its entry point
// (cbinv::msvc2_call) directly, and keeps the round-trip self-test.
//
//     wt inverse msvc2inv                 # round-trip battery: bytes -> C -> (oracle) -> bytes
//
// Typing notes:
//   * invert() answers the Python's dict {nargs, expr, c, node} as the record Inverted (None: an empty optional);
//     `node` is msvc2_forward's Node and `expr` its Expr (None when the node is a statement list).
//   * `mask` is the set of byte positions to ignore (None: each candidate's own relocation slots).
//   * main returns the exit status (the Python's sys.exit).
#pragma once

#include <optional>

#include "codebreaker/invert/msvc2_forward.hpp"
#include "py.hpp"

namespace codebreaker::invert::msvc2 {

struct Inverted {
    int64_t nargs = 0;
    std::optional<msvc2_forward::ExprPtr> expr;
    py::str c;
    msvc2_forward::Node node;
};

// bytes -> {nargs, expr, c, node} byte-exact, or None if out of the modeled class.  `mask`:
// the byte positions to ignore (default: each candidate's own relocation slots).
std::optional<Inverted> invert(const py::bytes& code, const std::optional<py::set<int64_t>>& mask = std::nullopt);

// Round-trip: oracle-compile each modeled shape -> bytes -> invert -> C -> re-check by
// msvc2model.emit (== the oracle bytes we already validate). True bytes->C->bytes.
bool selftest(bool show = false);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::msvc2
