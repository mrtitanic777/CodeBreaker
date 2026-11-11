// Tests of cb/port/c_source: the scanners of old C. The expected values were produced by the Python original
// (codebreaker/port/pre_ansi.py) on these inputs and reviewed; a place where the new code answers differently is in
// a DIVERGES comment.
#include "cb/port/c_source.hpp"

#include "cb/base/ctext.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

std::string show_spans(const std::optional<std::vector<Span>>& s) {
    if (!s) return "none";
    std::string out;
    for (const Span& x : *s) out += "[" + std::to_string(x.begin) + "," + std::to_string(x.end) + "]";
    return out;
}

std::string show_decl(const std::optional<Declaration>& d) {
    if (!d) return "none";
    std::string out = d->base + " |";
    for (const Declarator& x : d->names) out += " " + x.name + "=" + x.text;
    return out;
}

constexpr const char* kOld = R"(/* old style */
f(a, b)
int a;
char *b;
{
    return a;
}

static char *
g(x, y)
register int x;
{
    y = x;
}

struct node *h(p)
struct node *p;
{
    p->next = 0;
    return p;
}
)";

}  // namespace

CB_TEST("port.c_source.vocabulary", [] {
    CB_CHECK(is_keyword("while"));
    CB_CHECK(is_keyword("volatile"));
    CB_CHECK(!is_keyword("FILE"));
    CB_CHECK(!is_keyword(""));
    CB_CHECK(is_type_word("FILE"));
    CB_CHECK(is_type_word("uintptr_t"));
    CB_CHECK(is_type_word("volatile"));
    CB_CHECK(!is_type_word("far"));
    CB_CHECK(!is_type_word("size_t"));
});

CB_TEST("port.c_source.rules_are_on_unless_the_environment_says", [] {
    CB_CHECK(rule_enabled("alloc_places"));
    {
        fs::ScopedEnv env("CB_PORT_RULES", "-alloc_places, +bss_room");
        CB_CHECK(!rule_enabled("alloc_places"));
        CB_CHECK(rule_enabled("bss_room"));
        CB_CHECK(rule_enabled("bss_ptr_tables"));
    }
    {
        fs::ScopedEnv env("CB_PORT_RULES", "-all,bss_room");
        CB_CHECK(!rule_enabled("alloc_places"));
        CB_CHECK(rule_enabled("bss_room"));   // the later word wins
    }
    CB_CHECK(rule_enabled("alloc_places"));
});

CB_TEST("port.c_source.lines_and_braces", [] {
    CB_CHECK_EQ(split_lines("a\n\nb\n").size(), 4u);   // (an empty piece after the last newline: the file's line numbers)
    CB_CHECK_EQ(split_lines("").size(), 1u);
    CB_CHECK_EQ(join_lines(split_lines("x\ny\n")), "x\ny\n");
    const std::vector<std::string> code = {"int f()", "{", "  if (x) {", "  }", "}"};
    CB_CHECK(brace_depths(code) == std::vector<int>{0, 0, 1, 2, 1});
    CB_CHECK_EQ(match_brace("{ a { b } c } d", 0), 12u);
    CB_CHECK_EQ(match_brace("{ never", 0), 7u);   // (it never closes: the end of the text)
});

CB_TEST("port.c_source.word_helpers", [] {
    CB_CHECK_EQ(collapse_space("a \t b\n\nc"), "a b c");
    CB_CHECK_EQ(find_word("xa a ab a", "a"), 3u);
    CB_CHECK_EQ(find_word("xa a ab a", "a", 4), 8u);
    CB_CHECK_EQ(find_word("ab", "a"), std::string_view::npos);
    CB_CHECK_EQ(regex_quote("struct a.b", true), "struct\\s+a\\.b");
    CB_CHECK_EQ(regex_quote("x(y)"), "x\\(y\\)");
    CB_CHECK_EQ(std::string(indent_of("  \tint x;")), "  \t");
    CB_CHECK(is_word_char('_') && is_word_char('9') && !is_word_char('-'));
});

