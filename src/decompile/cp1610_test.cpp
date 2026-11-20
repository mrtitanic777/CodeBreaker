// cb-unity: alone
// The CP1610's table: every word pattern that decodes is written as text that assembles back to the same decles; the syntax in the header, by example.
#include "decompile/cp1610.hpp"

#include <format>

#include "core/testing.hpp"

namespace cb::decompile::cp1610 {
namespace {

using Words = std::vector<std::uint16_t>;

Words words_of(std::string_view line, std::uint32_t pc = 0x5000, const NameLabels& labels = {}, bool sdbd = false) { return assemble(line, pc, labels, false, sdbd); }

}  // namespace

CB_TEST("decompile.cp1610.the_syntax_by_example", [] {
    CB_CHECK((words_of("MVII #$12,R0") == Words{0x2B8, 0x12}));        // the first instruction of an IntyBASIC program
    CB_CHECK((words_of("MVII #$1234,R1", 0x5000, {}, true) == Words{0x2B9, 0x34, 0x12}));
    CB_CHECK((words_of("MOVR R5,R7") == Words{0xAF}));                  // RETURN
    CB_CHECK((words_of("SDBD") == Words{1}));
    CB_CHECK((words_of("MVO R0,$0100") == Words{0x240, 0x100}));
    CB_CHECK((words_of("MVI $0100,R3") == Words{0x283, 0x100}));
    CB_CHECK((words_of("MVI@ R4,R1") == Words{0x2A1}));
    CB_CHECK((words_of("MVO@ R1,R4") == Words{0x261}));
    CB_CHECK((words_of("ADDI #5,R2") == Words{0x2FA, 5}));
    CB_CHECK((words_of("CMP@ R3,R0") == Words{0x358}));
    CB_CHECK((words_of("JSR R5,$5100") == Words{4, 0x150, 0x100}));
    CB_CHECK((words_of("J $5000") == Words{4, 0x350, 0}));
    CB_CHECK((words_of("JSRE R6,label", 0x5000, {{"label", 0x5100}}) == Words{4, 0x251, 0x100}));
    CB_CHECK((words_of("BNEQ there", 0x5000, {{"there", 0x5010}}) == Words{0x20C, 0xE}));
    CB_CHECK((words_of("B there", 0x5000, {{"there", 0x5000}}) == Words{0x220, 1}));
    CB_CHECK((words_of("BEXT 3,there", 0x5000, {{"there", 0x5002}}) == Words{0x213, 0}));
    CB_CHECK((words_of("SLL R1,2") == Words{0x4D}));
    CB_CHECK((words_of("SARC R3") == Words{0x7B}));
    CB_CHECK((words_of("INCR R3") == Words{0x0B}));
    CB_CHECK((words_of("GSWD R2") == Words{0x32}));
    CB_CHECK((words_of("RSWD R5") == Words{0x3D}));
    CB_CHECK((words_of("NOP") == Words{0x34}));
    CB_CHECK((words_of("SIN 1") == Words{0x37}));
    CB_CHECK((words_of("MVII #table,R4", 0x5000, {{"table", 0x2F5}}) == Words{0x2BC, 0x2F5}));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVII #$400,R0"));         // not 10 bits without SDBD
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVO R0,$4000"));          // a direct address is 10 bits
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("MVI@ R0,R1"));            // R0 and R7 are not pointers
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("BNEQ there", 0x5000, {{"there", 0x6000}}));   // out of reach
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("GSWD R5"));
    CB_CHECK_THROWS_AS(ParseError, (void)words_of("FROB"));
});

