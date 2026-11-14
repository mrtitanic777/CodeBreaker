// Tests of the Master System writer over a small cartridge of its own. The program is machine code written out byte by byte; the writer is
// held to what it is for -- the source it writes, built again, is the cartridge it came from -- and to the text it writes, a golden
// file that was read through for correctness (testdata/writers/sms.golden.asm).

#include "cb/source/sms_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::sms_writer {
namespace {

// $0000: DI; IM 1; JP $0080      $0038: EI; RETI      $0066: RETN
// $0080: LD HL,$C000; LD A,1; OUT ($BF),A; CALL $0100; LD ($FFFD),A; LD HL,$0120; JP *
// $0100: LD A,0; OUT ($BE),A; RET      $0120: a table of text
Bytes cartridge(std::size_t size = 0x8000) {
    Bytes rom(size, 0xFF);
    const auto put = [&](std::size_t at, const Bytes& b) { std::copy(b.begin(), b.end(), rom.begin() + static_cast<std::ptrdiff_t>(at)); };
    put(0x00, {0xF3, 0xED, 0x56, 0xC3, 0x80, 0x00});
    put(0x38, {0xFB, 0xED, 0x4D});
    put(0x66, {0xED, 0x45});
    put(0x80, {0x21, 0x00, 0xC0, 0x3E, 0x01, 0xD3, 0xBF, 0xCD, 0x00, 0x01, 0x32, 0xFD, 0xFF, 0x21, 0x20, 0x01, 0xC3, 0x90, 0x00});
    put(0x100, {0x3E, 0x00, 0xD3, 0xBE, 0xC9});
    const std::string_view table = "HELLO WORLD";
    std::copy(table.begin(), table.end(), rom.begin() + 0x120);
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

CB_TEST("source.sms_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom", [] {
    fs::ScopedTempDir dir("cb_sms");
    const Bytes image = cartridge();
    const image_writer::Written r = write(formats::sms::parse(image, ".sms"), "demo.sms", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x00, "reset"}, {0x38, "irq"}, {0x66, "nmi"}, {0x100, "sub_0100"}}));
    CB_CHECK_EQ(r.code_bytes, 35u);   // 6 + 3 + 2 + the 19 bytes of the main loop + 5
    CB_CHECK_EQ(r.image_bytes, 0x8000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built && *r.built == dir.path() / "demo.built.sms" && fs::read_file(*r.built) == image);
    check_golden("sms.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.sms_writer.the_golden_names_what_the_code_touches", [] {
    // Read for what it says: the ports and the mapper register the code uses are defined and written by name, a word loaded into HL that
    // is the start of a line has a data label, and a jump target a local label.
    const std::string text = golden("sms.golden.asm");
    for (const char* line : {".DEFINE VDP_DATA         $BE", ".DEFINE VDP_CONTROL      $BF", ".DEFINE MAPPER_SLOT0     $FFFD", "        OUT (VDP_CONTROL),A",
                             "        OUT (VDP_DATA),A", "        LD (MAPPER_SLOT0),A", "        LD HL,D_0120", "D_0120:", "        CALL sub_0100", "sub_0100:",
                             "L_0090:", "        JP L_0090", "reset:", "irq:", "nmi:", ".BANK 0 SLOT 0", ".BANK 1 SLOT 1", "BANKSTOTAL 2"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    CB_CHECK(text.find("        LD HL,$C000") != std::string::npos);   // a RAM address is a number: not a line of the program
});

CB_TEST("source.sms_writer.pages_after_the_second_are_shown_in_the_third_slot_as_data", [] {
    CB_CHECK_EQ(slot_of(0), 0u);
    CB_CHECK_EQ(slot_of(1), 1u);
    CB_CHECK_EQ(slot_of(2), 2u);
    CB_CHECK_EQ(slot_of(7), 2u);
    fs::ScopedTempDir dir("cb_sms");
    const Bytes image = cartridge(0x10000);
    const image_writer::Written r = write(formats::sms::parse(image, ".sms"), "big.sms", dir.path());
    CB_CHECK(r.same);
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("BANKSTOTAL 4") != std::string::npos);
    CB_CHECK(source.find(".BANK 3 SLOT 2") != std::string::npos);
    CB_CHECK_EQ(r.functions.size(), 4u);
});

CB_TEST("source.sms_writer.the_game_gears_own_port_is_named_for_a_game_gear_only", [] {
    Bytes rom = cartridge();
    rom[0x85] = 0xDB;   // IN A,($00): the Game Gear's start button
    rom[0x86] = 0x00;
    fs::ScopedTempDir dir("cb_sms");
    const image_writer::Written gg = write(formats::sms::parse(rom, ".gg"), "game.gg", dir.path() / "gg");
    CB_CHECK(gg.same);
    const std::string gg_source = fs::read_text(gg.source);
    CB_CHECK(gg_source.find("; Game Gear, 32 KB ROM") != std::string::npos);
    CB_CHECK(gg_source.find(".DEFINE GG_START") != std::string::npos);
    CB_CHECK(gg_source.find("        IN A,(GG_START)") != std::string::npos);
    const image_writer::Written sms = write(formats::sms::parse(rom, ".sms"), "game.sms", dir.path() / "sms");
    CB_CHECK(sms.same);
    CB_CHECK(fs::read_text(sms.source).find("GG_START") == std::string::npos);   // the same bytes on a Master System are just port 0
    CB_CHECK(fs::read_text(sms.source).find("        IN A,($00)") != std::string::npos);
});

CB_TEST("source.sms_writer.a_cartridge_header_is_said_in_the_head", [] {
    Bytes rom = cartridge();
    const Bytes header = {'T', 'M', 'R', ' ', 'S', 'E', 'G', 'A', 0, 0, 0x78, 0x56, 0x34, 0x12, 0x02, 0x4C};
    std::copy(header.begin(), header.end(), rom.begin() + 0x7FF0);
    const formats::sms::Rom parsed = formats::sms::parse(rom, ".sms");
    CB_CHECK(parsed.header.has_value());
    fs::ScopedTempDir dir("cb_sms");
    const image_writer::Written r = write(parsed, "demo.sms", dir.path());
    CB_CHECK(r.same);
    const std::string expected = std::format("; Master System, 32 KB ROM, header product {} version {} ({})", parsed.header->product, parsed.header->version, parsed.header->region);
    CB_CHECK(fs::read_text(r.source).find(expected) != std::string::npos);
    CB_CHECK(expected.find("version 2") != std::string::npos);
});

CB_TEST("source.sms_writer.the_tables_are_the_documented_ones", [] {
    CB_CHECK_EQ(ports().at(0xBE), "VDP_DATA");
    CB_CHECK_EQ(ports().at(0xDC), "JOYPAD_1");
    CB_CHECK_EQ(ports().size(), 8u);
    CB_CHECK_EQ(game_gear_ports().at(0x06), "GG_STEREO");
    CB_CHECK_EQ(mapper_registers().at(0xFFFC), "MAPPER_CONTROL");
});

CB_TEST("source.sms_writer.reconstruct_reads_the_file_and_refuses_what_is_not_one", [] {
    fs::ScopedTempDir dir("cb_sms");
    fs::write_file(dir.path() / "game.sms", cartridge());
    const image_writer::Written r = reconstruct(dir.path() / "game.sms", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game.asm");
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "none.sms", dir.path() / "out"));
});

}  // namespace
}  // namespace cb::source::sms_writer
