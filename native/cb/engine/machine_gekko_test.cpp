// cb-unity: alone (file-local names that other files in a batch also use)
// The GameCube, Wii and Wii U lanes: registered in the machine lane table, each takes its own console's files, a program comes back identical,
// and the functions of a program are run in the PowerPC interpreter over its memory, the same in the original and in a rebuilt copy and different
// when an instruction is changed.
#include "cb/engine/machine_gekko.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/source/gekko_test_images.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace gt = cb::source::gekko_test;
namespace vl = cb::verify::lanes;

CB_TEST("engine.machine_gekko.is_registered_once_and_takes_its_files", [] {
    register_gekko_lanes();
    register_gekko_lanes();   // (twice is fine)
    for (const char* name : {"gamecube", "wii", "wiiu"}) CB_CHECK(machine_lanes().find(name) != nullptr);
    fs::ScopedTempDir dir("cb_gekko_lane");
    fs::write_file(dir.path() / "gc.dol", gt::dol(false));
    fs::write_file(dir.path() / "wii.dol", gt::dol(true));
    fs::write_file(dir.path() / "gc.elf", gt::elf(false, false));
    fs::write_file(dir.path() / "wii.elf", gt::elf(false, true));
    fs::write_file(dir.path() / "u.rpx", gt::elf(true));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "gc.dol"), "gamecube");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "wii.dol"), "wii");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "gc.elf"), "gamecube");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "wii.elf"), "wii");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "u.rpx"), "wiiu");
    fs::write_file(dir.path() / "other.bin", Bytes(0x2000, 0x41));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "other.bin"), "");
});

CB_TEST("engine.machine_gekko.functions_run_in_the_powerpc_interpreter", [] {
    fs::ScopedTempDir dir("cb_gekko_lane");
    const stdfs::path dol = dir.path() / "a.dol";
    fs::write_file(dol, gt::dol());
    const vl::ImageLaneSpec spec = gekko_lane_spec("gamecube");
    CB_CHECK_EQ(spec.kind, "gamecube");
    CB_CHECK_EQ(spec.states.size(), std::size_t{4});
    CB_CHECK_EQ(spec.what.size(), std::size_t{9});
    CB_CHECK_EQ(spec.key_digits, 8u);
    // the entry runs to its return: it calls helper, which adds the first byte of the string ('H', 0x48) to r3; it reads the video interface's first register
    const vl::Effects entry = spec.effects(dol, gt::kText, spec.states[0]);
    CB_CHECK_EQ(entry.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(entry[0]), std::string("ret"));
    CB_CHECK_EQ(std::get<std::int64_t>(entry[1]), std::int64_t{0x80004048});
    const auto& io = std::get<std::vector<std::string>>(entry[8]);
    CB_CHECK_EQ(io.size(), std::size_t{1});
    CB_CHECK_EQ(io[0], std::string("read cc002000 4 0"));
    // the callback returns 7; helper adds the first byte of what r3 points at (from the state with a filled work area: the word 0x60000100)
    CB_CHECK_EQ(std::get<std::int64_t>(spec.effects(dol, gt::kCallback, spec.states[0])[1]), std::int64_t{7});
    const vl::Effects helper = spec.effects(dol, gt::kHelper, spec.states[1]);
    CB_CHECK_EQ(std::get<std::string>(helper[0]), std::string("ret"));
    CB_CHECK_EQ(std::get<std::int64_t>(helper[1]), std::int64_t{0x60000060});
    // the rebuilt program behaves the same from every state
    const vl::Written w = spec.writer(dol, dir.path() / "out");
    CB_CHECK(w.linked && w.same);
    for (const vl::State& state : spec.states)
        for (const std::uint32_t entry_at : {gt::kText, gt::kHelper, gt::kCallback}) CB_CHECK(spec.effects(dol, entry_at, state) == spec.effects(w.built, entry_at, state));
});

CB_TEST("engine.machine_gekko.an_edited_function_runs_differently", [] {
    fs::ScopedTempDir dir("cb_gekko_lane");
    Bytes changed = gt::dol();
    changed[0x100 + (gt::kCallback - gt::kText) + 3] = 8;   // li r3,7 -> li r3,8
    fs::write_file(dir.path() / "a.dol", gt::dol());
    fs::write_file(dir.path() / "b.dol", changed);
    const vl::ImageLaneSpec spec = gekko_lane_spec("gamecube");
    const vl::Effects a = spec.effects(dir.path() / "a.dol", gt::kCallback, spec.states[0]);
    const vl::Effects b = spec.effects(dir.path() / "b.dol", gt::kCallback, spec.states[0]);
    CB_CHECK(a != b);
    CB_CHECK_EQ(std::get<std::int64_t>(a[1]), std::int64_t{7});
    CB_CHECK_EQ(std::get<std::int64_t>(b[1]), std::int64_t{8});
    // the functions that were not changed run the same
    CB_CHECK(spec.effects(dir.path() / "a.dol", gt::kHelper, spec.states[1]) == spec.effects(dir.path() / "b.dol", gt::kHelper, spec.states[1]));
    CB_CHECK(spec.effects(dir.path() / "a.dol", gt::kText, spec.states[2]) == spec.effects(dir.path() / "b.dol", gt::kText, spec.states[2]));
});

CB_TEST("engine.machine_gekko.each_console_logs_its_own_hardware", [] {
    fs::ScopedTempDir dir("cb_gekko_lane");
    // the Wii program also reads the Hollywood's register block (lis r7,0xCD80 only builds an address: no access): the same log as the GameCube's
    fs::write_file(dir.path() / "wii.dol", gt::dol(true));
    const vl::ImageLaneSpec wii = gekko_lane_spec("wii");
    CB_CHECK_EQ(wii.kind, "wii");
    const vl::Effects e = wii.effects(dir.path() / "wii.dol", gt::kText, wii.states[0]);
    CB_CHECK_EQ(std::get<std::string>(e[0]), std::string("ret"));
    CB_CHECK_EQ(std::get<std::vector<std::string>>(e[8]).size(), std::size_t{1});
    // on the Wii U the video interface's window is no hardware: the read is memory, and logs nothing
    fs::write_file(dir.path() / "u.rpx", gt::elf(true));
    const vl::ImageLaneSpec wiiu = gekko_lane_spec("wiiu");
    CB_CHECK_EQ(wiiu.kind, "wiiu");
    const vl::Effects u = wiiu.effects(dir.path() / "u.rpx", gt::kText, wiiu.states[0]);
    CB_CHECK_EQ(std::get<std::string>(u[0]), std::string("ret"));
    CB_CHECK(std::get<std::vector<std::string>>(u[8]).empty());
});

CB_TEST("engine.machine_gekko.the_writer_rebuilds_each_format_identical", [] {
    fs::ScopedTempDir dir("cb_gekko_lane");
    fs::write_file(dir.path() / "a.dol", gt::dol());
    fs::write_file(dir.path() / "a.elf", gt::elf());
    fs::write_file(dir.path() / "a.rpx", gt::elf(true));
    for (const char* name : {"a.dol", "a.elf", "a.rpx"}) {
        const vl::Written w = gekko_lane_spec(name[2] == 'r' ? "wiiu" : "gamecube").writer(dir.path() / name, dir.path() / (std::string(name) + ".out"));
        CB_CHECK(w.linked && w.same);
        CB_CHECK_EQ(w.functions.size(), std::size_t{3});
    }
});
