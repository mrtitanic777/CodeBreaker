// cb/build/msvc152.hpp -- compile 16-bit C with the PERIOD compiler (Microsoft Visual C++ 1.52c) under DOSBox-X.
//
// The foundation of byte-exact C matching (matching decompilation): to prove a reconstructed C function is
// right, compile it with the SAME compiler and flags that built retail Win98 and check the emitted bytes against
// the shipped bytes. That requires the real compiler -- Microsoft Visual C++ 1.52c, the Win16 (MS C8) toolchain --
// which is a 16-bit DOS program (no NTVDM on x64), so it runs under the vendored DOSBox-X, exactly like the MASM
// and link16 drivers.
//
// This module just RUNS the compiler: given C source (+ flags), it produces the .OBJ and the `/Fc` assembly
// listing. Extracting the code bytes from the OBJ, diffing against the target, and the permuter that searches
// flags/source for a byte-match are the layers built on top (see docs / COMPILER_RE.md's codegen oracle).
//
//     Compiled c = msvc152::compile_c(runner, tools, source, "/AM /Ox");
//     // c.obj, c.cod (the listing), c.log; c.work is the folder it ran in (the caller cleans it up)
//
// The `cc16` command (`cc16 --selftest`, `cc16 foo.c --flags "/AM /Ox"`) is declared with the app's commands;
// it compiles kSelfTestSource or the file and prints report().
#pragma once

#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "cb/build/dos_run.hpp"

namespace cb::build::msvc152 {

// Where the emulator and the compiler are (settings.json: dosbox_x, msvc152).
struct Tools {
    std::filesystem::path dosbox_x;   // the DOSBox-X executable
    std::filesystem::path msvc_root;  // Microsoft C/C++ 8.00c: BIN/CL.EXE, INCLUDE, LIB
};

// What a compile made; each part is present only when that file was made.
struct Compiled {
    std::optional<std::filesystem::path> work;   // the folder it ran in (empty when the emulator did not run)
    std::optional<Bytes> obj;
    std::optional<std::string> cod;   // the /Fc listing (Latin-1 text, as UTF-8)
    std::optional<std::string> log;   // the raw compiler log
};

// Two functions to compile when only the toolchain is being checked.
inline constexpr std::string_view kSelfTestSource =
    "int add2(int a, int b) { return a + b; }\nlong mulshift(long x) { return (x * 3) >> 2; }\n";

// Compile `source` with MSVC 1.52c under DOSBox-X. `stem` names the DOS files (1-8 letters, digits or
// underscores); `flags` are CL's switches and may hold none of the characters DOS reads as redirection or a
// command separator (UsageError). NotFoundError when the emulator or CL.EXE is missing.
Compiled compile_c(const DosRunner& runner, const Tools& tools, std::string_view source,
                   std::string_view flags = "/AM /c", std::string_view stem = "t",
                   std::chrono::seconds timeout = std::chrono::seconds(180));

// What the `cc16` command prints for a compile: the first 3000 bytes of the listing, the object's size, or the
// compiler log when no object was made, and the work folder.
std::string report(const Compiled& compiled);

}  // namespace cb::build::msvc152
