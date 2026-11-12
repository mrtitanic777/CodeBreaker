#include "cb/port/oldlayout.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
namespace oltest {

namespace a16 = cb::port::arith16;
namespace old = cb::port::oldlayout;
namespace stdfs = std::filesystem;

const stdfs::path kClangPath = "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang.exe";

stdfs::path data(std::string_view name) { return cb::testing::data_path("port/testdata/oldlayout/" + std::string(name)); }

bool have_clang() { return stdfs::exists(kClangPath); }

old::Splice open(std::uint64_t at, int depth, std::string text) { return {at, false, depth, std::move(text)}; }
old::Splice close(std::uint64_t at, int depth, std::string text) { return {at, true, depth, std::move(text)}; }

// A tree made of a document written out here (what clang prints, less everything this pass does not read).
a16::Tree tree_of(const std::string& json) {
    std::optional<a16::Tree> t = a16::parse_tree(json);
    CB_ASSERT(t.has_value(), "the test's tree is a document");
    return std::move(*t);
}

// What the Python's pass made of prog.c and prog.h (testdata/gen_oldlayout_golden.py), read through by hand.
const std::vector<std::string> kProg{"prog.c"};

}  // namespace oltest

using namespace oltest;

// DIVERGES from the Python: declare() scans on from the end of each body it writes (the Python used offsets of the text as it was
// before the first definition of a tag changed it: damaged text for a second definition); the typed tree names a header's
// locations by their own file (the includedFrom fault of arith16).

// ---- types ---------------------------------------------------------------------------------------------------------

CB_TEST("port.oldlayout.record_tag", [] {
    CB_CHECK_EQ(old::record_tag("struct expr *"), "struct expr");
    CB_CHECK_EQ(old::record_tag("const struct expr"), "struct expr");
    CB_CHECK_EQ(old::record_tag("struct  expr   *"), "struct expr");
    CB_CHECK_EQ(old::record_tag("union val"), "union val");
    CB_CHECK_EQ(old::record_tag("const union val *"), "union val");
    // not a record, or not a record alone
    CB_CHECK_EQ(old::record_tag("union (unnamed at x.c:1:1)"), "");
    CB_CHECK_EQ(old::record_tag("struct expr **"), "");
    CB_CHECK_EQ(old::record_tag("struct expr *const"), "");
    CB_CHECK_EQ(old::record_tag("char *"), "");
    CB_CHECK_EQ(old::record_tag("int"), "");
    CB_CHECK_EQ(old::record_tag("struct"), "");
    CB_CHECK_EQ(old::record_tag("struct *"), "");
    CB_CHECK_EQ(old::record_tag(""), "");
});

// ---- the insertions -------------------------------------------------------------------------------------------------

CB_TEST("port.oldlayout.apply_nests_by_depth", [] {
    // two wrappers open at 2 (the outer, depth 1, first) and three close at 4 (the innermost, depth 3, first)
    const std::vector<old::Splice> s{open(2, 3, "<B"), open(2, 1, "<A"), close(4, 1, "A>"), close(4, 3, "B>"), open(4, 2, "<C"), open(6, 0, "!")};
    CB_CHECK_EQ(old::apply("abcdef", s), "ab<A<BcdB>A><Cef!");
    CB_CHECK_EQ(old::apply("abc", {}), "abc");
    CB_CHECK_EQ(old::apply("", std::vector<old::Splice>{open(0, 1, "x")}), "x");
    // the same depth keeps the order given
    CB_CHECK_EQ(old::apply("ab", std::vector<old::Splice>{open(1, 1, "1"), open(1, 1, "2")}), "a12b");
});

CB_TEST("port.oldlayout.apply_past_the_end", [] { CB_CHECK_THROWS_AS(cb::UsageError, (void)old::apply("abc", std::vector<old::Splice>{open(4, 0, "x")})); });

// ---- the definitions ------------------------------------------------------------------------------------------------

