#include "cb/recompile/builders/memory.hpp"

#include "cb/recompile/builders/test_support.hpp"

using namespace cb;
namespace mm = cb::recompile::builders::memory;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::test::TokenOperands;

namespace {

std::vector<std::string> memory_of(std::string_view mnemonic, std::vector<std::string> operands) {
    const TokenOperands tokens;
    const auto got = mm::emit(Instruction{mnemonic, operands, 0, 0}, tokens);
    return got.value_or(std::vector<std::string>{"<not ours>"});
}

}  // namespace

CB_TEST("recompile.memory.every_case_matches_the_reference", [] {
    const std::size_t cases = recompile::builders::test::check_golden("memory.golden.txt", mm::emit);
    CB_CHECK_EQ(cases, 1068u);
});

CB_TEST("recompile.memory.mnemonics_in_builder_order", [] {
    const auto names = mm::mnemonics();
    CB_CHECK_EQ(names.size(), 91u);
    CB_CHECK_EQ(std::string(names[0]), "li");
    CB_CHECK_EQ(std::string(names[1]), "lis");
    CB_CHECK_EQ(std::string(names[2]), "lbz");
    CB_CHECK_EQ(std::string(names.back()), "stvx128");
    // the dotted and the bare store-conditional both
    CB_CHECK(std::find(names.begin(), names.end(), "stwcx.") != names.end());
    CB_CHECK(std::find(names.begin(), names.end(), "stdcx") != names.end());
    CB_CHECK(mm::runtime_helpers().find("VectorMaskL") != std::string_view::npos);
});

CB_TEST("recompile.memory.an_r0_base_is_a_zero_base", [] {
    // D form: the displacement alone; with a base, base + displacement
    CB_CHECK(memory_of("lwz", {"r3", "16(r0)"}) == std::vector<std::string>{"ctx.r3.u64 = PPC_LOAD_U32(16);"});
    CB_CHECK(memory_of("lwz", {"r3", "-0x70(r1)"}) == std::vector<std::string>{"ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + -112);"});
    // X form: rB alone when rA is r0 (or the token 0, as Capstone writes it)
    CB_CHECK(memory_of("lwzx", {"r3", "r0", "r5"}) == std::vector<std::string>{"ctx.r3.u64 = PPC_LOAD_U32(ctx.r5.u32);"});
    CB_CHECK(memory_of("stfiwx", {"f0", "0", "r11"}) ==
             std::vector<std::string>{"ctx.fpscr.disableFlushMode();", "PPC_STORE_U32(ctx.r11.u32, ctx.f0.u32);"});
    CB_CHECK(memory_of("lwzx", {"r3", "r4", "r5"}) == std::vector<std::string>{"ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + ctx.r5.u32);"});
});

CB_TEST("recompile.memory.update_forms_write_the_effective_address_back", [] {
    CB_CHECK(memory_of("stwu", {"r1", "-0x50(r1)"}) ==
             std::vector<std::string>{"ea = -80 + ctx.r1.u32;", "PPC_STORE_U32(ea, ctx.r1.u32);", "ctx.r1.u32 = ea;"});
    CB_CHECK(memory_of("lbzux", {"r3", "r4", "r5"}) ==
             std::vector<std::string>{"ea = ctx.r4.u32 + ctx.r5.u32;", "ctx.r3.u64 = PPC_LOAD_U8(ea);", "ctx.r4.u32 = ea;"});
    // stfsux converts, then forms the address (the order of the reference)
    CB_CHECK(memory_of("stfsux", {"f1", "r4", "r5"}) ==
             std::vector<std::string>{"ctx.fpscr.disableFlushMode();", "temp.f32 = float(ctx.f1.f64);", "ea = ctx.r4.u32 + ctx.r5.u32;",
                                      "PPC_STORE_U32(ea, temp.u32);", "ctx.r4.u32 = ea;"});
});

CB_TEST("recompile.memory.atomics_pair_through_the_reservation", [] {
    CB_CHECK(memory_of("lwarx", {"r3", "r0", "r4"}) ==
             std::vector<std::string>{"ea = ctx.r4.u32;", "ctx.reserved.u32 = *(uint32_t*)PPC_RAW_ADDR(ea);",
                                      "ctx.r3.u64 = __builtin_bswap32(ctx.reserved.u32);"});
    const auto store = memory_of("stdcx.", {"r3", "r0", "r4"});
    CB_CHECK_EQ(store.size(), 5u);
    CB_CHECK(store[3].find("__sync_bool_compare_and_swap(reinterpret_cast<uint64_t*>(PPC_RAW_ADDR(ea)), ctx.reserved.s64, __builtin_bswap64(ctx.r3.s64))") !=
             std::string::npos);
    CB_CHECK(memory_of("stwcx", {"r3", "r0", "r4"}) == memory_of("stwcx.", {"r3", "r0", "r4"}));
});

CB_TEST("recompile.memory.vector_stores_reverse_the_element_index", [] {
    CB_CHECK(memory_of("stvewx", {"v3", "r0", "r4"}) ==
             std::vector<std::string>{"ea = (ctx.r4.u32) & ~0x3;", "PPC_STORE_U32(ea, ctx.v3.u32[3 - ((ea & 0xF) >> 2)]);"});
    // the loop statement is two lines, the second one indented with a tab
    const auto left = memory_of("stvlx", {"v3", "r0", "r4"});
    CB_CHECK_EQ(left.size(), 3u);
    CB_CHECK(left[2].starts_with("\tPPC_STORE_U8(ea + i, ctx.v3.u8[15 - i]);"));
    CB_CHECK(memory_of("lvx128", {"v127", "r3", "r4"}) == memory_of("lvx", {"v127", "r3", "r4"}));
});

CB_TEST("recompile.memory.li_and_lis", [] {
    CB_CHECK(memory_of("li", {"r3", "-1"}) == std::vector<std::string>{"ctx.r3.s64 = -1;"});
    CB_CHECK(memory_of("li", {"r3", "0xFFFFFFFF"}) == std::vector<std::string>{"ctx.r3.s64 = -1;"});
    CB_CHECK(memory_of("lis", {"r3", "0x8200"}) == std::vector<std::string>{"ctx.r3.s64 = -2113929216;"});   // 0x82000000 as a signed word
    CB_CHECK(memory_of("lis", {"r3", "1"}) == std::vector<std::string>{"ctx.r3.s64 = 65536;"});
});

CB_TEST("recompile.memory.other_mnemonics_are_not_ours", [] {
    CB_CHECK(memory_of("add", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(memory_of("lfq", {"f3", "0(r3)"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(memory_of("", {}) == std::vector<std::string>{"<not ours>"});
});

CB_TEST("recompile.memory.bad_operands_are_parse_errors", [] {
    const TokenOperands tokens;
    const std::vector<std::string> no_mem = {"r3", "r4"};
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"lwz", no_mem, 0, 0}, tokens));      // r4 is not disp(rA)
    const std::vector<std::string> one = {"r3"};
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"lwz", one, 0, 0}, tokens));
    const std::vector<std::string> wrong_file = {"f3", "0(r3)"};
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"lwz", wrong_file, 0, 0}, tokens));  // rD must be general
    const std::vector<std::string> gpr_for_fpr = {"r3", "0(r3)"};
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"lfd", gpr_for_fpr, 0, 0}, tokens));
    const std::vector<std::string> short_x = {"r3", "r4"};
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"lwzx", short_x, 0, 0}, tokens));
    CB_CHECK_THROWS_AS(ParseError, (void)mm::emit(Instruction{"li", one, 0, 0}, tokens));
});
