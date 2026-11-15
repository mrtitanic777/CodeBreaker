// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/wonderswan.hpp"

using namespace cb::platforms;

CB_TEST("platforms.wonderswan.descriptor", [] {
    const Platform p = wonderswan();
    CB_CHECK(p.id == Id::WonderSwan);
    CB_CHECK_EQ(p.name, "wonderswan");
    CB_CHECK_EQ(p.title, "WonderSwan / WonderSwan Color");
    CB_CHECK_EQ(p.family, "x86");
    CB_CHECK(p.lane.has_value() && *p.lane == "wonderswan");
    CB_CHECK(std::holds_alternative<CapstoneDecoder>(p.cpu.decoder));
    CB_CHECK(p.emulator.runs_code);
});
