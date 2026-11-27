// cb-unity: alone
// The CD-i lane: registered, a disc comes back identical, and an edited module's functions are run beside the original's over the player memory,
// system calls and device accesses compared.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_cdi.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/cdi_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::cdi::test_image;

CB_TEST("engine.machine_cdi.is_registered_and_takes_its_files", [] {
    register_cdi_lane();
    register_cdi_lane();
    CB_CHECK(machine_lanes().find("cdi") != nullptr);
    fs::ScopedTempDir dir("cb_cdi_reg");
    fs::write_file(dir.path() / "a.iso", ti::disc());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.iso"), "cdi");
    CB_CHECK_EQ(cdi_lane_spec().kind, "cdi");
});

CB_TEST("engine.machine_cdi.a_disc_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_cdi_lane");
    const verify::lanes::ImageLane lane(cdi_lane_spec());
    const stdfs::path original = dir.path() / "game.iso";
    fs::write_file(original, ti::disc());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == store::Verdict::Equivalent);

    // the subroutine asks the system for I_Write; the edit asks for I_Read ($89) instead: the logged call differs
    Bytes edited = ti::disc();
    edited[4096 + 0x53] = 0x89;
    fs::write_file(dir.path() / "edited.iso", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x10050).verdict == store::Verdict::Equivalent);
    const store::Judgement j = lane.judge_function(false, original, dir.path() / "edited.iso", 0x10050);
    CB_CHECK(j.verdict == store::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    // the colour written to the video chip in the entry function
    Bytes colour = ti::disc();
    colour[4096 + 0x43] = 0x35;
    fs::write_file(dir.path() / "colour.iso", colour);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "colour.iso", 0x10040).verdict == store::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "colour.iso", 0x10050).verdict == store::Verdict::Equivalent);
});