CB_TEST("port.oldlayout.declare_makes_handles", [] {
    // `struct sym *next, *prev;` is two declarations; the members found are uint16_t handles; a record not kept old is not touched
    const std::string text = "struct sym {\n\tstruct sym *next, *prev;\n\tint n;\n\tconst struct sym *up;\n\tchar *name; struct sym *x; int y;\n};\n"
                             "struct other { struct sym *next; };\n";
    const old::Members found{{{"struct sym", "next"}, "struct sym *"}, {{"struct sym", "up"}, "const struct sym *"}, {{"struct sym", "x"}, "struct sym *"}};
    CB_CHECK_EQ(old::declare(text, {"struct sym"}, found),
                "struct sym {\n\tuint16_t next;\t/* (a handle: struct sym *) */ struct sym *prev;\n\tint n;\n"
                "\tuint16_t up;\t/* (a handle: const struct sym *) */\n\tchar *name; uint16_t x;\t/* (a handle: struct sym *) */ int y;\n};\n"
                "struct other { struct sym *next; };\n");
});

CB_TEST("port.oldlayout.declare_one_word_again", [] {
    CB_CHECK_EQ(old::declare("union val {\n\tintptr_t w;\n\tlong l; { intptr_t z; }\n};\n", {"union val"}, {}),
                "union val {\n\tint16_t w;\t/* (one word, as it was) */\n\tlong l; { int16_t z;\t/* (one word, as it was) */ }\n};\n");
    // intptr_t outside the record is not touched
    CB_CHECK_EQ(old::declare("intptr_t g;\nstruct s { int a; };\n", {"struct s"}, {}), "intptr_t g;\nstruct s { int a; };\n");
});

CB_TEST("port.oldlayout.declare_two_definitions_of_a_tag", [] {
    // (the Python took the second one's place from the text as it was before the first was changed: damaged text)
    const std::string def = "struct sym {\n\tstruct sym *next;\n};\n";
    const std::string out = old::declare(def + "/* between */\n" + def, {"struct sym"}, {{{"struct sym", "next"}, "struct sym *"}});
    const std::string one = "struct sym {\n\tuint16_t next;\t/* (a handle: struct sym *) */\n};\n";
    CB_CHECK_EQ(out, one + "/* between */\n" + one);
});

CB_TEST("port.oldlayout.declare_unbalanced_body_runs_to_the_end", [] {
    CB_CHECK_EQ(old::declare("struct s {\n\tstruct s *p;\n", {"struct s"}, {{{"struct s", "p"}, "struct s *"}}),
                "struct s {\n\tuint16_t p;\t/* (a handle: struct s *) */\n");
});

CB_TEST("port.oldlayout.null_reads", [] {
    // a read straight through a handle is a read through what may be NULL; a handle kept in a variable keeps dos_hp
    const std::string text = "a = ((struct n *)dos_hp(h))->op;\nl = ((struct n *)dos_hp(h));\nif (l) l->op = ((struct n *)dos_hp(k)) ->x + (((struct n *)dos_hp(m)));\n"
                             "/* ((struct n *)dos_hp(z))->w */ s = \"((struct n *)dos_hp(q))->r\";\n";
    CB_CHECK_EQ(old::null_reads(text),
                "a = ((struct n *)dos_hpr(h))->op;\nl = ((struct n *)dos_hp(h));\nif (l) l->op = ((struct n *)dos_hpr(k)) ->x + (((struct n *)dos_hp(m)));\n"
                "/* ((struct n *)dos_hp(z))->w */ s = \"((struct n *)dos_hp(q))->r\";\n");
    CB_CHECK_EQ(old::null_reads(""), "");
    CB_CHECK_EQ(old::null_reads("((struct n *)dos_hp(h"), "((struct n *)dos_hp(h");   // (never closed)
    CB_CHECK_EQ(old::null_reads("((union u *)dos_hp(f(g(1)))) ->k"), "((union u *)dos_hpr(f(g(1)))) ->k");
});

// ---- which records ---------------------------------------------------------------------------------------------------

CB_TEST("port.oldlayout.mentions", [] {
    CB_CHECK(old::mentions(data(""), "prog.c", std::vector<std::string>{"expr"}));        // (prog.c names it itself)
    CB_CHECK(old::mentions(data(""), "prog.c", std::vector<std::string>{"nosuch", "plain"}));
    CB_CHECK(!old::mentions(data(""), "prog.c", std::vector<std::string>{"nosuch"}));
    CB_CHECK(!old::mentions(data(""), "prog.c", std::vector<std::string>{"exp"}));       // (a word: expr is not exp)
    CB_CHECK(!old::mentions(data(""), "missing.c", std::vector<std::string>{"expr"}));   // (not there: names nothing)
    CB_CHECK(old::mentions(data(""), "prog.h", std::vector<std::string>{"sym"}));
});

