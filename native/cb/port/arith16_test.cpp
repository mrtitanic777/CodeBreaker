#include "cb/port/arith16.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace a16 = cb::port::arith16;
namespace stdfs = std::filesystem;

const stdfs::path kClang = "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang.exe";

stdfs::path testfile(std::string_view name) { return cb::testing::data_path("port/testdata/arith16/" + std::string(name)); }

bool have_clang() { return stdfs::exists(kClang); }

// A tree made of a document that is written out here: nodes are {"kind", "opcode", "type", "range", "inner"} and
// a place is {"offset", "tokLen"} (what clang prints, less everything this pass does not read).
a16::Tree tree_of(const std::string& json) {
    std::optional<a16::Tree> t = a16::parse_tree(json);
    CB_CHECK(t.has_value());
    CB_ASSERT(t.has_value(), "the test's tree is a document");
    return std::move(*t);
}

std::string place(int from, int to) {
    return std::format(R"({{"begin":{{"offset":{},"file":"t.c","tokLen":1}},"end":{{"offset":{},"tokLen":1}}}})", from, to);
}

std::string ref(const char* type, int at) {
    return std::format(R"({{"kind":"DeclRefExpr","type":{{"qualType":"{}"}},"range":{}}})", type, place(at, at));
}

std::string wrap_tu(const std::string& node) {
    return R"({"kind":"TranslationUnitDecl","inner":[)" + node + "]}";
}

std::string format_edits(const std::vector<a16::Edit>& edits) {
    std::string out;
    for (const a16::Edit& e : edits) out += std::format("{}\t{}\n", e.at, e.text);
    return out;
}

// ---- reading the tree ---------------------------------------------------------------------------------------------

CB_TEST("port.arith16.tree.names_the_file_where_clang_does", [] {
    // clang names a location's file where it changes. The first location of h.h names it and says a.c included it;
    // the nodes after it, with no name, are still in h.h; then a.c is named again.
    const a16::Tree t = tree_of(R"({"kind":"TranslationUnitDecl","inner":[
        {"kind":"A","loc":{"offset":5,"file":".\\h.h","line":1,"tokLen":1,"includedFrom":{"file":"a.c"}},
         "range":{"begin":{"offset":0,"tokLen":4,"includedFrom":{"file":"a.c"}},
                  "end":{"offset":9,"tokLen":1,"includedFrom":{"file":"a.c"}}}},
        {"kind":"B","loc":{"offset":12,"line":2,"tokLen":1,"includedFrom":{"file":"a.c"}},
         "range":{"begin":{"offset":10,"tokLen":4,"includedFrom":{"file":"a.c"}},
                  "end":{"offset":15,"tokLen":1,"includedFrom":{"file":"a.c"}}}},
        {"kind":"C","loc":{"offset":3,"file":"a.c","line":9,"tokLen":1},
         "range":{"begin":{"offset":3,"tokLen":4},"end":{"offset":8,"tokLen":1}}},
        {"kind":"D","loc":{"offset":30,"line":10,"tokLen":1},
         "range":{"begin":{"offset":30,"tokLen":4},"end":{"offset":35,"tokLen":1}}}]})");
    CB_CHECK_EQ(t.files().size(), std::size_t{2});
    CB_CHECK_EQ(t.files()[0], ".\\h.h");
    CB_CHECK_EQ(t.files()[1], "a.c");
    CB_CHECK_EQ(t.top().size(), std::size_t{4});
    CB_CHECK_EQ(t.top()[0].loc.file, 0);
    CB_CHECK_EQ(t.top()[0].range.begin.file, 0);
    CB_CHECK_EQ(t.top()[1].range.begin.file, 0);   // not a.c: includedFrom does not move where we are
    CB_CHECK_EQ(t.top()[1].range.end.file, 0);
    CB_CHECK_EQ(t.top()[2].loc.file, 1);
    CB_CHECK_EQ(t.top()[3].range.end.file, 1);
    CB_CHECK_EQ(t.find_file("h.h"), 0);
    CB_CHECK_EQ(t.find_file("./h.h"), 0);
    CB_CHECK_EQ(t.find_file("a.c"), 1);
    CB_CHECK_EQ(t.find_file("b.c"), a16::kNoFile);
});

