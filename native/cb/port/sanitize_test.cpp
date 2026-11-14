// Tests of cb/port/sanitize: the reports of AddressSanitizer (and of the bounds check) as it prints them, over small programs
// written as the port writes them, and what each becomes (a rule of the port, and the note saying why).
//
// The program's facts are the explicit `Target`; paths are std::filesystem; a report line number the file does not have is no
// report; a table of zero elements makes no room; rules are `DeclRule`s with `${1}` templates.

#include "cb/port/sanitize.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::port;
using namespace cb::port::sanitize;

namespace {

struct Rig {
    Files out;
    Files files;
    OrderedMap<std::string, std::uint64_t> ds;
    pre_ansi::StructLayouts layouts;
    std::map<std::string, std::uint64_t> bss;
    bool next_joins = false;
    bool bss_joins = false;
    int bss_asked = 0;
    std::map<std::string, std::set<std::string>> extra_decls, need_headers;
    std::map<std::string, std::map<std::size_t, pre_ansi::LineOrigin>> line_origin;
    pre_ansi::LineFixes line_fixes;
    pre_ansi::Notes mended;
    std::vector<pre_ansi::NullRead> null_reads;
    std::map<std::string, std::uint64_t> reach, grown;
    std::vector<pre_ansi::DeclRule> decl_rules;

    Target target() {
        return Target{out,
                      files,
                      ds,
                      [this](std::string_view) -> const pre_ansi::StructLayouts& { return layouts; },
                      bss,
                      [this](const std::string&) { return next_joins; },
                      [this](const std::string&) {
                          ++bss_asked;
                          return bss_joins;
                      },
                      extra_decls,
                      need_headers,
                      line_origin,
                      line_fixes,
                      mended,
                      null_reads,
                      reach,
                      grown,
                      decl_rules};
    }
};

pre_ansi::StructLayout row_layout() {
    pre_ansi::StructLayout l;
    l.members = {{"a", 0, 2, "a", false, "int16_t"}, {"b", 2, 2, "b", false, "int16_t"}};
    l.size = 4;
    return l;
}

}  // namespace

CB_TEST("port.sanitize.c_layout_is_todays_c", [] {
    const auto l = c_layout("int16_t a; char b[3]; char *p; double d;");
    CB_CHECK(l.has_value());
    CB_CHECK_EQ(l->members.at("a").first, std::uint64_t{0});
    CB_CHECK_EQ(l->members.at("b").first, std::uint64_t{2});
    CB_CHECK_EQ(l->members.at("b").second, std::uint64_t{3});
    CB_CHECK_EQ(l->members.at("p").first, std::uint64_t{8});   // a pointer is 8 bytes and aligned
    CB_CHECK_EQ(l->members.at("d").first, std::uint64_t{16});
    CB_CHECK_EQ(l->size, std::uint64_t{24});
    // qualifiers do not matter; two names of one declaration; an array of arrays multiplies
    const auto q = c_layout("register unsigned char x, y; const int16_t m[2][3];");
    CB_CHECK(q.has_value());
    CB_CHECK_EQ(q->members.at("y").first, std::uint64_t{1});
    CB_CHECK_EQ(q->members.at("m").second, std::uint64_t{12});
    // a member of a type it does not know: no layout
    CB_CHECK(!c_layout("struct inner z; int16_t a;").has_value());
    CB_CHECK_EQ(c_layout("").value().size, std::uint64_t{0});
});

