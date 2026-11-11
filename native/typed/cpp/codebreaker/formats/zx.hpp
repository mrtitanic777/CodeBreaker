// codebreaker/formats/zx.hpp -- typed C++ from codebreaker.formats.zx.
//
// ZX Spectrum programs: snapshots (`.sna`, `.z80`) and tapes (`.tap`).
//
//     Program p = zx::read(path);
//     p.origin, p.image           the program's block of memory and the address it starts at
//     p.entries                   {address: name}: where it starts (a snapshot's PC, a tape's USR calls,
//                                 an IM 2 interrupt handler)
//     p.registers                 a snapshot's registers (a f b c d e h l ix iy sp i im), else {}
//     p.kind                      "snapshot" / "tape"
//
// A snapshot is the machine stopped: the registers, and RAM from $4000 (16 KB of it the screen and
// the system variables, the program above). .sna (48K) is a 27-byte header and the 48 KB, with the
// program counter on the stack; .z80 holds the registers and the memory, compressed (ED ED n b runs)
// in its first version and in 16 KB pages from the second on (48K: page 8 at $4000, 4 at $8000, 5 at
// $C000; 128K: pages 3-10 are banks 0-7: bank 5 at $4000, 2 at $8000, the one paged in at $C000).
// The program block taken from a snapshot is RAM from $5B00 (after the screen and the printer
// buffer) to its end.
//
// A tape is a list of blocks: a 17-byte header (type: 0 a BASIC program, 3 code; name; length; the
// load address or the BASIC autostart line) then the data. The program is the code block; its
// entry is the address BASIC's USR calls (`RANDOMIZE USR 32768`), read from the tokenised BASIC.
//
// Typing notes:
//   * The Python's Program is a bare object whose attributes the reader's answer fills in; here it is the record
//     Program (the answer's keys).  `memory` is a bytearray in Python (the answer's bytes, made mutable); it is a
//     py::bytes here, as nothing writes to it.
//   * read() asks the reader linked in (native/formats/zx/zx.cc, through cbf_call).
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::zx {

extern const std::tuple<py::str, py::str, py::str> EXTENSIONS;

// a Spectrum program (the dict formats.dll answers; the Python's Program attributes)
struct Program {
    py::str path;
    py::str kind;                                // "snapshot" / "tape"
    py::dict<py::str, int64_t> registers;
    py::bytes memory;                            // the 64 KB of the machine
    int64_t origin = 0;
    py::bytes image;
    py::dict<int64_t, py::str> entries;          // {address: name}
};
template <class F> void for_each_field_mut(Program& p, F&& f) {
    f("path", p.path, true);
    f("kind", p.kind, true);
    f("registers", p.registers, true);
    f("memory", p.memory, true);
    f("origin", p.origin, true);
    f("image", p.image, true);
    f("entries", p.entries, true);
}

bool is_zx(const py::str& path);

// the answer as a Program (the memory made a bytearray in Python)
Program _program(const Program& answer);

// The Program of a .sna, .z80 or .tap file (formats.dll: native/formats/zx).
Program read(const py::str& path);

}  // namespace codebreaker::formats::zx
