// codebreaker/build/sm83_asm.hpp -- C++ from codebreaker.build.sm83_asm.
#pragma once

#include "py.hpp"
#include "codebreaker/build/z80_asm.hpp"
#include "codebreaker/decompile/sm83.hpp"
#include "codebreaker/decompile/z80.hpp"

// sm83_asm.py -- CodeBreaker's own Game Boy assembler, for the source the Game Boy lane writes.
//
//     rom = sm83_asm::build(source_text, size)     // the ROM image (size: its bytes; unwritten ones $FF)
//
// It reads the part of RGBDS's language that source/gb_writer.py writes, and that Game Boy homebrew
// is commonly written in, so the source also builds with RGBDS (rgbasm, rgblink -n / rgbfix):
//
//     SECTION "name", ROM0[$addr]                 bank 0, at a fixed address
//     SECTION "name", ROMX[$addr], BANK[n]        bank n (seen at $4000-$7FFF)
//     db / dw                                     bytes, little-endian words: numbers, labels, "strings"
//     ds n, value                                 n bytes of value
//     INCBIN "file"
//     DEF NAME EQU value  /  NAME EQU value       constants
//     label:  /  .local:                          the address of the next byte
//     ; comment
//
// Dependencies (other modules' C++, by the signatures the Python calls them with):
//   codebreaker::build::z80_asm   AsmError, _strip(str) -> str, _items(str) -> list<str>
//   codebreaker::decompile::z80   value(str expr, dict<str,int64_t> labels, int64_t pc = 0) -> int64_t
//                                 (raises py::KeyError for an unknown label, py::ValueError otherwise)
//   codebreaker::decompile::sm83  assemble(str line, int64_t pc, dict<str,int64_t> labels) -> bytes

namespace codebreaker::build::sm83_asm {

extern const int64_t BANK;

// The ROM `source` assembles to: `size` bytes, the unwritten ones $FF; INCBIN files are read from `folder`.
// Raises AsmError ("line N: <text> (<reason>)") for a line the assembler refuses.
py::bytes build(const py::str& source, int64_t size, const py::str& folder = py::str("."));

py::bytes _size_guess(const py::str& line, int64_t pc);
void _put(const py::bytearray& image, int64_t bank, int64_t pos, const py::bytes& data);

}  // namespace codebreaker::build::sm83_asm
