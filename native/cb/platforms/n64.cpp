#include "cb/platforms/n64.hpp"

namespace cb::platforms {

Platform n64() {
    MachineSpec s;
    s.name = "n64";
    s.title = "Nintendo 64";
    s.family = "mips";
    s.container = "cartridge ROM (.z64, .v64, .n64)";
    s.cpu = {"MIPS:BE:64:default", 64, Endian::Big, OwnDecoder::Mips,
             {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"}, "SP"};
    s.results = {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
