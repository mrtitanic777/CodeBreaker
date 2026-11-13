// cb/base/settings.hpp -- `codebreaker settings`: where the things CodeBreaker does not contain are found on this machine.
//
//     codebreaker settings
//
// Prints settings.json's place, then every setting, its value and whether it is there:
//
//       ok = found, !! = set but not there, -- = not set (optional unless a command asks for it)
//
// The marks are for the settings that name a file or a folder (the compilers, the tool folders, the corpus); the others show their
// value alone. The values are what cb::config::Config answers (settings.json, the defaults, the CB_<KEY> overrides): the command asks
// nothing else.
#pragma once

#include <string>

#include "cb/base/config.hpp"
#include "cb/core/cli.hpp"

namespace cb::base::settings {

struct Environment {
    const config::Config* config = nullptr;   // required; must outlive the command
};

// The text the command prints.
std::string describe(const config::Config& config);

cli::Command command(const Environment& environment);

}  // namespace cb::base::settings
