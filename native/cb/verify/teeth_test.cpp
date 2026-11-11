// Tests of teeth: the five breaks on C text (each case reviewed by hand against what the break is meant to do),
// reach, the round loop, a whole run over a fake lane, and the command.

#include "cb/verify/teeth.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::teeth;

namespace {

const std::string kHead = "void f()\n{\n";
const std::string kTail = "\n}\n";

// The statements `stmt` as the break of that kind makes them, or nullopt if the break does not apply.
std::optional<std::string> break_of(BreakKind kind, const std::string& stmt) {
    for (const TextBreak& b : breaks(kHead + stmt + kTail)) {
        if (b.kind != kind)
            continue;
        CB_CHECK(b.text.starts_with(kHead) && b.text.ends_with(kTail));
        return b.text.substr(kHead.size(), b.text.size() - kHead.size() - kTail.size());
    }
    return std::nullopt;
}

using Opt = std::optional<std::string>;

} // namespace

CB_TEST("verify.teeth.whole_function", [] {
    const std::string t = "int f(int a, int b)\n{\n    int x;\n    char buf[8];\n\n    x = a + 4;\n    if (a == b) {\n"
                          "        *p = 0x1f;\n    }\n    g(a,b);\n    return x;\n}\n";
    std::vector<TextBreak> got = breaks(t);
    CB_CHECK_EQ(got.size(), std::size_t{5}); // all five apply, in the order they are tried
    // the declarations at the top are not touched (the 8 in `buf[8]` stays), the statements are
    CB_CHECK(got[0].kind == BreakKind::Constant);
    CB_CHECK_EQ(got[0].text, text::replace_first(t, "a + 4;", "a + 5;"));
    CB_CHECK(got[1].kind == BreakKind::Comparison);
    CB_CHECK_EQ(got[1].text, text::replace_first(t, "a == b", "a != b"));
    CB_CHECK(got[2].kind == BreakKind::Operator);
    CB_CHECK_EQ(got[2].text, text::replace_first(t, "a + 4", "a - 4"));
    CB_CHECK(got[3].kind == BreakKind::Store);
    CB_CHECK_EQ(got[3].text, text::replace_first(t, "        *p = 0x1f;\n", ""));
    CB_CHECK(got[4].kind == BreakKind::Arguments);
    CB_CHECK_EQ(got[4].text, text::replace_first(t, "g(a,b)", "g(b,a)"));
    CB_CHECK_EQ(std::string(word(BreakKind::Arguments)), "arguments");
});

CB_TEST("verify.teeth.nothing_to_break", [] {
    CB_CHECK(breaks("int f(void)\n{\n    return x;\n}\n").empty());
    CB_CHECK(breaks("no braces at all").empty());
    CB_CHECK(breaks("").empty());
    CB_CHECK(breaks("int f(void)\n{\n    int x;\n    char buf[8];\n}\n").empty()); // only declarations
});

CB_TEST("verify.teeth.constant", [] {
    auto c = [](const std::string& s) { return break_of(BreakKind::Constant, s); };
    CB_CHECK(c("    x = 9;") == Opt("    x = 10;"));
    CB_CHECK(c("    x = 0xff;") == Opt("    x = 0x100;"));
    CB_CHECK(c("    x = 0x0F;") == Opt("    x = 0x10;")); // leading zeros do not count; hex comes back lowercase
    CB_CHECK(c("    x = 007;") == Opt("    x = 8;"));
    CB_CHECK(c("    x = 99999999999999999999;") == Opt("    x = 100000000000000000000;")); // past 64 bits
    CB_CHECK(c("    x = 0xFFFFFFFFFFFFFFFFFFFF;") == Opt("    x = 0x100000000000000000000;"));
    CB_CHECK(c("    x = 2 + 3;") == Opt("    x = 3 + 3;")); // the first one
    // not part of a name, a member, an index or a float
    CB_CHECK(!c("    v1 = a[3] + y.5 + 1.5 + z2;"));
    CB_CHECK(!c("    x = 12ab;"));
    CB_CHECK(!c("    x = 0XFF;")); // only a lowercase 0x is a hexadecimal
    CB_CHECK(!c("    x = 0xg;"));
    // not inside a string or a comment
    CB_CHECK(c("    puts(\"7\"); x = 2;") == Opt("    puts(\"7\"); x = 3;"));
    CB_CHECK(c("    /* 7 */ x = 2;") == Opt("    /* 7 */ x = 3;"));
    CB_CHECK(c("    c = '4'; x = 5;") == Opt("    c = '4'; x = 6;"));
    // an apostrophe with no partner on its line is not a character constant
    CB_CHECK(c("    x = 1; /* it's */") == Opt("    x = 2; /* it's */"));
});

