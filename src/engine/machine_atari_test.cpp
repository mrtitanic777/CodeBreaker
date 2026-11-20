// The Atari 2600 and 7800 lanes: registered in the machine lane table, a cartridge comes back identical, and an edited program's functions are run beside
// the original's in the 6502 over the machine's memory, hardware accesses compared. The programs are written byte by byte here.
#include "engine/machine_atari.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_atari2600.hpp"
#include "engine/machine_atari7800.hpp"
#include "engine/machine_lanes.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// 2600: reset $F000: LDA #$0E, STA COLUBK, JSR $F010, JMP $F000; sub $F010: LDA INTIM, STA $80, RTS; vectors at $FFC.
Bytes a2600_program() {
    Bytes rom(0x1000, 0xFF);
    const Bytes code = {0xA9, 0x0E, 0x85, 0x09, 0x20, 0x10, 0xF0, 0x4C, 0x00, 0xF0};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes sub = {0xAD, 0x84, 0x02, 0x85, 0x80, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x10);
    const Bytes vectors = {0x00, 0xF0, 0x00, 0xF0};
    std::copy(vectors.begin(), vectors.end(), rom.begin() + 0xFFC);
    return rom;
}

// 2600, F8: each bank's reset touches a hotspot and returns: bank 0 $1FF9 (selects bank 1), bank 1 $1FF8 (selects bank 0).
Bytes a2600_f8_program() {
    Bytes rom;
    for (std::size_t b = 0; b < 2; ++b) {
        Bytes bank(0x1000, 0xFF);
        const Bytes code = {0xAD, static_cast<std::uint8_t>(b == 0 ? 0xF9 : 0xF8), 0x1F, 0x60};
        std::copy(code.begin(), code.end(), bank.begin());
        const Bytes vectors = {0x00, 0xF0, 0x00, 0xF0};
        std::copy(vectors.begin(), vectors.end(), bank.begin() + 0xFFC);
        rom.insert(rom.end(), bank.begin(), bank.end());
    }
    return rom;
}

// 7800 (16 KB at $C000): reset: SEI CLD LDA #$40 STA CTRL JSR $C020 JMP $C006; sub $C020: LDA MSTAT, BPL, STA.w WSYNC, RTS; nmi/irq $C030: RTI.
Bytes a7800_program() {
    Bytes file(128, 0);
    file[0] = 1;
    const std::string magic = "ATARI7800";
    std::copy(magic.begin(), magic.end(), file.begin() + 1);
    const std::string title = "TEST";
    std::copy(title.begin(), title.end(), file.begin() + 17);
    file[49] = 0;
    file[50] = 0;
    file[51] = 0x40;
    file[52] = 0;
    Bytes rom(0x4000, 0xFF);
    const Bytes code = {0x78, 0xD8, 0xA9, 0x40, 0x85, 0x3C, 0x20, 0x20, 0xC0, 0x4C, 0x06, 0xC0};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes sub = {0xA5, 0x28, 0x10, 0xFC, 0x8D, 0x24, 0x00, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x20);
    rom[0x30] = 0x40;
    const Bytes vectors = {0x30, 0xC0, 0x00, 0xC0, 0x30, 0xC0};
    std::copy(vectors.begin(), vectors.end(), rom.begin() + 0x3FFA);
    file.insert(file.end(), rom.begin(), rom.end());
    return file;
}

}  // namespace

CB_TEST("engine.machine_atari.are_registered_once_and_take_their_files", [] {
    register_atari_lanes();
    register_atari_lanes();   // (twice is fine)
    CB_CHECK(machine_lanes().find("atari2600") != nullptr);
    CB_CHECK(machine_lanes().find("atari7800") != nullptr);
    CB_CHECK(machine_lanes().find("lynx") != nullptr);
    fs::ScopedTempDir dir("cb_atari_lanes");
    fs::write_file(dir.path() / "a.a26", a2600_program());
    fs::write_file(dir.path() / "b.a78", a7800_program());
    fs::write_file(dir.path() / "c.bin", Bytes(100, 0x11));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.a26"), "atari2600");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "b.a78"), "atari7800");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "c.bin"), "");
    CB_CHECK_EQ(atari2600_lane_spec().kind, "atari2600");
    CB_CHECK_EQ(atari7800_lane_spec().kind, "atari7800");
});

CB_TEST("engine.machine_atari2600.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_a2600_lane");
    const verify::lanes::ImageLane lane(atari2600_lane_spec());
    const stdfs::path original = dir.path() / "game.a26";
    fs::write_file(original, a2600_program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);   // reset and the subroutine
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), 2u);
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = a2600_program();
    edited[1] = 0x1E;   // LDA #$1E: COLUBK gets another colour
    const stdfs::path changed = dir.path() / "edited.a26";
    fs::write_file(changed, edited);
    CB_CHECK(lane.judge_function(false, original, original, 0xF000).verdict == verify::Verdict::Equivalent);   // run twice: agree
    const verify::Judgement j = lane.judge_function(false, original, changed, 0xF000);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, changed, 0xF010).verdict == verify::Verdict::Equivalent);   // the subroutine nobody edited
    // the same edit in the subroutine's RAM store: another zero-page address changes the RAM compared
    Bytes ram_edit = a2600_program();
    ram_edit[0x14] = 0x81;
    fs::write_file(dir.path() / "ram.a26", ram_edit);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "ram.a26", 0xF010).verdict == verify::Verdict::Divergent);
});

CB_TEST("engine.machine_atari2600.a_bank_switch_is_an_effect", [] {
    fs::ScopedTempDir dir("cb_a2600_f8_lane");
    const verify::lanes::ImageLane lane(atari2600_lane_spec());
    const stdfs::path original = dir.path() / "f8.a26";
    fs::write_file(original, a2600_f8_program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);   // one reset per bank
    Bytes edited = a2600_f8_program();
    edited[1] = 0xF8;   // bank 0 touches $1FF8 (its own bank) instead of $1FF9
    fs::write_file(dir.path() / "edited.a26", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x0F000).verdict == verify::Verdict::Equivalent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.a26", 0x0F000).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.a26", 0x1F000).verdict == verify::Verdict::Equivalent);   // bank 1 is as it was
});

CB_TEST("engine.machine_atari7800.a_cartridge_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_a7800_lane");
    const verify::lanes::ImageLane lane(atari7800_lane_spec());
    const stdfs::path original = dir.path() / "game.a78";
    fs::write_file(original, a7800_program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);   // reset, the subroutine, the interrupt handler
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = a7800_program();
    edited[128 + 0x25] = 0x25;   // STA $0025 (P1C1) instead of WSYNC
    fs::write_file(dir.path() / "edited.a78", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0xC020).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.a78", 0xC020);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.a78", 0xC030).verdict == verify::Verdict::Equivalent);
});
