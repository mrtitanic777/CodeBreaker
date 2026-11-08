// codebreaker/source/gb_writer.hpp -- typed C++ of codebreaker.source.gb_writer.
//
// gb_writer.py -- a Game Boy program as assembly source that builds back into it.
//
//     gb_writer::main(argv)                  python -m codebreaker.source.gb_writer <game.gb> [<out folder>]
//     r = gb_writer::reconstruct(path, folder);
//     r.source                       game.asm (RGBDS syntax)
//     r.same                         the source, built, is the cartridge image byte for byte
//     r.functions                    {address: Function(name)}
//
// Game Boy programs were written in assembly, so assembly is what comes back. Code is what the
// program's own flow reaches from the entry at $0100 and the interrupt vectors in the two banks
// mapped at power-on (formats/gb.py); every byte it reaches is written as an instruction, every
// other byte as data. The cartridge header ($0104-$014F) is written as data with its fields named
// in comments. Functions are named for their vector (`entry`, `vblank`, `stat`, `timer`, `serial`,
// `joypad`) or `sub_<address>`, jump targets `L_<address>`; the I/O registers by the names
// Pan Docs gives them (rLCDC, rSTAT, ...), as hardware.inc does.
//
// It builds with CodeBreaker's own assembler (build/sm83_asm.py) or with RGBDS
// (`rgbasm -o game.o game.asm && rgblink -o game.gb game.o`).
//
// Typing notes: the result is image_writer's Result, its "rom bytes" field in `more`; the module's
// docstring (what the command line prints) is DOC; `_HEADER` is a list of (offset, size, field)
// triples and `_HEADER_AT` its dictionary {offset: (size, field)}.
#pragma once

#include <tuple>

#include "codebreaker/decompile/sm83.hpp"
#include "codebreaker/formats/gb.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::gb_writer {

// the module's docstring (w.main prints its first paragraph)
extern const py::str DOC;

// the I/O registers, as hardware.inc names them
extern const py::dict<int64_t, py::str> REGISTERS;
// the bank controller's registers (writes into ROM space), as hardware.inc names them
extern const py::dict<int64_t, py::str> MBC_REGISTERS;
extern const py::list<std::tuple<int64_t, int64_t, py::str>> _HEADER;
extern const py::dict<int64_t, std::tuple<int64_t, py::str>> _HEADER_AT;

std::tuple<py::dict<int64_t, decompile::sm83::Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const formats::gb::ROM& rom);

image_writer::Result reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::gb_writer
