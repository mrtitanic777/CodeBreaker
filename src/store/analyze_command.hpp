// store/analyze_command.hpp -- `codebreaker analyze`, complete: the report of analyze/report over the corpus, the function store and the decompiler's knowledge.
//
//     cli::Cli cli("codebreaker", {store::analyze_command::command({.config = config, .corpus = corpus})});
//
// analyze/report runs the analyses on a module and files their facts in the function store's fact table; what it asks of the world -- which file a corpus
// name is, the module's content hash, the function store to note the module in, the knowledge the 16-bit description is made with and the names of the
// system's exports -- comes from the corpus, the state folder and the lane kit (engine/lane_kit), all made when the command runs.
#pragma once

#include <memory>
#include <optional>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "jobs/workers.hpp"

namespace cb::store::analyze_command {

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
    // Worker processes of the host program to run observe's functions in (the program's own launcher: jobs::WorkerLauncher::for_this_program()); the
    // emulator never uses more than about one core on threads, so the functions are shared out over processes. The host's main must have called
    // `register_worker_kinds`. None: threads.
    std::optional<jobs::WorkerLauncher> workers;
};

cli::Command command(const Environment& environment);

// Registers the worker kind observe's processes run as ("observe"), with the surroundings a worker makes for itself from the same configuration.
void register_worker_kinds(jobs::WorkerKinds& kinds, const Environment& environment);

}  // namespace cb::store::analyze_command
