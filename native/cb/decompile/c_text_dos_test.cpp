// Tests of a DOS program's C made compilable (c_text_dos.hpp).
#include "cb/decompile/c_text_dos.hpp"

#include <string>

#include "cb/core/testing.hpp"

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
    CB_CHECK_EQ(r.placed, std::size_t{2});
    const std::size_t lex = r.text.find("/* ---- lex.c ---- */"), trees = r.text.find("/* ---- trees.c ---- */");
    CB_CHECK(lex != std::string::npos && trees != std::string::npos && lex < trees);
    CB_CHECK(lex < r.text.find("\nvoid __cdecl16far sub_00100(void)\n") && r.text.find("\nvoid __cdecl16far sub_00100(void)\n") < trees);
    CB_CHECK(ctdt_has(r.text, "/* ---- (not placed in a source file) ---- */\n\nvoid __cdecl16far sub_00300(void)"));
    // without a layout, no sections
    CB_CHECK(!ctdt_has(compilable(text, "").text, "/* ---- "));
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
