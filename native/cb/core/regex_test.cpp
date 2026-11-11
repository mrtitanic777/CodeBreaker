#include "cb/core/regex.hpp"

#include "cb/core/testing.hpp"

using namespace cb;

namespace {
// The text of the first match, or "<none>".
std::string found(const Regex& re, std::string_view s) {
    auto m = re.search(s);
    return m ? std::string(m->whole()) : "<none>";
}
}  // namespace

CB_TEST("core.regex.literals_and_dot", [] {
    Regex re("abc");
    CB_CHECK_EQ(found(re, "xxabcxx"), "abc");
    CB_CHECK_EQ(found(re, "ab"), "<none>");
    CB_CHECK_EQ(found(Regex("a.c"), "a\nc abc"), "abc");           // . stops at a newline
    CB_CHECK_EQ(found(Regex("a.c", RegexFlags::DotAll), "a\nc"), "a\nc");
    CB_CHECK_EQ(found(Regex(""), "abc"), "");                      // the empty pattern matches empty
    CB_CHECK_EQ(re.search("xxabcxx")->start(), 2u);
    CB_CHECK_EQ(re.search("xxabcxx")->end(), 5u);
});

CB_TEST("core.regex.classes_are_ascii", [] {
    CB_CHECK_EQ(found(Regex(R"(\d+)"), "ab 1234 cd"), "1234");
    CB_CHECK_EQ(found(Regex(R"(\w+)"), "  foo_Bar9 "), "foo_Bar9");
    CB_CHECK_EQ(found(Regex(R"(\s+)"), "a \t\n b"), " \t\n ");
    CB_CHECK_EQ(found(Regex(R"(\W)"), "ab-c"), "-");
    CB_CHECK_EQ(found(Regex(R"(\D+)"), "12ab34"), "ab");
    CB_CHECK_EQ(found(Regex(R"(\S+)"), "  xy  "), "xy");
    // \w does not match the bytes of a non-ASCII character: "caf" ends before the 0xC3 0xA9 of e-acute
    CB_CHECK_EQ(found(Regex(R"(\w+)"), "caf\xC3\xA9"), "caf");
    CB_CHECK_EQ(found(Regex("[a-c]+"), "xxabcabcxx"), "abcabc");
    CB_CHECK_EQ(found(Regex("[^a-c]+"), "abxyzc"), "xyz");
    CB_CHECK_EQ(found(Regex(R"([\d.]+)"), "v1.25x"), "1.25");
    CB_CHECK_EQ(found(Regex("[]a]+"), "x]a]"), "]a]");             // a leading ] is a literal
    CB_CHECK_EQ(found(Regex("[a-]+"), "x-a-"), "-a-");             // a trailing - is a literal
    CB_CHECK_EQ(found(Regex(R"([\]\\])"), "a\\b"), "\\");
    CB_CHECK_EQ(found(Regex(R"([\x41-\x43]+)"), "zABCz"), "ABC");
    CB_CHECK_EQ(found(Regex("[^a]"), "a\nb"), "\n");               // a negated class takes newlines
});

CB_TEST("core.regex.quantifiers_greedy_and_lazy", [] {
    CB_CHECK_EQ(found(Regex("a*"), "aaab"), "aaa");
    CB_CHECK_EQ(found(Regex("a+?"), "aaab"), "a");
    CB_CHECK_EQ(found(Regex("a??b"), "ab"), "ab");
    CB_CHECK_EQ(found(Regex("<.*>"), "<a><b>"), "<a><b>");
    CB_CHECK_EQ(found(Regex("<.*?>"), "<a><b>"), "<a>");
    CB_CHECK_EQ(found(Regex("a{3}"), "aaaaa"), "aaa");
    CB_CHECK_EQ(found(Regex("a{2,}"), "aaaaa"), "aaaaa");
    CB_CHECK_EQ(found(Regex("a{2,3}"), "aaaaa"), "aaa");
    CB_CHECK_EQ(found(Regex("a{2,3}?"), "aaaaa"), "aa");
    CB_CHECK_EQ(found(Regex("(ab){2}"), "ababab"), "abab");
    CB_CHECK_EQ(found(Regex("(?:ab)+"), "xababab"), "ababab");
    CB_CHECK_EQ(found(Regex("(?:ab)+?"), "xababab"), "ab");
    CB_CHECK_EQ(found(Regex("x{,3}"), "x{,3}"), "x{,3}");        // not a quantifier: a literal brace
    CB_CHECK_EQ(found(Regex("a{"), "a{"), "a{");
    CB_CHECK_EQ(found(Regex(R"(\d{1,2}:\d{2})"), "at 9:05pm"), "9:05");
    CB_CHECK_EQ(found(Regex("a*?b"), "aaab"), "aaab");              // lazy still extends to find the match
    CB_CHECK_EQ(found(Regex("(a|b)*c"), "ababc"), "ababc");
});

