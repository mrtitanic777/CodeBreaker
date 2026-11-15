// cb/source/n64_test_rom.hpp -- a small Nintendo 64 ROM the tests of the lane share: a header, a stand-in for the boot code, and a program that
// touches what the writer and the interpreter care about (hardware registers, branch likely, doubleword and FPU instructions, a call, a function
// found only through a table, an exception handler ending in ERET, strings and tables). Built with CodeBreaker's own big-endian assembler.
#pragma once

#include <format>
#include <string>

#include "cb/build/mips_asm.hpp"
#include "cb/core/bytes.hpp"
#include "cb/formats/n64.hpp"
#include "cb/source/isa_r4300.hpp"

namespace cb::source::n64_test {

// The program: at $80001000, `entry` calls `print`, polls PI_STATUS, does a little FPU arithmetic, returns; `helper` and `handler` are reached
// only through the table; the data is a string, a float, the table, a word that is no instruction, a run of zeros.
inline std::string program() {
    return R"(
        .org 0x80001000
entry:  daddiu $sp, $sp, -32
        sd    $ra, 24($sp)
        lui   $a0, %hi(msg)
        jal   print
        daddiu $a0, $a0, %lo(msg)
        lui   $t0, 0xA430
        lw    $t1, 0x0C($t0)
        lui   $t2, 0xA440
        sw    $t1, 4($t2)
        lui   $t3, 0xA460
poll:   lw    $t4, 0x10($t3)
        andi  $t4, $t4, 3
        bnel  $t4, $zero, poll
        nop
        lui   $t5, %hi(one)
        lwc1  $f0, %lo(one)($t5)
        add.s $f2, $f0, $f0
        c.eq.s $f0, $f2
        bc1fl done
        mtc1  $zero, $f4
        nop
done:   ld    $ra, 24($sp)
        daddiu $sp, $sp, 32
        jr    $ra
        nop
print:  jr    $ra
        move  $v0, $a0
helper: daddiu $sp, $sp, -16
        dsll32 $v0, $a0, 4
        jr    $ra
        daddiu $sp, $sp, 16
handler: mfc0 $k0, $13
        eret
msg:    .asciz "HELLO, NINTENDO 64"
        .align 2
one:    .word 0x3F800000
table:  .word entry, print, helper, handler
        .word 0xFFFFFFFF
        .space 32
        .word 0x12345678
)";
}

// The boot code's stand-in: 1008 words of fixed pseudo-random values, with a text in the middle.
inline std::string boot_code() {
    std::string out;
    std::uint32_t x = 0x1234567;
    for (unsigned i = 0; i < 1008; ++i) {
        x = x * 1664525u + 1013904223u;
        if (i == 500) out += "        .asciz \"IPL3 STAND-IN FOR THE TESTS\"\n        .align 2\n";
        else if (i > 500 && i < 507) continue;   // (the text is seven words: 500 to 506)
        else out += std::format("        .word 0x{:08X}\n", x);
    }
    return out;
}

// The ROM: header, boot code, program, then `extra` bytes of cartridge data after the first megabyte (0: none). `order` is the byte order of the
// file.
inline Bytes test_rom(formats::n64::ByteOrder order = formats::n64::ByteOrder::Z64, std::size_t extra = 0, std::string_view changed_program = {}) {
    std::string source = R"(
        .word 0x80371240
        .word 0x0000000F
        .word 0x80001000
        .word 0x00001444
        .word 0x11223344, 0x55667788
        .space 8
        .ascii "N64 LANE TEST       "
        .space 7
        .byte 0x4E
        .ascii "NT"
        .byte 0x45, 0x00
)" + boot_code() +
                         std::string(changed_program.empty() ? program() : std::string(changed_program));
    if (extra != 0) source += std::format("\n        .space 0x80101000 - .\n        .space {}\n        .word 0xCAFEBABE\n", extra);
    Bytes rom = build::mips_asm::Assembler(isa::r4300_encoder(), build::mips_asm::ByteOrder::Big).build(source);
    return formats::n64::Rom::convert(rom, formats::n64::ByteOrder::Z64, order);
}

}  // namespace cb::source::n64_test
