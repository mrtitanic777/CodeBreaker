#include "cb/port/csource.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::port::csource;

CB_TEST("port.csource.lines_and_depths", [] {
    CB_CHECK_EQ(split_lines("a\nb\n").size(), 3u); // (the empty piece after the last newline stays: Python's split)
    CB_CHECK_EQ(join_lines(split_lines("a\r\nb\n")), "a\r\nb\n");
    CB_CHECK_EQ(split_lines("").size(), 1u);
    const auto ds = depths(split_lines("f()\n{\n  if (x) {\n    y;\n  }\n}\n"));
    CB_CHECK(ds == (std::vector<int>{0, 0, 1, 2, 2, 1, 0}));
    // an unbalanced close does not go below what the text says
    CB_CHECK(depths(split_lines("}\nx")) == (std::vector<int>{0, -1}));
});

CB_TEST("port.csource.call_args", [] {
    const auto spans = call_args("x = f(a, g(b, c), d[1, 2]);", "f");
    CB_CHECK(spans.has_value());
    CB_CHECK_EQ(spans->size(), 3u);
    const std::string line = "x = f(a, g(b, c), d[1, 2]);";
    CB_CHECK_EQ(line.substr((*spans)[0].start, (*spans)[0].end - (*spans)[0].start), "a");
    CB_CHECK_EQ(line.substr((*spans)[1].start, (*spans)[1].end - (*spans)[1].start), " g(b, c)");
    CB_CHECK_EQ(line.substr((*spans)[2].start, (*spans)[2].end - (*spans)[2].start), " d[1, 2]");
    // a call with no arguments has one empty one, as the Python's did
    CB_CHECK_EQ(call_args("f()", "f")->size(), 1u);
    // the first call of that name; a longer name that ends the same way is another
    CB_CHECK(!call_args("xf(1)", "f").has_value());
    CB_CHECK(call_args("xf(1) + f (2, 3)", "f")->size() == 2u);
    // not closed on this line: only with `partial`, and only when an argument closed
    CB_CHECK(!call_args("f(a, g(b,", "f").has_value());
    CB_CHECK(call_args("f(a, g(b,", "f", true).has_value());
    CB_CHECK(!call_args("f(", "f", true).has_value());
    CB_CHECK(!call_args("no call here", "f").has_value());
});

CB_TEST("port.csource.call_at", [] {
    const std::string line = "  r = outer(a, inner(b, c), d);";
    const auto at_b = call_at(line, line.find('b'));
    CB_CHECK(at_b.has_value());
    CB_CHECK_EQ(at_b->function, "inner");
    CB_CHECK_EQ(at_b->index, 0u);
    const auto at_d = call_at(line, line.find('d'));
    CB_CHECK_EQ(at_d->function, "outer");
    CB_CHECK_EQ(at_d->index, 2u);
    // keywords are not calls; a column in no call is none
    CB_CHECK(!call_at("if (x) y;", 4).has_value());
    CB_CHECK(!call_at("  r = 1;", 2).has_value());
    // the smaller of the calls that hold the column (h is no call)
    const auto both = call_at("f(g(h))", 4);
    CB_CHECK_EQ(both->function, "g");
    CB_CHECK_EQ(call_at("f(g(h))", 2)->function, "f");
});

CB_TEST("port.csource.object_before", [] {
    const std::string l = "x = s->a[2].b->next";
    CB_CHECK_EQ(object_before(l, l.rfind("->")), 4u); // s->a[2].b
    const std::string c = "y = (*p)->q + f(a)->z";
    CB_CHECK_EQ(object_before(c, c.find("->")), 4u); // (*p)
    CB_CHECK_EQ(object_before(c, c.rfind("->")), c.find("f(a)"));
    CB_CHECK_EQ(object_before("((int *)p)->x", 10), 0u); // a parenthesized object: the whole
    // DIVERGES: an unbalanced bracket was an index of -1 in the Python; the object starts at the line's start here
    CB_CHECK_EQ(object_before("a) ->x", 3), 0u);
});