CB_TEST("verify.teeth.comparison", [] {
    auto c = [](const std::string& s) { return break_of(BreakKind::Comparison, s); };
    CB_CHECK(c("    if (a == b) y = 1;") == Opt("    if (a != b) y = 1;"));
    CB_CHECK(c("    if (a != b) y = 1;") == Opt("    if (a == b) y = 1;"));
    CB_CHECK(c("    if (a < b) y = 1;") == Opt("    if (a >= b) y = 1;"));
    CB_CHECK(c("    if (a > b) y = 1;") == Opt("    if (a <= b) y = 1;"));
    CB_CHECK(c("    if (a < b && c == d) y = 1;") == Opt("    if (a < b && c != d) y = 1;")); // equality comes first
    CB_CHECK(!c("    if (a <= b) y = 1;")); // (not touched: only spaced < and > are)
    CB_CHECK(!c("    if (a >= b) y = 1;"));
    CB_CHECK(!c("    x = a << 2;"));
    CB_CHECK(!c("    x = a >> 2;"));
    CB_CHECK(!c("    x = p -> q;"));
    CB_CHECK(!c("    x === y;"));
    CB_CHECK(!c("    puts(\"a == b\");"));
    CB_CHECK(c("    x = a<b;") == std::nullopt); // no spaces: not touched
});

CB_TEST("verify.teeth.operator", [] {
    auto c = [](const std::string& s) { return break_of(BreakKind::Operator, s); };
    CB_CHECK(c("    x = a + 4;") == Opt("    x = a - 4;"));
    CB_CHECK(c("    x = f(a) + *p;") == Opt("    x = f(a) - *p;"));
    CB_CHECK(c("    x = a[1] + (b);") == Opt("    x = a[1] - (b);"));
    CB_CHECK(!c("    x = a +b;"));
    CB_CHECK(!c("    x = a+ b;"));
    CB_CHECK(!c("    x = a + -b;"));
    CB_CHECK(!c("    x = \"a + b\";"));
    CB_CHECK(!c("    x += 1;"));
});

CB_TEST("verify.teeth.store", [] {
    auto c = [](const std::string& s) { return break_of(BreakKind::Store, s); };
    CB_CHECK(c("    a = 1;\n    *p = 2;\n    b = 3;") == Opt("    a = 1;\n    b = 3;"));
    CB_CHECK(c("    *p = f(x);\n    b = 3;") == Opt("    b = 3;"));
    CB_CHECK(c("    *(int *)p = v;   \n    b = 3;") == Opt("    b = 3;"));
    CB_CHECK(!c("    *p == 1;"));
    CB_CHECK(!c("    *p = a == b;"));
    CB_CHECK(!c("    p->x = 1;"));
    CB_CHECK(!c("    *p = 1; y = 2;")); // more after the statement on the line
    CB_CHECK(!c("    /* *p = 1; */"));
    CB_CHECK(!c("    x = *p;"));
});

CB_TEST("verify.teeth.arguments", [] {
    auto c = [](const std::string& s) { return break_of(BreakKind::Arguments, s); };
    CB_CHECK(c("    f(a,b);") == Opt("    f(b,a);"));
    CB_CHECK(c("    f(a,0x10,c);") == Opt("    f(0x10,a,c);"));
    CB_CHECK(c("    f(1,2);") == Opt("    f(2,1);"));
    CB_CHECK(c("    x = h(p,q) + 1;") == Opt("    x = h(q,p) + 1;"));
    CB_CHECK(!c("    f(a,a);"));        // two alike: nothing changes
    CB_CHECK(!c("    if(a,b) x = 1;")); // a keyword is not a call
    CB_CHECK(!c("    return(a,b);"));
    CB_CHECK(!c("    x = sizeof(a,b);"));
    CB_CHECK(!c("    f(a, b);")); // spaced: not the decompiler's layout
    CB_CHECK(!c("    f(g(x),y);"));
    CB_CHECK(!c("    f(a,b+1);"));
    CB_CHECK(!c("    puts(\"f(a,b)\");"));
    // the first call that fits decides, even if it is a keyword: nothing after it is tried
    CB_CHECK(!c("    while(a,b) f(c,d);"));
    CB_CHECK(c("    f(g(x),y); h(1,2);") == Opt("    f(g(x),y); h(2,1);"));
});

