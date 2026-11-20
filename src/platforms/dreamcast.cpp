#include "platforms/dreamcast.hpp"

#include <format>

namespace cb::platforms {

Platform dreamcast() {
    std::vector<std::string> registers;   // R0-R15, the SH-4's special registers and its FPU's
    for (int i = 0; i < 16; ++i) registers.push_back(std::format("R{}", i));
    for (const char* name : {"PR", "MACH", "MACL", "SR", "GBR", "VBR", "SSR", "SPC", "SGR", "DBR", "FPSCR", "FPUL"}) registers.push_back(name);
    for (int i = 0; i < 16; ++i) registers.push_back(std::format("FR{}", i));

    MachineSpec s;
    s.name = "dreamcast";
    s.title = "Sega Dreamcast";
    s.family = "superh";
    s.container = "disc image (.gdi with its track files, .cdi, .iso, .bin): IP.BIN and the first program";
    s.cpu = {"SuperH:LE:32:SH-4", 32, Endian::Little, OwnDecoder::Sh4, registers, "R15"};
    s.results = {"R0", "R1", "FR0", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "FR12", "FR13", "FR14", "FR15", "PR", "MACH", "MACL", "SR", "GBR", "FPSCR"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
