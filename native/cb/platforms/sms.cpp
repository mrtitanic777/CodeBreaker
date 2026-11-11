#include "cb/platforms/sms.hpp"

namespace cb::platforms {

Platform sms() {
    MachineSpec s;
    s.id = Id::Sms;
    s.title = "Master System / Game Gear";
    s.family = "z80";
    s.container = "cartridge image (.sms, .gg, .sg)";
    s.cpu = {"z80:LE:16:default", 8, Endian::Little, OwnDecoder::Z80, {"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}, "SP"};
    s.results = {"A", "F", "BC", "DE", "HL", "IX", "IY"};
    s.toolchain = "CodeBreaker's own assembler, or WLA-DX";
    return machine(s);
}

}  // namespace cb::platforms
