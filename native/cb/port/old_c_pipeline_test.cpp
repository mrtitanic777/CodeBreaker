// Tests of the helpers and pipeline steps of cb/port/old_c_data (old_c_scan, joins_next, global_definitions,
// loose_calls, gather_notes, rename_pascal_calls, declared_return_types, add_missing_includes, allocator_block_sizes,
// array_sizes_of, guard_headers). Where the Python has the same function the expected values are what it gave on the same
// input (state/work/cpp-refactor/p2/unit_expect.py); the steps that were inline in Port.modernize are checked by hand.
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_data.hpp"
#include "cb/port/old_c_scan.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

std::string show_globals(const std::vector<GlobalDefinition>& defs) {
    std::string out;
    for (const GlobalDefinition& g : defs)
        out += std::to_string(g.line) + ":" + g.name + "|" + g.base + "|" + g.shape + "|" + g.initializer.value_or("<none>") + "\n";
    return out;
}

}  // namespace

CB_TEST("port.old_c_scan.integers_are_hex_or_decimal_and_whole", [] {
    CB_CHECK_EQ(c_integer("0x1F").value_or(0), 31u);
    CB_CHECK_EQ(c_integer("0X1f").value_or(0), 31u);
    CB_CHECK_EQ(c_integer("010").value_or(99), 8u);    // (octal, as C reads it; DIVERGES: the Python's int("010", 0) raised)
    CB_CHECK_EQ(c_integer("08").value_or(99), 8u);     // (not C: read as decimal)
    CB_CHECK_EQ(c_integer("0").value_or(99), 0u);
    CB_CHECK(!c_integer(""));
    CB_CHECK(!c_integer("0x"));
    CB_CHECK(!c_integer("12L"));
    CB_CHECK(!c_integer("-1"));
    CB_CHECK(!c_integer("0xFFFFFFFFFFFFFFFFF"));   // (past 64 bits: not edited)
});

CB_TEST("port.old_c_scan.pointer_types_are_written_one_way", [] {
    CB_CHECK_EQ(pointer_type_text("char *"), "char *");
    CB_CHECK_EQ(pointer_type_text("char*"), "char *");
    CB_CHECK_EQ(pointer_type_text("struct  node   * *"), "struct node **");
    CB_CHECK_EQ(pointer_type_text("const char\n*"), "const char *");
});

CB_TEST("port.old_c_scan.pointer_targets_name_the_structure_each_variable_points_to", [] {
    const auto t = pointer_targets("struct  s *a, *b;\nunion u *c;\nvoid f(struct t *d) { x = (struct s *)e; }\nstruct s *a2;");
    CB_CHECK_EQ(t.at("a"), "struct s");
    CB_CHECK_EQ(t.at("d"), "struct t");
    CB_CHECK_EQ(t.at("c"), "union u");
    CB_CHECK_EQ(t.at("a2"), "struct s");
    CB_CHECK(!t.contains("b"));   // (a second name in a list is not seen: `struct s *a, *b;` gives a)
    CB_CHECK(!t.contains("e"));
});

CB_TEST("port.old_c_scan.call_sites_are_whole_words_followed_by_a_paren", [] {
    const std::string code = "f(1); xf(2); f (3); a.f(4); p->f(5); f;";
    const auto all = call_sites(code, [](std::string_view w) { return w == "f"; });
    CB_CHECK_EQ(all.size(), 4u);   // (f(1), f (3), a.f(4), p->f(5))
    const auto plain = call_sites(code, [](std::string_view w) { return w == "f"; }, /*not_member=*/true);
    CB_CHECK_EQ(plain.size(), 2u);
    CB_CHECK_EQ(plain[0].at, 0u);
    CB_CHECK_EQ(plain[1].at, 13u);
});

CB_TEST("port.old_c_scan.edits_apply_from_the_last_back", [] {
    CB_CHECK_EQ(apply_edits("0123456789", {{1, 3, "AB"}, {5, 6, "xyz"}, {8, 8, "!"}}), "0AB34xyz67!89");
    CB_CHECK_EQ(apply_edits("abc", {{0, 1, "X"}, {0, 1, "X"}}, /*unique=*/true), "Xbc");
    CB_CHECK_EQ(line_of("a\nb\nc", 3), 1u);
});

