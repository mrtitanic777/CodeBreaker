// cb-unity: alone
// The M6809 decoder (the flow the source writers follow): sizes, how control leaves, where it goes. Reading and writing its text is the SLEIGH description's.
#include "decompile/m6809.hpp"

#include "core/testing.hpp"

namespace cb::decompile::m6809 {
CB_TEST("decompile.m6809.flow", [] {
    const Bytes code = {0xBD, 0xF1, 0x92, 0x26, 0xFB, 0x39, 0x6E, 0x84, 0x35, 0x80};
    const auto jsr = decode(code, 0, 0x100);
    CB_CHECK(jsr && jsr->flow == Flow::Call && jsr->target == 0xF192u && jsr->size == 3);
    const auto bne = decode(code, 3, 0x103);
    CB_CHECK(bne && bne->flow == Flow::Branch && bne->target == 0x100u);
    CB_CHECK(decode(code, 5, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 6, 0)->flow == Flow::Indirect);   // JMP ,X
    CB_CHECK(decode(code, 8, 0)->flow == Flow::Return);     // PULS PC
    CB_CHECK(!decode(Bytes{0x01}, 0, 0).has_value());       // undefined
    CB_CHECK(!decode(Bytes{0xBD, 0xF1}, 0, 0).has_value()); // cut off
    CB_CHECK(!decode(Bytes{0xA6, 0x87}, 0, 0).has_value()); // an indexed postbyte the 6809 does not define
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
