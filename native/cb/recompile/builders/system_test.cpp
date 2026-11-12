#include "cb/recompile/builders/system.hpp"

#include "cb/recompile/builders/test_support.hpp"

using namespace cb;
namespace sy = cb::recompile::builders::system;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::test::TokenOperands;

namespace {

// "<not ours>" when the builder does not answer; the trap builders read `word`.
std::vector<std::string> system_of(std::string_view mnemonic, std::vector<std::string> operands, std::uint32_t word = 0) {
    const TokenOperands tokens;
    const auto got = sy::emit(Instruction{mnemonic, operands, 0, word}, tokens);
    return got.value_or(std::vector<std::string>{"<not ours>"});
}

// the instruction words of the trap forms: opcode, TO, rA, then rB or the immediate
constexpr std::uint32_t twi_word(unsigned to, unsigned ra, std::uint16_t simm) { return (3u << 26) | (to << 21) | (ra << 16) | simm; }
constexpr std::uint32_t tdi_word(unsigned to, unsigned ra, std::uint16_t simm) { return (2u << 26) | (to << 21) | (ra << 16) | simm; }
constexpr std::uint32_t tw_word(unsigned to, unsigned ra, unsigned rb) { return (31u << 26) | (to << 21) | (ra << 16) | (rb << 11); }

}  // namespace

CB_TEST("recompile.system.every_case_matches_the_reference", [] {
    const std::size_t cases = recompile::builders::test::check_golden("system.golden.txt", sy::emit);
    CB_CHECK_EQ(cases, 1195u);
});

CB_TEST("recompile.system.mnemonics_in_builder_order", [] {
    const auto names = sy::mnemonics();
    CB_CHECK_EQ(names.size(), 95u);
    CB_CHECK_EQ(std::string(names.front()), "nop");
    CB_CHECK_EQ(std::string(names.back()), "tdllt");
    CB_CHECK_EQ(sy::runtime_helpers(), "");
});

