// codebreaker/decompile/xenos.hpp -- C++ port of codebreaker.decompile.xenos:
// the Xbox 360 GPU's (Xenos) shader microcode, as instructions.
#pragma once

#include "py.hpp"

namespace codebreaker::decompile::xenos {

extern const py::str DOC;   // the module docstring

extern const py::dict<int64_t, py::str> VECTOR;
// how many sources each vector opcode reads
extern const py::list<int64_t> VECTOR_SOURCES;
extern const py::dict<int64_t, py::str> SCALAR;
extern const py::dict<int64_t, py::str> CONTROL;
extern const py::str CHANNELS_;   // Python's _CHANNELS

// a pixel shader as fxc built it (/XuFormatRaw /Fh), and fxc's own listing of it
extern const py::list<int64_t> SAMPLE;
extern const py::list<py::str> SAMPLE_LISTING;

py::str _mask(int64_t m);
py::str _swizzle(int64_t sw);
py::str _register(int64_t selected, int64_t number);

// The words are 32-bit values (as the hardware's are); Python's unbounded ints beyond 32 bits are not modelled.
py::list<py::str> alu(int64_t d0, int64_t d1, int64_t d2);
py::list<py::str> control(int64_t d0, int64_t d1, int64_t d2);
py::list<py::str> disassemble(const py::list<int64_t>& dwords);
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::decompile::xenos
