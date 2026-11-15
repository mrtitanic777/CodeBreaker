// cb-unity: alone
// The directive layer the 8048 and F8 assemblers share, and the 8048 assembler over it.
#include "cb/build/byte_asm.hpp"

#include "cb/build/asm_text.hpp"
#include "cb/build/i8048_asm.hpp"
#include "cb/core/testing.hpp"

namespace cb::build::byte_asm {

CB_TEST("build.byte_asm.directives_labels_and_forward_references", [] {
    const char* source =
        "; a program\n"
        "PORT    EQU $A1\n"
        "        ORG $0400\n"
        "start:\n"
        "        MOV R0,#PORT\n"
        "        MOVX A,@R0\n"
        "        JNZ done\n"
        "        CALL sub\n"
        "done:   RET\n"
        "sub:    MOV A,#<table\n"
        "        RET\n"
        "table:  DB 1, $02, \"AB\"\n"
        "        DW $1234, done\n"
        "        FILL 3,$FF\n";
    const Image image = i8048_asm::build(source);
    CB_CHECK_EQ(image.base, 0x400u);
    CB_CHECK((Bytes(image.data.begin(), image.data.begin() + 9) == Bytes{0xB8, 0xA1, 0x80, 0x96, 0x07, 0x94, 0x08, 0x83, 0x23}));
    CB_CHECK_EQ(image.data[9], 0x0B);    // the low byte of table ($040B)
    CB_CHECK_EQ(image.data[10], 0x83);
    CB_CHECK((Bytes(image.data.begin() + 11, image.data.begin() + 19) == Bytes{0x01, 0x02, 'A', 'B', 0x34, 0x12, 0x07, 0x04}));
    CB_CHECK_EQ(image.data.size(), 22u);
});

CB_TEST("build.byte_asm.errors", [] {
    CB_CHECK_THROWS_AS(AsmError, (void)i8048_asm::build("        JNZ nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)i8048_asm::build("        FROB\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)i8048_asm::build("        ORG $0400\n        ORG $0300\n        DB 1\n"));
});

}  // namespace cb::build::byte_asm
