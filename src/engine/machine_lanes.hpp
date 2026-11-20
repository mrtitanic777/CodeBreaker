// engine/machine_lanes.hpp -- the table a platform adds its machine lane to, without editing the lane kit.
//
// (A platform lane registers through engine/lane_registry.hpp: one registration object, which fills this table with its probe and spec, and the platform
// registry, `info`'s detectors and `reach`'s extensions too. This table is what the lane kit asks for the lanes' specs.)
//
// A machine lane (a cartridge, a snapshot, a tape: the NES, Game Boy ... lanes of lane_kit_machines.cpp) is made of what is particular
// to its machine -- how to tell its files, and the `ImageLaneSpec` its lane is built from. A platform that is not one of the lane kit's
// own gives those two things here, once, before the kit is used:
//
//     engine::machine_lanes().add({
//         .name = "toy",                                                  // what results are filed under
//         .probe = [](const std::filesystem::path& p) { return p.extension() == ".toy"; },
//         .make_spec = [] { verify::lanes::ImageLaneSpec s; s.kind = "toy"; ...; return s; }});
//
// and the kit then has the lane like any other: `LaneKit::machine_lane("toy")`, `lane_named("toy")` (worker processes use that),
// `add_lanes` (so `measure` and `reach --judge` judge its files), and the reconstruction path's choice of lane for a file
// (`kit_detail::machine_of_file`). The lane kit's own seven machines are tried first; a registered machine is asked in the order added.
//
// The table is process-wide and thread-safe. A host program registers at the top of its main (before a LaneKit is made, as
// `LaneKit::register_worker_kinds` is), and so does its worker: the same program starts with the same registrations. Registering from a
// static initialiser works only in a translation unit the linker keeps -- an object file of a static library nothing else refers to is
// dropped -- so a plugin gives its host one function to call.
#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

struct MachineLane {
    // The name results are filed under: lower-case letters and digits, not one of the lane kit's own (`built_in_machine`).
    std::string name;
    // Is this file one of the machine's (a cartridge, a snapshot, a tape)? false for a file that is not, unreadable or damaged;
    // an `Error` thrown is taken as false.
    std::function<bool(const std::filesystem::path&)> probe;
    // The spec the lane is made from; made when the lane is first asked for. Its `kind` must be `name`.
    std::function<verify::lanes::ImageLaneSpec()> make_spec;
};

// The names the lane kit builds itself ("nes", "sms", "gb", "genesis", "ps1", "gba", "zx", "msx", "coleco", "cpc").
bool built_in_machine(std::string_view name);

class MachineLaneRegistry {
public:
    // UsageError: an empty or ill-formed name, a missing function, a name added already or a built-in one.
    void add(MachineLane lane);
    // Takes a lane out again (a test's); false when there was none.
    bool remove(std::string_view name);

    // The lane, or null. The pointer stays valid whatever is added or removed after.
    std::shared_ptr<const MachineLane> find(std::string_view name) const;
    // The names, in the order added.
    std::vector<std::string> names() const;
    // The name of the first lane whose probe takes the file; empty when none does.
    std::string name_of_file(const std::filesystem::path& path) const;

private:

    mutable std::mutex lock_;
    std::vector<std::shared_ptr<const MachineLane>> lanes_;
};

// The process's table.
MachineLaneRegistry& machine_lanes();

}  // namespace cb::engine
