// cb-unity: alone
// The 3DO lane: registered, a bare AIF comes back identical, and an edited program's functions are run beside the original's as a big-endian ARM over
// the console's memory. The program is written word by word here.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_threedo.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

void put_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// entry $40: push {r4, lr} ; mov r4, #1 ; bl sub ; ldr r1, [pc, #4] ; pop {r4, pc} ; data ; the literal $03400014 ; sub $5C: ldr r0, [r1] ; strb r0, [r2] ; mov pc, lr.
Bytes aif() {
    Bytes f(0x68, 0);
    put_be(f, 0, 0xE1A00000);
    put_be(f, 4, 0xE1A00000);
    put_be(f, 8, 0xE1A00000);
    put_be(f, 12, 0xEB00000B);
    put_be(f, 16, 0xEF000011);
    put_be(f, 20, 0x68);
    put_be(f, 48, 0x20);
    put_be(f, 0x40, 0xE92D4010);
    put_be(f, 0x44, 0xE3A04001);
    put_be(f, 0x48, 0xEB000003);
    put_be(f, 0x4C, 0xE59F1004);
    put_be(f, 0x50, 0xE8BD8010);
    put_be(f, 0x54, 0x12345678);
    put_be(f, 0x58, 0x03400014);
    put_be(f, 0x5C, 0xE5910000);
    put_be(f, 0x60, 0xE5C20001);   // strb r0, [r2, #1]
    put_be(f, 0x64, 0xE1A0F00E);
    return f;
}

}  // namespace

CB_TEST("engine.machine_threedo.is_registered_and_takes_its_files", [] {
    register_threedo_lane();
    register_threedo_lane();
    CB_CHECK(machine_lanes().find("threedo") != nullptr);
    fs::ScopedTempDir dir("cb_3do_reg");
    fs::write_file(dir.path() / "LaunchMe", aif());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "LaunchMe"), "threedo");
    CB_CHECK_EQ(threedo_lane_spec().kind, "threedo");
});

CB_TEST("engine.machine_threedo.an_aif_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_3do_lane");
    const verify::lanes::ImageLane lane(threedo_lane_spec());
    const stdfs::path original = dir.path() / "LaunchMe";
    fs::write_file(original, aif());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = aif();
    put_be(edited, 0x60, 0xE5C20002);   // strb r0, [r2, #2]: the byte goes to another place (a big-endian byte lane, not a word)
    fs::write_file(dir.path() / "edited.aif", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x5C).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.aif", 0x5C);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
});
