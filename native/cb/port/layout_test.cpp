// Tests of cb/port/old_sizes, struct_layout and union_views: the sizes the old compilers gave, the layouts they made, and
// the unions the port lays out again. Expected values come from the Python original (reviewed).
#include "cb/core/testing.hpp"
#include "cb/port/old_sizes.hpp"
#include "cb/port/struct_layout.hpp"
#include "cb/port/union_views.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

const std::string kSrc =
    "struct s { int a; char *p; };\n"
    "union u { struct { char kind; int len; } a; struct { char *p; int q; } b; long l; };\n"
    "struct t { struct s one; char name[5]; far char *fp; int arr[3]; };\n";

std::string show(const StructLayout& l) {
    std::string out = std::to_string(l.size) + ":";
    for (const Member& m : l.members) out += " " + m.name + "@" + std::to_string(m.offset) + "+" + std::to_string(m.size);
    return out;
}

}  // namespace

CB_TEST("port.old_sizes.scalars", [] {
    CB_CHECK_EQ(scalar_size("int").value_or(0), 2u);
    CB_CHECK_EQ(scalar_size("unsigned long int").value_or(0), 4u);
    CB_CHECK_EQ(scalar_size("double").value_or(0), 8u);
    CB_CHECK_EQ(scalar_size("int16_t").value_or(0), 2u);
    CB_CHECK(!scalar_size("size_t"));
    CB_CHECK_EQ(old_scalar_size("signed").value_or(0), 2u);
    CB_CHECK(!old_scalar_size("int16_t"));   // (the original had no fixed-width names)
    CB_CHECK_EQ(scalar_sizes().size(), 23u);
    CB_CHECK_EQ(old_scalar_sizes().size(), 16u);
});

CB_TEST("port.old_sizes.array_count", [] {
    CB_CHECK_EQ(array_count("a").value_or(0), 1u);
    CB_CHECK_EQ(array_count("a[3]").value_or(0), 3u);
    CB_CHECK_EQ(array_count("v[ 2 ][3]").value_or(0), 6u);
    CB_CHECK_EQ(array_count("v[]").value_or(0), 1u);   // (no number: no dimension counted)
    CB_CHECK(!array_count("v[4294967296][4294967296][4294967296]"));   // (past 64 bits: no size)
});

CB_TEST("port.old_sizes.size_of", [] {
    const StructLayouts lay = lay_out_structs(std::vector<std::string>{kSrc}, 2, 2);
    CB_CHECK_EQ(size_of("int", "a[3]", "a", 2, lay).value_or(0), 6u);
    CB_CHECK_EQ(size_of("char", "*p", "p", 4, lay).value_or(0), 4u);          // (a pointer: the caller's size)
    CB_CHECK_EQ(size_of("register struct s", "x", "x", 2, lay).value_or(0), 4u);
    CB_CHECK(!size_of("struct q", "x", "x", 2, lay));                        // (a structure nobody defined)
    CB_CHECK_EQ(size_of("unsigned long int", "v[2][3]", "v", 2, lay).value_or(0), 24u);
    CB_CHECK_EQ(size_of("int", "(*f)()", "f", 2, lay).value_or(0), 2u);       // (a function pointer)
    const StructSizeFn only_s = [](std::string_view tag) -> std::optional<std::uint64_t> {
        if (tag == "struct s") return 4;
        return std::nullopt;
    };
    CB_CHECK_EQ(size_of("struct s", "x[2]", "x", 2, only_s).value_or(0), 8u);
});

CB_TEST("port.struct_layout.the_8086_layout", [] {
    const StructLayouts lay = lay_out_structs(std::vector<std::string>{kSrc}, 2, 2);
    CB_CHECK_EQ(show(lay.at("struct s")), "4: a@0+2 p@2+2");
    // a union's members all start at 0; a nested structure is one member of its size
    CB_CHECK_EQ(show(lay.at("union u")), "4: a@0+4 b@0+4 l@0+4");
    CB_CHECK_EQ(show(lay.at("union u.a")), "4: kind@0+1 len@2+2");   // (the int on an even offset: a byte of padding)
    // `far` before a name is taken off the declaration first (the type is char, the pointer is far: 2 bytes here, 4 with far=4)
    CB_CHECK_EQ(show(lay.at("struct t")), "18: one@0+4 name@4+5 fp@10+2 arr@12+6");
    const StructLayouts lay4 = lay_out_structs(std::vector<std::string>{kSrc}, 2, 4);
    CB_CHECK_EQ(lay4.at("struct t").find("fp")->size, 4u);
});

CB_TEST("port.struct_layout.the_new_layout_has_bigger_pointers", [] {
    const StructLayouts lay = lay_out_structs(std::vector<std::string>{kSrc}, 8, 8);
    CB_CHECK_EQ(show(lay.at("struct s")), "10: a@0+2 p@2+8");
    CB_CHECK_EQ(show(lay.at("union u")), "10: a@0+4 b@0+10 l@0+4");
    CB_CHECK_EQ(show(lay.at("struct t")), "30: one@0+10 name@10+5 fp@16+8 arr@24+6");
});

