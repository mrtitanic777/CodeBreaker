#include "cb/jobs/pool.hpp"

#include <atomic>
#include <chrono>
#include <mutex>
#include <set>
#include <string>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from codebreaker/jobs/pool.py (and from the typed port):
//   * run() keeps what a failed share had already finished (a dead worker PROCESS lost its whole share's results);
//     the items that were not reached run again and the item that was on is retried alone, as before;
//   * threads() starts every item even after one has failed, and rethrows the first failure in the items' order (the
//     port stopped starting later items; the Python's executor ran them all);
//   * the results come back in the items' order (a dict in completion order before); an item whose key an earlier one
//     had replaces its result in that one's place;
//   * there is no `context` (the work lambda captures it), no `processes` option (threads either way: a worker
//     PROCESS is jobs/workers.hpp) and `isolated()` takes its argv up front (IsolatedItem) instead of calling argv_for;
//   * a failed isolated item's "error" for a program that cannot start is the Error's kind (not Python's exception name);
//     the state file keeps only "done" and "failed" (the Python kept any other top-level key it found).

using namespace cb;
using namespace cb::jobs;
using knowledge::Value;

namespace {

std::vector<int> iota(int n) {
    std::vector<int> v(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) v[static_cast<std::size_t>(i)] = i;
    return v;
}

}  // namespace

CB_TEST("jobs.pool.default_jobs_follows_cb_jobs", [] {
    const fs::ScopedEnv env("CB_JOBS", "3");
    CB_CHECK_EQ(default_jobs(), 3);
});

CB_TEST("jobs.pool.threads_keep_order", [] {
    const std::vector<int> got = threads(iota(20), [](int n) { return n * n; }, 4);
    CB_CHECK_EQ(got.size(), 20u);
    for (int n = 0; n < 20; ++n) CB_CHECK_EQ(got[static_cast<std::size_t>(n)], n * n);
    CB_CHECK(threads(std::vector<int>{}, [](int n) { return n; }, 2).empty());
});

CB_TEST("jobs.pool.threads_run_every_item_and_rethrow_the_first_failure", [] {
    std::atomic<int> ran{0};
    auto bad = [&](int n) {
        ++ran;
        if (n == 3) throw FormatError("test", 3, "three");
        if (n == 5) throw ToolError("test", 5, "five");
        return n;
    };
    CB_CHECK_THROWS_AS(FormatError, (void)threads(iota(6), bad, 2));   // item 3 comes before item 5
    CB_CHECK_EQ(ran.load(), 6);
});

CB_TEST("jobs.pool.threads_of_void_work", [] {
    std::atomic<int> sum{0};
    threads(iota(10), [&](int n) { sum += n; }, 3);
    CB_CHECK_EQ(sum.load(), 45);
});

CB_TEST("jobs.pool.threads_work_in_parallel", [] {
    // four items that each wait for the other three to have started can only finish if they run side by side
    std::atomic<int> started{0};
    const std::vector<int> got = threads(iota(4), [&](int n) {
        ++started;
        for (int spin = 0; spin < 5000 && started.load() < 4; ++spin) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return started.load() == 4 ? n : -1;
    }, 4);
    CB_CHECK((got == std::vector<int>{0, 1, 2, 3}));
});

CB_TEST("jobs.pool.as_done_reports_each_item_on_the_calling_thread", [] {
    const std::thread::id caller = std::this_thread::get_id();
    std::map<int, int> seen;
    bool on_caller = true;
    as_done(iota(8), [](int n) { return n * 10; }, [&](int item, int result) {
        on_caller = on_caller && std::this_thread::get_id() == caller;
        seen[item] = result;
    }, AsDoneOptions<int, int>{.jobs = 3});
    CB_CHECK(on_caller);
    CB_CHECK_EQ(seen.size(), 8u);
    for (int n = 0; n < 8; ++n) CB_CHECK_EQ(seen[n], n * 10);
});

