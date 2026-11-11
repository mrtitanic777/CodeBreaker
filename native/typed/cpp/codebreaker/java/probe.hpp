// codebreaker/java/probe.hpp -- typed C++ port of codebreaker.java.probe.
//
// probe.py -- run a Java class and its C++ translation on the same inputs and compare what they do.
//
//     auto result = probe::run(java_src_dir, cpp_dir, cls, work, cxx, javac, java);
//
// The behavioural half of the java kind's judge (verify/lanes/java.py). For a class, every method it can call
// is called on a fixed, deterministic set of inputs, in the JVM and natively, and what comes back compared:
//
//   * what it can pass: primitives, Strings, and objects of classes it can make -- a class is made with its
//     first non-private constructor, else a static factory of the class returning it, whose parameters are
//     primitives or Strings (or the implicit constructor). The maker's arguments come from the same input set.
//   * what it compares: a primitive or String result; an object result by its fields (the non-private,
//     non-static primitive and String fields of the declared class and its ancestors); and for a void
//     instance method, the object's fields after the call. Integers by value, floats and doubles by their
//     bits (any NaN equals any NaN), Strings by their bytes. A null String in Java is the empty string here
//     -- the port's model of a String is a value -- and they compare alike.
//   * the original: the class's Java source compiled by javac (-sourcepath: it pulls in only what the
//     harness needs) and run on the JVM -- twice: a method any of whose results changes between the two runs
//     (a Random seeded from the clock, the time) is left out, it says nothing about the translation.
//   * the translation: a generated harness linked against the whole emitted program (every .cpp compiled once
//     per module, into an archive the linker takes what it needs from), run natively after the linked
//     classes' static initialisers in the order j2x_init.cpp gives.
//
// An input on which the Java side throws is not run on the C++ side (Java's exception is C++'s undefined
// behaviour) and not counted. A class is judged on the methods it could run and says how many those were.
//
// Notes on the port:
//   * the Python's tuples with a tag (a slot ('obj', class, maker); a result ('obj'|'self', class, fields)) are
//     records: ObjSlot and Slot (a variant of a primitive/String type name and an ObjSlot), Result with its
//     Kind; a maker is the Maker record; a method of `probeable` is a Probeable.
//   * an input tuple is a py::list<str> of the literals.
//   * run()'s dict is RunResult; compile_java's {class: (folder, main)} is a dict of 2-tuples of str (the
//     folder is '' and the second item says why when javac rejected it).
//   * subprocess.run(timeout=) is not in the runtime: the child processes are run by a local helper that waits
//     for the timeout and ends the child it started (the Python's TimeoutExpired paths).
//   * the module's caches (_INDEX, _PARSED, the compiled patterns) are guarded by a mutex: the Python's judge
//     runs classes on threads under the GIL.
//   * CXX_STD is a module variable the C++17 back end's caller reassigns: a mutable str here.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/java/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::probe {

using ClassRef = py::ref<ir::JClass>;
using JTypeRef = py::ref<ir::JType>;

inline constexpr int64_t REV = 11;
inline constexpr int64_t TIMEOUT = 60;
inline constexpr int64_t _STEP = 180;   // seconds any one compile or link may take
extern py::str CXX_STD;                 // the dialect the program and harnesses compile as (c++17 for the C++17 back end)
inline constexpr int64_t _MAX_CASES = 48;

// Java primitive -> (C++ type, inputs). Inputs are the same on both sides, written as literals.
extern const py::dict<py::str, std::tuple<py::str, py::list<py::str>>> _INPUTS;
extern const py::str _CALL_RE;   // r'\bstatic\s+[\w<>* ]+\b%s\s*\('

// ---------------------------------------------------------------------------------------------- classes
// {class name: .java path} of a staged source tree (cached per tree).
py::dict<py::str, py::str> class_index(const py::str& java_dir);
// The parsed class `name` of the tree, or null.
ClassRef lookup(const py::str& java_dir, const py::str& name);

// A primitive or String type (not an array).
bool _simple(const JTypeRef& t);

// How an instance of a class is made: `factory` is None for a constructor and the name of a static factory
// otherwise; `params` the primitive/String parameter types.
struct Maker {
    std::optional<py::str> factory;
    py::list<py::str> params;
};
// (None: it cannot be made)
std::optional<Maker> maker(const ClassRef& cls);

// [(name, java type)] the class's state is compared by: its own and its ancestors' non-static, non-private
// primitive and String fields that the emitted header declares under that name.
py::list<std::tuple<py::str, py::str>> fields(const py::str& java_dir, const ClassRef& cls, const py::str& cpp_dir);

