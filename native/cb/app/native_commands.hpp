// cb/app/native_commands.hpp -- the commands codebreaker.exe runs on native/cb directly, with no Python behind them
// (the switch-over, docs/CPP_DESIGN.md 8). Each one moves here once its whole module chain is refactored; the rest are
// still the translated Python's.
//
//     codebreaker info <file-or-module|folder> ...     cb::formats::inspect
//     codebreaker flat-thunk <module> ...              cb::build::flat_thunk
//
// The exe reaches this through one C entry point, cb_native_command(), called from native/cbexe/commands.cc; the
// translated codebreaker/__main__.py hands it a command line whose command is_native() before anything else reads it
// (so `--corpus NAME` is acted on here, by CommandTable).
#pragma once

#include <memory>
#include <span>
#include <string>

#include "cb/app/commands.hpp"

namespace cb::base {
class Corpus;
}

namespace cb::app {

// Registers every natively run command on `table`; module names resolve through `corpus`.
void register_native_commands(CommandTable& table, std::shared_ptr<base::Corpus> corpus);

// Whether `name` is run natively.
bool is_native_command(std::string_view name);

// Runs one command line (`--corpus NAME` anywhere, the command's name first of the rest) with cb::config::current();
// returns its exit code (errors are reported on stderr and mapped to their exit codes; output is flushed).
int run_native_command(std::span<const std::string> args);

}  // namespace cb::app

// The exe's entry. Sets *handled to 0 (and returns 2) when the line's command is not native.
extern "C" int cb_native_command(int argc, const char* const* argv, int* handled);
