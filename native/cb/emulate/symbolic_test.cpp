// cb/emulate/symbolic_test.cpp -- the expression pool's folding rules, the inputs of an expression, and the Z3 terms
// checked by asking the solver real questions.
#include "cb/emulate/symbolic.hpp"

#include <optional>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/z3_api.hpp"

using namespace cb;
using namespace cb::emulate::symbolic;

namespace {

// Whether the conditions can all hold at once (Z3 says sat), and the value of `var` in the model when they can.
struct Answer {
    bool sat = false;
    std::optional<std::uint64_t> value;
};

Answer solve(const std::vector<const Expr*>& conditions, const Expr* wanted = nullptr) {
    const auto& z = z3::api();
    Z3Context context;
    Z3Terms terms(context.handle());
    Z3_solver solver = z.Z3_mk_solver(context.handle());
    z.Z3_solver_inc_ref(context.handle(), solver);
    for (const Expr* c : conditions) z.Z3_solver_assert(context.handle(), solver, terms.term(c));
    Answer a;
    a.sat = z.Z3_solver_check(context.handle(), solver) == Z3_L_TRUE;
    if (a.sat && wanted != nullptr) {
        Z3_model model = z.Z3_solver_get_model(context.handle(), solver);
        z.Z3_model_inc_ref(context.handle(), model);
        Z3_ast value = nullptr;
        if (z.Z3_model_eval(context.handle(), model, terms.term(wanted), true, &value)) {
            std::uint64_t v = 0;
            if (z.Z3_get_numeral_uint64(context.handle(), value, &v)) a.value = v;
        }
        z.Z3_model_dec_ref(context.handle(), model);
    }
    z.Z3_solver_dec_ref(context.handle(), solver);
    return a;
}

}  // namespace

CB_TEST("emulate.symbolic.constants_are_masked_to_their_width", [] {
    ExprPool p;
    CB_CHECK_EQ(p.constant(0x1FF, 8)->value, 0xFFu);
    CB_CHECK_EQ(p.constant(~std::uint64_t{0}, 64)->value, ~std::uint64_t{0});
    CB_CHECK_EQ(p.constant(5, 3)->value, 5u);
    CB_CHECK_EQ(p.constant(8, 3)->value, 0u);
    CB_CHECK_THROWS_AS(UsageError, (void)p.constant(1, 65));    // held in 64 bits
    CB_CHECK_THROWS_AS(UsageError, (void)p.constant(1, 0));
});

CB_TEST("emulate.symbolic.widths_follow_the_left_operand", [] {
    ExprPool p;
    const Expr* x = p.var("x", 32);
    const Expr* y = p.var("y", 32);
    CB_CHECK_EQ(p.binary(Op::Add, x, y)->bits, 32u);
    CB_CHECK_EQ(p.unary(Op::Neg, x)->bits, 32u);
    CB_CHECK_EQ(p.compare(Op::ULt, x, y)->bits, 0u);              // a condition has no width
    CB_CHECK(is_condition(Op::ULt) && !is_condition(Op::Add));
    CB_CHECK_EQ(p.to_bool(p.compare(Op::Eq, x, y))->bits, 8u);
    CB_CHECK_EQ(p.zext(x, 64)->bits, 64u);
    CB_CHECK(p.zext(x, 32) == x);                                 // no change: the same node
    CB_CHECK(p.sext(x, 32) == x);
    CB_CHECK_EQ(std::string(to_text(Op::URem)), "urem");
    for (int i = static_cast<int>(Op::Var); i <= static_cast<int>(Op::True); ++i) {
        CB_CHECK(op_from_text(to_text(static_cast<Op>(i))) == static_cast<Op>(i));   // every word names its op back
    }
    CB_CHECK(!op_from_text("frobnicate").has_value());
    CB_CHECK_THROWS_AS(InternalError, (void)p.binary(Op::Neg, x, y));
    CB_CHECK_THROWS_AS(InternalError, (void)p.unary(Op::Add, x));
});

CB_TEST("emulate.symbolic.extract_folds_through_what_it_takes_from", [] {
    ExprPool p;
    const Expr* x = p.var("x", 32);
    CB_CHECK(p.extract(x, 0, 32) == x);                           // the whole value
    const Expr* low16 = p.extract(x, 0, 16);
    CB_CHECK(low16->op == Op::Extract && low16->arg[0] == x && low16->lo == 0 && low16->bits == 16);
    const Expr* byte1 = p.extract(low16, 8, 8);                    // a part of a part: straight from x
    CB_CHECK(byte1->op == Op::Extract && byte1->arg[0] == x && byte1->lo == 8);
    // a part of a zero extension that lies in the original
    const Expr* wide = p.zext(low16, 32);
    const Expr* back = p.extract(wide, 4, 8);
    CB_CHECK(back->op == Op::Extract && back->arg[0] == x && back->lo == 4);
    // a part of a concatenation that lies in one side
    const Expr* y = p.var("y", 16);
    const Expr* both = p.concat(y, low16);                         // y:low16
    CB_CHECK(p.extract(both, 16, 16) == y);                       // exactly the high part
    const Expr* in_low = p.extract(both, 2, 4);
    CB_CHECK(in_low->arg[0] == x && in_low->lo == 2);
    const Expr* across = p.extract(both, 12, 8);                   // straddles: stays an extract of the concat
    CB_CHECK(across->op == Op::Extract && across->arg[0] == both);
});

