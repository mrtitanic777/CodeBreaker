// cb/analyze/report.hpp -- `codebreaker analyze`: run the analyses on a module and write what they found.
//
//     codebreaker analyze <module> [--what a,b,c] [--out FILE] [--run N|all] [--facts] [--starts FILE] [--json] [--no-store] [--summary]
//
//     <module>   any module the corpus names, or a path: a 32-bit module (PE), a 16-bit module (NE) or a driver (LE); "vmm32.vxd#vmm" names one driver
//                inside a bundle
//     --what     which analyses, of: call_graph, conventions, globals, strings, structures, classes, dispatch, resources, observe
//                (default: all but observe)
//     --run N    also run N functions in the emulator (observe), "all" for every function (observe alone: the first hundred). 32-bit modules only.
//     --out FILE where the report goes (default: <state>/analyze/<module>.txt)
//     --facts    list every fact in the report, not only the counts and highlights
//     --starts   write the function starts found that the decompiler's description lacks, as lines of that description
//                (`F <address> <name> <bytes removed> [model]`)
//     --json     print the counts and timings as one JSON object
//     --summary  print one line per analysis instead of the report's path
//     --no-store do not write the facts to the function store
//
//     const report::Result r = report::run("lzexpand.dll", {.what = {Analysis::CallGraph}}, env);
//     for (const report::Section& s : r.sections) s.counts ...
//
// The analyses run in the order of the list above; each is given what the ones before it found (the call graph, the strings, the calling
// conventions, the structures). One failing analysis is a finding, not the end: its section says `failed` and the rest run. The facts of the
// analyses go to the fact table of the function store (analyze/facts.hpp), replacing what the same analysis stored for the module before:
// call_graph, conventions, globals, strings, structures, classes, dispatch, resources and observe store theirs.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/analyze/facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/core/cli.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/emulate/emulator.hpp"

namespace cb::analyze::report {

enum class Analysis { CallGraph, Conventions, Globals, Strings, Structures, Classes, Dispatch, Resources, Observe };

inline constexpr std::size_t kAnalyses = 9;
std::string_view name_of(Analysis analysis);                    // "call_graph" ...
std::optional<Analysis> from_name(std::string_view name);
std::vector<Analysis> defaults();                               // all but observe, in order
std::string names();                                            // "call_graph, conventions, ..."

// A count as the report names it: "functions", 18.
using Counts = std::vector<std::pair<std::string, std::int64_t>>;

struct Section {
    Analysis analysis = Analysis::CallGraph;
    double seconds = 0.0;
    Counts counts;
    std::vector<std::string> text;
    std::vector<facts::Row> rows;               // for the fact table; empty for an analysis whose facts are not stored
    std::vector<std::optional<Va>> row_at;      // the address of each row (none: a fact about the module), parallel to `rows`
    bool stores_facts = false;
    std::optional<std::string> failed;          // why the analysis did not run
};

struct Result {
    std::string module;                         // the file's name in lower case ("lz32.dll")
    std::filesystem::path path;
    ImageKind kind = ImageKind::Pe;
    int bits = 32;
    std::size_t functions = 0;
    std::size_t discovered = 0;                 // found from calls and held addresses beyond the decompiler's description
    double load_seconds = 0.0, total_seconds = 0.0;
    std::vector<Section> sections;
    std::string sha;                            // when stored
    std::vector<std::string> starts;            // the lines `--starts` writes
    std::shared_ptr<Image> image;               // the module the analyses ran on (labels for the facts)
};

// What the command needs of the rest of the program.
struct Environment {
    // A corpus name -> the module's file (corpus.retail); nullopt when there is none. A path that is a file is taken as it is.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    // The content hash facts are filed under; nullopt: the file is not one the corpus knows (it is hashed here).
    std::function<std::optional<std::string>(const std::filesystem::path&)> sha;
    // Note the module in the function store before its facts go there (FunctionStore::add_module).
    std::function<void(std::string_view sha, std::string_view name, const std::filesystem::path& path, std::string_view kind)> add_module;
    std::filesystem::path facts_file;           // the function store's file the fact table lives in; empty: nothing is stored
    std::filesystem::path state;                // reports are written to <state>/analyze/<module>.txt; the 16-bit description's files under <state>/analyze/work
    // What the description of a 16-bit module asks (argument sizes of the system's functions); none given: it knows nothing.
    decompile::describe::Knowledge* knowledge = nullptr;
    // The name of an exported function of a system module by its number ("KERNEL", 17 -> "OpenFile"); nullopt when not known.
    std::function<std::optional<std::string>(std::string_view module, std::uint32_t ordinal)> export_name;
    // What the emulator that runs a module's functions (observe) asks: how the headers declare a Windows function, and how many arguments an imported
    // function pops (`import_argc`: none given, the emulator's default). Its modules are found by `retail`.
    emulate::emulator::Environment emulator;
    emulate::emulator::ImportArgc import_argc;
};

struct Options {
    std::vector<Analysis> what;                 // empty: defaults()
    bool store = true;
    std::optional<std::size_t> observed = 100;  // the functions observe runs, the informative first (--run N); nullopt: every one (--run all)
};

// Load a module and run the analyses on it. UsageError for a module that is not found; UnsupportedError for one of a format not read here (a DOS program).
Result run(std::string_view module, const Options& options, const Environment& environment);

// The report as text. With `all_facts`, every fact is listed.
std::string text(const Result& result, bool all_facts);

// The counts and timings as one JSON document (module, kind, functions, counts, seconds).
std::string json(const Result& result);

// One line for each analysis: "call_graph     0.1s  functions 28, ...".
std::string summary(const Result& result);

// `analyze <module> ...`, declared; the program's command table registers it.
cli::Command command(const Environment& environment);

}  // namespace cb::analyze::report
