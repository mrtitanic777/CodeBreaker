#include "cb/exact/draft/candidates.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;
using namespace cb::exact::draft;
namespace cand = cb::exact::draft::candidates;

// The module is held to the Python module it replaced on a recorded corpus (candidates_replay_test.cpp: the checked-in part
// in testdata/draft_candidates, the whole of it - 39,824 cases of every pass, the longs, the structs, the frame, the
// localizer, the byte fields read off real KRNL386/GDI code - with CB_CANDIDATES_REPLAY). The cases below say what a pass is for,
// each on the smallest text that shows it.
//
// DIVERGES from the Python module (draft/candidates.py), on purpose:
//   * the module's tables (FPINFO, STRUCTPTR, FRAMEW, FRAME, FILLNAME, and text.py's ARRAYS, UCHAR, LONGINFO ...) are a State held
//     by the caller; FRAME is an argument of fill_frame and candidates; FILLNAME is FillerName
//   * a library macro whose `as` begins with no name (`$%s`) has no name to leave out of the declarations (the Python raised
//     AttributeError for the whole localize)
//   * a number that is the place of a string the data never ends (no zero byte before the image's end) is no string: the
//     Python cut the last byte off such a run (find returned -1)
//   * byte_fields gives nothing - and leaves the caller's tables as they were - when the checker declines, as the Python did; the
//     tables it fills (the frame slots, the ranges) are in its result instead of module tables
//   * a number in an instruction is read as Python's int(x, 0): a leading zero is no number (it raised ValueError, which the range
//     scan swallowed)

namespace {

constexpr const char* kLmul = "int f(a, b)\nint a, b;\n{\n    int v4;\n    v4 = lmul(a, b);\n    return v4;\n}";

cand::State fresh() { return cand::State{}; }

}  // namespace

CB_TEST("exact.draft.candidates.long_helpers_are_the_operators", [] {
    const std::map<std::string, std::string> helpers = {{"lmul", "*"}, {"almul", "*="}, {"ulless", "<"}};
    CB_CHECK_EQ(cand::long_helpers("    x = lmul(a, b);", helpers), "    x = (a * b);");
    CB_CHECK_EQ(cand::long_helpers("    almul(&x, 3);", helpers), "    x *= 3;");   // (an assigning one, on its own line: no parentheses)
    CB_CHECK_EQ(cand::long_helpers("    foo(lmul(a, b), 1);", helpers), "    foo((int)(a * b), 1);");   // (an argument pushed as one word: its low half)
    CB_CHECK_EQ(cand::long_helpers("    x = lmul(a, b);", {}), "    x = lmul(a, b);");
    CB_CHECK_EQ(cand::long_helpers(kLmul, helpers).find("v4 = (a * b);") != std::string::npos, true);
});

CB_TEST("exact.draft.candidates.the_8087s_conversion_is_cs_own", [] {
    cand::State s = fresh();
    CB_CHECK_EQ(cand::fp_conversions("    x = ftol(foo(2));", {"ftol", "__ftol"}, s), "    x = foo(2);");
    // the high half too: a long through a pointer
    CB_CHECK_EQ(cand::fp_conversions("    p->f0 = ftol(y);\n    p->f2 = dx;", {"ftol"}, s), "    *p = y;");
    CB_CHECK_EQ(s.fp.types.at("*p"), "long");
    CB_CHECK_EQ(cand::fp_conversions("    x = ftol(y);", {}, s), "    x = ftol(y);");
});

CB_TEST("exact.draft.candidates.types_the_8087_showed_are_declared", [] {
    FpInfo fp;
    fp.types["v2"] = "double";
    fp.types["*p"] = "float";
    fp.ret = "double";
    const std::string src = "f(p)\nint p;\n{\n    int v2;\n    return v2;\n}";
    CB_CHECK_EQ(cand::fp_types(src, fp), "double\nf(p)\nfloat *p;\n{\n    double v2;\n    return v2;\n}");
    CB_CHECK_EQ(cand::fp_types(src, FpInfo{}), src);
});

CB_TEST("exact.draft.candidates.a_function_that_gives_dx_returns_a_long", [] {
    const std::string src = "f(a)\nint a;\n{\n    int v2;\n    int v4;\n    dx = v2;\n    return v4;\n}";
    CB_CHECK_EQ(cand::long_returns(src), "long\nf(a)\nint a;\n{\n    long v4;\n    return v4;\n}");
    // dx = 0 before the only return: the value widened - C does that itself, the return keeps its type
    CB_CHECK_EQ(cand::long_returns("f(a)\n{\n    dx = 0;\n    return a;\n}"), "long\nf(a)\n{\n    return a;\n}");
    // one way out without dx: no long
    CB_CHECK_EQ(cand::long_returns("f(a)\n{\n    if (a) {\n        return 1;\n    }\n    dx = 0;\n    return a;\n}"), "f(a)\n{\n    if (a) {\n        return 1;\n    }\n    dx = 0;\n    return a;\n}");
});

