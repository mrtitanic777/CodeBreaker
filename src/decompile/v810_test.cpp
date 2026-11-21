// cb-unity: alone
// The V810 decoder (the flow the source writers follow): sizes, how control leaves, where it goes. Reading and writing its text is the SLEIGH description's.
#include "decompile/v810.hpp"

#include "core/testing.hpp"

namespace cb::decompile::v810 {
CB_TEST("decompile.v810.flow", [] {
    const Bytes code = {0x00, 0xAC, 0x00, 0x00, 0x1F, 0x18, 0x01, 0x18, 0x00, 0x64, 0x00, 0x68, 0x00, 0x94};
    const auto jal = decode(code, 0, 0x100);
    CB_CHECK(jal && jal->flow == Flow::Call && jal->size == 4 && jal->target == 0x100u);
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
