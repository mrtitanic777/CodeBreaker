#include "cb/platforms/saturn.hpp"

#include <format>

namespace cb::platforms {

Platform saturn() {
    std::vector<std::string> registers;   // R0-R15 and the SH-2's special registers
    for (int i = 0; i < 16; ++i) registers.push_back(std::format("R{}", i));
    for (const char* name : {"PR", "MACH", "MACL", "SR", "GBR", "VBR"}) registers.push_back(name);

    MachineSpec s;
    s.id = Id::Saturn;
    s.title = "Sega Saturn";
    s.family = "superh";
    s.container = "disc image (.iso, .bin with a .cue sheet): IP.BIN and the first program";
    s.cpu = {"SuperH:BE:32:SH-2", 32, Endian::Big, OwnDecoder::Sh2, registers, "R15"};
    s.results = {"R0", "R1", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "PR", "MACH", "MACL", "SR", "GBR"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
