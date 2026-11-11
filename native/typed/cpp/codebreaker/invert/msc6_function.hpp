// codebreaker/invert/msc6_function.hpp -- C++17 port of codebreaker.invert.msc6_function.
//
// msc6_function.py -- whole-function MSC 6.0a inverse for the long-global-free idiom (LZEXPAND's
// Cleanup: `if (gpX != 0L) { GlobalFree((WORD)(gpX>>16)); gpX = 0L; }` blocks), and the reloc mask the
// models compare recompiled bytes under: the DGROUP offsets and far-call targets the linker fills.
//
// Both are C++ (native/invert/msc6_small.cc, msc6_helpers.cc; docs/NATIVE.md); the Python called them through
// ctypes, this calls their entry points (cbinv::msc6_mask, cbinv::msc6_cleanup) directly.
//
//     python -m codebreaker invert --tool msc6-function     # the Cleanup self-test against the real compiler
//
// Typing notes: all three signatures are plain-typed (bytes, dict of int to str, str).  _selftest prints
// its report with py::print, as the Python does; it needs Microsoft C 6.0a (invert/compilers).
#pragma once

#include "py.hpp"

namespace codebreaker::invert::msc6_function {

// The bytes with the relocated operands zeroed: a far call's target, a near call's rel16, the
// imm16 of mov ax,[m] / mov [m],ax / push imm16, and a direct [imm16] operand's displacement.
py::bytes _mask(const py::bytes& code);

// The Cleanup function's C.  syms: {lo-word offset: name} for each long global.
py::str invert_cleanup(const py::bytes& code, const py::dict<int64_t, py::str>& syms);

// the module's `if __name__ == "__main__"` block
void _selftest();

}  // namespace codebreaker::invert::msc6_function
