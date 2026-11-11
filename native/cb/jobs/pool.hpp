// cb/jobs/pool.hpp -- do one thing to many items at once: the one way CodeBreaker runs work side by side.
//
//     auto texts = jobs::threads(names, [&](const std::string& n) { return compile_one(n); }, 4);   // results, in the items' order
//     jobs::as_done(items, build_one, [&](const Item& item, Built& b) { ... });                       // each as it finishes
//     auto judged = jobs::run(items, judge_one, [](const Item& i) { return i.name; });                // {key: result}, items' order
//     jobs::Isolated out = jobs::isolated(sweep_items, {.state_path = "sweep.json"});                 // a subprocess each, resumable
//
// How many run at once is the machine's budget (cb::parallel::workers(): CB_JOBS, the `workers` setting) unless `jobs`
// says. Everything here runs on THREADS of this process; a unit of work that must be isolated from a native fault (an
// emulator that can take its process down) goes to a worker PROCESS instead -- cb/jobs/workers.hpp -- or, for a long
// sweep, to `isolated()`, a subprocess per item with a timeout.
//
// What the knowledge base is asked on a worker thread is the asker's read: the recording in force on the calling
// thread (cb/knowledge/reads.hpp) is put in force on every thread the work runs on.
//
// run(): the items are dealt out to the threads in shares. A work function that throws loses nothing but the item
// it was on: the items its share had already finished keep their results, that item is retried alone, an item that
// fails again gets `crashed(item)` as its result, and everything its share had not reached is run again. No item is
// silently dropped. (A UsageError or an InternalError is a broken setup or a bug, not a bad item: it leaves run().)
#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/knowledge/reads.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::jobs {

// How many things run at once when `jobs` is not given.
int default_jobs();

namespace detail {

struct TaskRun {
    std::vector<char> ran;          // ran[i]: task(i) was started
    std::exception_ptr init_error;  // the first exception `init` threw: that pool was broken, what had not started did not run
};

// Run task(0) .. task(n-1) on min(jobs, n) threads, starting them in that order; every thread runs `init` first.
// `below_normal` lowers the priority of those threads (not of the process). `on_done(i)` is called on the CALLING
// thread as each task finishes. A task or `on_done` that throws: the rest still run to their end, then the first
// exception is rethrown here. Returns once every started task has ended.
TaskRun run_tasks(int jobs, std::size_t n, const std::function<void()>& init, bool below_normal,
                  const std::function<void(std::size_t)>& task, const std::function<void(std::size_t)>& on_done = {});

// A UsageError or an InternalError: a broken setup, not a bad item.
bool is_setup_fault(const std::exception_ptr& e);

}  // namespace detail

// ---- run: {key: result} for every item, in shares ----------------------------------------------------------------
template <class Item, class R>
struct RunOptions {
    std::optional<int> jobs;                                        // nullopt or 0: default_jobs()
    std::function<R(const Item&)> crashed;                          // the result of an item that fails alone; unset: R{}
    std::size_t small = 24;                                         // fewer items than this are done by the caller itself, in order
    std::size_t share = 0;                                          // items per share (0: a sixth of a worker's, at least 8)
    std::function<void()> init;                                     // run first on each worker thread
    std::function<void(std::size_t done, std::size_t total)> progress;   // told, on the calling thread, as each share comes back
    bool below_normal = true;                                       // false: the workers keep normal priority (the caller waits on them)
};

// work(item) for every item; the results by key(item), in the items' order (an item whose key an earlier one had:
// its result replaces that one's, in that one's place).
template <class Item, class Work, class Key,
          class R = std::invoke_result_t<Work&, const Item&>,
          class K = std::invoke_result_t<Key&, const Item&>>
