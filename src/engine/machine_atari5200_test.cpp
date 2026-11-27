// cb-unity: alone
// The Atari 5200 lane: registered in the machine lane table, a cartridge comes back identical, and an edited program's functions are run beside the
// original's over the console's memory, chip accesses compared.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_atari.hpp"
#include "engine/machine_atari5200.hpp"
#include "engine/machine_lanes.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// start $8000: SEI CLD LDA #$0E STA COLBK JSR $8020 JMP $8002; sub $8020: LDA VCOUNT, STA WSYNC, STA $80, RTS.
Bytes a5200_program() {
    Bytes rom(0x4000, 0xFF);
    const Bytes code = {0x78, 0xD8, 0xA9, 0x0E, 0x8D, 0x1A, 0xC0, 0x20, 0x20, 0x80, 0x4C, 0x02, 0x80};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes sub = {0xAD, 0x0B, 0xD4, 0x8D, 0x0A, 0xD4, 0x85, 0x80, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x20);
    const Bytes tail = {0x00, 0xFF, 0x00, 0x80};
    std::copy(tail.begin(), tail.end(), rom.end() - 4);
    return rom;
}

}  // namespace

CB_TEST("engine.machine_atari5200.is_registered_and_takes_its_files", [] {
    register_atari_lanes();
    register_atari_lanes();
    CB_CHECK(machine_lanes().find("atari5200") != nullptr);
    fs::ScopedTempDir dir("cb_a5200_reg");
    fs::write_file(dir.path() / "a.a52", a5200_program());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.a52"), "atari5200");
    CB_CHECK_EQ(atari5200_lane_spec().kind, "atari5200");
});

CB_TEST("engine.machine_atari5200.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_a5200_lane");
    const verify::lanes::ImageLane lane(atari5200_lane_spec());
    const stdfs::path original = dir.path() / "game.a52";
    fs::write_file(original, a5200_program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == store::Verdict::Equivalent);

    Bytes edited = a5200_program();
    edited[0x27] = 0x81;   // STA $81 instead of $80: another RAM byte changes
    fs::write_file(dir.path() / "edited.a52", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x8020).verdict == store::Verdict::Equivalent);
    const store::Judgement j = lane.judge_function(false, original, dir.path() / "edited.a52", 0x8020);
    CB_CHECK(j.verdict == store::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());

    Bytes colour = a5200_program();
    colour[3] = 0x0F;   // another background colour written to COLBK
    fs::write_file(dir.path() / "colour.a52", colour);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "colour.a52", 0x8020).verdict == store::Verdict::Equivalent);   // (the subroutine is as it was)
});
