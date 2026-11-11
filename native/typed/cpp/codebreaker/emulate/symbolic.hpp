// codebreaker/emulate/symbolic.hpp -- SHIM over cb/emulate/symbolic.hpp (docs/CPP_DESIGN.md 6.3): the old Expr record
// (py::str op, shared sub-expressions) for the old shadow, the path explorer and native_shadow, whose constructors
// forward to the new ExprPool, one process-wide pool kept by this header. Deleted with its last py:: consumer
// (emulate/shadow, emulate/native_shadow, verify/concolic).
//
// Typing notes (the Python's tuple shapes, as before):
//       var      (op, bits, name)                  -> name
//       const    (op, bits, value)                 -> value
//       extract  (op, width, a, lo)                -> args = {a}, lo
//       others   (op, bits, sub-expressions...)    -> args
//   An Expr is shared and never changed; "is" (the identity the folding and the memos rely on) is pointer equality of
//   the ExprP. A constant's value is an int64_t holding the bit pattern (a 64-bit constant of 2**63 or more is
//   negative here). z3's main context (z3_of's ctx=None) is main_context().
//
// COMPAT(py): z3_of below builds the old consumers' z3++ terms (their solver contexts are z3++ contexts of the old
// build, which cb::z3's context cannot hand over); the same lowering is cb::emulate::symbolic::Z3Terms. Remove with
// verify/concolic.
#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#if __has_include(<z3++.h>)
#include <z3++.h>
#define CB_SYMBOLIC_SHIM_Z3PP 1   // (the old build has z3++; a machine without it cannot build the old consumers anyway)
#endif

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/symbolic.hpp"

namespace codebreaker::emulate::symbolic {

struct Expr;
using ExprP = std::shared_ptr<const Expr>;

struct Expr {
    py::str op;
    int64_t bits = 0;
    std::vector<ExprP> args;            // the sub-expressions, in order
    std::optional<py::str> name;        // var: the input's name
    int64_t value = 0;                  // const: its value
    int64_t lo = 0;                     // extract: the first bit taken
};

namespace shim {

namespace cbs = cb::emulate::symbolic;

struct State {
    std::mutex lock;
    cbs::ExprPool pool;
    std::unordered_map<const cbs::Expr*, ExprP> wrapped;                                // new node -> its old record
    std::unordered_map<const Expr*, std::pair<ExprP, const cbs::Expr*>> nodes;          // old record -> its new node
};
inline State& state() {
    static State s;
    return s;
}

inline const cbs::Expr* node_of(State& s, const ExprP& e);

// The old record of a node of the pool (made once, so identity is kept).
inline ExprP wrap(State& s, const cbs::Expr* n) {
    auto it = s.wrapped.find(n);
    if (it != s.wrapped.end()) return it->second;
    auto e = std::make_shared<Expr>();
    e->op = py::str(std::string(cbs::to_text(n->op)));
    e->bits = n->bits;
    if (n->op == cbs::Op::Var) {
        e->name = py::str(std::string(n->name));
    } else if (n->op == cbs::Op::Const) {
        e->value = static_cast<int64_t>(n->value);
    } else {
        for (const cbs::Expr* a : n->arg) {
            if (a != nullptr) e->args.push_back(wrap(s, a));
        }
        if (n->op == cbs::Op::Extract) e->lo = n->lo;
    }
    s.wrapped.emplace(n, e);
    s.nodes.emplace(e.get(), std::make_pair(ExprP(e), n));
    return e;
}

// The pool's node of an old record (one made by hand, not by a constructor here, is rebuilt in the pool).
inline const cbs::Expr* node_of(State& s, const ExprP& e) {
    auto it = s.nodes.find(e.get());
    if (it != s.nodes.end()) return it->second.second;
    const auto op = cbs::op_from_text(e->op.utf8());
    if (!op) throw py::ValueError(py::percent("no such expression: %r", e->op));
    const cbs::Expr* n = nullptr;
    auto kid = [&](size_t i) { return node_of(s, e->args.at(i)); };
    switch (*op) {
        case cbs::Op::Var: n = s.pool.var(e->name->utf8(), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::Const: n = s.pool.constant(static_cast<uint64_t>(e->value), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::True: n = s.pool.true_condition(); break;
        case cbs::Op::Extract: n = s.pool.extract(kid(0), static_cast<uint32_t>(e->lo), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::Ite: n = s.pool.ite(kid(0), kid(1), kid(2)); break;
        case cbs::Op::Neg: case cbs::Op::Not: n = s.pool.unary(*op, kid(0)); break;
        case cbs::Op::ZExt: n = s.pool.zext(kid(0), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::SExt: n = s.pool.sext(kid(0), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::Concat: n = s.pool.concat(kid(0), kid(1)); break;
        case cbs::Op::PopCount: n = s.pool.popcount(kid(0), static_cast<uint32_t>(e->bits)); break;
        case cbs::Op::Bool: n = s.pool.to_bool(kid(0)); break;
        case cbs::Op::Nz: n = s.pool.nonzero(kid(0)); break;
        case cbs::Op::BNot: n = s.pool.negate(kid(0)); break;
        case cbs::Op::BAnd: case cbs::Op::BOr: case cbs::Op::BXor: n = s.pool.logic(*op, kid(0), kid(1)); break;
        case cbs::Op::Eq: case cbs::Op::ULt: case cbs::Op::ULe: case cbs::Op::SLt: case cbs::Op::SLe:
            n = s.pool.compare(*op, kid(0), kid(1)); break;
        default: n = s.pool.binary(*op, kid(0), kid(1)); break;
    }
    s.nodes.emplace(e.get(), std::make_pair(e, n));   // (e kept: its address stays its own)
    s.wrapped.emplace(n, e);                           // and it is the node's record, so identity holds both ways
    return n;
}

}  // namespace shim

inline const ExprP TRUE = [] {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.true_condition());
}();

inline ExprP var(const py::str& name, int64_t bits) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.var(name.utf8(), static_cast<uint32_t>(bits)));
}
inline ExprP const_(const int64_t value, int64_t bits) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.constant(static_cast<uint64_t>(value), static_cast<uint32_t>(bits)));
}
inline int64_t bits(const ExprP& e) { return e->bits; }
inline ExprP binary(const py::str& op, const ExprP& a, const ExprP& b) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.binary(*shim::cbs::op_from_text(op.utf8()), shim::node_of(s, a), shim::node_of(s, b)));
}
inline ExprP unary(const py::str& op, const ExprP& a) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.unary(*shim::cbs::op_from_text(op.utf8()), shim::node_of(s, a)));
}
inline ExprP zext(const ExprP& a, int64_t to) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.zext(shim::node_of(s, a), static_cast<uint32_t>(to)));
}
inline ExprP sext(const ExprP& a, int64_t to) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.sext(shim::node_of(s, a), static_cast<uint32_t>(to)));
}
// `width` bits of `a` from bit `lo`.
inline ExprP extract(const ExprP& a, int64_t lo, int64_t width) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.extract(shim::node_of(s, a), static_cast<uint32_t>(lo), static_cast<uint32_t>(width)));
}
// hi:lo. Two neighbouring parts of one value: that part.
inline ExprP concat(const ExprP& hi, const ExprP& lo) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.concat(shim::node_of(s, hi), shim::node_of(s, lo)));
}
// A condition as a p-code boolean byte.
inline ExprP to_bool(const ExprP& c) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.to_bool(shim::node_of(s, c)));
}
// The condition that bit-vector `e` is not zero.
inline ExprP nonzero(const ExprP& e) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.nonzero(shim::node_of(s, e)));
}
// eq / ult / ule / slt / sle of two bit-vectors (a condition).
inline ExprP compare(const py::str& op, const ExprP& a, const ExprP& b) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.compare(*shim::cbs::op_from_text(op.utf8()), shim::node_of(s, a), shim::node_of(s, b)));
}
inline ExprP negate(const ExprP& c) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.negate(shim::node_of(s, c)));
}
inline ExprP logic(const py::str& op, const ExprP& x, const ExprP& y) {
    auto& s = shim::state();
    std::lock_guard<std::mutex> g(s.lock);
    return shim::wrap(s, s.pool.logic(*shim::cbs::op_from_text(op.utf8()), shim::node_of(s, x), shim::node_of(s, y)));
}

