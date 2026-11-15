// cb-unity: alone
#include "cb/platforms/jaguar.hpp"

#include <format>

namespace cb::platforms {

Platform jaguar() {
    std::vector<std::string> registers;   // D0..D7, A0..A7, SR
    for (int i = 0; i < 8; ++i) registers.push_back(std::format("D{}", i));
    for (int i = 0; i < 8; ++i) registers.push_back(std::format("A{}", i));
    registers.push_back("SR");

    MachineSpec s;
    s.id = Id::Jaguar;
    s.title = "Atari Jaguar";
    s.family = "68000";
    s.container = "cartridge (.j64, .jag, .rom) or COFF executable (.cof, .abs)";
    s.cpu = {"68000:BE:32:default", 32, Endian::Big, OwnDecoder::M68k, registers, "A7"};
    for (int i = 0; i < 8; ++i) s.results.push_back(std::format("D{}", i));
    for (int i = 0; i < 7; ++i) s.results.push_back(std::format("A{}", i));
    s.toolchain = "CodeBreaker's own assembler (Motorola syntax)";
    return machine(s);
}

}  // namespace cb::platforms
