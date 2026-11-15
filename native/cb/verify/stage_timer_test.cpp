// Tests of the stage timer: totals by stage, in the order of their time, and nothing without the switch.
#include "cb/verify/stage_timer.hpp"

#include <thread>

#include "cb/core/testing.hpp"

namespace {

using namespace cb::verify;

CB_TEST("verify.stage_timer.totals_are_summed_by_stage_longest_first", [] {
    stage_timer::reset();
    CB_CHECK(stage_timer::report().empty());
    stage_timer::add("short", 0.25);
    stage_timer::add("long", 1.5);
    stage_timer::add("short", 0.25);
    const std::string report = stage_timer::report();
    CB_CHECK(report.find("long") < report.find("short"));
    CB_CHECK(report.find("0.500 s") != std::string::npos);   // short: two calls
    CB_CHECK(report.find("2 calls") != std::string::npos);
    stage_timer::reset();
    CB_CHECK(stage_timer::report().empty());
});

CB_TEST("verify.stage_timer.threads_add_to_the_same_totals", [] {
    stage_timer::reset();
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) threads.emplace_back([] { for (int i = 0; i < 100; ++i) stage_timer::add("shared", 0.001); });
    for (std::thread& t : threads) t.join();
    CB_CHECK(stage_timer::report().find("400 calls") != std::string::npos);
    stage_timer::reset();
});

CB_TEST("verify.stage_timer.a_timer_without_the_switch_records_nothing", [] {
    stage_timer::reset();
    if (stage_timer::enabled()) return;   // (the suite was started with CB_PROFILE: the timer is on)
    { const StageTimer timer("quiet"); }
    CB_CHECK(stage_timer::report().empty());
});

}  // namespace
