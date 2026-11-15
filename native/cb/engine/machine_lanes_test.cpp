// Tests of the table machine lanes are added to (engine/machine_lanes): what it accepts and refuses, the order files are asked in, and that the lane
// kit then has a registered machine like one of its own -- the lane by name, in `add_lanes`, and as the lane of a file.
#include "cb/engine/machine_lanes.hpp"

#include <algorithm>
#include <iostream>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/lane_kit_internal.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// A machine of one-file programs: its probe takes the files with `extension`.
MachineLane toy(std::string name, std::string extension) {
    MachineLane lane;
    lane.name = name;
    lane.probe = [extension](const stdfs::path& p) { return p.extension() == extension; };
    lane.make_spec = [name] {
        verify::lanes::ImageLaneSpec s;
        s.kind = name;
        s.rev = 3;
        s.states = {{{"a", 0}, {"fill", 0}}};
        s.effects = [](const stdfs::path&, std::uint32_t, const verify::lanes::State&) { return verify::lanes::Effects{std::string("ret")}; };
        s.what = {"end"};
        s.start = [](const verify::lanes::State&) { return std::string("from A=00"); };
        s.writer = [](const stdfs::path&, const stdfs::path&) { return verify::lanes::Written{}; };
        return s;
    };
    return lane;
}

// Takes the lanes a test added to the process's table out again, whatever the test did.
struct Cleanup {
    std::vector<std::string> names;
    ~Cleanup() {
        for (const std::string& n : names) machine_lanes().remove(n);
    }
};

}  // namespace

CB_TEST("engine.machine_lanes.a_lane_is_found_by_name_and_names_keep_the_order_added", [] {
    MachineLaneRegistry registry;
    CB_CHECK(registry.names().empty());
    registry.add(toy("toyb", ".tb"));
    registry.add(toy("toya", ".ta"));
    CB_CHECK(registry.names() == std::vector<std::string>{"toyb", "toya"});
    CB_CHECK(registry.find("toya") != nullptr);
    CB_CHECK_EQ(registry.find("toya")->name, "toya");
    CB_CHECK(registry.find("toyc") == nullptr);
    CB_CHECK(registry.remove("toyb"));
    CB_CHECK(!registry.remove("toyb"));
    CB_CHECK(registry.names() == std::vector<std::string>{"toya"});
});

CB_TEST("engine.machine_lanes.a_pointer_found_outlives_the_lane_being_removed", [] {
    MachineLaneRegistry registry;
    registry.add(toy("toya", ".ta"));
    const std::shared_ptr<const MachineLane> held = registry.find("toya");
    registry.remove("toya");
    CB_CHECK(held != nullptr);
    if (held) CB_CHECK_EQ(held->make_spec().kind, "toya");
});

CB_TEST("engine.machine_lanes.what_is_not_a_lane_is_refused", [] {
    MachineLaneRegistry registry;
    registry.add(toy("toya", ".ta"));
    CB_CHECK_THROWS_AS(UsageError, registry.add(toy("toya", ".tx")));   // twice
    CB_CHECK_THROWS_AS(UsageError, registry.add(toy("", ".tx")));
    CB_CHECK_THROWS_AS(UsageError, registry.add(toy("Toy", ".tx")));    // not lower case
    CB_CHECK_THROWS_AS(UsageError, registry.add(toy("to-y", ".tx")));
    for (const char* built_in : {"nes", "sms", "gb", "genesis", "ps1", "zx", "msx"}) {
        CB_CHECK(built_in_machine(built_in));
        CB_CHECK_THROWS_AS(UsageError, registry.add(toy(built_in, ".tx")));
    }
    CB_CHECK(!built_in_machine("toya"));
    MachineLane no_probe = toy("toyp", ".tp");
    no_probe.probe = nullptr;
    CB_CHECK_THROWS_AS(UsageError, registry.add(std::move(no_probe)));
    MachineLane no_spec = toy("toys", ".ts");
    no_spec.make_spec = nullptr;
    CB_CHECK_THROWS_AS(UsageError, registry.add(std::move(no_spec)));
    CB_CHECK(registry.names() == std::vector<std::string>{"toya"});   // none of the refused got in
});

