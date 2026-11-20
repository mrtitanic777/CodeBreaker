// cb/core/nasm_capture.hpp -- the cases x86asm (docs/X86ASM_DESIGN.md) is proven against: every run of the NASM program
// CodeBreaker makes, saved while CB_NASM_CAPTURE names a folder.
//
//     <folder>/<id>/case.json      the arguments (the program as "nasm"), the working folder's name, exit code, messages
//     <folder>/<id>/in/<file>      the sources it read: the working folder's and each -I folder's .asm/.inc/.mac/.s/.def
//     <folder>/<id>/out/<file>     what it wrote: the -o file, the -l listing, a [map ... FILE] map
//
// The id is the SHA-1 of the arguments and the sources, so the same run seen twice is one case. process::run calls
// `before` and `after` around every child; nothing happens unless the variable is set and the program is NASM.
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/process.hpp"

namespace cb::process::nasm_capture {

// A run being captured: where its case goes, where it ran, the sources it was given.
struct Pending {
    std::filesystem::path folder;
    std::filesystem::path cwd;
    std::vector<std::pair<std::string, Bytes>> inputs;   // (name as the case keeps it, bytes)
    std::filesystem::file_time_type started;             // an output older than this was not written by the run
};

// Null unless this run is to be captured. Never throws: capturing must not change what the run does.
std::unique_ptr<Pending> before(const ProcessSpec& spec);
void after(std::unique_ptr<Pending> pending, const ProcessSpec& spec, const ProcessResult& result);

}  // namespace cb::process::nasm_capture