CB_TEST("port.old_c_scan.named_regex_compiles_each_name_once_and_quotes_it", [] {
    NamedRegex re(R"(\b@1@\s*->\s*@2@\b)");
    CB_CHECK(re.with("p", "next").test("x = p -> next;"));
    CB_CHECK(!re.with("p", "next").test("x = pp->next;"));
    CB_CHECK(!re.with("p", "nex").test("p->next"));
    CB_CHECK(NamedRegex(R"(@1@)").with("a.b").test("a.b"));
    CB_CHECK(!NamedRegex(R"(@1@)").with("a.b").test("axb"));
    CB_CHECK(&re.with("p", "next") == &re.with("p", "next"));
});

CB_TEST("port.old_c_data.joins_next_finds_a_table_followed_by_another_definition", [] {
    Files files;
    files.insert_or_assign("a.c", "int t[3] = { 1, 2, 3 };\nint u = 4;\nchar c[2] = {1, 2};\nlong l;\n");
    CB_CHECK(joins_next(files, "t"));
    CB_CHECK(!joins_next(files, "u"));
    CB_CHECK(!joins_next(files, "c"));   // (what follows has no value)
    CB_CHECK(!joins_next(files, "zz"));
    files.insert_or_assign("b.c", "int a[2] = {1};\nchar other[4] = {0};\nunsigned b = 1;\nint s = 1;\nint a2[2] = {2};\n");
    CB_CHECK(!joins_next(files, "a"));   // (the next is another type)
    CB_CHECK(!joins_next(files, "other"));
    CB_CHECK(!joins_next(files, "b"));
});

CB_TEST("port.old_c_data.global_definitions_list_the_one_name_definitions_at_a_files_top", [] {
    const std::string text =
        "int a;\nstatic char *p = \"x\";\nstruct s *sp;\nint tab[3] = { 1, 2, 3 };\nextern int e;\nint f();\nint (*fp)();\n"
        "int x, y;\nunsigned long big = 70000L;\nvoid g() { int local; }\nstatic unsigned  int  w  =  5 ;\nchar name[] = \"a;b\";\n"
        "struct s { int m; };\nFILE *out = 0;\nregister int r;\n";
    // as the Python's _globals gives them (the function pointer and the lists are no one-name definitions; a structure
    // on one line is read as a member, as it always was)
    CB_CHECK_EQ(show_globals(global_definitions(text)),
                "0:a|int|@|<none>\n1:p|char|*@|\"x\"\n2:sp|struct s|*@|<none>\n3:tab|int|@[3]|{ 1, 2, 3 }\n8:big|unsigned long|@|70000L\n"
                "10:w|unsigned int|@|5\n11:name|char|@[]|\"a;b\"\n12:m|struct s { int|@;}|<none>\n13:out|FILE|*@|0\n14:r|register int|@|<none>\n");
});

CB_TEST("port.old_c_data.loose_calls_make_the_integers_given_an_old_style_definition_intptr_t", [] {
    OrderedMap<std::string, Fn> fns;
    Fn f;
    f.name = "f";
    f.params = {{"a", "int a"}, {"b", "char *b"}, {"c", "register unsigned c"}};
    f.loose = true;
    fns.insert_or_assign("f", f);
    Fn vf;
    vf.name = "vf";
    vf.params = {{"fmt", "char *fmt"}};
    vf.va_slots = true;
    fns.insert_or_assign("vf", vf);
    Fn plain;
    plain.name = "plain";
    plain.params = {{"a", "int a"}};
    fns.insert_or_assign("plain", plain);
    Files out;
    out.insert_or_assign("x.c", "int f_new(a, b, c)\nint a;\n{\n    f_new(1, \"s\", x + 2);\n    vf(\"%d\", 3, y, (intptr_t)z);\n    plain(4);\n    f_new((intptr_t)1, 2, 3);\n}\n");
    out.insert_or_assign("y.c", "void h() { vf(1, 2); }\n");
    out.insert_or_assign("z.h", "void h() { vf(1, 2); }\n");   // (headers are not touched)
    Notes notes;
    loose_calls(out, fns, {{"f", "f_new"}}, notes);
    CB_CHECK_EQ(out.at("x.c"),
                "int f_new(a, b, c)\nint a;\n{\n    f_new((intptr_t)(1), \"s\", (intptr_t)(x + 2));\n    vf(\"%d\", (intptr_t)(3), (intptr_t)(y), (intptr_t)z);\n"
                "    plain(4);\n    f_new((intptr_t)1, 2, (intptr_t)(3));\n}\n");
    CB_CHECK_EQ(out.at("y.c"), "void h() { vf(1, (intptr_t)(2)); }\n");
    CB_CHECK_EQ(out.at("z.h"), "void h() { vf(1, 2); }\n");
    CB_CHECK_EQ(text::join(notes.at("an integer given to an old-style definition kept (unchecked calls): made its intptr_t"), " "),
                "x.c:4 x.c:5 x.c:7 y.c:1");
});