CB_TEST("port.csource.declarators", [] {
    const auto d = declarators("register struct s *a, b[3];");
    CB_CHECK(d.has_value());
    CB_CHECK_EQ(d->base, "register struct s");
    CB_CHECK_EQ(d->declarators.size(), 2u);
    CB_CHECK_EQ(d->declarators[0].name, "a");
    CB_CHECK_EQ(d->declarators[0].text, "*a");
    CB_CHECK_EQ(d->declarators[1].name, "b");
    CB_CHECK_EQ(d->declarators[1].text, "b[3]");
    const auto fp = declarators("int (*cmp)(char *, char *)");
    CB_CHECK_EQ(fp->declarators[0].name, "cmp");
    CB_CHECK_EQ(fp->base, "int");
    CB_CHECK(!declarators("unsigned long").has_value());
    CB_CHECK(!declarators("").has_value());
    // a comma inside parentheses does not split
    CB_CHECK_EQ(declarators("int a, (*f)(int, int)")->declarators.size(), 2u);
    CB_CHECK(split_top("a, f(b, c), [d, e]") == (std::vector<std::string>{"a", " f(b, c)", " [d, e]"}));
});

CB_TEST("port.csource.abstract_type_and_ctype", [] {
    CB_CHECK_EQ(abstract_type("register char *name", "name"), "char *");
    CB_CHECK_EQ(abstract_type("struct bcd *bcd", "bcd"), "struct bcd *"); // the name, not the tag
    CB_CHECK_EQ(abstract_type("int tab[]", "tab"), "int *");
    CB_CHECK_EQ(abstract_type("char   *p", "p"), "char *");
    TagSet tags{"foo", "node"};
    CB_CHECK_EQ(ctype("foo *", tags), "struct foo *");
    CB_CHECK_EQ(ctype("struct foo *", tags), "struct foo *"); // never twice
    CB_CHECK_EQ(ctype("union foo", tags), "union foo");
    CB_CHECK_EQ(ctype("__cdecl foo", tags), "struct foo");
    CB_CHECK_EQ(ctype("char * (aka 'char *')"), "char *");
    CB_CHECK_EQ(ctype("  const   char  *  "), "const char *");
    CB_CHECK_EQ(ctype("int (*)(struct node *, bar)", tags), "int (*)(struct node *, bar)");
});

CB_TEST("port.csource.used_as_number", [] {
    CB_CHECK(used_as_number("n /= 2;", "n"));
    CB_CHECK(used_as_number("x = n * v;", "n"));
    CB_CHECK(used_as_number("x = v << n;", "n"));
    CB_CHECK(used_as_number("if (n > 3) f();", "n"));
    CB_CHECK(used_as_number("a = (off + n) & ~7;", "n") == false); // (n is added, not computed on alone)
    CB_CHECK(used_as_number("t = b[n];", "n"));
    CB_CHECK(used_as_number("t = b[++n];", "n"));
    CB_CHECK(!used_as_number("p = q;", "n"));
    CB_CHECK(!used_as_number("count = 1; f(count);", "n")); // a word of the same letters
    CB_CHECK(!used_as_number("*n = 1;", "n"));
    CB_CHECK(used_as_number("x = (n + 1) & mask;", "n"));
    CB_CHECK(!used_as_number("if (a && n) y;", "n"));
});

CB_TEST("port.csource.only_tested", [] {
    CB_CHECK(only_tested("if (p) return 1; if (!p || q) f();", "p"));
    CB_CHECK(only_tested("while (p && q) r();", "p"));
    CB_CHECK(!only_tested("if (p) *p = 1;", "p"));
    CB_CHECK(!only_tested("f(p);", "p"));   // a call's argument is no test
    CB_CHECK(!only_tested("return;", "p")); // never used: not "only tested"
    CB_CHECK(only_tested("x = (p) && y;", "p"));
    CB_CHECK(!only_tested("x = pp;", "p"));
});

CB_TEST("port.csource.members", [] {
    const std::string a = "struct node { struct node *next; int16_t val; char *name; };\nunion u { int i; char *s; };";
    const std::string b = "struct other { int val; };";
    const auto m = members({a, b});
    CB_CHECK(m.find("val") != nullptr);
    CB_CHECK((*m.find("val") == std::vector<std::string>{"struct node", "struct other"}));
    CB_CHECK((*m.find("s") == std::vector<std::string>{"union u"}));
    CB_CHECK(m.find("zzz") == nullptr);
    // text inside a comment or string is not a member
    const auto c = members({std::string("struct t { int a; /* int hidden; */ char *s; };")});
    CB_CHECK(c.find("hidden") == nullptr && c.find("a") != nullptr);
});

