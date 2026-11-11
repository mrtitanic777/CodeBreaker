// Tests of measure: the tally of unverified causes and compile failures, and the command around it.

#include "cb/verify/measure.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::measure;

namespace {

Row row(Verdict v, std::string reason) {
    return Row{v, std::move(reason)};
}

} // namespace

CB_TEST("verify.measure.summarise_orders_by_count_then_first_seen", [] {
    std::vector<Row> rows = {
        row(Verdict::Divergent, "writes-differ"), row(Verdict::Divergent, "ret-differs"),
        row(Verdict::Divergent, "ret-differs"),   row(Verdict::Equivalent, "ignored"),
        row(Verdict::Divergent, "calls-differ"),  row(Verdict::Divergent, "writes-differ"),
        row(Verdict::Divergent, "ret-differs"),
    };
    Summary s = summarise(rows);
    CB_CHECK_EQ(s.unverified.size(), std::size_t{3});
    CB_CHECK_EQ(s.unverified[0].reason, "ret-differs"); // 3
    CB_CHECK_EQ(s.unverified[0].count, std::size_t{3});
    CB_CHECK_EQ(s.unverified[1].reason, "writes-differ"); // 2
    CB_CHECK_EQ(s.unverified[2].reason, "calls-differ");  // 1
    CB_CHECK(s.no_compile.empty());
    CB_CHECK_EQ(render(s), "unverified by cause: ret-differs 3, writes-differ 2, calls-differ 1\n");
});

CB_TEST("verify.measure.ties_keep_the_order_first_seen", [] {
    std::vector<Row> rows = {row(Verdict::Divergent, "b"), row(Verdict::Divergent, "a"), row(Verdict::Divergent, "c")};
    Summary s = summarise(rows);
    CB_CHECK_EQ(render(s), "unverified by cause: b 1, a 1, c 1\n");
});

CB_TEST("verify.measure.no_compile_causes_are_cut_and_limited", [] {
    const std::string longer(100, 'x');
    std::vector<Row> rows = {
        row(Verdict::NoCompile, longer + "A"),
        row(Verdict::NoCompile, longer + "B"), // alike in their first 60
        row(Verdict::NoCompile, "error C2065: undeclared"),
        row(Verdict::NoCompile, "e3"),
        row(Verdict::NoCompile, "e4"),
        row(Verdict::NoCompile, "e5"), // a fifth cause is not shown
    };
    Summary s = summarise(rows);
    CB_CHECK_EQ(s.no_compile.size(), std::size_t{4});
    CB_CHECK_EQ(s.no_compile[0].reason, std::string(60, 'x'));
    CB_CHECK_EQ(s.no_compile[0].count, std::size_t{2});
    CB_CHECK_EQ(render(s),
                "does not compile: " + std::string(60, 'x') + " (2); error C2065: undeclared (1); e3 (1); e4 (1)\n");
});

CB_TEST("verify.measure.cut_does_not_split_a_character", [] {
    // 59 ASCII bytes then a two-byte character straddling the cut at 60
    const std::string reason = std::string(59, 'a') + "\xC3\xA9 tail";
    Summary s = summarise(std::vector<Row>{row(Verdict::NoCompile, reason)});
    CB_CHECK_EQ(s.no_compile[0].reason, std::string(59, 'a'));
    CB_CHECK(text::is_valid_utf8(s.no_compile[0].reason));
});

CB_TEST("verify.measure.nothing_to_say", [] {
    std::vector<Row> rows = {row(Verdict::Equivalent, ""), row(Verdict::Consistent, ""),
                             row(Verdict::Inconclusive, "")};
    CB_CHECK(render(summarise(rows)).empty());
    CB_CHECK(render(summarise({})).empty());
    // a divergent row with no reason is still counted
    CB_CHECK_EQ(render(summarise(std::vector<Row>{Row{Verdict::Divergent, ""}})),
                "unverified by cause: (no reason given) 1\n");
    // a row whose verdict was not a verdict counts for nothing
    CB_CHECK(render(summarise(std::vector<Row>{Row{std::nullopt, "x"}})).empty());
});

CB_TEST("verify.measure.command", [] {
    std::filesystem::path judged;
    Engine engine_seen = Engine::Default;
    Services services;
    services.find_module = [](std::string_view name) -> std::optional<std::filesystem::path> {
        if (name == "lz32.dll")
            return std::filesystem::path("C:/corpus/lz32.dll");
        return std::nullopt;
    };
    services.judge = [&](const std::filesystem::path& p, Engine e) {
        judged = p;
        engine_seen = e;
        return std::vector<Row>{row(Verdict::Divergent, "ret-differs"), row(Verdict::NoCompile, "C2065")};
    };
    cli::Command c = command(services);
    cli::Cli cli("codebreaker", {c});

    testing::CapturedStd cap;
    CB_CHECK_EQ(cli.run({"measure", "lz32.dll"}), 0);
    CB_CHECK_EQ(cap.out(), "unverified by cause: ret-differs 1\ndoes not compile: C2065 (1)\n");
    CB_CHECK(engine_seen == Engine::Default);
    CB_CHECK_EQ(judged.generic_string(), "C:/corpus/lz32.dll");
    CB_CHECK_EQ(cli.run({"measure", "lz32.dll", "--engine"}), 0);
    CB_CHECK(engine_seen == Engine::On);
    CB_CHECK_EQ(cli.run({"measure", "--no-engine", "lz32.dll"}), 0);
    CB_CHECK(engine_seen == Engine::Off);
    CB_CHECK_EQ(cli.run({"measure", "--engine", "--no-engine", "lz32.dll"}), 0);
    CB_CHECK(engine_seen == Engine::Off);          // both given: the old path wins
    CB_CHECK_EQ(cli.run({"measure"}), kExitUsage); // a module is needed
    CB_CHECK_THROWS_AS(NotFoundError, cli.run({"measure", "nope.dll"}));
});
