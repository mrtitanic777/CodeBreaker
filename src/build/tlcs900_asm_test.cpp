// cb-unity: alone
// The TLCS-900/H assembler over the directive layer of build/byte_asm: ORG, DB, FILL, EQU, labels, and the instructions whose size depends on a value (a forward label).
#include "build/tlcs900_asm.hpp"

#include "build/asm_text.hpp"

#include "core/testing.hpp"

namespace cb::build::tlcs900_asm {

CB_TEST("build.tlcs900_asm.directives_labels_and_forward_references", [] {
    const char* source =
        "; a program\n"
        "HW_WATCHDOG EQU 0x006F\n"
        "        ORG 0x200040\n"
        "start:\n"
        "        ld (HW_WATCHDOG),0x4E\n"
        "        ld XWA,table\n"
        "        call sub\n"
        "        jr NZ,start\n"
        "        ld A,(table+1)\n"
        "        ret\n"
        "sub:    ld A,(XWA+2)\n"
        "        ret\n"
        "table:  DB 1, 0x02, \"AB\"\n"
        "        FILL 3,0xFF\n";
    const byte_asm::Image image = build(source);
    CB_CHECK_EQ(image.base, 0x200040u);
    // ld (0x6F),0x4E is the three bytes $08 $6F $4E; ld XWA,table the five $40 and the address; call sub the four $1D and its address (sub is further on: sized in the next round)
    CB_CHECK((Bytes(image.data.begin(), image.data.begin() + 3) == Bytes{0x08, 0x6F, 0x4E}));
    CB_CHECK_EQ(image.data[3], 0x40);
    CB_CHECK_EQ(image.data[8], 0x1D);
    CB_CHECK((Bytes(image.data.begin() + 8, image.data.begin() + 12) == Bytes{0x1D, 0x54, 0x00, 0x20}));   // (sub is at $200054)
    CB_CHECK_EQ(image.data.size(), 3u + 5 + 4 + 2 + 5 + 1 + 3 + 1 + 4 + 3);
    CB_CHECK_EQ(image.data[image.data.size() - 1], 0xFF);
});

CB_TEST("build.tlcs900_asm.errors", [] {
    CB_CHECK_THROWS_AS(AsmError, (void)build("        jr NZ,nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        FROB\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        ORG 0x200400\n        ORG 0x200300\n        DB 1\n"));
    CB_CHECK_THROWS_AS(AsmError, (void)build("        ld A,0x1234\n"));
});

}  // namespace cb::build::tlcs900_asm
