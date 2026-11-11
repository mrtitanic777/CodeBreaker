#include "cb/platforms/msx.hpp"

namespace cb::platforms {

Platform msx() {
    MachineSpec s;
    s.id = Id::Msx;
    s.title = "MSX";
    s.family = "z80";
    s.container = "cartridge image (.rom)";
    s.cpu = {"z80:LE:16:default", 8, Endian::Little, OwnDecoder::Z80, {"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}, "SP"};
    s.results = {"A", "F", "BC", "DE", "HL", "IX", "IY"};
    s.toolchain = "CodeBreaker's own assembler, or sjasmplus / pasmo";
    return machine(s);
}

}  // namespace cb::platforms
