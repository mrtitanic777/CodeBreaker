#include "cb/jobs/pool.hpp"

#include <windows.h>

#include <nlohmann/json.hpp>

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"

namespace cb::jobs {

using knowledge::Value;

int default_jobs() { return parallel::workers(); }

namespace detail {

bool is_setup_fault(const std::exception_ptr& e) {
    try {
        std::rethrow_exception(e);
    } catch (const UsageError&) {
        return true;
    } catch (const InternalError&) {
        return true;
    } catch (...) {   // classification only: nothing is swallowed, the caller decides
        return false;
    }
}

TaskRun run_tasks(int jobs, std::size_t n, const std::function<void()>& init, bool below_normal,
                  const std::function<void(std::size_t)>& task, const std::function<void(std::size_t)>& on_done) {
    TaskRun result;
    result.ran.assign(n, 0);
    if (n == 0) return result;

    struct Shared {
        std::mutex mutex;
        std::condition_variable woke;
        std::deque<std::size_t> finished;   // tasks that ended, not yet reported to on_done
        std::size_t live = 0;               // threads still running
        std::exception_ptr task_error;      // the first exception a task threw
    } shared;
    std::atomic<std::size_t> next{0};
    std::atomic<bool> broken{false};

    const std::size_t threads = std::min<std::size_t>(static_cast<std::size_t>(std::max(1, jobs)), n);
    shared.live = threads;
    auto worker = [&] {
        if (below_normal) SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);   // the thread, not the process
        bool ok = true;
        if (init) {
            try {
                init();
            } catch (...) {   // a broken pool: what has not started does not run
                std::lock_guard lock(shared.mutex);
                if (!result.init_error) result.init_error = std::current_exception();
                broken = true;
                ok = false;
            }
        }
        while (ok && !broken.load()) {
            const std::size_t i = next.fetch_add(1);
            if (i >= n) break;
            try {
                task(i);
            } catch (...) {   // reported after the others have ended
                std::lock_guard lock(shared.mutex);
                if (!shared.task_error) shared.task_error = std::current_exception();
            }
            std::lock_guard lock(shared.mutex);
            result.ran[i] = 1;
            shared.finished.push_back(i);
            shared.woke.notify_all();
        }
        std::lock_guard lock(shared.mutex);
        --shared.live;
        shared.woke.notify_all();
    };

    std::exception_ptr callback_error;
    {
        std::vector<std::jthread> pool;
        pool.reserve(threads);
        for (std::size_t t = 0; t < threads; ++t) pool.emplace_back(worker);
        for (;;) {
            std::vector<std::size_t> batch;
            {
                std::unique_lock lock(shared.mutex);
                shared.woke.wait(lock, [&] { return !shared.finished.empty() || shared.live == 0; });
                batch.assign(shared.finished.begin(), shared.finished.end());
                shared.finished.clear();
                if (batch.empty()) break;   // nothing finished and nobody left running
            }
            if (!on_done) continue;
            for (const std::size_t i : batch) {
                if (callback_error) break;   // the tasks still run to their end
                try {
                    on_done(i);
                } catch (...) {   // reported once every task has ended
                    callback_error = std::current_exception();
                }
            }
        }
    }   // the threads are joined here
    if (shared.task_error) std::rethrow_exception(shared.task_error);
    if (callback_error) std::rethrow_exception(callback_error);
    return result;
}

}  // namespace detail