CB_TEST("exact.draft.candidates.the_last_return_of_a_function_of_no_value", [] {
    CB_CHECK_EQ(*cand::void_variant("f()\n{\n    foo();\n    return bar(1);\n}"), "f()\n{\n    foo();\n    bar(1);\n}");
    CB_CHECK_EQ(*cand::void_variant("f()\n{\n    foo();\n    return 5;\n}"), "f()\n{\n    foo();\n\n}");   // (the statement goes, its line's end stays)
    CB_CHECK(!cand::void_variant("f()\n{\n    foo();\n}").has_value());
    CB_CHECK(!cand::void_variant("f()\n{\n    if (a) {\n        return 1;\n    }\n    foo();\n}").has_value());   // (a return inside the body, not its end)
});

CB_TEST("exact.draft.candidates.the_frames_unused_part_is_a_filler", [] {
    const std::string src = "f(a)\nint a;\n{\n    int x;\n    x = a;\n    return x;\n}";
    CB_CHECK_EQ(cand::fill_frame(src, 6), "f(a)\nint a;\n{\n    int x;\n    char _pf[4];\n    x = a;\n    return x;\n}");
    CB_CHECK_EQ(cand::fill_frame(src, 6, true, cand::FillerName::ByPlace), "f(a)\nint a;\n{\n    int x;\n    char _p2[4];\n    x = a;\n    return x;\n}");
    CB_CHECK_EQ(cand::fill_frame(src, 2), src);   // (the declared locals fill it)
    CB_CHECK_EQ(cand::fill_frame(src, 0), src);
    CB_CHECK_EQ(cand::fill_frame("f()\n{\n    register int r_si;\n    return 0;\n}", 4, true), "f()\n{\n    register int r_si;\n    char _pf[2];\n    return 0;\n}");
    CB_CHECK_EQ(cand::fill_frame("f()\n{\n    register int r_si;\n    return 0;\n}", 4, false), "f()\n{\n    register int r_si;\n    char _pf[4];\n    return 0;\n}");
});

CB_TEST("exact.draft.candidates.locals_only_read_as_bytes_are_chars", [] {
    cand::State s = fresh();
    s.frame_slots = {{-2, cand::SlotWidth::Byte}, {4, cand::SlotWidth::Byte}, {-4, cand::SlotWidth::Byte}, {-4, cand::SlotWidth::ZeroExtended}, {6, cand::SlotWidth::Word}};
    const std::string src = "f(a, b)\nint a, b;\n{\n    int v2;\n    int v4;\n    return v2;\n}";
    CB_CHECK_EQ(cand::frame_chars(src, s), "f(a, b)\nint b;\nchar a;\n{\n    char v2;\n    unsigned char v4;\n    return v2;\n}");
});

CB_TEST("exact.draft.candidates.a_locals_address_makes_it_a_buffer", [] {
    const std::string src = "f()\n{\n    int v2;\n    int v10;\n    foo(&v10);\n    v10 = 1;\n    return v2;\n}";
    CB_CHECK_EQ(*cand::buffers(src), "f()\n{\n    int v2;\n    char v10[8];\n    foo(v10);\n    *(int *)v10 = 1;\n    return v2;\n}");
    CB_CHECK(!cand::buffers("f()\n{\n    int v2;\n    return v2;\n}").has_value());
});

CB_TEST("exact.draft.candidates.a_pair_of_words_is_one_long", [] {
    cand::State s = fresh();
    const std::string src = "f(a, b)\nint a, b;\n{\n    int v4;\n    v4 = *(long *)&a;\n    return v4;\n}";
    const std::string out = cand::long_pairs(src, s);
    CB_CHECK_EQ(out, "f(a)\nlong a;\n{\n    int v4;\n    v4 = a;\n    return v4;\n}");
    // the same text without the pair is left alone and records nothing
    cand::State t = fresh();
    CB_CHECK_EQ(cand::long_pairs("f(a)\n{\n    return a;\n}", t), "f(a)\n{\n    return a;\n}");
    CB_CHECK(t.facts.longs.fields.empty() && t.facts.longs.globals.empty());
});

