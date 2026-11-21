// A small Virtual Boy cartridge for the Virtual Boy tests, its code written in the V810 SLEIGH description's syntax (the one assembler; its jr and jal are written as their bytes).
#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <string>

#include "core/bytes.hpp"
#include "source/processors.hpp"
#include "formats/vb.hpp"

namespace cb::formats::vb::test_image {

// A 16 KB ROM (the smallest the reader takes). The reset slot is `jr main`; the VIP's vector slot is `reti`. main (at $07000000):
//     movhi 0x0500,r0,r6 ; ld.w 0[r6],r7 ; add 1,r7 ; st.w r7,0[r6]          count in work RAM
//     movhi 6,r0,r8 ; ld.h -2048[r8],r9                                      read the VIP's interrupt-pending register ($0005F800)
//     mov lp,r11 ; jal sub ; mov r11,lp ; jmp [lp]
// sub (at $07000030): mov 5,r10 ; shl 1,r10 ; jmp [lp]
// The header says "TEST CART", maker "CB", game code "TEST", version 1.
inline Bytes cartridge(std::size_t size = 0x4000) {
    Bytes rom(size, 0xFF);
    const auto put = [&](std::size_t offset, const std::string& line) {
        const Bytes b = source::processors::assemble_described(source::processors::find("v810"), std::format(".org 0x{:x}\n        {}\n", kBase + static_cast<std::uint32_t>(offset), line), ".").data;
        std::copy(b.begin(), b.end(), rom.begin() + static_cast<std::ptrdiff_t>(offset));
    };
    put(0x00, "movhi 0x500,r0,r6");
    put(0x04, "ld.w 0x0[r6],r7");
    put(0x08, "add 0x1,r7");
    put(0x0A, "st.w r7,0x0[r6]");
    put(0x0E, "movhi 0x6,r0,r8");
    put(0x12, "ld.h -0x800[r8],r9");
    put(0x16, "mov r31,r11");
    {   // jal 0x07000030 (the description's assembler does not take the 26-bit displacement yet)
        const Bytes jal = {0x00, 0xac, 0x18, 0x00};
        std::copy(jal.begin(), jal.end(), rom.begin() + 0x18);
    }
    put(0x1C, "mov r11,r31");
    put(0x1E, "jmp [r31]");
    put(0x30, "mov 0x5,r10");
    put(0x32, "shl 0x1,r10");
    put(0x34, "jmp [r31]");
    put(size - 0x1C0, "reti");
    // `jr 0x07000000` in the reset slot (its displacement from the slot's own address)
    {
        const Bytes b = {0xff, 0xab, 0x10, 0xc0};
        std::copy(b.begin(), b.end(), rom.begin() + static_cast<std::ptrdiff_t>(size - 0x10));
    }
    const std::string title = "TEST CART           ";
    std::copy(title.begin(), title.begin() + 20, rom.begin() + static_cast<std::ptrdiff_t>(size - 0x220));
    std::fill(rom.begin() + static_cast<std::ptrdiff_t>(size - 0x220 + 20), rom.begin() + static_cast<std::ptrdiff_t>(size - 0x220 + 25), std::uint8_t{0});
    const std::string tail = "CBTEST";
    std::copy(tail.begin(), tail.end(), rom.begin() + static_cast<std::ptrdiff_t>(size - 0x220 + 25));
    rom[size - 0x220 + 31] = 1;
    return rom;
}

}  // namespace cb::formats::vb::test_image
