// cb-unity: alone (file-local names that other files in a batch also use)
// The Nintendo 64 lane: registered in the machine lane table, a ROM comes back identical, and the functions of a ROM are run in the R4300i
// interpreter over the console's memory, the same in the original and in a rebuilt copy, and different when an instruction is changed.
#include "cb/engine/machine_n64.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/source/n64_test_rom.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace nt = cb::source::n64_test;
namespace vl = cb::verify::lanes;

namespace {

// `helper`: the second function after the entry (`print` is the first, `handler` the last), in address order.
std::uint32_t helper_of(const vl::Written& w) {
    unsigned seen = 0;
    for (const auto& [address, name] : w.functions)
        if (name != "entry" && ++seen == 2) return address;
    return 0;
}

}  // namespace

CB_TEST("engine.machine_n64.is_registered_once_and_takes_its_files", [] {
    register_n64_lane();
    register_n64_lane();   // (twice is fine)
    CB_CHECK(machine_lanes().find("n64") != nullptr);
    fs::ScopedTempDir dir("cb_n64_lane");
    for (const char* name : {"a.z64", "b.v64", "c.n64"}) {
        fs::write_file(dir.path() / name, nt::test_rom(name[2] == 'v' ? formats::n64::ByteOrder::V64 : name[2] == 'n' ? formats::n64::ByteOrder::N64 : formats::n64::ByteOrder::Z64));
        CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / name), "n64");
    }
    fs::write_file(dir.path() / "other.bin", Bytes(0x2000, 0x41));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "other.bin"), "");
});

CB_TEST("engine.machine_n64.functions_run_in_the_r4300_interpreter", [] {
    fs::ScopedTempDir dir("cb_n64_lane");
    const stdfs::path rom = dir.path() / "a.z64";
    fs::write_file(rom, nt::test_rom());
    const vl::ImageLaneSpec spec = n64_lane_spec();
    CB_CHECK_EQ(spec.kind, "n64");
    CB_CHECK_EQ(spec.states.size(), std::size_t{4});
    CB_CHECK_EQ(spec.what.size(), std::size_t{10});
    CB_CHECK_EQ(spec.key_digits, 8u);
    // the entry runs to its return: it calls `print`, reads MI_INTR_MASK, writes VI_ORIGIN, polls PI_STATUS, does the FPU compare
    const vl::Effects entry = spec.effects(rom, 0x80001000, spec.states[0]);
    CB_CHECK_EQ(entry.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(entry[0]), std::string("return"));
    const auto& io = std::get<std::vector<std::string>>(entry[9]);
    CB_CHECK_EQ(io.size(), std::size_t{3});
    CB_CHECK_EQ(io[0], std::string("r4 430000c 0"));   // MI_INTR_MASK
    CB_CHECK_EQ(io[1], std::string("w4 4400004 0"));   // VI_ORIGIN, written with what was read
    CB_CHECK_EQ(io[2], std::string("r4 4600010 0"));   // PI_STATUS: ready, so the poll falls through
    // `helper` shifts its argument left by 36 bits, in all 64 of them (the state's A0 is sign-extended into its register)
    const vl::Written w = spec.writer(rom, dir.path() / "out");
    CB_CHECK(w.linked && w.same);
    const std::uint32_t helper = helper_of(w);
    CB_CHECK(helper != 0);
    for (const vl::State& state : spec.states) {
        const vl::Effects a = spec.effects(rom, helper, state);
        CB_CHECK_EQ(std::get<std::string>(a[0]), std::string("return"));
        // the rebuilt ROM behaves the same from every state
        CB_CHECK(a == spec.effects(w.built, helper, state));
    }
    const vl::Effects shifted = spec.effects(rom, helper, spec.states[1]);   // A0 = $80600000
    CB_CHECK_EQ(std::get<std::int64_t>(shifted[1]), static_cast<std::int64_t>(0xFFFFFFFF80600000ull << 36));
});

CB_TEST("engine.machine_n64.an_edited_function_runs_differently", [] {
    fs::ScopedTempDir dir("cb_n64_lane");
    std::string changed = nt::program();
    changed.replace(changed.find("dsll32 $v0, $a0, 4"), 18, "dsll32 $v0, $a0, 5");
    fs::write_file(dir.path() / "a.z64", nt::test_rom());
    fs::write_file(dir.path() / "b.z64", nt::test_rom(formats::n64::ByteOrder::Z64, 0, changed));
    const vl::ImageLaneSpec spec = n64_lane_spec();
    const vl::Written w = spec.writer(dir.path() / "a.z64", dir.path() / "out");
    const std::uint32_t helper = helper_of(w);
    // helper's effect differs in V0 (and only the functions that were changed differ: the entry is the same in both)
    const vl::Effects a = spec.effects(dir.path() / "a.z64", helper, spec.states[1]);
    const vl::Effects b = spec.effects(dir.path() / "b.z64", helper, spec.states[1]);
    CB_CHECK(a != b);
    CB_CHECK_EQ(std::get<std::int64_t>(a[1]), static_cast<std::int64_t>(0xFFFFFFFF80600000ull << 36));
    CB_CHECK_EQ(std::get<std::int64_t>(b[1]), static_cast<std::int64_t>(0xFFFFFFFF80600000ull << 37));
    CB_CHECK(spec.effects(dir.path() / "a.z64", 0x80001000, spec.states[1]) == spec.effects(dir.path() / "b.z64", 0x80001000, spec.states[1]));
});

CB_TEST("engine.machine_n64.the_writer_rebuilds_the_rom_identical", [] {
    fs::ScopedTempDir dir("cb_n64_lane");
    const stdfs::path rom = dir.path() / "a.v64";
    fs::write_file(rom, nt::test_rom(formats::n64::ByteOrder::V64));
    const vl::Written w = n64_lane_spec().writer(rom, dir.path() / "out");
    CB_CHECK(w.linked && w.same);
    CB_CHECK_EQ(w.functions.size(), std::size_t{4});
});
