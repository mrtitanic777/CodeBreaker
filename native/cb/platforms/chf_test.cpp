// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/chf.hpp"

using namespace cb::platforms;

CB_TEST("platforms.chf.descriptor", [] {
    const Platform p = chf();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "chf");
    CB_CHECK_EQ(p.title, "Fairchild Channel F");
    CB_CHECK_EQ(p.family, "F8");
    CB_CHECK(p.lane.has_value() && *p.lane == "chf");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::F8);
    CB_CHECK(p.emulator.runs_code);
});
