// cb-unity: alone
#include "platforms/vectrex.hpp"

namespace cb::platforms {

Platform vectrex() {
    MachineSpec s;
    s.name = "vectrex";
    s.title = "Vectrex";
    s.family = "6809";
    s.container = "cartridge (.vec, .bin)";
    s.cpu = {"6809:BE:16:default", 8, Endian::Big, OwnDecoder::M6809, {"A", "B", "X", "Y", "U", "S", "DP", "CC"}, "S"};
    s.results = {"A", "B", "X", "Y", "U", "DP", "CC"};
    s.toolchain = "CodeBreaker's own assembler (6809 syntax), or lwasm";
    return machine(s);
}

}  // namespace cb::platforms
