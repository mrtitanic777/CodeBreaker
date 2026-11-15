// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/vectrex.hpp"

using namespace cb::platforms;

CB_TEST("platforms.vectrex.descriptor", [] {
    const Platform p = vectrex();
    CB_CHECK(p.id == Id::Vectrex);
    CB_CHECK_EQ(p.name, "vectrex");
    CB_CHECK_EQ(p.title, "Vectrex");
    CB_CHECK_EQ(p.family, "6809");
    CB_CHECK(p.lane.has_value() && *p.lane == "vectrex");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M6809);
    CB_CHECK(p.emulator.runs_code);
});
