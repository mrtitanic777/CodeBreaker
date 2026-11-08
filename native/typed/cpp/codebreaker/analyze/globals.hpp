// codebreaker/analyze/globals.hpp -- typed C++ port of codebreaker.analyze.globals.
//
// globals.py -- a module's global variables and its initialised tables of addresses.
//
//     from codebreaker.analyze import image, globals as global_variables
//     img = image.load("comctl32.dll")
//     global_variables.variables(img)        {address: fact}
//     global_variables.pointer_tables(img)   [table]
//
// A variable is an address the code reads or writes by name -- in 32-bit code an operand that is
// an address inside the module, in 16-bit code a fixed offset into the data segment. For each:
//
//     size        the widest access to it
//     type        what its uses say it is (see _type): a string, a function pointer (called
//                 through, or initialised with a function's address), a pointer (what it holds is
//                 dereferenced), a far pointer, a float or double, a counter, the result of a named
//                 system call (a handle), an array (indexed, with its element size), else an
//                 integer of its size
//     readers, writers    the functions that do each;  taken: its address is passed around
//     initial     the value the file gives it, or "uninitialised"
//     values      the constants the code stores in it or compares it with (a small set is a state
//                 or a mode)
//     extent      bytes to the next thing known at a higher address -- room for an array or a
//                 structure that starts here
//
// A table of addresses is a run of places where the file says the loader writes an address
// (relocation records), outside the instructions: tables of handlers, virtual tables, arrays of
// string pointers; and, as "records", addresses repeating at a fixed stride (an array of structures
// with a pointer in it). A module without relocation records gets only the runs whose every entry
// is a function or a string.
//
// Both are worked out in C++ (native/analyze, globals.cc) over the image cbanalyze.dll holds.
//
// Typing notes:
//   * The image (codebreaker.analyze.image) has no typed port. Image below is the strings port's
//     Image (bits, label(), and the four answers of the strings questions that _ask makes first)
//     with the two globals answers added: ask_variables() and ask_tables() are the Python's
//     _ask(img, "variables") and _ask(img, "tables") -- h = img._handle(); h.names(img);
//     h.ask("globals." + what) -- already read into typed rows. img.memo(key, make) is the two
//     memo_* members.
//   * The dicts of the Python (a variable, a table, the facts, the counts) have fixed keys and are
//     records here (for_each_field gives their keys in order); the two kinds of fact together
//     are a std::variant.
//   * A variable's "initial" is a number or the string "uninitialised" (or a label): a
//     std::variant<int64_t, py::str>. The engine writes the held value unsigned; it is read as
//     int64_t (the values the engine holds are at most 4 bytes wide in the modules it reads).
#pragma once

#include <cstdint>
#include <optional>
#include <variant>

#include "py.hpp"
#include "codebreaker/analyze/strings.hpp"

namespace codebreaker::analyze::globals {

using Initial = std::variant<int64_t, py::str>;

// One row of the engine's "globals.variables" answer.
struct RawVariable {
    int64_t at = 0;
    int64_t size = 0;
    py::list<int64_t> readers;
    py::list<int64_t> writers;
    int64_t taken = 0;
    py::list<py::str> notes;
    py::list<int64_t> values;
    bool writable = false;
    int64_t element = 0;
    int64_t extent = 0;
    Initial initial = int64_t(0);
    py::str type;
};

// One row of the engine's "globals.tables" answer.
struct RawTable {
    int64_t at = 0;
    int64_t count = 0;
    int64_t stride = 0;
    py::str shape;                  // "run" | "records"
    py::list<py::str> kinds;        // "function" | "import" | "string" | "code" | "data"
    py::list<int64_t> targets;
};

// A variable, as variables() holds it (the Python's dict with keys in this order).
struct Variable {
    std::optional<int64_t> addr;     // None
    py::str kind = "global";
    int64_t at = 0;
    int64_t size = 0;
    py::set<int64_t> readers;
    py::set<int64_t> writers;
    int64_t taken = 0;
    py::set<py::str> notes;
    py::set<int64_t> values;
    bool writable = false;
    int64_t element = 0;
    int64_t extent = 0;
    Initial initial = int64_t(0);
    py::str type;
};
// What this module asks of the image.
class Image : public strings::Image {
public:
    virtual py::list<RawVariable> ask_variables() = 0;
    virtual py::list<RawTable> ask_tables() = 0;

