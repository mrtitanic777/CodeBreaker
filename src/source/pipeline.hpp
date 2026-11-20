// source/pipeline.hpp -- the commands of the reconstruction pipeline, complete: each is made from the settings and the corpus alone.
//
//     source::pipeline::Environment environment{.config = config::Config::load(checkout), .corpus = corpus};
//     cli::Cli cli("codebreaker", source::pipeline::commands(environment));
//     cli.run({"decompile", "lz32.dll"});
//
//     decompile <module> [--out FILE]               any module as C (decompile/decompiler)
//     module <module>... | scoreboard               reconstruct, judge and write the module's source file; the scoreboard (source/module_writer)
//     build <module>... [--out DIR] [--no-run]      the whole module built and compared with the original (build/module)
//     measure <module>                              reconstruct and judge, print the tally (verify/measure)
//     teeth <module> [--rounds N] [--json FILE]     does the judge notice a broken function (verify/teeth)
//     sweep [...]                                   every module of the corpus, resumably (verify/sweep)
//     match <module> [...]                          would the period compiler give the original's bytes back (verify/authored/match)
//     score16 [...]                                 the 16-bit gate's scoreboard (verify/gate16/scoreboard16)
//     repair <module> [...]                         search for source that compiles to the original's bytes (verify/authored/repair)
//     place <module>... | --all [--list]            where each module's source goes (source/placement)
//
// Nothing is made until a command runs: the lane kit (the decompiler, the tools, the emulators), the function store and the reconstructor are
// opened when the first command that needs them starts, and are that command's own -- two commands share nothing but the files they write.
#pragma once

#include <memory>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"

namespace cb::source::pipeline {

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
    // The program is a host for the lane kit's worker processes (it registered `LaneKit::register_worker_kinds` at the top of main and runs
    // `jobs::run_as_worker`): judging and building are shared out over processes of it, as many as the machine's budget -- an emulator is one lock
    // per process, so threads alone use about one core. false: everything runs in this process.
    bool workers = false;
};

cli::Command decompile(const Environment& environment);
cli::Command module(const Environment& environment);
cli::Command scoreboard(const Environment& environment);
cli::Command build(const Environment& environment);
cli::Command measure(const Environment& environment);
cli::Command teeth(const Environment& environment);
cli::Command sweep(const Environment& environment);
cli::Command match(const Environment& environment);
cli::Command score16(const Environment& environment);
cli::Command repair(const Environment& environment);
cli::Command place(const Environment& environment);

// Every command above, in that order.
std::vector<cli::Command> commands(const Environment& environment);

}  // namespace cb::source::pipeline
