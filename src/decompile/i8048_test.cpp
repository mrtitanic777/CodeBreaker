// cb-unity: alone
// The I8048 decoder (the flow the source writers follow): sizes, how control leaves, where it goes. Reading and writing its text is the SLEIGH description's.
#include "decompile/i8048.hpp"

#include "core/testing.hpp"

namespace cb::decompile::i8048 {
CB_TEST("decompile.i8048.flow", [] {
    const Bytes code = {0x94, 0x30, 0x83, 0xB3, 0x96, 0x05, 0x84, 0x50, 0x23};
    const auto call = decode(code, 0, 0x400);
    CB_CHECK(call && call->flow == Flow::Call && call->target == 0x430u && call->size == 2);
    CB_CHECK(decode(code, 2, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 3, 0)->flow == Flow::Indirect);
    const auto jnz = decode(code, 4, 0x4FF);   // on the last byte of its page: the next page
    CB_CHECK(jnz && jnz->flow == Flow::Branch && jnz->target == 0x505u);
    const auto jmp = decode(code, 6, 0x800);   // the memory bank is that of the address
    CB_CHECK(jmp && jmp->flow == Flow::Jump && jmp->target == 0xC50u);
    CB_CHECK(!decode(code, 8, 0).has_value());              // cut off
    CB_CHECK(!decode(Bytes{0x01}, 0, 0).has_value());       // undefined
});
}  // namespace cb::decompile::i8048
