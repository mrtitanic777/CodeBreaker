// Tests of `codebreaker reach` as a complete command (coverage/reach_command): the services made from the settings alone, on real programs of the
// compare/testdata/versions folder (a PE, two NE drivers and a DOS driver), in a temporary state folder (CB_STATE).
//
// CB_REACH_PROOF="<name>;<file or folder>[;<file or folder>...]" runs reach over those inputs (a run named <name>) and prints what the command says and
// how long it took, for comparing runs of `reach` on the same inputs (its results file is <state>/reach/<name>.json).
#include "coverage/reach_command.hpp"

#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;
using namespace cb;

namespace {

stdfs::path data(const std::string& name) { return testing::data_path("compare/testdata/versions/" + name); }

struct Surrounding {
    fs::ScopedTempDir state{"reach-command-state"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(state.path())};
    config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli cli{"codebreaker", {coverage::reach_command::command({.config = config, .corpus = nullptr})}};
};

}  // namespace

CB_TEST("coverage.reach_command.what_a_folder_of_programs_comes_to", [] {
    Surrounding s;
    testing::CapturedStd cap;
    CB_CHECK_EQ(s.cli.run({"reach", fs::to_text(data("win98")), "--name", "t"}), 0);
    const std::string report = cap.out();
    CB_CHECK(report.find("# Reach") != std::string::npos);
    CB_CHECK(report.find("Windows") != std::string::npos);   // (the platform of a Windows driver and a Windows program)
    // the results and the report are kept under the state folder, by the run's name
    CB_CHECK(stdfs::exists(s.config.state() / "reach" / "t.json"));
    CB_CHECK(report.find(text::trim(fs::read_text(s.config.state() / "reach" / "t.md"))) != std::string::npos);
    const Json results = json::load(s.config.state() / "reach" / "t.json");
    CB_CHECK_EQ(results.at("programs").size(), std::size_t{3});
});

CB_TEST("coverage.reach_command.the_last_report_again", [] {
    Surrounding s;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(s.cli.run({"reach", fs::to_text(data("win98/tracert.exe")), "--name", "again"}), 0);
    }
    testing::CapturedStd cap;
    CB_CHECK_EQ(s.cli.run({"reach", "--report", "again"}), 0);
    CB_CHECK(cap.out().find("# Reach") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, (void)s.cli.run({"reach"}));
});

CB_TEST("coverage.reach_command.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_REACH_PROOF");
    if (!proof || proof->empty()) return;
    std::vector<std::string> parts;
    for (const std::string_view piece : text::split(*proof, ';')) parts.emplace_back(piece);
    if (parts.size() < 2) return;
    Surrounding s;
    std::vector<std::string> args{"reach"};
    args.insert(args.end(), parts.begin() + 1, parts.end());
    args.push_back("--name");
    args.push_back(parts[0]);
    const auto started = std::chrono::steady_clock::now();
    const int code = s.cli.run(args);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
    std::cout << "results: " << (s.config.state() / "reach" / (parts[0] + ".json")) << "\n";
    if (const auto keep = fs::get_env("CB_REACH_PROOF_KEEP"); keep && !keep->empty()) {
        fs::make_dirs(fs::from_text(*keep));
        fs::copy_tree(s.config.state() / "reach", fs::from_text(*keep));
    }
    CB_CHECK_EQ(code, 0);
});
