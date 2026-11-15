// cb-unity: alone
#include "cb/core/testing.hpp"
#include "cb/platforms/ngp.hpp"

using namespace cb::platforms;

CB_TEST("platforms.ngp.descriptor", [] {
    const Platform p = ngp();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "ngp");
    CB_CHECK_EQ(p.title, "SNK Neo Geo Pocket / Pocket Color");
    CB_CHECK_EQ(p.family, "TLCS-900/H");
    CB_CHECK(p.lane.has_value() && *p.lane == "ngp");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Tlcs900);
    CB_CHECK(p.emulator.runs_code);
});
