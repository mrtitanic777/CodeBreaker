// formats/nro_test_image.hpp -- small Nintendo Switch programs for the tests, made here from assembly source with CodeBreaker's own assembler: a NRO whose code takes the address
// of its data with adrp, calls a function, makes a kernel call and is relocated by two R_AARCH64_RELATIVE entries (one to the code, one into the bss), the same with an asset
// section, the same as a NSO with LZ4-compressed segments, and a tiny AArch64 ELF.
#pragma once

#include <string>

#include "build/a64_asm.hpp"
#include "core/bytes.hpp"
#include "formats/nro.hpp"

namespace cb::formats::nro::test_image {

inline const char* nro_source() {
    return R"(
        .equ    bss, 0x3000
        .equ    bss_end, 0x4000
        b       start                   // 0x00
        .word   mod0                    // 0x04: the offset of MOD0
        .ascii  "HOMEBREW"              // 0x08
        .ascii  "NRO0"                  // 0x10
        .word   0                       // version
        .word   0x3000                  // size
        .word   0                       // flags
        .word   0, 0x1000               // text
        .word   0x1000, 0x1000          // rodata
        .word   0x2000, 0x1000          // data
        .word   0x1000                  // bss size
        .word   0
        .fill   32, 1, 0xAB             // build id
        .word   0, 0                    // dso handle offset, reserved
        .word   0, 0, 0, 0, 0, 0        // api info, dynstr, dynsym
        .org    0x80
start:
        stp     x29, x30, [sp, #-16]!
        mov     x29, sp
        adrp    x8, counter
        add     x8, x8, :lo12:counter
        ldr     x9, [x8]
        add     x9, x9, #1
        str     x9, [x8]
        bl      helper
        adrp    x1, msg
        add     x1, x1, :lo12:msg
        ldp     x29, x30, [sp], #16
        ret
helper:
        sub     sp, sp, #16
        str     w0, [sp, #12]
        ldr     x2, [sp]
        svc     #0x21
        add     sp, sp, #16
        ret
        .balign 16
mod0:
        .ascii  "MOD0"
        .word   dynamic - mod0, bss - mod0, bss_end - mod0, 0, 0, 0
        .org    0x1000
rela:
        .quad   slot, 0x403, helper
        .quad   bssptr, 0x403, 0x3010
msg:
        .asciz  "hello, switch"
        .org    0x2000
dynamic:
        .quad   7, rela, 8, 48, 9, 24, 0, 0
slot:
        .quad   helper
counter:
        .quad   0x10
bssptr:
        .quad   0x3010
        .org    0x2FF8
        .quad   0
)";
}

inline Bytes nro_bytes() { return build::a64_asm::build(nro_source()).data; }

// The same with an asset section: ASET, the icon at 0x38, the control data at 0x40 and the RomFS at 0x48 (8 bytes each).
inline Bytes nro_with_assets() {
    Bytes b = nro_bytes();
    const auto push32 = [&](std::uint32_t v) {
        for (unsigned i = 0; i < 4; ++i) b.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    };
    const auto push64 = [&](std::uint64_t v) {
        push32(static_cast<std::uint32_t>(v));
        push32(static_cast<std::uint32_t>(v >> 32));
    };
    for (const char c : std::string("ASET")) b.push_back(static_cast<std::uint8_t>(c));
    push32(0);
    push64(0x38);
    push64(8);
    push64(0x40);
    push64(8);
    push64(0x48);
    push64(8);
    for (unsigned i = 0; i < 24; ++i) b.push_back(static_cast<std::uint8_t>('a' + i));
    return b;
}

// The image as a NSO: the three segments LZ4-compressed after the 0x100-byte header.
inline Bytes nso_bytes() {
    const Bytes image = nro_bytes();
    Bytes out(0x100, 0);
    const auto put32 = [&](std::size_t at, std::uint32_t v) {
        for (unsigned i = 0; i < 4; ++i) out[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    out[0] = 'N';
    out[1] = 'S';
    out[2] = 'O';
    out[3] = '0';
    put32(0x0C, 7);   // all three compressed
    std::size_t at = 0x100;
    for (unsigned i = 0; i < 3; ++i) {
        const Bytes part(image.begin() + 0x1000 * i, image.begin() + 0x1000 * (i + 1));
        const Bytes packed = lz4_compress_simple(part);
        put32(0x10 + 0x10 * i, static_cast<std::uint32_t>(at));
        put32(0x14 + 0x10 * i, 0x1000 * i);
        put32(0x18 + 0x10 * i, 0x1000);
        put32(0x60 + 4 * i, static_cast<std::uint32_t>(packed.size()));
        out.insert(out.end(), packed.begin(), packed.end());
        at += packed.size();
    }
    put32(0x3C, 0x1000);   // bss
    for (unsigned i = 0; i < 32; ++i) out[0x40 + i] = static_cast<std::uint8_t>(i);
    return out;
}

// An AArch64 ELF: one loadable segment at address 0 holding `mov x0, #7; ret` and a word of data, entry 0.
inline Bytes elf_bytes() {
    Bytes b(64 + 56 + 16, 0);
    const auto put = [&](std::size_t at, std::uint64_t v, unsigned n) {
        for (unsigned i = 0; i < n; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    b[0] = 0x7F;
    b[1] = 'E';
    b[2] = 'L';
    b[3] = 'F';
    b[4] = 2;
    b[5] = 1;
    b[6] = 1;
    put(16, 3, 2);      // ET_DYN
    put(18, 183, 2);    // EM_AARCH64
    put(20, 1, 4);
    put(24, 0, 8);      // entry
    put(32, 64, 8);     // phoff
    put(52, 64, 2);
    put(54, 56, 2);
    put(56, 1, 2);      // one program header
    // PT_LOAD, R+X, file offset 120, address 0, 16 bytes
    put(64, 1, 4);
    put(68, 5, 4);
    put(72, 120, 8);
    put(80, 0, 8);
    put(88, 0, 8);
    put(96, 16, 8);
    put(104, 16, 8);
    put(112, 0x1000, 8);
    put(120, 0xD28000E0, 4);   // mov x0, #7
    put(124, 0xD65F03C0, 4);   // ret
    put(128, 0x12345678, 4);
    return b;
}

}  // namespace cb::formats::nro::test_image
