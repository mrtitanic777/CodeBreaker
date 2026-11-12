// The old port.asm16 names (codebreaker::port::asm16::*, over py::) as the compat shim at
// native/typed/cpp/codebreaker/port/asm16.hpp gives them: conversion and forwarding to cb/port/asm16. Builds at /W3.
// listing_offsets (it runs the old emulator) lives in the old translation unit cb_tests cannot link; cb_shim_check compiles it.
// The behaviour of the new code is run by port.asm16.* in native/cb/port, against the Python's output; here the shim is run on
// the same small modules to show that what goes through the old records and back is the same.
#include "codebreaker/port/asm16.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace as16 = codebreaker::port::asm16;

py::str text_of(std::string_view name) { return py::str(::cb::fs::read_text(::cb::testing::data_path("port/testdata/asm16/" + std::string(name)))); }

const char* const kHeader =
    "struct node { struct node *next; int val; char *name; long big; };\n"
    "struct pair { int a; int b; };\n"
    "extern struct node *_gv;\n"
    "int _bytev;\n"
    "char *_buf;\n";

}  // namespace

CB_TEST("compat.port.asm16.the_source_reads_back_as_the_old_records", [] {
    const py::list<as16::LineP> lines = as16::parse(py::str("_TEXT SEGMENT BYTE PUBLIC 'CODE'\n PUBLIC _f\n_f PROC NEAR\nlab: mov ax, [bx+2] ; c\n rep movsb\n"));
    CB_CHECK(lines.size() == 6);
    CB_CHECK(lines[0]->directive && lines[0]->directive->kind == py::str("SEGMENT") && *lines[0]->directive->name == py::str("_TEXT"));
    CB_CHECK(lines[3]->label && *lines[3]->label == py::str("lab") && *lines[3]->op == py::str("mov"));
    CB_CHECK(lines[3]->args.size() == 2 && lines[3]->args[1] == py::str("[bx+2]"));
    CB_CHECK(lines[4]->prefix && *lines[4]->prefix == py::str("rep"));
    CB_CHECK(as16::code_segments(lines).contains(py::str("_TEXT")));
    CB_CHECK(as16::number(py::str("0ffh")) == 255 && !as16::number(py::str("x")));
    CB_CHECK(as16::strip_comment(py::str("mov ax, 1 ; c")) == py::str("mov ax, 1 "));
    CB_CHECK(as16::split_ops(py::str("a, [b,c]")).size() == 2);
    CB_CHECK(as16::CALL_WORDS.contains(py::str("pascal")));
    bool threw = false;
    try {
        as16::parse(py::str("rep"));
    } catch (const as16::Asm16Error&) {   // (a prefix with nothing after it: the module's own error, not an IndexError)
        threw = true;
    }
    CB_CHECK(threw);
});

CB_TEST("compat.port.asm16.ctypes_and_functions_through_the_old_records", [] {
    py::list<py::str> texts;
    texts.append(py::str(kHeader));
    const as16::CTypes ct(texts, texts);
    CB_CHECK(ct.is_pointer(py::str("_gv")) && !ct.is_pointer(py::str("_bytev")));
    CB_CHECK(*ct.pointee(py::str("_buf")) == py::str("char"));
    CB_CHECK(ct.var(py::str("_gv")) && std::get<0>(*ct.var(py::str("_gv"))) == py::str("struct node"));
    const as16::Layout* old = ct.old.get(py::str("struct node"));
    CB_CHECK(old && old->size == 10);   // (a pointer, an int, a pointer, a long: 2+2+2+4)
    CB_CHECK(old->members.size() == 4 && old->members[2].first == py::str("name") && old->members[2].second.isptr);
    const as16::Layout* now = ct.new_.get(py::str("struct node"));
    CB_CHECK(now && now->size == 22);   // (8 + 2 + 8 + 4: a word needs an even place only)
    const auto f = ct.field(py::str("struct node"), 4);
    CB_CHECK(f && std::get<0>(*f) == py::str("name") && std::get<1>(*f) == 4 && std::get<3>(*f) == 10);
    CB_CHECK(std::get<5>(*f) && std::get<6>(*f) == py::str("char"));
    CB_CHECK(std::get<0>(*ct.elem_sizes(py::str("_gv"))) == 2 && std::get<1>(*ct.elem_sizes(py::str("_gv"))) == 8);
    CB_CHECK(*ct.member_type(py::str("struct node"), 0) == py::str("struct node *"));
    // the layouts of other texts, as the old callers ask for them
    const as16::Layouts both = as16::CTypes::_layouts(texts, 2, 4);
    CB_CHECK(both.contains(py::str("struct pair")) && both.at(py::str("struct pair")).size == 4);
    // a CTypes made by hand (old and new set as pre_ansi's cast_twins does) answers `field` too
    as16::CTypes by_hand;
    by_hand.old = both;
    by_hand.new_ = as16::CTypes::_layouts(texts, 8, 8);
    CB_CHECK(by_hand.field(py::str("struct pair"), 2) && std::get<0>(*by_hand.field(py::str("struct pair"), 2)) == py::str("b"));
    py::list<py::str> src;
    src.append(py::str("#define NEAR near pascal\nint NEAR add(a, b)\nint a, b;\n{\n}\nchar *getp(p)\nchar *p;\n{\n}\n"));
    const py::dict<py::str, as16::CSig> fns = as16::c_functions(src);
    CB_CHECK(fns.contains(py::str("add")) && fns.at(py::str("add")).pascal && fns.at(py::str("add")).args.size() == 2);
    CB_CHECK(fns.at(py::str("getp")).ret == py::str("ptr") && *fns.at(py::str("getp")).to == py::str("char"));
});

