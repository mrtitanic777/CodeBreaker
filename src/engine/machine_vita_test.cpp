// cb-unity: alone
// The PlayStation Vita lane: registered, a small ELF of Thumb code (written byte by byte here) comes back identical, and an edited program's functions are run beside
// the original's in the ARM interpreter over the process's memory.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_vita.hpp"
#include "formats/vita.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}

// entry: push {r4, lr}; movs r4, #1; bl sub; pop {r4, pc}; nop.n; sub $0C: ldr r0, [r1]; strb r0, [r2, #1]; bx lr.
Bytes velf(std::uint8_t store_high = 0x70) {
    const Bytes code = {0x10, 0xB5, 0x01, 0x24, 0x00, 0xF0, 0x02, 0xF8, 0x10, 0xBD, 0x00, 0xBF, 0x08, 0x68, 0x50, store_high, 0x70, 0x47};
    Bytes f(0x60 + code.size(), 0);
    f[0] = 0x7F;
    f[1] = 'E';
    f[2] = 'L';
    f[3] = 'F';
    f[4] = 1;
    f[5] = 1;
    f[6] = 1;
    f[16] = 0x00;
    f[17] = 0xFE;   // ET_SCE_EXEC
    f[18] = 40;
    put32(f, 20, 1);
    put32(f, 28, 52);
    f[40] = 52;
    f[42] = 32;
    f[44] = 1;
    put32(f, 52, 1);
    put32(f, 56, 0x60);
    put32(f, 60, 0x81000000);
    put32(f, 64, 0x81000000);
    put32(f, 68, static_cast<std::uint32_t>(code.size()));
    put32(f, 72, static_cast<std::uint32_t>(code.size()));
    put32(f, 76, 5);
    put32(f, 80, 0x1000);
    std::copy(code.begin(), code.end(), f.begin() + 0x60);
    return f;
}

}  // namespace

CB_TEST("engine.machine_vita.is_registered_and_takes_its_files", [] {
    register_vita_lane();
    register_vita_lane();
    CB_CHECK(machine_lanes().find("vita") != nullptr);
    fs::ScopedTempDir dir("cb_vita_reg");
    fs::write_file(dir.path() / "app.velf", velf());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "app.velf"), "vita");
    CB_CHECK_EQ(vita_lane_spec().kind, "vita");
    const formats::vita::Program p = formats::vita::read(dir.path() / "app.velf");
    CB_CHECK_EQ(p.segments.size(), 1u);
    CB_CHECK(p.in_code(0));
});

CB_TEST("engine.machine_vita.an_elf_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_vita_lane");
    const verify::lanes::ImageLane lane(vita_lane_spec());
    const stdfs::path original = dir.path() / "app.velf";
    fs::write_file(original, velf());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    fs::write_file(dir.path() / "edited.velf", velf(0x90));   // strb r0, [r2, #2]
    CB_CHECK(lane.judge_function(false, original, original, 0xC).verdict == verify::Verdict::Equivalent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.velf", 0xC).verdict == verify::Verdict::Divergent);
});
