// Tests of a DOS program's C made compilable (c_text_dos.hpp).
#include "decompile/c_text_dos.hpp"

#include <string>

#include "core/testing.hpp"

namespace cb::decompile::c_text_dos {
namespace {

bool ctdt_has(const std::string& text, const std::string& part) { return text.find(part) != std::string::npos; }

constexpr const char* kCtdtProgram =
    "/* T.EXE -- decompiled by CodeBreaker; not built, not judged. */\n"
    "\n"
    "#include <string.h>\n"
    "\n"
    "void __cdecl16far sub_00100(s_sub_00100_param_1 *param_1)\n"
    "\n"
    "{\n"
    "  xunknown2 unaff_DS;\n"
    "  xunknown2 in_BX;\n"
    "  int2 *piVar1;\n"
    "  \n"
    "  param_1->f_4 = in_BX;\n"
    "  *MK_FP(*(xunknown2 *)0x5cd4,(int2 *)0x122) = 1;\n"
    "  *MK_FP(param_1._2_2_,piVar1) = *(uint1 *)0x5cd4;\n"
    "  xRam000000fa = 0;\n"
    "  return;\n"
    "}\n"
    "\n"
    "void __cdecl16near sub_00200(void)\n"
    "\n"
    "{\n"
    "  xunknown2 xVar1;\n"
    "  \n"
    "  xVar1 = sub_00300();\n"
    "  sub_00100(0);\n"
    "  sub_00400(1,xVar1++);\n"
    "  sub_00400(1);\n"
    "  int21(xVar1);\n"
    "  return;\n"
    "}\n"
    "\n"
    "void __cdecl16near sub_00300(void)\n"
    "\n"
    "{\n"
    "  return;\n"
    "}\n"
    "\n"
    "void __cdecl16near sub_00400(int2 param_1)\n"
    "\n"
    "{\n"
    "  return;\n"
    "}\n";

CB_TEST("decompile.c_text_dos.the_program_gets_the_header_and_every_prototype_first", [] {
    const Result r = compilable(kCtdtProgram, "typedef short int2;\n");
    CB_CHECK(r.text.starts_with("/* T.EXE -- decompiled by CodeBreaker; not built, not judged. */\n\ntypedef short int2;\n"));
    CB_CHECK(ctdt_has(r.text, "#include <string.h>"));
    CB_CHECK_EQ(r.report.functions, std::size_t{4});
    // every function declared before the first is defined
    const std::size_t first_definition = r.text.find("sub_00100(s_sub_00100_param_1 *param_1)\n");
    CB_CHECK(r.text.find("void __cdecl16far sub_00100(s_sub_00100_param_1 *param_1);") < first_definition);
    CB_CHECK(ctdt_has(r.text, "int2 int21();"));   // a helper nothing defines, declared without a prototype
});

CB_TEST("decompile.c_text_dos.far_pointers_pieces_and_memory_outside_the_program_are_c", [] {
    const Result r = compilable(kCtdtProgram, "");
    CB_CHECK(ctdt_has(r.text, "*((int2 __far *)codebreaker_far(g_5cd4,(uint2)(0x122))) = 1;"));
    // no cast on the offset: the pointer it is made of says what it points to
    CB_CHECK(ctdt_has(r.text, "*((int2 __far *)codebreaker_far(CODEBREAKER_PIECE2(param_1,2),(uint2)(piVar1)))"));
    CB_CHECK(ctdt_has(r.text, "#define xRam000000fa (*(xunknown2 __far *)codebreaker_far(0xf, 0xa))"));
    CB_CHECK(!ctdt_has(r.text, "MK_FP("));
    CB_CHECK(!ctdt_has(r.text, "._2_2_"));
});

CB_TEST("decompile.c_text_dos.a_data_segment_word_is_named_once_with_the_type_most_uses_give_it", [] {
    const Result r = compilable(kCtdtProgram, "");
    CB_CHECK_EQ(r.report.globals, std::size_t{1});
    CB_CHECK(ctdt_has(r.text, "#define g_5cd4 (*(") && ctdt_has(r.text, " __near *)0x5cd4)"));
    // one read each as a word and as a byte: the first read's type names it, the other reads through the name (the same bytes)
    CB_CHECK(ctdt_has(r.text, "#define g_5cd4 (*(xunknown2 __near *)0x5cd4)"));
    CB_CHECK(ctdt_has(r.text, "= (*(uint1 __near *)&g_5cd4);"));
});

CB_TEST("decompile.c_text_dos.unused_registers_go_and_structures_are_declared", [] {
    const Result r = compilable(kCtdtProgram, "");
    CB_CHECK(!ctdt_has(r.text, "unaff_DS;"));        // declared, never used
    CB_CHECK(ctdt_has(r.text, "xunknown2 in_BX;"));  // used: kept
    CB_CHECK_EQ(r.report.registers_dropped, std::size_t{1});
    CB_CHECK(ctdt_has(r.text, "typedef struct s_sub_00100_param_1 { uint1 pad_0[4]; xunknown2 f_4; } s_sub_00100_param_1;"));
});

CB_TEST("decompile.c_text_dos.calls_settle_each_functions_result_and_arguments", [] {
    const Result r = compilable(kCtdtProgram, "");
    // sub_00300 is written void, but its caller uses its result
    CB_CHECK(ctdt_has(r.text, "xunknown2 __cdecl16near sub_00300(void);"));
    // sub_00400's callers pass 2 and 1 arguments (the second does something, so it stays): defined the old way, declared
    // without a prototype
    CB_CHECK(ctdt_has(r.text, "void __cdecl16near sub_00400();"));
    CB_CHECK(ctdt_has(r.text, "void __cdecl16near sub_00400(param_1,param_2)\n  int2 param_1;\n  xunknown2 param_2;\n"));
    CB_CHECK_EQ(r.report.old_style, std::size_t{1});
});

CB_TEST("decompile.c_text_dos.functions_are_written_under_the_source_file_whose_data_they_read", [] {
    // two files' version strings at 100h and 200h of the data segment, a third at 300h (its data runs on: no votes);
    // segment A's functions read 120h and 130h (the first file's data), segment B's 210h and 220h (the second's)
    const std::string text =
        "/* T.EXE -- decompiled by CodeBreaker; not built, not judged. */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  *(int2 *)0x120 = *(int2 *)0x130;\n  return;\n}\n\n"
        "void __cdecl16far sub_00200(void)\n\n{\n  *(int2 *)0x210 = *(int2 *)0x220;\n  return;\n}\n\n"
        "void __cdecl16far sub_00300(void)\n\n{\n  *(int2 *)0x310 = 0;\n  return;\n}\n";
    Layout layout;
    layout.files = {{0x100, "lex.c"}, {0x200, "trees.c"}, {0x300, "strings.c"}};
    layout.segment_of = {{"sub_00100", 0x1000}, {"sub_00200", 0x2000}, {"sub_00300", 0x3000}};
    const Result r = compilable(text, "", layout);
    CB_CHECK_EQ(r.placed, std::size_t{3});   // (the third by the link order: the one file after trees.c)
    const std::size_t lex = r.text.find("/* ---- lex.c ---- */"), trees = r.text.find("/* ---- trees.c ---- */");
    CB_CHECK(lex != std::string::npos && trees != std::string::npos && lex < trees);
    CB_CHECK(lex < r.text.find("\nvoid __cdecl16far sub_00100(void)\n") && r.text.find("\nvoid __cdecl16far sub_00100(void)\n") < trees);
    CB_CHECK(ctdt_has(r.text, "/* ---- strings.c ---- */\n\nvoid __cdecl16far sub_00300(void)"));
    // without a layout, no sections
    CB_CHECK(!ctdt_has(compilable(text, "").text, "/* ---- "));
    CB_CHECK(compilable(text, "").files.empty());
    // and the program as those files: the header every file includes, the files in link order, the rest last
    layout.stem = "t";
    const Result split = compilable(text, "", layout);
    CB_CHECK_EQ(split.files.size(), std::size_t{4});
    CB_CHECK_EQ(split.files[0].first, "t.h");
    CB_CHECK_EQ(split.files[1].first, "lex.c");
    CB_CHECK_EQ(split.files[2].first, "trees.c");
    CB_CHECK_EQ(split.files[3].first, "strings.c");
    CB_CHECK(ctdt_has(split.files[0].second, "#ifndef CODEBREAKER_T_H\n"));
    CB_CHECK(ctdt_has(split.files[0].second, "void __cdecl16far sub_00100(void);"));   // (every prototype)
    CB_CHECK(!ctdt_has(split.files[0].second, "\nvoid __cdecl16far sub_00100(void)\n"));
    CB_CHECK(ctdt_has(split.files[1].second, "#include \"t.h\"\n"));
    CB_CHECK(ctdt_has(split.files[1].second, "\nvoid __cdecl16far sub_00100(void)\n"));
    CB_CHECK(!ctdt_has(split.files[1].second, "sub_00200(void)\n"));
    CB_CHECK(ctdt_has(split.files[3].second, "\nvoid __cdecl16far sub_00300(void)\n"));
});

CB_TEST("decompile.c_text_dos.a_segment_no_data_places_is_the_file_the_link_order_puts_there", [] {
    // lex.c, trees.c, types.c linked in that order (their version strings at 100h, 200h, 300h; 400h ends the last's data);
    // A reads lex.c's data, C types.c's, B nothing: B is trees.c. D is in the library's segment: the run-time's code
    const std::string text =
        "/* T.EXE -- decompiled by CodeBreaker; not built, not judged. */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  *(int2 *)0x120 = *(int2 *)0x130;\n  return;\n}\n\n"
        "void __cdecl16far sub_00200(void)\n\n{\n  return;\n}\n\n"
        "void __cdecl16far sub_00300(void)\n\n{\n  *(int2 *)0x310 = *(int2 *)0x320;\n  return;\n}\n\n"
        "void __cdecl16far sub_00400(void)\n\n{\n  return;\n}\n";
    Layout layout;
    layout.stem = "t";
    layout.base = 0x10000;
    layout.files = {{0x100, "lex.c"}, {0x200, "trees.c"}, {0x300, "types.c"}, {0x400, "end.c"}};
    layout.segment_of = {{"sub_00100", 0x11000}, {"sub_00200", 0x12000}, {"sub_00300", 0x13000}, {"sub_00400", 0x14000}};
    layout.runtime_segments = {0x14000};
    const Result r = compilable(text, "", layout);
    std::map<std::string, std::string> files(r.files.begin(), r.files.end());
    CB_CHECK(files.contains("lex.c") && files.contains("trees.c") && files.contains("types.c") && files.contains("runtime.c"));
    CB_CHECK(!files.contains("t.c"));   // (every function placed)
    CB_CHECK(ctdt_has(files["trees.c"], "\nvoid __cdecl16far sub_00200(void)\n"));
    CB_CHECK(ctdt_has(files["trees.c"], "where the link order places them"));
    CB_CHECK(ctdt_has(files["runtime.c"], "\nvoid __cdecl16far sub_00400(void)\n"));
    // with two files between and one segment, which it is stays open: a file of its own, named by its segment
    layout.files = {{0x100, "lex.c"}, {0x200, "trees.c"}, {0x280, "msgs.c"}, {0x300, "types.c"}, {0x400, "end.c"}};
    const Result open = compilable(text, "", layout);
    std::map<std::string, std::string> open_files(open.files.begin(), open.files.end());
    CB_CHECK(open_files.contains("seg0200.c"));
    CB_CHECK(ctdt_has(open_files["seg0200.c"], "one of trees.c, msgs.c"));
});

CB_TEST("decompile.c_text_dos.a_pointer_to_a_string_of_the_data_segment_shows_the_string", [] {
    const std::string text =
        "/* T.EXE -- decompiled by CodeBreaker; not built, not judged. */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  getenv((xunknown1 *)0x4);\n  puts((xunknown1 *)0x6);\n  return;\n}\n";
    Layout layout;
    layout.data = std::string("\0\0\0\0MSC\0*/x\0", 13);   // a string at 4; at 6 the middle of one (not shown)
    const Result r = compilable(text, "", layout);
    CB_CHECK(ctdt_has(r.text, "getenv((xunknown1 *)0x4 /* \"MSC\" */);"));
    CB_CHECK(ctdt_has(r.text, "puts((xunknown1 *)0x6);"));
    CB_CHECK_EQ(r.report.strings, std::size_t{1});
});

CB_TEST("decompile.c_text_dos.callers_passing_different_words_get_a_variadic_prototype", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(int2 a1,...,xunknown2 param_3)\n\n{\n  sub_00100(a1, 2, param_3);\n  return;\n}\n\n"
        "void __cdecl16far sub_00200(void)\n\n{\n  sub_00100(1);\n  sub_00100(1, 2, 3);\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "void __cdecl16far sub_00100(int2 a1,...);\n"));
    // a word the body reads past the fixed ones: its place beside the first argument
    CB_CHECK(ctdt_has(r.text, "{\n  xunknown2 param_3 = *(xunknown2 *)((xunknown2 *)&a1 + 2);\n"));
    CB_CHECK(ctdt_has(r.text, "  sub_00100(1);\n  sub_00100(1, 2, 3);\n"));
});

CB_TEST("decompile.c_text_dos.a_call_passing_a_far_pointers_words_passes_the_pointer", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void *a1,int2 a3)\n\n{\n  return;\n}\n\n"
        "void __cdecl16far sub_00200(int2 a1,int2 a2)\n\n{\n  sub_00100(a1,a2,0x5 /* \",x\" */);\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "  sub_00100((void *)CONCAT22(a2,a1),0x5 /* \",x\" */);\n"));
    CB_CHECK(ctdt_has(r.text, "void __cdecl16far sub_00100(void *a1,int2 a3);\n"));   // (the comma in the comment is no argument)
});

