#include "cb/core/parallel.hpp"

#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::parallel;

CB_TEST("core.parallel.workers_policy", [] {
    fs::ScopedEnv jobs("CB_JOBS", std::nullopt);
    fs::ScopedEnv low("CB_LOW_LOAD", std::nullopt);
    CB_CHECK(workers() >= 1);
    CB_CHECK_EQ(workers({.workers = 3}), 3);
    CB_CHECK_EQ(workers({.workers = 12, .low_load = true}), kLowLoad);   // low_load caps it
    CB_CHECK_EQ(workers({.workers = 2, .low_load = true}), 2);
    {
        fs::ScopedEnv env("CB_JOBS", "5");
        CB_CHECK_EQ(workers({.workers = 3}), 5);        // the environment wins over the setting
        fs::ScopedEnv lowenv("CB_LOW_LOAD", "1");
        CB_CHECK_EQ(workers(), kLowLoad);
        fs::ScopedEnv off("CB_LOW_LOAD", "0");
        CB_CHECK_EQ(workers({.low_load = true}), 5);    // an explicit "off" in the environment wins too
    }
    {
        fs::ScopedEnv bad("CB_JOBS", "many");
        CB_CHECK_EQ(workers({.workers = 4}), 4);        // junk is ignored
    }
    CB_CHECK(available_gb() > 0.0);
});

CB_TEST("core.parallel.pool_submit", [] {
    ThreadPool pool(3);
    CB_CHECK_EQ(pool.size(), 3u);
    auto a = pool.submit([] { return 6 * 7; });
    auto b = pool.submit([] { return std::string("done"); });
    CB_CHECK_EQ(a.get(), 42);
    CB_CHECK_EQ(b.get(), "done");
    auto failing = pool.submit([]() -> int { throw FormatError("t", 1, "bad"); });
    CB_CHECK_THROWS_AS(FormatError, failing.get());
    // tasks queued before destruction still run
    std::atomic<int> ran{0};
    {
        ThreadPool small(1);
        for (int i = 0; i < 50; ++i) small.enqueue([&ran] { ++ran; });
    }
    CB_CHECK_EQ(ran.load(), 50);
});

CB_TEST("core.parallel.parallel_for_covers_every_index_once", [] {
    ThreadPool pool(4);
    std::vector<std::atomic<int>> hits(1000);
    parallel_for(pool, hits.size(), [&](std::size_t i) { ++hits[i]; });
    for (auto& h : hits) CB_CHECK_EQ(h.load(), 1);
    parallel_for(pool, 0, [](std::size_t) { CB_CHECK(false); });   // nothing to do
    std::atomic<int> total{0};
    parallel_for(25, [&](std::size_t i) { total += static_cast<int>(i); });   // the pool-less form
    CB_CHECK_EQ(total.load(), 300);
});

CB_TEST("core.parallel.parallel_for_rethrows_first_error_after_finishing", [] {
    ThreadPool pool(3);
    std::atomic<int> ran{0};
    CB_CHECK_THROWS_AS(UsageError, parallel_for(pool, 100, [&](std::size_t i) {
        ++ran;
        if (i == 10) throw UsageError("ten");
    }));
    CB_CHECK_EQ(ran.load(), 100);   // the rest of the range still ran
});

CB_TEST("core.parallel.nested_parallel_for_does_not_deadlock", [] {
    ThreadPool pool(2);   // fewer threads than outer tasks: the callers must help
    std::atomic<int> inner{0};
    parallel_for(pool, 6, [&](std::size_t) { parallel_for(pool, 5, [&](std::size_t) { ++inner; }); });
    CB_CHECK_EQ(inner.load(), 30);
});

CB_TEST("core.parallel.parallel_map_keeps_order", [] {
    ThreadPool pool(4);
    std::vector<int> in(200);
    for (int i = 0; i < 200; ++i) in[static_cast<std::size_t>(i)] = i;
    auto out = parallel_map(pool, in, [](const int& v) { return std::to_string(v * 2); });
    CB_CHECK_EQ(out.size(), 200u);
    CB_CHECK_EQ(out[0], "0");
    CB_CHECK_EQ(out[199], "398");
    CB_CHECK_EQ(parallel_map(pool, std::vector<int>{}, [](const int& v) { return v; }).size(), 0u);
});

CB_TEST("core.parallel.sharded_cache", [] {
    ShardedCache<std::string, int> cache;
    std::atomic<int> computed{0};
    ThreadPool pool(4);
    parallel_for(pool, 400, [&](std::size_t i) {
        const std::string key = "k" + std::to_string(i % 10);
        const int v = cache.get_or_compute(key, [&] { ++computed; return static_cast<int>(i % 10) * 100; });
        CB_CHECK_EQ(v, static_cast<int>(i % 10) * 100);
    });
    CB_CHECK_EQ(cache.size(), 10u);
    CB_CHECK(computed.load() >= 10);
    CB_CHECK_EQ(*cache.find("k3"), 300);
    CB_CHECK(!cache.find("zz").has_value());
    cache.clear();
    CB_CHECK_EQ(cache.size(), 0u);
});
