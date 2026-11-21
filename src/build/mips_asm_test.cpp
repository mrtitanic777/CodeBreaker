// Tests of the R3000A assembler's directive layer: labels and the two passes, `.org` (an address, not a position in the file), the
// data directives (little-endian), strings and their escapes, `.space` `.fill` `.align`, `.equ`, the ignored `.set` options and
// section directives, comments, and every way a line is refused. The instructions are decompile/mips's; the layer is tested over
// them through a small encoder of its own.
#include "build/mips_asm.hpp"

#include <format>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "decompile/mips.hpp"

namespace {

using cb::Bytes;
using cb::build::AsmError;
namespace mips = cb::decompile::mips;
namespace mips_asm = cb::build::mips_asm;

class Syntax final : public cb::build::Encoder {
public:
    cb::Result<std::int64_t> value(std::string_view expr, const cb::build::Labels& labels, std::int64_t pc) const override {
        try {
            return mips::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const cb::decompile::z80::UnknownLabel& e) {
            return cb::Failure(cb::build::UndefinedSymbolError(e.name()));
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
    cb::Result<Bytes> assemble(std::string_view line, std::int64_t pc, const cb::build::Labels& labels) const override {
        try {
            return mips::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const cb::decompile::z80::UnknownLabel& e) {
            return cb::Failure(cb::build::UndefinedSymbolError(e.name()));
        } catch (const cb::ParseError& e) {
            return cb::Failure(e.clone());
        }
    }
};

Bytes build(std::string_view source) {
    static const Syntax syntax;
    return mips_asm::Assembler(syntax).build(source);
}

std::string hex(const Bytes& b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

}  // namespace

CB_TEST("build.mips_asm.instructions_are_written_little_endian_in_order", [] {
    CB_CHECK_EQ(hex(build("addiu $sp, $sp, -32\njr $ra\nnop\n")), "e0ffbd27" "0800e003" "00000000");
});

CB_TEST("build.mips_asm.org_sets_the_address_and_not_the_place_in_the_file", [] {
    // the header's word, then the program at its load address: the branch and the label use $80010000, the file is just the bytes
    const std::string source =
        "        .word 0x12345678\n"
        "        .org 0x80010000\n"
        "start:  beq $zero, $zero, start\n"
        "        nop\n"
        "        jal start\n"
        "        nop\n";
    CB_CHECK_EQ(hex(build(source)), "78563412" "ffff0010" "00000000" "0040000c" "00000000");
});

CB_TEST("build.mips_asm.labels_may_be_used_before_they_are_defined", [] {
    const std::string source =
        ".org 0x80010000\n"
        "        j later\n"
        "        nop\n"
        "        la $a0, msg\n"
        "later:  jr $ra\n"
        "        nop\n"
        "msg:    .asciz \"hi\"\n"
        "        .align 2\n"
        "        .word later, msg + 1\n";
    const Bytes image = build(source);
    // j later (later = 0x80010010): index 0x4004
    CB_CHECK_EQ(hex(Bytes(image.begin(), image.begin() + 4)), "04400008");
    // la $a0, msg (msg = 0x80010018): lui $a0, 0x8001; addiu $a0, $a0, 0x18
    CB_CHECK_EQ(hex(Bytes(image.begin() + 8, image.begin() + 16)), "0180043c" "18008424");
    CB_CHECK_EQ(hex(Bytes(image.begin() + 0x18, image.begin() + 0x1B)), "686900");
    CB_CHECK_EQ(image.size(), std::size_t{0x18 + 3 + 1 + 8});
    CB_CHECK_EQ(hex(Bytes(image.end() - 8, image.end())), "10000180" "19000180");
});

CB_TEST("build.mips_asm.data_directives", [] {
    CB_CHECK_EQ(hex(build(".byte 1, 2, 0xFF\n")), "0102ff");
    CB_CHECK_EQ(hex(build(".half 0x1234, -1\n")), "3412ffff");
    CB_CHECK_EQ(hex(build(".word 0xDEADBEEF\n")), "efbeadde");
    CB_CHECK_EQ(hex(build(".4byte 1\n.long 2\n.short 3\n.2byte 4\n")), "01000000" "02000000" "0300" "0400");
    CB_CHECK_EQ(hex(build(".ascii \"AB\"\n.asciz \"C\"\n.asciiz \"D\"\n")), "4142" "4300" "4400");
    CB_CHECK_EQ(hex(build(".ascii \"a\\n\\t\\\\\\\"\\101\\0\"\n")), "610a095c224100");
    CB_CHECK_EQ(hex(build(".ascii \"a, b # not a comment\"\n")), hex(Bytes{'a', ',', ' ', 'b', ' ', '#', ' ', 'n', 'o', 't', ' ', 'a', ' ', 'c', 'o', 'm', 'm', 'e', 'n', 't'}));
    CB_CHECK_EQ(hex(build(".space 3\n.space 2, 0xAA\n")), "000000" "aaaa");
    CB_CHECK_EQ(hex(build(".fill 2, 4, 0x01020304\n.fill 3\n.fill 2, 2, 7\n")), "04030201" "04030201" "000000" "07000700");
    CB_CHECK_EQ(hex(build(".byte 1\n.align 2\n.byte 2\n.p2align 1\n.byte 3\n")), "01000000" "02" "00" "03");
    CB_CHECK_EQ(hex(build(".word 1 + 2 * 3, (1 << 4) | 1, 0x10 - 1\n")), "07000000" "11000000" "0f000000");
});

CB_TEST("build.mips_asm.constants_and_ignored_directives", [] {
    const std::string source =
        "        .set noreorder\n"
        "        .set noat\n"
        "        .section .text\n"
        "        .globl main\n"
        "        .ent main\n"
        "GP1 = 0x1F801814\n"
        "        .equ COUNT, 3\n"
        "        .set LIMIT, COUNT * 2\n"
        "main:   .word GP1, COUNT, LIMIT\n"
        "        .end main\n"
        "        .size main, 12\n";
    CB_CHECK_EQ(hex(build(source)), "1418801f" "03000000" "06000000");
    CB_CHECK_EQ(hex(build("        .set reorder\n.word 1\n")), "01000000");
});

CB_TEST("build.mips_asm.comments_and_label_forms", [] {
    CB_CHECK_EQ(hex(build("# a comment\n.word 1  # another\n\n   \n.word 2\n")), "01000000" "02000000");
    CB_CHECK_EQ(hex(build("a: b: .word a\n")), "00000000");
    CB_CHECK_EQ(hex(build(".org 8\na:\n.word a\n.word b\nb:\n")), "08000000" "10000000");
    CB_CHECK_EQ(hex(build("one:\r\n.word 1\r\ntwo: .word 2\r\n")), "01000000" "02000000");
    CB_CHECK_EQ(std::string(mips_asm::strip("lw $t0, 4($sp)   # x")), "lw $t0, 4($sp)");
    CB_CHECK_EQ(std::string(mips_asm::strip("    .ascii \"#\" # real")), "    .ascii \"#\"");
});

CB_TEST("build.mips_asm.size_guess_gives_the_size_a_forward_reference_will_have", [] {
    static const Syntax syntax;
    const cb::build::Labels none;
    const auto r = mips_asm::size_guess(syntax, "jal elsewhere", 0x80010000, none);
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().size(), std::size_t{4});
    const auto la = mips_asm::size_guess(syntax, "la $a0, elsewhere", 0x80010000, none);
    CB_CHECK(la.ok());
    CB_CHECK_EQ(la.value().size(), std::size_t{8});
});

CB_TEST("build.mips_asm.every_refusal_names_its_line", [] {
    CB_CHECK_THROWS_AS(AsmError, build(".word 1\nfrobnicate $t0\n"));
    CB_CHECK_THROWS_AS(AsmError, build("jal nowhere\n"));                       // an undefined label on the last pass
    CB_CHECK_THROWS_AS(AsmError, build(".byte 1\nnop\n"));                      // an instruction not at a multiple of four
    CB_CHECK_THROWS_AS(AsmError, build(".bogus 1\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".ascii hello\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".ascii \"\\q\"\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".space -1\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".space 99999999999\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".align 40\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".set\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".equ A\n"));
    CB_CHECK_THROWS_AS(AsmError, build(".fill 1, 9, 0\n"));
    try {
        build(".word 1\n.word 2\nlw $t0, 4\n");
        CB_CHECK(false);
    } catch (const AsmError& e) {
        CB_CHECK_EQ(e.line(), 3u);   // the line that failed
    }
    // an instruction whose size depends on a label defined later settles in another pass: lui $t0,0x1234 and ori $t0,$t0,0x5678
    CB_CHECK(build("li $t0, far\n.equ far, 0x12345678\n") == (Bytes{0x34, 0x12, 0x08, 0x3C, 0x78, 0x56, 0x08, 0x35}));
    // one that can never settle is still refused: one word puts `after` where the value needs two, two words where it needs one
    CB_CHECK_THROWS_AS(AsmError, build("li $t0, 0x10005-after\nafter:\n"));
});
