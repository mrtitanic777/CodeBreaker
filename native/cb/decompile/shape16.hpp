// cb/decompile/shape16.hpp -- a 16-bit function's statements in the form its instructions say they were written in.
//
//     auto said = shape16::reshape(doc, code);   // doc edited in place
//
// The decompiler says what a statement does; the compiler's choice of instruction says how it was written, where C 8
// makes different code of two forms with one meaning (measured with C 8.00c):
//
//   ++ / --        `inc` / `dec` of a variable's place: x++ / x-- (the decompiler, knowing x was 1, writes x = 0 -- which
//                  C 8 makes `mov word ptr [bp-2], 0`). Only where the statement is x = x + 1 / x - 1 itself, or (a place
//                  in the frame) x = a number: the instruction a statement came from may be one of several (`x = x + 3` of
//                  three, LZEXPAND 8:00AA), another variable's value (`x = in_AX + 1`, SUWIN 9:0238; `b = p == 0` from a
//                  dec that sets the flag, KEYBOARD's VKKEYSCAN), or a pointer's own step (`p = (int4 *)((int2)p + 6)`,
//                  `p = (int2 *)0x32` from a dec of the global, LZDONE), where x++ means something else
//   if-return      an if whose then-part returns, followed by a return: C 8 lays the then-part out right after its test, so
//                  the part the original has first (the lower addresses) is the then-part -- else the condition is turned
//                  round and the two returns traded
//
// Each statement's instructions are L2's origin "at" (the decompiler's PrintL2). A pass on the function's L2 tree before
// the C back end writes it, after frame16 (decompiler.c_from_l2, CB_FRAME16=1).
//
// reshape edits the document (l2tree.hpp) in place and answers what it did, one phrase each (`x++`, `an if-return turned
// round`). `code` is the segment's bytes; the instruction of a statement is read from the eight bytes at its offset.
#pragma once

#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"

namespace cb::decompile::shape16 {

std::vector<std::string> reshape(Json& doc, ByteView code);

}  // namespace cb::decompile::shape16
