// cb-unity: alone
#include "core/testing.hpp"
#include "platforms/vb.hpp"

using namespace cb::platforms;

CB_TEST("platforms.vb.descriptor", [] {
    const Platform p = vb();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "vb");
    CB_CHECK_EQ(p.title, "Nintendo Virtual Boy");
    CB_CHECK_EQ(p.family, "V810");
    CB_CHECK(p.lane.has_value() && *p.lane == "vb");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::V810);
    CB_CHECK(p.emulator.runs_code);
});
