// cb/source/ne_asm/run.hpp -- `codebreaker ne-asm <tool> [arguments]`: the hand-written assembly of a 16-bit module as the
// source its authors wrote.
//
// The 16-bit kernel, USER and GDI were written largely in assembly with the SDK's cMacros; the tools of this command
// produce that form, not a disassembly listing: every place a loader patches is a label, and each range is reassembled
// (MASM, or a MASM-compatible NASM -- settings: masm, nasm) and compared. Each tool is a command of its own; this one
// only names them and hands over the rest of the command line. The application registers the tools (`idiom`, `segdiff`,
// ...) by name when it builds its command table.
#pragma once

#include <span>
#include <string>
#include <string_view>

#include "cb/base/commands.hpp"

namespace cb::source::ne_asm::run {

// The command's help: what each tool does, and what the form is.
std::string_view usage();

// The tools the command knows, in the order the help lists them.
std::span<const std::string_view> tool_names();

// args[0] names a tool in `tools`; the tool gets the rest. With no tool or an unknown one the help and the tool names
// are printed and cb::kExitUsage returned.
int main(const base::commands::ToolTable& tools, std::span<const std::string> args);

}  // namespace cb::source::ne_asm::run
