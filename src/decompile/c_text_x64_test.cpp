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

CB_TEST("decompile.c_text_x64.stack_variables_not_end_to_end_share_a_storage_as_wide_as_the_value", [] {
    // (user32: `_iStack_c8 = CONCAT13(..)` writes four bytes where the text names only the first two, and hands `&iStack_c8` on)
    const std::string got = ct::x64_function_text(
        "uint8 f(char a,char b)\n\n{\n  int2 iStack_c8;\n  int2 *piStack_c0;\n  int2 *piVar6;\n  \n"
        "  _iStack_c8 = CONCAT13(a,CONCAT12(b,0xffff));\n  piVar6 = &iStack_c8;\n  piStack_c0 = piVar6;\n  return iStack_c8;\n}");
    CB_CHECK(x64_has(got, "  uint4 __cb_frame_iStack_c8;\n  int2 *piStack_c0;\n"));
    CB_CHECK(x64_has(got, "  __cb_frame_iStack_c8 = CONCAT13(a,CONCAT12(b,0xffff));\n"));
    CB_CHECK(x64_has(got, "  piVar6 = &(*(int2 *)((uint1 *)&__cb_frame_iStack_c8 + 0));\n"));
    CB_CHECK(x64_has(got, "  return (*(int2 *)((uint1 *)&__cb_frame_iStack_c8 + 0));\n"));
    CB_CHECK(!x64_has(got, " _iStack_c8 ="));
    // an array and the variable after it, written as one (8 bytes: the second variable is in the storage too)
    const std::string two = ct::x64_function_text(
        "uint8 f(uint8 g)\n\n{\n  xunknown1 axStack_48 [4];\n  xunknown4 xStack_44;\n  \n  _axStack_48 = g;\n  return xStack_44;\n}");
    CB_CHECK(x64_has(two, "  uint8 __cb_frame_axStack_48;\n  \n  __cb_frame_axStack_48 = g;\n"));
    CB_CHECK(x64_has(two, "  return (*(xunknown4 *)((uint1 *)&__cb_frame_axStack_48 + 4));\n"));
    // a variable partly inside the value's bytes: left as it was
    const std::string partly = "uint8 f(uint8 g)\n\n{\n  xunknown2 xStack_48;\n  xunknown8 xStack_44;\n  \n  _xStack_48 = g;\n  return 0;\n}";
    CB_CHECK(x64_has(ct::x64_function_text(partly), "_xStack_48 = g;"));
});

CB_TEST("decompile.c_text_x64.bytes_of_the_frame_named_by_their_place_are_read_from_the_variables_there", [] {
    // (user32: `stack0xffffffffffffff44`, four bytes at -0xbc: the last two of a six-byte variable and the two-byte one after it)
    const std::string got = ct::x64_function_text(
        "uint8 f(uint4 param_2)\n\n{\n  unkbyte6 Stack_c0;\n  int2 iStack_ba;\n  \n"
        "  _Stack_c0 = CONCAT44(stack0xffffffffffffff44,param_2);\n  return iStack_ba;\n}");
    CB_CHECK(x64_has(got, "CONCAT44((((uint8)*(uint2 *)((uint1 *)&Stack_c0 + 4) << 0) | ((uint8)*(uint2 *)((uint1 *)&iStack_ba + 0) << 16)),param_2)"));
    CB_CHECK(!x64_has(got, "stack0x"));
});

CB_TEST("decompile.c_text_x64.the_gs_segments_base_is_the_thread_blocks_own_address", [] {
    const std::string got = ct::x64_function_text(
        "xunknown4 f(void)\n\n{\n  int8 unaff_GS_OFFSET;\n  \n  return *(xunknown4 *)(unaff_GS_OFFSET + 0x68);\n}");
    CB_CHECK(x64_has(got, "  int8 unaff_GS_OFFSET = (int8)__readgsqword(0x30);\n"));
    const std::string typed = ct::x64_function_text(
        "xunknown4 f(void)\n\n{\n  s_f_unaff_GS_OFFSET *unaff_GS_OFFSET;\n  \n  return unaff_GS_OFFSET->f_68;\n}");
    CB_CHECK(x64_has(typed, "  s_f_unaff_GS_OFFSET *unaff_GS_OFFSET = (s_f_unaff_GS_OFFSET *)__readgsqword(0x30);\n"));
});

