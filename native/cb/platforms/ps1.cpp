#include "cb/platforms/ps1.hpp"

namespace cb::platforms {

Platform ps1() {
    MachineSpec s;
    s.id = Id::Ps1;
    s.title = "PlayStation";
    s.family = "mips";
    s.container = "PS-X EXE (.exe, .psx)";
    s.cpu = {"MIPS:LE:32:default", 32, Endian::Little, OwnDecoder::Mips,
             {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO"}, "SP"};
    s.results = {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