CB_TEST("port.arith16.tree.reads_typed_nodes", [] {
    const a16::Tree t = tree_of(wrap_tu(
        R"({"kind":"BinaryOperator","opcode":"+","type":{"qualType":"int","desugaredQualType":"int"},"valueCategory":"prvalue",
            "range":{"begin":{"offset":1,"tokLen":1},"end":{"offset":9,"tokLen":2}},
            "inner":[{"kind":"IntegerLiteral","value":"70000","type":{"qualType":"long"}},
                     {"kind":"CharacterLiteral","value":-1,"type":{"qualType":"char"}},
                     {"kind":"UnaryOperator","opcode":"-","type":{"qualType":"uint16_t"}},
                     {"kind":"UnaryOperator","opcode":"*","type":{"qualType":"int16_t"}},
                     {"kind":"NamespaceDecl","inner":[{"kind":"IntegerLiteral","value":"7"}]}]})"));
    const a16::Node& add = t.top().at(0);
    CB_CHECK(add.kind == a16::Kind::BinaryOperator);
    CB_CHECK(add.op == a16::Op::Add);
    CB_CHECK_EQ(add.qual_type, "int");
    CB_CHECK(!add.prim.has_value());   // int is not one of the 8086's types: it is what they were made of
    CB_CHECK_EQ(add.range.end.tok_len, std::uint64_t{2});
    CB_CHECK_EQ(add.inner.size(), std::size_t{5});
    CB_CHECK_EQ(add.inner[0].value.value_or(0), 70000);
    CB_CHECK(add.inner[0].prim == a16::Prim::I32);
    CB_CHECK_EQ(add.inner[1].value.value_or(0), -1);
    CB_CHECK(add.inner[2].op == a16::Op::Sub);     // a unary minus
    CB_CHECK(add.inner[3].op == a16::Op::Deref);   // a unary *, not a Mul
    CB_CHECK(add.inner[4].kind == a16::Kind::Other);
    CB_CHECK_EQ(add.inner[4].inner.size(), std::size_t{1});   // (children of nodes it does not read are still read)
});

CB_TEST("port.arith16.tree.not_a_tree", [] {
    CB_CHECK(!a16::parse_tree("").has_value());
    CB_CHECK(!a16::parse_tree("[]").has_value());
    CB_CHECK(!a16::parse_tree("not json").has_value());
    CB_CHECK(!a16::parse_tree(R"({"kind":"TranslationUnitDecl","inner":[{"kind":"A")").has_value());   // cut short
    CB_CHECK(!a16::parse_tree(R"({"kind":"TranslationUnitDecl"} {)").has_value());                    // junk after
    CB_CHECK(a16::parse_tree(R"({})").has_value());                                                    // empty is a tree
});

CB_TEST("port.arith16.tree.integer_too_big_for_64_bits", [] {
    const a16::Tree t = tree_of(wrap_tu(R"({"kind":"IntegerLiteral","value":"18446744073709551615"})"));
    CB_CHECK(t.top().at(0).value.value_or(0) > 0xFFFF);
});

// ---- the 8086's types ---------------------------------------------------------------------------------------------

CB_TEST("port.arith16.common_follows_the_8086", [] {
    using P = a16::Prim;
    const std::optional<P> none;
    CB_CHECK(!a16::common(none, P::I16).has_value());
    CB_CHECK(!a16::common(P::U16, none).has_value());
    CB_CHECK(a16::common(P::I16, P::I16) == P::I16);
    CB_CHECK(a16::common(P::I16, P::U16) == P::U16);   // an int met by an unsigned is unsigned
    CB_CHECK(a16::common(P::U16, P::I32) == P::I32);
    CB_CHECK(a16::common(P::I32, P::U32) == P::U32);
    CB_CHECK(a16::common(P::I16, P::U32) == P::U32);
});

