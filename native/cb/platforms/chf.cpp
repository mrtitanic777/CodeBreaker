// cb-unity: alone
#include "cb/platforms/chf.hpp"

namespace cb::platforms {

Platform chf() {
    MachineSpec s;
    s.id = Id::Chf;
    s.title = "Fairchild Channel F";
    s.family = "F8";
    s.container = "cartridge (.chf, or .bin starting $55 $2B; at $0800)";
    s.cpu = {"F8:BE:16:default", 8, Endian::Big, OwnDecoder::F8, {"A", "W", "ISAR", "DC0", "DC1", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11"}, "ISAR"};
    s.results = {"A", "W", "ISAR", "DC0", "DC1", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11"};
    s.toolchain = "CodeBreaker's own assembler (F8 syntax)";
    return machine(s);
}

}  // namespace cb::platforms
