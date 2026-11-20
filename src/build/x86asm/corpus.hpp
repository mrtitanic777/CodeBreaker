// build/x86asm/corpus.hpp -- x86asm's acceptance (docs/X86ASM_DESIGN.md): the captured NASM runs, replayed through x86asm.
//
// A case (a folder: case.json, in/, out/) was captured from the reference nasm.exe, while it still existed, and holds a run's arguments, the files it read under the paths it read them by, and what the
// reference nasm.exe wrote. The replay hands x86asm the same arguments and the same files under the same names -- no path
// is rewritten, so an object that records its source's name must record the same one -- and compares every output
// byte for byte, the exit code, and the messages.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "build/x86asm/x86asm.hpp"

namespace cb::build::x86asm::corpus {

struct Case {
    std::string id;                                   // the case's folder name
    std::filesystem::path cwd;                        // where the reference run was
    Request request;                                  // the arguments and the files, as the run named them
    int exit = 0;                                     // the reference's
    std::string messages;                             // the reference's stderr then stdout
    std::vector<std::pair<std::string, Bytes>> outputs;   // (the name the run wrote it under, the reference's bytes)
};

// FormatError for a folder that is not a case.
Case load(const std::filesystem::path& folder);

// Empty when x86asm does what the reference did; else each difference, in a line.
std::vector<std::string> compare(const Case& c, const Result& made);

// Every case under `root` (each folder with a case.json), by id.
std::vector<std::filesystem::path> cases(const std::filesystem::path& root);

}  // namespace cb::build::x86asm::corpus
