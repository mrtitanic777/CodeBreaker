// cb-unity: alone
#include "cb/platforms/n3ds.hpp"

namespace cb::platforms {

Platform n3ds() {
    MachineSpec s;
    s.name = "n3ds";
    s.title = "Nintendo 3DS";
    s.family = "arm";
    s.container = "3DSX homebrew executable (.3dsx), or the ARM ELF it is linked from (.elf)";
    s.cpu = {"ARM:LE:32:v6", 32, Endian::Little, OwnDecoder::Arm, {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"}, "SP"};
    s.results = {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"};
    s.toolchain = "CodeBreaker's own assembler, or devkitARM's GNU as";
    return machine(s);
}

}  // namespace cb::platforms
