// source/console_snes.hpp -- the Super Nintendo on the console writer (console_snes.cpp, config/platforms/snes.json): what a function's address
// says beyond its place. A function's address in the result is the 24-bit canonical address with the state it starts in above it, as the lane
// runs it (engine/machine_snes).
#pragma once

#include <cstdint>

namespace cb::source::console_writer::snes_code {

inline constexpr std::uint32_t kStartsWith16BitAccumulator = 1u << 24;   // M clear
inline constexpr std::uint32_t kStartsWith16BitIndex = 1u << 25;         // X clear
inline constexpr std::uint32_t kStartsNative = 1u << 26;
inline constexpr std::uint32_t kAddressMask = 0xFFFFFF;

}  // namespace cb::source::console_writer::snes_code
