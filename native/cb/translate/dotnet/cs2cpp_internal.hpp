// cb/translate/dotnet/cs2cpp_internal.hpp -- what the modules of the C# -> C++03 pass share and nobody else needs.
#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include <string_view>

#include "cb/translate/dotnet/cs2cpp.hpp"

namespace cb::translate::dotnet::cs2cpp::impl {

// IR primitive kinds -> the Java names; and the boxed class of each.
const std::map<std::string, std::string, std::less<>>& primitive_java();
const std::map<std::string, std::string, std::less<>>& boxed_classes();
// .NET classes -> the Java classes they are rebound to; Math functions -> java.lang.Math's.
const std::map<std::string, std::string, std::less<>>& library_classes();
const std::map<std::string, std::string, std::less<>>& math_functions();
// .NET method names the converter's Object methods are called by in Java.
const std::map<std::string, std::string, std::less<>>& object_methods();

ir::TypeP string_type();   // java.lang.String
ir::TypeP object_type();   // java.lang.Object

// A type from its IR kind: a primitive, or void.
ir::TypeP type_of_kind(std::string_view kind);
// A method symbol of Java's library (or of the program).
ir::SymP method_sym(std::string name, std::optional<std::string> owner, std::vector<ir::TypeP> params, ir::TypeP ret, bool is_static = false);

inline bool one_of(std::string_view s, std::initializer_list<std::string_view> xs) {
    for (std::string_view x : xs) {
        if (s == x) return true;
    }
    return false;
}
inline bool is_str(const std::optional<std::string>& s, std::string_view x) { return s && *s == x; }
inline std::string or_none(const std::optional<std::string>& s) { return s ? *s : std::string("None"); }

// Whether a body slot holds nothing (an abstract or compiler-supplied member).
inline bool body_absent(const ir::Suite& b) { return std::holds_alternative<ir::Node*>(b) && std::get<ir::Node*>(b) == nullptr; }
inline const ir::Sym* property_sym(const ir::Node* n) {
    const auto* s = ir::get<ir::Select>(n);
    return s != nullptr && s->sym && s->sym->kind == ir::SymKind::Property ? s->sym.get() : nullptr;
}

}  // namespace cb::translate::dotnet::cs2cpp::impl