CB_TEST("port.oldlayout.tree.reads_the_declarations", [] {
    const a16::Tree t = tree_of(R"j({"kind":"TranslationUnitDecl","inner":[
        {"id":"0x1f0","kind":"RecordDecl","tagUsed":"union","name":"val","completeDefinition":true,"inner":[
            {"id":"0x1f8","kind":"FieldDecl","name":"p","type":{"qualType":"struct sym *"}}]},
        {"id":"0x200","kind":"RecordDecl","tagUsed":"struct","name":"fwd"},
        {"kind":"MemberExpr","referencedMemberDecl":"0x1f8","type":{"qualType":"struct sym *"}},
        {"kind":"CallExpr","inner":[{"kind":"DeclRefExpr","referencedDecl":{"id":"0x9","kind":"FunctionDecl","name":"take","type":{"qualType":"void ()"}}}]},
        {"kind":"CompoundAssignOperator","opcode":"|="}]})j");
    const a16::Node& rec = t.top().at(0);
    CB_CHECK(rec.kind == a16::Kind::RecordDecl);
    CB_CHECK_EQ(rec.id, std::uint64_t{0x1f0});
    CB_CHECK_EQ(rec.name, "val");
    CB_CHECK(rec.complete);
    CB_CHECK(rec.is_union);
    CB_CHECK_EQ(rec.inner.at(0).name, "p");
    CB_CHECK_EQ(rec.inner.at(0).qual_type, "struct sym *");
    CB_CHECK(!t.top().at(1).complete);   // (only declared)
    CB_CHECK_EQ(t.top().at(2).member_decl, std::uint64_t{0x1f8});
    CB_CHECK_EQ(t.top().at(3).inner.at(0).name, "take");
    CB_CHECK(t.top().at(4).op == a16::Op::OrAssign);
    CB_CHECK_EQ(a16::op_text(a16::Op::OrAssign), "|=");
});

// ---- real C through clang -------------------------------------------------------------------------------------------

CB_TEST("port.oldlayout.clang.records_taken_as_bytes", [] {
    if (!have_clang()) return;
    // prog.c writes (char *)&copy and (char *)&e->e_u, and reads into e: struct expr is kept old; struct plain, only
    // used, and struct sym, only pointed to, are not
    const std::optional<a16::Tree> tree = a16::dump(kClangPath, "prog.c", data(""));
    CB_CHECK(tree.has_value());
    if (!tree) return;
    CB_CHECK(old::byte_records(*tree) == old::RecordSet{"struct expr"});
});

CB_TEST("port.oldlayout.clang.the_program", [] {
    if (!have_clang()) return;
    const std::optional<a16::Tree> tree = a16::dump(kClangPath, "prog.c", data(""));
    CB_CHECK(tree.has_value());
    if (!tree) return;
    const std::string text = cb::fs::read_text(data("prog.c"));
    const old::RecordSet olds{"struct expr"};
    const old::PointerParams params{{"take", {true, false}}, {"later", {true}}};
    const old::Analysis a = old::analyse(*tree, text, tree->find_file("prog.c"), olds, params);
    const old::Members want{{{"struct expr", "e_left"}, "struct expr *"}, {{"struct expr", "e_p"}, "struct sym *"},
                            {{"struct expr", "e_right"}, "struct expr *"}, {{"struct expr", "e_sym"}, "struct sym *"}};
    CB_CHECK(a.found == want);
    CB_CHECK_EQ(a.refused.size(), std::size_t{2});
    CB_CHECK_EQ(a.refused.at(0).op, "&");
    CB_CHECK_EQ(a.refused.at(1).op, "++");
    // the member taken by & and ++ is not converted: the text there is what it was
    CB_CHECK(text.substr(a.refused.at(0).at, 8) == "e->e_sym");
    CB_CHECK(text.substr(a.refused.at(1).at, 9) == "e->e_left");
    std::string converted = old::apply(text, a.splices);
    converted = old::declare(converted, olds, a.found);
    converted = old::null_reads(converted);
    CB_CHECK_EQ(converted, cb::fs::read_text(data("expected/prog.c")));
    CB_CHECK_EQ(old::declare(cb::fs::read_text(data("prog.h")), olds, a.found), cb::fs::read_text(data("expected/prog.h")));
    // without the old-style function's parameters the handle given to take() is not turned back
    const old::Analysis without = old::analyse(*tree, text, tree->find_file("prog.c"), olds);
    CB_CHECK_EQ(a.splices.size() - without.splices.size(), std::size_t{2});
});

