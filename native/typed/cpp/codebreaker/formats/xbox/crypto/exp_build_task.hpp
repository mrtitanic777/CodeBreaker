// codebreaker/formats/xbox/crypto/exp_build_task.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.exp_build_task.
//
// Builds the HvxExpansionInstall payloads: assembles HVPP and Spoofy, signs them as generic and xbOnline
// expansions (devkit, test kit and retail) and writes each as a C++ byte array header.
//
// Typing notes:
//   * main() works in the current folder (the relative "Patches/...", "Output/..." and "Keys/..." paths); a
//     failing tool is build_lib's AssertionError, a missing file the runtime's FileNotFoundError, a missing
//     1BL key (with encrypt on) exp_signer's TypeError. It returns 0 (the `exit(main())` of the script's
//     __main__ block is left to the caller).
//   * `from .build_lib import *` and the other imports have no C++ counterpart: the headers are included.
#pragma once

#include <cstdint>

namespace codebreaker::formats::xbox::crypto::exp_build_task {

inline constexpr int64_t DEFAULT_EXP_ID = 0x48565050;
inline constexpr int64_t XBONLINE_EXP_ID = 0x48564050;

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::exp_build_task
