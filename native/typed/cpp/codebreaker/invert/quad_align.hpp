// codebreaker/invert/quad_align.hpp -- C++17 port of codebreaker.invert.quad_align.
//
// quad_align.py - align a quad file and its compiled object per-function, yielding
// (name, quad_records, code_bytes) triples: the ground-truth (input,output) pairs of
// the MC2 code generator. Foundation for mc2model (forward quad->bytes model).
//
// Usage: python quad_align.py <corp.q/corpq.bin> <corp.OBJ>
//
// Typing notes:
//   * The OMF record walk and index decoding are the typed codebreaker::formats::omf::walk / index
//     (the Python's `walk` generator is the class omf::Walk: next() gives the records, then throws the
//     failure the walk met).
//   * main() takes sys.argv (the program name first), as invert::quad_dump::cli does; it returns
//     nothing (the Python's main() returns None and the script never calls sys.exit).
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::quad_align {

// one quad record: (opcode, payload bytes)
using QuadRecord = std::tuple<int64_t, py::bytes>;

// Return list of functions, each = list of (opcode, payload_bytes).
py::list<py::list<QuadRecord>> parse_quads(const py::str& qpath);

// Return [(name, off, bytes)] for PROG functions, in address order.
py::list<std::tuple<py::str, int64_t, py::bytes>> parse_obj_funcs(const py::str& objpath);

// The script: `python quad_align.py <quads> <obj>`; `argv` is sys.argv (the program name first). Prints
// the alignment to stdout; raises as the Python does (IndexError for a missing argument, OSError ...).
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::quad_align
