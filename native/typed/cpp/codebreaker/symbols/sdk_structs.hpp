// codebreaker/symbols/sdk_structs.hpp -- typed C++ port of codebreaker.symbols.sdk_structs.
//
// sdk_structs.py -- the layouts of the SDK's structures, read from its headers.
//
//     auto s = sdk_structs::layout("RECT");       // Struct(name, size, fields) or None
//     s->at(8, 4);                                // "right": the field that starts at 8 and is 4 bytes
//     sdk_structs::pointed_to("LPCRECT");         // "RECT": the structure a pointer type points at
//     sdk_structs::definition(*s);                // C text defining it, field for field, at the SDK's offsets
//
// A structure is read from its `typedef struct tagX { ... } X, *PX, FAR *LPX;` in the 32-bit headers
// (symbols/prototypes finds them), once: the layouts are kept in the knowledge base. Offsets follow the
// headers' packing (#include <pshpackN.h> ... <poppack.h>, natural alignment otherwise). A field of a type
// this does not know the size of makes the whole structure unknown -- a wrong offset would name the wrong
// field. The definition written out lays the fields at those offsets explicitly (packed to 1, with the gaps
// as padding), so it means the same to any compiler.
//
// Notes on the port:
//   * The stamp of the kinds in the knowledge base is the Python's hash of prototypes._stamp plus the source
//     text of two Python functions (_from_headers, readable._constants); a C++ program has no source text of
//     functions, so those are a revision number here (kReading). A base made by one implementation is thus
//     made again by the other.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::symbols::sdk_structs {

// (offset, size, name, c type)
using Field = std::tuple<int64_t, int64_t, py::str, py::str>;

// What the knowledge base keeps of a structure: (size, fields, biggest alignment).
struct Layout {
    int64_t size = 0;
    py::list<Field> fields;
    int64_t biggest = 1;
};

class Struct {
  public:
    Struct(const py::str& name, int64_t size, const py::list<Field>& fields) : name(name), size(size), fields(fields) {}
    py::str name;
    int64_t size;
    py::list<Field> fields;                  // [(offset, size, name, c type)]

    // (field path, its C type) for the field starting at `offset` that is `size` bytes, or None.
    std::optional<std::tuple<py::str, py::str>> typed(int64_t offset, int64_t size) const;

    // The field (a dotted path into a nested structure) starting at `offset` that is `size` bytes, or None.
    std::optional<py::str> at(int64_t offset, int64_t size) const;
};

// The structures as the knowledge base keeps them (a structure the reading could not lay out is None) and
// the pointer types ({pointer type: structure}).
using Structs = py::dict<py::str, std::optional<Layout>>;
using Pointers = py::dict<py::str, py::str>;

// (structures, pointers) as the knowledge base keeps them (platform win32, kinds "struct" and
// "struct.pointer"): read from the headers again when a header folder or this reading changes.
std::tuple<Structs, Pointers> _read();

// Read from the headers (always, without the knowledge base).
std::tuple<Structs, Pointers> _from_headers();

std::optional<Struct> layout(const py::str& name);

// The structure a parameter of this type points at, or None ("LPRECT", "CONST RECT *").
std::optional<py::str> pointed_to(const py::str& type_text);

// C text defining structure `s` (and the structures in it, before it), laid out field for field at the SDK's
// offsets. `known`: structures already defined.
py::str definition(const Struct& s, const py::set<py::str>& known = py::set<py::str>());

}  // namespace codebreaker::symbols::sdk_structs
