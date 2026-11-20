// cb-unity: alone
// The Intellivision lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, device accesses
// compared. The program is the small one of formats/intv_test_image.hpp.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_intv.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/intv_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::intv::test_image;

CB_TEST("engine.machine_intv.is_registered_and_takes_its_files", [] {
    register_intv_lane();
    register_intv_lane();
    CB_CHECK(machine_lanes().find("intv") != nullptr);
    fs::ScopedTempDir dir("cb_intv_reg");
    fs::write_file(dir.path() / "a.rom", ti::file());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.rom"), "intv");
    CB_CHECK_EQ(intv_lane_spec().kind, "intv");
});

CB_TEST("engine.machine_intv.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_intv_lane");
    const verify::lanes::ImageLane lane(intv_lane_spec());
    const stdfs::path original = dir.path() / "game.rom";
    fs::write_file(original, ti::file());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ti::file();
    edited[5 + 2 * 0x30 + 1] = 0x10;   // DECR R0 instead of INCR R0 in the subroutine: another value in R0
    fs::write_file(dir.path() / "edited.rom", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x5030).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.rom", 0x5030);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes stic = ti::file();
    stic[5 + 2 * 0x25 + 1] = 0x21;   // another STIC register written in the start function: the access log differs
    fs::write_file(dir.path() / "stic.rom", stic);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "stic.rom", 0x5020).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "stic.rom", 0x5030).verdict == verify::Verdict::Equivalent);
});