CB_TEST("core.regex.groups_and_alternation", [] {
    Regex re(R"((\w+)@(\w+)\.com)");
    auto m = re.search("mail bob@example.com now");
    CB_CHECK(m.has_value());
    CB_CHECK_EQ(m->group_count(), 3u);
    CB_CHECK_EQ(m->group(1), "bob");
    CB_CHECK_EQ(m->group(2), "example");
    CB_CHECK_EQ(m->start(1), 5u);
    CB_CHECK_EQ(m->end(2), 16u);
    CB_CHECK_EQ(re.group_count(), 2u);
    Regex alt("cat|dog|bird");
    CB_CHECK_EQ(found(alt, "a hotdog"), "dog");
    CB_CHECK_EQ(found(Regex("(a|ab)(c|bcd)"), "abcd"), "abcd");
    // a group that did not take part
    auto opt = Regex("(a)|(b)").search("b");
    CB_CHECK(opt.has_value());
    CB_CHECK(!opt->has(1));
    CB_CHECK(opt->has(2));
    CB_CHECK_EQ(opt->group(1), "");
    CB_CHECK_EQ(opt->start(1), RegexMatch::npos);
    // a group repeated keeps its last iteration
    auto last = Regex("(?:(\\d),)+").search("1,2,3,");
    CB_CHECK_EQ(last->group(1), "3");
});

CB_TEST("core.regex.named_groups", [] {
    Regex re(R"((?<name>\w+)=(?P<value>\d+))");
    auto m = re.search("size=42");
    CB_CHECK_EQ(*m->named("name"), "size");
    CB_CHECK_EQ(*m->named("value"), "42");
    CB_CHECK(!m->named("nope").has_value());
    CB_CHECK_EQ(*re.group_index("value"), 2u);
    CB_CHECK(!re.group_index("zzz").has_value());
    CB_CHECK_EQ(m->group(1), "size");   // named groups are numbered too
});

CB_TEST("core.regex.anchors_and_boundaries", [] {
    CB_CHECK_EQ(found(Regex("^abc"), "abcabc"), "abc");
    CB_CHECK_EQ(found(Regex("^abc"), "xabc"), "<none>");
    CB_CHECK_EQ(found(Regex("abc$"), "abcabc"), "abc");
    CB_CHECK_EQ(found(Regex("^b"), "a\nb"), "<none>");                       // text anchors without Multiline
    CB_CHECK_EQ(found(Regex("^b", RegexFlags::Multiline), "a\nb"), "b");
    CB_CHECK_EQ(found(Regex("a$", RegexFlags::Multiline), "a\nb"), "a");
    CB_CHECK_EQ(found(Regex(R"(\Ab)", RegexFlags::Multiline), "a\nb"), "<none>");   // \A ignores Multiline
    CB_CHECK_EQ(found(Regex(R"(a\z)", RegexFlags::Multiline), "a\n"), "<none>");
    CB_CHECK_EQ(found(Regex(R"(\bcat\b)"), "concat cat."), "cat");
    CB_CHECK_EQ(found(Regex(R"(\Bcat)"), "cat concat"), "cat");
    CB_CHECK_EQ(Regex(R"(\Bcat)").search("cat concat")->start(), 7u);
    CB_CHECK_EQ(Regex("^").search("abc")->end(), 0u);
    CB_CHECK_EQ(found(Regex("$"), "abc"), "");
    CB_CHECK_EQ(Regex("$").search("abc")->start(), 3u);
});

