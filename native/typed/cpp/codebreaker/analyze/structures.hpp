// codebreaker/analyze/structures.hpp -- interface of codebreaker.analyze.structures (typed C++; implementation pending).
//
// structures.py -- structures recovered from how pointers are used.
//
//     auto found = structures::recover(*image::load("comctl32.dll"));
//     for s in *found: s.name, s.size, s.exact       // exact: the size is an allocation's, not a lower bound
//                      s.fields                      // {offset: Field{size, reads, writes, notes, ...}}
//                      s.roots                       // the pointers that were unified into this structure
//
// A pointer is followed from where it comes into view (an argument, `this` in ecx, a global pointer, an allocation's
// result with a constant size, a pointer field), through register copies, `lea` and the frame slots unoptimised code
// keeps it in. Every access `[pointer + offset]` is a field: its offset, width, read or written, whether what it holds
// is itself an address (a pointer field), is indexed (an array), is loaded into the FPU (a float), or is given the
// address of a table of functions (a virtual table -- classes.py turns those into classes). Two pointers are the same
// structure when one is handed on as the other; all such pointers are unified.
//
// Typing notes:
//   * A root (where a followed pointer came from) is a recursive tuple in the Python: ("arg", fn, index),
//     ("this", fn), ("global", va), ("alloc", site), ("returns", fn), ("result", site) or ("field", root, offset).
//     `Root` holds them: `kind`; `addr` = fn / va / site; `index` = the argument number (arg) or the field offset
//     (field); `inner` = the root a "field" is of. Root has ==, < (the Python orders roots by repr) and std::hash.
//     (decompile/struct_text.hpp has a smaller Root of its own for the ("arg"|"this", va, index) subset it reads; the
//     two are bridged where recover() is wired to it.)
//   * A field is the dict {size, reads, writes, notes, stores, users, element}; `element` is only set for arrays
//     (0 otherwise, as f.get("element", 0)). The structure is the dict {roots, fields, exact, size, passed_to,
//     functions, name}.
//   * analyse(): the one kind of fact is "structure" (StructureFact); counts is a dict of ints.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::structures {
struct Root;
}
namespace std {
template <> struct hash<codebreaker::analyze::structures::Root>;      // (defined below; roots key dicts and sets)
}

namespace codebreaker::analyze::structures {

// Offsets of a structure reach no further than this.
inline constexpr int64_t MAX_OFFSET = 0x4000;
// A pointer handed over from more places than this is anybody's.
inline constexpr int64_t GENERIC = 8;

// Where a followed pointer came from (see the typing notes).
struct Root {
    py::str kind;                             // "arg" | "this" | "global" | "alloc" | "returns" | "result" | "field"
    int64_t addr = 0;                         // the function ("arg", "this", "returns"), the variable ("global") or the call site
    int64_t index = 0;                        // "arg": argument number; "field": the field's offset
    std::shared_ptr<Root> inner;              // "field": the pointer whose field it is
    bool operator==(const Root& o) const;
    bool operator!=(const Root& o) const { return !(*this == o); }
    bool operator<(const Root& o) const;      // repr order
};

// One field of a structure.
struct Field {
    int64_t size = 0;
    int64_t reads = 0;
    int64_t writes = 0;
    py::set<py::str> notes;                   // "pointer", "address taken", "array", "float", "called"
    py::set<int64_t> stores;                  // constants stored into it (a table's address ...)
    py::set<int64_t> users;                   // functions that touch it
    int64_t element = 0;                      // an array's element size
};

// A structure the code shows.
struct Structure {
    py::list<Root> roots;
    py::dict<int64_t, Field> fields;          // offset -> field
    bool exact = false;                       // the size is an allocation's, not a lower bound
    int64_t size = 0;
    py::list<py::str> passed_to;              // "<api> argument <n>"
    py::list<int64_t> functions;              // the functions that touch it, sorted
    py::str name;                             // struct_<n>, by size of the field count
};

// Used by: analyze.observe, decompile.struct_text, source.classes_writer (via classes), tests.
std::shared_ptr<py::list<Structure>> _recover(image::Image& img);

// Every structure the image's code shows, largest first (memo "structures").
std::shared_ptr<py::list<Structure>> recover(image::Image& img);

// A C type for one recovered field ("unsigned long", "pointer", "function pointer", "float", "unsigned short[]" ...).
py::str field_type(const Field& f, int64_t word = 4);

// The structure as C text, gaps as padding.
py::str layout(const image::Image& img, const Structure& s);

// How a root is described in words.
py::str _describe(const image::Image& img, const Root& root);

// ---- analyse() ----
struct FieldFact {                            // one field of a StructureFact
    int64_t size = 0;
    py::str type;
    int64_t reads = 0;
    int64_t writes = 0;
    py::list<py::str> notes;                  // sorted
};
struct StructureFact {                        // kind "structure"; addr is None
    py::str kind = py::str("structure");
    py::str name;
    int64_t size = 0;
    bool exact_size = false;
    py::dict<py::str, FieldFact> fields;      // "+8" -> field
    py::list<py::str> seen_as;
    int64_t pointers_unified = 0;
    py::list<py::str> functions;
    py::list<py::str> passed_to;
};
struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<StructureFact> facts;
    py::list<py::str> text;
};
Analysis analyse(image::Image& img);

}  // namespace codebreaker::analyze::structures

namespace std {
template <> struct hash<codebreaker::analyze::structures::Root> {
    size_t operator()(const codebreaker::analyze::structures::Root& r) const;
};
}  // namespace std
