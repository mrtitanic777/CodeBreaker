// A small Neo Geo Pocket Color cartridge for the Neo Geo Pocket tests, its code written with the TLCS-900/H assembler's own syntax.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "cb/build/tlcs900_asm.hpp"
#include "cb/core/bytes.hpp"
#include "cb/formats/ngp.hpp"

namespace cb::formats::ngp::test_image {

// 4 KB at $200000: the licence text, start $200040, catalogue $0042, colour, "TEST CART". Then
//   start ($200040): kick the watchdog, install `vbl` as the VBlank handler, switch the display on, read the raster line, call `sub` with BC = 5 and a stack argument,
//                    call the BIOS (swi 1), point XHL at `table` and compare its third byte, loop to start when it is "0"
//   sub   ($200080): WA = the stack argument + BC; ret
//   vbl   ($2000A0): kick the watchdog; reti
//   table ($2000C0): "TEST0"
// and $FF for the rest.
inline Bytes cartridge(std::size_t size = 0x1000) {
    const std::string source =
        "        ORG 0x200040\n"
        "start:  ld (0x6F),0x4E\n"
        "        ld XWA,vbl\n"
        "        ld (0x6FCC),XWA\n"
        "        ld (0x8000),0x80\n"
        "        ld A,(0x8009)\n"
        "        ld BC,5\n"
        "        pushw 0x0003\n"
        "        call sub\n"
        "        inc 2,XSP\n"
        "        ld RW3,5\n"
        "        swi 1\n"
        "        ld XHL,table\n"
        "        ld A,(XHL+2)\n"
        "        cp A,0x53\n"
        "        jr Z,start\n"
        "        ret\n"
        "        ORG 0x200080\n"
        "sub:    ld WA,(XSP+4)\n"
        "        add WA,BC\n"
        "        ret\n"
        "        ORG 0x2000A0\n"
        "vbl:    ld (0x6F),0x4E\n"
        "        reti\n"
        "        ORG 0x2000C0\n"
        "table:  DB \"TEST0\"\n";
    const build::byte_asm::Image image = build::tlcs900_asm::build(source);
    Bytes rom(std::max<std::size_t>(size, 0x100), 0xFF);   // (built whole, then cut: nothing is written past the end)
    const std::string licence = " LICENSED BY SNK CORPORATION";
    std::copy(licence.begin(), licence.end(), rom.begin());
    const std::uint8_t fields[] = {0x40, 0x00, 0x20, 0x00, 0x42, 0x00, 0x03, 0x10};
    std::copy(std::begin(fields), std::end(fields), rom.begin() + 0x1C);
    const std::string title = "TEST CART   ";
    std::copy(title.begin(), title.end(), rom.begin() + 0x24);
    std::fill(rom.begin() + 0x30, rom.begin() + 0x40, std::uint8_t{0});
    std::copy(image.data.begin(), image.data.end(), rom.begin() + (image.base - kRomBase));
    return rom;
}

}  // namespace cb::formats::ngp::test_image
