// codebreaker/source/z80_writer.hpp -- typed C++ of codebreaker.source.z80_writer.
//
// z80_writer.py -- a flat Z80 program (one block of memory at one address) as assembly source that
// builds back into it. The ZX Spectrum and MSX lanes write through it (source/zx_writer.py's and
// msx's callers give the block, its entry points and the machine's names).
//
//     r = z80_writer::write(image, origin, entries, folder, stem, ports, names, header);
//     r.source                        <stem>.asm (sjasmplus / pasmo syntax: ORG, DEFB, DEFW, DEFS, EQU)
//     r.same                          the source, built, is the block byte for byte
//     r.functions                     {address: Function(name)}
//
// Code is what the program's own flow reaches from its entry points; every byte it reaches is
// written as an instruction, every other byte as data. Entry points keep the names given,
// called functions are `sub_<address>`, jump targets `L_<address>`, and pointers loaded into HL
// DE IX IY (or read and written through) that name the start of a line get that line's label, so
// the source can be moved and edited, not only rebuilt. Ports and fixed addresses (system
// variables, ROM routines) are written by the names the machine's documentation gives them.
//
// Typing notes (what the Python leaves open):
//   * The result is image_writer's Result (the dict every writer gives back) with the one field this
//     writer adds, r["built image"] (the bytes the source built to, when it built), as `built_image`.
//   * `entries` is the Python's {address: name}; walk() reads only its keys, so it also takes a list of
//     addresses (the Python takes any iterable of them).
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/decompile/z80.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::z80_writer {

// What write() gives back: the result every writer gives, and r["built image"] when the source built.
struct Written : image_writer::Result {
    std::optional<py::bytes> built_image;
};

// ({address: Insn} reached from the entries, {targets}, [function starts])
std::tuple<py::dict<int64_t, decompile::z80::Insn>, py::set<int64_t>, py::list<int64_t>> walk(const py::bytes& image, int64_t origin,
                                                                                               const py::list<int64_t>& entries);
std::tuple<py::dict<int64_t, decompile::z80::Insn>, py::set<int64_t>, py::list<int64_t>> walk(const py::bytes& image, int64_t origin,
                                                                                               const py::dict<int64_t, py::str>& entries);

// `entries` {address: name}; `ports` {port: name}; `names` {address: name} for fixed addresses
// outside the block (system variables, ROM routines).
Written write(const py::bytes& image, int64_t origin, const py::dict<int64_t, py::str>& entries, const py::str& folder, const py::str& stem,
              const py::dict<int64_t, py::str>& ports = py::dict<int64_t, py::str>(),
              const py::dict<int64_t, py::str>& names = py::dict<int64_t, py::str>(),
              const py::list<py::str>& header = py::list<py::str>());

}  // namespace codebreaker::source::z80_writer
