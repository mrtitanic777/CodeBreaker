// The 65C816's instructions: the table, the disassembler and the assembler agree on every opcode at every register width.
#include "decompile/w65816.hpp"

#include <format>
#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace w = cb::decompile::w65816;
using cb::Bytes;

namespace {

std::string dis(const Bytes& code, bool m16 = false, bool x16 = false, std::uint32_t pc = 0x008000, const w::AddressLabels& labels = {}, const w::AddressLabels& dp = {}) {
    const auto insn = w::decode(code, 0, pc, m16, x16);
    return insn ? w::text(*insn, labels, dp) : "<none>";
}

}  // namespace

CB_TEST("decompile.w65816.every_opcode_is_an_instruction", [] {
    for (std::size_t op = 0; op < 256; ++op) {
        const w::OpcodeEntry& e = w::opcode_table()[op];
        CB_CHECK(!e.mnemonic.empty());
        if (e.mnemonic.empty()) continue;
        const auto back = w::opcode_of(e.mnemonic, e.mode);
        CB_CHECK(back.has_value() && *back == op);
    }
    CB_CHECK_EQ(w::opcode_of("LDA", w::Mode::ImmM).value(), 0xA9);
    CB_CHECK_EQ(w::opcode_of("LDA", w::Mode::LongX).value(), 0xBF);
    CB_CHECK_EQ(w::opcode_of("STA", w::Mode::SrIndY).value(), 0x93);
    CB_CHECK(!w::opcode_of("STA", w::Mode::ImmM).has_value());
    CB_CHECK_EQ(w::opcode_of("JSL", w::Mode::Long).value(), 0x22);
    CB_CHECK_EQ(w::opcode_of("MVN", w::Mode::Block).value(), 0x54);
    CB_CHECK_EQ(w::opcode_of("XCE", w::Mode::Imp).value(), 0xFB);
    CB_CHECK(w::is_branch("BRL") && w::is_branch("BRA") && !w::is_branch("JMP"));
    CB_CHECK(w::ends_flow("RTL") && w::ends_flow("JML") && !w::ends_flow("JSL"));
});

CB_TEST("decompile.w65816.the_register_widths_change_the_length_of_an_immediate", [] {
    const Bytes lda = {0xA9, 0x34, 0x12};
    CB_CHECK_EQ(w::decode(lda, 0, 0x8000, false, false)->size, 2);
    CB_CHECK_EQ(w::decode(lda, 0, 0x8000, true, false)->size, 3);
    CB_CHECK_EQ(w::decode(lda, 0, 0x8000, false, true)->size, 2);   // (X does not change LDA)
    const Bytes ldx = {0xA2, 0x34, 0x12};
    CB_CHECK_EQ(w::decode(ldx, 0, 0x8000, true, false)->size, 2);
    CB_CHECK_EQ(w::decode(ldx, 0, 0x8000, false, true)->size, 3);
    CB_CHECK_EQ(w::decode(Bytes{0xC2, 0x30, 0x12}, 0, 0x8000, true, true)->size, 2);   // REP is always a byte
    CB_CHECK(!w::decode(lda, 0, 0x8000, true, false).has_value() == false);
    CB_CHECK(!w::decode(Bytes{0xA9, 0x34}, 0, 0x8000, true, false).has_value());   // a 16-bit immediate cut off
    CB_CHECK_EQ(dis(lda, true, false), "LDA #$1234");
    CB_CHECK_EQ(dis(Bytes{0xA9, 0x34}, false, false), "LDA #$34");
});

CB_TEST("decompile.w65816.every_opcode_disassembles_and_assembles_back_to_its_bytes_at_every_width", [] {
    for (const bool m16 : {false, true}) {
        for (const bool x16 : {false, true}) {
            for (int op = 0; op < 256; ++op) {
                const Bytes code = {static_cast<std::uint8_t>(op), 0x12, 0x34, 0x56, 0x78};
                const auto insn = w::decode(code, 0, 0x008000, m16, x16);
                CB_CHECK(insn.has_value());
                if (!insn) continue;
                const std::string line = w::text(*insn);
                Bytes built;
                try {
                    built = w::assemble(line, 0x008000, {}, m16, x16);
                } catch (const cb::ParseError& e) {
                    CB_CHECK_EQ(std::string(e.what()), std::string("assembles: ") + line);
                }
                CB_CHECK_EQ(built.size(), static_cast<std::size_t>(insn->size));
                CB_CHECK(std::equal(built.begin(), built.end(), code.begin()));
            }
        }
    }
});

