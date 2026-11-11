// The old port.pre_ansi PART 5 functions (codebreaker::port::pre_ansi::*, over py::) as the compat shim at
// native/typed/cpp/codebreaker/port/pre_ansi_p5.hpp gives them: conversion and forwarding to cb/port. Builds at /W3.
// The Port-taking functions (mend, stored_wide, passed_on) need the old Port, whose constructor is in the old
// translation units that cb_tests cannot link; they are compiled by cb_shim_check (pre_ansi.cpp includes the shim) and
// their new halves are run by port.mend.* and port.pre_ansi_p5.golden.* in native/cb/port.
#include "codebreaker/port/pre_ansi.hpp"

#include "cb/core/testing.hpp"

namespace {

namespace pa = codebreaker::port::pre_ansi;

}  // namespace

CB_TEST("compat.port.pre_ansi_p5.call_at_and_ctype", [] {
    const auto at = pa::call_at(py::str("  r = outer(a, inner(b, c), d);"), 21);
    CB_CHECK(at.has_value());
    CB_CHECK(std::get<0>(*at) == py::str("inner") && std::get<1>(*at) == 0);
    CB_CHECK(std::get<0>(std::get<2>(*at)) == 21 && std::get<1>(std::get<2>(*at)) == 22);
    CB_CHECK(!pa::call_at(py::str("  r = 1;"), 2).has_value());
    CB_CHECK(pa::ctype(py::str("foo *"), py::set<py::str>{py::str("foo")}) == py::str("struct foo *"));
    CB_CHECK(pa::ctype(py::str("  const   char  *  "), py::set<py::str>()) == py::str("const char *"));
});

CB_TEST("compat.port.pre_ansi_p5.text_passes_merge_their_notes", [] {
    pa::Notes notes;
    const py::str out = pa::stdio_inits(py::str("FILE *out = stdout;\n"), notes, py::str("a.c"));
    CB_CHECK(out == py::str("FILE *out;\nDOS_INIT(dos_init_out) { out = stdout; }\n"));
    CB_CHECK(notes.size() == 1);
    // notes already there are added to, not replaced
    const py::str again = pa::stdio_inits(py::str("FILE *e = stderr;\n"), notes, py::str("b.c"));
    CB_CHECK(again == py::str("FILE *e;\nDOS_INIT(dos_init_e) { e = stderr; }\n"));
    CB_CHECK(notes.size() == 1);
    CB_CHECK(notes.items().begin() != notes.items().end());
    for (const auto& kv : notes.items()) CB_CHECK(std::get<1>(kv).size() == 2);
    pa::Notes quiet;
    CB_CHECK(pa::file_internals(py::str("int x;"), quiet, py::str("a.c")) == py::str("int x;"));
    CB_CHECK(quiet.size() == 0);
    const py::str wide = pa::word_unions(py::str("union u { int16_t i; char *s; };"), quiet, py::str("a.c"), py::set<py::str>());
    CB_CHECK(wide == py::str("union u { intptr_t i; char *s; };"));
    CB_CHECK(pa::word_copies(py::str("union u { int16_t i; char *s; };"), quiet, py::str("a.c"), py::set<py::str>{py::str("i")}) ==
             py::str("union u { intptr_t i; char *s; };"));
    CB_CHECK(pa::block_heads(py::str("p = (char *)q + 4;"), quiet, py::str("a.c"), py::set<py::str>{py::str("q")}) ==
             py::str("p = (char *)q + DOS_HEAD(4);"));
    CB_CHECK(pa::whole_word_stores(py::str("void f(int16_t *q, intptr_t p)\n{\n    *(int *)q = p;\n}\n"), quiet, py::str("a.c")) ==
             py::str("void f(int16_t *q, intptr_t p)\n{\n    *(intptr_t *)q = p;\n}\n"));
});

