// cb/core/parallel.hpp -- threads are the default way CodeBreaker runs things side by side.
//
// ThreadPool runs submitted tasks on a fixed set of threads; parallel_for / parallel_map spread a range over a
// pool (the calling thread helps, so calling them from inside a pool task cannot deadlock). The first
// exception a task throws is rethrown to the caller once the range has finished. workers() is the one number
// that says how many things run at once: CB_JOBS, or the `workers` setting, otherwise every processor but two,
// and never more than kLowLoad when low_load is on. Each Unicorn engine or DOS executive belongs to ONE
// thread; instances share nothing mutable except through a ShardedCache.
// See docs/CPP_DESIGN.md 3.11.
#pragma once

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace cb::parallel {

// What the settings contribute to the worker count (from cb::Config once that exists); the environment
// overrides both.
struct WorkerPolicy {
    int workers = 0;          // settings "workers"; 0 = automatic
    bool low_load = false;    // settings "low_load"
};

inline constexpr int kLowLoad = 4;   // what low_load holds everything to

// How many things run side by side: CB_JOBS, else policy.workers, else (processors - 2, at least 1); under
// low_load (CB_LOW_LOAD or the policy) at most kLowLoad.
int workers(const WorkerPolicy& policy = {});
int cpu_count();

// Memory the system can still give out without paging, in GB (RAM free and commit left, whichever is less);
// negative if unknown.
double available_gb();

// Lower this process's priority and/or pin it to the first `cores` processors (children inherit both).
bool throttle(int cores = 0, bool below_normal = true);

class ThreadPool {
public:
    // `threads` workers; 0 means workers().
    explicit ThreadPool(std::size_t threads = 0);
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool();   // finishes queued tasks, then joins

    std::size_t size() const { return threads_.size(); }

    // Queue a task; the future carries its result or its exception.
    template <class F>
    auto submit(F&& fn) -> std::future<std::invoke_result_t<std::decay_t<F>>> {
        using R = std::invoke_result_t<std::decay_t<F>>;
        auto task = std::make_shared<std::packaged_task<R()>>(std::forward<F>(fn));
        std::future<R> fut = task->get_future();
        enqueue([task] { (*task)(); });
        return fut;
    }

    // Queue a fire-and-forget task (exceptions are swallowed: use submit when the outcome matters).
    void enqueue(std::function<void()> task);

private:
    void run();
    std::mutex mutex_;
    std::condition_variable wake_;
    std::deque<std::function<void()>> queue_;
    bool stopping_ = false;
    std::vector<std::thread> threads_;
};

// Call fn(i) for every i in [0, count), spread over the pool; returns when all have run. The first exception
// is rethrown (the rest of the range still runs).
template <class F>
void parallel_for(ThreadPool& pool, std::size_t count, F&& fn) {
    if (count == 0) return;
    struct State {
        std::decay_t<F> fn;
        std::atomic<std::size_t> next{0};
        std::atomic<std::size_t> done{0};
        std::size_t count;
        std::mutex mutex;
        std::condition_variable finished;
        std::exception_ptr first_error;
        State(F&& f, std::size_t n) : fn(std::forward<F>(f)), count(n) {}
    };
    auto state = std::make_shared<State>(std::forward<F>(fn), count);
    auto work = [state] {
        while (true) {
            const std::size_t i = state->next.fetch_add(1);
            if (i >= state->count) return;
            try {
                state->fn(i);
            } catch (...) {
                std::lock_guard lock(state->mutex);
                if (!state->first_error) state->first_error = std::current_exception();
            }
            if (state->done.fetch_add(1) + 1 == state->count) {
                std::lock_guard lock(state->mutex);
                state->finished.notify_all();
            }
        }
    };
    const std::size_t helpers = std::min(pool.size(), count) - (count > 0 ? 1 : 0);
    for (std::size_t h = 0; h < helpers; ++h) pool.enqueue(work);
    work();   // the caller works too
    std::unique_lock lock(state->mutex);
    state->finished.wait(lock, [&] { return state->done.load() == state->count; });
    if (state->first_error) std::rethrow_exception(state->first_error);
}

// The same on a pool made for the call, sized by workers().
template <class F>
void parallel_for(std::size_t count, F&& fn) {
    ThreadPool pool(static_cast<std::size_t>(std::min<std::size_t>(count, static_cast<std::size_t>(workers()))));
    parallel_for(pool, count, std::forward<F>(fn));
}

// fn applied to every element, results in input order.
template <class In, class F>
auto parallel_map(ThreadPool& pool, const std::vector<In>& input, F&& fn) {
    using R = std::invoke_result_t<F&, const In&>;
    std::vector<std::optional<R>> slots(input.size());
    parallel_for(pool, input.size(), [&](std::size_t i) { slots[i].emplace(fn(input[i])); });
    std::vector<R> out;
    out.reserve(slots.size());
    for (auto& s : slots) out.push_back(std::move(*s));
    return out;
}

// A map shared between threads: sharded by key hash so that threads rarely contend, keyed by content hash by
// convention (a decoded-instruction cache, a compiled-pattern cache). Values are returned by copy.
template <class Key, class Value, class Hash = std::hash<Key>, std::size_t Shards = 16>
class ShardedCache {
public:
    // The cached value, computing it (outside the shard's lock) if absent. Two threads racing on the same new
    // key may both compute; the first insertion wins and both get that value.
    template <class Compute>
    Value get_or_compute(const Key& key, Compute&& compute) {
        Shard& s = shard(key);
        {
            std::shared_lock lock(s.mutex);
            if (auto it = s.map.find(key); it != s.map.end()) return it->second;
        }
        Value fresh = compute();
        std::unique_lock lock(s.mutex);
        return s.map.try_emplace(key, std::move(fresh)).first->second;
    }
    std::optional<Value> find(const Key& key) const {
        const Shard& s = shard(key);
        std::shared_lock lock(s.mutex);
        auto it = s.map.find(key);
        if (it == s.map.end()) return std::nullopt;
        return it->second;
    }
    std::size_t size() const {
        std::size_t n = 0;
        for (const Shard& s : shards_) {
            std::shared_lock lock(s.mutex);
            n += s.map.size();
        }
        return n;
    }
    void clear() {
        for (Shard& s : shards_) {
            std::unique_lock lock(s.mutex);
            s.map.clear();
        }
    }

private:
    struct Shard {
        mutable std::shared_mutex mutex;
        std::unordered_map<Key, Value, Hash> map;
    };
    Shard& shard(const Key& k) { return shards_[Hash{}(k) % Shards]; }
    const Shard& shard(const Key& k) const { return shards_[Hash{}(k) % Shards]; }
    std::array<Shard, Shards> shards_;
};

}  // namespace cb::parallel
