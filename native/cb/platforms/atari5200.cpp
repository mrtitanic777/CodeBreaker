#include "cb/platforms/atari5200.hpp"

namespace cb::platforms {

Platform atari5200() {
    MachineSpec s;
    s.id = Id::Atari5200;
    s.title = "Atari 5200";
    s.family = "6502";
    s.container = "raw cartridge (.a52, .bin)";
    s.cpu = {"6502:LE:16:Sally:default", 8, Endian::Little, OwnDecoder::M6502, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P"};
    s.toolchain = "CodeBreaker's own assembler (dasm syntax), or dasm";
    return machine(s);
}

}  // namespace cb::platforms
