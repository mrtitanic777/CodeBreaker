// cb-unity: alone
#include "core/testing.hpp"
#include "platforms/jaguar.hpp"

using namespace cb::platforms;

CB_TEST("platforms.jaguar.descriptor", [] {
    const Platform p = jaguar();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "jaguar");
    CB_CHECK_EQ(p.title, "Atari Jaguar");
    CB_CHECK_EQ(p.family, "68000");
    CB_CHECK(p.lane.has_value() && *p.lane == "jaguar");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
    CB_CHECK_EQ(p.convention.results.size(), 15u);
});
