// cb/coverage/reach_command.hpp -- `codebreaker reach`, complete: the command of coverage/reach with its Services made from the settings.
//
//     cli::Cli cli("codebreaker", {coverage::reach_command::command({.config = config, .corpus = corpus})});
//
// reach asks the platform registry what a file is, the function store for the tally of what `measure` made of a program (filed under the
// program's identity, which the verifier's lanes say), and this executable to measure; the settings give the state folder (results under
// state/reach) and the checkout (the tools' revision is its git HEAD). The function store and the lanes are made when the first tally is asked
// for, and the store is opened read-only.
#pragma once

#include <memory>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"
#include "cb/coverage/reach.hpp"
#include "cb/platforms/package.hpp"

namespace cb::coverage::reach_command {

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
};

// The registry of every platform CodeBreaker knows, over the readers of cb::formats.
std::unique_ptr<platforms::Registry> standard_registry();

// reach's Services over the settings.
reach::Services services(const Environment& environment);

cli::Command command(const Environment& environment);

}  // namespace cb::coverage::reach_command