// The memo of inputs(): by identity, the expression (kept so its address stays its own) and its names.
using InputsMemo = std::unordered_map<const Expr*, std::pair<ExprP, py::set<py::str>>>;
// The names of the inputs expression `e` is over (a frozenset; `memo` by identity).
inline py::set<py::str> inputs(const ExprP& e, InputsMemo& memo) {
    auto got = memo.find(e.get());
    if (got != memo.end()) return got->second.second;
    py::set<py::str> out;
    if (e->op == py::str("var")) {
        out.add(*e->name);
    } else {
        for (const ExprP& x : e->args) {
            py::set<py::str> sub = inputs(x, memo);
            for (const py::str& n : sub) out.add(n);   // out |= inputs(x)
        }
    }
    memo.emplace(e.get(), std::make_pair(e, out));
    return out;
}

#ifdef CB_SYMBOLIC_SHIM_Z3PP
// The memo of z3_of(): by identity, the expression and its term. One memo is for one context.
using Z3Memo = std::unordered_map<const Expr*, std::pair<ExprP, z3::expr>>;
// z3's main context (what the Python's ctx=None means)
inline z3::context& main_context() {
    static z3::context ctx;
    return ctx;
}
// The z3 term of expression `e` (made once per node: `memo`, by identity), in z3 context `ctx`. Raises ValueError for
// an op that is no expression.
inline z3::expr z3_of(const ExprP& e, Z3Memo& memo, z3::context& ctx) {
    auto got = memo.find(e.get());
    if (got != memo.end()) return got->second.second;
    namespace cbs = shim::cbs;
    const auto op = cbs::op_from_text(e->op.utf8());
    if (!op) throw py::ValueError(py::percent("no such expression: %r", e->op));
    auto bv_val = [&](int64_t value, int64_t width) {
        return ctx.bv_val(std::to_string(static_cast<uint64_t>(value)).c_str(), static_cast<unsigned>(width));
    };
    std::optional<z3::expr> t;
    if (*op == cbs::Op::Var) {
        t = ctx.bv_const(e->name->utf8().c_str(), static_cast<unsigned>(e->bits));
    } else if (*op == cbs::Op::Const) {
        t = bv_val(e->value, e->bits);
    } else if (*op == cbs::Op::True) {
        t = ctx.bool_val(true);
    } else {
        std::vector<z3::expr> args;
        for (const ExprP& x : e->args) args.push_back(z3_of(x, memo, ctx));
        // z3py's `a == b`, `a < b`, `a <= b` with a numeral on the right and a non-numeral on the left go to the
        // numeral's own reflected method: the term is built as (= b a), (bvsgt b a), (bvsge b a). The same truth.
        bool swapped = e->args.size() == 2 && e->args[1]->op == py::str("const") && e->args[0]->op != py::str("const");
        switch (*op) {
            case cbs::Op::Add: t = args[0] + args[1]; break;
            case cbs::Op::Sub: t = args[0] - args[1]; break;
            case cbs::Op::And: t = args[0] & args[1]; break;
            case cbs::Op::Or: t = args[0] | args[1]; break;
            case cbs::Op::Xor: t = args[0] ^ args[1]; break;
            case cbs::Op::Mul: t = args[0] * args[1]; break;
            case cbs::Op::UDiv: t = z3::udiv(args[0], args[1]); break;
            case cbs::Op::URem: t = z3::urem(args[0], args[1]); break;
            case cbs::Op::SDiv: t = args[0] / args[1]; break;   // (a / b on bit-vectors: signed division)
            case cbs::Op::SRem: t = z3::srem(args[0], args[1]); break;
            case cbs::Op::Shl: t = z3::shl(args[0], args[1]); break;
            case cbs::Op::LShr: t = z3::lshr(args[0], args[1]); break;
            case cbs::Op::AShr: t = z3::ashr(args[0], args[1]); break;
            case cbs::Op::Neg: t = -args[0]; break;
            case cbs::Op::Not: t = ~args[0]; break;
            case cbs::Op::ZExt: t = z3::zext(args[0], static_cast<unsigned>(e->bits - e->args[0]->bits)); break;
            case cbs::Op::SExt: t = z3::sext(args[0], static_cast<unsigned>(e->bits - e->args[0]->bits)); break;
            case cbs::Op::Extract: t = args[0].extract(static_cast<unsigned>(e->lo + e->bits - 1), static_cast<unsigned>(e->lo)); break;
            case cbs::Op::Concat: t = z3::concat(args[0], args[1]); break;
            case cbs::Op::Ite: t = z3::ite(args[0], args[1], args[2]); break;
            case cbs::Op::Bool: t = z3::ite(args[0], bv_val(1, 8), bv_val(0, 8)); break;
            case cbs::Op::PopCount: {
                z3::expr acc = bv_val(0, e->bits);
                for (int64_t k = 0; k < e->args[0]->bits; ++k) {
                    acc = acc + z3::zext(args[0].extract(static_cast<unsigned>(k), static_cast<unsigned>(k)), static_cast<unsigned>(e->bits - 1));
                }
                t = acc;
                break;
            }
            case cbs::Op::Eq: t = swapped ? args[1] == args[0] : args[0] == args[1]; break;
            case cbs::Op::ULt: t = z3::ult(args[0], args[1]); break;
            case cbs::Op::ULe: t = z3::ule(args[0], args[1]); break;
            case cbs::Op::SLt: t = swapped ? args[1] > args[0] : args[0] < args[1]; break;     // (< on bit-vectors: signed)
            case cbs::Op::SLe: t = swapped ? args[1] >= args[0] : args[0] <= args[1]; break;
            case cbs::Op::BNot: t = !args[0]; break;
            case cbs::Op::BAnd: case cbs::Op::BOr: {
                z3::expr_vector vec(ctx);
                for (const z3::expr& x : args) vec.push_back(x);
                t = *op == cbs::Op::BAnd ? z3::mk_and(vec) : z3::mk_or(vec);
                break;
            }
            case cbs::Op::BXor: t = args[0] ^ args[1]; break;
            case cbs::Op::Nz: t = args[0] != ctx.bv_val(0, static_cast<unsigned>(e->args[0]->bits)); break;
            default: throw py::ValueError(py::percent("no such expression: %r", e->op));
        }
    }
    memo.emplace(e.get(), std::make_pair(e, *t));   // (e kept: its address stays its own)
    return *t;
}
inline z3::expr z3_of(const ExprP& e, Z3Memo& memo) { return z3_of(e, memo, main_context()); }
#endif  // CB_SYMBOLIC_SHIM_Z3PP

}  // namespace codebreaker::emulate::symbolic
