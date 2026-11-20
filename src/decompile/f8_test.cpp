// cb-unity: alone
// The F8's table: every opcode decodes and its text assembles back to its own bytes; the syntax in the header, by example.
#include "decompile/f8.hpp"

#include <format>

#include "core/testing.hpp"

namespace cb::decompile::f8 {
namespace {

Bytes bytes_of(std::string_view line, std::uint32_t pc = 0x0800, const NameLabels& labels = {}) { return assemble(line, pc, labels); }

}  // namespace

CB_TEST("decompile.f8.the_syntax_by_example", [] {
    CB_CHECK((bytes_of("LI $12") == Bytes{0x20, 0x12}));
    CB_CHECK((bytes_of("li 18") == Bytes{0x20, 0x12}));
    CB_CHECK((bytes_of("LR A,R3") == Bytes{0x43}));
    CB_CHECK((bytes_of("LR S,A") == Bytes{0x5C}));
    CB_CHECK((bytes_of("LR A,I") == Bytes{0x4D}));
    CB_CHECK((bytes_of("LR D,A") == Bytes{0x5E}));
    CB_CHECK((bytes_of("LR A,KU") == Bytes{0x00}));
    CB_CHECK((bytes_of("LR DC,H") == Bytes{0x10}));
    CB_CHECK((bytes_of("AS R5") == Bytes{0xC5}));
    CB_CHECK((bytes_of("NS I") == Bytes{0xFD}));
    CB_CHECK((bytes_of("DS S") == Bytes{0x3C}));
    CB_CHECK((bytes_of("LIS 5") == Bytes{0x75}));
    CB_CHECK((bytes_of("LISU 3") == Bytes{0x63}));
    CB_CHECK((bytes_of("LISL 7") == Bytes{0x6F}));
    CB_CHECK((bytes_of("INS 4") == Bytes{0xA4}));
    CB_CHECK((bytes_of("OUTS 15") == Bytes{0xBF}));
    CB_CHECK((bytes_of("OUT $01") == Bytes{0x27, 0x01}));
    CB_CHECK((bytes_of("IN 0") == Bytes{0x26, 0x00}));
    CB_CHECK((bytes_of("PI $0900") == Bytes{0x28, 0x09, 0x00}));
    CB_CHECK((bytes_of("JMP sub", 0x0800, {{"sub", 0x1234}}) == Bytes{0x29, 0x12, 0x34}));
    CB_CHECK((bytes_of("DCI $2800") == Bytes{0x2A, 0x28, 0x00}));
    CB_CHECK((bytes_of("BR $0810", 0x0800) == Bytes{0x90, 0x0F}));
    CB_CHECK((bytes_of("BT 4,there", 0x0820, {{"there", 0x0820}}) == Bytes{0x84, 0xFF}));
    CB_CHECK((bytes_of("BF 2,$0802", 0x0800) == Bytes{0x92, 0x01}));
    CB_CHECK((bytes_of("BR7 $0800", 0x0810) == Bytes{0x8F, 0xEF}));
    CB_CHECK((bytes_of("POP") == Bytes{0x1C}));
    CB_CHECK((bytes_of("NOP") == Bytes{0x2B}));
    CB_CHECK((bytes_of("SR 4") == Bytes{0x14}));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("BR $0900"));          // out of reach
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("LR A,R15"));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("LIS 16"));
    CB_CHECK_THROWS_AS(ParseError, (void)bytes_of("FROB"));
});

CB_TEST("decompile.f8.text_and_flow", [] {
    const Bytes code = {0x28, 0x09, 0x00, 0x1C, 0x29, 0x08, 0x10, 0x84, 0xFF, 0x0D, 0x2A, 0x28, 0x00};
    const auto pi = decode(code, 0, 0x800);
    CB_CHECK(pi && pi->flow == Flow::Call && pi->size == 3 && pi->target == 0x900u);
    CB_CHECK_EQ(text(*pi, {{0x900, "sub_0900"}}), "PI sub_0900");
    CB_CHECK(decode(code, 3, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 4, 0)->flow == Flow::Jump);
    const auto bt = decode(code, 7, 0x820);
    CB_CHECK(bt && bt->flow == Flow::Branch && bt->target == 0x820u);   // the displacement counts from its own address
    CB_CHECK(decode(code, 9, 0)->flow == Flow::Indirect);               // LR P0,Q
    const auto dci = decode(code, 10, 0);
    CB_CHECK(dci && dci->flow == Flow::None && dci->value == 0x2800u);
    CB_CHECK_EQ(text(*dci, {{0x2800, "D_2800"}}, true), "DCI D_2800");
    CB_CHECK_EQ(text(*dci, {{0x2800, "D_2800"}}, false), "DCI $2800");
    CB_CHECK(!decode(code, 11, 0).has_value());                          // cut off
    CB_CHECK(!decode(Bytes{0x2D}, 0, 0).has_value());                    // undefined
    CB_CHECK(!decode(Bytes{0x4F}, 0, 0).has_value());                    // R15
    CB_CHECK(decode(Bytes{0x80, 0x05}, 0, 0)->flow == Flow::None);       // BT 0 never branches
});

CB_TEST("decompile.f8.every_opcode_assembles_back_to_itself", [] {
    std::size_t decoded = 0;
    for (unsigned op = 0; op < 256; ++op) {
        for (const unsigned operand : {0x00u, 0x01u, 0x7Fu, 0x80u, 0xFFu, 0x5Au}) {
            const Bytes code = {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(operand), static_cast<std::uint8_t>(operand ^ 0x33)};
            const auto insn = decode(code, 0, 0x0800);
            if (!insn) continue;
            if (operand == 0) ++decoded;
            const std::string line = text(*insn);
            try {
                CB_CHECK_EQ(assemble(line, 0x0800) == Bytes(code.begin(), code.begin() + insn->size), true);
            } catch (const Error& e) {
                CB_CHECK_EQ(line + " -- " + e.what(), std::string("(assembles)"));
            }
        }
    }
    CB_CHECK_EQ(decoded, 246u);
});

}  // namespace cb::decompile::f8
