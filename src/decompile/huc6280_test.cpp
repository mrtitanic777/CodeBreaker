// The HuC6280's instructions: the table, the disassembler and the assembler agree on every opcode.
#include "decompile/huc6280.hpp"

#include <format>
#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace hu = cb::decompile::huc6280;
using cb::Bytes;

namespace {

std::string dis(const Bytes& code, std::uint32_t pc = 0xE000, const hu::AddressLabels& labels = {}, const hu::AddressLabels& zp = {}) {
    const auto insn = hu::decode(code, 0, pc);
    return insn ? hu::text(*insn, labels, zp) : "<none>";
}

}  // namespace

CB_TEST("decompile.huc6280.the_table_is_the_65c02_and_hudson's_additions", [] {
    int documented = 0;
    for (const auto& entry : hu::opcode_table()) documented += entry.has_value();
    CB_CHECK_EQ(documented, 178 + 32 + 24);   // the 65SC02's 178 (151 and 27 more), the 32 Rockwell bit instructions, 24 of Hudson's
    CB_CHECK_EQ(hu::opcode_of("TII", hu::Mode::Block).value(), 0x73);
    CB_CHECK_EQ(hu::opcode_of("TAI", hu::Mode::Block).value(), 0xF3);
    CB_CHECK_EQ(hu::opcode_of("TAM", hu::Mode::Imm).value(), 0x53);
    CB_CHECK_EQ(hu::opcode_of("BBR0", hu::Mode::ZpRel).value(), 0x0F);
    CB_CHECK_EQ(hu::opcode_of("BBS7", hu::Mode::ZpRel).value(), 0xFF);
    CB_CHECK_EQ(hu::opcode_of("RMB3", hu::Mode::Zp).value(), 0x37);
    CB_CHECK_EQ(hu::opcode_of("SMB0", hu::Mode::Zp).value(), 0x87);
    CB_CHECK_EQ(hu::opcode_of("TST", hu::Mode::ImmAbx).value(), 0xB3);
    CB_CHECK_EQ(hu::opcode_of("LDA", hu::Mode::Imm).value(), 0xA9);
    CB_CHECK(hu::is_mnemonic("CSH") && hu::is_mnemonic("PHX") && !hu::is_mnemonic("WAI"));
    CB_CHECK(hu::is_branch("BSR") && hu::is_branch("BRA") && !hu::is_branch("BBR0"));
    CB_CHECK(hu::ends_flow("BRA") && !hu::ends_flow("BSR"));
});