CB_TEST("port.c_source.split_top_keeps_brackets_together", [] {
    const auto parts = split_top("a, f(b, c), d[1,2], e");
    CB_CHECK_EQ(parts.size(), 4u);
    CB_CHECK_EQ(std::string(parts[1]), " f(b, c)");
    CB_CHECK_EQ(std::string(parts[2]), " d[1,2]");
    CB_CHECK_EQ(split_top("").size(), 1u);
});

CB_TEST("port.c_source.declarators", [] {
    CB_CHECK_EQ(show_decl(declarators("register struct s *a, b[3]")), "register struct s | a=*a b=b[3]");
    CB_CHECK_EQ(show_decl(declarators("unsigned long x;")), "unsigned long | x=x");
    CB_CHECK_EQ(show_decl(declarators("extern int g[10], h")), "extern int | g=g[10] h=h");
    CB_CHECK_EQ(show_decl(declarators("int (*fp)(int, char *), z")), "int | fp=(*fp)(int, char *) z=z");
    CB_CHECK_EQ(show_decl(declarators("FILE *f")), "FILE | f=*f");
    CB_CHECK_EQ(show_decl(declarators("struct s;")), "none");   // (a tag alone: no variable)
    CB_CHECK_EQ(show_decl(declarators("int")), "none");
    // `far` is no type word: it is taken for the name; the callers strip far/near/huge first
    CB_CHECK_EQ(show_decl(declarators("char far *p, q")), "char | far=far *p q=q");
    // a number's tail is an identifier too (the scan finds `x1f` in `0x1f`): the old code's habit, kept
    CB_CHECK_EQ(show_decl(declarators("0x1f z")), "0 | x1f=x1f z");
});

CB_TEST("port.c_source.array_param", [] {
    CB_CHECK_EQ(array_param("int a[]", "a"), "int *a");
    CB_CHECK_EQ(array_param("int a[][3]", "a"), "int (*a)[3]");
    CB_CHECK_EQ(array_param("char *s[]", "s"), "char **s");
    CB_CHECK_EQ(array_param("struct t m[4][5]", "m"), "struct t (*m)[5]");
    CB_CHECK_EQ(array_param("int a", "a"), "int a");
    CB_CHECK_EQ(array_param("int ab[]", "a"), "int ab[]");   // (the name is a whole word)
});

CB_TEST("port.c_source.definitions_of_old_style_functions", [] {
    const std::vector<std::string> lines = split_lines(kOld);
    const std::vector<std::string> code = split_lines(base::ctext::blank(kOld));
    const std::vector<Definition> defs = definitions(lines, code);
    CB_CHECK_EQ(defs.size(), 3u);
    const Fn& f = defs[0].fn;
    CB_CHECK_EQ(f.name, "f");
    CB_CHECK_EQ(f.ret, "");
    CB_CHECK_EQ(f.params.size(), 2u);
    CB_CHECK_EQ(f.params[0].type, "int a");
    CB_CHECK_EQ(f.params[1].type, "char *b");
    CB_CHECK(!f.is_static);
    CB_CHECK(f.implicit.empty());
    CB_CHECK_EQ(defs[0].first, 1u);
    CB_CHECK_EQ(defs[0].open, 4u);
    CB_CHECK_EQ(defs[0].last, 6u);
    // the return type on the line before the name; a parameter never declared is an int
    const Fn& g = defs[1].fn;
    CB_CHECK_EQ(g.name, "g");
    CB_CHECK_EQ(g.ret, "char *");
    CB_CHECK(g.is_static);
    CB_CHECK_EQ(g.params[0].type, "register int x");
    CB_CHECK_EQ(g.params[1].type, "int y");
    CB_CHECK(g.implicit.contains("y") && g.implicit.size() == 1);
    CB_CHECK_EQ(defs[1].first, 8u);
    CB_CHECK_EQ(g.line, 10u);
    const Fn& h = defs[2].fn;
    CB_CHECK_EQ(h.ret, "struct node *");
    CB_CHECK_EQ(defs[2].last, 20u);
    CB_CHECK_EQ(f.proto(), "int f(int a, char *b)");
    CB_CHECK_EQ(g.proto(), "static char *g(register int x, int y)");
    Fn loose = f;
    loose.loose = true;
    CB_CHECK_EQ(loose.proto(), "int f()");
    Fn v = f;
    v.varargs = true;
    v.returns_void = true;
    CB_CHECK_EQ(v.proto(), "void f(int a, char *b, ...)");
    Fn none;
    none.name = "z";
    CB_CHECK_EQ(none.proto(), "int z(void)");
});

