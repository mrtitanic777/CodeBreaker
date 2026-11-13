// cb/store/versions_command.hpp -- `codebreaker versions`, complete: the comparison of compare/versions with the production ledger.
//
//     cli::Cli cli("codebreaker", {store::versions_command::command({.config = config, .corpus = corpus})});
//
// The command matches the functions of two builds; what is already known of the old build (its reconstructions, the names of its bodies) comes from
// the function store of the state folder, the knowledge base's body tables, and the verifier's identity of a module (the lanes of engine/lane_kit).
// All of that is made when the command runs. The function store is opened for writing only with `--carry`.
#pragma once

#include <memory>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"

namespace cb::store::versions_command {

struct Environment {
    config::Config config;                  // the settings and the state folder
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
};

cli::Command command(const Environment& environment);

}  // namespace cb::store::versions_command
