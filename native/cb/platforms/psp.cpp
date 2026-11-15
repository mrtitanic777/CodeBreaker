#include "cb/platforms/psp.hpp"

namespace cb::platforms {

Platform psp() {
    MachineSpec s;
    s.id = Id::Psp;
    s.title = "PSP";
    s.family = "mips";
    s.container = "ELF (.elf), module (.prx), EBOOT.PBP";
    s.cpu = {"MIPS:LE:32:default", 32, Endian::Little, OwnDecoder::Mips,
             {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"}, "SP"};
    s.results = {"V0", "V1", "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "GP", "SP", "FP", "RA", "HI", "LO", "F0"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as (ps2sdk / pspsdk)";
    return machine(s);
}

}  // namespace cb::platforms
