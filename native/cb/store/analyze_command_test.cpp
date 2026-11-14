// Tests of `codebreaker analyze` as a complete command (store/analyze_command): the report over the real corpus services, in a temporary state folder
// (CB_STATE), on the real Windows 95 builds of compare/testdata/versions: a PE program and a 16-bit printer driver.
//
// CB_ANALYZE_COMMAND_PROOF="<module path>" runs the command on that module (store in the temporary state) and prints its answer and how long it took.
#include "cb/store/analyze_command.hpp"

#include <chrono>
#include <iostream>

#include "cb/analyze/facts.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/function_store.hpp"

namespace stdfs = std::filesystem;
using namespace cb;

namespace {

stdfs::path data(const std::string& name) { return testing::data_path("compare/testdata/versions/win95/" + name); }

struct Surrounding {
    fs::ScopedTempDir state{"analyze-command-state"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(state.path())};
    config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli cli{"codebreaker", {store::analyze_command::command({.config = config, .corpus = nullptr})}};
};

}  // namespace

CB_TEST("store.analyze_command.a_module_is_analysed_and_its_facts_are_filed", [] {
    Surrounding s;
    testing::CapturedStd cap;
    CB_CHECK_EQ(s.cli.run({"analyze", fs::to_text(data("tracert.exe")), "--what", "call_graph,conventions,strings"}), 0);
    CB_CHECK(cap.out().starts_with("tracert.exe: "));
    CB_CHECK(stdfs::exists(s.config.state() / "analyze" / "tracert.exe.txt"));
    // the module is in the function store and its facts are in the fact table beside it
    engine::FunctionStore store(s.config.state("functions.sqlite"), s.config.state("db"), true);
    const auto overview = store.overview();
    CB_CHECK_EQ(overview.size(), std::size_t{1});
    CB_CHECK_EQ(overview.front().name, "tracert.exe");
    analyze::facts::FactStore facts = analyze::facts::FactStore::open(analyze::facts::default_path(s.config));
    const analyze::facts::Tally tally = facts.tally(overview.front().sha);
    CB_CHECK(tally.contains("call_graph"));
    CB_CHECK(tally.contains("conventions"));
});

CB_TEST("store.analyze_command.a_16_bit_module_through_the_decompilers_description", [] {
    Surrounding s;
    testing::CapturedStd cap;
    CB_CHECK_EQ(s.cli.run({"analyze", fs::to_text(data("thinkjet.drv")), "--what", "call_graph,conventions", "--no-store", "--summary"}), 0);
    CB_CHECK(cap.out().find("call_graph") != std::string::npos);
    CB_CHECK(cap.out().find("conventions") != std::string::npos);
});

CB_TEST("store.analyze_command.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_ANALYZE_COMMAND_PROOF");
    if (!proof || proof->empty()) return;
    Surrounding s;
    const auto started = std::chrono::steady_clock::now();
    // CB_ANALYZE_COMMAND_OUT: write the whole report (every fact listed) there, for inspecting its text; CB_ANALYZE_COMMAND_WHAT: which analyses
    std::vector<std::string> words{"analyze", *proof, "--no-store"};
    if (const auto report = fs::get_env("CB_ANALYZE_COMMAND_OUT"); report && !report->empty()) {
        words.insert(words.end(), {"--facts", "--out", *report});
    } else {
        words.emplace_back("--summary");
    }
    if (const auto what = fs::get_env("CB_ANALYZE_COMMAND_WHAT"); what && !what->empty()) words.insert(words.end(), {"--what", *what});
    if (const auto run = fs::get_env("CB_ANALYZE_COMMAND_RUN"); run && !run->empty()) words.insert(words.end(), {"--run", *run});
    const int code = s.cli.run(words);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