CB_TEST("port.c_source.definitions_of_a_file_that_never_closes", [] {
    // DIVERGES: a definition whose brace is never closed ends on the file's last line (the Python's end was one past it).
    const std::string text = "f(a)\nint a;\n{\n  x();\n";
    const auto lines = split_lines(text);
    const auto defs = definitions(lines, split_lines(base::ctext::blank(text)));
    CB_CHECK_EQ(defs.size(), 1u);
    CB_CHECK_EQ(defs[0].last, lines.size() - 1);
});

CB_TEST("port.c_source.definitions_any_new_style_too", [] {
    const std::string text = "int main(argc, argv)\n{\n}\nstatic int z(void)\n{\n}\nif (x)\n{\n}\n";
    const auto lines = split_lines(text);
    const auto defs = definitions_any(lines, lines);
    CB_CHECK_EQ(defs.size(), 2u);   // (`if (x)` is a keyword)
    CB_CHECK_EQ(defs[0].name, "main");
    CB_CHECK_EQ(defs[0].last, 2u);
    CB_CHECK_EQ(defs[1].name, "z");
    CB_CHECK_EQ(defs[1].open, 4u);
});

CB_TEST("port.c_source.call_args", [] {
    const std::string line = "x = f(a, g(b, c), d[1,2]); y = f(1)";
    CB_CHECK_EQ(show_spans(call_args(line, "f")), "[6,7][8,16][17,24]");
    CB_CHECK_EQ(show_spans(call_args(line, "g")), "[11,12][13,15]");
    CB_CHECK_EQ(show_spans(call_args(line, "h")), "none");
    CB_CHECK_EQ(show_spans(call_args("f(a, b", "f")), "none");   // (not closed: no call)
    CB_CHECK_EQ(show_spans(call_args("f(a, b", "f", true)), "[2,3]");   // (partial: the arguments whole on this line)
    CB_CHECK_EQ(show_spans(call_args("f(a, b)", "f", true)), "[2,3][4,6]");
    CB_CHECK_EQ(show_spans(call_args("f({1, 2}, 3)", "f")), "[2,8][9,11]");   // (braces nest here)
    CB_CHECK_EQ(show_spans(call_args("xf(a)", "f")), "none");   // (a whole word)
    CB_CHECK_EQ(show_spans(call_args("f (a)", "f")), "[3,4]");
});

CB_TEST("port.c_source.calls_of_lists_every_call", [] {
    const auto calls = calls_of("f(a, b); x = f(c, (d, e)); f(g(f(q)))", "f");
    CB_CHECK_EQ(calls.size(), 4u);
    CB_CHECK_EQ(show_spans(calls[0]), "[2,3][4,6]");
    CB_CHECK_EQ(show_spans(calls[1]), "[15,16][17,24]");
    CB_CHECK_EQ(show_spans(calls[2]), "[29,36]");
    CB_CHECK_EQ(show_spans(calls[3]), "[33,34]");   // (the call inside the argument of the one before)
    CB_CHECK(call_starts("f(a); g(f(b))", "f") == std::vector<std::size_t>{0, 8});
});

