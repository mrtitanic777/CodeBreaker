#include "platforms/gba.hpp"

namespace cb::platforms {

Platform gba() {
    MachineSpec s;
    s.name = "gba";
    s.title = "Game Boy Advance";
    s.family = "arm";
    s.container = "cartridge ROM (.gba)";
    s.cpu = {"ARM:LE:32:v4t", 32, Endian::Little, OwnDecoder::Arm, {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"}, "SP"};
    s.results = {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