CB_TEST("core.regex.lookahead", [] {
    CB_CHECK_EQ(found(Regex("foo(?=bar)"), "foobaz foobar"), "foo");
    CB_CHECK_EQ(Regex("foo(?=bar)").search("foobaz foobar")->start(), 7u);
    CB_CHECK_EQ(Regex("foo(?!bar)").search("foobar foobaz")->start(), 7u);
    CB_CHECK_EQ(found(Regex(R"(\w+(?=,))"), "a b, c"), "b");
    CB_CHECK_EQ(found(Regex("(?=(\\d+))\\d"), "x123"), "1");
    auto m = Regex("(?=(\\d+))\\d").search("x123");
    CB_CHECK_EQ(m->group(1), "123");                                // a positive lookahead keeps its captures
    CB_CHECK_EQ(found(Regex("^(?!.*bad).*$"), "all good"), "all good");
    CB_CHECK_EQ(found(Regex("^(?!.*bad).*$"), "so bad"), "<none>");
});

CB_TEST("core.regex.lookbehind", [] {
    CB_CHECK_EQ(found(Regex("(?<=\\$)\\d+"), "cost $42"), "42");
    CB_CHECK_EQ(found(Regex("(?<!\\$)\\b\\d+"), "$42 17"), "17");
    CB_CHECK_EQ(found(Regex("(?<![\\w.])0x[0-9a-f]+"), "a.0x10 0x20"), "0x20");   // the shape the ports used
    CB_CHECK_EQ(found(Regex("(?<=ab|cd)x"), "cdx"), "x");                         // alternatives of equal width
    CB_CHECK_EQ(found(Regex("(?<=a)b"), "b"), "<none>");                          // not enough text before
    CB_CHECK_THROWS_AS(ParseError, (void)Regex("(?<=a+)b"));                            // variable width is refused
    CB_CHECK_THROWS_AS(ParseError, (void)Regex("(?<=a|bc)b"));
});

CB_TEST("core.regex.backreference", [] {
    CB_CHECK_EQ(found(Regex(R"((\w)\1)"), "hello"), "ll");
    CB_CHECK_EQ(found(Regex(R"((['"]).*?\1)"), "say 'hi' \"x\""), "'hi'");
    CB_CHECK_EQ(found(Regex(R"((a)\1)", RegexFlags::IgnoreCase), "aA"), "aA");
    CB_CHECK_EQ(found(Regex(R"((a)?b\1)"), "b"), "<none>");          // an unset group matches nothing
    CB_CHECK_THROWS_AS(ParseError, (void)Regex(R"(\1)"));                  // no such group
    CB_CHECK_THROWS_AS(ParseError, (void)Regex(R"((a)\2)"));
});

CB_TEST("core.regex.flags", [] {
    CB_CHECK_EQ(found(Regex("hello", RegexFlags::IgnoreCase), "Say HeLLo"), "HeLLo");
    CB_CHECK_EQ(found(Regex("[a-c]+", RegexFlags::IgnoreCase), "xABCabc"), "ABCabc");
    CB_CHECK_EQ(found(Regex("(?i)hello"), "HELLO"), "HELLO");                     // inline flags at the start
    CB_CHECK_EQ(found(Regex("(?ims)^a.b$"), "x\nA\nB"), "A\nB");
    CB_CHECK_EQ(found(Regex("(?s)a.b"), "a\nb"), "a\nb");
    CB_CHECK_EQ(found(Regex("(?m)^b"), "a\nb"), "b");
    CB_CHECK(Regex("A", RegexFlags::IgnoreCase | RegexFlags::Multiline).test("a"));
});

CB_TEST("core.regex.match_and_full_match", [] {
    Regex re(R"(\d+)");
    CB_CHECK(re.match("123abc").has_value());
    CB_CHECK(!re.match("abc123").has_value());
    CB_CHECK_EQ(re.match("ab123", 2)->whole(), "123");                // anchored at `from`
    CB_CHECK(!re.full_match("123abc").has_value());
    CB_CHECK(re.full_match("123").has_value());
    CB_CHECK(!re.full_match("").has_value());
    // full_match backtracks to make the match reach the end
    CB_CHECK(Regex("a*ab").full_match("aaab").has_value());
    CB_CHECK(Regex("a|ab").full_match("ab").has_value());
    CB_CHECK(Regex("[A-Za-z_][A-Za-z0-9_]*").full_match("_name9").has_value());
    CB_CHECK(!Regex("[A-Za-z_][A-Za-z0-9_]*").full_match("9name").has_value());
    CB_CHECK(!re.search("abc", 4).has_value());                       // `from` past the end is just no match
    CB_CHECK(re.search("abc", 3).has_value() == false);
});

