// cb/formats/pce_test_image.hpp -- a small HuCard for tests (header only; a test includes it): two 8 KB banks.
//
//     Bytes file = pce_test_image::cartridge();         // 16 KB
//     Bytes file = pce_test_image::cartridge(true);      // the same behind a 512-byte copier header
//
// Bank 0 (at $E000): reset sets up the mapping (CSH, TAM #$01 with $FF, TAM #$02 with $F8, TAM #$04 with bank 1), calls bank 1's function at $4000 (JSR)
// and the subroutine at $E020 (BSR), writes the VDC (ST0 #$05, ST1, ST2) and loops. The subroutine at $E020 loads a zero-page byte, stores it
// plus one, copies four bytes from $E100 to the RAM (TII), takes a BBS0, and writes $0000. nmi $E040, irq1 $E041 and irq2 and timer $E042 are RTI.
// Bank 1 (at $4000): LDA #$3C, STA $0002 (the VDC's data port), TST #$01,$20, RTS.
#pragma once

#include <algorithm>
#include <cstdint>

#include "cb/core/bytes.hpp"

namespace cb::formats::pce_test_image {

inline Bytes cartridge(bool header = false) {
    Bytes bank0(0x2000, 0xFF), bank1(0x2000, 0xFF);
    const auto put = [](Bytes& bank, std::size_t at, const Bytes& code) { std::copy(code.begin(), code.end(), bank.begin() + static_cast<std::ptrdiff_t>(at)); };
    put(bank0, 0x000, {0x78, 0xD4, 0xD8, 0xA9, 0xFF, 0x53, 0x01, 0xA9, 0xF8, 0x53, 0x02, 0xA9, 0x01, 0x53, 0x04, 0x20, 0x00, 0x40, 0x44, 0x0C, 0x03, 0x05,
                       0x13, 0x80, 0x23, 0x00, 0x80, 0xFE});
    put(bank0, 0x020, {0xA5, 0x20, 0x1A, 0x85, 0x21, 0x73, 0x00, 0xE1, 0x00, 0x20, 0x04, 0x00, 0x8F, 0x21, 0x01, 0xEA, 0xA9, 0x07, 0x8D, 0x00, 0x00, 0x60});
    put(bank0, 0x040, {0x40, 0x40, 0x40});
    put(bank0, 0x100, {0x01, 0x02, 0x03, 0x04});
    put(bank0, 0x1FF6, {0x42, 0xE0, 0x41, 0xE0, 0x42, 0xE0, 0x40, 0xE0, 0x00, 0xE0});   // IRQ2, IRQ1, TIMER, NMI, RESET
    put(bank1, 0x000, {0xA9, 0x3C, 0x8D, 0x02, 0x00, 0x83, 0x01, 0x20, 0x60});
    Bytes file(header ? 512 : 0, 0);
    if (header) file[0] = 0x40;   // (a copier header's bytes are the copier's)
    file.insert(file.end(), bank0.begin(), bank0.end());
    file.insert(file.end(), bank1.begin(), bank1.end());
    return file;
}

}  // namespace cb::formats::pce_test_image
