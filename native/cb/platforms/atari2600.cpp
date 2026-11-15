#include "cb/platforms/atari2600.hpp"

namespace cb::platforms {

Platform atari2600() {
    MachineSpec s;
    s.id = Id::Atari2600;
    s.title = "Atari 2600";
    s.family = "6502";
    s.container = "raw cartridge (.a26, .bin)";
    s.cpu = {"6502:LE:16:6507:default", 8, Endian::Little, OwnDecoder::M6502, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P"};
    s.toolchain = "CodeBreaker's own assembler (dasm syntax), or dasm";
    return machine(s);
}

}  // namespace cb::platforms
