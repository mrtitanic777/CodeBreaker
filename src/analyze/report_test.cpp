// Tests of `codebreaker analyze` (analyze/report) on real modules of compare/testdata/versions: a 32-bit one (TRACERT.EXE) and a 16-bit one
// (THINKJET.DRV). What each analysis finds is its own tests'; here is that the report runs them in order on what the ones before found, writes the
// text, the JSON and the starts, stores the facts that have rows, and says what failed or is not run.
//
// CB_ANALYZE_PROOF="<module path>" runs every analysis but observe on that module (no store) and prints the counts as JSON and the time.
#include "analyze/report.hpp"

#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

#include "analyze/facts.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;
using namespace cb;
namespace rp = cb::analyze::report;

namespace {

stdfs::path module(const char* name) { return testing::data_path(std::string("compare/testdata/versions/win95/") + name); }

struct Surrounding {
    fs::ScopedTempDir dir{"analyze-report"};
    rp::Environment environment;
    Surrounding() {
        environment.state = dir.path() / "state";
        environment.facts_file = dir.path() / "functions.sqlite";
        environment.retail = [](std::string_view name) -> std::optional<stdfs::path> {
            if (name == "tracert.exe" || name == "thinkjet.drv") return module(std::string(name).c_str());
            if (name == "vmouse.vxd") return testing::data_path("analyze/testdata/le/vmouse.vxd");
            return std::nullopt;
        };
        environment.add_module = [this](std::string_view sha, std::string_view name, const stdfs::path&, std::string_view kind) {
            added.push_back(std::string(sha) + " " + std::string(name) + " " + std::string(kind));
        };
    }
    std::vector<std::string> added;
};

std::int64_t count(const rp::Section& s, std::string_view name) {
    for (const auto& [k, v] : s.counts)
        if (k == name) return v;
    return -1;
}

}  // namespace

CB_TEST("analyze.report.the_names_of_the_analyses", [] {
    CB_CHECK_EQ(rp::defaults().size(), std::size_t{8});
    CB_CHECK(rp::from_name("call_graph") == rp::Analysis::CallGraph);
    CB_CHECK(rp::from_name("observe") == rp::Analysis::Observe);
    CB_CHECK(!rp::from_name("no_such").has_value());
    CB_CHECK_EQ(rp::names(), "call_graph, conventions, globals, strings, structures, classes, dispatch, resources, observe");
});

CB_TEST("analyze.report.a_32_bit_module", [] {
    Surrounding s;
    const rp::Result r = rp::run("tracert.exe", {}, s.environment);
    CB_CHECK_EQ(r.module, "tracert.exe");
    CB_CHECK_EQ(r.bits, 32);
    CB_CHECK(r.kind == cb::analyze::ImageKind::Pe);
    CB_CHECK_EQ(r.sections.size(), std::size_t{8});
    for (const rp::Section& section : r.sections) CB_CHECK(!section.failed);
    CB_CHECK_EQ(count(r.sections[0], "functions"), static_cast<std::int64_t>(r.functions));
    CB_CHECK_EQ(count(r.sections[1], "functions"), static_cast<std::int64_t>(r.functions));
    CB_CHECK(count(r.sections[0], "import_call_sites") > 0);   // a program that calls the system
    // the text
    const std::string text = rp::text(r, false);
    CB_CHECK(text.starts_with("tracert.exe  (32-bit Windows module, 32-bit)"));
    CB_CHECK(text.find("== call graph") != std::string::npos);
    CB_CHECK(text.find("what the decompiler's description could be told:") != std::string::npos);
    CB_CHECK(rp::text(r, true).size() > text.size());   // with the facts listed
    // the json
    const nlohmann::json doc = nlohmann::json::parse(rp::json(r));
    CB_CHECK_EQ(doc.at("module").get<std::string>(), "tracert.exe");
    CB_CHECK_EQ(doc.at("counts").at("call_graph").at("functions").get<std::int64_t>(), static_cast<std::int64_t>(r.functions));
    CB_CHECK(doc.at("seconds").contains("total"));
    // the facts of the analyses that have rows are in the fact table, filed under the module's hash
    CB_CHECK_EQ(s.added.size(), std::size_t{1});
    CB_CHECK(!r.sha.empty());
    analyze::facts::FactStore store = analyze::facts::FactStore::open(s.environment.facts_file);
    const analyze::facts::Tally tally = store.tally(r.sha);
    CB_CHECK(tally.contains("call_graph"));
    CB_CHECK(tally.contains("conventions"));
    CB_CHECK_EQ(tally.at("call_graph").at("calls"), static_cast<std::int64_t>(r.functions));
    // a second run replaces what the first stored
    (void)rp::run("tracert.exe", {.what = {rp::Analysis::CallGraph}}, s.environment);
    CB_CHECK_EQ(analyze::facts::FactStore::open(s.environment.facts_file).tally(r.sha).at("call_graph").at("calls"), static_cast<std::int64_t>(r.functions));
});

