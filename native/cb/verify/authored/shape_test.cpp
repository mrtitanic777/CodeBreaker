// Tests of the shape rewrites. The bodies are the decompiler's C as the repair step sees it (the text inside the function's
// braces, closing brace included); the expected rewrites were taken from the original module and reviewed.
#include "cb/verify/authored/shape.hpp"

#include "cb/core/testing.hpp"

namespace {
namespace sh = cb::verify::authored::shape;
using sh::Rewrite;
using sh::Span;

std::vector<Rewrite> switches(const std::string& body) { return sh::switches(body, body); }
std::vector<Rewrite> folds(const std::vector<std::string>& decls, const std::string& body) { return sh::folds(decls, body, body); }
std::optional<std::string> returns(const std::string& head, const std::vector<std::string>& decls, const std::string& body) {
    return sh::returns(head, decls, body, body);
}

const std::string kHead = "int4 __stdcall foo(int4 a, int4 b)\n{";
const std::vector<std::string> kDecls = {"int4 r;", "int4 t;"};
}  // namespace

CB_TEST("verify.shape.statements", [] {
    const std::string s = "{ a = 1; if (x) { b(); } else { c(); } do { d(); } while (e); f(g(1; 2)); }";
    // an if with all its elses is one statement, a do with its while is one, a `;` inside parentheses ends nothing
    CB_CHECK((sh::statements(s, 1, s.size() - 1) == std::vector<Span>{{2, 8}, {9, 38}, {39, 61}, {62, 73}}));
    const std::string chain = "if (a) { x; } else if (b) { y; } else { z; } w;";
    CB_CHECK((sh::statements(chain, 0, 45) == std::vector<Span>{{0, 44}}));
    // a block that is not closed within `hi` ends the list
    CB_CHECK((sh::statements("{ a = 1; if (x) { b(", 1, 20) == std::vector<Span>{{2, 8}}));
    CB_CHECK(sh::statements("", 0, 0).empty());
    CB_CHECK(sh::statements("   ", 0, 3).empty());
    // DIVERGES: a `hi` past the end of the text was an IndexError in the Python; it is the end of the text here
    CB_CHECK((sh::statements("a = 1; b();", 0, 100) == std::vector<Span>{{0, 6}, {7, 11}}));
    CB_CHECK(sh::statements("a = 1;", 10, 100).empty());
    // zeros alone are not octal: `00` is zero
    CB_CHECK_EQ(switches("if (x == 00) { a(); } else if (x == 2) { b(); }\n}").size(), std::size_t{1});
});

CB_TEST("verify.shape.switches", [] {
    // a chain with an else
    const auto one = switches(
        "\n    if (x == 1) {\n        a();\n    }\n    else if (x == 2) {\n        b();\n    }\n    else {\n        c();\n    }\n    return 0;\n}");
    CB_CHECK_EQ(one.size(), std::size_t{1});
    CB_CHECK_EQ(one[0].begin, std::size_t{5});
    CB_CHECK_EQ(one[0].end, std::size_t{109});
    CB_CHECK_EQ(one[0].text, std::string("switch (x) {\ncase 1:\n{\n        a();\n    }\nbreak;\ncase 2:\n{\n        b();\n    }\nbreak;\ndefault:\n{\n        c();\n    }\n}"));
    // a hex constant is kept as written
    const auto hex = switches("\n    if (x == 1) {\n        a();\n    } else if (x == 0x10) { b(); }\n    y = 2;\n}");
    CB_CHECK_EQ(hex.size(), std::size_t{1});
    CB_CHECK_EQ(hex[0].text, std::string("switch (x) {\ncase 1:\n{\n        a();\n    }\nbreak;\ncase 0x10:\n{ b(); }\nbreak;\n}"));
    // two chains, on different values
    const auto two = switches(
        "\n    p();\n    if (x == 1) { a(); } else if (x == 2) { b(); }\n    q();\n    if (v == 3) { c(); } else if (v == 4) { d(); } else { e(); }\n}");
    CB_CHECK_EQ(two.size(), std::size_t{2});
    CB_CHECK_EQ(two[0].begin, std::size_t{14});
    CB_CHECK_EQ(two[0].end, std::size_t{60});
    CB_CHECK_EQ(two[1].text, std::string("switch (v) {\ncase 3:\n{ c(); }\nbreak;\ncase 4:\n{ d(); }\nbreak;\ndefault:\n{ e(); }\n}"));
    // the chain may start the body
    CB_CHECK_EQ(switches("if (x == 1) { a(); } else if (x == 2) { b(); }\n}").size(), std::size_t{1});
    // not a switch: one test only; a value tested twice (-1 and 0xFFFFFFFF are the same 32 bits too); a break that would
    // leave the switch instead of the loop; a chain that starts with another kind of test
    CB_CHECK(switches("\n    if (x == 1) { a(); }\n}").empty());
    CB_CHECK(switches("\n    if (x == 1) { a(); } else if (x == 1) { b(); }\n}").empty());
    CB_CHECK(switches("\n    if (x == -1) { a(); } else if (x == 0xFFFFFFFF) { b(); }\n}").empty());
    CB_CHECK(switches("\n    while (i) {\n        if (x == 1) { a(); } else if (x == 2) { break; }\n    }\n}").empty());
    CB_CHECK(switches("\n    if (y) { z(); } else if (x == 1) { a(); } else if (x == 2) { b(); }\n}").empty());
    // a chain that is not closed
    CB_CHECK(switches("\n    if (x == 1) { a(); } else if (x == 2) { b(); ").empty());
    // DIVERGES: a constant written in octal (a leading 0: the Python's int(x, 0) raised ValueError and the whole call
    // failed) is left alone, so that chain is not rewritten; chains with other constants still are
    CB_CHECK(switches("\n    if (x == 012) { a(); } else if (x == 2) { b(); }\n}").empty());
    CB_CHECK_EQ(switches("\n    if (x == 012) { a(); } else if (x == 2) { b(); }\n    if (v == 5) { a(); } else if (v == 6) { b(); }\n}").size(), std::size_t{1});
});

