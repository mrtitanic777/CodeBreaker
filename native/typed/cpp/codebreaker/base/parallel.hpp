// codebreaker/base/parallel.hpp -- SHIM over cb/core/parallel.hpp and cb/base/parallel.hpp (docs/CPP_DESIGN.md 6.3): the
// old types in, the new functions called, the old types out. No logic; deleted with its last py:: consumer.
//
// How much of the machine CodeBreaker takes: the budget every runner asks for. The settings it reads (workers, low_load,
// min_free_gb) are the old settings'; the environment (CB_JOBS, CB_LOW_LOAD, CB_MIN_FREE_GB) overrides them.
//
//     n = parallel::workers();                      // how many things run side by side
//     { parallel::slots s(6, 256, "decompile"); int64_t n = s.enter(); ... }   // the machine's slots, given back at the end
//     parallel::throttle(4);                        // this process below normal priority, on four cores
//     parallel::wait_for_memory("verify");          // before a heavy job: while memory is short, wait
#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>

#include "cb/base/parallel.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/core/parallel.hpp"

namespace codebreaker::base::parallel {

namespace newp = ::cb::base::budget;

inline int64_t cpu_count() { return ::cb::parallel::cpu_count(); }

inline const int64_t LOW_LOAD = ::cb::parallel::kLowLoad;   // what the setting low_load holds everything to

// How many things run side by side, everywhere: settings.json "workers", or CB_JOBS; otherwise every processor but two.
// With low_load, never more than LOW_LOAD.
inline int64_t workers() { return ::cb::parallel::workers(::cb::compat::legacy::worker_policy()); }

// settings.json "low_load" (or CB_LOW_LOAD=1)
inline bool low_load() {
    if (const auto env = ::cb::fs::get_env("CB_LOW_LOAD"); env && !env->empty()) {
        const std::string t = ::cb::text::lower(::cb::text::trim(*env));
        return !(t == "0" || t == "false" || t == "no" || t == "off" || t == "none");
    }
    return ::cb::compat::legacy::worker_policy().low_load;
}

// Lower this process's priority and/or pin it to the first `cores` CPUs. Child processes inherit both.
inline bool throttle(std::optional<int64_t> cores = std::nullopt, bool below_normal = true) {
    return ::cb::parallel::throttle(cores ? static_cast<int>(*cores) : 0, below_normal);
}

// the memory the system can still give out without paging, in GB (None where not known)
inline std::optional<double> available_gb() {
    const double gb = ::cb::parallel::available_gb();
    return gb < 0 ? std::nullopt : std::optional<double>(gb);
}

// settings.json "min_free_gb" (or CB_MIN_FREE_GB)
inline double min_free_gb() { return newp::min_free_gb(::cb::compat::legacy::budget_limits()); }

// before a heavy job: wait while the system has less than min_free_gb free. Returns the seconds waited.
inline double wait_for_memory(const py::str& label = py::str(""), double poll = 2.0, double limit = 600) {
    const auto waited = newp::wait_for_memory(::cb::compat::legacy::budget_limits(), label.utf8(),
                                              std::chrono::milliseconds(static_cast<std::int64_t>(poll * 1000)),
                                              std::chrono::milliseconds(static_cast<std::int64_t>(limit * 1000)));
    return static_cast<double>(waited.count()) / 1000.0;
}

// the machine's memory in GB (None where not known)
inline std::optional<double> total_gb() { return newp::total_gb(); }

// memory the governor leaves to the system: min_free_gb, but no more than a quarter of the machine
inline double reserve_gb() { return newp::reserve_gb(::cb::compat::legacy::budget_limits()); }

// processors nothing is using, measured over `sample` seconds (None where not known)
inline std::optional<double> idle_cores(double sample = 0.25) {
    return newp::idle_cores(std::chrono::milliseconds(static_cast<std::int64_t>(sample * 1000)));
}

// how many pieces of `mb_each` MB the free memory carries past the reserve (None: not known)
inline std::optional<int64_t> memory_allows(int64_t mb_each) {
    const auto n = newp::memory_allows(::cb::compat::legacy::budget_limits(), static_cast<int>(mb_each));
    return n ? std::optional<int64_t>(*n) : std::nullopt;
}

//     parallel::slots s(6, 256, "decompile");  int64_t n = s.enter();  ...  s.exit();
// Up to `want` of the machine's slots, as many as are free and the memory carries -- at least one: with none free it waits
// for one (up to `limit` seconds, then runs on one regardless).
class slots {
public:
    slots(int64_t want, int64_t mb_each = 256, const py::str& label = py::str(""), double limit = 600, double poll = 0.5)
        : want(want), mb_each(mb_each), label(label), limit(limit), poll(poll) {}
    slots(const slots&) = delete;
    slots& operator=(const slots&) = delete;
    int64_t enter() {   // __enter__: how many slots are held (at least 1)
        held_ = std::make_unique<newp::Slots>(::cb::compat::legacy::budget_limits(), static_cast<int>(want), static_cast<int>(mb_each),
                                              label.utf8(), std::chrono::milliseconds(static_cast<std::int64_t>(limit * 1000)),
                                              std::chrono::milliseconds(static_cast<std::int64_t>(poll * 1000)));
        return held_->count();
    }
    bool exit() {       // __exit__: gives them back; False, so nothing is swallowed
        held_.reset();
        return false;
    }

    int64_t want;
    int64_t mb_each;
    py::str label;
    double limit;
    double poll;

private:
    std::unique_ptr<newp::Slots> held_;
};

}  // namespace codebreaker::base::parallel
