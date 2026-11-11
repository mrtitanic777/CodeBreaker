#include "cb/base/ctext.hpp"

#include <algorithm>

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
using ctext::Mode;

CB_TEST("base.ctext.blank_inside_literals", [] {
    CB_CHECK_EQ(ctext::blank(""), "");
    CB_CHECK_EQ(ctext::blank("int x = 1;"), "int x = 1;");
    CB_CHECK_EQ(ctext::blank("/* c */ int x;"), "        int x;");
    // a comment keeps its newlines, loses everything else, delimiters included
    CB_CHECK_EQ(ctext::blank("/* a\nb\nc */ x"), "    \n \n     x");
    // a string keeps its quotes
    CB_CHECK_EQ(ctext::blank("\"str\" x"), "\"   \" x");
    CB_CHECK_EQ(ctext::blank("'a' 'b'"), "' ' ' '");
    // an escaped quote does not end the string
    CB_CHECK_EQ(ctext::blank("\"a\\\"b\" c"), "\"    \" c");
    CB_CHECK_EQ(ctext::blank("'\\''"), "'  '");
    // a comment marker inside a string, and a quote inside a comment, are not what they look like
    CB_CHECK_EQ(ctext::blank("x /* \"in comment\" */ y \"/* in string */\""),
                "x" + std::string(20, ' ') + "y \"" + std::string(15, ' ') + "\"");
});

CB_TEST("base.ctext.open_literals_run_to_the_end", [] {
    CB_CHECK_EQ(ctext::blank("/* open comment"), "               ");
    CB_CHECK_EQ(ctext::blank("\"open string\nnext"), "\"           \nnext");
    CB_CHECK_EQ(ctext::blank("\"a\\"), "\" \\");   // a trailing backslash is not part of the string
    CB_CHECK_EQ(ctext::blank("\""), "\"");
    // "/*/" opens a comment, it does not also close one
    CB_CHECK_EQ(ctext::blank("/*/"), "   ");
    CB_CHECK_EQ(ctext::blank("/**/"), "    ");
    CB_CHECK_EQ(ctext::blank("/* /* */ */"), "         */");
    // DIVERGES: an open string whose last character is an escaped quote stays open: its final quote is blanked too.
    // The Python took the final quote for the closing one and left it showing (`"abc\"` -> `"   "`).
    CB_CHECK_EQ(ctext::blank("\"abc\\\"\nx"), "\"     \nx");
});

CB_TEST("base.ctext.blank_whole_closed", [] {
    CB_CHECK_EQ(ctext::blank("\"str\" x", Mode::WholeClosed), "      x");
    CB_CHECK_EQ(ctext::blank("/* c */ int x;", Mode::WholeClosed), "        int x;");
    // a comment that never closes is not a comment here: what is inside it is read as code
    CB_CHECK_EQ(ctext::blank("/* open comment", Mode::WholeClosed), "/* open comment");
    CB_CHECK_EQ(ctext::blank("/* \"s\" ", Mode::WholeClosed), "/*     ");
    // a quote with no partner on its line is left alone (an apostrophe), the next line is not looked at
    CB_CHECK_EQ(ctext::blank("don't \"quote\" it's", Mode::WholeClosed), "don" + std::string(14, ' ') + "s");
    CB_CHECK_EQ(ctext::blank("'it's", Mode::WholeClosed), "    s");
    CB_CHECK_EQ(ctext::blank("\"open\nnext\"", Mode::WholeClosed), "\"open\nnext\"");
    // a comment's newlines go with it here
    CB_CHECK_EQ(ctext::blank("a/* x\ny */b", Mode::WholeClosed), "a" + std::string(9, ' ') + "b");
});

CB_TEST("base.ctext.blank_whole_across_lines", [] {
    CB_CHECK_EQ(ctext::blank("\"multi\nline\" x", Mode::WholeClosedAcrossLines), std::string(13, ' ') + "x");
    CB_CHECK_EQ(ctext::blank("'multi\nline' x", Mode::WholeClosedAcrossLines), std::string(13, ' ') + "x");
    CB_CHECK_EQ(ctext::blank("\"multi\nline\" x", Mode::WholeClosed), "\"multi\nline\" x");
    // a backslash before the newline continues the line in either mode
    CB_CHECK_EQ(ctext::blank("\"a\\\nb\" x", Mode::WholeClosed), "       x");
});

CB_TEST("base.ctext.offsets_and_lines_are_preserved", [] {
    const std::string text = "printf(\"%d\\n\", x); /* done\n  */ // tail\r\n'\\''";
    for (Mode m : {Mode::InsideLiterals, Mode::WholeClosed, Mode::WholeClosedAcrossLines}) {
        const std::string b = ctext::blank(text, m);
        CB_CHECK_EQ(b.size(), text.size());
    }
    const std::string b = ctext::blank(text);
    CB_CHECK_EQ(std::count(b.begin(), b.end(), '\n'), std::count(text.begin(), text.end(), '\n'));
    // multi-byte characters inside a literal are blanked byte by byte: the length stays
    CB_CHECK_EQ(ctext::blank("\"\xC3\xA9\" x"), "\"  \" x");
});

CB_TEST("base.ctext.sub_code_ignores_literals_and_comments", [] {
    static const Regex digits(R"([0-9]+)");
    const auto wrap = [](const RegexMatch& m) { return "<" + std::string(m.whole()) + ">"; };
    CB_CHECK_EQ(ctext::sub_code("a1 \"2\" /* 3 */ b4 '5'", digits, wrap), "a<1> \"2\" /* 3 */ b<4> '5'");
    static const Regex word(R"(\bint\b)");
    CB_CHECK_EQ(ctext::sub_code("int x; \"int\" int", word, [](const RegexMatch&) { return std::string("long"); }),
                "long x; \"int\" long");
    CB_CHECK_EQ(ctext::sub_code("", digits, wrap), "");
});
