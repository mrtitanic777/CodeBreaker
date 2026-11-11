// codebreaker/jobs/pool.hpp -- typed C++17 port of codebreaker.jobs.pool.
//
// pool.py -- do one thing to many items at once: the one way CodeBreaker runs work side by side.
//
//     auto results = pool::run_first(work, items, context);          // {key: result}, workers
//     auto texts   = pool::threads(compile_one, names, 4);           // [result], in the items' order
//     pool::as_done(build_one, items, [&](const Item& item, const R& r) { ... });   // each as it finishes
//     auto out     = pool::isolated(argv_for, items, opts);          // a subprocess each, resumable
//
// How many run at once is the machine's budget (base/parallel workers(): settings.json "workers")
// unless `jobs` says. Threads are for work that waits on another program -- a compiler, an
// emulator -- and workers for work that computes.
//
// Notes on the port (what is not the Python's, and why):
//   - Python's run() starts worker PROCESSES (ProcessPoolExecutor); here the workers are THREADS of this
//     process, a bounded pool of `jobs` std::threads running the same chunking, the same crash handling
//     and the same retry rounds. What a thread cannot give is isolation from a native fault: a fault
//     inside `work` takes the whole process, which a dead worker process did not. An item whose `work`
//     THROWS is the case the retry logic handles here (it was the "worker died on this item" case):
//     the chunk is lost, the item it was on is retried alone, and an item that fails again gets
//     crashed(item). Everything else is as the Python: no item is silently dropped.
//   - the workers run below normal priority (the thread, not the process: the process is the asker's).
//     parallel.throttle's core affinity (CB_CORES) is not applied: the thread count is the bound.
//   - work(context, item) takes the context by reference (no pickling); `init(*initargs)` is `init`, a
//     callable with its arguments bound, run once at the start of each worker thread.
//   - processes_safe() is true: threads need no __main__ guard.
//   - the knowledge-base reads of an item come back with its result (knowledge/reads.hpp Carry/Carried):
//     run() wraps `work` in a Carry when a recording is active and `work` does not already return a
//     Carried (Python: isinstance(work, reads.Carry)).
//   - the default key of run is `_first`: run_first() (tuple/list items by their first element, any other
//     item as itself); run() takes the key function.
//   - as_done is a generator in Python; here `each(item, result)` is called, in the caller's thread, as
//     each item finishes. If `each` throws, the remaining items are still run to their end (the executor's
//     shutdown waits for them) before the exception leaves.
//   - isolated() result dicts are JSON objects (knowledge::sources::Json values); `timeout` is whole seconds.
#pragma once

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

#include "codebreaker/base/parallel.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::jobs::pool {

using Json = knowledge::sources::Json;
using Result = py::dict<py::str, Json>;   // one item's result of isolated(): a JSON object

// Can worker processes be started from here? (Threads can always be: true.)
bool processes_safe();

int64_t default_jobs();

// ---------------------------------------------------------------------------------------------
// The bounded worker pool every function below runs on.
namespace detail {

// Runs task(0) .. task(n-1), in that order of starting, on min(jobs, n) threads; every thread runs `init`
// first (a throwing init breaks the pool: the tasks not yet started are not run, as a broken process
// pool); `nice`: the threads run below normal priority. Returns once every started task has ended.
// -> ran[i]: whether task(i) was run.
std::vector<char> run_tasks(int64_t jobs, size_t n, const std::function<void()>& init, bool nice,
                            const std::function<void(size_t)>& task);

}  // namespace detail

// ---------------------------------------------------------------------------------------------
// run

// _first: an item's first element when it is a tuple or a list, else the item
template <class Item> struct FirstKey { using type = Item; static Item of(const Item& item) { return item; } };
template <class T, class... R> struct FirstKey<std::tuple<T, R...>> {
    using type = T;
    static T of(const std::tuple<T, R...>& item) { return std::get<0>(item); }
};
template <class T> struct FirstKey<py::list<T>> {
    using type = T;
    static T of(const py::list<T>& item) { return item[0]; }
};

template <class T> struct is_carried : std::false_type {};
template <class T> struct is_carried<knowledge::reads::Carried<T>> : std::true_type {};

