// A small Vectrex cartridge written byte by byte for the Vectrex tests.
#pragma once

#include <algorithm>
#include <cstdint>

#include "core/bytes.hpp"

namespace cb::formats::vectrex::test_image {

// Header `g GCE 1982` $80, music $FD0D, one title line `TEST`, a zero: the program at $17.
//   $17 JSR Wait_Recal ; LDA #$7F ; STA VIA_int_enable ; BSR sub ; BRA start ; (two NOPs: not reached)   sub $25: LDA $C800 ; INCA ; STA $C800 ; RTS
inline Bytes cartridge(std::size_t size = 0x40) {
    Bytes rom(std::max<std::size_t>(size, 0x40), 0xFF);   // (built whole, then cut to `size`: a short one is a cut-off cartridge)
    const Bytes header = {'g', ' ', 'G', 'C', 'E', ' ', '1', '9', '8', '2', 0x80, 0xFD, 0x0D, 0xF8, 0x50, 0x20, 0xD0, 'T', 'E', 'S', 'T', 0x80, 0x00};
    std::copy(header.begin(), header.end(), rom.begin());
    const Bytes code = {0xBD, 0xF1, 0x92, 0x86, 0x7F, 0xB7, 0xD0, 0x0E, 0x8D, 0x04, 0x20, 0xF4, 0x12, 0x12, 0xB6, 0xC8, 0x00, 0x4C, 0xB7, 0xC8, 0x00, 0x39};
    std::copy(code.begin(), code.begin() + 14, rom.begin() + 0x17);
    std::copy(code.begin() + 14, code.end(), rom.begin() + 0x25);
    rom.resize(size);
    return rom;
}

}  // namespace cb::formats::vectrex::test_image
