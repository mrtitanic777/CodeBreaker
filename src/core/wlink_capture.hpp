// core/wlink_capture.hpp -- the cases x86link (docs/X86LINK_DESIGN.md) is proven against: every run of OpenWatcom's wlink
// CodeBreaker makes, saved while CB_WLINK_CAPTURE names a folder.
//
//     <folder>/<id>/case.json     the arguments, the working folder, the environment wlink reads (WATCOM, LIB), the exit code
//                                 and messages, and for each input its path as the run named it and its content's hash
//     <folder>/<id>/out/<file>    what it wrote: the `name` file and a map
//     <folder>/blobs/<sha1>       every input once (the link scripts, the objects, the libraries, wlsystem.lnk), shared by
//                                 the cases: OpenWatcom's libraries are kept once however many links read them
//
// The id is the SHA-1 of the arguments and the inputs, so the same link seen twice is one case. process::run calls `before`
// and `after` around every child; nothing happens unless the variable is set and the program is wlink.
//
// The same for OpenWatcom's C compilers (the proof of compiling in-process): while CB_WCC_CAPTURE names a folder, every run
// of wcc or wcc386 is kept the same way -- its sources, the headers beside them and in -i= folders (not OpenWatcom's own),
// and the object and error file it writes.
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/process.hpp"

namespace cb::process::wlink_capture {

struct Pending {
    std::filesystem::path folder;    // the case's
    std::filesystem::path root;      // CB_WLINK_CAPTURE
    std::filesystem::path cwd;
    std::filesystem::file_time_type started;
    std::vector<std::pair<std::string, std::filesystem::path>> inputs;   // (the path as the run named it, the file)
    std::vector<std::string> outputs;                                     // the files it is to write (name, map)
};

// Null unless this run is to be captured. Never throws: capturing must not change what the run does.
std::unique_ptr<Pending> before(const ProcessSpec& spec);
void after(std::unique_ptr<Pending> pending, const ProcessSpec& spec, const ProcessResult& result);

}  // namespace cb::process::wlink_capture
