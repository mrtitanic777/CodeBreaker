// codebreaker/java/ir.hpp -- typed C++ port of codebreaker.java.ir.
//
// ir.py -- the Intermediate Representation: what a Java source file becomes.
//
// This is the CONTRACT between the front-end (`parse/`) and the back-end
// (`types`, `emit`). Everything downstream depends on these shapes and nothing
// else, so the parser can be reworked without touching emit as long as it keeps
// producing valid IR -- and emit can be reworked without touching the parser.
//
// The IR is deliberately dumb: it holds *structure*, not decisions. No C++ types,
// no value/pointer calls, no translation. Those belong to `types` and `emit`.
// Method bodies are kept as raw source lines (with their original line numbers, so
// emit can attribute ledger events back to the Java) until the statement
// translator in `emit/` consumes them.
//
// Notes on the port:
//   * the Python classes are mutable and shared (a Param's JType is the very object a Method holds), so
//     the objects are held as py::ref<T> (a shared_ptr);
//   * "or None" members are a null py::ref (JType) or std::optional (str);
//   * JType.__eq__ / __hash__ / __repr__ are operator== / hash() / repr(); Field/Param/Method/EnumConst/JClass
//     get repr() for __repr__ (their __eq__ is identity, as in Python: compare the py::ref);
//   * hash() is the hash of (name, array_dims, args) -- equal JTypes hash equal (the value differs from CPython's);
//   * JType.element() returns the object itself for a scalar, so it takes the owning py::ref as `self`;
//   * Method.signature() returns the Python tuple (name, tuple-of-reprs) as a tuple of the name and a list of reprs;
//   * body items are (line_no, raw_source_line) tuples; static_inits / inst_inits are lists of such bodies.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::java::ir {

// Access levels, lowest-visibility last. 'package' = Java default (no modifier).
extern const py::str PUBLIC;
extern const py::str PROTECTED;
extern const py::str PACKAGE;
extern const py::str PRIVATE;

struct JType;
struct Field;
struct Param;
struct Method;
struct EnumConst;
struct JClass;

// A Java type reference exactly as written at a use site.
//
// Examples:
//     int                 -> JType('int')
//     int[][]             -> JType('int', array_dims=2)
//     List<ItemStack>     -> JType('List', args=[JType('ItemStack')])
//     String              -> JType('String')
struct JType {
    py::str name;
    int64_t array_dims = 0;
    py::list<py::ref<JType>> args;

    JType(const py::str& name, int64_t array_dims = 0, const py::list<py::ref<JType>>& args = py::list<py::ref<JType>>());

    bool is_array() const;
    bool is_generic() const;
    // The element type of an array (one dimension peeled); self if scalar.
    py::ref<JType> element(const py::ref<JType>& self) const;
    bool operator==(const JType& other) const;
    bool operator!=(const JType& other) const;
    int64_t hash() const;
    py::str repr() const;
};

struct Field {
    py::str name;
    py::ref<JType> jtype;
    py::str access;
    bool is_static = false;
    bool is_final = false;
    std::optional<py::str> initializer;  // raw Java expression string, or None
    int64_t line = 0;

    Field(const py::str& name, const py::ref<JType>& jtype, const py::str& access = PACKAGE, bool is_static = false,
          bool is_final = false, const std::optional<py::str>& initializer = std::nullopt, int64_t line = 0);
    py::str repr() const;
};

struct Param {
    py::str name;
    py::ref<JType> jtype;
    bool is_final = false;
    // Set by pipeline.compute_collection_out_params: a Java collection param the method MUTATES, so the
    // emitter must pass it by C++ reference (JList<T>&) -- a by-value copy loses the callee's writes.
    bool by_ref = false;

    Param(const py::str& name, const py::ref<JType>& jtype, bool is_final = false);
    py::str repr() const;
};

