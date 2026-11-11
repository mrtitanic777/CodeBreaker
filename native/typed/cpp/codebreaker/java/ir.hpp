// codebreaker/java/ir.hpp -- SHIM over cb/java/ir.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
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

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/ir.hpp"

namespace codebreaker::java::ir {

// Access levels, lowest-visibility last. 'package' = Java default (no modifier).
inline const py::str PUBLIC("public");
inline const py::str PROTECTED("protected");
inline const py::str PACKAGE("package");
inline const py::str PRIVATE("private");

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



// ---------------------------------------------------------------------------------------------- JType
inline JType::JType(const py::str& name, int64_t array_dims, const py::list<py::ref<JType>>& args)
    : name(name), array_dims(array_dims), args(args.copy()) {}

inline bool JType::is_array() const { return array_dims > 0; }

inline bool JType::is_generic() const { return !args.empty(); }

inline py::ref<JType> JType::element(const py::ref<JType>& self) const {
    if (!array_dims) {
        return self;
    }
    return std::make_shared<JType>(name, array_dims - 1, args);
}

inline bool JType::operator==(const JType& other) const {
    if (!(name == other.name && array_dims == other.array_dims && args.size() == other.args.size())) {
        return false;
    }
    for (int64_t i = 0; i < args.size(); ++i) {
        if (!(*args[i] == *other.args[i])) {
            return false;
        }
    }
    return true;
}

inline bool JType::operator!=(const JType& other) const { return !(*this == other); }

inline int64_t JType::hash() const {
    uint64_t h = static_cast<uint64_t>(py::str_hash()(name));
    h = h * 1000003u ^ static_cast<uint64_t>(array_dims);
    for (const py::ref<JType>& a : args) {
        h = h * 1000003u ^ static_cast<uint64_t>(a->hash());
    }
    return static_cast<int64_t>(h);
}

inline py::str JType::repr() const {
    py::str s = name;
    if (!args.empty()) {
        py::list<py::str> parts;
        for (const py::ref<JType>& a : args) {
            parts.append(a->repr());
        }
        s = s + py::str("<") + py::join(py::str(", "), parts) + py::str(">");
    }
    return s + py::repeat(py::str("[]"), array_dims);
}

// ---------------------------------------------------------------------------------------------- Field
inline Field::Field(const py::str& name, const py::ref<JType>& jtype, const py::str& access, bool is_static, bool is_final,
             const std::optional<py::str>& initializer, int64_t line)
    : name(name), jtype(jtype), access(access), is_static(is_static), is_final(is_final), initializer(initializer),
      line(line) {}

inline py::str Field::repr() const {
    py::str mods = (is_static ? py::str("static ") : py::str("")) + (is_final ? py::str("final ") : py::str(""));
    return py::percent(py::str("<Field %s%s %s>"), mods, jtype->repr(), name);
}

// ---------------------------------------------------------------------------------------------- Param
inline Param::Param(const py::str& name, const py::ref<JType>& jtype, bool is_final)
    : name(name), jtype(jtype), is_final(is_final), by_ref(false) {}

inline py::str Param::repr() const { return py::percent(py::str("<Param %s %s>"), jtype->repr(), name); }

// ---------------------------------------------------------------------------------------------- Method
inline Method::Method(const py::str& name, const py::ref<JType>& return_type, const py::list<py::ref<Param>>& params,
               const py::str& access, bool is_static, bool is_abstract, bool is_final, bool is_ctor,
               const py::list<std::tuple<int64_t, py::str>>& body, int64_t line)
    : name(name), return_type(return_type), params(params.copy()), access(access), is_static(is_static),
      is_abstract(is_abstract), is_final(is_final), is_ctor(is_ctor), body(body.copy()), line(line) {}

inline int64_t Method::arity() const { return params.size(); }

inline py::list<py::ref<JType>> Method::param_types() const {
    py::list<py::ref<JType>> out;
    for (const py::ref<Param>& p : params) {
        out.append(p->jtype);
    }
    return out;
}

inline std::tuple<py::str, py::list<py::str>> Method::signature() const {
    py::list<py::str> reprs;
    for (const py::ref<Param>& p : params) {
        reprs.append(p->jtype->repr());
    }
    return std::make_tuple(name, reprs);
}

inline py::str Method::repr() const {
    py::str kind = is_ctor ? py::str("ctor") : (return_type ? return_type->repr() : py::str("void"));
    return py::percent(py::str("<Method %s %s(%d)>"), kind, name, arity());
}

// ---------------------------------------------------------------------------------------------- EnumConst
inline EnumConst::EnumConst(const py::str& name, const py::list<py::str>& args) : name(name), args(args.copy()) {}

inline py::str EnumConst::repr() const { return py::percent(py::str("<EnumConst %s(%d)>"), name, args.size()); }

// ---------------------------------------------------------------------------------------------- JClass
inline JClass::JClass(const py::str& name, const std::optional<py::str>& package, const std::optional<py::str>& parent,
               const py::list<py::str>& interfaces, bool is_interface, bool is_abstract, bool is_enum,
               const std::optional<py::str>& source_path)
    : name(name), package(package), parent(parent), interfaces(interfaces.copy()), is_interface(is_interface),
      is_abstract(is_abstract), is_enum(is_enum), source_path(source_path), has_init_statics(false) {}

inline py::ref<Field> JClass::field(const py::str& name) const {
    for (const py::ref<Field>& f : fields) {
        if (f->name == name) {
            return f;
        }
    }
    return nullptr;
}

inline py::list<py::ref<Field>> JClass::instance_fields() const {
    py::list<py::ref<Field>> out;
    for (const py::ref<Field>& f : fields) {
        if (!f->is_static) {
            out.append(f);
        }
    }
    return out;
}

inline py::list<py::ref<Field>> JClass::static_fields() const {
    py::list<py::ref<Field>> out;
    for (const py::ref<Field>& f : fields) {
        if (f->is_static) {
            out.append(f);
        }
    }
    return out;
}

inline py::list<py::ref<Method>> JClass::constructors() const {
    py::list<py::ref<Method>> out;
    for (const py::ref<Method>& m : methods) {
        if (m->is_ctor) {
            out.append(m);
        }
    }
    return out;
}

inline py::ref<Method> JClass::method(const py::str& name) const {
    for (const py::ref<Method>& m : methods) {
        if (m->name == name) {
            return m;
        }
    }
    return nullptr;
}

inline bool JClass::owns_heap_field() const {
    for (const py::ref<Field>& f : fields) {
        // `f.initializer and 'new ' in f.initializer`: None and '' are both falsy
        if (f->initializer && f->initializer->size() > 0 && f->initializer->contains(py::str("new "))) {
            return true;
        }
    }
    return false;
}

inline bool JClass::is_typed_enum() const {
    if (!is_enum) {
        return false;
    }
    for (const py::ref<EnumConst>& ec : enum_consts) {
        if (!ec->args.empty()) {
            return true;
        }
    }
    if (!instance_fields().empty()) {
        return true;
    }
    for (const py::ref<Method>& m : methods) {
        if (!m->is_ctor) {
            return true;
        }
    }
    return false;
}

inline py::str JClass::repr() const {
    py::str kind = is_interface ? py::str("interface") : (is_enum ? py::str("enum") : py::str("class"));
    py::str ext = parent ? py::percent(py::str(" extends %s"), *parent) : py::str("");
    return py::percent(py::str("<JClass %s %s%s f=%d m=%d>"), kind, name, ext, fields.size(), methods.size());
}


// ---- conversion to and from the new IR (cb/java/ir.hpp) ------------------------------------------------------
//
// The old IR is a graph of shared, mutable objects that its consumers (parse, types, emit) mutate in place, so
// this shim keeps it whole; refactored code works on cb::java::ir and meets it through these converters. A class
// converts alone: `inherited_fields` and `mi_forwards` are derived by the pipeline from the full class set and are
// not carried either way.

inline cb::java::ir::JType to_new(const JType& t) {
    cb::java::ir::JType out;
    out.name = cb::compat::from_py(t.name);
    out.array_dims = cb::compat::int_from_py<std::uint32_t>(t.array_dims);
    for (const py::ref<JType>& a : t.args) out.args.push_back(to_new(*a));
    return out;
}

inline py::ref<JType> from_new(const cb::java::ir::JType& t) {
    py::list<py::ref<JType>> args;
    for (const auto& a : t.args) args.append(from_new(a));
    return std::make_shared<JType>(cb::compat::to_py(t.name), static_cast<int64_t>(t.array_dims), args);
}

namespace conv_ {
inline py::str access(cb::java::ir::Access a) { return cb::compat::to_py(cb::java::ir::to_string(a)); }
inline cb::java::ir::Access access(const py::str& s) {
    auto a = cb::java::ir::parse_access(cb::compat::from_py(s));
    if (!a) throw py::ValueError("not an access level: " + cb::compat::from_py(s));
    return *a;
}
inline cb::java::ir::Body body(const py::list<std::tuple<int64_t, py::str>>& b) {
    cb::java::ir::Body out;
    for (const auto& [n, text] : b) out.push_back({cb::compat::int_from_py<std::uint32_t>(n), cb::compat::from_py(text)});
    return out;
}
inline py::list<std::tuple<int64_t, py::str>> body(const cb::java::ir::Body& b) {
    py::list<std::tuple<int64_t, py::str>> out;
    for (const auto& l : b) out.append(std::make_tuple(static_cast<int64_t>(l.number), cb::compat::to_py(l.text)));
    return out;
}
}  // namespace conv_

inline cb::java::ir::Field to_new(const Field& f) {
    cb::java::ir::Field out;
    out.name = cb::compat::from_py(f.name);
    out.type = to_new(*f.jtype);
    out.access = conv_::access(f.access);
    out.is_static = f.is_static;
    out.is_final = f.is_final;
    if (f.initializer) out.initializer = cb::compat::from_py(*f.initializer);
    out.line = cb::compat::int_from_py<std::uint32_t>(f.line);
    return out;
}

inline py::ref<Field> from_new(const cb::java::ir::Field& f) {
    return std::make_shared<Field>(cb::compat::to_py(f.name), from_new(f.type), conv_::access(f.access), f.is_static,
                                   f.is_final,
                                   f.initializer ? std::optional<py::str>(cb::compat::to_py(*f.initializer)) : std::nullopt,
                                   static_cast<int64_t>(f.line));
}

inline cb::java::ir::Method to_new(const Method& m) {
    cb::java::ir::Method out;
    out.name = cb::compat::from_py(m.name);
    if (m.return_type) out.return_type = to_new(*m.return_type);
    for (const py::ref<Param>& p : m.params) {
        out.params.push_back({cb::compat::from_py(p->name), to_new(*p->jtype), p->is_final, p->by_ref});
    }
    out.access = conv_::access(m.access);
    out.is_static = m.is_static;
    out.is_abstract = m.is_abstract;
    out.is_final = m.is_final;
    out.is_ctor = m.is_ctor;
    out.body = conv_::body(m.body);
    out.line = cb::compat::int_from_py<std::uint32_t>(m.line);
    return out;
}

inline py::ref<Method> from_new(const cb::java::ir::Method& m) {
    py::list<py::ref<Param>> params;
    for (const auto& p : m.params) {
        auto q = std::make_shared<Param>(cb::compat::to_py(p.name), from_new(p.type), p.is_final);
        q->by_ref = p.by_ref;
        params.append(q);
    }
    return std::make_shared<Method>(cb::compat::to_py(m.name), m.return_type ? from_new(*m.return_type) : nullptr,
                                    params, conv_::access(m.access), m.is_static, m.is_abstract, m.is_final, m.is_ctor,
                                    conv_::body(m.body), static_cast<int64_t>(m.line));
}

inline cb::java::ir::JClass to_new(const JClass& c) {
    cb::java::ir::JClass out;
    out.name = cb::compat::from_py(c.name);
    if (c.package) out.package = cb::compat::from_py(*c.package);
    if (c.parent) out.parent = cb::compat::from_py(*c.parent);
    out.interfaces = cb::compat::from_py(c.interfaces);
    out.is_interface = c.is_interface;
    out.is_abstract = c.is_abstract;
    out.is_enum = c.is_enum;
    for (const py::ref<Field>& f : c.fields) out.fields.push_back(to_new(*f));
    for (const py::ref<Method>& m : c.methods) out.methods.push_back(to_new(*m));
    for (const py::ref<EnumConst>& e : c.enum_consts) out.enum_consts.push_back({cb::compat::from_py(e->name), cb::compat::from_py(e->args)});
    for (const auto& b : c.static_inits) out.static_inits.push_back(conv_::body(b));
    for (const auto& b : c.inst_inits) out.inst_inits.push_back(conv_::body(b));
    if (c.source_path) out.source_path = cb::fs::from_text(cb::compat::from_py(*c.source_path));
    out.has_init_statics = c.has_init_statics;
    return out;
}

inline py::ref<JClass> from_new(const cb::java::ir::JClass& c) {
    auto out = std::make_shared<JClass>(
        cb::compat::to_py(c.name),
        c.package ? std::optional<py::str>(cb::compat::to_py(*c.package)) : std::nullopt,
        c.parent ? std::optional<py::str>(cb::compat::to_py(*c.parent)) : std::nullopt, cb::compat::to_py(c.interfaces),
        c.is_interface, c.is_abstract, c.is_enum,
        c.source_path ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*c.source_path))) : std::nullopt);
    for (const auto& f : c.fields) out->fields.append(from_new(f));
    for (const auto& m : c.methods) out->methods.append(from_new(m));
    for (const auto& e : c.enum_consts) out->enum_consts.append(std::make_shared<EnumConst>(cb::compat::to_py(e.name), cb::compat::to_py(e.args)));
    for (const auto& b : c.static_inits) out->static_inits.append(conv_::body(b));
    for (const auto& b : c.inst_inits) out->inst_inits.append(conv_::body(b));
    out->has_init_statics = c.has_init_statics;
    return out;
}
}  // namespace codebreaker::java::ir



