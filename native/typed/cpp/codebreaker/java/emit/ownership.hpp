// codebreaker/java/emit/ownership.hpp -- SHIM over cb/java/emit/ownership.hpp (docs/CPP_DESIGN.md 6.3); deleted with
// its last py:: consumer.
//
// Who owns what: the parameter and return spellings and the heap a class must free (see the new header for the rules).
// The old IR's methods and classes are converted to the new IR for each call (ir::to_new) and the old resolver
// answers through the new one it stands for (types::TypeResolver::resolver()).
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/emit/ownership.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"

namespace codebreaker::java::emit::ownership {

using MethodRef = py::ref<ir::Method>;
using ClassRef = py::ref<ir::JClass>;
using types::TypeResolver;

namespace cbo = cb::java::emit::ownership;

namespace shim_ {
inline std::vector<cb::java::ir::JClass> classes_to_new(const py::list<ClassRef>& classes) {
    std::vector<cb::java::ir::JClass> out;
    out.reserve(static_cast<std::size_t>(classes.size()));
    for (const ClassRef& c : classes) out.push_back(ir::to_new(*c));
    return out;
}
}  // namespace shim_

// Names of value-struct PARAMS used as nullable references in the body (compared/assigned null).
inline py::set<py::str> nullable_value_params(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::nullable_value_params(ir::to_new(*method), resolver.resolver())); });
}

// Param names the body REASSIGNS (`p = ...`, `p += ...`, `p++`).
inline py::set<py::str> _reassigned_params(const MethodRef& method) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::reassigned_params(ir::to_new(*method))); });
}

// The C++ parameter list for a method, promoting nullable value-struct params to pointers.
inline py::str params_str(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::params_str(ir::to_new(*method), resolver.resolver())); });
}

// For a static field initialized as `new T[]{...}`, the raw brace contents; else none.
inline std::optional<py::str> aggregate_array_body(const py::ref<ir::Field>& field) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::aggregate_array_body(ir::to_new(*field))); });
}

// {field_name: is_array} for POINTER instance fields this object owns and the destructor may safely `delete`.
inline py::dict<py::str, bool> owned_pointers(const ClassRef& cls, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::owned_pointers(ir::to_new(*cls), resolver.resolver())); });
}

// {field_name: size_accessor} for CONTAINER fields whose every element is a fresh allocation.
inline py::dict<py::str, py::str> owned_containers(const ClassRef& cls, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::owned_containers(ir::to_new(*cls), resolver.resolver())); });
}

// True if the object owns ANY heap (Rule of Three).
inline bool owns_heap(const ClassRef& cls, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cbo::owns_heap(ir::to_new(*cls), resolver.resolver()); });
}

inline bool returns_owned_container(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cbo::returns_owned_container(ir::to_new(*method), resolver.resolver()); });
}

// Program-wide set of method NAMES that return a freshly-built owned container.
inline py::set<py::str> owned_return_container_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver) {
    return cb::compat::guard([&] {
        const auto news = shim_::classes_to_new(classes);
        return cb::compat::to_py(cbo::owned_return_container_methods(news, resolver.resolver()));
    });
}

// True iff EVERY non-null return of the method is a fresh `new X(...)` (the caller owns the result).
inline bool returns_owned_scalar(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cbo::returns_owned_scalar(ir::to_new(*method), resolver.resolver()); });
}

// {method_name: cpp_element_type} for methods that return a fresh heap scalar.
inline py::dict<py::str, py::str> owned_scalar_return_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver) {
    return cb::compat::guard([&] {
        const auto news = shim_::classes_to_new(classes);
        return cb::compat::to_py(cbo::owned_scalar_return_methods(news, resolver.resolver()));
    });
}

// {method_name: set(param_index)} for parameters that SOME definition retains.
inline py::dict<py::str, py::set<int64_t>> retained_param_positions(const py::list<ClassRef>& classes) {
    return cb::compat::guard([&] {
        const auto news = shim_::classes_to_new(classes);
        py::dict<py::str, py::set<int64_t>> out;
        for (const auto& [name, positions] : cbo::retained_param_positions(news)) {
            py::set<int64_t> set;
            for (const std::size_t p : positions) set.add(static_cast<int64_t>(p));
            out.set(cb::compat::to_py(name), set);
        }
        return out;
    });
}

// Whether the method's return should be a C++ pointer (reference types, or a value type promoted by its body).
inline bool returns_pointer(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cbo::returns_pointer(ir::to_new(*method), resolver.resolver()); });
}

// The C++ return type for a method, applying dual-use promotion.
inline py::str return_cpp_type(const MethodRef& method, const TypeResolver& resolver) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbo::return_cpp_type(ir::to_new(*method), resolver.resolver())); });
}

}  // namespace codebreaker::java::emit::ownership
