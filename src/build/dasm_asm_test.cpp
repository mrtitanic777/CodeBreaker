// Tests of the dasm-syntax assembler over the real 6502 encoder: expected bytes worked out by hand from the opcode map.
#include "build/dasm_asm.hpp"

#include "core/testing.hpp"
#include "source/isa.hpp"
#include "source/isa_65xx.hpp"

namespace {

using namespace cb;
using build::dasm_asm::Assembler;

Bytes assemble(std::string_view source) { return Assembler(source::isa::m6502_encoder()).build(source); }

}  // namespace

CB_TEST("build.dasm.instructions_labels_and_data", [] {
    const Bytes image = assemble(
        "        processor 6502\n"
        "WSYNC   = $02\n"
        "        ORG $0000\n"
        "        RORG $F000\n"
        "Start\n"
        "        SEI\n"
        "        LDA #$00\n"
        "        STA WSYNC\n"       // zero page: 85 02
        "        STA.w WSYNC\n"     // forced absolute: 8D 02 00
        "loop    DEX\n"             // a label with an instruction on its line
        "        BNE loop\n"        // D0 FD
        "        JMP Start\n"       // 4C 00 F0
        "        .byte $12, \"AB\"\n"
        "        .word Start\n"
        "        ds.b 3,$FF\n"
        "        hex 0102\n");
    const Bytes expected = {0x78, 0xA9, 0x00, 0x85, 0x02, 0x8D, 0x02, 0x00, 0xCA, 0xD0, 0xFD, 0x4C, 0x00, 0xF0,
                            0x12, 'A',  'B',  0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x01, 0x02};
    CB_CHECK(image == expected);
});

CB_TEST("build.dasm.org_gap_and_forward_reference", [] {
    const Bytes image = assemble(
        "        ORG $1000\n"
        "        RORG $F000\n"
        "        JSR later\n"       // forward: 20 05 F0
        "        RTS\n"             // 60
        "        ORG $1005\n"       // the gap before it is zero
        "        RORG $F005\n"
        "later   RTS\n");
    const Bytes expected = {0x20, 0x05, 0xF0, 0x60, 0x00, 0x60};
    CB_CHECK(image == expected);
});

CB_TEST("build.dasm.errors", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)assemble("        ORG $F000\n        LDA nowhere\n"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)assemble("        NOP\n"));   // code before ORG
    CB_CHECK_THROWS_AS(cb::ParseError, (void)assemble("        ORG $F000\n        FOO\n"));
});

CB_TEST("build.dasm.cmos_instructions", [] {
    const Bytes image = Assembler(source::isa::m65sc02_encoder())
                            .build("        ORG $0200\n        BRA skip\n        STZ $20\n        INC A\nskip    PHX\n");
    const Bytes expected = {0x80, 0x03, 0x64, 0x20, 0x1A, 0xDA};
    CB_CHECK(image == expected);
});
