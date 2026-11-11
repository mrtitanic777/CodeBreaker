#include "cb/exact/draft/text.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::draft::text;
using Json = nlohmann::json;

// DIVERGES from the Python module (draft/text.py), on purpose:
//   * a line index that is negative or past the end was Python indexing (a wrap-around or IndexError); here it is a
//     bug in the caller (InternalError)
//   * \w was Unicode in Python, here it is ASCII: a draft is ASCII, and a non-ASCII byte is not part of a name
//   * the register of live_after / plain_store is a name, not a regular expression
//   * the module's tables (UCHAR, ARRAYS, LONGINFO ...) are FunctionFacts / ProjectCaches held by the passes
//   * GOTO_LIVE, a global switch nothing set, is LiveOptions::follow_gotos

namespace {

using Strings = std::vector<std::string>;

Json json_of(const Strings& v) {
    Json a = Json::array();
    for (const std::string& s : v) a.push_back(s);
    return a;
}

// ---- the golden file: cases the old harness ran against the Python module, with the results it gave -----------------
// One case per line: `op <TAB> arg ... <TAB> => <TAB> {"r": result}`. An argument is `S<text>` (a string), `N` (None)
// or `L<item><US><item>...` (a list, each item `S<text>` or `N`); the text escapes \t \n \r and \\.

struct Arg {
    bool none = false, is_list = false;
    std::string text;
    std::vector<std::optional<std::string>> items;
    std::int64_t num() const { return *text::parse_int<std::int64_t>(text); }
    Strings lines() const {
        Strings out;
        for (const auto& i : items) out.push_back(i.value_or(""));
        return out;
    }
};

std::string unescape(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            const char c = s[++i];
            out += c == 't' ? '\t' : c == 'n' ? '\n' : c == 'r' ? '\r' : c;
        } else {
            out += s[i];
        }
    }
    return out;
}

Arg parse_arg(std::string_view f) {
    Arg a;
    if (f == "N") {
        a.none = true;
    } else if (f[0] == 'S') {
        a.text = unescape(f.substr(1));
    } else {
        a.is_list = true;
        if (f.size() > 1)
            for (std::string_view item : text::split(f.substr(1), '\x1f'))
                a.items.push_back(item == "N" ? std::nullopt : std::optional<std::string>(unescape(item.substr(1))));
    }
    return a;
}

// The words a harness read pattern names (\bX\b or \b(?:X|Y)\b or \bX\b|\bY\b).
Strings words_of(const std::string& pattern) {
    std::string p = text::replace_all(pattern, "\\b", "");
    p = text::replace_all(text::replace_all(text::replace_all(p, "(?:", ""), "(", ""), ")", "");
    Strings words;
    for (std::string_view w : text::split(p, '|')) words.emplace_back(w);
    return words;
}

Json run_case(const std::string& op, const std::vector<Arg>& a) {
    if (op == "strip") return strip_parens(a[0].text);
    if (op == "truth") return truth(a[0].text);
    if (op == "balanced") return balanced(a[0].text);
    if (op == "not") return negate(a[0].text);
    if (op == "topsplit") return json_of(top_split(a[0].text, a[1].text));
    if (op == "topcommas") return json_of(top_commas(a[0].text));
    if (op == "conj") return join_tests(a[0].text, a[1].text, a[2].text);
    if (op == "opener" || op == "blockend") {
        const auto r = op == "opener" ? opener(a[0].lines(), static_cast<std::size_t>(a[1].num())) : block_end(a[0].lines(), static_cast<std::size_t>(a[1].num()));
        return r ? Json(*r) : Json(nullptr);
    }
    if (op == "openers") {
        Json out = Json::array();
        for (std::size_t x : openers(a[0].lines(), static_cast<std::size_t>(a[1].num()))) out.push_back(x);
        return out;
    }
    if (op == "ifparts") {
        const auto r = if_parts(a[0].lines(), static_cast<std::size_t>(a[1].num()));
        if (!r) return nullptr;
        return Json::array({r->cond, json_of(r->then_arm), r->else_arm ? json_of(*r->else_arm) : Json(nullptr), r->end});
    }
    if (op == "inloop") return in_loop(a[0].lines(), a[1].num(), static_cast<std::size_t>(a[2].num()), static_cast<std::size_t>(a[3].num()));
    if (op == "elseifs") {
        Strings lines = a[0].lines();
        else_ifs(lines);
        return Json::array({json_of(lines), true, json_of(lines)});   // (the result and the rewritten argument are one list)
    }
    if (op == "liveafter" || op == "liveafter0") {
        std::vector<std::optional<std::string>> lines = a[0].items;
        Strings words;
        if (!a[3].none && !a[3].text.empty()) words = words_of(a[3].text);
        const std::vector<std::string_view> views(words.begin(), words.end());
        const std::int64_t k = a[1].num();
        return live_after(lines, static_cast<std::size_t>(k + 1), a[2].text, views, LiveOptions{op == "liveafter"});
    }
    if (op == "plainstore") return plain_store(a[0].text, a[1].text);
    if (op == "unlabelled") {
        std::optional<Strings> other;
        if (!a[1].none) other = a[1].lines();
        return json_of(unlabelled(a[0].lines(), other));
    }
    if (op == "samearm") return same_arm(a[0].lines(), a[1].lines());
    if (op == "declptr") {
        const auto r = declare_pointer(a[0].text, a[1].text, a[2].text);
        return r ? Json(*r) : Json(nullptr);
    }
    if (op == "splitbody") {
        const BodySplit s = split_body(a[0].text);
        return Json::array({s.head, s.brace, s.body});
    }
    throw std::runtime_error("unknown op " + op);
}

}  // namespace

