#include "cb/recompile/builders/vector.hpp"

#include "cb/recompile/builders/test_support.hpp"

using namespace cb;
namespace vc = cb::recompile::builders::vector;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::test::TokenOperands;

namespace {

std::vector<std::string> vector_of(std::string_view mnemonic, std::vector<std::string> operands) {
    const TokenOperands tokens;
    const auto got = vc::emit(Instruction{mnemonic, operands, 0, 0}, tokens);
    return got.value_or(std::vector<std::string>{"<not ours>"});
}

constexpr const char* kFlush = "ctx.fpscr.enableFlushMode();";

}  // namespace

CB_TEST("recompile.vector.every_case_matches_the_reference", [] {
    const std::size_t cases = recompile::builders::test::check_golden("vector.golden.txt", vc::emit);
    CB_CHECK_EQ(cases, 1137u);
});

CB_TEST("recompile.vector.mnemonics_in_builder_order", [] {
    const auto names = vc::mnemonics();
    CB_CHECK_EQ(names.size(), 172u);
    CB_CHECK_EQ(std::string(names.front()), "vaddfp");
    CB_CHECK_EQ(std::string(names.back()), "vupklsh128");
    CB_CHECK(vc::runtime_helpers().find("simde_mm_vctsxs") != std::string_view::npos);
});

CB_TEST("recompile.vector.float_arithmetic_flushes_denormals_first", [] {
    const auto add = vector_of("vaddfp", {"v1", "v2", "v3"});
    CB_CHECK_EQ(add.size(), 2u);
    CB_CHECK_EQ(add[0], kFlush);
    CB_CHECK_EQ(add[1], "simde_mm_store_ps(ctx.v1.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v2.f32), simde_mm_load_ps(ctx.v3.f32)));");
    CB_CHECK(vector_of("vaddfp128", {"v1", "v2", "v3"}) == add);
    // a * c + b: the multiplier is the third operand, the addend the fourth
    CB_CHECK(vector_of("vmaddfp", {"v1", "v2", "v3", "v4"})[1].find("simde_mm_mul_ps(simde_mm_load_ps(ctx.v2.f32), simde_mm_load_ps(ctx.v3.f32)), "
                                                                       "simde_mm_load_ps(ctx.v4.f32)") != std::string::npos);
    // integer arithmetic does not
    CB_CHECK_EQ(vector_of("vadduwm", {"v1", "v2", "v3"}).size(), 1u);
});

CB_TEST("recompile.vector.record_compares_update_cr6", [] {
    const auto plain = vector_of("vcmpequw", {"v1", "v2", "v3"});
    const auto record = vector_of("vcmpequw.", {"v1", "v2", "v3"});
    CB_CHECK_EQ(plain.size(), 1u);
    CB_CHECK_EQ(record.size(), 2u);
    CB_CHECK_EQ(record[0], plain[0]);
    CB_CHECK_EQ(record[1], "ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v1.f32), 0xF);");
    // the byte compare masks all 16 bits
    CB_CHECK_EQ(vector_of("vcmpequb.", {"v1", "v2", "v3"})[1], "ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v1.u8), 0xFFFF);");
});

CB_TEST("recompile.vector.logical_with_the_same_register_simplifies", [] {
    CB_CHECK(vector_of("vxor", {"v3", "v4", "v4"}) ==
             std::vector<std::string>{"simde_mm_store_si128((simde__m128i*)ctx.v3.u8, simde_mm_setzero_si128());"});
    CB_CHECK(vector_of("vor", {"v3", "v4", "v4"}) ==
             std::vector<std::string>{"simde_mm_store_si128((simde__m128i*)ctx.v3.u8, simde_mm_load_si128((simde__m128i*)ctx.v4.u8));"});
    // vandc is vA & ~vB: andnot takes its operands the other way round
    CB_CHECK(vector_of("vandc", {"v3", "v4", "v5"})[0].find("simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v5.u8), "
                                                           "simde_mm_load_si128((simde__m128i*)ctx.v4.u8))") != std::string::npos);
});

