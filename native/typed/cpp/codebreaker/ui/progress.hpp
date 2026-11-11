// codebreaker/ui/progress.hpp -- COMPAT SHIM over cb/ui/progress.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/ui/progress.{hpp,cpp}: the reading (worker count, log tail, rate, estimate, text) and the
// watch loop. It does not know the function store or the coverage ledger -- those are other modules, not yet refactored
// -- so it is handed a `ReadFn`. This header supplies that function from the OLD store and ledger and keeps the old
// py:: API (Snapshot, snapshot, line, main ...) over the new one.
//
// COMPAT(py): `read_store` below calls coverage/ledger and store/functions (old); remove it when they are refactored
// and the application passes their cb:: readers to cb::ui::progress::command.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/ui/progress.hpp"
#include "codebreaker/coverage/ledger.hpp"
#include "codebreaker/store/functions.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::ui::progress {

inline constexpr int64_t WINDOW = 600;            // seconds of results the rate is taken over

// the time now, in seconds since the epoch (a seam; default: py::time::time)
inline std::function<double()>& clock() {
    static std::function<double()> c = [] { return py::time::time(); };
    return c;
}

// one kind of module in a reading: {kind, name, modules, measured, functions, <the store's classes>}
struct KindReading {
    py::str kind, name;
    int64_t modules = 0, measured = 0, functions = 0;
    py::dict<py::str, int64_t> classes;           // {class: functions} for every class of the store's ORDER
};

// one module whose results were written lately: {module, kind, results, age}
struct Recent {
    py::str module, kind;
    int64_t results = 0, age = 0;
};

struct Snapshot {
    py::str time;                                  // "HH:MM:SS"
    py::list<KindReading> kinds;
    py::list<Recent> recent;
    double rate = 0.0;                             // results per minute over the last ten minutes
    std::optional<int64_t> idle;                   // seconds since the last result, None when there are none
    int64_t workers = 0;
    std::optional<int64_t> eta;                    // seconds, None when nothing is arriving
    bool active = false;
    py::str log;
    py::str store;
};

