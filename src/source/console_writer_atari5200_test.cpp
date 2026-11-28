// cb-unity: alone
// The Atari 5200 writer over a small program written byte by byte: the source it writes, built again by CodeBreaker's dasm-syntax assembler, is the file.

#include "source/console_writer.hpp"

#include "source/dialect.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/atari5200.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer::atari5200_test {
namespace {

// `data` kept in `folder` as `name` and written beside it by the platform's writer (config/platforms/atari5200.json).
image_writer::Written write(const Bytes& data, std::string_view name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    fs::write_file(folder / std::string(name), data);
    return console_writer::reconstruct("atari5200", folder / std::string(name), folder);
}


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

CB_TEST("source.console_writer.atari5200.the_program_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_a5200");
    const image_writer::Written r = write(program(), "demo.a52", dir.path());
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

CB_TEST("source.console_writer.atari5200.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_a5200_edit");
    const image_writer::Written r = write(program(), "demo.a52", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #$0E");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$0F");
    Bytes expected = program();
    expected[3] = 0x0F;
    CB_CHECK(m6502_source::assemble(m6502_source::Cpu::M6502, dialect::find("dasm").parse(s), dir.path()) == expected);
});

}  // namespace cb::source::console_writer::atari5200_test
