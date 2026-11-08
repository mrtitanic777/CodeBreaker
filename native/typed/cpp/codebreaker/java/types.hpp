// codebreaker/java/types.hpp -- typed C++ from codebreaker.java.types.
//
// types.py -- Module 3: resolve a Java type to its C++03 form + OWNERSHIP.
//
// Ownership is the decision that fixes the converter's #1 bug class (value vs
// pointer -> `.` vs `->`, C2227/C2228). It is decided ONCE, here, from the type --
// not patched afterward by regex. `emit` asks this resolver and emits correct code
// the first time.
//
//   ownership(jtype) -> "value"   stack object, accessed with '.'   (int, Vec3D, std::string)
//                    -> "pointer" heap object, accessed with '->'   (Block*, World*, Entity*)
//
// cpp_type(jtype, context) produces the C++ spelling for a field / param / return /
// local / generic-or-array element, applying the proven array and string rules.
//
// Notes on the port:
//   * the Python's `None` defaults of TypeResolver's constructor ("not given" vs "given empty" matters for
//     value_types) are std::optional parameters;
//   * a JType is a py::ref<ir::JType> (shared, as in Python); "or None" results are a null ref;
//   * `members` is {class_name: {'fields': {...}, 'methods': {...}, 'parent': ...}}: a record (MemberEnv)
//     whose `parent` is optional (the Python reads it with .get());
//   * the namedtuple OwnershipCatalog is a struct of its four fields.
#pragma once

#include <cstdint>
#include <optional>

#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/typemap.hpp"
#include "py.hpp"

namespace codebreaker::java::types {

using JTypeRef = py::ref<ir::JType>;

// Whole-program no-GC caller-frees facts the source emitter consults:
//   return_methods  -- method names that return a freshly-built OWNED container (Pattern A cleanup)
//   scalar_methods  -- {name: cpp_elem_type} of methods that return a fresh heap SCALAR (Pattern B)
//   retained_params -- {name: {arg positions}} the method retains -> a temp there must NOT be freed
//   corpus_methods  -- every method name in the corpus (so retention of unknown callees stays unsafe)
struct OwnershipCatalog {
    py::set<py::str> return_methods;
    py::dict<py::str, py::str> scalar_methods;
    py::dict<py::str, py::set<int64_t>> retained_params;
    py::set<py::str> corpus_methods;

    static OwnershipCatalog empty();
};

// One class's entry of the member environment (inheritance already flattened in).
struct MemberEnv {
    py::dict<py::str, JTypeRef> fields;
    py::dict<py::str, JTypeRef> methods;
    std::optional<py::str> parent;
};

class TypeResolver {
  public:
    // user-defined classes parsed from the corpus (everything here is a reference type)
    py::set<py::str> known;
    // class names that are C++ VALUE/struct types (stack, '.' access)
    py::set<py::str> values;
    py::set<py::str> interfaces;
    py::set<py::str> enums;
    // TYPED enums (constants carry state/behaviour) -> emitted as real pointer-classes with a
    // static instance table, NOT int constants. Still in `enums` so `Enum.CONST` resolves to
    // `Enum::CONST` (no s_ prefix), but ownership/cpp_type treat them as pointers.
    py::set<py::str> typed_enums;
    // classes whose objects run asynchronously (Thread subclasses / Runnable) -> the leak
    // transform must NEVER stack-allocate them (the object must outlive the started thread).
    py::set<py::str> thread_like;
    // dropped MCP EmptyN marker types -> strip `(EmptyN*)NULL` bridge args at call sites.
    py::set<py::str> markers;
    // member environment for expression-type inference (chained '.' vs '->'):
    //   {class_name: {'fields': {name: JType}, 'methods': {name: return JType}}}
    // inheritance is already flattened in (closer definitions win). Built by the
    // pipeline from the full corpus; empty in unit tests that don't need chains.
    py::dict<py::str, MemberEnv> members;
    // {class: set(method names colliding with a field)} -> emit those methods as `name_`
    py::dict<py::str, py::set<py::str>> method_renames;
    // whole-program no-GC ownership catalog (caller-frees analysis for the source emitter). Populated
    // by pipeline.build_resolver over the whole corpus; empty in unit tests that don't set it.
    OwnershipCatalog own_catalog;

    TypeResolver(const std::optional<py::set<py::str>>& known_classes = std::nullopt,
                 const std::optional<py::set<py::str>>& value_types = std::nullopt,
                 const std::optional<py::set<py::str>>& interface_types = std::nullopt,
                 const std::optional<py::set<py::str>>& enum_types = std::nullopt,
                 const std::optional<py::dict<py::str, MemberEnv>>& members = std::nullopt,
                 const std::optional<py::dict<py::str, py::set<py::str>>>& method_renames = std::nullopt,
                 const std::optional<py::set<py::str>>& typed_enum_types = std::nullopt,
                 const std::optional<py::set<py::str>>& thread_like = std::nullopt);

    // The C++ emit name for a method: `name_` if it collides with a same-named field in
    // class_name's hierarchy (so field and method don't clash), else name unchanged.
    py::str renamed_method(const py::str& class_name, const py::str& method_name) const;

    // -- classification ------------------------------------------------------
    bool is_primitive(const JTypeRef& jtype) const;
    // "value" or "pointer".
    py::str ownership(const JTypeRef& jtype) const;
    bool is_pointer(const JTypeRef& jtype) const;
    bool is_value(const JTypeRef& jtype) const;

    // -- member environment (for chained '.' vs '->' inference) ---------------
    // JType of class_name's field (own or inherited), or null if unknown.
    JTypeRef member_field_type(const py::str& class_name, const py::str& field_name) const;
    // Return JType of class_name's method (own or inherited), or null if unknown.
    JTypeRef member_method_type(const py::str& class_name, const py::str& method_name) const;
    // True if class_name (transitively) derives from a by-value container (JList/JMap/JSet) --
    // e.g. argo's J_JsonNodeList : List, J_JsonObjectNodeList : Map. Such a class instance IS the
    // container value, so `new X(...)` must be dereferenced where the base container value is wanted.
    bool extends_value_container(const py::str& class_name) const;
    // A PORT message if this type is mapped lossily, else None (for emit's ledger).
    std::optional<py::str> port_note(const JTypeRef& jtype) const;

    // -- C++ spelling --------------------------------------------------------
    py::str base_cpp(const py::str& name) const;
    // C++03 type for a use site.
    //
    // context: 'field' | 'param' | 'return' | 'local' | 'element'
    // 'element' = inside an array or generic; the caller adds any pointer star.
    py::str cpp_type(const JTypeRef& jtype, const py::str& context = py::str("field")) const;

    // convenience wrappers mirroring the call sites emit will use
    py::str field_type(const JTypeRef& jtype) const;
    py::str param_type(const JTypeRef& jtype) const;
    py::str return_type(const JTypeRef& jtype) const;
};

}  // namespace codebreaker::java::types