namespace detail_shim {
namespace nw = cb::ui::progress;
namespace ledger = coverage::ledger;
namespace F = store::functions;

inline nw::Clock::time_point at(double seconds) {
    return nw::Clock::time_point(std::chrono::duration_cast<nw::Clock::duration>(std::chrono::duration<double>(seconds)));
}
inline double epoch(nw::Clock::time_point t) { return std::chrono::duration<double>(t.time_since_epoch()).count(); }

// COMPAT(py): the store and the ledger as they are today
inline nw::StoreReading read_store(nw::Clock::time_point now) {
    nw::StoreReading out;
    out.store_path = F::default_path().utf8();
    ledger::Store fs = ledger::open_store();
    ledger::Ledger led = ledger::measure(fs);
    for (const ledger::Summary& r : led.kinds) {
        std::optional<py::str> digest;
        if (led.profiles.contains(r.kind)) digest = led.profiles.at(r.kind);
        if (!digest || digest->empty()) continue;
        nw::KindReading k{.kind = r.kind.utf8(), .name = r.name.utf8(),
                          .modules = static_cast<std::uint64_t>(r.modules),
                          .measured = static_cast<std::uint64_t>(r.measured),
                          .functions = static_cast<std::uint64_t>(r.functions)};
        for (const py::str& c : F::ORDER())
            k.classes.push_back({c.utf8(), F::TITLE().at(c).utf8(), static_cast<std::uint64_t>(r.classes.at(c))});
        out.kinds.push_back(std::move(k));
    }
    if (!fs) return out;
    out.opened = true;
    try {
        for (const auto& row : fs->recent(8)) {
            const py::str kind = engine::native_engine::cell_str(row[1]);
            out.recent.push_back({engine::native_engine::cell_str(row[0]).utf8(),
                                  (ledger::TITLE().contains(kind) ? ledger::TITLE().at(kind) : kind).utf8(),
                                  static_cast<std::uint64_t>(engine::native_engine::cell_int(row[2])),
                                  at(engine::native_engine::cell_double(row[3]))});
        }
        out.fresh = static_cast<std::uint64_t>(engine::native_engine::cell_int(
            fs->verdicts_since(epoch(now) - static_cast<double>(WINDOW))));
    } catch (...) {
        fs->close();
        throw;
    }
    fs->close();
    return out;
}

inline Snapshot to_old(const nw::Snapshot& s) {
    Snapshot out;
    out.time = cb::compat::to_py(s.time);
    for (const nw::KindReading& k : s.kinds) {
        KindReading o{cb::compat::to_py(k.kind), cb::compat::to_py(k.name), static_cast<int64_t>(k.modules),
                      static_cast<int64_t>(k.measured), static_cast<int64_t>(k.functions), {}};
        for (const nw::ClassCount& c : k.classes) o.classes.set(cb::compat::to_py(c.id), static_cast<int64_t>(c.count));
        out.kinds.append(o);
    }
    for (const nw::Recent& r : s.recent)
        out.recent.append({cb::compat::to_py(r.module), cb::compat::to_py(r.kind), static_cast<int64_t>(r.results),
                           r.age.count()});
    out.rate = s.rate;
    if (s.idle) out.idle = s.idle->count();
    out.workers = static_cast<int64_t>(s.workers);
    if (s.eta) out.eta = s.eta->count();
    out.active = s.active;
    out.log = cb::compat::to_py(s.log);
    out.store = cb::compat::to_py(s.store);
    return out;
}

inline nw::Snapshot to_new(const Snapshot& s) {
    nw::Snapshot out;
    out.time = s.time.utf8();
    for (const KindReading& k : s.kinds) {
        nw::KindReading n{.kind = k.kind.utf8(), .name = k.name.utf8(), .modules = static_cast<std::uint64_t>(k.modules),
                          .measured = static_cast<std::uint64_t>(k.measured),
                          .functions = static_cast<std::uint64_t>(k.functions)};
        for (const py::str& c : F::ORDER())   // the store's classes, in its order
            n.classes.push_back({c.utf8(), F::TITLE().at(c).utf8(), static_cast<std::uint64_t>(k.classes.at(c))});
        out.kinds.push_back(std::move(n));
    }
    for (const Recent& r : s.recent)
        out.recent.push_back({r.module.utf8(), r.kind.utf8(), static_cast<std::uint64_t>(r.results),
                              std::chrono::seconds(r.age)});
    out.rate = s.rate;
    if (s.idle) out.idle = std::chrono::seconds(*s.idle);
    out.workers = static_cast<std::uint64_t>(s.workers);
    if (s.eta) out.eta = std::chrono::seconds(*s.eta);
    out.active = s.active;
    out.log = s.log.utf8();
    out.store = s.store.utf8();
    return out;
}
}  // namespace detail_shim

// Worker processes of the job runner that are in the middle of an item right now.
inline int64_t _busy_workers() {
    return static_cast<int64_t>(cb::ui::progress::busy_workers(cb::fs::from_text(py::tempfile::gettempdir().utf8()),
                                                               detail_shim::at(clock()())));
}

// the last non-blank line of the end of a file ("" when it cannot be read)
inline py::str _last_line(const py::str& path) {
    return cb::compat::to_py(cb::ui::progress::last_line(cb::fs::from_text(path.utf8())));
}

// One reading.
inline Snapshot snapshot(const std::optional<py::str>& log = std::nullopt) {
    std::optional<std::filesystem::path> file;
    if (log) file = cb::fs::from_text(log->utf8());
    return detail_shim::to_old(
        cb::ui::progress::snapshot(detail_shim::read_store, file, detail_shim::at(clock()())));
}

// "1h 05m" / "3m 07s" ("-" for None)
inline py::str _clock(const std::optional<int64_t>& seconds) {
    std::optional<std::chrono::seconds> span;
    if (seconds) span = std::chrono::seconds(*seconds);
    return cb::compat::to_py(cb::ui::progress::clock_text(span));
}

// A reading as one line of text.
inline py::str line(const Snapshot& s) { return cb::compat::to_py(cb::ui::progress::line(detail_shim::to_new(s))); }

// python -m codebreaker progress [--once] [--interval SECONDS] [--log FILE]
inline int64_t main(const py::list<py::str>& argv) {
    std::vector<std::string> args{"progress"};
    for (const py::str& a : argv) args.push_back(a.utf8());
    try {
        return cb::cli::Cli("codebreaker", {cb::ui::progress::command(detail_shim::read_store)}).run(args);
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

}  // namespace codebreaker::ui::progress
