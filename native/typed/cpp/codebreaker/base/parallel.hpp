// codebreaker/base/parallel.hpp -- C++17 port of codebreaker.base.parallel.
//
// parallel.py -- how much of the machine CodeBreaker takes: the budget every runner asks for.
//
//     n = parallel::workers();                      // how many things run side by side (settings.json "workers")
//     {
//         parallel::slots s(6, 256, "decompile");   // the machine's slots, shared by every CodeBreaker process
//         int64_t n = s.enter();
//         ... run n at once ...
//     }                                             // (`with parallel.slots(...) as n:`; the end of the scope gives them back)
//     parallel::throttle(4);                        // this process below normal priority, on four cores
//     parallel::wait_for_memory("verify");          // before a heavy job: while memory is short, wait
//
// The work itself is run by jobs/pool.py (threads, worker processes, a subprocess each); this
// holds what they all ask before they start: how many, and whether the machine has room.
//
// Notes on the port:
//   - the settings it reads (low_load, workers, min_free_gb) are read with py::settings::get_str; settings.json
//     holds some of them as numbers (get_str is for string settings and raises TypeError for those), so the
//     number, boolean or null is then read from settings.json's text here, as Python's settings.get gave it
//     (inside codebreaker.exe that reading needs CB_CHECKOUT_ROOT; without it the TypeError stands).
//   - Python's `with parallel.slots(...)` is  enter()  and  exit() ; the destructor calls exit(), which
//     gives the slots back once.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "py.hpp"

namespace codebreaker::base::parallel {

int64_t cpu_count();

extern const int64_t LOW_LOAD;                 // what the setting low_load holds everything to

// settings.json "low_load" (or CB_LOW_LOAD=1): the machine is someone's to work on while
// CodeBreaker runs -- every count below is held to LOW_LOAD
bool low_load();

// How many things run side by side, everywhere: the one number every part of the workbench
// asks for (compiles, judging, sweeps). settings.json "workers", or CB_JOBS; otherwise every
// processor but two (the two left for the machine's owner and for the process handing the work
// out). With low_load, never more than LOW_LOAD.
int64_t workers();

// ---------------------------------------------------------------------------
// Background work: below normal priority, on a bounded number of cores.
// ---------------------------------------------------------------------------
// Lower this process's priority and/or pin it to the first `cores` CPUs, so a background
// sweep leaves the machine usable. Child processes inherit both. Windows only; a no-op (False)
// elsewhere or on failure.
bool throttle(std::optional<int64_t> cores = std::nullopt, bool below_normal = true);

// the memory the system can still give out without paging, in GB (None where not known)
std::optional<double> available_gb();

// settings.json "min_free_gb" (or CB_MIN_FREE_GB): below this much free memory no new heavy job starts
double min_free_gb();

// before a heavy job: wait while the system has less than min_free_gb free - the machine first, the work
// after. Returns the seconds waited.
double wait_for_memory(const py::str& label = py::str(""), double poll = 2.0, double limit = 600);

// the machine's memory in GB (None where not known)
std::optional<double> total_gb();

// ---------------------------------------------------------------------------
// The governor: one budget of heavy jobs for the whole machine.
//
// Each part used to decide on its own how much to run at once, so they multiplied: a sweep taking
// two binaries apart, each splitting its decompiler six ways, asked for twelve at once. Now a
// heavy job holds a slot while it runs -- slots are the machine's `workers()`, shared by every
// CodeBreaker process (a locked file each: the system lets go of it when its process ends, even
// killed) -- and it is given no more than the free memory carries, at `mb_each` a piece, past the
// reserve the system keeps. A small machine runs one at a time; nobody waits for ever.
// ---------------------------------------------------------------------------
// One slot: a locked file (Python's _Slot).
class Slot_ {
public:
    explicit Slot_(const py::str& path) : path(path) {}
    Slot_(const Slot_&) = delete;
    Slot_& operator=(const Slot_&) = delete;
    ~Slot_();
    bool take();
    void give_back();
    py::str path;
private:
    int fd_ = -1;   // the open file (Python's self.f), -1 for None
};

// memory the governor leaves to the system: min_free_gb, but no more than a quarter of the machine
double reserve_gb();

// processors nothing is using, measured over `sample` seconds (None where not known): what
// other programs keep busy -- a build beside us -- is not ours to take
std::optional<double> idle_cores(double sample = 0.25);

// how many pieces of `mb_each` MB the free memory carries past the reserve (None: not known)
std::optional<int64_t> memory_allows(int64_t mb_each);

//     parallel::slots s(6, 256, "decompile");  int64_t n = s.enter();  ...  s.exit();
// Up to `want` of the machine's slots, as many as are free and the memory carries -- at least
// one: with none free it waits for one (up to `limit` seconds, then runs on one regardless).
class slots {
public:
    slots(int64_t want, int64_t mb_each = 256, const py::str& label = py::str(""), double limit = 600, double poll = 0.5);
    slots(const slots&) = delete;
    slots& operator=(const slots&) = delete;
    ~slots();
    int64_t enter();    // __enter__: how many slots are held (at least 1)
    bool exit();        // __exit__: gives them back; False, so nothing is swallowed

    int64_t want;
    int64_t mb_each;
    py::str label;
    double limit;
    double poll;
    std::vector<std::shared_ptr<Slot_>> held;
};

}  // namespace codebreaker::base::parallel
