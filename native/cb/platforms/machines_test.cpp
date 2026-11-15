// Tests of the console descriptors built with `machine()`: nes, sms, gb, genesis, ps1. Each is checked against the values below.
#include "cb/platforms/gb.hpp"
#include "cb/platforms/genesis.hpp"
#include "cb/platforms/ps1.hpp"
#include "cb/platforms/nes.hpp"
#include "cb/platforms/sms.hpp"

#include "cb/core/testing.hpp"

// The descriptors are data only (the readers, runners, assemblers and source writers are the layers' own tables keyed
// on Id); recognising a file is the registry's probe (`is_nes`, `is_sms`, `is_gb`, `is_genesis`).

using namespace cb::platforms;

namespace {

using Strings = std::vector<std::string>;

void check_machine(const Platform& p, Id id, const char* name, const char* title, const char* family, const char* container) {
    CB_CHECK(p.id == id);
    CB_CHECK_EQ(p.name, name);
    CB_CHECK_EQ(p.title, title);
    CB_CHECK_EQ(p.family, family);
    CB_CHECK_EQ(p.loader.container, container);
    CB_CHECK_EQ(*p.lane, name);   // a console is its own lane
    CB_CHECK(*p.source == SourceLanguage::Asm);
    CB_CHECK(p.steps == Steps::Lanes);
    CB_CHECK(*p.convention.order == CallOrder::Registers);
    CB_CHECK(std::holds_alternative<InRegisters>(p.convention.arguments));
    CB_CHECK(*p.convention.pops == Pops::Caller);
    CB_CHECK((p.convention.names == Strings{"registers"}));
    CB_CHECK(p.emulator.engine == Engine::OwnInterpreter);
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(*p.compiler.language == Language::Asm);
    CB_CHECK(std::holds_alternative<OwnDecoder>(p.cpu.decoder));
}

}  // namespace

CB_TEST("platforms.nes.descriptor", [] {
    const Platform p = nes();
    check_machine(p, Id::Nes, "nes", "NES", "6502", "iNES (.nes)");
    CB_CHECK_EQ(p.cpu.language, "6502:LE:16:NES:default");
    CB_CHECK_EQ(*p.cpu.bits, 8u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M6502);
    CB_CHECK((p.cpu.registers == Strings{"A", "X", "Y", "P", "S"}));
    CB_CHECK_EQ(p.cpu.stack, "S");
    CB_CHECK((p.convention.results == Strings{"A", "X", "Y", "P"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or cc65");
});

CB_TEST("platforms.sms.descriptor", [] {
    const Platform p = sms();
    check_machine(p, Id::Sms, "sms", "Master System / Game Gear", "z80", "cartridge image (.sms, .gg, .sg)");
    CB_CHECK_EQ(p.cpu.language, "z80:LE:16:default");
    CB_CHECK_EQ(*p.cpu.bits, 8u);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Z80);
    CB_CHECK((p.cpu.registers == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}));
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or WLA-DX");
});

CB_TEST("platforms.gb.descriptor", [] {
    const Platform p = gb();
    check_machine(p, Id::Gb, "gb", "Game Boy / Game Boy Color", "sm83", "cartridge image (.gb, .gbc)");
    CB_CHECK_EQ(p.cpu.language, "sm83:LE:16:default");
    CB_CHECK_EQ(*p.cpu.bits, 8u);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Sm83);
    CB_CHECK((p.cpu.registers == Strings{"A", "F", "BC", "DE", "HL", "SP"}));
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == Strings{"A", "F", "BC", "DE", "HL"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or RGBDS");
});

CB_TEST("platforms.genesis.descriptor", [] {
    const Platform p = genesis();
    check_machine(p, Id::Genesis, "genesis", "Genesis / Mega Drive", "68000", "cartridge image (.md, .bin, .gen, .smd)");
    CB_CHECK_EQ(p.cpu.language, "68000:BE:32:default");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Big);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
    CB_CHECK_EQ(p.cpu.registers.size(), 17u);   // D0..D7, A0..A7, SR
    CB_CHECK_EQ(p.cpu.registers[0], "D0");
    CB_CHECK_EQ(p.cpu.registers[7], "D7");
    CB_CHECK_EQ(p.cpu.registers[8], "A0");
    CB_CHECK_EQ(p.cpu.registers[15], "A7");
    CB_CHECK_EQ(p.cpu.registers[16], "SR");
    CB_CHECK_EQ(p.cpu.stack, "A7");
    CB_CHECK_EQ(p.convention.results.size(), 15u);   // D0..D7, A0..A6
    CB_CHECK_EQ(p.convention.results.front(), "D0");
    CB_CHECK_EQ(p.convention.results.back(), "A6");
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler");
});

CB_TEST("platforms.ps1.descriptor", [] {
    const Platform p = ps1();
    check_machine(p, Id::Ps1, "ps1", "PlayStation", "mips", "PS-X EXE (.exe, .psx)");
    CB_CHECK_EQ(p.cpu.language, "MIPS:LE:32:default");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Mips);
    CB_CHECK_EQ(p.cpu.registers.size(), 16u);   // V0 V1 S0..S7 GP SP FP RA HI LO
    CB_CHECK_EQ(p.cpu.registers[0], "V0");
    CB_CHECK_EQ(p.cpu.registers[13], "RA");
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == p.cpu.registers));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or GNU as");
});

CB_TEST("platforms.consoles.registry_identifies_each_by_its_probe", [] {
    Probes probes;
    probes.is_nes = [](const std::filesystem::path& p) { return p.extension() == ".nes"; };
    probes.is_sms = [](const std::filesystem::path& p) { return p.extension() == ".sms"; };
    probes.is_gb = [](const std::filesystem::path& p) { return p.extension() == ".gb"; };
    probes.is_genesis = [](const std::filesystem::path& p) { return p.extension() == ".md"; };
    probes.is_ps1 = [](const std::filesystem::path& p) { return p.extension() == ".psx"; };
    Registry registry(probes);
    registry.add(genesis());
    registry.add(ps1());
    registry.add(gb());
    registry.add(sms());
    registry.add(nes());
    CB_CHECK_EQ(registry.identify("g.nes")->name, "nes");   // kOrder, not the order they were added
    CB_CHECK_EQ(registry.identify("g.sms")->name, "sms");
    CB_CHECK_EQ(registry.identify("g.gb")->name, "gb");
    CB_CHECK_EQ(registry.identify("g.md")->name, "genesis");
    CB_CHECK_EQ(registry.identify("g.psx")->name, "ps1");
    CB_CHECK(registry.identify("g.bin") == nullptr);
    CB_CHECK_EQ(registry.all().front().name, "nes");
    CB_CHECK_EQ(registry.runners().size(), 5u);
});
