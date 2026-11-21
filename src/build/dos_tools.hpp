// build/dos_tools.hpp -- where the DOS tools of the period toolchains are set up.
//
// The tools themselves run on CodeBreaker's own DOS executive (build/native_dos, build/native_dos_runner); this module only
// names where they are.
#pragma once

#include <filesystem>
#include <optional>

#include "base/config.hpp"

namespace cb::build::dos_tools {

// The folder settings.json "masm" names when it holds BIN/MASM.EXE, else nullopt.
std::optional<std::filesystem::path> masm_root(const config::Config& config);

}  // namespace cb::build::dos_tools
