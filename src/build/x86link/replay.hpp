// build/x86link/replay.hpp -- the proof of x86link (docs/X86LINK_DESIGN.md): every wlink run CodeBreaker made, as
// core/wlink_capture saved it (<corpus>/<id>/case.json, out/, <corpus>/blobs/<sha1>), linked again in-process and
// compared with what wlink.exe wrote.
//
//     for (const replay::Outcome& o : replay::all(corpus, scratch)) if (!o.same) cb::out("{}: {}\n", o.id, o.why);
//
// A case is set up in a folder of its own under `scratch`: each input where the run had it, its recorded absolute path
// mapped into the folder (the working folder to <case>/cwd, WATCOM to <case>/watcom, any other folder to
// <case>/other/<n>), and the same mapping made in the arguments, the environment and the text of the link scripts. Then
// x86link links it there, and the case is the same when the exit code, every output file (byte for byte) and the
// messages (the folders mapped back; wlink's banner line, which carries its build time, left out) are.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace cb::build::x86link::replay {

struct Outcome {
    std::string id;     // the case's folder name
    bool same = false;
    bool skipped = false;   // the capture lacks an input: not linked again (why says which)
    std::string why;    // what differed (empty when the same)
};

// `watcom`: the OpenWatcom the runs used (its rel folder): its libraries and wlink.lnk are read from there.
Outcome one(const std::filesystem::path& case_dir, const std::filesystem::path& blobs, const std::filesystem::path& scratch,
            const std::filesystem::path& watcom);
std::vector<Outcome> all(const std::filesystem::path& corpus, const std::filesystem::path& scratch, const std::filesystem::path& watcom);

}  // namespace cb::build::x86link::replay
