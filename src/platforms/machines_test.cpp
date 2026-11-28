// Tests of the console descriptors built with `machine()`: nes, sms, gb, genesis, ps1. Each is checked against the values below.
#include "platforms/segacd.hpp"

#include "core/testing.hpp"

// The descriptors are data only (the readers, runners, assemblers and source writers are the layers' own tables keyed
// on the platform's name); recognising a file is the probe a lane registers with its descriptor (engine/lane_registry.hpp).

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
    const Platform p = described("nes");
    check_machine(p, Id::Machine, "nes", "NES", "6502", "iNES (.nes)");
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
    const Platform p = described("sms");
    check_machine(p, Id::Machine, "sms", "Master System / Game Gear", "z80", "cartridge image (.sms, .gg, .sg, .sc)");
    CB_CHECK_EQ(p.cpu.language, "z80:LE:16:default");
    CB_CHECK_EQ(*p.cpu.bits, 8u);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Z80);
    CB_CHECK((p.cpu.registers == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}));
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or WLA-DX");
});

CB_TEST("platforms.gb.descriptor", [] {
    const Platform p = described("gb");
    check_machine(p, Id::Machine, "gb", "Game Boy / Game Boy Color", "sm83", "cartridge image (.gb, .gbc)");
    CB_CHECK_EQ(p.cpu.language, "sm83:LE:16:default");
    CB_CHECK_EQ(*p.cpu.bits, 8u);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Sm83);
    CB_CHECK((p.cpu.registers == Strings{"A", "F", "BC", "DE", "HL", "SP"}));
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == Strings{"A", "F", "BC", "DE", "HL"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or RGBDS");
});

CB_TEST("platforms.genesis.descriptor", [] {
    const Platform p = described("genesis");
    check_machine(p, Id::Machine, "genesis", "Genesis / Mega Drive", "68000", "cartridge image (.md, .bin, .gen, .smd)");
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
    const Platform p = described("ps1");
    check_machine(p, Id::Machine, "ps1", "PlayStation", "mips", "PS-X EXE (.exe, .psx)");
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
    // each descriptor with the probe its lane would give it: the files of one extension
    const auto taking = [](Platform p, const char* extension) {
        p.recognise = [extension](const std::filesystem::path& path) { return path.extension() == extension; };
        return p;
    };
    Registry registry;
    registry.add(taking(described("nes"), ".nes"));
    registry.add(taking(described("sms"), ".sms"));
    registry.add(taking(described("gb"), ".gb"));
    registry.add(taking(described("genesis"), ".md"));
    registry.add(taking(described("ps1"), ".psx"));
    registry.add(taking(described("gba"), ".gba"));
    CB_CHECK_EQ(registry.identify("g.nes")->name, "nes");   // (the registered machines are tried in the order they were added)
    CB_CHECK_EQ(registry.identify("g.sms")->name, "sms");
    CB_CHECK_EQ(registry.identify("g.gb")->name, "gb");
    CB_CHECK_EQ(registry.identify("g.md")->name, "genesis");
    CB_CHECK_EQ(registry.identify("g.psx")->name, "ps1");
    CB_CHECK_EQ(registry.identify("g.gba")->name, "gba");
    CB_CHECK(registry.identify("g.bin") == nullptr);
    CB_CHECK_EQ(registry.all().front().name, "nes");
    CB_CHECK_EQ(registry.runners().size(), 6u);
});

CB_TEST("platforms.gba.descriptor", [] {
    const Platform p = described("gba");
    check_machine(p, Id::Machine, "gba", "Game Boy Advance", "arm", "cartridge ROM (.gba)");
    CB_CHECK_EQ(p.cpu.language, "ARM:LE:32:v4t");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Arm);
    CB_CHECK_EQ(p.cpu.registers.size(), 11u);   // R0 R1 R4..R11 SP
    CB_CHECK_EQ(p.cpu.registers[0], "R0");
    CB_CHECK_EQ(p.cpu.registers[10], "SP");
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK((p.convention.results == p.cpu.registers));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or GNU as");
});

CB_TEST("platforms.coleco.descriptor", [] {
    const Platform p = described("coleco");
    check_machine(p, Id::Machine, "coleco", "ColecoVision", "z80", "cartridge image (.col)");
    CB_CHECK_EQ(p.cpu.language, "z80:LE:16:default");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Z80);
    CB_CHECK((p.convention.results == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY"}));
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or sjasmplus / pasmo");
});

CB_TEST("platforms.cpc.descriptor", [] {
    const Platform p = described("cpc");
    check_machine(p, Id::Machine, "cpc", "Amstrad CPC", "z80", "disk (.dsk), snapshot (.sna), AMSDOS binary (.bin)");
    CB_CHECK_EQ(p.cpu.language, "z80:LE:16:default");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Z80);
    CB_CHECK((p.cpu.registers == Strings{"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}));
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or sjasmplus / pasmo");
});

CB_TEST("platforms.neogeo.descriptor", [] {
    const Platform p = described("neogeo");
    check_machine(p, Id::Machine, "neogeo", "Neo Geo (AES / MVS)", "68000", "cartridge P-ROM (.p1, .bin, word-swapped or not) or .neo file");
    CB_CHECK_EQ(p.cpu.language, "68000:BE:32:default");
    CB_CHECK(*p.cpu.endian == Endian::Big);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
    CB_CHECK_EQ(p.cpu.registers.size(), 17u);   // D0..D7, A0..A7, SR
    CB_CHECK_EQ(p.cpu.stack, "A7");
    CB_CHECK_EQ(p.convention.results.size(), 15u);   // D0..D7, A0..A6
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler");
});

CB_TEST("platforms.segacd.descriptor", [] {
    const Platform p = segacd();
    check_machine(p, Id::Machine, "segacd", "Sega CD / Mega-CD", "68000", "disc image boot sectors (.iso, .bin; 2048 or 2352 bytes a sector)");
    CB_CHECK_EQ(p.cpu.language, "68000:BE:32:default");
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::M68k);
    CB_CHECK_EQ(p.cpu.registers.size(), 17u);
    CB_CHECK_EQ(p.convention.results.front(), "D0");
    CB_CHECK_EQ(p.convention.results.back(), "A6");
});
