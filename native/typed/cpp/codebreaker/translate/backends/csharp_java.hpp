// codebreaker/translate/backends/csharp_java.hpp -- typed C++ port of codebreaker.translate.backends.csharp_java.
//
// csharp_java.py -- the C# back end's knowledge of Java: Java's library and Java's semantics, in C#.
//
// A Java unit (Unit.lang 'java', from the javac front end) is written by backends/csharp.py's Writer with
// this as its library. It maps Java's types onto .NET's (java.lang.String -> string, java.util.List ->
// List<T>, java.util.function.Function -> Func<T, R>, ...), Java's library calls onto .NET's through the
// API-mapping registry (codebreaker/java/api_maps/java.csharp.json: a template per call; JavaRt, the
// small Java runtime in C#, where .NET's call means something else), and Java's semantics where C#'s
// differ:
//
//   * dispatch: a Java instance method is virtual unless final, private or static -- `virtual`, and
//     `override` where it overrides a class of these units or Object (toString -> ToString, equals ->
//     Equals, hashCode -> GetHashCode); `main(String[])` is C#'s `Main`;
//   * construction order: Java runs instance field initialisers and instance blocks after the base
//     constructor, C# before it; where the base is a class of these units (whose constructor could see
//     them) they are moved into each constructor after its base call. Static initialisers and static
//     blocks keep Java's one textual order, in a static constructor;
//   * text: every value Java turns into text differently (doubles, floats, booleans, null, collections)
//     goes through JavaRt.Str;
//   * HashMap and HashSet iterate in hash order, Dictionary and HashSet<T> in another: iterating one is
//     declined (TreeMap/TreeSet map to the sorted collections, whose order is Java's);
//   * Java's boxed types map to .NET's values (Integer -> int): a boxed value that is null is declined.
//
// What it cannot carry it declines (csharp::Decline), with the reason.
//
// Typing notes (see also csharp.hpp):
//   * JavaLib is in namespace csharp (csharp.hpp forward-declares it for the Writer); prepare() is in
//     namespace csharp_java, as the Python module's function is.
//   * The methods that answer "a value or None" return std::optional: an expression's (text, precedence)
//     pair is csharp::Ex, a text is a py::str.
//   * The tables _TYPES, _GENERIC, _ARITY, _ORDERED, _FUNCTIONS, _FAMILY, _FIELDS, _OBJECT_METHODS and
//     _PARAM_KIND stay in the .cpp. _FUNCTIONS' lambdas (how an interface's type arguments become the
//     delegate's) are a mode per interface.
//   * `_lookup` answers the registry's mapping and the row's csharp entry (apimap::MapEntry); a missing
//     row is a default MapEntry (the Python's {}).
//   * `registry()` is the cached apimap::load(); the folder of the api_maps is java/_paths' (the host sets
//     it, as for the apimap port).
//   * Python ints are int64_t.
#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <utility>

#include "codebreaker/java/apimap.hpp"
#include "codebreaker/translate/backends/csharp.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::csharp {

class JavaLib {
  public:
    using ClassP = py::ref<ir::ClassDecl>;
    using MethodP = py::ref<ir::MethodDecl>;

    Writer& w;
    Program& prog;

    explicit JavaLib(Writer& w);

    // ---- types
    std::optional<py::str> type_name(const TypeP& t);
    bool keep_interface(const TypeP& t);
    py::str exception(const TypeP& t);

    // ---- declarations
    void check_class(const ClassP& c);
    py::list<py::str> class_mods(const ClassP& c);
    py::list<py::str> field_mods(const py::ref<ir::FieldDecl>& f, const ClassP& c);
    py::list<py::str> method_mods(const MethodP& m, const ClassP& c);
    bool overrides(const ClassP& c, const MethodP& m);
    py::str method_name(const MethodP& m, const ClassP& c);
    py::str member_name(const SymP& s);
    // Java's initialisation order: instance initialisers after the base constructor (moved into the
    // constructors, where the base is one of these units' classes), statics in one textual order.
    py::list<NodeP> arrange_members(const ClassP& c, const py::list<NodeP>& members);
    bool ctor_needed(const ClassP& c, const MethodP& m);
    py::list<NodeP> ctor_prologue(const ClassP& c, const MethodP& m, const py::ref<ir::CtorCall>& call);

    // ---- expressions
    std::optional<Ex> select(const py::ref<ir::Select>& e);
    std::optional<Ex> call(const py::ref<ir::Call>& e, bool statement = false);
    std::optional<py::str> stmt_call(const py::ref<ir::Call>& e);
    py::str fill(const py::str& tmpl, const py::ref<ir::Call>& e);
    // a as Java turns it into text (String.valueOf).
    py::str string_of(const NodeP& a);
    std::optional<py::str> concat_part(const NodeP& p);
    std::optional<Ex> new_(const py::ref<ir::New>& e);
    std::optional<Ex> binary(const py::ref<ir::Binary>& e);
    std::optional<py::str> coerce(const NodeP& e, const TypeP& t);
    // what a name holds: the class of the object made where it is declared (a Map that is a TreeMap)
    void record(const SymP& sym, const NodeP& init);
    std::optional<py::str> concrete(const NodeP& e);
    std::optional<py::str> iterable(const NodeP& e);
    void check_lambda(const py::ref<ir::Lambda>& e);

  private:
    bool access_const_(const py::ref<ir::FieldDecl>& f) const;
    py::list<py::str> access_(const py::list<py::str>& mods, const ClassP& c) const;
    bool closeable_(const ClassP& c) const;
};

}  // namespace codebreaker::translate::backends::csharp

namespace codebreaker::translate::backends::csharp_java {

// Before writing: an abstract Java class that leaves an interface's method to its subclasses declares
// it abstract in C# (C# requires the class to say so).
void prepare(csharp::Program& prog);

}  // namespace codebreaker::translate::backends::csharp_java