OrderedMap<K, R> run(const std::vector<Item>& items, Work&& work, Key&& key, const RunOptions<Item, R>& options = {}) {
    static_assert(std::is_default_constructible_v<R>, "run(): R must be default-constructible (or give `crashed`)");
    const auto crashed_of = [&](const Item& item) -> R { return options.crashed ? options.crashed(item) : R{}; };
    const int jobs = options.jobs && *options.jobs > 0 ? *options.jobs : default_jobs();
    const std::size_t total = items.size();
    std::vector<std::optional<R>> slot(total);   // by item index: an item never runs twice into one slot at a time

    auto assemble = [&] {
        OrderedMap<K, R> out;
        out.reserve(total);
        for (std::size_t i = 0; i < total; ++i) out.insert_or_assign(key(items[i]), std::move(*slot[i]));
        return out;
    };

    if (jobs <= 1 || total < options.small) {   // the caller's own thread
        for (std::size_t i = 0; i < total; ++i) slot[i].emplace(work(items[i]));
        return assemble();
    }

    const std::shared_ptr<knowledge::Reads> asker = knowledge::current_recording();
    std::atomic<std::size_t> finished{0};
    // one item, on a worker thread: its result, or the exception it throws
    auto attempt = [&](std::size_t i) {
        const knowledge::AdoptRecording adopt(asker);
        slot[i].emplace(work(items[i]));
        ++finished;
    };
    auto unfinished = [&](const std::vector<std::size_t>& from) {
        std::vector<std::size_t> rest;
        for (std::size_t i : from)
            if (!slot[i]) rest.push_back(i);
        return rest;
    };

    const std::size_t workers = static_cast<std::size_t>(jobs);
    std::size_t share = options.share != 0 ? options.share : std::max<std::size_t>(8, (total + workers * 6 - 1) / (workers * 6));
    std::vector<std::size_t> todo(total);
    for (std::size_t i = 0; i < total; ++i) todo[i] = i;

    for (int round = 0; round < 12 && !todo.empty(); ++round) {
        std::vector<std::vector<std::size_t>> shares;
        for (std::size_t at = 0; at < todo.size(); at += share)
            shares.emplace_back(todo.begin() + static_cast<std::ptrdiff_t>(at),
                                todo.begin() + static_cast<std::ptrdiff_t>(std::min(todo.size(), at + share)));
        std::vector<std::exception_ptr> failure(shares.size());
        std::vector<std::optional<std::size_t>> on(shares.size());   // the item a share died on
        const detail::TaskRun ran = detail::run_tasks(
            jobs, shares.size(), options.init, options.below_normal,
            [&](std::size_t s) {
                for (std::size_t i : shares[s]) {
                    try {
                        attempt(i);
                    } catch (...) {   // isolation boundary: one share of a batch
                        failure[s] = std::current_exception();
                        on[s] = i;
                        return;
                    }
                }
            },
            [&](std::size_t s) {
                if (failure[s] && detail::is_setup_fault(failure[s])) std::rethrow_exception(failure[s]);
                if (options.progress) options.progress(finished.load(), total);
            });
        if (ran.init_error && detail::is_setup_fault(ran.init_error)) std::rethrow_exception(ran.init_error);

        std::vector<std::size_t> suspects;
        for (const auto& s : on)
            if (s) suspects.push_back(*s);
        todo = unfinished(todo);
        if (todo.empty()) break;
        if (suspects.empty()) {   // died outside any item: go one at a time
            share = 1;
            continue;
        }
        for (std::size_t i : suspects) {   // each alone: four of five were innocent
            std::exception_ptr failed;
            const detail::TaskRun alone = detail::run_tasks(1, 1, options.init, options.below_normal, [&](std::size_t) {
                try {
                    attempt(i);
                } catch (...) {   // isolation boundary: one item
                    failed = std::current_exception();
                }
            });
            if (failed && detail::is_setup_fault(failed)) std::rethrow_exception(failed);
            if (alone.init_error && detail::is_setup_fault(alone.init_error)) std::rethrow_exception(alone.init_error);
            if (!slot[i]) slot[i].emplace(crashed_of(items[i]));   // it killed its share twice
        }
        todo = unfinished(todo);
    }
    for (std::size_t i : todo) slot[i].emplace(crashed_of(items[i]));   // twelve rounds and still not done
    return assemble();
}

// run() keyed by the item itself.
template <class Item, class Work, class R = std::invoke_result_t<Work&, const Item&>>
OrderedMap<Item, R> run(const std::vector<Item>& items, Work&& work, const RunOptions<Item, R>& options = {}) {
    return run(items, std::forward<Work>(work), [](const Item& item) { return item; }, options);
}

// ---- threads: results in the items' order -------------------------------------------------------------------------
// work(item) for every item, `jobs` at a time. Every item runs; an item's exception is rethrown here once they have
// all ended (the first in the items' order). For work that waits on another program -- a compiler, an emulator.
template <class Item, class Work, class R = std::invoke_result_t<Work&, const Item&>>
auto threads(const std::vector<Item>& items, Work&& work, std::optional<int> jobs = std::nullopt) {
    const int n_jobs = jobs ? std::max(1, *jobs) : default_jobs();
    const std::shared_ptr<knowledge::Reads> asker = knowledge::current_recording();
    std::vector<std::exception_ptr> failure(items.size());
    auto guarded = [&](std::size_t i, auto&& body) {
        const knowledge::AdoptRecording adopt(asker);
        try {
            body();
        } catch (...) {   // isolation boundary: one item of a batch; rethrown below
            failure[i] = std::current_exception();
        }
    };
    auto rethrow_first = [&] {
        for (const std::exception_ptr& e : failure)
            if (e) std::rethrow_exception(e);
    };
    if constexpr (std::is_void_v<R>) {
        detail::run_tasks(n_jobs, items.size(), {}, false, [&](std::size_t i) { guarded(i, [&] { work(items[i]); }); });
        rethrow_first();
    } else {
        std::vector<std::optional<R>> slot(items.size());
        detail::run_tasks(n_jobs, items.size(), {}, false,
                          [&](std::size_t i) { guarded(i, [&] { slot[i].emplace(work(items[i])); }); });
        rethrow_first();
        std::vector<R> out;
        out.reserve(slot.size());
        for (auto& s : slot) out.push_back(std::move(*s));
        return out;
    }
}

