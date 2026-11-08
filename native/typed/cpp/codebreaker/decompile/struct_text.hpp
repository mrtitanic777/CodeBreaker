// codebreaker/decompile/struct_text.hpp -- C++ port of codebreaker/decompile/struct_text.py.
//
// struct_text.py -- which field accesses are fields: the module's decision, for the C back end.
//
//     plan = struct_text::plan(module_path, docs, by_va, declared, analysis);
//     header += struct_text::declarations(plan, used);
//
// The Python module reaches three collaborators that are not part of it: the image loader and
// structure recovery (codebreaker.analyze.image / structures) and the C back end's
// parameter_names / struct_accesses over a function's L2 document. In C++ they are handed in as
// `Analysis`, and a function's document is named by the function (`docs` is the set of functions
// that have one); the callbacks look the document up.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::decompile::struct_text {

// How big (in bytes) each scalar type name is.
extern const py::dict<py::str, int64_t> SIZE_;

// One root of a recovered structure: ("arg", va, index) or ("this", va) (no index).
struct Root {
    py::str kind;
    int64_t va = 0;
    std::optional<int64_t> index;
};

// A structure analyze/structures.py recovered: its name and the places that are known to point at it.
struct Structure {
    py::str name;
    py::list<Root> roots;
};

// One access `*(base stars)(param + offset)` through a parameter (cback.struct_accesses).
struct Access {
    py::str base;
    py::str stars;
    int64_t offset = 0;
};

// (type as written, size)
using Field = std::tuple<py::str, int64_t>;
// {offset: (type, size)}
using Fields = py::dict<int64_t, Field>;
// (structure name, its fields)
using Planned = std::tuple<py::str, Fields>;
// {function: {parameter: (structure, {offset: (type, size)})}}
using Plan = py::dict<py::str, py::dict<py::str, Planned>>;

struct Analysis {
    // image.load(module_path) then structures.recover(...); may throw (the plan is then empty).
    std::function<py::list<Structure>(const py::str& module_path)> recover;
    // cback.parameter_names(docs[fn])
    std::function<py::list<py::str>(const py::str& fn)> parameter_names;
    // cback.struct_accesses(docs[fn], param)
    std::function<py::list<Access>(const py::str& fn, const py::str& param)> struct_accesses;
};

// (the field's type as written, its size) for an access `*(base stars)...`; (None, 0) when not a field.
std::tuple<std::optional<py::str>, int64_t> kind(const py::str& base, const py::str& stars);

// {function: {parameter: (structure, {offset: (type, size)})}} -- the accesses through each
// function's parameters that are a structure's fields, decided over the module: `docs` the
// functions that have an L2 document, `declared` {function: its heading as written}. On any trouble
// in the analysis, nothing: this is for reading, never for meaning.
Plan plan(const py::str& module_path, const py::set<py::str>& docs, const py::dict<int64_t, py::str>& by_va,
          const py::dict<py::str, py::str>& declared, const Analysis& analysis);

// The header's declarations of the structures `used` (the ones a function was written with a
// field of), each field at its offset, gaps as padding (a line's end after them, whether or not
// there are any: the header the module had is followed by it wherever a plan was made).
py::str declarations(const Plan& planned, const py::set<py::str>& used);

}  // namespace codebreaker::decompile::struct_text
