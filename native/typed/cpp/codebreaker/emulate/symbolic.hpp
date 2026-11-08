// codebreaker/emulate/symbolic.hpp -- C++17 port of codebreaker.emulate.symbolic.
//
// symbolic.py -- expressions over a run's inputs, made cheaply and handed to the solver only when
// asked.
//
// The shadow of a run (emulate/shadow.py) makes an expression for every value an instruction
// derives from an input -- a compare's six flags, most of which no jump ever reads. Built as z3
// terms they cost the run more than the run itself, so here an expression is a plain tuple, and
// z3() makes the solver's term of only those a path's conditions use (once each).
//
//     a bit-vector   (op, bits, ...)        op: var const add sub and or xor mul udiv urem sdiv
//                                           srem neg not shl lshr ashr zext sext extract concat
//                                           ite popcount bool
//     a condition    (op, 0, ...)           op: eq ult ule slt sle bnot band bor bxor nz true
//
// `bool` is a p-code boolean: a byte that is 1 where its condition holds, else 0; `nz` is the
// condition that a bit-vector is not zero. The constructors fold what is plain (a condition
// tested for being 1 is that condition) so that the conditions stay the size of what was
// compared.
//
// Typing notes:
//   An expression is a Python tuple of several shapes (its op decides the rest). Here it is the Expr
//   record: op, bits and then, by op,
//       var      (op, bits, name)                  -> name
//       const    (op, bits, value)                 -> value
//       extract  (op, width, a, lo)                -> args = {a}, lo
//       others   (op, bits, sub-expressions...)    -> args
//   An Expr is shared and never changed, as a tuple is: "is" (the identity the folding and the memos
//   rely on) is pointer equality of the ExprP.
//   A constant's value is an int64_t: the bits up to 64 are kept as they lie (a 64-bit constant of
//   2**63 or more is held as its two's-complement pattern, which Python's unbounded int would show as
//   a positive number); constants wider than 64 bits and a concat of constants wider than 64 bits are
//   outside int64 (the latter raises OverflowError).
//   z3's main context (z3_of's ctx=None) is main_context().
#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <z3++.h>

#include "py.hpp"

namespace codebreaker::emulate::symbolic {

struct Expr;
using ExprP = std::shared_ptr<const Expr>;

struct Expr {
    py::str op;
    int64_t bits = 0;
    std::vector<ExprP> args;            // the sub-expressions, in order (a tuple's tuple items)
    std::optional<py::str> name;        // var: the input's name
    int64_t value = 0;                  // const: its value
    int64_t lo = 0;                     // extract: the first bit taken
};

extern const ExprP TRUE;

ExprP var(const py::str& name, int64_t bits);
ExprP const_(const int64_t value, int64_t bits);   // (`const` is a C++ keyword)
int64_t bits(const ExprP& e);
ExprP binary(const py::str& op, const ExprP& a, const ExprP& b);
ExprP unary(const py::str& op, const ExprP& a);
ExprP zext(const ExprP& a, int64_t to);
ExprP sext(const ExprP& a, int64_t to);
// `width` bits of `a` from bit `lo`.
ExprP extract(const ExprP& a, int64_t lo, int64_t width);
// hi:lo. Two neighbouring parts of one value: that part.
ExprP concat(const ExprP& hi, const ExprP& lo);
// A condition as a p-code boolean byte.
ExprP to_bool(const ExprP& c);
// The condition that bit-vector `e` is not zero.
ExprP nonzero(const ExprP& e);
// eq / ult / ule / slt / sle of two bit-vectors (a condition).
ExprP compare(const py::str& op, const ExprP& a, const ExprP& b);
ExprP negate(const ExprP& c);
ExprP logic(const py::str& op, const ExprP& x, const ExprP& y);

// The memo of inputs(): by identity, the expression (kept so its address stays its own) and its names.
using InputsMemo = std::unordered_map<const Expr*, std::pair<ExprP, py::set<py::str>>>;
// The names of the inputs expression `e` is over (a frozenset; `memo` by identity).
py::set<py::str> inputs(const ExprP& e, InputsMemo& memo);

// The memo of z3_of(): by identity, the expression and its term. One memo is for one context.
using Z3Memo = std::unordered_map<const Expr*, std::pair<ExprP, z3::expr>>;
// z3's main context (what the Python's ctx=None means)
z3::context& main_context();
// The z3 term of expression `e` (made once per node: `memo`, by identity), in z3 context `ctx`.
// Raises ValueError for an op that is no expression.
z3::expr z3_of(const ExprP& e, Z3Memo& memo, z3::context& ctx);
z3::expr z3_of(const ExprP& e, Z3Memo& memo);   // ctx=None: z3's main context

}  // namespace codebreaker::emulate::symbolic
