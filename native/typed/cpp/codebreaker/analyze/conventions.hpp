// codebreaker/analyze/conventions.hpp -- interface of codebreaker.analyze.conventions (typed C++; implementation pending).
//
// conventions.py -- how each function is called and what kind of function it is.
//
//     auto facts = conventions::functions(*img);          // {start: fact}
//     facts[va].convention   "stdcall" | "cdecl" | "thiscall" | "fastcall" | "register"
//                            16-bit: "pascal far" | "pascal near" | "cdecl far" | "cdecl near"
//     facts[va].args         number of stack arguments (words in 16-bit code), or None
//     facts[va].arg_source   what says so: "return" (`ret N`), "callers", "frame", "callers (at least)", ...
//     facts[va].reg_args     registers it reads before writing them (ecx: `this`)
//     facts[va].result       "none" | "value" | "flag" | "float" | "wide" | None (not known)
//     facts[va].shape        "import thunk" | "jump thunk" | "empty" | "constant" | "leaf" | "calls" | "not code"
//     ... frame, locals, saved registers, loops, inline string operations, floating point use, whether it takes a
//     variable argument list, a 16-bit function's entry sequence and, when it is a window, dialog, hook or
//     enumeration procedure, which.
//
// The facts are worked out in C++ (native/analyze, conventions.cc) over the image cbanalyze.dll holds; duplicates and
// the bridge imports are found here.
//
// Typing notes:
//   * A fact (the DLL's {"addr", "kind": "convention", ...} dict) is the record `Fact`. Keys the DLL leaves out when
//     they do not apply (thunk_of, constant, tail_calls, arg_slots, loops, string_ops, float_instructions,
//     float_args, variadic, entry, procedure) are std::optional / empty / false. Tests and other modules read it with
//     f.get("removes") (None when absent): the optionals carry that.
//   * analyse() reports `facts` as the rows of the convention table plus two other kinds of row ("duplicates",
//     "thunk_layer"): a std::variant of the three records. counts is {name: int}, kept as a dict (order = report order).
#pragma once

#include <cstdint>
#include <optional>
#include <variant>

#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::conventions {

// The 16/32-bit bridge APIs (THUNKCONNECT32 ... ), upper-cased.
extern const py::list<py::str> _THUNK_APIS;

// {"callers_use", "callers_ignore"}: how many callers use the result / ignore it
struct ResultEvidence {
    int64_t callers_use = 0;
    int64_t callers_ignore = 0;
};

// One function's facts (a row of the DLL's conventions.functions).
struct Fact {
    int64_t addr = 0;
    py::str kind = py::str("convention");
    py::str name;
    py::str shape;                                         // "import thunk" | "jump thunk" | "empty" | "constant" | "leaf" | "calls" | "not code"
    std::optional<py::str> thunk_of;                       // an import thunk's import ("USER32!X"), a jump thunk's target
    std::optional<int64_t> constant;                       // shape "constant": what it returns
    std::optional<py::list<py::str>> tail_calls;           // names of the functions it jumps to
    std::optional<py::str> convention;
    py::list<py::str> reg_args;                            // registers read before written
    std::optional<int64_t> removes;                        // bytes its return removes (0: caller cleans), None: no return seen
    std::optional<int64_t> args;
    std::optional<py::str> arg_source;
    std::optional<py::dict<py::str, int64_t>> arg_slots;   // {"+8": width read ...}: argument slots read
    bool frame = false;                                    // builds a frame (push ebp / mov ebp, esp)
    int64_t locals = 0;
    py::list<py::str> saved;                               // callee-saved registers it pushes
    std::optional<py::str> result;                         // "none" | "value" | "flag" | "float" | "wide" | None
    std::optional<ResultEvidence> result_evidence;
    std::optional<int64_t> loops;
    std::optional<py::list<py::str>> string_ops;
    std::optional<int64_t> float_instructions;
    std::optional<py::list<int64_t>> float_args;
    bool variadic = false;
    std::optional<py::list<py::str>> entry;                // 16-bit: notes on the entry sequence
    std::optional<py::str> procedure;                      // "window procedure" | "dialog procedure" | "hook" | ...
};
using Facts = py::dict<int64_t, Fact>;

// Bytes `ins` adds to what is pushed (negative: removes), or None when it is not plain.
std::optional<int64_t> stack_delta(const image::Ins& ins);

// A digest of a function's code that two copies of the same function share.
py::str _signature(image::Image& img, const image::Function& fn);

// The facts, worked out in C++ over the image the DLL holds.
std::shared_ptr<Facts> _functions(image::Image& img);

// {start: fact} for every function of the image (memo "conventions").
std::shared_ptr<Facts> functions(image::Image& img);

// Groups of functions with the same code (the same source compiled more than once), sorted by first address.
py::list<py::list<int64_t>> duplicates(image::Image& img, int64_t least = 6);

// The 16/32-bit bridge functions a module imports ([] when it is not such a layer).
py::list<py::str> thunk_layer(const image::Image& img);

// ---- analyse() ----
struct DuplicatesRow {                   // {"addr": g[0], "kind": "duplicates", "functions": [label ...]}
    int64_t addr = 0;
    py::str kind = py::str("duplicates");
    py::list<py::str> functions;
};
struct ThunkLayerRow {                   // {"addr": None, "kind": "thunk_layer", "imports": [...]}
    py::str kind = py::str("thunk_layer");
    py::list<py::str> imports;
};
using AnyFact = std::variant<Fact, DuplicatesRow, ThunkLayerRow>;
struct Analysis {
    py::dict<py::str, int64_t> counts;   // functions, convention_known, callee_removes_arguments, ... bridge_imports
    py::list<AnyFact> facts;
    py::list<py::str> text;
};
Analysis analyse(image::Image& img);

}  // namespace codebreaker::analyze::conventions
