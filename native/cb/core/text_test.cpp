#include "cb/core/text.hpp"

#include "cb/core/testing.hpp"
#include "cb/core/win.hpp"

using namespace cb;
namespace t = cb::text;

CB_TEST("core.text.case_is_ascii_only", [] {
    CB_CHECK_EQ(t::lower("Hello WORLD_9"), "hello world_9");
    CB_CHECK_EQ(t::upper("Hello world"), "HELLO WORLD");
    CB_CHECK_EQ(t::lower("\xC3\x89"), "\xC3\x89");   // non-ASCII bytes are left alone
    CB_CHECK(t::iequals("KERNEL32.DLL", "kernel32.dll"));
    CB_CHECK(!t::iequals("a", "ab"));
    CB_CHECK(t::istarts_with("Windows\\System", "WINDOWS"));
    CB_CHECK(t::iends_with("setup.EXE", ".exe"));
    CB_CHECK(!t::iends_with("a", ".exe"));
});

CB_TEST("core.text.imap_is_case_insensitive", [] {
    t::IMap<int> m;
    m["Kernel32.dll"] = 1;
    m["USER32.DLL"] = 2;
    CB_CHECK_EQ(m.count("kernel32.DLL"), 1u);
    CB_CHECK_EQ(m.at("user32.dll"), 2);
    CB_CHECK_EQ(m.size(), 2u);
    m["KERNEL32.DLL"] = 5;
    CB_CHECK_EQ(m.size(), 2u);
    CB_CHECK_EQ(t::IHash{}("AbC"), t::IHash{}("aBc"));
});

CB_TEST("core.text.trim", [] {
    CB_CHECK_EQ(t::trim("  \t x y \r\n"), "x y");
    CB_CHECK_EQ(t::ltrim("  x "), "x ");
    CB_CHECK_EQ(t::rtrim(" x  "), " x");
    CB_CHECK_EQ(t::trim(""), "");
    CB_CHECK_EQ(t::trim("   "), "");
    CB_CHECK_EQ(t::trim("__ab__", "_"), "ab");
    CB_CHECK_EQ(t::trim("____", "_"), "");
});

CB_TEST("core.text.split", [] {
    auto parts = t::split("a,b,,c", ',');
    CB_CHECK_EQ(parts.size(), 4u);
    CB_CHECK_EQ(parts[2], "");
    CB_CHECK_EQ(t::split("", ',').size(), 1u);          // "" has one empty piece, as Python's split(",")
    CB_CHECK_EQ(t::split("abc", ',').size(), 1u);
    CB_CHECK_EQ(t::split("a::b::c", "::").size(), 3u);
    CB_CHECK_EQ(t::split("a,b", "").size(), 1u);          // an empty separator splits nothing
    auto limited = t::split_n("a b c d", ' ', 2);
    CB_CHECK_EQ(limited.size(), 3u);
    CB_CHECK_EQ(limited[2], "c d");
    auto ws = t::split_ws("  mov   ax,\tbx \n");
    CB_CHECK_EQ(ws.size(), 3u);
    CB_CHECK_EQ(ws[0], "mov");
    CB_CHECK_EQ(ws[1], "ax,");
    CB_CHECK_EQ(ws[2], "bx");
    CB_CHECK_EQ(t::split_ws("   ").size(), 0u);
    auto st = t::split_trim(" a , b ,c", ',');
    CB_CHECK_EQ(st.size(), 3u);
    CB_CHECK_EQ(st[1], "b");
});

CB_TEST("core.text.lines", [] {
    auto l = t::lines("one\ntwo\r\nthree");
    CB_CHECK_EQ(l.size(), 3u);
    CB_CHECK_EQ(l[1], "two");
    CB_CHECK_EQ(t::lines("a\n").size(), 1u);              // a trailing terminator starts no extra line
    CB_CHECK_EQ(t::lines("a\n\nb").size(), 3u);
    CB_CHECK_EQ(t::lines("").size(), 0u);
    CB_CHECK_EQ(t::lines("\r\n")[0], "");
});

