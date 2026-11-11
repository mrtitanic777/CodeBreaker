#include "cb/exact/draft/shapes.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb::exact::draft::shapes;
namespace fs = cb::fs;
namespace testing = cb::testing;
namespace dt = cb::exact::draft::text;
namespace txt = cb::text;
using Json = nlohmann::json;

// DIVERGES from the Python module (draft/shapes.py), on purpose:
//   * a text that sends the Python store_joins past its last line (31 of the 11,900 drafts of the old harness; an
//     IndexError there): the pass finds nothing to rewrite. The Python was fixed the same way (see the _bugs list of
//     the refactor), so both give this answer
//   * \w \s \b are ASCII (Python's were Unicode): a draft is ASCII, and a non-ASCII byte is not part of a name
//   * a number written with more digits than fit in 64 bits (a case label, a field offset, a step's amount) is not read:
//     the pass leaves that text alone (Python's integers had no end, and no draft has such a number)
//   * the module's tables (UCHAR, PTRSIZE, LONGINFO, RANGES) are the FunctionFacts the caller passes

namespace {

using Strings = std::vector<std::string>;
using dt::FunctionFacts;

// ---- every pass by its name ---------------------------------------------------------------------------------------

struct Result {
    Json value;
    bool is_text = false;   // a pass that gives text (so "unchanged" can be told from the text itself)
    std::string text;
};

Result of_text(std::string s) { return {Json(s), true, s}; }
Result of_optional(const std::optional<std::string>& s) { return s ? of_text(*s) : Result{Json(nullptr), false, {}}; }
Result of_list(const Strings& v) {
    Json a = Json::array();
    for (const std::string& s : v) a.push_back(s);
    return {a, false, {}};
}

using Pass = std::function<Result(const std::string&, FunctionFacts&)>;

const std::map<std::string, Pass>& passes() {
    static const std::map<std::string, Pass> m = [] {
        std::map<std::string, Pass> t;
#define TEXT(NAME) t[#NAME] = [](const std::string& s, FunctionFacts&) { return of_text(NAME(s)); }
#define OPT(NAME) t[#NAME] = [](const std::string& s, FunctionFacts&) { return of_optional(NAME(s)); }
#define LEARN(NAME) t[#NAME] = [](const std::string& s, FunctionFacts& f) { return of_text(NAME(s, f)); }
        TEXT(compare_values); TEXT(logical_values); TEXT(step_tests); TEXT(conditional_values); TEXT(branch_returns);
        TEXT(push_returns); TEXT(chain_returns); LEARN(zero_extends); LEARN(high_zero); TEXT(widen_bytes);
        TEXT(post_increments); TEXT(loop_registers); OPT(const_tests); OPT(loop_guards); TEXT(first_breaks);
        OPT(loop_tests); OPT(goto_tails); TEXT(call_tests); TEXT(fall_dups); TEXT(fall_gotos); TEXT(tested_zero);
        TEXT(continue_loops); TEXT(empty_loops); TEXT(stored_reads); TEXT(stored_tests); OPT(flat_returns);
        TEXT(switch_follow); OPT(return_cases_first); TEXT(selector_reads); TEXT(word_offsets); TEXT(byte_heads);
        TEXT(common_tails); TEXT(common_heads); OPT(call_pairs); TEXT(lvalue_choices); OPT(label_sinks); OPT(shared_exit);
        LEARN(pointer_index); OPT(heads_variant); OPT(chain_tails); TEXT(goto_returns); OPT(char_widens);
        TEXT(tail_continues); OPT(assign_loops); TEXT(store_joins); TEXT(loop_collapse); TEXT(switch_ifs);
        TEXT(ternary_joins); TEXT(stored_values); TEXT(two_case_switches); LEARN(long_tests); LEARN(sunk_pairs);
        TEXT(sunk_tails); TEXT(register_chains); TEXT(store_loops); TEXT(copy_props); TEXT(self_updates);
        TEXT(post_steps); TEXT(copy_tests); TEXT(test_first_loops); TEXT(stored_calls); OPT(for_loops); TEXT(temporaries);
        TEXT(fold_registers);
#undef TEXT
#undef OPT
#undef LEARN
        t["goto_tails1"] = [](const std::string& s, FunctionFacts&) { return of_optional(goto_tails(s, 1)); };
        t["case_guards"] = [](const std::string& s, FunctionFacts&) { return of_list(case_guards(s)); };
        t["unsigned_fields"] = [](const std::string& s, FunctionFacts&) { return of_list(unsigned_fields(s)); };
        t["unsigned_fields1"] = [](const std::string& s, FunctionFacts&) { return of_list(unsigned_fields(s, 1)); };
        t["adjacent_swaps"] = [](const std::string& s, FunctionFacts&) { return of_list(adjacent_swaps(s)); };
        t["adjacent_swaps1"] = [](const std::string& s, FunctionFacts&) { return of_list(adjacent_swaps(s, 1)); };
        t["range_switches"] = [](const std::string& s, FunctionFacts& f) { return of_text(range_switches(s, f)); };
        t["range_switches_f"] = t["range_switches"];
        t["range_switches_all"] = t["range_switches"];
        return t;
    }();
    return m;
}

// The ranges the harness gave range_switches: every run LO..HI (LO 0..5, up to 4 more) in the direction of "below",
// and for "all" in both.
void give_ranges(const std::string& op, FunctionFacts& facts) {
    if (op != "range_switches_f" && op != "range_switches_all") return;
    for (std::int64_t lo = 0; lo < 6; ++lo)
        for (std::int64_t d = 0; d < 5; ++d) {
            facts.ranges.insert(dt::Range{lo, lo + d, false});
            if (op == "range_switches_all") facts.ranges.insert(dt::Range{lo, lo + d, true});
        }
}

// What a pass recorded of the function, as the golden file writes it ("t").
Json tables_of(const FunctionFacts& f) {
    Json t = Json::object();
    if (!f.unsigned_sites.empty() || !f.unsigned_globals.empty()) {
        Strings items;
        for (const dt::Site& s : f.unsigned_sites) items.push_back(Json::array({s.pointer, s.offset}).dump());
        for (const std::string& g : f.unsigned_globals) items.push_back(Json(g).dump());
        std::sort(items.begin(), items.end());
        t["u"] = items;
    }
    if (!f.word_pointers.empty()) {
        Json p = Json::array();
        for (const std::string& w : f.word_pointers) p.push_back(Json::array({w, 2}));
        t["p"] = p;
    }
    Json l = Json::object();
    if (!f.longs.fields.empty()) l["f"] = std::vector<std::int64_t>(f.longs.fields.begin(), f.longs.fields.end());
    if (!f.longs.globals.empty()) l["g"] = std::vector<std::string>(f.longs.globals.begin(), f.longs.globals.end());
    if (!l.empty()) t["l"] = l;
    return t;
}

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

}  // namespace