CB_TEST("verify.teeth.changed", [] {
    CB_CHECK_EQ(changed("a\nb\nc", "a\nB\nc"), "b  ->  B");
    CB_CHECK_EQ(changed("  x = 1;  \ny", "  x = 2;\ny"), "x = 1;  ->  x = 2;"); // lines are trimmed
    CB_CHECK_EQ(changed("a\nb", "a"), "b  ->  (removed)");
    CB_CHECK_EQ(changed("a\nb\nc", "a\nc"), "b  ->  (removed)");
    const std::string longer(300, 'z');
    CB_CHECK_EQ(changed("a", longer).size(), std::size_t{200});
});

CB_TEST("verify.teeth.reach", [] {
    // a calls b, b calls c (and itself), d stands alone, e mentions an a that is only part of a name
    std::vector<NamedText> fs = {
        {"a", "x = b(1);"}, {"b", "c(); b();"}, {"c", "return 0;"}, {"d", "y = 2;"}, {"e", "aa = a_b;"}};
    auto r = reach(fs);
    CB_CHECK_EQ(r.size(), std::size_t{5});
    CB_CHECK((r[0] == std::vector<std::uint32_t>{0, 1, 2}));
    CB_CHECK((r[1] == std::vector<std::uint32_t>{1, 2}));
    CB_CHECK((r[2] == std::vector<std::uint32_t>{2}));
    CB_CHECK((r[3] == std::vector<std::uint32_t>{3}));
    CB_CHECK((r[4] == std::vector<std::uint32_t>{4}));
    // a cycle reaches all of itself
    auto cyc = reach(std::vector<NamedText>{{"p", "q();"}, {"q", "p();"}});
    CB_CHECK((cyc[0] == std::vector<std::uint32_t>{0, 1}) && (cyc[1] == std::vector<std::uint32_t>{0, 1}));
    CB_CHECK(reach({}).empty());
});

CB_TEST("verify.teeth.plant_loop", [] {
    int next = 0;
    std::vector<std::string> log;
    auto next_round = [&]() -> std::optional<Round> {
        if (next == 2)
            return std::nullopt;
        ++next;
        log.push_back("next");
        return Round{{"f" + std::to_string(next), "constant", "text"}, {"g" + std::to_string(next), "store", "text"}};
    };
    auto judge = [&](const Round& r) {
        log.push_back("judge");
        std::vector<int> said;
        for (const Attempt& a : r)
            said.push_back(a.name[0] == 'f' ? 1 : 0);
        return said;
    };
    auto outcome = [](const int& s) { return s == 1 ? Outcome::Caught : Outcome::Missed; };
    std::vector<std::string> told;
    plant<int>(next_round, judge, outcome, [&](std::span<const Planted<int>> p) {
        log.push_back("each");
        for (const auto& x : p)
            told.push_back(x.attempt->name + (x.outcome == Outcome::Caught ? ":caught" : ":missed"));
    });
    CB_CHECK((told == std::vector<std::string>{"f1:caught", "g1:missed", "f2:caught", "g2:missed"}));
    // the next round is asked for only after `each` has returned (the third ask finds the end)
    CB_CHECK((log == std::vector<std::string>{"next", "judge", "each", "next", "judge", "each"}));
});

