// codebreaker/analyze/classes.hpp -- interface of codebreaker.analyze.classes (typed C++; implementation pending).
//
// classes.py -- C++ in a module: virtual tables, classes and their hierarchy, MFC's class records and message maps,
// and exception-handling tables.
//
//     classes::vtables(img)          [{at, slots, installed_by, com}]
//     classes::runtime_classes(img)  MFC's CRuntimeClass records: name, object size, base
//     classes::message_maps(img)     MFC message maps: the class, and every entry as its macro
//     classes::hierarchy(img)        [{name, vtable, bases, size, methods, evidence}]
//     classes::exception_tables(img) C++ try/catch tables and __try/__except scope tables
//
// A virtual table is a table of function addresses whose own address the code stores into an object; a class's NAME
// is in the binary as run-time type information or as MFC's CRuntimeClass record; without either, inheritance is
// inferred and said to be (evidence "probably: ..."). A message map is found as MFC lays it out (24-byte entries ending
// in a zero entry) and tied to its class through GetMessageMap. 32-bit modules only (16-bit tables are in globals).
//
// Typing notes:
//   * hierarchy() answers `Class`: core16::ClassRecord (the record the source writers share, core16_types.hpp) plus the
//     two keys only the instance layout adds (`layout`, `fields`).
//   * The dicts of the Python are records: Vtable, RuntimeClass, MessageMap (its entries are 6-int tuples), CxxTable /
//     CxxTry, ExceptionFrame, ExceptionTables.
//   * runtime_classes() answers {table address: RuntimeClass}.
//   * analyse(): five kinds of fact, a std::variant of five records; counts is a dict of ints.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/analyze/image.hpp"
#include "codebreaker/core16_types.hpp"
#include "py.hpp"

