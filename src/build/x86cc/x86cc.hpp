// build/x86cc/x86cc.hpp -- CodeBreaker's x86 C compilers: OpenWatcom's wcc (16-bit code) and wcc386 (32-bit), built from
// their own source into this program (build/x86cc: cb_wcc_i86, cb_wcc_386) and run in-process, so a compile no longer
// starts wcc.exe or wcc386.exe.
//
//     x86cc::Run r = x86cc::compile(x86cc::Target::i386, {"-bt=nt", "-fo=hello.obj", "hello.c"}, folder);
//     if (r.exit_code != 0) cb::err("{}", r.err);
//
// It behaves as the program given the same arguments, in the same working folder, with the same environment: the same
// object and error file, the same messages (banner and notes on stdout, warnings and errors on stderr, as the drivers split
// them: watcom/c/idedrv.c) and the same exit code (0, or 1). `environment` overrides variables for the run (INCLUDE,
// WATCOM, WCC386, ...); the rest are the process's. Each compiler works in the process's
// current folder, so compiles run one at a time (a lock), each in its folder. Each compile has a state of its own
// (run/cbrun.hpp), new at its start and gone at its end: a process that compiles many times gives each compile what a
// process of its own would. Windows only for now.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace cb::build::x86cc {

enum class Target { i86, i386 };

struct Run {
    int exit_code = 0;
    std::string out;   // what the program writes to stdout
    std::string err;   // and to stderr
};

bool available();

// From now on process::run runs `wcc` / `wcc386` (with or without .exe) with these compilers instead of starting them,
// unless CB_WCC=exe. The program calls it at startup (app/codebreaker_main.cpp), after x86link::install().
void install();

// One compile: the compiler's arguments (what follows its name on the command line), run in `folder`.
Run compile(Target target, const std::vector<std::string>& args, const std::filesystem::path& folder,
            const std::map<std::string, std::string>& environment = {});

}  // namespace cb::build::x86cc
