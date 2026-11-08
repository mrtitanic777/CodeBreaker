// codebreaker/source/sms_writer.hpp -- typed C++ of codebreaker.source.sms_writer.
//
// sms_writer.py -- a Master System / Game Gear program as assembly source that builds back into it.
//
//     sms_writer::main(argv)                 python -m codebreaker.source.sms_writer <game.sms> [<out folder>]
//     r = sms_writer::reconstruct(path, folder);
//     r.source                       game.asm (WLA-DX syntax)
//     r.same                         the source, built, is the cartridge image byte for byte
//     r.functions                    {address: Function(name)}
//
// These programs were written in assembly, so assembly is what comes back. Code is what the
// program's own flow reaches from the reset, interrupt and NMI vectors in the pages mapped at
// power-on (formats/sms.py); every byte it reaches is written as an instruction, every other byte
// as data. Function starts are named for their vector (`reset`, `irq`, `nmi`) or `sub_<address>`,
// jump targets `L_<address>`, and absolute words naming the start of a line get that line's label,
// so the source can be moved and edited, not only rebuilt. The console's ports and the mapper's
// registers are written by name.
//
// It builds with CodeBreaker's own assembler (build/z80_asm.py) or with WLA-DX
// (`wla-z80 -o game.o game.asm && wlalink linkfile game.sms`).
//
// Typing notes: the result is image_writer's Result, its "rom bytes" field in `more`; the module's
// docstring (what the command line prints) is DOC.
#pragma once

#include <tuple>

#include "codebreaker/decompile/z80.hpp"
#include "codebreaker/formats/sms.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::sms_writer {

// the module's docstring (w.main prints its first paragraph)
extern const py::str DOC;

// the console's I/O ports and the mapper's registers, by the names the hardware documentation gives them
extern const py::dict<int64_t, py::str> PORTS;
extern const py::dict<int64_t, py::str> GG_PORTS;
extern const py::dict<int64_t, py::str> REGISTERS;

// {ROM offset: Insn} for what flow reaches in the power-on mapping, its targets, the starts.
std::tuple<py::dict<int64_t, decompile::z80::Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const formats::sms::ROM& rom);

int64_t _slot(int64_t page);

image_writer::Result reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::sms_writer
