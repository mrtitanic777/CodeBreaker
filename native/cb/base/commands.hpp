// cb/base/commands.hpp -- run one of a package's own tools from inside a command.
//
// Some packages are a set of tools, each with its own command line (the compiler models, the driver-kit source
// chain). A command names them in a table and hands the rest of its arguments to the one asked for:
//
//     commands::ToolTable tools = {{"wcc", run_wcc}, {"wlink", run_wlink}};
//     return commands::dispatch(tools, args, "usage: codebreaker compilers <tool> [arguments]");
#pragma once
#include <functional>
#include <map>
#include <span>
#include <string>
#include <string_view>

namespace cb::base::commands {

// A tool: given the arguments after its name, returns its exit code.
using ToolFn = std::function<int(std::span<const std::string> args)>;
// Sorted by name, which is the order the usage text lists them in.
using ToolTable = std::map<std::string, ToolFn>;

// args[0] names one of `tools`; the rest are that tool's arguments, and the tool's exit code is returned. With no
// arguments or an unknown tool name, writes `usage` and the tool names to stdout and returns cb::kExitUsage.
int dispatch(const ToolTable& tools, std::span<const std::string> args, std::string_view usage = "");

}  // namespace cb::base::commands