CB_TEST("recompile.system.nops_and_barriers_emit_nothing_but_are_ours", [] {
    for (const char* mn : {"nop", "sync", "lwsync", "eieio", "dcbt", "dcbf", "db16cyc", "cctpl"}) {
        const TokenOperands tokens;
        const auto got = sy::emit(Instruction{mn, {}, 0, 0}, tokens);
        CB_CHECK(got.has_value());
        if (got) CB_CHECK(got->empty());
    }
    CB_CHECK(system_of("isync", {}) == std::vector<std::string>{"<not ours>"});   // no builder
    CB_CHECK(system_of("add", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
});

CB_TEST("recompile.system.traps_decode_their_fields_from_the_word", [] {
    // twi 31, r0, 5: the unconditional service trap whose code is the immediate
    CB_CHECK(system_of("twi", {}, twi_word(31, 0, 5)) == std::vector<std::string>{"ppc_trap(ctx, base, 5);"});
    // tweqi r3, 5: TO = 4 (equal)
    CB_CHECK(system_of("tweqi", {}, twi_word(4, 3, 5)) == std::vector<std::string>{"if (ctx.r3.s32 == 5) ppc_trap(ctx, base, 0);"});
    // TO = 0 never traps, TO = 31 with a register other than r0 always does
    CB_CHECK(system_of("twi", {}, twi_word(0, 3, 5)).empty());
    CB_CHECK(system_of("twi", {}, twi_word(31, 3, 5)) == std::vector<std::string>{"ppc_trap(ctx, base, 0);"});
    // the immediate is sign-extended; the unsigned comparison sees the 32-bit unsigned value
    CB_CHECK(system_of("twlgti", {}, twi_word(1, 3, 0xFFFF)) == std::vector<std::string>{"if (ctx.r3.u32 > 4294967295u) ppc_trap(ctx, base, 0);"});
    CB_CHECK(system_of("twlti", {}, twi_word(0x10, 3, 0xFFFF)) == std::vector<std::string>{"if (ctx.r3.s32 < -1) ppc_trap(ctx, base, 0);"});
    // tdi works on 64 bits
    CB_CHECK(system_of("tdeqi", {}, tdi_word(4, 3, 5)) == std::vector<std::string>{"if (ctx.r3.s64 == 5ll) ppc_trap(ctx, base, 0);"});
    CB_CHECK(system_of("tdlgti", {}, tdi_word(1, 3, 0xFFFF)) == std::vector<std::string>{"if (ctx.r3.u64 > 18446744073709551615ull) ppc_trap(ctx, base, 0);"});
    // tw / td compare two registers; several TO bits join with ||
    CB_CHECK(system_of("tweq", {}, tw_word(4, 3, 4)) == std::vector<std::string>{"if (ctx.r3.s32 == ctx.r4.s32) ppc_trap(ctx, base, 0);"});
    CB_CHECK(system_of("tdlge", {}, tw_word(0x0D, 3, 4)) ==
             std::vector<std::string>{"if (ctx.r3.s64 > ctx.r4.s64 || ctx.r3.s64 == ctx.r4.s64 || ctx.r3.u64 > ctx.r4.u64) ppc_trap(ctx, base, 0);"});
});

CB_TEST("recompile.system.record_forms_append_a_cr0_compare", [] {
    CB_CHECK(system_of("mr", {"r3", "r4"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64;"});
    CB_CHECK(system_of("mr.", {"r3", "r4"}) ==
             std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64;", "ctx.cr0.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);"});
    CB_CHECK(system_of("clrldi.", {"r3", "r4", "32"}) ==
             std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 & 0xFFFFFFFF;", "ctx.cr0.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);"});
    CB_CHECK(system_of("clrldi", {"r3", "r4", "0"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 & 0xFFFFFFFFFFFFFFFF;"});
    CB_CHECK(system_of("clrldi", {"r3", "r4", "64"}) == std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 & 0x0;"});
});

CB_TEST("recompile.system.condition_register_moves", [] {
    const auto mfcr = system_of("mfcr", {"r3"});
    CB_CHECK_EQ(mfcr.size(), 32u);
    CB_CHECK_EQ(mfcr.front(), "ctx.r3.u64 = ctx.cr0.lt ? 0x80000000 : 0;");
    CB_CHECK_EQ(mfcr[1], "ctx.r3.u64 |= ctx.cr0.gt ? 0x40000000 : 0;");
    CB_CHECK_EQ(mfcr.back(), "ctx.r3.u64 |= ctx.cr7.so ? 0x1 : 0;");
    const auto mtcrf = system_of("mtcrf", {"0x01", "r3"});   // field 7 only
    CB_CHECK_EQ(mtcrf.size(), 4u);
    CB_CHECK_EQ(mtcrf[0], "ctx.cr7.lt = (ctx.r3.u32 & 0x8) != 0;");
    CB_CHECK_EQ(mtcrf[3], "ctx.cr7.so = (ctx.r3.u32 & 0x1) != 0;");
    CB_CHECK_EQ(system_of("mtcrf", {"0xFF", "r3"}).size(), 32u);
    CB_CHECK(system_of("mtcrf", {"0", "r3"}).empty());
    CB_CHECK(system_of("mfocrf", {"r3", "0x80"}) ==
             std::vector<std::string>{"ctx.r3.u64 = (ctx.cr0.lt << 31) | (ctx.cr0.gt << 30) | (ctx.cr0.eq << 29) | (ctx.cr0.so << 28);"});
    CB_CHECK(system_of("mcrf", {"cr6", "cr0"}) == std::vector<std::string>{"ctx.cr6 = ctx.cr0;"});
});

CB_TEST("recompile.system.condition_register_bit_operations", [] {
    // the operands are bit numbers: field * 4 + lt/gt/eq/so
    CB_CHECK(system_of("crand", {"cr0lt", "cr1gt", "cr7so"}) == std::vector<std::string>{"ctx.cr0.lt = ctx.cr1.gt & ctx.cr7.so;"});
    CB_CHECK(system_of("crnor", {"cr0eq", "cr1gt", "cr7so"}) == std::vector<std::string>{"ctx.cr0.eq = !(ctx.cr1.gt | ctx.cr7.so);"});
    CB_CHECK(system_of("crandc", {"cr0eq", "cr1gt", "cr7so"}) == std::vector<std::string>{"ctx.cr0.eq = ctx.cr1.gt & !(ctx.cr7.so);"});
    CB_CHECK(system_of("creqv", {"cr0eq", "cr1gt", "cr7so"}) == std::vector<std::string>{"ctx.cr0.eq = !(ctx.cr1.gt ^ ctx.cr7.so);"});
    CB_CHECK(system_of("crorc", {"cr0eq", "cr1gt", "cr7so"}) == std::vector<std::string>{"ctx.cr0.eq = ctx.cr1.gt | !(ctx.cr7.so);"});
    CB_CHECK(system_of("crxor", {"cr3so", "cr3so", "cr3so"}) == std::vector<std::string>{"ctx.cr3.so = ctx.cr3.so ^ ctx.cr3.so;"});
});

CB_TEST("recompile.system.special_registers_and_cache_zeroing", [] {
    CB_CHECK(system_of("mflr", {"r0"}) == std::vector<std::string>{"ctx.r0.u64 = ctx.lr;"});
    CB_CHECK(system_of("mtctr", {"r12"}) == std::vector<std::string>{"ctx.ctr.u64 = ctx.r12.u64;"});
    CB_CHECK(system_of("mftb", {"r3"}) == std::vector<std::string>{"ctx.r3.u64 = PPC_QUERY_TIMEBASE();"});
    CB_CHECK(system_of("mffs", {"f3"}) == std::vector<std::string>{"ctx.f3.u64 = ctx.fpscr.loadFromHost();"});
    // r13 enters the global lock, any other register leaves it
    CB_CHECK_EQ(system_of("mtmsrd", {"r13"}).back(), "PPC_ENTER_GLOBAL_LOCK();");
    CB_CHECK_EQ(system_of("mtmsrd", {"r3"}).back(), "PPC_LEAVE_GLOBAL_LOCK();");
    // mtfsf: every field is a plain store; a partial mask merges with the current value
    CB_CHECK(system_of("mtfsf", {"0xFF", "f3"}) == std::vector<std::string>{"ctx.fpscr.storeFromGuest(ctx.f3.u32);"});
    CB_CHECK(system_of("mtfsf", {"0x80", "f3"}) ==
             std::vector<std::string>{"ctx.fpscr.storeFromGuest((ctx.fpscr.loadFromHost() & 0xFFFFFFF0) | (ctx.f3.u32 & 0x0000000F));"});
    // dcbz zeroes the 32-byte block the address falls in, dcbzl the 128-byte one
    CB_CHECK(system_of("dcbz", {"r0", "r5"}) == std::vector<std::string>{"ea = (ctx.r5.u32) & ~31;", "memset((void*)PPC_RAW_ADDR(ea), 0, 32);"});
    CB_CHECK(system_of("dcbzl", {"r4", "r5"}) ==
             std::vector<std::string>{"ea = (ctx.r4.u32 + ctx.r5.u32) & ~127;", "memset((void*)PPC_RAW_ADDR(ea), 0, 128);"});
});

CB_TEST("recompile.system.bad_operands_are_parse_errors", [] {
    const TokenOperands tokens;
    const std::vector<std::string> not_bits = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"crand", not_bits, 0, 0}, tokens));
    const std::vector<std::string> short_bits = {"cr0lt", "cr0gt"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"cror", short_bits, 0, 0}, tokens));
    const std::vector<std::string> vec = {"v3"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"mflr", vec, 0, 0}, tokens));
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"mflr", {}, 0, 0}, tokens));
    const std::vector<std::string> too_far = {"r3", "r4", "65"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"clrldi", too_far, 0, 0}, tokens));   // a count above 64
    const std::vector<std::string> not_imm = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"clrldi", not_imm, 0, 0}, tokens));
    const std::vector<std::string> gpr_for_fpr = {"r3"};
    CB_CHECK_THROWS_AS(ParseError, (void)sy::emit(Instruction{"mffs", gpr_for_fpr, 0, 0}, tokens));
});