CB_TEST("decompile.w65816.the_forms", [] {
    CB_CHECK_EQ(dis({0xA5, 0x12}), "LDA $12");
    CB_CHECK_EQ(dis({0xAD, 0x12, 0x00}), "LDA a:$0012");   // an absolute address below $100 keeps its size
    CB_CHECK_EQ(dis({0xAD, 0x34, 0x12}), "LDA $1234");
    CB_CHECK_EQ(dis({0xAF, 0x34, 0x12, 0x00}), "LDA f:$001234");   // a long one below $10000 too
    CB_CHECK_EQ(dis({0xBF, 0x00, 0x90, 0x7E}), "LDA $7E9000,x");
    CB_CHECK_EQ(dis({0xB7, 0x12}), "LDA [$12],y");
    CB_CHECK_EQ(dis({0xB3, 0x12}), "LDA ($12,s),y");
    CB_CHECK_EQ(dis({0xA3, 0x12}), "LDA $12,s");
    CB_CHECK_EQ(dis({0x7C, 0x00, 0x90}), "JMP ($9000,x)");
    CB_CHECK_EQ(dis({0xDC, 0x00, 0x90}), "JML [$9000]");
    CB_CHECK_EQ(dis({0x54, 0x7E, 0x00}), "MVN $00, $7E");   // the bytes are the destination bank then the source: the text says source first
    CB_CHECK_EQ(dis({0x22, 0x40, 0x80, 0x00}), "JSL $008040");
    CB_CHECK_EQ(dis({0x22, 0x40, 0x80, 0x00}, false, false, 0x008000, {{0x008040, "sub_008040"}}), "JSL sub_008040");
    CB_CHECK_EQ(dis({0x20, 0x50, 0x80}, false, false, 0x018000, {{0x018050, ".loword(sub_018050)"}}), "JSR .loword(sub_018050)");
    CB_CHECK_EQ(dis({0x80, 0x10}, false, false, 0x008000), "BRA $008012");
    CB_CHECK_EQ(dis({0x82, 0xFD, 0xFF}, false, false, 0x008000), "BRL $008000");
    CB_CHECK_EQ(dis({0x8D, 0x00, 0x21}, false, false, 0x008000, {{0x2100, "INIDISP"}}), "STA INIDISP");
    CB_CHECK_EQ(dis({0x8D, 0x02, 0x00}, false, false, 0x008000, {{0x0002, "NAME"}}), "STA a:NAME");
    CB_CHECK_EQ(dis({0x85, 0x10}, false, false, 0x008000, {}, {{0x10, "dp_10"}}), "STA dp_10");
    CB_CHECK_EQ(dis({0xAF, 0x00, 0x90, 0x00}, false, false, 0x008000, {{0x009000, "D_009000"}}), "LDA f:D_009000");
    const auto jsl = w::decode(Bytes{0x22, 0x40, 0x80, 0x01}, 0, 0x008000, false, false);
    CB_CHECK(jsl && jsl->target && *jsl->target == 0x018040);
    const auto jsr = w::decode(Bytes{0x20, 0x40, 0x80}, 0, 0x018000, false, false);
    CB_CHECK(jsr && jsr->target && *jsr->target == 0x018040);   // JSR stays in its bank
    const auto bra = w::decode(Bytes{0x80, 0xFE}, 0, 0x00FFFE, false, false);
    CB_CHECK(bra && bra->target && *bra->target == 0x00FFFE);
});

CB_TEST("decompile.w65816.assembling_names_and_errors", [] {
    const w::NameLabels labels = {{"table", 0x9000}, {"far", 0x018123}, {"dp_20", 0x20}, {"loop", 0x8000}, {"INIDISP", 0x2100}};
    CB_CHECK(w::assemble("LDA table,x", 0x8000, labels) == (Bytes{0xBD, 0x00, 0x90}));
    CB_CHECK(w::assemble("LDA f:table,x", 0x8000, labels) == (Bytes{0xBF, 0x00, 0x90, 0x00}));
    CB_CHECK(w::assemble("LDA far", 0x8000, labels) == (Bytes{0xAF, 0x23, 0x81, 0x01}));   // past 16 bits: the long form
    CB_CHECK(w::assemble("JSR .loword(far)", 0x8000, labels) == (Bytes{0x20, 0x23, 0x81}));
    CB_CHECK(w::assemble("JSL far", 0x8000, labels) == (Bytes{0x22, 0x23, 0x81, 0x01}));
    CB_CHECK(w::assemble("STA INIDISP ; the screen", 0x8000, labels) == (Bytes{0x8D, 0x00, 0x21}));
    CB_CHECK(w::assemble("STA dp_20", 0x8000, labels) == (Bytes{0x85, 0x20}));
    CB_CHECK(w::assemble("STA a:dp_20", 0x8000, labels) == (Bytes{0x8D, 0x20, 0x00}));
    CB_CHECK(w::assemble("BRA loop", 0x8010, labels) == (Bytes{0x80, 0xEE}));
    CB_CHECK(w::assemble("BRL loop", 0x8010, labels) == (Bytes{0x82, 0xED, 0xFF}));
    CB_CHECK(w::assemble("LDA #$1234", 0x8000, labels, true, false) == (Bytes{0xA9, 0x34, 0x12}));
    CB_CHECK(w::assemble("LDX #$1234", 0x8000, labels, false, true) == (Bytes{0xA2, 0x34, 0x12}));
    CB_CHECK(w::assemble("  mvn $7E, $00", 0x8000, labels) == (Bytes{0x54, 0x00, 0x7E}));
    CB_CHECK(w::assemble("ASL", 0x8000, labels) == (Bytes{0x0A}));
    CB_CHECK(w::assemble("PEI ($12)", 0x8000, labels) == (Bytes{0xD4, 0x12}));
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("LDA #$1234", 0x8000, labels, false, false));   // too wide for an 8-bit accumulator
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("JSR far", 0x8000, labels));                     // 24 bits in a 16-bit form
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("BRA nowhere", 0x8000, labels));
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("BRA far", 0x8000, labels));   // out of reach
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("STA #$12", 0x8000, labels));
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("STA z:$1234", 0x8000, labels));   // a value past a byte is not direct page
    CB_CHECK_THROWS_AS(cb::ParseError, w::assemble("XYZ", 0x8000, labels));
});
