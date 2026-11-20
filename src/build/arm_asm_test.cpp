// Tests of the ARM7TDMI assembler's directive layer: labels and the two passes, `.org` (an address, not a position in the file), the
// data directives (little-endian), strings and their escapes, `.space` `.fill` `.align`, `.equ`, the ignored section directives, the
// two instruction states (`.arm`, `.thumb`), comments, and every way a line is refused. The instructions are decompile/arm's; the layer
// is tested over them through a small encoder of its own.
#include "build/arm_asm.hpp"

#include <format>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "decompile/arm.hpp"

namespace {

using cb::Bytes;
using cb::build::AsmError;
namespace arm = cb::decompile::arm;
namespace arm_asm = cb::build::arm_asm;

class Syntax final : public cb::build::Encoder {
public:
    cb::Result<std::int64_t> value(std::string_view expr, const cb::build::Labels& labels, std::int64_t pc) const override {
        try {
            return arm::value(expr, labels, static_cast<std::uint32_t>(pc) & ~1u);
        } catch (const cb::decompile::z80::UnknownLabel& e) {
            return cb::Failure(cb::build::UndefinedSymbolError(e.name()));
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
    cb::Result<Bytes> assemble(std::string_view line, std::int64_t pc, const cb::build::Labels& labels) const override {
        try {
            return arm::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const cb::decompile::z80::UnknownLabel& e) {
            return cb::Failure(cb::build::UndefinedSymbolError(e.name()));
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
};

Bytes build(std::string_view source) {
    static const Syntax syntax;
    return arm_asm::Assembler(syntax).build(source);
}

std::string hex(const Bytes& b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

}  // namespace

CB_TEST("build.arm_asm.instructions_are_written_little_endian_in_order", [] {
    CB_CHECK_EQ(hex(build("push {r4, lr}\nmov r0, #0\nbx lr\n")), "10402de9" "0000a0e3" "1eff2fe1");
});

CB_TEST("build.arm_asm.thumb_state_and_arm_state", [] {
    const std::string source =
        ".org 0x08000000\n"
        "        .thumb\n"
        "start:  movs r0, #1\n"
        "        bl far\n"
        "        bx lr\n"
        "        .align 2\n"
        "        .arm\n"
        "far:    bx lr\n"
        "        .code 16\n"
        "        nop\n";
    // movs r0,#1 = 2001; bl far (far = 0x08000008: 0x08000002 + 4 -> offset 2): F000 F801; bx lr = 4770; align; ARM bx lr; nop
    CB_CHECK_EQ(hex(build(source)), "0120" "00f0" "01f8" "7047" "1eff2fe1" "c046");
});

CB_TEST("build.arm_asm.org_sets_the_address_and_not_the_place_in_the_file", [] {
    const std::string source =
        "        .word 0x12345678\n"
        "        .org 0x08000000\n"
        "start:  b start\n"
        "        bl start\n";
    CB_CHECK_EQ(hex(build(source)), "78563412" "feffffea" "fdffffeb");
});

CB_TEST("build.arm_asm.labels_may_be_used_before_they_are_defined", [] {
    const std::string source =
        ".org 0x08000000\n"
        "        b later\n"
        "        ldr r0, msg\n"
        "later:  bx lr\n"
        "msg:    .asciz \"hi\"\n"
        "        .align 2\n"
        "        .word later, msg + 1\n";
    const Bytes image = build(source);
    CB_CHECK_EQ(hex(Bytes(image.begin(), image.begin() + 4)), "000000ea");   // b later (+8, offset 0)
    CB_CHECK_EQ(hex(Bytes(image.begin() + 4, image.begin() + 8)), "00009fe5");   // ldr r0, msg: msg = pc + 8, offset 0
    CB_CHECK_EQ(hex(Bytes(image.begin() + 0x0C, image.begin() + 0x0F)), "686900");
    CB_CHECK_EQ(image.size(), std::size_t{0x10 + 8});
    CB_CHECK_EQ(hex(Bytes(image.end() - 8, image.end())), "08000008" "0d000008");
});

CB_TEST("build.arm_asm.data_directives", [] {
    CB_CHECK_EQ(hex(build(".byte 1, 2, 0xFF\n")), "0102ff");
    CB_CHECK_EQ(hex(build(".half 0x1234, -1\n")), "3412ffff");
    CB_CHECK_EQ(hex(build(".word 0xDEADBEEF\n")), "efbeadde");
    CB_CHECK_EQ(hex(build(".4byte 1\n.long 2\n.int 5\n.short 3\n.2byte 4\n.hword 6\n")), "01000000" "02000000" "05000000" "0300" "0400" "0600");
    CB_CHECK_EQ(hex(build(".ascii \"AB\"\n.asciz \"C\"\n.asciiz \"D\"\n")), "4142" "4300" "4400");
    CB_CHECK_EQ(hex(build(".ascii \"a\\n\\t\\\\\\\"\\101\\0\"\n")), "610a095c224100");
    CB_CHECK_EQ(hex(build(".ascii \"a, b @ not a comment\"\n")), hex(Bytes{'a', ',', ' ', 'b', ' ', '@', ' ', 'n', 'o', 't', ' ', 'a', ' ', 'c', 'o', 'm', 'm', 'e', 'n', 't'}));
    CB_CHECK_EQ(hex(build(".space 3\n.space 2, 0xAA\n")), "000000" "aaaa");
    CB_CHECK_EQ(hex(build(".fill 2, 4, 0x01020304\n.fill 3\n.fill 2, 2, 7\n")), "04030201" "04030201" "000000" "07000700");
    CB_CHECK_EQ(hex(build(".byte 1\n.align 2\n.byte 2\n.p2align 1\n.byte 3\n.balign 8\n.byte 4\n")), "01000000" "02" "00" "03" "00" "04");
    CB_CHECK_EQ(hex(build(".word 1 + 2 * 3, (1 << 4) | 1, 0x10 - 1\n")), "07000000" "11000000" "0f000000");
});

CB_TEST("build.arm_asm.constants_and_ignored_directives", [] {
    const std::string source =
        "        .syntax unified\n"
        "        .cpu arm7tdmi\n"
        "        .section .text\n"
        "        .globl main\n"
        "        .type main, %function\n"
        "IO = 0x04000000\n"
        "        .equ COUNT, 3\n"
        "        .set LIMIT, COUNT * 2\n"
        "main:   .word IO, COUNT, LIMIT\n"
        "        .size main, 12\n";
    CB_CHECK_EQ(hex(build(source)), "00000004" "03000000" "06000000");
    CB_CHECK_EQ(hex(build("IO = 0x04000000\n        mov r0, #IO >> 24\n")), "0400a0e3");
});

CB_TEST("build.arm_asm.comments_and_label_forms", [] {
    CB_CHECK_EQ(hex(build("# a comment\n.word 1  @ another\n\n   \n.word 2\n  # also\n")), "01000000" "02000000");
    CB_CHECK_EQ(hex(build("mov r0, #1 @ immediates keep their #\n")), "0100a0e3");
    CB_CHECK_EQ(hex(build("a: b: .word a\n")), "00000000");
    CB_CHECK_EQ(hex(build(".org 8\na:\n.word a\n.word b\nb:\n")), "08000000" "10000000");
    CB_CHECK_EQ(hex(build("one:\r\n.word 1\r\ntwo: .word 2\r\n")), "01000000" "02000000");
    CB_CHECK_EQ(std::string(arm_asm::strip("ldr r0, [r1, #4]   @ x")), "ldr r0, [r1, #4]");
    CB_CHECK_EQ(std::string(arm_asm::strip("    .ascii \"@\" @ real")), "    .ascii \"@\"");
    CB_CHECK_EQ(std::string(arm_asm::strip("    # whole line")), "");
});

CB_TEST("build.arm_asm.size_guess_gives_the_size_a_forward_reference_will_have", [] {
    static const Syntax syntax;
    const cb::build::Labels none;
    const auto arm_branch = arm_asm::size_guess(syntax, "bl elsewhere", 0x08000000, none);
    CB_CHECK(arm_branch.ok());
    CB_CHECK_EQ(arm_branch.value().size(), std::size_t{4});
    const auto thumb_branch = arm_asm::size_guess(syntax, "bl elsewhere", 0x08000001, none);
    CB_CHECK(thumb_branch.ok());
    CB_CHECK_EQ(thumb_branch.value().size(), std::size_t{4});
    const auto thumb_load = arm_asm::size_guess(syntax, "ldr r0, elsewhere", 0x08000003, none);   // (a Thumb load at a halfword that is not a word)
    CB_CHECK(thumb_load.ok());
    CB_CHECK_EQ(thumb_load.value().size(), std::size_t{2});
});

CB_TEST("build.arm_asm.forward_references_in_thumb_code_settle", [] {
    // a load of a word that comes later, from an instruction at a halfword that is not a word
    const std::string source =
        ".org 0x08000000\n"
        "        .thumb\n"
        "        nop\n"
        "        ldr r0, pool\n"
        "        bx lr\n"
        "        nop\n"
        "pool:   .word 0x04000000\n";
    // ldr at 0x2: base = (0x2 + 4) & ~3 = 0x4; pool = 0x8: offset 4 -> imm8 = 1
    CB_CHECK_EQ(hex(build(source)), "c046" "0148" "7047" "c046" "00000004");
});

CB_TEST("build.arm_asm.every_refusal_names_its_line", [] {
    CB_CHECK_THROWS_AS(AsmError, build(".word 1\nfrobnicate r0\n"));
    CB_CHECK_THROWS_AS(AsmError, build("b nowhere\n"));                         // an undefined label on the last pass
    CB_CHECK_THROWS_AS(AsmError, build(".byte 1\nnop\n"));                      // an ARM instruction not at a multiple of four
    CB_CHECK_THROWS_AS(AsmError, build(".thumb\n.byte 1\nnop\n"));              // a Thumb instruction not at a multiple of two
    CB_CHECK_THROWS_AS(AsmError, build(".bogus 1\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".code 8\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".ascii hello\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".ascii \"\\q\"\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".space -1\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".space 99999999999\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".align 40\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".balign 0\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".set\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".equ A\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".fill 1, 9, 0\n"));
    CB_CHECK_THROWS_AS(AsmError, build("ldr r0, =0x12345678\n"));               // literal pools are not assembled
    try {
        build(".word 1\n.word 2\nldr r0, [r1, #8192]\n");
        CB_CHECK(false);
    } catch (const AsmError& e) {
        CB_CHECK_EQ(e.line(), 3u);   // the line that failed
    }
});

namespace {

class Syntax5 final : public cb::build::Encoder {
public:
    cb::Result<std::int64_t> value(std::string_view expr, const cb::build::Labels& labels, std::int64_t pc) const override {
        try {
            return arm::value(expr, labels, static_cast<std::uint32_t>(pc) & ~1u);
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
    cb::Result<Bytes> assemble(std::string_view line, std::int64_t pc, const cb::build::Labels& labels) const override {
        try {
            return arm::assemble(line, static_cast<std::uint32_t>(pc), labels, arm::Arch::V5TE);
        } catch (const cb::decompile::z80::UnknownLabel& e) {
            return cb::Failure(cb::build::UndefinedSymbolError(e.name()));
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
};

Bytes build_dual(std::string_view source) {
    static const Syntax v4;
    static const Syntax5 v5;
    return arm_asm::Assembler(v4, &v5).build(source);
}

}  // namespace

CB_TEST("build.arm_asm.cpu_picks_the_instruction_set", [] {
    // `.cpu arm946e-s` (or `.arch armv5te`) selects the ARMv5TE encoder, `.cpu arm7tdmi` the ARMv4T one again; the assembler holds no state between builds
    CB_CHECK_EQ(hex(build_dual(".cpu arm946e-s\nclz r0, r1\n")), "110f6fe1");
    CB_CHECK_EQ(hex(build_dual(".arch armv5te\nblx r3\n")), "33ff2fe1");
    CB_CHECK_THROWS_AS(AsmError, build_dual("clz r0, r1\n"));                                // (no `.cpu`: the ARM7TDMI's)
    CB_CHECK_THROWS_AS(AsmError, build_dual(".cpu arm946e-s\n.cpu arm7tdmi\nclz r0, r1\n"));
    CB_CHECK_THROWS_AS(AsmError, build("clz r0, r1\n"));                                     // (an assembler given one encoder never has the ARMv5TE set)
    CB_CHECK_EQ(hex(build_dual(".cpu arm7tdmi\nbx lr\n")), "1eff2fe1");
    CB_CHECK_EQ(hex(build_dual(".cpu arm946e-s\n.thumb\nblx r3\n")), "9847");
});
