// cb-unity: alone
// The WonderSwan writer over a small program written byte by byte. The tests that build run NASM (settings: nasm, or CB_NASM) and are skipped where there is none.

#include "cb/source/wonderswan_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::source::wonderswan_writer {
namespace {

// F000:0000 mov ax, 0x1234 ; out DISPLAY_CTRL, al ; call sub ; jmp start.   sub F000:000E: in al, KEY_SCAN ; mov [0x200], al ; ret.   The footer jumps to F000:0000.
Bytes program() {
    Bytes rom(0x10000, 0xFF);
    const Bytes code = {0xB8, 0x34, 0x12, 0xE6, 0x00, 0xE8, 0x06, 0x00, 0xEB, 0xF6, 0x90, 0x90, 0x90, 0x90, 0xE4, 0xB5, 0xA2, 0x00, 0x02, 0xC3};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes footer = {0xEA, 0x00, 0x00, 0x00, 0xF0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    std::copy(footer.begin(), footer.end(), rom.end() - 16);
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i + 2 < rom.size(); ++i) sum += rom[i];
    rom[rom.size() - 2] = static_cast<std::uint8_t>(sum);
    rom[rom.size() - 1] = static_cast<std::uint8_t>(sum >> 8);
    return rom;
}

}  // namespace

CB_TEST("source.wonderswan_writer.the_code_scan_finds_the_start_and_the_call", [] {
    const formats::wonderswan::Rom rom = formats::wonderswan::parse(program());
    const std::vector<std::uint32_t> functions = rom.functions(decoder(rom.code_base()));
    CB_CHECK((functions == std::vector<std::uint32_t>{0xF0000, 0xF000E}));
});

CB_TEST("source.wonderswan_writer.the_program_comes_back_as_source_that_builds_the_same_file", [] {
    if (!find_nasm()) return;   // (no assembler here)
    fs::ScopedTempDir dir("cb_ws_writer");
    const image_writer::Written r = write(formats::wonderswan::parse(program()), "demo.ws", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0xF0000, "start"}, {0xF000E, "sub_F000E"}}));
    CB_CHECK_EQ(r.image_bytes, 0x10000u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"bits 16", "DISPLAY_CTRL", "KEY_SCAN", "equ 0xB5", "call sub_F000E", "start:", "sub_F000E:", "mov ax, 0x1234", "section .code"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

}  // namespace cb::source::wonderswan_writer