CB_TEST("decompile.cp1610.text_and_flow", [] {
    const Words code = {4, 0x150, 0x100, 0xAF, 0x20C, 0xE};
    const auto jsr = decode(code, 0, 0x5000);
    CB_CHECK(jsr && jsr->flow == Flow::Call && jsr->target == 0x5100u && jsr->size == 3);
    CB_CHECK_EQ(text(*jsr, {{0x5100, "sub_5100"}}), "JSR R5,sub_5100");
    CB_CHECK(decode(code, 3, 0)->flow == Flow::Return);             // MOVR R5,R7
    const auto bne = decode(code, 4, 0x5010);
    CB_CHECK(bne && bne->flow == Flow::Branch && bne->target == 0x5020u);
    CB_CHECK(decode(Words{0x400 | 0x2B8, 5}, 0, 0)->value == 5u);   // the bits above the tenth are not read
    CB_CHECK(!decode(Words{4, 0x150}, 0, 0).has_value());            // cut off
    CB_CHECK(!decode(Words{4, 0x153, 0}, 0, 0).has_value());         // an undefined jump flag
    CB_CHECK(decode(Words{0x2B8, 0x400 | 7}, 0, 0)->value == 7u);
    const auto double_byte = decode(Words{0x2B9, 0x34, 0x12}, 0, 0, true);
    CB_CHECK(double_byte && double_byte->size == 3 && double_byte->value == 0x1234u);
    CB_CHECK_EQ(text(*double_byte), "MVII #$1234,R1");
    CB_CHECK(decode(Words{0x2BF, 0x10}, 0, 0)->flow == Flow::Jump);  // MVII #$10,R7
    CB_CHECK(decode(Words{0x2BF, 0x10}, 0, 0)->target == 0x10u);
    CB_CHECK(!decode(Words{0x2BF}, 0, 0).has_value());               // an immediate needs its decle
    CB_CHECK(!decode(Words{0x2B8, 0x100, 0x1}, 0, 0, true).has_value());   // eight bits per decle with SDBD
    CB_CHECK(decode(Words{0x2B7}, 0, 0)->flow == Flow::Return);      // MVI@ R6,R7: pulling the pc
});

// Every word pattern, with operand decles that exercise each field, with and without SDBD.
CB_TEST("decompile.cp1610.every_decodable_instruction_assembles_back_to_itself", [] {
    std::size_t decoded = 0, failures = 0;
    const std::uint32_t pc = 0x5000;
    const auto check = [&](const Words& code, bool sdbd) {
        const auto insn = decode(code, 0, pc, sdbd);
        if (!insn) return;
        ++decoded;
        const Words original(code.begin(), code.begin() + insn->size);
        const std::string line = text(*insn);
        try {
            if (const Words built = assemble(line, pc, {}, false, sdbd); built != original) {
                ++failures;
                CB_CHECK_EQ(line + std::format(" from {:03X} {:03X} {:03X} (sdbd {})", code[0], code[1], code[2], sdbd), std::string("(assembles to the same decles)"));
            }
        } catch (const Error& e) {
            ++failures;
            CB_CHECK_EQ(line + " -- " + e.what(), std::string("(assembles)"));
        }
    };
    for (unsigned w = 0; w < 0x400; ++w) {
        for (const bool sdbd : {false, true}) {
            for (const unsigned a : {0u, 1u, 0x34u, 0xFFu, 0x100u, 0x3FFu, 0x153u, 0x2A5u}) {
                for (const unsigned b : {0u, 0x12u, 0xFFu, 0x3FFu}) check(Words{static_cast<std::uint16_t>(w), static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b)}, sdbd);
            }
        }
    }
    CB_CHECK(decoded > 15000);
    CB_CHECK_EQ(failures, 0u);
});

CB_TEST("decompile.cp1610.the_opcode_map", [] {
    std::size_t one = 0;
    for (unsigned w = 0; w < 0x400; ++w) {
        if (decode(Words{static_cast<std::uint16_t>(w), 0x10, 0x10}, 0, 0x5000)) ++one;
    }
    CB_CHECK_EQ(one, 0x400u - 8u);   // every pattern but the eight MVO@ R,R7
});

}  // namespace cb::decompile::cp1610
