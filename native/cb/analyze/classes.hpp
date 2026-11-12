// cb/analyze/classes.hpp -- C++ in a module: virtual tables, classes and their hierarchy, MFC's class records and message maps, and
// exception-handling tables.
//
//     const Inputs in{image, graph, conventions};
//     const Model model = build_model(in);                       // the tables, the MFC records, the message maps
//     for (const Class& c : hierarchy(image, model, structures)) c.name, c.bases, c.evidence ...
//     const Analysis report = analyse(in, structures);           // every fact and the counts, as the report prints them
//
// A virtual table is a table of function addresses whose own address the code stores into an object; a class's NAME is in the binary as
// run-time type information or as MFC's CRuntimeClass record; without either, inheritance is inferred and said to be so (the
// evidence starts "probably"). A message map is found as MFC lays it out (24-byte entries ending in a zero entry) and tied to its class
// through GetMessageMap. C++ try/catch tables and `__try`/`__except` scope tables are read where the compiler leaves them. 32-bit modules
// only (16-bit tables are in globals): for another image every answer is empty.
//
// What this reads of the rest of analysis it is handed: the image, the call graph (where an address is taken), the calling-convention
// facts (how many bytes of arguments each virtual function removes: the shape of a COM interface), and the structures recovered (the
// instance layout of a class whose first field is given the table's address). An Image decodes its functions lazily, so these take it by
// reference and are not thread-safe on one image.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/convention_facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core16_types.hpp"

