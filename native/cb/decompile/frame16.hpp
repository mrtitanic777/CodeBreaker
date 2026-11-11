// cb/decompile/frame16.hpp -- a 16-bit function's locals declared so that Microsoft C 8 lays them out as the original's.
//
//     auto said = frame16::lay_out(doc, code, start, end);   // doc edited in place
//
// C 8 places a function's locals itself (measured with C 8.00c, 2026-10-29):
//
//     /Os   in the order they are declared, from [bp-2] down, each taking a whole number of words
//     /Ox   bytes first (two to a word), then by size; of one size, by first use
//     /Od   (an order of its own, not read yet)
//
// so a source gives the original's frame when its locals are the original's -- the same places, the same sizes --
// declared nearest the frame pointer first. The decompiler's are not: it splits a buffer whose address is taken into the
// pieces read from it (SUWIN 3:5DAA's 16-byte buffer at [bp-12h] came out as xStack_14, xStack_10, xStack_e), and gives a
// flag kept in a word of the frame no place at all (a bool). The instructions say what the frame was:
//
//   its places      every [bp-k] the function's instructions reach, as wide as they reach it; one whose address is taken
//                   (lea) is a buffer, up to the next place above it
//   each local's    a local the decompiler kept on the stack (L2 Local "at": {"stack": n}, which the decompiler gives with
//   place           CB_FRAME16=1; BP being the entry SP less 2) at n + 2; one with none, the place the instruction of a
//                   statement assigning it writes (L2 origin "at")
//
// and the tree is rewritten to them: a local in a place of its own takes the place's width as its type (a bool kept in a
// word is an int2); the pieces of a buffer become one array of its bytes, each piece read through it at its offset; the
// locals are declared nearest the frame pointer first.
//
// lay_out edits the document (l2tree.hpp) in place and answers what it did, one sentence each; the tree is unchanged,
// with the reason, where the frame cannot be read.
//
// A pass on the function's L2 tree before the C back end writes it (decompiler.c_from_l2, CB_FRAME16=1; the module's L2
// tree is made of the trees so laid out).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/decompile/inline_asm.hpp"

namespace cb::decompile::frame16 {

// code: the segment's bytes; [start, end) the function's range in it (clamped to the segment). `decoder` is the 16-bit
// decoder with operand sizes and access (default: inline_asm::decode16).
std::vector<std::string> lay_out(Json& doc, ByteView code, std::uint32_t start, std::uint32_t end,
                                 const inline_asm::Decoder& decoder = {});

}  // namespace cb::decompile::frame16
