// cb/exact/hooks_lattice.hpp -- answers a pass of the Lattice-lineage compilers is given at a place (profile hooks).
//
// cmp_test: the code generator of MS C 1.04's own executables stood between the two shipped ones. When it compared
// two longs it loaded both words of the left one first only if neither operand was reached through a pointer (type
// flag 2000h); the shipped MC2.EXE 1.04 does so for every long - the test in the function that generates comparisons
// (image 0x4D79). There is no room to patch a longer test in, so the pass is told the outcome at 0x4D7F: go on at
// 0x4D85 (load) or 0x4DA1 (do not).
#pragma once

#include <cstdint>
#include <functional>

namespace cb::exact::hooks_lattice {

// The profile hook's view of the emulated machine's memory: the word at segment:offset.
using WordReader = std::function<std::uint16_t(std::uint32_t segment, std::uint32_t offset)>;

inline constexpr std::uint32_t kLoadBoth = 0x4D85;   // load both words of the left long first
inline constexpr std::uint32_t kSkipLoad = 0x4DA1;   // do not

// Where the pass goes on at 0x4D7F: `bp` is the generating function's frame, `ss`/`ds` its segments.
std::uint32_t cmp_test(const WordReader& read_word, std::uint32_t bp, std::uint32_t ss, std::uint32_t ds);

}  // namespace cb::exact::hooks_lattice
