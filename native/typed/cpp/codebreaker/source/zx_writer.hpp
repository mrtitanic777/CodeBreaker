// codebreaker/source/zx_writer.hpp -- typed C++ from codebreaker.source.zx_writer.
//
// zx_writer.py -- a ZX Spectrum program as assembly source that builds back into it.
//
//     python -m codebreaker.source.zx_writer <game.z80|.sna|.tap> [<out folder>]
//     r = zx_writer::reconstruct(path, folder)     // as source/z80_writer.py's write()
//
// The program block (formats/zx.py: a snapshot's RAM above the screen, a tape's code block) is
// written through source/z80_writer.py, with the Spectrum's names: its ULA port, the 48K ROM's
// documented entry points and the system variables, as the Spectrum manual and The Complete
// Spectrum ROM Disassembly name them.
//
// Typing notes:
//   * reconstruct() gives z80_writer's Written (image_writer's Result with `built_image`).
//   * The command line is image_writer::main with this module's reconstruct and docstring (DOC); main() returns the exit code.
#pragma once

#include <cstdint>

#include "codebreaker/source/z80_writer.hpp"
#include "py.hpp"

namespace codebreaker::source::zx_writer {

// the module's docstring (what the command prints for --help)
extern const py::str DOC;

extern const py::dict<int64_t, py::str> PORTS;
extern const py::dict<int64_t, py::str> NAMES;

z80_writer::Written reconstruct(const py::str& path, const py::str& folder);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::zx_writer
