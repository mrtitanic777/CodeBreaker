// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/jaguar.hpp"

using namespace cb::platforms;

CB_TEST("platforms.jaguar.descriptor", [] {
    const Platform p = jaguar();
    CB_CHECK(p.id == Id::Jaguar);
    CB_CHECK_EQ(p.name, "jaguar");
    CB_CHECK_EQ(p.title, "Atari Jaguar");
    CB_CHECK_EQ(p.family, "68000");
    CB_CHECK(p.lane.has_value() && *p.lane == "jaguar");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
    CB_CHECK_EQ(p.convention.results.size(), 15u);
});