CB_TEST("verify.shape.returns", [] {
    // an `r = x;` that only the final `return r;` follows is `return x;`: in each branch of an if
    CB_CHECK_EQ(returns(kHead, kDecls, "\n    if (a == 1) {\n        r = 5;\n    }\n    else {\n        r = foo(a);\n    }\n    return r;\n}").value(),
                std::string("\n    if (a == 1) {\n        return 5;\n    }\n    else {\n        return foo(a);\n    }\n    return r;\n}"));
    CB_CHECK_EQ(returns(kHead, kDecls, "\n    r = 1;\n    return r;\n}").value(), std::string("\n    return 1;\n    return r;\n}"));
    CB_CHECK_EQ(returns(kHead, kDecls, "\n    if (a) {\n        r = 1;\n    }\n    else if (b) {\n        r = 2;\n    }\n    else {\n        r = 3;\n    }\n    return r;\n}").value(),
                std::string("\n    if (a) {\n        return 1;\n    }\n    else if (b) {\n        return 2;\n    }\n    else {\n        return 3;\n    }\n    return r;\n}"));
    CB_CHECK_EQ(returns(kHead, kDecls, "\n    a = 0;\n    if (a) {\n        if (b) {\n            r = 1;\n        }\n        else {\n            r = 2;\n        }\n    }\n    return r;\n}").value(),
                std::string("\n    a = 0;\n    if (a) {\n        if (b) {\n            return 1;\n        }\n        else {\n            return 2;\n        }\n    }\n    return r;\n}"));
    // r may be a parameter of the function's own return type
    CB_CHECK_EQ(returns("int4 f(int4 a, int4 r) {", {}, "\n  r = a + 1;\n  return r;\n}").value(), std::string("\n  return a + 1;\n  return r;\n}"));
    // not when r is narrower than what the function returns; not when something else follows the assignment; not when the
    // return is the only statement
    CB_CHECK(!returns(kHead, {"int2 r;"}, "\n    r = 1;\n    return r;\n}"));
    CB_CHECK(!returns(kHead, kDecls, "\n    r = 1;\n    t = r;\n    return r;\n}"));
    CB_CHECK(!returns(kHead, kDecls, "\n    return r;\n}"));
    CB_CHECK(!returns(kHead, kDecls, ""));
});

CB_TEST("verify.shape.returned_type", [] {
    CB_CHECK_EQ(sh::returned_type("static int4 __stdcall foo(int4 a)").value(), std::string("int4"));
    CB_CHECK_EQ(sh::returned_type("int4 __cdecl h(int2 a, int4 r)\n{").value(), std::string("int4"));
    CB_CHECK(!sh::returned_type("int4\nbar(int4 a,\n   int4 r)\n{"));   // the heading line has the name alone
    CB_CHECK(!sh::returned_type("foo("));
    CB_CHECK(!sh::returned_type(""));
});

CB_TEST("verify.shape.folds", [] {
    const std::vector<std::string> decls = {"int4 t;", "int4 u;"};
    const auto one = folds(decls, "\n    t = foo(a, b);\n    if (t == 0) {\n        x = 1;\n    }\n}");
    CB_CHECK_EQ(one.size(), std::size_t{1});
    CB_CHECK_EQ(one[0].begin, std::size_t{1});
    CB_CHECK_EQ(one[0].end, std::size_t{37});
    CB_CHECK_EQ(one[0].text, std::string("    if (foo(a, b) == 0) {"));
    CB_CHECK_EQ(folds(decls, "\n    t = foo(a);\n    if (t != -1) {\n    }\n}")[0].text, std::string("    if (foo(a) != -1) {"));
    CB_CHECK_EQ(folds(decls, "\n    t = foo(g(a), b);\n    if (t >= 5) {\n    }\n}")[0].text, std::string("    if (foo(g(a), b) >= 5) {"));
    // not when t is used again; not when two calls make the value; not for a narrower variable
    CB_CHECK(folds(decls, "\n    t = foo(a);\n    if (t == 0) {\n        x = t;\n    }\n}").empty());
    CB_CHECK(folds(decls, "\n    t = foo(a) + g(b);\n    if (t < 0x10) {\n    }\n}").empty());
    CB_CHECK(folds({"int2 t;"}, "\n    t = foo(a);\n    if (t == 0) {\n    }\n}").empty());
    CB_CHECK(folds({}, "\n    t = foo(a);\n    if (t == 0) {\n    }\n}").empty());
});

CB_TEST("verify.shape.the_shadow_blanks_strings", [] {
    // what is found in the shadow (strings and comments blanked) is written from the body
    const std::string body = "\n    puts(\"x; if (x == 1) { a(); } else if (x == 2) { b(); }\");\n}";
    std::string shadow = body;
    const std::size_t open = shadow.find('"'), close = shadow.rfind('"');
    for (std::size_t i = open + 1; i < close; ++i) shadow[i] = ' ';
    CB_CHECK(sh::switches(body, shadow).empty());
    CB_CHECK_EQ(sh::switches(body, body).size(), std::size_t{1});   // (without the blanking the string would be taken for code)
});
