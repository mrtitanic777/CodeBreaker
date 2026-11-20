#include "platforms/sega32x.hpp"

#include <format>

namespace cb::platforms {

namespace {

// The SH-2's registers: R0-R15, the procedure register, the multiply-accumulate registers, the status, global and vector base registers.
std::vector<std::string> sh2_registers() {
    std::vector<std::string> r;
    for (int i = 0; i < 16; ++i) r.push_back(std::format("R{}", i));
    for (const char* name : {"PR", "MACH", "MACL", "SR", "GBR", "VBR"}) r.push_back(name);
    return r;
}

}  // namespace

Platform sega32x() {
    MachineSpec s;
    s.name = "sega32x";
    s.title = "Sega 32X";
    s.family = "superh";
    s.container = "32X cartridge (.32x): a Genesis ROM with the SH-2 programs of the add-on in it";
    s.cpu = {"SuperH:BE:32:SH-2", 32, Endian::Big, OwnDecoder::Sh2, sh2_registers(), "R15"};
    s.results = {"R0", "R1", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "PR", "MACH", "MACL", "SR", "GBR"};
    s.toolchain = "CodeBreaker's own assembler, or GNU as";
    return machine(s);
}

}  // namespace cb::platforms