CB_TEST("emulate.symbolic.concat_of_neighbours_is_the_whole", [] {
    ExprPool p;
    const Expr* x = p.var("x", 32);
    const Expr* lo = p.extract(x, 0, 16);
    const Expr* hi = p.extract(x, 16, 16);
    CB_CHECK(p.concat(hi, lo) == x);                              // x[31:16]:x[15:0] is x
    const Expr* gap = p.extract(x, 24, 8);
    const Expr* joined = p.concat(gap, lo);                        // not neighbours: a concat
    CB_CHECK(joined->op == Op::Concat && joined->bits == 24);
    const Expr* c = p.concat(p.constant(0x12, 8), p.constant(0x34, 8));
    CB_CHECK(c->op == Op::Const && c->bits == 16 && c->value == 0x1234);
    // DIVERGES: two constants whose width passes 64 bits stay a concatenation (the Python made one wider constant)
    const Expr* wide = p.concat(p.constant(1, 40), p.constant(2, 40));
    CB_CHECK(wide->op == Op::Concat && wide->bits == 80);
});

CB_TEST("emulate.symbolic.conditions_on_boolean_bytes_fold", [] {
    ExprPool p;
    const Expr* x = p.var("x", 32);
    const Expr* y = p.var("y", 32);
    const Expr* cond = p.compare(Op::ULt, x, y);
    const Expr* flag = p.to_bool(cond);                            // the byte that is 1 where x < y
    CB_CHECK(p.nonzero(flag) == cond);                            // nz(bool(c)) is c
    CB_CHECK(p.nonzero(p.zext(flag, 32)) == cond);                // ... also through a zero extension
    const Expr* ne = p.nonzero(x);
    CB_CHECK(ne->op == Op::Nz && ne->arg[0] == x);
    CB_CHECK(p.compare(Op::Eq, flag, p.constant(1, 8)) == cond);  // bool == 1 is the condition
    CB_CHECK(p.compare(Op::Eq, p.constant(1, 8), flag) == cond);  // either way round
    const Expr* is_zero = p.compare(Op::Eq, flag, p.constant(0, 8));
    CB_CHECK(is_zero->op == Op::BNot && is_zero->arg[0] == cond);  // bool == 0 is its negation
    const Expr* other = p.compare(Op::Eq, flag, p.constant(7, 8));
    CB_CHECK(other->op == Op::Eq);                                 // nothing to fold
    CB_CHECK(p.negate(p.negate(cond)) == cond);                   // not not c
    CB_CHECK_EQ(p.compare(Op::Eq, x, y)->op, Op::Eq);
    CB_CHECK_EQ(p.logic(Op::BAnd, cond, ne)->op, Op::BAnd);
    CB_CHECK_THROWS_AS(InternalError, (void)p.logic(Op::Add, cond, ne));
    CB_CHECK(p.true_condition() == p.true_condition());           // one TRUE
});

CB_TEST("emulate.symbolic.inputs_are_the_variables_an_expression_is_over", [] {
    ExprPool p;
    const Expr* a = p.var("a", 32);
    const Expr* b = p.var("b", 32);
    const Expr* sum = p.binary(Op::Add, a, p.constant(4, 32));
    const Expr* cond = p.compare(Op::ULt, sum, b);
    InputsMemo memo;
    const auto& names = inputs(cond, memo);
    CB_CHECK(names == (std::set<std::string_view>{"a", "b"}));
    CB_CHECK(inputs(sum, memo) == (std::set<std::string_view>{"a"}));          // the memo answers sub-expressions too
    CB_CHECK(inputs(p.constant(1, 8), memo).empty());
    CB_CHECK(inputs(p.true_condition(), memo).empty());
    // a deep chain does not run out of stack
    const Expr* chain = a;
    for (int i = 0; i < 100000; ++i) chain = p.binary(Op::Add, chain, a);
    InputsMemo big;
    CB_CHECK(inputs(chain, big) == (std::set<std::string_view>{"a"}));
});

CB_TEST("emulate.symbolic.z3_finds_the_input_that_satisfies_a_compare", [] {
    ExprPool p;
    const Expr* x = p.var("x", 32);
    // x + 1 == 5
    const Expr* eq = p.compare(Op::Eq, p.binary(Op::Add, x, p.constant(1, 32)), p.constant(5, 32));
    Answer a = solve({eq}, x);
    CB_CHECK(a.sat);
    CB_CHECK_EQ(a.value.value_or(99), 4u);
    // x + 1 == 5 and x != 4 cannot both hold
    CB_CHECK(!solve({eq, p.negate(eq)}).sat);
    CB_CHECK(!solve({eq, p.nonzero(p.binary(Op::Xor, x, p.constant(4, 32)))}).sat);
});

