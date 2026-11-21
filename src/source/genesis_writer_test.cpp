// Tests of the Genesis writer over a small cartridge of its own: the vector table and the header, a reset handler that writes the VDP's
// control port and calls a subroutine and then loops, the subroutine, a vertical-blank handler and a table of text. The program is machine
// code written out byte by byte; the writer is held to what it is for -- the source it writes, built again, is the cartridge it came
// from -- and to the text it writes, a golden file that was read through for correctness (testdata/writers/genesis.golden.asm).

#include "source/genesis_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::genesis_writer {
namespace {

void put32(Bytes& rom, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) rom[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

void put_text(Bytes& rom, std::size_t at, std::size_t width, std::string_view text) {
    for (std::size_t i = 0; i < width; ++i) rom[at + i] = i < text.size() ? static_cast<std::uint8_t>(text[i]) : ' ';
}

// $200 reset: MOVE.W #$0080,($C00004).L ; JSR ($220).L ; BRA.S *
// $220 sub:   MOVEA.L #$400,A0 ; RTS
// $300 vblank handler: RTE
// $400: a table of text
Bytes cartridge(std::size_t size = 0x1000) {
    Bytes rom(size, 0xFF);
    std::fill(rom.begin(), rom.begin() + 0x200, std::uint8_t{0});
    put32(rom, 0, 0x00FF0000);   // the stack
    put32(rom, 4, 0x200);        // reset
    put32(rom, 4 * 30, 0x300);   // vertical blank
    put_text(rom, 0x100, 16, "SEGA MEGA DRIVE");
    put_text(rom, 0x110, 16, "(C)TEST 2026.NOV");
    put_text(rom, 0x120, 48, "TEST CARTRIDGE");
    put_text(rom, 0x150, 48, "TEST CARTRIDGE");
    put_text(rom, 0x180, 14, "GM 00000000-00");
    rom[0x18E] = 0x12;
    rom[0x18F] = 0x34;
    put_text(rom, 0x190, 16, "J");
    put32(rom, 0x1A0, 0);
    put32(rom, 0x1A4, static_cast<std::uint32_t>(size - 1));
    put32(rom, 0x1A8, 0xFF0000);
    put32(rom, 0x1AC, 0xFFFFFF);
    put_text(rom, 0x1B0, 12, "");
    put_text(rom, 0x1BC, 12, "");
    put_text(rom, 0x1C8, 40, "");
    put_text(rom, 0x1F0, 16, "JUE");
    const Bytes reset = {0x33, 0xFC, 0x00, 0x80, 0x00, 0xC0, 0x00, 0x04, 0x4E, 0xB9, 0x00, 0x00, 0x02, 0x20, 0x60, 0xFE};
    std::copy(reset.begin(), reset.end(), rom.begin() + 0x200);
    const Bytes sub = {0x20, 0x7C, 0x00, 0x00, 0x04, 0x00, 0x4E, 0x75};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x220);
    rom[0x300] = 0x4E;
    rom[0x301] = 0x73;
    const std::string_view table = "SCORE LEVEL LIVES";
    std::copy(table.begin(), table.end(), rom.begin() + 0x400);
    return rom;
}

// A copier's dump of `rom`: its 512-byte header, then each 16 KB block with the odd bytes first.
Bytes smd_dump(const Bytes& rom) {
    Bytes out(512, 0);
    out[8] = 0xAA;
    out[9] = 0xBB;
    for (std::size_t block = 0; block < rom.size(); block += 0x4000) {
        Bytes odd(0x2000), even(0x2000);
        for (std::size_t i = 0; i < 0x2000; ++i) {
            odd[i] = rom[block + 2 * i + 1];
            even[i] = rom[block + 2 * i];
        }
        out.insert(out.end(), odd.begin(), odd.end());
        out.insert(out.end(), even.begin(), even.end());
    }
    return out;
}

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.genesis_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom", [] {
    fs::ScopedTempDir dir("cb_md");
    const Bytes image = cartridge();
    const image_writer::Written r = write(formats::genesis::parse(image), "demo.md", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x200, "reset"}, {0x220, "sub_000220"}, {0x300, "vblank"}}));
    CB_CHECK_EQ(r.code_bytes, 26u);   // MOVE.W (8), JSR (6), BRA.S (2), MOVEA.L (6), RTS (2), RTE (2)
    CB_CHECK_EQ(r.image_bytes, 0x1000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built && *r.built == dir.path() / "demo.built.md" && fs::read_file(*r.built) == image);
    check_golden("genesis.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.genesis_writer.the_golden_names_what_the_code_touches", [] {
    // Read for what it says: the VDP's port by name, the vector table naming its handlers, the header's fields as data with their names,
    // a long immediate that is the start of a line is a data label, a loop to itself a local label.
    const std::string text = golden("genesis.golden.asm");
    for (const char* line :
         {"VDP_CTRL         equ $C00004", "        org $0", "        dc.l $00FF0000  ; initial stack pointer", "        dc.l reset  ; reset", "        dc.l vblank  ; vblank",
          "        dc.l $00000000  ; bus_error", "        dc.b \"SEGA MEGA DRIVE \"  ; system", "        dc.b $12,$34  ; checksum", "reset:",
          "        MOVE.W #$80,(VDP_CTRL).L", "        JSR (sub_000220).L", "L_00020E:", "        BRA.S L_00020E", "sub_000220:",
          "        MOVEA.L #D_000400,A0", "vblank:", "        RTE", "D_000400:"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    // sixty-four longs between `org $0` and the header, whatever they hold
    std::size_t longs = 0;
    for (const std::string_view line : text::lines(text)) longs += line.starts_with("        dc.l ") ? 1 : 0;
    CB_CHECK_EQ(longs, 64u);
});

CB_TEST("source.genesis_writer.a_handler_many_vectors_share_is_the_catch_all", [] {
    Bytes rom = cartridge();
    put32(rom, 4 * 2, 0x300);   // bus error, address error and illegal instruction all go where vblank goes
    put32(rom, 4 * 3, 0x300);
    put32(rom, 4 * 4, 0x300);
    fs::ScopedTempDir dir("cb_md");
    const image_writer::Written r = write(formats::genesis::parse(rom), "shared.md", dir.path());
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.at(0x300), "exception");
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("        dc.l exception  ; bus_error") != std::string::npos);
    CB_CHECK(source.find("        dc.l exception  ; vblank") != std::string::npos);
});