// ---- isolated ------------------------------------------------------------------------------------------------------
namespace {

constexpr std::size_t kStderrTail = 300;
constexpr std::size_t kDetailMax = 200;

Json to_json(const Value& v) {
    switch (v.kind()) {
        case Value::Kind::Null: return nullptr;
        case Value::Kind::Bool: return *v.boolean();
        case Value::Kind::Int: return *v.integer();
        case Value::Kind::Double: return *v.number();
        case Value::Kind::Text: return *v.text();
        case Value::Kind::List: {
            Json a = Json::array();
            for (const Value& x : *v.list()) a.push_back(to_json(x));
            return a;
        }
        case Value::Kind::Map: {
            Json o = Json::object();
            for (const auto& [k, x] : *v.map()) o[k] = to_json(x);
            return o;
        }
    }
    return nullptr;
}

Value object_of(std::initializer_list<std::pair<std::string, Value>> entries) {
    Value::Map m;
    for (const auto& [k, x] : entries) m.insert_or_assign(k, x);
    return Value(std::move(m));
}

// The last `n` bytes of `text`, without cutting a UTF-8 sequence in two.
std::string tail_of(std::string_view text, std::size_t n) {
    if (text.size() > n) {
        text.remove_prefix(text.size() - n);
        while (!text.empty() && (static_cast<unsigned char>(text.front()) & 0xC0) == 0x80) text.remove_prefix(1);
    }
    return std::string(text);
}

std::string_view trim_ws(std::string_view s) {
    constexpr std::string_view kWs = " \t\r\n\f\v";
    const std::size_t a = s.find_first_not_of(kWs);
    if (a == std::string_view::npos) return {};
    return s.substr(a, s.find_last_not_of(kWs) - a + 1);
}

// The JSON object on the last line of stdout that starts with "{" (reading backwards); that line must parse.
std::optional<Value> last_json_object(std::string_view text) {
    text = trim_ws(text);
    while (!text.empty()) {
        const std::size_t cut = text.rfind('\n');
        const std::string_view line = cut == std::string_view::npos ? text : text.substr(cut + 1);
        if (line.starts_with('{')) {
            try {
                return Value::from_json_text(line);
            } catch (const Error&) {   // a line that is not JSON is "no result", not a fault
                return std::nullopt;
            }
        }
        if (cut == std::string_view::npos) break;
        text = text.substr(0, cut);
    }
    return std::nullopt;
}

// One subprocess: the child's result, or the failure as an object.
Value run_child(const IsolatedItem& item, const IsolatedOptions& o) {
    try {
        process::ProcessSpec spec;
        spec.argv = item.argv;
        spec.cwd = o.cwd;
        spec.env = o.env;
        spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(o.timeout);
        const process::ProcessResult r = process::run(spec);
        if (r.timed_out) return object_of({{"error", "timeout"}, {"timeout", static_cast<std::int64_t>(o.timeout.count())}});
        const std::string out(r.out.begin(), r.out.end());
        if (std::optional<Value> v = last_json_object(out)) return *v;
        const std::string err(r.err.begin(), r.err.end());
        return object_of({{"error", "no-output"},
                          {"rc", static_cast<std::int64_t>(r.exit_code)},
                          {"stderr", tail_of(trim_ws(err), kStderrTail)}});
    } catch (const Error& e) {   // isolation boundary: one item of a sweep
        std::string detail = e.what();
        if (detail.size() > kDetailMax) detail = tail_of(std::string_view(detail).substr(0, kDetailMax), kDetailMax);
        return object_of({{"error", std::string(e.kind())}, {"detail", detail}});
    }
}

struct State {
    OrderedMap<std::string, Value> done;
    OrderedMap<std::string, FailedItem> failed;
};

State load_state(const std::filesystem::path& path) {
    State st;
    Value doc;
    try {
        doc = Value::from_json_text(fs::read_text(path), fs::to_text(path));
    } catch (const Error&) {   // a missing or damaged state file is a fresh sweep
        return st;
    }
    if (const Value* done = doc.get("done"); done && done->map())
        for (const auto& [k, v] : *done->map()) st.done.insert_or_assign(k, v);
    if (const Value* failed = doc.get("failed"); failed && failed->map()) {
        for (const auto& [k, v] : *failed->map()) {
            FailedItem f{v, 0};
            if (const Value* t = v.get("tries"); t && t->integer()) f.tries = static_cast<int>(*t->integer());
            st.failed.insert_or_assign(k, std::move(f));
        }
    }
    return st;
}

// What a failure is filed as: the object plus its count of tries.
Value filed(const FailedItem& f) {
    Value::Map m;
    if (const Value::Map* have = f.result.map()) m = *have;
    m.insert_or_assign("tries", Value(static_cast<std::int64_t>(f.tries)));
    return Value(std::move(m));
}

void save_state(const std::filesystem::path& path, const State& st) {
    Json done = Json::object(), failed = Json::object();
    for (const auto& [k, v] : st.done) done[k] = to_json(v);
    for (const auto& [k, f] : st.failed) failed[k] = to_json(filed(f));
    Json doc = Json::object();
    doc["done"] = std::move(done);
    doc["failed"] = std::move(failed);
    json::save(path, doc, json::Style::PythonCompatible);
}

}  // namespace

Isolated isolated(const std::vector<IsolatedItem>& items, const IsolatedOptions& options) {
    State st = options.state_path ? load_state(*options.state_path) : State{};
    std::vector<IsolatedItem> todo;
    std::size_t skipped = 0;
    for (const IsolatedItem& item : items) {
        if (st.done.contains(item.key)) {
            ++skipped;
        } else if (const FailedItem* f = st.failed.find(item.key); !options.retry_failed && f && f->tries >= options.max_tries) {
            ++skipped;
        } else {
            todo.push_back(item);
        }
    }

    if (!todo.empty()) {
        AsDoneOptions<IsolatedItem, Value> ao;
        ao.jobs = static_cast<int>(std::min<std::size_t>(static_cast<std::size_t>(std::max(1, options.jobs)), todo.size()));
        as_done(
            todo, [&](const IsolatedItem& item) { return run_child(item, options); },
            [&](const IsolatedItem& item, const Value& res) {
                const bool ok = res.get("error") == nullptr;
                if (ok) {
                    st.done.insert_or_assign(item.key, res);
                    st.failed.erase(item.key);
                } else {
                    const FailedItem* prev = st.failed.find(item.key);
                    st.failed.insert_or_assign(item.key, FailedItem{res, (prev ? prev->tries : 0) + 1});
                }
                if (options.state_path) save_state(*options.state_path, st);
                if (options.on_result) options.on_result(item, res, ok);
            },
            ao);
    }
    return Isolated{std::move(st.done), std::move(st.failed), todo.size(), skipped};
}

}  // namespace cb::jobs