CB_TEST("decompile.c_text_dos.a_four_byte_local_that_is_dereferenced_is_a_pointer", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(int2 a1,int2 a2)\n\n{\n  xunknown4 xStack_6;\n  xunknown4 xStack_a;\n  \n"
        "  xStack_6 = (char *)CONCAT22(a2,a1);\n  xStack_a = (char *)CONCAT22(a2,a1);\n  *xStack_6 = 0;\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "  char *xStack_6;\n"));
    CB_CHECK(ctdt_has(r.text, "  xunknown4 xStack_a;\n"));   // (never dereferenced: as it was)
    CB_CHECK_EQ(r.report.pointers, std::size_t{1});
});

CB_TEST("decompile.c_text_dos.a_local_read_through_a_cast_is_a_pointer_and_one_nothing_types_points_at_words", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  xunknown2 *a1;\n  xunknown4 xStack_6;\n  \n"
        "  a1 = (xunknown2 *)*xStack_6;\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "  xunknown2 *xStack_6;\n"));   // (`(T *)*x` is a cast and a read, not a product)
});

CB_TEST("decompile.c_text_dos.a_data_segment_word_read_after_a_cast_is_the_global", [] {
    // (in the large model a bare (T *)0x2564 is the far pointer 0000:2564, not the data segment's word: C1's putc wrote
    // through what low memory held there)
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  int2 x;\n  \n"
        "  x = (int2)((uint4)*(xunknown4 *)0x2564 >> 0x10) + (int2)*(xunknown4 *)0x2564;\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "x = (int2)((uint4)g_2564 >> 0x10) + (int2)g_2564;"));
    CB_CHECK(!ctdt_has(r.text, "(xunknown4 *)0x2564;"));
});

