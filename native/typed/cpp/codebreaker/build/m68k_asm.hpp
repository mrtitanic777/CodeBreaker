// codebreaker/build/m68k_asm.hpp -- C++17 port of codebreaker.build.m68k_asm.
//
// m68k_asm.py -- CodeBreaker's own 68000 assembler, for the source the Genesis lane writes.
//
//     py::bytes image = m68k_asm::build(source_text);     // the bytes from address 0 to the last one written
//
// It reads the part of the Motorola assemblers' language (asm68k, SNASM68K, vasm's mot syntax) that
// source/genesis_writer.py writes, so the source also builds with those:
//
//     org $addr                       where the next bytes go
//     dc.b / dc.w / dc.l              bytes, words and longs, big-endian: numbers, labels, "strings"
//     dcb.b n,value                   n bytes of value (dcb.w / dcb.l: words, longs)
//     NAME equ value                  constants
//     label:                          the address of the next byte
//     ; comment  (or * at the start of a line)
//
// Unwritten bytes between two org's are zero.
//
// Notes on the port:
//  * AsmError is the one of build/z80_asm (the Python imports it from there).
//  * `_size_guess` hands the assembler a dict that has every name (the Python's `_Any` subclass);
//    here the labels are copied and grown by each name the assembler asks for, worth pc + 4.
#pragma once

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/build/z80_asm.hpp"

namespace codebreaker::build::m68k_asm {

using codebreaker::build::z80_asm::AsmError;

py::str _strip(const py::str& line);
py::bytes build(const py::str& source);

// On the first pass, an instruction naming a label not defined yet: assembled with every unknown name
// standing for a nearby address (the source gives each operand's size: BRA.S / .W, (x).W / (x).L), so its
// size is the size it will have.
py::bytes _size_guess(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels);

void _put(const py::bytearray& image, int64_t at, const py::bytes& data);

}  // namespace codebreaker::build::m68k_asm
