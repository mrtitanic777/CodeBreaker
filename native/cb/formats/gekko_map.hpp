// cb/formats/gekko_map.hpp -- the memory-mapped hardware the Nintendo PowerPC consoles' programs touch, by name.
//
//     gekko_map::hardware_name(Console::GameCube, 0xCC006000)    "DI_STATUS"
//     gekko_map::hardware_name(Console::GameCube, 0xCC002072)    "VI_2072"      (a block whose register the table does not name)
//     gekko_map::devices(Console::Wii)                           the address ranges the interpreter logs instead of treating as memory
//
// The GameCube's hardware is a set of register blocks at 0xCC000000 (the uncached view of physical 0x0C000000): the command processor,
// the pixel engine, video, the processor interface, memory, the DSP, the disc, serial (controllers), external devices (memory cards, the
// real-time clock) and audio; plus the write-gather pipe at 0xCC008000 that feeds the graphics FIFO. The Wii keeps all of these and adds
// the inter-processor mailbox with its ARM (0xCD000000) and the Hollywood registers (0xCD800000). The Wii U's PowerPC cores are not
// given hardware by the programs that run on them (CafeOS owns it), so only the Latte window the system libraries map is named, by its
// offset. A register is named by the width the console uses (the video block's 16-bit registers by their own offsets); an address in a
// block without a name of its own is `<BLOCK>_<offset in hex>`.
//
// Sources: Yet Another Gamecube Documentation (the register layout) and libogc's headers (the names the homebrew uses).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/formats/ppc_program.hpp"

namespace cb::formats::gekko_map {

// The name of the register at `address` (any of its views: the uncached 0xCCxxxxxx and cached physical 0x0Cxxxxxx addresses), or nullopt
// when the address is not hardware of the console.
std::optional<std::string> hardware_name(ppc_program::Console console, std::uint32_t address);

// The ranges [lo, hi) of hardware registers: an access to one is not an access to memory.
struct Range {
    std::uint32_t lo = 0, hi = 0;
};
std::vector<Range> devices(ppc_program::Console console);

}  // namespace cb::formats::gekko_map