CB_TEST("decompile.c_text_dos.a_comparisons_result_is_a_word", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "bool __cdecl16far sub_00100(int2 a1)\n\n{\n  \n  return 0xee3U < (uint2)a1;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "int2 __cdecl16far sub_00100(int2 a1)\n"));
    CB_CHECK(!ctdt_has(r.text, "bool __cdecl16far"));
});

CB_TEST("decompile.c_text_dos.two_arguments_read_as_one_pointer_point_at_what_the_body_says", [] {
    // (strchr: `_a1 = (char *)...`, then `*_a1 == '\0'` -- one byte, not the word an xunknown2 pointer reads)
    const std::string text =
        "/* T.EXE */\n\n"
        "int4 __cdecl16far sub_00100(int2 a1,int2 a2)\n\n{\n  \n"
        "  while (*_a1 != '\\0') {\n    _a1 = (char *)CONCAT22(a2,(char __near *)(a1 + 1));\n  }\n"
        "  return CONCAT22(a2,a1);\n}\n";
    CB_CHECK(ctdt_has(compilable(text, "").text, "#define _a1 (*(char **)&a1)"));
});

CB_TEST("decompile.c_text_dos.a_function_that_takes_a_locals_address_keeps_the_originals_frame", [] {
    // (the long-multiply helper writes four bytes at &uStack_6: uStack_6 and uStack_4 must be side by side, low word first)
    const std::string text =
        "/* T.EXE */\n\n"
        "int4 __cdecl16far sub_00100(int2 a1)\n\n{\n  uint2 uStack_6;\n  uint2 uStack_4;\n  int2 unaff_SS;\n  \n"
        "  uStack_6 = 0;\n  uStack_4 = 0;\n  sub_00200((void *)CONCAT22(unaff_SS,(int2)&uStack_6),a1);\n"
        "  return CONCAT22(uStack_4,uStack_6);\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "#define uStack_6 (*(uint2 *)&codebreaker_frame[0])"));
    CB_CHECK(ctdt_has(r.text, "#define uStack_4 (*(uint2 *)&codebreaker_frame[2])"));
    CB_CHECK(ctdt_has(r.text, "  uint1 codebreaker_frame[4];"));
    CB_CHECK(!ctdt_has(r.text, "  uint2 uStack_6;"));
    CB_CHECK(ctdt_has(r.text, "#undef uStack_6"));
    CB_CHECK_EQ(r.report.frames, std::size_t{1});
    // (the stack segment, as the function was given it: the far pointer to the local is SS's)
    CB_CHECK(ctdt_has(r.text, "  int2 unaff_SS = (int2)codebreaker_ss();"));
    // (no address taken: the locals stay as they are)
    const std::string plain =
        "/* T.EXE */\n\n"
        "int2 __cdecl16far sub_00100(void)\n\n{\n  uint2 uStack_6;\n  \n  uStack_6 = 1;\n  return uStack_6;\n}\n";
    CB_CHECK(ctdt_has(compilable(plain, "").text, "  uint2 uStack_6;"));
});

