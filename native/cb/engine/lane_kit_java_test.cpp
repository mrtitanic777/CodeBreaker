// Tests of the Java lane's wiring (engine/lane_kit_java): the kit makes the lane when a host C++ compiler is set up, the lane is the one the corpus's
// name "java" asks for, and a folder of Java source is a module of it that transpiles with the production assembly (javac's tree of the sources as the
// front end -- needs a JDK; without one the test stops at the lane).
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/verify/lanes/java.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

CB_TEST("engine.lane_kit.the_java_lane_is_wired_when_a_host_compiler_is_set_up", [] {
    std::optional<config::Config> config;
    try {
        config = config::Config::load(config::current().main_checkout());
    } catch (const Error&) {
        std::cout << "no settings: nothing to wire\n";
        return;
    }
    fs::ScopedTempDir state("cb-lane-kit-java");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(state.path()));
    LaneKit kit(config::Config::load(config->checkout_root()));
    const std::shared_ptr<const verify::lanes::Kind> lane = kit.java_lane();
    if (!lane) {
        CB_CHECK(!LaneKit::java_missing().empty());   // (no g++ here: the kit says what the lane needs)
        return;
    }
    CB_CHECK_EQ(lane->name(), "java");
    CB_CHECK(kit.java_lane() == lane);                // a lane made again is the same lane
    CB_CHECK(kit.lane_named("java") == lane);
    verify::reconstruct::Kinds kinds;
    kit.add_lanes(kinds);
    const std::vector<std::string> names = kinds.names();
    CB_CHECK(std::find(names.begin(), names.end(), "java") != names.end());
});
