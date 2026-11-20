// verify/reconstruct.hpp -- the path from a module to judged reconstructions of its functions.
//
//     reconstruct::Kinds kinds;                              // one lane per kind of module (verify/lanes/*)
//     kinds.add(std::make_shared<lanes::DosLane>(writer, tools));
//     reconstruct::Reconstructor rec({.kinds = kinds, .store = store, .kind_of_file = ..., .content_sha = ..., .scratch = ...});
//     reconstruct::Outcome done = rec.run(module_path);       // the results are in the function store
//
// Decompile the module, build the functions that have no verdict yet for the text they have now, run each beside the
// original on the same inputs, and record what was found. Functions already judged for their current text are not
// built or run again. Functions that are a runtime library's (the C runtime, compiler helpers) are recorded as such, in
// addition: they are decompiled, built and judged like every other function.
//
// The path is the same whatever the module is. What differs between kinds of module -- 32-bit or 16-bit Windows,
// PowerPC, drivers, real-mode programs, cartridges, Java programs -- is one lane per kind (lanes/kind.hpp), which this
// module is given in a `Kinds` registry; what a lane adds to that interface (`KindTraits`) is said when it is added.
//
// What results for a module are filed under is its identity: the content hash and the digest of the verifier's
// settings for that kind of file. `Reconstructor::identity` says it, and `identity_hook` is that as the function
// `store::VersionsLedger` (store/versions_ledger.hpp) wants in its Services:
//
//     store::VersionsLedger ledger({.corpus = corpus, .store = store,
//                                   .identity = reconstruct::identity_hook<compare::versions::Ledger::Identity>(rec)});
//
// (the hook borrows `rec`: it must outlive the ledger).
//
// Not part of this module: the engine's own way of making the same results (engine/lanes, step by step with
// the facts of the program database) belongs to that module; and the judge's worker processes,, which would be the answer to
// emulators keeping state per process -- a lane's judge is thread-safe now (kind.hpp), and `Services::judge_batch` is the
// place for a caller that wants processes (cb/jobs/workers.hpp) anyway.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "engine/function_store.hpp"
#include "verify/lanes/kind.hpp"
#include "verify/verdicts.hpp"

namespace cb::platforms {
class Registry;
}

namespace cb::verify::reconstruct {

// Who a candidate in the store came from.
inline constexpr std::string_view kSource = "decompiler";

// ---- the lanes ----------------------------------------------------------------------------------------------------------

// What a lane says besides lanes::Kind.
struct KindTraits {
    // Each function has its own text and is built and judged with what it reaches, so a verdict is kept while that stays
    // the same (the signatures are per function); else one signature, the preparation's, stands for every function.
    bool per_function = false;
    // The kind has runtime-library functions to identify (a folder of Java source has none).
    bool identifies_library = true;
    // What the candidates of this kind are filed as made by (their revision); default: Services::decompiler_rev.
    std::optional<std::string> source_rev;
    // What a module's results are filed under when it is not the file's SHA-256 (a Java program is a folder).
    std::function<std::optional<std::string>(const std::filesystem::path&)> sha;
    // {function name: digest of the function's compiled code} of the build in `work` (a per-function kind): with it a
    // function whose text changed to no effect on its code keeps its verdict.
    std::function<std::map<std::string, std::string>(const std::filesystem::path& work)> object_digests;
};

// The lanes by the name results are filed under, in the order added.
class Kinds {
public:
    // Replaces a lane of the same name (the digests that depend on it are asked again).
    void add(std::shared_ptr<const lanes::Kind> kind, KindTraits traits = {});
    const lanes::Kind* find(std::string_view name) const;
    const KindTraits& traits(std::string_view name) const;   // the defaults for a lane that said nothing
    std::vector<std::string> names() const;
    std::uint64_t generation() const { return generation_; }

private:
    struct Entry {
        std::shared_ptr<const lanes::Kind> kind;
        KindTraits traits;
    };
    std::vector<Entry> entries_;   // a handful, searched by name; the order added is the order reported
    std::uint64_t generation_ = 0;
};

// The verifier settings in force per kind, from the lanes alone (no function store needed): what Reconstructor::digests() / profiles() say.
std::map<std::string, std::optional<std::string>> digests_of(const Kinds& kinds);
std::map<std::string, lanes::Profile> profiles_of(const Kinds& kinds);

// ---- what a run is given --------------------------------------------------------------------------------------------------

struct Services {
    const Kinds& kinds;
    engine::FunctionStore& store;
    // Which kind a file is ("pe", "ne", "dos" ...): nullopt when nothing reconstructs it. `platform_kind` makes it from a
    // platform registry.
    std::function<std::optional<std::string>(const std::filesystem::path&)> kind_of_file;
    // The content hash a module's results are filed under unless its kind says another (the corpus's SHA-256).
    std::function<std::optional<std::string>(const std::filesystem::path&)> content_sha;
    // Where a run keeps its work: <scratch>/reconstruct/<kind>/<module> (build::watcom::scratch_dir()).
    std::filesystem::path scratch;
    // {function key: {name, library}} of the functions of a module that are a runtime library's. Unset: none are known.
    std::function<std::map<std::string, std::pair<std::string, std::string>>(const std::filesystem::path&)> identify = {};
    // The decompiler's revision, the revision of candidates whose kind says none.
    std::string decompiler_rev;
    // How many functions are judged at once: 1 judges one at a time in the calling thread.
    int judge_threads = 1;
    // Judges the named functions of a build. Unset, or answering nullopt: `judge_threads` threads call built.judge. A caller
    // that keeps emulators in worker processes (cb/jobs/workers.hpp) does it here -- `module` and `work` are what a worker prepares and
    // builds the module again from -- and a function it gives no answer for is not run.
    struct JudgeBatch {
        const lanes::Kind& kind;
        const lanes::Built& built;
        const std::vector<std::string>& names;
        const std::filesystem::path& module;
        const std::filesystem::path& work;
    };
    std::function<std::optional<std::map<std::string, Judgement>>(const JudgeBatch&)> judge_batch = {};
    // Where the one line about a run goes (nullopt: nowhere).
    std::function<void(std::string_view)> log = {};
};

// What results for a module are filed under.
struct Identity {
    std::string sha, profile;
};

// What a run did.
struct Outcome {
    std::string sha, profile, name;
    std::size_t functions = 0;                    // every function of the module
    std::size_t judged = 0;                       // judged now (the others had a verdict for their text)
    std::size_t kept = 0;                         // verdict kept: the code built is the code that was judged
    std::map<Class, std::int64_t> scores;         // the module's functions by class, runtime library on its own
    double seconds = 0, decompile_seconds = 0, build_seconds = 0, judge_seconds = 0;