CB_TEST("jobs.pool.as_done_failed_handler_and_rethrow", [] {
    auto bad = [](int n) {
        if (n == 3) throw FormatError("test", 3, "three");
        return std::to_string(n);
    };
    std::map<int, std::string> got;
    AsDoneOptions<int, std::string> o;
    o.jobs = 3;
    o.failed = [](int item, const std::exception_ptr&) { return "failed: " + std::to_string(item); };
    as_done(iota(6), bad, [&](int item, const std::string& r) { got[item] = r; }, o);
    CB_CHECK_EQ(got.size(), 6u);
    CB_CHECK_EQ(got[3], "failed: 3");
    CB_CHECK_EQ(got[4], "4");

    int each_calls = 0;
    CB_CHECK_THROWS_AS(FormatError, as_done(iota(6), bad, [&](int, const std::string&) { ++each_calls; }));
    CB_CHECK(each_calls <= 5);   // the failed item is not reported; the others may or may not be, before the throw

    // an `each` that throws leaves after the rest have ended
    std::atomic<int> ran{0};
    CB_CHECK_THROWS_AS(UsageError, as_done(iota(6), [&](int n) { ++ran; return n; },
                                           [](int, int) { throw UsageError("each"); }, AsDoneOptions<int, int>{.jobs = 2}));
    CB_CHECK_EQ(ran.load(), 6);
});

CB_TEST("jobs.pool.run_small_lists_run_on_the_calling_thread_in_order", [] {
    const std::thread::id caller = std::this_thread::get_id();
    std::vector<int> order;
    const auto got = run(iota(5), [&](int n) {
        CB_CHECK(std::this_thread::get_id() == caller);
        order.push_back(n);
        return n + 100;
    }, [](int n) { return n; });
    CB_CHECK((order == std::vector<int>{0, 1, 2, 3, 4}));
    CB_CHECK_EQ(got.size(), 5u);
    CB_CHECK_EQ(got.at(3), 103);
    // the results are in the items' order
    int expect = 0;
    for (const auto& [k, v] : got) {
        CB_CHECK_EQ(k, expect);
        CB_CHECK_EQ(v, expect + 100);
        ++expect;
    }
});

CB_TEST("jobs.pool.run_in_shares_gives_every_item_its_result", [] {
    RunOptions<int, int> o;
    o.jobs = 4;
    o.small = 1;
    o.share = 3;
    std::mutex m;
    std::set<std::thread::id> threads_used;
    const auto got = run(iota(40), [&](int n) {
        { std::lock_guard lock(m); threads_used.insert(std::this_thread::get_id()); }
        return n * n - 7;
    }, [](int n) { return n; }, o);
    CB_CHECK_EQ(got.size(), 40u);
    for (int n = 0; n < 40; ++n) CB_CHECK_EQ(got.at(n), n * n - 7);
    CB_CHECK(!threads_used.contains(std::this_thread::get_id()));   // workers are threads of their own
    // keyed by the item itself, and by a key function that maps two items to one key
    const auto by_item = run(iota(30), [](int n) { return n * 2; }, o);
    CB_CHECK_EQ(by_item.at(29), 58);
    const auto folded = run(std::vector<int>{1, 2, 3, 4}, [](int n) { return n; }, [](int n) { return n % 2; });
    CB_CHECK_EQ(folded.size(), 2u);
    CB_CHECK_EQ(folded.at(1), 3);   // the later item's result, in the first's place
    CB_CHECK_EQ(folded.entries()[0].first, 1);
});

CB_TEST("jobs.pool.run_retries_the_item_a_share_died_on", [] {
    RunOptions<int, int> o;
    o.jobs = 3;
    o.small = 1;
    o.share = 5;
    o.crashed = [](int n) { return -n; };
    std::atomic<int> thirteen_runs{0}, seven_runs{0};
    const auto got = run(iota(30), [&](int n) -> int {
        if (n == 13) { ++thirteen_runs; throw EmulationError("always fails"); }
        if (n == 7 && ++seven_runs == 1) throw EmulationError("fails once");
        return n * 2;
    }, [](int n) { return n; }, o);
    CB_CHECK_EQ(got.size(), 30u);
    CB_CHECK_EQ(got.at(13), -13);    // crashed(item)
    CB_CHECK_EQ(got.at(7), 14);      // an innocent bystander, retried alone
    for (int n = 0; n < 30; ++n)
        if (n != 13) CB_CHECK_EQ(got.at(n), n * 2);   // everything else that had not finished is run again
    CB_CHECK(thirteen_runs.load() >= 2);              // the share, then alone
});

CB_TEST("jobs.pool.run_without_crashed_gives_the_default", [] {
    RunOptions<int, int> o;
    o.jobs = 2;
    o.small = 1;
    const auto got = run(iota(10), [](int n) -> int { if (n == 4) throw FormatError("t", 0, "x"); return n + 1; },
                         [](int n) { return n; }, o);
    CB_CHECK_EQ(got.at(4), 0);
    CB_CHECK_EQ(got.at(5), 6);
});

