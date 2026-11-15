// cb-unity: alone
#include "cb/platforms/vb.hpp"

#include <format>

namespace cb::platforms {

Platform vb() {
    std::vector<std::string> registers;   // r1..r31 (r0 is zero), PSW
    for (int i = 1; i < 32; ++i) registers.push_back(std::format("r{}", i));
    registers.push_back("PSW");

    MachineSpec s;
    s.id = Id::Vb;
    s.title = "Nintendo Virtual Boy";
    s.family = "V810";
    s.container = "cartridge (.vb, .vboy)";
    s.cpu = {"V810:LE:32:default", 32, Endian::Little, OwnDecoder::V810, registers, "r3"};
    s.results = registers;
    s.toolchain = "CodeBreaker's own assembler (GNU V810 syntax), or the GNU assembler for the V810";
    return machine(s);
}

}  // namespace cb::platforms