CB_TEST("recompile.vector.splat_immediates_and_scales", [] {
    // vspltisb: a 5-bit signed immediate: 0x1F is -1, which is 0xFF in a byte
    CB_CHECK(vector_of("vspltisb", {"v3", "31"}) ==
             std::vector<std::string>{"simde_mm_store_si128((simde__m128i*)ctx.v3.u8, simde_mm_set1_epi8(char(0xFF)));"});
    CB_CHECK(vector_of("vspltisb", {"v3", "16"})[0].find("char(0xF0)") != std::string::npos);   // -16
    // vsldoi shifts left by n bytes: alignr takes 16 - n
    CB_CHECK(vector_of("vsldoi", {"v3", "v4", "v5", "5"})[0].ends_with("ctx.v5.u8), 11));"));
    // vctsxs scales by 2^u, vcfsx by 2^-u (the bits of the single-precision float)
    CB_CHECK(vector_of("vctsxs", {"v3", "v4", "3"})[1].find("simde_mm_set1_ps(8)") != std::string::npos);
    CB_CHECK(vector_of("vctsxs", {"v3", "v4", "0"})[1].find("set1_ps") == std::string::npos);
    CB_CHECK(vector_of("vcfsx", {"v3", "v4", "3"})[1].find("0x3E000000") != std::string::npos);
    CB_CHECK(vector_of("vcfux", {"v3", "v4", "1"})[1].find("0x3F000000") != std::string::npos);
});

CB_TEST("recompile.vector.vmx128_misidentified_unpack_forms", [] {
    // vupkhsb128 with 0x60 as the third operand is the halfword form
    CB_CHECK(vector_of("vupkhsb128", {"v3", "v4", "0x60"}) == vector_of("vupkhsh", {"v3", "v4"}));
    CB_CHECK(vector_of("vupkhsb128", {"v3", "v4", "5"}) == vector_of("vupkhsb", {"v3", "v4"}));
    CB_CHECK(vector_of("vupklsb128", {"v3", "v4", "0x60"}) == vector_of("vupklsh", {"v3", "v4"}));
    CB_CHECK(vector_of("vupklsb128", {"v3", "v4"}) == vector_of("vupklsb", {"v3", "v4"}));
});

CB_TEST("recompile.vector.direct3d_pack_and_unpack_formats", [] {
    // vpkd3d128 vD, vB, type, mask, shift: an unknown type traps in the generated code
    const auto bad = vector_of("vpkd3d128", {"v3", "v4", "9", "0", "0"});
    CB_CHECK(bad == (std::vector<std::string>{kFlush, "__builtin_debugtrap();"}));
    // the colour format with the full mask writes zero
    const auto colour = vector_of("vpkd3d128", {"v3", "v4", "0", "3", "3"});
    CB_CHECK_EQ(colour.back(), "ctx.v3.u32[0] = 0;");
    CB_CHECK_EQ(colour.front(), kFlush);
    // vupkd3d128 selects its format by (operand >> 2); an unknown one traps
    CB_CHECK(vector_of("vupkd3d128", {"v3", "v4", "0x1C"}) == std::vector<std::string>{"__builtin_debugtrap();"});
    CB_CHECK_EQ(vector_of("vupkd3d128", {"v3", "v4", "0"}).size(), 5u);   // four colour words and the store
    CB_CHECK_EQ(vector_of("vupkd3d128", {"v3", "v4", "0x18"}).size(), 8u);   // NORMPACKED64
});

CB_TEST("recompile.vector.other_mnemonics_are_not_ours", [] {
    CB_CHECK(vector_of("add", {"r3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});
    CB_CHECK(vector_of("lvx", {"v3", "r4", "r5"}) == std::vector<std::string>{"<not ours>"});   // the memory builder's
    CB_CHECK(vector_of("vmaddfp.", {"v3", "v4", "v5", "v6"}).size() == 2u);                     // '.' is stripped by every vector form
});

CB_TEST("recompile.vector.bad_operands_are_parse_errors", [] {
    const TokenOperands tokens;
    const std::vector<std::string> gpr = {"r3", "r4", "r5"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vaddfp", gpr, 0, 0}, tokens));
    const std::vector<std::string> two = {"v3", "v4"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vaddfp", two, 0, 0}, tokens));
    const std::vector<std::string> no_imm = {"v3", "v4", "v5"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vspltw", no_imm, 0, 0}, tokens));
    // a rotate index outside 0..3, a scale outside what a float-to-integer conversion can carry
    const std::vector<std::string> bad_index = {"v3", "v4", "5", "4"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vrlimi128", bad_index, 0, 0}, tokens));
    const std::vector<std::string> bad_scale = {"v3", "v4", "63"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vctsxs", bad_scale, 0, 0}, tokens));
    const std::vector<std::string> negative_scale = {"v3", "v4", "-1"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vcfsx", negative_scale, 0, 0}, tokens));
    const std::vector<std::string> d3d = {"v3", "v4", "0", "3"};
    CB_CHECK_THROWS_AS(ParseError, (void)vc::emit(Instruction{"vpkd3d128", d3d, 0, 0}, tokens));   // the shift operand is missing
});
