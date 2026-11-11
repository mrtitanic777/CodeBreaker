// Tests of cb/port/original_program (the program as its old compiler saw it) and cb/port/complaints. Expected values come
// from the Python original on the same inputs (reviewed).
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/complaints.hpp"
#include "cb/port/original_program.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

const char* const kH =
    "struct inode { int a; int b; };\n"
    "struct far_rec { char far *name; int n; };\n"
    "extern struct inode itab[10];\n"
    "extern char far *ftab[3];\n"
    "char names[2][5];\n"
    "int wide[7];\n"
    "struct inode one;\n";

const char* const kC =
    "#include \"x.h\"\n"
    "struct inode *ip;\n"
    "static char *alloc(n, size)\n"
    "int n, size;\n"
    "{\n"
    "    return grow(n, size);\n"
    "}\n"
    "grow(n, s)\n"
    "int n, s;\n"
    "{\n"
    "    return 0;\n"
    "}\n"
    "f()\n"
    "{\n"
    "    struct inode *q;\n"
    "    ip = (struct inode *)alloc(1, 4);\n"
    "    q = grow(2, 4);\n"
    "}\n";

Files program() {
    Files f;
    f.insert_or_assign("x.h", kH);
    f.insert_or_assign("x.c", kC);
    return f;
}

}  // namespace

CB_TEST("port.original_program.layouts_are_the_old_compilers", [] {
    OriginalProgram p(program(), 2);
    CB_CHECK_EQ(p.layouts().at("struct inode").size, 4u);
    CB_CHECK_EQ(p.layouts().at("struct far_rec").size, 6u);   // (a far pointer: 4 bytes in the original)
    OriginalProgram wide(program(), 8);
    CB_CHECK_EQ(wide.layouts().at("struct inode").size, 4u);
    CB_CHECK_EQ(wide.layouts().at("struct far_rec").size, 6u);   // (the far pointer is the engine's 4 whatever the near one)
});

CB_TEST("port.original_program.sizes_of_the_arrays_the_program_defines", [] {
    OriginalProgram p(program(), 2);
    const auto& elems = p.elem_sizes();
    CB_CHECK_EQ(elems.at("itab"), 4u);    // a struct inode
    CB_CHECK_EQ(elems.at("ftab"), 4u);    // a far pointer
    CB_CHECK_EQ(elems.at("names"), 1u);
    CB_CHECK_EQ(elems.at("wide"), 2u);
    CB_CHECK_EQ(elems.size(), 4u);
    CB_CHECK_EQ(elems.entries().front().first, "itab");   // (in the order the program declares them)
    OrderedMap<std::string, std::uint64_t> asm_sizes;
    asm_sizes.insert_or_assign("wide", 16);   // what the assembler's data placed wins
    asm_sizes.insert_or_assign("extra", 3);
    const auto vars = p.var_sizes(asm_sizes);
    CB_CHECK_EQ(vars.at("itab"), 40u);
    CB_CHECK_EQ(vars.at("ftab"), 12u);
    CB_CHECK_EQ(vars.at("names"), 10u);
    CB_CHECK_EQ(vars.at("wide"), 16u);
    CB_CHECK_EQ(vars.at("extra"), 3u);
    CB_CHECK_EQ(p.var_sizes({}).at("wide"), 14u);
});

CB_TEST("port.original_program.an_allocator_is_a_call_given_a_structures_whole_size", [] {
    OriginalProgram p(program(), 2);
    const Files ported = program();
    const Allocators& a = p.allocators(ported);
    CB_CHECK_EQ(a.size(), 2u);
    CB_CHECK(a.at("alloc") == std::set<std::size_t>{1});   // (alloc(1, 4): the size is its second argument)
    CB_CHECK(a.at("grow") == std::set<std::size_t>{1});    // (alloc passes its own `size` on to grow)
    CB_CHECK(&p.allocators({}) == &a);                      // (worked out once)
});

CB_TEST("port.original_program.allocators_without_places_when_the_rule_is_off", [] {
    OriginalProgram p(program(), 2);
    fs::ScopedEnv env("CB_PORT_RULES", "-alloc_places");
    const Allocators& a = p.allocators(program());
    CB_CHECK_EQ(a.size(), 2u);   // the names alone: no place of the size known
    CB_CHECK(a.at("alloc").empty() && a.at("grow").empty());
});

