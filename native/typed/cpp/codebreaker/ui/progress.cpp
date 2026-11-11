// codebreaker/ui/progress.cpp -- typed C++ of codebreaker.ui.progress (see progress.hpp).
//
// progress.py -- watch a long run from outside it: every result a sweep or a module run produces goes into the
// function store as it is made, so the store is the progress.  The store is opened read-only, so watching cannot
// disturb the run.

#include "codebreaker/ui/progress.hpp"

#include <io.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>

#include "codebreaker/coverage/ledger.hpp"
#include "codebreaker/store/functions.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::ui::progress {

using py::str;
namespace ledger = coverage::ledger;
namespace F = store::functions;

std::function<double()>& clock() {
    static std::function<double()> c = [] { return py::time::time(); };
    return c;
}

namespace {

bool starts_with_ci(const str& s, const char* prefix) { return s.lower().startswith(str(prefix)); }
bool hidden(const str& n) { return n.startswith(str(".")); }   // (glob's wildcard does not match a leading dot)

// round(x, 1)
double round1(double x) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.1f", x);
    return std::strtod(buf, nullptr);
}

}  // namespace

int64_t _busy_workers() {
    int64_t busy = 0;
    py::list<str> top;
    try {
        top = py::os::listdir(py::tempfile::gettempdir());
    } catch (const py::OSError&) {
        return 0;
    }
    for (const str& a : top) {                                   // <temp>/cbjobs*/*/*.cur
        if (hidden(a) || !starts_with_ci(a, "cbjobs")) continue;
        str da = py::os::path::join(py::tempfile::gettempdir(), a);
        if (!py::os::path::isdir(da)) continue;
        py::list<str> mid;
        try {
            mid = py::os::listdir(da);
        } catch (const py::OSError&) {
            continue;
        }
        for (const str& b : mid) {
            if (hidden(b)) continue;
            str db = py::os::path::join(da, b);
            if (!py::os::path::isdir(db)) continue;
            py::list<str> notes;
            try {
                notes = py::os::listdir(db);
            } catch (const py::OSError&) {
                continue;
            }
            for (const str& c : notes) {
                if (hidden(c) || !c.lower().endswith(str(".cur"))) continue;
                str note = py::os::path::join(db, c);
                try {
                    busy += (py::os::path::getsize(note) != 0 && clock()() - py::os::path::getmtime(note) < 3600) ? 1 : 0;
                } catch (const py::OSError&) {
                }
            }
        }
    }
    return busy;
}

str _last_line(const str& path) {
    try {
        py::file f = py::open_rb(path);
        f.seek(std::max<int64_t>(0, py::os::path::getsize(path) - 4096));
        py::list<str> lines;
        for (const str& ln : py::splitlines(f.read().decode("utf-8", "replace")))
            if (!ln.strip().empty()) lines.append(ln);
        return !lines.empty() ? lines[-1].strip() : str("");
    } catch (const py::OSError&) {
        return str("");
    }
}

