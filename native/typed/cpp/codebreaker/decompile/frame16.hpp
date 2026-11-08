// codebreaker/decompile/frame16.hpp -- interface of codebreaker.decompile.frame16 (typed C++; implementation pending).
//
// frame16.py -- a 16-bit function's locals declared so that Microsoft C 8 lays them out as the original's.
//
//     auto [doc, said] = frame16::lay_out(doc, code, start, end);
//
// C 8 places a function's locals itself (/Os: in declaration order from [bp-2] down; /Ox: bytes first, then by size),
// so a source gives the original's frame when its locals are the original's -- same places, same sizes. The
// instructions say what the frame was (every [bp-k] reached, as wide as it is reached; one whose address is taken
// (lea) is a buffer), and the tree is rewritten to them: a bool kept in a word becomes an int2, the pieces of a
// buffer become one array of its bytes read through at their offsets, the locals are declared nearest the frame
// pointer first. -> (the tree, what was done); unchanged, with the reason, where the frame cannot be read.
//
// Typing notes:
//   * `doc` is the L2 document, translate::ir::Dict (= core16::L2Doc; see core16_types.hpp); lay_out works on a deep copy and answers it.
//   * `code` is the segment's bytes; [start, end) the function's range in it.
//   * _walk (private in the Python, reachable from tests) yields every dict node of a tree, depth first, a node
//     before its members: here those nodes in that order, as a list.
#pragma once

#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::decompile::frame16 {

std::tuple<translate::ir::Dict, py::list<py::str>> lay_out(const translate::ir::Dict& doc, const py::bytes& code, int64_t start,
                                                     int64_t end);

py::list<translate::ir::Dict> _walk(const translate::ir::Val& n);

}  // namespace codebreaker::decompile::frame16
