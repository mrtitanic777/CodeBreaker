#include "cb/platforms/gb.hpp"

namespace cb::platforms {

Platform gb() {
    MachineSpec s;
    s.id = Id::Gb;
    s.title = "Game Boy / Game Boy Color";
    s.family = "sm83";
    s.container = "cartridge image (.gb, .gbc)";
    s.cpu = {"sm83:LE:16:default", 8, Endian::Little, OwnDecoder::Sm83, {"A", "F", "BC", "DE", "HL", "SP"}, "SP"};
    s.results = {"A", "F", "BC", "DE", "HL"};
    s.toolchain = "CodeBreaker's own assembler, or RGBDS";
    return machine(s);
}

}  // namespace cb::platforms
