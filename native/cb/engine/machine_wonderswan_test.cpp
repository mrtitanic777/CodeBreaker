// cb-unity: alone
// The WonderSwan lane: registered, a cartridge comes back identical (where NASM is), and an edited program's functions are run beside the original's on
// the x86 engine over the console memory, port accesses compared. The program is written byte by byte here.
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_wonderswan.hpp"
#include "cb/source/wonderswan_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// F000:0000 mov ax, 0x1234 ; out DISPLAY_CTRL, al ; call sub ; jmp start.   sub F000:000E: in al, KEY_SCAN ; mov [0x800], al ; ret.
Bytes program() {
    Bytes rom(0x10000, 0xFF);
    const Bytes code = {0xB8, 0x34, 0x12, 0xE6, 0x00, 0xE8, 0x06, 0x00, 0xEB, 0xF6, 0x90, 0x90, 0x90, 0x90, 0xE4, 0xB5, 0xA2, 0x00, 0x08, 0xC3};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes footer = {0xEA, 0x00, 0x00, 0x00, 0xF0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    std::copy(footer.begin(), footer.end(), rom.end() - 16);
    return rom;
}

}  // namespace

CB_TEST("engine.machine_wonderswan.is_registered_and_takes_its_files", [] {
    register_wonderswan_lane();
    register_wonderswan_lane();
    CB_CHECK(machine_lanes().find("wonderswan") != nullptr);
    fs::ScopedTempDir dir("cb_ws_reg");
    fs::write_file(dir.path() / "a.ws", program());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.ws"), "wonderswan");
    CB_CHECK_EQ(wonderswan_lane_spec().kind, "wonderswan");
});

CB_TEST("engine.machine_wonderswan.an_edit_diverges_and_a_function_agrees_with_itself", [] {
    fs::ScopedTempDir dir("cb_ws_lane");
    const verify::lanes::ImageLane lane(wonderswan_lane_spec());
    const stdfs::path original = dir.path() / "game.ws";
    fs::write_file(original, program());
    Bytes edited = program();
    edited[0x10] = 0xA2;
    edited[0x11] = 0x01;   // mov [0x801], al instead of [0x800]: another byte of RAM
    fs::write_file(dir.path() / "edited.ws", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0xF000E).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.ws", 0xF000E);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    Bytes port = program();
    port[0x4] = 0x01;   // out BACK_COLOR instead of DISPLAY_CTRL in the start function
    fs::write_file(dir.path() / "port.ws", port);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "port.ws", 0xF0000).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "port.ws", 0xF000E).verdict == verify::Verdict::Equivalent);
});

CB_TEST("engine.machine_wonderswan.a_cartridge_comes_back_identical", [] {
    fs::ScopedTempDir dir("cb_ws_lane2");
    const verify::lanes::ImageLane lane(wonderswan_lane_spec());
    const stdfs::path original = dir.path() / "game.ws";
    fs::write_file(original, program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);
});
