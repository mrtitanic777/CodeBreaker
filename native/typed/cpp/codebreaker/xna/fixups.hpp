// codebreaker/xna/fixups.hpp -- C++ port of codebreaker.xna.fixups.
//
// fixups.py -- what ILSpy writes for an XNA game that is not valid C#, rewritten as the C# it stands for.
//
//     from codebreaker.xna import fixups
//     text, n = fixups.ref_casts(text)        ((Color)(ref white)).A = 64   ->   (white).A = 64
//     text, n, left = fixups.constructors(text)
//                                             base._002Ector(a);  ->  `: base(a)` on the constructor;
//                                             v._002Ector(a);     ->  v = new T(a);   (T from v's declaration)
//     text, n = fixups.data_holders(text)     a compiler's static-data holder (<PrivateImplementationDetails>,
//                                             renamed by an obfuscator): its private blob structs internal
//     fixups.apply(folder)                    all of them, over every .cs file: {"ref casts", "constructors",
//                                             "data holders", "left": [(file, line, why)]}
//
// Each rewrite gives the same program: a cast of a value to its own type is no cast (the `ref` in it
// is how the decompiler marks a mutable struct's member being reached in place), and a base or this
// constructor call at the top of a constructor is what an initializer is (it runs before the body
// either way). What a rewrite cannot place with certainty is left as it is and listed.
//
// These came from the Arcadecraft decompile (../Arcadecraft/tools/fix_*.py), where they made 65 000
// lines of decompiled C# compile again.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::xna::fixups {

extern const py::str _LVALUE;
extern const py::re::Pattern _REF_CAST;
extern const py::re::Pattern _CTOR;

// ((Color)(ref white)).A = 64   ->   (white).A = 64      -> (text, rewrites)
std::tuple<py::str, int64_t> ref_casts(const py::str& text);

// The text inside the parentheses that open at open_paren, and the position after the closing one;
// (None, -1) when they never close.
std::tuple<std::optional<py::str>, int64_t> _matching(const py::str& text, int64_t open_paren);

// The position of the `{` that encloses pos, or -1.
int64_t _enclosing_brace(const py::str& text, int64_t pos);

// The type of the nearest declaration of `var` before `before` (the decompiler writes one per line).
std::optional<py::str> _declared_type(const py::str& text, const py::str& var, int64_t before);

// -> (text, rewrites, left as (line, why))
std::tuple<py::str, int64_t, py::list<std::tuple<int64_t, py::str>>> constructors(const py::str& text);

// The compiler's holder of static array data (<PrivateImplementationDetails>; an obfuscator renames it):
// ILSpy writes its fields as `internal static StructN X /* Not supported: data(...) */` over private blob
// structs, which C# refuses (CS0052), and turns the code that used the data into array literals. The blob
// structs become internal; nothing reads them. -> (text, structs changed)
std::tuple<py::str, int64_t> data_holders(const py::str& text);

// apply()'s report: the Python dict {"ref casts", "constructors", "data holders", "left"}.
struct Report {
    int64_t ref_casts = 0;
    int64_t constructors = 0;
    int64_t data_holders = 0;
    py::list<std::tuple<py::str, int64_t, py::str>> left;   // (file relative to the folder, line, why)
};

// All the rewrites, over every .cs file under the folder (changed files are written back).
Report apply(const py::str& folder);

}  // namespace codebreaker::xna::fixups
