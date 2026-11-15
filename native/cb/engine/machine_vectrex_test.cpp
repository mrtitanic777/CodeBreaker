// cb-unity: alone
// The Vectrex lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the machine memory, VIA accesses
// and BIOS calls compared. The program is written byte by byte.
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_vectrex.hpp"
#include "cb/formats/vectrex_test_image.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ti = cb::formats::vectrex::test_image;

CB_TEST("engine.machine_vectrex.is_registered_and_takes_its_files", [] {
    register_vectrex_lane();
    register_vectrex_lane();
    CB_CHECK(machine_lanes().find("vectrex") != nullptr);
    fs::ScopedTempDir dir("cb_vec_reg");
    fs::write_file(dir.path() / "a.vec", ti::cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.vec"), "vectrex");
    CB_CHECK_EQ(vectrex_lane_spec().kind, "vectrex");
});

CB_TEST("engine.machine_vectrex.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_vec_lane");
    const verify::lanes::ImageLane lane(vectrex_lane_spec());
    const stdfs::path original = dir.path() / "game.vec";
    fs::write_file(original, ti::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ti::cartridge();
    edited[0x28] = 0x4A;   // DECA instead of INCA in the subroutine: another value in RAM
    fs::write_file(dir.path() / "edited.vec", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x25).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.vec", 0x25);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes via = ti::cartridge();
    via[0x1B] = 0x7E;   // another value written to the VIA in the start function; the subroutine is as it was
    fs::write_file(dir.path() / "via.vec", via);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "via.vec", 0x17).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "via.vec", 0x25).verdict == verify::Verdict::Equivalent);
    Bytes bios = ti::cartridge();
    bios[0x19] = 0xA2;   // JSR Intensity_a-ish ($F1A2: Set_Refresh) instead of Wait_Recal: the BIOS call differs
    fs::write_file(dir.path() / "bios.vec", bios);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "bios.vec", 0x17).verdict == verify::Verdict::Divergent);
});
