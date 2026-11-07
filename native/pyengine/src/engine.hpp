// engine.hpp -- one run of the porting engine: load a package, solve its types, lower every
// function, and write L2 per module plus the coverage ledger.
#pragma once
#include <map>
#include <string>
#include <vector>

namespace pyengine {

struct EngineOptions {
    std::string root;                 // the package's folder
    std::string package;              // its import name (default: the folder's name)
    std::string policy = "strict";    // strict | checked
    std::string out;                  // where to write (empty: nothing written)
    bool extended = true;             // the engine's extensions (false: exactly the old engine's rules)
    bool log = false;
    std::vector<std::string> modules; // the units to write (empty: all)
    bool backend = true;              // (extended) every translated function written by the C++17 back end
    std::string cpp;                  // where to write the C++17 (empty: nothing written)
    std::string api;                  // the API maps the back end reads (default: <root>/java/api_maps)
    std::string stubs;                // a folder of <module>.pyi stubs (MonkeyType's output), read as parameter hints
    std::string hints;              // the hints folder (default: <root>/../native/pyengine/hints)
};

struct FnStatus {
    std::string status;               // translated | refused
    std::string reason;
    int lines = 0;
    std::string l2;                   // the MethodDecl's JSON when translated
    std::string ranges;               // {int parameter: [lo, hi]} the callers give (JSON; "None" unbounded)
    std::vector<std::string> all;     // a refused function's every blocker ("cls:detail"), its own
    std::vector<std::string> calls;   // the program's functions it calls
};

struct EngineResult {
    int rounds = 0;
    size_t modules = 0, functions = 0, translated = 0, lines = 0, translated_lines = 0;
    double seconds_solve = 0, seconds_lower = 0;
    std::map<std::string, FnStatus> fns;
    std::map<std::string, std::string> units;   // module -> its Unit's L2 JSON
    std::string error;
    size_t stub_files = 0, stub_applied = 0, stub_agree = 0, stub_conflicts = 0, stub_ignored = 0;
};

int run_engine(const EngineOptions& o, EngineResult& r);
std::string reason_key(const std::string& why);          // py2cpp's grouping of like refusals
std::string coverage_markdown(const EngineResult& r);

}  // namespace pyengine