CB_TEST("jobs.pool.run_a_broken_setup_is_not_a_crashed_item", [] {
    RunOptions<int, int> o;
    o.jobs = 2;
    o.small = 1;
    CB_CHECK_THROWS_AS(InternalError, (void)run(iota(10), [](int n) -> int { if (n == 2) throw InternalError("bug"); return n; },
                                                [](int n) { return n; }, o));
    RunOptions<int, int> init_fails = o;
    init_fails.init = [] { throw UsageError("missing setup"); };
    CB_CHECK_THROWS_AS(UsageError, (void)run(iota(10), [](int n) { return n; }, [](int n) { return n; }, init_fails));
});

CB_TEST("jobs.pool.run_init_runs_first_on_each_worker_and_progress_is_told", [] {
    std::atomic<int> inits{0};
    thread_local bool initialised = false;
    std::vector<std::size_t> told;
    RunOptions<int, int> o;
    o.jobs = 3;
    o.small = 1;
    o.share = 4;
    o.init = [&] { initialised = true; ++inits; };
    o.progress = [&](std::size_t done, std::size_t total) {
        CB_CHECK_EQ(total, 24u);
        told.push_back(done);
    };
    const auto got = run(iota(24), [](int n) { return initialised ? n : -1; }, [](int n) { return n; }, o);
    for (int n = 0; n < 24; ++n) CB_CHECK_EQ(got.at(n), n);
    CB_CHECK(inits.load() >= 1 && inits.load() <= 3);
    CB_CHECK_EQ(told.size(), 6u);                    // one per share
    CB_CHECK_EQ(told.back(), 24u);
    for (std::size_t i = 1; i < told.size(); ++i) CB_CHECK(told[i] >= told[i - 1]);
});

CB_TEST("jobs.pool.what_workers_read_of_the_knowledge_base_is_the_askers", [] {
    knowledge::Recording recording;
    RunOptions<int, int> o;
    o.jobs = 3;
    o.small = 1;
    o.share = 2;
    (void)run(iota(6), [](int n) { knowledge::record("win32", "prototype", "item" + std::to_string(n)); return n; },
              [](int n) { return n; }, o);
    (void)threads(std::vector<int>{1}, [](int n) { knowledge::record("win16", "struct", "t" + std::to_string(n)); return n; }, 2);
    std::size_t seen = 0;
    as_done(std::vector<int>{9}, [](int n) { knowledge::record("win16", "struct", "d" + std::to_string(n)); return n; },
            [&](int, int) { ++seen; });
    CB_CHECK_EQ(seen, 1u);
    CB_CHECK_EQ(recording.reads()->size(), 8u);
    CB_CHECK(recording.reads()->contains({"win32", "prototype", "item5"}));
    CB_CHECK(recording.reads()->contains({"win16", "struct", "t1"}));
    CB_CHECK(recording.reads()->contains({"win16", "struct", "d9"}));
});

// ---- isolated ---------------------------------------------------------------------------------------------------
namespace {

// A child that behaves by its argument: ok* prints noise and an object, crash exits 3 with nothing, err reports an
// error, slow waits past the timeout, junk ends with a line that is not JSON.
constexpr const char* kChild =
    "@echo off\r\n"
    "if \"%1\"==\"crash\" exit /b 3\r\n"
    "if \"%1\"==\"slow\" ping -n 30 127.0.0.1 >nul\r\n"
    "if \"%1\"==\"err\" goto err\r\n"
    "if \"%1\"==\"junk\" goto junk\r\n"
    "echo noise\r\n"
    "echo {\"item\":\"%1\",\"n\":%2}\r\n"
    "exit /b 0\r\n"
    ":err\r\n"
    "echo {\"error\":\"boom\"}\r\n"
    "exit /b 0\r\n"
    ":junk\r\n"
    "echo {not json\r\n"
    "exit /b 0\r\n";

std::vector<IsolatedItem> sweep(const std::filesystem::path& script, std::initializer_list<std::string> names) {
    std::vector<IsolatedItem> items;
    for (const std::string& n : names)
        items.push_back({n, {"cmd", "/c", fs::to_text(script), n, std::to_string(n.size())}});
    return items;
}

}  // namespace

