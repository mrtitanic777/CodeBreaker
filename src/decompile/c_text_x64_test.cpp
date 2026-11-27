// Tests of cb/decompile/c_text_x64's rewrites of one function's text (x64_function_text): the decompiler's C as it writes it for
// kernel32, made what Visual C++'s x64 compiler takes.
#include <string>

#include "core/testing.hpp"
#include "decompile/c_text.hpp"

namespace ct = cb::decompile::c_text;

namespace {

bool x64_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

}  // namespace

CB_TEST("decompile.c_text_x64.stack_variables_written_as_one_are_each_written_their_part", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(uint4 p)\n\n{\n  xunknown8 xStack_38;\n  xunknown4 xStack_30;\n  xunknown4 xStack_2c;\n  \n"
        "  _xStack_30 = CONCAT44(1,p);\n  return xStack_2c;\n}");
    CB_CHECK(x64_has(got, "{ uint8 __cb_piece = (uint8)(CONCAT44(1,p)); xStack_30 = (xunknown4)(__cb_piece >> 0); xStack_2c = (xunknown4)(__cb_piece >> 32); }"));
    CB_CHECK(!x64_has(got, "_xStack_30"));
});

CB_TEST("decompile.c_text_x64.stack_variables_of_two_sizes_read_and_written_as_six_bytes", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(uint2 u)\n\n{\n  xunknown4 xStack_60;\n  uint2 uStack_5c;\n  xunknown2 xStack_5a;\n  \n"
        "  _xStack_60 = 0;\n  _xStack_60 = CONCAT24(u,xStack_60);\n  return _xStack_60;\n}");
    CB_CHECK(x64_has(got, "uStack_5c = (uint2)(__cb_piece >> 32); }"));
    CB_CHECK(!x64_has(got, "xStack_5a ="));   // (6 bytes: not the variable after them)
    CB_CHECK(x64_has(got, "return (((uint8)(uint4)xStack_60 << 0) | ((uint8)(uint2)uStack_5c << 32));"));
});

CB_TEST("decompile.c_text_x64.stack_variables_not_end_to_end_are_left", [] {
    const std::string text = "uint8 f(uint4 p)\n\n{\n  xunknown4 xStack_30;\n  xunknown4 xStack_28;\n  \n  _xStack_30 = CONCAT44(1,p);\n  return 0;\n}";
    CB_CHECK(x64_has(ct::x64_function_text(text), "_xStack_30 = CONCAT44(1,p);"));
});

CB_TEST("decompile.c_text_x64.products_of_16_byte_variables_and_shifted_values_are_the_headers_helper", [] {
    const std::string got = ct::x64_function_text(
        "xunknown8 f(void)\n\n{\n  xunknown1 axVar1 [16];\n  xunknown1 axVar2 [16];\n  \n"
        "  if (SUB168(axVar1 * axVar2,8) == 0) return 1;\n  return SUB168(axVar1 * (ZEXT416(uRam000000007ffe0004) << 0x20),8);\n}");
    CB_CHECK(x64_has(got, "__cb_mulx128(__cb_ld128(axVar1), __cb_ld128(axVar2), 8) == 0"));
    CB_CHECK(x64_has(got, "return __cb_mulx128(__cb_ld128(axVar1), __cb_shl128(__cb_zx128((uint8)(uRam000000007ffe0004)), 0x20), 8);"));
});

CB_TEST("decompile.c_text_x64.a_number_local_reached_as_a_pointer_is_declared_as_one", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(int8 p)\n\n{\n  xunknown8 xStack_338;\n  xunknown8 xStack_350;\n  uint8 uVar1;\n  \n"
        "  xStack_338 = (s_f_x *)p;\n  xStack_350 = (int8 *)p;\n  uVar1 = xStack_338->f_0 + xStack_350[1];\n  return uVar1;\n}");
    CB_CHECK(x64_has(got, "  s_f_x *xStack_338;\n"));
    CB_CHECK(x64_has(got, "  int8 *xStack_350;\n"));
    CB_CHECK(x64_has(got, "  uint8 uVar1;\n"));
});

