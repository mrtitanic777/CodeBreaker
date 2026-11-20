#include "platforms/cpc.hpp"

namespace cb::platforms {

Platform cpc() {
    MachineSpec s;
    s.name = "cpc";
    s.title = "Amstrad CPC";
    s.family = "z80";
    s.container = "disk (.dsk), snapshot (.sna), AMSDOS binary (.bin)";
    s.cpu = {"z80:LE:16:default", 8, Endian::Little, OwnDecoder::Z80, {"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}, "SP"};
    s.results = {"A", "F", "BC", "DE", "HL", "IX", "IY"};
    s.toolchain = "CodeBreaker's own assembler, or sjasmplus / pasmo";
    return machine(s);
}

}  // namespace cb::platforms