CB_TEST("jobs.pool.isolated_separates_failures_and_resumes", [] {
    const fs::ScopedTempDir dir("cbpool");
    const std::filesystem::path script = dir.path() / "child.cmd";
    fs::write_file(script, as_bytes(kChild));
    const std::filesystem::path state = dir.path() / "state.json";

    IsolatedOptions o;
    o.jobs = 5;
    o.timeout = std::chrono::seconds(2);
    o.state_path = state;
    std::vector<std::pair<std::string, bool>> seen;
    o.on_result = [&](const IsolatedItem& it, const Value&, bool ok) { seen.emplace_back(it.key, ok); };

    Isolated out = isolated(sweep(script, {"ok1", "ok22", "crash", "err", "slow"}), o);
    CB_CHECK_EQ(out.done.size(), 2u);
    CB_CHECK(out.done.contains("ok1") && out.done.contains("ok22"));
    CB_CHECK_EQ(*out.done.at("ok22").get("item")->text(), "ok22");
    CB_CHECK_EQ(*out.done.at("ok22").get("n")->integer(), 4);
    CB_CHECK_EQ(*out.failed.at("crash").result.get("error")->text(), "no-output");
    CB_CHECK_EQ(*out.failed.at("crash").result.get("rc")->integer(), 3);
    CB_CHECK_EQ(*out.failed.at("err").result.get("error")->text(), "boom");
    CB_CHECK_EQ(*out.failed.at("slow").result.get("error")->text(), "timeout");   // a hang is bounded, and NOT done
    CB_CHECK_EQ(*out.failed.at("slow").result.get("timeout")->integer(), 2);
    CB_CHECK_EQ(out.ran, 5u);
    CB_CHECK_EQ(out.skipped, 0u);
    CB_CHECK_EQ(seen.size(), 5u);
    std::sort(seen.begin(), seen.end());
    CB_CHECK_EQ(seen[0].first, "crash");
    CB_CHECK(!seen[0].second);
    CB_CHECK_EQ(seen[2].first, "ok1");
    CB_CHECK(seen[2].second);

    // the state file is JSON with the settled items under "done"
    const Value file = Value::from_json_text(fs::read_text(state));
    CB_CHECK_EQ(file.get("done")->map()->size(), 2u);
    CB_CHECK_EQ(*file.get("failed")->map()->at("crash").get("tries")->integer(), 1);

    // resume: completed items are skipped, failed ones are retried (max_tries 2)
    o.timeout = std::chrono::seconds(5);
    Isolated out2 = isolated(sweep(script, {"ok1", "ok22", "crash", "err"}), o);
    CB_CHECK_EQ(out2.ran, 2u);
    CB_CHECK_EQ(out2.skipped, 2u);
    CB_CHECK_EQ(out2.failed.at("crash").tries, 2);

    // tries exhausted: skipped, unless retry_failed
    Isolated out3 = isolated(sweep(script, {"crash", "err"}), o);
    CB_CHECK_EQ(out3.ran, 0u);
    CB_CHECK_EQ(out3.skipped, 2u);
    o.retry_failed = true;
    Isolated out4 = isolated(sweep(script, {"crash"}), o);
    CB_CHECK_EQ(out4.ran, 1u);
    CB_CHECK_EQ(out4.failed.at("crash").tries, 3);
});

CB_TEST("jobs.pool.isolated_without_a_state_file_and_odd_outputs", [] {
    const fs::ScopedTempDir dir("cbpool");
    const std::filesystem::path script = dir.path() / "child.cmd";
    fs::write_file(script, as_bytes(kChild));
    IsolatedOptions o;
    o.jobs = 2;
    o.timeout = std::chrono::seconds(5);
    Isolated out = isolated(sweep(script, {"a", "junk"}), o);
    CB_CHECK_EQ(out.done.size(), 1u);
    CB_CHECK_EQ(*out.failed.at("junk").result.get("error")->text(), "no-output");   // a last "{" line that is not JSON
    CB_CHECK(isolated({}, o).done.empty());

    // a program that cannot be started is that item's failure, not an exception of the sweep
    Isolated missing = isolated({{"nope", {"cb-no-such-program-anywhere.exe"}}}, o);
    CB_CHECK_EQ(missing.failed.size(), 1u);
    CB_CHECK_EQ(*missing.failed.at("nope").result.get("error")->text(), "ToolError");

    // a damaged state file is a fresh sweep
    const std::filesystem::path state = dir.path() / "state.json";
    fs::write_file(state, as_bytes("{not json"));
    o.state_path = state;
    Isolated fresh = isolated(sweep(script, {"a"}), o);
    CB_CHECK_EQ(fresh.ran, 1u);
    CB_CHECK_EQ(fresh.done.size(), 1u);
});
