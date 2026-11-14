// Tests of the Game Boy writer over a small cartridge of its own. The program is machine code written out byte by byte; the writer is
// held to what it is for -- the source it writes, built again, is the cartridge it came from -- and to the text it writes, a golden
// file that was read through for correctness (testdata/writers/gb.golden.asm).

#include "cb/source/gb_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::gb_writer {
namespace {

// $0100: NOP; JP $0150      $0040 (vblank): RETI
// $0150: CALL $0170; wait: LDH A,($FF44); CP $90; JR NZ,wait; LD HL,$0180; LD ($2000),A; JP *
// $0170: LD A,$91; LDH ($FF40),A; RET             $0180: a table of text
Bytes cartridge(std::size_t size = 0x8000, std::uint8_t color = 0, std::uint8_t type = 0) {
    Bytes rom(size, 0xFF);
    rom[0x40] = 0xD9;
    const Bytes entry = {0x00, 0xC3, 0x50, 0x01};
    std::copy(entry.begin(), entry.end(), rom.begin() + 0x100);
    std::copy(formats::gb::kLogo.begin(), formats::gb::kLogo.end(), rom.begin() + 0x104);
    std::fill(rom.begin() + 0x134, rom.begin() + 0x150, std::uint8_t{0});
    const std::string_view title = "DEMO";
    std::copy(title.begin(), title.end(), rom.begin() + 0x134);
    rom[0x143] = color;
    rom[0x147] = type;
    const Bytes main_code = {0xCD, 0x70, 0x01, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x21, 0x80, 0x01, 0xEA, 0x00, 0x20, 0xC3, 0x5F, 0x01};
    std::copy(main_code.begin(), main_code.end(), rom.begin() + 0x150);
    const Bytes sub = {0x3E, 0x91, 0xE0, 0x40, 0xC9};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x170);
    const std::string_view table = "TILES AND MAPS";
    std::copy(table.begin(), table.end(), rom.begin() + 0x180);
    return rom;
}

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.gb_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom", [] {
    fs::ScopedTempDir dir("cb_gb");
    const Bytes image = cartridge();
    const image_writer::Written r = write(formats::gb::parse(image), "demo.gb", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x40, "vblank"}, {0x100, "entry"}, {0x170, "sub_0170"}}));
    CB_CHECK_EQ(r.code_bytes, 28u);   // RETI, the entry, the main loop (CALL, LDH, CP, JR, LD HL, LD (nn), JP) and the subroutine
    CB_CHECK_EQ(r.image_bytes, 0x8000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built && *r.built == dir.path() / "demo.built.gb" && fs::read_file(*r.built) == image);
    check_golden("gb.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.gb_writer.the_golden_names_what_the_code_touches", [] {
    // The golden file, read for what it says: the registers the code touches are defined and named in its operands, the header is data with
    // its fields named, a pointer into the program gets a data label, and a write into ROM space is a bank controller register, not a pointer.
    const std::string text = golden("gb.golden.asm");
    for (const char* line : {"DEF rLY      EQU $FF44", "DEF rLCDC    EQU $FF40", "DEF rROMB0   EQU $2000", "        LDH A,(rLY)", "        LD (rROMB0),A",
                             "        LD HL,D_0180", "D_0180:", "SECTION \"bank0\", ROM0[$0000]", "SECTION \"bank1\", ROMX[$4000], BANK[1]",
                             "  ; Nintendo's logo (the boot ROM checks it)", "  ; header checksum", "vblank:", "entry:", "sub_0170:", "        CALL sub_0170"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    CB_CHECK(text.find("D_2000") == std::string::npos);
});

CB_TEST("source.gb_writer.banks_past_the_second_are_data_and_the_head_says_what_the_cartridge_is", [] {
    fs::ScopedTempDir dir("cb_gb");
    const Bytes image = cartridge(0x10000, 0x80, 0x01);
    const image_writer::Written r = write(formats::gb::parse(image), "big.gbc", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(r.built && *r.built == dir.path() / "big.built.gbc");   // the extension is the original's
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("; \"DEMO\", MBC1, 64 KB ROM, Game Boy Color (also)") != std::string::npos);
    CB_CHECK(source.find("SECTION \"bank3\", ROMX[$4000], BANK[3]") != std::string::npos);
    CB_CHECK_EQ(r.functions.size(), 3u);
    CB_CHECK_EQ(r.image_bytes, 0x10000u);
});

CB_TEST("source.gb_writer.registers_are_named_only_when_the_code_uses_them", [] {
    CB_CHECK_EQ(registers().at(0xFF40), "rLCDC");
    CB_CHECK_EQ(registers().at(0xFFFF), "rIE");
    CB_CHECK_EQ(registers().size(), 55u);
    CB_CHECK_EQ(mbc_registers().at(0x2000), "rROMB0");
    fs::ScopedTempDir dir("cb_gb");
    const image_writer::Written r = write(formats::gb::parse(cartridge()), "demo.gb", dir.path());
    CB_CHECK(fs::read_text(r.source).find("rNR10") == std::string::npos);   // a register no instruction touches is not defined
});

CB_TEST("source.gb_writer.a_file_too_short_for_a_header_is_refused", [] {
    fs::ScopedTempDir dir("cb_gb");
    fs::write_file(dir.path() / "short.gb", Bytes(0x100, 0));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "short.gb", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "none.gb", dir.path() / "out"));
});

CB_TEST("source.gb_writer.reconstruct_reads_the_file", [] {
    fs::ScopedTempDir dir("cb_gb");
    fs::write_file(dir.path() / "game.gb", cartridge());
    const image_writer::Written r = reconstruct(dir.path() / "game.gb", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game.asm");
});

}  // namespace
}  // namespace cb::source::gb_writer
