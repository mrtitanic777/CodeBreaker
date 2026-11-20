// build/x86asm/x86asm.hpp -- CodeBreaker's own x86 assembler (docs/X86ASM_DESIGN.md): what the rest of CodeBreaker calls.
//
//     x86asm::Request r;
//     r.args = {"-f", "bin", "-o", "lz32.dll", "lz32.dll.asm"};      // NASM's command line (see `codebreaker nasm`)
//     r.files["lz32.dll.asm"] = source;                              // the files the run may read, by the name it uses
//     x86asm::Result out = x86asm::assemble(r);
//     if (out.exit == 0) image = out.files.at("lz32.dll");          // and the map, the listing ... by the names asked for
//
// Everything is in memory: the run reads only `files` (and, where `read` is set, what it returns for other names: an
// %include of a file on disk) and writes only into `Result::files`. It behaves as NASM 3.02 with CodeBreaker's MASM mode
// (`--masm`) did for the same command line: the same bytes, the same messages, the same exit code. Runs share nothing:
// any number may run at once.
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"

namespace cb::build::x86asm {

struct Request {
    std::vector<std::string> args;                       // NASM's arguments (not the program's name)
    std::string cwd;                                     // the folder a relative name is in (a name only: nothing is read from it)
    std::map<std::string, Bytes> files;                  // full path, '/'-separated -> contents (a name the run asks for is
                                                         // made full against `cwd` and looked up here, case-blind on Windows)
    // Files the run asks for that are not in `files` (an %include, a -I folder): nullopt when there is none.
    std::function<std::optional<Bytes>(const std::string& name)> read;
};

struct Result {
    int exit = 0;                                        // NASM's exit code: 0 assembled, 1 errors, 2+ fatal
    std::map<std::string, Bytes> files;                  // every file written, by the name the run was given for it
                                                         // (`-o x.obj` -> "x.obj"): the output, the listing, the map ...
    std::string messages;                                // what NASM wrote to stderr (and stdout), in its words
};

Result assemble(const Request& request);

}  // namespace cb::build::x86asm