// ---- the golden file: cases the Python module ran, with the results it gave -----------------------------------------

CB_TEST("exact.draft.shapes.golden_cases_from_the_python_module", [] {
    const std::string file = fs::read_text(testing::data_path("exact/testdata/draft_shapes.cases"));
    std::set<std::string> ops_seen;
    std::size_t cases = 0;
    for (std::string_view line : txt::lines(file)) {
        if (line.empty()) continue;
        const std::vector<std::string_view> fields = txt::split(line, '\t');
        std::size_t arrow = fields.size();
        while (arrow-- > 0 && fields[arrow] != "=>") {}
        CB_CHECK(arrow != static_cast<std::size_t>(-1) && arrow + 2 == fields.size());
        const std::string op(fields[0]);
        std::vector<std::string> args;
        for (std::size_t i = 1; i < arrow; ++i) args.push_back(unescape(fields[i].substr(1)));
        const Json expected = Json::parse(fields[arrow + 1]);
        FunctionFacts facts;
        std::string got_body;
        Json got_tables;
        if (op == "ternary") {
            got_body = Json{{"r", ternary(args[0], args[1], args[2])}}.dump();
        } else if (op == "pairlong") {
            const auto r = pair_long(args[0], args[1], facts);
            got_body = Json{{"r", r ? Json(*r) : Json(nullptr)}}.dump();
        } else {
            const auto it = passes().find(op);
            CB_CHECK(it != passes().end());
            if (it == passes().end()) continue;
            give_ranges(op, facts);
            const Result r = it->second(args[0], facts);
            got_body = r.is_text && r.text == args[0] ? std::string(R"({"same":true})") : Json{{"r", r.value}}.dump();
        }
        const Json got_json = Json::parse(got_body);
        const Json got_t = tables_of(facts);
        Json want = expected;
        const Json want_t = want.contains("t") ? want["t"] : Json::object();
        want.erase("t");
        if (got_json != want || got_t != want_t)
            testing::record_failure(__FILE__, __LINE__,
                                    std::format("{} {}\n    got:      {} {}\n    expected: {} {}", op, args.empty() ? "" : args[0].substr(0, 200),
                                                got_body.substr(0, 400), got_t.dump(), want.dump().substr(0, 400), want_t.dump()));
        ops_seen.insert(op);
        ++cases;
    }
    CB_CHECK(cases > 400);
    CB_CHECK(ops_seen.size() > 70);
});

