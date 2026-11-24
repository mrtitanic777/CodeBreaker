// Tests of the Sega CD writer over a small disc of its own (formats/segacd_test_image.hpp): the boot header, the security code and the IP of the main
// CPU, the SP of the sub CPU with its jump table. The writer is held to what it is for -- the source it writes, built again, is the disc's system area --
// and to the text it writes, a golden file that was read through for correctness (testdata/writers/segacd.golden.asm).

#include "source/segacd_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/segacd_test_image.hpp"

namespace cb::source::segacd_writer {
namespace {

namespace img = formats::segacd_test_image;

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.segacd_writer.the_system_area_comes_back_as_source_that_builds_the_same_area", [] {
    fs::ScopedTempDir dir("cb_scd");
    const Bytes boot = img::system_area();
    const image_writer::Written r = write(formats::segacd::parse(boot), "demo.iso", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x6028, "sp_init"}, {0x6032, "sp_main"}, {0x6034, "sp_int2"}, {0xFF0000, "security"}, {0xFF0600, "ip_start"}, {0xFF0610, "sub_FF0610"}}));
    // BRA.W (4); MOVE.B (8) BSR.S (2) JMP (6) TST.B (6) BNE.S (2) RTS (2); MOVEQ (2) JSR (6) RTS (2) BRA.S (2) RTE (2)
    CB_CHECK_EQ(r.code_bytes, 4u + 8 + 2 + 6 + 6 + 2 + 2 + 2 + 6 + 2 + 2 + 2);
    CB_CHECK_EQ(r.image_bytes, 0x8000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built && *r.built == dir.path() / "demo.built.iso" && fs::read_file(*r.built) == boot);
    check_golden("segacd.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.segacd_writer.the_golden_names_what_the_code_touches", [] {
    // Read for what it says: the gate array and the CD BIOS's call by name, each program under the address it runs at, the header's fields as data with
    // their names, the SP's entries as functions.
    const std::string text = golden("segacd.golden.asm");
    for (const char* line :
         {"GA_MEMMODE_LO      equ $A12003", "GA_COMFLAG_SUB     equ $A1200F", "CDBIOS             equ $005F22", "        org $0   ; the boot header",
          "        dc.b \"SEGADISCSYSTEM  \"  ; disc ID", "        dc.l $00000800  ; IP: start on the disc", "        dc.l $00001000  ; SP: start on the disc",
          "        dc.b \"SEGA MEGA DRIVE \"  ; system type", "security:", "        BRA.W ip_start", "ip_start:", "        MOVE.B #$1,(GA_MEMMODE_LO).L",
          "        BSR.S sub_FF0610", "        JMP ($00200000).L", "sub_FF0610:", "        TST.B (GA_COMFLAG_SUB).L", "sp_init:", "        MOVEQ #1,D0",
          "        JSR (CDBIOS).L", "sp_main:", "sp_int2:", "        RTE"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    CB_CHECK(text.find("org $FF0000") != std::string::npos);
    CB_CHECK(text.find("org $6000") != std::string::npos);
});

CB_TEST("source.segacd_writer.a_raw_image_is_written_from_its_system_area", [] {
    fs::ScopedTempDir dir("cb_scd");
    const Bytes boot = img::system_area();
    const image_writer::Written r = write(formats::segacd::parse(img::raw(boot)), "raw.bin", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(r.built);   // (a failed rebuild fails here, not in the read below)
    CB_CHECK(fs::read_file(*r.built) == boot);   // what it builds back is the cooked system area
    CB_CHECK(fs::read_text(r.source).find("of a 2352-byte-sector image") != std::string::npos);
});

CB_TEST("source.segacd_writer.a_disc_without_a_sub_program_writes_the_main_one_alone", [] {
    Bytes boot = img::system_area();
    img::put_long(boot, 0x44, 0);
    fs::ScopedTempDir dir("cb_scd");
    const image_writer::Written r = write(formats::segacd::parse(boot), "main.iso", dir.path());
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.size(), 3u);
    CB_CHECK(fs::read_text(r.source).find("org $6000") == std::string::npos);
});

CB_TEST("source.segacd_writer.reconstruct_reads_the_file_and_refuses_what_is_not_a_disc", [] {
    fs::ScopedTempDir dir("cb_scd");
    fs::write_file(dir.path() / "game.iso", img::system_area());
    fs::write_file(dir.path() / "not.iso", Bytes(0x8000, 0));
    const image_writer::Written r = reconstruct(dir.path() / "game.iso", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game.asm");
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "not.iso", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "missing.iso", dir.path() / "out"));
});

CB_TEST("source.segacd_writer.the_ports_are_the_genesis_ones_and_the_gate_array_and_the_bios", [] {
    CB_CHECK_EQ(hardware().at(0xC00004), "VDP_CTRL");
    CB_CHECK_EQ(hardware().at(0xA12000), "GA_RESET");
    CB_CHECK_EQ(hardware().at(0xFF800E), "SGA_COMFLAGS");
    CB_CHECK_EQ(hardware().at(0x5F22), "CDBIOS");
});

}  // namespace
}  // namespace cb::source::segacd_writer
