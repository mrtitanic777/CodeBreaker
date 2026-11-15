// cb/source/sh2_test_programs.hpp -- small SH-2 programs and the images made of them (a 32X cartridge, a Saturn disc), for the tests of the SH-2's
// format readers, writers, interpreter and lanes. The programs are assembled from GNU as source with CodeBreaker's own assembler, so every
// test works without a real ROM.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>

#include "cb/build/sh_asm.hpp"
#include "cb/core/bytes.hpp"
#include "cb/source/isa_sh2.hpp"

namespace cb::source::sh2_test {

// The 32X's SH-2 program at SDRAM ($06000000): the vector table, `entry` (the master's start: it counts to ten in a communication port,
// calls `helper` through a constant, calls `tally`), a word constant, a string and a table of the three functions.
inline constexpr std::string_view kSdramProgram = R"(
        .org 0x06000000
        .long entry
        .long 0x0603FFFC
        .long entry
        .long 0x0603FFFC
        .space 48
entry:
        mov.l   r14,@-r15
        sts.l   pr,@-r15
        mov.l   K_comm,r1
        mov.l   K_count,r2
        mov     #0,r3
loop:
        add     #1,r3
        mov.l   r3,@r1
        cmp/ge  r2,r3
        bf      loop
        mov.l   K_helper,r0
        jsr     @r0
        mov     r3,r4
        mov.l   r0,@(4,r1)
        bsr     tally
        nop
        lds.l   @r15+,pr
        mov.l   @r15+,r14
        rts
        nop
        .align 2
K_comm: .long 0x20004020
K_count: .long 10
K_helper: .long helper
tally:
        mov.w   K_mask,r1
        and     r1,r0
        rts
        add     #1,r0
K_mask: .word 0x00FF
        .align 2
helper:
        mov.l   r14,@-r15
        mov     r4,r0
        add     #3,r0
        rts
        mov.l   @r15+,r14
msg:    .asciz "hello, 32x"
        .align 2
table:  .long entry, tally, helper
        .long 0xFFFFFFFF
        .space 32
)";

// The Saturn's first program at $06004000: waits for the vertical blank, reads the SCU's interrupt status into a variable, calls `leaf`.
inline constexpr std::string_view kSaturnProgram = R"(
        .org 0x06004000
first:  mov.l   r14,@-r15
        sts.l   pr,@-r15
        mov.l   K_tvstat,r1
wait:   mov.w   @r1,r0
        tst     #8,r0
        bt      wait
        mov.l   K_ist,r2
        mov.l   @r2,r3
        mov.l   K_var,r4
        mov.l   r3,@r4
        bsr     leaf
        nop
        lds.l   @r15+,pr
        mov.l   @r15+,r14
        rts
        nop
        .align 2
K_tvstat: .long 0x25F80004
K_ist:  .long 0x25FE00A4
K_var:  .long 0x06080000
leaf:   mov.l   @r4,r0
        rts
        add     #1,r0
        .asciz "SATURN"
        .align 2
        .long 0x12345678
)";

inline Bytes assemble(std::string_view source) { return build::sh_asm::Assembler(isa::sh2_encoder()).build(source); }

inline void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
inline void put_text(Bytes& b, std::size_t at, std::string_view text, std::size_t width) {
    for (std::size_t i = 0; i < width; ++i) b[at + i] = i < text.size() ? static_cast<std::uint8_t>(text[i]) : ' ';
}

// A 32X cartridge: the Genesis header, the 32X's header with the SH-2 program at ROM offset $1000, a 68000 initial program at $3F0.
inline Bytes cartridge_32x(std::string_view sdram_program = kSdramProgram) {
    Bytes rom(0x4000, 0);
    const Bytes sh2 = assemble(sdram_program);
    put32(rom, 0, 0x00FF0000);
    put32(rom, 4, 0x000003F0);
    put_text(rom, 0x100, "SEGA 32X", 16);
    put_text(rom, 0x110, "(C)TEST 2026.NOV", 16);
    put_text(rom, 0x120, "SH-2 TEST CARTRIDGE", 48);
    put_text(rom, 0x150, "SH-2 TEST CARTRIDGE", 48);
    put_text(rom, 0x180, "GM 00000000-00", 14);
    put_text(rom, 0x190, "J", 16);
    put32(rom, 0x1A0, 0);
    put32(rom, 0x1A4, static_cast<std::uint32_t>(rom.size() - 1));
    put32(rom, 0x1A8, 0x00FF0000);
    put32(rom, 0x1AC, 0x00FFFFFF);
    put_text(rom, 0x1F0, "JUE", 3);
    put_text(rom, 0x3C0, "TESTROM32X", 16);
    put32(rom, 0x3D4, 0x1000);
    put32(rom, 0x3DC, static_cast<std::uint32_t>(sh2.size()));
    put32(rom, 0x3E0, 0x06000040);
    put32(rom, 0x3E4, 0x06000040);
    put32(rom, 0x3E8, 0x06000000);
    put32(rom, 0x3EC, 0x06000000);
    const std::uint8_t initial[] = {0x46, 0xFC, 0x27, 0x00, 0x60, 0xFE};   // move.w #$2700,sr ; bra.s *
    std::copy(std::begin(initial), std::end(initial), rom.begin() + 0x3F0);
    std::copy(sh2.begin(), sh2.end(), rom.begin() + 0x1000);
    return rom;
}

