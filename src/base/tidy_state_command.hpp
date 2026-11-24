// base/tidy_state_command.hpp -- `codebreaker tidy-state`: how big the state folder is, part by part, and what of it is stale, gone.
//
//     codebreaker tidy-state                 the sizes, and the stale removed
//     codebreaker tidy-state --dry           the sizes, and what would be removed
//     codebreaker tidy-state --stray         also remove what no part of CodeBreaker writes
//     codebreaker tidy-state --keep-builds   leave Converted/*/build/ alone
//
// The rules are tidy_state.hpp's; this is the command over them, with the result cache's own clean-up (cache.hpp's prune and
// its cap) as the cache rule. It needs the settings and nothing else: the state and Converted folders and the cache's caps all
// come from the configuration, and the cache is told no kinds (it keeps, of each, the generation written to most recently --
// what the code of now writes -- which is also how files of an earlier implementation are looked after).
#pragma once

#include "base/cache_command.hpp"
#include "base/config.hpp"
#include "base/tidy_state.hpp"
#include "core/cli.hpp"

namespace cb::base::tidy_state {

struct CommandEnvironment {
    Layout layout;                          // the state and Converted folders
    cache::CommandEnvironment cache;        // where the caches are and their caps (for the cache rule)
};

// The environment of a configuration: its state folder, its Converted folder (the setting "converted", else <checkout>/Converted),
// and the cache folders and caps of cache::settings_of.
CommandEnvironment environment_of(const cb::config::Config& configuration);

// What tidy_state::run asks of the cache, over a cache opened on the environment's folders when the rule runs (a dry run opens none).
CacheHooks cache_hooks(const cache::CommandEnvironment& environment);

cli::Command command(const CommandEnvironment& environment);

}  // namespace cb::base::tidy_state