// ---- the passes on small drafts, written out -------------------------------------------------------------------------

CB_TEST("exact.draft.shapes.values_the_compiler_leaves_behind_a_test", [] {
    // a comparison's value: the compiler's 1, then 0 unless it holds
    CB_CHECK_EQ(compare_values("    if ((a < b)) {\n        ax = 1;\n    } else {\n        ax = 0;\n    }"), "    ax = (a < b);");
    CB_CHECK_EQ(compare_values("    if ((a < b)) {\n        ax = 0;\n    } else {\n        ax = 1;\n    }"), "    ax = !((a < b));");
    // a byte given one or the other and widened: a char's ?:
    CB_CHECK_EQ(compare_values("    if ((x)) {\n        al = 3;\n    } else {\n        al = 4;\n    }\n    ah = 0;"),
                "    ax = ((unsigned char)((x) ? 3 : 4));");
    // || and && taken as values
    CB_CHECK_EQ(logical_values("    if ((a)) {\n        if ((b)) {\n            r = 1;\n        } else {\n            r = 0;\n        }\n    } else {\n        r = 0;\n    }"),
                "    r = (a) && (b);");
    // a register given one value or the other by a test and read once after it: C's ?:
    CB_CHECK_EQ(conditional_values("    if ((c)) {\n        ax = 1;\n    } else {\n        ax = 2;\n    }\n    x = ax;"), "    ax = ((c) ? 1 : 2);\n    x = ax;");
    // two tests of the same two values join
    CB_CHECK_EQ(ternary("c", "(d ? X : Y)", "Y"), "(c) && (d) ? X : Y");
    CB_CHECK_EQ(ternary("c", "X", "Y"), "c ? X : Y");
    // inc al; jne
    CB_CHECK_EQ(step_tests("    al = (x + 1);\n    if ((al == 0)) {"), "    if ((x == -1)) {");
    // a call whose value the next test reads
    CB_CHECK_EQ(call_tests("    f(a);\n    if ((ax == 0)) {"), "    if ((f(a) == 0)) {");
    CB_CHECK_EQ(call_tests("    if (((x + 1) == 0)) {"), "    if ((x == -1)) {");
});

