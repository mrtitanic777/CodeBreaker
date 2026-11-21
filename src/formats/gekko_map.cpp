// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/gekko_map.hpp"

#include <format>

#include "knowledge/package.hpp"

namespace cb::formats::gekko_map {

namespace {

using ppc_program::Console;

struct Reg {
    std::uint32_t offset;
    std::string name;
};

// A register block: [base, base + size), its prefix for unnamed registers, and the registers it names.
struct Block {
    std::uint32_t base, size;
    std::string prefix;
    std::vector<Reg> regs;
};

// A platform's blocks as its description gives them (config/knowledge/<platform>/registers.json: the regions of the table `blocks`, and the
// registers of that table in the block that holds them), in the file's order.
std::vector<Block> blocks_of(std::string_view platform) {
    knowledge::Knowledge& kb = knowledge::standard();
    std::vector<Block> blocks;
    for (const knowledge::HardwareRegion& region : kb.hardware_regions(platform, "blocks")) blocks.push_back({region.address, region.size, region.name, {}});
    for (const knowledge::HardwareRegister& r : kb.hardware_registers(platform, "blocks"))
        for (Block& b : blocks)
            if (r.address >= b.base && r.address - b.base < b.size) {
                b.regs.push_back({r.address - b.base, r.name});
                break;
            }
    return blocks;
}

// The GameCube's blocks (and the Wii's, which keeps them), at their uncached addresses.
const std::vector<Block>& gamecube_blocks() {
    static const std::vector<Block> blocks = blocks_of("gamecube");
    return blocks;
}

// What the Wii adds: the mailbox with its ARM processor (IPC) and the Hollywood registers.
const std::vector<Block>& wii_blocks() {
    static const std::vector<Block> blocks = blocks_of("wii");
    return blocks;
}

// What the Wii U's programs see of the Latte chip: a window named by offset.
const std::vector<Block>& wiiu_blocks() {
    static const std::vector<Block> blocks = blocks_of("wiiu");
    return blocks;
}

// The cached physical view (0x0Cxxxxxx, 0x0Dxxxxxx) of a block is the same register as its uncached 0xCCxxxxxx, 0xCDxxxxxx view.
std::uint32_t uncached(std::uint32_t a) { return (a >> 24) == 0x0C || (a >> 24) == 0x0D ? a | 0xC0000000u : a; }

std::optional<std::string> find_in(const std::vector<Block>& blocks, std::uint32_t a) {
    for (const Block& b : blocks) {
        if (a < b.base || a - b.base >= b.size) continue;
        const std::uint32_t offset = a - b.base;
        for (const Reg& r : b.regs)
            if (r.offset == offset) return std::string(r.name);
        return std::format("{}_{:X}", b.prefix, offset);
    }
    return std::nullopt;
}

}  // namespace

std::optional<std::string> hardware_name(Console console, std::uint32_t address) {
    if (console == Console::WiiU) return find_in(wiiu_blocks(), address);
    const std::uint32_t a = uncached(address);
    if (console == Console::Wii) {
        if (auto n = find_in(wii_blocks(), a)) return n;
    }
    return find_in(gamecube_blocks(), a);
}

std::vector<Range> devices(Console console) {
    switch (console) {
        case Console::GameCube: return {{0xCC000000, 0xCC010000}};
        case Console::Wii: return {{0xCC000000, 0xCC010000}, {0xCD000000, 0xCD010000}, {0xCD800000, 0xCD900000}};
        case Console::WiiU: return {{0x0D000000, 0x0E000000}};
    }
    return {};
}

}  // namespace cb::formats::gekko_map
