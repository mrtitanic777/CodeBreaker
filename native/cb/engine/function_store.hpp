// cb/engine/function_store.hpp -- the function store: every function of every module, and what was made of it.
//
//     FunctionStore store("state/functions.sqlite", "state/db");
//     store.add_module(sha, "kernel32.dll", path, "pe");
//     store.record_many(sha, "decompile", rev, profile, {{.addr = "1:7cd4", .name = "_f", .source = c, .sig = sig, .verdict = ...}});
//     auto best = store.best(sha, profile, {"decompile"});            // the best candidate of each function, by class
//     auto tally = store.tally(profile, sha);                         // {class: how many functions}
//
// A module's tables (function, candidate, verdict, text, identified, built, variant) are in that binary's program database
// (<folder>/<sha>.sqlite, the file engine/facts keeps its facts in); the store's own file is their index (module, profile,
// summary, store_about). A *function* is one address of one module; a *candidate* is what one engine (a decompiler's
// version, a hand edit) made of it; a *verdict* is what a judge said of a candidate under one profile (a set of settings);
// a *variant* is a rebuilt candidate whose bytes were compared with the original's. The schema is the one the old C engine
// (cbengine.dll) wrote, so a store it wrote reads the same.
//
// Verdict and class words are the vocabulary of verify/verdicts: a stored word in an older spelling reads as the word now, and
// is written back as the word.
//
// The store from before the module databases (its function, candidate and verdict tables in the index itself) is not carried
// over any more: that was a one-time move, done at the milestone of 2026-11-04.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/verify/verdicts.hpp"

namespace cb::engine {

// One engine's candidate for a function, with its verdict under one profile (none when it has not been judged under it).
struct Candidate {
    std::string addr, engine;
    std::optional<std::string> rev, sig, name, source, problem;
    std::optional<std::string> verdict;              // the judge's word, as stored now ("equivalent", "divergent" ...)
    std::string reason, limits, detail;              // the verdict's; empty when there is none
    std::optional<std::string> judged_sig;           // the sig of the candidate when it was judged

    // What the verdict amounts to (no source when there is none).
    verify::Classification classification() const;
};

// A function's best candidate under a profile, with the class it counts as.
struct Best {
    Candidate candidate;
    verify::Class cls = verify::Class::NoSource;
    verify::WeakLimits weak;                         // why a weak match is weak
};

// One candidate of a function with each verdict it has (stored()).
struct StoredVerdict {
    std::string profile;
    std::optional<std::string> verdict;
    std::string reason, limits;
    bool carried = false;                            // moved over from the store before the module databases
};
struct Stored {
    std::string engine;
    std::optional<std::string> rev, name;
    std::string source, problem;
    std::vector<StoredVerdict> verdicts;             // newest first
};

// A function as record_many takes it.
struct Record {
    std::string addr;
    std::optional<std::string> name, source, sig;
    std::optional<std::string> verdict;              // any spelling the vocabulary reads; stored as the word
    std::string reason, limits, detail;
    std::optional<std::string> problem;
};

// A module's tally of its functions by class.
using ClassCount = std::map<verify::Class, std::int64_t>;

// A byte-match variant of a function.
struct Variant {
    std::string addr, compiler;
    std::optional<std::string> source;
    std::optional<std::int64_t> parent;              // the variant it was made from
    std::string knobs;
    std::optional<std::int64_t> distance, total;     // bytes that differ, bytes compared
    std::optional<std::string> match;                // the byte comparison's word ("identical", "same size" ...)
    std::optional<std::string> behaves;              // the judge's word for it, "" when it was not judged
};

// What a module is in the index.
struct ModuleRow {
    std::string sha, name, path, kind;
};

// The index's summary of a module, kept up to date by every write.
struct Summary {
    std::int64_t functions = 0, identified = 0, judged = 0;
    std::optional<double> last;                      // when a verdict was last recorded (seconds since 1970)
    std::optional<std::string> last_profile;
    std::vector<std::string> profiles;               // sorted
    ClassCount latest;                               // by each function's latest verdict under any profile
};

struct Overview {
    std::string sha, name, kind, path;
    std::int64_t functions = 0, identified = 0;
    ClassCount latest;                               // no-source counts the functions with no verdict at all
};

struct RecentModule {
    std::string name, kind;
    std::int64_t judged = 0;
    double last = 0;
};

class FunctionStore {
public:
    // The index in `index_file` and the modules' databases in `folder`. Opened for writing the files and folders are made;
    // `readonly`: a store that is not there reads as empty and any write is an IoError.
    FunctionStore(std::filesystem::path index_file, std::filesystem::path folder, bool readonly = false);
    ~FunctionStore();
    FunctionStore(const FunctionStore&) = delete;
    FunctionStore& operator=(const FunctionStore&) = delete;

