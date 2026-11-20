// Tests of NASM's string quoting rules (quote.hpp), the string transforms (strfunc.hpp) and the character classes
// (nctype.hpp). The expected bytes of every unquoting come from nasm.exe assembling `db <string>`.
#include "build/x86asm/quote.hpp"

#include <string>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/nctype.hpp"
#include "build/x86asm/strfunc.hpp"
#include "core/testing.hpp"

namespace {
using namespace cb::build::x86asm;

std::string qt_hex(std::string_view s) {
    std::string out;
    for (const char c : s) {
        constexpr char d[] = "0123456789abcdef";
        out += d[static_cast<unsigned char>(c) >> 4];
        out += d[static_cast<unsigned char>(c) & 15];
    }
    return out;
}

// The bytes a quoted string stands for, as qt_hex.
std::string qt_unq(std::string_view text) {
    Assembly as;
    return qt_hex(nasm_unquote(as, text).text);
}
}  // namespace

CB_TEST("x86asm.quote.unquote_escapes", [] {
    // nasm: db `\n\t\x41\101\e\a\b\f\v\r\0`
    CB_CHECK_EQ(qt_unq("`\\n\\t\\x41\\101\\e\\a\\b\\f\\v\\r\\0`"), "0a0941411b07080c0b0d00");
    // nasm: db `\xZ\x4\x{}\x{4g}\u12\u{1F}\ud\d9\d065\o\o7\8\1\18\777\^\^@end`
    CB_CHECK_EQ(qt_unq("`\\xZ\\x4\\x{}\\x{4g}\\u12\\u{1F}\\ud\\d9\\d065\\o\\o7\\8\\1\\18\\777\\^\\^@end`"),
                "785a0400787b34677d121f0d09416f0738010138ff1c5e40656e64");
    // nasm: db `\x{41}\^A\^?\d065\q`
    CB_CHECK_EQ(qt_unq("`\\x{41}\\^A\\^?\\d065\\q`"), "41017f4171");
    // nasm: db `\\\'\"\`\?z`
    CB_CHECK_EQ(qt_unq("`\\\\\\'\\\"\\`\\?z`"), "5c2722603f7a");
    // nasm: db `\U7FFFFFFF`, `\U80000000`, `é`   (a value above 31 bits still gets a six-byte form)
    CB_CHECK_EQ(qt_unq("`\\U7FFFFFFF`"), "fdbfbfbfbfbf");
    CB_CHECK_EQ(qt_unq("`\\U80000000`"), "fe8080808080");
    CB_CHECK_EQ(qt_unq("`\\u00e9`"), "c3a9");
    CB_CHECK_EQ(qt_unq("`\\u20ac`"), "e282ac");
    CB_CHECK_EQ(qt_unq("`\\U0001F600`"), "f09f9880");
});

CB_TEST("x86asm.quote.unquote_plain_and_ends", [] {
    // '...' and "..." have no escapes at all and no doubling of the quote.
    CB_CHECK_EQ(qt_unq("'a\\n'"), "615c6e");
    CB_CHECK_EQ(qt_unq("\"b\\x41\""), "625c783431");
    Assembly as;
    Unquoted u = nasm_unquote(as, "'ab''cd'");
    CB_CHECK_EQ(u.text, "ab");
    CB_CHECK_EQ(u.end, std::size_t{3});   // at the closing quote; the rest is another string
    // Unterminated: the end is the end of the input.
    u = nasm_unquote(as, "`abc\\`");
    CB_CHECK_EQ(u.text, "abc`");
    CB_CHECK_EQ(u.end, std::size_t{6});
    u = nasm_unquote(as, "'abc");
    CB_CHECK_EQ(u.text, "abc");
    CB_CHECK_EQ(u.end, std::size_t{4});
    // Not a string at all: the input as it is.
    u = nasm_unquote(as, "plain");
    CB_CHECK_EQ(u.text, "plain");
    // A lone backslash at the end of a backquote string stays.
    CB_CHECK_EQ(qt_unq("`ab\\"), "61625c");
});

