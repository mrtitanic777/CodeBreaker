// codebreaker/translate/dotnet/cs2cpp.hpp -- typed C++ port of codebreaker.translate.dotnet.cs2cpp.
//
// cs2cpp.py -- C# into the C++03 back end: the C# kit's L2 written by backends/cpp03.py.
//
// The C++03 back end (the Xbox 360 path) is L2-driven for bodies only. Three things about it are still
// Java's, and this module is what a C# source has to cross to reach it (docs/TRANSLATOR_DESIGN.md, wave 3:
// "checks the IR is not Java-shaped"):
//
//   1. its declarations come from Java source text: the transpiler (codebreaker/java) parses .java files
//      for classes, fields and signatures, and asks the back end only for bodies. So a C# program is given
//      a Java declaration skeleton, written here from its L2 (signatures, no bodies), and the transpiler's
//      front-end call is handed the C# kit's L2 units instead of javac's;
//   2. its runtime is Java's library in C++ (JavaString, JList, java.lang.Math, System.out): C#'s library
//      calls are rebound to the Java calls of the same meaning (Console.WriteLine -> System.out.println,
//      List<T> -> java.util.ArrayList, s.Length -> s.length(), ...), and C#'s text conversions are
//      written out where Java's differ (a bool is "True"/"False" in C#: a conditional);
//   3. it knows no properties: a C# property becomes a field and get/set methods, and every read and
//      write of it a call (C#'s own extension lowered into the core, as the design asks).
//
// What has no faithful form on that path is refused with the reason, never guessed: structs (value
// semantics), records, generics of the program's own, delegates and lambdas (C++03), unsigned integers
// (Java's runtime has none), nested classes (the transpiler's body lookup is by top-level class), using,
// lock, goto, checked arithmetic, doubles turned into text (C#'s format is not Java's), and any BCL call
// not in the table.
//
//     auto [cpp_dir, refused, stats] = cs2cpp::convert(units, work, transpiler);   // units from the Roslyn front end
//
// Typing notes (how the Python's values are typed here):
//   * The units are ir::Unit nodes (py::list<ir::NodeP>); ToJava rewrites them in place, as the Python does.
//   * Refuse is the Python's exception (kind, why). A refusal is the tuple (where, kind, why): where is None for
//     nothing the Python gives (a class's name is None until set), so it is an optional.
//   * The Python's props / auto dicts are keyed by the tuple (owner, name); a property's (getter, setter or None,
//     static) triple is the struct Accessors.
//   * The Python's convert() imports the Java transpiler and the C++03 back end and, for the length of the call,
//     REPLACES two of the back end's module attributes (`java_units`, to hand the transpiler the C# kit's L2 units
//     instead of running javac, and `Bodies`, to keep the instance the transpiler makes, for what it declined and
//     why). The typed transpiler (java/transpile.cpp) reads javac's trees itself and has no such hook, so the
//     transpiler call is an argument: a Transpiler is given (skeleton folder, output folder, the units) and does
//     what `transpile.convert(src, out, platform=None, write_report=False)` does with those two attributes
//     replaced; it answers the summary and, when it made a back-end instance, that instance's `reasons` and
//     `declined` counters (Transpiled::has_tree false: the Python's `seen` is empty).
//   * ir.walk is ir::walk; `__slots__`-less `self.loops` is the member `loops`.
//   * Python ints are int64_t.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/java/transpile.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::dotnet::cs2cpp {

namespace ir = codebreaker::translate::ir;

extern const py::dict<py::str, py::str> _PRIM_JAVA;
extern const py::dict<py::str, py::str> _BOXED;
extern const py::dict<py::str, py::str> _CLASSES;
extern const py::dict<py::str, py::str> _MATH;
extern const ir::TypeP STRING;

class Refuse : public py::Exception {
  public:
    py::str kind, why;
    Refuse(const py::str& kind, const py::str& why) : py::Exception(why), kind(kind), why(why) {}
    const char* type() const override { return "Refuse"; }
};

// (where, kind, why)
using Refusal = std::tuple<std::optional<py::str>, py::str, py::str>;

ir::SymP _m(const py::str& name, const std::optional<py::str>& owner, const py::list<ir::TypeP>& params, const ir::TypeP& ret,
            bool static_ = false);

// A property's accessors: (getter Sym, setter Sym or None, static)
struct Accessors {
    ir::SymP getter;
    ir::SymP setter;
    bool static_ = false;
};

// Rebind C# L2 to Java's library and lower C#'s properties into methods, in place.
class ToJava {
  public:
    using Key = std::tuple<std::optional<py::str>, py::str>;

    py::list<ir::NodeP> units;
    py::dict<std::optional<py::str>, py::ref<ir::ClassDecl>> classes;
    py::dict<Key, Accessors> props;     // (owner, name) -> (getter Sym, setter Sym or None, static)
    py::dict<Key, ir::SymP> auto_;      // (owner, name) -> an auto-property's field  (the Python's `auto`)
    py::list<Refusal> refused;
    int64_t loops = 0;