CB_TEST("source.genesis_writer.a_handler_one_vector_names_takes_its_name_and_the_reset_wins_a_shared_one", [] {
    Bytes rom = cartridge();
    put32(rom, 4 * 30, 0);        // no vertical blank
    put32(rom, 4 * 6, 0x300);     // the CHK handler is what is at $300
    put32(rom, 4 * 28, 0x200);    // horizontal blank at the reset's place: two vectors, the reset is the name
    fs::ScopedTempDir dir("cb_md");
    const image_writer::Written r = write(formats::genesis::parse(rom), "named.md", dir.path());
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.at(0x300), "chk");
    CB_CHECK_EQ(r.functions.at(0x200), "reset");
});

CB_TEST("source.genesis_writer.a_copiers_dump_is_written_from_the_plain_image", [] {
    const Bytes plain = cartridge(0x4000);
    fs::ScopedTempDir dir("cb_md");
    const image_writer::Written r = write(formats::genesis::parse(smd_dump(plain), ".smd"), "dump.smd", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(r.built && fs::read_file(*r.built) == plain);   // what it builds back is the plain ROM
    CB_CHECK(fs::read_text(r.source).find("; \"TEST CARTRIDGE\" / \"TEST CARTRIDGE\", GM 00000000-00, 16 KB ROM, from an .smd dump") != std::string::npos);
    CB_CHECK_EQ(r.functions.size(), 3u);
});

CB_TEST("source.genesis_writer.a_file_too_short_is_refused", [] {
    fs::ScopedTempDir dir("cb_md");
    fs::write_file(dir.path() / "short.md", Bytes(0x100, 0));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "short.md", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "none.md", dir.path() / "out"));
});

CB_TEST("source.genesis_writer.reconstruct_reads_the_file", [] {
    fs::ScopedTempDir dir("cb_md");
    fs::write_file(dir.path() / "game.md", cartridge());
    const image_writer::Written r = reconstruct(dir.path() / "game.md", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game.asm");
});

CB_TEST("source.genesis_writer.the_ports_are_the_documented_ones", [] {
    CB_CHECK_EQ(hardware().size(), 17u);
    CB_CHECK_EQ(hardware().at(0xC00000), "VDP_DATA");
    CB_CHECK_EQ(hardware().at(0xA11100), "Z80_BUSREQ");
});

}  // namespace
}  // namespace cb::source::genesis_writer