CB_TEST("analyze.report.a_16_bit_module", [] {
    Surrounding s;
    const rp::Result r = rp::run("thinkjet.drv", {.what = {rp::Analysis::CallGraph, rp::Analysis::Conventions, rp::Analysis::Dispatch}, .store = false}, s.environment);
    CB_CHECK_EQ(r.bits, 16);
    CB_CHECK(r.kind == cb::analyze::ImageKind::Ne);
    CB_CHECK_EQ(r.sections.size(), std::size_t{3});
    for (const rp::Section& section : r.sections) CB_CHECK(!section.failed);
    CB_CHECK(r.functions > 10);
    CB_CHECK(s.added.empty());   // (--no-store: nothing is filed)
    for (const std::string& line : r.starts) CB_CHECK(line.starts_with("F\t"));
});

CB_TEST("analyze.report.observe_runs_the_modules_functions", [] {
    Surrounding s;
    const rp::Result r = rp::run("tracert.exe", {.what = {rp::Analysis::Observe, rp::Analysis::CallGraph}, .store = false, .observed = 3}, s.environment);
    CB_CHECK_EQ(r.sections.size(), std::size_t{2});
    CB_CHECK(r.sections[0].analysis == rp::Analysis::CallGraph);   // (in the order of the list)
    CB_CHECK(!r.sections[0].failed);
    CB_CHECK(r.sections[1].analysis == rp::Analysis::Observe);
    CB_CHECK(!r.sections[1].failed);
    std::int64_t run = 0;
    for (const auto& [k, v] : r.sections[1].counts)
        if (k == "functions_run") run = v;
    CB_CHECK(run > 0 && run <= 3);   // (the first three, the informative ones)
    CB_CHECK(r.sections[1].stores_facts);
});

CB_TEST("analyze.report.a_driver_is_read", [] {
    Surrounding s;
    const rp::Result r = rp::run("vmouse.vxd", {.what = {rp::Analysis::CallGraph, rp::Analysis::Dispatch}, .store = false}, s.environment);
    CB_CHECK(r.kind == cb::analyze::ImageKind::Le);
    CB_CHECK_EQ(r.bits, 32);
    CB_CHECK(r.functions > 5);
    for (const rp::Section& section : r.sections) CB_CHECK(!section.failed);
    CB_CHECK(rp::text(r, false).find("virtual device driver") != std::string::npos);
});

CB_TEST("analyze.report.what_is_not_a_module", [] {
    Surrounding s;
    CB_CHECK_THROWS_AS(UsageError, (void)rp::run("no-such-module.dll", {}, s.environment));
});

CB_TEST("analyze.report.the_command", [] {
    Surrounding s;
    const cli::Cli cli("codebreaker", {rp::command(s.environment)});
    const stdfs::path out = s.dir.path() / "report.txt";
    const stdfs::path starts = s.dir.path() / "starts.txt";
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"analyze", "tracert.exe", "--what", "call_graph, strings", "--out", fs::to_text(out), "--starts", fs::to_text(starts), "--no-store"}), 0);
        CB_CHECK(cap.out().starts_with("tracert.exe: "));
        CB_CHECK(cap.out().find("report in ") != std::string::npos);
    }
    CB_CHECK(fs::read_text(out).find("== strings") != std::string::npos);
    CB_CHECK(stdfs::exists(starts));
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"analyze", "tracert.exe", "--what", "call_graph", "--json", "--no-store", "--out", fs::to_text(out)}), 0);
        CB_CHECK(nlohmann::json::parse(cap.out()).contains("counts"));
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"analyze", "tracert.exe", "--what", "call_graph", "--summary", "--no-store", "--out", fs::to_text(out)}), 0);
        CB_CHECK(cap.out().starts_with("call_graph"));
    }
    testing::CapturedStd cap;
    CB_CHECK_EQ(cli.run({"analyze", "tracert.exe", "--what", "nonsense", "--no-store"}), 1);
    CB_CHECK(cap.err().find("no analysis called nonsense") != std::string::npos);
    CB_CHECK_EQ(cli.run({"analyze"}), 2);   // (a usage error: its message on stderr, exit code 2)
});

CB_TEST("analyze.report.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_ANALYZE_PROOF");
    if (!proof || proof->empty()) return;
    Surrounding s;
    const auto started = std::chrono::steady_clock::now();
    // CB_ANALYZE_PROOF_STORE: a function store file the facts are written to as well
    if (const auto store = fs::get_env("CB_ANALYZE_PROOF_STORE"); store && !store->empty()) s.environment.facts_file = fs::from_text(*store);
    const rp::Result r = rp::run(*proof, {.store = !s.environment.facts_file.empty() && fs::get_env("CB_ANALYZE_PROOF_STORE").has_value()}, s.environment);
    std::cout << rp::json(r) << "\n";
    std::cout << "wall " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
