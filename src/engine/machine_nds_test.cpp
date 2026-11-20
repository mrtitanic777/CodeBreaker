// The Nintendo DS lane: registered in the machine lane table, a ROM comes back identical, and the functions of a ROM are run in the interpreter
// of their own processor (the ARM9's or the ARM7's, by the function's address) over the DS memory.
#include "engine/machine_nds.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "source/nds_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

CB_TEST("engine.machine_nds.is_registered_once_and_takes_its_files", [] {
    register_nds_lane();
    register_nds_lane();   // (twice is fine)
    CB_CHECK(machine_lanes().find("nds") != nullptr);
    fs::ScopedTempDir dir("cb_nds_lane");
    fs::write_file(dir.path() / "a.nds", source::nds_test::test_rom());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.nds"), "nds");
});

CB_TEST("engine.machine_nds.functions_run_in_the_interpreter_of_their_processor", [] {
    fs::ScopedTempDir dir("cb_nds_lane");
    const stdfs::path rom = dir.path() / "a.nds";
    fs::write_file(rom, source::nds_test::test_rom());
    const verify::lanes::ImageLaneSpec spec = nds_lane_spec();
    CB_CHECK_EQ(spec.kind, "nds");
    CB_CHECK_EQ(spec.states.size(), std::size_t{4});
    const auto effects = [&](std::uint32_t entry) { return spec.effects(rom, entry, spec.states[0]); };
    // the ARM9 entry runs to its return: it writes REG_IME, calls into Thumb and back, does CLZ, LDRD (two register reads) and a CP15 write
    const verify::lanes::Effects nine = effects(0x02000000);
    CB_CHECK_EQ(std::get<std::string>(nine[0]), std::string("return"));
    CB_CHECK_EQ(std::get<std::int64_t>(nine[1]), std::int64_t{30});   // r0 as the last Thumb function left it
    const auto& io = std::get<std::vector<std::string>>(nine[6]);
    CB_CHECK_EQ(io.size(), std::size_t{4});
    CB_CHECK_EQ(io.front(), std::string("w4 4000208 1"));
    CB_CHECK(io.back().starts_with("w4 fff007a4 "));
    // the ARM7 entry loops (it branches to itself): it ends at the step limit, and it wrote REG_IME too
    const verify::lanes::Effects seven = effects(0x037F8000);
    CB_CHECK_EQ(std::get<std::string>(seven[0]), std::string("limit"));
    CB_CHECK_EQ(std::get<std::vector<std::string>>(seven[6]).front(), std::string("w4 4000208 1"));
    // an address no program holds says so
    CB_CHECK_EQ(std::get<std::string>(effects(0x08000000)[0]), std::string("no program holds the address"));
});

CB_TEST("engine.machine_nds.the_writer_rebuilds_the_rom_identical", [] {
    fs::ScopedTempDir dir("cb_nds_lane");
    const stdfs::path rom = dir.path() / "a.nds";
    fs::write_file(rom, source::nds_test::test_rom());
    const verify::lanes::Written w = nds_lane_spec().writer(rom, dir.path() / "out");
    CB_CHECK(w.linked && w.same);
    CB_CHECK(w.functions.size() >= 6);
});
