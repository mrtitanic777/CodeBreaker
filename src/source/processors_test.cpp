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
const Case kCases[] = {
    {"z80", "\tLD A,$12\n\tRET\n", {"LD A,$12", "RET"}},
    {"sm83", "\tld a,$12\n\tret\n", {"LD A,$12", "RET"}},
    {"6502", "\tprocessor 6502\n\torg $8000\n\tlda #$12\n\trts\n", {"LDA #$12", "RTS"}},
    {"65c02", "\tprocessor 65c02\n\torg $8000\n\tlda #$12\n\trts\n", {"LDA #$12", "RTS"}},
    {"huc6280", "\tprocessor 6502\n\torg $e000\n\tlda #$12\n\trts\n", {"LDA #$12", "RTS"}},
    {"65816", "\tprocessor 6502\n\torg $8000\n\tlda #$12\n\trts\n", {"LDA #$12", "RTS"}},
    {"m68k", "\tmove.l d0,d1\n\trts\n", {"MOVE.L D0,D1", "RTS"}},
    {"m6809", "\tlda #$12\n\trts\n", {"LDA #$12", "RTS"}},
    {"mips", "\taddiu $sp, $sp, -32\n\tjr $ra\n\tnop\n", {"addiu $sp, $sp, -32", "jr $ra", "nop"}},
    {"r4300", "\taddiu $sp, $sp, -32\n\tjr $ra\n\tnop\n", {"addiu $sp, $sp, -32", "jr $ra", "nop"}},
    {"r5900", "\taddiu $sp, $sp, -32\n\tjr $ra\n\tnop\n", {"addiu $sp, $sp, -32", "jr $ra", "nop"}},
    {"arm", "\tmov r0, #1\n\tbx lr\n", {"mov r0, #1", "bx lr"}},
    {"arm9", "\tmov r0, #1\n\tbx lr\n", {"mov r0, #1", "bx lr"}},
    {"arm11", "\tmov r0, #1\n\tbx lr\n", {"mov r0, #1", "bx lr"}},
    {"armv7", "\tmov r0, #1\n\tbx lr\n", {"mov r0, #1", "bx lr"}},
    {"a64", "\tadd x0, x1, #0x10\n\tret\n", {"add x0, x1, #0x10", "ret"}},
    {"ppc", "\tli r3,1\n\tblr\n", {"li r3,1", "blr"}},
    {"sh2", "\tmov #1,r0\n\trts\n\tnop\n", {"mov     #1,r0", "rts", "nop"}},
    {"sh4", "\tmov #1,r0\n\trts\n\tnop\n", {"mov     #1,r0", "rts", "nop"}},
    {"v810", "\tmov r1,r2\n\tjmp [r31]\n", {"mov r1,r2", "jmp [r31]"}},
    {"cp1610", "        FORMAT RAW\n        SEGMENT $50,$50\n        ORG $5000\n        MVII #$0123,R0\n        ADDR R1,R2\n", {"MVII #$123,R0", "ADDR R1,R2"}},
    {"i8048", "\tMOV A,#$12\n\tRET\n", {"MOV A,#$12", "RET"}},
    {"f8", "\tLI $12\n\tPOP\n", {"LI $12", "POP"}},
    {"tlcs900", "\tld XWA,0x00200100\n\tret\n", {"ld XWA,0x00200100", "ret"}},
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

CB_TEST("source.processors.the_sleigh_descriptions_read_what_the_own_tables_read", [] {
    // one program per processor that has a description, read both ways: the same instructions, the same lengths
    for (const Case& c : kCases) {
        const Processor& p = find(c.cpu);
        if (p.sla.empty()) continue;
        const Image image = p.assemble(c.source, ".");
        const std::vector<Line> own = p.disassemble(image.data, image.base, c.texts.size());
        const std::vector<Line> described = read_described(p, image.data, image.base, c.texts.size());
        CB_CHECK_EQ(described.size(), own.size());
        for (std::size_t k = 0; k < own.size() && k < described.size(); ++k) {
            CB_CHECK_EQ(std::format("{}: {}", c.cpu, described[k].bytes.size()), std::format("{}: {}", c.cpu, own[k].bytes.size()));
            CB_CHECK(!described[k].text.starts_with(".d"));
        }
    }
    const Bytes ld = {0x3E, 0x12};
    CB_CHECK_EQ(read_described(find("z80"), ld, 0, 1).at(0).text, std::string("LD A,0x12"));
    CB_CHECK_THROWS_AS(UsageError, (void)read_described(find("f8"), ld, 0, 1));   // (no description yet)
});

}  // namespace
}  // namespace cb::source::processors
