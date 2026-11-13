// cb_analyze_run: runs `analyze` of store/analyze_command on a command line, with the settings of the main checkout, to compare its report with the
// Python's on real modules. Not built by default.
//
//     cb_analyze_run analyze crtdll.dll --run 60
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/log.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/jobs/workers.hpp"
#include "cb/store/analyze_command.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    const config::Config config = config::Config::load(config::current().main_checkout());
    store::analyze_command::Environment environment{.config = config, .corpus = nullptr, .workers = std::nullopt};   // (observe on threads)
    cli::Cli program("cb_analyze_run", {store::analyze_command::command(environment)});
    const std::vector<std::string> args(argv + 1, argv + argc);
    try {
        return program.run(args);
    } catch (const Error& e) {
        err("error: {}\n", e.what());
        return e.exit_code();
    }
}