CB_TEST("exact.draft.text.golden_cases_from_the_python_module", [] {
    const std::string file = fs::read_text(testing::data_path("exact/testdata/draft_text.cases"));
    std::size_t cases = 0;
    for (std::string_view line : text::lines(file)) {
        const std::vector<std::string_view> fields = text::split(line, '\t');
        // op, args..., "=>", result
        std::size_t arrow = fields.size();
        while (arrow-- > 0 && fields[arrow] != "=>") {}
        CB_CHECK(arrow != static_cast<std::size_t>(-1) && arrow + 2 == fields.size());
        std::vector<Arg> args;
        for (std::size_t i = 1; i < arrow; ++i) args.push_back(parse_arg(fields[i]));
        const Json expected = Json::parse(fields[arrow + 1]).at("r");
        Json got;
        try {
            got = run_case(std::string(fields[0]), args);
        } catch (const std::exception& e) {
            got = std::string("exception: ") + e.what();
        }
        if (got != expected) {
            testing::record_failure(__FILE__, __LINE__, std::format("{} {}\n    got:      {}\n    expected: {}", fields[0], fields[1].substr(0, 200), got.dump().substr(0, 300), expected.dump().substr(0, 300)));
        }
        ++cases;
    }
    CB_CHECK(cases > 250);
});

CB_TEST("exact.draft.text.tests_and_their_negation", [] {
    CB_CHECK_EQ(strip_parens("((a == b))"), "(a == b)");
    CB_CHECK_EQ(strip_parens("((a) + (b))"), "(a) + (b)");
    CB_CHECK_EQ(strip_parens("((a)) && ((b))"), "((a)) && ((b))");   // the outer pair does not wrap the whole
    CB_CHECK_EQ(truth("(x != 0)"), "x");
    CB_CHECK_EQ(truth("(p->f == 0)"), "!p->f");
    CB_CHECK_EQ(truth("(a + b == 0)"), "!(a + b)");
    CB_CHECK_EQ(negate("(a == b)"), "(a != b)");
    CB_CHECK_EQ(negate("(a < 0)"), "(a >= 0)");
    CB_CHECK_EQ(negate("(x == 0)"), "x");
    CB_CHECK_EQ(negate("(x != 0)"), "!x");
    CB_CHECK_EQ(negate("!(a && b)"), "(a && b)");
    CB_CHECK_EQ(negate("(a && b)"), "(!a || !b)");   // De Morgan, a level down
    CB_CHECK_EQ(join_tests("&&", "a || b", "c"), "(a || b) && c");   // the other kind of join is parenthesized
    CB_CHECK_EQ(join_tests("&&", "(a || b)", "c"), "(a || b) && c");  // ... once
    CB_CHECK_EQ(join_tests("||", "a", "b"), "a || b");
    CB_CHECK((top_split("(a && b) && c && (d || e)", "&&") == Strings{"(a && b)", "c", "(d || e)"}));
    CB_CHECK((top_commas("f(a, b), [c, d], e") == Strings{"f(a, b)", " [c, d]", " e"}));
    CB_CHECK(balanced("(a(b))") && !balanced("(a))(") && !balanced("(a"));
});

