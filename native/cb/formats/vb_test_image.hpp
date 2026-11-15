// A small Virtual Boy cartridge for the Virtual Boy tests, its code written with the V810 assembler's own syntax.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "cb/core/bytes.hpp"
#include "cb/decompile/v810.hpp"
#include "cb/formats/vb.hpp"

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
        const Bytes b = decompile::v810::assemble(line, kBase + static_cast<std::uint32_t>(offset));
        std::copy(b.begin(), b.end(), rom.begin() + static_cast<std::ptrdiff_t>(offset));
    };
    put(0x00, "movhi 0x0500,r0,r6");
    put(0x04, "ld.w 0[r6],r7");
    put(0x08, "add 1,r7");
    put(0x0A, "st.w r7,0[r6]");
    put(0x0E, "movhi 6,r0,r8");
    put(0x12, "ld.h -2048[r8],r9");
    put(0x16, "mov lp,r11");
    put(0x18, "jal 0x07000030");
    put(0x1C, "mov r11,lp");
    put(0x1E, "jmp [lp]");
    put(0x30, "mov 5,r10");
    put(0x32, "shl 1,r10");
    put(0x34, "jmp [lp]");
    put(size - 0x1C0, "reti");
    // `jr` is written with an absolute target: the reset slot's own address is the pc
    {
        const std::uint32_t pc = kBase + static_cast<std::uint32_t>(size - 0x10);
        const Bytes b = decompile::v810::assemble("jr 0x07000000", pc);
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
