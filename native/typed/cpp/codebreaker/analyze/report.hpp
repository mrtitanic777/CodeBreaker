// codebreaker/analyze/report.hpp -- typed C++ port of codebreaker.analyze.report.
//
// report.py -- run the analyses on a module and write what they found.
//
//     python -m codebreaker analyze <module> [--what a,b,c] [--out FILE] [--run N|all] [--facts]
//                                            [--starts FILE] [--json] [--no-store] [--summary]
//
//     <module>     any module the corpus names, or a path: a 32-bit module, a 16-bit module, or a
//                  driver ("vmm32.vxd#vmm" names one driver inside a bundle)
//     --what       which analyses, of: call_graph, conventions, globals, strings, structures,
//                  classes, dispatch, resources, observe      (default: all but observe)
//     --run N      also run N functions in the emulator (observe); "all" for every function.
//                  32-bit modules only; a few hundred functions take minutes.
//     --out FILE   where the report goes (default: state/analyze/<module>.txt)
//     --facts      list every fact in the report, not only the counts and highlights
//     --starts FILE  write the function starts found here that the decompiler's description lacks,
//                  as lines of that description (`F <address> <name> <bytes removed> [model]`)
//     --json       print the counts and timings as one JSON object
//     --summary    print one line per analysis instead of the report's path
//     --no-store   do not write the facts to the function store
//
// The facts go to the function store's `fact` table (analyze/facts), replacing what the same
// analysis stored for the module before.
//
//     auto r = report::run("comctl32.dll");          // counts: {analysis: {...}}, seconds: {...}, ...
//
// Notes on the port:
//   * Each analysis answers with its own typed record (call_graph::Analysis, strings::Analysis ...); report.py handles them as dicts
//     {"counts", "facts", "text"}. `Result` is that dict: the counts as ordered (name, value) pairs, the facts as `Fact` (the fact's
//     address and kind, then its other fields in order, as JSON), the text lines. `ANALYSES()` says how each analysis's record is read
//     into one (`convert`).
//   * The facts of conventions, structures, classes and dispatch are NOT converted: their typed records give an absent key and a
//     key that holds None alike, and the Python's dicts differ in which keys they have; with those four the facts list is empty (counts
//     and text are converted). So `--facts` lists, and the function store keeps, only the facts of call_graph, globals, strings and
//     resources.
//   * "observe" (the emulator) is not linked here: `observe_hook()` answers it when the program sets it; unset, asking for it raises
//     ImportError, as the Python re-raises one.
//   * Fact values are `Json` (knowledge::sources::Json): the one place the Python mixes types.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/analyze/image.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::analyze::report {

using Json = knowledge::sources::Json;

// One fact: its address (None for a fact of the module), its kind, its other fields in order.
struct Fact {
    std::optional<int64_t> addr;
    py::str kind;
    py::list<std::tuple<py::str, Json>> fields;
};

// What an analysis answers, as the Python's dict: counts (ordered), facts, text.
struct Result {
    py::list<std::tuple<py::str, Json>> counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

struct Analysis {
    py::str name;
    // the analysis run on the image; `limit` is observe's number of functions to run (None: all)
    std::function<Result(image::Image&, const std::optional<int64_t>& limit)> run;
};
const py::list<Analysis>& ANALYSES();
// the names of the analyses run unless --what says
const py::list<py::str>& DEFAULT();

// observe, when the program has linked the emulator's port
std::function<Result(image::Image&, const std::optional<int64_t>& limit)>& observe_hook();

// {"module", "path", "kind", "bits", "functions", "counts", "seconds", "results", "image", and "sha" when stored}
struct Run {
    py::str module, path, kind;
    int64_t bits = 0;
    int64_t functions = 0;
    py::list<std::tuple<py::str, py::list<std::tuple<py::str, Json>>>> counts;   // {analysis: {name: value}}
    py::list<std::tuple<py::str, double>> seconds;                              // {"load", each analysis, "total"}
    py::list<std::tuple<py::str, Result>> results;
    image::ImageP image;
    std::optional<py::str> sha;
    double second(const py::str& name) const;
    const py::list<std::tuple<py::str, Json>>* count_of(const py::str& analysis) const;
    const Result* result_of(const py::str& analysis) const;
};

// Analyse one module. `observed`: how many functions to run in the emulator when "observe" is asked for (None: all).
// ValueError for an analysis that is not one of ANALYSES.
Run run(const py::str& module, const std::optional<py::list<py::str>>& what = std::nullopt, bool store = true,
        const std::optional<int64_t>& observed = std::nullopt);

// The function starts found beyond the decompiler's description, as description lines
// (see decompile/describe): address, name, bytes of arguments the function removes (-1 when
// its return was not reached) and, for 16-bit code, how it is called.
py::list<py::str> starts(image::Image& img);

// What the analyses found that a description for the decompiler could carry.
py::list<py::str> _feedback(const Run& result);

// A fact's value as report text: addresses in hex, long lists cut.
py::str _value(const Json& v);

// The report as text.
py::str text(const Run& result, bool all_facts = false);

// python -m codebreaker analyze ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::analyze::report
