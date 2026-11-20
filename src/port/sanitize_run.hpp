// port/sanitize_run.hpp -- the port built with AddressSanitizer and run on the verify cases until it reports nothing.
//
// What only a run shows: an array written past its end where the old linker put the next variable, a table read before its start.
// The loop builds the port with `-fsanitize=address` (and `array-bounds` under clang), runs every case of CASES.json, hands what the
// sanitizer printed to `sanitize::mend`, and goes round again with the rules it made, up to six rounds. A port and cases as they were
// the last time the loop came out clean are not run again (`.sanitized.json` in the port's folder keeps what it found then, under
// a key of everything the build and its runs depend on).
#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "base/cache.hpp"
#include "port/sanitize.hpp"
#include "port/tool_build.hpp"
#include "port/tool_compilers.hpp"
#include "port/verify.hpp"

namespace cb::port::tool {

// Everything a sanitized build and its runs depend on: the port's files, the cases file and the inputs they name, the other passes'
// ports the cases run (by size and date), the compiler.
std::string sanitize_key(const std::filesystem::path& out, const std::filesystem::path& cases, const std::filesystem::path& cc);

// What the loop needs of the port it is run on.
struct SanitizeHooks {
    // Write the port's files into the folder (as they are now, with every rule made so far).
    std::function<void()> write;
    // The rules from what the sanitizer printed; what was mended.
    std::function<std::vector<sanitize::Mended>(const std::vector<sanitize::Report>&)> mend;
};

struct SanitizeSetup {
    cache::Cache* results = nullptr;             // where checks and objects are kept
    const Compilers* compilers = nullptr;
    std::string compiler;                        // the one the port is built for (gcc, clang, msvc): tried first, except msvc
    std::filesystem::path out;                   // the port's folder
    std::vector<std::string> sources;            // the files the build is given
    std::filesystem::path cases;                 // CASES.json
    const verify::Services* services = nullptr;  // what runs the cases
    int rounds = 6;
    int jobs = 4;
};

// The rounds' notes. A machine with no compiler that has the sanitizer gives one note saying so and runs nothing.
std::vector<std::string> sanitize_loop(const SanitizeSetup& setup, const SanitizeHooks& hooks);

}  // namespace cb::port::tool