CB_TEST("port.old_c_data.gather_notes_keeps_the_mended_ones_first_then_the_passes_in_the_reports_order", [] {
    Notes mended;
    note(mended, "from the loop", "a.c:1");
    PassNotes lists;
    lists.agreed = {"a.c (g)"};
    lists.pools = {"b.c (p)"};
    lists.tables = {"c.c (t: u)"};
    Notes unions;
    note(unions, "a union's", "d.h");
    OrderedMap<std::string, std::size_t> dropped;
    dropped.insert_or_assign("e.c", 3);
    const std::vector<std::string> none, wide = {"f.c (w)"}, fixes = {"g.h: struct s's m"};
    const Notes n = gather_notes({mended, lists, wide, none, unions, fixes, dropped});
    const char* const order[] = {"from the loop", "a variable another file", "a view's pointer where", "a pooled allocator's blocks",
                                 "a union's", "member retyped:", "a table read past its end", "far / near / pascal"};
    CB_CHECK_EQ(n.size(), 8u);
    std::size_t i = 0;
    for (const auto& kv : n) {
        CB_CHECK(i < 8 && kv.first.starts_with(order[i]));
        ++i;
    }
    CB_CHECK_EQ(n.at("far / near / pascal and their kind dropped (one flat memory, one way of calling)")[0], "e.c (3)");
});

CB_TEST("port.old_c_data.pascal_names_called_in_other_letters_get_the_definitions", [] {
    Files files;
    files.insert_or_assign("a.c", "void f() { FOO(1); foo(2); Bar(); known(); BAZ(); }\n/* QUX(); */\n");
    std::map<std::string, std::string> renames = {{"BAZ", "kept"}};
    Notes notes;
    rename_pascal_calls(files, {"foo", "bar", "Known", "baz", "qux"}, {"known"}, renames, notes);
    CB_CHECK_EQ(renames.size(), 3u);
    CB_CHECK_EQ(renames.at("FOO"), "foo");
    CB_CHECK_EQ(renames.at("Bar"), "bar");   // (a call that is not the definition's spelling)
    CB_CHECK_EQ(renames.at("BAZ"), "kept");   // (already renamed)
    CB_CHECK(!renames.contains("known"));
    CB_CHECK(!renames.contains("QUX"));       // (in a comment)
    CB_CHECK_EQ(text::join(notes.at("a pascal name called in other letters (the linker made both capitals): the definition's"), " "),
                "a.c (Bar) a.c (FOO)");
});

CB_TEST("port.old_c_data.a_callers_declaration_gives_the_return_type_of_an_old_int_function", [] {
    Files files;
    files.insert_or_assign("a.c", "struct node *mk();\nextern char *name();\nint plain();\nstatic int hidden();\nunsigned long big();\nvoid done();\n");
    OrderedMap<std::string, Fn> fns;
    for (const char* n : {"mk", "name", "plain", "hidden", "big", "done"}) {
        Fn f;
        f.name = n;
        fns.insert_or_assign(n, f);
    }
    fns.at("done").ret = "void";
    Notes notes;
    declared_return_types(files, fns, notes);
    CB_CHECK_EQ(fns.at("mk").ret, "struct node *");
    CB_CHECK(fns.at("mk").ret_set);
    CB_CHECK_EQ(fns.at("name").ret, "char *");
    CB_CHECK_EQ(fns.at("plain").ret, "");
    CB_CHECK_EQ(fns.at("hidden").ret, "");        // (static: not a caller's declaration)
    CB_CHECK_EQ(fns.at("big").ret, "unsigned long");
    CB_CHECK_EQ(fns.at("done").ret, "void");     // (it had its type)
    CB_CHECK_EQ(notes.at("what a function of old C's int gives, as its callers declared it").size(), 3u);
});

