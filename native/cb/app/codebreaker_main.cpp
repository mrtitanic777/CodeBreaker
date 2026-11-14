// cb/app/codebreaker_main.cpp -- THE `codebreaker` program: main is the command table of
// cb::app directly, with no translated Python behind it. It runs every native command as codebreaker.exe does, hosts the worker processes
// of the lane kit and of analyze (it launches ITSELF as the worker: host_hook.cpp), and says plainly, exit 2, for a command that is not
// native yet (there is no fallback).
//
//     codebreaker [--corpus NAME] <command> [arguments]
//     codebreaker                 in a terminal: the terminal interface; `ui [module]` likewise
//
// The terminal interface is the one thing that stays Python (codebreaker/ui/terminal.py, Textual): it runs under a real python found by
// find_python() and talks to this program's `serve` (CB_EXE is set to this program for it; ui/client.py starts the service with it).
#include <process.h>
#include <stdlib.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/app/commands.hpp"
#include "cb/app/native_commands.hpp"
#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/process.hpp"
#include "cb/core/win.hpp"

extern "C" int cb_host_main_hook(int argc, char** argv);

namespace {

bool is_help(std::string_view w) { return w == "-h" || w == "--help" || w == "help"; }

std::string env_text(const char* name) {
    char* value = nullptr;
    std::size_t n = 0;
    std::string out;
    if (_dupenv_s(&value, &n, name) == 0 && value) out = value;
    std::free(value);
    return out;
}

// A python that runs: CB_PYTHON, the setting `python`, then `python` and `py -3` on the PATH. The first whose `-c pass` succeeds
// (the Microsoft Store's stub for a missing python fails it). Empty argv when there is none.
std::vector<std::string> find_python(const cb::config::Config& config) {
    std::vector<std::vector<std::string>> tries;
    if (const std::string e = env_text("CB_PYTHON"); !e.empty()) tries.push_back({e});
    if (const std::string s = config.text("python"); !s.empty()) tries.push_back({s});
    tries.push_back({"python"});
    tries.push_back({"py", "-3"});
    for (std::vector<std::string>& t : tries) {
        std::vector<std::string> probe = t;
        probe.insert(probe.end(), {"-c", "pass"});
        try {
            if (cb::process::run({.argv = probe, .err = cb::process::Capture::Discard, .timeout = std::chrono::seconds(20)}).ok()) return t;
        } catch (const std::exception&) {   // not found
        }
    }
    return {};
}

std::wstring self_path() {
    char* path = nullptr;
    _get_pgmptr(&path);   // the full path of this program
    return path ? cb::win::to_wide(path) : std::wstring();
}

// The terminal interface, under real python, in this console (stdin, stdout and stderr are inherited). `args` are the words after `ui`.
int run_terminal_interface(const std::vector<std::string>& args) {
    const cb::config::Config& config = cb::config::current();
    const std::filesystem::path root = config.checkout_root();
    if (!std::filesystem::exists(root / "codebreaker" / "ui" / "terminal.py")) {
        std::fprintf(stderr, "codebreaker: the terminal interface (codebreaker/ui/terminal.py) was not found in %s\n", root.string().c_str());
        return 2;
    }
    std::vector<std::string> python = find_python(config);
    if (python.empty()) {
        std::fprintf(stderr,
                     "codebreaker: the terminal interface needs Python 3 with Textual, and no python was found.\n"
                     "  Install Python (python.org), then:  pip install textual\n"
                     "  or name a python in settings.json (\"python\") or the CB_PYTHON environment variable.\n"
                     "  Every other command works without it.\n");
        return 2;
    }
    std::vector<std::wstring> words;
    for (const std::string& w : python) words.push_back(std::filesystem::u8path(w).wstring());
    words.insert(words.end(), {L"-m", L"codebreaker.ui.terminal"});
    for (const std::string& a : args) words.push_back(std::filesystem::u8path(a).wstring());
    std::vector<const wchar_t*> argv;
    for (const std::wstring& w : words) argv.push_back(w.c_str());
    argv.push_back(nullptr);

    // CB_EXE: the program the client starts as the service. PYTHONPATH: the checkout, so `codebreaker.ui` imports from anywhere.
    _wputenv_s(L"CB_EXE", self_path().c_str());
    std::wstring path = root.wstring();
    wchar_t* old = nullptr;
    std::size_t old_size = 0;
    if (_wdupenv_s(&old, &old_size, L"PYTHONPATH") == 0 && old && *old) path += L";" + std::wstring(old);
    std::free(old);
    _wputenv_s(L"PYTHONPATH", path.c_str());
    const intptr_t rc = _wspawnvp(_P_WAIT, argv[0], argv.data());
    if (rc == -1) {
        std::fprintf(stderr, "codebreaker: python could not be started for the terminal interface\n");
        return 2;
    }
    return static_cast<int>(rc);
}

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
    // the terminal interface: `ui [module]`, or no command in a terminal (the one thing that is Python)
    {
        std::vector<std::string> rest;   // the line with `--corpus NAME` taken off
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "--corpus") { ++i; continue; }
            rest.push_back(args[i]);
        }
        if (!rest.empty() && rest[0] == "ui") return run_terminal_interface({rest.begin() + 1, rest.end()});
        if (rest.empty() && cb::app::stdio_are_terminals()) return run_terminal_interface({});
    }
    // no command, help, or a word that is no command: the table prints the catalog or the usage; a listed command that is not native is said plainly
    if (!name.empty() && !is_help(name) && cb::app::find_command_doc(name) && !cb::app::is_native_command(name)) {
        std::fprintf(stderr, "codebreaker: the command '%.*s' is not native yet (this program has no Python behind it; use codebreaker.exe)\n",
                     static_cast<int>(name.size()), name.data());
        return 2;
    }
    return cb::app::run_native_command(args);
}
