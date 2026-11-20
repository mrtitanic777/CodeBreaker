// Tests of the platform lane registry (engine/lane_registry): what a registration may be, that every lane of the program is a complete one, and that the
// places that used to list lanes (the platform registry, `info`'s detectors, the machine lane table, reach's extensions) are filled from it. No lane is named
// or counted here: adding a lane adds a registration, and these are the properties each registration has.
// cb-unity: alone
#include "engine/lane_registry.hpp"

#include <algorithm>
#include <set>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "core/text.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "platforms/package.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// The unit tests run without a main that registers the lanes (the program's own mains do): the lanes are registered before any test runs, so that
// `info`'s detectors and the machine lane table are the program's, as in codebreaker.exe.
[[maybe_unused]] const bool g_lanes_registered = (register_all_lanes(), true);

LaneRegistration toy_lane(const std::string& name, const std::string& extension) {
    LaneRegistration lane;
    lane.name = name;
    lane.platform = [name] {
        platforms::MachineSpec s;
        s.name = name;
        s.title = "Toy " + name;
        s.family = "toy";
        s.container = "toy file";
        s.cpu = {"toy:LE:8:default", 8, platforms::Endian::Little, platforms::OwnDecoder::Z80, {"A"}, "SP"};
        s.results = {"A"};
        return platforms::machine(s);
    };
    lane.probe = [extension](const stdfs::path& p) { return p.extension() == extension; };
    lane.program_extensions = {extension};
    lane.detectors = {{.priority = 5, .detect = [name, extension](const formats::detect::DetectInput& in) -> std::optional<std::vector<formats::detect::Detected>> {
                           if (!formats::detect::name_ends_with(in.name, {extension})) return std::nullopt;
                           return formats::detect::one("Toy cartridge", "", name, "reach");
                       }}};
    return lane;
}

}  // namespace

CB_TEST("engine.lane_registry.every_lane_is_a_complete_registration", [] {
    const auto lanes = registered_lanes();
    CB_CHECK(!lanes.empty());
    std::set<std::string> seen;
    for (const auto& lane : lanes) {
        CB_CHECK(seen.insert(lane->name).second);          // a name once
        CB_CHECK(!lane->name.empty());
        CB_CHECK(lane->platform != nullptr);
        CB_CHECK(lane->probe != nullptr);                   // every lane tells its files
        for (const char c : lane->name) CB_CHECK((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
        const platforms::Platform p = lane_platform(*lane);
        CB_CHECK_EQ(p.name, lane->name);                    // the platform, the lane and the results all go by the one name
        CB_CHECK(p.lane.has_value() && *p.lane == lane->name);
        CB_CHECK(!p.title.empty());
        CB_CHECK(!p.family.empty());
        CB_CHECK(p.recognise != nullptr);
        CB_CHECK(p.emulator.runs_code);
        for (const LaneDetector& d : lane->detectors) CB_CHECK(d.detect != nullptr);
        for (const std::string& e : lane->program_extensions) CB_CHECK(e.size() > 1 && e[0] == '.' && e == text::lower(e));   // (lower case, with the dot)
        if (lane->make_spec) {                              // a lane the machine lane table builds: its decoder, writer and machine model
            const auto machine = machine_lanes().find(lane->name);
            CB_CHECK(machine != nullptr);
            const verify::lanes::ImageLaneSpec spec = lane->make_spec();
            CB_CHECK_EQ(spec.kind, lane->name);
            CB_CHECK(!spec.states.empty());
            CB_CHECK(spec.effects != nullptr);
            CB_CHECK(spec.writer != nullptr);
            CB_CHECK(!spec.what.empty());
        } else {
            CB_CHECK(built_in_machine(lane->name));         // the lane kit's own machines are the only lanes without a spec
        }
    }
});

CB_TEST("engine.lane_registry.the_platform_registry_is_built_from_the_lanes_in_order", [] {
    platforms::Registry registry;
    std::vector<std::string> names;
    for (const auto& lane : registered_lanes()) {
        registry.add(lane_platform(*lane));
        names.push_back(lane->name);
    }
    std::vector<std::string> got;
    for (const platforms::Platform& p : registry.all()) got.push_back(p.name);
    CB_CHECK(got == names);                                 // machines keep the order they were added in
    for (const auto& lane : registered_lanes()) CB_CHECK(registry.named(lane->name) != nullptr);
    CB_CHECK_THROWS_AS(UsageError, registry.add(lane_platform(*registered_lanes().front())));   // a machine is told by its name
});

CB_TEST("engine.lane_registry.a_registration_reaches_every_table_and_goes_again", [] {
    CB_CHECK(!machine_lanes().find("toyreg"));
    LaneRegistration toy = toy_lane("toyreg", ".toyreg");
    toy.make_spec = [] {
        verify::lanes::ImageLaneSpec s;
        s.kind = "toyreg";
        return s;
    };
    register_lane(toy);
    register_lane(toy);                                     // (a second call, by a second thread or a worker, does nothing)
    CB_CHECK(machine_lanes().find("toyreg") != nullptr);
    CB_CHECK_EQ(machine_lanes().name_of_file("x.toyreg"), "toyreg");
    const auto exts = lane_program_extensions();
    CB_CHECK(std::find(exts.begin(), exts.end(), ".toyreg") != exts.end());
    const Bytes none;
    const auto found = formats::detect::identify(ByteView(none), "game.toyreg");
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found[0].format, "Toy cartridge");
    CB_CHECK(unregister_lane("toyreg"));
    CB_CHECK(!unregister_lane("toyreg"));
    CB_CHECK(machine_lanes().find("toyreg") == nullptr);
    CB_CHECK(formats::detect::identify(ByteView(none), "game.toyreg").empty());
});

CB_TEST("engine.lane_registry.refuses_what_cannot_be_a_lane", [] {
    LaneRegistration lane = toy_lane("Toy", ".t");
    CB_CHECK_THROWS_AS(UsageError, register_lane(lane));    // upper case
    lane = toy_lane("toyx", ".t");
    lane.probe = nullptr;
    CB_CHECK_THROWS_AS(UsageError, register_lane(lane));    // no probe
    lane = toy_lane("toyx", ".t");
    lane.platform = nullptr;
    CB_CHECK_THROWS_AS(UsageError, register_lane(lane));    // no platform
});
