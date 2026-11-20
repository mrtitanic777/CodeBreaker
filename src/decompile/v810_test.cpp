// cb-unity: alone
// The V810's table: every bit pattern that decodes is written as text that assembles back to its own bytes; the syntax in the header, by example.
#include "decompile/v810.hpp"

#include <format>

#include "core/testing.hpp"

namespace cb::decompile::v810 {
namespace {

Bytes bytes_of(std::string_view line, std::uint32_t pc = 0x07001000, const NameLabels& labels = {}) { return assemble(line, pc, labels); }

}  // namespace

CB_TEST("decompile.v810.the_syntax_by_example", [] {
    CB_CHECK((bytes_of("movhi 0x0701,r0,r1") == Bytes{0x20, 0xBC, 0x01, 0x07}));      // the reset code of the Virtual Boy demos
    CB_CHECK((bytes_of("movea 0x320C,r1,r1") == Bytes{0x21, 0xA0, 0x0C, 0x32}));
    CB_CHECK((bytes_of("jmp [r1]") == Bytes{0x01, 0x18}));
    CB_CHECK((bytes_of("jmp [lp]") == Bytes{0x1F, 0x18}));
    CB_CHECK((bytes_of("mov r1,r2") == Bytes{0x41, 0x00}));
    CB_CHECK((bytes_of("mov -3,r5") == Bytes{0xBD, 0x40}));
    CB_CHECK((bytes_of("nop") == Bytes{0x00, 0x00}));
    CB_CHECK((bytes_of("shl 3,r4") == Bytes{0x83, 0x50}));
    CB_CHECK((bytes_of("shl r1,r4") == Bytes{0x81, 0x10}));
    CB_CHECK((bytes_of("setf 2,r6") == Bytes{0xC2, 0x48}));
    CB_CHECK((bytes_of("ld.w 8[r5],r6") == Bytes{0xC5, 0xCC, 0x08, 0x00}));
    CB_CHECK((bytes_of("st.h r6,-2[sp]") == Bytes{0xC3, 0xD4, 0xFE, 0xFF}));
    CB_CHECK((bytes_of("ld.b [r3],r2") == bytes_of("ld.b 0[r3],r2")));
    CB_CHECK((bytes_of("addi -1,r3,r3") == Bytes{0x63, 0xA4, 0xFF, 0xFF}));
    CB_CHECK((bytes_of("addi 65535,r3,r3") == Bytes{0x63, 0xA4, 0xFF, 0xFF}));
    CB_CHECK((bytes_of("ldsr r7,psw") == Bytes{0xE5, 0x70}));
    CB_CHECK((bytes_of("stsr eipc,r8") == Bytes{0x00, 0x75}));
    CB_CHECK((bytes_of("trap 5") == Bytes{0x05, 0x60}));
    CB_CHECK((bytes_of("reti") == Bytes{0x00, 0x64}));
    CB_CHECK((bytes_of("sch0bsu") == Bytes{0x00, 0x7C}));
    CB_CHECK((bytes_of("addf.s r1,r2") == Bytes{0x41, 0xF8, 0x00, 0x10}));
    CB_CHECK((bytes_of("xb r3") == Bytes{0x60, 0xF8, 0x00, 0x20}));
    CB_CHECK((bytes_of("bne there", 0x07001000, {{"there", 0x07001010}}) == Bytes{0x10, 0x94}));
    CB_CHECK((bytes_of("br there", 0x07001000, {{"there", 0x07000FF0}}) == Bytes{0xF0, 0x8B}));
    CB_CHECK((bytes_of("jr there", 0x07001000, {{"there", 0x07001100}}) == Bytes{0x00, 0xA8, 0x00, 0x01}));
    CB_CHECK((bytes_of("jal there", 0x07001000, {{"there", 0x06FFF000}}) == Bytes{0xFF, 0xAF, 0x00, 0xE0}));
    CB_CHECK((bytes_of("movhi hi(there),r0,r6", 0x07001000, {{"there", 0x0701A000}}) == Bytes{0xC0, 0xBC, 0x02, 0x07}));   // (hi rounds up: the low half is negative)
    CB_CHECK((bytes_of("movea lo(there),r6,r6", 0x07001000, {{"there", 0x0701A000}}) == Bytes{0xC6, 0xA0, 0x00, 0xA0}));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("bne 0x07002000"));      // out of reach
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("mov 99,r1"));           // not 5 bits
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("ld.w r1,r2"));          // not a memory operand
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("frob"));
});

