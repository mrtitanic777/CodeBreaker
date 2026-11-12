// cb/engine/lanes.hpp -- every lane's reconstruct-and-judge as steps of the engine, and the 32-bit module build.
//
//     Steps steps;  Engine engine(steps, databases, digests, jobs);
//     Lanes lanes(steps, engine, kind_of_file);                        // kind_of_file: which lane a file is ("pe", "ne", ...)
//     lanes.declare({.kind = pe_lane, .traits = {.per_function = true}, .platform = "win32"});
//     Judged judged = lanes.judged("lz32.dll");                        // the rows reconstruct::Reconstructor::run records
//
// A lane (verify/lanes/kind.hpp) is four calls: profile, prepare, build, judge. The same steps serve every lane -- 32-bit
// Windows, 16-bit Windows' judge, the drivers, DOS, the Xbox 360, the consoles -- filed under the name its results are
// (K: pe, ne, le, dos, powerpc, nes, ...); each asks the lane for the part that is its own:
//
//     binary                    the file (given)
//     lane.K.profile            the lane's profile: [digest, settings] -- everything but the source a verdict depends on;
//                               asked every run                                                  <- binary
//     lane.K.module             the module decompiled (the lane's prepare), as a document: header, order, keys, texts, sig
//                                                                                                <- binary
//     lane.K.keys               the keys of its functions with C, and of all                     <- lane.K.module
//     lane.K.function <key>     one function: [name, C] -- or given by hand (Engine::put): the build and the rows then have
//                               that C                                                           <- lane.K.module
//     lane.K.texts              every function's name, signature and C as they now are, and the header
//                                                                                                <- lane.K.module, .function
//     lane.K.build              the functions built (the lane's build): each function's name, why the others are not in it,
//                               the names that reach a stand-in, and each function's built signature (its compiled code and what
//                               it reaches)                                                      <- lane.K.module, .function
//     lane.K.built <key>        what a function's verdict is reached for: ["built", its built signature], or (a lane that builds
//                               the module as one) its signature; or ["dropped", why it is not in the build]    <- lane.K.build
//     lane.K.verdict <key>      the lane's judge: [verdict, reason, limits, detail]              <- lane.K.built, lane.K.profile
//     lane.K.rows               the rows reconstruct records in the function store               <- all of the above
//
// So a verdict is made again exactly when the store would judge the function again: its built code (or, for a lane that does not
// build per function, its text) or the lane's profile changed. A function whose C changed to no effect on its code keeps its
// verdict, as reconstruct kept it by the store's `built` table.
//
// The 32-bit module build (the lane "pe", the packager's link):
//
//     pe.image                  the module linked and checked: {name, report, rejected, image: the file's name in the build folder}
//                                                                                          <- lane.pe.module, .keys, .function
//
// What a fact is, here: a document. The decompiled module of the Python was pickled into its fact; here the fact is the document
// (ModuleDoc) and the live module -- the lane's own Prepared, which may carry what no document can -- is kept by this object in a
// table under the document's hash. A process that finds the fact kept and the module not (a new run) asks the lane to prepare it
// again, and so does a build asked for with functions given by hand (a Prepared cannot be copied: it may be a lane's own type).
// The same holds for the build the judge runs: the files are in the database's work folder, the judge in this object.
//
// The judge runs on threads (a lane's judge is thread-safe: verify/lanes/kind.hpp), `jobs` at a time.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/engine/engine.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/reconstruct.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::engine::lanes {

// The decompiled module as its fact keeps it.
struct ModuleDoc {
    std::string header;
    std::vector<std::string> order;                       // function names, by address
    std::unordered_map<std::string, std::string> key;     // name -> its address as the store's keys write it
    std::vector<std::string> all_keys;                    // every function's key, sorted
    std::unordered_map<std::string, std::string> texts;   // name -> the text it is judged for
    std::string sig;                                      // what a verdict was reached for: the source, and the lane's revision
};

// What one lane's steps need from outside.
struct Setup {
    std::shared_ptr<const verify::lanes::Kind> kind;
    verify::reconstruct::KindTraits traits;
    std::string platform;                           // the platform the steps are declared under ("win32")
    // The tools the lane's prepare and its build run and the settings they read, digested into the steps' versions ("" none).
    std::function<std::string()> module_stamp;
    std::function<std::string()> build_stamp;
};