CB_TEST("exact.draft.text.blocks_by_indentation", [] {
    const Strings lines = {"if ((a)) {", "    x = 1;", "    if ((b)) {", "        y = 2;", "    }", "} else {", "    z = 3;", "}", "w = 4;"};
    CB_CHECK_EQ(*opener(lines, 4), 2u);          // the `}` at depth 1 closes the if opened at line 2
    CB_CHECK_EQ(*opener(lines, 7), 5u);          // the final `}` closes the else (the nearest before it at its indent)
    CB_CHECK((openers(lines, 3) == std::vector<std::size_t>{2, 0}));
    CB_CHECK_EQ(*block_end(lines, 0), 5u);       // the first line back at the if's own indent
    CB_CHECK(!block_end(lines, 8).has_value());
    const auto parts = if_parts(lines, 0);
    CB_CHECK(parts.has_value());
    CB_CHECK_EQ(parts->cond, "(a)");
    CB_CHECK_EQ(parts->then_arm.size(), 4u);
    CB_CHECK(parts->else_arm.has_value() && parts->else_arm->size() == 1);
    CB_CHECK_EQ(parts->end, 8u);
    CB_CHECK(!if_parts(lines, 1).has_value());   // not an if
    // a line not deeper than the if inside its arm ends the hope
    CB_CHECK(!if_parts(Strings{"if ((a)) {", "x = 1;", "}"}, 0).has_value());
    // loops and switches
    // a loop opened after line `lo`, at an indentation deeper than `ind`
    const Strings looped = {"f() {", "    while ((a)) {", "        if ((b)) {", "            x = 1;", "        }", "    }", "}"};
    CB_CHECK(in_loop(looped, -1, 3, 0));
    CB_CHECK(!in_loop(looped, -1, 3, 4));         // the loop is at indent 4: not deeper than 4
    CB_CHECK(!in_loop(looped, 1, 3, 0));          // opened at line 1, not after it
    CB_CHECK(!in_loop(looped, -1, 1, 0));         // line 1 is the loop itself
});

CB_TEST("exact.draft.text.else_if_chains_become_nested_ifs", [] {
    Strings lines = {"if ((a)) {", "    x = 1;", "} else if ((b)) {", "    y = 2;", "} else {", "    z = 3;", "}"};
    else_ifs(lines);
    CB_CHECK((lines == Strings{"if ((a)) {", "    x = 1;", "} else {", "    if ((b)) {", "        y = 2;", "    } else {", "        z = 3;", "    }", "}"}));
    Strings none = {"x = 1;"};
    else_ifs(none);
    CB_CHECK((none == Strings{"x = 1;"}));
});

CB_TEST("exact.draft.text.a_register_read_later", [] {
    const Strings straight = {"ax = 1;", "bx = ax;", "return bx;"};
    CB_CHECK(live_after(straight, 1, "ax"));                       // read on line 1
    CB_CHECK(!live_after(straight, 2, "ax"));
    CB_CHECK(!live_after(Strings{"ax = 1;", "ax = 2;", "return ax;"}, 1, "ax"));   // given a new value first
    CB_CHECK(live_after(Strings{"ax = 1;", "ax = ax + 1;"}, 1, "ax"));              // updated, not given
    // in a nested block a value is not on every path: it ends nothing
    CB_CHECK(live_after(Strings{"if ((c)) {", "    ax = 2;", "}", "return ax;"}, 0, "ax"));
    // the byte of a register: the words of `reads`
    const std::string_view reads[] = {"ax", "al"};
    CB_CHECK(live_after(Strings{"cx = al;"}, 0, "ax", reads));
    CB_CHECK(!live_after(Strings{"cx = al;"}, 0, "ax"));
    // a jump elsewhere: read there before it is given a value?
    const Strings jump = {"goto L_9;", "return;", "L_9:", "dx = ax;"};
    CB_CHECK(live_after(jump, 0, "ax"));
    CB_CHECK(!live_after(jump, 0, "ax", {}, LiveOptions{false}));
    // dropped lines are skipped
    const std::vector<std::optional<std::string>> dropped = {std::nullopt, std::string("cx = ax;")};
    CB_CHECK(live_after(dropped, 0, "ax"));
    // the else arm beside the start's is not on its path
    CB_CHECK(!live_after(Strings{"    bx = 1;", "} else {", "    cx = ax;", "}"}, 0, "ax"));
});

