#include "cb/platforms/snes.hpp"

namespace cb::platforms {

Platform snes() {
    MachineSpec s;
    s.name = "snes";
    s.title = "Super Nintendo";
    s.family = "65816";
    s.container = "cartridge image (.sfc, .smc)";
    s.cpu = {"65816:LE:16:default", 16, Endian::Little, OwnDecoder::W65816, {"A", "X", "Y", "P", "S", "D", "DBR"}, "S"};
    s.results = {"A", "X", "Y", "P", "S", "D", "DBR"};
    s.toolchain = "CodeBreaker's own assembler (ca65 syntax), or cc65 (ca65 and ld65)";
    return machine(s);
}

}  // namespace cb::platforms
