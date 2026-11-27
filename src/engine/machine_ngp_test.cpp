// cb-unity: alone
// The Neo Geo Pocket lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, device
// accesses compared. The program is the small one of formats/ngp_test_image.hpp.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_ngp.hpp"
#include "formats/ngp_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::ngp::test_image;

CB_TEST("engine.machine_ngp.is_registered_and_takes_its_files", [] {
    register_ngp_lane();
    register_ngp_lane();
    CB_CHECK(machine_lanes().find("ngp") != nullptr);
    fs::ScopedTempDir dir("cb_ngp_reg");
    fs::write_file(dir.path() / "a.ngp", ti::cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.ngp"), "ngp");
    CB_CHECK_EQ(ngp_lane_spec().kind, "ngp");
});

CB_TEST("engine.machine_ngp.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_ngp_lane");
    const verify::lanes::ImageLane lane(ngp_lane_spec());
    const stdfs::path original = dir.path() / "game.ngp";
    fs::write_file(original, ti::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == store::Verdict::Equivalent);

    // sub ($200080): `add WA,BC` ($D9 $80) as `sub WA,BC` ($D9 $A0): another value in WA
    Bytes edited = ti::cartridge();
    CB_CHECK_EQ(edited[0x80 + 3], 0xD9);   // (ld WA,(XSP+4) is $9F $04 $20; the add follows)
    edited[0x80 + 4] = 0xA0;
    fs::write_file(dir.path() / "edited.ngp", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x200080).verdict == store::Verdict::Equivalent);
    const store::Judgement j = lane.judge_function(false, original, dir.path() / "edited.ngp", 0x200080);
    CB_CHECK(j.verdict == store::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    // start calls the edited sub: it differs too
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.ngp", 0x200040).verdict == store::Verdict::Divergent);
    // the function that only kicks the watchdog agrees
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.ngp", 0x2000A0).verdict == store::Verdict::Equivalent);
});
