// codebreaker/translate/frontends/clang/j2x.hpp -- typed C++ port of
// codebreaker.translate.frontends.clang.j2x.
//
// j2x.py -- C++ written by the Java lane (codebreaker/java: the C++03 back end and its runtime,
// platform/canonical) read back as the Java it came from: the library half of the Java -> C++ -> Java round
// trip (docs/TRANSLATOR_DESIGN.md, phase 5 exit check).
//
// The clang front end lowers that C++ like any other; what it cannot know is that `JavaString`, `JArray<T>`,
// `JCat() + "x" + n`, `System::out.println(...)` and `j2xToInt(d)` are the Java runtime written in C++. This
// pass maps them back onto Java's own library in the IR -- the shapes the javac front end gives
// (java.lang.String, arrays, Concat, java.io.PrintStream calls, numeric casts) -- so the Java back end
// writes Java again:
//
//     JavaString / JCat / std::string     -> java.lang.String          JArray<T>            -> T[]
//     JavaString(x)._s, JavaString(x)     -> x                         JCat() + a + b       -> Concat(a, b)
//     arr[i] (JArray::operator[])         -> Index                     arr.length / ->length -> Length
//     System::out / System::err           -> System.out / System.err   println/print        -> PrintStream's
//     j2xToInt / j2xToLong (saturating)   -> Java's (int) / (long)     javaMathSqrt & co    -> Math.sqrt ...
//     j2xUshr(v, n)                       -> v >>> n                   s_name (a static)     -> name
//     initStatics()                       -> the class's static initialiser
// and the sequencing temporaries the C++ back end adds to keep Java's left-to-right order
// (`int j2xo1 = 0; ... f((j2xo1 = a, j2xo2 = b, ...))`) are folded back into their declarations.
// What it does not know stays as it is, and the back end refuses it with the name.
//
// Typing notes:
//   * The Python's private names keep their meaning with a trailing underscore instead of a leading one:
//     _strip -> strip_, _value -> value_, _commas -> commas_, _first_comma -> first_comma_; the tables
//     _STRINGS and _MATH stay in the .cpp.
//   * Lift.node(n) takes "anything" in the Python: a node (returned, possibly replaced) or a list (a new
//     list of nodes). Here those are the two overloads node(NodeP) and node(list). Anything else the
//     Python lets through (a str, a Type, None) is not lifted by it and has no overload; None is a null
//     NodeP. Lift.node walks the slots of a node by their C++ types: a node, a list of nodes or a node
//     or list (Index.index) is lifted, a Type slot is lift_type'd, a Sym slot is sym()'d, a Lit's value
//     that is a Type is lift_type'd; lists of Types, strings and dicts are left alone, as in the Python
//     (where lifting a list lifts only its nodes).
//   * Python's `x.type` on a node that has no `type` slot (a statement) or on None raises AttributeError;
//     so does the C++.
//   * _first_comma returns (node, setter) or None: here an optional tuple of the comma node and a
//     std::function that stores a node in its place (empty for the Python's setter None, whose call is a
//     TypeError).
//   * Lift.renamed is a std::map keyed by (owner or None, name).
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"

namespace codebreaker::translate::frontends::clang::j2x {

namespace ir = codebreaker::translate::ir;

extern const ir::TypeP STRING;
extern const ir::TypeP SYSTEM;
extern const ir::TypeP PRINTSTREAM;
extern const ir::TypeP MATH;

ir::TypeP lift_type(const ir::TypeP& t);

ir::NodeP strip_(const ir::NodeP& e);

class Lift {
  public:
    Lift();

    // (owner, s_name) -> name
    std::map<std::tuple<std::optional<py::str>, py::str>, py::str> renamed;

    // A static field s_x is x again (the C++ back end's prefix), where no field x exists.
    void names(const py::ref<ir::Unit>& u);

    py::ref<ir::Unit> unit(const py::ref<ir::Unit>& u);

    ir::SymP sym(const ir::SymP& s);

    ir::NodeP node(const ir::NodeP& n);
    py::list<ir::NodeP> node(const py::list<ir::NodeP>& n);

    // ---- expressions (children already lifted)
    ir::NodeP x_Lit(const py::ref<ir::Lit>& e);
    ir::NodeP x_Cast(const py::ref<ir::Cast>& e);
    ir::NodeP x_Select(const py::ref<ir::Select>& e);
    ir::NodeP x_Name(const py::ref<ir::Name>& e);
    ir::NodeP x_Unary(const py::ref<ir::Unary>& e);
    ir::NodeP x_New(const py::ref<ir::New>& e);
    ir::NodeP x_Call(const py::ref<ir::Call>& e);
    ir::NodeP x_Binary(const py::ref<ir::Binary>& e);

    // ---- the sequencing temporaries
    // `int j2xo1 = 0; ...; S((j2xo1 = a, j2xo2 = b, X))` -> `int j2xo1 = a; ...; S(X)`.
    py::list<ir::NodeP> fold_temps(const py::list<ir::NodeP>& stmts);
};

// An argument bound to a reference parameter (Unary '&' typed ref): the value it binds.
ir::NodeP value_(const ir::NodeP& a);

py::list<ir::NodeP> commas_(const ir::NodeP& e);

using Setter = std::function<void(const ir::NodeP&)>;

// The first comma expression in a statement, with a function that puts something in its place.
std::optional<std::tuple<ir::NodeP, Setter>> first_comma_(const ir::NodeP& stmt);

// The Java lane's C++ (its units) read back as Java's library, in place.
py::list<py::ref<ir::Unit>> lift(const py::list<py::ref<ir::Unit>>& units);

}  // namespace codebreaker::translate::frontends::clang::j2x