    // The line `run` logs: "lz32.dll: 63 functions, 5 judged now, 3 kept (same code): Verified 40 ...  (12s: decompile 1, build 6, judge 5)".
    std::string line() const;
};

// ---- the reconstructor ------------------------------------------------------------------------------------------------------

class Reconstructor {
public:
    explicit Reconstructor(Services services) : services_(std::move(services)) {}

    // Which kind a file is, or nullopt when nothing reconstructs it.
    std::optional<std::string> kind_name(const std::filesystem::path& module_path) const;
    // The lane of a file; UsageError when nothing reconstructs that kind of file yet.
    const lanes::Kind& kind_of(const std::filesystem::path& module_path) const;
    // What a module's results are filed under: the kind's own hash, else the corpus's; "" when neither knows the file.
    std::string module_sha(std::string_view kind, const std::filesystem::path& module_path) const;

    // {kind: digest of the verifier settings in force now}: nullopt for a kind whose tools are not set up on this machine.
    // Asked once, and again when a lane is added.
    std::map<std::string, std::optional<std::string>> digests() const;
    // {kind: the verifier profile (digest and the settings it was made from) in force now}: a kind whose tools are not set up here is left out.
    // What `codebreaker migrate-state` matches a state folder's older profiles against.
    std::map<std::string, lanes::Profile> profiles() const;
    // (content hash, profile digest) of a module; UsageError for a file nothing reconstructs.
    Identity identity(const std::filesystem::path& module_path) const;
    // The same for a caller that treats "nothing reconstructs it" (and "that lane's tools are not set up") as no identity.
    std::optional<Identity> try_identity(const std::filesystem::path& module_path) const;

    // Reconstruct and judge a module: the results are in the store.
    Outcome run(const std::filesystem::path& module_path) const;

    const Services& services() const { return services_; }

private:
    Services services_;
    mutable std::map<std::string, std::optional<std::string>> digests_;
    mutable std::uint64_t digests_generation_ = ~std::uint64_t{0};
};

// try_identity as the function store/versions_ledger.hpp takes: `Id` is its Identity ({sha, profile}).
template <class Id>
std::function<std::optional<Id>(const std::filesystem::path&)> identity_hook(const Reconstructor& reconstructor) {
    return [&reconstructor](const std::filesystem::path& module) -> std::optional<Id> {
        const std::optional<Identity> id = reconstructor.try_identity(module);
        if (!id)
            return std::nullopt;
        return Id{.sha = id->sha, .profile = id->profile};
    };
}

// ---- what the signatures are made of ------------------------------------------------------------------------------------------

// {name: what that function's verdict was reached for}: its own text, the header, and the text of every function it can
// reach -- it is built and run together with those, so a change in one of them is a change in what was judged. A function
// none of that changed for keeps its verdict, however much of the rest of the module changed. Functions that call each
// other in a ring are one group with one signature. `whole` is the signature for a module that is not made of separate texts.
//
// With `own` {name: digest of the function's compiled code}, that stands in the signature in place of the function's text
// and the header: the signature is then of what was BUILT, and does not change when the source changes to no effect on the
// code (a declaration added to the header, a comment, a rename).
std::map<std::string, std::string> signatures(const lanes::Prepared& prepared, std::string_view whole,
                                              const std::map<std::string, std::string>* own = nullptr);

// The names in `built` that call a function that was dropped (now a stand-in), directly or through others.
std::set<std::string> reaches(const std::unordered_map<std::string, std::string>& texts, std::span<const std::string> built,
                              const std::set<std::string>& dropped);

// The kind of a file by the platform registry (the platform's name when it has a lane).
std::function<std::optional<std::string>(const std::filesystem::path&)> platform_kind(const platforms::Registry& registry);

}  // namespace cb::verify::reconstruct
