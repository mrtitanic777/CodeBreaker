// cb/source/pipeline_match16.hpp -- `match` and `score16` for a 16-bit Windows module: the production assembly of verify/authored/match16
// (internal to source/pipeline_*.cpp).
//
// What match16::run is given -- the toolchain (Microsoft C 6.0a under DOSBox-X, or Visual C++ 1.52c's C 8.00c, which runs on Windows itself), the
// original's functions with the places a loader fills in masked, each function said several ways (the passes of build/msc6) -- is made here from the
// settings, the lane kit's decompiler and the module's file. `Match16Run` is everything the run leaves for `score16`, which compiles the same C
// with OpenWatcom and sets the three beside the original.
#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/source/pipeline_world.hpp"
#include "cb/verify/authored/match16.hpp"

namespace cb::source::pipeline {

struct Match16Options {
    std::optional<std::string> flags;      // every segment is built with these switches (by the first compiler)
    std::optional<std::string> compiler;   // "msc6" or "msvc152": the only compiler; nothing is remembered
    std::size_t first = 0;                 // only the first N functions (0: all)
    bool choose = false;                   // find the settings again though an earlier run remembered them
    bool sdk = false;                      // say the Windows calls through WINDOWS.H's prototypes
    bool fold = true;                      // fold the decompiler's temporaries back (and join far parameters, split the frame)
    bool wine = false;                     // type the imports as Wine's .spec files do
    int search = 0;                        // rounds of rewrites of the last bytes of the near misses
};

struct Match16Run {
    verify::authored::match16::Result result;
    std::vector<verify::authored::match16::Function> functions;   // the functions judged, in the module's order
    std::string header;                                           // the module's header as the compiler was given it (before the SDK's)
    std::unique_ptr<verify::authored::match16::Toolchain> toolchain;
};

// Runs the match of `module` (an NE file). `log` hears each line the run says as it goes. UsageError when Microsoft C is not set up.
Match16Run run_match16(World& world, const std::filesystem::path& module, const Match16Options& options, const verify::authored::match16::Log& log);

// `match <16-bit module> ...`: runs it and prints what the Python prints.
int match16_command(World& world, const std::filesystem::path& module, const cli::Args& args);

}  // namespace cb::source::pipeline