CB_TEST("port.original_program.extern_declarations_of_named_arrays", [] {
    OriginalProgram p(program(), 2);
    const ExternDecls d = p.extern_decls({"itab", "ftab", "wide"});
    CB_CHECK_EQ(d.size(), 1u);   // (wide is no extern; the old scan takes `far` in `char far *ftab[3]` for the name: ftab is not found)
    CB_CHECK_EQ(d.at("itab").first, "struct inode");
    CB_CHECK_EQ(d.at("itab").second, "itab[10]");
});

CB_TEST("port.original_program.a_tag_defined_two_ways_is_seen_as_the_including_file_sees_it", [] {
    Files f;
    f.insert_or_assign("a.h", "struct t { int x; };\nstruct u { struct t m; };\n");
    f.insert_or_assign("b.h", "struct t { long y; long z; };\n");
    f.insert_or_assign("a.c", "#include \"a.h\"\n");
    f.insert_or_assign("b.c", "#include \"b.h\"\n");
    f.insert_or_assign("n.c", "int z;\n");
    OriginalProgram p(f, 2);
    CB_CHECK_EQ(p.layouts_for("a.c").at("struct t").size, 2u);
    CB_CHECK_EQ(p.layouts_for("b.c").at("struct t").size, 8u);
    CB_CHECK_EQ(p.layouts_for("b.c").at("struct u").size, 8u);   // (u uses the t of b.h in b.c)
    CB_CHECK_EQ(p.layouts_for("n.c").at("struct t").size, 2u);   // (no includes: the first definition in file order)
    CB_CHECK_EQ(p.layouts().at("struct t").size, 2u);
    const auto in = p.layout_input("b.c");
    CB_CHECK(in.order == std::vector<std::string>{"b.c", "b.h"});
    CB_CHECK_EQ(in.texts.front(), "struct t { long y; long z; };\n");   // (the header before the file that includes it, then the rest)
    // a program whose tags are defined once has one layout for every file
    OriginalProgram plain(program(), 2);
    CB_CHECK(&plain.layouts_for("x.c") == &plain.layouts());
});

CB_TEST("port.complaints.said_names_the_kind_and_its_parts", [] {
    const auto lib = said("conflicting types for 'foo'");
    CB_CHECK(lib.has_value());
    CB_CHECK(lib->kind == Complaint::LibraryName);
    CB_CHECK_EQ(std::string(*lib->part("fn")), "foo");
    const auto arg = said("passing argument 2 of 'bar' from incompatible pointer type");
    CB_CHECK(arg->kind == Complaint::ArgumentType);
    CB_CHECK_EQ(std::string(*arg->part("n")), "2");
    CB_CHECK_EQ(std::string(*arg->part("how")), "from incompatible pointer type");
    const auto clang = said("incompatible pointer types passing 'int *' (aka 'int *') to parameter of type 'char *'");
    CB_CHECK(clang->kind == Complaint::ArgumentType);
    CB_CHECK_EQ(std::string(*clang->part("frm")), "int *");
    CB_CHECK_EQ(std::string(*clang->part("to")), "char *");
    CB_CHECK(said("'struct s' has no member named 'zz'")->kind == Complaint::NoMember);
    CB_CHECK_EQ(std::string(*said("'struct s' has no member named 'zz'")->part("m")), "zz");
    CB_CHECK(said("too few actual parameters")->kind == Complaint::Arity);
    CB_CHECK(!said("too few actual parameters")->part("fn").has_value());   // (the part is not in this wording)
    CB_CHECK(said("unsequenced modification and access to 'i'")->kind == Complaint::Unsequenced);
    CB_CHECK(said("cast to pointer from integer of different size")->kind == Complaint::IntegerCastSize);
    CB_CHECK(!said("nothing matches this").has_value());
    CB_CHECK(!said("").has_value());
});

CB_TEST("port.complaints.the_table_is_in_the_order_the_kinds_are_tried", [] {
    const auto& table = complaint_table();
    CB_CHECK_EQ(table.size(), 19u);
    CB_CHECK(table.front().kind == Complaint::LibraryName);
    CB_CHECK(table.back().kind == Complaint::Arity);
    std::size_t patterns = 0;
    for (const auto& c : table) patterns += c.patterns.size();
    CB_CHECK_EQ(patterns, 55u);
    // every pattern compiles
    for (const auto& c : table)
        for (const std::string_view p : c.patterns) CB_CHECK_NOTHROW((void)Regex(p));
    CB_CHECK(Regex(cast_pattern()).search("(struct s *)p").has_value());
    CB_CHECK(Regex(cast_pattern()).search("(int (*)(char *))f").has_value());
});
