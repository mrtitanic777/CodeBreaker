// cb-unity: alone
// The Odyssey 2 lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, port and data accesses
// compared. The program is the small one of formats/o2_test_image.hpp.
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_o2.hpp"
#include "cb/formats/o2_test_image.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::o2::test_image;

CB_TEST("engine.machine_o2.is_registered_and_takes_its_files", [] {
    register_o2_lane();
    register_o2_lane();
    CB_CHECK(machine_lanes().find("o2") != nullptr);
    fs::ScopedTempDir dir("cb_o2_reg");
    fs::write_file(dir.path() / "a.o2", ti::cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.o2"), "o2");
    CB_CHECK_EQ(o2_lane_spec().kind, "o2");
});

CB_TEST("engine.machine_o2.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_o2_lane");
    const verify::lanes::ImageLane lane(o2_lane_spec());
    const stdfs::path original = dir.path() / "game.o2";
    fs::write_file(original, ti::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ti::cartridge();
    edited[0x21] = 0x07;   // DEC A instead of INC A in the subroutine
    fs::write_file(dir.path() / "edited.o2", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x420).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.o2", 0x420);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes data = ti::cartridge();
    data[4] = 0x91;   // MOVX @R1,A instead of @R0: another data-space address written in the start function
    fs::write_file(dir.path() / "data.o2", data);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "data.o2", 0x400).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "data.o2", 0x420).verdict == verify::Verdict::Equivalent);
});
