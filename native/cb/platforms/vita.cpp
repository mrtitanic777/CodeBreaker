// cb-unity: alone
#include "cb/platforms/vita.hpp"

namespace cb::platforms {

Platform vita() {
    MachineSpec s;
    s.name = "vita";
    s.title = "PlayStation Vita";
    s.family = "arm";
    s.container = "unencrypted SELF (eboot.bin of a .vpk) or the ELF it holds (.velf)";
    s.cpu = {"ARM:LE:32:v7", 32, Endian::Little, OwnDecoder::Arm, {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"}, "SP"};
    s.results = {"R0", "R1", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "SP"};
    s.toolchain = "CodeBreaker's own assembler, or VitaSDK's GNU as";
    return machine(s);
}

}  // namespace cb::platforms
