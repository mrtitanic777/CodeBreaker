// cb/exact/emu/dos_run_command.hpp -- `codebreaker dos-run`: a real-mode DOS program run on CodeBreaker's own DOS executive
// (exact/emu/dos, on its Unicorn), in the process: no DOSBox-X.
//
//     codebreaker dos-run [--dir D] [--save S] [--trace-calls] [--out results.json] <program> [its command line ...]
//
// The program runs with D (default: the current folder) as its drive C:; what it reads comes from there, and what it writes
// is kept in memory and reported -- each file it made or changed with its size and SHA-1, and written into S when --save
// names a folder (the disk under D is never written). --trace-calls adds how many times each address was called. The
// report is printed (and with --out written as JSON): the exit code, what the program wrote to the console, the files.
// Exit status: the program's exit code; 3 when it did not end within the executive's limits.
//
// This is the seed of the DOSBox-X replacement (docs/BUILTIN_ENGINES_PLAN.md, item 4): the jobs DOSBox-X runs today --
// standard input, the environment, more drives, time limits -- grow here, without changing what the command does now.
#pragma once

#include "cb/core/cli.hpp"

namespace cb::exact::emu::dos_run {

cli::Command command();

}  // namespace cb::exact::emu::dos_run
