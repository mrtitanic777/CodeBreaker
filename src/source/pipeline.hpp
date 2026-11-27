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

#include <cstddef>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "verify/reconstruct.hpp"
#include "store/verdicts.hpp"

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

// ---- judging one program, as `check` (measure) does -------------------------------------------------------------------------------
//
//     const Judging j = judge(environment, "notepad.exe", {.progress = ..., .stop = ...});
//     if (j.judged) ... j.outcome.scores (the module by class), j.functions (each function's candidate and its class)
//     else          ... j.why_not ("nothing judges this kind of file", "the win64 lane's tools are not set up on this computer")
//
// The same path `measure` takes: the lane kit's lanes, the reconstructor over the function store of the state folder (decompile, build, run
// each function beside the original, record), only what changed since the last run made again. Convert's judging stage calls it.

struct JudgeSteps {
    std::function<void(verify::reconstruct::Step, std::size_t done, std::size_t total)> progress;   // Services::progress of reconstruct
    std::function<bool()> stop;                                                                     // Services::stop: true leaves by reconstruct::Stopped
};

struct JudgedFunction {
    std::string key, name;     // the lane's key of the function (its address, as hex) and the name it built it by (empty when it built nothing)
    std::optional<store::Verdict> verdict;   // the judge's verdict; none when nothing is recorded
    store::Classification classification;
    std::string reason;        // why the judge said so, or the compiler's message
};

struct Judging {
    bool judged = false;
    std::string lane;          // the lane that judged it ("win64"); empty when none
    std::string why_not;       // not judged: why, in a sentence
    verify::reconstruct::Outcome outcome;
    std::vector<JudgedFunction> functions;
};

// Judge `program` (a path, or a corpus name). Throws what the lane throws, and reconstruct::Stopped when `steps.stop` said to stop.
Judging judge(const Environment& environment, const std::filesystem::path& program, const JudgeSteps& steps = {});

}  // namespace cb::source::pipeline