CB_TEST("decompile.c_text_x64.a_system_call_stub_takes_its_arguments_and_makes_the_call_of_its_number", [] {
    // ntdll's NtClose: mov r10, rcx; mov eax, 0xf; test byte ptr [0x7ffe0308], 1; jne +3; syscall; ret; int 2eh; ret
    const std::uint8_t code[] = {0x4c, 0x8b, 0xd1, 0xb8, 0x0f, 0x00, 0x00, 0x00, 0xf6, 0x04, 0x25, 0x08, 0x03, 0xfe, 0x7f,
                                 0x01, 0x75, 0x03, 0x0f, 0x05, 0xc3, 0xcd, 0x2e, 0xc3};
    CB_CHECK(ct::system_call_number(cb::ByteView(code, sizeof code)) == std::optional<std::uint32_t>(0xf));
    // (a number whose low byte is mov eax's own, 0xb8: NtCreateLowBoxToken)
    const std::uint8_t b8[] = {0x4c, 0x8b, 0xd1, 0xb8, 0xb8, 0x00, 0x00, 0x00, 0xf6, 0x04, 0x25, 0x08, 0x03, 0xfe, 0x7f, 0x01, 0x75, 0x03, 0x0f, 0x05, 0xc3};
    CB_CHECK(ct::system_call_number(cb::ByteView(b8, sizeof b8)) == std::optional<std::uint32_t>(0xb8));
    const std::uint8_t none[] = {0x48, 0x8b, 0xc1, 0xc3};
    CB_CHECK(!ct::system_call_number(cb::ByteView(none, sizeof none)));
    const std::string got = ct::system_call_text(
        "xunknown8 NtClose(void)\n\n{\n  code *pcVar1;\n  xunknown8 xVar2;\n  \n  if ((uRam000000007ffe0308 & 1) == 0) {\n"
        "    syscall();\n    return 0xf;\n  }\n  pcVar1 = (code *)swi(0x2e);\n  xVar2 = (*pcVar1)();\n  return xVar2;\n}", 0xf);
    CB_CHECK(x64_has(got, "xunknown8 NtClose(uint8 param_1,uint8 param_2,uint8 param_3,uint8 param_4)\n"));
    CB_CHECK(x64_has(got, "    return system_call(0xf,param_1,param_2,param_3,param_4);\n  }\n"));
    CB_CHECK(x64_has(got, "{\n  __declspec(allocate(\".text\")) static const unsigned char system_call_0xf[] = "
                          "{0x4c, 0x8b, 0xd1, 0xb8, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc3};\n  code *pcVar1;\n"));
    CB_CHECK(!x64_has(got, "syscall()"));
    // a function with parameters of its own is not a stub: left
    const std::string other = "uint8 f(uint8 a)\n\n{\n  syscall();\n  return a;\n}";
    CB_CHECK(ct::system_call_text(other, 0x12) == other);
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

CB_TEST("decompile.c_text_x64.a_byte_of_16_bytes_reached_through_a_pointer_and_a_product_of_a_variable_with_itself", [] {
    const std::string got = ct::x64_function_text(
        "char f(xunknown1 (*param_4) [16])\n\n{\n  char cVar1;\n  \n  cVar1 = SUB161(*param_4,1);\n  return cVar1;\n}");
    CB_CHECK(x64_has(got, "cVar1 = ((uint1)__cb_bytes128(__cb_ld128(*param_4), 1));"));
    // (both halves are of the old value: computed before either is written)
    const std::string self = ct::x64_function_text(
        "uint8 f(uint4 uVar6)\n\n{\n  xunknown1 axVar1 [16];\n  \n  axVar1 = axVar1 * ZEXT416(uVar6);\n  return 0;\n}");
    CB_CHECK(x64_has(self, "{ uint8 __cb_lo = __cb_mulx128(__cb_ld128(axVar1), __cb_zx128((uint8)(uVar6)), 0); "
                           "uint8 __cb_hi = __cb_mulx128(__cb_ld128(axVar1), __cb_zx128((uint8)(uVar6)), 8); "
                           "*(uint8 *)axVar1 = __cb_lo; *(uint8 *)((uint1 *)axVar1 + 8) = __cb_hi; }"));
});

CB_TEST("decompile.c_text_x64.ntdlls_odd_casts_structures_numbers_and_code_pointers", [] {
    // a number given a structure (`(S)(x)`): the structure of the number's bytes
    const std::string cast = ct::x64_function_text(
        "uint8 f(int2 *piVar1)\n\n{\n  s_f_piVar9 sStack_80;\n  \n  sStack_80 = (s_f_piVar9)\n              ((uint8)(uint2)piVar1[2] << 0x20);\n  return 0;\n}");
    CB_CHECK(x64_has(cast, "sStack_80 = (*(s_f_piVar9 *)__cb_bits((uint8)((uint8)(uint2)piVar1[2] << 0x20)));"));
    // a structure held by value, used as the pointer it is
    const std::string value = ct::x64_function_text("uint8 f(s_f_puVar4 param_1)\n\n{\n  uint8 uVar3;\n  \n  uVar3 = *(uint8 *)param_1;\n  return uVar3;\n}");
    CB_CHECK(x64_has(value, "uVar3 = *(uint8 *)*(uint8 *)&param_1;"));
    // a function pointer moved by a number
    const std::string code = ct::x64_function_text(
        "uint8 f(void)\n\n{\n  code *pcVar3;\n  \n  if ((code *)0xfffffffffffffffd < pcVar3 + -1) {\n    return 1;\n  }\n  return 0;\n}");
    CB_CHECK(x64_has(code, "if ((code *)0xfffffffffffffffd < (code *)((uint1 *)pcVar3 + -1)) {"));
    // the SSE control register written
    const std::string csr = ct::x64_function_text("uint8 f(uint4 param_1)\n\n{\n  MXCSR = param_1;\n  return MXCSR;\n}");
    CB_CHECK(x64_has(csr, "  _mm_setcsr((unsigned int)(param_1));\n  return MXCSR;"));
    // a number followed as a pointer: to what it is compared with, the number staying one
    const std::string followed = ct::x64_function_text(
        "uint8 f(uint2 uVar6)\n\n{\n  xunknown8 xStack_50;\n  uint2 uStack_34;\n  \n  if (*xStack_50 == uVar6) {\n    uStack_34 = *xStack_50;\n  }\n"
        "  xStack_50 = xStack_50 + 1;\n  return uStack_34;\n}");
    CB_CHECK(x64_has(followed, "if ((*(uint2 *)xStack_50) == uVar6) {"));
    CB_CHECK(x64_has(followed, "uStack_34 = (*(uint2 *)xStack_50);"));
    CB_CHECK(x64_has(followed, "xStack_50 = xStack_50 + 1;"));
    // a piece of a member, and an unnamed field of a structure cast from an expression
    const std::string member = ct::x64_function_text(
        "uint8 f(int8 param_1,int8 param_2)\n\n{\n  s_f_sVar9 sVar9;\n  xunknown1 *pxVar21;\n  \n  sVar9.f_0._0_1_ = *(uint1 *)(param_2 + 0x20);\n"
        "  pxVar21 = &((s_f_puVar23 *)((int8)param_1 + 0xf0))->field_0x24;\n  return 0;\n}");
    CB_CHECK(x64_has(member, "(*(uint1 *)((uint1 *)&sVar9.f_0 + 0)) = *(uint1 *)(param_2 + 0x20);"));
    CB_CHECK(x64_has(member, "pxVar21 = ((xunknown1 *)((uint1 *)((int8)param_1 + 0xf0) + 0x24));"));
});

CB_TEST("decompile.c_text_x64.user32s_stack_pieces_of_chars_and_an_array_in_a_storage", [] {
    // (user32 sub_04D5C0: two chars after a two-byte variable, the four bytes written as one)
    const std::string chars = ct::x64_function_text(
        "uint8 f(char a)\n\n{\n  int2 iStack_c8;\n  char cStack_c6;\n  char cStack_c5;\n  \n  _iStack_c8 = CONCAT13(a,0xffff);\n  return cStack_c5;\n}");
    CB_CHECK(x64_has(chars, "cStack_c6 = (char)(__cb_piece >> 16); cStack_c5 = (char)(__cb_piece >> 24); }"));
    // (user32 sub_0890D0: a four-byte array and the variable after it in one storage; the array read as a number and given whole)
    const std::string array = ct::x64_function_text(
        "uint8 f(int8 param_3,uint8 g)\n\n{\n  xunknown1 axStack_48 [4];\n  xunknown4 xStack_44;\n  \n  _axStack_48 = g;\n"
        "  *(xunknown1 (*) [4])(param_3 + 0x16) = axStack_48;\n  _axStack_48 = (unkuint6)(uint4)axStack_48;\n  return xStack_44;\n}");
    CB_CHECK(x64_has(array, "__movsb((uint1 *)&(*(xunknown1 (*) [4])(param_3 + 0x16)), (uint1 *)&((*(xunknown1 (*)[4])((uint1 *)&__cb_frame_axStack_48 + 0))), 4);"));
    CB_CHECK(x64_has(array, "__cb_frame_axStack_48 = (unkuint6)(*(uint4 *)((uint1 *)&__cb_frame_axStack_48 + 0));"));
});
