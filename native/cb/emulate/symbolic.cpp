// cb/emulate/symbolic.cpp -- see symbolic.hpp.
#include "cb/emulate/symbolic.hpp"

#include <format>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/z3_api.hpp"

namespace cb::emulate::symbolic {

std::string_view to_text(Op op) {
    switch (op) {
        case Op::Var: return "var";
        case Op::Const: return "const";
        case Op::Add: return "add";
        case Op::Sub: return "sub";
        case Op::And: return "and";
        case Op::Or: return "or";
        case Op::Xor: return "xor";
        case Op::Mul: return "mul";
        case Op::UDiv: return "udiv";
        case Op::URem: return "urem";
        case Op::SDiv: return "sdiv";
        case Op::SRem: return "srem";
        case Op::Neg: return "neg";
        case Op::Not: return "not";
        case Op::Shl: return "shl";
        case Op::LShr: return "lshr";
        case Op::AShr: return "ashr";
        case Op::ZExt: return "zext";
        case Op::SExt: return "sext";
        case Op::Extract: return "extract";
        case Op::Concat: return "concat";
        case Op::Ite: return "ite";
        case Op::PopCount: return "popcount";
        case Op::Bool: return "bool";
        case Op::Eq: return "eq";
        case Op::ULt: return "ult";
        case Op::ULe: return "ule";
        case Op::SLt: return "slt";
        case Op::SLe: return "sle";
        case Op::BNot: return "bnot";
        case Op::BAnd: return "band";
        case Op::BOr: return "bor";
        case Op::BXor: return "bxor";
        case Op::Nz: return "nz";
        case Op::True: return "true";
    }
    return "?";
}

std::optional<Op> op_from_text(std::string_view text) {
    for (int i = static_cast<int>(Op::Var); i <= static_cast<int>(Op::True); ++i) {
        if (to_text(static_cast<Op>(i)) == text) return static_cast<Op>(i);
    }
    return std::nullopt;
}

bool is_condition(Op op) { return op >= Op::Eq; }

// ---- the pool -----------------------------------------------------------------------------------------------

const Expr* ExprPool::make(Op op, std::uint32_t bits, const Expr* a, const Expr* b, const Expr* c) {
    Expr* e = arena_.make<Expr>();
    e->op = op;
    e->bits = bits;
    e->arg[0] = a;
    e->arg[1] = b;
    e->arg[2] = c;
    ++nodes_;
    return e;
}

const Expr* ExprPool::var(std::string_view name, std::uint32_t bits) {
    CB_ASSERT(bits >= 1, "a variable has at least one bit");
    Expr* e = const_cast<Expr*>(make(Op::Var, bits));
    e->name = arena_.intern(name);
    return e;
}

const Expr* ExprPool::constant(std::uint64_t value, std::uint32_t bits) {
    if (bits < 1 || bits > kMaxConstBits) {
        throw UsageError(std::format("a constant of {} bits (1..{} are held)", bits, kMaxConstBits));
    }
    Expr* e = const_cast<Expr*>(make(Op::Const, bits));
    e->value = bits == 64 ? value : value & ((std::uint64_t{1} << bits) - 1);
    return e;
}

const Expr* ExprPool::true_condition() {
    if (true_ == nullptr) true_ = make(Op::True, 0);
    return true_;
}

const Expr* ExprPool::binary(Op op, const Expr* a, const Expr* b) {
    CB_ASSERT((op >= Op::Add && op <= Op::SRem) || (op >= Op::Shl && op <= Op::AShr), "not a binary bit-vector op");
    return make(op, a->bits, a, b);
}

const Expr* ExprPool::unary(Op op, const Expr* a) {
    CB_ASSERT(op == Op::Neg || op == Op::Not, "not a unary bit-vector op");
    return make(op, a->bits, a);
}

const Expr* ExprPool::zext(const Expr* a, std::uint32_t to) { return a->bits == to ? a : make(Op::ZExt, to, a); }

const Expr* ExprPool::sext(const Expr* a, std::uint32_t to) { return a->bits == to ? a : make(Op::SExt, to, a); }

const Expr* ExprPool::extract(const Expr* a, std::uint32_t lo, std::uint32_t width) {
    if (lo == 0 && width == a->bits) return a;
    if (a->op == Op::Concat) {   // (a part of a part: that part)
        const Expr* hi_part = a->arg[0];
        const Expr* lo_part = a->arg[1];
        if (lo >= lo_part->bits) return extract(hi_part, lo - lo_part->bits, width);
        if (lo + width <= lo_part->bits) return extract(lo_part, lo, width);
    }
    if (a->op == Op::ZExt && lo + width <= a->arg[0]->bits) return extract(a->arg[0], lo, width);
    if (a->op == Op::Extract) return extract(a->arg[0], a->lo + lo, width);
    Expr* e = const_cast<Expr*>(make(Op::Extract, width, a));
    e->lo = lo;
    return e;
}

const Expr* ExprPool::concat(const Expr* hi, const Expr* lo) {
    if (hi->op == Op::Extract && lo->op == Op::Extract && hi->arg[0] == lo->arg[0] && hi->lo == lo->lo + lo->bits) {
        return extract(hi->arg[0], lo->lo, hi->bits + lo->bits);
    }
    // Two constants make one while it still fits a constant; wider, the concatenation stays a node (the same value).
    if (hi->op == Op::Const && lo->op == Op::Const && hi->bits + lo->bits <= kMaxConstBits) {
        return constant((hi->value << lo->bits) | lo->value, hi->bits + lo->bits);
    }
    return make(Op::Concat, hi->bits + lo->bits, hi, lo);
}

const Expr* ExprPool::ite(const Expr* condition, const Expr* then_value, const Expr* else_value) {
    return make(Op::Ite, then_value->bits, condition, then_value, else_value);
}

const Expr* ExprPool::popcount(const Expr* a, std::uint32_t result_bits) { return make(Op::PopCount, result_bits, a); }

const Expr* ExprPool::to_bool(const Expr* condition) { return make(Op::Bool, 8, condition); }

const Expr* ExprPool::nonzero(const Expr* e) {
    if (e->op == Op::Bool) return e->arg[0];
    if (e->op == Op::ZExt && e->arg[0]->op == Op::Bool) return e->arg[0]->arg[0];
    return make(Op::Nz, 0, e);
}

const Expr* ExprPool::compare(Op op, const Expr* a, const Expr* b) {
    if (op == Op::Eq) {
        // (a boolean byte against 0 or 1)
        const Expr* pairs[2][2] = {{a, b}, {b, a}};
        for (auto& p : pairs) {
            const Expr* x = p[0];
            const Expr* y = p[1];
            if (x->op == Op::Bool && y->op == Op::Const) {
                if (y->value == 1) return x->arg[0];
                if (y->value == 0) return negate(x->arg[0]);
                return make(Op::Eq, 0, x, y);
            }
        }
    }
    return make(op, 0, a, b);
}

const Expr* ExprPool::negate(const Expr* condition) {
    if (condition->op == Op::BNot) return condition->arg[0];
    return make(Op::BNot, 0, condition);
}

const Expr* ExprPool::logic(Op op, const Expr* x, const Expr* y) {
    CB_ASSERT(op == Op::BAnd || op == Op::BOr || op == Op::BXor, "not a condition connective");
    return make(op, 0, x, y);
}

// ---- inputs --------------------------------------------------------------------------------------------------

const std::set<std::string_view>& inputs(const Expr* root, InputsMemo& memo) {
    // Post-order without recursion: a long chain of conditions can be far deeper than the stack allows.
    std::vector<std::pair<const Expr*, bool>> stack{{root, false}};
    while (!stack.empty()) {
        auto [e, expanded] = stack.back();
        stack.pop_back();
        if (memo.contains(e)) continue;
        if (!expanded) {
            stack.emplace_back(e, true);
            for (const Expr* a : e->arg) {
                if (a != nullptr && !memo.contains(a)) stack.emplace_back(a, false);
            }
            continue;
        }
        std::set<std::string_view> out;
        if (e->op == Op::Var) {
            out.insert(e->name);
        } else {
            for (const Expr* a : e->arg) {
                if (a != nullptr) out.insert(memo.at(a).begin(), memo.at(a).end());
            }
        }
        memo.emplace(e, std::move(out));
    }
    return memo.at(root);
}

// ---- Z3 ------------------------------------------------------------------------------------------------------

Z3Context::Z3Context() {
    const auto& z = z3::api();
    Z3_config cfg = z.Z3_mk_config();
    ctx_ = z.Z3_mk_context(cfg);
    z.Z3_del_config(cfg);
}

Z3Context::~Z3Context() { z3::api().Z3_del_context(ctx_); }

namespace {

void same_width(const Expr* e, const Expr* a, const Expr* b) {
    CB_ASSERT(a->bits == b->bits, std::format("{}: operands of {} and {} bits", to_text(e->op), a->bits, b->bits));
}

}  // namespace

_Z3_ast* Z3Terms::term(const Expr* root) {
    const auto& z = z3::api();
    Z3_context c = ctx_;
    auto bv_value = [&](std::uint64_t value, std::uint32_t bits) {
        return z.Z3_mk_unsigned_int64(c, value, z.Z3_mk_bv_sort(c, bits));
    };
    auto built = [&](const Expr* e) -> Z3_ast { return memo_.at(e); };

    std::vector<std::pair<const Expr*, bool>> stack{{root, false}};
    while (!stack.empty()) {
        auto [e, expanded] = stack.back();
        stack.pop_back();
        if (memo_.contains(e)) continue;
        if (!expanded) {
            stack.emplace_back(e, true);
            for (const Expr* a : e->arg) {
                if (a != nullptr && !memo_.contains(a)) stack.emplace_back(a, false);
            }
            continue;
        }
        const Expr* a = e->arg[0];
        const Expr* b = e->arg[1];
        const Expr* d = e->arg[2];
        Z3_ast t = nullptr;
        switch (e->op) {
            case Op::Var: {
                const std::string name(e->name);
                t = z.Z3_mk_const(c, z.Z3_mk_string_symbol(c, name.c_str()), z.Z3_mk_bv_sort(c, e->bits));
                break;
            }
            case Op::Const: t = bv_value(e->value, e->bits); break;
            case Op::True: t = z.Z3_mk_true(c); break;
            case Op::Add: same_width(e, a, b); t = z.Z3_mk_bvadd(c, built(a), built(b)); break;
            case Op::Sub: same_width(e, a, b); t = z.Z3_mk_bvsub(c, built(a), built(b)); break;
            case Op::And: same_width(e, a, b); t = z.Z3_mk_bvand(c, built(a), built(b)); break;
            case Op::Or: same_width(e, a, b); t = z.Z3_mk_bvor(c, built(a), built(b)); break;
            case Op::Xor: same_width(e, a, b); t = z.Z3_mk_bvxor(c, built(a), built(b)); break;
            case Op::Mul: same_width(e, a, b); t = z.Z3_mk_bvmul(c, built(a), built(b)); break;
            case Op::UDiv: same_width(e, a, b); t = z.Z3_mk_bvudiv(c, built(a), built(b)); break;
            case Op::URem: same_width(e, a, b); t = z.Z3_mk_bvurem(c, built(a), built(b)); break;
            case Op::SDiv: same_width(e, a, b); t = z.Z3_mk_bvsdiv(c, built(a), built(b)); break;
            case Op::SRem: same_width(e, a, b); t = z.Z3_mk_bvsrem(c, built(a), built(b)); break;
            case Op::Shl: same_width(e, a, b); t = z.Z3_mk_bvshl(c, built(a), built(b)); break;
            case Op::LShr: same_width(e, a, b); t = z.Z3_mk_bvlshr(c, built(a), built(b)); break;
            case Op::AShr: same_width(e, a, b); t = z.Z3_mk_bvashr(c, built(a), built(b)); break;
            case Op::Neg: t = z.Z3_mk_bvneg(c, built(a)); break;
            case Op::Not: t = z.Z3_mk_bvnot(c, built(a)); break;
            case Op::ZExt:
                CB_ASSERT(e->bits >= a->bits, "zext to fewer bits");
                t = z.Z3_mk_zero_ext(c, e->bits - a->bits, built(a));
                break;
            case Op::SExt:
                CB_ASSERT(e->bits >= a->bits, "sext to fewer bits");
                t = z.Z3_mk_sign_ext(c, e->bits - a->bits, built(a));
                break;
            case Op::Extract:
                CB_ASSERT(e->lo + e->bits <= a->bits, "extract past the end");
                t = z.Z3_mk_extract(c, e->lo + e->bits - 1, e->lo, built(a));
                break;
            case Op::Concat: t = z.Z3_mk_concat(c, built(a), built(b)); break;
            case Op::Ite: t = z.Z3_mk_ite(c, built(a), built(b), built(d)); break;
            case Op::Bool: t = z.Z3_mk_ite(c, built(a), bv_value(1, 8), bv_value(0, 8)); break;
            case Op::PopCount: {
                // the sum of the bits, each widened to the result
                t = bv_value(0, e->bits);
                for (std::uint32_t k = 0; k < a->bits; ++k) {
                    Z3_ast bit = z.Z3_mk_extract(c, k, k, built(a));
                    t = z.Z3_mk_bvadd(c, t, e->bits > 1 ? z.Z3_mk_zero_ext(c, e->bits - 1, bit) : bit);
                }
                break;
            }
            case Op::Eq: same_width(e, a, b); t = z.Z3_mk_eq(c, built(a), built(b)); break;
            case Op::ULt: same_width(e, a, b); t = z.Z3_mk_bvult(c, built(a), built(b)); break;
            case Op::ULe: same_width(e, a, b); t = z.Z3_mk_bvule(c, built(a), built(b)); break;
            case Op::SLt: same_width(e, a, b); t = z.Z3_mk_bvslt(c, built(a), built(b)); break;
            case Op::SLe: same_width(e, a, b); t = z.Z3_mk_bvsle(c, built(a), built(b)); break;
            case Op::BNot: t = z.Z3_mk_not(c, built(a)); break;
            case Op::BAnd: {
                Z3_ast both[2] = {built(a), built(b)};
                t = z.Z3_mk_and(c, 2, both);
                break;
            }
            case Op::BOr: {
                Z3_ast both[2] = {built(a), built(b)};
                t = z.Z3_mk_or(c, 2, both);
                break;
            }
            case Op::BXor: t = z.Z3_mk_xor(c, built(a), built(b)); break;
            case Op::Nz: t = z.Z3_mk_not(c, z.Z3_mk_eq(c, built(a), bv_value(0, a->bits))); break;
        }
        CB_ASSERT(t != nullptr, std::format("no term for {}", to_text(e->op)));
        memo_.emplace(e, t);
    }
    return memo_.at(root);
}

}  // namespace cb::emulate::symbolic