CB_TEST("decompile.c_text_x64.an_unnamed_field_read_or_written_is_the_byte_there", [] {
    const std::string got = ct::x64_function_text("uint8 f(s_f_p *p)\n\n{\n  p[1].field_0xc = 1;\n  return p->field_0x20 + p->a.field_0x4;\n}");
    CB_CHECK(x64_has(got, "(*(xunknown1 *)((uint1 *)&p[1] + 0xc)) = 1;"));
    CB_CHECK(x64_has(got, "(*(xunknown1 *)((uint1 *)p + 0x20)) + p->a.field_0x4"));
});

CB_TEST("decompile.c_text_x64.a_16_byte_variable_given_to_the_place_a_pointer_points_at_is_a_byte_copy", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(xunknown1 (*param_2) [16])\n\n{\n  xunknown1 axVar2 [16];\n  \n  *param_2 = axVar2;\n  return 0;\n}");
    CB_CHECK(x64_has(got, "__movsb((uint1 *)&(*param_2), (uint1 *)axVar2, 16);"));
});

CB_TEST("decompile.c_text_x64.a_signed_128_bit_product_is_the_headers_helper_on_sign_extended_halves", [] {
    const std::string got = ct::x64_function_text("int8 f(int8 a)\n\n{\n  return SUB168(SEXT816(-0x20c49ba5e353f7cf) * SEXT816((int8)(a * 3)),8);\n}");
    CB_CHECK(x64_has(got, "return __cb_mulx128(__cb_sx128((int8)(int8)(-0x20c49ba5e353f7cf)), __cb_sx128((int8)(int8)((int8)(a * 3))), 8);"));
});

CB_TEST("decompile.c_text_x64.a_local_given_a_pointer_parameter_is_declared_as_that_pointer", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(s_f_param_1 *param_1,uint4 param_2)\n\n{\n  xunknown8 xStack_e0;\n  \n  xStack_e0 = param_1;\n  return xStack_e0->f_8;\n}");
    CB_CHECK(x64_has(got, "  s_f_param_1 *xStack_e0;\n"));
});

CB_TEST("decompile.c_text_x64.an_array_given_a_value_holds_it_in_its_low_bytes", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(void)\n\n{\n  xunknown1 axStack_58 [8];\n  xunknown1 axStack_98 [16];\n  \n  axStack_58 = (xunknown1  [8])0x0;\n"
        "  axStack_98 = ZEXT816(0);\n  return 0;\n}");
    CB_CHECK(x64_has(got, "  *(uint8 *)axStack_58 = (uint8)(0x0);\n"));
    CB_CHECK(x64_has(got, "  *(uint8 *)axStack_98 = (uint8)(0); *(uint8 *)((uint1 *)axStack_98 + 8) = 0;\n"));
});

CB_TEST("decompile.c_text_x64.a_label_written_twice_is_renamed_the_second_time", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(int4 a)\n\n{\n  if (a == 0) goto code_r0x000180099458;\n  if (a == 1) {\ncode_r0x000180099458:\n    return 5;\n  }\n"
        "  if (a == 2) {\ncode_r0x000180099458:\n    return 6;\n  }\n  return 0;\n}");
    CB_CHECK(x64_has(got, "\ncode_r0x000180099458:\n    return 5;"));
    CB_CHECK(x64_has(got, "\ncode_r0x000180099458_2:\n    return 6;"));
});

CB_TEST("decompile.c_text_x64.a_function_returning_a_pointer_to_an_array_returns_a_pointer_to_its_first_element", [] {
    const std::string got = ct::x64_function_text(
        "/* WARNING: a note */\n\nxunknown1 (*) [16] strrchr(xunknown1 (*param_1) [16],uint1 param_2)\n\n{\n  return param_1;\n}");
    CB_CHECK(x64_has(got, "\nxunknown1 *strrchr(xunknown1 (*param_1) [16],uint1 param_2)\n"));
});

