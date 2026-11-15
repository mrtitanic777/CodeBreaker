// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/intv.hpp"

using namespace cb::platforms;

CB_TEST("platforms.intv.descriptor", [] {
    const Platform p = intv();
    CB_CHECK(p.id == Id::Intv);
    CB_CHECK_EQ(p.name, "intv");
    CB_CHECK_EQ(p.title, "Mattel Intellivision");
    CB_CHECK_EQ(p.family, "CP1610");
    CB_CHECK(p.lane.has_value() && *p.lane == "intv");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Cp1610);
    CB_CHECK(p.emulator.runs_code);
});
