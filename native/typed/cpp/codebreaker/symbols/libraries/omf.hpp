// codebreaker/symbols/libraries/omf.hpp -- typed C++ from codebreaker.symbols.libraries.omf.
//
// omf.py -- the functions of a 16-bit OMF object module or library: each one's name, bytes and mask.
//
//     omf::functions(data)         [Function] of every object module in a .LIB (or one .OBJ)
//
// The 16-bit tools' libraries (Microsoft C's and Visual C++ 1.5's run-time libraries) are object
// modules laid end to end on pages. A function is a public name inside a code segment (one whose
// class says CODE), or a piece the linker places on its own (a COMDAT: the C++ compiler writes one
// per function). What the linker fills in is listed by the fix-up records after each data record:
// the bytes of each fix-up's field (1 in the mask) -- and the opcode of a far call (2), which a
// linker that translates far calls rewrites, with the address, as `push cs` / `call near` when the
// callee ends up in the same segment. Any true value leaves a byte out; a matcher that knows its
// program was linked without that translation compares a far call's opcode (`fields_only`).
#pragma once

#include <tuple>

#include "py.hpp"

namespace codebreaker::symbols::libraries::omf {

// One function of a library: its symbol, its bytes, its mask and its offset in its section.
// (codebreaker.symbols.libraries.Function, a namedtuple "name code mask at" of the package's __init__, which has no
// C++ port yet: this is its stand-in. The mask is a bytearray here, as omf.py makes it.)
struct Function {
    py::str name;
    py::bytes code;
    py::bytearray mask;
    int64_t at = 0;
};

extern const py::dict<int64_t, int64_t> _WIDTH;   // bytes a 16-bit fix-up covers, by kind

// the number at b[i] (one byte, or two when the high bit is set) and the index after it
std::tuple<int64_t, int64_t> _number(const py::bytes& b, int64_t i);

// The mask without the opcodes of far calls: only what a fix-up's field covers.
py::bytes fields_only(const py::bytes& mask);

// The byte mask of `size` bytes from `start`, out of {offset: 1 | 2} for a whole buffer.
py::bytearray _mask(const py::dict<int64_t, int64_t>& masked, int64_t start, int64_t size);

// [Function] for every function in a library (a dictionary-paged .LIB) or one object module.
// A module the reader cannot follow is left out.
py::list<Function> functions(const py::bytes& data);

// [Function] for the public functions of one object module (its bytes, MODEND included).
py::list<Function> module_functions(const py::bytes& module);

}  // namespace codebreaker::symbols::libraries::omf