CB_TEST("decompile.v810.text_and_flow", [] {
    const Bytes code = {0x00, 0xAC, 0x00, 0x00, 0x1F, 0x18, 0x01, 0x18, 0x00, 0x64, 0x00, 0x68, 0x00, 0x94};
    const auto jal = decode(code, 0, 0x100);
    CB_CHECK(jal && jal->flow == Flow::Call && jal->size == 4 && jal->target == 0x100u);
    CB_CHECK_EQ(text(*jal, {{0x100, "start"}}), "jal start");
    CB_CHECK(decode(code, 4, 0)->flow == Flow::Return);      // jmp [lp]
    CB_CHECK(decode(code, 6, 0)->flow == Flow::Indirect);    // jmp [r1]
    CB_CHECK(decode(code, 8, 0)->flow == Flow::Return);      // reti
    CB_CHECK(decode(code, 10, 0)->flow == Flow::Halt);
    const auto bne = decode(code, 12, 0x200);
    CB_CHECK(bne && bne->flow == Flow::Branch && bne->target == 0x200u);
    CB_CHECK(!decode(Bytes{0x00, 0x6C}, 0, 0).has_value());                // opcode 1B
    CB_CHECK(!decode(Bytes{0x00, 0xA8, 0x00}, 0, 0).has_value());          // cut off
    CB_CHECK(!decode(Bytes{0x01, 0x80}, 0, 0).has_value());                // a branch displacement with its low bit set
    CB_CHECK(!decode(Bytes{0x41, 0x18}, 0, 0).has_value());                // jmp with a second register
    CB_CHECK(decode(Bytes{0x01, 0xF8, 0x00, 0x00}, 0, 0).has_value());   // cmpf.s r1,r0 is an instruction
    CB_CHECK(!decode(Bytes{0x01, 0xF8, 0x00, 0x44}, 0, 0).has_value());    // an undefined extended instruction
    CB_CHECK(!decode(Bytes{0x01, 0xF8, 0x01, 0x10}, 0, 0).has_value());    // reserved bits set
});

// Every first halfword, with second halfwords that exercise each field: what decodes is written as text that assembles to the same bytes.
CB_TEST("decompile.v810.every_decodable_instruction_assembles_back_to_itself", [] {
    std::size_t decoded = 0, failures = 0;
    const std::uint32_t pc = 0x07001000;
    const auto check = [&](unsigned hw0, unsigned hw1) {
        const Bytes code = {static_cast<std::uint8_t>(hw0), static_cast<std::uint8_t>(hw0 >> 8), static_cast<std::uint8_t>(hw1), static_cast<std::uint8_t>(hw1 >> 8)};
        const auto insn = decode(code, 0, pc);
        if (!insn) return;
        ++decoded;
        const Bytes original(code.begin(), code.begin() + insn->size);
        const std::string line = text(*insn);
        try {
            if (const Bytes built = assemble(line, pc); built != original) {
                ++failures;
                CB_CHECK_EQ(line + std::format(" from {:04X} {:04X} built {} bytes", hw0, hw1, built.size()), std::string("(assembles to the same bytes)"));
            }
        } catch (const Error& e) {
            ++failures;
            CB_CHECK_EQ(line + " -- " + e.what(), std::string("(assembles)"));
        }
    };
    for (unsigned hw0 = 0; hw0 < 0x10000; ++hw0) {
        const unsigned opcode = hw0 >> 10;
        if (opcode < 0x28 || (hw0 >> 13) == 4) {
            check(hw0, 0);   // (a 16-bit instruction: the second halfword is the next one's)
        } else if (opcode == id::kExtended) {
            if ((hw0 & 0x3FF) % 5 != 0) continue;   // (a spread of register pairs)
            for (unsigned sub = 0; sub < 64; ++sub) check(hw0, sub << 10);
            check(hw0, 0x0001);
        } else {
            for (const unsigned v : {0x0000u, 0x0001u, 0x0002u, 0x7FFEu, 0x8000u, 0xFFFEu, 0x1234u, 0xFFFFu}) check(hw0, v);
        }
    }
    CB_CHECK(decoded > 150000);
    CB_CHECK_EQ(failures, 0u);
});

CB_TEST("decompile.v810.the_table_has_the_v810_opcodes", [] {
    std::size_t main = 0, ext = 0, bits = 0;
    for (const auto& e : opcode_table()) main += e.has_value() && !e->mnemonic.empty() ? 1 : 0;
    for (const auto& e : extended_table()) ext += e.has_value() ? 1 : 0;
    for (const auto& e : bitstring_table()) bits += e.has_value() ? 1 : 0;
    CB_CHECK_EQ(main, 51u);   // (the branches are not in the table; the two prefixes are not counted)
    CB_CHECK_EQ(ext, 12u);
    CB_CHECK_EQ(bits, 12u);
    CB_CHECK(is_mnemonic("bne") && is_mnemonic("BNZ") && is_mnemonic("sch1bsd") && is_mnemonic("cvt.ws") && !is_mnemonic("jsr"));
});

}  // namespace cb::decompile::v810