CB_TEST("port.sanitize.a_write_past_a_table_gives_it_room", [] {
    Rig rig;
    rig.out.insert_or_assign("prog.c", "int tab[10];\nint main() { return 0; }\n");
    rig.out.insert_or_assign("prog.h", "extern int tab[10];\n");
    Target t = rig.target();
    const std::string report =
        "==1==ERROR: AddressSanitizer: global-buffer-overflow on address 0x0001 at pc 0x2 bp 0x3\n"
        "WRITE of size 2 at 0x0001 thread T0\n"
        "    #0 0x4 in main C:\\work\\prog.c:12:7\n"
        "0x0001 is located 4 bytes after global variable 'tab' defined in 'prog.c:1:5' (0x5) of size 20\n";
    const std::vector<Mended> done = mend(t, {{"c1", report}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    CB_CHECK_EQ(done[0].subject, "tab");
    CB_CHECK_EQ(done[0].why, "tab[12] written (case 'c1'): the original reached into the variable its linker put after it");
    CB_CHECK_EQ(rig.grown.at("tab"), std::uint64_t{13});   // 10 elements and the (4 + 2) / 2 it reached
    // the rule makes both the definition and the extern that size
    CB_CHECK_EQ(rig.decl_rules.size(), std::size_t{1});
    CB_CHECK_EQ(rig.decl_rules[0].apply("int tab[10];"), "int tab[13];");
    CB_CHECK_EQ(rig.decl_rules[0].apply("extern int tab[10];"), "extern int tab[13];");
    CB_CHECK_EQ(rig.decl_rules[0].apply("int tab2[10];"), "int tab2[10];");
    CB_CHECK_EQ(rig.mended.at("array reached past its end (as the original did, harmlessly there): room made for it").size(), std::size_t{1});
    // the same report again, the port not yet rewritten: already grown, nothing more
    CB_CHECK(mend(t, {{"c1", report}}).empty());
});

CB_TEST("port.sanitize.a_string_table_without_a_size_gets_the_reported_one_and_more", [] {
    Rig rig;
    rig.out.insert_or_assign("prog.c", "char msg[] = \"hello\";\n");
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: global-buffer-overflow\n"
        "READ of size 1 at 0x1\n"
        "0x1 is located 0 bytes after global variable 'msg' defined in 'prog.c:1:6' (0x2) of size 6\n";
    const std::vector<Mended> done = mend(t, {{"c", report}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    CB_CHECK_EQ(rig.grown.at("msg"), std::uint64_t{7});
    CB_CHECK_EQ(rig.decl_rules[0].apply("char msg[] = \"hello\";"), "char msg[7] = \"hello\";");
});

CB_TEST("port.sanitize.a_table_read_into_the_next_one_is_one_table", [] {
    Rig rig;
    rig.files.insert_or_assign("t.c", "short t[4] = {1, 2, 3, 4};\nshort u[2] = {5, 6};\n");
    rig.out = rig.files;
    rig.next_joins = true;
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: global-buffer-overflow\n"
        "READ of size 2 at 0x1\n"
        "0x1 is located 0 bytes after global variable 't' defined in 't.c:1:7' (0x2) of size 8\n";
    const auto done = mend(t, {{"c", report}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    CB_CHECK_EQ(rig.reach.at("t"), std::uint64_t{4});   // (8 + 0 + 2 - 1) / 2: the elements it reached, counted from 0
    CB_CHECK(done[0].why.starts_with("t[4] read (case 'c'): the table went on into the variables after it"));
    CB_CHECK(rig.decl_rules.empty());
});

CB_TEST("port.sanitize.a_bss_array_gone_into_the_next_is_joined", [] {
    Rig rig;
    rig.out.insert_or_assign("b.c", "char buf[8];\nchar next[8];\n");
    rig.bss = {{"buf", 8}, {"next", 8}};
    rig.bss_joins = true;
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: global-buffer-overflow\n"
        "WRITE of size 1 at 0x1\n"
        "0x1 is located 3 bytes after global variable 'buf' defined in 'b.c:1:6' (0x2) of size 8\n";
    const auto done = mend(t, {{"c", report}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    CB_CHECK_EQ(rig.bss_asked, 1);
    CB_CHECK_EQ(done[0].why, "buf write past its end (case 'c'): the BSS variable after it, one array with it");
    CB_CHECK(rig.decl_rules.empty());
    CB_CHECK(rig.grown.empty());
    // when it cannot be joined, it grows like any table
    Rig other;
    other.out = rig.out;
    other.bss = rig.bss;
    other.bss_joins = false;
    Target t2 = other.target();
    CB_CHECK_EQ(mend(t2, {{"c", report}}).size(), std::size_t{1});
    CB_CHECK_EQ(other.grown.at("buf"), std::uint64_t{12});   // 8 + (3 + 1)
});

namespace {

// A program whose table is read before its start: `tab[k - 5].b` with k at 3.
struct BeforeRig {
    Rig rig;
    BeforeRig() {
        rig.out.insert_or_assign("tab.c", "struct row { int16_t a; int16_t b; };\nstruct row tab[3] = { {1, 2}, {3, 4}, {5, 6} };\nint16_t lead;\n");
        rig.out.insert_or_assign("use.c", "int f(int k) { return tab[k - 5].b; }\n");
        rig.layouts.insert_or_assign("struct row", row_layout());
        rig.ds.insert_or_assign("tab", 0x100);
        rig.ds.insert_or_assign("lead", 0xFA);       // what the old linker put 6 bytes before the table
        rig.ds.insert_or_assign("far_below", 0x10);
    }
};

}  // namespace

CB_TEST("port.sanitize.a_read_before_a_table_reads_what_the_linker_put_there", [] {
    BeforeRig b;
    Target t = b.rig.target();
    const std::string report =
        "use.c:1:25: runtime error: index -2 out of bounds for type 'struct row[3]'\n";
    const auto done = mend(t, {{"c", report}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    CB_CHECK_EQ(done[0].why, "use.c:1 tab[-2].b (case 'c'): before the table - the original read lead there");
    // the line becomes: the row's member when the index is that, else the variable the original had there
    CB_CHECK_EQ(b.rig.line_fixes.size(), std::size_t{1});
    const auto& [key, fixed] = *b.rig.line_fixes.begin();
    CB_CHECK_EQ(key.file, "use.c");
    CB_CHECK_EQ(key.text, "int f(int k) { return tab[k - 5].b; }");
    CB_CHECK(!key.nth.has_value());
    CB_CHECK_EQ(fixed, "int f(int k) { return ((k - 5) == -2 ? lead : tab[k - 5].b); }");
    CB_CHECK_EQ(b.rig.mended.size(), std::size_t{1});
});

CB_TEST("port.sanitize.the_line_is_kept_by_its_origin_when_the_port_knows_it", [] {
    BeforeRig b;
    b.rig.line_origin["use.c"][1] = {"int f(int k) { return tab[k - 5].b; } /* as read */", 2};
    Target t = b.rig.target();
    const auto done = mend(t, {{"c", "use.c:1:25: runtime error: index -2 out of bounds for type 'struct row[3]'\n"}});
    CB_CHECK_EQ(done.size(), std::size_t{1});
    const auto& [key, fixed] = *b.rig.line_fixes.begin();
    CB_CHECK_EQ(key.text, "int f(int k) { return tab[k - 5].b; } /* as read */");
    CB_CHECK(key.nth == std::optional<std::size_t>(2));
});

CB_TEST("port.sanitize.a_report_with_a_distance_finds_the_row_too", [] {
    BeforeRig b;
    Target t = b.rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: global-buffer-overflow on address 0x5\n"
        "READ of size 2 at 0x5 thread T0\n"
        "    #0 0x1234 in f C:\\src\\use.c:1:25\n"
        "0x5 is located 6 bytes before global variable 'tab' defined in 'tab.c:2:12' (0x6) of size 12\n";
    const auto why = before_start(t, "c", report);
    CB_CHECK(why.has_value());
    if (why) CB_CHECK_EQ(*why, "use.c:1 tab[-2].b (case 'c'): before the table - the original read lead there");
    // (the same with the path as the frame says it: no drive, then no column)
    const std::string plain =
        "ERROR: AddressSanitizer: global-buffer-overflow on address 0x5\n"
        "READ of size 2 at 0x5 thread T0\n"
        "    #0 0x1234 in f use.c:1\n"
        "0x5 is located 6 bytes before global variable 'tab' defined in 'tab.c:2:12' (0x6) of size 12\n";
    CB_CHECK(before_start(t, "c", plain).has_value());
});

CB_TEST("port.sanitize.a_read_that_is_not_a_member_of_a_row_is_left_alone", [] {
    BeforeRig b;
    Target t = b.rig.target();
    // 5 bytes before the table is no member's place (b is at 2 of a row of 4)
    const std::string report =
        "ERROR: AddressSanitizer: global-buffer-overflow\n"
        "READ of size 2 at 0x5\n"
        "    #0 0x1234 in f use.c:1\n"
        "0x5 is located 5 bytes before global variable 'tab' defined in 'tab.c:2:12' (0x6) of size 12\n";
    CB_CHECK(!before_start(t, "c", report).has_value());
    // a line with an increment in the index is more than a read
    b.rig.out.insert_or_assign("use.c", "int f(int k) { return tab[k++].b; }\n");
    CB_CHECK(!before_start(t, "c", "use.c:1:25: runtime error: index -2 out of bounds for type 'struct row[3]'").has_value());
    // a table the program does not place, or a file the port does not have
    Rig none;
    none.out = b.rig.out;
    Target t2 = none.target();
    CB_CHECK(!before_start(t2, "c", "use.c:1:25: runtime error: index -2 out of bounds for type 'x'").has_value());
    CB_CHECK(!before_start(t, "c", "gone.c:1:25: runtime error: index -2 out of bounds for type 'x'").has_value());
    CB_CHECK(!before_start(t, "c", "use.c:9:1: runtime error: index -2 out of bounds for type 'x'").has_value());   // no such line
    CB_CHECK(!before_start(t, "c", "nothing to see").has_value());
});

CB_TEST("port.sanitize.what_is_before_the_table_may_be_another_tables_member", [] {
    BeforeRig b;
    // the variable the linker put there is itself a table of rows: its member, by the old layout
    b.rig.out.insert_or_assign("tab.c", "struct row { int16_t a; int16_t b; };\nstruct row tab[3] = { {1, 2}, {3, 4}, {5, 6} };\nstruct row prev[2];\n");
    b.rig.out.insert_or_assign("rows.h", "struct row { int16_t a; int16_t b; };\n");
    b.rig.ds.insert_or_assign("prev", 0xF0);
    b.rig.ds.insert_or_assign("lead", 0x10);   // (no longer the nearest)
    Target t = b.rig.target();
    const auto why = before_start(t, "c", "use.c:1:25: runtime error: index -2 out of bounds for type 'struct row[3]'");
    CB_CHECK(why.has_value());
    // 0xFA - 0xF0 = 10 = row 2, member b (2)
    CB_CHECK_EQ(*why, "use.c:1 tab[-2].b (case 'c'): before the table - the original read prev[2].b there");
    CB_CHECK(b.rig.extra_decls.at("use.c").contains("extern struct row prev[];"));
    CB_CHECK(b.rig.need_headers.at("use.c").contains("rows.h"));
});

CB_TEST("port.sanitize.a_read_at_a_small_address_goes_through_the_null_segment", [] {
    Rig rig;
    rig.out.insert_or_assign("list.c", "struct node *nodes[10];\nint walk(int i) {\n    return nodes[i]->val;\n}\n");
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: access-violation on unknown address 0x000000000004 (pc 0x1 bp 0x2 sp 0x3 T0)\n"
        "The signal is caused by a READ memory access.\n"
        "    #0 0x7ff6 in walk C:\\src\\list.c:3\n"
        "    #1 0x7ff7 in main C:\\src\\main.c:9\n";
    const auto why = low_reads(t, "c", report);
    CB_CHECK(why.has_value());
    CB_CHECK_EQ(*why, "list.c: nodes[...] a small number at times (read at 4h, case 'c'): read through the null segment there");
    CB_CHECK_EQ(rig.null_reads.size(), std::size_t{1});
    CB_CHECK_EQ(rig.null_reads[0].var, "nodes");
    CB_CHECK_EQ(rig.null_reads[0].type, "struct node *");
    CB_CHECK(rig.null_reads[0].kind == pre_ansi::NullRead::Kind::Plain);
    // the same variable is not made a rule twice
    CB_CHECK(!low_reads(t, "c", report).has_value());
    CB_CHECK_EQ(rig.null_reads.size(), std::size_t{1});
});

CB_TEST("port.sanitize.a_pointer_that_was_null_is_read_through_as_dos_read_it", [] {
    Rig rig;
    rig.out.insert_or_assign("list.c", "struct list *l;\nint head() {\n    return l->op;\n}\n");
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: access-violation on unknown address 0x000000000002 (pc 0x1)\n"
        "    #0 0x7ff6 in head list.c:3\n";
    const auto why = low_reads(t, "case7", report);
    CB_CHECK(why.has_value());
    CB_CHECK_EQ(*why, "list.c: l a NULL at times (read at 2h, case 'case7'): read through the null segment there");
    CB_CHECK(rig.null_reads[0].kind == pre_ansi::NullRead::Kind::Var);
    CB_CHECK_EQ(rig.null_reads[0].type, "struct list *");
    CB_CHECK_EQ(rig.mended.at("a pointer read through when NULL (DOS read the null segment there): dos_np").size(), std::size_t{1});
});

CB_TEST("port.sanitize.a_link_of_a_chain_that_was_null", [] {
    Rig rig;
    rig.out.insert_or_assign("node.h", "struct node { struct node *next; struct node *prev; int v; };\n");
    rig.out.insert_or_assign("node.c", "#include \"node.h\"\nstruct node *p;\nvoid unlink() {\n    p->next->prev = 0;\n}\n");
    Target t = rig.target();
    const std::string report =
        "ERROR: AddressSanitizer: access-violation on unknown address 0x000000000002 (pc 0x1)\n"
        "    #0 0x7ff6 in unlink node.c:4\n";
    const auto why = low_reads(t, "c", report);
    CB_CHECK(why.has_value());
    CB_CHECK_EQ(*why, "node.c: p->next a NULL at times (at 2h, case 'c'): through the null segment there");
    CB_CHECK(rig.null_reads[0].kind == pre_ansi::NullRead::Kind::Mem);
    CB_CHECK_EQ(rig.null_reads[0].var, "next");
    CB_CHECK_EQ(rig.null_reads[0].type, "struct node *");

    // and with the chains rule off, the chain is not tried: the plain `p->` of the line is
    fs::ScopedEnv off("CB_PORT_RULES", "-np_chains");
    Rig plain;
    plain.out = rig.out;
    Target t2 = plain.target();
    const auto why2 = low_reads(t2, "c", report);
    CB_CHECK(why2.has_value());
    CB_CHECK_EQ(*why2, "node.c: p a NULL at times (read at 2h, case 'c'): read through the null segment there");
});

CB_TEST("port.sanitize.a_value_given_back_null", [] {
    Rig rig;
    rig.out.insert_or_assign("q.c", "struct item *q;\nint f(struct item *p) {\n    return (q = next(p))->kind;\n}\n");
    Target t = rig.target();
    const auto why = low_reads(t, "c", "ERROR: AddressSanitizer: access-violation on unknown address 0x000000000000 (pc 0x1)\n    #0 0x1 in f q.c:3\n");
    CB_CHECK(why.has_value());
    CB_CHECK_EQ(*why, "q.c: q as given a NULL at times (at 0h, case 'c'): read through the null segment there");
    CB_CHECK_EQ(rig.null_reads.size(), std::size_t{2});   // the assignment, and q->kind after it
    CB_CHECK(rig.null_reads[0].kind == pre_ansi::NullRead::Kind::Assign);
    CB_CHECK(rig.null_reads[1].kind == pre_ansi::NullRead::Kind::Var);
});

CB_TEST("port.sanitize.reports_with_nothing_to_mend", [] {
    Rig rig;
    rig.out.insert_or_assign("p.c", "int x;\n");
    Target t = rig.target();
    CB_CHECK(mend(t, {}).empty());
    CB_CHECK(mend(t, {{"c", "all fine\n"}, {"d", ""}, {"e", "AddressSanitizer: heap-use-after-free\n"}}).empty());
    // a fault at a small address on a line with nothing to read through
    CB_CHECK(!low_reads(t, "c", "access-violation on unknown address 0x2\n    #0 0x1 in f p.c:1\n").has_value());
    // a fault in a file the port does not have
    CB_CHECK(!low_reads(t, "c", "access-violation on unknown address 0x2\n    #0 0x1 in f other.c:1\n").has_value());
});

CB_TEST("port.sanitize.runtime_dir_is_the_newest_that_has_the_dll", [] {
    fs::ScopedTempDir tmp("cb-sanitize");
    const auto compiler = tmp.path() / "llvm" / "bin" / "clang.exe";
    CB_CHECK(!runtime_dir(compiler).has_value());   // no lib folder at all
    fs::write_text(tmp.path() / "llvm" / "lib" / "clang" / "17" / "lib" / "windows" / "clang_rt.asan_dynamic-x86_64.dll", "x");
    fs::write_text(tmp.path() / "llvm" / "lib" / "clang" / "19" / "lib" / "windows" / "clang_rt.builtins.lib", "x");   // no sanitizer there
    const auto found = runtime_dir(compiler);
    CB_CHECK(found.has_value());
    CB_CHECK(fs::to_text(*found).ends_with("clang/17/lib/windows"));
    fs::write_text(tmp.path() / "llvm" / "lib" / "clang" / "20" / "lib" / "windows" / "clang_rt.asan_dynamic-x86_64.dll", "x");
    CB_CHECK(fs::to_text(*runtime_dir(compiler)).ends_with("clang/20/lib/windows"));
});
