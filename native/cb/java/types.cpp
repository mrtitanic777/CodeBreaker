// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/types.hpp"

namespace cb::java::types {

namespace {

bool is_upper_ascii(char c) { return c >= 'A' && c <= 'Z'; }

bool has_name(const typemap::NameSet& set, std::string_view name) { return set.find(name) != set.end(); }

bool ends_with_star(const std::string& s) { return !s.empty() && s.back() == '*'; }

}  // namespace

std::string_view to_string(Ownership o) { return o == Ownership::Value ? "value" : "pointer"; }

ResolverData ResolverData::for_profile(const typemap::TypeProfile& profile) {
    ResolverData d;
    d.values = profile.default_value_types;
    return d;
}

std::string TypeResolver::renamed_method(std::string_view class_name, std::string_view method_name) const {
    const auto it = data_.method_renames.find(class_name);
    if (it != data_.method_renames.end() && has_name(it->second, method_name)) return std::string(method_name) + "_";
    return std::string(method_name);
}

bool TypeResolver::is_primitive(const ir::JType& t) const { return has_name(typemap::primitives(), t.name); }

Ownership TypeResolver::ownership(const ir::JType& t) const {
    if (t.is_array()) return Ownership::Value;   // Java arrays -> JArray<T> (value wrapper)
    const std::string& n = t.name;
    if (has_name(typemap::primitives(), n)) return Ownership::Value;
    if (has_name(data_.typed_enums, n)) return Ownership::Pointer;   // typed enum -> real object (singleton instances)
    if (has_name(data_.enums, n)) return Ownership::Value;           // constant-only enum -> int (value)
    if (has_name(typemap::value_stdlib(), n)) return Ownership::Value;   // std::string
    if (has_name(typemap::containers(), n)) return Ownership::Value;     // std::vector / std::map held by value
    if (has_name(data_.values, n)) return Ownership::Value;              // Vec3D / AxisAlignedBB / ...
    // Object -> JObject (a boxed VALUE), and boxed primitives (Integer/Character/...) map to C++ primitives -- all
    // value types, not pointers.
    if (n == "Object" || has_name(typemap::boxed_value_types(), n)) return Ownership::Value;
    // everything else -- user classes, interfaces, enums, unknown Capitalized -- is a reference
    if (has_name(data_.known, n) || has_name(data_.interfaces, n) || has_name(data_.enums, n) ||
        (!n.empty() && is_upper_ascii(n.front()))) {
        return Ownership::Pointer;
    }
    return Ownership::Value;   // stray lowercase / generic param
}

const ir::JType* TypeResolver::member_field_type(std::string_view class_name, std::string_view field_name) const {
    const auto e = data_.members.find(class_name);
    if (e == data_.members.end()) return nullptr;
    const auto f = e->second.fields.find(field_name);
    return f == e->second.fields.end() ? nullptr : &f->second;
}

const ir::JType* TypeResolver::member_method_type(std::string_view class_name, std::string_view method_name) const {
    const auto e = data_.members.find(class_name);
    if (e == data_.members.end()) return nullptr;
    const auto m = e->second.methods.find(method_name);
    return m == e->second.methods.end() ? nullptr : &m->second;
}

bool TypeResolver::extends_value_container(std::string_view class_name) const {
    std::set<std::string, std::less<>> seen;
    std::string_view cur = class_name;
    while (!cur.empty() && !seen.contains(cur)) {
        seen.emplace(cur);
        const auto e = data_.members.find(cur);
        if (e == data_.members.end() || !e->second.parent || e->second.parent->empty()) return false;
        const std::string& parent = *e->second.parent;
        const std::string cpp = base_cpp(parent);
        if (cpp == "JList" || cpp == "JSet" || cpp == "JMap") return true;
        cur = parent;
    }
    return false;
}

std::optional<std::string_view> TypeResolver::port_note(const ir::JType& t) const {
    const auto& notes = typemap::port_note_types();
    const auto it = notes.find(t.name);
    if (it == notes.end()) return std::nullopt;
    return std::string_view(it->second);
}

std::string TypeResolver::base_cpp(std::string_view name) const {
    const auto& map = typemap::java_to_cpp();
    const auto it = map.find(name);
    return it == map.end() ? std::string(name) : it->second;
}

std::string TypeResolver::cpp_type(const ir::JType& t, Use use) const {
    // Arrays: JArray<elem> -- a length-carrying, bounds-checked value wrapper. A reference-type element is itself a
    // pointer (JArray<Block*>); a value/primitive element is stored directly (JArray<int>); a nested array is
    // JArray<JArray<int> >.
    if (t.is_array()) {
        const ir::JType elem_t = t.element();
        // Object[] -> JArray<JObject>: a boxed element that can hold a mixed String/char/pointer (Java autoboxing).
        // Scalar Object stays void*; only the array element is boxed.
        if (elem_t.name == "Object" && !elem_t.is_array()) return "JArray<JObject >";
        // byte[] BUFFER stays unsigned char (chunk/packet/inflate data read via & 0xFF), even though a SCALAR byte is
        // signed char. This is the one place the two byte policies split.
        if (elem_t.name == "byte" && !elem_t.is_array()) return "JArray<unsigned char >";
        std::string elem = cpp_type(elem_t, Use::Element);
        if (ownership(elem_t) == Ownership::Pointer && !ends_with_star(elem)) elem += '*';
        return "JArray<" + elem + " >";   // the trailing space keeps a nested >> valid C++03
    }

    // Generics: std::vector<Elem(*)>, std::map<...>, etc.
    if (t.is_generic()) {
        std::string out = base_cpp(t.name) + "<";
        bool first = true;
        for (const ir::JType& a : t.args) {
            std::string ac = cpp_type(a, Use::Element);
            if (is_pointer(a) && !ends_with_star(ac)) ac += '*';
            if (!first) out += ", ";
            first = false;
            out += ac;
        }
        return out + ">";
    }

    const std::string& name = t.name;
    if (has_name(data_.enums, name) && !has_name(data_.typed_enums, name)) return "int";   // constant-only enum -> int
    // String is passed as const ref (cheap), but RETURNED by value: methods often build a new string (concat), and
    // returning that as const& would dangle. JavaString (a std::string subclass with Java's methods) is used so
    // s.equals()/s.trim()/... compile on the parameter too.
    if (name == "String" && use == Use::Param) return "const JavaString&";

    std::string cpp = base_cpp(name);
    // raw (non-generic) container -> give it a concrete element type so it is valid C++
    const auto& raw = typemap::raw_container_default();
    if (const auto it = raw.find(cpp); it != raw.end()) cpp += it->second;
    // Add the ownership pointer for reference types -- except in Element context, where the array/generic caller is
    // responsible for the star.
    if (use != Use::Element && ownership(t) == Ownership::Pointer && !ends_with_star(cpp)) cpp += '*';
    return cpp;
}

}  // namespace cb::java::types