// the knobs of run(): jobs, crashed, small, init, progress, size, below_normal
template <class Item, class R> struct Options {
    std::optional<int64_t> jobs;                       // None: the machine's budget
    std::function<R(const Item&)> crashed;             // None: R() (Python's None)
    int64_t small = 24;                                // fewer items than this are done by the caller itself
    std::function<void()> init;                        // run first in each worker
    std::function<void(int64_t, int64_t)> progress;    // told (done, total) as each share comes back
    std::optional<int64_t> size;                       // a worker's share (default: a sixth of its, at least eight)
    bool below_normal = true;                          // False: the workers keep normal priority
};

namespace detail {

// _chunk(work, context, items, key, mark, ...) -> {key: result}. `cur` is the index of the item in progress
// (Python's note file: it is written before each item and emptied at the end), left set if `work` throws.
template <class Work, class Context, class Item, class Key, class K, class R>
void chunk_(Work& work, const Context& context, const std::vector<Item>& items, Key& key, py::dict<K, R>& out,
            int64_t& cur) {
    for (size_t n = 0; n < items.size(); ++n) {
        cur = static_cast<int64_t>(n);
        out.set(key(items[n]), work(context, items[n]));
    }
    cur = -1;
}

template <class Work, class Context, class Item, class Key>
auto run_core(Work work, const py::list<Item>& items_in, const Context& context, Key key,
              const Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>& o)
    -> py::dict<std::invoke_result_t<Key, const Item&>, std::invoke_result_t<Work, const Context&, const Item&>> {
    using R = std::invoke_result_t<Work, const Context&, const Item&>;
    using K = std::invoke_result_t<Key, const Item&>;
    std::vector<Item> items = items_in.vec();
    int64_t jobs = o.jobs ? *o.jobs : 0;
    if (!jobs) jobs = default_jobs();   // (Python's `jobs or default_jobs()`)
    auto crashed_of = [&](const Item& item) -> R { return o.crashed ? o.crashed(item) : R(); };
    py::dict<K, R> out;
    if (jobs <= 1 || static_cast<int64_t>(items.size()) < o.small || !processes_safe()) {
        for (const Item& item : items) out.set(key(item), work(context, item));   // (the caller's own process)
        return out;
    }
    int64_t size = (o.size && *o.size) ? *o.size : std::max<int64_t>(8, -(-static_cast<int64_t>(items.size()) / (jobs * 6)));
    std::vector<Item> todo = items;
    auto not_done = [&](const std::vector<Item>& from) {
        std::vector<Item> rest;
        for (const Item& it : from) if (!out.contains(key(it))) rest.push_back(it);
        return rest;
    };
    for (int round = 0; round < 12; ++round) {
        if (todo.empty()) break;
        std::vector<std::vector<Item>> chunks;
        for (size_t i = 0; i < todo.size(); i += static_cast<size_t>(size))
            chunks.emplace_back(todo.begin() + i, todo.begin() + std::min(todo.size(), i + static_cast<size_t>(size)));
        std::vector<py::dict<K, R>> got(chunks.size());
        std::vector<std::exception_ptr> errors(chunks.size());
        std::vector<int64_t> cur(chunks.size(), -1);
        run_tasks(jobs, chunks.size(), o.init, o.below_normal, [&](size_t i) {
            try {
                py::dict<K, R> mine;
                chunk_(work, context, chunks[i], key, mine, cur[i]);
                got[i] = mine;
            } catch (...) {
                errors[i] = std::current_exception();   // (the share is lost: its results are not kept)
            }
        });
        // out.update(fut.result()) in the shares' order; a share that failed: ImportError is raised, the rest passed
        for (size_t i = 0; i < chunks.size(); ++i) {
            if (errors[i]) {
                try {
                    std::rethrow_exception(errors[i]);
                } catch (const py::ImportError&) {
                    throw;   // a broken setup is not a crashed item
                } catch (const std::exception&) {
                }
            } else {
                out.update(got[i]);
            }
            if (o.progress) o.progress(out.size(), static_cast<int64_t>(items.size()));
        }
        std::vector<Item> suspects;
        for (size_t i = 0; i < chunks.size(); ++i)   // which item was each dead worker on
            if (cur[i] >= 0 && !out.contains(key(chunks[i][static_cast<size_t>(cur[i])])))
                suspects.push_back(chunks[i][static_cast<size_t>(cur[i])]);
        todo = not_done(todo);
        if (todo.empty()) break;
        if (suspects.empty()) {   // died outside any item: go one at a time
            size = 1;
            continue;
        }
        for (const Item& item : suspects) {
            std::vector<Item> one{item};
            py::dict<K, R> mine;
            int64_t dummy = -1;
            std::exception_ptr err;
            std::vector<char> ran = run_tasks(1, 1, o.init, o.below_normal, [&](size_t) {
                try {
                    chunk_(work, context, one, key, mine, dummy);
                } catch (...) {
                    err = std::current_exception();
                }
            });
            if (err) {
                try {
                    std::rethrow_exception(err);
                } catch (const py::ImportError&) {
                    throw;
                } catch (const std::exception&) {
                    out.set(key(item), crashed_of(item));
                }
            } else if (!ran[0]) {
                out.set(key(item), crashed_of(item));   // (a pool that could not start)
            } else {
                out.update(mine);
            }
        }
        todo = not_done(todo);
    }
    for (const Item& item : todo) out.set(key(item), crashed_of(item));
    return out;
}

}  // namespace detail

