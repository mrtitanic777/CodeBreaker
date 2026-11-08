// codebreaker/invert/quad_dump.hpp -- C++17 port of codebreaker.invert.quad_dump.
//
// quad_dump.py - structural parser for Lattice C 2.12 quad files (<base>.q).
//
// Usage: python quad_dump.py <file.q>      (here: quad_dump::cli, below)
//
// Record framing (from LC2's reader @0x28c5 and confirmed by tiling every sample):
//   each quad record = [opcode:1][length:1][payload: `length` bytes]
// The reader reads 2 bytes (opcode,length) then `length` more, then parses up to 3
// operands from the payload. A function is a run of records starting at FUNC-BEGIN
// (0x23); the module ends at a 0xff terminator, after which come symbol trailer bytes
// and the NUL-terminated name pool.
//
// Opcode map (differentially verified via dynamic analysis; see
// re/notes/05-quad-format-and-harness.md, 07-quad-framing.md):
#pragma once

#include "py.hpp"

namespace codebreaker::invert::quad_dump {

// the module's docstring (what `python quad_dump.py` with no file prints before the opcode list)
extern const py::str DOC;

// opcode -> mnemonic
extern const py::dict<int64_t, py::str> OPC;

// The offset where the trailing run of printable ASCII and NUL bytes (the name pool) starts.
int64_t namepool_start(const py::bytes& d);

// operands are packed little-endian words; show a compact view
py::str fmt_payload(const py::bytes& p);

// Dump the quad file at `path` to stdout (raises as the Python does: OSError, struct.error).
void main(const py::str& path);

// The module's `if __name__ == "__main__":` block. `argv` is sys.argv (the program name first).
// -> the process's exit status (sys.exit(1) after the usage text, else 0).
int64_t cli(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::quad_dump
