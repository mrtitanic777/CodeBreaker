// codebreaker/source/msx_writer.hpp -- typed C++ of codebreaker.source.msx_writer.
//
// msx_writer.py -- an MSX cartridge as assembly source that builds back into it.
//
//     msx_writer::main(argv)                  python -m codebreaker.source.msx_writer <game.rom> [<out folder>]
//     r = msx_writer::reconstruct(path, folder);     as source/z80_writer's write()
//
// The cartridge (formats/msx) is written through source/z80_writer from its header's entry
// points, with the MSX's names: the VDP, PSG and PPI ports and the BIOS's documented entry points
// and system variables, as the MSX2 Technical Handbook names them.
//
// Typing notes: reconstruct() gives z80_writer's Written (image_writer's Result and the built image).
#pragma once

#include "codebreaker/source/z80_writer.hpp"
#include "py.hpp"

namespace codebreaker::source::msx_writer {

// the module's docstring (main prints its first paragraph)
extern const py::str DOC;

extern const py::dict<int64_t, py::str> PORTS;
extern const py::dict<int64_t, py::str> NAMES;

z80_writer::Written reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::msx_writer
