// The commands of source/pipeline.hpp that are not made yet: each is declared, and says so when it runs.
#include "cb/source/pipeline.hpp"

namespace cb::source::pipeline {

namespace {

cli::Command not_made(std::string name, std::string summary) {
    cli::Command declared{std::move(name), std::move(summary), {cli::Positional{"args", "", false, true}}, {}, {}, "Reconstruct"};
    declared.passthrough = true;
    declared.handler = [word = declared.name](const cli::Args&) -> int { throw UsageError(word + ": not made native yet"); };
    return declared;
}

}  // namespace

cli::Command score16(const Environment&) { return not_made("score16", "where each 16-bit function differs from the original"); }
cli::Command repair(const Environment&) { return not_made("repair", "search for source that compiles to the original's bytes"); }

}  // namespace cb::source::pipeline
