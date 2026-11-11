// cb/ui/progress.hpp -- watch a long run from outside it.
//
//     codebreaker progress                 one status line, rewritten every few seconds
//     codebreaker progress --once          print the status once and stop
//     codebreaker progress --interval 10   seconds between readings (default 3)
//     codebreaker progress --log FILE      also show the last line of a run's output file
//
// A sweep or a module run is not asked to report anything: every result it produces goes into the function store as
// it is made, so the store is the progress. A reading says, per kind of module that can be reconstructed, how many
// modules of the corpus are measured under the verifier settings in force and how their functions stand (from the
// coverage ledger); which module results were last written for and how long ago; how many results arrived in the
// last ten minutes, and from that a rough time to the end of the corpus; and how many of the runner's worker
// processes are in the middle of an item (the job runner keeps a note per worker in the temporary folder). The store
// is opened read-only, so watching cannot disturb the run.
//
// The store and the ledger are other modules; this one is handed what they say (`StoreReading`, filled by a `ReadFn`
// the application supplies) and does the rest: the worker count, the log's last line, the rate, the estimate, the
// text. What the terminal interface and the MCP server show is a `Snapshot`.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <stop_token>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"

namespace cb::ui::progress {

using Clock = std::chrono::system_clock;

// The span of results the rate is taken over.
inline constexpr std::chrono::seconds kWindow{600};

// How many functions of a kind stand in one of the store's classes (verified, weak, ...).
struct ClassCount {
    std::string id;      // "verified"
    std::string title;   // "Verified"
    std::uint64_t count = 0;
};

// One kind of module in a reading.
struct KindReading {
    std::string kind, name;
    std::uint64_t modules = 0, measured = 0, functions = 0;
    std::vector<ClassCount> classes;   // every class of the store, best first
};

// A module whose results were written lately, as the store reports it.
struct RecentResult {
    std::string module;
    std::string kind;                  // the kind's title
    std::uint64_t results = 0;
    Clock::time_point last_written;
};

// What the function store and the coverage ledger say right now.
struct StoreReading {
    std::string store_path;
    std::vector<KindReading> kinds;    // only the kinds that have a verifier profile in force
    bool opened = false;               // false: there is no store yet, so there is nothing to read
    std::vector<RecentResult> recent;  // newest first, a handful
    std::uint64_t fresh = 0;           // results written within kWindow before the reading
};
// Reads the store at a time. The application supplies it (it knows where the store is).
using ReadFn = std::function<StoreReading(Clock::time_point now)>;

struct Recent {
    std::string module, kind;
    std::uint64_t results = 0;
    std::chrono::seconds age{0};
};

struct Snapshot {
    std::string time;                           // "HH:MM:SS", local
    std::vector<KindReading> kinds;
    std::vector<Recent> recent;
    double rate = 0.0;                          // results per minute over the window, to a tenth
    std::optional<std::chrono::seconds> idle;   // since the last result; none when there are none
    std::uint64_t workers = 0;                  // job-runner workers in the middle of an item
    std::optional<std::chrono::seconds> eta;    // none when nothing is arriving
    bool active = false;
    std::string log;                            // the last line of the run's output file, when one is named
    std::string store;
};

// Job-runner workers that are in the middle of an item right now: `<temp>/cbjobs*/*/*.cur` notes that are not empty
// and were written within the hour.
std::uint64_t busy_workers(const std::filesystem::path& temp, Clock::time_point now);

// The last non-blank line of the end of a file (trimmed); "" when the file cannot be read.
std::string last_line(const std::filesystem::path& file);

// One reading. `log`, when given, is a run's output file whose last line is shown.
Snapshot snapshot(const ReadFn& read, const std::optional<std::filesystem::path>& log = std::nullopt,
                  Clock::time_point now = Clock::now());

// "1h 05m" or "3m 07s"; "-" for none.
std::string clock_text(std::optional<std::chrono::seconds> span);

// A reading as one line of text.
std::string line(const Snapshot& s);

struct Options {
    std::chrono::duration<double> interval{3.0};   // seconds between readings (at least one)
    std::optional<std::filesystem::path> log;
    bool once = false;
};

// Print readings until `stop` is requested (or once). On a console the line is rewritten in place; otherwise each
// reading is a line of its own. Returns the exit code.
int watch(const Options& options, const ReadFn& read, std::stop_token stop = {});

// The declared command (`progress [--once] [--interval SECONDS] [--log FILE]`).
cli::Command command(ReadFn read, std::stop_token stop = {});

}  // namespace cb::ui::progress