namespace {

// A module of four functions and a judge that answers from a table.
class FakeLane : public Lane {
public:
    FakeLane() {
        functions_ = {
            {"f1", "int f1()\n{\n    x = f2() + 4;\n}\n", Class::Verified}, // reaches f2
            {"f2", "int f2()\n{\n    return 8 + 2;\n}\n", Class::Verified},
            {"f3", "int f3()\n{\n    y = 1;\n}\n", Class::Weak},
            {"f4", "int f4()\n{\n    return z;\n}\n", Class::Verified}, // nothing to break
            {"f5", "int f5()\n{\n    y = 7;\n}\n", std::nullopt},       // had not passed
        };
    }
    std::string kind() const override { return "pe"; }
    std::string module_name() const override { return "fake.dll"; }
    const std::vector<Function>& functions() const override { return functions_; }
    std::vector<Said> judge(std::span<const Replacement> broken) override {
        std::vector<std::string> names;
        std::vector<Said> out;
        for (const Replacement& r : broken) {
            names.push_back(functions_[r.function].name);
            texts.push_back(r.text);
            if (r.function == 0)
                out.push_back({Verdict::Divergent, "ret-differs", {}});
            else if (r.function == 2)
                out.push_back({Verdict::Equivalent, "", {"truncated", "layout-differs"}});
            else
                out.push_back({std::nullopt, "C2065", {}});
        }
        rounds.push_back(names);
        return out;
    }
    std::vector<std::vector<std::string>> rounds;
    std::vector<std::string> texts;

private:
    std::vector<Function> functions_;
};

} // namespace

CB_TEST("verify.teeth.run", [] {
    FakeLane lane;
    std::vector<std::string> log;
    Report r = run(lane, 3, [&](std::string_view l) { log.emplace_back(l); });
    // round 1: f1 (which reaches f2, so f2 waits) and f3; round 2: f2; a third round has nothing left to break
    CB_CHECK_EQ(lane.rounds.size(), std::size_t{2});
    CB_CHECK((lane.rounds[0] == std::vector<std::string>{"f1", "f3"}));
    CB_CHECK((lane.rounds[1] == std::vector<std::string>{"f2"}));
    // the kinds of break in turn: f1 first of its two (constant), f3 its only one, f2 in round 2 the second (operator)
    CB_CHECK_EQ(lane.texts[0], "int f1()\n{\n    x = f2() + 5;\n}\n");
    CB_CHECK_EQ(lane.texts[1], "int f3()\n{\n    y = 2;\n}\n");
    CB_CHECK_EQ(lane.texts[2], "int f2()\n{\n    return 8 - 2;\n}\n");

    CB_CHECK_EQ(r.module, "fake.dll");
    CB_CHECK_EQ(r.kind, "pe");
    CB_CHECK_EQ(r.rounds, 3);
    CB_CHECK_EQ(r.results.size(), std::size_t{3});
    CB_CHECK_EQ(r.results[0].name, "f1");
    CB_CHECK(r.results[0].now == Fate::Caught && r.results[0].was == Class::Verified);
    CB_CHECK_EQ(r.results[0].made, "constant");
    CB_CHECK_EQ(r.results[0].reason, "ret-differs");
    CB_CHECK_EQ(r.results[0].change, "x = f2() + 4;  ->  x = f2() + 5;");
    CB_CHECK(r.results[1].now == Fate::Survived && r.results[1].was == Class::Weak);
    CB_CHECK_EQ(r.results[1].limits, "truncated,layout-differs");
    CB_CHECK(r.results[2].now == Fate::DoesNotCompile);
    CB_CHECK_EQ(r.results[2].reason, "C2065");
    CB_CHECK_EQ(r.results[2].made, "operator");
    CB_CHECK(r.results[2].change.empty());

    // what was caught, by what the judge had said and by kind of break (sorted by name; f2 never ran: not counted)
    CB_CHECK_EQ(r.caught.size(), std::size_t{3});
    CB_CHECK_EQ(r.caught[0].name, "by break: constant");
    CB_CHECK(r.caught[0].caught == 1 && r.caught[0].of == 2);
    CB_CHECK_EQ(r.caught[1].name, "verified");
    CB_CHECK(r.caught[1].caught == 1 && r.caught[1].of == 1);
    CB_CHECK_EQ(r.caught[2].name, "weak");
    CB_CHECK(r.caught[2].caught == 0 && r.caught[2].of == 1);

    CB_CHECK((log == std::vector<std::string>{"  round 1: 2 functions broken; so far 1 of 2 caught",
                                              "  round 2: 1 functions broken; so far 1 of 2 caught"}));

    // rounds limits the loop; none logs nothing when there is no log
    FakeLane one;
    Report r1 = run(one, 1);
    CB_CHECK_EQ(one.rounds.size(), std::size_t{1});
    CB_CHECK_EQ(r1.results.size(), std::size_t{2});
    FakeLane none;
    CB_CHECK(run(none, 0).results.empty());
});

