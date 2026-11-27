// app/codebreaker_main.cpp -- THE `codebreaker` program: main is the command table of cb::app directly. It runs every native command
// as codebreaker.exe does, hosts the worker processes of the lane kit and of analyze (it launches ITSELF as the worker: host_hook.cpp), and says
// plainly, exit 2, for a command that is not native yet (there is no fallback).
//
//     codebreaker [--corpus NAME] <command> [arguments]
//     codebreaker                 no command: the window (app/window.hpp); `app` likewise
//
// One program, console subsystem: with arguments it is the command line (output, errors and exit codes as ever, so it pipes); with none it is
// the Windows app, which hides the console it was started with (window_win.cpp).
#include <cstdint>
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
#include "build/x86cc/x86cc.hpp"
#include "build/pklite.hpp"
#include "build/x86link/x86link.hpp"
#include "core/fs.hpp"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

extern "C" int cb_host_main_hook(int argc, char** argv);

namespace {

#ifdef _WIN32
// A crash says what it was and where, on stderr, before the process ends: without it a crash ends a long run with no output at
// all (a `check` of kernel32 once ended so after 14 minutes). Each frame is a module and an offset into it, which the linker's map
// of that module names.
void say_where(const void* address) {
    HMODULE module = nullptr;
    char name[MAX_PATH] = "?";
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, static_cast<LPCSTR>(address), &module) &&
        module != nullptr) {
        char path[MAX_PATH];
        if (const DWORD n = GetModuleFileNameA(module, path, MAX_PATH); n > 0 && n < MAX_PATH) {
            const char* base = path;
            for (const char* c = path; *c; ++c)
                if (*c == '\\' || *c == '/') base = c + 1;
            lstrcpynA(name, base, MAX_PATH);
        }
    }
    const auto offset = reinterpret_cast<std::uintptr_t>(address) - reinterpret_cast<std::uintptr_t>(module);
    std::fprintf(stderr, "  %s+0x%llx\n", name, static_cast<unsigned long long>(module != nullptr ? offset : reinterpret_cast<std::uintptr_t>(address)));
}

LONG WINAPI report_crash(EXCEPTION_POINTERS* e) {
    const EXCEPTION_RECORD& r = *e->ExceptionRecord;
    std::fprintf(stderr, "codebreaker: crashed: exception 0x%08lX at\n", static_cast<unsigned long>(r.ExceptionCode));
    say_where(r.ExceptionAddress);
    if (r.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r.NumberParameters >= 2)
        std::fprintf(stderr, "  (%s address 0x%llx)\n", r.ExceptionInformation[0] == 0 ? "reading" : r.ExceptionInformation[0] == 1 ? "writing" : "running",
                     static_cast<unsigned long long>(r.ExceptionInformation[1]));
    // the faulting thread's stack, from the exception's context
    CONTEXT context = *e->ContextRecord;
    std::fprintf(stderr, "called from\n");
    for (int depth = 0; depth < 32 && context.Rip != 0; ++depth) {
        DWORD64 image_base = 0;
        PRUNTIME_FUNCTION f = RtlLookupFunctionEntry(context.Rip, &image_base, nullptr);
        if (f == nullptr) {   // (a leaf: its return address is on top of the stack)
            context.Rip = *reinterpret_cast<DWORD64*>(context.Rsp);
            context.Rsp += 8;
        } else {
            void* handler_data = nullptr;
            DWORD64 frame = 0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, image_base, context.Rip, f, &context, &handler_data, &frame, nullptr);
        }
        if (context.Rip != 0) say_where(reinterpret_cast<const void*>(context.Rip));
    }
    std::fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

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
#ifdef _WIN32
    SetUnhandledExceptionFilter(&report_crash);
#endif
    cb::build::pklite::install();    // PKLITE programs are read from their streams
    cb::build::x86link::install();   // wlink runs in this program (x86link), not as wlink.exe
    cb::build::x86cc::install();     // and wcc / wcc386 (x86cc), not as wcc.exe / wcc386.exe
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