CB_TEST("port.c_source.object_before_finds_the_start_of_a_postfix_expression", [] {
    const std::string ob = "r = p->q[2].s->t; z = (p + 1)->m; u = a(b)->c; w = q . r -> s";
    CB_CHECK_EQ(object_before(ob, ob.find("->t")).value_or(9999), 4u);        // p->q[2].s
    CB_CHECK_EQ(object_before(ob, ob.find("->m")).value_or(9999), 22u);       // (p + 1): a parenthesized object, whole
    CB_CHECK_EQ(object_before(ob, ob.find("->c")).value_or(9999), 38u);       // a(b): a call
    CB_CHECK_EQ(object_before(ob, ob.find("-> s")).value_or(9999), 55u);      // q . r
    CB_CHECK_EQ(object_before(ob, ob.find(". r")).value_or(9999), 51u);
    // DIVERGES: brackets that do not balance give no start (the Python read the line's end from the wrong side).
    CB_CHECK(!object_before("a) ->b", 3).has_value());
});

CB_TEST("port.c_source.abstract_type", [] {
    CB_CHECK_EQ(abstract_type("register char *name", "name"), "char *");
    CB_CHECK_EQ(abstract_type("struct bcd *bcd", "bcd"), "struct bcd *");   // (the name, not the tag)
    CB_CHECK_EQ(abstract_type("int a[]", "a"), "int *");
    CB_CHECK_EQ(abstract_type("int a[10]", "a"), "int *");
    CB_CHECK_EQ(abstract_type("char  *  p", "p"), "char *");
    CB_CHECK_EQ(abstract_type("union u x", "x"), "union u");
});

CB_TEST("port.c_source.only_tested", [] {
    CB_CHECK(!only_tested("if (p) { x = 1; } if (!p) y = 2; z = p && q;", "p"));   // (`z = p && q`: a use as a value)
    CB_CHECK(!only_tested("f(p);", "p"));        // (a call's argument is no test)
    CB_CHECK(!only_tested("", "p"));
    CB_CHECK(!only_tested("x = p;", "p"));
    CB_CHECK(only_tested("if (a && p) b;", "p"));
    CB_CHECK(only_tested("return (p);", "p"));
    CB_CHECK(only_tested("if (p) a(); while (!p) b();", "p"));
});

CB_TEST("port.c_source.struct_members", [] {
    const std::string src = "struct s { int a; char *p; };\nunion u { struct { char kind; int len; } a; struct { char *p; int q; } b; long l; };\n"
                            "struct t { struct s one; char name[5]; far char *fp; int arr[3]; };\n";
    const auto m = struct_members({src});
    CB_CHECK(m.at("a") == std::vector<std::string>{"struct s", "union u"});
    CB_CHECK(m.at("p") == std::vector<std::string>{"struct s", "union u"});
    CB_CHECK(m.at("kind") == std::vector<std::string>{"union u"});
    CB_CHECK(m.at("far") == std::vector<std::string>{"struct t"});   // (the old scan takes `far` for the name: kept)
    CB_CHECK_EQ(m.size(), 11u);
    CB_CHECK_EQ(m.entries().front().first, "a");   // (in the order first seen)
});

CB_TEST("port.c_source.c_string_bytes", [] {
    auto s = [](const Bytes& b) { return std::string(as_text(b)); };
    CB_CHECK_EQ(s(c_string_bytes("\"a\\0b\" \"c\\n\"")), std::string("a\0bc\n", 5));
    CB_CHECK_EQ(s(c_string_bytes("\"\\x41\\101\\\\\"")), "AA\\");
    CB_CHECK_EQ(s(c_string_bytes("\"no\\tq\"")), "no\tq");
    CB_CHECK_EQ(s(c_string_bytes("x \"ab\" y")), "ab");
    CB_CHECK_EQ(s(c_string_bytes("")), "");
    CB_CHECK_EQ(s(c_string_bytes("\"\\q\"")), "\\q");   // (an escape C does not have is kept as written)
    // a character past Latin-1 is '?'; one of Latin-1 is its byte (text from a binary is decoded once, at the reader)
    CB_CHECK_EQ(s(c_string_bytes("\"\xC3\xA9\"")), "\xE9");
    CB_CHECK_EQ(s(c_string_bytes("\"\xE2\x82\xAC\"")), "?");
    CB_CHECK_THROWS_AS(ParseError, (void)c_string_bytes("\"\\x4\""));   // DIVERGES: a \x escape wants two hex digits (the Python raised too, from deep inside)
});
