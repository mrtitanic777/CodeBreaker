// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/nswitch.hpp"

using namespace cb::platforms;

CB_TEST("platforms.nswitch.descriptor", [] {
    const Platform p = nswitch();
    CB_CHECK(p.id == Id::Switch);
    CB_CHECK_EQ(p.name, "switch");
    CB_CHECK_EQ(p.title, "Nintendo Switch");
    CB_CHECK_EQ(p.family, "AArch64");
    CB_CHECK(p.lane.has_value() && *p.lane == "switch");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::A64);
    CB_CHECK_EQ(*p.cpu.bits, 64u);
    CB_CHECK(p.emulator.runs_code);
});
