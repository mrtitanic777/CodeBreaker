// cb-unity: alone
// The 8048's table: every opcode decodes and its text assembles back to its own bytes; the syntax in the header, by example.
#include "cb/decompile/i8048.hpp"

#include <format>

#include "cb/core/testing.hpp"

namespace cb::decompile::i8048 {
namespace {

Bytes bytes_of(std::string_view line, std::uint32_t pc = 0x420, const NameLabels& labels = {}) { return assemble(line, pc, labels); }

}  // namespace

CB_TEST("decompile.i8048.the_syntax_by_example", [] {
    CB_CHECK((bytes_of("MOV A,#$12") == Bytes{0x23, 0x12}));
    CB_CHECK((bytes_of("mov a, #18") == Bytes{0x23, 0x12}));
    CB_CHECK((bytes_of("MOV A,R3") == Bytes{0xFB}));
    CB_CHECK((bytes_of("MOV @R1,A") == Bytes{0xA1}));
    CB_CHECK((bytes_of("MOV R5,#$FF") == Bytes{0xBD, 0xFF}));
    CB_CHECK((bytes_of("ADD A,@R0") == Bytes{0x60}));
    CB_CHECK((bytes_of("MOVX @R1,A") == Bytes{0x91}));
    CB_CHECK((bytes_of("MOVP3 A,@A") == Bytes{0xE3}));
    CB_CHECK((bytes_of("JMPP @A") == Bytes{0xB3}));
    CB_CHECK((bytes_of("ANLD P5,A") == Bytes{0x9D}));
    CB_CHECK((bytes_of("ORL P2,#$0F") == Bytes{0x8A, 0x0F}));
    CB_CHECK((bytes_of("SEL RB1") == Bytes{0xD5}));
    CB_CHECK((bytes_of("JMP $0450", 0x400) == Bytes{0x84, 0x50}));
    CB_CHECK((bytes_of("CALL $0123", 0x400) == Bytes{0x34, 0x23}));
    CB_CHECK((bytes_of("JMP there", 0x400, {{"there", 0x7FF}}) == Bytes{0xE4, 0xFF}));
    CB_CHECK((bytes_of("DJNZ R2,$0410", 0x400) == Bytes{0xEA, 0x10}));
    CB_CHECK((bytes_of("JNZ loop", 0x4F0, {{"loop", 0x405}}) == Bytes{0x96, 0x05}));
    CB_CHECK((bytes_of("JB3 $0412", 0x400) == Bytes{0x72, 0x12}));
    CB_CHECK((bytes_of("MOV A,#<table", 0x400, {{"table", 0x0634}}) == Bytes{0x23, 0x34}));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("JNZ $0520", 0x400));      // another page
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("MOV A,#300"));            // not a byte
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("MOV A,R8"));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("FROB"));
});

CB_TEST("decompile.i8048.text_and_flow", [] {
    const Bytes code = {0x94, 0x30, 0x83, 0xB3, 0x96, 0x05, 0x84, 0x50, 0x23};
    const auto call = decode(code, 0, 0x400);
    CB_CHECK(call && call->flow == Flow::Call && call->target == 0x430u && call->size == 2);
    CB_CHECK_EQ(text(*call, {{0x430, "sub_0430"}}), "CALL sub_0430");
    CB_CHECK(decode(code, 2, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 3, 0)->flow == Flow::Indirect);
    const auto jnz = decode(code, 4, 0x4FF);   // on the last byte of its page: the next page
    CB_CHECK(jnz && jnz->flow == Flow::Branch && jnz->target == 0x505u);
    const auto jmp = decode(code, 6, 0x800);   // the memory bank is that of the address
    CB_CHECK(jmp && jmp->flow == Flow::Jump && jmp->target == 0xC50u);
    CB_CHECK(!decode(code, 8, 0).has_value());              // cut off
    CB_CHECK(!decode(Bytes{0x01}, 0, 0).has_value());       // undefined
});

CB_TEST("decompile.i8048.every_opcode_assembles_back_to_itself", [] {
    std::size_t decoded = 0;
    for (unsigned op = 0; op < 256; ++op) {
        for (const unsigned operand : {0x00u, 0x01u, 0x7Fu, 0x80u, 0xFFu, 0x5Au}) {
            const Bytes code = {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(operand)};
            const auto insn = decode(code, 0, 0x420);
            if (!insn) {
                CB_CHECK(!opcode_table()[op].has_value());
                continue;
            }
            if (operand == 0) ++decoded;
            const std::string line = text(*insn);
            try {
                CB_CHECK_EQ(assemble(line, 0x420) == Bytes(code.begin(), code.begin() + insn->size), true);
            } catch (const Error& e) {
                CB_CHECK_EQ(line + " -- " + e.what(), std::string("(assembles)"));
            }
        }
    }
    CB_CHECK_EQ(decoded, 231u);
    CB_CHECK(!opcode_table()[0x01].has_value() && !opcode_table()[0x38].has_value());
});

}  // namespace cb::decompile::i8048
