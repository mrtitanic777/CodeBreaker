// The proof harness of `cache` and `tidy-state`: runs the native commands on a state folder and writes what they print to a file. Inert unless CB_PROOF_ARGS is set (a command line,
// "cache --prune" or "tidy-state --dry"; the settings come from the CB_* environment variables:
// CB_STATE, CB_BUILD_CACHE, CB_CONVERTED, CB_CACHE_MAX_GB), so an ordinary run of the tests does nothing here.
#include <sstream>

#include "base/tidy_state_command.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

CB_TEST("base.cache_proof.runs_the_command_named_by_the_environment", [] {
    const std::optional<std::string> line = cb::fs::get_env("CB_PROOF_ARGS");
    if (!line || line->empty()) return;
    const std::optional<std::string> root = cb::fs::get_env("CB_PROOF_ROOT");
    const cb::config::Config configuration = cb::config::Config::load(root ? std::filesystem::path(*root) : std::filesystem::current_path());
    cb::base::cache::CommandEnvironment cache_environment;
    cache_environment.cache = cb::base::cache::settings_of(configuration);
    std::vector<std::string> args;
    std::istringstream words(*line);
    for (std::string w; words >> w;) args.push_back(w);
    std::string out;
    int code = 0;
    {
        cb::testing::CapturedStd cap;
        cb::cli::Cli cli("codebreaker", {cb::base::cache::command(cache_environment),
                                         cb::base::tidy_state::command(cb::base::tidy_state::environment_of(configuration))});
        code = cli.run(args);
        out = cap.out();
    }
    if (const std::optional<std::string> file = cb::fs::get_env("CB_PROOF_OUT")) cb::fs::write_text(*file, out);
    CB_CHECK_EQ(code, 0);
});