// ---------------------------------------------------------------------------------------------- what can be run
using Fields = py::list<std::tuple<py::str, py::str>>;
// A parameter's input slot: a primitive/String type name, or an object of a class (with how it is made).
struct ObjSlot {
    py::str cls;
    Maker made;
};
using Slot = std::variant<py::str, ObjSlot>;
// A parameter's input slot, or None when it cannot be passed.
std::optional<Slot> _slot(const py::str& java_dir, const JTypeRef& t);

// How a result is compared.
struct Result {
    enum Kind { None, Prim, Obj, Self } kind = None;
    py::str type;     // Prim: the primitive/String type name
    py::str cls;      // Obj / Self: the class
    Fields fields;    // Obj / Self: what it is compared by
};
Result _result(const py::str& java_dir, const py::str& cpp_dir, const JTypeRef& t);

extern const py::str _CLOCK_PATTERN;
// (methods whose result depends on the clock, whether the constructors read it)
std::tuple<py::set<py::str>, bool> reads_clock(const ClassRef& cls);

// One method the harness can call: its name, input slots, how its result is compared and how its object is
// made (None for a static method).
struct Probeable {
    py::str name;
    py::list<Slot> slots;
    Result result;
    std::optional<Maker> made;
};
// The methods it can call: present under their name in the emitted header, every parameter passable, the
// result comparable (a void instance method compares its object afterwards).
py::list<Probeable> probeable(const ClassRef& cls, const py::str& header_text, const std::optional<py::str>& java_dir = std::nullopt,
                              const std::optional<py::str>& cpp_dir = std::nullopt);

// How many methods a full judgement would have to run (all non-abstract methods and constructors).
int64_t counted(const ClassRef& cls);

// ---------------------------------------------------------------------------------------------- inputs
// The primitive/String types a method's inputs are drawn for: the maker's, then each slot's.
py::list<py::str> _flat(const py::list<Slot>& slots, const std::optional<Maker>& made);
// The deterministic input tuples for a list of primitive/String types: every combination, or -- for several
// -- a fixed sample of at most _MAX_CASES of them, each input of each type used.
py::list<py::list<py::str>> cases(const py::list<py::str>& types);

py::str _java_literal(const py::str& t, const py::str& v);
py::str _cpp_literal(const py::str& t, const py::str& v);

// (make-expression or None, [argument expressions]) for one input tuple.
std::tuple<std::optional<py::str>, py::list<py::str>> _args(const py::list<Slot>& slots, const std::optional<Maker>& made,
                                                            const py::list<py::str>& values, const py::str& lang, const py::str& cls_name);

// ---------------------------------------------------------------------------------------------- harness
extern const py::str _JAVA_HELPERS;
extern const py::str _CPP_HELPERS;
extern const py::dict<py::str, py::str> _FMT;

// A dump_<Class> function per class whose objects are compared by fields.
py::list<py::str> _dumpers(const py::list<Probeable>& methods, const py::str& lang);

// The Java harness class `name`, in the class's own package (so package-private members can be reached).
// `only`: the case keys to run (all of them when None).
py::str java_harness(const py::str& cls_name, const py::list<Probeable>& methods, const std::optional<py::str>& package = std::nullopt,
                     const py::str& name = py::str("J2XProbe"), const std::optional<py::set<py::str>>& only = std::nullopt);

// The C++ side for the inputs in `keep` (the keys Java returned on). `init_order`: classes whose
// initStatics() run first, in order. `ancestry`: (class, base) pairs registered for instanceof first.
py::str cpp_harness(const py::str& cls_name, const py::list<Probeable>& methods, const py::set<py::str>& keep,
                    const py::list<py::str>& init_order, const py::set<py::str>& extra_headers = py::set<py::str>(),
                    const py::list<std::tuple<py::str, py::str>>& ancestry = py::list<std::tuple<py::str, py::str>>());

// The statement printing one call's outcome under {key}.
py::str _statement(const py::str& name, const py::list<Slot>& slots, const Result& result, const std::optional<Maker>& made,
                   const std::optional<py::str>& obj, const py::list<py::str>& args, const py::str& lang, const py::str& cls_name);

// Classes the harness names besides the class itself (made as arguments, compared as results).
py::set<py::str> _classes_named(const py::list<Probeable>& methods, const py::str& cls_name);

// ---------------------------------------------------------------------------------------------- compare
py::dict<py::str, py::str> _results(const py::str& text);
// Do two printed results agree? Floats and doubles by bits, any NaN equal to any NaN.
bool same(const py::str& rt, const py::str& a, const py::str& b);

