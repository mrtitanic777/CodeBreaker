// build/dos_run.hpp -- the contract for running DOS programs (the assemblers, compilers and linkers of the
// 1980s and 90s) on CodeBreaker's own DOS executive: what to mount, what to run, which files to bring back.
//
//     DosRunSpec spec;
//     spec.mounts = {{"c", tools_dir}};
//     spec.commands = {"CL /AM /c /FcT.COD /FoT.OBJ T.C > T.LOG 2>&1"};
//     spec.artifacts = {"T.OBJ", "T.COD", "T.LOG"};
//     spec.files["T.C"] = text;
//     DosRunResult r = runner.run(spec);
//
// `run` gives each folder a drive (the work folder is d: unless named; the last drive listed is the current one), runs
// the commands as a batch file's lines and reads the named files back. `ok` is false -- and nothing is pretended --
// when the executive could not complete a program (`why` says what stopped it) or a file was not made.
//
// The runner is build/native_dos_runner: the toolchain drivers in this folder (msvc152, period16) are written against
// this interface so they can be tested with a stand-in.
#pragma once

#include <chrono>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "core/ordered_map.hpp"

namespace cb::build {

// A host folder shown to DOS as a drive.
struct DosMount {
    std::string drive;   // "c"
    std::filesystem::path folder;
};

// A file written into the work folder before the commands run: text (written with DOS line ends) or bytes.
using DosFile = std::variant<std::string, Bytes>;

struct DosRunSpec {
    std::vector<DosMount> mounts;
    std::vector<std::string> commands;    // the lines of BLD.BAT
    std::vector<std::string> artifacts;   // files to read back from the work folder
    std::chrono::seconds timeout{90};
    std::map<std::string, DosFile> files; // work-folder files to write first (name -> contents)
    // Settings of the emulated machine (`cpu`, `dos`; the executive has its own and ignores them) and the variables set
    // before the commands run, each in the order they are written.
    OrderedMap<std::string, std::string> cpu;
    OrderedMap<std::string, std::string> dos;
    OrderedMap<std::string, std::string> variables;   // PATH, INCLUDE, LIB ...
    std::optional<std::filesystem::path> work;        // the folder to run in (default: a new one), kept
};

struct DosRunResult {
    bool ok = false;                      // every artifact was made
    std::string why;                      // when the executive could not complete the run: what stopped it
    std::map<std::string, Bytes> made;    // artifact name -> contents, for those that exist
    std::optional<std::filesystem::path> work;   // the folder it ran in, when it ran
};

class DosRunner {
public:
    virtual ~DosRunner() = default;
    // `ok` false (with `why`) when the run could not be completed.
    virtual DosRunResult run(const DosRunSpec& spec) const = 0;
};

}  // namespace cb::build
