// Tests of the PowerPC assembler. Every expected word below was worked out by hand from the PowerPC instruction
// formats (opcode<<26, RT/RS<<21, RA<<16, RB<<11, XO<<1) and agrees with what Capstone's PowerPC decoder prints for
// it (the encodings tests/test_xex_edit.py checked).
#include "cb/build/ppc_asm.hpp"

#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

std::uint32_t one(std::string_view line, std::uint32_t address = 0, const Labels& labels = {}) {
    const auto w = ppc_asm::assemble_one(line, address, labels);
    return w ? *w : 0xDEADBEEF;
}

std::uint32_t word_at(const Bytes& b, std::size_t i) {
    return (std::uint32_t{b[i]} << 24) | (std::uint32_t{b[i + 1]} << 16) | (std::uint32_t{b[i + 2]} << 8) | b[i + 3];
}

}  // namespace

CB_TEST("build.ppc_asm.encodings", [] {
    CB_CHECK_EQ(one("nop"), 0x60000000u);
    CB_CHECK_EQ(one("blr"), 0x4E800020u);
    CB_CHECK_EQ(one("bctr"), 0x4E800420u);
    CB_CHECK_EQ(one("li r3, 1"), 0x38600001u);
    CB_CHECK_EQ(one("lis r4, 0x8200"), 0x3C808200u);
    CB_CHECK_EQ(one("mr r31, r3"), 0x7C7F1B78u);
    CB_CHECK_EQ(one("mflr r12"), 0x7D8802A6u);
    CB_CHECK_EQ(one("mtlr r0"), 0x7C0803A6u);
    CB_CHECK_EQ(one("mtctr r11"), 0x7D6903A6u);
    CB_CHECK_EQ(one("mtspr ctr, r11"), 0x7D6903A6u);
    CB_CHECK_EQ(one("mfspr r12, lr"), 0x7D8802A6u);
    CB_CHECK_EQ(one("addi r1, r1, -0x60"), 0x3821FFA0u);
    CB_CHECK_EQ(one("subi r1, r1, 0x60"), 0x3821FFA0u);
    CB_CHECK_EQ(one("ori r3, r3, 0x1234"), 0x60631234u);
    CB_CHECK_EQ(one("lwz r3, 0x10(r31)"), 0x807F0010u);
    CB_CHECK_EQ(one("stw r0, -8(r1)"), 0x9001FFF8u);
    CB_CHECK_EQ(one("stwu r1, -0x60(r1)"), 0x9421FFA0u);
    CB_CHECK_EQ(one("lbz r5, 0(r4)"), 0x88A40000u);
    CB_CHECK_EQ(one("add r3, r4, r5"), 0x7C642A14u);
    CB_CHECK_EQ(one("cmpwi r3, 0"), 0x2C030000u);
    CB_CHECK_EQ(one("cmplw r3, r4"), 0x7C032040u);
    CB_CHECK_EQ(one("neg r3, r4"), 0x7C6400D0u);
    CB_CHECK_EQ(one("extsh r3, r4"), 0x7C830734u);
    CB_CHECK_EQ(one("LI R3,1"), 0x38600001u);   // case does not matter; commas and spaces both separate
    CB_CHECK_EQ(one("li r3 1 # trailing comment"), 0x38600001u);
    CB_CHECK_EQ(one("trap"), 0x7FE00008u);
});

CB_TEST("build.ppc_asm.destination_first_forms", [] {
    // DIVERGES: the port encoded `or r3,r4,r5` as `or r4,r3,r5` (destination and source swapped), unseen while the
    // two registers were equal, and `sub rD,rA,rB` as `subf rD,rA,rB` (rB-rA). In assembler syntax `or rA,rS,rB`
    // names the destination first but encodes RS (the second operand) in the high register field; `sub rD,rA,rB`
    // is rA-rB, that is `subf rD,rB,rA`.
    CB_CHECK_EQ(one("or r3, r4, r5"), 0x7C832B78u);
    CB_CHECK_EQ(one("ori r3, r4, 5"), 0x60830005u);
    CB_CHECK_EQ(one("and. r3, r3, r4"), 0x7C632039u);
    CB_CHECK_EQ(one("not r3, r4"), 0x7C8320F8u);
    CB_CHECK_EQ(one("sub r3, r4, r5"), 0x7C652050u);
    CB_CHECK_EQ(one("subf r3, r4, r5"), 0x7C642850u);   // subf keeps its own order
});

