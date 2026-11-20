// cb-unity: alone
// The Jaguar lane: registered, a cartridge comes back identical, and an edited program's functions are run beside the original's over the
// machine memory, Tom and Jerry accesses compared. The program is written byte by byte here.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_atari.hpp"
#include "engine/machine_jaguar.hpp"
#include "engine/machine_lanes.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// start: MOVE.W #$1234,BG ; JSR sub ; BRA start.   sub: MOVE.L VC,D0 ; MOVE.W D0,(A0) ; RTS.
Bytes code(std::uint32_t base) {
    Bytes c(0x30, 0xFF);
    const Bytes start = {0x33, 0xFC, 0x12, 0x34, 0x00, 0xF0, 0x00, 0x58, 0x4E, 0xB9, 0, 0, 0, 0, 0x60, 0xF0};
    std::copy(start.begin(), start.end(), c.begin());
    put32(c, 10, base + 0x20);
    const Bytes sub = {0x20, 0x39, 0x00, 0xF0, 0x00, 0x06, 0x30, 0x80, 0x4E, 0x75};
    std::copy(sub.begin(), sub.end(), c.begin() + 0x20);
    return c;
}

Bytes cartridge() {
    Bytes rom(0x4000, 0xFF);
    put32(rom, 0x400, 0x04040404);
    put32(rom, 0x404, 0x802000);
    const Bytes c = code(0x802000);
    std::copy(c.begin(), c.end(), rom.begin() + 0x2000);
    return rom;
}

}  // namespace

CB_TEST("engine.machine_jaguar.is_registered_and_takes_its_files", [] {
    register_atari_lanes();
    register_atari_lanes();
    CB_CHECK(machine_lanes().find("jaguar") != nullptr);
    fs::ScopedTempDir dir("cb_jag_reg");
    fs::write_file(dir.path() / "a.j64", cartridge());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.j64"), "jaguar");
    CB_CHECK_EQ(jaguar_lane_spec().kind, "jaguar");
});

CB_TEST("engine.machine_jaguar.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_jag_lane");
    const verify::lanes::ImageLane lane(jaguar_lane_spec());
    const stdfs::path original = dir.path() / "game.j64";
    fs::write_file(original, cartridge());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = cartridge();
    edited[0x2020] = 0x22;   // MOVE.L VC,D1 instead of D0
    fs::write_file(dir.path() / "edited.j64", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x802020).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.j64", 0x802020);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());

    Bytes colour = cartridge();
    colour[0x2003] = 0x35;   // another value written to BG: the start function diverges, the subroutine does not
    fs::write_file(dir.path() / "colour.j64", colour);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "colour.j64", 0x802020).verdict == verify::Verdict::Equivalent);
});
