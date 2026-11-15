// Tests of the SuperH assembler's directive layer: labels and the two passes, `.org` (an address, not a position in the file), the data
// directives (big-endian; `.word` is 16 bits), strings and their escapes, `.space` `.fill` `.align`, `.equ`, `!` comments, and every way a line
// is refused. The instructions are decompile/sh2's.
#include "cb/build/sh_asm.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/isa_sh2.hpp"

namespace {

using cb::Bytes;
using cb::build::AsmError;
namespace sh_asm = cb::build::sh_asm;

bool same(const Bytes& a, const Bytes& b) { return a == b; }

Bytes build(std::string_view source) { return sh_asm::Assembler(cb::source::isa::sh2_encoder()).build(source); }

}  // namespace

CB_TEST("build.sh_asm.instructions_and_labels", [] {
    // a backward branch, a forward one (found on the second pass), a PC-relative constant after the code, big-endian words
    const Bytes b = build(R"(
        .org 0x06000000
start:  mov.l   K,r1            ! a constant
        add     #-1,r1
        cmp/pl  r1
        bt/s    start
        nop
        bra     out
        nop
out:    rts
        nop
        .align 2
K:      .long   0x12345678
)");
    CB_CHECK(same(b, (Bytes{0xD1, 0x04, 0x71, 0xFF, 0x41, 0x15, 0x8D, 0xFB, 0x00, 0x09, 0xA0, 0x00, 0x00, 0x09, 0x00, 0x0B, 0x00, 0x09, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78})));
});

CB_TEST("build.sh_asm.directives", [] {
    CB_CHECK(same(build(".long 0x01020304, 5\n.word 0x0607, 8\n.byte 9, 10"), (Bytes{1, 2, 3, 4, 0, 0, 0, 5, 6, 7, 0, 8, 9, 10})));
    CB_CHECK(same(build(".short 1\n.2byte 2\n.hword 3\n.int 4\n.4byte 5"), (Bytes{0, 1, 0, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5})));
    CB_CHECK(same(build(".ascii \"ab\"\n.asciz \"c\\n\\\"\\101\"\n.string \"d\""), (Bytes{'a', 'b', 'c', '\n', '"', 'A', 0, 'd', 0})));
    CB_CHECK(same(build(".space 3\n.space 2, 0xAA\n.fill 2, 2, 0x1234\n.skip 1"), (Bytes{0, 0, 0, 0xAA, 0xAA, 0x12, 0x34, 0x12, 0x34, 0})));
    // `.align n` is 2^n bytes on the SH's ELF targets; `.balign n` is n bytes
    CB_CHECK(same(build(".byte 1\n.align 2\n.byte 2"), (Bytes{1, 0, 0, 0, 2})));
    CB_CHECK(same(build(".byte 1\n.balign 2\n.byte 2\n.p2align 1\n.byte 3"), (Bytes{1, 0, 2, 0, 3})));
    // .org says where the next byte is seen, not where it is put
    CB_CHECK(same(build(".org 0x06004000\nlabel: .long label\n.org 0x10\n.long label"), (Bytes{6, 0, 0x40, 0, 6, 0, 0x40, 0})));
    CB_CHECK(same(build("X = 3\n.equ Y, X + 1\n.set Z, Y * 2\n.byte X, Y, Z"), (Bytes{3, 4, 8})));
    CB_CHECK(same(build("  ! only a comment\n\nnop ! trailing\n.ascii \"a!b\" ! the ! inside the string is text"), (Bytes{0x00, 0x09, 'a', '!', 'b'})));
    CB_CHECK(same(build(".section .text\n.globl main\n.type main, @function\nmain: nop\n.size main, 2\n.text\n.data\n.file \"x.c\""), (Bytes{0x00, 0x09})));
});

CB_TEST("build.sh_asm.refusals", [] {
    CB_CHECK_THROWS_AS(AsmError, build("nop\nfrob r1"));
    CB_CHECK_THROWS_AS(AsmError, build(".byte 1\nnop"));                     // an instruction at an odd address
    CB_CHECK_THROWS_AS(AsmError, build(".bogus 1"));
    CB_CHECK_THROWS_AS(AsmError, build("bra nowhere\nnop"));                 // a label that never gets a value
    CB_CHECK_THROWS_AS(AsmError, build(".asciz abc"));                       // a string is in double quotes
    CB_CHECK_THROWS_AS(AsmError, build(".space 100000000"));
    CB_CHECK_THROWS_AS(AsmError, build(".align 99"));
    CB_CHECK_THROWS_AS(AsmError, build(".set only_name"));
    CB_CHECK_THROWS_AS(AsmError, build("nop\nbf 0x06100000"));   // out of the 8-bit field reach
});

CB_TEST("build.sh_asm.strip", [] {
    CB_CHECK_EQ(std::string(sh_asm::strip("mov r1,r2 ! comment")), std::string("mov r1,r2"));
    CB_CHECK_EQ(std::string(sh_asm::strip(".ascii \"a ! b\"  ! c")), std::string(".ascii \"a ! b\""));
    CB_CHECK_EQ(std::string(sh_asm::strip(".ascii \"a\\\" ! b\"")), std::string(".ascii \"a\\\" ! b\""));
    CB_CHECK_EQ(std::string(sh_asm::strip("mov #1,r1   ")), std::string("mov #1,r1"));
});