CB_TEST("core.regex.for_each_match_and_find_all", [] {
    Regex re(R"(\d+)");
    CB_CHECK(re.find_all("a1b22c333") == (std::vector<std::string>{"1", "22", "333"}));
    CB_CHECK_EQ(re.find_all("none").size(), 0u);
    std::size_t seen = 0;
    CB_CHECK_EQ(re.for_each_match("1 2 3 4", [&](const RegexMatch&) { return ++seen < 2; }), 2u);   // stops when told to
    // empty matches: reported once per position
    CB_CHECK_EQ(Regex("x*").find_all("axb").size(), 4u);              // "", "x", "" (before b's end), ""
    CB_CHECK_EQ(Regex("").find_all("ab").size(), 3u);
});

CB_TEST("core.regex.replace", [] {
    CB_CHECK_EQ(Regex(R"(\W)").replace_all("a-b c.d", "_"), "a_b_c_d");               // the old re.sub(r"\W", "_", s)
    CB_CHECK_EQ(Regex(R"(\s+)").replace_all("a  b\t\tc", " "), "a b c");
    CB_CHECK_EQ(Regex(R"((\w+)=(\w+))").replace_all("a=1, b=2", "$2=$1"), "1=a, 2=b");
    CB_CHECK_EQ(Regex(R"((?<k>\w+)=)").replace_all("a=1", "${k}:"), "a:1");
    CB_CHECK_EQ(Regex(R"(\d+)").replace_all("a1b2", "<$&>"), "a<1>b<2>");
    CB_CHECK_EQ(Regex(R"(\d)").replace_all("a1", "$$"), "a$");
    CB_CHECK_EQ(Regex(R"(\d)").replace_all("a1", "$x"), "a$x");                       // an unknown $x stays literal
    CB_CHECK_EQ(Regex(R"(b)").replace_first("abab", "X"), "aXab");
    CB_CHECK_EQ(Regex(R"(z)").replace_first("abab", "X"), "abab");
    CB_CHECK_EQ(Regex(R"(\d+)").replace_all("a12b3", [](const RegexMatch& m) { return std::to_string(m.whole().size()); }), "a2b1");
    CB_CHECK_EQ(Regex("^").replace_all("a", ">"), ">a");
    CB_CHECK_EQ(Regex("x").replace_all("", "y"), "");
    CB_CHECK_THROWS_AS(UsageError, Regex("(a)").replace_all("a", "$2"));
    CB_CHECK_THROWS_AS(UsageError, Regex("(a)").replace_all("a", "${zzz}"));
    CB_CHECK_THROWS_AS(UsageError, Regex("(a)").replace_all("a", "${open"));
});

CB_TEST("core.regex.split", [] {
    CB_CHECK(Regex(",\\s*").split("a, b,c") == (std::vector<std::string>{"a", "b", "c"}));
    CB_CHECK(Regex(",").split("a,,b") == (std::vector<std::string>{"a", "", "b"}));
    CB_CHECK(Regex(",").split("") == (std::vector<std::string>{""}));
    CB_CHECK(Regex("x*").split("ab") == (std::vector<std::string>{"ab"}));        // empty matches do not split
});

CB_TEST("core.regex.the_shapes_the_ports_use", [] {
    // the NASM-listing / Watcom-map / MASM DESCRIPTION forms and the demangler's lazy match
    Regex listing(R"(^\s*(\d+)\s+([0-9A-F]{8})\s+((?:[0-9A-F]{2})+)\s+(.*)$)", RegexFlags::Multiline);
    auto m = listing.search("   12 00000010 B800004C   mov eax, 0x4c000000");
    CB_CHECK(m.has_value());
    CB_CHECK_EQ(m->group(1), "12");
    CB_CHECK_EQ(m->group(2), "00000010");
    CB_CHECK_EQ(m->group(3), "B800004C");
    Regex desc(R"(DESCRIPTION\s+'(.*?)')");
    CB_CHECK_EQ(desc.search("DESCRIPTION 'A ''quoted'' text' more 'x'")->group(1), "A ");
    Regex hexnum(R"(-?(0x[0-9a-f]+|\d+))", RegexFlags::IgnoreCase);
    CB_CHECK_EQ(found(hexnum, "mov ax, -0X1F"), "-0X1F");
    Regex hex_not_after_word(R"((?<![\w.])0x[0-9a-f]+)");
    CB_CHECK_EQ(hex_not_after_word.replace_all("a0x10 0x20", "N"), "a0x10 N");
    Regex comment(R"(/\*.*?\*/)", RegexFlags::DotAll);
    CB_CHECK_EQ(comment.replace_all("a /* x\n y */ b /* z */ c", ""), "a  b  c");
});

