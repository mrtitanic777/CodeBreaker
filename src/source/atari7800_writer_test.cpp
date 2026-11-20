// Tests of the Atari 7800 writer over a small program of its own, written out byte by byte: the source it writes, built again by the 6502's SLEIGH
// description, is the cartridge file it came from (the header included).

#include "source/atari7800_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::atari7800_writer {
namespace {

// reset $C000: SEI CLD LDA #$40 STA CTRL JSR $C020 JMP $C006; sub $C020: LDA MSTAT, BPL, STA.w WSYNC, RTS; the interrupt handler $C030: RTI.
Bytes program(std::uint16_t type = 0, std::size_t size = 0x4000) {
    Bytes file(128, 0);
    file[0] = 1;
    const std::string magic = "ATARI7800";
    std::copy(magic.begin(), magic.end(), file.begin() + 1);
    const std::string title = "TEST";
    std::copy(title.begin(), title.end(), file.begin() + 17);
    file[49] = static_cast<std::uint8_t>(size >> 24);
    file[50] = static_cast<std::uint8_t>(size >> 16);
    file[51] = static_cast<std::uint8_t>(size >> 8);
    file[52] = static_cast<std::uint8_t>(size);
    file[53] = static_cast<std::uint8_t>(type >> 8);
    file[54] = static_cast<std::uint8_t>(type);
    Bytes rom(size, 0xFF);
    const std::size_t top = size - 0x4000;   // (a SuperGame's fixed bank is the last 16 KB)
    const Bytes code = {0x78, 0xD8, 0xA9, 0x40, 0x85, 0x3C, 0x20, 0x20, 0xC0, 0x4C, 0x06, 0xC0};
    std::copy(code.begin(), code.end(), rom.begin() + top);
    const Bytes sub = {0xA5, 0x28, 0x10, 0xFC, 0x8D, 0x24, 0x00, 0x60};
    std::copy(sub.begin(), sub.end(), rom.begin() + top + 0x20);
    rom[top + 0x30] = 0x40;
    const Bytes vectors = {0x30, 0xC0, 0x00, 0xC0, 0x30, 0xC0};
    std::copy(vectors.begin(), vectors.end(), rom.end() - 6);
    file.insert(file.end(), rom.begin(), rom.end());
    return file;
}

CB_TEST("source.atari7800_writer.the_program_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_a7800");
    const image_writer::Written r = write(formats::atari7800::parse(program()), "demo.a78", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0xC000, "reset"}, {0xC020, "sub_C020"}, {0xC030, "nmi"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    CB_CHECK_EQ(r.code_bytes, 12u + 8u + 1u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    const std::string s = fs::read_text(r.source);
    // (STA $0024 with the absolute encoding, which the description's text cannot say: `STA 0x24` is the zero-page one, so it stays as its bytes)
    for (const char* want : {".equ CTRL, 0x3c", ".equ MSTAT, 0x28", ".equ WSYNC, 0x24", "        .org 0x0000", "; file offset 0x0080\n        .org 0xc000",
                             "        STA CTRL", "        JSR sub_C020", "        LDA MSTAT", "        BPL sub_C020", "        .byte 0x8d,0x24,0x00  ; STA 0x24",
                             "        JMP L_C006", "        .half nmi,reset,nmi", "reset:\n", "nmi:\n"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.atari7800_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_a7800_edit");
    const image_writer::Written r = write(formats::atari7800::parse(program()), "demo.a78", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #0x40");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 9, "LDA #0x41");
    const Bytes edited = m6502_source::assemble(m6502_source::Cpu::M6502, s, dir.path());
    Bytes expected = program();
    expected[128 + 3] = 0x41;
    CB_CHECK(edited == expected);
});

CB_TEST("source.atari7800_writer.a_supergame_board_writes_each_bank_and_does_not_guess_the_selected_one", [] {
    fs::ScopedTempDir dir("cb_a7800_sg");
    const image_writer::Written r = write(formats::atari7800::parse(program(formats::atari7800::kTypeSuperGame, 0x20000)), "sg.a78", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, 0x20000u);
    CB_CHECK_EQ(r.functions.at(0x7C000), "reset");   // in the fixed bank (7)
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("        .org 0x8000") != std::string::npos);
    CB_CHECK(s.find("        .org 0xc000") != std::string::npos);
    CB_CHECK(s.find("; file offset 0x0080\n") != std::string::npos);
});

}  // namespace
}  // namespace cb::source::atari7800_writer