CB_TEST("engine.machine_lanes.a_file_belongs_to_the_first_lane_that_takes_it", [] {
    MachineLaneRegistry registry;
    registry.add(toy("toya", ".ta"));
    MachineLane greedy = toy("toyg", ".tg");
    greedy.probe = [](const stdfs::path&) { return true; };
    registry.add(std::move(greedy));
    registry.add(toy("toyz", ".tz"));
    CB_CHECK_EQ(registry.name_of_file("game.ta"), "toya");
    CB_CHECK_EQ(registry.name_of_file("game.tz"), "toyg");   // asked in the order added: the greedy one says yes first
    CB_CHECK_EQ(MachineLaneRegistry().name_of_file("game.ta"), "");
});

CB_TEST("engine.machine_lanes.a_probe_that_throws_an_error_says_no_and_the_next_lane_is_asked", [] {
    MachineLaneRegistry registry;
    MachineLane damaged = toy("toyd", ".td");
    damaged.probe = [](const stdfs::path&) -> bool { throw FormatError("toy", 0, "damaged"); };
    registry.add(std::move(damaged));
    registry.add(toy("toya", ".ta"));
    CB_CHECK_EQ(registry.name_of_file("x.ta"), "toya");
    CB_CHECK_EQ(registry.name_of_file("x.td"), "");
});

CB_TEST("engine.machine_lanes.a_probe_may_use_the_registry_itself", [] {
    MachineLaneRegistry registry;
    MachineLane looks = toy("toyl", ".tl");
    looks.probe = [&registry](const stdfs::path&) { return registry.find("toyl") != nullptr; };   // (the probe runs outside the lock)
    registry.add(std::move(looks));
    CB_CHECK_EQ(registry.name_of_file("x"), "toyl");
});

CB_TEST("engine.machine_lanes.threads_add_and_ask_at_once", [] {
    MachineLaneRegistry registry;
    registry.add(toy("toya", ".ta"));
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t)
        threads.emplace_back([&registry, t] {
            registry.add(toy("toyt" + std::to_string(t), ".t" + std::to_string(t)));
            for (int i = 0; i < 200; ++i) {
                (void)registry.name_of_file("x.ta");
                (void)registry.names();
            }
        });
    for (std::thread& th : threads) th.join();
    CB_CHECK_EQ(registry.names().size(), std::size_t{5});
});

CB_TEST("engine.machine_lanes.the_lane_kit_has_a_registered_machine_like_one_of_its_own", [] {
    Cleanup cleanup;
    cleanup.names = {"toykit"};
    machine_lanes().add(toy("toykit", ".toykit"));
    // the reconstruction path asks this for the lane of a file
    CB_CHECK_EQ(kit_detail::machine_of_file("game.toykit"), "toykit");
    CB_CHECK_EQ(kit_detail::machine_of_file("game.unheard"), "");
    std::optional<config::Config> config;
    try {
        config = config::Config::load(config::current().main_checkout());
    } catch (const Error&) {
        std::cout << "no settings: the table alone was tested\n";
        return;
    }
    fs::ScopedTempDir state("cb-machine-lanes");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(state.path()));
    LaneKit kit(config::Config::load(config->checkout_root()));
    const std::shared_ptr<const verify::lanes::Kind> lane = kit.machine_lane("toykit");
    CB_CHECK(lane != nullptr);
    if (!lane) return;
    CB_CHECK_EQ(lane->name(), "toykit");
    CB_CHECK(kit.machine_lane("toykit") == lane);   // made once
    CB_CHECK(kit.lane_named("toykit") == lane);     // what a worker process asks
    CB_CHECK(!lane->profile("game.toykit").digest.empty());
    CB_CHECK(kit.machine_lane("nonesuch") == nullptr);
    verify::reconstruct::Kinds kinds;
    kit.add_lanes(kinds);
    const std::vector<std::string> names = kinds.names();
    CB_CHECK(std::find(names.begin(), names.end(), "toykit") != names.end());
    CB_CHECK(std::find(names.begin(), names.end(), "gb") != names.end());   // (the kit's own are still there)
});
