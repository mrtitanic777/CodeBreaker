// cb/verify/lanes/msx.hpp -- what is particular to reconstructing an MSX cartridge: the BIOS ROM at $0000.
//
// The MSX's reader (formats/msx) and writer (source/msx_writer) are its platform's (platforms/msx). The lane that runs a flat Z80
// program (z80_flat: one block at one address, judged by running each function in the original's memory and in the rebuilt one's)
// is the ZX Spectrum's as well; each machine gives it three things, which are all this module is: the kind's name, the lane's
// version, and where the machine's ROM is. The ROM is not run: each of its bytes reads as RET, so a call into it returns at once,
// the same way in both runs. (Bumping `kRev` makes results an older lane filed be judged again.)
#pragma once

#include <cstdint>
#include <string_view>

namespace cb::verify::lanes::msx {

inline constexpr std::string_view kKind = "msx";
inline constexpr int kRev = 2;
// The addresses the machine's ROM occupies, [kRomLow, kRomHigh).
inline constexpr std::uint32_t kRomLow = 0x0000;
inline constexpr std::uint32_t kRomHigh = 0x4000;

// Whether an address is in the machine's ROM.
inline constexpr bool in_rom(std::uint32_t address) { return address >= kRomLow && address < kRomHigh; }

}  // namespace cb::verify::lanes::msx
