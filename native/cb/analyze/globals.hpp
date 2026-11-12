// cb/analyze/globals.hpp -- a module's global variables and its initialised tables of addresses.
//
//     const call_graph::Graph graph = call_graph::build(image);
//     const strings::Table table = strings::build_table(image, graph);
//     std::map<Va, Variable> vars = globals::variables(image, graph, table);
//     std::vector<PointerTable> tables = globals::pointer_tables(image, table);
//     globals::Report report = globals::analyse(vars, tables, label);
//
// A variable is an address the code reads or writes by name -- in 32-bit code an operand that is an address inside the
// module, in 16-bit code a fixed offset into the data segment. For each:
//
//     size            the widest access to it
//     type            what its uses say it is: a string, a function pointer (called through, or initialised with a
//                     function's address), a pointer (what it holds is dereferenced), a far pointer, a float or
//                     double, a counter, the result of a named system call (a handle), an array (indexed, with its
//                     element size), else an integer of its size
//     readers/writers the functions that do each;  taken: its address is passed around
//     initial         the value the file gives it, the address it is given, or nothing (uninitialised)
//     values          the constants the code stores in it or compares it with (a small set is a state or a mode)
//     extent          bytes to the next thing known at a higher address -- room for an array or a structure
//
// A table of addresses is a run of places where the file says the loader writes an address (relocation records),
// outside the instructions: tables of handlers, virtual tables, arrays of string pointers; and, as "records",
// addresses repeating at a fixed stride (an array of structures with a pointer in it). A module without relocation
// records gets only the runs whose every entry is a function or a string.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/native.hpp"
#include "cb/analyze/strings.hpp"

namespace cb::analyze::globals {

// What the file gives a variable at start.
struct Initial {
    enum class Kind { Uninitialised, Value, Address };
    Kind kind = Kind::Uninitialised;
    std::uint64_t value = 0;              // Value: the bytes the file holds there, little-endian (at most 8)
    std::string label;                    // Address: "&" and the name of what the variable is given the address of
};

struct Variable {
    Va at = 0;
    std::int64_t size = 0;
    std::set<Va> readers, writers;        // functions
    std::int64_t taken = 0;               // times its address is taken
    std::set<std::string> notes;          // "array", "called", "float", "counter", "flags", "pointer", "result of X", ...
    std::set<std::int64_t> values;
    bool writable = false;
    std::int64_t element = 0;             // the element size of an indexed array
    std::int64_t extent = 0;
    Initial initial;
    std::string type;                     // "unsigned long", "char[12]", "function pointer", "counter (4 bytes)", ...
};

// Every variable of the module, by address.
std::map<Va, Variable> variables(Image& image, const call_graph::Graph& graph, const strings::Table& table);

enum class SlotKind { Function, Import, String, Code, Data };
std::string_view name_of(SlotKind kind);                  // "function" "import" "string" "code" "data"

struct PointerTable {
    enum class Shape { Run, Records };
    Va at = 0;
    std::int64_t count = 0;
    std::int64_t stride = 0;
    Shape shape = Shape::Run;
    std::vector<Va> targets;
    std::vector<SlotKind> kinds;          // what each target is
};
std::string_view name_of(PointerTable::Shape shape);      // "run" "records"

// The tables of addresses in the module's data, by address. Runs that are all code are jump tables (analyze/dispatch's).
std::vector<PointerTable> pointer_tables(Image& image, const strings::Table& table);

// ---- everything, as facts ---------------------------------------------------------------------------------------

struct Counts {
    std::int64_t globals = 0;
    std::int64_t typed_beyond_size = 0;
    std::int64_t pointers = 0;
    std::int64_t function_pointers = 0;
    std::int64_t strings = 0;
    std::int64_t arrays = 0;              // indexed arrays (a string is counted as a string)
    std::int64_t counters = 0;
    std::int64_t flags = 0;
    std::int64_t results_of_system_calls = 0;
    std::int64_t floats = 0;
    std::int64_t uninitialised = 0;
    std::int64_t written_never_read = 0;
    std::int64_t read_never_written = 0;
    std::int64_t pointer_tables = 0;
    std::int64_t record_tables = 0;
    std::int64_t table_entries = 0;
    std::int64_t function_table_entries = 0;
};

// A variable as reported: readers and writers counted, notes and values sorted (at most 24), no element size.
struct GlobalFact {
    Va at = 0;
    std::int64_t size = 0;
    std::int64_t readers = 0, writers = 0;
    std::int64_t taken = 0;
    std::vector<std::string> notes;
    std::vector<std::int64_t> values;
    bool writable = false;
    std::int64_t extent = 0;
    Initial initial;
    std::string type;
};

struct TableFact {
    Va at = 0;
    std::int64_t count = 0;
    std::int64_t stride = 0;
    PointerTable::Shape shape = PointerTable::Shape::Run;
    std::map<std::string, std::int64_t> holds;     // slot kind -> how many
    std::vector<std::string> first;                // labels of the first eight targets
};

struct Report {
    Counts counts;
    std::vector<GlobalFact> globals;               // by address
    std::vector<TableFact> tables;                 // by address
    std::vector<std::string> text;                 // a short summary for people
};

// The report from the parts; `label` names an address.
Report make_report(const std::map<Va, Variable>& variables, const std::vector<PointerTable>& tables,
                   const std::function<std::string(Va)>& label);

Report analyse(Image& image, const call_graph::Graph& graph, const strings::Table& table);

}  // namespace cb::analyze::globals