CB_TEST("core.text.join_replace", [] {
    std::vector<std::string> v = {"a", "b", "c"};
    CB_CHECK_EQ(t::join(v, ", "), "a, b, c");
    CB_CHECK_EQ(t::join(std::vector<std::string>{}, ","), "");
    CB_CHECK_EQ(t::join(t::split("x y z", ' '), "-"), "x-y-z");
    CB_CHECK_EQ(t::replace_all("aaa", "a", "bb"), "bbbbbb");
    CB_CHECK_EQ(t::replace_all("abab", "ab", ""), "");
    CB_CHECK_EQ(t::replace_all("abc", "", "x"), "abc");
    CB_CHECK_EQ(t::replace_all("a.b.c", ".", "::"), "a::b::c");
    CB_CHECK_EQ(t::replace_first("a.b.c", ".", "-"), "a-b.c");
    CB_CHECK_EQ(t::replace_first("abc", "z", "-"), "abc");
});

CB_TEST("core.text.identifiers", [] {
    CB_CHECK_EQ(t::sanitize_identifier("my-func.v2 (x)"), "my_func_v2__x_");
    CB_CHECK_EQ(t::sanitize_identifier("ok_name9"), "ok_name9");
    CB_CHECK(t::is_identifier("_start"));
    CB_CHECK(t::is_identifier("a1_B2"));
    CB_CHECK(!t::is_identifier("9lives"));
    CB_CHECK(!t::is_identifier(""));
    CB_CHECK(!t::is_identifier("a-b"));
    CB_CHECK(!t::is_identifier("a b"));
});

