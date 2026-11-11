// codebreaker/exact/data.hpp -- typed C++ of codebreaker.exact.data
//
// data.py -- a module's initialized data written as C: where it stands, and definitions proposed from the bytes.
//
//     python -m codebreaker exact PROJECT data MODULE [--draft] [--write]
//
// The module's functions are compiled alone (no data files): their literals, each with the function
// that names it first, are lined up with the program's data segment from the module's data start
// (src/MODULES). What lies between them is the module's own: its variables, and the strings their
// values point at. Which declarations those are, and where they stood in the source, the toolchain
// profile's "data" rule says:
//
//     "order": "after_next_function"   (Microsoft C 3.00) a file's _DATA follows the source: an
//         initializer's strings where the declaration stands, a function's literals with the function,
//         and a variable's bytes after the literals of the next function after its declaration - so a
//         run after F's literals is variables declared before the function after F (src/dNEXT.c), and
//         the strings before such a run are their values; in a file without functions, all literals
//         come first, then the variables
//
// The definitions are typed by the functions' extern declarations (int where none says), their
// values by the program's bytes: a word that points at one of the module's strings is that string, at a
// known place its name. What the bytes cannot tell is marked /* ? */.
//
//     data::walk(project, row) -> Walked{runs [(lo, hi, after function, kind)], strings, dstart, dend, functions}
//     data::draft(project, row) -> {file name: text}
//
// Typing notes (the Python's duck-typed objects):
//   * `prj` is data::Project: exact.modules' Project (the members modules.py reads, which walk/draft use too: src,
//     path(), switches, toolchain(), headers(), include, image, symbols(), dgroup) and `data_order()`, the
//     toolchain profile's `p.get("data", {}).get("order", "after_next_function")` (a dict-valued entry of the
//     profile has no str type in the modules::Toolchain interface).  Its Toolchain is modules::Toolchain.
//   * The module's globals CODE, FUNCS and HOME are the dicts CODE(), FUNCS(), HOME(): draft() and parse_structs()
//     refill them, _definition() reads them, as the Python functions do.
//   * A set of (type, declarator) pairs (what externs() keeps for a name) is a py::set; where the Python walks one
//     with `for t, i in sorted(ty)` to take the first that qualifies (_declared_size) the pairs are taken in sorted
//     order.
//   * A module whose MODULES row has no data start (dstart None): the Python's walk raises TypeError when another
//     row has one and goes on with None otherwise (and every caller then fails at a %04x or a comparison with
//     None); here walk raises TypeError for both.
//   * walk's dstart, dend are ints; a recursion through structs that contain themselves is Python's RecursionError
//     (a local exception of that name, raised past 480 levels).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/exact/modules.hpp"
#include "codebreaker/formats/omf.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::data {

// Python's RecursionError (a RuntimeError): the runtime's
using RecursionError = py::RecursionError;

// exact.project.Project, as data.py reads it
class Project : public modules::Project {
  public:
    // prj.toolchain().p.get("data", {}).get("order", "after_next_function")
    virtual py::str data_order() const = 0;
};

extern const py::str BS;                            // chr(92)

// the bytes as a C string literal
py::str cstr(const py::bytes& b);

using Obj = codebreaker::formats::omf::Obj;
// -> (the functions' object, the addresses of the files' functions), or None when the module has no function files
std::optional<std::tuple<Obj, py::list<int64_t>>> _functions_only(const modules::Project& prj, const modules::Row& row);

using Run = std::tuple<int64_t, int64_t, py::str, py::str>;                           // (lo, hi, after function, "strings" | "vars" | "?")
using Lit = std::tuple<int64_t, py::bytes, std::optional<py::str>>;                   // (place in the literals, bytes + NUL, function)

// walk()'s answer
struct Walked {
    py::list<Run> runs;
    py::list<Lit> strings;
    int64_t dstart = 0;
    int64_t dend = 0;
    py::list<std::tuple<py::str, int64_t>> functions;       // (name, address) of the compiled functions
};

Walked walk(const modules::Project& prj, const modules::Row& row);

// the places of the literals, in order, in ds[lo:hi]: each at an occurrence after the one before such that all
// the rest can still be found (an initializer's string can equal a function's literal and stand before it)
py::list<std::optional<int64_t>> _align(const py::bytes& ds, const py::list<py::bytes>& lits, int64_t lo, int64_t hi);

using Declarations = py::dict<py::str, py::set<std::tuple<py::str, py::str>>>;      // {name: {(type, declarator)}}

// {name: {(type, declarator)}} - how the functions declare the data they name (files: only those)
Declarations externs(const py::str& src, const std::optional<py::list<py::str>>& files = std::nullopt);

// {file name: text} - the module's data files proposed (the profile's rule says where each run goes)
py::dict<py::str, py::str> draft(const Project& prj, const modules::Row& row);

py::dict<int64_t, py::str>& CODE();                 // code address -> function name (tables of functions)
py::dict<py::str, py::str>& FUNCS();                // the functions a draft's tables name: their type

// {code address & 0xFFFF: name}
py::dict<int64_t, py::str> _code_names(const modules::Project& prj);

using Field = std::tuple<py::str, int64_t, py::list<int64_t>, py::str>;              // (base type, stars, [dimensions], field name)
using Structs = py::dict<py::str, py::list<Field>>;

// the bytes a declaration takes, when it says (a scalar, a pointer, an array of a known count, a struct)
std::optional<int64_t> _declared_size(const py::set<std::tuple<py::str, py::str>>& ty, const Structs& structs);

extern const py::dict<py::str, int64_t> SIZES;

// {struct name: [(base type, stars, [dimensions], field name)]} of the project's headers
Structs parse_structs(const modules::Project& prj);

py::dict<py::str, py::str>& HOME();                 // struct name -> the header that defines it

// -> (size, alignment)
std::tuple<int64_t, int64_t> _sizeof(const py::str& base, int64_t stars, const py::list<int64_t>& dims, const Structs& structs, int64_t align);
std::tuple<int64_t, int64_t> _struct_size(const py::str& name, const Structs& structs, int64_t align);

// the definition (C text) of the variable at `a` whose bytes run to `nx`
py::str _definition(const py::bytes& ds, int64_t a, int64_t nx, const py::dict<int64_t, py::str>& syms, const Declarations& decl,
                    const py::dict<int64_t, py::bytes>& strs, const std::optional<Structs>& structs = std::nullopt, int64_t align = 2,
                    const std::optional<py::set<py::str>>& local = std::nullopt);

// val(v): the text a word that points at a known place stands for, or None
using ValFn = std::function<std::optional<py::str>(int64_t)>;
py::str _struct_value(const py::bytes& ds, int64_t at, const py::str& name, const Structs& structs, int64_t align, const ValFn& val);

// `python -m codebreaker exact PROJECT data ...`: prints with py::print; the exit status
int64_t main(const Project& prj, const py::list<py::str>& argv);

}  // namespace codebreaker::exact::data