CB_TEST("port.arith16.place_in_a_macro", [] {
    // BIG, a macro of no arguments: its use is the place; ADD(a,b), a macro with arguments, has none
    const std::string text = "x = BIG; y = ADD(a,b); z = NEG ;";
    const auto macro = [](int at, int len) {
        return std::format(
            R"({{"kind":"IntegerLiteral","value":"1","range":{{"begin":{{"spellingLoc":{{"offset":900,"tokLen":1}},
                  "expansionLoc":{{"offset":{},"file":"t.c","tokLen":{}}}}},"end":{{"spellingLoc":{{"offset":900,"tokLen":1}},
                  "expansionLoc":{{"offset":{},"tokLen":{}}}}}}}}})",
            at, len, at, len);
    };
    const a16::Tree t = tree_of(wrap_tu(macro(4, 3) + "," + macro(13, 3) + "," + macro(27, 3)));
    const a16::Source src{text, 0};
    const auto big = a16::place(t.top()[0], text);
    CB_CHECK(big.has_value());
    CB_CHECK_EQ(big->start, std::uint64_t{4});
    CB_CHECK_EQ(big->end, std::uint64_t{7});
    CB_CHECK_EQ(a16::spelled(t.top()[0], src).value_or("?"), "BIG");
    CB_CHECK(!a16::place(t.top()[1], text).has_value());   // ADD( ... ) : the '(' follows
    const auto neg = a16::place(t.top()[2], text);          // NEG then a blank then ';'
    CB_CHECK(neg.has_value());
    CB_CHECK_EQ(neg->end, std::uint64_t{30});
});