CB_TEST("core.regex.utf8_passes_through_as_bytes", [] {
    // "é" is two bytes; . matches one byte at a time and the text comes back intact
    CB_CHECK_EQ(found(Regex("caf.."), "caf\xC3\xA9!"), "caf\xC3\xA9");
    CB_CHECK_EQ(found(Regex("caf\xC3\xA9"), "x caf\xC3\xA9"), "caf\xC3\xA9");            // non-ASCII literals are fine outside classes
    CB_CHECK_THROWS_AS(ParseError, (void)Regex("[\xC3\xA9]"));                                  // but not inside a class
    CB_CHECK_EQ(Regex(R"(\xC3\xA9)").search("caf\xC3\xA9")->start(), 3u);                 // \xHH reaches any byte
});

CB_TEST("core.regex.bad_patterns_are_parse_errors", [] {
    for (const char* bad : {"(", "(a", "a)", "[a", "[z-a]", "*a", "+", "?", "a**", "a{2,1}", "\\", "(?<n>a)(?<n>b)",
                            "(?P<1>a)", "(?>a)", "(?#c)", "\\Z", "\\p{L}", "a{99999}", "(?<name", "[\\d-z]", "(?=a)*", "^*", "\\q"}) {
        CB_CHECK_THROWS_AS(ParseError, (void)Regex(bad));
        CB_CHECK(!Regex::compile(bad).ok());
    }
    try {
        (void)Regex("ab(");
    } catch (const ParseError& e) {
        CB_CHECK_EQ(e.file(), "<regex>");
        CB_CHECK(std::string(e.what()).find("ab(") != std::string::npos);
    }
    CB_CHECK(Regex::compile("a(b)c").ok());
    CB_CHECK_EQ(Regex::compile("a(b)c").value().group_count(), 1u);
});

CB_TEST("core.regex.deep_nesting_is_refused_not_crashed", [] {
    std::string nested(500, '(');
    nested += "a";
    nested += std::string(500, ')');
    CB_CHECK_THROWS_AS(ParseError, (void)Regex(nested));
    CB_CHECK_NOTHROW((void)Regex(std::string(50, '(') + "a" + std::string(50, ')')));
});

CB_TEST("core.regex.step_budget_stops_pathological_patterns", [] {
    // classic catastrophic backtracking: (a+)+b over a run of a's with no b
    Regex evil("(a+)+b", RegexFlags::None, 200000);
    const std::string subject(40, 'a');
    auto r = evil.try_search(subject);
    CB_CHECK(!r.ok());
    CB_CHECK_EQ(r.error().kind(), "UnsupportedError");
    CB_CHECK_THROWS_AS(UnsupportedError, evil.search(subject));
    CB_CHECK_THROWS_AS(UnsupportedError, evil.find_all(subject));
    // the same engine answers normal inputs under the same budget
    auto fine = evil.try_search("aaab");
    CB_CHECK(fine.ok());
    CB_CHECK(fine.value().has_value());
    // empty-loop bodies do not spin forever
    CB_CHECK_EQ(found(Regex("(a*)*b"), "aab"), "aab");
    CB_CHECK_EQ(found(Regex("(a*)+$"), "aaa"), "aaa");
    CB_CHECK_EQ(found(Regex("(?:a|)*b"), "aab"), "aab");
});

CB_TEST("core.regex.long_subjects_do_not_overflow_the_stack", [] {
    const std::string big(2'000'000, 'x');
    CB_CHECK_EQ(Regex("x*").search(big)->whole().size(), big.size());
    CB_CHECK_EQ(Regex("^.*y").search(big).has_value(), false);     // fails after backtracking the whole run
    CB_CHECK_EQ(Regex("(?:x)*").search(big.substr(0, 400000))->whole().size(), 400000u);
    CB_CHECK_EQ(Regex("[x]+?$").search(big)->whole().size(), big.size());
});

CB_TEST("core.regex.copies_share_one_program_and_threads_may_use_it", [] {
    static const Regex re(R"((\d+)-(\d+))");
    Regex copy = re;
    CB_CHECK_EQ(copy.search("10-20")->group(2), "20");
    auto m = Regex(R"((?<x>a))").search("a");
    CB_CHECK_EQ(*m->named("x"), "a");    // the match outlives the temporary Regex
});