    const std::filesystem::path& path() const;
    const std::filesystem::path& folder() const;
    bool readonly() const;
    // A module's database file (<folder>/<module>.sqlite).
    std::filesystem::path database_path(std::string_view module) const;
    // The module's database with its tables made (writable stores), for a caller that opens it itself; nullopt when
    // a read-only store has none.
    std::optional<std::filesystem::path> module_database(std::string_view module);

    // ---- writing
    void add_module(std::string_view sha, std::string_view name, std::string_view path = "", std::string_view kind = "");
    void add_profile(std::string_view digest, std::string_view settings);
    void add_functions(std::string_view module, std::span<const std::string> addrs);
    // What an engine made of the functions, judged under `profile`: a candidate whose sig changed loses its verdicts under
    // other profiles. The module's summary is brought up to date.
    void record_many(std::string_view module, std::string_view engine, std::optional<std::string_view> rev,
                     std::string_view profile, std::span<const Record> records);
    void record(std::string_view module, std::string_view engine, std::optional<std::string_view> rev,
                std::string_view profile, const Record& record);
    void put_text(std::string_view module, std::string_view engine, std::string_view kind, std::string_view body);
    void set_built(std::string_view module, std::string_view profile, const std::map<std::string, std::string>& sigs);
    // Which functions are runtime library's, by name and library (replaces the module's list).
    void set_identified(std::string_view module, const std::map<std::string, std::pair<std::string, std::string>>& found);
    // The variant's id.
    std::int64_t record_variant(std::string_view module, std::string_view addr, std::string_view compiler, std::string_view source,
                                std::optional<std::int64_t> distance, std::optional<std::int64_t> total, std::string_view match,
                                std::string_view knobs = "", std::optional<std::int64_t> parent = std::nullopt,
                                std::string_view behaves = "");

    // ---- reading a module
    // Every candidate of the module with its verdict under `profile`; those of one engine when named.
    std::vector<Candidate> candidates(std::string_view module, std::string_view profile, std::optional<std::string_view> engine = std::nullopt);
    // The candidates whose verdict under `profile` is of the sig they have now, by address.
    std::map<std::string, Candidate> judged(std::string_view module, std::string_view profile, std::string_view engine);
    // Every candidate of a function.
    std::vector<Stored> stored(std::string_view module, std::string_view addr);
    // The body put_text kept, "" when none.
    std::string get_text(std::string_view module, std::string_view engine, std::string_view kind);
    // {address: sig} of what was built under `profile`.
    std::map<std::string, std::optional<std::string>> built(std::string_view module, std::string_view profile);
    // The best candidate of each judged function: the best class first, then the engine named earliest in `prefer`.
    std::map<std::string, Best> best(std::string_view module, std::string_view profile, std::span<const std::string> prefer = {});
    // {address: class} by each function's latest verdict under any profile.
    std::map<std::string, verify::Class> latest(std::string_view module);
    // Every function's address, sorted.
    std::vector<std::string> addresses(std::string_view module);
    // How many functions are of each class under `profile` (best candidate of each, no source for the rest): every class of
    // verify::kClassOrder is a key.
    ClassCount tally(std::string_view profile, std::string_view module, std::span<const std::string> prefer = {});
    // The same with the runtime library's functions counted on their own (a verified or weak one stays that): every class of
    // verify::kClassScored is a key.
    ClassCount scores(std::string_view profile, std::string_view module, std::span<const std::string> prefer = {});
    // {address: {name, library}} of the functions recognised as a runtime library's.
    std::map<std::string, std::pair<std::optional<std::string>, std::optional<std::string>>> identified(std::string_view module);
    std::vector<Variant> variants(std::string_view module, std::optional<std::string_view> compiler = std::nullopt);
    // The best variant of each function: one that behaves (or was not judged) first, then the fewest bytes apart.
    std::map<std::string, Variant> best_variant(std::string_view module, std::string_view compiler);
    struct MatchTally {
        std::int64_t functions = 0, identical = 0, same_size = 0, other = 0;
    };
    MatchTally match_tally(std::string_view module, std::string_view compiler);

    // ---- across modules (the index)
    std::map<std::string, Summary> summaries();
    // {sha, name} of every module by name; with a profile, those judged under it.
    std::vector<ModuleRow> modules(std::optional<std::string_view> profile = std::nullopt);
    // Every module (path and kind too) by name; of one kind when named.
    std::vector<ModuleRow> module_rows(std::optional<std::string_view> kind = std::nullopt);
    std::optional<std::string> module_path(std::string_view name);
    // The profile the latest verdict of a module of `kind` was under.
    std::optional<std::string> latest_profile(std::string_view kind);
    // The modules last judged, newest first.
    std::vector<RecentModule> recent(std::int64_t limit = 8);
    // How many verdicts were recorded after `t`.
    std::int64_t verdicts_since(double t);
    // Every module at a glance, by name.
    std::vector<Overview> overview();
    std::optional<std::string> about(std::string_view name);
    void set_about(std::string_view name, std::string_view value);

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::engine
