// cb-unity: alone
#include "core/testing.hpp"
#include "platforms/package.hpp"

using namespace cb::platforms;

CB_TEST("platforms.vectrex.descriptor", [] {
    const Platform p = described("vectrex");
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "vectrex");
    CB_CHECK_EQ(p.title, "Vectrex");
    CB_CHECK_EQ(p.family, "6809");
    CB_CHECK(p.lane.has_value() && *p.lane == "vectrex");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M6809);
    CB_CHECK(p.emulator.runs_code);
});
