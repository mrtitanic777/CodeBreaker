// codebreaker/translate/backends/c.hpp -- typed C++ port of codebreaker.translate.backends.c.
//
// c.py -- the hub's C back end: an L2 function document as C (docs/IR_UNIFICATION.md, step 2).
//
// The decompiler's PrintL2 writes each function as L2 (`/*@L2 {...} */`, CB_L2=1). This writes it
// back as C in the decompiler's own dialect -- its type names (int2, uint4, xunknown2, code), its
// calling-convention words (__cdecl16far, __stdcall), hexadecimal past 9 -- so what it writes goes
// through decompile/c_text.py's passes as the decompiler's C does today, and is compiled and judged
// the same way. tools/l2c.py holds it to that C, character for character, over the golden corpus.
//
// The writer is C++ (native/l2: l2c in cbl2.dll, docs/NATIVE.md step 5): the layout (PrintC's
// pretty printer), the writer, every c_text pass done on the tree and what the module decides. Each
// entry point below is the library's, through translate/native_l2 (here: native_l2.hpp, which calls the
// library linked in); the Python writer it was ported from line by line is gone.
//
//     function(doc, moved=()) -> C text of one function          type_text(t, name='') -> a declaration
//     module(doc) -> (C text, {name: assembly}) of a Module: its functions, and what stayed assembly
//
// `moved` names the passes of decompile/c_text.py done here instead, on the tree (step 3): each is
// written where the address spaces and types L2 carries say so, and c_text leaves its own out for C
// written from L2. MOVED is every one there is. What they add is laid out as taking no room: c_text
// adds the same to the printed C, after PrintC broke its lines.
//
// Typing notes:
//   * The Python's `function = _E["function"]` ... are the functions of native_l2.hpp, named here by
//     using-declarations: same names, same signatures (see there for the types).
//   * MOVED, IN_MODULE, FLAT and POWERPC are tuples of names (py::list<py::str>); SIZEOF is {type name: bytes}.
//   * _param_name returns the document's own value (a Val: a str, or None where the document has none).
#pragma once

#include <cstdint>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::c {

namespace ir = codebreaker::translate::ir;

// c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order)
extern const py::list<py::str> MOVED;
// ... those of them that need what the module's facts say of the function (its model in the
// description, what its instructions read and start with): done only when `function` is given
// them, as c_text gives them when it makes the module (c_text._write16); the C the decompiler is
// run for at first (decompiler.c_from_l2) is written without them
extern const py::list<py::str> IN_MODULE;
// ... those of them the 32-bit path has (compilable32)
extern const py::list<py::str> FLAT;
// ... and those of the PowerPC path (c_text.compilable_powerpc; the facts' "lane": "powerpc")
extern const py::list<py::str> POWERPC;

// A node this back end does not write.
using Decline = native_l2::Decline;   // (the C back end is the L2 core library: its decline is the one native_l2 raises)

// (bytes of the decompiler's scalar type names)
extern const py::dict<py::str, int64_t> SIZEOF;

// (p.get('sym') or {}).get('name') or p.get('name', ''): the name as the document holds it (a str, or None)
ir::Val _param_name(const ir::Dict& p);

// ---------------------------------------------------------------------------------------------- the entry points
// (each the library's: the same names, arguments, text, facts and exceptions the Python writer had)
using native_l2::argument_words;
using native_l2::arguments_at;
using native_l2::caller_frame_plan;
using native_l2::condition;
using native_l2::expr;
using native_l2::flags;
using native_l2::fragment;
using native_l2::function;
using native_l2::module;
using native_l2::module_names32;
using native_l2::parameter_names;
using native_l2::parameters;
using native_l2::prototypes;
using native_l2::seg_layout;
using native_l2::segment_plan;
using native_l2::signature;
using native_l2::struct_accesses;
using native_l2::takes32;
using native_l2::type_names;
using native_l2::type_text;
using native_l2::words;

}  // namespace codebreaker::translate::backends::c