CB_TEST("decompile.c_text_dos.the_compilers_stack_probe_is_not_the_sources", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  \n  _chkstk();\n  *(int2 *)0x50ac = 0x174;\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(!ctdt_has(r.text, "_chkstk();"));
    CB_CHECK(ctdt_has(r.text, "= 0x174;"));
    CB_CHECK_EQ(r.report.stack_probes, std::size_t{1});
});

CB_TEST("decompile.c_text_dos.a_far_offset_cast_then_stepped_keeps_its_cast", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void *a1,int2 a2)\n\n{\n  xunknown2 xVar1;\n  \n"
        "  xVar1 = *MK_FP(a2,(xunknown2 *)a1 + 1);\n  xVar1 = *MK_FP(a2,(xunknown2 *)a1);\n  return;\n}\n";
    const Result r = compilable(text, "");
    // `(xunknown2 *)a1 + 1` is the word after a1: the cast stays with what it steps through
    CB_CHECK(ctdt_has(r.text, "((xunknown2 __far *)codebreaker_far(a2,(uint2)((xunknown2 *)a1 + 1)))"));
    CB_CHECK(ctdt_has(r.text, "((xunknown2 __far *)codebreaker_far(a2,(uint2)(a1)))"));   // (one operand: the cast is the type)
});

