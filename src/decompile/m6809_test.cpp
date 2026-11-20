// cb-unity: alone
// The 6809's table: every opcode decodes and its text assembles back to its own bytes; the syntax in the header, by example.
#include "decompile/m6809.hpp"

#include <format>

#include "core/testing.hpp"

namespace cb::decompile::m6809 {
namespace {

Bytes bytes_of(std::string_view line, std::uint32_t pc = 0x1000, const NameLabels& labels = {}) { return assemble(line, pc, labels); }

}  // namespace

CB_TEST("decompile.m6809.the_syntax_by_example", [] {
    CB_CHECK((bytes_of("LDA #$12") == Bytes{0x86, 0x12}));
    CB_CHECK((bytes_of("LDX #$C800") == Bytes{0x8E, 0xC8, 0x00}));
    CB_CHECK((bytes_of("LDA <$10") == Bytes{0x96, 0x10}));
    CB_CHECK((bytes_of("LDA $C800") == Bytes{0xB6, 0xC8, 0x00}));
    CB_CHECK((bytes_of("STA ,X+") == Bytes{0xA7, 0x80}));
    CB_CHECK((bytes_of("STA ,--Y") == Bytes{0xA7, 0xA3}));
    CB_CHECK((bytes_of("LDA 5,X") == Bytes{0xA6, 0x05}));
    CB_CHECK((bytes_of("LDA -16,U") == Bytes{0xA6, 0x50}));
    CB_CHECK((bytes_of("LDA 100,X") == Bytes{0xA6, 0x88, 0x64}));
    CB_CHECK((bytes_of("LDA <5,X") == Bytes{0xA6, 0x88, 0x05}));
    CB_CHECK((bytes_of("LDA >5,X") == Bytes{0xA6, 0x89, 0x00, 0x05}));
    CB_CHECK((bytes_of("LDA B,X") == Bytes{0xA6, 0x85}));
    CB_CHECK((bytes_of("LDD [,Y]") == Bytes{0xEC, 0xB4}));
    CB_CHECK((bytes_of("LDD [$C800]") == Bytes{0xEC, 0x9F, 0xC8, 0x00}));
    CB_CHECK((bytes_of("PSHS A,B,X,PC") == Bytes{0x34, 0x96}));
    CB_CHECK((bytes_of("PSHU S") == Bytes{0x36, 0x40}));
    CB_CHECK((bytes_of("TFR A,B") == Bytes{0x1F, 0x89}));
    CB_CHECK((bytes_of("EXG X,Y") == Bytes{0x1E, 0x12}));
    CB_CHECK((bytes_of("LBRA $1100") == Bytes{0x16, 0x00, 0xFD}));
    CB_CHECK((bytes_of("BNE $1010") == Bytes{0x26, 0x0E}));
    CB_CHECK((bytes_of("CMPD #$1234") == Bytes{0x10, 0x83, 0x12, 0x34}));
    CB_CHECK((bytes_of("LDY #$1234") == Bytes{0x10, 0x8E, 0x12, 0x34}));
    CB_CHECK((bytes_of("SWI3") == Bytes{0x11, 0x3F}));
    CB_CHECK((bytes_of("JSR Wait_Recal", 0x1000, {{"Wait_Recal", 0xF192}}) == Bytes{0xBD, 0xF1, 0x92}));
    CB_CHECK((bytes_of("BRA there", 0x1000, {{"there", 0x1000}}) == Bytes{0x20, 0xFE}));
    CB_CHECK((bytes_of("LDX #<$1234") == Bytes{0x8E, 0x00, 0x34}));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("BRA $2000"));      // out of reach
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("STA #$12"));       // no immediate store
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("FROB"));
});

CB_TEST("decompile.m6809.text_and_flow", [] {
    const Bytes code = {0xBD, 0xF1, 0x92, 0x26, 0xFB, 0x39, 0x6E, 0x84, 0x35, 0x80};
    const auto jsr = decode(code, 0, 0x100);
    CB_CHECK(jsr && jsr->flow == Flow::Call && jsr->target == 0xF192u && jsr->size == 3);
    CB_CHECK_EQ(text(*jsr, {{0xF192, "Wait_Recal"}}), "JSR Wait_Recal");
    const auto bne = decode(code, 3, 0x103);
    CB_CHECK(bne && bne->flow == Flow::Branch && bne->target == 0x100u);
    CB_CHECK(decode(code, 5, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 6, 0)->flow == Flow::Indirect);   // JMP ,X
    CB_CHECK(decode(code, 8, 0)->flow == Flow::Return);     // PULS PC
    CB_CHECK(!decode(Bytes{0x01}, 0, 0).has_value());       // undefined
    CB_CHECK(!decode(Bytes{0xBD, 0xF1}, 0, 0).has_value()); // cut off
    CB_CHECK(!decode(Bytes{0xA6, 0x87}, 0, 0).has_value()); // an indexed postbyte the 6809 does not define
});