CB_TEST("verify.teeth.report_json", [] {
    FakeLane lane;
    Report r = run(lane, 3);
    Json j = json::parse(report_json(r));
    CB_CHECK_EQ(j["module"].get<std::string>(), "fake.dll");
    CB_CHECK_EQ(j["kind"].get<std::string>(), "pe");
    CB_CHECK_EQ(j["rounds"].get<int>(), 3);
    CB_CHECK_EQ(j["results"].size(), std::size_t{3});
    CB_CHECK_EQ(j["results"][0]["now"].get<std::string>(), "caught");
    CB_CHECK_EQ(j["results"][0]["was"].get<std::string>(), "verified");
    CB_CHECK_EQ(j["results"][1]["limits"].get<std::string>(), "truncated,layout-differs");
    CB_CHECK_EQ(j["results"][2]["now"].get<std::string>(), "does not compile");
    CB_CHECK(!j["results"][2].contains("change")); // it never ran: nothing it changed to, no limits
    CB_CHECK(!j["results"][2].contains("limits"));
    CB_CHECK_EQ(j["caught"]["verified"]["caught"].get<int>(), 1);
    CB_CHECK_EQ(j["caught"]["by break: constant"]["of"].get<int>(), 2);
});

CB_TEST("verify.teeth.command", [] {
    fs::ScopedTempDir dir("teeth-test");
    Services services;
    services.find_module = [](std::string_view name) -> std::optional<std::filesystem::path> {
        if (name == "fake.dll")
            return std::filesystem::path("C:/corpus/fake.dll");
        return std::nullopt;
    };
    services.open = [](const std::filesystem::path&) -> std::unique_ptr<Lane> { return std::make_unique<FakeLane>(); };
    services.report_dir = dir.path() / "reports";
    cli::Cli cli("codebreaker", {command(services)});

    testing::CapturedStd cap;
    CB_CHECK_EQ(cli.run({"teeth", "fake.dll"}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("  round 1: 2 functions broken; so far 1 of 2 caught\n") != std::string::npos);
    CB_CHECK(out.find("  by break: constant                 caught 1 of 2  (50%)\n") != std::string::npos);
    CB_CHECK(out.find("  it had called VERIFIED             caught 1 of 1  (100%)\n") != std::string::npos);
    CB_CHECK(out.find("  it had called VERIFIED (weak)      caught 0 of 1  (0%)\n") != std::string::npos);
    CB_CHECK(out.find("  not counted: 1 (the break did not compile, or could not be judged)\n") != std::string::npos);
    const std::filesystem::path report = dir.path() / "reports" / "fake.dll.json"; // the folder is made
    CB_CHECK(std::filesystem::exists(report));
    CB_CHECK(out.find("written to " + fs::to_text(report)) != std::string::npos);
    CB_CHECK_EQ(json::load(report)["results"].size(), std::size_t{3});

    // --json FILE and --rounds
    const std::filesystem::path other = dir.path() / "x.json";
    CB_CHECK_EQ(cli.run({"teeth", "fake.dll", "--rounds", "1", "--json", fs::to_text(other)}), 0);
    CB_CHECK_EQ(json::load(other)["results"].size(), std::size_t{2});
    CB_CHECK_EQ(json::load(other)["rounds"].get<int>(), 1);

    // the strong verdict's survivors are listed: none here (f1 was caught), so no "survived:" line
    CB_CHECK(out.find("survived:") == std::string::npos);

    CB_CHECK_THROWS_AS(NotFoundError, cli.run({"teeth", "nope.dll"}));
    CB_CHECK_THROWS_AS(UsageError, cli.run({"teeth", "fake.dll", "--rounds", "many"}));
    CB_CHECK_EQ(cli.run({"teeth"}), kExitUsage); // a module is needed
});