// A Saturn disc (2048 bytes a sector): the 32 KB system area with IP.BIN of $1000 bytes, an ISO 9660 volume whose root holds 0.BIN (the
// first program) after the licence text ABS.TXT, which is not a program.
inline Bytes disc_saturn(std::string_view program = kSaturnProgram) {
    constexpr std::size_t kSectorBytes = 2048;
    const Bytes first = assemble(program);
    Bytes disc(kSectorBytes * 18 + ((first.size() + kSectorBytes - 1) / kSectorBytes) * kSectorBytes, 0);
    put_text(disc, 0x00, "SEGA SEGASATURN ", 16);
    put_text(disc, 0x10, "SEGA TP T-0001  ", 16);
    put_text(disc, 0x20, "T-0001    ", 10);
    put_text(disc, 0x2A, "V1.000", 6);
    put_text(disc, 0x30, "20261115", 8);
    put_text(disc, 0x38, "CD-1/1  ", 8);
    put_text(disc, 0x40, "JTUE", 10);
    put_text(disc, 0x50, "J", 16);
    put_text(disc, 0x60, "SH-2 TEST DISC", 112);
    put32(disc, 0xE0, 0x1000);
    put32(disc, 0xE8, 0x06004000);
    put32(disc, 0xEC, 0x06001000);
    put32(disc, 0xF0, 0x06004000);
    // the four area-code entries, each a `bra` to the initial program, then the initial program: it loads the first program and jumps to it
    static constexpr const char* kAreas[] = {"For JAPAN.", "For TAIWAN.", "For USA.", "For EUROPE."};
    for (std::size_t i = 0; i < 4; ++i) {
        const Bytes entry = assemble("        .org " + std::to_string(0x06002E00 + 0x20 * i) + "\n        bra 0x06002E80\n        nop\n");
        std::copy(entry.begin(), entry.end(), disc.begin() + 0xE00 + 0x20 * i);
        const std::string_view area = kAreas[i];
        std::copy(area.begin(), area.end(), disc.begin() + 0xE00 + 0x20 * i + 4);
    }
    const Bytes init = assemble(R"(
        .org 0x06002E80
        mov.l   K_first,r0
        jmp     @r0
        nop
        .align 2
K_first: .long 0x06004000
)");
    std::copy(init.begin(), init.end(), disc.begin() + 0xE80);
    // the volume: a primary descriptor at sector 16, the root directory at 17, the program at 18
    const std::size_t pvd = 16 * kSectorBytes;
    disc[pvd] = 1;
    put_text(disc, pvd + 1, "CD001", 5);
    disc[pvd + 6] = 1;
    const auto both32 = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) {
            disc[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
            disc[at + 4 + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
        }
    };
    const auto record = [&](std::size_t at, std::uint32_t lba, std::uint32_t size, bool directory, std::string_view name) {
        const std::size_t length = 33 + name.size() + (name.size() % 2 == 0 ? 1 : 0);
        disc[at] = static_cast<std::uint8_t>(length);
        both32(at + 2, lba);
        both32(at + 10, size);
        disc[at + 25] = directory ? 2 : 0;
        disc[at + 32] = static_cast<std::uint8_t>(name.size());
        std::copy(name.begin(), name.end(), disc.begin() + static_cast<std::ptrdiff_t>(at) + 33);
        return length;
    };
    record(pvd + 156, 17, kSectorBytes, true, std::string(1, '\0'));
    std::size_t at = 17 * kSectorBytes;
    at += record(at, 17, kSectorBytes, true, std::string(1, '\0'));
    at += record(at, 17, kSectorBytes, true, std::string(1, '\1'));
    at += record(at, 18, 0x10, false, "ABS.TXT;1");
    at += record(at, 18, static_cast<std::uint32_t>(first.size()), false, "0.BIN;1");
    std::copy(first.begin(), first.end(), disc.begin() + 18 * static_cast<std::ptrdiff_t>(kSectorBytes));
    return disc;
}

// What the writer rebuilds a disc into: the system area and the first program.
inline Bytes disc_image(const Bytes& disc, std::size_t program_size) {
    Bytes image(disc.begin(), disc.begin() + 0x8000);
    image.insert(image.end(), disc.begin() + 18 * 2048, disc.begin() + 18 * 2048 + static_cast<std::ptrdiff_t>(program_size));
    return image;
}

}  // namespace cb::source::sh2_test
