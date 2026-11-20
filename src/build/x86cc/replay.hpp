// build/x86cc/replay.hpp -- the proof of x86cc: every wcc / wcc386 run CodeBreaker made, as core/wlink_capture saved it
// (<corpus>/<id>/case.json with "program", out/, <corpus>/blobs/<sha1>), compiled again in-process and compared with what
// the program wrote.
//
// A case is set up in a folder of its own under `scratch` as x86link's are (build/x86link/replay.hpp): each input where
// the run had it, the recorded folders mapped into the case's (the working folder to <case>/cwd, WATCOM to the OpenWatcom
// given, any other folder to <case>/other/<n>), in the arguments and the environment too. The case is the same when the
// exit code, every output file (the object byte for byte, but for the times of the files it depends on: see replay.cpp)
// and the messages (the folders mapped back) are.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace cb::build::x86cc::replay {

struct Outcome {
    std::string id;     // the case's folder name
    bool same = false;
    std::string why;    // what differed (empty when the same)
};

Outcome one(const std::filesystem::path& case_dir, const std::filesystem::path& blobs, const std::filesystem::path& scratch,
            const std::filesystem::path& watcom);
std::vector<Outcome> all(const std::filesystem::path& corpus, const std::filesystem::path& scratch, const std::filesystem::path& watcom);

}  // namespace cb::build::x86cc::replay
