// cb_analyze_run: runs `analyze` of app/analyze_command on a command line, with the settings of the main checkout, to inspect its report on real modules. Not built by default.
//
//     cb_analyze_run analyze crtdll.dll --run 60
#include <string>
#include <vector>

#include "core/cli.hpp"
#include "core/log.hpp"
#include "engine/lane_kit.hpp"
#include "jobs/workers.hpp"
#include "app/analyze_command.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    const config::Config config = config::Config::load(config::current().main_checkout());
    app::analyze_command::Environment environment{.config = config, .corpus = nullptr, .workers = std::nullopt};   // (observe on threads)
    cli::Cli program("cb_analyze_run", {app::analyze_command::command(environment)});
    const std::vector<std::string> args(argv + 1, argv + argc);
    try {
        return program.run(args);
    } catch (const Error& e) {
        err("error: {}\n", e.what());
        return e.exit_code();
    }
}
