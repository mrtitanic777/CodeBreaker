#include "cb/platforms/neogeo.hpp"

#include <format>

namespace cb::platforms {

Platform neogeo() {
    std::vector<std::string> registers;   // D0..D7, A0..A7, SR
    for (int i = 0; i < 8; ++i) registers.push_back(std::format("D{}", i));
    for (int i = 0; i < 8; ++i) registers.push_back(std::format("A{}", i));
    registers.push_back("SR");

    MachineSpec s;
    s.name = "neogeo";
    s.title = "Neo Geo (AES / MVS)";
    s.family = "68000";
    s.container = "cartridge P-ROM (.p1, .bin, word-swapped or not) or .neo file";
    s.cpu = {"68000:BE:32:default", 32, Endian::Big, OwnDecoder::M68k, registers, "A7"};
    for (int i = 0; i < 8; ++i) s.results.push_back(std::format("D{}", i));
    for (int i = 0; i < 7; ++i) s.results.push_back(std::format("A{}", i));   // A7 is the stack pointer: not a result
    s.toolchain = "CodeBreaker's own assembler";
    return machine(s);
}

}  // namespace cb::platforms
