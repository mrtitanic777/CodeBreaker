// Tests of the Atari 2600 writer over small programs of its own, written out byte by byte: the source it writes, built again by CodeBreaker's
// dasm-syntax assembler, is the cartridge it came from.

#include "source/atari2600_writer.hpp"

#include "source/dialect.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari2600_writer {
namespace {

// reset at $F000: SEI CLD LDX #$FF TXS LDA #0; clear: STA $00,X DEX BNE clear;
// main at $F00C: LDA #$0E, STA COLUBK, STA.w WSYNC, JSR sub, JMP main; sub at $F01C: LDA INTIM, RTS; vectors at $FFC
Bytes program() {
    Bytes rom(0x1000, 0xFF);
    const Bytes code = {0x78, 0xD8, 0xA2, 0xFF, 0x9A, 0xA9, 0x00, 0x95, 0x00, 0xCA, 0xD0, 0xFB, 0xA9, 0x0E, 0x85, 0x09,
                        0x8D, 0x02, 0x00, 0x20, 0x1C, 0xF0, 0x4C, 0x0C, 0xF0, 0xEA, 0x00, 0x00, 0xAD, 0x84, 0x02, 0x60};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes vectors = {0x00, 0xF0, 0x00, 0xF0};
    std::copy(vectors.begin(), vectors.end(), rom.begin() + 0xFFC);
    return rom;
}

std::string source_of(const image_writer::Written& r) { return fs::read_text(r.source); }

CB_TEST("source.atari2600_writer.the_program_comes_back_as_source_that_builds_the_same_rom", [] {
    fs::ScopedTempDir dir("cb_a2600");
    const image_writer::Written r = write(formats::atari2600::parse(program()), "demo.a26", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0xF000, "reset"}, {0xF01C, "sub_F01C"}}));
    CB_CHECK_EQ(r.image_bytes, 0x1000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    const std::string s = source_of(r);
    for (const char* want : {"        processor 6502", "WSYNC      = $02", "COLUBK     = $09", "INTIM      = $0284", "        ORG $0000", "        RORG $F000", "reset\n",
                             "        SEI", "        STA COLUBK", "        STA.w WSYNC", "        BNE L_F007", "L_F007\n", "        JSR sub_F01C", "        JMP L_F00C",
                             "        LDA INTIM", "        .word reset, reset"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.atari2600_writer.an_edited_source_builds_a_different_rom", [] {
    fs::ScopedTempDir dir("cb_a2600_edit");
    const image_writer::Written r = write(formats::atari2600::parse(program()), "demo.a26", dir.path());
    std::string s = source_of(r);
    const std::size_t at = s.find("LDA #$0E");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$1E");

    const Bytes edited = m6502_source::assemble(m6502_source::Cpu::M6502, dialect::find("dasm").parse(s), dir.path());
    Bytes expected = program();
    expected[13] = 0x1E;
    CB_CHECK(edited == expected);
});

CB_TEST("source.atari2600_writer.banked_cartridges_write_each_bank_and_name_by_bank", [] {
    // two 4 KB banks (F8): each starts SEI; JMP $F00B, with a RTS at $F00B; bank 1 has a distinct byte in its data
    Bytes rom;
    for (std::size_t b = 0; b < 2; ++b) {
        Bytes bank(0x1000, 0xFF);
        const Bytes code = {0x78, 0x4C, 0x0B, 0xF0, 0xAD, 0xF8, 0x1F, 0xAD, 0xF9, 0x1F, 0xEA, 0x60};   // bank switch touches then an RTS
        std::copy(code.begin(), code.end(), bank.begin());
        bank[0x80] = static_cast<std::uint8_t>(0xA0 + b);
        bank[0xFFC] = 0x00;
        bank[0xFFD] = 0xF0;
        bank[0xFFE] = 0x00;
        bank[0xFFF] = 0xF0;
        rom.insert(rom.end(), bank.begin(), bank.end());
    }
    fs::ScopedTempDir dir("cb_a2600_f8");
    const image_writer::Written r = write(formats::atari2600::parse(rom), "f8.a26", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, 0x2000u);
    CB_CHECK_EQ(r.functions.at(0x0000F000), "bank0_reset");
    CB_CHECK_EQ(r.functions.at(0x0001F000), "reset");   // the last bank is where the program starts
    const std::string s = source_of(r);
    CB_CHECK(s.find("        ORG $1000") != std::string::npos);
    CB_CHECK(s.find("        RORG $F000") != std::string::npos);
});

CB_TEST("source.atari2600_writer.a_two_kilobyte_cartridge_and_a_mirrored_operand", [] {
    Bytes rom(0x800, 0xFF);
    // reset $F800: JMP $1804 (a mirror of $F804: left as written, still followed); $F804: RTS
    const Bytes code = {0x4C, 0x04, 0x18, 0xEA, 0x60};
    std::copy(code.begin(), code.end(), rom.begin());
    rom[0x7FC] = 0x00;
    rom[0x7FD] = 0xF8;
    rom[0x7FE] = 0x00;
    rom[0x7FF] = 0xF8;
    fs::ScopedTempDir dir("cb_a2600_2k");
    const image_writer::Written r = write(formats::atari2600::parse(rom), "two.a26", dir.path());
    CB_CHECK(r.linked && r.same);
    const std::string s = source_of(r);
    CB_CHECK(s.find("        RORG $F800") != std::string::npos);
    CB_CHECK(s.find("JMP $1804") != std::string::npos);
    CB_CHECK_EQ(r.code_bytes, 4u);
});

}  // namespace
}  // namespace cb::source::atari2600_writer
