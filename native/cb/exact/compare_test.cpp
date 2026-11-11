#include "cb/exact/compare.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

Bytes hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

// A program with one code segment at image offset 0 and its data segment at 0x40, loaded at segment 0x10.
compare::Program program() {
    compare::Program p;
    p.dgroup = 0x40;
    p.loadseg = 0x10;
    p.code_base = [](std::uint32_t) { return 0u; };
    return p;
}

ObjectFile object(std::string_view code_hex) {
    ObjectFile o;
    o.seg.insert_or_assign("_TEXT", hex(code_hex));
    return o;
}

Bytes image_with(std::size_t size, std::size_t at, std::string_view bytes_hex) {
    Bytes img(size);
    const Bytes b = hex(bytes_hex);
    std::copy(b.begin(), b.end(), img.begin() + static_cast<std::ptrdiff_t>(at));
    return img;
}

bool has_note(const compare::Report& r, std::string_view needle, compare::Severity severity = compare::Severity::Error) {
    return std::ranges::any_of(r.notes, [&](const compare::Note& n) { return n.severity == severity && n.text.find(needle) != std::string::npos; });
}

}  // namespace

CB_TEST("exact.compare.a_call_to_a_name_is_learnt_then_held_to", [] {
    // push bp; mov bp, sp; call _foo; pop bp; ret -- the word of the call is the linker's
    ObjectFile o = object("558bece800005dc3");
    o.fix = {{"_TEXT", 4, FixKind::Rel, {TargetKind::External, "_foo"}, 0}};
    SymbolTable syms, learnt;
    const compare::Subject f{o, "main", 0, 8, 0x10};
    // the program calls the place 0x40 from 0x10: rel = 0x40 - (0x10 + 4 + 2)
    const compare::Report first = compare::function(f, image_with(0x50, 0x10, "558bece82a005dc3"), program(), syms, learnt);
    CB_CHECK(first.bad.empty() && first.notes.empty());
    CB_CHECK(first.identical());
    CB_CHECK_EQ(learnt.size(), 1u);
    CB_CHECK((*learnt.find("_foo") == SymPlace{SymKind::Code, 0x40}));
    CB_CHECK(syms.contains("_foo"));
    // another function of the program calling the same place from 0x30 agrees
    SymbolTable none;
    const compare::Subject g{o, "other", 0, 8, 0x30};
    const compare::Report again = compare::function(g, image_with(0x50, 0x30, "558bece80a005dc3"), program(), syms, none);
    CB_CHECK(again.identical() && none.empty());
    // one calling 4 bytes further on does not
    const compare::Report wrong = compare::function(g, image_with(0x50, 0x30, "558bece80e005dc3"), program(), syms, none);
    CB_CHECK(wrong.bad.empty() && !wrong.identical());   // (the bytes are the linker words: only the note says so)
    CB_CHECK(has_note(wrong, "_foo is C 0040 in SYMBOLS, here C 0044"));
});

CB_TEST("exact.compare.bytes_that_differ", [] {
    SymbolTable syms, learnt;
    ObjectFile o = object("b80500c3");
    const compare::Report r = compare::function({o, "f", 0, 4, 0}, image_with(8, 0, "b80700c3"), program(), syms, learnt);
    CB_CHECK((r.bad == std::vector<std::uint32_t>{1}));
    CB_CHECK(!r.identical());
    // a lone byte off by a constant amount is a constant that differs: said as a warning
    CB_CHECK(has_note(r, "1 value +2 in the program (+1)", compare::Severity::Warning));
    // bytes past the end of the image differ
    const compare::Report cut = compare::function({o, "f", 0, 4, 0}, image_with(3, 0, "b80500"), program(), syms, learnt);
    CB_CHECK((cut.bad == std::vector<std::uint32_t>{3}));
    // the same, but every byte
    const compare::Report same = compare::function({o, "f", 0, 4, 0}, image_with(4, 0, "b80500c3"), program(), syms, learnt);
    CB_CHECK(same.identical() && same.bad.empty());
});

