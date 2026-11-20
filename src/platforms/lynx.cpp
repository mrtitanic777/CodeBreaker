#include "platforms/lynx.hpp"

namespace cb::platforms {

Platform lynx() {
    MachineSpec s;
    s.name = "lynx";
    s.title = "Atari Lynx";
    s.family = "6502";
    s.container = "Lynx cartridge (.lnx) or BS93 object (.o)";
    s.cpu = {"6502:LE:16:default", 8, Endian::Little, OwnDecoder::M6502, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P", "S"};
    s.toolchain = "CodeBreaker's own assembler, or cc65";
    return machine(s);
}

}  // namespace cb::platforms
