// codebreaker/source/nes_writer.hpp -- typed C++ of codebreaker.source.nes_writer.
//
// nes_writer.py -- a NES program as assembly source that builds back into it.
//
//     nes_writer::main(argv)                 python -m codebreaker.source.nes_writer <game.nes> [<out folder>]
//     r = nes_writer::reconstruct(path, folder);
//     r.source, r.more["config"]     game.s (ca65 syntax) and game.cfg (ld65), beside the CHR as game.chr
//     r.same                         the source, built, is the cartridge image byte for byte
//     r.functions                    {address: Function(name)}
//
// NES programs were written in assembly almost without exception, so assembly is what comes back
// (docs/PLATFORM_DESCRIPTOR.md). Code is what the program's own flow reaches from its three
// vectors (formats/nes.py); every byte it reaches is written as an instruction, every other byte
// as data. Function starts are named for their vector (`reset`, `nmi`, `irq`) or `sub_<address>`,
// branch and jump targets `L_<address>`, and an absolute operand naming the start of a line gets
// that line's label -- so the source can be moved and edited, not only rebuilt. The last six
// bytes of the bank that holds them are the vectors, written as `.word nmi, reset, irq`.
//
// It builds with CodeBreaker's own assembler (build/m6502_asm.py) or with the cc65 suite
// (`ca65 game.s && ld65 -C game.cfg -o game.nes game.o`).
//
// Typing notes: the result is image_writer's Result, with the writer's fields "config" (a path) and
// "prg bytes" (a count) in `more`; the module's docstring (what the command line prints) is DOC.
#pragma once

#include <tuple>

#include "codebreaker/decompile/m6502.hpp"
#include "codebreaker/formats/nes.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::nes_writer {

// the module's docstring (w.main prints its first paragraph)
extern const py::str DOC;

// the console's registers, by the names the hardware documentation gives them
extern const py::dict<int64_t, py::str> HARDWARE;

// [(segment name, CPU address of its first byte, PRG offset, size)]
py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> _banks(const formats::nes::ROM& rom);

// {PRG offset of an instruction's first byte: Insn} for what flow reaches, and {target}
std::tuple<py::dict<int64_t, decompile::m6502::Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const formats::nes::ROM& rom);

py::list<py::str> _lines(const py::list<int64_t>& data);

image_writer::Result reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::nes_writer