CB_TEST("decompile.huc6280.every_opcode_disassembles_and_assembles_back_to_its_bytes", [] {
    for (int op = 0; op < 256; ++op) {
        if (!hu::opcode_table()[static_cast<std::size_t>(op)]) {
            CB_CHECK(!hu::decode(Bytes{static_cast<std::uint8_t>(op), 1, 2, 3, 4, 5, 6}, 0, 0xE000));
            continue;
        }
        const Bytes code = {static_cast<std::uint8_t>(op), 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
        const auto insn = hu::decode(code, 0, 0xE000);
        CB_CHECK(insn.has_value());
        if (!insn) continue;
        const std::string line = hu::text(*insn);
        Bytes built;
        try {
            built = hu::assemble(line, 0xE000);
        } catch (const cb::ParseError& e) {
            CB_CHECK_EQ(std::string(e.what()), std::string("assembles: ") + line);
        }
        CB_CHECK_EQ(built.size(), static_cast<std::size_t>(insn->size));
        CB_CHECK(std::equal(built.begin(), built.end(), code.begin()));
        CB_CHECK(!hu::decode(Bytes(code.begin(), code.begin() + insn->size - 1), 0, 0xE000));   // cut off by the end
    }
});

CB_TEST("decompile.huc6280.the_forms_beyond_the_65c02", [] {
    CB_CHECK_EQ(dis({0x73, 0x00, 0x40, 0x00, 0x20, 0x00, 0x01}), "TII $4000, $2000, $0100");
    CB_CHECK_EQ(dis({0xE3, 0x00, 0xE0, 0x02, 0x00, 0x00, 0x20}), "TIA $E000, $0002, $2000");
    CB_CHECK_EQ(dis({0x0F, 0x21, 0x05}, 0xE000), "BBR0 $21, $E008");
    CB_CHECK_EQ(dis({0xFF, 0x21, 0xFE}, 0xE000), "BBS7 $21, $E001");
    CB_CHECK_EQ(dis({0x83, 0x01, 0x20}), "TST #$01, $20");
    CB_CHECK_EQ(dis({0xB3, 0x80, 0x00, 0x30}), "TST #$80, $3000,x");
    CB_CHECK_EQ(dis({0x53, 0x04}), "TAM #$04");
    CB_CHECK_EQ(dis({0x03, 0x05}), "ST0 #$05");
    CB_CHECK_EQ(dis({0x44, 0x10}, 0xE000), "BSR $E012");
    CB_CHECK_EQ(dis({0xD4}), "CSH");
    CB_CHECK_EQ(dis({0xAD, 0x02, 0x00}), "LDA a:$0002");   // an absolute address below $100 keeps its size
    CB_CHECK_EQ(dis({0xA5, 0x20}, 0xE000, {}, {{0x20, "zp_20"}}), "LDA zp_20");
    CB_CHECK_EQ(dis({0x8D, 0x02, 0x00}, 0xE000, {{0x0002, "VDC_DATA_L"}}), "STA a:VDC_DATA_L");
    const auto bsr = hu::decode(Bytes{0x44, 0x10}, 0, 0xE000);
    CB_CHECK(bsr && bsr->target && *bsr->target == 0xE012);
    const auto block = hu::decode(Bytes{0x73, 1, 0, 2, 0, 3, 0}, 0, 0xE000);
    CB_CHECK(block && block->size == 7 && block->operands == 3 && block->operand[2] == 3);
});

CB_TEST("decompile.huc6280.assembling_names_and_errors", [] {
    const hu::NameLabels labels = {{"src", 0x4000}, {"dst", 0x2000}, {"zp_20", 0x20}, {"loop", 0xE000}, {"VDC_DATA_L", 0x0002}};
    CB_CHECK(hu::assemble("TII src, dst, $0100", 0xE000, labels) == (Bytes{0x73, 0x00, 0x40, 0x00, 0x20, 0x00, 0x01}));
    CB_CHECK(hu::assemble("BBR3 zp_20, loop", 0xE000, labels) == (Bytes{0x3F, 0x20, 0xFD}));
    CB_CHECK(hu::assemble("TST #$01, zp_20,x", 0xE000, labels) == (Bytes{0xA3, 0x01, 0x20}));
    CB_CHECK(hu::assemble("TST #$01, dst", 0xE000, labels) == (Bytes{0x93, 0x01, 0x00, 0x20}));
    CB_CHECK(hu::assemble("STA a:VDC_DATA_L ; the VDC", 0xE000, labels) == (Bytes{0x8D, 0x02, 0x00}));
    CB_CHECK(hu::assemble("  sxy", 0xE000, labels) == (Bytes{0x02}));
    CB_CHECK(hu::assemble("LDA (zp_20),y", 0xE000, labels) == (Bytes{0xB1, 0x20}));
    CB_CHECK_THROWS_AS(cb::ParseError, hu::assemble("TII src, dst", 0xE000, labels));
    CB_CHECK_THROWS_AS(cb::ParseError, hu::assemble("TAM src", 0xE000, labels));
    CB_CHECK_THROWS_AS(cb::ParseError, hu::assemble("BSR nowhere", 0xE000, labels));
    CB_CHECK_THROWS_AS(cb::ParseError, hu::assemble("BBR0 zp_20, $F000", 0xE000, labels));   // out of reach
    CB_CHECK_THROWS_AS(cb::ParseError, hu::assemble("WAI", 0xE000, labels));
});
