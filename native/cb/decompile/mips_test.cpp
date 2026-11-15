// cb/decompile/mips_test.cpp -- the R3000A's instructions: every encoding checked against words worked out by hand from the
// MIPS I instruction formats (R: op rs rt rd sa funct, I: op rs rt imm16, J: op target26) and the GTE's command word
// (op 0x12 and bit 25, fields in the low 25 bits), the text of each form, the assembler's refusals, and the round trip
// (every word that decodes is written as text that assembles to that word).
#include "cb/decompile/mips.hpp"

#include <format>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace m = cb::decompile::mips;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x80010000;

std::uint32_t word_of(const Bytes& b) {
    return static_cast<std::uint32_t>(b.at(0)) | static_cast<std::uint32_t>(b.at(1)) << 8 | static_cast<std::uint32_t>(b.at(2)) << 16 |
           static_cast<std::uint32_t>(b.at(3)) << 24;
}

std::uint32_t assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) {
    return word_of(m::assemble(line, pc, labels));
}

string disassembled(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = m::decode_word(word, pc);
    return insn ? m::text(*insn, labels) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.mips.words_and_text", [] {
    // each word worked out from the field layout; the text is what the disassembler writes and the assembler reads back
    const Case cases[] = {
        {0x00000000, "nop"},
        {0x27BDFFE0, "addiu $sp, $sp, -32"},
        {0x03E00008, "jr $ra"},
        {0x8FBF001C, "lw $ra, 28($sp)"},
        {0xAFBF001C, "sw $ra, 28($sp)"},
        {0x3C048001, "lui $a0, 0x8001"},
        {0x00851021, "addu $v0, $a0, $a1"},
        {0x00801025, "move $v0, $a0"},
        {0x00801021, "addu $v0, $a0, $zero"},
        {0x00094080, "sll $t0, $t1, 2"},
        {0x00094082, "srl $t0, $t1, 2"},
        {0x00094083, "sra $t0, $t1, 2"},
        {0x01244004, "sllv $t0, $a0, $t1"},
        {0x0085102A, "slt $v0, $a0, $a1"},
        {0x0085102B, "sltu $v0, $a0, $a1"},
        {0x00850018, "mult $a0, $a1"},
        {0x0085001B, "divu $a0, $a1"},
        {0x00001010, "mfhi $v0"},
        {0x00001012, "mflo $v0"},
        {0x00800011, "mthi $a0"},
        {0x312800FF, "andi $t0, $t1, 0xFF"},
        {0x35280001, "ori $t0, $t1, 0x1"},
        {0x2528FFFF, "addiu $t0, $t1, -1"},
        {0x2C88000A, "sltiu $t0, $a0, 10"},
        {0x8088FFFF, "lb $t0, -1($a0)"},
        {0x94880002, "lhu $t0, 2($a0)"},
        {0xA4880002, "sh $t0, 2($a0)"},
        {0x88880003, "lwl $t0, 3($a0)"},
        {0x0320F809, "jalr $t9"},
        {0x0080F009, "jalr $fp, $a0"},
        {0x0000000C, "syscall"},
        {0x000001CD, "break 0x7"},
        {0x40086000, "mfc0 $t0, $12"},
        {0x40886000, "mtc0 $t0, $12"},
        {0x42000010, "rfe"},
        {0x48880000, "mtc2 $t0, $0"},
        {0x48080800, "mfc2 $t0, $1"},
        {0x48C8F800, "ctc2 $t0, $31"},
        {0xC8800000, "lwc2 $0, 0($a0)"},
        {0xE8810004, "swc2 $1, 4($a0)"},
        {0x4A180001, "rtps"},
        {0x4B400006, "nclip"},
        {0x4A280030, "rtpt"},
        {0x4A0A0012, "mvmva 1, 1, 0, 0, 0, 0x0"},   // (the SDK numbers MVMVA with bit 22, which this word does not have)
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
});