    // img.memo("globals", ...) and img.memo("globals.tables", ...): worked out once per image
    std::optional<py::dict<int64_t, Variable>> memo_globals;
    std::optional<py::list<RawTable>> memo_tables;
};

using Tables = py::list<RawTable>;

// ---- analyse() -----------------------------------------------------------------------------------
struct Counts {
    int64_t globals = 0;
    int64_t typed_beyond_size = 0;
    int64_t pointers = 0;
    int64_t function_pointers = 0;
    int64_t strings = 0;
    int64_t arrays = 0;
    int64_t counters = 0;
    int64_t flags = 0;
    int64_t results_of_system_calls = 0;
    int64_t floats = 0;
    int64_t uninitialised = 0;
    int64_t written_never_read = 0;
    int64_t read_never_written = 0;
    int64_t pointer_tables = 0;
    int64_t record_tables = 0;
    int64_t table_entries = 0;
    int64_t function_table_entries = 0;
};
// a variable as analyse() reports it: readers and writers counted, notes and values sorted, no element
struct GlobalFact {
    std::optional<int64_t> addr;     // None
    py::str kind = "global";
    int64_t at = 0;
    int64_t size = 0;
    int64_t readers = 0;
    int64_t writers = 0;
    int64_t taken = 0;
    py::list<py::str> notes;
    py::list<int64_t> values;        // at most 24
    bool writable = false;
    int64_t extent = 0;
    Initial initial = int64_t(0);
    py::str type;
};
struct PointerTableFact {
    std::optional<int64_t> addr;     // None
    py::str kind = "pointer_table";
    int64_t at = 0;
    int64_t count = 0;
    int64_t stride = 0;
    py::str shape;
    py::dict<py::str, int64_t> holds;
    py::list<py::str> first;
};
using Fact = std::variant<GlobalFact, PointerTableFact>;
struct Analysis {
    Counts counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

template <class F> void for_each_field(const Variable& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("at", r.at); f("size", r.size); f("readers", r.readers);
    f("writers", r.writers); f("taken", r.taken); f("notes", r.notes); f("values", r.values);
    f("writable", r.writable); f("element", r.element); f("extent", r.extent); f("initial", r.initial);
    f("type", r.type);
}
template <class F> void for_each_field(const GlobalFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("at", r.at); f("size", r.size); f("readers", r.readers);
    f("writers", r.writers); f("taken", r.taken); f("notes", r.notes); f("values", r.values);
    f("writable", r.writable); f("extent", r.extent); f("initial", r.initial); f("type", r.type);
}
template <class F> void for_each_field(const PointerTableFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("at", r.at); f("count", r.count); f("stride", r.stride);
    f("shape", r.shape); f("holds", r.holds); f("first", r.first);
}
template <class F> void for_each_field(const Counts& r, F&& f) {
    f("globals", r.globals); f("typed_beyond_size", r.typed_beyond_size); f("pointers", r.pointers);
    f("function_pointers", r.function_pointers); f("strings", r.strings); f("arrays", r.arrays);
    f("counters", r.counters); f("flags", r.flags); f("results_of_system_calls", r.results_of_system_calls);
    f("floats", r.floats); f("uninitialised", r.uninitialised); f("written_never_read", r.written_never_read);
    f("read_never_written", r.read_never_written); f("pointer_tables", r.pointer_tables);
    f("record_tables", r.record_tables); f("table_entries", r.table_entries);
    f("function_table_entries", r.function_table_entries);
}
template <class F> void for_each_field(const Analysis& r, F&& f) {
    f("counts", r.counts); f("facts", r.facts); f("text", r.text);
}

// ---- the functions --------------------------------------------------------------------------------
// _variables: {address: variable}, from the engine's answer
py::dict<int64_t, Variable> _variables(Image& img);
// {address: variable}, worked out once per image
py::dict<int64_t, Variable> variables(Image& img);
// the pointer tables, worked out once per image
Tables pointer_tables(Image& img);
Analysis analyse(Image& img);

}  // namespace codebreaker::analyze::globals
