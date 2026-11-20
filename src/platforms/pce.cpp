#include "platforms/pce.hpp"

namespace cb::platforms {

Platform pce() {
    MachineSpec s;
    s.name = "pce";
    s.title = "PC Engine / TurboGrafx-16";
    s.family = "6502";
    s.container = "HuCard image (.pce, .tg16)";
    s.cpu = {"HuC6280:LE:16:default", 8, Endian::Little, OwnDecoder::Huc6280, {"A", "X", "Y", "P", "S"}, "S"};
    s.results = {"A", "X", "Y", "P", "S"};
    s.toolchain = "CodeBreaker's own assembler (ca65 syntax), or cc65 (ca65 and ld65)";
    return machine(s);
}

}  // namespace cb::platforms