CB_TEST("build.ppc_asm.branches", [] {
    CB_CHECK_EQ(one("b 0x82001000", 0x82000000), 0x48001000u);
    CB_CHECK_EQ(one("bl 0x82000000", 0x82000004), 0x4BFFFFFDu);
    CB_CHECK_EQ(one("beq 0x82000010", 0x82000008), 0x41820008u);
    CB_CHECK_EQ(one("bne 0x81FFFFF0", 0x8200000C), 0x4082FFE4u);
    CB_CHECK_EQ(one("bdnz 0x20", 0x30), 0x4200FFF0u);
    CB_CHECK_EQ(one("bc 12, 2, 0x10", 0), 0x41820010u);
    CB_CHECK_EQ(one("ba 0x100"), 0x48000102u);
    const Labels labels{{"top", 0x82000000}};
    CB_CHECK_EQ(one("b top", 0x82000004, labels), 0x4BFFFFFCu);
    // out of reach: 26 bits of displacement reach +-32 MB, 16 bits +-32 KB
    CB_CHECK_THROWS_AS(AsmError, (void)one("b 0x10000000", 0));
    CB_CHECK_THROWS_AS(AsmError, (void)one("beq 0x9000", 0));
    CB_CHECK_THROWS_AS(AsmError, (void)one("ba 0x82000000"));   // not a sign-extended 26-bit address
    CB_CHECK_THROWS_AS(AsmError, (void)one("b 0x2", 0));        // not 4-aligned
    CB_CHECK_THROWS_AS(AsmError, (void)one("b nowhere", 0));
});

CB_TEST("build.ppc_asm.multi_line_with_labels", [] {
    const Bytes code = ppc_asm::assemble(
        "start:\n"
        "    li r3, 1        # comment\n"
        "    b done          ; a forward label\n"
        "    nop\n"
        "done:\n"
        "    blr\n"
        "    b start\n",
        0x82000000);
    CB_CHECK_EQ(code.size(), 20u);
    CB_CHECK_EQ(word_at(code, 0), 0x38600001u);
    CB_CHECK_EQ(word_at(code, 4), 0x48000008u);   // done is two instructions on
    CB_CHECK_EQ(word_at(code, 8), 0x60000000u);
    CB_CHECK_EQ(word_at(code, 12), 0x4E800020u);
    CB_CHECK_EQ(word_at(code, 16), 0x4BFFFFF0u);   // b start: 16 bytes back
    CB_CHECK(ppc_asm::assemble("").empty());
    CB_CHECK(ppc_asm::assemble("; nothing\n\n# here\n").empty());
    CB_CHECK(!ppc_asm::assemble_one("start:", 0).has_value());   // a label alone is not an instruction
});

CB_TEST("build.ppc_asm.errors", [] {
    auto line_of = [](std::string_view text) {
        try {
            (void)ppc_asm::assemble(text);
        } catch (const AsmError& e) {
            return e.line();
        }
        return 0u;
    };
    CB_CHECK_EQ(line_of("nop\nfly r3\n"), 2u);               // unsupported mnemonic
    CB_CHECK_EQ(line_of("nop\n,,\n"), 2u);                   // nothing but separators (found by the old harness: it crashed)
    CB_CHECK_EQ(line_of("nop\nnop\nli r3\n"), 3u);           // missing operand
    CB_CHECK_EQ(line_of("li r32, 1\n"), 1u);                 // no such register
    CB_CHECK_EQ(line_of("li rx, 1\n"), 1u);
    CB_CHECK_EQ(line_of("li r3, 0x10000\n"), 1u);            // does not fit 16 bits
    CB_CHECK_EQ(line_of("li r3, banana\n"), 1u);
    CB_CHECK_EQ(line_of("ori r3, r3, -1\n"), 1u);            // an unsigned field takes no negative
    CB_CHECK_EQ(line_of("ori r3, r3, 0x12345\n"), 1u);       // DIVERGES: it was silently cut to 16 bits
    CB_CHECK_EQ(line_of("lwz r3, r4\n"), 1u);                // not D(rA)
    CB_CHECK_EQ(line_of("lwz r3, 8(r40)\n"), 1u);            // DIVERGES: r40 was taken as r8
    CB_CHECK_EQ(line_of("lwz r3, 0x10000(r1)\n"), 1u);       // DIVERGES: the displacement was cut to 16 bits
    CB_CHECK_EQ(line_of("lwz r3, ff(r1)\n"), 1u);            // bare hex is not a number here
    CB_CHECK_EQ(line_of("mtspr 1024, r3\n"), 1u);            // SPR numbers are 10 bits
});