Snapshot snapshot(const std::optional<str>& log) {
    double now = clock()();
    Snapshot out;
    {
        std::time_t t = std::time(nullptr);
        std::tm tmv{};
        localtime_s(&tmv, &t);
        char buf[16];
        std::strftime(buf, sizeof buf, "%H:%M:%S", &tmv);
        out.time = str(buf);
    }
    out.workers = _busy_workers();
    out.log = log && !log->empty() ? _last_line(*log) : str("");
    out.store = F::default_path();
    ledger::Store fs = ledger::open_store();
    ledger::Ledger led = ledger::measure(fs);
    for (const ledger::Summary& r : led.kinds) {
        std::optional<str> digest;
        if (led.profiles.contains(r.kind)) digest = led.profiles.at(r.kind);
        if (digest && !digest->empty()) {
            KindReading k;
            k.kind = r.kind;
            k.name = r.name;
            k.modules = r.modules;
            k.measured = r.measured;
            k.functions = r.functions;
            for (const str& c : F::ORDER()) k.classes.set(c, r.classes.at(c));
            out.kinds.append(k);
        }
    }
    if (!fs) return out;
    int64_t fresh = 0;
    try {
        for (const auto& row : fs->recent(8)) {
            Recent r;
            r.module = engine::native_engine::cell_str(row[0]);
            str kind = engine::native_engine::cell_str(row[1]);
            r.kind = ledger::TITLE().contains(kind) ? ledger::TITLE().at(kind) : kind;
            r.results = engine::native_engine::cell_int(row[2]);
            r.age = std::max<int64_t>(0, static_cast<int64_t>(now - engine::native_engine::cell_double(row[3])));
            out.recent.append(r);
        }
        fresh = engine::native_engine::cell_int(fs->verdicts_since(now - static_cast<double>(WINDOW)));
    } catch (...) {
        fs->close();
        throw;
    }
    fs->close();
    out.rate = round1(static_cast<double>(fresh) * 60.0 / static_cast<double>(WINDOW));
    if (!out.recent.empty()) out.idle = out.recent[0].age;
    out.active = out.workers != 0 || (out.idle && *out.idle < 120);
    int64_t done = 0, left = 0, functions = 0;
    for (const KindReading& k : out.kinds) {
        done += k.measured;
        left += k.modules - k.measured;
        functions += k.functions;
    }
    if (fresh && done && left)                                    // as many functions a module as so far
        out.eta = static_cast<int64_t>(static_cast<double>(left) * (static_cast<double>(functions) / static_cast<double>(done)) /
                                       (static_cast<double>(fresh) / static_cast<double>(WINDOW)));
    return out;
}

str _clock(const std::optional<int64_t>& seconds) {
    if (!seconds) return str("-");
    auto hr = py::divmod(*seconds, 3600);
    int64_t h = std::get<0>(hr), rest = std::get<1>(hr);
    return h ? py::percent("%dh %02dm", h, rest / 60) : py::percent("%dm %02ds", rest / 60, rest % 60);
}

str line(const Snapshot& s) {
    py::list<str> parts;
    for (const KindReading& k : s.kinds) {
        py::list<str> classes;
        for (const str& c : F::ORDER())
            if (k.classes.at(c)) classes.append(py::percent("%s %d", F::TITLE().at(c), k.classes.at(c)));
        str shown = py::join(str(", "), classes);
        parts.append(py::percent("%s %d/%d modules, %d functions (%s)", k.name, k.measured, k.modules, k.functions,
                                 shown.empty() ? str("nothing yet") : shown));
    }
    if (!s.recent.empty()) parts.append(py::percent("last: %s %s ago", s.recent[0].module, _clock(s.idle)));
    parts.append(s.rate != 0.0 ? py::percent("%s results/min", py::to_str(s.rate)) : !s.active ? str("idle") : str("working"));
    if (s.workers) parts.append(py::percent("%d workers busy", s.workers));
    if (s.eta) parts.append(py::percent("about %s left", _clock(s.eta)));
    if (!s.log.empty()) parts.append(s.log.slice(0, 80));
    str joined = py::join(str(" | "), parts);
    return py::percent("[%s] %s", s.time, joined.empty() ? str("no results yet") : joined);
}

int64_t main(const py::list<str>& argv) {
    auto option = [&](const char* name, const std::optional<str>& dflt = std::nullopt) -> std::optional<str> {
        for (int64_t i = 0; i < argv.size(); i++)
            if (argv[i] == str(name)) return i + 1 < argv.size() ? std::optional<str>(argv[i + 1]) : dflt;
        return dflt;
    };
    double interval = std::max(1.0, py::float_from_str(*option("--interval", str("3"))));
    std::optional<str> log = option("--log");
    bool once = argv.contains(str("--once"));
    int64_t width = 0;
    for (;;) {
        str text = line(snapshot(log));
        if (once || !_isatty(1)) {
            py::print(text);
            std::fflush(stdout);
        } else {
            std::string shown = "\r" + text.slice(0, 200).ljust(width).utf8();
            std::fputs(shown.c_str(), stdout);
            std::fflush(stdout);
            width = text.slice(0, 200).size();
        }
        if (once) return 0;
        py::time::sleep(interval);
    }
}

}  // namespace codebreaker::ui::progress