struct Method {
    py::str name;
    py::ref<JType> return_type;  // null for constructors / void handled as JType('void')
    py::list<py::ref<Param>> params;
    py::str access;
    bool is_static = false;
    bool is_abstract = false;
    bool is_final = false;
    bool is_ctor = false;
    py::list<std::tuple<int64_t, py::str>> body;  // list of (line_no, raw_source_line)
    int64_t line = 0;

    Method(const py::str& name, const py::ref<JType>& return_type = nullptr,
           const py::list<py::ref<Param>>& params = py::list<py::ref<Param>>(), const py::str& access = PACKAGE,
           bool is_static = false, bool is_abstract = false, bool is_final = false, bool is_ctor = false,
           const py::list<std::tuple<int64_t, py::str>>& body = py::list<std::tuple<int64_t, py::str>>(),
           int64_t line = 0);

    int64_t arity() const;
    py::list<py::ref<JType>> param_types() const;
    // A name+arity+param-type key, for overload-aware lookups.
    std::tuple<py::str, py::list<py::str>> signature() const;
    py::str repr() const;
};

struct EnumConst {
    py::str name;
    py::list<py::str> args;  // raw Java argument expression strings

    explicit EnumConst(const py::str& name, const py::list<py::str>& args = py::list<py::str>());
    py::str repr() const;
};

// A parsed Java top-level type.
struct JClass {
    py::str name;
    std::optional<py::str> package;
    std::optional<py::str> parent;  // str (extends), or None
    py::list<py::str> interfaces;
    bool is_interface = false;
    bool is_abstract = false;
    bool is_enum = false;
    py::list<py::ref<Field>> fields;
    py::list<py::ref<Method>> methods;
    py::list<py::ref<EnumConst>> enum_consts;
    // raw Java bodies of `static { ... }` and instance `{ ... }` initializer blocks, in
    // source order. Static blocks run once at class load; instance blocks run in each ctor.
    py::list<py::list<std::tuple<int64_t, py::str>>> static_inits;
    py::list<py::list<std::tuple<int64_t, py::str>>> inst_inits;
    std::optional<py::str> source_path;
    // instance fields inherited from ancestors (populated by the pipeline from the
    // full class set); used for value/pointer access of inherited fields in bodies.
    py::list<py::ref<Field>> inherited_fields;
    // synthesized MI-diamond forwarding overrides (populated by pipeline.compute_mi_forwards):
    // [(interface_Method, base_class_name)] -- an interface pure-virtual a base implements.
    py::list<std::tuple<py::ref<Method>, py::str>> mi_forwards;
    // set by emit.source.render_source: True if initStatics() has a non-empty body (deferred
    // static field inits or static{} blocks) -> the pipeline adds it to the ordered init driver.
    bool has_init_statics = false;

    explicit JClass(const py::str& name, const std::optional<py::str>& package = std::nullopt,
                    const std::optional<py::str>& parent = std::nullopt,
                    const py::list<py::str>& interfaces = py::list<py::str>(), bool is_interface = false,
                    bool is_abstract = false, bool is_enum = false,
                    const std::optional<py::str>& source_path = std::nullopt);

    // -- queries used by types/emit -----------------------------------------
    py::ref<Field> field(const py::str& name) const;  // null when there is none
    py::list<py::ref<Field>> instance_fields() const;
    py::list<py::ref<Field>> static_fields() const;
    py::list<py::ref<Method>> constructors() const;
    // First method by that name (use signature() for overload precision); null when none.
    py::ref<Method> method(const py::str& name) const;
    // True if any field initializer allocates (a Rule-of-Three signal at the IR level).
    bool owns_heap_field() const;
    // A Java 5 'typed' enum -- constants carry constructor args and/or the enum declares
    // instance state/behaviour (EnumToolMaterial, EnumArt, EnumCreatureType, ...). These must
    // become real objects (a class with a static instance table), NOT bare int constants.
    // A constant-only enum (EnumMobType: `everything, mobs, players;`) stays int constants.
    bool is_typed_enum() const;
    py::str repr() const;
};

}  // namespace codebreaker::java::ir
