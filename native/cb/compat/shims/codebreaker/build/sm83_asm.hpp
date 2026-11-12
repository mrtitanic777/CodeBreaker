// codebreaker/build/sm83_asm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/sm83_asm.hpp for the ports that still use the
// old API (source/gb_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's build is
// wired for C++20, like its neighbour z80_asm, whose AsmError and as_old this uses.
//
// Old API: BANK, build. (_size_guess and _put, the pass's own helpers, are not carried over: nothing outside sm83_asm.cpp called them.)
//
// Unlike z80_asm's shim, this one needs no old decompile/*: the new module calls cb/decompile/sm83 and z80 itself.
//
// DIVERGES, for the old callers: an expression that was wrong (a division by zero, a syntax error) is an AsmError naming the line
// where the Python let ZeroDivisionError / SyntaxError out; an INCBIN file that is not there is the matching OSError of the old layer.
#pragma once

#include "z80_asm.hpp"   // the shim beside this one: AsmError, as_old

#include "cb/build/sm83_asm.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"

namespace codebreaker::build::sm83_asm {

inline constexpr int64_t BANK = ::cb::build::sm83_asm::kBank;

// The ROM `source` assembles to: `size` bytes, the unwritten ones $FF; INCBIN files are read from `folder`.
// Raises AsmError ("line N: <text> (<reason>)") for a line the assembler refuses.
inline py::bytes build(const py::str& source, int64_t size, const py::str& folder = py::str(".")) {
    return z80_asm::as_old([&] {
        const ::cb::Bytes rom = ::cb::build::sm83_asm::build(source.utf8(), static_cast<std::size_t>(size < 0 ? 0 : size),
                                                              ::cb::fs::from_text(folder.utf8()));
        return ::cb::compat::to_py(::cb::ByteView(rom));
    });
}

}  // namespace codebreaker::build::sm83_asm
