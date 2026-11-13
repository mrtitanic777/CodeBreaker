// Tests of the lane kit (engine/lane_kit): the lanes' surroundings made from the settings, and the reconstruction path run through them on real
// modules.
//
// The first tests need nothing but the checkout: the kit is made over the settings of the main checkout, its lanes are the ones the reconstruction
// path is told of, and a lane whose tools are not set up on this machine is left out (the tests say so and pass).
//
// `engine.lane_kit.proof` is the product check: with CB_LANE_KIT_PROOF set to a folder and CB_LANE_KIT_MODULES to the modules to run (corpus
// names or paths, separated by ';'), it runs the new Reconstructor with the wired lanes on each module -- the decompiler, OpenWatcom, the
// emulators, the judge -- and writes <folder>/<module>.json: {module, seconds, functions: {address: {name, verdict, reason, limits, detail}}},
// the same document tools/lanekit_py.py writes of the Python's run (the Python is the reference: state/work/lanekit/run_py.py). It is slow
// (minutes per module), so it is off unless asked for; CB_STATE should name a folder of its own.
#include "cb/engine/lane_kit.hpp"

#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

std::optional<config::Config> kit_main_config() {
    try {
        return config::Config::load(config::current().main_checkout());
    } catch (const Error&) {
        return std::nullopt;
    }
}

}  // namespace

CB_TEST("engine.lane_kit.the_kit_is_made_of_the_settings_and_says_which_lanes_it_has", [] {
    const std::optional<config::Config> config = kit_main_config();
    if (!config) {
        std::cout << "no settings: nothing to wire\n";
        return;
    }
    fs::ScopedTempDir state("cb-lane-kit");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(state.path()));
    LaneKit kit(config::Config::load(config->checkout_root()));
    verify::reconstruct::Kinds kinds;
    kit.add_lanes(kinds);
    const std::vector<std::string> names = kinds.names();
    // the lanes that can be wired are filed under the corpus's names for them; the PowerPC and Java lanes are not wired, and say why
    for (const std::string& name : names) CB_CHECK(name == "pe" || name == "ne" || name == "le");
    CB_CHECK(!LaneKit::powerpc_missing().empty());
    CB_CHECK(!LaneKit::java_missing().empty());
    // a lane made again is the same lane: one decompiler, one builder, one knowledge base
    CB_CHECK(kit.win32_lane() == kit.win32_lane());
    CB_CHECK(&kit.knowledge() == &kit.knowledge());
    CB_CHECK(&kit.decompiler() == &kit.decompiler());
});

CB_TEST("engine.lane_kit.proof", [] {
    const std::optional<std::string> out = fs::get_env("CB_LANE_KIT_PROOF");
    const std::optional<std::string> modules = fs::get_env("CB_LANE_KIT_MODULES");
    if (!out || out->empty() || !modules || modules->empty()) return;   // (slow: asked for by name)
    const config::Config config = config::Config::load(config::current().main_checkout());
    LaneKit kit(config);
    // CB_LANE_KIT_WORKERS: the functions are judged in worker processes (cb_lane_worker.exe, beside the tests)
    if (const auto workers = fs::get_env("CB_LANE_KIT_WORKERS"); workers && !workers->empty() && *workers != "0")
        kit.set_worker_launcher(jobs::WorkerLauncher(jobs::WorkerLauncher::for_this_program().exe().parent_path() / "cb_lane_worker.exe"));
    verify::reconstruct::Kinds kinds;
    kit.add_lanes(kinds);
    engine::FunctionStore store(config.state("functions.sqlite"), config.state("db"));
    verify::reconstruct::Services services = kit.reconstruct_services(kinds, store);
    services.log = [](std::string_view line) { std::cout << line << "\n"; };
    const verify::reconstruct::Reconstructor reconstructor(std::move(services));
    fs::make_dirs(fs::from_text(*out));
    for (const std::string_view name : text::split(*modules, ';')) {
        if (name.empty()) continue;
        const stdfs::path path = kit.corpus().resolve(name);
        const auto started = std::chrono::steady_clock::now();
        const verify::reconstruct::Outcome done = reconstructor.run(path);
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
        nlohmann::ordered_json doc;
        doc["module"] = fs::to_text(path.filename());
        doc["seconds"] = seconds;
        nlohmann::ordered_json functions = nlohmann::ordered_json::object();
        for (const auto& [addr, best] : store.judged(done.sha, done.profile, verify::reconstruct::kSource)) {
            nlohmann::ordered_json row;
            row["name"] = best.name.value_or("");
            row["verdict"] = best.verdict.value_or("");
            row["reason"] = best.reason;
            row["limits"] = best.limits;
            row["detail"] = best.detail;
            functions[addr] = std::move(row);
        }
        doc["functions"] = std::move(functions);
        fs::write_text(fs::from_text(*out) / (fs::to_text(path.filename()) + ".json"), doc.dump(1));
        std::cout << done.line() << "\n";
    }
});
