// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/platforms/wiiu.hpp"

#include <format>

namespace cb::platforms {

namespace {

// r1, r3, r4, f1 and the registers a function must hand back unchanged.
std::vector<std::string> wiiu_results() {
    std::vector<std::string> r = {"r1", "r3", "r4", "f1"};
    for (int i = 14; i < 32; ++i) r.push_back(std::format("r{}", i));
    return r;
}

std::vector<std::string> wiiu_registers() {
    std::vector<std::string> r;
    for (int i = 0; i < 32; ++i) r.push_back(std::format("r{}", i));
    for (int i = 0; i < 32; ++i) r.push_back(std::format("f{}", i));
    return r;
}

}  // namespace

Platform wiiu() {
    MachineSpec s;
    s.name = "wiiu";
    s.title = "Nintendo Wii U";
    s.family = "powerpc";
    s.container = "RPX executable (.rpx), RPL library (.rpl): an ELF with compressed sections";
    s.cpu = {"PowerPC:BE:32:default", 32, Endian::Big, OwnDecoder::PowerPc, wiiu_registers(), "r1"};
    s.results = wiiu_results();
    s.toolchain = "CodeBreaker's own assembler, or GNU as (wut)";
    return machine(s);
}

}  // namespace cb::platforms
