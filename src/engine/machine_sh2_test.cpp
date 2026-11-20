// The Sega 32X and Saturn lanes: registered in the machine lane table, a cartridge and a disc come back identical, and the functions are run in the SH-2
// interpreter over the machine's memory (the 32X's two sides by the function's address), and a copy with an edited function is judged beside the
// original.
#include "engine/machine_sh2.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "source/sh2_test_programs.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace t = cb::source::sh2_test;
namespace vl = cb::verify::lanes;

namespace {

bool contains(const std::vector<std::string>& lines, std::string_view line) {
    return std::find(lines.begin(), lines.end(), std::string(line)) != lines.end();
}

// The program's function `name` (its address) in a written result.
std::uint32_t address_of(const vl::Written& w, std::string_view name) {
    for (const auto& [address, n] : w.functions)
        if (n == name) return address;
    return 0;
}

}  // namespace

CB_TEST("engine.machine_sh2.are_registered_once_and_take_their_files", [] {
    register_sh2_lanes();
    register_sh2_lanes();   // (twice is fine)
    CB_CHECK(machine_lanes().find("sega32x") != nullptr);
    CB_CHECK(machine_lanes().find("saturn") != nullptr);
    fs::ScopedTempDir dir("cb_sh2_lanes");
    fs::write_file(dir.path() / "a.32x", t::cartridge_32x());
    fs::write_file(dir.path() / "a.iso", t::disc_saturn());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.32x"), "sega32x");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.iso"), "saturn");
    CB_CHECK_EQ(sega32x_lane_spec().kind, "sega32x");
    CB_CHECK_EQ(saturn_lane_spec().kind, "saturn");
    CB_CHECK_EQ(sega32x_lane_spec().states.size(), std::size_t{4});
});

CB_TEST("engine.machine_sh2.the_32x_runs_both_sides_in_their_own_interpreter", [] {
    fs::ScopedTempDir dir("cb_sh2_lanes");
    const stdfs::path rom = dir.path() / "a.32x";
    fs::write_file(rom, t::cartridge_32x());
    const vl::ImageLaneSpec spec = sega32x_lane_spec();
    CB_CHECK_EQ(spec.what.size(), std::size_t{6});
    const vl::Effects sh2 = spec.effects(rom, 0x06000040, spec.states[0]);
    CB_CHECK_EQ(sh2.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(sh2[0]), std::string("return"));
    // r0 = 14 (helper's 10 + 3, then tally's + 1) and r1 = 0xFF (tally's mask)
    const Bytes results = std::get<Bytes>(sh2[1]);
    CB_CHECK(results == (Bytes{0, 0, 0, 14, 0, 0, 0, 0xFF}));
    // ten writes of the count to communication port 0, one of helper's result to port 2
    const auto& io = std::get<std::vector<std::string>>(sh2[5]);
    CB_CHECK_EQ(io.size(), std::size_t{11});
    CB_CHECK_EQ(io.front(), std::string("w4 4020 1"));
    CB_CHECK_EQ(io[9], std::string("w4 4020 a"));
    CB_CHECK_EQ(io.back(), std::string("w4 4024 d"));
    CB_CHECK(contains(std::get<std::vector<std::string>>(sh2[4]), "0603fef8=1f"));   // (the return address, pushed on the stack and left there)
    // the 68000's initial program (move #$2700,sr ; bra.s *) is run in the 68000: it runs or stops, and has the same six places
    const vl::Effects m68k = spec.effects(rom, 0x3F0, spec.states[0]);
    CB_CHECK_EQ(m68k.size(), spec.what.size());
    CB_CHECK(!std::get<std::string>(m68k[0]).empty());
    // the same function from another state shows its state: r4 and the memory are different
    const vl::Effects other = spec.effects(rom, 0x06000040, spec.states[1]);
    CB_CHECK_EQ(std::get<std::string>(other[0]), std::string("return"));
});

