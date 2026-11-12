// cb/java/types.hpp -- resolve a Java type to its C++03 form and its OWNERSHIP.
//
// Ownership is the decision that fixes the converter's #1 bug class (value vs pointer -> `.` vs `->`, C2227/C2228).
// It is decided ONCE, here, from the type -- not patched afterwards by regex. `emit` asks this resolver and emits
// correct code the first time.
//
//   ownership(jtype) -> Value    stack object, accessed with '.'   (int, Vec3D, std::string)
//                    -> Pointer  heap object, accessed with '->'   (Block*, World*, Entity*)
//
// cpp_type(jtype, use) produces the C++ spelling for a field / param / return / local / generic-or-array element,
// applying the proven array and string rules.
//
// A resolver is an ordinary value built from the corpus of one conversion (`ResolverData`); nothing is global.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>

#include "cb/java/ir.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::types {

enum class Ownership { Value, Pointer };

std::string_view to_string(Ownership o);   // "value", "pointer"

// Where a type is spelled. Element = inside an array or a generic: the caller adds any pointer star.
enum class Use { Field, Param, Return, Local, Element };

// Whole-program no-GC caller-frees facts the source emitter consults.
struct OwnershipCatalog {
    // method names that return a freshly-built OWNED container (Pattern A cleanup)
    std::set<std::string, std::less<>> return_methods;
    // {method name: C++ element type} of methods that return a fresh heap SCALAR (Pattern B)
    std::map<std::string, std::string, std::less<>> scalar_methods;
    // {method name: argument positions it retains}: a temporary passed there must NOT be freed
    std::map<std::string, std::set<std::size_t>, std::less<>> retained_params;
    // every method name in the corpus (so retention of unknown callees stays unsafe)
    std::set<std::string, std::less<>> corpus_methods;
};

// One class's entry of the member environment (inheritance already flattened in, closer definitions winning).
struct MemberEnv {
    std::map<std::string, ir::JType, std::less<>> fields;    // field name -> type
    std::map<std::string, ir::JType, std::less<>> methods;   // method name -> return type
    std::optional<std::string> parent;
};

// What a resolver knows about the corpus. All of it is empty by default (an empty corpus, the generic profile).
struct ResolverData {
    // user-defined classes parsed from the corpus (everything here is a reference type)
    typemap::NameSet known;
    // class names that are C++ VALUE/struct types (stack, '.' access); a profile's default_value_types
    typemap::NameSet values;
    typemap::NameSet interfaces;
    typemap::NameSet enums;
    // TYPED enums (constants carry state/behaviour) -> emitted as real pointer-classes with a static instance
    // table, NOT int constants. Still in `enums` so `Enum.CONST` resolves to `Enum::CONST` (no s_ prefix), but
    // ownership/cpp_type treat them as pointers.
    typemap::NameSet typed_enums;
    // classes whose objects run asynchronously (Thread subclasses / Runnable): the leak transform must NEVER
    // stack-allocate them (the object must outlive the started thread).
    typemap::NameSet thread_like;
    // dropped MCP EmptyN marker types -> strip `(EmptyN*)NULL` bridge args at call sites.
    typemap::NameSet markers;
    // member environment for expression-type inference (chained '.' vs '->'), built by the pipeline from the
    // full corpus; empty where a caller does not need chains.
    std::map<std::string, MemberEnv, std::less<>> members;
    // {class: method names colliding with a field} -> emit those methods as `name_`
    std::map<std::string, typemap::NameSet, std::less<>> method_renames;
    // whole-program no-GC ownership catalog (caller-frees analysis for the source emitter)
    OwnershipCatalog own_catalog;

    // The data of the generic corpus under a target profile: no classes, the profile's value types.
    static ResolverData for_profile(const typemap::TypeProfile& profile);
};

class TypeResolver {
public:
    explicit TypeResolver(ResolverData data = {}) : data_(std::move(data)) {}

    // The data the resolver was built from; the pipeline fills in what it learns later (the dropped marker
    // types, the ownership catalog) through the mutable view.
    const ResolverData& data() const { return data_; }
    ResolverData& data() { return data_; }

    // The C++ emit name for a method: `name_` if it collides with a same-named field in class_name's hierarchy (so
    // field and method don't clash), else the name unchanged.
    std::string renamed_method(std::string_view class_name, std::string_view method_name) const;

    // -- classification ------------------------------------------------------
    bool is_primitive(const ir::JType& t) const;
    Ownership ownership(const ir::JType& t) const;
    bool is_pointer(const ir::JType& t) const { return ownership(t) == Ownership::Pointer; }
    bool is_value(const ir::JType& t) const { return ownership(t) == Ownership::Value; }

    // -- member environment (for chained '.' vs '->' inference) ---------------
    // The type of class_name's field (own or inherited), or nullptr if unknown.
    const ir::JType* member_field_type(std::string_view class_name, std::string_view field_name) const;
    // The return type of class_name's method (own or inherited), or nullptr if unknown.
    const ir::JType* member_method_type(std::string_view class_name, std::string_view method_name) const;
    // True if class_name (transitively) derives from a by-value container (JList/JMap/JSet) -- e.g. argo's
    // J_JsonNodeList : List, J_JsonObjectNodeList : Map. Such a class instance IS the container value, so
    // `new X(...)` must be dereferenced where the base container value is wanted.
    bool extends_value_container(std::string_view class_name) const;
    // A PORT message if this type is mapped lossily, else nullopt (for emit's ledger).
    std::optional<std::string_view> port_note(const ir::JType& t) const;

    // -- C++ spelling --------------------------------------------------------
    std::string base_cpp(std::string_view name) const;
    // The C++03 type for a use site.
    std::string cpp_type(const ir::JType& t, Use use = Use::Field) const;
    // the wrappers mirroring the call sites emit uses
    std::string field_type(const ir::JType& t) const { return cpp_type(t, Use::Field); }
    std::string param_type(const ir::JType& t) const { return cpp_type(t, Use::Param); }
    std::string return_type(const ir::JType& t) const { return cpp_type(t, Use::Return); }

private:
    ResolverData data_;
};

}  // namespace cb::java::types
