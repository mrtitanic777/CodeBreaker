// codebreaker/jobs/pool.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/jobs/pool.hpp for the ports that still use the old
// API (py::list items, a context argument, py::dict results). No logic: conversion and forwarding. Staged under
// native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: processes_safe, default_jobs, Options, FirstKey, run, run_first, threads, AsDoneOptions, as_done, State
// (not carried: it was the sweep's state-file record, now private to cb::jobs::isolated), IsolatedOptions, Isolated,
// isolated.
//
// What differs, and why it does not matter to the callers that exist (all of them pass module-level work):
//   * results come back in the items' order (the port's dict was in completion order);
//   * isolated() builds every item's argv before the sweep starts (the port called argv_for inside the child's try);
//   * a py::ImportError thrown by `work` of run() still leaves run() as a py::ImportError (the "broken setup is not a
//     crashed item" rule), carried across the new pool as a cb::UsageError.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/jobs/pool.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::jobs::pool {

using Json = knowledge::sources::Json;
using Result = py::dict<py::str, Json>;   // one item's result of isolated(): a JSON object

inline bool processes_safe() { return true; }
inline int64_t default_jobs() { return cb::jobs::default_jobs(); }

namespace detail {

// A py::ImportError that crossed the new pool as a UsageError: its text, to be thrown again as it was.
struct ImportCarrier {
    std::optional<std::string> text;
};

template <class Item, class R>
cb::jobs::RunOptions<std::size_t, R> run_options(const std::vector<Item>& items, const std::optional<int64_t>& jobs,
                                                 const std::function<R(const Item&)>& crashed, int64_t small,
                                                 const std::function<void()>& init,
                                                 const std::function<void(int64_t, int64_t)>& progress,
                                                 const std::optional<int64_t>& size, bool below_normal) {
    cb::jobs::RunOptions<std::size_t, R> ro;
    if (jobs) ro.jobs = static_cast<int>(*jobs);
    if (crashed) ro.crashed = [&items, crashed](const std::size_t& i) { return crashed(items[i]); };
    ro.small = small > 0 ? static_cast<std::size_t>(small) : 0;
    if (size && *size) ro.share = static_cast<std::size_t>(*size);
    ro.init = init;
    if (progress) ro.progress = [progress](std::size_t done, std::size_t total) { progress(static_cast<int64_t>(done), static_cast<int64_t>(total)); };
    ro.below_normal = below_normal;
    return ro;
}

}  // namespace detail

template <class Item, class R>
struct Options {
    std::optional<int64_t> jobs;
    std::function<R(const Item&)> crashed;
    int64_t small = 24;
    std::function<void()> init;
    std::function<void(int64_t, int64_t)> progress;
    std::optional<int64_t> size;
    bool below_normal = true;
};

template <class Item>
struct FirstKey {
    using type = Item;
    static Item of(const Item& item) { return item; }
};
template <class T, class... R>
struct FirstKey<std::tuple<T, R...>> {
    using type = T;
    static T of(const std::tuple<T, R...>& item) { return std::get<0>(item); }
};
template <class T>
struct FirstKey<py::list<T>> {
    using type = T;
    static T of(const py::list<T>& item) { return item[0]; }
};

// -> {key(item): work(context, item)} for every item.
template <class Work, class Context, class Item, class Key>
auto run(Work work, const py::list<Item>& items, const Context& context, Key key,
         const Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>& o =
             Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>())
    -> py::dict<std::invoke_result_t<Key, const Item&>, std::invoke_result_t<Work, const Context&, const Item&>> {
    using R = std::invoke_result_t<Work, const Context&, const Item&>;
    using K = std::invoke_result_t<Key, const Item&>;
    const std::vector<Item> list = items.vec();
    std::vector<std::size_t> index(list.size());
    for (std::size_t i = 0; i < list.size(); ++i) index[i] = i;
    detail::ImportCarrier carrier;
    auto one = [&](const std::size_t& i) -> R {
        try {
            return work(context, list[i]);
        } catch (const py::ImportError& e) {
            carrier.text = std::string(e.what());
            throw cb::UsageError("import error");
        }
    };
    try {
        const auto got = cb::jobs::run(index, one, [](const std::size_t& i) { return i; },
                                       detail::run_options<Item, R>(list, o.jobs, o.crashed, o.small, o.init, o.progress, o.size, o.below_normal));
        py::dict<K, R> out;
        for (const auto& [i, r] : got) out.set(key(list[i]), r);
        return out;
    } catch (const cb::UsageError&) {
        if (carrier.text) throw py::ImportError(py::str(*carrier.text));
        throw;
    }
}

// run() with the default key, `_first`
template <class Work, class Context, class Item>
auto run_first(Work work, const py::list<Item>& items, const Context& context,
               const Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>& o =
                   Options<Item, std::invoke_result_t<Work, const Context&, const Item&>>()) {
    return run(work, items, context, [](const Item& item) { return FirstKey<Item>::of(item); }, o);
}

// -> [work(item) for item in items], `jobs` at a time on threads of this process.
template <class Work, class Item>
auto threads(Work work, const py::list<Item>& items, std::optional<int64_t> jobs = std::nullopt)
    -> py::list<std::invoke_result_t<Work, const Item&>> {
    using R = std::invoke_result_t<Work, const Item&>;
    std::optional<int> n;
    if (jobs) n = static_cast<int>(*jobs);
    py::list<R> out;
    for (const R& r : cb::jobs::threads(items.vec(), work, n)) out.append(r);
    return out;
}

