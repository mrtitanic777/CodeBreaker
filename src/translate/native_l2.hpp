// translate/native_l2.hpp -- the L2 core library (cb/translate/l2, the library cb_l2) as a typed client: the C back end, the L2 node
// model's reader and writer, and the schema check, which the library does, asked through its one entry point.
//
// The library is linked into the program and has one C entry point -- `l2_call(name, arguments, ...)` -- that takes the
// arguments of an entry point as a JSON array and answers a JSON value. `Library::builtin()` hands over its two functions
// (a test may hand its own: `Library(call, free)`); this is the one place that knows the protocol. Everything else is typed: each entry point is a method, its arguments are what the
// library reads, its result a struct, its failure an exception.
//
//     const Library lib = Library::builtin();
//     std::string c = lib.function(doc, moved);                   // C text of one L2 function document
//     ModuleNames names = lib.module_names32(doc);
//
// The wire form is the library's own. A document is L2 JSON (cb/translate/l2.hpp writes and reads it as typed nodes; the
// back end entry points take it as the `Json` it is). What JSON has no word for, the library says tagged: a tuple as
// {"$t": [...]}, a set as {"$s": [...]}, a dict with keys that are not text as {"$d": [[key, value], ...]}, a float that is
// not finite as {"$f": "nan"}. The facts a function is written with and the plans the module planners answer are made of
// those (a record of text, numbers, tuples and dicts of other keys per entry): they are passed and returned as `Json` with
// the tags as they are; everything with a fixed shape is typed. An integer above the 64-bit range is not kept whole.
//
// Errors. A failure the library reports is a LibraryError carrying the library's own word for it (`type()`: "KeyError",
// "ValueError", "TypeError", "IndexError", "AttributeError" or another); a node the C back end does not write is Declined
// (and Cpp17Declined for the C++17 back end's), so a caller that tries a back end and falls back to another catches those.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/error.hpp"
#include "core/json_fwd.hpp"
#include "core/ordered_map.hpp"

namespace cb::translate::native_l2 {

// An error the library reports. `type()` is its word for the kind ("KeyError", "ValueError", "Decline" ...).
class LibraryError : public Error {
public:
    LibraryError(std::string type, const std::string& message) : Error(message), type_(std::move(type)) {}
    const std::string& type() const { return type_; }
    CB_ERROR_BOILERPLATE(LibraryError)

private:
    std::string type_;
};

// The C back end does not write a node (the caller tries another way).
class Declined : public LibraryError {
public:
    explicit Declined(const std::string& message) : LibraryError("Decline", message) {}
    CB_ERROR_BOILERPLATE(Declined)
};

// The C++17 back end does not write a node.
class Cpp17Declined : public LibraryError {
public:
    explicit Cpp17Declined(const std::string& message) : LibraryError("Cpp17Decline", message) {}
    CB_ERROR_BOILERPLATE(Cpp17Declined)
};

// ---- what the entry points answer -----------------------------------------------------------------------------------

// A module written by the C++17 back end: its header and source, and where each function's body is in the source: {function
// name: (first line, last line)}, so a compiler's error can be pinned on it.
struct Cpp17Unit {
    std::string header;
    std::string source;
    OrderedMap<std::string, std::pair<std::int64_t, std::int64_t>> spans;
};

// A module's C text, and the assembly of what stayed assembly: {function name: its assembly}.
struct ModuleText {
    std::string text;
    OrderedMap<std::string, std::string> assembly;
};

// What a 32-bit function's document names: its own names, the functions it calls, those called through a pointer, and how
// many arguments each call passes.
struct ModuleNames {
    std::set<std::string> names;
    std::set<std::string> calls;
    std::set<std::string> called_through;
    std::vector<std::pair<std::string, std::int64_t>> arity;
};

// One read through a parameter: `base` type, the pointer stars, the byte offset.
struct StructAccess {
    std::string base;
    std::string stars;
    std::int64_t offset = 0;
};

// An older-style definition: its heading and its parameters as (name, type).
struct Definition {
    std::string heading;
    std::vector<std::pair<std::string, std::string>> parameters;
};

// The arguments of a call as written, and the index just after its `)`.
struct ArgumentList {
    std::vector<std::string> arguments;
    std::int64_t end = 0;
};

// The schema check: whether the document is L2, and if not why.
struct Validation {
    bool valid = false;
    std::string why;
};

// ---- the library ----------------------------------------------------------------------------------------------------

// l2_call and l2_free of cb/translate/l2/src/api.cc.
using CallFn = int (*)(const char* name, const char* arguments, std::size_t length, char** out, std::size_t* out_length);
using FreeFn = void (*)(char* result);

// A handle on the library; copies share it. Calls are serialised (the library keeps state between them).
class Library {
public:
    Library(CallFn call, FreeFn release);

    // The library linked into this program (cb_l2): its own l2_call and l2_free.
    static Library builtin();