// -> {key(item): work(context, item)} for every item. A worker is given `size` items at a time (default:
// a sixth of its share, at least eight); init is run first in each worker; progress(done, total) is told
// as each share comes back. below_normal=False: the workers keep normal priority.
template <class Work, class Context, class Item, class Key>
auto run(Work work, const py::list<Item>& items, const Context& context, Key key,
         const Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>& o =
             Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>())
    -> py::dict<std::invoke_result_t<Key, const Item&>, std::invoke_result_t<Work, const Context&, const Item&>> {
    using R = std::invoke_result_t<Work, const Context&, const Item&>;
    using K = std::invoke_result_t<Key, const Item&>;
    if constexpr (!is_carried<R>::value) {
        if (knowledge::reads::active()) {
            // (asked by a step of the engine: what each item read of the knowledge base comes back with its
            // result and is the step's read -- knowledge/reads.py)
            using C = knowledge::reads::Carried<R>;
            Options<Item, C> wo;
            wo.jobs = o.jobs;
            wo.small = o.small;
            wo.init = o.init;
            wo.progress = o.progress;
            wo.size = o.size;
            wo.below_normal = o.below_normal;
            if (o.crashed) {
                auto crashed = o.crashed;
                wo.crashed = [crashed](const Item& item) { return C{crashed(item), py::list<py::str>()}; };
            }
            auto carry = [work](const Context& ctx, const Item& item) -> C {
                knowledge::reads::recording r;
                R value = work(ctx, item);
                return C{value, r.sorted_keys()};
            };
            py::dict<K, C> got = detail::run_core(carry, items, context, key, wo);
            py::dict<K, R> out;
            for (const auto& kv : got.items()) out.set(std::get<0>(kv), knowledge::reads::unwrap(std::get<1>(kv)));
            return out;
        }
    }
    return detail::run_core(work, items, context, key, o);
}

// run() with the default key, `_first`
template <class Work, class Context, class Item>
auto run_first(Work work, const py::list<Item>& items, const Context& context,
               const Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>& o =
                   Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>()) {
    return run(work, items, context, [](const Item& item) { return FirstKey<Item>::of(item); }, o);
}

// ---------------------------------------------------------------------------------------------
// threads, as_done

