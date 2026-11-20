// cb-unity: alone
#include "platforms/o2.hpp"

namespace cb::platforms {

Platform o2() {
    MachineSpec s;
    s.name = "o2";
    s.title = "Magnavox Odyssey 2";
    s.family = "8048";
    s.container = "cartridge (.o2, up to 3 KB at $0400)";
    s.cpu = {"8048:LE:8:default", 8, Endian::Little, OwnDecoder::I8048, {"A", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "PSW", "P1", "P2"}, "SP"};
    s.results = {"A", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "PSW", "P1", "P2"};
    s.toolchain = "CodeBreaker's own assembler (8048 syntax)";
    return machine(s);
}

}  // namespace cb::platforms