CB_TEST("engine.machine_sh2.the_saturn_runs_functions_over_its_memory", [] {
    fs::ScopedTempDir dir("cb_sh2_lanes");
    const stdfs::path iso = dir.path() / "a.iso";
    fs::write_file(iso, t::disc_saturn());
    const vl::ImageLaneSpec spec = saturn_lane_spec();
    const vl::Effects e = spec.effects(iso, 0x06004000, spec.states[0]);
    CB_CHECK_EQ(e.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(e[0]), std::string("return"));
    // the wait for the vertical blank ended on the ninth read of the status register; the SCU's interrupt status is read once; leaf adds one to what
    // was stored in the variable
    const auto& io = std::get<std::vector<std::string>>(e[5]);
    CB_CHECK_EQ(io.size(), std::size_t{10});
    CB_CHECK(io.front().starts_with("r2 5f80004 0"));
    CB_CHECK_EQ(io[8], std::string("r2 5f80004 8"));
    CB_CHECK_EQ(io.back(), std::string("r4 5fe00a4 2"));
    const Bytes results = std::get<Bytes>(e[1]);
    CB_CHECK_EQ(static_cast<int>(results[3]), 3);
    CB_CHECK(contains(std::get<std::vector<std::string>>(e[4]), "06080003=02"));
    // IP.BIN's initial program is run too: it loads the first program's address and jumps there, which runs the first program
    const vl::Effects init = spec.effects(iso, 0x06002E80, spec.states[0]);
    CB_CHECK_EQ(init.size(), spec.what.size());
});

CB_TEST("engine.machine_sh2.the_writers_rebuild_identical", [] {
    fs::ScopedTempDir dir("cb_sh2_lanes");
    fs::write_file(dir.path() / "a.32x", t::cartridge_32x());
    fs::write_file(dir.path() / "a.iso", t::disc_saturn());
    const vl::Written w = sega32x_lane_spec().writer(dir.path() / "a.32x", dir.path() / "out32");
    CB_CHECK(w.linked && w.same);
    CB_CHECK(w.functions.size() >= 4);   // the SH-2 three and the 68000 reset
    const vl::Written s = saturn_lane_spec().writer(dir.path() / "a.iso", dir.path() / "outsat");
    CB_CHECK(s.linked && s.same);
    CB_CHECK(s.functions.size() >= 7);
});

CB_TEST("engine.machine_sh2.an_edited_function_is_judged_divergent", [] {
    fs::ScopedTempDir dir("cb_sh2_lanes");
    // the 32X: `helper` adds 3, the copy adds 4; `entry` calls it, `tally` does not
    {
        const Bytes original = t::cartridge_32x();
        const stdfs::path file = dir.path() / "a.32x";
        fs::write_file(file, original);
        const vl::ImageLane lane(sega32x_lane_spec());
        const vl::Written written = sega32x_lane_spec().writer(file, dir.path() / "out32");
        CB_CHECK(written.same);
        Bytes edited = original;
        bool found = false;
        for (std::size_t i = 0x1000; i + 1 < edited.size(); i += 2) {
            if (edited[i] == 0x70 && edited[i + 1] == 0x03) {   // add #3,r0
                edited[i + 1] = 0x04;
                found = true;
                break;
            }
        }
        CB_CHECK(found);
        const stdfs::path changed = dir.path() / "b.32x";
        fs::write_file(changed, edited);
        vl::Written w = written;
        w.built = changed;
        const auto [a, b] = lane.subjects_of(file, w);
        const std::uint32_t entry = address_of(written, "master_entry");
        CB_CHECK(entry != 0);
        const verify::Judgement same = lane.judge_function(false, a, a, entry);
        CB_CHECK(same.verdict == verify::Verdict::Equivalent);
        const verify::Judgement diverged = lane.judge_function(false, a, b, entry);
        CB_CHECK(diverged.verdict == verify::Verdict::Divergent);
        CB_CHECK(!diverged.reason.empty());
        // an identical rebuild is the original, whatever the function does
        CB_CHECK(lane.judge_function(true, a, b, entry).verdict == verify::Verdict::Equivalent);
    }
    // the Saturn: `leaf` adds 1, the copy adds 2
    {
        const Bytes original = t::disc_saturn();
        const stdfs::path file = dir.path() / "a.iso";
        fs::write_file(file, original);
        const vl::ImageLane lane(saturn_lane_spec());
        const vl::Written written = saturn_lane_spec().writer(file, dir.path() / "outsat");
        CB_CHECK(written.same);
        Bytes edited = original;
        bool found = false;
        for (std::size_t i = 18 * 2048; i + 1 < edited.size(); i += 2) {
            if (edited[i] == 0x70 && edited[i + 1] == 0x01) {   // add #1,r0
                edited[i + 1] = 0x02;
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
        const std::uint32_t entry = address_of(written, "first_read");
        const verify::Judgement diverged = lane.judge_function(false, a, b, entry);
        CB_CHECK(diverged.verdict == verify::Verdict::Divergent);
        CB_CHECK(lane.judge_function(false, a, a, entry).verdict == verify::Verdict::Equivalent);
    }
});
