// A small CD-RTOS module and a disc holding it, written byte by byte for the CD-i tests.
#pragma once

#include <algorithm>
#include <cstdint>

#include "core/bytes.hpp"

namespace cb::formats::cdi::test_image {

inline void put16(Bytes& b, std::size_t at, std::uint16_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 8);
    b[at + 1] = static_cast<std::uint8_t>(v);
}
inline void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    put16(b, at, static_cast<std::uint16_t>(v >> 16));
    put16(b, at + 2, static_cast<std::uint16_t>(v));
}

// A program module "main" of $5C bytes: the header, then at its execution offset $40: MOVE.W #$1234,$200000.L ; BSR.S sub ; TRAP #0 + F_Exit ; BRA.S *;
// and sub at $50: TRAP #0 + I_Write ; RTS. `last_type` is the module's type byte.
inline Bytes module(std::uint8_t type = 1) {
    Bytes m(0x5C, 0);
    put16(m, 0, 0x4AFC);
    put16(m, 2, 1);
    put32(m, 4, 0x5C);
    put32(m, 12, 0x58);
    put16(m, 0x10, 0x0555);
    m[0x12] = type;
    m[0x13] = 1;
    m[0x14] = 0x80;
    m[0x15] = 1;
    put16(m, 0x16, 1);
    put32(m, 0x30, 0x40);
    put32(m, 0x38, 0x1000);
    put32(m, 0x3C, 0x400);
    const Bytes code = {0x33, 0xFC, 0x12, 0x34, 0x00, 0x20, 0x00, 0x00, 0x61, 0x06, 0x4E, 0x40, 0x00, 0x06, 0x60, 0xFE, 0x4E, 0x40, 0x00, 0x8A, 0x4E, 0x75};
    std::copy(code.begin(), code.end(), m.begin() + 0x40);
    const Bytes name = {'m', 'a', 'i', 'n' | 0x80};
    std::copy(name.begin(), name.end(), m.begin() + 0x58);
    std::uint16_t x = 0;
    for (std::size_t i = 0; i < 0x2E; i += 2) x ^= static_cast<std::uint16_t>(m[i] << 8 | m[i + 1]);
    put16(m, 0x2E, static_cast<std::uint16_t>(~x));
    return m;
}

// A disc image: the Disc Label in sector 0, the module at sector 2.
inline Bytes disc(const Bytes& mod = module()) {
    Bytes d(2048 * 3, 0);
    d[0] = 1;
    const char label[] = "CD-I ";
    std::copy(label, label + 5, d.begin() + 1);
    d[6] = 1;
    std::copy(mod.begin(), mod.end(), d.begin() + 4096);
    return d;
}

}  // namespace cb::formats::cdi::test_image