CB_TEST("x86asm.quote.control_characters", [] {
    // A C-string unquote rejects control characters other than BEL BS TAB ESC, and cuts the text before the first.
    Assembly as;
    const Unquoted ok = nasm_unquote_cstr(as, "`a\\tb\\ec`");
    CB_CHECK_EQ(qt_hex(ok.text), "6109621b63");
    CB_CHECK(as.diagnostics().empty());
    as.src.set(3, "f.asm");
    const Unquoted bad = nasm_unquote_cstr(as, "`a\\nb`");
    CB_CHECK_EQ(bad.text, "a");
    CB_CHECK_EQ(bad.end, std::size_t{5});
    CB_CHECK_EQ(as.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(as.diagnostics()[0].text, "f.asm:3: error: control character in string not allowed here");
});

CB_TEST("x86asm.quote.skip_string", [] {
    CB_CHECK_EQ(*nasm_skip_string("'ab' rest"), std::size_t{3});
    CB_CHECK_EQ(*nasm_skip_string("\"a'b\" x"), std::size_t{4});
    CB_CHECK_EQ(*nasm_skip_string("`a\\`b` x"), std::size_t{5});     // \` does not end it
    CB_CHECK_EQ(*nasm_skip_string("`a\\^`` x"), std::size_t{5});     // \^` is the control character, then the real end
    CB_CHECK_EQ(*nasm_skip_string("'open"), std::size_t{5});         // unterminated: the end of the input
    CB_CHECK(!nasm_skip_string("abc").has_value());
});

CB_TEST("x86asm.quote.nasm_quote", [] {
    CB_CHECK_EQ(nasm_quote("abc"), "'abc'");
    CB_CHECK_EQ(nasm_quote(""), "''");
    CB_CHECK_EQ(nasm_quote("it's"), "\"it's\"");
    CB_CHECK_EQ(nasm_quote("a\\b"), "'a\\b'");                          // a backslash alone does not need backquotes
    CB_CHECK_EQ(nasm_quote("a\"b'c"), "`a\"b'c`");
    CB_CHECK_EQ(nasm_quote("a`b'\""), "`a\\`b'\"`");
    CB_CHECK_EQ(nasm_quote("a\nb"), "`a\\nb`");
    CB_CHECK_EQ(nasm_quote(std::string("\a\b\t\v\f\r\x1b", 7)), "`\\a\\b\\t\\v\\f\\r\\e`");
    CB_CHECK_EQ(nasm_quote(std::string("\x01", 1)), "`\\1`");
    CB_CHECK_EQ(nasm_quote(std::string("\x01" "1", 2)), "`\\0011`");   // the full form when a digit follows
    CB_CHECK_EQ(nasm_quote(std::string("\x7f\xff", 2)), "`\\177\\377`");
    CB_CHECK_EQ(nasm_quote(std::string("a\0b", 3)), "`a\\0b`");
    // What it makes unquotes to what it was given.
    const std::string tricky("x\0\x01" "9\n\"'`\\\xe9z", 11);
    Assembly as;
    CB_CHECK_EQ(nasm_unquote(as, nasm_quote(tricky)).text, tricky);
});

CB_TEST("x86asm.strfunc.transforms", [] {
    // nasm: dw __utf16le__("a€"), __utf16be__("a€")   dd __utf32__("a€"), __utf32be__("a€")
    const std::string euro = "a\xe2\x82\xac";
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF16LE)), "6100ac20");
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF16)), "6100ac20");
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF16BE)), "006120ac");
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF32)), "61000000ac200000");
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF32LE)), "61000000ac200000");
    CB_CHECK_EQ(qt_hex(*string_transform(euro, STRFUNC_UTF32BE)), "00000061000020ac");
    // nasm: dd __utf16__(`\U0001F600`), __utf16be__(`\U0001F600`): a surrogate pair; the big-endian form writes the low
    // surrogate first (as NASM does).
    const std::string face = "\xf0\x9f\x98\x80";
    CB_CHECK_EQ(qt_hex(*string_transform(face, STRFUNC_UTF16)), "3dd800de");
    CB_CHECK_EQ(qt_hex(*string_transform(face, STRFUNC_UTF16BE)), "de00d83d");
    CB_CHECK_EQ(qt_hex(*string_transform(face, STRFUNC_UTF32)), "00f60100");
    CB_CHECK_EQ(qt_hex(*string_transform("", STRFUNC_UTF16)), "");
});

