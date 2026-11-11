// codebreaker/symbols/sdk_structs.hpp -- compat shim for cb/symbols/sdk_structs.hpp (docs/CPP_DESIGN.md 6.3). The old types
// in, the new reader called, the old types out; the old knowledge base (platform win32, kinds "struct" and
// "struct.pointer") still keeps what is read. Deleted with its last old-layer consumer.
//
//     auto s = sdk_structs::layout("RECT");          Struct(name, size, fields) or None
//     s->at(8, 4)                                     "right": the field that starts at 8 and is 4 bytes
//     sdk_structs::pointed_to("LPCRECT")              "RECT": the structure a pointer type points at
//     sdk_structs::definition(*s)                     C text defining it, field for field, at the SDK's offsets
//
// The stamp of the kinds in the knowledge base is a hash of prototypes::_stamp and the reader's revision
// (cb::symbols::sdk_structs::kReaderRevision), so a base made by an earlier reader is made again. The base keeps a
// structure as the old layer's pickle of (size, [(offset, size, name, ctype), ...], biggest) (facts.cpp writes and
// reads it).
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
    // Per field: its c type is the old layer's constant "uint4" of a pointer field (one str object in every such field,
    // which a pickle of the layout keeps once); empty for a layout read back from the base. See facts.hpp.
    py::list<bool> pointer;
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

// The structures as the knowledge base keeps them (a structure the reading could not lay out is None) and the pointer
// types ({pointer type: structure}).
using Structs = py::dict<py::str, std::optional<Layout>>;
using Pointers = py::dict<py::str, py::str>;

// (structures, pointers) as the knowledge base keeps them (platform win32, kinds "struct" and "struct.pointer"): read
// from the headers again when a header folder or the reader changes.
std::tuple<Structs, Pointers> _read();

// Read from the headers (always, without the knowledge base).
std::tuple<Structs, Pointers> _from_headers();

std::optional<Struct> layout(const py::str& name);

// The structure a parameter of this type points at, or None ("LPRECT", "CONST RECT *").
std::optional<py::str> pointed_to(const py::str& type_text);

// C text defining structure `s` (and the structures in it, before it), laid out field for field at the SDK's offsets.
// `known`: structures already defined.
py::str definition(const Struct& s, const py::set<py::str>& known = py::set<py::str>());

}  // namespace codebreaker::symbols::sdk_structs
