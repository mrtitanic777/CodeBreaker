// Tests of the Atari 7800 writer over a small program of its own, written out byte by byte: the source it writes, built again by CodeBreaker's dasm-syntax
// assembler, is the cartridge file it came from (the header included).

#include "source/console_writer.hpp"

#include "source/dialect.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/atari7800.hpp"
#include "core/text.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::console_writer::atari7800_test {
namespace {

// `data` kept in `folder` as `name` and written beside it by the platform's writer (config/platforms/atari7800.json).
image_writer::Written write(const Bytes& data, std::string_view name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    fs::write_file(folder / std::string(name), data);
    return console_writer::reconstruct("atari7800", folder / std::string(name), folder);
}


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

CB_TEST("source.console_writer.atari7800.the_program_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_a7800");
    const image_writer::Written r = write(program(), "demo.a78", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0xC000, "reset"}, {0xC020, "sub_C020"}, {0xC030, "nmi"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    CB_CHECK_EQ(r.code_bytes, 12u + 8u + 1u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"        processor 6502", "CTRL       = $3C", "MSTAT      = $28", "WSYNC      = $24", "        ORG $0000", "        ORG $0080",
                             "        RORG $C000", "        STA CTRL", "        JSR sub_C020", "        LDA MSTAT", "        BPL sub_C020", "        STA.w WSYNC",
                             "        JMP L_C006", "        .word nmi, reset, nmi", "reset\n", "nmi\n"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.console_writer.atari7800.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_a7800_edit");
    const image_writer::Written r = write(program(), "demo.a78", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("LDA #$40");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 8, "LDA #$41");
    const Bytes edited = m6502_source::assemble(m6502_source::Cpu::M6502, dialect::find("dasm").parse(s), dir.path());
    Bytes expected = program();
    expected[128 + 3] = 0x41;
    CB_CHECK(edited == expected);
});

CB_TEST("source.console_writer.atari7800.a_supergame_board_writes_each_bank_and_does_not_guess_the_selected_one", [] {
    fs::ScopedTempDir dir("cb_a7800_sg");
    const image_writer::Written r = write(program(formats::atari7800::kTypeSuperGame, 0x20000), "sg.a78", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, 0x20000u);
    CB_CHECK_EQ(r.functions.at(0x7C000), "reset");   // in the fixed bank (7)
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("        RORG $8000") != std::string::npos);
    CB_CHECK(s.find("        RORG $C000") != std::string::npos);
    CB_CHECK(s.find("        ORG $0080") != std::string::npos);
});

}  // namespace
}  // namespace cb::source::console_writer::atari7800_test