namespace codebreaker::analyze::classes {

// ---- virtual tables ----
struct Vtable {
    int64_t at = 0;
    py::list<int64_t> slots;                   // the functions, in table order
    py::list<int64_t> installed_by;            // functions that store the table's address into an object, sorted
    py::list<int64_t> at_offsets;              // the offsets they store it at, sorted
    py::list<int64_t> static_objects;          // objects in the data whose first word points at it, sorted
    bool com = false;                          // QueryInterface/AddRef/Release shape (first three remove 12, 4, 4)
    bool dispatch = false;                     // ... and IDispatch's four more (8, 16, 24, 36)
};
std::shared_ptr<py::list<Vtable>> _vtables(image::Image& img);
std::shared_ptr<py::list<Vtable>> vtables(image::Image& img);             // memo "classes.vtables"

// ---- run-time type information ----
// `.?AVCFoo@@` -> "CFoo";  `.?AVInner@Outer@@` -> "Outer::Inner"; None when it is not a decorated class name.
std::optional<py::str> _decorated(const py::str& name);
std::optional<py::str> _type_name(image::Image& img, int64_t descriptor);
// (class name, [base names]) from the type information before a virtual table, or None.
std::optional<std::tuple<py::str, py::list<py::str>>> _rtti(image::Image& img, int64_t table);

// ---- MFC ----
// The address the code at `va` returns, when it is just `mov eax, offset X / ret` (or `mov eax, [import] / ret`).
std::optional<int64_t> _returned(const image::Image& img, int64_t va);
// {function: address} for the functions that just return an address (memo "classes.getters").
std::shared_ptr<py::dict<int64_t, int64_t>> _returns_address(image::Image& img);
// The class an imported CRuntimeClass record belongs to: ?classCWnd@CWnd@@2UCRuntimeClass@@B.
py::str _class_of_import(const py::str& name);

struct RuntimeClass {                          // MFC's CRuntimeClass record
    int64_t at = 0;
    py::str name;
    int64_t size = 0;                          // object size
    std::optional<int64_t> schema;             // None: 0xFFFF
    bool creatable = false;
    std::optional<py::str> base;
};
std::shared_ptr<py::dict<int64_t, RuntimeClass>> _runtime_classes(image::Image& img);
std::shared_ptr<py::dict<int64_t, RuntimeClass>> runtime_classes(image::Image& img);   // memo "classes.runtime"

// One message map entry: the six words (message, code, id, last id, signature, handler).
using MapEntry = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>;
// An entry as the MFC macro that makes it.
py::str _macro(const image::Image& img, const MapEntry& e);
py::str _ids(int64_t first, int64_t last);

struct MessageMap {
    int64_t at = 0;
    py::list<MapEntry> entries;
    std::optional<int64_t> map;                // the AFX_MSGMAP {base, entries} record that points at it
    py::list<int64_t> class_tables;            // vtables of the classes it belongs to, sorted
    std::optional<int64_t> base_map;
};
std::shared_ptr<py::list<MessageMap>> _message_maps(image::Image& img);
std::shared_ptr<py::list<MessageMap>> message_maps(image::Image& img);

// ---- the hierarchy ----
// One class: core16::ClassRecord + the instance layout's name and field count (a structure whose first field is
// given the table's address).
struct Class : core16::ClassRecord {
    std::optional<py::str> layout;
    std::optional<int64_t> fields;
};
std::shared_ptr<py::list<Class>> _hierarchy(image::Image& img);
std::shared_ptr<py::list<Class>> hierarchy(image::Image& img);            // memo "classes.hierarchy"

// ---- exception handling ----
struct CxxTry {
    int64_t low = 0, high = 0;
    py::list<std::tuple<py::str, int64_t>> catches;   // (type, handler)
};
struct CxxTable {
    int64_t at = 0;
    int64_t states = 0;
    int64_t cleanups = 0;
    py::list<CxxTry> tries;
};
// The entries of a __try scope table: (enclosing level or None, filter or None, handler).
using Scope = std::tuple<std::optional<int64_t>, std::optional<int64_t>, int64_t>;
struct ExceptionFrame {
    int64_t function = 0;
    int64_t handler = 0;
    std::optional<int64_t> cxx;                // the C++ table the frame's handler thunk hands to the run-time
    std::optional<int64_t> scope_table;
    std::optional<py::list<Scope>> scopes;     // absent for a frame with no scope table
};
struct ExceptionTables {
    py::list<CxxTable> cxx;
    py::list<ExceptionFrame> frames;
};
py::list<CxxTable> _cxx_tables(image::Image& img);
py::list<Scope> _scope_table(const image::Image& img, int64_t at);
std::shared_ptr<ExceptionTables> _exception_tables(image::Image& img);
std::shared_ptr<ExceptionTables> exception_tables(image::Image& img);

// ---- analyse() ----
struct ClassFact {                             // kind "class"
    py::str kind = py::str("class");
    py::str name;
    int64_t vtable = 0;
    int64_t virtual_functions = 0;
    py::list<py::str> methods;                 // first 64, as labels
    py::list<py::str> bases;
    py::list<py::str> evidence;
    std::optional<int64_t> size;
    py::list<py::str> constructors_or_destructors;
    bool com_interface = false;
    std::optional<py::str> layout;
    std::optional<int64_t> fields;
    std::optional<int64_t> message_map;
    std::optional<bool> size_exact;
};
struct MfcClassFact {                          // kind "mfc_class": a RuntimeClass plus kind
    py::str kind = py::str("mfc_class");
    RuntimeClass record;
};
struct MessageMapFact {                        // kind "message_map"
    py::str kind = py::str("message_map");
    int64_t at = 0;
    std::optional<py::str> class_;             // "class" in the Python
    py::list<py::str> entries;                 // as macros
    std::optional<int64_t> base_map;
};
struct TryBlockFact {                          // one try block of a cxx_exception_table
    py::str states;                            // "low..high"
    py::list<py::str> catches;                 // "catch (T) at <hex>"
};
struct CxxTableFact {                          // kind "cxx_exception_table"
    std::optional<int64_t> addr;               // the owning function
    py::str kind = py::str("cxx_exception_table");
    int64_t at = 0, states = 0, cleanups = 0;
    py::list<TryBlockFact> try_blocks;
};
struct ScopeFact {
    std::optional<int64_t> enclosing;
    py::str kind;                              // "__except" | "__finally"
    std::optional<int64_t> filter;
    int64_t handler = 0;
};
struct ExceptionFrameFact {                    // kind "exception_frame"
    int64_t addr = 0;
    py::str kind = py::str("exception_frame");
    py::str handler;                           // label
    std::optional<py::list<ScopeFact>> scopes;
};
using AnyFact = std::variant<ClassFact, MfcClassFact, MessageMapFact, CxxTableFact, ExceptionFrameFact>;
struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<AnyFact> facts;
    py::list<py::str> text;
};
Analysis analyse(image::Image& img);

}  // namespace codebreaker::analyze::classes
