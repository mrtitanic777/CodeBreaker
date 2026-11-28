// The SH-2 machines' descriptors: the Sega 32X and the Saturn, recorded field by field.
#include "core/testing.hpp"
#include "platforms/sega32x.hpp"

namespace cb::platforms {
namespace {

void check_sh2_machine(const Platform& p, Id id, const std::string& name, const std::string& title, const std::string& container) {
    CB_CHECK(p.id == id);
    CB_CHECK_EQ(p.name, name);
    CB_CHECK_EQ(p.title, title);
    CB_CHECK_EQ(p.family, "superh");
    CB_CHECK(p.source == SourceLanguage::Asm);
    CB_CHECK_EQ(*p.lane, name);
    CB_CHECK(p.steps == Steps::Lanes);
    CB_CHECK_EQ(p.loader.container, container);
    CB_CHECK_EQ(p.cpu.language, "SuperH:BE:32:SH-2");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Big);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Sh2);
    CB_CHECK_EQ(p.cpu.registers.size(), 22u);   // R0-R15, PR, MACH, MACL, SR, GBR, VBR
    CB_CHECK_EQ(p.cpu.registers[15], "R15");
    CB_CHECK_EQ(p.cpu.stack, "R15");
    CB_CHECK(std::holds_alternative<InRegisters>(p.convention.arguments));
    CB_CHECK((p.convention.results == std::vector<std::string>{"R0", "R1", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "PR", "MACH", "MACL", "SR", "GBR"}));
    CB_CHECK(p.emulator.engine == Engine::OwnInterpreter);
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(*p.compiler.language == Language::Asm);
    CB_CHECK_EQ(p.compiler.toolchain, "CodeBreaker's own assembler, or GNU as");
}

CB_TEST("platforms.sega32x.descriptor", [] {
    check_sh2_machine(sega32x(), Id::Machine, "sega32x", "Sega 32X", "32X cartridge (.32x): a Genesis ROM with the SH-2 programs of the add-on in it");
});

CB_TEST("platforms.saturn.descriptor", [] {
    check_sh2_machine(described("saturn"), Id::Machine, "saturn", "Sega Saturn", "disc image (.iso, .bin with a .cue sheet): IP.BIN and the first program");
});

}  // namespace
}  // namespace cb::platforms