CB_TEST("decompile.c_text_dos.a_double_read_through_as_a_pointer_is_the_accumulators_address", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  xunknown2 *pxVar1;\n  \n"
        "  pxVar1 = (xunknown2 *)atof((char *)0x100);\n  return;\n}\n";
    Layout layout;
    layout.library_double = {"atof"};
    const Result r = compilable(text, "", layout);
    CB_CHECK(ctdt_has(r.text, "pxVar1 = (xunknown2 *)codebreaker_fac(atof((char *)0x100));"));
    CB_CHECK(ctdt_has(r.text, "static double codebreaker_fac_value;"));
    CB_CHECK(!ctdt_has(compilable(text, "").text, "codebreaker_fac"));
});

CB_TEST("decompile.c_text_dos.structure_fields_are_as_wide_as_the_decompiler_found_them", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(void)\n\n{\n  s_sub_00100_v *psVar1;\n  \n"
        "  psVar1 = (s_sub_00100_v *)*(xunknown2 *)0x10;\n  psVar1->f_a = 0;\n  psVar1->f_b = 1;\n  return;\n}\n";
    Layout layout;
    layout.structure_fields["s_sub_00100_v"] = {{0xa, "xunknown1"}, {0xb, "uint1"}};
    const Result r = compilable(text, "", layout);
    // (the byte at 0xb is its own field, not inside a word at 0xa)
    CB_CHECK(ctdt_has(r.text, "typedef struct s_sub_00100_v { uint1 pad_0[10]; xunknown1 f_a; uint1 f_b; } s_sub_00100_v;"));
    CB_CHECK(ctdt_has(compilable(text, "").text, "typedef struct s_sub_00100_v { uint1 pad_0[10]; xunknown2 f_a; } s_sub_00100_v;"));
});

