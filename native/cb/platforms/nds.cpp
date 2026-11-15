#include "cb/platforms/nds.hpp"

namespace cb::platforms {

Platform nds() {
    MachineSpec s;
    s.name = "nds";
    s.title = "Nintendo DS";
    s.family = "arm";
    s.container = "cartridge ROM (.nds)";
    s.cpu = {"ARM:LE:32:v5t", 32, Endian::Little, OwnDecoder::Arm, {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"}, "SP"};
    s.results = {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
