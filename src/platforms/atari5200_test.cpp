// cb-unity: alone
#include "core/testing.hpp"
#include "platforms/atari5200.hpp"

using namespace cb::platforms;

CB_TEST("platforms.atari5200.descriptor", [] {
    const Platform p = atari5200();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "atari5200");
    CB_CHECK_EQ(p.title, "Atari 5200");
    CB_CHECK_EQ(p.family, "6502");
    CB_CHECK(p.lane.has_value() && *p.lane == "atari5200");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M6502);
    CB_CHECK(p.emulator.runs_code);
});
