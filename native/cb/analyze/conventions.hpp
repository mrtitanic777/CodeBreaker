// cb/analyze/conventions.hpp -- how each function is called and what kind of function it is.
//
//     const call_graph::Graph graph = call_graph::build(image);
//     conventions::Facts facts = conventions::functions(image, graph);          // {start: fact}
//     facts.at(va).convention                "stdcall" | "cdecl" | "thiscall" | "fastcall" | "register"
//                                            16-bit: "pascal far" | "pascal near" | "cdecl far" | "cdecl near"
//     facts.at(va).args                      number of stack arguments (words in 16-bit code), when known
//     facts.at(va).arg_source                what says so: "return" (`ret N`), "callers" (the bytes callers remove after
//                                            calling it), "frame" (the argument slots it reads), "shape"
//     facts.at(va).reg_args                  registers it reads before writing them (ecx: `this`)
//     facts.at(va).result                    none | value | flag | float | wide (not known: nullopt)
//     facts.at(va).shape                     import thunk | jump thunk | empty | constant | leaf | calls | not code
//     ... frame, locals, saved registers, loops, inline string operations, floating point use, whether it takes a variable
//     argument list, a 16-bit function's entry sequence and, when it is a window, dialog, hook or enumeration procedure,
//     which.
//
// What each fact rests on is the code itself and its callers:
//
//   * a function that removes its own arguments says how many in its return instruction;
//   * one that leaves them to the caller is given away by its callers (`add esp, 8` after the call) and by the highest
//     argument slot it reads;
//   * a register read before anything wrote it was passed in;
//   * a result is a value the callers use, or that the function sets just before returning; callers that all ignore it,
//     and no path that sets it, mean there is none.
//
// The decompiler's description already carries the bytes a function removes on return. What it cannot yet be told, and this
// finds: the argument count of a caller-cleans function, register arguments, and a function that returns nothing.
// dispatch and structures read a smaller record of these facts (analyze/convention_facts.hpp): `summary()` makes it.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/convention_facts.hpp"
#include "cb/analyze/facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::analyze::conventions {

enum class Shape { ImportThunk, JumpThunk, Empty, Constant, Leaf, Calls, NotCode };
std::string_view name_of(Shape shape);   // "import thunk" "jump thunk" "empty" "constant" "leaf" "calls" "not code"

enum class ResultKind { None, Value, Flag, Float, Wide };
std::string_view name_of(ResultKind kind);   // "none" "value" "flag" "float" "wide"

// One function's facts.
struct Fact {
    Va addr = 0;
    std::string name;                                // its name, or its address in hex
    Shape shape = Shape::NotCode;
    std::optional<std::string> thunk_of;             // an import thunk's import ("USER32!X"), a jump thunk's target
    std::optional<std::int64_t> constant;            // shape Constant: what it returns
    std::vector<std::string> tail_calls;             // the names it jumps to, sorted
    std::optional<std::string> convention;
    std::vector<std::string> reg_args;               // registers read before written, sorted
    std::optional<std::int64_t> removes;             // bytes its return removes (0: the caller does); nullopt: no return seen
    std::optional<std::int64_t> args;
    std::optional<std::string> arg_source;
    std::map<std::int64_t, std::int64_t> arg_slots;  // offset of an argument slot the code reads -> the widest read
    bool frame = false;                              // builds a frame (push ebp / mov ebp, esp)
    std::int64_t locals = 0;
    std::vector<std::string> saved;                  // callee-saved registers it pushes
    std::optional<ResultKind> result;
    std::int64_t callers_use = 0, callers_ignore = 0;   // what the callers do with a result: the evidence for `result`
    std::int64_t loops = 0;                          // branches back to or before themselves
    std::vector<std::string> string_ops;             // copy fill scan compare load, sorted
    std::int64_t float_instructions = 0;
    std::vector<std::int64_t> float_args;            // argument slots a float instruction reads, ascending
    bool variadic = false;
    std::vector<std::string> entry;                  // 16-bit: notes on the entry sequence
    std::optional<std::string> procedure;            // "window procedure" | "dialog procedure" | "hook procedure" | "enumeration procedure"
};

using Facts = std::map<Va, Fact>;   // by function start; every function of the image has one

// Bytes `ins` adds to what is pushed (negative: removes), or nullopt when it is not plain.
std::optional<std::int64_t> stack_delta(const Instruction& ins);

// How every function is called, worked out from the code and the callers the graph knows.
Facts functions(Image& image, const call_graph::Graph& graph);

// The smaller record dispatch and structures read.
ConventionFacts summary(const Facts& facts);

// A fact as the document the fact table keeps: the keys the analysis has always written (the optional ones only when they
// apply), `addr` and `kind` ("convention") included.
knowledge::Value to_value(const Fact& fact);

// A digest of a function's code that two copies of the same function share: its instructions as text, a branch inside the
// function by its distance from the start, any other target by its name.
std::string signature(Image& image, const Function& function);

// Groups of functions with the same code (the same source compiled more than once), by their first address. `least` is
// the fewest instructions a function may have to count.
std::vector<std::vector<Va>> duplicates(Image& image, std::size_t least = 6);

// The 16/32-bit bridge functions a module imports (empty when it is not such a layer), upper case, sorted.
std::vector<std::string> thunk_layer(const Image& image);

// ---- the report ------------------------------------------------------------------------------------------------------------

struct Counts {
    std::int64_t functions = 0;
    std::int64_t convention_known = 0;
    std::int64_t callee_removes_arguments = 0;
    std::int64_t caller_removes_arguments = 0;
    std::int64_t argument_count_known = 0;
    std::int64_t argument_count_from_return = 0;
    std::int64_t argument_count_from_callers = 0;
    std::int64_t argument_count_from_frame = 0;
    std::int64_t register_arguments = 0;
    std::int64_t this_in_ecx = 0;
    std::int64_t result_known = 0;
    std::int64_t result_none = 0;
    std::int64_t import_thunks = 0;
    std::int64_t jump_thunks = 0;
    std::int64_t tail_calling = 0;
    std::int64_t empty_or_constant = 0;
    std::int64_t leaf = 0;
    std::int64_t with_loops = 0;
    std::int64_t inline_string_operations = 0;
    std::int64_t floating_point = 0;
    std::int64_t variadic = 0;
    std::int64_t system_called_procedures = 0;
    std::int64_t duplicate_groups = 0;
    std::int64_t duplicate_functions = 0;
    std::int64_t bridge_imports = 0;
};

// A group of identical functions, as a fact: the first one's address and every one's name.
struct DuplicatesFact {
    Va addr = 0;
    std::vector<std::string> functions;
};

struct Report {
    Counts counts;
    Facts facts;
    std::vector<DuplicatesFact> duplicates;
    std::vector<std::string> bridge_imports;   // thunk_layer()
    std::vector<std::string> text;
};

Report analyse(Image& image, const call_graph::Graph& graph);

// The rows the fact table keeps of a report: one `convention` row per function, a `duplicates` row per group (addressed by its
// first function), and a `thunk_layer` row (no address) when the module is a bridge layer.
std::vector<facts::Row> fact_rows(const Report& report, ImageKind kind);

}  // namespace cb::analyze::conventions