// What the link of a 32-bit module made: the packager's link and the check of the linked image against the original.
struct Linked {
    std::string name;
    std::optional<Bytes> image;                     // the linked file; none: the link made none
    std::map<std::string, std::string> rejected;    // function -> why the compiler turned it down
    std::string report;
};
// Links the module (with each function's C as now it is) in `folder`. It does not change the module.
using PeLink = std::function<Linked(const verify::lanes::Prepared& module, const std::filesystem::path& binary, const std::filesystem::path& folder)>;

// One row reconstruct records in the function store.
struct JudgedRow {
    std::string addr;
    std::string sig;                                // "-" for a function the decompiler produced nothing for
    std::string name;
    std::optional<std::string> source;              // the C (none: nothing produced)
    verify::Verdict verdict = verify::Verdict::NoSource;
    std::string reason, limits, detail;             // limits: the codes joined by ","
    std::optional<std::string> problem;             // "none" (no source), "compile"
};

// What lane.K.rows gives, and how the run went.
struct Judged {
    std::string profile, settings, header;
    std::vector<std::string> all_keys;
    std::vector<JudgedRow> rows;
    std::map<std::string, std::string> built;       // key -> built signature
    std::vector<Run::Count> made;                   // per kind of fact: kept and made in this run
};

// What module build of a 32-bit module made.
struct PeBuild {
    std::string name;
    std::string report;
    std::map<std::string, std::string> rejected;
    std::optional<std::filesystem::path> image;     // the linked file, in `outdir` or the database's build folder
};

namespace detail {
struct LaneState;   // the live modules and builds, and what the steps are declared on
}

class Lanes {
public:
    // `kind_of_file` says which lane a file is (nullopt: none): reconstruct::platform_kind(registry) is that.
    Lanes(Steps& steps, Engine& engine, std::function<std::optional<std::string>(const std::filesystem::path&)> kind_of_file);
    ~Lanes();
    Lanes(const Lanes&) = delete;
    Lanes& operator=(const Lanes&) = delete;

    // Declares lane.K.profile ... lane.K.rows for the lane (replacing the steps of a lane of the same name); -> K.
    std::string declare(Setup setup);
    // Declares pe.image for the lane "pe" (declared before or after).
    void declare_pe_image(PeLink link, std::string platform = "win32", std::function<std::string()> stamp = {});

    // The lane names declared so far, in the order declared.
    std::vector<std::string> kinds() const;
    // Is this module's lane on these steps?
    bool on(const std::filesystem::path& module) const;

    // The module's database: a binary's by its bytes; a folder (a Java program's source) by what its lane files its results under.
    std::shared_ptr<facts::Database> open_module(const std::filesystem::path& module);

    // reconstruct's decompile, build and judge of a module, made by the engine. UsageError for a module no declared lane takes.
    Judged judged(const std::filesystem::path& module, const Run::Log& log = nullptr);

    // module build of a module of the lane "pe", made by the engine: the image in `outdir`, else the database's build folder.
    // UsageError for a module of another lane. (Running the built program is the caller's.)
    PeBuild build_pe(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir = std::nullopt,
                     const Run::Log& log = nullptr);

private:
    std::shared_ptr<detail::LaneState> state_;
};

// The signatures of a prepared module as reconstruct gives them: per function for a lane that builds per function, else the
// module's own for each.
std::map<std::string, std::string> signatures_of(const verify::lanes::Prepared& module, bool per_function);

// The module with `given` (name -> its C) in place of the texts it has: its texts and its sig (the sig changes with them). The
// document of the module as a lane's step keeps it.
ModuleDoc patched(const ModuleDoc& doc, const std::map<std::string, std::string>& given);

// ModuleDoc <-> the fact that keeps it. FormatError for a document that is not one.
knowledge::Value to_value(const ModuleDoc& doc);
ModuleDoc module_doc_of(const knowledge::Value& value);

}  // namespace cb::engine::lanes
