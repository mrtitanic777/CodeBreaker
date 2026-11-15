// cb-unity: alone
#include "cb/platforms/intv.hpp"

namespace cb::platforms {

Platform intv() {
    MachineSpec s;
    s.id = Id::Intv;
    s.title = "Mattel Intellivision";
    s.family = "CP1610";
    s.container = "cartridge (.rom Intellicart / jzIntv, raw .int)";
    s.cpu = {"CP1610:BE:16:default", 16, Endian::Big, OwnDecoder::Cp1610, {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7"}, "R6"};
    s.results = {"R0", "R1", "R2", "R3", "R4", "R6", "S", "Z", "OV", "C"};
    s.toolchain = "CodeBreaker's own assembler (as1600 syntax), or as1600";
    return machine(s);
}

}  // namespace cb::platforms