CB_TEST("port.arith16.place_needs_one_file_and_no_spelling", [] {
    const a16::Tree t = tree_of(wrap_tu(
        R"({"kind":"A","range":{"begin":{"offset":0,"file":"a.c","tokLen":1},"end":{"offset":4,"tokLen":2}}},
            {"kind":"B","range":{"begin":{"offset":0,"tokLen":1},"end":{"offset":4,"file":"b.c","tokLen":2}}},
            {"kind":"C","range":{"begin":{"offset":0,"tokLen":1}}},
            {"kind":"D","range":{}},
            {"kind":"E","range":{"begin":{"spellingLoc":{"offset":1},"expansionLoc":{"offset":1,"tokLen":1}},
                                 "end":{"offset":4,"tokLen":1}}})"));
    const auto a = a16::place(t.top()[0], "");
    CB_CHECK(a.has_value());
    CB_CHECK_EQ(a->start, std::uint64_t{0});
    CB_CHECK_EQ(a->end, std::uint64_t{6});   // the end token's own length
    CB_CHECK_EQ(a->file, 0);
    CB_CHECK(!a16::place(t.top()[1], "").has_value());   // begins in a.c-less ... ends in b.c
    CB_CHECK(!a16::place(t.top()[2], "").has_value());
    CB_CHECK(!a16::place(t.top()[3], "").has_value());
    CB_CHECK(!a16::place(t.top()[4], "").has_value());   // a spelling on one side only
});

// ---- the casts, from trees written out ---------------------------------------------------------------------------

CB_TEST("port.arith16.casts.unsigned_against_signed", [] {
    // u > s, u unsigned and s signed: the 8086 made s unsigned
    const std::string text = "u > s";
    const a16::Tree t = tree_of(wrap_tu(std::format(R"({{"kind":"BinaryOperator","opcode":">","range":{},"inner":[{},{}]}})",
                                                    place(0, 4), ref("uint16_t", 0), ref("int16_t", 4))));
    const std::vector<a16::Edit> e = a16::edits_of(t, text, 0);
    CB_CHECK_EQ(e.size(), std::size_t{1});
    CB_CHECK_EQ(format_edits(e), "4\t(uint16_t)\n");
    CB_CHECK_EQ(a16::apply_edits(text, e), "u > (uint16_t)s");
    // both signed: nothing to say; another file than the tree's main one: nothing
    const a16::Tree same = tree_of(wrap_tu(std::format(R"({{"kind":"BinaryOperator","opcode":">","range":{},"inner":[{},{}]}})",
                                                       place(0, 4), ref("int16_t", 0), ref("int16_t", 4))));
    CB_CHECK(a16::edits_of(same, text, 0).empty());
    CB_CHECK(a16::edits_of(t, text, a16::kNoFile).empty());
});

CB_TEST("port.arith16.casts.edits_only_inside_the_text", [] {
    // a tree of another version of the file: a place past the end of this text is not this file's
    const a16::Tree t = tree_of(wrap_tu(std::format(R"({{"kind":"BinaryOperator","opcode":">","range":{},"inner":[{},{}]}})",
                                                    place(0, 40), ref("uint16_t", 0), ref("int16_t", 40))));
    CB_CHECK(a16::edits_of(t, "u > s", 0).empty());
});

// ---- applying edits -----------------------------------------------------------------------------------------------

CB_TEST("port.arith16.apply", [] {
    using E = a16::Edit;
    const std::vector<E> none;
    CB_CHECK_EQ(a16::apply_edits("0123456789", none), "0123456789");
    CB_CHECK_EQ(a16::apply_edits("0123456789", std::vector<E>{{3, "(int16_t)("}, {8, ")"}}), "012(int16_t)(34567)89");
    // a ")" ends an expression where the next may begin: it goes first, and duplicates are each kept
    CB_CHECK_EQ(a16::apply_edits("0123456789", std::vector<E>{{3, "(int16_t)("}, {8, ")"}, {3, ")"}, {3, "(int16_t)("}, {8, ")"}}),
                "012)(int16_t)((int16_t)(34567))89");
    CB_CHECK_EQ(a16::apply_edits("0123456789", std::vector<E>{{0, "A"}, {10, "Z"}, {5, "(x)"}, {5, ")"}}), "A01234)(x)56789Z");
    CB_CHECK_EQ(a16::apply_edits("", std::vector<E>{{0, "a"}, {0, ")"}}), ")a");
    CB_CHECK_EQ(a16::apply_edits("abc", std::vector<E>{{1, "(uint16_t)"}, {2, "(int16_t)("}, {3, ")"}, {1, ")"}}),
                "a)(uint16_t)b(int16_t)(c)");
    // openers at one offset keep the order they were given in
    CB_CHECK_EQ(a16::apply_edits("x", std::vector<E>{{0, "(A)("}, {0, "(B)("}, {1, ")"}, {1, ")"}}), "(A)((B)(x))");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)a16::apply_edits("abc", std::vector<E>{{4, "x"}}));
});

// ---- the real thing: clang's tree of the port's C -----------------------------------------------------------------

std::vector<std::string> edit_lines(const std::vector<a16::Edit>& edits) {
    std::vector<std::string> out;
    for (const a16::Edit& e : edits) out.push_back(std::format("{}\t{}", e.at, e.text));
    return out;
}

// The casts clang's tree of one file of the test data gives, against the files kept in expected/ (made by the
// Python's pass, once its includedFrom fault was mended, and read through).
void check_against_expected(const std::string& name, std::size_t edit_count) {
    const std::optional<a16::Tree> tree = a16::dump(kClang, name, testfile(""));
    CB_CHECK(tree.has_value());
    if (!tree) return;
    const std::string text = cb::fs::read_text(testfile(name));
    const std::vector<a16::Edit> edits = a16::edits_of(*tree, text, tree->find_file(name));
    CB_CHECK_EQ(edits.size(), edit_count);
    std::string want_edits = cb::fs::read_text(testfile("expected/" + name + ".edits"));
    CB_CHECK_EQ(format_edits(edits), want_edits);
    CB_CHECK_EQ(a16::apply_edits(text, edits), cb::fs::read_text(testfile("expected/" + name + ".cast")));
}