// ---------------------------------------------------------------------------------------------- building
// The module's .java files laid out by package (javac's -sourcepath needs net/minecraft/src/X.java; a
// decompiled tree is often flat). -> dest
py::str stage_sources(const py::str& java_dir, const py::str& dest);

extern const py::str _BASE_PATTERN;
// (class, base) for every class of the program, from j2x_rtti.cpp.
py::list<std::tuple<py::str, py::str>> ancestry(const py::str& cpp_dir);
// Classes in the order j2x_init.cpp runs their static initialisers.
py::list<py::str> init_order(const py::str& cpp_dir);

// unit -> obj (written under a temporary name and moved into place). -> '' or the error.
py::str _compile(const py::str& cxx, const py::str& cpp_dir, const py::str& unit, const py::str& obj, const std::optional<py::str>& prefix);

// The whole emitted program as one archive, every .cpp compiled once (in parallel): the harnesses link
// against it and the linker takes what each needs. -> the archive path ('' if it cannot be made). A unit that
// does not compile is left out of it (its class is DOES NOT COMPILE anyway).
py::str library(const py::str& cxx, const py::str& cpp_dir, const py::str& work, const std::optional<py::str>& prefix = std::nullopt,
                const std::optional<int64_t>& jobs = std::nullopt);

// Compile the harness and link it against the program's archive. -> (ok, why, [classes linked])
std::tuple<bool, py::str, py::list<py::str>> build_native(const py::str& cxx, const py::str& cpp_dir, const py::str& harness, const py::str& exe,
                                                          const py::str& lib, const std::optional<py::str>& prefix = std::nullopt);

// ---------------------------------------------------------------------------------------------- run
// True when the class is built in two phases (its header declares j2xInit; see twophase.py).
bool _two_phase(const py::str& cpp_dir, const py::str& name);
py::str harness_name(const py::str& cls_name);

// The jars the game is compiled and run against (settings.json `java_libs`: a folder of them -- LWJGL, the
// sound system), sorted; [] when none is set.
py::list<py::str> libraries();

// Compile the stand-ins in probe_shims/ into work/probe/_shims, once. -> the folder, or '' when there are no
// libraries to compile them against or they did not compile.
py::str shims(const py::str& work, const py::str& javac);

// A classpath: these folders, the stand-ins (ahead of the library they stand in for), the libraries.
py::str _cp(const py::str& work, const py::list<py::str>& dirs = py::list<py::str>());

// {class name: (classes folder, harness class to run) or ('', why)}
using Compiled = py::dict<py::str, std::tuple<py::str, py::str>>;
using Todo = py::list<std::tuple<ClassRef, py::list<Probeable>>>;
// Compile the Java harnesses of `todo` in batches of `size`, one javac each, the batches side by side. A batch
// javac rejects is split in half and tried again, so a class whose source cannot compile fails alone.
Compiled compile_java(const py::str& java_dir, const Todo& todo, const py::str& work, const py::str& javac, int64_t size = 40,
                      const std::optional<int64_t>& jobs = std::nullopt, const py::str& tag = py::str("batch"));

// Run the original once more on just the cases in `only`, in their order. -> {key: result}, or None when it
// could not be compiled or run.
std::optional<py::dict<py::str, py::str>> _java_again(const py::str& java_dir, const ClassRef& cls, const py::list<Probeable>& methods,
                                                      const py::str& work, const py::str& javac, const py::str& java, const py::str& jdir,
                                                      const py::set<py::str>& only);

// The named classes that have something to run (see `probeable`).
Todo plan(const py::str& java_dir, const py::str& cpp_dir, const py::list<py::str>& names);

struct RunResult {
    py::list<py::str> methods;
    int64_t judged = 0;
    int64_t total = 0;
    int64_t cases = 0;
    py::list<std::tuple<py::str, py::list<py::str>, py::str, py::str>> differs;   // (method, input, java, cpp)
    py::str why;
    int64_t unsteady = 0;
};

// `java_dir`: the module's sources laid out by package (stage_sources); `compiled`: compile_java's answer for
// the module, when the harnesses were compiled in batches beforehand.
RunResult run(const py::str& java_dir, const py::str& cpp_dir, const ClassRef& cls, const py::str& work, const py::str& cxx,
              const py::str& javac, const py::str& java, const std::optional<py::str>& prefix = std::nullopt,
              const std::optional<Compiled>& compiled = std::nullopt);

}  // namespace codebreaker::java::probe
