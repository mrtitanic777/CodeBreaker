#include "cb/base/parallel.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
using namespace std::chrono_literals;

namespace {

// The environment would override the limits: tests run with it clear.
struct CleanEnv {
    fs::ScopedEnv jobs{"CB_JOBS", std::nullopt};
    fs::ScopedEnv low{"CB_LOW_LOAD", std::nullopt};
    fs::ScopedEnv free_gb{"CB_MIN_FREE_GB", std::nullopt};
};

budget::Limits limits_in(const fs::ScopedTempDir& dir, int workers) {
    budget::Limits l;
    l.workers.workers = workers;
    l.min_free_gb = 0;
    l.slot_dir = dir.path() / "slots";
    return l;
}

}  // namespace

CB_TEST("base.parallel.min_free_gb_is_the_setting_unless_the_environment_says", [] {
    CleanEnv env;
    budget::Limits l;
    l.min_free_gb = 6.5;
    CB_CHECK_EQ(budget::min_free_gb(l), 6.5);
    {
        fs::ScopedEnv set("CB_MIN_FREE_GB", "2.25");
        CB_CHECK_EQ(budget::min_free_gb(l), 2.25);
    }
    {
        fs::ScopedEnv junk("CB_MIN_FREE_GB", "lots");   // not a number: the setting stands
        CB_CHECK_EQ(budget::min_free_gb(l), 6.5);
    }
});

CB_TEST("base.parallel.reserve_is_at_most_a_quarter_of_the_machine", [] {
    CleanEnv env;
    budget::Limits l;
    l.min_free_gb = 1e9;
    const auto total = budget::total_gb();
    CB_CHECK(total.has_value());
    CB_CHECK(*total > 0.5);
    CB_CHECK_EQ(budget::reserve_gb(l), *total / 4);
    l.min_free_gb = 0.25;
    CB_CHECK_EQ(budget::reserve_gb(l), 0.25);
});

CB_TEST("base.parallel.memory_allows_counts_pieces_past_the_reserve", [] {
    CleanEnv env;
    budget::Limits l;
    l.min_free_gb = 0;
    const auto pieces = budget::memory_allows(l, 1);   // one megabyte each: more than any machine has fewer than
    CB_CHECK(pieces.has_value());
    CB_CHECK(*pieces > 100);
    l.min_free_gb = 1e9;   // the reserve is capped at a quarter of the machine, so something may be left or not
    const auto some = budget::memory_allows(l, 1);
    CB_CHECK(some.has_value());
    CB_CHECK(*some >= 0);
    CB_CHECK(*budget::memory_allows(l, 1 << 30) == 0);   // a piece bigger than any memory
});

CB_TEST("base.parallel.wait_for_memory_returns_at_once_when_there_is_room_and_gives_up_when_there_is_none", [] {
    CleanEnv env;
    budget::Limits l;
    l.min_free_gb = 0;
    CB_CHECK_EQ(budget::wait_for_memory(l).count(), 0);
    l.min_free_gb = 1e9;   // never enough
    testing::CapturedStd cap;
    const auto waited = budget::wait_for_memory(l, "verify", 10ms, 35ms);
    CB_CHECK(waited >= 35ms);
    CB_CHECK(waited < 5s);
    CB_CHECK(cap.err().find("(waiting:") != std::string::npos);
    CB_CHECK(cap.err().find("- verify") != std::string::npos);
});

CB_TEST("base.parallel.idle_cores_is_between_none_and_all", [] {
    const auto idle = budget::idle_cores(20ms);
    if (!idle) return;   // the system gave no reading
    CB_CHECK(*idle >= 0.0);
    CB_CHECK(*idle <= ::cb::parallel::cpu_count() + 0.5);
});

CB_TEST("base.parallel.slots_hold_at_least_one_and_no_more_than_the_workers", [] {
    CleanEnv env;
    fs::ScopedTempDir dir("cbslots");
    budget::Slots s(limits_in(dir, 3), 100, 1, "test");
    CB_CHECK(s.count() >= 1);
    CB_CHECK(s.count() <= 3);
    CB_CHECK_EQ(s.count(), std::max(1, s.held()));
    CB_CHECK(s.held() >= 1);   // an empty temp folder: nothing else holds its slots
});

CB_TEST("base.parallel.a_slot_in_use_is_not_given_twice_and_is_given_back", [] {
    CleanEnv env;
    fs::ScopedTempDir dir("cbslots");
    const budget::Limits l = limits_in(dir, 1);   // one slot in all
    {
        budget::Slots first(l, 1, 1);
        CB_CHECK_EQ(first.held(), 1);
        testing::CapturedStd cap;
        budget::Slots second(l, 1, 1, "second", 30ms, 10ms);   // none free: waits, then runs on one regardless
        CB_CHECK_EQ(second.held(), 0);
        CB_CHECK_EQ(second.count(), 1);
        CB_CHECK(cap.err().find("(waiting for a free slot - second)") != std::string::npos);
    }
    budget::Slots again(l, 1, 1);   // the first went out of scope
    CB_CHECK_EQ(again.held(), 1);
});
