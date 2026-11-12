#include "cb/recompile/builders/logical.hpp"

#include "cb/recompile/builders/test_support.hpp"

using namespace cb;
namespace lg = cb::recompile::builders::logical;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::test::TokenOperands;

namespace {

std::vector<std::string> logical_of(std::string_view mnemonic, std::vector<std::string> operands) {
    const TokenOperands tokens;
    const auto got = lg::emit(Instruction{mnemonic, operands, 0, 0}, tokens);
    return got.value_or(std::vector<std::string>{"<not ours>"});
}

}  // namespace

CB_TEST("recompile.logical.every_case_matches_the_reference", [] {
    const std::size_t cases = recompile::builders::test::check_golden("logical.golden.txt", lg::emit);
    CB_CHECK_EQ(cases, 930u);
});

CB_TEST("recompile.logical.mnemonics_in_builder_order", [] {
    const auto names = lg::mnemonics();
    CB_CHECK_EQ(names.size(), 41u);
    CB_CHECK_EQ(std::string(names.front()), "and");
    CB_CHECK_EQ(std::string(names.back()), "srw");
    CB_CHECK_EQ(lg::runtime_helpers(), "");
});

CB_TEST("recompile.logical.rotate_masks_follow_the_manual", [] {
    // bit 0 is the most significant: MASK(0, 63) is every bit, MASK(32, 63) the low word, MASK(0, 31) the high word
    CB_CHECK_EQ(lg::rotate_mask(0, 63), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(lg::rotate_mask(32, 63), 0x00000000FFFFFFFFull);
    CB_CHECK_EQ(lg::rotate_mask(0, 31), 0xFFFFFFFF00000000ull);
    CB_CHECK_EQ(lg::rotate_mask(63, 63), 0x1ull);
    CB_CHECK_EQ(lg::rotate_mask(0, 0), 0x8000000000000000ull);
    // start after stop: the mask wraps round (ones everywhere but between stop + 1 and start - 1)
    CB_CHECK_EQ(lg::rotate_mask(62, 1), 0xC000000000000003ull);
    // both are taken modulo 64
    CB_CHECK_EQ(lg::rotate_mask(64 + 32, 64 + 63), 0x00000000FFFFFFFFull);
});

CB_TEST("recompile.logical.rlwinm_masks_the_low_word", [] {
    // rlwinm rA, rS, 5, 5, 26: bits 5..26 of the word, which is bits 37..58 of the doubleword: 0x07FFFFE0
    CB_CHECK(logical_of("rlwinm", {"r3", "r4", "5", "5", "26"}) ==
             std::vector<std::string>{"ctx.r3.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 5) & 0x7FFFFE0;"});
    // rlwinm with sh = 0, mb = 0, me = 31 keeps the whole low word
    CB_CHECK(logical_of("rlwinm", {"r3", "r4", "0", "0", "31"}).front().ends_with("& 0xFFFFFFFF;"));
    // clrlwi rA, rS, 16 keeps the low 16 bits
    CB_CHECK(logical_of("clrlwi", {"r3", "r4", "16"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u32 & 0xFFFF;"});
    CB_CHECK(logical_of("clrlwi", {"r3", "r4", "0"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u32 & 0xFFFFFFFF;"});
    CB_CHECK(logical_of("clrlwi", {"r3", "r4", "32"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u32 & 0x0;"});
});

CB_TEST("recompile.logical.record_andi_sets_cr0_itself_and_shifts_by_sixteen", [] {
    CB_CHECK(logical_of("andi", {"r3", "r4", "255"}) ==
             std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 & 255;", "ctx.cr0.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);"});
    CB_CHECK(logical_of("andis", {"r3", "r4", "1"}).front() == "ctx.r3.u64 = ctx.r4.u64 & 65536;");
    CB_CHECK(logical_of("oris", {"r3", "r4", "0xFFFF"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 | 4294901760;"});
    CB_CHECK(logical_of("xoris", {"r3", "r4", "-1"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 ^ -65536;"});
});

CB_TEST("recompile.logical.shift_by_zero_and_algebraic_shifts", [] {
    // an algebraic shift by zero clears the carry and copies; otherwise the carry is set by the bits shifted out
    CB_CHECK(logical_of("sradi", {"r3", "r4", "0"}) == std::vector<std::string>{"ctx.xer.ca = 0;", "ctx.r3.s64 = ctx.r4.s64;"});
    CB_CHECK(logical_of("srawi", {"r3", "r4", "4"}) ==
             std::vector<std::string>{"ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xF) != 0);", "ctx.r3.s64 = ctx.r4.s32 >> 4;"});
    CB_CHECK(logical_of("sradi", {"r3", "r4", "1"}).front() == "ctx.xer.ca = (ctx.r4.s64 < 0) & ((ctx.r4.u64 & 0x1) != 0);");
    // rldic with a zero shift is a plain mask
    CB_CHECK(logical_of("rldic", {"r3", "r4", "0", "8"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 & 0xFFFFFFFFFFFFFF;"});
});

CB_TEST("recompile.logical.other_mnemonics_are_not_ours", [] {
    CB_CHECK(logical_of("add", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(logical_of("crand", {"cr0lt", "cr0gt", "cr0eq"}) == std::vector<std::string>{"<not ours>"});   // the system builder's
    CB_CHECK(logical_of("or.", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});               // the recompiler strips '.'
    CB_CHECK(logical_of("", {}) == std::vector<std::string>{"<not ours>"});
});

CB_TEST("recompile.logical.bad_operands_are_parse_errors", [] {
    const TokenOperands tokens;
    const std::vector<std::string> one = {"r3"};
    CB_CHECK_THROWS_AS(ParseError, (void)lg::emit(Instruction{"or", one, 0, 0}, tokens));
    const std::vector<std::string> not_imm = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)lg::emit(Instruction{"ori", not_imm, 0, 0}, tokens));
    const std::vector<std::string> short_rot = {"r3", "r4", "5", "5"};
    CB_CHECK_THROWS_AS(ParseError, (void)lg::emit(Instruction{"rlwinm", short_rot, 0, 0}, tokens));   // me missing
    // a clear count above 32 has no mask
    const std::vector<std::string> too_many = {"r3", "r4", "33"};
    CB_CHECK_THROWS_AS(ParseError, (void)lg::emit(Instruction{"clrlwi", too_many, 0, 0}, tokens));
    // an immediate that cannot be shifted left by 16 inside 64 bits
    const std::vector<std::string> huge = {"r3", "r4", "0x7FFFFFFFFFFFFFFF"};
    CB_CHECK_THROWS_AS(ParseError, (void)lg::emit(Instruction{"andis", huge, 0, 0}, tokens));
});
