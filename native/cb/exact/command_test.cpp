// Tests of `codebreaker exact` as a complete command (exact/command): the table of stages made from the settings alone, in a temporary state
// folder (CB_STATE). What a stage says is the stage's own tests'; here is that the command reaches every one of them and answers as run.hpp says.
//
// CB_EXACT_PROOF="<word>;<word>;..." runs the command with those words (the project, the stage, its options) and prints what it said and how long it
// took, for comparing with the translated Python's `exact` on the same words.
#include "cb/exact/command.hpp"

#include <chrono>
#include <iostream>

#include "cb/exact/real_projects.hpp"
#include "cb/exact/run.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

struct Surrounding {
    fs::ScopedTempDir state{"exact-command-state"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(state.path())};
    command::Environment environment{.config = config::Config::load(config::current().main_checkout())};
};

std::vector<std::string> words(std::initializer_list<const char*> list) { return {list.begin(), list.end()}; }

}  // namespace

CB_TEST("exact.command.the_commands_own_answers", [] {
    Surrounding s;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command::run(std::vector<std::string>{}, s.environment), 2);
        CB_CHECK(cap.out().find("codebreaker exact PROJECT STAGE [options]") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command::run(words({"--help"}), s.environment), 0);
        CB_CHECK_EQ(cap.out(), run::usage() + "\n");
    }
    // --toolchains: the profiles of the checkout, as the Python listed them
    testing::CapturedStd cap;
    CB_CHECK_EQ(command::run(words({"--toolchains"}), s.environment), test::golden().at("run_toolchains").at(1).get<int>());
    test::check(cap.out() == test::golden().at("run_toolchains").at(0).get<std::string>(), "--toolchains");
    CB_CHECK_THROWS_AS(UsageError, (void)command::run(words({"--compile", "msc300"}), s.environment));
});

CB_TEST("exact.command.every_stage_is_in_the_table", [] {
    test::Real real;
    if (test::skipped(real, "command stages")) return;
    Surrounding s;
    testing::CapturedStd cap;
    const std::string p0 = fs::to_text(test::research_root() / "msc-3.0/recon/p0");
    CB_CHECK_EQ(command::run(std::vector<std::string>{p0, "nope"}, s.environment), 2);
    CB_CHECK_EQ(cap.out(), "no stage nope (stages: ledger, library, carve, modules, link, bss, data, repair, names, identify, bench, seed, readable, draft)\n");
});

CB_TEST("exact.command.a_stage_through_the_command_is_the_stage", [] {
    test::Real real;
    if (test::skipped(real, "command carve")) return;
    Surrounding s;
    const Json& want = test::golden().at("carve").at("msc-3.0/recon/p0");
    testing::CapturedStd cap;
    CB_CHECK_EQ(command::run(std::vector<std::string>{fs::to_text(test::research_root() / "msc-3.0/recon/p0"), "carve", "-v"}, s.environment), 0);
    test::check(cap.out() == want.at("main").at(0).get<std::string>(), "the carve stage through the command");
});

CB_TEST("exact.command.the_draft_stage_without_its_model_says_so", [] {
    test::Real real;
    if (test::skipped(real, "command draft")) return;
    Surrounding s;
    CB_CHECK_THROWS_AS(UsageError, (void)command::run(std::vector<std::string>{fs::to_text(test::research_root() / "msc-3.0/recon/p0"), "draft"}, s.environment));
});

CB_TEST("exact.command.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_EXACT_PROOF");
    if (!proof || proof->empty()) return;
    std::vector<std::string> args;
    for (const std::string_view piece : text::split(*proof, ';')) args.emplace_back(piece);
    Surrounding s;
    const auto started = std::chrono::steady_clock::now();
    const int code = command::run(args, s.environment);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
