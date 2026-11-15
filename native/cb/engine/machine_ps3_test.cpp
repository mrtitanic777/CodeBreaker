// cb-unity: alone
// The PlayStation 3 lane: registered, a small PowerPC64 ELF (built word by word here: no PS3 homebrew with plain segments was found, so the lane is proved on such
// files) comes back identical, a function descriptor in the data finds its function, and an edited program's functions are run beside the original's in the
// PowerPC interpreter.
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_ps3.hpp"
#include "cb/formats/ps3.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

void put32(Bytes& b, std::size_t at, std::uint64_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void put64(Bytes& b, std::size_t at, std::uint64_t v) {
    put32(b, at, v >> 32);
    put32(b, at + 4, v);
}
void put16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 8);
    b[at + 1] = static_cast<std::uint8_t>(v);
}

// text at $10000: entry: mflr r0; std r0,16(r1); stdu r1,-112(r1); bl sub; addi r1,r1,112; ld r0,16(r1); mtlr r0; blr;
// sub $10020: ld r3,0(r4); add r3,r3,r5; std r3,8(r4); blr.   data at $10000000: the descriptors {code, toc, 0} of the entry and of sub.
Bytes elf(std::uint32_t store = 0xF8640008) {
    Bytes f(0x230, 0);
    f[0] = 0x7F;
    f[1] = 'E';
    f[2] = 'L';
    f[3] = 'F';
    f[4] = 2;
    f[5] = 2;
    f[6] = 1;
    f[7] = 0x66;
    put16(f, 16, 2);
    put16(f, 18, 21);
    put32(f, 20, 1);
    put64(f, 24, 0x10000000);   // the entry's descriptor
    put64(f, 32, 64);
    put16(f, 52, 64);
    put16(f, 54, 56);
    put16(f, 56, 2);
    const auto segment = [&](std::size_t at, std::uint32_t flags, std::uint64_t offset, std::uint64_t vaddr, std::uint64_t size) {
        put32(f, at, 1);
        put32(f, at + 4, flags);
        put64(f, at + 8, offset);
        put64(f, at + 16, vaddr);
        put64(f, at + 24, vaddr);
        put64(f, at + 32, size);
        put64(f, at + 40, size);
        put64(f, at + 48, 0x10000);
    };
    segment(64, 5, 0x100, 0x10000, 0x30);
    segment(120, 6, 0x200, 0x10000000, 0x30);
    const std::uint32_t code[] = {0x7C0802A6, 0xF8010010, 0xF821FF91, 0x48000015, 0x38210070, 0xE8010010, 0x7C0803A6, 0x4E800020,
                                  0xE8640000, 0x7C632A14, store,      0x4E800020};
    for (std::size_t i = 0; i < std::size(code); ++i) put32(f, 0x100 + 4 * i, code[i]);
    put64(f, 0x200, 0x10000);
    put64(f, 0x208, 0x10008000);
    put64(f, 0x218, 0x10020);
    put64(f, 0x220, 0x10008000);
    return f;
}

}  // namespace

CB_TEST("engine.machine_ps3.is_registered_and_takes_its_files", [] {
    register_ps3_lane();
    register_ps3_lane();
    CB_CHECK(machine_lanes().find("ps3") != nullptr);
    fs::ScopedTempDir dir("cb_ps3_reg");
    fs::write_file(dir.path() / "app.elf", elf());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "app.elf"), "ps3");
    CB_CHECK_EQ(ps3_lane_spec().kind, "ps3");
    const formats::ps3::Program p = formats::ps3::read(dir.path() / "app.elf");
    CB_CHECK_EQ(p.segments.size(), 2u);
    CB_CHECK_EQ(p.entry_code, 0x10000u);
    CB_CHECK_EQ(p.toc, 0x10008000u);
    CB_CHECK_EQ(formats::ps3::syscall_name(1), "process_getpid");
    CB_CHECK(!formats::ps3::function_name(0x1cf98800).empty());   // ioPadInit
});

CB_TEST("engine.machine_ps3.an_elf_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_ps3_lane");
    const verify::lanes::ImageLane lane(ps3_lane_spec());
    const stdfs::path original = dir.path() / "app.elf";
    fs::write_file(original, elf());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    fs::write_file(dir.path() / "edited.elf", elf(0xF8640010));   // std r3,16(r4)
    CB_CHECK(lane.judge_function(false, original, original, 0x20).verdict == verify::Verdict::Equivalent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.elf", 0x20).verdict == verify::Verdict::Divergent);
});
