// A small Neo Geo Pocket Color cartridge for the Neo Geo Pocket tests, its code written in the TLCS-900/H SLEIGH description's syntax (the one assembler).
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "source/processors.hpp"
#include "core/bytes.hpp"
#include "formats/ngp.hpp"

namespace cb::formats::ngp::test_image {

// 4 KB at $200000: the licence text, start $200040, catalogue $0042, colour, "TEST CART". Then
//   start ($200040): kick the watchdog, install `vbl` as the VBlank handler, switch the display on, read the raster line, call `sub` with BC = 5 and a stack argument,
//                    call the BIOS (swi 1), point XHL at `table` and compare its third byte, loop to start when it is "0"
//   sub   ($200080): WA = the stack argument + BC; ret
//   vbl   ($2000A0): kick the watchdog; reti
//   table ($2000C0): "TEST0"
// and $FF for the rest.
inline Bytes cartridge(std::size_t size = 0x1000) {
    // the four stretches of code, each at its address (the one assembler's `.org` does not pad a file): the names are the addresses
    const std::string names =
        "        .equ start, 0x200040\n"
        "        .equ sub, 0x200080\n"
        "        .equ vbl, 0x2000a0\n"
        "        .equ table, 0x2000c0\n";
    const std::string at_start =
        "        .org 0x200040\n"
        "        ld (0x6f),0x4e\n"
        "        ld XWA,vbl\n"
        "        ld (0x6fcc),XWA\n"
        "        ld (0x8000),0x80\n"
        "        ld A,(0x8009)\n"
        "        ld BC,0x5\n"
        "        pushw 0x3\n"
        "        call sub\n"
        "        inc 2,XSP\n"
        "        ld RW3,0x5\n"
        "        swi 0x1\n"
        "        ld XHL,table\n"
        "        ld A,(XHL+0x2)\n"
        "        cp A,0x53\n"
        "        jr Z,start\n"
        "        ret\n";
    const std::string at_sub =
        "        .org 0x200080\n"
        "        ld WA,(XSP+0x4)\n"
        "        add WA,BC\n"
        "        ret\n";
    const std::string at_vbl =
        "        .org 0x2000a0\n"
        "        ld (0x6f),0x4e\n"
        "        reti\n";
    const std::string at_table =
        "        .org 0x2000c0\n"
        "        .ascii \"TEST0\"\n";
    Bytes rom(std::max<std::size_t>(size, 0x100), 0xFF);   // (built whole, then cut: nothing is written past the end)
    const auto place = [&](const std::string& piece, std::uint32_t address) {
        const Bytes code = source::processors::assemble_described(source::processors::find("tlcs900"), names + piece, ".").data;
        std::copy(code.begin(), code.end(), rom.begin() + (address - kRomBase));
    };
    const std::string licence = " LICENSED BY SNK CORPORATION";
    std::copy(licence.begin(), licence.end(), rom.begin());
    const std::uint8_t fields[] = {0x40, 0x00, 0x20, 0x00, 0x42, 0x00, 0x03, 0x10};
    std::copy(std::begin(fields), std::end(fields), rom.begin() + 0x1C);
    const std::string title = "TEST CART   ";
    std::copy(title.begin(), title.end(), rom.begin() + 0x24);
    std::fill(rom.begin() + 0x30, rom.begin() + 0x40, std::uint8_t{0});
    place(at_start, 0x200040);
    place(at_sub, 0x200080);
    place(at_vbl, 0x2000a0);
    place(at_table, 0x2000c0);
    return rom;
}

}  // namespace cb::formats::ngp::test_image
