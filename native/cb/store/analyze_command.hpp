// cb/store/analyze_command.hpp -- `codebreaker analyze`, complete: the report of analyze/report over the corpus, the function store and the decompiler's knowledge.
//
//     cli::Cli cli("codebreaker", {store::analyze_command::command({.config = config, .corpus = corpus})});
//
// analyze/report runs the analyses on a module and files their facts in the function store's fact table; what it asks of the world -- which file a corpus
// name is, the module's content hash, the function store to note the module in, the knowledge the 16-bit description is made with and the names of the
// system's exports -- comes from the corpus, the state folder and the lane kit (engine/lane_kit), all made when the command runs.
#pragma once

#include <memory>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"

namespace cb::store::analyze_command {

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
};

cli::Command command(const Environment& environment);

}  // namespace cb::store::analyze_command
