// cb-unity: alone
// The 6809 assembler's directives and labels (the instructions are decompile/m6809's).
#include "build/m6809_asm.hpp"

#include "build/asm_text.hpp"
#include "core/testing.hpp"

namespace cb::build::m6809_asm {

CB_TEST("build.m6809_asm.directives_labels_and_forward_references", [] {
    const char* source =
        "; a program\n"
        "VIA_port_b      EQU $D000\n"
        "        ORG $0010\n"
        "start\n"
        "        LDA #$12\n"
        "        STA VIA_port_b\n"
        "        BNE done\n"
        "        JSR sub\n"
        "done:\n"
        "        RTS\n"
        "sub     LDX #table\n"
        "        RTS\n"
        "table   FDB $1234, done\n"
        "        FCB 1, $02, \"A\"\n"
        "        FCC \"hi\"\n"
        "        FILL 3,$FF\n"
        "        RMB 2\n";
    const Bytes image = build(source);
    CB_CHECK_EQ(image.size(), 0x2Du);   // from address 0 to the last byte
    const std::size_t at = 0x10;
    CB_CHECK((Bytes(image.begin() + at, image.begin() + at + 10) == Bytes{0x86, 0x12, 0xB7, 0xD0, 0x00, 0x26, 0x03, 0xBD, 0x00, 0x1B}));
    CB_CHECK_EQ(image[0x1A], 0x39);   // done: RTS
    CB_CHECK((Bytes(image.begin() + 0x1B, image.begin() + 0x1E) == Bytes{0x8E, 0x00, 0x1F}));
    CB_CHECK((Bytes(image.begin() + 0x1F, image.begin() + 0x23) == Bytes{0x12, 0x34, 0x00, 0x1A}));
});

CB_TEST("build.m6809_asm.strings_fills_and_errors", [] {
    const Bytes image = build("        FCB \"AB\",0\n        FCC /x y/\n        FILL 4,$AA\n        FDB 1\n");
    CB_CHECK((image == Bytes{'A', 'B', 0, 'x', ' ', 'y', 0xAA, 0xAA, 0xAA, 0xAA, 0, 1}));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        LDA #$12\n        BRA nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        FROB\n"));
});

}  // namespace cb::build::m6809_asm