CB_TEST("port.csource.source_index", [] {
    // a new-style definition, an old-style one (parameters declared after the head, the type on the line before)
    const std::string text = "int16_t add(int16_t a, int16_t b)\n{\n    int16_t t;\n    return a + b + t;\n}\n"
                             "char *\nold(p, n)\nchar *p;\nint n;\n{\n    return p + n;\n}\n"
                             "int x = 3;\n";
    const SourceIndex index(text);
    CB_CHECK_EQ(index.functions().size(), 2u);
    CB_CHECK_EQ(index.functions()[0].name, "add");
    CB_CHECK_EQ(index.functions()[0].first, 0u);
    CB_CHECK_EQ(index.functions()[0].open, 1u);
    CB_CHECK_EQ(index.functions()[0].last, 4u);
    CB_CHECK_EQ(index.functions()[1].name, "old");
    CB_CHECK_EQ(index.functions()[1].first, 5u); // the line with `char *`
    CB_CHECK_EQ(index.functions()[1].open, 9u);
    CB_CHECK_EQ(index.functions()[1].last, 11u);
    CB_CHECK_EQ(index.enclosing(4)->name, "add"); // 1-based, as a compiler counts
    CB_CHECK_EQ(index.enclosing(11)->name, "old");
    CB_CHECK(index.enclosing(13) == nullptr);
    CB_CHECK(index.enclosing(0) == nullptr);
    CB_CHECK(index.declares_local(*index.enclosing(4), "t"));
    CB_CHECK(index.declares_local(*index.enclosing(4), "a"));
    CB_CHECK(!index.declares_local(*index.enclosing(4), "x"));
    CB_CHECK(!index.declares_local(*index.enclosing(4), "tt"));
    // braces that never close: the function runs to the end
    const SourceIndex open_one("void f(int a)\n{\n    a = 1;\n");
    CB_CHECK_EQ(open_one.functions().size(), 1u);
    CB_CHECK_EQ(open_one.functions()[0].last, 4u); // the line count: the braces never close
    // a keyword with parentheses is no function
    CB_CHECK(SourceIndex("if (x)\n{\n}\n").functions().empty());
    CB_CHECK(SourceIndex("").functions().empty());
});

CB_TEST("port.csource.find_tag_body_and_edits", [] {
    const std::string code = "struct a { int x; };\nunion a { int y; };\nstruct  b\n{ int z; };";
    CB_CHECK_EQ(find_tag_body(code, "struct", "a"), 10u);
    CB_CHECK_EQ(find_tag_body(code, "union", "a"), 30u);
    CB_CHECK(find_tag_body(code, "struct", "b") != std::string::npos);
    CB_CHECK(find_tag_body(code, "struct", "ab") == std::string::npos);
    CB_CHECK(find_tag_body(code, "struct", "z") == std::string::npos);
    CB_CHECK(find_tag_body("mystruct a {", "struct", "a") == std::string::npos); // a whole word
    CB_CHECK_EQ(apply_edits("0123456789", {{2, 4, "AB"}, {6, 6, "-"}, {8, 10, ""}}), "01AB45-67");
    const LineIndex lines("ab\ncd\n\nef");
    CB_CHECK_EQ(lines.line_of(0), 0u);
    CB_CHECK_EQ(lines.line_of(2), 0u);
    CB_CHECK_EQ(lines.line_of(3), 1u);
    CB_CHECK_EQ(lines.line_of(6), 2u);
    CB_CHECK_EQ(lines.line_of(8), 3u);
});

CB_TEST("port.csource.name_regex", [] {
    CB_CHECK(name_regex(R"(\b@@\s*=)", "a.b").test("x a.b = 1"));
    CB_CHECK(!name_regex(R"(\b@@\s*=)", "a.b").test("x aXb = 1")); // the dot is a dot
    CB_CHECK(name_regex(R"(@1@->@2@)", "p", "q").test("p->q"));
    CB_CHECK_EQ(regex_escape("a+b(c)"), "a\\+b\\(c\\)");
    CB_CHECK(is_keyword("while") && !is_keyword("whale"));
    CB_CHECK(is_type_word("FILE") && is_type_word("intptr_t") && !is_type_word("node"));
});
