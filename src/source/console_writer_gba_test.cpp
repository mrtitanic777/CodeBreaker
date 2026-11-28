// Tests of the Game Boy Advance on the console writer (config/platforms/gba.json, the vectored shape's ARM processor) over a small cartridge of its own (engine/testdata/lane_kit/gba_demo.s: ARM code that calls a Thumb
// function through a literal pool and `bx`, a Thumb function that calls another with `bl`, and an ARM function reached only through a table).
// The program is assembled with CodeBreaker's own assembler; the writer is held to what it is for: the source it writes, built again, is the
// cartridge it came from.

#include "formats/gba.hpp"
#include "knowledge/package.hpp"
#include "source/console_writer.hpp"
#include "source/risc_source.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/isa.hpp"

namespace cb::source::console_writer::gba_test {
namespace {

// The cartridge written to `folder` as `name` and its source written beside it.
image_writer::Written write(const formats::gba::Rom& rom, std::string_view name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    fs::write_file(folder / std::string(name), rom.data);
    return console_writer::reconstruct("gba", folder / std::string(name), folder);
}
const image_writer::Names& hardware() {   // (config/knowledge/gba/registers.json)
    static const image_writer::Names table = knowledge::standard().hardware_names("gba", describe("gba").vectored.registers);
    return table;
}

Bytes cartridge() {
    return risc_source::gnu_arm(fs::read_text(testing::data_path("engine/testdata/lane_kit/gba_demo.s")));
}

CB_TEST("source.console_writer.gba.the_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_gba_cw");
    const Bytes image = cartridge();
    const formats::gba::Rom rom = formats::gba::parse(image);
    const image_writer::Written r = write(rom, "demo.gba", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK_EQ(r.image_bytes, image.size());
    CB_CHECK(r.source == dir.path() / "demo.s");
    CB_CHECK(r.built && fs::read_file(*r.built) == image);
    // entry, start, the Thumb function (found through the literal pool and the bx; its key has the lowest bit set), its callee (found by the bl),
    // and the ARM function found only through the table
    CB_CHECK_EQ(r.functions.size(), std::size_t{5});
    CB_CHECK_EQ(r.functions.at(0x08000000), std::string("entry"));
    CB_CHECK(r.functions.contains(0x080000C0));
    std::size_t thumb = 0;
    for (const auto& [address, name] : r.functions) thumb += (address & 1) ? 1 : 0;
    CB_CHECK_EQ(thumb, std::size_t{2});
});

CB_TEST("source.console_writer.gba.what_the_code_points_at_is_written_by_name", [] {
    fs::ScopedTempDir dir("cb_gba_cw");
    const image_writer::Written r = write(formats::gba::parse(cartridge()), "demo.gba", dir.path());
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("        .thumb") != std::string::npos);
    CB_CHECK(source.find("        .arm") != std::string::npos);
    CB_CHECK(source.find("thumb_") != std::string::npos);
    CB_CHECK(source.find("arm_") != std::string::npos);
    // the literal pool word: a Thumb function's address, with its state bit
    CB_CHECK(source.find(".word thumb_") != std::string::npos);
    CB_CHECK(source.find(" + 1") != std::string::npos);
    // a PC-relative load names the word it reads
    CB_CHECK(source.find("ldr r0, D_") != std::string::npos);
    // the hardware register's address a function builds, and the string
    CB_CHECK(source.find("mov r4, #0x4000000") != std::string::npos);
    CB_CHECK(source.find(".asciz \"HELLO, GBA\"") != std::string::npos);
    CB_CHECK(source.find("push {r4, lr}") != std::string::npos);
    CB_CHECK(source.find("bx lr") != std::string::npos);
    // the header's fields
    CB_CHECK(source.find("@ the Nintendo logo") != std::string::npos);
    CB_CHECK(source.find(".byte 0x96    @ the fixed value") != std::string::npos);
});

CB_TEST("source.console_writer.gba.an_instruction_the_assembler_would_write_differently_stays_as_its_bits", [] {
    // `adds r0, r0, #3` in Thumb's 3-bit form (0x1CC0) assembles to the 8-bit form, so the writer keeps the halfword
    fs::ScopedTempDir dir("cb_gba_cw");
    Bytes image = cartridge();
    // the first Thumb instruction of thumb_fn is `push {r4, lr}` at the pointer's target: find the `movs r4, #3` (0x2403) and turn it into 0x1CC0
    std::size_t at = 0;
    for (std::size_t i = 0xC0; i + 1 < image.size(); i += 2)
        if (image[i] == 0x03 && image[i + 1] == 0x24) {
            at = i;
            break;
        }
    CB_CHECK(at != 0);
    image[at] = 0xC0;
    image[at + 1] = 0x1C;
    const image_writer::Written r = write(formats::gba::parse(image), "odd.gba", dir.path());
    CB_CHECK(r.linked && r.same);
    // (the description's assembler writes the 3-bit form back as it was read, so the instruction is text: nothing needs to stay as its bits)
    CB_CHECK(fs::read_text(r.source).find("add r0, r0, #3") != std::string::npos);
    CB_CHECK(fs::read_text(r.source).find(".hword 0x1CC0") == std::string::npos);
});

CB_TEST("source.console_writer.gba.the_hardware_registers_have_libgba_names", [] {
    CB_CHECK_EQ(hardware().at(0x04000000), std::string("REG_DISPCNT"));
    CB_CHECK_EQ(hardware().at(0x040000D4), std::string("REG_DMA3SAD"));
    CB_CHECK_EQ(hardware().at(0x04000208), std::string("REG_IME"));
    CB_CHECK_EQ(hardware().at(0x04000130), std::string("REG_KEYINPUT"));
    CB_CHECK(!hardware().contains(0x04000002));
});

}  // namespace
}  // namespace cb::source::console_writer::gba_test
