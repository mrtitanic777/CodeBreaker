// Tests of the lane kit (engine/lane_kit): the lanes' surroundings made from the settings, and the reconstruction path run through them on real
// modules.
//
// The first tests need nothing but the checkout: the kit is made over the settings of the main checkout, its lanes are the ones the reconstruction
// path is told of, and a lane whose tools are not set up on this machine is left out (the tests say so and pass).
//
// `engine.lane_kit.proof` is the product check: with CB_LANE_KIT_PROOF set to a folder and CB_LANE_KIT_MODULES to the modules to run (corpus
// names or paths, separated by ';'), it runs the new Reconstructor with the wired lanes on each module -- the decompiler, OpenWatcom, the
// emulators, the judge -- and writes <folder>/<module>.json: {module, seconds, functions: {address: {name, verdict, reason, limits, detail}}},
// the same shape as the recorded <module>.python.json documents. It is slow
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
    // the lanes that can be wired are filed under the corpus's names for them; the Java lane is wired when a host C++ compiler is set up, and says what it needs when not
    for (const std::string& name : names)
        CB_CHECK(name == "pe" || name == "ne" || name == "le" || name == "powerpc" || name == "dos" || name == "nes" || name == "sms" || name == "gb" ||
                 name == "genesis" || name == "ps1" || name == "gba" || name == "zx" || name == "msx" || name == "coleco" || name == "cpc" || name == "atari2600" || name == "atari7800" || name == "lynx" || name == "pce" || name == "snes" || name == "neogeo" || name == "segacd" || name == "nds" || name == "n64" || name == "sega32x" || name == "saturn" || name == "dreamcast" || name == "gamecube" || name == "wii" || name == "wiiu" || name == "java");
    CB_CHECK(std::find(names.begin(), names.end(), "gb") != names.end());   // (the machines need no tools but CodeBreaker's own)
    CB_CHECK(!LaneKit::java_missing().empty());
    // a lane made again is the same lane: one decompiler, one builder, one knowledge base
    CB_CHECK(kit.win32_lane() == kit.win32_lane());
    CB_CHECK(&kit.knowledge() == &kit.knowledge());
    CB_CHECK(&kit.decompiler() == &kit.decompiler());
});

CB_TEST("engine.lane_kit.the_worker_kinds_are_registered_and_no_launcher_means_this_process_judges", [] {
    jobs::WorkerKinds kinds;
    LaneKit::register_worker_kinds(kinds);
    CB_CHECK(kinds.find("lane-judge") != nullptr);
    CB_CHECK(kinds.find("lane-build") != nullptr);
    CB_CHECK(kinds.find("nonesuch") == nullptr);
    const std::optional<config::Config> config = kit_main_config();
    if (!config) return;
    fs::ScopedTempDir state("cb-lane-kit");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(state.path()));
    LaneKit kit(config::Config::load(config->checkout_root()));
    const std::vector<std::string> names{"a", "b", "c", "d", "e", "f", "g", "h"};
    const stdfs::path module = "m.dll", work = "w";
    if (!kit.win32_lane()) return;   // (no tools for the lane: nothing to ask of it)
    const verify::lanes::Built built;
    // no launcher: the threads of the caller judge, which is what nullopt says; a program with workers disabled is the same
    CB_CHECK(!kit.judge_apart({*kit.win32_lane(), built, names, module, work}));
    kit.set_worker_launcher(jobs::WorkerLauncher(stdfs::path()));
    CB_CHECK(!kit.judge_apart({*kit.win32_lane(), built, names, module, work}));
    CB_CHECK(!kit.build_apart("lz32.dll", std::nullopt, true));
    // a lane is found by the name results are filed under
    CB_CHECK(kit.lane_named("nonesuch") == nullptr);
    CB_CHECK(kit.lane_named("pe") == kit.win32_lane());
    CB_CHECK(kit.lane_named("powerpc") == kit.powerpc_lane());
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
        // against the recorded document of the same module (testdata/lane_kit/<module>.python.json): the same verdict, reason and limits of every function.
        // Not compared: the coverage figure (the two draw their guessed inputs differently), and for a driver the number of paths (the recorded run wrote the driver back
        // with another writer, so it explored other instructions).
        const stdfs::path golden = testing::data_path("engine/testdata/lane_kit/" + fs::to_text(path.filename()) + ".python.json");
        if (!stdfs::exists(golden)) continue;
        const nlohmann::ordered_json want = json::load(golden);
        const bool driver = path.extension() == ".vxd";
        const auto normal = [&](std::string limits) {
            std::vector<std::string> kept;
            for (const std::string_view limit : text::split(limits, ',')) {
                if (limit.starts_with("coverage=") || limit.empty() || (driver && limit.starts_with("paths="))) continue;
                kept.emplace_back(limit);
            }
            std::ranges::sort(kept);
            return text::join(kept, ",");
        };
        std::size_t same = 0;
        for (const auto& [addr, row] : want.at("functions").items()) {
            const auto got = doc.at("functions").find(addr);
            const bool alike = got != doc.at("functions").end() && (*got)["verdict"] == row["verdict"] && (*got)["reason"].get<std::string>() == row["reason"].get<std::string>() &&
                               normal((*got)["limits"].get<std::string>()) == normal(row["limits"].is_null() ? std::string() : row["limits"].get<std::string>());
            if (alike) ++same;
            else std::cout << "  differs from the recorded: " << addr << " " << row["name"].get<std::string>() << "\n";
        }
        std::cout << "  " << same << " of " << want.at("functions").size() << " functions as recorded\n";
        CB_CHECK_EQ(same, want.at("functions").size());
    }
});

// The module builder over the kit's services: `engine.lane_kit.module_build` with CB_LANE_KIT_BUILD naming the modules (';' separated) builds each whole
// and prints its report -- the product check of the builder's wiring (slow, so off unless asked for; CB_STATE should name a folder of its own).
CB_TEST("engine.lane_kit.module_build", [] {
    const std::optional<std::string> modules = fs::get_env("CB_LANE_KIT_BUILD");
    if (!modules || modules->empty()) return;
    const config::Config config = config::Config::load(config::current().main_checkout());
    LaneKit kit(config);
    const build::module::ModuleBuilder builder(kit.module_services());
    for (const std::string_view name : text::split(*modules, ';')) {
        if (name.empty()) continue;
        build::module::BuildOptions options;
        options.outdir = config.state("lane_kit_build") / std::string(name);
        const auto started = std::chrono::steady_clock::now();
        const build::module::Built b = builder.build(name, options);
        std::cout << b.report() << "\n(" << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << "s)\n";
        CB_CHECK(b.linked());
    }
});

CB_TEST("engine.lane_kit.the_host_hook_lets_a_program_be_the_host_of_the_workers", [] {
    // a program not started as a worker goes on (-1) and is a host from then on; one started as a worker answers with its own exit code (an unknown kind: 2)
    char program[] = "codebreaker", flag[] = "--cb-worker", kind[] = "no-such-kind";
    char* plain[] = {program};
    CB_CHECK(!hosting_workers());
    CB_CHECK_EQ(host_main_hook(1, plain), -1);
    CB_CHECK(hosting_workers());
    char* worker[] = {program, flag, kind};
    CB_CHECK_EQ(host_main_hook(3, worker), jobs::kWorkerUnknownKind);
});