CB_TEST("core.text.parse_int", [] {
    CB_CHECK_EQ(*t::parse_int<int>("42"), 42);
    CB_CHECK_EQ(*t::parse_int<int>("-42"), -42);
    CB_CHECK_EQ(*t::parse_int<int>("+7"), 7);
    CB_CHECK_EQ(*t::parse_int<unsigned>("ff", 16), 255u);
    CB_CHECK_EQ(*t::parse_int<unsigned>("0xFF", 16), 255u);
    CB_CHECK_EQ(*t::parse_int<unsigned>("0x10", 0), 16u);
    CB_CHECK_EQ(*t::parse_int<unsigned>("10", 0), 10u);
    CB_CHECK_EQ(*t::parse_int<std::uint64_t>("18446744073709551615"), 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK_EQ(*t::parse_int<std::int64_t>("-9223372036854775808"), INT64_MIN);
    CB_CHECK_EQ(*t::parse_int<std::int8_t>("-128"), -128);
    // failures
    CB_CHECK(!t::parse_int<int>("").has_value());
    CB_CHECK(!t::parse_int<int>("-").has_value());
    CB_CHECK(!t::parse_int<int>("12abc").has_value());
    CB_CHECK(!t::parse_int<int>(" 12").has_value());
    CB_CHECK(!t::parse_int<int>("12 ").has_value());
    CB_CHECK(!t::parse_int<int>("--1").has_value());
    CB_CHECK(!t::parse_int<int>("0x").has_value());
    CB_CHECK(!t::parse_int<unsigned>("-1").has_value());
    CB_CHECK(!t::parse_int<std::uint8_t>("256").has_value());
    CB_CHECK(!t::parse_int<std::int8_t>("128").has_value());
    CB_CHECK(!t::parse_int<std::int8_t>("-129").has_value());
    CB_CHECK(!t::parse_int<std::int64_t>("9223372036854775808").has_value());
    CB_CHECK(!t::parse_int<std::uint64_t>("18446744073709551616").has_value());
    CB_CHECK(!t::parse_int<int>("0x10").has_value());   // base 10 does not take a 0x prefix
});

CB_TEST("core.text.format_double", [] {
    CB_CHECK_EQ(t::format_double(0.5), "0.5");
    CB_CHECK_EQ(t::format_double(0.1), "0.1");
    CB_CHECK_EQ(t::format_double(1.0), "1");
    CB_CHECK_EQ(t::format_double(-2.25), "-2.25");
    CB_CHECK_EQ(std::stod(t::format_double(3.141592653589793)), 3.141592653589793);   // round-trips
});

CB_TEST("core.text.latin1", [] {
    const Bytes b = {'A', 0xE9, 0xFF};
    std::string u = t::from_latin1(b);
    CB_CHECK_EQ(u, "A\xC3\xA9\xC3\xBF");
    CB_CHECK(t::to_latin1(u) == b);
    CB_CHECK(t::to_latin1("\xE2\x82\xAC") == Bytes{'?'});   // the euro sign is not in Latin-1
});

CB_TEST("core.text.cp437", [] {
    const Bytes b = {0xC9, 0xCD, 0xBB, 0x80, 'x', 0xFB};
    std::string u = t::from_cp437(b);
    CB_CHECK_EQ(u, "\xE2\x95\x94\xE2\x95\x90\xE2\x95\x97\xC3\x87x\xE2\x88\x9A");
    CB_CHECK(t::to_cp437(u) == b);
    CB_CHECK_EQ(t::from_cp437(Bytes{'h', 'i'}), "hi");
    // all 256 bytes survive a round trip
    Bytes all;
    for (int i = 0; i < 256; ++i) all.push_back(static_cast<std::uint8_t>(i));
    CB_CHECK(t::to_cp437(t::from_cp437(all)) == all);
});

CB_TEST("core.text.cp1252", [] {
    const Bytes b = {0x80, 0x93, 0x94, 0xE9, 'a'};
    std::string u = t::from_cp1252(b);
    CB_CHECK_EQ(u, "\xE2\x82\xAC\xE2\x80\x9C\xE2\x80\x9D\xC3\xA9" "a");
    CB_CHECK(t::to_cp1252(u) == b);
    Bytes all;
    for (int i = 0; i < 256; ++i) all.push_back(static_cast<std::uint8_t>(i));
    CB_CHECK(t::to_cp1252(t::from_cp1252(all)) == all);
});

CB_TEST("core.text.utf16le", [] {
    const Bytes b = {'h', 0, 0xE9, 0, 0x3D, 0xD8, 0x00, 0xDE};   // h, e-acute, U+1F600 as a surrogate pair
    std::string u = t::from_utf16le(b);
    CB_CHECK_EQ(u, "h\xC3\xA9\xF0\x9F\x98\x80");
    CB_CHECK(t::to_utf16le(u) == b);
    CB_CHECK_EQ(t::from_utf16le(Bytes{0x00, 0xD8}), "\xEF\xBF\xBD");        // lone high surrogate
    CB_CHECK_EQ(t::from_utf16le(Bytes{'a', 0, 'b'}), "a");                  // trailing odd byte ignored
    CB_CHECK_EQ(t::from_utf16le(Bytes{}), "");
});

CB_TEST("core.text.utf8_validity", [] {
    CB_CHECK(t::is_valid_utf8("plain ascii"));
    CB_CHECK(t::is_valid_utf8("caf\xC3\xA9 \xF0\x9F\x98\x80"));
    CB_CHECK(!t::is_valid_utf8("\xC3"));                // truncated sequence
    CB_CHECK(!t::is_valid_utf8("\xC0\x80"));            // overlong NUL
    CB_CHECK(!t::is_valid_utf8("\xED\xA0\x80"));        // surrogate
    CB_CHECK(!t::is_valid_utf8("\xF5\x80\x80\x80"));    // past U+10FFFF
    CB_CHECK(!t::is_valid_utf8("a\xFFz"));
    CB_CHECK_EQ(t::sanitize_utf8("a\xFFz"), "a\xEF\xBF\xBDz");
    CB_CHECK_EQ(t::sanitize_utf8("ok"), "ok");
});

CB_TEST("core.text.display_width", [] {
    CB_CHECK_EQ(t::display_width("abc"), 3u);
    CB_CHECK_EQ(t::display_width("caf\xC3\xA9"), 4u);
    CB_CHECK_EQ(t::display_width("e\xCC\x81"), 1u);                  // e + combining acute
    CB_CHECK_EQ(t::display_width("\xE4\xBD\xA0\xE5\xA5\xBD"), 4u);   // two CJK characters
    CB_CHECK_EQ(t::display_width(""), 0u);
    CB_CHECK_EQ(t::utf8_prefix("abcdef", 3), "abc");
    CB_CHECK_EQ(t::utf8_prefix("ab", 10), "ab");
    CB_CHECK_EQ(t::utf8_prefix("\xE4\xBD\xA0\xE5\xA5\xBD", 3), "\xE4\xBD\xA0");   // never cuts a character
    CB_CHECK_EQ(t::utf8_prefix("abc", 0), "");
});

CB_TEST("core.win.wide_boundary", [] {
    const std::string u = "C:/caf\xC3\xA9/\xF0\x9F\x98\x80.txt";
    std::wstring w = win::to_wide(u);
    CB_CHECK_EQ(win::from_wide(w), u);
    CB_CHECK_EQ(win::to_wide("").size(), 0u);
    CB_CHECK_EQ(win::from_wide(L"").size(), 0u);
    CB_CHECK_EQ(win::to_wide("a\xFF").size(), 2u);   // an invalid byte becomes U+FFFD, not a failure
    CB_CHECK(!win::error_message(2).empty());
    CB_CHECK(win::error_message(2).find('\n') == std::string::npos);
});
