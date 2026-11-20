// cb-unity: alone
// The Nintendo 3DS lane: registered, a small 3DSX (written word by word here) comes back identical, its kernel call is named, and an edited program's
// functions are run beside the original's in the ARM interpreter over the process's memory.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_n3ds.hpp"
#include "formats/n3ds.hpp"
#include "source/n3ds_writer.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

void put(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}

// header (32 bytes) + three relocation headers (24) + code at $38: entry: push {r4, lr}; mov r4, #1; bl sub; pop {r4, pc}; nop;
// sub $4C: ldr r0, [r1]; svc 0x32; strb r0, [r2, #1]; bx lr.
Bytes x3dsx() {
    Bytes f(0x5C, 0);
    f[0] = '3';
    f[1] = 'D';
    f[2] = 'S';
    f[3] = 'X';
    f[4] = 0x20;
    f[6] = 8;
    put(f, 16, 0x24);   // the code's size
    put(f, 0x38, 0xE92D4010);
    put(f, 0x3C, 0xE3A04001);
    put(f, 0x40, 0xEB000001);
    put(f, 0x44, 0xE8BD8010);
    put(f, 0x48, 0xE1A00000);
    put(f, 0x4C, 0xE5910000);
    put(f, 0x50, 0xEF000032);
    put(f, 0x54, 0xE5C20001);
    put(f, 0x58, 0xE12FFF1E);
    return f;
}

}  // namespace

CB_TEST("engine.machine_n3ds.is_registered_and_takes_its_files", [] {
    register_n3ds_lane();
    register_n3ds_lane();
    CB_CHECK(machine_lanes().find("n3ds") != nullptr);
    fs::ScopedTempDir dir("cb_3ds_reg");
    fs::write_file(dir.path() / "app.3dsx", x3dsx());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "app.3dsx"), "n3ds");
    CB_CHECK_EQ(n3ds_lane_spec().kind, "n3ds");
    CB_CHECK(formats::n3ds::encrypted_container(Bytes(0x200, 0)) == std::nullopt);
});

CB_TEST("engine.machine_n3ds.a_3dsx_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_3ds_lane");
    const verify::lanes::ImageLane lane(n3ds_lane_spec());
    const stdfs::path original = dir.path() / "app.3dsx";
    fs::write_file(original, x3dsx());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    CB_CHECK(fs::read_text(image.written.source).find("svcSendSyncRequest") != std::string::npos);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = x3dsx();
    put(edited, 0x54, 0xE5C20002);   // strb r0, [r2, #2]
    fs::write_file(dir.path() / "edited.3dsx", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x4C).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.3dsx", 0x4C);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
});
