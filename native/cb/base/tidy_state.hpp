// cb/base/tidy_state.hpp -- how big state/ is, part by part, and what of it is stale, gone.
//
//     codebreaker tidy-state                 the sizes, and the stale removed
//     codebreaker tidy-state --dry           the sizes, and what would be removed
//     codebreaker tidy-state --stray         also remove what no part of CodeBreaker writes
//     codebreaker tidy-state --keep-builds   leave Converted/*/build/ alone
//
// state/ is CodeBreaker's to fill, and once filled 70 GB: fingerprints under comparer revisions long gone, millions of
// cache files, the disk and CD copies of killed boot runs. What fills it now looks after itself -- the cache holds its
// generation of now under a cap, and every run that copies a disk works in a run folder deleted when it ends
// (base/runs.hpp). This is the look over all of it, and the clean-up of what came before or slipped through. The rules:
//
//     cache        older generations of every kind dropped, the cap held (the cache's own prune, given as a hook)
//     old caches   the fingerprints' own file of before (state/fingerprints.sqlite), now in the cache
//     runs         run folders older than a day (killed runs), of every tool that has them, and those of before run
//                  folders were (phase3/walk_*, setup16_*, t_*; vxdboot/run_*; record16's memory dumps)
//     kept disks   an installed disk a Setup walk kept to compare (record16/walk/<tag>/hd.img), a week old
//     builds       the object files of a Converted/<name>/build/ not built for two weeks (the programs and their
//                  symbols stay; the next build compiles them again)
//     stray        anything at the top of state/ no part of CodeBreaker writes -- listed, removed only with --stray
//                  (it may be someone's hand work)
//
// What a run of the code would make again is all that goes; the stores (results.sqlite, functions.sqlite), hand work
// (java/overlays, xna/overlays) and the tools' outputs are left.
#pragma once
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::base::tidy_state {

// Where the things to tidy are.
struct Layout {
    std::filesystem::path state;       // CodeBreaker's state folder
    std::filesystem::path converted;   // the Converted folder (its <name>/build/ folders hold object files)
};

// What the stale-state rules cover.
enum class Rule { OldCaches, Runs, KeptDisks, Builds, Stray };
// The rule's name as the report prints it ("old caches", "runs", "kept disks", "builds", "stray").
std::string_view name(Rule rule);

// One thing the rules say can go.
struct Stale {
    std::filesystem::path path;
    std::uint64_t bytes = 0;
    Rule rule = Rule::Runs;
};

// One top-level part of state/ and its size.
struct Part {
    std::filesystem::path path;
    std::uint64_t bytes = 0;
};

// The names at the top of state/ that CodeBreaker's code writes. The list is curated here (it was read from the
// Python source before there was none to read), and a part that writes a new top-level folder adds its name; a name
// that is a SQLite file's also covers its -wal, -shm and -journal.
const std::set<std::string>& known_names();

// What the cache's own clean-up did: for each kind of cache entry, the old generations it dropped.
struct CachePruned {
    std::string kind;
    std::vector<std::string> generations;
};

// What the tidy-up asks of the cache, given by whoever owns it (the cache module, the application).
struct CacheHooks {
    std::function<std::vector<CachePruned>()> prune;   // drop old generations, hold the cap
    std::function<std::uint64_t()> max_bytes;          // the cap, for the report
};

using Clock = std::chrono::system_clock;

// Everything the rules say can go (the cache apart: it prunes itself). `now` is when "older than" is measured from.
std::vector<Stale> stale(const Layout& layout, Clock::time_point now = Clock::now());

// What is at the top of state/ that no part of CodeBreaker writes.
std::vector<Part> stray(const Layout& layout);

// state/'s top-level parts, the biggest first.
std::vector<Part> parts(const Layout& layout);

// Every Converted/<name>/build/ folder.
std::vector<std::filesystem::path> build_folders(const Layout& layout);

// The size of a file or of everything under a folder (what cannot be read counts for nothing).
std::uint64_t size_of(const std::filesystem::path& p);

struct Options {
    bool dry = false;           // report what would be removed, remove nothing
    bool remove_stray = false;  // also remove what no part of CodeBreaker writes
    bool keep_builds = false;   // leave Converted/*/build/ alone
};

// The report, and (unless `dry`) the clean-up. Writes to stdout. `cache` may be left empty (no cache to prune).
// Returns the exit code (always 0: nothing here fails the command).
int run(const Layout& layout, const Options& options, const CacheHooks& cache = {});

}  // namespace cb::base::tidy_state
