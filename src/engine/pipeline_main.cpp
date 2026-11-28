// cb_pipeline: runs the pipeline commands of source/pipeline.hpp on a command line, with the settings of the main checkout (and the state folder CB_STATE
// names), so each can be compared with a recorded answer on real inputs. Not built by default (`cmake --build <dir> --target cb_pipeline`).
//
//     CB_STATE=<folder> cb_pipeline decompile lz32.dll
#include <string>
#include <vector>

#include "core/cli.hpp"
#include "core/log.hpp"
#include "engine/lane_kit.hpp"
#include "jobs/workers.hpp"
#include "engine/pipeline.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    if (const int worker = engine::host_main_hook(argc, argv); worker >= 0) return worker;   // (what codebreaker.exe's main does first)

    const config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli program("cb_pipeline", engine::pipeline::commands({.config = config, .corpus = nullptr, .workers = false}));
    const std::vector<std::string> args(argv + 1, argv + argc);
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
