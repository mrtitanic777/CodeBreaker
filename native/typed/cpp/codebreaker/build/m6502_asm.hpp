// codebreaker/build/m6502_asm.hpp -- C++17 port of codebreaker.build.m6502_asm.
//
// m6502_asm.py -- assemble and link 6502 source in ca65 syntax, with an ld65 configuration.
//
//     py::bytes image = m6502_asm::build(source_text, config_text, folder);   // the bytes ld65 would write
//
// The subset the NES lane writes (source/nes_writer.py), enough that it rebuilds without the
// cc65 suite on the machine; the same files build with ca65 and ld65 (`ca65 game.s` and
// `ld65 -C game.cfg game.o`). Understood:
//
//     .segment "NAME"         switch to a segment (the configuration says where it loads)
//     name:                   a label at the current address
//     NAME = $2000            a constant
//     .byte 1, $02, "text"    bytes and strings          .word $1234, label    little-endian words
//     .incbin "file"          a file's bytes, from `folder`
//     LDA $0200,x ...         an instruction (decompile/m6502.assemble)
//     ; ...                   a comment
//
// The configuration's MEMORY areas, in order, are the output file: each starts at its `start`,
// holds the segments that `load` into it, and is filled to its `size` with `fill` value 0
// (fill=yes), as ld65 does.
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::m6502_asm {

extern const py::re::Pattern _AREA;

// one MEMORY area: (name, start, size, fill)
using Area = std::tuple<py::str, int64_t, int64_t, bool>;

int64_t _number(const py::str& s);

// (memory areas [(name, start, size, fill)], {segment: area})
std::tuple<py::list<Area>, py::dict<py::str, py::str>> config(const py::str& text);

py::list<py::str> _split_args(const py::str& s);
py::str _strip_comment(const py::str& line);

// -> the linked image (bytes). Raises ValueError with the line on an error.
py::bytes build(const py::str& source, const py::str& config_text, const py::str& folder = py::str("."));

}  // namespace codebreaker::build::m6502_asm
