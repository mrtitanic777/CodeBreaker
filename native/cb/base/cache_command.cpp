// cb/base/cache_command.cpp -- see cache_command.hpp.
#include "cb/base/cache_command.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::base::cache {

namespace {

// A cap setting in gigabytes as a number of bytes: the default when it is not set or not a number.
std::uint64_t cap_bytes(const cb::config::Config& configuration, std::string_view key, std::uint64_t fallback) {
    const std::optional<double> gb = configuration.number(key);
    if (!gb || *gb <= 0) return fallback;
    return static_cast<std::uint64_t>(*gb * static_cast<double>(kGiB));
}

}  // namespace

Config settings_of(const cb::config::Config& configuration) {
    Config c;
    c.state_dir = configuration.state() / "cache";
    c.shared_dir = configuration.path("build_cache").value_or(configuration.main_checkout() / "state" / "build_cache");
    c.max_bytes = cap_bytes(configuration, "cache_max_gb", kDefaultMaxBytes);
    c.shared_max_bytes = cap_bytes(configuration, "build_cache_max_gb", kDefaultSharedMaxBytes);
    c.disabled = Config::disabled_by_environment();
    return c;
}

cli::Command command(const CommandEnvironment& environment) {
    cli::Command c;
    c.name = "cache";
    c.summary = "what the result cache holds, kind by kind; --prune drops old generations, --clear empties it";
    c.params = {
        cli::Option{"--stats", "", "what is kept, kind by kind, and the cap (the default)", cli::Arg::Flag},
        cli::Option{"--prune", "", "drop the results of old generations and hold the cap, then list", cli::Arg::Flag},
        cli::Option{"--clear", "", "drop everything kept", cli::Arg::Flag},
    };
    c.handler = [environment](const cli::Args& a) {
        Cache cache(environment.cache);
        if (environment.declare_kinds) environment.declare_kinds(cache);
        if (a.flag("clear")) {
            out("cache cleared: {} freed\n", size_text(cache.clear()));
            return kExitOk;
        }
        if (a.flag("prune"))
            for (const Dropped& d : cache.prune())
                out("  {:<16} dropped old generation(s) {}\n", d.kind, text::join(d.generations, ", "));
        out_text(describe(cache.stats(), environment.cache.max_bytes));
        return kExitOk;
    };
    return c;
}

}  // namespace cb::base::cache
