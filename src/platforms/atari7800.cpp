#include "platforms/atari7800.hpp"

namespace cb::platforms {

Platform atari7800() {
    MachineSpec s;
    s.name = "atari7800";
    s.title = "Atari 7800";
    s.family = "6502";
    s.container = "A78 cartridge (.a78)";
    s.cpu = {"6502:LE:16:Sally:default", 8, Endian::Little, OwnDecoder::M6502, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P"};
    s.toolchain = "CodeBreaker's own assembler (dasm syntax), or dasm";
    return machine(s);
}

}  // namespace cb::platforms
