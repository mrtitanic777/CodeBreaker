// cb/source/dos_build.hpp -- `dos-build`: a real-mode program made again from the folder `dos-source` wrote.
//
//     codebreaker dos-build <source folder> [--against ORIGINAL]
//
// Assembles and links the program from its folder alone -- the .asm, .lnk, .build.json and the side files `dos_writer::reconstruct` left -- and lays
// the file out as the original is, without the original. With --against, the file made is compared with the original byte for byte (exit code 1
// when it differs).
#pragma once

#include "cb/core/cli.hpp"
#include "cb/source/dos_writer.hpp"

namespace cb::source::dos_build {

// The tools the program is assembled and linked with (OpenWatcom's); nothing else of the writer is used.
cli::Command command(const dos_writer::Options& options);

}  // namespace cb::source::dos_build
