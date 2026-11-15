// cb-unity: alone
#include "cb/platforms/threedo.hpp"

namespace cb::platforms {

Platform threedo() {
    MachineSpec s;
    s.id = Id::ThreeDo;
    s.title = "3DO";
    s.family = "arm";
    s.container = "Opera disc image (.iso) or AIF executable (LaunchMe)";
    s.cpu = {"ARM:BE:32:v3", 32, Endian::Big, OwnDecoder::Arm, {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"}, "SP"};
    s.results = {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