CB_TEST("x86asm.strfunc.invalid_and_readstrnum", [] {
    CB_CHECK(!string_transform("\x80", STRFUNC_UTF16).has_value());          // a continuation byte alone
    CB_CHECK(!string_transform("\xc3", STRFUNC_UTF32).has_value());          // truncated
    CB_CHECK(!string_transform("\xc0\x80", STRFUNC_UTF16).has_value());      // overlong
    CB_CHECK(!string_transform("\xed\xa0\x80", STRFUNC_UTF32).has_value());  // a surrogate
    CB_CHECK(!string_transform("\xf4\x90\x80\x80", STRFUNC_UTF16).has_value());   // above 0x10FFFF: no UTF-16 form
    CB_CHECK(string_transform("\xf4\x90\x80\x80", STRFUNC_UTF32).has_value());    // but UTF-32 takes it
    CB_CHECK(!string_transform("\xfe", STRFUNC_UTF32).has_value());
    bool warn = true;
    CB_CHECK_EQ(readstrnum("ab", warn), std::int64_t{0x6261});
    CB_CHECK(!warn);
    CB_CHECK_EQ(readstrnum("abcdefghi", warn), std::int64_t{0x6867666564636261});
    CB_CHECK(warn);
    CB_CHECK_EQ(readstrnum("", warn), std::int64_t{0});
    CB_CHECK_EQ(readstrnum("\xff\xff\xff\xff\xff\xff\xff\xff", warn), std::int64_t{-1});
});

CB_TEST("x86asm.nctype.classes", [] {
    CB_CHECK(nasm_isidstart('a') && nasm_isidstart('_') && nasm_isidstart('.') && nasm_isidstart('@') && nasm_isidstart('?'));
    CB_CHECK(!nasm_isidstart('1') && !nasm_isidstart('$') && !nasm_isidstart('#') && !nasm_isidstart('+'));
    CB_CHECK(nasm_isidchar('1') && nasm_isidchar('$') && nasm_isidchar('#') && nasm_isidchar('~') && nasm_isidchar('\xc3'));
    CB_CHECK(!nasm_isidchar('-') && !nasm_isidchar(' ') && !nasm_isidchar('\x01'));
    CB_CHECK(nasm_isbrcchar('-') && nasm_isbrcchar('a'));
    CB_CHECK(nasm_isnumstart('7') && nasm_isnumstart('$') && !nasm_isnumstart('a'));
    CB_CHECK(nasm_isnumchar('a') && nasm_isnumchar('_') && !nasm_isnumchar('$') && !nasm_isnumchar('.'));
    CB_CHECK(nasm_isspace(' ') && nasm_isspace('\t') && !nasm_isspace('\n') && !nasm_isspace('a'));
    CB_CHECK(nasm_isquote('\'') && nasm_isquote('"') && nasm_isquote('`') && !nasm_isquote('a'));
    CB_CHECK_EQ(nasm_hexval('7'), 7u);
    CB_CHECK_EQ(nasm_hexval('F'), 15u);
    CB_CHECK(nasm_hexval('g') >= 16u && nasm_hexval('_') >= 16u);
    CB_CHECK_EQ(nasm_radix_letter('H'), 16u);
    CB_CHECK_EQ(nasm_radix_letter('y'), 2u);
    CB_CHECK_EQ(nasm_radix_letter('q'), 8u);
    CB_CHECK_EQ(nasm_radix_letter('t'), 10u);
    CB_CHECK_EQ(nasm_radix_letter('z'), 0u);
});
