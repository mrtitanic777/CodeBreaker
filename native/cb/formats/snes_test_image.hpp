// cb/formats/snes_test_image.hpp -- a small LoROM cartridge for tests (header only; a test includes it): 32 KB.
//
//     Bytes file = snes_test_image::cartridge();         // 32 KB
//     Bytes file = snes_test_image::cartridge(true);      // the same behind a 512-byte copier header
//     Bytes file = snes_test_image::hirom();              // 64 KB, HiROM
//     snes_test_image::put(rom, 0x100, {0xA9, 0x01});     // code of a test at $00:8100 (the checksum is made again)
//
// LoROM, code from $00:8000. reset: SEI, CLC, XCE (native), REP #$30, LDA #$1234, LDX #$1FFF, TXS, SEP #$20, LDA #$8F, STA INIDISP ($2100), JSL $008040,
// JSR $008050, JML to itself. $8040 (entered with 16-bit index registers): LDX #$0010, LDA $009000,X (long), STA $10, STA VMDATAL ($2118), RTL. $8050:
// REP #$20, LDA #$ABCD, STA $0300, SEP #$20, RTS. nmi $8060, irq $8061, cop $8062 and brk $8063 are RTI. $9000 holds 0 1 2 ... $1F.
// The HiROM one has reset at $C0:8000 (CLC, XCE, REP #$30, LDA #$1234, STA $2000, JSL $C08020, RTL) and, at $C0:8020, LDA #$0001, RTL.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats::snes_test_image {

// The internal header at `at` of `rom`: title, map mode, size, vectors (native NMI, IRQ, COP, BRK at $8060-$8063; emulation RESET at `reset`), and the
// checksum pair, which is the sum of every byte with the pair read as $FFFF and $0000.
inline void header(Bytes& rom, std::size_t at, std::uint8_t map_mode, std::uint8_t size_code, std::uint16_t reset) {
    const std::string_view title = "CODEBREAKER TEST";
    std::fill(rom.begin() + static_cast<std::ptrdiff_t>(at), rom.begin() + static_cast<std::ptrdiff_t>(at) + 21, std::uint8_t{' '});
    std::copy(title.begin(), title.end(), rom.begin() + static_cast<std::ptrdiff_t>(at));
    rom[at + 0x15] = map_mode;
    rom[at + 0x17] = size_code;
    rom[at + 0x1A] = 0x33;
    const auto word = [&](std::size_t off, std::uint16_t v) {
        rom[at + off] = static_cast<std::uint8_t>(v);
        rom[at + off + 1] = static_cast<std::uint8_t>(v >> 8);
    };
    word(0x24, 0x8062);   // native COP
    word(0x26, 0x8063);   // BRK
    word(0x28, 0x8060);   // ABORT
    word(0x2A, 0x8060);   // NMI
    word(0x2E, 0x8061);   // IRQ
    word(0x34, 0x8062);   // emulation COP
    word(0x38, 0x8060);   // ABORT
    word(0x3A, 0x8060);   // NMI
    word(0x3C, reset);    // RESET
    word(0x3E, 0x8063);   // IRQ / BRK
    rom[at + 0x1C] = rom[at + 0x1D] = 0xFF;
    rom[at + 0x1E] = rom[at + 0x1F] = 0;
    std::uint32_t sum = 0;
    for (const std::uint8_t b : rom) sum += b;
    const std::uint16_t checksum = static_cast<std::uint16_t>(sum);
    word(0x1C, static_cast<std::uint16_t>(checksum ^ 0xFFFF));
    word(0x1E, checksum);
}

inline void put(Bytes& rom, std::size_t at, const Bytes& code) { std::copy(code.begin(), code.end(), rom.begin() + static_cast<std::ptrdiff_t>(at)); }

inline void remake_checksum(Bytes& rom, std::size_t header_at, std::uint8_t map_mode, std::uint8_t size_code, std::uint16_t reset) { header(rom, header_at, map_mode, size_code, reset); }

inline Bytes cartridge(bool copier_header = false) {
    Bytes rom(0x8000, 0xFF);
    put(rom, 0x000, {0x78, 0x18, 0xFB, 0xC2, 0x30, 0xA9, 0x34, 0x12, 0xA2, 0xFF, 0x1F, 0x9A, 0xE2, 0x20, 0xA9, 0x8F, 0x8D, 0x00, 0x21,
                     0x22, 0x40, 0x80, 0x00, 0x20, 0x50, 0x80, 0x5C, 0x1A, 0x80, 0x00});
    put(rom, 0x040, {0xA2, 0x10, 0x00, 0xBF, 0x00, 0x90, 0x00, 0x85, 0x10, 0x8D, 0x18, 0x21, 0x6B});
    put(rom, 0x050, {0xC2, 0x20, 0xA9, 0xCD, 0xAB, 0x8D, 0x00, 0x03, 0xE2, 0x20, 0x60});
    put(rom, 0x060, {0x40, 0x40, 0x40, 0x40});
    for (std::uint8_t i = 0; i < 0x20; ++i) rom[0x1000 + i] = i;
    header(rom, 0x7FC0, 0x20, 0x05, 0x8000);
    Bytes file(copier_header ? 512 : 0, 0);
    if (copier_header) file[0] = 0x40;   // (a copier header's bytes are the copier's)
    file.insert(file.end(), rom.begin(), rom.end());
    return file;
}

inline Bytes hirom() {
    Bytes rom(0x10000, 0xFF);
    put(rom, 0x8000, {0x18, 0xFB, 0xC2, 0x30, 0xA9, 0x34, 0x12, 0x8D, 0x00, 0x20, 0x22, 0x20, 0x80, 0xC0, 0x6B});
    put(rom, 0x8020, {0xA9, 0x01, 0x00, 0x6B});
    put(rom, 0x8060, {0x40, 0x40, 0x40, 0x40});
    header(rom, 0xFFC0, 0x21, 0x06, 0x8000);
    return rom;
}

}  // namespace cb::formats::snes_test_image
