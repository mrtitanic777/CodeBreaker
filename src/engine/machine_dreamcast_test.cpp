// cb-unity: alone (file-local names that other files in a batch also use)
// The Dreamcast lane: registered in the machine lane table, a disc comes back identical, functions are run in the SH-4 interpreter over the machine's memory
// (the PowerVR's status register, the BIOS's system-call stubs, the FPU), and a copy with an edited function is judged beside the original.
#include "engine/machine_dreamcast.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "source/dreamcast_test_programs.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace t = cb::source::dreamcast_test;
namespace vl = cb::verify::lanes;

namespace {

bool contains(const std::vector<std::string>& lines, std::string_view line) {
    return std::find(lines.begin(), lines.end(), std::string(line)) != lines.end();
}

bool starts(const std::vector<std::string>& lines, std::string_view prefix) {
    return std::any_of(lines.begin(), lines.end(), [&](const std::string& l) { return l.starts_with(prefix); });
}

}  // namespace

CB_TEST("engine.machine_dreamcast.is_registered_once_and_takes_its_files", [] {
    register_dreamcast_lane();
    register_dreamcast_lane();   // (twice is fine)
    CB_CHECK(machine_lanes().find("dreamcast") != nullptr);
    fs::ScopedTempDir dir("cb_dc_lane");
    fs::write_file(dir.path() / "a.iso", t::iso(false));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.iso"), "dreamcast");
    CB_CHECK_EQ(dreamcast_lane_spec().kind, "dreamcast");
    CB_CHECK_EQ(dreamcast_lane_spec().states.size(), std::size_t{4});
    CB_CHECK_EQ(dreamcast_lane_spec().what.size(), std::size_t{6});
});

CB_TEST("engine.machine_dreamcast.the_writer_rebuilds_the_disc_identical", [] {
    fs::ScopedTempDir dir("cb_dc_lane");
    fs::write_file(dir.path() / "a.iso", t::iso(false));
    const vl::Written w = dreamcast_lane_spec().writer(dir.path() / "a.iso", dir.path() / "out");
    CB_CHECK(w.linked && w.same);
    CB_CHECK(w.functions.size() >= 4);
});

CB_TEST("engine.machine_dreamcast.functions_run_over_the_machines_memory_and_registers", [] {
    fs::ScopedTempDir dir("cb_dc_lane");
    const stdfs::path disc = dir.path() / "a.iso";
    fs::write_file(disc, t::iso(false));
    const vl::ImageLaneSpec spec = dreamcast_lane_spec();
    const vl::Effects e = spec.effects(disc, 0x0C010000, spec.states[0]);
    CB_CHECK_EQ(e.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(e[0]), std::string("return"));
    // r0 = 4 (leaf doubles the 2.0 the program stored, and truncates), r1 = 0, fr0 = 4.0
    const Bytes results = std::get<Bytes>(e[1]);
    CB_CHECK_EQ(results.size(), std::size_t{12});
    CB_CHECK_EQ(results[3], std::uint8_t{4});
    CB_CHECK_EQ(results[8], std::uint8_t{0x40});
    CB_CHECK_EQ(results[9], std::uint8_t{0x80});
    // the work window holds 2.0 (little-endian: the high byte is last)
    const std::vector<std::string> changed = std::get<std::vector<std::string>>(e[4]);
    CB_CHECK(contains(changed, "0c800003=40"));
    // the wait on SPG_STATUS read the PowerVR's register until the vertical sync showed, every read logged
    const std::vector<std::string> io = std::get<std::vector<std::string>>(e[5]);
    CB_CHECK(starts(io, "r4 5f810c "));
    // the same program from a state that fills memory: still returns, and is deterministic
    const vl::Effects again = spec.effects(disc, 0x0C010000, spec.states[0]);
    CB_CHECK(std::get<std::vector<std::string>>(again[5]) == io);
});

CB_TEST("engine.machine_dreamcast.bios_calls_are_stubs_and_are_logged", [] {
    fs::ScopedTempDir dir("cb_dc_lane");
    const stdfs::path disc = dir.path() / "a.iso";
    fs::write_file(disc, t::iso(false));
    const vl::ImageLaneSpec spec = dreamcast_lane_spec();
    const vl::Written written = spec.writer(disc, dir.path() / "out");
    bool seen = false;
    for (const auto& [address, name] : written.functions) {
        if (address < 0x0C010000) continue;
        const vl::Effects e = spec.effects(disc, address, spec.states[0]);
        // the system-information vector at $8C0000B0 points at a stub: calling it is a `call` entry with the command in r7, and the stub returns 0
        if (starts(std::get<std::vector<std::string>>(e[5]), "call 100 7")) {
            seen = true;
            CB_CHECK_EQ(std::get<std::string>(e[0]), std::string("return"));
            CB_CHECK_EQ(std::get<Bytes>(e[1])[3], std::uint8_t{0});   // r0: the stub's result
        }
    }
    CB_CHECK(seen);
});

CB_TEST("engine.machine_dreamcast.an_edited_function_is_judged_divergent", [] {
    fs::ScopedTempDir dir("cb_dc_lane");
    const Bytes original = t::iso(false);
    const stdfs::path file = dir.path() / "a.iso";
    fs::write_file(file, original);
    const vl::ImageLane lane(dreamcast_lane_spec());
    const vl::Written written = dreamcast_lane_spec().writer(file, dir.path() / "out");
    CB_CHECK(written.same);
    // the program adds 1.0 and 1.0 (fadd fr1,fr0: the word $F010, bytes 10 F0); the copy subtracts (fsub, $F011)
    Bytes edited = original;
    bool found = false;
    for (std::size_t i = 18 * 2048; i + 1 < edited.size(); i += 2) {
        if (edited[i] == 0x10 && edited[i + 1] == 0xF0) {
            edited[i] = 0x11;
            found = true;
            break;
        }
    }
    CB_CHECK(found);
    const stdfs::path changed = dir.path() / "b.iso";
    fs::write_file(changed, edited);
    vl::Written w = written;
    w.built = changed;
    const auto [a, b] = lane.subjects_of(file, w);
    CB_CHECK(lane.judge_function(false, a, a, 0x0C010000).verdict == verify::Verdict::Equivalent);
    const verify::Judgement diverged = lane.judge_function(false, a, b, 0x0C010000);
    CB_CHECK(diverged.verdict == verify::Verdict::Divergent);
    CB_CHECK(!diverged.reason.empty());
    // an identical rebuild is the original, whatever the function does
    CB_CHECK(lane.judge_function(true, a, b, 0x0C010000).verdict == verify::Verdict::Equivalent);
});