CB_TEST("port.arith16.clang.a_c_with_its_header", [] {
    if (!have_clang()) return;
    // a.c includes hdr.h, which has casts of its own to make: those are the header's (offsets of hdr.h), and
    // must not be put into a.c
    check_against_expected("a.c", 58);
    const std::optional<a16::Tree> tree = a16::dump(kClang, "a.c", testfile(""));
    CB_CHECK(tree.has_value());
    if (!tree) return;
    CB_CHECK_EQ(tree->files().size(), std::size_t{2});
    CB_CHECK_EQ(tree->find_file("a.c"), 1);
    CB_CHECK_EQ(tree->find_file("hdr.h"), 0);
});

CB_TEST("port.arith16.clang.casts_that_share_a_place", [] {
    if (!have_clang()) return;
    // Two casts that end at one place each have their ")" (line 5: the whole comparison and the sum in it), and two that
    // open at one place nest the outer one first, each with its own type (line 6: (uint16_t)( ... (int16_t)(arr[2] + ...
    // The Python kept one ")" of two, and put the two openers in the order of a set: not valid C, or the casts' types swapped.
    check_against_expected("nested.c", 14);
    const std::string cast = cb::fs::read_text(testfile("expected/nested.c.cast"));
    CB_CHECK(cast.find("(uint16_t)((int16_t)(arr[2] + fn(3)) >> (i ^ st.f) % arr[2]) > u") != std::string::npos);
    CB_CHECK_EQ(std::ranges::count(cast, '('), std::ranges::count(cast, ')'));
});

CB_TEST("port.arith16.clang.the_header_itself", [] {
    if (!have_clang()) return;
    check_against_expected("hdr.h", 4);
});

CB_TEST("port.arith16.clang.small_cases", [] {
    if (!have_clang()) return;
    check_against_expected("b.c", 1);
    // a file with a syntax error still gives a tree (clang goes on); nothing in it needs a cast
    const std::optional<a16::Tree> bad = a16::dump(kClang, "bad.c", testfile(""));
    CB_CHECK(bad.has_value());
    if (bad) CB_CHECK(a16::edits_of(*bad, cb::fs::read_text(testfile("bad.c")), bad->find_file("bad.c")).empty());
    // a file clang cannot read, and an empty one: no tree of them
    CB_CHECK(!a16::dump(kClang, "missing.c", testfile("")).has_value());
    const std::optional<a16::Tree> empty = a16::dump(kClang, "empty.c", testfile(""));
    if (empty) CB_CHECK(a16::edits_of(*empty, "", empty->find_file("empty.c")).empty());
    // an include that is not found is an error clang reports and goes on from
    const std::optional<a16::Tree> nohdr = a16::dump(kClang, "nohdr.c", testfile(""));
    CB_CHECK(nohdr.has_value());
    if (nohdr) CB_CHECK(a16::edits_of(*nohdr, cb::fs::read_text(testfile("nohdr.c")), nohdr->find_file("nohdr.c")).empty());
});

CB_TEST("port.arith16.clang.a_second_pass_finds_nothing_to_do", [] {
    if (!have_clang()) return;
    // the casts made are casts already: the pass on its own result adds none
    cb::fs::ScopedTempDir dir("cb-arith16");
    cb::fs::copy_tree(testfile(""), dir.path());
    cb::fs::write_text(dir.path() / "a2.c", cb::fs::read_text(testfile("expected/a.c.cast")));
    const std::optional<a16::Tree> tree = a16::dump(kClang, "a2.c", dir.path());
    CB_CHECK(tree.has_value());
    if (tree) CB_CHECK(a16::edits_of(*tree, cb::fs::read_text(dir.path() / "a2.c"), tree->find_file("a2.c")).empty());
});

CB_TEST("port.arith16.clang_missing", [] {
    CB_CHECK_THROWS_AS(cb::ToolError, (void)a16::dump("C:/no/such/clang.exe", "a.c", testfile("")));
});

// ---- keys and the cache -------------------------------------------------------------------------------------------

