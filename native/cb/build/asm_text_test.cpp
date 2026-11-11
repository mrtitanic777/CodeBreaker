#include "cb/build/asm_text.hpp"

#include "cb/core/testing.hpp"

namespace {
using namespace cb::build;
}

CB_TEST("build.asm_text.strip_comment_respects_strings", [] {
    CB_CHECK_EQ(strip_comment("LD A,5 ; load"), "LD A,5 ");
    CB_CHECK_EQ(strip_comment("; whole line"), "");
    CB_CHECK_EQ(strip_comment(".DB \"a;b\", 1 ; tail"), ".DB \"a;b\", 1 ");
    CB_CHECK_EQ(strip_comment("no comment"), "no comment");
    CB_CHECK_EQ(strip_comment("\"unterminated ; string"), "\"unterminated ; string");
});

CB_TEST("build.asm_text.split_items", [] {
    auto items = split_items("1, $02 ,\"a,b\" , label", false);
    CB_CHECK_EQ(items.size(), 4u);
    CB_CHECK_EQ(items[0], "1");
    CB_CHECK_EQ(items[1], "$02");
    CB_CHECK_EQ(items[2], "\"a,b\"");
    CB_CHECK_EQ(items[3], "label");
    CB_CHECK_EQ(split_items("1,2,", false).size(), 2u);                 // an empty last item is dropped
    CB_CHECK_EQ(split_items("", false).size(), 0u);
    CB_CHECK_EQ(split_items("(1,2),3", true).size(), 2u);              // Z80: a comma inside parentheses stays
    CB_CHECK_EQ(split_items("(1,2),3", false).size(), 3u);
    CB_CHECK_EQ(split_items("a,,b", false).size(), 3u);                 // an empty middle item is kept
});

CB_TEST("build.asm_text.parse_number", [] {
    CB_CHECK_EQ(parse_number("31").value(), 31);
    CB_CHECK_EQ(parse_number("0x1F").value(), 31);
    CB_CHECK_EQ(parse_number("0b101").value(), 5);
    CB_CHECK_EQ(parse_number("0o17").value(), 15);
    CB_CHECK_EQ(parse_number("-8").value(), -8);
    CB_CHECK_EQ(parse_number("  7 ").value(), 7);
    CB_CHECK(!parse_number("").has_value());
    CB_CHECK(!parse_number("1F").has_value());
    CB_CHECK(!parse_number("0x").has_value());
    CB_CHECK(!parse_number("--1").has_value());
    CB_CHECK(!parse_number("99999999999999999999").has_value());   // past 64 bits
    CB_CHECK(!parse_number("12abc").has_value());
});

CB_TEST("build.asm_text.errors", [] {
    const AsmError e(12, "LD A,", "missing operand");
    CB_CHECK_EQ(e.line(), 12u);
    CB_CHECK(std::string(e.what()).find("LD A, (missing operand)") != std::string::npos);
    CB_CHECK_EQ(e.kind(), "AsmError");
    const UndefinedSymbolError u("foo");
    CB_CHECK_EQ(u.name(), "foo");
    CB_CHECK(is_undefined(u));
    CB_CHECK(!is_undefined(e));
});
