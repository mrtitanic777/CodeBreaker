// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/o2.hpp"

using namespace cb::platforms;

CB_TEST("platforms.o2.descriptor", [] {
    const Platform p = o2();
    CB_CHECK(p.id == Id::O2);
    CB_CHECK_EQ(p.name, "o2");
    CB_CHECK_EQ(p.title, "Magnavox Odyssey 2");
    CB_CHECK_EQ(p.family, "8048");
    CB_CHECK(p.lane.has_value() && *p.lane == "o2");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::I8048);
    CB_CHECK(p.emulator.runs_code);
});