CB_TEST("decompile.mips.branches_and_jumps", [] {
    // a branch's offset counts words from the delay slot; a jump keeps the top four bits of the delay slot's address
    CB_CHECK_EQ(assembled("beq $a0, $zero, 0x80010010"), 0x10800003u);
    CB_CHECK_EQ(disassembled(0x10800003), string("beqz $a0, 0x80010010"));
    CB_CHECK_EQ(assembled("beqz $a0, 0x80010010"), 0x10800003u);
    CB_CHECK_EQ(assembled("bne $a0, $a1, 0x80010000"), 0x1485FFFFu);   // back to itself: offset -1
    CB_CHECK_EQ(disassembled(0x1485FFFF), string("bne $a0, $a1, 0x80010000"));
    CB_CHECK_EQ(disassembled(0x1000FFFF), string("b 0x80010000"));
    CB_CHECK_EQ(assembled("b 0x80010000"), 0x1000FFFFu);
    CB_CHECK_EQ(assembled("bnez $v0, 0x80010008"), 0x14400001u);
    CB_CHECK_EQ(assembled("bgez $a0, 0x80010008"), 0x04810001u);
    CB_CHECK_EQ(assembled("bltzal $a0, 0x80010008"), 0x04900001u);
    CB_CHECK_EQ(assembled("bgezal $zero, 0x80010008"), 0x04110001u);
    CB_CHECK_EQ(assembled("bgtz $a0, 0x80010008"), 0x1C800001u);
    CB_CHECK_EQ(assembled("blez $a0, 0x80010008"), 0x18800001u);
    CB_CHECK_EQ(assembled("bc2f 0x80010008"), 0x49000001u);
    CB_CHECK_EQ(assembled("bc2t 0x80010008"), 0x49010001u);
    CB_CHECK_EQ(assembled("jal 0x80010100"), 0x0C004040u);
    CB_CHECK_EQ(assembled("j 0x80010100"), 0x08004040u);
    CB_CHECK_EQ(disassembled(0x0C004040), string("jal 0x80010100"));
    CB_CHECK_EQ(disassembled(0x0C004040, kBase, {{0x80010100, "sub_80010100"}}), string("jal sub_80010100"));
    // labels
    const cb::decompile::NameLabels labels = {{"loop", 0x80010000}, {"main", 0x80010100}};
    CB_CHECK_EQ(assembled("jal main", kBase, labels), 0x0C004040u);
    CB_CHECK_EQ(assembled("bne $a0, $a1, loop", kBase + 8, labels), 0x1485FFFDu);
    // out of reach, into another region, not a word
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("beq $a0, $a1, 0x80030008"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("j 0xA0010100"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("j 0x80010101"));
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, assembled("jal nowhere"));
});

CB_TEST("decompile.mips.decode_facts", [] {
    const auto jr_ra = m::decode_word(0x03E00008, kBase).value();
    CB_CHECK(jr_ra.flow == m::Flow::Return && jr_ra.delay_slot);
    const auto jr_t0 = m::decode_word(0x01000008, kBase).value();
    CB_CHECK(jr_t0.flow == m::Flow::Indirect && jr_t0.delay_slot);
    const auto jal = m::decode_word(0x0C004040, kBase).value();
    CB_CHECK(jal.flow == m::Flow::Call && jal.target == 0x80010100u && jal.delay_slot);
    const auto beq = m::decode_word(0x10800003, kBase).value();
    CB_CHECK(beq.flow == m::Flow::Branch && beq.target == 0x80010010u);
    const auto b = m::decode_word(0x1000FFFF, kBase).value();
    CB_CHECK(b.flow == m::Flow::Jump);
    CB_CHECK(m::decode_word(0x8FBF001C, kBase)->flow == m::Flow::None);
    CB_CHECK(!m::decode_word(0x8FBF001C, kBase)->delay_slot);
    // the word as the code holds it: little-endian
    const Bytes code = {0xE0, 0xFF, 0xBD, 0x27};
    const auto insn = m::decode(code, 0, kBase).value();
    CB_CHECK_EQ(insn.word, 0x27BDFFE0u);
    CB_CHECK_EQ(insn.bytes().size(), std::size_t{4});
    CB_CHECK(!m::decode(code, 1, kBase));   // fewer than four bytes
    // a word with a field the instruction does not use is not an instruction
    CB_CHECK(!m::decode_word(0x00200000, kBase));   // sll with rs set
    CB_CHECK(!m::decode_word(0x04040000, kBase));   // regimm rt=4
    CB_CHECK(!m::decode_word(0x44000000, kBase));   // COP1: no FPU on the PlayStation
    CB_CHECK(!m::decode_word(0x4C000000, kBase));   // COP3
    CB_CHECK(!m::decode_word(0xFFFFFFFF, kBase));
});

