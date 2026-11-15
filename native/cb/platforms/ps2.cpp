#include "cb/platforms/ps2.hpp"

namespace cb::platforms {

Platform ps2() {
    MachineSpec s;
    s.id = Id::Ps2;
    s.title = "PlayStation 2";
    s.family = "mips";
    s.container = "ELF (.elf), disc image (.iso)";
    s.cpu = {"MIPS:LE:64:default", 64, Endian::Little, OwnDecoder::Mips,
             {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"}, "SP"};
    s.results = {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as (ps2sdk / pspsdk)";
    return machine(s);
}

}  // namespace cb::platforms
