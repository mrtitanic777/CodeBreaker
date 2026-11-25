// java/tools.hpp -- `codebreaker java <tool>`, complete: every tool of run.hpp's table wired to the module that does its work.
//
//     table.set(java::tools::command({.config = config}));                       // as a declared command
//     const int code = java::tools::main({"convert", src, out, "--platform", "xbox360"}, {.config = config});
//
// run.hpp declares the tools and runs the ones whose module needs nothing from outside (apimap, bugfix-ledger, debug, reflect-factories); the rest
// are handlers, and this makes them from the configuration:
//
//   convert, decompile     the transpiler assembled over the lane's folder (codebreaker/java: platform layer, API maps, the projects' profiles) with the
//                          Java front end (java/frontend: TreeDump run by the JDK of setting java, the libraries of setting java_libs); a Minecraft version
//                          is decompiled first (java/projects/minecraft/prep, natively) when --mc-version names one
//   scaffold, signatures   the Java source parsed into skeletons or the codebase map
//   report, worklist       the conversion report of an emitted tree; every .cpp compiled by the host compiler (setting host_cxx, else g++)
//   sdk, platform-config   the console SDK (settings xbox360_sdk ...), and the controller, keyboard and storage choices the port is emitted with
//   assets, xact           the game's jar as Content, the XACT audio banks
//   achievements, project-layout, vcxproj, build, debug, and the rewrites of an emitted tree
//
// The lane's own folder, where the apimap and the debug text are read, is the checkout's (CB_CHECKOUT_ROOT is set for the length of the call).
// A line that reports a problem is written to stderr; the result of a tool, to stdout.
#pragma once

#include <filesystem>
#include <functional>
#include <span>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "java/run.hpp"

namespace cb::java::tools {

struct Environment {
    cb::config::Config config;
};

// The handlers of every tool run.hpp does not run itself.
run::Tools make_tools(const Environment& environment);

// The `java` command with every tool wired.
cli::Command command(const Environment& environment);

// The PC build of a converted project (the CMakeLists.txt `java port` writes): CMake configures it in <project>/build and builds it -- with the
// MinGW-w64 prefix's own gcc and make in <project>/build-mingw when the SDL2 CMake found is MinGW's. `quiet`: what the tools print is kept in
// <project>/build.log instead of shown, and its errors (else its last lines) are `tail`.
struct PcBuild {
    enum class Status { Built, Failed, NoCMake };
    Status status = Status::Failed;
    int exit_code = 0;
    std::filesystem::path exe;   // Built: the program
    std::filesystem::path log;   // quiet: the build's log
    std::string tail;            // quiet: the lines that say what went wrong
};
PcBuild build_pc_project(const std::filesystem::path& project, bool quiet);

// `codebreaker java <tool> ...`: `args` are the words after `java`.
int main(const std::vector<std::string>& args, const Environment& environment);
std::function<int(std::span<const std::string>)> runner(const Environment& environment);

}  // namespace cb::java::tools
