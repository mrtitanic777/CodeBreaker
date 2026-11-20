// build/x86asm/driver.hpp -- one run of x86asm with the streams kept apart (the command `codebreaker nasm` and the
// tests use it; `assemble` in x86asm.hpp merges the streams the way the corpus compares them).
#pragma once

#include <map>
#include <string>
#include <string_view>

#include "build/x86asm/x86asm.hpp"

namespace cb::build::x86asm {

// What a run did: NASM's exit status, every file it wrote (by the name it was given), and what it wrote to stdout and to
// stderr (in NASM's words).
struct RunResult {
    int exit = 0;
    std::map<std::string, Bytes> files;
    std::string out;
    std::string err;
};

// Run NASM's driver (asm/nasm.c) over `request`. `text_crlf`: text-mode output (the listing, a map file, a text format, the
// messages) is written with "\r\n" line ends, as the reference nasm.exe wrote on Windows; false leaves "\n".
RunResult run(const Request& request, bool text_crlf = true);

}  // namespace cb::build::x86asm