CB_TEST("port.old_c_data.a_file_gets_the_headers_of_the_structures_it_uses_and_the_declarations_it_needs", [] {
    Files out;
    out.insert_or_assign("a.h", "struct s { int x; };\nstruct both { int y; };\n");
    out.insert_or_assign("c.h", "struct both { int z; };\n");   // (a tag two headers define: either's, none added)
    out.insert_or_assign("b.c", "#include \"port.h\"\nstruct s *p;\nstruct both *q;\n");
    out.insert_or_assign("d.c", "#include \"a.h\"\nstruct s *p;\n");
    out.insert_or_assign("e.c", "int e;\n");
    out.insert_or_assign("port.h", "");
    std::map<std::string, std::vector<std::string>> need = {{"e.c", {"extra.h"}}};
    ExtraDecls extra = {{"b.c", {"extern int g[3];", "extern int a[1];"}}};
    Notes notes;
    const auto tags = add_missing_includes(out, need, extra, notes);
    CB_CHECK_EQ(tags.size(), 1u);
    CB_CHECK_EQ(tags.at("struct s"), "a.h");
    CB_CHECK_EQ(out.at("b.c"), "#include \"port.h\"\n#include \"a.h\"\nextern int a[1];\nextern int g[3];\nstruct s *p;\nstruct both *q;\n");
    CB_CHECK_EQ(out.at("d.c"), "#include \"a.h\"\nstruct s *p;\n");   // (it has it)
    CB_CHECK_EQ(out.at("e.c"), "#include \"extra.h\"\nint e;\n");     // (DIVERGES: the Python raised on a file with no #include)
    CB_CHECK_EQ(text::join(notes.at("header of the structures a file uses, which it did not include: included"), " | "),
                "b.c (a.h) | e.c (extra.h)");
});

CB_TEST("port.old_c_data.an_allocators_old_block_size_becomes_the_structures_size", [] {
    Files out;
    out.insert_or_assign("a.c", "#include \"port.h\"\nchar *pool_alloc(n)\nint n;\n{\n    return malloc(34) + 34 + x34 + 340;\n}\nint other() { return 34; }\n");
    OrderedMap<std::string, Fn> fns;
    Fn f;
    f.name = "pool_alloc";
    f.file = "a.c";
    fns.insert_or_assign("pool_alloc", f);
    AllocatorSizes sizes;
    sizes["pool_alloc"].insert({"struct node", 34});
    Notes notes;
    allocator_block_sizes(out, fns, sizes, {{"struct node", "n.h"}}, notes);
    CB_CHECK_EQ(out.at("a.c"),
                "#include \"port.h\"\n#include \"n.h\"\nchar *pool_alloc(n)\nint n;\n{\n    return malloc(sizeof(struct node)) + sizeof(struct node) + x34 + 340;\n}\n"
                "int other() { return 34; }\n");
    CB_CHECK_EQ(notes.at("the program's own allocator keeps its blocks by a structure's old size: its size now")[0], "a.c (pool_alloc: 34)");
});

CB_TEST("port.old_c_data.array_sizes_count_elements_and_rows", [] {
    Files texts;
    texts.insert_or_assign("a.c", "int a[10];\nstatic char b[] = { 1, 2, (3, 4), {5, 6} };\nextern int c[5];\nint f(int x[4]);\nint a[3];\nint d[2] = { 0 };\n");
    texts.insert_or_assign("bss.c", "int e[ 7 ];");
    const auto sizes = array_sizes_of(texts, {});
    CB_CHECK_EQ(sizes.size(), 4u);
    CB_CHECK_EQ(sizes.at("a"), 10u);   // (the first definition of a name)
    CB_CHECK_EQ(sizes.at("b"), 4u);
    CB_CHECK_EQ(sizes.at("d"), 2u);
    CB_CHECK_EQ(sizes.at("e"), 7u);
    // the declaration rules are applied to each text first
    const std::vector<DeclRule> rules = {DeclRule(R"(\bint(\s+a\s*\[)\s*10)", "intptr_t${1}10")};
    CB_CHECK_EQ(array_sizes_of(texts, rules).at("a"), 10u);
});

CB_TEST("port.old_c_data.a_header_is_guarded_once", [] {
    Files out;
    out.insert_or_assign("a.h", "int x;\n\n");
    out.insert_or_assign("b-c.h", "#ifndef DOSRT_ONCE_X\nint y;\n#endif\n");
    out.insert_or_assign("a.c", "int z;\n");
    guard_headers(out);
    CB_CHECK_EQ(out.at("a.h"), "#ifndef DOSRT_ONCE_A_H\n#define DOSRT_ONCE_A_H\nint x;\n#endif\n");
    CB_CHECK_EQ(out.at("b-c.h"), "#ifndef DOSRT_ONCE_X\nint y;\n#endif\n");
    CB_CHECK_EQ(out.at("a.c"), "int z;\n");
    guard_headers(out);
    CB_CHECK_EQ(out.at("a.h"), "#ifndef DOSRT_ONCE_A_H\n#define DOSRT_ONCE_A_H\nint x;\n#endif\n");
});
