// cb-unity: alone
// The V810 assembler's directives and labels (the instructions are decompile/v810's).
#include "build/v810_asm.hpp"

#include "build/asm_text.hpp"
#include "core/testing.hpp"

namespace cb::build::v810_asm {

CB_TEST("build.v810_asm.directives_labels_and_forward_references", [] {
    const char* source =
        "; a program\n"
        ".equ VIP_INTPND, 0x0005F800\n"
        "        .org 0x07000010\n"
        "start:\n"
        "        movhi hi(VIP_INTPND),r0,r6\n"
        "        ld.h lo(VIP_INTPND)[r6],r7\n"
        "        bne done\n"
        "        jal sub\n"
        "done:\n"
        "        jmp [lp]\n"
        "sub:    movhi hi(table),r0,r8\n"
        "        jmp [lp]\n"
        "table:  .word 0x12345678, done\n"
        "        .hword 0xBEEF, table\n"
        "        .byte 1, 0x02, \"AB\", $FF\n"
        "        .ascii \"hi\"\n"
        "        .fill 3,$FF\n";
    const Image image = build(source);
    CB_CHECK_EQ(image.base, 0x07000010u);
    CB_CHECK_EQ(image.data.size(), 44u);
    CB_CHECK((Bytes(image.data.begin(), image.data.begin() + 4) == Bytes{0xC0, 0xBC, 0x06, 0x00}));   // movhi 6,r0,r6
    CB_CHECK((Bytes(image.data.begin() + 4, image.data.begin() + 8) == Bytes{0xE6, 0xC4, 0x00, 0xF8}));   // ld.h 0xF800[r6],r7
});

CB_TEST("build.v810_asm.strings_fills_and_errors", [] {
    const Image image = build("        .org 0x100\n        .byte \"AB\",0\n        .ascii \"x, y\"\n        .fill 4,$AA\n        .word 1\n        .hword 2\n");
    CB_CHECK_EQ(image.base, 0x100u);
    CB_CHECK((image.data == Bytes{'A', 'B', 0, 'x', ',', ' ', 'y', 0xAA, 0xAA, 0xAA, 0xAA, 1, 0, 0, 0, 2, 0}));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        mov 1,r2\n        br nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        frob\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        .frob 1\n"));
});

}  // namespace cb::build::v810_asm
