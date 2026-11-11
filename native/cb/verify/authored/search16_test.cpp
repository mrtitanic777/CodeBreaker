#include "cb/verify/authored/search16.hpp"

#include <set>

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
namespace s16 = cb::verify::authored::search16;
using cb::verify::ByteMatch;

std::vector<std::string> lines_of(std::string_view text) {
    std::vector<std::string> out;
    for (const std::string_view l : cb::text::split(text, '\n')) out.emplace_back(l);
    return out;
}

// a function as the decompiler writes it, K&R parameters and all
const char* const kFunction =
    "int2 __cdecl f(a, b)\n"
    "int2 a;\n"
    "int2 b;\n"
    "{\n"
    "    int2 x;\n"
    "    uint4 y;\n"
    "\n"
    "    x = a;\n"
    "    y = b;\n"
    "    if (x <= 0x19) {\n"
    "        y = 1;\n"
    "    }\n"
    "    else {\n"
    "        y = 2;\n"
    "    }\n"
    "    return x;\n"
    "}";

std::optional<std::string> rewritten(std::string_view text, std::uint64_t seed = 1) {
    s16::Rng rng(seed);
    return s16::rewrite(text, rng);
}
}  // namespace

CB_TEST("verify.search16.leading_declarations", [] {
    const auto lines = lines_of(kFunction);
    // the K&R parameter declarations before the `{` are not locals; a blank line does not end the run
    CB_CHECK((s16::leading_declarations(lines) == std::vector<std::size_t>{4, 5}));
    CB_CHECK(s16::leading_declarations(lines_of("f()\nint x;")).empty());   // no body
    CB_CHECK((s16::leading_declarations(lines_of("f()\n{\n    char buf[16];\n    int4 **p;\n    x = 1;\n    int4 z;\n}")) ==
              std::vector<std::size_t>{2, 3}));   // arrays and pointers are declarations; the run ends at the first statement
});

CB_TEST("verify.search16.negate_condition", [] {
    CB_CHECK_EQ(s16::negate_condition("x == 3"), std::string("x != 3"));
    CB_CHECK_EQ(s16::negate_condition("x != 3"), std::string("x == 3"));
    CB_CHECK_EQ(s16::negate_condition("x < 3"), std::string("x >= 3"));
    CB_CHECK_EQ(s16::negate_condition("x <= 3"), std::string("x > 3"));
    CB_CHECK_EQ(s16::negate_condition("x > 3"), std::string("x <= 3"));
    CB_CHECK_EQ(s16::negate_condition("x >= 3"), std::string("x < 3"));
    CB_CHECK_EQ(s16::negate_condition("  x.y   ==   a  "), std::string("x.y != a"));
    // anything with parentheses or logic goes under a !
    CB_CHECK_EQ(s16::negate_condition("f(x) == 1"), std::string("!(f(x) == 1)"));
    CB_CHECK_EQ(s16::negate_condition("a && b"), std::string("!(a && b)"));
    CB_CHECK_EQ(s16::negate_condition("flag"), std::string("!(flag)"));
});

CB_TEST("verify.search16.if_else_blocks_and_swappable_statements", [] {
    const auto lines = lines_of(kFunction);
    // the if on line 9, its else on 12, the closing brace of the else on 14
    CB_CHECK((s16::if_else_blocks(lines) == std::vector<s16::IfElse>{{9, 12, 14}}));
    // no else: nothing; an else at another indent: nothing
    CB_CHECK(s16::if_else_blocks(lines_of("{\n    if (x) {\n        a();\n    }\n}")).empty());
    CB_CHECK(s16::if_else_blocks(lines_of("{\n    if (x) {\n        a();\n    }\n  else {\n        b();\n    }\n}")).empty());
    // two adjacent simple statements, one an assignment: lines 7 and 8 (x = a; y = b;)
    CB_CHECK((s16::swappable_statements(lines, 5) == std::vector<std::size_t>{7}));
    // a declaration is not a statement to swap; a control line is not either
    CB_CHECK(s16::swappable_statements(lines_of("{\n    int2 x;\n    int2 y;\n    return x;\n}"), 1).empty());
});

