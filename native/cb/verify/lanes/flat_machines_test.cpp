// Tests of the two machines the flat Z80 lane serves (verify/lanes/msx, verify/lanes/zx): their kind names, the lane's version, and
// the ROM each says it has. Both machines have 16 KB of ROM at $0000, and the kind names are the platforms' (platforms/msx, platforms/zx).
#include "cb/verify/lanes/msx.hpp"
#include "cb/verify/lanes/zx.hpp"

#include "cb/core/testing.hpp"

namespace {
namespace msx = cb::verify::lanes::msx;
namespace zx = cb::verify::lanes::zx;
}  // namespace

CB_TEST("verify.lanes.msx", [] {
    CB_CHECK_EQ(msx::kKind, std::string_view("msx"));
    CB_CHECK_EQ(msx::kRev, 2);
    CB_CHECK_EQ(msx::kRomHigh - msx::kRomLow, std::uint32_t{0x4000});   // 16 KB
    CB_CHECK(msx::in_rom(0x0000));
    CB_CHECK(msx::in_rom(0x3FFF));
    CB_CHECK(!msx::in_rom(0x4000));    // the first byte past the ROM is the program's
    CB_CHECK(!msx::in_rom(0xFFFF));
});

CB_TEST("verify.lanes.zx", [] {
    CB_CHECK_EQ(zx::kKind, std::string_view("zx"));
    CB_CHECK_EQ(zx::kRev, 2);
    CB_CHECK_EQ(zx::kRomHigh - zx::kRomLow, std::uint32_t{0x4000});
    CB_CHECK(zx::in_rom(0x0000));
    CB_CHECK(zx::in_rom(0x3FFF));
    CB_CHECK(!zx::in_rom(0x4000));
    CB_CHECK(!zx::in_rom(0x8000));
});

CB_TEST("verify.lanes.flat_machines_agree", [] {
    // the two machines differ only in their name: one lane serves both
    CB_CHECK_EQ(msx::kRev, zx::kRev);
    CB_CHECK_EQ(msx::kRomLow, zx::kRomLow);
    CB_CHECK_EQ(msx::kRomHigh, zx::kRomHigh);
    CB_CHECK(msx::kKind != zx::kKind);
});