CB_TEST("exact.draft.text.arms_and_stores", [] {
    CB_CHECK(same_arm(Strings{"x = 1;"}, Strings{"x = 1;"}));
    CB_CHECK(same_arm(Strings{"x = 1;", "goto L_5;"}, Strings{"x = 1;", "L_5:", "y = 2;"}));
    CB_CHECK(!same_arm(Strings{"x = 1;", "goto L_5;"}, Strings{"x = 2;", "L_5:"}));
    CB_CHECK((unlabelled(Strings{"L_1:", "x = 1;", "L_2:"}) == Strings{"x = 1;"}));
    // with the other arm's goto: the label it went to is the one that goes (the arm now holds its code); the others stay
    CB_CHECK((unlabelled(Strings{"L_1:", "L_2:", "x = 1;"}, Strings{"y = 1;", "goto L_2;"}) == Strings{"L_1:", "x = 1;"}));
    // the other arm does not end in a goto: nothing is changed
    CB_CHECK((unlabelled(Strings{"L_1:", "x = 1;"}, Strings{"y = 1;"}) == Strings{"L_1:", "x = 1;"}));
    CB_CHECK(plain_store("    x = y;", "ax"));
    CB_CHECK(!plain_store("    ax = y;", "ax"));          // the register is the target
    CB_CHECK(!plain_store("    x = y++;", "ax"));
    CB_CHECK(!plain_store("    x = f(1);", "ax"));        // a call
    CB_CHECK(!plain_store("    x == y;", "ax"));
    CB_CHECK(plain_store("    = y;", "ax"));             // (a left side of blanks alone still reads as one)
    CB_CHECK(goto_target("    goto L_7;", true).value() == "L_7" && !goto_target("goto L_7; x", true) && goto_target("goto L_7; x", false));
    CB_CHECK(label_line("  L_7:") && !label_line("L_7: x") && !label_line("L_:"));
});

CB_TEST("exact.draft.text.the_function_text", [] {
    const std::string src = "int f(a, b)\nint a, b;\n{\n    register int x, y, z;\n    return x;\n}\n";
    const auto declared = declare_pointer(src, "y", "char");
    CB_CHECK(declared.has_value());
    CB_CHECK_EQ(*declared, "int f(a, b)\nint a, b;\n{\n    register int x, z;\n    register char *y;\n    return x;\n}\n");
    CB_CHECK(!declare_pointer(src, "q", "char").has_value());
    // as an int pointer: the byte offsets through it become word offsets
    const std::string body = "int f(p)\nint p;\n{\n    return *((int *)((char *)p + 4)) + p[(i << 1) + 2] + p[(j << 1)];\n}\n";
    const auto ptr = declare_pointer(body, "p", "int");
    CB_CHECK(ptr.has_value());
    CB_CHECK(ptr->find("int *p;") != std::string::npos);
    CB_CHECK(ptr->find("(p + 2)") != std::string::npos);
    CB_CHECK(ptr->find("p[i + 1]") != std::string::npos);
    CB_CHECK(ptr->find("p[j]") != std::string::npos);
    const BodySplit s = split_body("struct s {\n    int a;\n};\nint f()\n{\n    return 1;\n}\n");
    CB_CHECK_EQ(s.head, "struct s {\n    int a;\n};\nint f()\n");   // the body's own brace: a line of "{" alone
    CB_CHECK_EQ(s.brace, "{");
    CB_CHECK_EQ(s.body, "\n    return 1;\n}\n");
    const BodySplit none = split_body("int a;");
    CB_CHECK(none.brace.empty() && none.head == "int a;");
});

CB_TEST("exact.draft.text.what_is_ascii_here", [] {
    // a name is ASCII: the byte after an identifier that is not (a UTF-8 e-acute) ends it
    CB_CHECK_EQ(truth("(\xC3\xA9 == 0)"), "!(\xC3\xA9)");
    CB_CHECK(is_register("ax") && is_register("es") && !is_register("ex"));
    CB_CHECK(is_word_register("si") && !is_word_register("al"));
    CB_CHECK_EQ(size_of_type("char"), 1u);
    CB_CHECK_EQ(size_of_type("long"), 4u);
    CB_CHECK_EQ(size_of_type("float"), 2u);
    CB_CHECK_EQ(size_of_type("float", 8), 8u);
    FunctionFacts facts;
    facts.arrays["g_0010"] = "int";
    facts.streams.insert_or_assign("g_0020", "FILE");
    facts.longs.ret = true;
    facts.clear();
    CB_CHECK(facts.arrays.empty() && facts.streams.empty() && !facts.longs.ret.has_value());
});

CB_TEST("exact.draft.text.line_indexes_out_of_range_are_bugs", [] {
    const Strings lines = {"a", "b"};
    CB_CHECK_THROWS_AS(InternalError, (void)opener(lines, 2));
    CB_CHECK_THROWS_AS(InternalError, (void)block_end(lines, 9));
    CB_CHECK_THROWS_AS(InternalError, (void)if_parts(lines, 2));
});