CB_TEST("exact.draft.shapes.returns_gotos_and_tails", [] {
    // if (C) { r = E1; } else { ..; r = E2; } return r;  ->  each way returns its own
    CB_CHECK_EQ(branch_returns("    if ((c)) {\n        ax = 1;\n    } else {\n        foo();\n        ax = 2;\n    }\n    return ax;"),
                "    if ((c))\n        return 1;\n    foo();\n    return 2;");
    // a jump to a plain return is that return
    CB_CHECK_EQ(goto_returns("    if ((a)) {\n        goto L_1;\n    }\n    foo();\n    L_1:\n    return 0;"),
                "    if ((a)) {\n        return 0;\n    }\n    foo();\n    return 0;");
    // a goto that only falls to its label is gone, and the label with it
    CB_CHECK_EQ(fall_gotos("    goto L_1;\n    L_1:\n    x = 1;"), "    x = 1;");
    // both arms ending in the same statement: once, after the if
    CB_CHECK_EQ(common_tails("    if ((c)) {\n        a = 1;\n        done();\n    } else {\n        b = 2;\n        done();\n    }"),
                "    if ((c)) {\n        a = 1;\n    } else {\n        b = 2;\n    }\n    done();");
    // both arms beginning with the same statement: before the if
    CB_CHECK_EQ(common_heads("    if ((c)) {\n        x = 5;\n        a = 1;\n    } else {\n        x = 5;\n        b = 2;\n    }"),
                "    x = 5;\n    if ((c)) {\n        a = 1;\n    } else {\n        b = 2;\n    }");
    // the variants: nothing to change is nothing
    CB_CHECK(!goto_tails("    x = 1;").has_value());
    CB_CHECK(!flat_returns("    x = 1;").has_value());    // no function body at all
    CB_CHECK(!chain_tails("    x = 1;").has_value());
    CB_CHECK(!const_tests("    x = 1;\n    y = 2;").has_value());
    CB_CHECK_EQ(*const_tests("    x = 3;\n    if ((x < 5)) {"), "    x = 3;\n    if ((3 < 5)) {");
});

CB_TEST("exact.draft.shapes.loops", [] {
    CB_CHECK_EQ(first_breaks("    for (;;) {\n        if ((a)) break;\n        x();\n    }"), "    while (!a) {\n        x();\n    }");
    CB_CHECK_EQ(test_first_loops("    for (;;) {\n        if ((a == 3)) break;\n        x();\n    }"), "    while ((a != 3)) {\n        x();\n    }");
    // a while loop ending by stepping a variable is a for
    CB_CHECK_EQ(*for_loops("    i = 0;\n    while ((i < 5)) {\n        f(i);\n        i = i + 1;\n    }"),
                "    for (i = 0; (i < 5); i = i + 1) {\n        f(i);\n    }");
    CB_CHECK(!for_loops("    while ((i < 5)) {\n        f(i);\n    }").has_value());
    // a copy loop
    CB_CHECK_EQ(store_loops("    do {\n        al = *s;\n        s = s + 1;\n        *d = al;\n    } while ((al != 0));"), "    while (*d = *s++)\n        ;");
    // a continue the loop reaches anyway
    CB_CHECK_EQ(tail_continues("    while ((a)) {\n        if ((b)) {\n            x();\n            continue;\n        }\n    }"),
                "    while ((a)) {\n        if ((b)) {\n            x();\n        }\n    }");
});

CB_TEST("exact.draft.shapes.switches", [] {
    CB_CHECK_EQ(fall_dups("    switch (x) {\n    case 1:\n        a();\n        return;\n    case 2:\n        return;\n    }"),
                "    switch (x) {\n    case 1:\n        a();\n    case 2:\n        return;\n    }");
    // the arms that only return first
    CB_CHECK_EQ(*return_cases_first("    switch (x) {\n    case 1:\n        a();\n        break;\n    case 2:\n        return 7;\n    }"),
                "    switch (x) {\n    case 2:\n        return 7;\n    case 1:\n        a();\n        break;\n    }");
    // two values to one arm
    CB_CHECK_EQ(two_case_switches("    ax = n;\n    if ((ax != 1)) {\n        if ((ax == 2)) {\n            L_1:\n            work();\n        }\n    } else {\n        goto L_1;\n    }"),
                "    switch (n) {\n    case 1:\n    case 2:\n        work();\n    }");
});

// ---- what the passes learn of the function --------------------------------------------------------------------------

