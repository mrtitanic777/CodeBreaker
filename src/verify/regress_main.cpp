// cb_regress: `regress` of verify/regress.hpp as a program of its own, with the settings of the main checkout: the proof harness of the command
// (the application registers the same command). Not built by default (`cmake --build <dir> --target cb_regress`).
//
//     cb_regress all15 [--runs 2] [--vms 1]
#include <string>
#include <vector>

#include "core/cli.hpp"
#include "core/log.hpp"
#include "engine/lane_kit.hpp"
#include "verify/regress.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    if (const int worker = engine::host_main_hook(argc, argv); worker >= 0) return worker;
    const config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli program("cb_regress", {verify::regress::command(config)});
    std::vector<std::string> args = {"regress"};
    args.insert(args.end(), argv + 1, argv + argc);
    try {
        return program.run(args);
    } catch (const UsageError& e) {
        err("error: {}\n", e.what());
        return kExitUsage;
    } catch (const Error& e) {
        err("error: {}\n", e.what());
        return e.exit_code();
    }
}