CB_TEST("decompile.c_text_dos.a_library_call_passes_each_pointer_as_its_header_declares", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "void __cdecl16far sub_00100(xunknown1 *a1,uint1 *pxVar3)\n\n{\n"
        "  fwrite(a1,1,2,pxVar3);\n  fflush((FILE *)pxVar3);\n  fputs((xunknown1 *)0x31a,0);\n  iVar1 = exit(1);\n  return;\n}\n";
    Layout layout;
    layout.library_pointers = {{"fwrite", {"char *", "", "", "FILE *"}}, {"fflush", {"FILE *"}}, {"fputs", {"char *", "FILE *"}}};
    layout.library_void = {"exit"};
    const Result r = compilable(text, "", layout);
    CB_CHECK(ctdt_has(r.text, "  fwrite((char *)a1,1,2,(FILE *)pxVar3);\n"));
    CB_CHECK(ctdt_has(r.text, "  fflush((FILE *)pxVar3);\n"));
    CB_CHECK(ctdt_has(r.text, "  fputs((char *)0x31a,0);\n"));
    CB_CHECK(ctdt_has(r.text, "  exit(1);\n"));   // (it returns nothing: nothing is stored)
});

CB_TEST("decompile.c_text_dos.arguments_a_function_never_reads_are_not_passed_unless_they_do_something", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "int2 __cdecl16far sub_00100(void)\n\n{\n  return 1;\n}\n\n"
        "void __cdecl16far sub_00200(int2 a1,int2 a2)\n\n{\n  a1 = sub_00100();\n  a1 = sub_00100(a1,CONCAT22(a2,a1));\n"
        "  a1 = sub_00100(a2,sub_00100());\n  return;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "  a1 = sub_00100();\n  a1 = sub_00100();\n"));
    CB_CHECK(ctdt_has(r.text, "  a1 = sub_00100(a2,sub_00100());\n"));   // (a call among them: kept, the old way)
});

CB_TEST("decompile.c_text_dos.an_argument_pair_read_as_one_pointer_is_named_in_a_function_returning_a_pointer", [] {
    const std::string text =
        "/* T.EXE */\n\n"
        "xunknown2 * __cdecl16far sub_00100(int2 a1,int2 a2)\n\n{\n  int2 iVar1;\n  \n  return _a1;\n}\n";
    const Result r = compilable(text, "");
    CB_CHECK(ctdt_has(r.text, "#define _a1 (*(xunknown2 **)&a1)\nxunknown2 * __cdecl16far sub_00100(int2 a1,int2 a2)\n"));
});

}  // namespace
}  // namespace cb::decompile::c_text_dos