CB_TEST("exact.draft.shapes.facts_are_recorded_where_the_caller_holds_them", [] {
    FunctionFacts facts;
    // a byte widened with zeros: the place is an unsigned char
    CB_CHECK_EQ(zero_extends("    al = g_0010;\n    ah = 0;", facts), "    ax = g_0010;");
    CB_CHECK(facts.unsigned_globals.contains("g_0010"));
    CB_CHECK_EQ(zero_extends("    al = p->f6;\n    ah = 0;", facts), "    ax = p->f6;");
    CB_CHECK(facts.unsigned_sites.contains(dt::Site{"p", 6}));
    // a pointer stepped through by words
    CB_CHECK_EQ(pointer_index("    bx = n;\n    bx = (bx << 1) + g_0020;\n    ax = *bx;", facts), "    ax = *(g_0020 + n);");
    CB_CHECK(facts.word_pointers.contains("g_0020"));
    // a long's two words
    const auto one = pair_long("p->f2", "p->f4", facts);
    CB_CHECK(one.has_value() && *one == "*(long *)&p->f2");
    CB_CHECK(facts.longs.fields.contains(2));
    CB_CHECK(!pair_long("p->f2", "q->f4", facts).has_value());
    CB_CHECK_EQ(*pair_long("g_0010", "g_0012", facts), "g_0010");
    CB_CHECK(facts.longs.globals.contains("g_0010"));
    CB_CHECK_EQ(*pair_long("x", "(0 - (x >> 15))", facts), "(long)x");
    // the facts of one function are not the next one's
    CB_CHECK(FunctionFacts().unsigned_globals.empty());
    // the ranges a pass reads
    const std::string chain = "    ax = n;\n    if ((ax >= 2)) {\n        if ((ax > 3)) {\n            f();\n        } else {\n            g();\n        }\n    } else {\n        f();\n    }";
    FunctionFacts none;
    CB_CHECK_EQ(range_switches(chain, none), "    switch (n) {\n    default:\n        f();\n        break;\n    case 2:\n    case 3:\n        g();\n        break;\n    }");
});

// ---- bounds: a draft the passes cannot read gives the text back ------------------------------------------------------

CB_TEST("exact.draft.shapes.damaged_drafts_are_left_alone", [] {
    FunctionFacts facts;
    for (const auto& [name, pass] : passes()) {
        for (const char* t : {"", "\n", "}", "{", "    return ax;", "    goto L_1;", "L_1:", "    if ((a)) {", "} else {", "    switch (x) {",
                              "    case 1:", "    for (;;) {", "    ax = 1;\n    ah = 0;"}) {
            const Result r = pass(t, facts);   // no exception, whatever the text
            (void)r;
        }
    }
    CB_CHECK_EQ(store_joins("    L_1:\n    x = ax;\n    goto L_1;"), "    L_1:\n    x = ax;\n    goto L_1;");   // (a goto on the first line)
    // the number of a case or a field too large to read is not read
    CB_CHECK_EQ(zero_extends("    al = p->f99999999999999999999;\n    ah = 0;", facts), "    ax = p->f99999999999999999999;");
    CB_CHECK(!facts.unsigned_sites.contains(dt::Site{"p", 0}));
});

CB_TEST("exact.draft.shapes.long_drafts_do_not_hang", [] {
    // a draft of many lines: every pass finishes (the regular expressions run under a step budget)
    std::string draft = "int f(x)\nint x;\n{\n";
    for (int i = 0; i < 300; ++i)
        draft += std::format("    if ((a{} < {})) {{\n        ax = {};\n    }} else {{\n        ax = {};\n    }}\n    x{} = ax;\n", i, i, i, i + 1, i);
    draft += "}";
    FunctionFacts facts;
    std::size_t changed = 0;
    for (const auto& [name, pass] : passes()) {
        const Result r = pass(draft, facts);
        if (r.value != Json(nullptr) && !(r.is_text && r.text == draft)) ++changed;
    }
    CB_CHECK(changed > 0);
});