// ---- as_done: each result as its item finishes ---------------------------------------------------------------------
template <class Item, class R>
struct AsDoneOptions {
    std::optional<int> jobs;
    // The result of an item whose work threw. Unset: the exception leaves as_done() (after the rest have ended).
    std::function<R(const Item&, const std::exception_ptr&)> failed;
};

// each(item, result) for every item as it finishes, `jobs` at a time, on the calling thread. If `each` throws, the
// items still running are let finish before the exception leaves.
template <class Item, class Work, class Each, class R = std::invoke_result_t<Work&, const Item&>>
void as_done(const std::vector<Item>& items, Work&& work, Each&& each, const AsDoneOptions<Item, R>& options = {}) {
    const int n_jobs = options.jobs ? std::max(1, *options.jobs) : default_jobs();
    const std::shared_ptr<knowledge::Reads> asker = knowledge::current_recording();
    std::vector<std::optional<R>> slot(items.size());
    std::vector<std::exception_ptr> failure(items.size());
    std::exception_ptr raised;
    detail::run_tasks(
        n_jobs, items.size(), {}, false,
        [&](std::size_t i) {
            const knowledge::AdoptRecording adopt(asker);
            try {
                slot[i].emplace(work(items[i]));
            } catch (...) {   // isolation boundary: one item of a batch
                failure[i] = std::current_exception();
            }
        },
        [&](std::size_t i) {
            if (raised) return;   // (the rest still run to their end)
            try {
                if (!failure[i]) {
                    each(items[i], *slot[i]);
                } else if (options.failed) {
                    R r = options.failed(items[i], failure[i]);
                    each(items[i], r);
                } else {
                    std::rethrow_exception(failure[i]);
                }
            } catch (...) {
                raised = std::current_exception();
            }
        });
    if (raised) std::rethrow_exception(raised);
}

// ---- isolated: long sweeps, one subprocess per item ---------------------------------------------------------------
//
// A thread pool is the wrong tool for a multi-hour oracle sweep: one thread stuck inside an in-process emulator has no
// wall-clock limit, so the sweep waits forever, and a fault in native code takes every thread with it. Running each
// item as its own subprocess with a timeout bounds the damage to that item, and a state file makes the sweep
// resumable. Failures are recorded SEPARATELY from completions, so a timed-out item is retried on the next run
// instead of being silently counted as done.

struct IsolatedItem {
    std::string key;                    // what the state file and the result name it by
    std::vector<std::string> argv;      // its command line; the child reports by printing a JSON object as its last stdout line
};

struct FailedItem {
    knowledge::Value result;   // the child's object, or the failure's own ({"error": "timeout", ...})
    int tries = 0;             // how many runs ended in failure
};

struct IsolatedOptions {
    int jobs = 4;
    std::chrono::seconds timeout{1200};
    std::optional<std::filesystem::path> state_path;   // the sweep is resumable: done items are skipped, failed ones retried
    int max_tries = 2;                                  // a failed item is retried until it has been tried this often
    bool retry_failed = false;                          // ... or regardless
    std::optional<std::filesystem::path> cwd;
    std::map<std::string, std::string> env;             // overrides on the inherited environment
    // Called on the calling thread as each item finishes: its result (the child's object, or the failure) and whether it succeeded.
    std::function<void(const IsolatedItem&, const knowledge::Value& result, bool ok)> on_result;
};

struct Isolated {
    OrderedMap<std::string, knowledge::Value> done;
    OrderedMap<std::string, FailedItem> failed;
    std::size_t ran = 0;       // items run now
    std::size_t skipped = 0;   // items the state file already settled
};

// Run one subprocess per item and collect each one's result. An object carrying an "error" key, a timeout, a crash
// or no JSON object on the last stdout line counts as a FAILURE: {"error": "no-output", "rc": exit code, "stderr":
// the last 300 bytes} or {"error": "timeout", "timeout": seconds} or {"error": <the error's kind, e.g. "ToolError" when the program cannot be started>, "detail": ...}.
// The state file is {"done": {key: result}, "failed": {key: {"error": ..., "tries": n}}} as JSON (the layout the
// Python wrote); an unreadable one is an empty sweep.
Isolated isolated(const std::vector<IsolatedItem>& items, const IsolatedOptions& options = {});

}  // namespace cb::jobs
