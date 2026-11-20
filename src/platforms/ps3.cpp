// cb-unity: alone (file-local names that other files in a batch also use)
#include "platforms/ps3.hpp"

#include <format>

namespace cb::platforms {

namespace {

// r1, r3, r4, f1 and the registers a function must hand back unchanged.
std::vector<std::string> ps3_results() {
    std::vector<std::string> r = {"r1", "r3", "r4", "f1"};
    for (int i = 14; i < 32; ++i) r.push_back(std::format("r{}", i));
    return r;
}

std::vector<std::string> ps3_registers() {
    std::vector<std::string> r;
    for (int i = 0; i < 32; ++i) r.push_back(std::format("r{}", i));
    for (int i = 0; i < 32; ++i) r.push_back(std::format("f{}", i));
    return r;
}

}  // namespace

Platform ps3() {
    MachineSpec s;
    s.name = "ps3";
    s.title = "PlayStation 3";
    s.family = "powerpc";
    s.container = "PSL1GHT ELF (.elf), or an unencrypted SELF holding one";
    s.cpu = {"PowerPC:BE:64:default", 64, Endian::Big, OwnDecoder::PowerPc, ps3_registers(), "r1"};
    s.results = ps3_results();
    s.toolchain = "CodeBreaker's own assembler, or GNU as (PSL1GHT's powerpc64-ps3-elf)";
    return machine(s);
}

}  // namespace cb::platforms
