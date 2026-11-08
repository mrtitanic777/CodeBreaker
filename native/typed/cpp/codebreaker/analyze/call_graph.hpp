// codebreaker/analyze/call_graph.hpp -- C++ port of codebreaker.analyze.call_graph.
//
// call_graph.py -- who calls whom, what refers to what, and what the exports reach.
//
//     g = call_graph::build(image)
//     g.calls[f]          [(site, kind, destination)]  kind: "call" | "import" | "tail" | "indirect"
//     g.callers[f]        the functions that call f
//     g.refs              [(function, instruction, address, how)]   how: "r" | "w" | "rw" | "a"
//                         every reference from code to an address in the module ("a": its address
//                         is taken -- pushed, loaded or stored as a value)
//     g.taken[f]          where function f's address is taken (code sites and data addresses)
//     g.users[name]       the functions that call import `name`
//     g.order             every function, callees before callers (cycles in one piece)
//     g.reachable         the functions execution can reach from the exports and the entry point
//     g.reachable_with_tables   the same, also entering at every function whose address the
//                         module's data holds (tables of handlers, virtual tables)
//
// An operand counts as an address only when the file says so, where it does: a 32-bit module with
// relocations marks every address it holds, so a constant that merely looks like one is not taken
// for one. A module without them (the system's fixed-base DLLs) falls back to "lands inside the
// module". In 16-bit code the data segment's variables are the operands with a fixed offset, and
// the addresses are the places relocation writes.
//
// The graph is worked out in C++ (native/analyze, call_graph.cc) over the image cbanalyze.dll
// holds; this module makes its Graph from the answer and reports it.
//
// The counts also say how many bytes of the code sections no function reaches: tables, strings and
// padding kept among the code, and code whose start nothing gives away.
//
// Reached means: called, jumped to, or having its address taken by something reached. A function
// nothing refers to at all is reported as unreferenced -- dead, or entered in a way the code does
// not show (a table index, an ordinal computed at run time).
//
// The image the graph is built over (codebreaker.analyze.image) has no typed port yet, so this
// header names the part of it call_graph uses (Image, Function, Ins, Span below) and the shape of
// the answer the C++ engine gives to ask("call_graph.build") (RawGraph).
#pragma once

#include "py.hpp"

namespace codebreaker::analyze::call_graph {

// (tuples of operation names in the Python)
extern const py::list<py::str> READS;
extern const py::list<py::str> PURE_WRITES;

// ---- the parts of codebreaker.analyze.image that call_graph reads ---------------------------------

// One instruction (the real class also has mn, ops, text, rd, wr, flow and target).
struct Ins {
    int64_t addr = 0;
    int64_t size = 0;
};
using InsRef = std::shared_ptr<Ins>;

// One decoded function: `insns` in address order; `index` maps an instruction's address to its position.
struct Function {
    py::list<InsRef> insns;
    py::dict<int64_t, int64_t> index;
};

// (va, bytes, executable, writable, name, initialised length)
struct Span {
    int64_t va = 0;
    py::bytes data;
    bool executable = false;
    bool writable = false;
    py::str name;
    int64_t initialised = 0;
};

// What ask("call_graph.build") answers, as the engine writes it.
struct RawCall {            // [site, kind, destination or null]
    int64_t site = 0;
    py::str kind;
    std::optional<int64_t> destination;
};
struct RawTaken {           // ["code", function, site] or ["data", null, address]
    py::str kind;
    std::optional<int64_t> function;
    int64_t at = 0;
};
struct RawRef {             // [function, instruction address, address, how]
    int64_t function = 0;
    int64_t at = 0;
    int64_t va = 0;
    py::str how;
};
struct RawGraph {
    py::list<std::tuple<int64_t, py::list<RawCall>>> calls;
    py::list<std::tuple<int64_t, py::list<int64_t>>> callers;
    py::list<std::tuple<int64_t, py::list<int64_t>>> users;     // by the address standing for the import
    py::list<RawRef> refs;
    py::list<std::tuple<int64_t, py::list<RawTaken>>> taken;
    py::list<int64_t> order;
    py::list<py::list<int64_t>> cycles;
    py::list<int64_t> reachable;
    py::list<int64_t> reachable_with_tables;
    py::list<int64_t> unreferenced;
};

struct Graph;

class Image {
  public:
    virtual ~Image() = default;

    py::list<int64_t> functions;
    py::dict<int64_t, py::str> imports;     // address standing for an import -> "MODULE!name"
    py::list<int64_t> roots;
    py::list<Span> spans;
    py::set<int64_t> discovered;            // function starts found here that the description did not have

    virtual RawGraph ask_call_graph() = 0;                       // img._handle().ask("call_graph.build")
    virtual std::shared_ptr<Function> function(int64_t start) = 0;  // the decoded function starting at `start`
    virtual py::str label(int64_t va) = 0;                       // a name for an address

    // every function, decoded (a generator in the Python)
    py::list<std::shared_ptr<Function>> each();
    // img.memo("call_graph", build): worked out once per image
    std::shared_ptr<Graph> memo_call_graph;
};

// ---- the graph -----------------------------------------------------------------------------------

struct Call {               // (site, kind, destination)
    int64_t site = 0;
    py::str kind;
    std::optional<int64_t> destination;
};
struct Taken {              // ("code", function, site) | ("data", None, address)
    py::str kind;
    std::optional<int64_t> function;
    int64_t at = 0;
};
struct Reference {          // (function, instruction, address, how)
    int64_t function = 0;
    InsRef instruction;
    int64_t address = 0;
    py::str how;
};

struct Graph {
    py::dict<int64_t, py::list<Call>> calls;
    py::dict<int64_t, py::list<int64_t>> callers;
    py::list<Reference> refs;
    py::dict<int64_t, py::list<Taken>> taken;
    py::dict<py::str, py::list<int64_t>> users;
    py::list<int64_t> order;
    py::list<py::list<int64_t>> cycles;
    py::set<int64_t> reachable;
    py::list<int64_t> unreferenced;
    py::set<int64_t> reachable_with_tables;
};

// ---- what analyse reports --------------------------------------------------------------------------

struct Counts {
    int64_t functions = 0;
    int64_t functions_found_from_calls = 0;
    int64_t edges = 0;
    int64_t call_sites = 0;
    int64_t import_call_sites = 0;
    int64_t tail_jumps = 0;
    int64_t indirect_call_sites = 0;
    int64_t imports_used = 0;
    int64_t leaf_functions = 0;
    int64_t recursive_groups = 0;
    int64_t address_taken_functions = 0;
    int64_t data_references = 0;
    int64_t roots = 0;
    int64_t reached_from_roots = 0;
    int64_t reached_with_data_tables = 0;
    int64_t unreferenced_functions = 0;
    int64_t code_bytes_decoded = 0;
    int64_t code_section_bytes_not_reached = 0;
};
struct Fact {               // {"addr", "kind": "calls", "callees", "imports", "indirect", "callers", "address_taken", "reached"}
    int64_t addr = 0;
    py::str kind;
    py::list<py::str> callees;
    py::list<py::str> imports;
    int64_t indirect = 0;
    py::list<py::str> callers;
    int64_t address_taken = 0;
    bool reached = false;
};
struct Analysis {           // {"counts", "facts", "text"}
    Counts counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

// The graph, made from the answer of the engine's call_graph.build.
std::shared_ptr<Graph> _build(Image& img);
std::shared_ptr<Graph> build(Image& img);
Analysis analyse(Image& img);

}  // namespace codebreaker::analyze::call_graph
