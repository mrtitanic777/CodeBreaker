// cb_pipeline: runs the pipeline commands of source/pipeline.hpp on a command line, with the settings of the main checkout (and the state folder CB_STATE
// names), so each can be compared with the Python's answer on real inputs. Not built by default (`cmake --build <dir> --target cb_pipeline`).
//
//     CB_STATE=<folder> cb_pipeline decompile lz32.dll
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/log.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/jobs/workers.hpp"
#include "cb/source/pipeline.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    jobs::WorkerKinds worker_kinds;
    engine::LaneKit::register_worker_kinds(worker_kinds);
    if (const std::optional<int> code = jobs::run_as_worker(worker_kinds, argc, argv)) return *code;

    const config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli program("cb_pipeline", source::pipeline::commands({.config = config, .corpus = nullptr, .workers = true}));
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