CB_TEST("exact.compare.a_word_naming_the_files_own_data", [] {
    ObjectFile o = object("b80000c3");
    o.seg.insert_or_assign("_DATA", hex("68656c6c6f00776f726c6400"));   // "hello\0world\0"
    o.fix = {{"_TEXT", 1, FixKind::Off, {TargetKind::Segment, "_DATA"}, 0}};
    SymbolTable syms, learnt;
    // the program's data segment is at 0x40: the string is at DS:0x0010
    Bytes img = image_with(0x80, 0, "b81000c3");
    const Bytes hello = hex("68656c6c6f00");
    std::copy(hello.begin(), hello.end(), img.begin() + 0x40 + 0x10);
    const compare::Report ok = compare::function({o, "fn", 0, 4, 0}, img, program(), syms, learnt);
    CB_CHECK(ok.identical() && ok.notes.empty());
    CB_CHECK((*learnt.find("fn:_DATA+0") == SymPlace{SymKind::Data, 0x10}));
    // a different string there: the word points at the wrong thing
    img[0x40 + 0x10] = 'j';
    SymbolTable syms2, learnt2;
    const compare::Report bad = compare::function({o, "fn", 0, 4, 0}, img, program(), syms2, learnt2);
    CB_CHECK(has_note(bad, "_DATA+0 (b'hello\\x00') is not at DS:0010 (b'jello\\x00"));
    CB_CHECK(!bad.identical());
});

CB_TEST("exact.compare.a_segment_word", [] {
    ObjectFile o = object("b80000c3");
    o.fix = {{"_TEXT", 1, FixKind::Base, {TargetKind::Segment, "_TEXT"}, 0}};
    SymbolTable syms, learnt;
    // this code's own segment, loaded at 0x10: the word is 0x10
    CB_CHECK(compare::function({o, "f", 0, 4, 0x20}, image_with(0x40, 0x20, "b81000c3"), program(), syms, learnt).identical());
    CB_CHECK(has_note(compare::function({o, "f", 0, 4, 0x20}, image_with(0x40, 0x20, "b83400c3"), program(), syms, learnt), "segment 0034 is not this code's"));
    // another segment of the file: a word the loader sets, and what it is is learnt
    ObjectFile segmented = object("b80000c3");
    segmented.fix = {{"_TEXT", 1, FixKind::Base, {TargetKind::Segment, "FAR_DATA"}, 0}};
    SymbolTable s2, l2;
    CB_CHECK(compare::function({segmented, "f", 0, 4, 0}, image_with(8, 0, "b83412c3"), program(), s2, l2).identical());
    CB_CHECK((*l2.find("FAR_DATA@seg") == SymPlace{SymKind::Segment, 0x1234}));
    // with the program's relocation table, a word it does not list is not the loader's
    const std::unordered_set<std::uint32_t> relocs = {0x99};
    compare::Program with_table = program();
    with_table.relocs = &relocs;
    CB_CHECK(has_note(compare::function({segmented, "f", 0, 4, 0}, image_with(8, 0, "b83412c3"), with_table, s2, l2), "not a word the loader sets"));
});

CB_TEST("exact.compare.calls_between_functions_of_a_file_follow_where_each_went", [] {
    // A (0..5) calls B (8): `call B` has no fix-up, the compiler wrote it
    ObjectFile o = object("e80500" "90c3" "000000" "c3");   // A: call +5; nop; ret; padding; B: ret
    SymbolTable syms, learnt;
    // the program has A at 0x20 and B at 0x50: rel = 0x50 - (0x20 + 3)
    const Bytes img = image_with(0x60, 0x20, "e82d0090c3");
    const compare::Where where = [](std::uint32_t x) { return x == 0 ? 0x20 : 0x50; };
    const compare::Subject a{o, "a", 0, 5, 0x20};
    CB_CHECK(compare::function(a, img, program(), syms, learnt, where).identical());
    // the file's code is not at places of its own: the call is a difference
    CB_CHECK((compare::function(a, img, program(), syms, learnt).bad == std::vector<std::uint32_t>{1}));
});

CB_TEST("exact.compare.errors", [] {
    ObjectFile o = object("c3");
    SymbolTable syms, learnt;
    CB_CHECK_THROWS_AS(FormatError, (void)compare::function({o, "f", 0, 1, 0, "NO_SEGMENT"}, Bytes(4), program(), syms, learnt));
    CB_CHECK_THROWS_AS(FormatError, (void)compare::function({o, "f", 0, 9, 0}, Bytes(4), program(), syms, learnt));   // beyond the segment
    // a fix-up word the image does not have
    ObjectFile p = object("e80000");
    p.fix = {{"_TEXT", 1, FixKind::Rel, {TargetKind::External, "_x"}, 0}};
    CB_CHECK_THROWS_AS(TruncatedError, (void)compare::function({p, "f", 0, 3, 0}, Bytes(2), program(), syms, learnt));
});
