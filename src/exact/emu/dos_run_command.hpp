// exact/emu/dos_run_command.hpp -- `codebreaker dos-run`: a real-mode DOS program run on CodeBreaker's own DOS executive
// (exact/emu/dos, on its Unicorn), in the process.
//
//     codebreaker dos-run [--dir D] [--save S] [--trace-calls] [--out results.json] <program> [its command line ...]
//
// The program runs with D (default: the current folder) as its drive C:; what it reads comes from there, and what it writes
// is kept in memory and reported -- each file it made or changed with its size and SHA-1, and written into S when --save
// names a folder (the disk under D is never written). --trace-calls adds how many times each address was called. The
// report is printed (and with --out written as JSON): the exit code, what the program wrote to the console, the files.
// Exit status: the program's exit code; 3 when it did not end within the executive's limits.
//
// This is where the executive's other jobs -- standard input, the environment, more drives, time limits -- grow, without
// changing what the command does now.
#pragma once

#include "core/cli.hpp"

namespace cb::exact::emu::dos_run {

cli::Command command();

}  // namespace cb::exact::emu::dos_run
