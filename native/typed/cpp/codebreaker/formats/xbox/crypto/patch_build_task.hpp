// codebreaker/formats/xbox/crypto/patch_build_task.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.patch_build_task.
//
// Compiles the patches (patch_compile) and checks them (patch_checker).
//
// Typing notes:
//   * main() works in the current folder (the relative "Patches/..." and "Output/..." paths of its callees);
//     what a failing tool or a short patch file raises (AssertionError, struct.error ...) propagates.
//   * not ported: the `__name__ == "__main__"` block (`main()`), and the commented-out se_patcher step
//     (kept in the .cpp as a comment); the unused `se_patcher` import has no C++ counterpart.
#pragma once

namespace codebreaker::formats::xbox::crypto::patch_build_task {

void main();

}  // namespace codebreaker::formats::xbox::crypto::patch_build_task
