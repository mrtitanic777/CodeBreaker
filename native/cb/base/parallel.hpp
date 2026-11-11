// cb/base/parallel.hpp -- how much of the machine CodeBreaker takes: the budget every runner asks before it starts.
//
//     budget::Limits limits{.workers = {.workers = 8}, .min_free_gb = 6, .slot_dir = state / "slots"};
//     ::cb::parallel::workers(limits.workers);          // how many things run side by side (cb/core/parallel.hpp)
//     {
//         Slots slots(limits, /*want*/ 6, /*mb_each*/ 256, "decompile");   // the machine's slots, shared by every
//         run slots.count() at once;                                       // CodeBreaker process
//     }                                                                    // the end of the scope gives them back
//     wait_for_memory(limits, "verify");                // before a heavy job: while memory is short, wait
//
// The work itself runs on cb::parallel::ThreadPool; this holds what everything asks before it starts: how many, and
// whether the machine has room. (How many threads, the process's priority and the free-memory reading are in
// cb/core/parallel.hpp; this adds the machine-wide governor.)
//
// The governor: one budget of heavy jobs for the whole machine. Each part used to decide on its own how much to run
// at once, so they multiplied: a sweep taking two binaries apart, each splitting its decompiler six ways, asked for
// twelve at once. Now a heavy job holds a slot while it runs -- the slots are the machine's workers(), shared by every
// CodeBreaker process (a locked file each: the system lets go of it when its process ends, even killed) -- and it is
// given no more than the free memory carries, at `mb_each` a piece, past the reserve the system keeps. A small machine
// runs one at a time; nobody waits for ever.
#pragma once
#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>
#include <string_view>

#include "cb/core/parallel.hpp"

namespace cb::base::budget {

// What the application's configuration contributes. The environment overrides it: CB_JOBS and CB_LOW_LOAD (see
// cb::parallel::workers) and CB_MIN_FREE_GB.
struct Limits {
    ::cb::parallel::WorkerPolicy workers;   // settings "workers" and "low_load"
    double min_free_gb = 6.0;               // settings "min_free_gb": below this much free memory no heavy job starts
    std::filesystem::path slot_dir;         // where the slot files live: <state>/slots (made when slots are taken)
};

// The memory below which no new heavy job starts: CB_MIN_FREE_GB, else limits.min_free_gb.
double min_free_gb(const Limits& limits);

// Before a heavy job: wait while the system has less than min_free_gb() free -- the machine first, the work after --
// polling every `poll`, giving up after `limit`. Writes one "(waiting ...)" line to stderr when it starts to wait.
// Returns the time waited.
std::chrono::milliseconds wait_for_memory(const Limits& limits, std::string_view label = "",
                                          std::chrono::milliseconds poll = std::chrono::seconds(2),
                                          std::chrono::milliseconds limit = std::chrono::seconds(600));

// The machine's installed memory in GB (nullopt where the system does not say).
std::optional<double> total_gb();

// Memory the governor leaves to the system: min_free_gb(), but no more than a quarter of the machine.
double reserve_gb(const Limits& limits);

// Processors nothing is using, measured over `sample` (nullopt where the system does not say): what other programs
// keep busy -- a build beside us -- is not ours to take.
std::optional<double> idle_cores(std::chrono::milliseconds sample = std::chrono::milliseconds(250));

// How many pieces of `mb_each` MB the free memory carries past the reserve (nullopt: not known).
std::optional<int> memory_allows(const Limits& limits, int mb_each);

// Up to `want` of the machine's slots -- as many as are free and the memory carries, and the processors idle -- and at
// least one: with none free it waits for one (polling every `poll`, up to `limit`), then runs on one regardless.
// The slots are given back when this object is destroyed.
class Slots {
public:
    Slots(const Limits& limits, int want, int mb_each = 256, std::string_view label = "",
          std::chrono::milliseconds limit = std::chrono::seconds(600),
          std::chrono::milliseconds poll = std::chrono::milliseconds(500));
    Slots(const Slots&) = delete;
    Slots& operator=(const Slots&) = delete;
    ~Slots();

    // How many things may run at once: the slots held, at least 1.
    int count() const;
    // The slots actually held (0 when the wait ran out and the caller runs on one regardless).
    int held() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::base::budget