// -> [work(item) for item in items], `jobs` at a time on threads of this process. An item's exception is raised
// here (the first in the items' order; the items not yet started after it are not run).
template <class Work, class Item>
auto threads(Work work, const py::list<Item>& items_in, std::optional<int64_t> jobs = std::nullopt)
    -> py::list<std::invoke_result_t<Work, const Item&>> {
    using R = std::invoke_result_t<Work, const Item&>;
    std::vector<Item> items = items_in.vec();
    int64_t n_jobs = jobs ? std::max<int64_t>(1, *jobs) : default_jobs();
    std::shared_ptr<knowledge::reads::Reads> asker = knowledge::reads::capture();
    std::vector<std::optional<R>> results(items.size());
    std::vector<std::exception_ptr> errors(items.size());
    std::atomic<size_t> stop_after(items.size());   // a failed item at i: nothing after it is started
    detail::run_tasks(n_jobs, items.size(), nullptr, false, [&](size_t i) {
        if (i > stop_after.load()) return;
        knowledge::reads::Adopt a(asker);   // (the asker's recording: what is read here is the asker's read)
        try {
            results[i] = work(items[i]);
        } catch (...) {
            errors[i] = std::current_exception();
            size_t cur = stop_after.load();
            while (i < cur && !stop_after.compare_exchange_weak(cur, i)) {
            }
        }
    });
    for (size_t i = 0; i < items.size(); ++i)
        if (errors[i]) std::rethrow_exception(errors[i]);
    py::list<R> out;
    for (auto& r : results) out.append(*r);
    return out;
}

// the knobs of as_done
template <class Item, class R> struct AsDoneOptions {
    std::optional<int64_t> jobs;
    bool processes = false;                                            // (threads either way: see the notes)
    std::function<R(const Item&, const std::exception_ptr&)> failed;   // the result of an item whose work threw
};

// each(item, work(item)) for each item as it finishes, `jobs` at a time. failed(item, error), when given, is
// the result of an item whose work threw; otherwise the exception is thrown here.
template <class Work, class Item, class Each>
void as_done(Work work, const py::list<Item>& items_in, Each each,
             const AsDoneOptions<Item, std::invoke_result_t<Work, const Item&>>& o =
                 AsDoneOptions<Item, std::invoke_result_t<Work, const Item&>>()) {
    using R = std::invoke_result_t<Work, const Item&>;
    std::vector<Item> items = items_in.vec();
    int64_t n_jobs = o.jobs ? std::max<int64_t>(1, *o.jobs) : default_jobs();
    std::shared_ptr<knowledge::reads::Reads> asker = o.processes ? nullptr : knowledge::reads::capture();
    std::vector<std::optional<R>> results(items.size());
    std::vector<std::exception_ptr> errors(items.size());
    std::mutex m;
    std::condition_variable cv;
    std::deque<size_t> finished;
    bool all_ended = false;
    std::thread driver([&] {
        detail::run_tasks(n_jobs, items.size(), nullptr, false, [&](size_t i) {
            {
                knowledge::reads::Adopt a(asker);
                try {
                    results[i] = work(items[i]);
                } catch (...) {
                    errors[i] = std::current_exception();
                }
            }
            std::lock_guard<std::mutex> g(m);
            finished.push_back(i);
            cv.notify_all();
        });
        std::lock_guard<std::mutex> g(m);
        all_ended = true;
        cv.notify_all();
    });
    std::exception_ptr raised;
    size_t consumed = 0;
    while (consumed < items.size()) {
        size_t i;
        {
            std::unique_lock<std::mutex> g(m);
            cv.wait(g, [&] { return !finished.empty() || all_ended; });
            if (finished.empty()) break;
            i = finished.front();
            finished.pop_front();
        }
        ++consumed;
        if (raised) continue;   // (the rest still run to their end: the executor's shutdown waits for them)
        try {
            if (errors[i]) {
                if (!o.failed) std::rethrow_exception(errors[i]);
                R r = o.failed(items[i], errors[i]);
                each(items[i], r);
            } else {
                each(items[i], *results[i]);
            }
        } catch (...) {
            raised = std::current_exception();
        }
    }
    driver.join();
    if (raised) std::rethrow_exception(raised);
}

// ---------------------------------------------------------------------------------------------
// Long sweeps: one isolated subprocess per item.
//
// A process pool is the wrong tool for a multi-hour oracle sweep on Windows: one worker stuck inside an
// in-process emulator has no wall-clock limit, so the pool waits forever, and a dead worker takes the pool
// down with a BrokenPipe. Running each item as its own subprocess with a timeout bounds the damage to that
// item, and a state file makes the sweep resumable. Failures are recorded SEPARATELY from completions, so a
// timed-out item is retried on the next run instead of being silently counted as done.

