// islands.hpp -- typed islands inside the dynamic build (pyengine --dynamic --dyn-typed DIR).
//
// DIR is the typed path's own output (`pyengine <package> --out DIR --cpp DIR/cpp`): coverage.json says which
// functions it proved, DIR/l2 holds their modules' units and DIR/cpp their typed C++ (l2cpp17 over py.hpp).
// A module function the typed path translated becomes an island when nothing it does can be told apart
// from the dynamic body by its caller:
//
//   its parameters   int (with the range its proof assumed: coverage.json's `ranges`), bool, float, str,
//                    bytes, or None / tuples of the last four -- values; nothing the caller could see changed
//   its result       those, and lists, dicts and bytearrays of values made fresh (copied out on return)
//   its body, and every typed function it calls: no writes to files or the environment, no sleeping, no
//                    reading the environment; module names read only when they are constants (const in the
//                    typed unit, of a value type, set from literals)
//   its shape        one def at module level, bound only by it, positional parameters only, one overload
//
// The dynamic emitter (dyn.cc) writes each island's dynamic body as usual, under another name, and in
// its place a wrapper with the same signature: it checks and unboxes the arguments, calls the typed body
// through islands.cpp and boxes the result; any other argument, or any exception of the typed body, goes
// to the dynamic body (native/pyengine/islands/pyisl.hpp).
//
// The judge's verdicts (--dyn-islands FILE: {"islands": {qualname: {"verdict": "agree" | "differs" | "not
// run", "why": ...}}}) narrow the set to the islands whose traces agreed with their dynamic versions.
#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace pyengine {

struct IslandParam {
    std::string name;            // the Python parameter
    std::string kind;            // int, bool, float, str, bytes, val (None-or / tuple values: by `spec`)
    std::string spec;            // val: pyisl::value's spec
    std::string ctype;           // val: the typed side's C++ type (std::optional<py::str> ...)
    int64_t lo = INT64_MIN, hi = INT64_MAX;   // int: the range its callers give (the proof's assumption)
};
struct Island {
    std::string qual, module, name;     // codebreaker.x.f, codebreaker.x, f
    std::string ns;                     // the typed C++ namespace (codebreaker::x) and function name
    std::string cpp;
    std::vector<IslandParam> params;
    bool void_ret = false;
    int line = 0;
    std::vector<std::string> modules;   // the typed modules its body reaches (their C++ is compiled in)
};
struct IslandSet {
    std::string typed_dir;                          // DIR (DIR/cpp is compiled into the program)
    std::map<std::string, Island> emit;             // qualname -> the islands written
    std::map<std::string, std::string> not_islands; // qualname -> why not (translated functions only)
    std::map<std::string, std::string> held;        // qualname -> the judge's verdict that held it back
};

// reads DIR's coverage.json and units; verdicts (optional) as above. False with `err` when DIR is unusable.
// ints_unranged: an int parameter with no recorded range takes any int64 (the checked policy: the typed body
// raises OverflowError past it) -- for a native module, which has no dynamic body to send other values to
bool load_islands(const std::string& typed_dir, const std::string& verdicts, IslandSet& out, std::string& err, bool ints_unranged = false);
std::string export_params(const Island& is);
std::string islands_header(const IslandSet& s);   // islands.hpp: the exports, plain C++ (both sides include it)
std::string islands_glue(const IslandSet& s);     // islands.cpp: the typed side, each export calling its typed function
std::string islands_report(const IslandSet& s);   // islands.json: what was written, held, and why the rest are not islands
std::string islands_funcs(const IslandSet& s);    // islands.funcs: module:qualname per island (PYRT_TRACE_FUNCS for the judge)

// The judge: the program's islands.json, a folder of PYRT_TRACE files (PYRT_TRACE=DIR/t_%p.txt) of runs with the
// islands on and one of the same runs under PYISL_OFF=1. An island's body is a function of its arguments, so its
// calls are compared by their arguments: for every argument list met in both runs the outcomes (the result's repr
// hash, or the exception's type) must be the same. Writes the verdicts (--dyn-islands' FILE); 1 if any differs.
int judge_islands(const std::string& islands_json, const std::string& on_dir, const std::string& off_dir, const std::string& out);

}  // namespace pyengine
