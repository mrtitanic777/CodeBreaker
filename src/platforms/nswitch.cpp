// cb-unity: alone
#include "platforms/nswitch.hpp"

namespace cb::platforms {

Platform nswitch() {
    MachineSpec s;
    s.name = "switch";
    s.title = "Nintendo Switch";
    s.family = "AArch64";
    s.container = "homebrew executable (.nro), NSO executable (.nso), or the AArch64 ELF devkitA64 links (.elf)";
    s.cpu = {"AARCH64:LE:64:v8A", 64, Endian::Little, OwnDecoder::A64, {"X0", "X1", "X19", "X20", "X21", "X22", "X23", "X24", "X25", "X26", "X27", "X28", "SP"}, "SP"};
    s.results = {"X0", "X1", "X19", "X20", "X21", "X22", "X23", "X24", "X25", "X26", "X27", "X28", "SP"};
    s.toolchain = "CodeBreaker's own assembler (GNU as AArch64 syntax), or devkitA64's GNU as";
    return machine(s);
}

}  // namespace cb::platforms