CB_TEST("verify.search16.each_rewrite_alone", [] {
    // two locals declared in the other order
    CB_CHECK_EQ(rewritten("f()\n{\n    int2 x;\n    int4 y;\n}").value(), std::string("f()\n{\n    int4 y;\n    int2 x;\n}"));
    // a local's sign
    CB_CHECK_EQ(rewritten("f()\n{\n    int2 x;\n}").value(), std::string("f()\n{\n    uint2 x;\n}"));
    CB_CHECK_EQ(rewritten("f()\n{\n    xunknown4 *p;\n}").value(), std::string("f()\n{\n    int4 *p;\n}"));   // (a pointer declaration flips its type too)
    CB_CHECK_EQ(rewritten("f()\n{\n    uint4 x;\n}").value(), std::string("f()\n{\n    int4 x;\n}"));
    CB_CHECK_EQ(rewritten("f()\n{\n    xunknown1 x;\n}").value(), std::string("f()\n{\n    int1 x;\n}"));
    // a comparison with a constant, the other way round: `x <= 0x19` is `x < 0x1a`
    CB_CHECK_EQ(rewritten("f()\n{\n    if (x <= 0x19) {\n    }\n}").value(), std::string("f()\n{\n    if (x < 0x1a) {\n    }\n}"));
    CB_CHECK_EQ(rewritten("f()\n{\n    if (x > 10) {\n    }\n}").value(), std::string("f()\n{\n    if (x >= 0xb) {\n    }\n}"));
    CB_CHECK_EQ(rewritten("f()\n{\n    if (x < -3) {\n    }\n}").value(), std::string("f()\n{\n    if (x <= -0x4) {\n    }\n}"));
    CB_CHECK_EQ(rewritten("f()\n{\n    if (x >= 0) {\n    }\n}").value(), std::string("f()\n{\n    if (x > -0x1) {\n    }\n}"));
    // the constant first, as Ghidra writes it: `(0x19 < x)` is `(x >= 0x1a)`
    CB_CHECK_EQ(rewritten("f()\n{\n    if ((0x19 < iVar1)) {\n    }\n}").value(), std::string("f()\n{\n    if ((iVar1 >= 0x1a)) {\n    }\n}"));
    // an if/else with the branches exchanged under the negated test
    CB_CHECK_EQ(rewritten("{\n    if (f(x)) {\n        a();\n    }\n    else {\n        b();\n    }\n}").value(),
                std::string("{\n    if (!(f(x))) {\n        b();\n    }\n    else {\n        a();\n    }\n}"));
    // two adjacent statements in the other order
    CB_CHECK_EQ(rewritten("{\n    a = 1;\n    b = 2;\n}").value(), std::string("{\n    b = 2;\n    a = 1;\n}"));
    // `x = x + 1` as `x += 1`
    CB_CHECK_EQ(rewritten("{\n    x = x + 1;\n}").value(), std::string("{\n    x += 1;\n}"));
    CB_CHECK_EQ(rewritten("{\n    x = x << 2;\n}").value(), std::string("{\n    x <<= 2;\n}"));
    CB_CHECK_EQ(rewritten("{\n    x += 2;\n}").value(), std::string("{\n    x = x + 2;\n}"));
    // (a == b) as (b == a)
    CB_CHECK_EQ(rewritten("{\n    y = (a + b);\n}").value(), std::string("{\n    y = (b + a);\n}"));
    // nothing of the kind: nothing
    CB_CHECK(!rewritten("f()\n{\n}"));
    CB_CHECK(!rewritten(""));
    // DIVERGES: a constant written in octal (a leading 0: the Python's int(x, 0) raised ValueError) is left alone
    CB_CHECK(!rewritten("f()\n{\n    if (x <= 010) {\n    }\n}"));
});

CB_TEST("verify.search16.increment_and_back", [] {
    // `x += 1` becomes `x++` or `x = x + 1`, whichever the draw says: both are seen
    std::set<std::string> seen;
    for (std::uint64_t seed = 1; seed <= 40; ++seed) seen.insert(rewritten("{\n    x += 1;\n}", seed).value());
    CB_CHECK((seen == std::set<std::string>{"{\n    x++;\n}", "{\n    x = x + 1;\n}"}));
    seen.clear();
    for (std::uint64_t seed = 1; seed <= 40; ++seed) seen.insert(rewritten("{\n    x -= 0x1;\n}", seed).value());
    CB_CHECK((seen == std::set<std::string>{"{\n    x--;\n}", "{\n    x = x - 0x1;\n}"}));
});

CB_TEST("verify.search16.rewrites_are_repeatable_and_varied", [] {
    // the same seed, the same rewrite; every rewrite of a rich function is one of its own kinds and differs from it
    std::set<std::string> seen;
    for (std::uint64_t seed = 1; seed <= 60; ++seed) {
        const auto r = rewritten(kFunction, seed);
        CB_CHECK(r.has_value());
        CB_CHECK(*r != kFunction);
        CB_CHECK(r == rewritten(kFunction, seed));
        CB_CHECK_EQ(cb::text::lines(*r).size(), cb::text::lines(kFunction).size());   // no rewrite adds or drops a line
        seen.insert(*r);
    }
    CB_CHECK(seen.size() >= 5);   // the draw reaches several of the kinds
});

