// engine/lane_registry.hpp -- a platform lane is ONE registration object, defined in the lane's own files.
//
// What a platform has to say to the rest of CodeBreaker used to be written in a dozen shared lists (the platform enum and its probes, `reach`'s
// probes and extensions, `info`'s detection text, the lane kit's machine chain ...). It is said once, here:
//
//     void register_toy_lane() {                                          // engine/machine_toy.cpp, the lane's own file
//         register_lane({
//             .name = "toy",                                              // what results are filed under; the platform's name and the lane's
//             .platform = [] { return platforms::toy(); },                // the descriptor: CPU, convention, emulator, toolchain
//             .probe = [](const std::filesystem::path& p) { return formats::toy::probe(p); },   // is this file one of the lane's?
//             .make_spec = toy_lane_spec,                                 // the decoder, the writer and the machine model: what ImageLane runs
//             .detectors = {{.priority = 500, .detect = toy_detect}},     // the text `codebreaker info` prints for one of its files
//             .program_extensions = {".toy"},                             // `reach` looks for these inside containers
//         });
//     }
//
// and `engine/lane_list.cpp` names `register_toy_lane` once -- the one explicit list (an object file of a static library nothing refers to is
// dropped by the linker, so lanes do not register themselves from static initialisers). `register_all_lanes()` runs that list; the places that
// used to list lanes iterate the registry instead:
//
//   * the platform registry (`app/reach_command.cpp` standard_registry): `lane_platform(lane)` for each, in the order of the list;
//   * `formats::detect::identify` (`info`, `reach`): the lane's detectors (formats/detect.hpp), in the order of their priorities;
//   * the machine lane table (engine/machine_lanes.hpp): `make_spec`, so `measure` and the worker processes have the lane;
//   * `reach`'s list of program file extensions: `lane_program_extensions()`.
//
// The registry is process-wide and thread-safe. A host program calls `register_all_lanes()` at the top of its main (the lane kit, `reach` and the
// MCP server do); it is idempotent, and a worker process does the same.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "formats/detect.hpp"
#include "platforms/package.hpp"
#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What `codebreaker info` says of a file of the lane, and where among the other platforms' detectors.
struct LaneDetector {
    int priority = 1000;                   // lower is tried first (the existing platforms hold 10, 20, 30 ... in the order `info` has always tried them)
    formats::detect::Detector detect{};
};

struct LaneRegistration {
    // The name results are filed under, and the machine lane's kind: lower-case letters and digits ("gb", "ps2", "wonderswan").
    std::string name{};   // (every member has an initialiser of its own, so a registration may leave fields out without a -Wmissing-field-initializers warning)
    // The platform's descriptor (platforms/<name>.cpp); its `name` and `lane` are the registration's. Made when the registry is built.
    std::function<platforms::Platform()> platform{};
    // Is this file one of the lane's (a cartridge, a disc, a snapshot)? false for a file that is not, unreadable or damaged.
    std::function<bool(const std::filesystem::path&)> probe{};
    // The spec the lane is made from (engine/machine_lanes.hpp); empty: the lane kit builds this machine itself (its first ten machines).
    std::function<verify::lanes::ImageLaneSpec()> make_spec{};
    // The text `info` prints for the lane's files; none: `info` says nothing of them beyond the generic formats.
    std::vector<LaneDetector> detectors{};
    // The file extensions `reach` takes for programs when it looks inside a container (".gba"), lower case with the dot.
    std::vector<std::string> program_extensions{};
};

// Add a lane. UsageError: an ill-formed name or a missing platform / probe. A name registered already is left as it is (a second call
// of the same registration function, by a second thread or a worker, does nothing).
void register_lane(LaneRegistration lane);

// Take a lane out again (a test's): its detectors, its machine lane and its place in the registry. false when there was none.
bool unregister_lane(std::string_view name);

// Every lane of the program: runs the list of engine/lane_list.cpp once. Safe to call from every entry point and every thread.
void register_all_lanes();

// The lanes registered, in the order added (the order of the list, which is the order platforms are tried in). A snapshot.
std::vector<std::shared_ptr<const LaneRegistration>> registered_lanes();

// The lane's descriptor with its probe set: what the platform registry is given.
platforms::Platform lane_platform(const LaneRegistration& lane);

// The program extensions of every lane, in the order registered, without repeats.
std::vector<std::string> lane_program_extensions();

}  // namespace cb::engine
