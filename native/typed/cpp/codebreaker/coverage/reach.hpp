// codebreaker/coverage/reach.hpp -- typed C++ of codebreaker.coverage.reach (see reach.cpp).
//
// reach.py -- throw anything at CodeBreaker and see how far each program gets.
//
//     python -m codebreaker reach <file or folder> ... [--per-platform N] [--judge] [--timeout S]
//                                                      [--containers N] [--name RUN] [--again]
//     python -m codebreaker reach --report [RUN]       the last results again, without running anything
//
// Every file given -- and every file in every folder given, and the programs inside every disc image, Xbox 360 package or
// Games-on-Demand folder found -- is taken through these steps, as far as it goes:
//
//     found        what the file is (formats/detect), or "unknown"
//     platform     the platform it belongs to (codebreaker/platforms), or the platform CodeBreaker has no lane for yet,
//                  named ("Game Boy Advance (ARM7)": a gap, counted)
//     judged       with --judge: the platform's lane reconstructs it and runs its functions beside the original's
//                  (`measure`, in a process of its own, stopped after --timeout seconds); the function store's tally
//
// Load stays low on purpose: one program at a time, at most --per-platform programs of each platform (default 5: spread
// evenly across their sizes), at most --containers programs taken out of each container (default 20). Results are kept in
// state/reach/<run>.json by each file's content hash and the tools' revision, so a program already measured with the same
// tools is not measured again (--again measures it anyway). The report is written beside them, <run>.md.
//
// Typing notes:
//   * a result row (the Python's dict with the keys found, platform, lane, family, path, origin, size, sha and, once picked
//     and judged, picked, judged, tools) is the struct `Row`; `Judged` is {tally, seconds, stopped}.  A row read back from
//     a results file keeps the keys this version does not know in `extra`.  to_value(row) is the dict in the Python's order.
//   * `codebreaker.platforms` (the registry of platform lanes) has no typed port: `platform_identify()` is the hook that
//     answers platforms.identify(path) (its title, name, whether it has a lane, family and container); unset it raises an
//     ImportError, as a missing module would.
//   * `judge` ran `python -m codebreaker measure PATH` in a child process held in a Windows job; here the child is
//     `measure_argv(path)` (default: this executable with `measure PATH`, as codebreaker.exe is run), held in the same kind
//     of job.  `clock()` is the Python's time.time() (a seam for a test).
//   * argparse is the one parser of this command: its options, abbreviations, errors (usage text on stderr, SystemExit 2)
//     and --help text are written out for this parser alone.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/inspect.hpp"
#include "py.hpp"

namespace codebreaker::coverage::reach {

// ---- hooks
// what platforms.identify(path) answers for a program of a platform (None: no platform recognises it)
struct PlatformInfo {
    py::str title, name, family, container;
    bool lane = false;                          // the platform has a lane (p.lane)
};
using PlatformIdentify = std::function<std::optional<PlatformInfo>(const py::str& path)>;
PlatformIdentify& platform_identify();
// the command that measures one program (Python: [sys.executable, "-m", "codebreaker", "measure", path])
std::function<py::list<py::str>(const py::str& path)>& measure_argv();
// time.time()
std::function<double()>& clock();

// ---- the vocabulary
const py::list<py::str>& _PROGRAM_EXTS();
extern const py::list<py::str> _SKIP_FAMILIES;
extern const py::list<py::str> _CONTAINER_FORMATS;

// {"found", "platform", "lane", "family"} for one file
struct Described {
    py::str found, platform;
    std::optional<py::str> lane;
    py::str family;
};

struct Judged {
    py::dict<py::str, int64_t> tally;
    double seconds = 0.0;
    py::str stopped;
};

struct Row {
    py::str found, platform;
    std::optional<py::str> lane;
    py::str family, path, origin;
    int64_t size = 0;
    py::str sha;
    bool picked = false;
    std::optional<Judged> judged;               // absent: the key was not there
    std::optional<py::str> tools;
    formats::inspect::Value extra = formats::inspect::Value::dict();   // keys of a results file this version does not know
};
formats::inspect::Value to_value(const Row& r);

// (title, cpu) for a program of a platform CodeBreaker has no lane for, else None
std::optional<std::tuple<py::str, py::str>> _gap(const py::str& path, const py::bytes& head);

// what a file is, where detect has nothing to say
Described describe(const py::str& path);

py::str _sha(const py::str& path);

using Log = std::function<void(const py::str&)>;
// Programs inside a container (disc, package, GoD folder), written under state/reach/unpacked. Smallest first, at most `limit`.
py::list<py::str> _unpack(const py::str& path, int64_t limit, const Log& log);

// [(path, where it came from)] for every file under `inputs`, containers opened.
py::list<std::tuple<py::str, py::str>> programs(const py::list<py::str>& inputs, int64_t containers = 20, const Log& log = nullptr);

// The tools' revision: results measured by other tools are measured again ("?" when git does not say).
py::str _tools();

// Run the platform's lane on one program in its own process.
Judged judge(const py::str& path, int64_t timeout);

// (the function store's {class: count} for a program, "" or why there is none)
std::tuple<py::dict<py::str, int64_t>, py::str> tally_of(const py::str& path);

// `n` of `rows` evenly across their sizes (the smallest, the largest under `max_size`, and between)
py::list<Row> spread(const py::list<Row>& rows, int64_t n, int64_t max_size);

// The rows of a results file (state/reach/<run>.json: its "programs"); read as the Python reads them back.
py::list<Row> read_results(const py::str& path);

// The run. -> (the rows of this run, the report). log: default prints.
std::tuple<py::list<Row>, py::str> run(const py::list<py::str>& inputs, int64_t per_platform = 5, bool do_judge = false,
                                       int64_t timeout = 300, int64_t containers = 20, const py::str& name = py::str("reach"),
                                       bool again = false, int64_t max_size = int64_t(4) << 20, const Log& log = nullptr);

// Markdown: by platform, how many programs, how far they got.
py::str report(const py::list<Row>& rows);

extern const char* DOC;

// python -m codebreaker reach ...   (usage errors: usage text and message on stderr, then py::SystemExit(2))
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::coverage::reach
