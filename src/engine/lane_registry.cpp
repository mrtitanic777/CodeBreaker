// cb-unity: alone
#include "engine/lane_registry.hpp"

#include <algorithm>
#include <format>
#include <mutex>

#include "core/error.hpp"
#include "engine/machine_lanes.hpp"

namespace cb::engine {

namespace {

// (function-local: lanes are registered from static initialisers too -- a unit test's -- before a global of this file would be made)
struct LaneTable {
    std::mutex lock;
    std::vector<std::shared_ptr<const LaneRegistration>> lanes;
};
LaneTable& lane_table() {
    static LaneTable table;
    return table;
}

bool well_formed_lane_name(std::string_view name) {
    return !name.empty() && std::all_of(name.begin(), name.end(), [](char c) { return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'); });
}

}  // namespace

void register_lane(LaneRegistration lane) {
    if (!well_formed_lane_name(lane.name)) throw UsageError(std::format("lane name '{}': lower-case letters and digits only", lane.name));
    if (!lane.platform || !lane.probe) throw UsageError(std::format("lane '{}' needs its platform and its probe", lane.name));
    auto kept = std::make_shared<const LaneRegistration>(std::move(lane));
    {
        LaneTable& t = lane_table();
        std::lock_guard hold(t.lock);
        for (const auto& have : t.lanes)
            if (have->name == kept->name) return;
        t.lanes.push_back(kept);
    }
    // The places the lane is said to (each keeps its own table).
    for (const LaneDetector& d : kept->detectors)
        if (d.detect) formats::detect::add_detector(kept->name, d.priority, d.detect);
    if (kept->make_spec && !machine_lanes().find(kept->name)) {
        MachineLane machine;
        machine.name = kept->name;
        machine.probe = kept->probe;
        machine.make_spec = kept->make_spec;
        try {
            machine_lanes().add(std::move(machine));
        } catch (const UsageError&) {   // (another thread added it first)
        }
    }
}

bool unregister_lane(std::string_view name) {
    {
        LaneTable& t = lane_table();
        std::lock_guard hold(t.lock);
        const auto it = std::find_if(t.lanes.begin(), t.lanes.end(), [&](const auto& l) { return l->name == name; });
        if (it == t.lanes.end()) return false;
        t.lanes.erase(it);
    }
    formats::detect::remove_detectors(name);
    machine_lanes().remove(name);
    return true;
}

std::vector<std::shared_ptr<const LaneRegistration>> registered_lanes() {
    LaneTable& t = lane_table();
    std::lock_guard hold(t.lock);
    return t.lanes;
}

platforms::Platform lane_platform(const LaneRegistration& lane) {
    platforms::Platform p = lane.platform();
    p.recognise = lane.probe;
    return p;
}

std::vector<std::string> lane_program_extensions() {
    std::vector<std::string> out;
    for (const auto& lane : registered_lanes())
        for (const std::string& e : lane->program_extensions)
            if (std::find(out.begin(), out.end(), e) == out.end()) out.push_back(e);
    return out;
}

}  // namespace cb::engine