CB_TEST("port.arith16.included_names", [] {
    const std::vector<std::string> names = a16::included_names(
        "#include \"a.h\"\n#include <c.h>\n#  include\t\"missing.h\"\n# include<d.h>\n"
        "/* #include \"commented.h\" */\n#include \"unterminated\n#include \"\"\n#define X include \"no\"\n"
        "#include_next <n.h>\n#includ \"short.h\"\n#include \"last.h\"");
    CB_CHECK_EQ(names.size(), std::size_t{6});
    CB_CHECK_EQ(names[0], "a.h");
    CB_CHECK_EQ(names[1], "c.h");
    CB_CHECK_EQ(names[2], "missing.h");
    CB_CHECK_EQ(names[3], "d.h");
    CB_CHECK_EQ(names[4], "commented.h");   // (the scan does not know comments: an extra header only widens the key)
    CB_CHECK_EQ(names[5], "last.h");
    CB_CHECK(a16::included_names("").empty());
    CB_CHECK(a16::included_names("#").empty());
    CB_CHECK(a16::included_names("#include").empty());
});

CB_TEST("port.arith16.reach_key", [] {
    cb::fs::ScopedTempDir dir("cb-arith16-reach");
    const auto put = [&](const char* name, std::string_view text) { cb::fs::write_text(dir.path() / name, text); };
    put("x.c", "#include \"a.h\"\n#include <stdio.h>\nint main(void) { return 0; }\n");
    put("a.h", "#include \"b.h\"\n#include <c.h>\n#include \"missing.h\"\n#include \"a.h\"\n");   // (includes itself)
    put("b.h", "#include <a.h>\n");                                                              // (and a cycle)
    put("c.h", "int c;\n");
    put("lone.h", "int lone;\n");
    put("notes.txt", "not a header\n");
    const std::string base = a16::reach_key(dir.path(), "x.c");
    CB_CHECK_EQ(base.size(), std::size_t{40});
    CB_CHECK_EQ(a16::reach_key(dir.path(), "x.c"), base);                // the same twice
    CB_CHECK_NE(a16::reach_key(dir.path(), "a.h"), base);                // another file reaches other things
    put("notes.txt", "changed\n");
    put("lone.h", "int other;\n");
    CB_CHECK_EQ(a16::reach_key(dir.path(), "x.c"), base);                // what it does not reach does not matter
    put("c.h", "int c2;\n");
    const std::string after_c = a16::reach_key(dir.path(), "x.c");
    CB_CHECK_NE(after_c, base);                                          // a header it reaches does
    put("b.h", "#include <a.h>\n#include \"lone.h\"\n");
    CB_CHECK_NE(a16::reach_key(dir.path(), "x.c"), after_c);             // and what that reaches in turn
    put("x.c", "#include \"a.h\"\n#include <stdio.h>\nint main(void) { return 1; }\n");
    CB_CHECK_NE(a16::reach_key(dir.path(), "x.c"), after_c);             // and the file itself
    // a file that is not there, and one that is a folder: a key all the same
    CB_CHECK_EQ(a16::reach_key(dir.path(), "nonexistent.c").size(), std::size_t{40});
    stdfs::create_directory(dir.path() / "dir.h");
    CB_CHECK_EQ(a16::reach_key(dir.path(), "dir.h").size(), std::size_t{40});
});

CB_TEST("port.arith16.pass_key_depends_on_what_the_pass_does", [] {
    cb::fs::ScopedTempDir dir("cb-arith16-key");
    cb::fs::write_text(dir.path() / "x.c", "int x;\n");
    const std::string k = a16::pass_key("C:/no/clang.exe", dir.path(), "x.c", "arith16", "v1");
    CB_CHECK_EQ(a16::pass_key("C:/no/clang.exe", dir.path(), "x.c", "arith16", "v1"), k);
    CB_CHECK_NE(a16::pass_key("C:/no/clang.exe", dir.path(), "x.c", "arith16", "v2"), k);    // the code changed
    CB_CHECK_NE(a16::pass_key("C:/no/clang.exe", dir.path(), "x.c", "records", "v1"), k);    // another pass
    CB_CHECK_NE(a16::pass_key("C:/no/other.exe", dir.path(), "x.c", "arith16", "v1"), k);    // another compiler
    const std::vector<std::string> extra{"-DX"};
    CB_CHECK_NE(a16::pass_key("C:/no/clang.exe", dir.path(), "x.c", "arith16", "v1", extra), k);
});

