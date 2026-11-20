// base/cache_command.hpp -- `codebreaker cache`: what the result cache holds, and looking after it.
//
//     codebreaker cache [--stats]     what is kept, kind by kind, and the cap
//     codebreaker cache --prune       results of old generations dropped, the cap held, then the listing
//     codebreaker cache --clear       everything kept, gone
//
// The cache itself is cache.hpp's; this is the command over it. The kinds the program's modules declare (which generation each is of
// now) are the program's to say: the command opens its own Cache over the folders of the configuration and has the environment
// declare them.
#pragma once

#include <functional>

#include "base/cache.hpp"
#include "base/config.hpp"
#include "core/cli.hpp"

namespace cb::base::cache {

struct CommandEnvironment {
    Config cache;                                   // where the caches are, and their caps
    std::function<void(Cache&)> declare_kinds;      // says who makes each kind (Cache::declare_kind); none: every kind is of one generation
};

// The cache folders and caps of a configuration: state/cache (cache_max_gb, default 2), and the build cache (the setting build_cache, else
// the main checkout's state/build_cache; build_cache_max_gb, default 6). CB_NO_CACHE turns the cache off.
Config settings_of(const cb::config::Config& configuration);

cli::Command command(const CommandEnvironment& environment);

}  // namespace cb::base::cache
