#include "cb/platforms/nes.hpp"

namespace cb::platforms {

Platform nes() {
    MachineSpec s;
    s.id = Id::Nes;
    s.title = "NES";
    s.family = "6502";
    s.container = "iNES (.nes)";
    s.cpu = {"6502:LE:16:NES:default", 8, Endian::Little, OwnDecoder::M6502, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P"};
    s.toolchain = "CodeBreaker's own assembler, or cc65";
    return machine(s);
}

}  // namespace cb::platforms
