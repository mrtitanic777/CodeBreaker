// build/x86link/x86link.hpp -- CodeBreaker's x86 linker (docs/X86LINK_DESIGN.md): OpenWatcom's wlink, built from its own
// source into this program (build/x86link/wlink, cb_wlink) and run in-process, so a link no longer starts wlink.exe.
//
//     x86link::Run r = x86link::link({"system", "dos", "file", "hello.obj", "name", "hello.exe"}, folder);
//     if (r.exit_code != 0) cb::err("{}", r.err);
//
// It behaves as wlink.exe given the same arguments, in the same working folder, with the same environment: the same files
// written, the same messages (banner and notes on stdout, warnings and errors on stderr, as wlink.exe's driver splits them)
// and the same exit code (0, or 1). `environment` overrides variables for the run (WATCOM, LIB, ...); the rest are the
// process's. wlink keeps its state in globals and works in the process's current folder, so links run one at a time (a
// lock), each in its folder. Windows only for now (wlink's file layer is Win32); elsewhere available() is false.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace cb::build::x86link {

struct Run {
    int exit_code = 0;
    std::string out;   // what wlink.exe writes to stdout
    std::string err;   // and to stderr
};

bool available();

// One link: wlink's arguments (what follows `wlink` on its command line), run in `folder`.
Run link(const std::vector<std::string>& args, const std::filesystem::path& folder,
         const std::map<std::string, std::string>& environment = {});

}  // namespace cb::build::x86link