CB_TEST("verify.search16.search_rounds", [] {
    // two near misses. `f` is identical once its two locals are declared the other way round; `g` never improves.
    const std::string f_text = "f()\n{\n    int2 x;\n    int4 y;\n}";
    const std::string f_identical = "f()\n{\n    int4 y;\n    int2 x;\n}";
    const s16::Group group{"mc2", "-Ox", "w"};
    const s16::ByteVerdict far{ByteMatch::Different, 6, 20};
    std::vector<s16::NearMiss> near = {{"f", f_text, far, group}, {"g", "g()\n{\n    a = 1;\n    b = 2;\n}", far, group}};
    int calls = 0;
    const auto compile = [&](const s16::Group&, const std::map<std::string, std::string>& texts) {
        ++calls;
        std::map<std::string, s16::ByteVerdict> out;
        for (const auto& [fn, text] : texts) {
            out[fn] = text == f_identical ? s16::ByteVerdict{ByteMatch::Identical, 0, 20} : far;
        }
        return out;
    };
    const auto score = [](const s16::ByteVerdict& v) { return v.match == ByteMatch::Identical ? 1000.0 : -static_cast<double>(v.differing); };
    std::vector<std::string> log;
    s16::Options options{.rounds = 5, .log = [&](std::string_view line) { log.emplace_back(line); }};
    const auto best = s16::search(near, compile, score, options);
    CB_CHECK_EQ(best.size(), std::size_t{2});
    CB_CHECK_EQ(best[0].function, std::string("f"));
    CB_CHECK(best[0].verdict.match == ByteMatch::Identical);   // `f` found
    CB_CHECK_EQ(best[0].text, f_identical);
    CB_CHECK_EQ(best[1].function, std::string("g"));
    CB_CHECK(best[1].verdict.match == ByteMatch::Different);   // `g` is left as it was
    CB_CHECK_EQ(best[1].text, near[1].text);
    CB_CHECK_EQ(log.size(), std::size_t{5});
    CB_CHECK_EQ(log[0].substr(0, 10), std::string("  round 1:"));
    CB_CHECK(log.back().ends_with("1 identical"));
    CB_CHECK(calls >= 2);
    // a function that is identical already is not rewritten again; with nothing to rewrite the search ends early
    std::vector<s16::NearMiss> done = {{"f", f_identical, {ByteMatch::Identical, 0, 20}, group}};
    int again = 0;
    const auto count = [&](const s16::Group&, const std::map<std::string, std::string>&) { ++again; return std::map<std::string, s16::ByteVerdict>(); };
    const auto kept = s16::search(done, count, score, {.rounds = 3});
    CB_CHECK_EQ(again, 0);
    CB_CHECK_EQ(kept[0].text, f_identical);
});

CB_TEST("verify.search16.search_groups_and_repeatability", [] {
    // functions under different groups are compiled in group order, each group once a round
    const s16::ByteVerdict far{ByteMatch::Different, 6, 20};
    const std::string text = "f()\n{\n    int2 x;\n}";   // only a sign flip is possible
    std::vector<s16::NearMiss> near = {{"b", text, far, {"mc3", "-O", "w"}}, {"a", text, far, {"mc2", "-O", "w"}}, {"c", text, far, {"mc2", "-O", "w"}}};
    std::vector<std::string> order;
    const auto compile = [&](const s16::Group& g, const std::map<std::string, std::string>& texts) {
        std::string line = g.compiler + ":";
        std::map<std::string, s16::ByteVerdict> out;
        for (const auto& [fn, t] : texts) {
            line += fn;
            out[fn] = {ByteMatch::SameSize, 2, 20};
            (void)t;
        }
        order.push_back(line);
        out["unknown"] = far;   // a function that was not asked about is ignored
        return out;
    };
    const auto score = [](const s16::ByteVerdict& v) { return -static_cast<double>(v.differing); };
    const auto best = s16::search(near, compile, score, {.rounds = 1});
    CB_CHECK((order == std::vector<std::string>{"mc2:ac", "mc3:b"}));
    CB_CHECK_EQ(best.size(), std::size_t{3});
    for (const auto& b : best) CB_CHECK_EQ(b.text, std::string("f()\n{\n    uint2 x;\n}"));
    // repeatable: the same seed gives the same search
    const auto rich = [&](std::uint64_t seed) {
        std::vector<s16::NearMiss> n = {{"f", kFunction, far, {"mc2", "-O", "w"}}};
        const auto c = [&](const s16::Group&, const std::map<std::string, std::string>& texts) {
            std::map<std::string, s16::ByteVerdict> out;
            for (const auto& [fn, t] : texts) out[fn] = {ByteMatch::SameSize, t.size() % 7, 20};
            return out;
        };
        return s16::search(n, c, score, {.rounds = 8, .seed = seed})[0].text;
    };
    CB_CHECK_EQ(rich(7), rich(7));
});