    explicit ToJava(const py::list<ir::NodeP>& units);

    // ---- types
    ir::TypeP ty(const ir::TypeP& t, bool boxed = false);
    ir::SymP sym(const ir::SymP& s);

    // ---- the program
    py::list<Refusal> run();
    void check_class(const ir::NodeP& c);
    void collect_props(const ir::NodeP& c);
    // A property's accessors, looked up through the classes it is inherited from.
    std::optional<Accessors> prop(std::optional<py::str> owner, const py::str& name);
    void klass(const ir::NodeP& c);
    py::list<py::str> mods(const std::optional<py::list<py::str>>& mods);
    py::list<ir::NodeP> member(const ir::NodeP& m, const ir::NodeP& c);
    py::list<ir::NodeP> lower_prop(const ir::NodeP& p, const ir::NodeP& c);
    ir::NodeP method(const ir::NodeP& m, const ir::NodeP& c);

    // ---- statements
    ir::NodeP block(const ir::NodeP& b);
    ir::NodeP stmt(const ir::NodeP& s);
    // foreach over an array or a List: a counted loop (the C++03 back end writes no for-each; decompiled
    // Java never has one). Over a List it is the same while the body does not change the list, where C#
    // would throw.
    ir::NodeP foreach_(const ir::NodeP& s);
    // switch on a string: the C++03 back end switches on integers only (decompiled Java switches on the
    // hash). As tests in order: each C# section ends in a jump, so none falls into the next.
    ir::NodeP string_switch(const ir::NodeP& s);
    ir::NodeP set_prop(const ir::NodeP& e);
    ir::NodeP set_index(const ir::NodeP& e);

    // ---- expressions
    ir::NodeP expr(const ir::NodeP& e);
    ir::NodeP _typed(const ir::NodeP& e);
    ir::NodeP call(const ir::NodeP& target, const ir::SymP& sym, const py::list<ir::NodeP>& args, const ir::TypeP& t);
    // p as C# turns it into text, written in terms Java's concatenation gives the same.
    ir::NodeP text(const ir::NodeP& p);

    ir::NodeP x_Lit(const ir::NodeP& e);
    ir::NodeP x_Paren(const ir::NodeP& e);
    ir::NodeP x_Name(const ir::NodeP& e);
    ir::NodeP x_This(const ir::NodeP& e);
    ir::NodeP x_Super(const ir::NodeP& e);
    ir::NodeP x_TypeName(const ir::NodeP& e);
    ir::NodeP x_Select(const ir::NodeP& e);
    ir::NodeP x_Length(const ir::NodeP& e);
    ir::NodeP x_Index(const ir::NodeP& e);
    ir::NodeP x_Unary(const ir::NodeP& e);
    ir::NodeP x_Binary(const ir::NodeP& e);
    ir::NodeP x_Assign(const ir::NodeP& e);
    ir::NodeP x_Cond(const ir::NodeP& e);
    ir::NodeP x_Cast(const ir::NodeP& e);
    ir::NodeP x_InstanceOf(const ir::NodeP& e);
    ir::NodeP x_New(const ir::NodeP& e);
    ir::NodeP x_NewArray(const ir::NodeP& e);
    ir::NodeP x_Concat(const ir::NodeP& e);
    ir::NodeP x_Call(const ir::NodeP& e);
    [[noreturn]] ir::NodeP x_Lambda(const ir::NodeP& e);
    [[noreturn]] ir::NodeP x_MethodRef(const ir::NodeP& e);
    [[noreturn]] ir::NodeP x_Unsupported(const ir::NodeP& e);
};

// ---------------------------------------------------------------------------------------------- Java text
py::str _jtype(const ir::TypeP& t);
py::str _default(const ir::TypeP& t);

// The Java declarations of a class (no bodies: the transpiler takes those from L2).
py::str skeleton(const ir::NodeP& c);

// ---------------------------------------------------------------------------------------------- the conversion
// What a Transpiler answers: the summary (the Python's stats dict) and, for the instance of the C++03 back end's Bodies
// it made, its `reasons` and `declined` counters.
struct Transpiled {
    java::transpile::Summary stats;
    bool has_tree = false;
    py::dict<py::str, int64_t> reasons;
    py::dict<py::str, int64_t> declined;
};
using Transpiler = std::function<Transpiled(const py::str& src, const py::str& out, const py::list<ir::NodeP>& units)>;

// C# L2 units -> the C++03 back end's program in work/cpp. -> (cpp_dir, [refusals], the transpiler's summary)
std::tuple<py::str, py::list<Refusal>, java::transpile::Summary> convert(const py::list<ir::NodeP>& units, const py::str& work,
                                                                        const Transpiler& transpiler);

// The C++03 runtime's System.out is a no-op (a console has no standard output); for the judge on the
// host, a PrintStream that writes, with Java's text for each kind of value.
bool _host_print(const py::str& cpp_dir);

}  // namespace codebreaker::translate::dotnet::cs2cpp