namespace cb::analyze::classes {

struct Inputs {
    Image& image;
    const call_graph::Graph& graph;
    const ConventionFacts& conventions;
};

// ---- virtual tables ---------------------------------------------------------------------------------------------------------

struct Vtable {
    Va at = 0;
    std::vector<Va> slots;                     // the functions, in table order
    std::vector<Va> installed_by;              // functions that store the table's address into an object, sorted
    std::vector<std::int64_t> at_offsets;      // the offsets they store it at, sorted
    std::vector<Va> static_objects;            // objects in the data whose first word points at it, sorted
    bool com = false;                          // QueryInterface/AddRef/Release shape (the first three remove 12, 4, 4)
    bool dispatch = false;                     // ... and IDispatch's four more (8, 16, 24, 36)
};
std::vector<Vtable> find_vtables(const Inputs& in);

// ---- run-time type information ----------------------------------------------------------------------------------------------

// `.?AVCFoo@@` is "CFoo"; `.?AVInner@Outer@@` is "Outer::Inner"; nothing when it is not a decorated class name.
std::optional<std::string> decorated_name(std::string_view name);
// The class name the type descriptor at `descriptor` holds.
std::optional<std::string> type_name(const Image& image, Va descriptor);
struct Rtti {
    std::string name;
    std::vector<std::string> bases;
};
// The class name and the base names from the type information before a virtual table.
std::optional<Rtti> rtti(const Image& image, Va table);

// ---- MFC --------------------------------------------------------------------------------------------------------------------

// The address the code at `va` returns, when it is just `mov eax, offset X / ret` (or `mov eax, [import] / ret`).
std::optional<Va> returned_address(const Image& image, Va va);
// {function: address} for the functions that just return an address.
std::map<Va, Va> returns_address(const Image& image);
// The class an imported CRuntimeClass record belongs to: ?classCWnd@CWnd@@2UCRuntimeClass@@B is "CWnd".
std::string class_of_import(std::string_view name);

struct RuntimeClass {                          // MFC's CRuntimeClass record
    Va at = 0;
    std::string name;
    std::int64_t size = 0;                     // object size
    std::optional<std::int64_t> schema;        // none: 0xFFFF
    bool creatable = false;
    std::optional<std::string> base;
};
// The records in address order.
std::vector<RuntimeClass> find_runtime_classes(const Image& image);

// One message map entry: the six words of MFC's AFX_MSGMAP_ENTRY.
struct MapEntry {
    std::int64_t message = 0;
    std::int64_t code = 0;                     // the notification code
    std::int64_t id = 0;                       // the first control or command id
    std::int64_t last_id = 0;
    std::int64_t signature = 0;
    Va handler = 0;
};
// An entry as the MFC macro that makes it.
std::string macro_of(const Image& image, const MapEntry& entry);
// `100` or `100, 120`.
std::string id_range(std::int64_t first, std::int64_t last);

struct MessageMap {
    Va at = 0;
    std::vector<MapEntry> entries;
    std::optional<Va> map;                     // the AFX_MSGMAP {base, entries} record that points at it
    std::vector<Va> class_tables;              // vtables of the classes it belongs to, sorted
    std::optional<Va> base_map;
};
std::vector<MessageMap> find_message_maps(const Image& image, std::span<const Vtable> vtables);

// What is found once and read by the rest.
struct Model {
    std::vector<Vtable> vtables;
    std::map<Va, Va> getters;                  // the functions that just return an address
    std::vector<RuntimeClass> runtime;
    std::vector<MessageMap> maps;
};
Model build_model(const Inputs& in);

// ---- the hierarchy ----------------------------------------------------------------------------------------------------------

// One class: the record the source writers share, with the instance layout's name and field count (a structure whose first field is
// given the table's address).
struct Class : core16::ClassRecord {
    std::optional<std::string> layout;
    std::optional<std::int64_t> fields;
};
// The classes by table address. `structures` are those recovered for the module.
std::vector<Class> hierarchy(const Image& image, const Model& model, std::span<const structures::Structure> structures);

// ---- exception handling -----------------------------------------------------------------------------------------------------

struct CxxCatch {
    std::string type;                          // the class, "..." for catch (...), "?" when it has no name
    Va handler = 0;
};
struct CxxTry {
    std::int64_t low = 0, high = 0;            // the states the block covers
    std::vector<CxxCatch> catches;
};
struct CxxTable {
    Va at = 0;
    std::int64_t states = 0;
    std::int64_t cleanups = 0;
    std::vector<CxxTry> tries;
};
// One entry of a `__try` scope table.
struct Scope {
    std::optional<Va> enclosing;               // the level that encloses it
    std::optional<Va> filter;                  // none: a __finally
    Va handler = 0;
};
struct ExceptionFrame {
    Va function = 0;
    Va handler = 0;
    std::optional<Va> cxx;                     // the C++ table the frame's handler thunk hands to the run-time
    std::optional<Va> scope_table;
    std::optional<std::vector<Scope>> scopes;  // absent for a frame with no scope table
};
struct ExceptionTables {
    std::vector<CxxTable> cxx;
    std::vector<ExceptionFrame> frames;
};
std::vector<CxxTable> find_cxx_tables(const Image& image);
std::vector<Scope> read_scope_table(const Image& image, Va at);
ExceptionTables find_exception_tables(Image& image, const call_graph::Graph& graph);

// ---- the report -------------------------------------------------------------------------------------------------------------

struct ClassFact {
    std::string name;
    Va vtable = 0;
    std::int64_t virtual_functions = 0;
    std::vector<std::string> methods;          // the first 64, as labels
    std::vector<std::string> bases;
    std::vector<std::string> evidence;
    std::optional<Va> size;
    std::vector<std::string> constructors_or_destructors;
    bool com_interface = false;
    std::optional<std::string> layout;
    std::optional<std::int64_t> fields;
    std::optional<Va> message_map;
    std::optional<bool> size_exact;
};
struct MfcClassFact {
    RuntimeClass record;
};
struct MessageMapFact {
    Va at = 0;
    std::optional<std::string> class_name;
    std::vector<std::string> entries;          // as macros
    std::optional<Va> base_map;
};
struct TryBlockFact {
    std::string states;                        // "low..high"
    std::vector<std::string> catches;          // "catch (T) at <hex>"
};
struct CxxTableFact {
    std::optional<Va> function;                // the owning function
    Va at = 0;
    std::int64_t states = 0, cleanups = 0;
    std::vector<TryBlockFact> try_blocks;
};
struct ScopeFact {
    std::optional<Va> enclosing;
    std::string kind;                          // "__except" | "__finally"
    std::optional<Va> filter;
    Va handler = 0;
};
struct ExceptionFrameFact {
    Va function = 0;
    std::string handler;                       // as a label
    std::optional<std::vector<ScopeFact>> scopes;
};
using Fact = std::variant<ClassFact, MfcClassFact, MessageMapFact, CxxTableFact, ExceptionFrameFact>;

struct Analysis {
    OrderedMap<std::string, std::int64_t> counts;   // in the order the report prints them
    std::vector<Fact> facts;
    std::vector<std::string> text;
};
Analysis analyse(const Inputs& in, std::span<const structures::Structure> structures);

}  // namespace cb::analyze::classes
