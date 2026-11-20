// The PC Engine lane: registered in the machine lane table, a card comes back identical, and an edited program's functions are run beside the
// original's in the HuC6280 over the console's memory, chip accesses compared.
#include "engine/machine_pce.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/pce_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

CB_TEST("engine.machine_pce.is_registered_once_and_takes_its_files", [] {
    register_pce_lane();
    register_pce_lane();   // (twice is fine)
    CB_CHECK(machine_lanes().find("pce") != nullptr);
    fs::ScopedTempDir dir("cb_pce_lane_reg");
    fs::write_file(dir.path() / "a.pce", formats::pce_test_image::cartridge());
    fs::write_file(dir.path() / "b.bin", Bytes(100, 0x11));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.pce"), "pce");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "b.bin"), "");
    CB_CHECK_EQ(pce_lane_spec().kind, "pce");
});

CB_TEST("engine.machine_pce.a_card_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_pce_lane");
    const verify::lanes::ImageLane lane(pce_lane_spec());
    const stdfs::path original = dir.path() / "game.pce";
    fs::write_file(original, formats::pce_test_image::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 6u);   // reset, the subroutine, bank 1's function and the three handlers
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), 6u);
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = formats::pce_test_image::cartridge();
    edited[0x2000 + 1] = 0x3D;   // bank 1: LDA #$3D, so the VDC data port gets another byte
    const stdfs::path changed = dir.path() / "edited.pce";
    fs::write_file(changed, edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x014000).verdict == verify::Verdict::Equivalent);   // run twice: agree
    const verify::Judgement j = lane.judge_function(false, original, changed, 0x014000);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, changed, 0x00E020).verdict == verify::Verdict::Equivalent);   // the subroutine, which nothing edited touches

    Bytes written = formats::pce_test_image::cartridge();
    written[0x31] = 0x08;   // the subroutine's LDA #$08: a different value reaches VDC_SELECT
    fs::write_file(dir.path() / "written.pce", written);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "written.pce", 0x00E020).verdict == verify::Verdict::Divergent);

    Bytes copied = formats::pce_test_image::cartridge();
    copied[0x100] = 0x09;   // the data the block transfer copies: the RAM it leaves differs
    fs::write_file(dir.path() / "copied.pce", copied);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "copied.pce", 0x00E020).verdict == verify::Verdict::Divergent);

    Bytes mapped = formats::pce_test_image::cartridge();
    mapped[0x0B + 1] = 0x00;   // reset maps another bank at $4000: the mapping registers differ afterwards
    fs::write_file(dir.path() / "mapped.pce", mapped);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "mapped.pce", 0x00E000).verdict == verify::Verdict::Divergent);
});
