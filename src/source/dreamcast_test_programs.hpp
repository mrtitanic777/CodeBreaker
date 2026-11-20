// source/dreamcast_test_programs.hpp -- small SH-4 programs and the disc images made of them, for the tests of the Dreamcast's format reader, writer,
// interpreter and lane. The programs are assembled from GNU as source with CodeBreaker's own SH-4 assembler, so every test works without a real disc.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <algorithm>
#include "build/sh_asm.hpp"
#include "core/bytes.hpp"
#include "source/dreamcast_writer.hpp"
#include "source/isa_sh2.hpp"

namespace cb::source::dreamcast_test {

// 1ST_READ.BIN at $0C010000 (physical; the program names it $8C010000): waits for the PowerVR's vertical sync, stores 1.0 + 1.0 in a variable of
// the work window, calls `leaf` (which doubles a float it reads and returns its integer part in r0), and `syscall`, which calls the BIOS's
// system-information vector.
inline constexpr std::string_view kProgram = R"(
        .org 0x0C010000
first:  mov.l   r14,@-r15
        sts.l   pr,@-r15
        mov.l   K_spg,r1
        mov.l   K_mask,r2
wait:   mov.l   @r1,r0
        and     r2,r0
        tst     r0,r0
        bt      wait
        mov.l   K_var,r4
        fldi1   fr0
        fldi1   fr1
        fadd    fr1,fr0
        fmov.s  fr0,@r4
        bsr     leaf
        nop
        lds.l   @r15+,pr
        mov.l   @r15+,r14
        rts
        nop
        .align 2
K_spg:  .long 0xA05F810C
K_mask: .long 0x2000
K_var:  .long 0x8C800000
leaf:   fmov.s  @r4,fr0
        fadd    fr0,fr0
        ftrc    fr0,fpul
        sts     fpul,r0
        rts
        nop
syscall:
        sts.l   pr,@-r15
        mov.l   K_sysinfo,r1
        mov.l   @r1,r1
        jsr     @r1
        mov     #7,r7
        lds.l   @r15+,pr
        rts
        nop
        .align 2
K_sysinfo: .long 0x8C0000B0
        .asciz "DREAMCAST"
        .align 2
        .long 0x12345678
)";


inline Bytes assemble(std::string_view source) { return build::sh_asm::Assembler(isa::sh4_encoder(), true).build(source); }

inline void put_text(Bytes& b, std::size_t at, std::string_view text, std::size_t width) {
    for (std::size_t i = 0; i < width; ++i) b[at + i] = i < text.size() ? static_cast<std::uint8_t>(text[i]) : ' ';
}

// IP.BIN's 32 KB: the header's fields, a bootstrap at $300 that jumps to 1ST_READ.BIN, and zeros.
inline Bytes ip_bin() {
    Bytes ip(formats::dreamcast::kSystemBytes, 0);
    put_text(ip, 0x00, "SEGA SEGAKATANA ", 16);
    put_text(ip, 0x10, "SEGA ENTERPRISES", 16);
    put_text(ip, 0x20, "0000 GD-ROM1/1  ", 16);
    put_text(ip, 0x30, "JUE     ", 8);
    put_text(ip, 0x38, "E000F10 ", 8);
    put_text(ip, 0x40, "T-00000   ", 10);
    put_text(ip, 0x4A, "V1.000", 6);
    put_text(ip, 0x50, "20261115        ", 16);
    put_text(ip, 0x60, "1ST_READ.BIN    ", 16);
    put_text(ip, 0x70, "CODEBREAKER TEST", 16);
    put_text(ip, 0x80, "DREAMCAST TEST DISC", 128);
    const Bytes boot = assemble(R"(
        .org 0x0C008300
        mov.l   K_first,r0
        jmp     @r0
        nop
        .align 2
K_first: .long 0xAC010000
)");
    std::copy(boot.begin(), boot.end(), ip.begin() + 0x300);
    return ip;
}

// A larger program (hundreds of functions with delay slots), for the scramble's detection: a program too small to tell from noise is not found scrambled.
inline std::string big_source() {
    std::string s = "        .org 0x0C010000\n";
    for (int i = 0; i < 400; ++i) {
        const std::string n = std::to_string(i);
        s += "f" + n + ":  sts.l pr,@-r15\n        bsr g" + n + "\n        nop\n        lds.l @r15+,pr\n        rts\n        nop\ng" + n +
             ":  mov r4,r0\n        rts\n        add #1,r0\n";
    }
    return s;
}

// A disc of 2048-byte sectors: IP.BIN, then `program` as 1ST_READ.BIN (scrambled when asked), its volume numbered from `volume_base`.
inline Bytes iso(const Bytes& program, bool scrambled = false, std::uint32_t volume_base = 0) {
    Bytes image = ip_bin();
    image.insert(image.end(), program.begin(), program.end());
    return dreamcast_writer::minimal_disc(image, "1ST_READ.BIN", scrambled, volume_base);
}
inline Bytes iso(bool scrambled = false) { return iso(assemble(kProgram), scrambled); }

// The same sectors as a raw .bin: 2352 bytes each, a sync pattern, a header whose mode byte says 2, an XA subheader, the data, and the rest.
inline Bytes raw_sectors(const Bytes& cooked) {
    Bytes out;
    for (std::size_t s = 0; s * 2048 < cooked.size(); ++s) {
        Bytes sector(2352, 0);
        sector[0] = 0;
        for (std::size_t i = 1; i < 11; ++i) sector[i] = 0xFF;
        sector[15] = 2;
        std::copy_n(cooked.begin() + static_cast<std::ptrdiff_t>(s * 2048), 2048, sector.begin() + 24);
        out.insert(out.end(), sector.begin(), sector.end());
    }
    return out;
}

}  // namespace cb::source::dreamcast_test
