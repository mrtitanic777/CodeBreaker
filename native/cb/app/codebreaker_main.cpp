// cb/app/codebreaker_main.cpp -- the plain C++ `codebreaker` program (docs/PYTHON_RETIREMENT.md step 2): main is the command table of
// cb::app directly, with no translated Python behind it. It runs every native command as codebreaker.exe does, hosts the worker processes
// of the lane kit and of analyze (it launches ITSELF as the worker: host_hook.cpp), and says plainly, exit 2, for a command that is not
// native yet (there is no fallback).
//
//     codebreaker_cb [--corpus NAME] <command> [arguments]
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

#include "cb/app/commands.hpp"
#include "cb/app/native_commands.hpp"

extern "C" int cb_host_main_hook(int argc, char** argv);

namespace {

bool is_help(std::string_view w) { return w == "-h" || w == "--help" || w == "help"; }

}  // namespace

int main(int argc, char** argv) {
    if (const int code = cb_host_main_hook(argc, argv); code >= 0) return code;   // a worker process: its share, then exit

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string_view name;   // the command: the first word that is not `--corpus NAME`
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--corpus") { ++i; continue; }
        name = args[i];
        break;
    }
    // no command, or help: the table prints the catalog; a command that is not run natively yet is said plainly
    if (!name.empty() && !is_help(name) && !cb::app::is_native_command(name)) {
        std::fprintf(stderr, "codebreaker: the command '%.*s' is not native yet (this program has no Python behind it; use codebreaker.exe)\n",
                     static_cast<int>(name.size()), name.data());
        return 2;
    }
    return cb::app::run_native_command(args);
}
