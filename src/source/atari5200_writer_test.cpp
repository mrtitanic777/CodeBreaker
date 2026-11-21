// cb-unity: alone
// The Atari 5200 writer over a small program written byte by byte: the source it writes, built again by CodeBreaker's dasm-syntax assembler, is the file.

#include "source/atari5200_writer.hpp"

#include "source/dialect.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari5200_writer {
namespace {

// start $8000: SEI CLD LDA #$0E STA COLBK JSR $8020 JMP $8002; sub $8020: LDA VCOUNT, STA WSYNC, STA $80, RTS.
Bytes program() {
    Bytes rom(0x4000, 0xFF);
    const Bytes code = {0x78, 0xD8, 0xA9, 0x0E, 0x8D, 0x1A, 0xC0, 0x20, 0x20, 0x80, 0x4C, 0x02, 0x80};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes sub = {0xAD, 0x0B, 0xD4, 0x8D, 0x0A, 0xD4, 0x85, 0x80, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x20);
    const Bytes tail = {0x00, 0xFF, 0x00, 0x80};
    std::copy(tail.begin(), tail.end(), rom.end() - 4);
    return rom;
}

}  // namespace

CB_TEST("source.atari5200_writer.the_program_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_a5200");
    const image_writer::Written r = write(formats::atari5200::parse(program()), "demo.a52", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x8000, "start"}, {0x8020, "sub_8020"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    CB_CHECK_EQ(r.code_bytes, 13u + 9u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"        processor 6502", "COLBK      = $C01A", "VCOUNT     = $D40B", "WSYNC      = $D40A", "        ORG $8000", "        STA COLBK",
                             "        JSR sub_8020", "        LDA VCOUNT", "        STA WSYNC", "        JMP L_8002", "        .byte $00, $FF", "        .word start"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.atari5200_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_a5200_edit");
    const image_writer::Written r = write(formats::atari5200::parse(program()), "demo.a52", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #$0E");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$0F");
    Bytes expected = program();
    expected[3] = 0x0F;
    CB_CHECK(m6502_source::assemble(m6502_source::Cpu::M6502, dialect::find("dasm").parse(s), dir.path()) == expected);
});

}  // namespace cb::source::atari5200_writer