CB_TEST("emulate.symbolic.z3_terms_agree_with_the_arithmetic", [] {
    // For each operation: a condition `op(5, 3) == expected` over constants must be satisfiable, and `!= expected` not.
    struct Case { Op op; std::uint64_t a, b, expected; };
    const Case cases[] = {
        {Op::Add, 250, 10, 4},      {Op::Sub, 3, 5, 254},       {Op::And, 0xF0, 0x3C, 0x30}, {Op::Or, 0xF0, 0x0C, 0xFC},
        {Op::Xor, 0xFF, 0x0F, 0xF0}, {Op::Mul, 20, 20, 144},    {Op::UDiv, 200, 7, 28},      {Op::URem, 200, 7, 4},
        {Op::SDiv, 0xFA, 3, 0xFE},  {Op::SRem, 0xF9, 3, 0xFF},  {Op::Shl, 0x81, 1, 0x02},    {Op::LShr, 0x81, 1, 0x40},
        {Op::AShr, 0x81, 1, 0xC0},
    };
    for (const Case& c : cases) {
        ExprPool p;
        const Expr* r = p.binary(c.op, p.constant(c.a, 8), p.constant(c.b, 8));
        const Expr* expected = p.constant(c.expected, 8);
        CB_CHECK(solve({p.compare(Op::Eq, r, expected)}).sat);
        CB_CHECK(!solve({p.negate(p.compare(Op::Eq, r, expected))}).sat);
    }
    ExprPool p;
    const Expr* v = p.constant(0xB5, 8);                          // 1011 0101
    auto equals = [&](const Expr* e, std::uint64_t want, std::uint32_t bits) {
        return solve({p.compare(Op::Eq, e, p.constant(want, bits))}).sat &&
               !solve({p.negate(p.compare(Op::Eq, e, p.constant(want, bits)))}).sat;
    };
    CB_CHECK(equals(p.unary(Op::Neg, v), 0x4B, 8));
    CB_CHECK(equals(p.unary(Op::Not, v), 0x4A, 8));
    CB_CHECK(equals(p.zext(v, 16), 0x00B5, 16));
    CB_CHECK(equals(p.sext(v, 16), 0xFFB5, 16));
    CB_CHECK(equals(p.extract(v, 2, 4), 0xD, 4));                  // bits 5..2 of 10110101: 1101
    CB_CHECK(equals(p.concat(v, p.constant(0xA, 4)), 0xB5A, 12));
    CB_CHECK(equals(p.popcount(v, 8), 5, 8));
    CB_CHECK(equals(p.popcount(v, 1), 1, 1));                      // five bits set, one bit wide: odd
    const Expr* is_big = p.compare(Op::ULt, p.constant(100, 8), v);
    CB_CHECK(equals(p.ite(is_big, p.constant(7, 8), p.constant(9, 8)), 7, 8));
    CB_CHECK(equals(p.to_bool(is_big), 1, 8));
    CB_CHECK(equals(p.to_bool(p.negate(is_big)), 0, 8));
});

CB_TEST("emulate.symbolic.z3_comparisons_signed_and_unsigned", [] {
    ExprPool p;
    const Expr* minus_one = p.constant(0xFF, 8);
    const Expr* one = p.constant(1, 8);
    auto holds = [&](const Expr* c) { return solve({c}).sat && !solve({p.negate(c)}).sat; };
    CB_CHECK(holds(p.compare(Op::SLt, minus_one, one)));           // -1 < 1 signed
    CB_CHECK(holds(p.negate(p.compare(Op::ULt, minus_one, one)))); // but 255 is not < 1
    CB_CHECK(holds(p.compare(Op::ULe, one, one)));
    CB_CHECK(holds(p.compare(Op::SLe, minus_one, minus_one)));
    CB_CHECK(holds(p.logic(Op::BAnd, p.compare(Op::SLt, minus_one, one), p.nonzero(one))));
    CB_CHECK(holds(p.logic(Op::BOr, p.compare(Op::ULt, minus_one, one), p.nonzero(one))));
    CB_CHECK(holds(p.logic(Op::BXor, p.compare(Op::ULt, minus_one, one), p.nonzero(one))));
    CB_CHECK(holds(p.true_condition()));
    CB_CHECK(!solve({p.nonzero(p.constant(0, 8))}).sat);
});

CB_TEST("emulate.symbolic.z3_terms_are_made_once_per_node", [] {
    ExprPool p;
    Z3Context context;
    Z3Terms terms(context.handle());
    const Expr* x = p.var("x", 16);
    const Expr* sum = p.binary(Op::Add, x, x);
    CB_CHECK(terms.term(sum) == terms.term(sum));
    CB_CHECK(terms.term(x) == terms.term(x));
    // two nodes of the same shape are two terms in the pool's eyes but Z3 itself shares structurally equal terms
    CB_CHECK(terms.term(p.var("x", 16)) == terms.term(x));
});

CB_TEST("emulate.symbolic.mismatched_widths_are_an_internal_error_not_a_crash", [] {
    ExprPool p;
    Z3Context context;
    Z3Terms terms(context.handle());
    const Expr* bad = p.binary(Op::Add, p.var("x", 8), p.var("y", 16));
    CB_CHECK_THROWS_AS(InternalError, (void)terms.term(bad));
});
