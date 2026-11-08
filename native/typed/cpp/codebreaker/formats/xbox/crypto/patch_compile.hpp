// codebreaker/formats/xbox/crypto/patch_compile.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.patch_compile.
//
// Assembles the RGLoader and syscall-zero patches with the Free60 tools (through build_lib).
//
// Typing notes:
//   * main() assembles with relative paths ("Patches/...", "Output/..."), so it works from the folder that
//     holds them; a failed tool is build_lib's AssertionError.
//   * not ported: the `__name__ == "__main__"` block (`exit(main())`): the caller exits with main()'s result.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::patch_compile {

extern const py::str PATCH_DIR;   // "Patches"

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::patch_compile
