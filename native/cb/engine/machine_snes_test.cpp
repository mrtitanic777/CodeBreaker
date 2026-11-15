// The Super Nintendo lane: registered in the machine lane table, a cartridge comes back identical, and an edited program's functions are run beside the
// original's in the 65C816 over the console's memory, chip accesses compared.
#include "cb/engine/machine_snes.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/snes_test_image.hpp"
#include "cb/source/snes_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace img = cb::formats::snes_test_image;

namespace {

constexpr std::uint32_t kSub8040 = 0x008040 | source::snes_writer::kStartsNative | source::snes_writer::kStartsWith16BitIndex;
constexpr std::uint32_t kSub8050 = 0x008050 | source::snes_writer::kStartsNative | source::snes_writer::kStartsWith16BitIndex;

}  // namespace

CB_TEST("engine.machine_snes.is_registered_once_and_takes_its_files", [] {
    register_snes_lane();
    register_snes_lane();   // (twice is fine)
    CB_CHECK(machine_lanes().find("snes") != nullptr);
    fs::ScopedTempDir dir("cb_snes_lane_reg");
    fs::write_file(dir.path() / "a.sfc", img::cartridge());
    fs::write_file(dir.path() / "b.bin", Bytes(100, 0x11));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.sfc"), "snes");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "b.bin"), "");
    CB_CHECK_EQ(snes_lane_spec().kind, "snes");
});

CB_TEST("engine.machine_snes.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_snes_lane");
    const verify::lanes::ImageLane lane(snes_lane_spec());
    const stdfs::path original = dir.path() / "game.sfc";
    fs::write_file(original, img::cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 7u);   // reset, two subroutines and the four handlers
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), 7u);
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes ram = img::cartridge();
    ram[0x53] = 0xCE;   // sub_008050: LDA #$ABCE, so the RAM it leaves differs
    fs::write_file(dir.path() / "ram.sfc", ram);
    CB_CHECK(lane.judge_function(false, original, original, kSub8050).verdict == verify::Verdict::Equivalent);   // run twice: agree
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "ram.sfc", kSub8050);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, dir.path() / "ram.sfc", kSub8040).verdict == verify::Verdict::Equivalent);   // the one nobody edited

    Bytes data = img::cartridge();
    data[0x1010] = 0x77;   // the byte sub_008040 reads with LDA $009000,X (X = $10) and writes to VMDATAL
    fs::write_file(dir.path() / "data.sfc", data);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "data.sfc", kSub8040).verdict == verify::Verdict::Divergent);

    Bytes screen = img::cartridge();
    screen[0x0F] = 0x80;   // reset: LDA #$80, so INIDISP gets another byte (the function then loops: both runs end at the step limit)
    fs::write_file(dir.path() / "screen.sfc", screen);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "screen.sfc", 0x008000).verdict == verify::Verdict::Divergent);

    Bytes port = img::cartridge();
    port[0x4A] = 0x19;   // sub_008040: STA $2119 (VMDATAH) instead of $2118
    fs::write_file(dir.path() / "port.sfc", port);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "port.sfc", kSub8040).verdict == verify::Verdict::Divergent);
});
