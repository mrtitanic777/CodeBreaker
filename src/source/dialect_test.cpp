// Tests of the dialect layer (dialect.hpp): the printer says the description's text and the directive layer's lines in a period assembler's
// dialect, the parser reads that dialect back to what the one assembler reads, and the two undo each other.

#include "source/dialect.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/processors.hpp"

namespace cb::source::dialect {
namespace {

CB_TEST("source.dialect.the_neutral_dialect_changes_nothing", [] {
    const Dialect& d = find("sleigh");
    CB_CHECK(d.neutral());
    CB_CHECK_EQ(d.line("        LD HL,0xc000"), "        LD HL,0xc000");
    CB_CHECK_EQ(d.parse("        .org 0x8000\n"), "        .org 0x8000\n");
});

CB_TEST("source.dialect.wla_dx_says_numbers_as_dollar_hex_in_pairs_of_digits", [] {
    const Dialect& d = find("wla-dx");
    CB_CHECK_EQ(d.instruction("LD HL,0xc000"), "LD HL,$C000");
    CB_CHECK_EQ(d.instruction("LD A,0x1"), "LD A,$01");
    CB_CHECK_EQ(d.instruction("IN A,(0x0)"), "IN A,($00)");
    CB_CHECK_EQ(d.instruction("IM 1"), "IM 1");
    CB_CHECK_EQ(d.instruction("LD (IX+0x3),A"), "LD (IX+$03),A");
    CB_CHECK_EQ(d.instruction("LD HL,D_0x20"), "LD HL,D_0x20");   // (a name with 0x in it is a name)
    CB_CHECK_EQ(d.number(0xbe, 2), "$BE");
    CB_CHECK_EQ(d.number(0xfffd, 4), "$FFFD");
});

CB_TEST("source.dialect.wla_dx_says_the_directives_its_own_way", [] {
    const Dialect& d = find("wla-dx");
    CB_CHECK_EQ(d.line(".equ VDP_DATA, 0xbe"), ".DEFINE VDP_DATA         $BE");
    CB_CHECK_EQ(d.line("        .space 50, 0xff"), "        .DSB 50,$FF");
    CB_CHECK_EQ(d.line("        .byte \"HELLO WORLD\""), "        .DB \"HELLO WORLD\"");
    CB_CHECK_EQ(d.line("        .byte 0xce,0xed,\"DEMO\",0x00  ; title"), "        .DB $CE,$ED,\"DEMO\",$00  ; title");
    CB_CHECK_EQ(d.line("reset:"), "reset:");
    const std::vector<std::string> bank = d.block("bank", {{"bank", "1"}, {"slot", "1"}});
    CB_CHECK_EQ(bank.size(), 2u);
    CB_CHECK_EQ(bank[0], ".BANK 1 SLOT 1");
    CB_CHECK_EQ(bank[1], ".ORG $0000");
});

CB_TEST("source.dialect.data_rows_keep_strings_and_runs", [] {
    const Dialect& d = find("wla-dx");
    Bytes data = {0xF3, 'H', 'E', 'L', 'L', 'O', 0x00};
    data.insert(data.end(), 20, 0xFF);
    const std::vector<std::string> lines = d.data_lines(data);
    CB_CHECK_EQ(lines.size(), 2u);
    CB_CHECK_EQ(lines[0], "        .DB $F3,\"HELLO\",$00");
    CB_CHECK_EQ(lines[1], "        .DSB 20,$FF");
});

CB_TEST("source.dialect.wla_dx_reads_back_what_it_wrote_with_banks_in_their_slots", [] {
    const Dialect& d = find("wla-dx");
    std::string source;
    for (const std::string& l : d.block("prologue", {{"pages", "2"}})) source += l + "\n";
    source += d.line(".equ PORT, 0xbf") + "\n";
    for (const std::string& l : d.block("bank", {{"bank", "0"}, {"slot", "0"}})) source += l + "\n";
    source += "start:\n        LD A,$01\n        OUT (PORT),A\n        .DB \"AB\",$00\n        .DSB 3,$FF\n";
    for (const std::string& l : d.block("bank", {{"bank", "1"}, {"slot", "1"}})) source += l + "\n";
    source += "        .DB $01,$02\n";
    const processors::Image image = processors::assemble_described(processors::find("z80"), d.parse(source), {});
    Bytes expected = {0x3E, 0x01, 0xD3, 0xBF, 'A', 'B', 0x00, 0xFF, 0xFF, 0xFF, 0x01, 0x02};
    CB_CHECK(image.data == expected);
    // the second bank is at the second slot ($4000): a label there has that address
    const std::string neutral = d.parse(".MEMORYMAP\nSLOT 0 $0000\nSLOT 1 $4000\n.ENDME\n.BANK 1 SLOT 1\n.ORG $0010\n");
    CB_CHECK(neutral.find(".org 0x4010") != std::string::npos);
});

CB_TEST("source.dialect.an_unknown_dialect_is_a_usage_error", [] {
    CB_CHECK_THROWS_AS(UsageError, find("no-such-dialect"));
});

CB_TEST("source.dialect.gas_mips_says_registers_decimals_and_the_high_half_the_gnu_way", [] {
    const Dialect& d = find("gas-mips");
    CB_CHECK_EQ(d.instruction("addiu sp, sp, -0x18"), "addiu $sp, $sp, -24");
    CB_CHECK_EQ(d.instruction("lui t2, 0xe100"), "lui $t2, 0xE100");
    CB_CHECK_EQ(d.instruction("ori a0, a0, %up(name)"), "ori $a0, $a0, (name >> 16)");
    CB_CHECK_EQ(d.instruction("or v0, a0, zero"), "move $v0, $a0");
    CB_CHECK_EQ(d.parse("        ori $a0, $a0, (name >> 16)"), "        ori a0, a0, %up(name)\n");
    CB_CHECK_EQ(d.parse("        addiu $sp, $sp, -24"), "        addiu sp, sp, -0x18\n");
    CB_CHECK_EQ(d.fallback(".byte 0x80,0x81  ; ext v0, a0, 0x4, 0x8"), ".byte 0x80,0x81  ; ext $v0, $a0, 4, 8");
});

CB_TEST("source.dialect.vasm_says_absolute_long_and_short_branches_the_motorola_way", [] {
    const Dialect& d = find("vasm-m68k");
    CB_CHECK_EQ(d.instruction("jmp 0x200000.l"), "JMP ($00200000).L");
    CB_CHECK_EQ(d.instruction("bra.b L_0010"), "BRA.S L_0010");
    CB_CHECK_EQ(d.instruction("moveq 0x1,D0"), "MOVEQ #1,D0");
    CB_CHECK_EQ(d.parse("        JMP ($00200000).L\n        MOVEQ #1,D0"), "        jmp 0x200000.l\n        moveq 0x1,D0\n");
});

CB_TEST("source.dialect.dasm_has_the_forced_absolute_form_of_a_zero_page_address", [] {
    const Dialect& d = find("dasm");
    CB_CHECK_EQ(d.forced_absolute("sta", "WSYNC"), "STA.w WSYNC");
    CB_CHECK(d.forced_absolute("nop", "x").empty());
    CB_CHECK_EQ(d.parse("        STA.w WSYNC"), "        .byte 141,(WSYNC)&0xff,(WSYNC)>>8\n");
});

}  // namespace
}  // namespace cb::source::dialect
