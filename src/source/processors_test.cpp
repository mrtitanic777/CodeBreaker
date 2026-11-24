// Tests of the processor table (source/processors.hpp): every processor that assembles turns a small program into its bytes, and
// the one disassembler reads those bytes back as the same instructions.

#include "source/processors.hpp"

#include <format>
#include <set>

#include "core/error.hpp"

#include "core/testing.hpp"

namespace cb::source::processors {
namespace {

struct Case {
    std::string_view cpu, source;
    std::vector<std::string_view> texts;   // the first instructions as disassemble reads them back
};

// (x86 is left to the build tests: the table reads it through Capstone, which a test run may not have.)
// The sources are in the descriptions' syntax: the one assembler reads what the one disassembler writes.
const Case kCases[] = {
    {"z80", "\tLD A,0x12\n\tRET\n", {"LD A,0x12", "RET"}},
    {"sm83", "\tLD A,0x12\n\tRET\n", {"LD A,0x12", "RET"}},
    {"6502", "\tLDA #0x12\n\tRTS\n", {"LDA #0x12", "RTS"}},
    {"65c02", "\tLDA #0x12\n\tRTS\n", {"LDA #0x12", "RTS"}},
    {"huc6280", "\tLDA #0x12\n\tRTS\n", {"LDA #0x12", "RTS"}},
    {"65816", "\tLDA #0x12\n\tRTS\n", {"LDA #0x12", "RTS"}},
    {"m68k", "\tmove.l D0,D1\n\trts\n", {"move.l D0,D1", "rts"}},
    {"m6809", "\tLDA #0x12\n\tRTS\n", {"LDA #0x12", "RTS"}},
    {"mips", "\taddiu sp, sp, -0x20\n\tjr ra\n\tnop\n", {"addiu sp, sp, -0x20", "jr ra", "nop"}},
    {"r4300", "\taddiu sp, sp, -0x20\n\tjr ra\n\tnop\n", {"addiu sp, sp, -0x20", "jr ra", "nop"}},
    {"r5900", "\taddiu sp, sp, -0x20\n\tjr ra\n\tnop\n", {"addiu sp, sp, -0x20", "jr ra", "nop"}},
    {"arm", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"thumb", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"arm9", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"thumb9", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"arm11", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"armv7t", "\tmovs r0,#0x1\n\tbx lr\n", {"movs r0,#0x1", "bx lr"}},
    {"armv7", "\tmov r0,#0x1\n\tbx lr\n", {"mov r0,#0x1", "bx lr"}},
    {"a64", "\tadd x0, x1, #0x10\n\tret\n", {"add x0, x1, #0x10", "ret"}},
    {"ppc", "\tli r3,0x1\n\tblr\n", {"li r3,0x1", "blr"}},
    {"sh2", "\tmov #0x1,r0\n\trts\n\tnop\n", {"mov #0x1,r0", "rts", "nop"}},
    {"sh4", "\tmov #0x1,r0\n\trts\n\tnop\n", {"mov #0x1,r0", "rts", "nop"}},
    {"v810", "\tmov r1,r2\n\tjmp [r31]\n", {"mov r1,r2", "jmp [r31]"}},
    {"cp1610", "\tMVII #0x123, R0\n", {"MVII #0x123, R0"}},   // (one instruction: a second read after a two-word one loses its place)
    {"i8048", "\tMOV A,#0x12\n\tRET\n", {"MOV A,#0x12", "RET"}},
    {"f8", "\tLI 0x12\n\tPOP\n", {"LI 0x12", "POP"}},
    {"tlcs900", "\tnop\n\tret\n", {"nop", "ret"}},
};

CB_TEST("source.processors.each_assembles_and_reads_back", [] {
    for (const Case& c : kCases) {
        const Processor& p = find(c.cpu);
        CB_CHECK(static_cast<bool>(p.assemble));
        const Image image = p.assemble(c.source, ".");
        const std::vector<Line> lines = p.disassemble(image.data, image.base, c.texts.size());
        CB_CHECK_EQ(lines.size(), c.texts.size());
        for (std::size_t k = 0; k < lines.size() && k < c.texts.size(); ++k) CB_CHECK_EQ(std::format("{}: {}", c.cpu, lines[k].text), std::format("{}: {}", c.cpu, c.texts[k]));
    }
});

CB_TEST("source.processors.names_and_aliases_find_one_row", [] {
    CB_CHECK_EQ(find("gbz80").name, std::string_view("sm83"));
    CB_CHECK_EQ(find("NES").name, std::string_view("6502"));
    CB_CHECK_EQ(find("dreamcast").name, std::string_view("sh4"));
    CB_CHECK(lookup("no-such-cpu") == nullptr);
    CB_CHECK_THROWS_AS(UsageError, (void)find("no-such-cpu"));
    // every name and alias is one row's
    std::set<std::string_view> seen;
    for (const Processor& p : all()) {
        CB_CHECK(seen.insert(p.name).second);
        for (std::string_view a : p.aliases) CB_CHECK(seen.insert(a).second);
    }
});

CB_TEST("source.processors.bytes_that_are_no_instruction_are_data", [] {
    const Bytes code = {0xFF, 0xFF};   // two bytes of an opcode the 6502 does not have
    const std::vector<Line> lines = find("6502").disassemble(code, 0x8000, 0);
    CB_CHECK_EQ(lines.size(), std::size_t{2});
    CB_CHECK_EQ(lines[0].text, std::string(".db $FF"));
    CB_CHECK_EQ(lines[1].address, std::uint64_t{0x8001});
});

CB_TEST("source.processors.the_description_reads_a_known_instruction", [] {
    const Bytes ld = {0x3E, 0x12};
    CB_CHECK_EQ(read_described(find("z80"), ld, 0, 1).at(0).text, std::string("LD A,0x12"));
    CB_CHECK_THROWS_AS(UsageError, (void)read_described(find("x86-64"), ld, 0, 1));   // (no description yet)
});

CB_TEST("source.processors.the_one_assembler_rebuilds_what_the_one_disassembler_writes", [] {
    // each processor with a description: its program read as source through the description, assembled again through it -- the
    // same bytes (the one disassembler and the one assembler are one description read both ways)
    for (const Case& c : kCases) {
        const Processor& p = find(c.cpu);
        if (p.sla.empty()) continue;
        const Image image = p.assemble(c.source, ".");
        const std::string source = source_described(p, image.data, image.base);
        const Image again = assemble_described(p, source, ".");
        CB_CHECK_EQ(std::format("{}: {}", c.cpu, again.data.size()), std::format("{}: {}", c.cpu, image.data.size()));
        CB_CHECK(again.data == image.data);
    }
    // labels: a branch to a label defined further on (the first pass guesses it)
    const Image loop = assemble_described(find("z80"), "\tLD A,0x12\nloop:\n\tDEC A\n\tJR NZ,loop\n\tJP done\ndone:\n\tRET\n", ".");
    CB_CHECK(loop.data == (Bytes{0x3E, 0x12, 0x3D, 0x20, 0xFD, 0xC3, 0x08, 0x00, 0xC9}));
    // a forward label after a word of the description's own (the condition NZ, the register HL): the label is the unknown word
    CB_CHECK(assemble_described(find("z80"), "\tJR NZ,ahead\n\tLD HL,ahead\nahead:\n\tRET\n", ".").data == (Bytes{0x20, 0x03, 0x21, 0x05, 0x00, 0xC9}));
    // the SM83's CPL is not CP L (texts that differ only by a space between two words)
    CB_CHECK(assemble_described(find("sm83"), "\tCPL\n\tCP L\n", ".").data == (Bytes{0x2F, 0xBD}));
});

CB_TEST("source.processors.an_operand_may_be_an_expression_of_names_and_address_functions", [] {
    const Processor& v810 = find("v810");
    // the halves of an address: written as an expression, as hi() / lo(), or as the number the description reads
    const Bytes high = assemble_described(v810, "\t.org 0x7000000\n\tmovhi 0x6,r0,r8\n", ".").data;
    CB_CHECK(assemble_described(v810, "\t.equ VIP, 0x5f800\n\t.org 0x7000000\n\tmovhi (VIP+0x8000)>>16,r0,r8\n", ".").data == high);
    CB_CHECK(assemble_described(v810, "\t.equ VIP, 0x5f800\n\t.org 0x7000000\n\tmovhi hi(VIP),r0,r8\n", ".").data == high);
    const Bytes low = assemble_described(v810, "\t.org 0x7000000\n\tld.h -0x800[r8],r9\n", ".").data;
    CB_CHECK(assemble_described(v810, "\t.equ VIP, 0x5f800\n\t.org 0x7000000\n\tld.h lo(VIP)[r8],r9\n", ".").data == low);
    CB_CHECK(assemble_described(v810, "\t.org 0x7000000\n\tld.h lo(0x5f800)[r8],r9\n", ".").data == low);
    // ca65's .loword / .hiword / .bankbyte, shifts and masks, under the register widths of a `.context`
    const Processor& w65816 = find("65816");
    const std::string wide = "\t.org 0x8000\n\t.context m16=1 x16=1\n\t.equ far, 0x123456\n";
    CB_CHECK(assemble_described(w65816, wide + "\tLDA #.loword(far)\n", ".").data == (Bytes{0xA9, 0x56, 0x34}));
    CB_CHECK(assemble_described(w65816, wide + "\tLDA #.hiword(far)\n", ".").data == (Bytes{0xA9, 0x12, 0x00}));
    CB_CHECK(assemble_described(w65816, wide + "\tLDA #.bankbyte(far)\n", ".").data == (Bytes{0xA9, 0x12, 0x00}));
    CB_CHECK(assemble_described(w65816, wide + "\tLDA #(far>>8)&0xFF\n", ".").data == (Bytes{0xA9, 0x34, 0x00}));
    CB_CHECK(assemble_described(w65816, wide + "\tLDA #far&0xFFFF\n", ".").data == (Bytes{0xA9, 0x56, 0x34}));
    // `.context` keeps the variable a later one leaves out
    CB_CHECK(assemble_described(w65816, "\t.org 0x8000\n\t.context m16=1\n\t.context x16=1\n\tLDA #0x1234\n\tLDX #0x1234\n", ".").data ==
             (Bytes{0xA9, 0x34, 0x12, 0xA2, 0x34, 0x12}));
    // a name that is not defined still fails, as a parse error
    CB_CHECK_THROWS_AS(ParseError, (void)assemble_described(w65816, wide + "\tLDA #.loword(nowhere)\n", "."));
    CB_CHECK_THROWS_AS(ParseError, (void)assemble_described(v810, "\t.org 0x7000000\n\tmovhi hi(nowhere),r0,r8\n", "."));
    // text that only looks like an expression is left alone: a parenthesised address is a memory operand, not a sum
    CB_CHECK(assemble_described(find("z80"), "\tLD A,(0x1234)\n\tJP (HL)\n", ".").data == (Bytes{0x3A, 0x34, 0x12, 0xE9}));
    CB_CHECK(assemble_described(find("6502"), "\tLDA (0x10),Y\n\tLDA 0x1234,X\n", ".").data == (Bytes{0xB1, 0x10, 0xBD, 0x34, 0x12}));
});

CB_TEST("source.processors.a_forward_long_label_keeps_its_size_between_passes", [] {
    // `jml far` and `lda far` are four bytes whether the label is known yet or not
    const Image image = assemble_described(find("65816"), "\t.org 0x008000\n\tjml far\n\tlda far\n\t.org 0x018000\nfar:\n\tnop\n", ".");
    CB_CHECK_EQ(image.data.size(), 9u);   // (the origin moves the address, not the bytes)
    CB_CHECK((Bytes(image.data.begin(), image.data.begin() + 8) == Bytes{0x5C, 0x00, 0x80, 0x01, 0xAF, 0x00, 0x80, 0x01}));
});

}  // namespace
}  // namespace cb::source::processors
