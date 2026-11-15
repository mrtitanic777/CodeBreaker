// cb/verify/stage_timer.hpp -- where a judge's time goes, when asked.
//
// With CB_PROFILE set (and not empty) every StageTimer adds its elapsed time to the stage it names, and the totals are written to stderr when
// the process ends: stage, seconds summed over every thread, calls. A judging worker is a process of its own and prints its own table.
// Without it a StageTimer reads no clock and takes no lock: the cost of an off switch is one cached bool.
#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>

namespace cb::verify {

namespace stage_timer {

// True when CB_PROFILE is set and not empty (read once).
bool enabled();

// Adds `seconds` to the stage `name`, one call.
void add(std::string_view name, double seconds);

// The totals so far, one stage a line, longest first. Empty when nothing was timed.
std::string report();

// Forgets every total (for tests).
void reset();

}  // namespace stage_timer

// Times the rest of its scope as the stage `name` (a string that outlives the timer: a literal).
class StageTimer {
public:
    explicit StageTimer(std::string_view name) : name_(name), on_(stage_timer::enabled()) {
        if (on_) start_ = std::chrono::steady_clock::now();
    }
    StageTimer(const StageTimer&) = delete;
    StageTimer& operator=(const StageTimer&) = delete;
    ~StageTimer() {
        if (on_) stage_timer::add(name_, std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count());
    }

private:
    std::string_view name_;
    bool on_;
    std::chrono::steady_clock::time_point start_;
};

}  // namespace cb::verify
