// The Atari Lynx platform record.
#include "cb/platforms/lynx.hpp"

#include "cb/core/testing.hpp"

CB_TEST("platforms.lynx.descriptor", [] {
    const cb::platforms::Platform p = cb::platforms::lynx();
    CB_CHECK(p.id == cb::platforms::Id::Lynx);
    CB_CHECK_EQ(p.name, "lynx");
    CB_CHECK_EQ(p.title, "Atari Lynx");
    CB_CHECK_EQ(p.family, "6502");
    CB_CHECK(p.lane.has_value() && *p.lane == "lynx");
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(std::holds_alternative<cb::platforms::OwnDecoder>(p.cpu.decoder));
    CB_CHECK(std::get<cb::platforms::OwnDecoder>(p.cpu.decoder) == cb::platforms::OwnDecoder::M6502);
    CB_CHECK_EQ(p.cpu.registers.size(), 5u);
});
