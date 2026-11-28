// The Atari 2600 and 7800 platform records, and the registry telling their files apart from the other 6502 machines'.
#include "core/testing.hpp"
#include "platforms/package.hpp"

using namespace cb::platforms;

namespace {

void check_atari(const Platform& p, Id id, std::string_view name, std::string_view title, std::string_view container) {
    CB_CHECK(p.id == id);
    CB_CHECK_EQ(p.name, std::string(name));
    CB_CHECK_EQ(p.title, std::string(title));
    CB_CHECK_EQ(p.family, "6502");
    CB_CHECK_EQ(p.loader.container, std::string(container));
    CB_CHECK(p.lane.has_value() && *p.lane == name);
    CB_CHECK(p.source == SourceLanguage::Asm);
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M6502);
    CB_CHECK_EQ(p.cpu.registers.size(), 5u);
    CB_CHECK_EQ(p.cpu.stack, "S");
    CB_CHECK_EQ(p.convention.results.size(), 4u);   // A X Y P
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler (dasm syntax), or dasm");
}

}  // namespace

CB_TEST("platforms.atari2600.descriptor", [] { check_atari(described("atari2600"), Id::Machine, "atari2600", "Atari 2600", "raw cartridge (.a26, .bin)"); });

CB_TEST("platforms.atari7800.descriptor", [] { check_atari(described("atari7800"), Id::Machine, "atari7800", "Atari 7800", "A78 cartridge (.a78)"); });

CB_TEST("platforms.atari.registry_tells_the_6502_machines_apart", [] {
    // (a registered machine is recognised by the probe its lane gives: engine/lane_registry.hpp)
    Platform a7800 = described("atari7800"), a2600 = described("atari2600"), nes_platform = described("nes");
    nes_platform.recognise = [](const std::filesystem::path& p) { return p.extension() == ".nes"; };
    a7800.recognise = [](const std::filesystem::path& p) { return p.extension() == ".a78"; };
    a2600.recognise = [](const std::filesystem::path& p) { return p.extension() == ".a26"; };
    Registry registry;
    registry.add(a7800);
    registry.add(a2600);
    registry.add(nes_platform);
    CB_CHECK_EQ(registry.identify("game.a26")->name, "atari2600");
    CB_CHECK_EQ(registry.identify("game.a78")->name, "atari7800");
    CB_CHECK_EQ(registry.identify("game.nes")->name, "nes");
    CB_CHECK(registry.identify("game.bin") == nullptr);
});