CB_TEST("port.oldlayout.clang.nothing_kept_old", [] {
    if (!have_clang()) return;
    const std::optional<a16::Tree> tree = a16::dump(kClangPath, "prog.c", data(""));
    CB_CHECK(tree.has_value());
    if (!tree) return;
    const std::string text = cb::fs::read_text(data("prog.c"));
    CB_CHECK(old::analyse(*tree, text, tree->find_file("prog.c"), {}).splices.empty());
    CB_CHECK(old::analyse(*tree, text, a16::kNoFile, {"struct expr"}).splices.empty());   // (a file clang never named)
    CB_CHECK(old::analyse(*tree, text, tree->find_file("prog.c"), {"struct plain"}).splices.size() == 2);   // (pl.p_next = 0: a pointer member of plain, were it old: dos_ph(0))
});

CB_TEST("port.oldlayout.run", [] {
    if (!have_clang()) return;
    cb::fs::ScopedTempDir dir("cb-oldlayout-run");
    cb::fs::copy_tree(data(""), dir.path() / "port");
    cb::base::cache::Config config;
    config.state_dir = dir.path() / "state";
    config.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(config);
    const std::vector<std::string> files{"prog.c", "missing.c"};
    const old::PointerParams params{{"take", {true, false}}, {"later", {true}}};
    const old::Conversions first = old::run(cache, kClangPath, dir.path() / "port", files, params);
    CB_CHECK(first.olds == old::RecordSet{"struct expr"});
    CB_CHECK_EQ(first.found.size(), std::size_t{4});
    CB_CHECK_EQ(first.texts.size(), std::size_t{1});
    CB_CHECK_EQ(first.refused.size(), std::size_t{2});
    CB_CHECK_EQ(first.refused.at(0).file, "prog.c");
    // what run() gives is converted, not yet declared (the caller declares every file of the port)
    CB_CHECK_NE(first.texts.at("prog.c"), cb::fs::read_text(data("prog.c")));
    CB_CHECK(first.texts.at("prog.c").find("e->e_sym = dos_ph(lookup(3));") != std::string::npos);
    CB_CHECK(first.texts.at("prog.c").find("uint16_t") == std::string::npos);
    const auto found_before = cache.counts()[std::string(old::kCacheKind)].found;
    const old::Conversions again = old::run(cache, kClangPath, dir.path() / "port", files, params);
    CB_CHECK(again.texts == first.texts);
    CB_CHECK(again.found == first.found);
    CB_CHECK_EQ(again.refused.size(), first.refused.size());
    CB_CHECK_EQ(cache.counts()[std::string(old::kCacheKind)].found - found_before, std::uint64_t{3});   // (each file's records, prog.c's conversions, missing.c's records)
    // other parameters are another pass; the records are not asked again
    const old::Conversions other = old::run(cache, kClangPath, dir.path() / "port", files);
    CB_CHECK_EQ(first.texts.at("prog.c").size() - other.texts.at("prog.c").size(), std::string_view("dos_hp()").size());   // (later(e->e_word): an old-style function's pointer parameter)
    // a program with no record kept old: nothing
    cb::fs::write_text(dir.path() / "port" / "plain.c", "struct q { struct q *n; };\nvoid f(struct q *p) { p->n = 0; }\n");
    const old::Conversions none = old::run(cache, kClangPath, dir.path() / "port", std::vector<std::string>{"plain.c"});
    CB_CHECK(none.olds.empty());
    CB_CHECK(none.texts.empty());
});

CB_TEST("port.oldlayout.clang_missing", [] {
    cb::fs::ScopedTempDir dir("cb-oldlayout-missing");
    cb::base::cache::Config config;
    config.state_dir = dir.path() / "state";
    config.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(config);
    CB_CHECK_THROWS_AS(cb::ToolError, (void)old::run(cache, "C:/no/such/clang.exe", data(""), std::vector<std::string>{"prog.c"}));
});

}  // namespace
