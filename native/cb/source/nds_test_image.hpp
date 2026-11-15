// cb/source/nds_test_image.hpp -- a small Nintendo DS ROM for the tests of the DS lane: an ARM9 program (ARMv5TE: a call through a register
// to a Thumb function, BLX with a label, CLZ, LDRD, a coprocessor 15 write), an ARM7 program (ARMv4T: it writes the master interrupt register
// and calls a helper), the file name table, the file allocation table, a banner and one file. The programs are assembled with CodeBreaker's own
// assembler; the file is laid out here as ndstool lays one out.
#pragma once

#include <algorithm>
#include <cstring>
#include <string>

#include "cb/build/arm_asm.hpp"
#include "cb/core/bytes.hpp"
#include "cb/formats/gba.hpp"
#include "cb/formats/nds.hpp"
#include "cb/source/isa.hpp"

namespace cb::source::nds_test {

inline const char* arm9_source() {
    return R"(        .syntax unified
        .org 0x02000000
        .cpu arm946e-s
        .arm
arm9_start:
        push {r4, lr}
        ldr r4, pool_ime
        mov r0, #1
        str r0, [r4]
        mov r0, #0x100
        bl count
        ldr r1, pool_thumb
        blx r1
        blx thumb_two
        ldrd r2, r3, [r4, #8]
        mcr p15, 0, r0, c7, c10, 4
        pop {r4, pc}
count:  clz r0, r0
        bx lr
pool_ime:
        .word 0x04000208
pool_thumb:
        .word thumb_one + 1
        .thumb
thumb_one:
        movs r0, #7
        bx lr
thumb_two:
        push {lr}
        movs r0, #30
        pop {pc}
        .align 2
        .asciz "ARM9 DATA"
)";
}

inline const char* arm7_source() {
    return R"(        .syntax unified
        .org 0x037F8000
        .cpu arm7tdmi
        .arm
arm7_start:
        ldr r0, pool7
        mov r1, #1
        str r1, [r0]
        bl helper
        b arm7_start
helper: add r0, r0, r1
        bx lr
pool7:
        .word 0x04000208
)";
}

inline void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (unsigned i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}

inline std::uint32_t align_up(std::uint32_t v, std::uint32_t a) { return (v + a - 1) / a * a; }

inline Bytes test_rom() {
    const build::arm_asm::Assembler assembler(isa::arm_encoder(), &isa::arm9_encoder());
    const Bytes arm9 = assembler.build(arm9_source());
    const Bytes arm7 = assembler.build(arm7_source());
    const std::uint32_t arm9_at = 0x4000, arm7_at = align_up(arm9_at + static_cast<std::uint32_t>(arm9.size()) + 12, 0x200);
    const std::uint32_t fnt_at = align_up(arm7_at + static_cast<std::uint32_t>(arm7.size()), 0x200);
    Bytes fnt = {8, 0, 0, 0, 0, 0, 1, 0};   // the root directory: its names at 8, the first file 0, one directory
    const std::string name = "hello.txt";
    fnt.push_back(static_cast<std::uint8_t>(name.size()));
    fnt.insert(fnt.end(), name.begin(), name.end());
    fnt.push_back(0);
    const std::uint32_t fat_at = align_up(fnt_at + static_cast<std::uint32_t>(fnt.size()), 0x200);
    const std::uint32_t banner_at = align_up(fat_at + 8, 0x200);
    const std::uint32_t file_at = align_up(banner_at + 0x840, 0x200);
    const std::string content = "Hello from the DS file system.\n";
    const std::uint32_t total = file_at + static_cast<std::uint32_t>(content.size());
    Bytes rom(align_up(total, 0x200), 0);
    // the header
    std::memcpy(rom.data(), "NDSTEST", 7);
    std::memcpy(rom.data() + 0x0C, "ATSE", 4);
    std::memcpy(rom.data() + 0x10, "01", 2);
    rom[0x1E] = 1;
    put32(rom, 0x20, arm9_at);
    put32(rom, 0x24, 0x02000000);
    put32(rom, 0x28, 0x02000000);
    put32(rom, 0x2C, static_cast<std::uint32_t>(arm9.size()));
    put32(rom, 0x30, arm7_at);
    put32(rom, 0x34, 0x037F8000);
    put32(rom, 0x38, 0x037F8000);
    put32(rom, 0x3C, static_cast<std::uint32_t>(arm7.size()));
    put32(rom, 0x40, fnt_at);
    put32(rom, 0x44, static_cast<std::uint32_t>(fnt.size()));
    put32(rom, 0x48, fat_at);
    put32(rom, 0x4C, 8);
    put32(rom, 0x60, 0x00586000);
    put32(rom, 0x64, 0x001808F8);
    put32(rom, 0x68, banner_at);
    put32(rom, 0x80, total);
    put32(rom, 0x84, 0x4000);
    std::copy(formats::gba::kLogo.begin(), formats::gba::kLogo.end(), rom.begin() + 0xC0);
    rom[0x15C] = 0x56;
    rom[0x15D] = 0xCF;
    const std::uint16_t crc = formats::nds::crc16(ByteView(rom).subspan(0, 0x15E));
    rom[0x15E] = static_cast<std::uint8_t>(crc);
    rom[0x15F] = static_cast<std::uint8_t>(crc >> 8);
    std::copy(arm9.begin(), arm9.end(), rom.begin() + arm9_at);
    std::copy(arm7.begin(), arm7.end(), rom.begin() + arm7_at);
    std::copy(fnt.begin(), fnt.end(), rom.begin() + fnt_at);
    put32(rom, fat_at, file_at);
    put32(rom, fat_at + 4, total);
    rom[banner_at] = 1;
    const std::u16string title = u"Test\nCodeBreaker";
    for (std::size_t i = 0; i < title.size(); ++i) {
        rom[banner_at + 0x340 + 2 * i] = static_cast<std::uint8_t>(title[i]);   // (English)
        rom[banner_at + 0x341 + 2 * i] = static_cast<std::uint8_t>(title[i] >> 8);
    }
    for (std::size_t i = 0; i < 0x200; ++i) rom[banner_at + 0x20 + i] = static_cast<std::uint8_t>(i * 7);   // an icon
    std::copy(content.begin(), content.end(), rom.begin() + file_at);
    return rom;
}

}  // namespace cb::source::nds_test