CB_TEST("compat.port.pre_ansi_p5.whole_program_passes_write_back", [] {
    py::dict<py::str, py::str> out;
    out.set(py::str("a.c"), py::str("char *g = \"x\";\nvoid f() { long t; time(&t); }\n"));
    out.set(py::str("b.h"), py::str("char *h = \"y\";\n"));
    pa::Notes notes;
    pa::old_c_ways(out, notes);
    CB_CHECK(out.at(py::str("a.c")) == py::str("char *g = (char []){\"x\"};\nvoid f() { long t; (t = (int32_t)time(NULL)); }\n"));
    CB_CHECK(out.at(py::str("b.h")) == py::str("char *h = \"y\";\n"));   // only .c files
    CB_CHECK(notes.size() == 1);

    py::dict<py::str, py::str> files;
    files.set(py::str("a.c"), py::str("extern intptr_t g_74be;\n"));
    files.set(py::str("b.c"), py::str("extern int16_t g_74be;\n"));
    pa::Notes more;
    pa::agree_wide(files, py::list<py::str>(), more);
    CB_CHECK(files.at(py::str("b.c")) == py::str("extern intptr_t g_74be;"
                                                  "\n") ||
             files.at(py::str("b.c")) == py::str("intptr_t g_74be;\n"));
    pa::Notes places;
    py::dict<py::str, py::str> numbered;
    numbered.set(py::str("a.c"), py::str("void f() { g((struct s **)0x3a20); }\n"));
    pa::numbered_places(numbered, py::list<py::str>{py::str("struct s *g_3a20;")}, places);
    CB_CHECK(numbered.at(py::str("a.c")) == py::str("extern struct s *g_3a20;\nvoid f() { g((struct s **)g_3a20); }\n") ||
             numbered.at(py::str("a.c")) == py::str("void f() { g((struct s **)g_3a20); }\nextern struct s *g_3a20;\n"));
});

CB_TEST("compat.port.pre_ansi_p5.members_unions_and_tags", [] {
    const auto widened = pa::widen_member(py::str("struct list { int s4, s6; };"), py::str("list"), py::str("s6"));
    CB_CHECK(widened.has_value() && *widened == py::str("struct list { int s4; intptr_t s6; };"));
    CB_CHECK(!pa::widen_member(py::str("struct list { char *p; };"), py::str("list"), py::str("p")).has_value());
    py::dict<py::str, py::str> out;
    out.set(py::str("u.h"), py::str("union tokval { int32_t l; char *s; };\n"));
    out.set(py::str("a.c"), py::str("#include \"u.h\"\nstruct mine { int a; };\n"));
    CB_CHECK(pa::union_member_for(out, py::str("union tokval"), py::str("char *")).value() == py::str("s"));
    CB_CHECK(pa::union_member_for(out, py::str("union tokval"), py::str("long")).value() == py::str("l"));
    CB_CHECK(!pa::union_member_for(out, py::str("union none"), py::str("long")).has_value());
    const py::set<py::str> tags = pa::visible_tags(out, py::str("a.c"));
    CB_CHECK(tags.size() == 2 && tags.contains(py::str("mine")) && tags.contains(py::str("tokval")));
    CB_CHECK(pa::retyped_key(py::str("a.c"), py::str("f"), py::str("v")) == py::str("a.c\x01" "f\x01" "v"));
});

CB_TEST("compat.port.pre_ansi_p5.text_crosses_as_latin1_bytes", [] {
    // a character of the program's Latin-1 text is one byte on the new side and one code point on the old
    const py::str text = py::str(std::u32string(U"char *s = \"é\"; FILE *f = stdout;\n"));
    pa::Notes notes;
    const py::str out = pa::stdio_inits(py::str(std::u32string(U"FILE *f = stdout; /* é */\n")), notes, py::str("a.c"));
    CB_CHECK(out == py::str(std::u32string(U"FILE *f; /* é */\nDOS_INIT(dos_init_f) { f = stdout; }\n")));
    CB_CHECK(out.size() == py::str(std::u32string(U"FILE *f; /* é */\nDOS_INIT(dos_init_f) { f = stdout; }\n")).size());
    (void)text;
    CB_CHECK(pa::call_at(py::str(std::u32string(U"éé = f(1);")), 8).has_value());
});