template <class Item, class R>
struct AsDoneOptions {
    std::optional<int64_t> jobs;
    bool processes = false;   // (threads either way)
    std::function<R(const Item&, const std::exception_ptr&)> failed;
};

// each(item, work(item)) for each item as it finishes, in the caller's thread.
template <class Work, class Item, class Each>
void as_done(Work work, const py::list<Item>& items, Each each,
             const AsDoneOptions<Item, std::invoke_result_t<Work, const Item&>>& o =
                 AsDoneOptions<Item, std::invoke_result_t<Work, const Item&>>()) {
    using R = std::invoke_result_t<Work, const Item&>;
    cb::jobs::AsDoneOptions<Item, R> ao;
    if (o.jobs) ao.jobs = static_cast<int>(*o.jobs);
    ao.failed = o.failed;
    cb::jobs::as_done(items.vec(), work, [&](const Item& item, R& r) { each(item, r); }, ao);
}

// ---- isolated ----------------------------------------------------------------------------------------------------
namespace detail {

inline Json old_json(const cb::knowledge::Value& v) {
    using Kind = cb::knowledge::Value::Kind;
    Json j;
    switch (v.kind()) {
        case Kind::Null: j.kind = Json::None; break;
        case Kind::Bool: j.kind = Json::Bool; j.b = *v.boolean(); break;
        case Kind::Int: j.kind = Json::Int; j.i = *v.integer(); break;
        case Kind::Double: j.kind = Json::Float; j.d = *v.number(); break;
        case Kind::Text: j.kind = Json::Str; j.s = py::str(*v.text()); break;
        case Kind::List:
            j.kind = Json::List;
            for (const auto& x : *v.list()) j.list.push_back(old_json(x));
            break;
        case Kind::Map:
            j.kind = Json::Dict;
            for (const auto& [k, x] : *v.map()) j.dict.set(py::str(k), old_json(x));
            break;
    }
    return j;
}

inline Result old_object(const cb::knowledge::Value& v) {
    Result r;
    if (const auto* m = v.map())
        for (const auto& [k, x] : *m) r.set(py::str(k), old_json(x));
    return r;
}

}  // namespace detail

template <class Item>
struct IsolatedOptions {
    std::function<py::str(const Item&)> key;   // default: str(item)
    int64_t jobs = 4;
    int64_t timeout = 1200;
    std::optional<py::str> state_path;
    int64_t max_tries = 2;
    bool retry_failed = false;
    std::function<void(const Item&, const Result&, bool)> on_result;
    std::optional<py::str> cwd;
    std::optional<py::dict<py::str, py::str>> env;
};

struct Isolated {
    py::dict<py::str, Result> done;
    py::dict<py::str, Result> failed;
    int64_t ran = 0;
    int64_t skipped = 0;
};

template <class Item>
Isolated isolated(const std::function<py::list<py::str>(const Item&)>& argv_for, const py::list<Item>& items,
                  const IsolatedOptions<Item>& o = IsolatedOptions<Item>()) {
    std::function<py::str(const Item&)> key =
        o.key ? o.key : std::function<py::str(const Item&)>([](const Item& i) { return py::to_str(i); });
    const std::vector<Item> list = items.vec();
    std::vector<cb::jobs::IsolatedItem> work;
    for (const Item& it : list) {
        cb::jobs::IsolatedItem w;
        w.key = key(it).utf8();
        for (const py::str& a : argv_for(it)) w.argv.push_back(a.utf8());
        work.push_back(std::move(w));
    }
    cb::jobs::IsolatedOptions co;
    co.jobs = static_cast<int>(o.jobs);
    co.timeout = std::chrono::seconds(o.timeout);
    if (o.state_path) co.state_path = std::filesystem::path(o.state_path->utf8());
    co.max_tries = static_cast<int>(o.max_tries);
    co.retry_failed = o.retry_failed;
    if (o.cwd) co.cwd = std::filesystem::path(o.cwd->utf8());
    if (o.env)
        for (const auto& kv : o.env->items()) co.env[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    if (o.on_result) {
        co.on_result = [&](const cb::jobs::IsolatedItem& w, const cb::knowledge::Value& res, bool ok) {
            for (std::size_t i = 0; i < work.size(); ++i)
                if (work[i].key == w.key) {
                    o.on_result(list[i], detail::old_object(res), ok);
                    return;
                }
        };
    }
    const cb::jobs::Isolated got = cb::compat::guard([&] { return cb::jobs::isolated(work, co); });
    Isolated out;
    for (const auto& [k, v] : got.done) out.done.set(py::str(k), detail::old_object(v));
    for (const auto& [k, f] : got.failed) {
        Result r = detail::old_object(f.result);
        Json tries;
        tries.kind = Json::Int;
        tries.i = f.tries;
        r.set(py::str("tries"), tries);
        out.failed.set(py::str(k), r);
    }
    out.ran = static_cast<int64_t>(got.ran);
    out.skipped = static_cast<int64_t>(got.skipped);
    return out;
}

}  // namespace codebreaker::jobs::pool
