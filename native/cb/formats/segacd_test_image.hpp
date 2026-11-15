// cb/formats/segacd_test_image.hpp -- a small Sega CD disc's system area for tests (header only; a test includes it).
//
//     Bytes boot = segacd_test_image::system_area();        // 32 KB, 2048-byte sectors
//     Bytes raw  = segacd_test_image::raw(boot);            // the same sectors as raw 2352-byte mode 1 sectors
//
// Main CPU (loaded at $FF0000 from disc offset $200):
//     $200 BRA.W $800 (the security code goes straight to the IP)
//     $800 MOVE.B #1,($A12003).L ; BSR.S $810 ; JMP ($200000).L          (the IP: gives word RAM to the sub CPU, waits, jumps to word RAM)
//     $810 TST.B ($A1200F).L ; BNE.S $810 ; RTS
// Sub CPU (the SP at disc offset $1000, loaded at $6000): a "MAIN-SUBCPU" module header whose jump table is at $20, and
//     init  MOVEQ #1,D0 ; JSR ($5F22).L ; RTS         (a CD BIOS call)
//     main  BRA.S *
//     int2  RTE
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <initializer_list>

#include "cb/core/bytes.hpp"

namespace cb::formats::segacd_test_image {

inline void put_bytes(Bytes& b, std::size_t at, std::initializer_list<std::uint8_t> bytes) { std::copy(bytes.begin(), bytes.end(), b.begin() + static_cast<std::ptrdiff_t>(at)); }

inline void put_long(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

inline void put_text(Bytes& b, std::size_t at, std::size_t width, const char* text) {
    for (std::size_t i = 0; i < width; ++i) b[at + i] = i < std::strlen(text) ? static_cast<std::uint8_t>(text[i]) : ' ';
}

inline Bytes system_area() {
    Bytes b(0x8000, 0);
    put_text(b, 0, 16, "SEGADISCSYSTEM");
    put_text(b, 0x10, 11, "TESTCDGAME");
    put_bytes(b, 0x1C, {0x01, 0x00, 0x00, 0x01});
    put_text(b, 0x20, 11, "TESTCDGAME");
    put_long(b, 0x30, 0x800);    // IP: start, size, entry, work RAM
    put_long(b, 0x34, 0x200);
    put_long(b, 0x40, 0x1000);   // SP
    put_long(b, 0x44, 0x400);
    put_text(b, 0x100, 16, "SEGA MEGA DRIVE");
    put_text(b, 0x110, 16, "(C)TEST 2026.NOV");
    put_text(b, 0x120, 48, "TEST DISC");
    put_text(b, 0x150, 48, "TEST DISC");
    put_text(b, 0x180, 14, "GM 00000000-00");
    put_text(b, 0x190, 16, "J");
    put_text(b, 0x1F0, 16, "E");
    put_bytes(b, 0x200, {0x60, 0x00, 0x05, 0xFE});   // BRA.W to the IP ($FF0600)
    put_bytes(b, 0x800, {0x13, 0xFC, 0x00, 0x01, 0x00, 0xA1, 0x20, 0x03, 0x61, 0x06, 0x4E, 0xF9, 0x00, 0x20, 0x00, 0x00});
    put_bytes(b, 0x810, {0x4A, 0x39, 0x00, 0xA1, 0x20, 0x0F, 0x66, 0xF8, 0x4E, 0x75});
    put_text(b, 0x1000, 11, "MAIN-SUBCPU");
    b[0x100B] = 0;
    put_long(b, 0x1018, 0x20);   // the jump table
    put_bytes(b, 0x1020, {0x00, 0x08, 0x00, 0x12, 0x00, 0x14, 0x00, 0x00});
    put_bytes(b, 0x1028, {0x70, 0x01, 0x4E, 0xB9, 0x00, 0x00, 0x5F, 0x22, 0x4E, 0x75});
    put_bytes(b, 0x1032, {0x60, 0xFE});
    put_bytes(b, 0x1034, {0x4E, 0x73});
    return b;
}

// The system area as a raw disc image: each 2048-byte sector in a 2352-byte one (sync, header, mode 1, user data, no error correction).
inline Bytes raw(const Bytes& cooked) {
    Bytes out;
    for (std::size_t s = 0; s * 2048 < cooked.size(); ++s) {
        Bytes sector(2352, 0);
        sector[0] = 0;
        for (std::size_t i = 1; i < 11; ++i) sector[i] = 0xFF;
        sector[11] = 0;
        sector[15] = 1;   // mode 1
        std::copy_n(cooked.begin() + static_cast<std::ptrdiff_t>(s * 2048), 2048, sector.begin() + 16);
        out.insert(out.end(), sector.begin(), sector.end());
    }
    return out;
}

}  // namespace cb::formats::segacd_test_image