CB_TEST("exact.draft.candidates.fields_through_a_pointer_make_its_struct", [] {
    cand::State s = fresh();
    const std::string out = cand::structs("f(p)\nint p;\n{\n    x = p->f2;\n    p->f4 = 1;\n}", {}, s);
    CB_CHECK_EQ(out, "struct s_p { char _0[2]; int f2; int f4; };\nf(p)\nstruct s_p *p;\n{\n    x = p->f2;\n    p->f4 = 1;\n}");
    CB_CHECK_EQ(s.struct_pointers.at("p"), "s_p");
    // a byte read where the code reads a byte through the pointer: a char there
    cand::State b = fresh();
    const std::string bytes = cand::structs("f(p)\nint p;\n{\n    x = p->f2;\n}", {{"p", 2}}, b);
    CB_CHECK_EQ(bytes, "struct s_p { char _0[2]; char f2; };\nf(p)\nstruct s_p *p;\n{\n    x = p->f2;\n}");
});

CB_TEST("exact.draft.candidates.register_locals_are_declared_in_register_order", [] {
    const cand::RegisterParams none;
    const std::vector<std::string> regs{"si", "di"};
    CB_CHECK_EQ(cand::register_variables("f(a)\nint a;\n{\n    x = *si;\n    di = 1;\n}", regs, none),
                "f(a)\nint a;\n{\n    register char *r_si;\n    register int r_di;\n    x = *r_si;\n    r_di = 1;\n}");
    // a register the code loads a parameter into at the start is that parameter, declared register
    cand::RegisterParams loaded;
    loaded.insert_or_assign("si", 0);
    const std::string out = cand::register_variables("f()\n{\n    x = *si;\n}", regs, loaded);
    CB_CHECK_EQ(out, "f(a)\nregister char *a;\n{\n    x = *a;\n}");
});

CB_TEST("exact.draft.candidates.unsigned_locals_and_register_order_are_variants", [] {
    const std::string src = "f(x)\nint x;\n{\n    int v2;\n    int v4;\n    if ((v2 < x)) {\n        y = v4;\n    }\n}";
    const std::vector<std::string> u = cand::unsigned_locals(src);
    CB_CHECK_EQ(u.size(), 1u);   // only v2 is compared
    CB_CHECK_EQ(u[0], "f(x)\nint x;\n{\n    unsigned v2;\n    int v4;\n    if ((v2 < x)) {\n        y = v4;\n    }\n}");
    const std::string regs = "f()\n{\n    register int r_si;\n    int v2;\n    return 0;\n}";
    CB_CHECK_EQ(*cand::registers_last(regs, true), "f()\n{\n    int v2;\n    register int r_si;\n    return 0;\n}");
    CB_CHECK(!cand::registers_last(regs, false).has_value());
    CB_CHECK(!cand::registers_last("f()\n{\n    int v2;\n    register int r_si;\n    return 0;\n}", true).has_value());   // (already last)
});

CB_TEST("exact.draft.candidates.a_string_the_program_has_at_a_place_is_that_string", [] {
    Bytes image(0x20, 0);
    const std::string s = "hello";
    std::copy(s.begin(), s.end(), image.begin() + 12);   // at 12, after a zero byte, ended by one
    cand::Settings settings;
    settings.image = image;
    settings.data_segment = 0;
    const std::vector<std::string> v = cand::string_variants(settings, "f()\n{\n    puts(12);\n}");
    CB_CHECK_EQ(v.size(), 1u);
    if (v.empty()) return;
    CB_CHECK_EQ(v[0], "f()\n{\n    puts(\"hello\");\n}");
    // a run that no zero byte ends within the image is no string
    Bytes open_ended(0x14, 0);
    std::copy(s.begin(), s.end(), open_ended.begin() + 15);
    settings.image = open_ended;
    CB_CHECK(cand::string_variants(settings, "f()\n{\n    puts(15);\n}").empty());
});

CB_TEST("exact.draft.candidates.data_names_are_the_programs_by_place", [] {
    SymbolTable symbols;
    symbols.insert_or_assign("_buf", SymPlace{SymKind::Data, 0x2000});
    symbols.insert_or_assign("g_0010", SymPlace{SymKind::Data, 0x10});   // (its own name: nothing to say)
    symbols.insert_or_assign("_f", SymPlace{SymKind::Code, 0x1234});
    symbols.insert_or_assign("sym:odd", SymPlace{SymKind::Data, 0x400});
    symbols.insert_or_assign("a b", SymPlace{SymKind::Data, 0x500});
    symbols.insert_or_assign("__x", SymPlace{SymKind::Data, 0x600});
    const auto names = cand::data_names_of(symbols);
    CB_CHECK_EQ(names.size(), 2u);
    CB_CHECK_EQ(names.at(0x2000), "buf");
    CB_CHECK_EQ(names.at(0x600), "x");   // (every leading underscore goes)
});