// the sweep's state file: {"done": {key: result}, "failed": {key: {"error", "tries", ...}}} (and whatever else
// the file holds, kept in its order)
struct State {
    py::dict<py::str, Json> top;   // every top-level key in order; "done" and "failed" are placeholders here
    py::dict<py::str, Result> done;
    py::dict<py::str, Result> failed;
};

State _load_state(const py::str& path);
void _save_state(const py::str& path, const State& st);
// the JSON object on the last line of `text` that starts with "{" (reading backwards), or None
std::optional<Result> _last_json(const std::optional<py::str>& text);

// subprocess.run(argv, capture_output=True, text=True, timeout=, cwd=, env=) judged as isolated() does: the
// child's last JSON object, or {"error": ...}
// (argv_for is called inside, as the Python's run_one calls it inside its try)
Result run_one_process(const std::function<py::list<py::str>()>& argv_for, int64_t timeout, const std::optional<py::str>& cwd,
                       const std::optional<py::dict<py::str, py::str>>& env);

// the dict the child's failure is recorded as: res plus tries
Result with_tries(const Result& res, int64_t tries);

template <class Item> struct IsolatedOptions {
    std::function<py::str(const Item&)> key;   // default: str(item)
    int64_t jobs = 4;
    int64_t timeout = 1200;
    std::optional<py::str> state_path;
    int64_t max_tries = 2;
    bool retry_failed = false;
    std::function<void(const Item&, const Result&, bool)> on_result;   // in the caller's thread, as each item finishes
    std::optional<py::str> cwd;
    std::optional<py::dict<py::str, py::str>> env;
};

struct Isolated {
    py::dict<py::str, Result> done;
    py::dict<py::str, Result> failed;
    int64_t ran = 0;
    int64_t skipped = 0;
};

// Run one subprocess per item and collect each one's result.
//
// argv_for(item) -> the command line. The child reports by printing a JSON object as its last stdout line; an
// object carrying an "error" key, a timeout, a crash or no JSON at all counts as a FAILURE. With `state_path`
// the sweep is resumable: completed items are skipped, failed items are retried until they have been tried
// `max_tries` times (`retry_failed` retries them regardless). on_result(item, result, ok) is called as each
// item finishes.
template <class Item>
Isolated isolated(const std::function<py::list<py::str>(const Item&)>& argv_for, const py::list<Item>& items,
                  const IsolatedOptions<Item>& o = IsolatedOptions<Item>()) {
    std::function<py::str(const Item&)> key = o.key ? o.key : std::function<py::str(const Item&)>([](const Item& i) { return py::to_str(i); });
    State st = o.state_path ? _load_state(*o.state_path) : State();
    auto tries_of = [&](const py::str& k) -> int64_t {
        const Result& f = st.failed.at(k);
        return f.contains(py::str("tries")) ? f.at(py::str("tries")).i : 0;
    };
    std::vector<Item> todo;
    int64_t skipped = 0;
    for (const Item& it : items) {
        py::str k = key(it);
        if (st.done.contains(k)) {
            skipped += 1;
        } else if (!o.retry_failed && st.failed.contains(k) && tries_of(k) >= o.max_tries) {
            skipped += 1;
        } else {
            todo.push_back(it);
        }
    }
    if (!todo.empty()) {
        AsDoneOptions<Item, Result> ao;
        ao.jobs = std::min<int64_t>(o.jobs, static_cast<int64_t>(todo.size()));
        as_done(
            [&](const Item& it) { return run_one_process([&] { return argv_for(it); }, o.timeout, o.cwd, o.env); },
            py::list<Item>(todo),
            [&](const Item& it, const Result& res) {
                py::str k = key(it);
                bool ok = !res.contains(py::str("error"));
                if (ok) {
                    st.done.set(k, res);
                    if (st.failed.contains(k)) st.failed.pop(k);
                } else {
                    int64_t prev = st.failed.contains(k) ? tries_of(k) : 0;
                    st.failed.set(k, with_tries(res, prev + 1));
                }
                if (o.state_path) _save_state(*o.state_path, st);
                if (o.on_result) o.on_result(it, res, ok);
            },
            ao);
    }
    return Isolated{st.done, st.failed, static_cast<int64_t>(todo.size()), skipped};
}

}  // namespace codebreaker::jobs::pool
