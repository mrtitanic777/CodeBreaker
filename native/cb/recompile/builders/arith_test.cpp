#include "cb/recompile/builders/arith.hpp"

#include "cb/recompile/builders/test_support.hpp"

using namespace cb;
namespace ar = cb::recompile::builders::arith;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::test::TokenOperands;

namespace {

std::vector<std::string> arith_of(std::string_view mnemonic, std::vector<std::string> operands) {
    const TokenOperands tokens;
    const auto got = ar::emit(Instruction{mnemonic, operands, 0, 0}, tokens);
    return got.value_or(std::vector<std::string>{"<not ours>"});
}

}  // namespace

CB_TEST("recompile.arith.every_case_matches_the_reference", [] {
    const std::size_t cases = recompile::builders::test::check_golden("arith.golden.txt", ar::emit);
    CB_CHECK_EQ(cases, 544u);
});

CB_TEST("recompile.arith.mnemonics_and_helpers", [] {
    const auto names = ar::mnemonics();
    CB_CHECK_EQ(names.size(), 34u);
    CB_CHECK_EQ(std::string(names.front()), "add");
    CB_CHECK_EQ(std::string(names.back()), "cmpwi");
    CB_CHECK_EQ(ar::runtime_helpers(), "");
    // every mnemonic of the list is answered, given operands of the right kind
    const TokenOperands tokens;
    for (const auto name : names) {
        const bool compare = name.starts_with("cmp");
        const std::vector<std::string> ops = compare ? std::vector<std::string>{"cr6", "r4", name.ends_with('i') ? "7" : "r5"}
                                                     : std::vector<std::string>{"r3", "r4", name == "addi" || name == "addic" || name == "addis" || name == "mulli" || name == "subfic" ? "7" : "r5"};
        CB_CHECK(ar::emit(Instruction{name, ops, 0, 0}, tokens).has_value());
    }
});

CB_TEST("recompile.arith.a_record_form_is_its_base_and_overflow_forms_are_not_built", [] {
    CB_CHECK(arith_of("add.", {"r3", "r4", "r5"}) == arith_of("add", {"r3", "r4", "r5"}));
    CB_CHECK(arith_of("addo", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(arith_of("divwo", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(arith_of("", {}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(arith_of(".", {}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(arith_of("ori", {"r3", "r4", "1"}) == std::vector<std::string>{"<not ours>"});   // the logical builder's
});

CB_TEST("recompile.arith.immediates_are_32_bit_and_r0_as_a_base_is_zero", [] {
    // addi with rA = r0 loads the immediate; otherwise adds it
    CB_CHECK(arith_of("addi", {"r3", "r0", "-5"}) == std::vector<std::string>{"ctx.r3.s64 = -5;"});
    CB_CHECK(arith_of("addi", {"r3", "r4", "0xFFFF"}) == std::vector<std::string>{"ctx.r3.s64 = ctx.r4.s64 + 65535;"});
    CB_CHECK(arith_of("addi", {"r3", "r4", "0xFFFFFFFF"}) == std::vector<std::string>{"ctx.r3.s64 = ctx.r4.s64 + -1;"});
    // addis: the immediate shifted left by 16, as a 32-bit signed number
    CB_CHECK(arith_of("addis", {"r3", "r0", "0x8000"}) == std::vector<std::string>{"ctx.r3.s64 = -2147483648;"});
    CB_CHECK(arith_of("addis", {"r3", "r4", "1"}) == std::vector<std::string>{"ctx.r3.s64 = ctx.r4.s64 + 65536;"});
    // addic: carry when rA > ~imm (unsigned 32-bit): ~5 = 4294967290
    CB_CHECK(arith_of("addic", {"r3", "r4", "5"}).front() == "ctx.xer.ca = ctx.r4.u32 > 4294967290;");
    // subfic: carry when rA <= imm
    CB_CHECK(arith_of("subfic", {"r3", "r4", "-1"}).front() == "ctx.xer.ca = ctx.r4.u32 <= 4294967295;");
    // compares: signed immediates sign-extended from 32 bits, unsigned ones as 32-bit unsigned
    CB_CHECK(arith_of("cmpwi", {"cr6", "r4", "-1"}).front() == "ctx.cr6.compare<int32_t>(ctx.r4.s32, -1, ctx.xer);");
    CB_CHECK(arith_of("cmplwi", {"cr6", "r4", "-1"}).front() == "ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4294967295, ctx.xer);");
    CB_CHECK(arith_of("cmpldi", {"cr0", "r4", "0xFFFF"}).front() == "ctx.cr0.compare<uint64_t>(ctx.r4.u64, 65535, ctx.xer);");
});

CB_TEST("recompile.arith.division_and_subtract_from_follow_the_manual", [] {
    // divw: zero for a zero divisor and for INT_MIN / -1; rD = rA / rB
    const auto divw = arith_of("divw", {"r3", "r4", "r5"});
    CB_CHECK_EQ(divw.size(), 1u);
    CB_CHECK(divw.front().find("ctx.r5.s32 && !(ctx.r4.s32 == INT32_MIN && ctx.r5.s32 == -1)") != std::string::npos);
    CB_CHECK(divw.front().find("ctx.r4.s32 / ctx.r5.s32 : 0") != std::string::npos);
    // subf: rD = rB - rA (the first source is subtracted)
    CB_CHECK(arith_of("subf", {"r3", "r4", "r5"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r5.u64 - ctx.r4.u64;"});
    // subfc sets the carry from rB >= rA before the result is written
    CB_CHECK(arith_of("subfc", {"r3", "r4", "r5"}).front() == "ctx.xer.ca = ctx.r5.u32 >= ctx.r4.u32;");
});

CB_TEST("recompile.arith.bad_operands_are_parse_errors", [] {
    const TokenOperands tokens;
    const std::vector<std::string> two = {"r3", "r4"};
    CB_CHECK_THROWS_AS(ParseError, (void)ar::emit(Instruction{"add", two, 0, 0}, tokens));            // rB missing
    const std::vector<std::string> vec = {"v3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)ar::emit(Instruction{"add", vec, 0, 0}, tokens));            // not a general register
    const std::vector<std::string> no_imm = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)ar::emit(Instruction{"addi", no_imm, 0, 0}, tokens));        // r5 is not an immediate
    const std::vector<std::string> not_cr = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)ar::emit(Instruction{"cmpw", not_cr, 0, 0}, tokens));        // the field must come first
    CB_CHECK_THROWS_AS(ParseError, (void)ar::emit(Instruction{"neg", {}, 0, 0}, tokens));
});
