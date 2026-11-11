// cb/java/run.hpp -- the Java transpiler's tools: a Java program as C++03, and what its console port needs.
//
//     codebreaker java <tool> [arguments]      (`codebreaker java <tool> --help` for a tool's options)
//
//   Transpile       convert, decompile, scaffold, signatures
//   Judge, report   report, worklist, bugfix-ledger, apimap
//   Console port    sdk, platform-config, assets, xact, achievements, project-layout, vcxproj, build, debug
//   Rewrites of an emitted tree (each also runs by itself in an xbox360 convert)
//                   reflect-factories, sound-fix, controller-nav, render-fix, applet-ping, ui-mark, keybind-mark,
//                   scan-project
//
// This module is the tool table: every tool's name, summary, options and positionals declared for cb::cli (so the parse,
// the validation of choices and the help are ours), and the dispatch. A tool whose module lives in this tree runs here
// (apimap, bugfix-ledger, debug, reflect-factories); the others are run by what the tool's owning module provides through
// `Tools` -- an open extension point, filled as those modules arrive in this tree and wired by the app that owns the
// command table. A tool nobody provides is an UnsupportedError that names it.
//
// Exit codes follow docs/CPP_DESIGN.md 3.6: 0 success, 1 the tool ran and reports a problem (an unmapped symbol, a rewrite whose
// anchor was not found), 2 usage (a missing or wrong argument), 3 the SDK gate refused, 4 the decompile backend failed.
#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"

namespace cb::java::run {

// The tools that are not built in, by tool name ("convert"): each takes the tool's parsed command line and returns the
// exit code. Choices of an option (--edition, --layout ...) are validated before a handler is called.
struct Tools {
    std::map<std::string, cli::Handler, std::less<>> handlers;
};

// The tool names, in the order `java` lists them.
std::vector<std::string_view> tool_names();

// The `java` command: a subcommand for every tool.
cli::Command command(const Tools& tools = {});

// `codebreaker java <args...>`: `args` are the words after `java`. With no tool named it prints the tool list and returns 2.
int main(const std::vector<std::string>& args, const Tools& tools = {});

}  // namespace cb::java::run