namespace {

class FixedModel final : public cand::MarkingModel {
public:
    explicit FixedModel(std::optional<std::string> text) : text_(std::move(text)) {}
    std::optional<std::string> marked_c(ByteView) const override { return text_; }

private:
    std::optional<std::string> text_;
};

}  // namespace

CB_TEST("exact.draft.candidates.byte_fields_read_the_model_and_the_code", [] {
    // push bp; mov bp, sp; mov al, [bp - 4]; xor ah, ah; mov bx, [bp + 4]; mov al, [bx + 2]; mov dx, [si]; pop bp; ret
    const Bytes code = {0x55, 0x8B, 0xEC, 0x8A, 0x46, 0xFC, 0x32, 0xE4, 0x8B, 0x5E, 0x04, 0x8A, 0x47, 0x02, 0x8B, 0x14, 0x5D, 0xC3};
    const FixedModel model("x = BYTE(p->f2);\nBYTE(*q)");
    const auto found = cand::byte_fields(&model, code);
    CB_CHECK(found.has_value());
    CB_CHECK(found->sites.contains(text::Site{"p", 2}));
    CB_CHECK(found->sites.contains(text::Site{"q", 0}));
    CB_CHECK(found->sites.contains(text::Site{"bx*", 2}));   // (a byte through bx at 2)
    CB_CHECK(found->sites.contains(text::Site{"*", 2}));
    CB_CHECK(found->sites.contains(text::Site{"siw", 0}));   // (the word through si, at 0: its own width, apart from the bytes)
    CB_CHECK(found->frame_slots.contains(cand::FrameSlot{-4, cand::SlotWidth::ZeroExtended}));   // (al loaded from [bp-4], ah cleared)
    // no checker, or one that declines: nothing
    CB_CHECK(!cand::byte_fields(nullptr, code).has_value());
    const FixedModel declines{std::nullopt};
    CB_CHECK(!cand::byte_fields(&declines, code).has_value());
});

CB_TEST("exact.draft.candidates.a_function_with_nothing_to_rewrite_is_its_own_first_candidate", [] {
    cand::Settings settings;
    const cand::RegisterParams none;
    cand::State state = fresh();
    text::ProjectCaches caches;
    const auto out = cand::candidates(settings, "f()\n{\n    return foo();\n}", 0x1000, {}, {"si", "di"}, none, cand::ByteFields{}, 0, state, caches);
    CB_CHECK(!out.empty());
    CB_CHECK_EQ(out[0].source, "extern foo();\n\nf()\n{\n    return foo();\n}");
    CB_CHECK(out[0].registers_left.empty());
});

CB_TEST("exact.draft.candidates.localize_names_the_routines_and_the_data", [] {
    cand::Settings settings;
    cand::State state = fresh();
    text::ProjectCaches caches;
    const std::map<std::uint32_t, std::string> names = {{0x1234, "foo"}};
    // a call to 0x1000 + 0x234 is the engine's foo, one to 0x1000 + 0x1000 only a place
    const auto a = cand::localize(settings, "f()\n{\n    x = sub_234();\n    y = sub_1000();\n    g_5 = ax;\n}", 0x1000, names, state, caches);
    CB_CHECK_EQ(a.source, "extern int g_0005;\nextern f02000(), foo();\n\nf()\n{\n    x = foo();\n    y = f02000();\n    g_0005 = ax;\n}");
    CB_CHECK((a.registers_left == std::vector<std::string>{"ax"}));   // (a register the model left: a draft for a person)
});

// (tests/test_port_fixes.py, the exact.draft.candidates case) Merging arrays of structs of one size into one array: the second
// localize of the same text on the same facts lays out the merged struct array as the first did. The Python kept its tables in
// module globals, so the second run read the field list of the first as a field's type and declared `[(0, 'int'), (4, 'int')] f0;`;
// here the tables are the State the caller holds, and the second run must be the first.
CB_TEST("exact.draft.candidates.localize_twice_lays_out_the_merged_struct_array_the_same", [] {
    cand::Settings settings;
    text::ProjectCaches caches;
    cand::State state = fresh();
    for (const char* name : {"g_1000", "g_1004"}) {
        state.facts.arrays[name] = std::string("struct s_") + name;
        state.facts.struct_arrays[name] = text::StructArray{8, "int", {}};
    }
    const std::string src = "int f(i)\nint i;\n{\n    int x;\n    x = g_1000[i].f0;\n    g_1004[i].f0 = x;\n    return x;\n}";
    const auto first = cand::localize(settings, src, 0, {}, state, caches);
    const auto again = cand::localize(settings, src, 0, {}, state, caches);
    CB_CHECK(first.source.find("struct s_g_1000 { int f0; char _2[2]; int f4; char _6[2]; };") != std::string::npos);
    CB_CHECK_EQ(again.source, first.source);
});
