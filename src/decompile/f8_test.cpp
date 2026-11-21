// cb-unity: alone
// The F8 decoder (the flow the source writers follow): sizes, how control leaves, where it goes. Reading and writing its text is the SLEIGH description's.
#include "decompile/f8.hpp"

#include "core/testing.hpp"

namespace cb::decompile::f8 {
CB_TEST("decompile.f8.flow", [] {
    const Bytes code = {0x28, 0x09, 0x00, 0x1C, 0x29, 0x08, 0x10, 0x84, 0xFF, 0x0D, 0x2A, 0x28, 0x00};
    const auto pi = decode(code, 0, 0x800);
    CB_CHECK(pi && pi->flow == Flow::Call && pi->size == 3 && pi->target == 0x900u);
    CB_CHECK(decode(code, 3, 0)->flow == Flow::Return);
    CB_CHECK(decode(code, 4, 0)->flow == Flow::Jump);
    const auto bt = decode(code, 7, 0x820);
    CB_CHECK(bt && bt->flow == Flow::Branch && bt->target == 0x820u);   // the displacement counts from its own address
    CB_CHECK(decode(code, 9, 0)->flow == Flow::Indirect);               // LR P0,Q
    const auto dci = decode(code, 10, 0);
    CB_CHECK(dci && dci->flow == Flow::None && dci->value == 0x2800u);
    CB_CHECK(!decode(code, 11, 0).has_value());                          // cut off
    CB_CHECK(!decode(Bytes{0x2D}, 0, 0).has_value());                    // undefined
    CB_CHECK(!decode(Bytes{0x4F}, 0, 0).has_value());                    // R15
    CB_CHECK(decode(Bytes{0x80, 0x05}, 0, 0)->flow == Flow::None);       // BT 0 never branches
});
}  // namespace cb::decompile::f8
