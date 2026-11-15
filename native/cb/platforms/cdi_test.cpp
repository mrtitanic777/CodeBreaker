// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/cdi.hpp"

using namespace cb::platforms;

CB_TEST("platforms.cdi.descriptor", [] {
    const Platform p = cdi();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "cdi");
    CB_CHECK_EQ(p.title, "Philips CD-i");
    CB_CHECK_EQ(p.family, "68000");
    CB_CHECK(p.lane.has_value() && *p.lane == "cdi");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
});
