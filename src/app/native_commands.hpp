// app/native_commands.hpp -- the commands codebreaker.exe runs on src directly.
//
//     codebreaker info <file-or-module|folder> ...     cb::formats::inspect
//     codebreaker flat-thunk <module> ...              cb::build::flat_thunk
//     codebreaker driver-build <source> ...            cb::build::driver
//     codebreaker demangle <name> ...                  cb::symbols::demangle
//     codebreaker dos-interrupts [<int> [<AH> [<AL>]]]  cb::symbols::dos_interrupts
//
// codebreaker.exe's main hands every command line to this table (so `--corpus NAME` is acted on here, by CommandTable).
#pragma once

#include <memory>
#include <span>
#include <string>

#include "app/commands.hpp"

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
