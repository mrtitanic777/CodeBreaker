// base/tidy_state_command.cpp -- see tidy_state_command.hpp.
#include "base/tidy_state_command.hpp"

#include <memory>

namespace cb::base::tidy_state {

CommandEnvironment environment_of(const cb::config::Config& configuration) {
    CommandEnvironment e;
    e.layout.state = configuration.state();
    e.layout.converted = configuration.path("converted").value_or(configuration.checkout_root() / "Converted");
    e.cache.cache = cache::settings_of(configuration);
    return e;
}

CacheHooks cache_hooks(const cache::CommandEnvironment& environment) {
    // One cache for the prune and for the cap: opened by the first call that needs it, closed with the hooks.
    auto open = std::make_shared<std::unique_ptr<cache::Cache>>();
    const auto get = [open, environment]() -> cache::Cache& {
        if (!*open) {
            *open = std::make_unique<cache::Cache>(environment.cache);
            if (environment.declare_kinds) environment.declare_kinds(**open);
        }
        return **open;
    };
    CacheHooks hooks;
    hooks.prune = [get] {
        std::vector<CachePruned> out;
        for (cache::Dropped& d : get().prune()) out.push_back({std::move(d.kind), std::move(d.generations)});
        return out;
    };
    hooks.max_bytes = [environment] { return environment.cache.max_bytes; };
    return hooks;
}

cli::Command command(const CommandEnvironment& environment) {
    cli::Command c;
    c.name = "tidy-state";
    c.summary = "how big state/ is, part by part, and what of it is stale, gone";
    c.params = {
        cli::Option{"--dry", "", "the sizes, and what would be removed (nothing is)", cli::Arg::Flag},
        cli::Option{"--stray", "", "also remove what no part of CodeBreaker writes", cli::Arg::Flag},
        cli::Option{"--keep-builds", "", "leave Converted/*/build/ alone", cli::Arg::Flag},
    };
    c.handler = [environment](const cli::Args& a) {
        Options options;
        options.dry = a.flag("dry");
        options.remove_stray = a.flag("stray");
        options.keep_builds = a.flag("keep-builds");
        return run(environment.layout, options, cache_hooks(environment.cache));
    };
    return c;
}

}  // namespace cb::base::tidy_state
