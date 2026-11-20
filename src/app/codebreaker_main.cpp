// app/codebreaker_main.cpp -- THE `codebreaker` program: main is the command table of cb::app directly. It runs every native command
// as codebreaker.exe does, hosts the worker processes of the lane kit and of analyze (it launches ITSELF as the worker: host_hook.cpp), and says
// plainly, exit 2, for a command that is not native yet (there is no fallback).
//
//     codebreaker [--corpus NAME] <command> [arguments]
//     codebreaker                 no command: the window (app/window.hpp); `app` likewise
//
// One program, console subsystem: with arguments it is the command line (output, errors and exit codes as ever, so it pipes); with none it is
// the Windows app, which hides the console it was started with (window_win.cpp).
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "app/commands.hpp"
#include "app/native_commands.hpp"
#include "app/window.hpp"
#include "base/config.hpp"
#include "core/fs.hpp"

extern "C" int cb_host_main_hook(int argc, char** argv);

namespace {

bool is_help(std::string_view w) { return w == "-h" || w == "--help" || w == "help"; }

// With no work folder (and no "state" setting), what a command makes of a file it is given goes beside that file:
// <folder>/<name>-codebreaker/ (Logan, 2026-11-20). The first argument that names a file decides.
void state_beside_input(const std::vector<std::string>& args) {
    const cb::config::Config& config = cb::config::current();
    if (config.work_folder() || config.path("state")) return;
    for (const std::string& a : args) {
        if (a.empty() || a[0] == '-') continue;
        std::error_code ec;
        const std::filesystem::path p = cb::fs::from_text(a);
        if (!std::filesystem::is_regular_file(p, ec)) continue;
        const std::filesystem::path beside = std::filesystem::absolute(p, ec).parent_path() / (p.stem().string() + "-codebreaker");
#ifdef _WIN32
        _putenv_s("CB_STATE", cb::fs::to_text(beside).c_str());
#else
        setenv("CB_STATE", cb::fs::to_text(beside).c_str(), 1);
#endif
        return;
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (const int code = cb_host_main_hook(argc, argv); code >= 0) return code;   // a worker process: its share, then exit

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string_view name;   // the command: the first word that is not `--corpus NAME`
    bool has_command = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--corpus") { ++i; continue; }
        name = args[i];
        has_command = true;
        break;
    }
    // no command: the window
    if (!has_command && cb::app::window::available()) return cb::app::window::run({}, true);
    // no command, help, or a word that is no command: the table prints the catalog or the usage; a listed command that is not native is said plainly
    if (!name.empty() && !is_help(name) && cb::app::find_command_doc(name) && !cb::app::is_native_command(name)) {
        std::fprintf(stderr, "codebreaker: the command '%.*s' is not native yet (this program has no Python behind it; use codebreaker.exe)\n",
                     static_cast<int>(name.size()), name.data());
        return 2;
    }
    state_beside_input(args);
    return cb::app::run_native_command(args);
}