CB_TEST("port.arith16.run_keeps_each_result", [] {
    if (!have_clang()) return;
    cb::fs::ScopedTempDir dir("cb-arith16-run");
    cb::fs::copy_tree(testfile(""), dir.path() / "port");
    cb::base::cache::Config config;
    config.state_dir = dir.path() / "state";
    config.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(config);
    a16::declare_cache(cache);
    const std::vector<std::string> files{"a.c", "b.c", "bad.c", "empty.c", "nohdr.c", "missing.c", "hdr.h"};
    const auto first = a16::run(cache, kClang, dir.path() / "port", files);
    // a.c, b.c and hdr.h need casts; the others none (or cannot be read at all)
    CB_CHECK_EQ(first.size(), std::size_t{3});
    CB_CHECK_EQ(first.at("a.c"), cb::fs::read_text(testfile("expected/a.c.cast")));
    CB_CHECK_EQ(first.at("b.c"), cb::fs::read_text(testfile("expected/b.c.cast")));
    CB_CHECK_EQ(first.at("hdr.h"), cb::fs::read_text(testfile("expected/hdr.h.cast")));
    const auto found_before = cache.counts()[std::string(a16::kCacheKind)].found;
    const auto again = a16::run(cache, kClang, dir.path() / "port", files);
    CB_CHECK(again == first);
    const auto found_after = cache.counts()[std::string(a16::kCacheKind)].found;
    CB_CHECK_EQ(found_after - found_before, std::uint64_t{6});   // (every file clang could read: a result kept, even "none")
    // a header changed: the files that reach it are done again, the others are not
    cb::fs::write_text(dir.path() / "port" / "hdr.h", cb::fs::read_text(testfile("hdr.h")) + "int16_t more;\n");
    const auto changed = a16::run(cache, kClang, dir.path() / "port", files);
    CB_CHECK_EQ(changed.size(), std::size_t{3});
    CB_CHECK_EQ(cache.counts()[std::string(a16::kCacheKind)].found - found_after, std::uint64_t{4});   // (a.c and hdr.h are done again)
    CB_CHECK(a16::run(cache, kClang, dir.path() / "port", {}).empty());
});

// Timing, not a check: set CB_BENCH_JSON to a clang dump (clang -Xclang -ast-dump=json) and CB_BENCH_SOURCE to its source
// to see what reading the tree and making the casts cost (docs/TYPED_CPP_SPEED.md).
CB_TEST("port.arith16.bench", [] {
    const std::optional<std::string> json_path = cb::fs::get_env("CB_BENCH_JSON");
    const std::optional<std::string> source_path = cb::fs::get_env("CB_BENCH_SOURCE");
    if (!json_path || !source_path) return;
    const std::string json = cb::fs::read_text(cb::fs::from_text(*json_path));
    const std::string text = cb::fs::read_text(cb::fs::from_text(*source_path));
    constexpr int kRounds = 20;
    double parse_ms = 0, edits_ms = 0;
    std::size_t edits = 0;
    for (int i = 0; i < kRounds; ++i) {
        const auto t0 = std::chrono::steady_clock::now();
        const std::optional<a16::Tree> tree = a16::parse_tree(json);
        const auto t1 = std::chrono::steady_clock::now();
        const std::vector<a16::Edit> e = a16::edits_of(*tree, text, tree->find_file(cb::fs::to_text(cb::fs::from_text(*source_path).filename())));
        const auto t2 = std::chrono::steady_clock::now();
        parse_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();
        edits_ms += std::chrono::duration<double, std::milli>(t2 - t1).count();
        edits = e.size();
    }
    std::fprintf(stderr, "arith16 bench: %zu bytes of json, parse %.2f ms, edits_of %.2f ms (%zu edits), per round, %d rounds\n",
                 json.size(), parse_ms / kRounds, edits_ms / kRounds, edits, kRounds);
});

}  // namespace
