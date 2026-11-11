// codebreaker/ui/progress.hpp -- typed C++ of codebreaker.ui.progress (see progress.cpp).
//
// progress.py -- watch a long run from outside it.
//
//     python -m codebreaker progress                 one status line, rewritten every few seconds
//     python -m codebreaker progress --once          print the status once and stop
//     python -m codebreaker progress --interval 10   seconds between readings (default 3)
//     python -m codebreaker progress --log FILE      also show the last line of a run's output file
//
//     Snapshot s = progress::snapshot();      // what the line, the terminal interface and the MCP server show
//
// A sweep or a module run is not asked to report anything: every result it produces goes into the function store as it
// is made, so the store is the progress. A reading says, per kind of module that can be reconstructed, how many modules
// of the corpus are measured under the verifier settings in force and how their functions stand (from the coverage
// ledger); which module results were last written for and how long ago; how many results arrived in the last ten
// minutes, and from that a rough time to the end of the corpus; and how many of the runner's worker processes are in the
// middle of an item (jobs/pool keeps a note per worker in the temporary folder). The store is opened read-only, so
// watching cannot disturb the run.
//
// Typing notes: the reading's dict is the struct `Snapshot` (its 'kinds' / 'recent' rows are `KindReading` / `Recent`;
// `rate` is a float, `idle` / `eta` are None when there is none).  `clock()` is the time the reading is taken at (the
// Python's time.time(); a seam so a test can fix it).  The terminal's isatty and Ctrl-C are the C runtime's.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "py.hpp"

namespace codebreaker::ui::progress {

inline constexpr int64_t WINDOW = 600;            // seconds of results the rate is taken over

// the time now, in seconds since the epoch (a seam; default: py::time::time)
std::function<double()>& clock();

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

// Worker processes of the job runner that are in the middle of an item right now.
int64_t _busy_workers();

// the last non-blank line of the end of a file ("" when it cannot be read)
py::str _last_line(const py::str& path);

// One reading.
Snapshot snapshot(const std::optional<py::str>& log = std::nullopt);

// "1h 05m" / "3m 07s" ("-" for None)
py::str _clock(const std::optional<int64_t>& seconds);

// A reading as one line of text.
py::str line(const Snapshot& s);

// python -m codebreaker progress [--once] [--interval SECONDS] [--log FILE]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::ui::progress
