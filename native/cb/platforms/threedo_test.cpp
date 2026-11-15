// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/threedo.hpp"

using namespace cb::platforms;

CB_TEST("platforms.threedo.descriptor", [] {
    const Platform p = threedo();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "threedo");
    CB_CHECK_EQ(p.title, "3DO");
    CB_CHECK_EQ(p.family, "arm");
    CB_CHECK(p.lane.has_value() && *p.lane == "threedo");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Arm);
    CB_CHECK(p.cpu.endian == Endian::Big);
});