CB_TEST("decompile.c_text_x64.crc32_is_the_intrinsic_as_wide_as_its_value", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(uint4 param_3,xunknown8 *pxVar1)\n\n{\n  xunknown1 xVar8;\n  uint8 uVar11;\n  \n  param_3 = crc32(param_3,xVar8);\n"
        "  uVar11 = crc32(uVar11 & 0xffffffff,*pxVar1);\n  return uVar11;\n}");
    CB_CHECK(x64_has(got, "param_3 = ((uint8)_mm_crc32_u8((unsigned int)(param_3), (unsigned char)(xVar8)));"));
    CB_CHECK(x64_has(got, "uVar11 = ((uint8)_mm_crc32_u64((unsigned long long)(uVar11 & 0xffffffff), (unsigned long long)(*pxVar1)));"));
});

CB_TEST("decompile.c_text_x64.bytes_of_a_16_byte_value_shifted_right_are_the_headers_helpers", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(void)\n\n{\n  xunknown1 axVar8 [16];\n  \n  return (uint2)(SUB161(axVar8 >> 7,0) & 1) | (uint2)(SUB161(axVar8 >> 0xf,0) & 1) << 1;\n}");
    CB_CHECK(x64_has(got, "return (uint2)(((uint1)__cb_bytes128(__cb_shr128(__cb_ld128(axVar8), 7), 0)) & 1) | (uint2)(((uint1)__cb_bytes128(__cb_shr128(__cb_ld128(axVar8), 15), 0)) & 1) << 1;"));
});

CB_TEST("decompile.c_text_x64.parts_of_variables_nested_followed_or_wider_than_8_bytes", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(void)\n\n{\n  xunknown8 xStack_1b8;\n  xunknown1 axStack_38 [16];\n  xunknown1 axStack_98 [16];\n  xunknown1 axVar3 [16];\n  \n"
        "  xStack_1b8._0_6_ = CONCAT24(xStack_1b8._4_2_ + 1,(xunknown4)xStack_1b8);\n  axStack_98._4_12_ = axVar3._4_12_;\n"
        "  return *axStack_38._8_8_;\n}");
    CB_CHECK(x64_has(got, "__cb_wr6(((uint1 *)&xStack_1b8 + 0), CONCAT24((*(uint2 *)((uint1 *)&xStack_1b8 + 4)) + 1,(xunknown4)xStack_1b8));"));
    CB_CHECK(x64_has(got, "  __movsb((uint1 *)&axStack_98 + 4, (uint1 *)&axVar3 + 4, 12);\n"));
    CB_CHECK(x64_has(got, "return *(*(uint8 **)((uint1 *)&axStack_38 + 8));"));
});

CB_TEST("decompile.c_text_x64.a_case_label_at_a_blocks_end_gets_a_statement", [] {
    const std::string got = ct::x64_function_text("uint8 f(int4 a)\n\n{\n  switch(a) {\n  case 6:\n  case 0xc:\n  }\n  return 0;\n}");
    CB_CHECK(x64_has(got, "  case 0xc:;\n  }"));
    CB_CHECK(x64_has(got, "  case 6:\n"));
});

CB_TEST("decompile.c_text_x64.a_16_byte_variable_given_a_product_is_given_its_halves", [] {
    const std::string got = ct::x64_function_text(
        "uint8 f(uint4 uVar6)\n\n{\n  xunknown1 axVar2 [16];\n  xunknown1 axStack_1b4 [4];\n  \n  axVar2 = ZEXT416(uVar6 - 1) * ZEXT816(0x18);\n"
        "  axStack_1b4 = (xunknown1  [4])0x0;\n  return 0;\n}");
    CB_CHECK(x64_has(got, "  *(uint8 *)axVar2 = __cb_mulx128(__cb_zx128((uint8)(uVar6 - 1)), __cb_zx128((uint8)(0x18)), 0); "
                          "*(uint8 *)((uint1 *)axVar2 + 8) = __cb_mulx128(__cb_zx128((uint8)(uVar6 - 1)), __cb_zx128((uint8)(0x18)), 8);\n"));
    CB_CHECK(x64_has(got, "  *(uint4 *)axStack_1b4 = (uint4)(0x0);\n"));
});