CB_TEST("compat.port.asm16.a_module_ported_through_the_old_port", [] {
    // the synthetic module m2, plain: the text the Python made of it is the golden
    const py::str synth = text_of("synth.txt");
    py::list<py::str> calls;
    for (const char* c : {"walk", "sum", "lenp", "strx", "far1", "dosit", "p2", "p3", "bad", "q", "q5", "cfun", "other"}) calls.append(py::str(c));
    // (the synthetic header differs from kHeader, so the harness's case is built from its own text)
    const std::string all = synth.utf8();
    std::vector<std::string> parts;
    for (std::size_t pos = 0;;) {
        const std::size_t at = all.find("\n@@@@\n", pos);
        if (at == std::string::npos) {
            parts.push_back(all.substr(pos));
            break;
        }
        parts.push_back(all.substr(pos, at - pos));
        pos = at + 6;
    }
    py::list<py::str> synth_hdr;
    synth_hdr.append(py::str(parts[5]));
    const as16::CTypes synth_ct(synth_hdr, synth_hdr);
    as16::Port m(as16::module(py::str("m2"), py::str(parts[1])), synth_ct, calls);
    CB_CHECK(m.public_.contains(py::str("_p2")) && m.labels.contains(py::str("_p2")));
    CB_CHECK(m.cname.at(py::str("_p2")) == py::str("p2"));
    m.run();
    const py::str out = m.emit();
    CB_CHECK(out.utf8() == ::cb::fs::read_text(::cb::testing::data_path("port/testdata/asm16/golden/synth_m2_0.asm")));
    CB_CHECK(m.unreached.size() == 0);
    const auto exit = m.exit_of(py::str("_p2"));
    CB_CHECK(exit && exit->r.count(py::str("ax")) == 1);
    // a rename the caller makes before run reaches the text (the program's own routine of a C library name, kept under its own name)
    as16::Port renamed(as16::module(py::str("m2"), py::str(parts[1])), synth_ct, calls);
    renamed.cname[py::str("_p2")] = py::str("p2_");
    renamed.run();
    CB_CHECK(renamed.emit().utf8().find("PUBLIC p2_") != std::string::npos);
});

CB_TEST("compat.port.asm16.a_module_the_port_cannot_carry_over_is_the_old_error", [] {
    py::list<py::str> empty;
    empty.append(py::str(""));
    const as16::CTypes ct(empty, empty);
    py::list<py::str> calls;
    calls.append(py::str("f"));
    const py::str src("_TEXT SEGMENT BYTE PUBLIC 'CODE'\n ASSUME CS:_TEXT\n PUBLIC _f\n_f PROC NEAR\n frobnicate ax\n ret\n_f ENDP\n_TEXT ENDS\n END\n");
    as16::Port m(as16::module(py::str("t"), src), ct, calls);
    m.run();
    bool old_error = false;
    try {
        (void)m.emit();
    } catch (const as16::Asm16Error& e) {
        old_error = std::string(e.what()).find("no rule for frobnicate") != std::string::npos;
    }
    CB_CHECK(old_error);
});
