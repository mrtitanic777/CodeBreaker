// app/state_commands.hpp -- the commands that read and write the state folder: names, knowledge, coverage, progress, facts, origin,
// driver-services.
//
// Each is built over the configuration's state folder and the corpus; what is costly (the lane kit, the
// knowledge base) is worked out when a command first asks for it, so building the table stays cheap.
#pragma once

#include <memory>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"

namespace cb::app {

std::vector<cli::Command> state_commands(const config::Config& config, const std::shared_ptr<base::Corpus>& corpus);

}  // namespace cb::app
