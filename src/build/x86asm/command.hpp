// build/x86asm/command.hpp -- `codebreaker nasm <NASM's arguments>`: x86asm run on real files, as nasm.exe was.
//
// To register it, add to the command table in cb/app/native_commands.cpp (register_native_commands), and "nasm" to its kNative
// list:
//
//     table.set("nasm", [](std::span<const std::string> args) -> int { return build::x86asm::run_nasm_command(args); });
//
// (with #include "build/x86asm/command.hpp").
#pragma once

#include <span>
#include <string>

namespace cb::build::x86asm {

// Run nasm with `args` (NASM's own arguments, not the program's name): files are read from disk (relative names against the
// current folder), the files the run writes are written to disk, its messages go to stdout and stderr, and the result is NASM's
// exit status.
int run_nasm_command(std::span<const std::string> args);

}  // namespace cb::build::x86asm
