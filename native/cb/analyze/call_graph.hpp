// cb/analyze/call_graph.hpp -- who calls whom, what refers to what, and what the exports reach.
//
//     const call_graph::Graph g = call_graph::build(image);
//     g.calls.at(f)         every call or jump out of function f: (site, kind, destination)
//     g.callers.at(f)       the functions that call f
//     g.refs                every reference from code to an address in the module, with how it is used
//     g.taken.at(f)         where function f's address is taken (code sites and data addresses)
//     g.users.at(name)      the functions that call import `name`
//     g.order               every function, callees before callers (a cycle in one piece)
//     g.reachable           the functions execution can reach from the exports and the entry point
//     g.reachable_with_tables   the same, also entering at every function whose address the module's data holds
//                           (tables of handlers, virtual tables)
//
// An operand counts as an address only when the file says so, where it does: a 32-bit module with relocations marks
// every address it holds, so a constant that merely looks like one is not taken for one. A module without them (the
// system's fixed-base DLLs) falls back to "lands inside the module". In 16-bit code the data segment's variables are
// the operands with a fixed offset, and the addresses are the places relocation writes.
//
// Reached means: called, jumped to, or having its address taken by something reached. A function nothing refers to
// at all is reported as unreferenced -- dead, or entered in a way the code does not show (a table index, an ordinal
// computed at run time). The counts also say how many bytes of the code sections no function reaches: tables,
// strings and padding kept among the code, and code whose start nothing gives away.
//
// The graph is deterministic: maps are ordered by address, a function's callees are taken in the order its code
// calls them, and the components of `order` follow from that.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/analyze/native.hpp"

namespace cb::analyze::call_graph {

enum class CallKind { Call, Import, Tail, Indirect };
std::string_view name_of(CallKind kind);   // "call", "import", "tail", "indirect"

struct Call {
    Va site = 0;
    CallKind kind = CallKind::Call;
    std::optional<Va> destination;        // none for an indirect call
};

// How code uses an address in the module.
enum class Access { Read, Write, ReadWrite, AddressTaken };
std::string_view name_of(Access how);     // "r", "w", "rw", "a"

struct Reference {
    Va function = 0;                      // the function holding the instruction
    Va instruction = 0;                   // its address
    Va address = 0;                       // what it refers to
    Access how = Access::Read;
};

// Where a function's address is taken: by an instruction of another function, or held in data.
struct Taken {
    bool in_code = true;
    std::optional<Va> function;           // in_code: the function whose instruction takes it
    Va at = 0;                            // the instruction's address, or the data address
};

struct Graph {
    std::map<Va, std::vector<Call>> calls;               // every function, by start
    std::map<Va, std::vector<Va>> callers;               // callee -> its callers, ascending
    std::vector<Reference> refs;                          // in function order, then instruction order
    std::map<Va, std::vector<Taken>> taken;
    std::map<std::string, std::vector<Va>> users;         // import name -> the functions that call it, ascending
    std::vector<Va> order;
    std::vector<std::vector<Va>> cycles;                  // groups of mutually recursive functions (ascending)
    std::set<Va> reachable;
    std::set<Va> reachable_with_tables;
    std::vector<Va> unreferenced;
};

// The graph of a module whose functions have been discovered (Image::discover).
Graph build(Image& image);

// ---- what analyse reports --------------------------------------------------------------------------------------

struct Counts {
    std::int64_t functions = 0;
    std::int64_t functions_found_from_calls = 0;
    std::int64_t edges = 0;
    std::int64_t call_sites = 0;
    std::int64_t import_call_sites = 0;
    std::int64_t tail_jumps = 0;
    std::int64_t indirect_call_sites = 0;
    std::int64_t imports_used = 0;
    std::int64_t leaf_functions = 0;
    std::int64_t recursive_groups = 0;
    std::int64_t address_taken_functions = 0;
    std::int64_t data_references = 0;
    std::int64_t roots = 0;
    std::int64_t reached_from_roots = 0;
    std::int64_t reached_with_data_tables = 0;
    std::int64_t unreferenced_functions = 0;
    std::int64_t code_bytes_decoded = 0;
    std::int64_t code_section_bytes_not_reached = 0;
};

// The "calls" fact of one function.
struct CallsFact {
    Va addr = 0;
    std::vector<std::string> callees;     // labels, by address
    std::vector<std::string> imports;     // names, sorted
    std::int64_t indirect = 0;            // indirect call sites
    std::vector<std::string> callers;     // labels
    std::int64_t address_taken = 0;       // places the function's address is taken
    bool reached = false;
};

struct Report {
    Counts counts;
    std::vector<CallsFact> facts;
    std::vector<std::string> text;
};

// What a report is made from, apart from the graph (so that a caller that holds the module in another shape can ask).
struct ReportInput {
    std::vector<Va> functions;
    std::size_t roots = 0;                                   // how many entry points the module has
    std::vector<Va> discovered;                              // function starts found from calls
    std::int64_t decoded_bytes = 0;                          // the bytes of every decoded instruction
    std::int64_t code_bytes_held = 0;                        // the bytes of the executable sections the file holds
    std::function<std::string(Va)> label;                    // a name for an address
    std::function<std::optional<std::string>(Va)> import_name;   // the name an import's address stands for
};
Report make_report(const ReportInput& input, const Graph& graph);

Report analyse(Image& image, const Graph& graph);

}  // namespace cb::analyze::call_graph
