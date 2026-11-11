// cb/symbols/libraries/omf.hpp -- the functions of a 16-bit OMF object module or library: each one's name, bytes and mask.
//
//     const std::vector<Function> fs = omf::functions(lib_bytes);       // every object module of a .LIB (or one .OBJ)
//     const Bytes only_fields = omf::fields_only(f.mask);               // far-call opcodes compared too
//
// The 16-bit tools' libraries (Microsoft C's and Visual C++ 1.5's run-time libraries) are object modules laid end to
// end on pages. A function is a public name inside a code segment (one whose class says CODE), or a piece the linker
// places on its own (a COMDAT: the C++ compiler writes one per function). What the linker fills in is listed by the
// fix-up records after each data record: the bytes of each fix-up's field (1 in the mask) -- and the opcode of a far
// call (2), which a linker that translates far calls rewrites, with the address, as `push cs` / `call near` when the
// callee ends up in the same segment. Any non-zero value leaves a byte out; a matcher that knows its program was
// linked without that translation compares a far call's opcode (`fields_only`).
//
// (cb/formats/omf is the linker-side reader of object modules, for what a module declares; this one reads what a
// library holds, to find it again in a program.)
#pragma once

#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/symbols/libraries/package.hpp"

namespace cb::symbols::libraries::omf {

// The functions of every object module of a library (a dictionary-paged .LIB) or of one object module. A module the
// reader cannot follow (a record that runs past its end, a fix-up outside its segment) is left out whole; the
// others are still read. Data that is neither gives nothing.
std::vector<Function> functions(ByteView data);

// The functions of one object module (its bytes, MODEND included). TruncatedError / FormatError for a module that
// cannot be followed.
std::vector<Function> module_functions(ByteView module);

// The mask without the opcodes of far calls: only what a fix-up's field covers.
Bytes fields_only(ByteView mask);

}  // namespace cb::symbols::libraries::omf