    // One entry point, arguments already a JSON array and its answer as text: (status, text). Status 0 is success; any other
    // is an error whose text is the JSON {"type", "message", "facts"?}.
    std::pair<int, std::string> call_raw(std::string_view name, std::string_view arguments_json) const;
    // (CB_L2_RECORD: each call kept with its answer; native_l2.cpp)
    static void record(std::string_view fn, std::string_view arguments, int status, std::string_view answer);
    // One entry point with its arguments -> the answer. LibraryError (or its subtypes) for an error status; when `facts` is
    // given, the keys the library wrote to it (also when it fails) are put in it.
    Json call(std::string_view name, const Json& arguments, Json* facts = nullptr) const;
    Json answer_of(std::string_view name, const std::pair<int, std::string>& raw, Json* facts) const;   // (call_raw's answer, read as call reads it)

    // ---- the C back end (backends/c) ----

    // C text of one function's L2 document. `moved`: the passes of the module's C text done on the tree; `far`: the names of
    // the far pointers, when the decompiler's notes say; `facts`: what the function is written with, changed in place.
    std::string function(const Json& doc, std::span<const std::string> moved = {},
                         const std::optional<std::set<std::string>>& far = std::nullopt, Json* facts = nullptr) const;
    // The same with the facts already written as the library's JSON text (decompiler's facts_text): nothing is built or copied for them.
    // `left` (when given) receives what the library answered of them (the keys it wrote), not the facts as given.
    std::string function_with_facts_text(const Json& doc, std::span<const std::string> moved, std::string_view facts_text, Json* left) const;
    // A Module document: its functions as C, and what stayed assembly.
    ModuleText module(const Json& doc, std::span<const std::string> moved) const;
    // A MachineFragment as the C of its module holds it.
    std::string fragment(const Json& node) const;
    // A declaration of `name` of type `type` (an L2 type), or the type alone when `name` is empty. `far`: written __far.
    std::string type_text(const Json& type, std::string_view name = {}, bool far = false) const;
    std::string expr(const Json& expression) const;
    // A branch's condition, bracketed as the decompiler's printer does; `inner`: a loop's (comma separated).
    std::string condition(const Json& expression, bool inner = false) const;
    // The function's flags in the module, as its document and facts say.
    std::set<std::string> flags(const Json& doc, const Json& facts) const;
    // `int2 __cdecl16far sub_1_0000(void)`
    std::string signature(const Json& doc) const;

    // ---- the C++17 back end (backends/cpp17) ----

    // A Unit document of the Python front end as a C++17 module; `api_maps` is the folder of the Java API maps. Cpp17Declined for
    // what it does not write.
    Cpp17Unit cpp17_unit(const Json& unit, std::string_view api_maps) const;
    // A whole Java program (its Unit documents) as C++17 text; `main` names the main class. Cpp17Declined for what it does not write.
    std::string cpp17_program(std::span<const Json> units, const std::optional<std::string>& main) const;
    // The arguments a 32-bit definition with this heading takes; nullopt for a __cdecl one.
    std::optional<std::int64_t> takes32(std::string_view heading) const;
    ModuleNames module_names32(const Json& doc) const;
    // The decompiler's own type names a function's tree uses.
    std::set<std::string> type_names(const Json& doc) const;
    // The parameters' names as the function's heading writes them.
    std::vector<std::string> parameter_names(const Json& doc) const;
    // Every read through `param`.
    std::vector<StructAccess> struct_accesses(const Json& doc, std::string_view param) const;
    // {name: a declaration with its parameters}, in `order`. `headings` is {name: the heading of the function's writing}.
    OrderedMap<std::string, std::string> prototypes(const Json& headings, std::span<const std::string> order) const;
    // The module planners. Their arguments and answers are records of mixed shape (text, numbers, tuples, dicts of other
    // keys), passed as the library's JSON; `code_segments` are the numbers of the code segments, in order.
    Json segment_plan(const Json& functions, const Json& near, const Json& where, const Json& sites,
                      std::span<const std::int64_t> code_segments, const Json& data_segment, const Json& other_data,
                      const Json& first_selector) const;
    Json caller_frame_plan(const Json& reports, const Json& frames) const;
    // A selector's structure as a declaration's body, from {offset: type}; nullopt when the fields overlap.
    std::optional<std::vector<std::string>> seg_layout(const OrderedMap<std::int64_t, std::string>& fields) const;
    // The arguments of the call whose `(` is at `open_paren` in `text`.
    ArgumentList arguments_at(std::string_view text, std::int64_t open_paren) const;
    // The words a value of C type `type` takes on the stack; nullopt when it does not say.
    std::optional<std::int64_t> words(std::string_view type) const;
    // An older-style definition's heading and parameters; nullopt when `text` is none.
    std::optional<Definition> parameters(std::string_view text) const;
    // The words an argument takes on the stack, given the types of the names in it ({name: type}); nullopt when unknown.
    std::optional<std::int64_t> argument_words(std::string_view argument, const Json& types) const;

    // ---- the L2 core ----

    // The document the library's reader and writer make of `doc`, as text (what a round trip through the library gives).
    std::string roundtrip_text(const Json& doc) const;
    // Whether `doc` is L2, by the schema (l2.schema.json's text) -- the library is told the schema once.
    Validation validate(const Json& doc, std::string_view schema_text) const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

}  // namespace cb::translate::native_l2