CB_TEST("decompile.mips.gte_commands", [] {
    // the SDK's words are the bare names
    CB_CHECK_EQ(disassembled(0x4A180001), string("rtps"));
    CB_CHECK_EQ(disassembled(0x4B400006), string("nclip"));
    // a different sf or lm shows its fields; MVMVA always does
    const std::uint32_t rtps_sf0 = 0x4A100001;
    CB_CHECK_EQ(disassembled(rtps_sf0), string("rtps 0, 0"));
    CB_CHECK_EQ(assembled("rtps 0, 0"), rtps_sf0 | 0x100000u);   // the ignored bits default to the SDK's (bit 20 is rtps's number)
    CB_CHECK_EQ(assembled("rtps 0, 0, 0x0"), rtps_sf0 & ~0x100000u);
    CB_CHECK_EQ(assembled("rtps 1, 1"), 0x4A180401u);
    CB_CHECK_EQ(assembled("mvmva 1, 0, 0, 0, 0"), 0x4A480012u | 0x10000u * 0u);
    CB_CHECK_EQ(assembled("mvmva 1, 3, 2, 1, 1", kBase), 0x4A000012u | 0x400000u | (1u << 19) | (3u << 17) | (2u << 15) | (1u << 13) | (1u << 10));   // (the ignored bits default to the SDK's: bit 22)
    // a command word whose function the GTE has no name for is `cop2` with the number
    CB_CHECK_EQ(disassembled(0x4A000002), string("cop2 0x2"));
    CB_CHECK_EQ(assembled("cop2 0x2"), 0x4A000002u);
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("rtps 2, 0"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mvmva 1, 0"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("rtps 1, 0, 0x80000"));   // those bits are fields
});

CB_TEST("decompile.mips.expressions_and_pseudo_instructions", [] {
    const cb::decompile::NameLabels labels = {{"msg", 0x80018A90}, {"tab", 0x80010004}};
    // %hi carries the sign of %lo: msg's low half 0x8A90 is negative as an offset
    CB_CHECK_EQ(m::value("%hi(msg)", labels), 0x8002);
    CB_CHECK_EQ(m::value("%lo(msg)", labels), 0x8A90);
    CB_CHECK_EQ(m::value("msg + 4", labels), 0x80018A94);
    CB_CHECK_EQ(m::value("(1 << 4) | 3", labels), 0x13);
    CB_CHECK_EQ(m::value("-1", labels), -1);
    CB_CHECK_EQ(m::value("0x10 * 2 - 4 / 2", labels), 30);
    CB_CHECK_EQ(assembled("lui $a0, %hi(msg)", kBase, labels), 0x3C048002u);
    CB_CHECK_EQ(assembled("addiu $a0, $a0, %lo(msg)", kBase, labels), 0x24848A90u);
    CB_CHECK_EQ(assembled("lw $t0, %lo(msg)($a0)", kBase, labels), 0x8C888A90u);
    CB_CHECK_EQ(assembled("lw $t0, ($a0)", kBase, labels), 0x8C880000u);
    CB_CHECK_EQ(m::text(m::decode_word(0x3C048002, kBase).value(), {}, "%hi(msg)"), string("lui $a0, %hi(msg)"));
    CB_CHECK_EQ(m::text(m::decode_word(0x8C888A90, kBase).value(), {}, "%lo(msg)"), string("lw $t0, %lo(msg)($a0)"));
    // li and la
    CB_CHECK_EQ(m::assemble("li $t0, 5", kBase).size(), std::size_t{4});
    CB_CHECK_EQ(assembled("li $t0, 5"), 0x24080005u);
    CB_CHECK_EQ(assembled("li $t0, -5"), 0x2408FFFBu);
    CB_CHECK_EQ(assembled("li $t0, 0xFFFF"), 0x3408FFFFu);
    CB_CHECK_EQ(m::assemble("li $t0, 0x12340000", kBase).size(), std::size_t{4});   // lui only
    CB_CHECK_EQ(m::assemble("li $t0, 0x12345678", kBase).size(), std::size_t{8});
    CB_CHECK_EQ(m::assemble("la $a0, msg", kBase, labels).size(), std::size_t{8});
    CB_CHECK_EQ(assembled("la $a0, msg", kBase, labels), 0x3C048002u);   // (the first word: the lui)
    // registers
    CB_CHECK_EQ(*m::register_number("$sp"), 29u);
    CB_CHECK_EQ(*m::register_number("$29"), 29u);
    CB_CHECK_EQ(*m::register_number("$s8"), 30u);
    CB_CHECK(!m::register_number("sp"));   // no dollar: a label
    CB_CHECK(!m::register_number("$32"));
    CB_CHECK_EQ(string(m::register_name(31)), string("$ra"));
    // refused
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("addiu $t0, $t1"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("addiu $t0, $t1, 70000"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("sll $t0, $t1, 32"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("frobnicate $t0"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("lw $t0, 4"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("add $t0, $t1, $nope"));
});

CB_TEST("decompile.mips.every_word_that_decodes_assembles_back", [] {
    // a deterministic spread over the encoding space, and every opcode with each of its fields at extremes
    std::uint32_t state = 0x1234567;
    int decoded = 0;
    const auto check = [&](std::uint32_t word) {
        const auto insn = m::decode_word(word, kBase);
        if (!insn) return;
        ++decoded;
        const string line = m::text(*insn);
        const std::uint32_t again = assembled(line);
        if (again != word) cb::testing::record_failure(__FILE__, __LINE__, std::format("{:08X} -> '{}' -> {:08X}", word, line, again));
    };
    for (int i = 0; i < 400000; ++i) {
        state = state * 1664525u + 1013904223u;
        check(state);
    }
    for (std::uint32_t opc = 0; opc < 64; ++opc)
        for (std::uint32_t low : {0u, 0x1FFFFFFu & 0x3FFu, 0x0A5A5A5u, 0x1FFFFFFu}) check((opc << 26) | low);
    for (std::uint32_t func = 0; func < 64; ++func)
        for (std::uint32_t mid : {0u, 0x3FFFFFFu & ~0x3Fu, 0x0124080u}) check(mid | func);
    for (std::uint32_t command = 0; command < 0x2000000; command += 0x137) check(0x4A000000u | command);
    CB_CHECK(decoded > 100000);
});
