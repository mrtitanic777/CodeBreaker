// cb-unity: alone
#include "platforms/ngp.hpp"

namespace cb::platforms {

Platform ngp() {
    // the registers a result names: the current bank's long registers, the common ones, SR (the flags and the register bank)
    const std::vector<std::string> registers = {"XWA", "XBC", "XDE", "XHL", "XIX", "XIY", "XIZ", "XSP", "SR"};
    MachineSpec s;
    s.name = "ngp";
    s.title = "SNK Neo Geo Pocket / Pocket Color";
    s.family = "TLCS-900/H";
    s.container = "cartridge (.ngp, .ngc, .npc)";
    s.cpu = {"TLCS900H:LE:32:default", 32, Endian::Little, OwnDecoder::Tlcs900, registers, "XSP"};
    s.results = registers;
    s.toolchain = "CodeBreaker's own assembler (Toshiba TLCS-900/H syntax)";
    return machine(s);
}

}  // namespace cb::platforms
