// cb-unity: alone
// The Channel F lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, port and data accesses
// compared. The program is the small one of formats/chf_test_image.hpp.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_chf.hpp"
#include "formats/chf_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::chf::test_image;

CB_TEST("engine.machine_chf.is_registered_and_takes_its_files", [] {
    register_chf_lane();
    register_chf_lane();
    CB_CHECK(machine_lanes().find("chf") != nullptr);
    fs::ScopedTempDir dir("cb_chf_reg");
    fs::write_file(dir.path() / "a.chf", ti::cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.chf"), "chf");
    CB_CHECK_EQ(chf_lane_spec().kind, "chf");
});

CB_TEST("engine.machine_chf.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_chf_lane");
    const verify::lanes::ImageLane lane(chf_lane_spec());
    const stdfs::path original = dir.path() / "game.chf";
    fs::write_file(original, ti::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ti::cartridge();
    edited[0x22] = 0x04;   // AI $04 instead of AI $03 in the subroutine
    fs::write_file(dir.path() / "edited.chf", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x820).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.chf", 0x820);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes data = ti::cartridge();
    data[6] = 0x02;   // OUT $02 instead of OUT $01: another port written in the start function
    fs::write_file(dir.path() / "data.chf", data);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "data.chf", 0x800).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "data.chf", 0x820).verdict == verify::Verdict::Equivalent);
});
