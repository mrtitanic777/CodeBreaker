// cb/emulate/symbolic.hpp -- expressions over a run's inputs, made cheaply and handed to the solver only when asked.
//
// The shadow of a run (the path explorer's) makes an expression for every value an instruction derives from an input
// -- a compare's six flags, most of which no jump ever reads. Built as solver terms they cost the run more than the
// run itself, so an expression here is a plain node, and `Z3Terms` makes the solver's term of only those a path's
// conditions use (once each).
//
//     a bit-vector   bits > 0     Var Const Add Sub And Or Xor Mul UDiv URem SDiv SRem Neg Not Shl LShr AShr ZExt SExt
//                                 Extract Concat Ite PopCount Bool
//     a condition    bits == 0    Eq ULt ULe SLt SLe BNot BAnd BOr BXor Nz True
//
// `Bool` is a p-code boolean: a byte that is 1 where its condition holds, else 0; `Nz` is the condition that a
// bit-vector is not zero. The constructors fold what is plain (a condition tested for being 1 is that condition) so
// that the conditions stay the size of what was compared.
//
// Nodes live in an `ExprPool` and are immutable; a pointer is the node's identity (the folding relies on it: two
// extracts of the same value are of the same pointer). A pool is for one run on one thread, and its nodes must not
// outlive it.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string_view>
#include <unordered_map>

#include "cb/core/arena.hpp"

struct _Z3_context;   // Z3's handles (z3.h), declared here so the header stays light
struct _Z3_ast;

namespace cb::emulate::symbolic {

enum class Op : std::uint8_t {
    // bit-vectors
    Var, Const, Add, Sub, And, Or, Xor, Mul, UDiv, URem, SDiv, SRem, Neg, Not, Shl, LShr, AShr, ZExt, SExt, Extract,
    Concat, Ite, PopCount, Bool,
    // conditions
    Eq, ULt, ULe, SLt, SLe, BNot, BAnd, BOr, BXor, Nz, True,
};

std::string_view to_text(Op op);                  // "var", "const", "add" ... "nz", "true" (diagnostics, files)
std::optional<Op> op_from_text(std::string_view text);   // the inverse; absent for a word that is no op
bool is_condition(Op op);

struct Expr {
    Op op = Op::True;
    std::uint32_t bits = 0;                        // 0 for a condition
    const Expr* arg[3] = {nullptr, nullptr, nullptr};   // the sub-expressions in order (an Ite has three)
    std::uint64_t value = 0;                       // Const: its value, masked to `bits`
    std::uint32_t lo = 0;                          // Extract: the first bit taken
    std::string_view name;                         // Var: the input's name (kept by the pool)
};

// Widest bit-vector a node can be: constants are held in 64 bits.
inline constexpr std::uint32_t kMaxConstBits = 64;

class ExprPool {
public:
    const Expr* var(std::string_view name, std::uint32_t bits);
    const Expr* constant(std::uint64_t value, std::uint32_t bits);   // value & ((1 << bits) - 1); bits 1..64
    const Expr* true_condition();

    // `op` of two bit-vectors, as wide as `a`: Add Sub And Or Xor Mul UDiv URem SDiv SRem Shl LShr AShr.
    const Expr* binary(Op op, const Expr* a, const Expr* b);
    // Neg or Not of one bit-vector.
    const Expr* unary(Op op, const Expr* a);
    const Expr* zext(const Expr* a, std::uint32_t to);
    const Expr* sext(const Expr* a, std::uint32_t to);
    // `width` bits of `a` from bit `lo`.
    const Expr* extract(const Expr* a, std::uint32_t lo, std::uint32_t width);
    // hi:lo. Two neighbouring parts of one value: that part.
    const Expr* concat(const Expr* hi, const Expr* lo);
    const Expr* ite(const Expr* condition, const Expr* then_value, const Expr* else_value);
    const Expr* popcount(const Expr* a, std::uint32_t result_bits);
    // A condition as a p-code boolean byte.
    const Expr* to_bool(const Expr* condition);
    // The condition that bit-vector `e` is not zero.
    const Expr* nonzero(const Expr* e);
    // Eq ULt ULe SLt SLe of two bit-vectors (a condition).
    const Expr* compare(Op op, const Expr* a, const Expr* b);
    const Expr* negate(const Expr* condition);
    // BAnd BOr BXor of two conditions.
    const Expr* logic(Op op, const Expr* x, const Expr* y);

    std::size_t size() const { return nodes_; }

private:
    const Expr* make(Op op, std::uint32_t bits, const Expr* a = nullptr, const Expr* b = nullptr,
                     const Expr* c = nullptr);
    Arena arena_;
    std::size_t nodes_ = 0;
    const Expr* true_ = nullptr;
};

// The names of the inputs expression `e` is over, sorted. `memo` keeps the answer per node (by identity), so a
// shared sub-expression is walked once; one memo is for one pool.
using InputsMemo = std::unordered_map<const Expr*, std::set<std::string_view>>;
const std::set<std::string_view>& inputs(const Expr* e, InputsMemo& memo);

// A Z3 context made for the terms, deleted with this object.
class Z3Context {
public:
    Z3Context();
    Z3Context(const Z3Context&) = delete;
    Z3Context& operator=(const Z3Context&) = delete;
    ~Z3Context();
    _Z3_context* handle() const { return ctx_; }

private:
    _Z3_context* ctx_;
};

// The solver terms of expressions in one Z3 context, each made once (by node identity). The terms belong to the
// context, which the caller keeps alive for as long as it uses them. The context is made without reference counting
// (Z3_mk_context): its terms live until it is deleted.
class Z3Terms {
public:
    explicit Z3Terms(_Z3_context* ctx) : ctx_(ctx) {}
    // The term of `e`. A node that is not an expression of consistent widths is an InternalError.
    _Z3_ast* term(const Expr* e);

private:
    _Z3_context* ctx_;
    std::unordered_map<const Expr*, _Z3_ast*> memo_;
};

}  // namespace cb::emulate::symbolic
