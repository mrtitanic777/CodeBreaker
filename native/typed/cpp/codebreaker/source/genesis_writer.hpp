// codebreaker/source/genesis_writer.hpp -- typed C++ of codebreaker.source.genesis_writer.
//
// genesis_writer.py -- a Genesis / Mega Drive program as assembly source that builds back into it.
//
//     genesis_writer::main(argv)                 python -m codebreaker.source.genesis_writer <game.md> [<out folder>]
//     r = genesis_writer::reconstruct(path, folder);
//     r.source                     game.asm (Motorola syntax: asm68k, vasm)
//     r.same                       the source, built, is the cartridge image byte for byte
//     r.functions                  {address: Function(name)}
//
// Typing notes: reconstruct() gives image_writer's Result (the Python's dict); "rom bytes" is in its `more`.
#pragma once

#include <tuple>

#include "codebreaker/decompile/m68k.hpp"
#include "codebreaker/formats/genesis.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::genesis_writer {

// the module's docstring (w.main prints its first paragraph)
extern const py::str DOC;

// the console's registers, by the names the hardware documentation gives them
extern const py::dict<int64_t, py::str> HARDWARE;
extern const py::list<std::tuple<int64_t, int64_t, py::str>> _HEADER;
extern const py::dict<int64_t, std::tuple<int64_t, py::str>> _HEADER_AT;

int64_t _length(const decompile::m68k::Insn& insn);

std::tuple<py::dict<int64_t, decompile::m68k::Insn>, py::set<int64_t>, py::list<int64_t>> _walk(const formats::genesis::ROM& rom);

// Addresses an instruction's operands name (absolute, PC-relative, long immediates): (value, kind).
py::list<std::tuple<int64_t, py::str>> _refs(const decompile::m68k::Insn& insn);

image_writer::Result reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::genesis_writer