CB_TEST("port.struct_layout.forward_uses_settle_in_a_few_rounds", [] {
    // `struct a` uses `struct b`, defined after it: the first round does not know b's size
    const StructLayouts lay = lay_out_structs(std::vector<std::string>{"struct a { char c; struct b x; };\nstruct b { long l; int i; };\n"}, 2, 2);
    CB_CHECK_EQ(lay.at("struct b").size, 6u);
    CB_CHECK_EQ(show(lay.at("struct a")), "8: c@0+1 x@2+6");   // (as the Python gives: a's size is known the second round)
    CB_CHECK(lay_out_structs(std::vector<std::string>{}, 2, 2).empty());
});

CB_TEST("port.struct_layout.the_first_definition_of_a_tag_wins", [] {
    const StructLayouts lay = lay_out_structs(std::vector<std::string>{"struct a { int x; };", "struct a { long y, z; };"}, 2, 2);
    CB_CHECK_EQ(show(lay.at("struct a")), "2: x@0+2");
});

CB_TEST("port.union_views.split_view_lists", [] {
    CB_CHECK_EQ(split_view_lists("union { struct { char kind, flags; int n; } a; } u;\n"),
                "union { struct { char kind;\n        char flags; int n; } a; } u;\n");
    CB_CHECK_EQ(split_view_lists("struct { char a, b; } not_in_a_union;"), "struct { char a, b; } not_in_a_union;");
});

CB_TEST("port.union_views.views_meet_at_the_same_old_places", [] {
    const std::string un =
        "struct view {\n"
        "    union {\n"
        "        struct { char *p; int a; } x;\n"
        "        struct { char c; int w; } y;\n"
        "        long l;\n"
        "    } u;\n"
        "};\n";
    Notes notes;
    const std::string out = union_views(un, notes, "v.h");
    // the second view had nothing where the first has the pointer's room: a line of its own gives it (the Python's
    // placement, one line above the one-line view: kept)
    CB_CHECK_EQ(out,
                "struct view {\n"
                "    union {\n"
                "        struct { char *p; int a; } x;\n"
                "            char _old0[7];\t\t/* (where the other views have members) */\n"
                "        struct { char c; int w; } y;\n"
                "        long l;\n"
                "    } u;\n"
                "};\n");
    CB_CHECK_EQ(notes.size(), 1u);
    CB_CHECK(notes.entries().front().second == std::vector<std::string>{"v.h"});
});

CB_TEST("port.union_views.no_change_when_no_view_grows", [] {
    const std::string un = "union u { struct { char c; int i; } a; struct { int j; char d; } b; } v;\n";
    Notes notes;
    CB_CHECK_EQ(union_views(un, notes, "f.c"), un);
    CB_CHECK(notes.empty());
    // a union of plain members only has no views to meet
    CB_CHECK_EQ(union_views("union { char *p; long l; } z;\n", notes, "f.c"), "union { char *p; long l; } z;\n");
});

CB_TEST("port.union_views.a_view_with_nothing_where_another_has_a_pointer_gets_the_room", [] {
    const std::string un =
        "union u {\n"
        "    struct { char *p; int a; } x;\n"
        "    struct { char c;\n"
        "        int w; } y;\n"
        "    long l;\n"
        "};\n";
    Notes notes;
    CB_CHECK_EQ(union_views(un, notes, "f.c"),
                "union u {\n"
                "    struct { char *p; int a; } x;\n"
                "    struct { char c;\n"
                "        char _old0[7];\t\t/* (where the other views have members) */\n"
                "        int w; } y;\n"
                "    long l;\n"
                "};\n");
    CB_CHECK_EQ(notes.size(), 1u);
});

CB_TEST("port.union_views.a_byte_array_over_the_other_views_members_takes_their_room", [] {
    const std::string un =
        "union u {\n"
        "    struct {\n"
        "        char *p;\n"
        "        int a;\n"
        "    } x;\n"
        "    struct {\n"
        "        char tag[6];\n"
        "    } y;\n"
        "};\n";
    Notes notes;
    CB_CHECK_EQ(union_views(un, notes, "f.c"),
                "union u {\n"
                "    struct {\n"
                "        char *p;\n"
                "        int a;\n"
                "    } x;\n"
                "    struct {\n"
                "        char tag[12];\n"
                "    } y;\n"
                "};\n");
});

CB_TEST("port.union_views.a_structure_whose_size_changes_counts_as_a_view", [] {
    const std::string un =
        "union u {\n"
        "    struct { char a; int b; } x;\n"
        "    struct big y;\n"
        "};\n";
    SizeChanges sizes;
    sizes["struct big"] = {4, 12};
    Notes notes;
    // (the plain member `struct big y;` is a view at the start: the one-line view gets a byte of room above its line,
    //  as the Python places it)
    CB_CHECK_EQ(union_views(un, notes, "f.c", sizes),
                "union u {\n"
                "            char _old0[1];\t\t/* (where the other views have members) */\n"
                "    struct { char a; int b; } x;\n"
                "    struct big y;\n"
                "};\n");
    CB_CHECK_EQ(notes.size(), 1u);
});
