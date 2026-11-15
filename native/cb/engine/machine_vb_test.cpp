// cb-unity: alone
// The Virtual Boy lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, device
// accesses compared. The program is the small one of formats/vb_test_image.hpp.
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_vb.hpp"
#include "cb/formats/vb_test_image.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::vb::test_image;

CB_TEST("engine.machine_vb.is_registered_and_takes_its_files", [] {
    register_vb_lane();
    register_vb_lane();
    CB_CHECK(machine_lanes().find("vb") != nullptr);
    fs::ScopedTempDir dir("cb_vb_reg");
    fs::write_file(dir.path() / "a.vb", ti::cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.vb"), "vb");
    CB_CHECK_EQ(vb_lane_spec().kind, "vb");
});

CB_TEST("engine.machine_vb.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_vb_lane");
    const verify::lanes::ImageLane lane(vb_lane_spec());
    const stdfs::path original = dir.path() / "game.vb";
    fs::write_file(original, ti::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ti::cartridge();
    edited[0x32] = 0x42;   // shl 2 instead of shl 1 in the subroutine: another value in r10
    fs::write_file(dir.path() / "edited.vb", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x07000030).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.vb", 0x07000030);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes vip = ti::cartridge();
    vip[0x12 + 2] = 0x20;   // another register of the VIP read: ld.h -2016[r8],r9 (the displacement's low byte) -- the access log differs
    fs::write_file(dir.path() / "vip.vb", vip);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "vip.vb", 0x07003FF0).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "vip.vb", 0x07000030).verdict == verify::Verdict::Equivalent);
});
