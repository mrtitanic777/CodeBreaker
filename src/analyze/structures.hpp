// analyze/structures.hpp -- structures recovered from how pointers are used.
//
//     const call_graph::Graph graph = call_graph::build(image);
//     for (const Structure& s : structures::recover(image, graph, conventions)) {
//         s.name, s.size, s.exact          // exact: the size is an allocation's, not a lower bound
//         s.fields                         // by offset: size, reads, writes, notes, ...
//         s.roots                          // the pointers that were unified into this structure
//     }
//
// A pointer is followed from where it comes into view:
//
//     an argument          `mov esi, [ebp+8]`             (16-bit: `mov bx, [bp+6]`, `les bx, [bp+6]`)
//     `this`               ecx on entry, when the function reads it before writing it
//     a global pointer     `mov eax, [g]`
//     an allocation        the result of LocalAlloc / GlobalAlloc / HeapAlloc / malloc / operator new with a constant
//                          size -- which gives the structure's exact size
//     a pointer field      `mov eax, [esi+10h]` and then eax is used as an address
//
// through register copies, `lea`, and the frame slots unoptimised code keeps it in. Every access `[pointer + offset]` is
// a field: its offset, its width, whether it is read or written, whether what it holds is itself used as an address (a
// pointer field), is indexed (an array), is loaded into the FPU (a float), or is given the address of a table of
// functions (a virtual table -- analyze/classes turns those into classes).
//
// Two pointers are the same structure when one is handed on as the other: a function passing its argument 0 as argument
// 1 of another, or as `this`. All such pointers are unified, so a structure gathers its fields from every function that
// touches it, not only the one a caller happens to look at. A structure is reported when it has two fields or more, or
// one field and a known size.
//
// How each function is called (frame or not, `this` in ecx) is the calling-convention pass's finding and is handed in as
// `ConventionFacts`.
#pragma once

#include <compare>
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "analyze/call_graph.hpp"
#include "analyze/convention_facts.hpp"
#include "analyze/native.hpp"

namespace cb::analyze::structures {

// Offsets of a structure reach no further than this.
inline constexpr Va kMaxOffset = 0x4000;
// A pointer handed over from more places than this is anybody's.
inline constexpr std::size_t kGeneric = 8;

// Where a followed pointer came into view.
struct Root {
    enum class Kind { Alloc, Arg, Field, Global, Result, Returns, This };   // the order roots are sorted in
    Kind kind = Kind::Arg;
    Va at = 0;               // Arg, This, Returns: the function; Global: the variable; Alloc, Result: the call site
    Va index = 0;            // Arg: which argument (from 0); Field: the field's offset
    std::shared_ptr<const Root> of;   // Field: the pointer whose field it is

    friend bool operator==(const Root& a, const Root& b);
    friend std::strong_ordering operator<=>(const Root& a, const Root& b);
};

enum class Note { AddressTaken, Array, Called, Float, Pointer };   // in the order their names sort
std::string_view name_of(Note note);                                // "address taken", "array", "called", "float", "pointer"

struct Field {
    Va size = 0;
    std::int64_t reads = 0;
    std::int64_t writes = 0;
    std::set<Note> notes;
    std::set<Va> stores;     // constants stored into it (a table's address ...)
    std::set<Va> users;      // functions that touch it
    Va element = 0;          // an array's element size
};

struct Structure {
    std::string name;                        // struct_1, struct_2 ... by field count, largest first
    std::vector<Root> roots;                 // sorted
    std::map<Va, Field> fields;              // by offset
    bool exact = false;                      // the size is an allocation's, not a lower bound
    Va size = 0;
    std::vector<std::string> passed_to;      // "<api> argument <n>": the system calls it is handed to
    std::vector<Va> functions;               // the functions that touch it, sorted
};

// Every structure the image's code shows, largest first.
std::vector<Structure> recover(Image& image, const call_graph::Graph& graph, const ConventionFacts& conventions);

// A C type for one recovered field: "unsigned long", "pointer", "function pointer", "float", "unsigned short[]" ...
std::string field_type(const Field& field);

// The structure as C text, gaps as padding.
std::string layout(const Structure& structure);

// How a root is described in words.
std::string describe(const Image& image, const Root& root);

// ---- the report ---------------------------------------------------------------------------------------------------

struct Counts {
    std::int64_t structures = 0;
    std::int64_t fields = 0;
    std::int64_t with_exact_size = 0;
    std::int64_t pointer_fields = 0;
    std::int64_t function_pointer_fields = 0;
    std::int64_t array_fields = 0;
    std::int64_t seen_through_this = 0;
    std::int64_t seen_through_global_pointer = 0;
    std::int64_t unified_across_functions = 0;
    std::int64_t largest_fields = 0;
    std::int64_t functions_touching_a_structure = 0;
};

struct FieldFact {
    Va offset = 0;
    Va size = 0;
    std::string type;
    std::int64_t reads = 0;
    std::int64_t writes = 0;
    std::vector<std::string> notes;          // sorted
};

struct StructureFact {
    std::string name;
    Va size = 0;
    bool exact_size = false;
    std::vector<FieldFact> fields;           // by offset
    std::vector<std::string> seen_as;        // the first twelve roots, in words
    std::int64_t pointers_unified = 0;
    std::vector<std::string> functions;      // labels of the first forty
    std::vector<std::string> passed_to;      // the first twelve
};

struct Report {
    Counts counts;
    std::vector<StructureFact> facts;
    std::vector<std::string> text;           // the three largest in a line each, then the largest as C
};

Report analyse(Image& image, const call_graph::Graph& graph, const ConventionFacts& conventions);

}  // namespace cb::analyze::structures