// Every opcode of the three pages, with operands that exercise each field: what decodes is written as text that assembles to the same bytes.
CB_TEST("decompile.m6809.every_decodable_instruction_assembles_back_to_itself", [] {
    std::size_t decoded = 0, failures = 0;
    const std::uint32_t pc = 0x1000;
    const auto check = [&](const Bytes& code) {
        const auto insn = decode(code, 0, pc);
        if (!insn) return;
        ++decoded;
        CB_CHECK_EQ(static_cast<std::size_t>(insn->size) <= code.size(), true);
        const Bytes original(code.begin(), code.begin() + insn->size);
        std::string line = text(*insn);
        try {
            if (const Bytes built = assemble(line, pc); built != original) {
                ++failures;
                const auto hex = [](const Bytes& b) {
                    std::string h;
                    for (const std::uint8_t v : b) h += std::format("{:02X} ", v);
                    return h;
                };
                CB_CHECK_EQ(line + " from " + hex(original) + "built " + hex(built), std::string("(assembles to the same bytes)"));
            }
        } catch (const Error& e) {
            ++failures;
            CB_CHECK_EQ(line + " -- " + e.what(), std::string("(assembles)"));
        }
    };
    for (unsigned page = 0; page < 3; ++page) {
        for (unsigned op = 0; op < 256; ++op) {
            Bytes head;
            if (page != 0) head.push_back(static_cast<std::uint8_t>(0x0F + page));
            head.push_back(static_cast<std::uint8_t>(op));
            if (page == 0 && (op == 0x10 || op == 0x11)) continue;
            const auto& entry = opcode_table(page)[op];
            if (!entry) {
                CB_CHECK(!decode(head, 0, pc).has_value());
                continue;
            }
            const auto with = [&](std::initializer_list<std::uint8_t> tail) {
                Bytes c = head;
                c.insert(c.end(), tail);
                return c;
            };
            switch (entry->mode) {
                case Mode::Inh: check(head); break;
                case Mode::Imm8: case Mode::Dir: case Mode::Rel8: case Mode::Regs:
                    for (const int v : {0x00, 0x01, 0x7F, 0x80, 0xFF, 0x5A}) check(with({static_cast<std::uint8_t>(v)}));
                    break;
                case Mode::Pair:
                    for (unsigned v = 0; v < 256; ++v) check(with({static_cast<std::uint8_t>(v)}));
                    break;
                case Mode::Imm16: case Mode::Ext: case Mode::Rel16:
                    for (const unsigned v : {0x0000u, 0x0001u, 0x1234u, 0x8001u, 0xFFFFu}) check(with({static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)}));
                    break;
                case Mode::Idx:
                    for (unsigned post = 0; post < 256; ++post) {
                        for (const int lo : {0x00, 0x05, 0x7F, 0x80, 0xFF}) check(with({static_cast<std::uint8_t>(post), 0x12, static_cast<std::uint8_t>(lo)}));
                    }
                    break;
            }
        }
    }
    CB_CHECK(decoded > 20000);
    CB_CHECK_EQ(failures, 0u);
});

CB_TEST("decompile.m6809.the_table_has_the_6809_opcodes", [] {
    std::size_t count[3] = {0, 0, 0};
    for (unsigned page = 0; page < 3; ++page) {
        for (const auto& e : opcode_table(page)) count[page] += e.has_value() ? 1 : 0;
    }
    CB_CHECK_EQ(count[0], 221u);   // the first page, apart from the two prefixes
    CB_CHECK(count[1] > 30 && count[2] > 8);
    CB_CHECK(opcode_of("LDA", Mode::Imm8).has_value());
    CB_CHECK(!opcode_of("STA", Mode::Imm8).has_value());
    CB_CHECK(is_branch("LBNE") && is_branch("BSR") && !is_branch("JSR"));
});

}  // namespace cb::decompile::m6809
