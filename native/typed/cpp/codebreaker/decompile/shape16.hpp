// codebreaker/decompile/shape16.hpp -- interface of codebreaker.decompile.shape16 (typed C++; implementation pending).
//
// shape16.py -- a 16-bit function's statements in the form its instructions say they were written in.
//
//     auto [doc, said] = shape16::reshape(doc, code);
//
// The decompiler says what a statement does; the compiler's choice of instruction says how it was written, where
// C 8 makes different code of two forms with one meaning: `inc`/`dec` of a variable's place (x++ / x--) and an
// if-whose-then-part-returns followed by a return (the condition is turned round and the two returns traded).
// Each statement's instructions are the L2 origin "at". A pass on the function's L2 tree before the C back end
// writes it, after frame16 (decompiler.c_from_l2, CB_FRAME16=1).
//
// Typing notes:
//   * `doc` is the L2 document, translate::ir::Dict (the L2 tree: nested dicts/lists of no fixed key set, ir::Val values, as core16::L2Doc). reshape() works on a
//     deep copy and answers it; the caller's tree is not changed.
//   * `code` is the segment's bytes (indexed as code[a:a+8]).
//   * -> (the tree, what was done: [str])
#pragma once

#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::decompile::shape16 {

std::tuple<translate::ir::Dict, py::list<py::str>> reshape(const translate::ir::Dict& doc, const py::bytes& code);

}  // namespace codebreaker::decompile::shape16
