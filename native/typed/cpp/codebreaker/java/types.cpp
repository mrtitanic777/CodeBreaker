// codebreaker/java/types.cpp -- typed C++ from codebreaker.java.types.

#include "codebreaker/java/types.hpp"

namespace codebreaker::java::types {

namespace {

// set(x) / set(x or ()): a fresh copy (py::set is a shared handle, a Python set(...) call copies)
py::set<py::str> copy_of(const std::optional<py::set<py::str>>& x) {
    return x ? py::set<py::str>::from(*x) : py::set<py::str>();
}

}  // namespace

OwnershipCatalog OwnershipCatalog::empty() {
    return OwnershipCatalog{py::set<py::str>(), py::dict<py::str, py::str>(),
                            py::dict<py::str, py::set<int64_t>>(), py::set<py::str>()};
}

TypeResolver::TypeResolver(const std::optional<py::set<py::str>>& known_classes,
                           const std::optional<py::set<py::str>>& value_types,
                           const std::optional<py::set<py::str>>& interface_types,
                           const std::optional<py::set<py::str>>& enum_types,
                           const std::optional<py::dict<py::str, MemberEnv>>& members_,
                           const std::optional<py::dict<py::str, py::set<py::str>>>& method_renames_,
                           const std::optional<py::set<py::str>>& typed_enum_types,
                           const std::optional<py::set<py::str>>& thread_like_)
    : known(copy_of(known_classes)),
      values(value_types ? py::set<py::str>::from(*value_types) : py::set<py::str>::from(typemap::DEFAULT_VALUE_TYPES)),
      interfaces(copy_of(interface_types)),
      enums(copy_of(enum_types)),
      typed_enums(copy_of(typed_enum_types)),
      thread_like(copy_of(thread_like_)),
      markers(),
      // `members or {}`: a non-empty dict is kept (aliased), an empty or absent one is a fresh {}
      members(members_ && !members_->empty() ? *members_ : py::dict<py::str, MemberEnv>()),
      method_renames(method_renames_ && !method_renames_->empty() ? *method_renames_
                                                                  : py::dict<py::str, py::set<py::str>>()),
      own_catalog(OwnershipCatalog::empty()) {}

py::str TypeResolver::renamed_method(const py::str& class_name, const py::str& method_name) const {
    std::optional<py::set<py::str>> renames = method_renames.get(class_name);
    if (renames && renames->contains(method_name)) {
        return method_name + "_";
    }
    return method_name;
}

// -- classification ------------------------------------------------------
bool TypeResolver::is_primitive(const JTypeRef& jtype) const { return typemap::PRIMITIVES.contains(jtype->name); }

py::str TypeResolver::ownership(const JTypeRef& jtype) const {
    if (jtype->is_array()) {
        return py::str("value");  // Java arrays -> JArray<T> (value wrapper)
    }
    const py::str& n = jtype->name;
    if (typemap::PRIMITIVES.contains(n)) {
        return py::str("value");
    }
    if (typed_enums.contains(n)) {
        return py::str("pointer");  // typed enum -> real object (singleton instances)
    }
    if (enums.contains(n)) {
        return py::str("value");  // constant-only enum -> int (value)
    }
    if (typemap::VALUE_STDLIB.contains(n)) {
        return py::str("value");  // std::string
    }
    if (typemap::CONTAINERS.contains(n)) {
        return py::str("value");  // std::vector / std::map held by value
    }
    if (values.contains(n)) {
        return py::str("value");  // Vec3D / AxisAlignedBB / ...
    }
    // Object -> JObject (a boxed VALUE), and boxed primitives (Integer/Character/...) map to
    // C++ primitives -- all value types, not pointers.
    if (n == py::str("Object") || typemap::BOXED_VALUE_TYPES.contains(n)) {
        return py::str("value");
    }
    // everything else -- user classes, interfaces, enums, unknown Capitalized -- is a reference
    if (known.contains(n) || interfaces.contains(n) || enums.contains(n) || n.slice(0, 1).isupper()) {
        return py::str("pointer");
    }
    return py::str("value");  // stray lowercase / generic param
}

bool TypeResolver::is_pointer(const JTypeRef& jtype) const { return ownership(jtype) == py::str("pointer"); }

bool TypeResolver::is_value(const JTypeRef& jtype) const { return ownership(jtype) == py::str("value"); }

// -- member environment (for chained '.' vs '->' inference) ---------------
JTypeRef TypeResolver::member_field_type(const py::str& class_name, const py::str& field_name) const {
    std::optional<MemberEnv> e = members.get(class_name);
    return e ? e->fields.get(field_name) : JTypeRef();
}

JTypeRef TypeResolver::member_method_type(const py::str& class_name, const py::str& method_name) const {
    std::optional<MemberEnv> e = members.get(class_name);
    return e ? e->methods.get(method_name) : JTypeRef();
}

bool TypeResolver::extends_value_container(const py::str& class_name) const {
    py::set<py::str> seen;
    std::optional<py::str> cur = class_name;
    // `while cur and cur not in seen`
    while (cur && !cur->empty() && !seen.contains(*cur)) {
        seen.add(*cur);
        std::optional<MemberEnv> e = members.get(*cur);
        std::optional<py::str> parent = e ? e->parent : std::nullopt;
        if (!parent || parent->empty()) {
            return false;
        }
        py::str cpp = typemap::JAVA_TO_CPP.get(*parent, *parent);
        if (cpp == py::str("JList") || cpp == py::str("JSet") || cpp == py::str("JMap")) {
            return true;
        }
        cur = parent;
    }
    return false;
}

std::optional<py::str> TypeResolver::port_note(const JTypeRef& jtype) const {
    return typemap::PORT_NOTE_TYPES.get(jtype->name);
}

// -- C++ spelling --------------------------------------------------------
py::str TypeResolver::base_cpp(const py::str& name) const { return typemap::JAVA_TO_CPP.get(name, name); }

py::str TypeResolver::cpp_type(const JTypeRef& jtype, const py::str& context) const {
    // Arrays: JArray<elem> -- a length-carrying, bounds-checked value wrapper. A
    // reference-type element is itself a pointer (JArray<Block*>); a value/primitive
    // element is stored directly (JArray<int>); a nested array is JArray<JArray<int> >.
    if (jtype->is_array()) {
        JTypeRef elem_jt = jtype->element(jtype);
        // Object[] -> JArray<JObject>: a boxed element that can hold a mixed String/char/pointer
        // (Java autoboxing). Scalar Object stays void*; only the array element is boxed.
        if (elem_jt->name == py::str("Object") && !elem_jt->is_array()) {
            return py::str("JArray<JObject >");
        }
        // byte[] BUFFER stays unsigned char (chunk/packet/inflate data read via & 0xFF), even
        // though a SCALAR byte is signed char. This is the one place the two byte policies split.
        if (elem_jt->name == py::str("byte") && !elem_jt->is_array()) {
            return py::str("JArray<unsigned char >");
        }
        py::str elem = cpp_type(elem_jt, py::str("element"));
        if (ownership(elem_jt) == py::str("pointer") && !elem.endswith(py::str("*"))) {
            elem += py::str("*");
        }
        return py::str("JArray<") + elem + py::str(" >");  // trailing space keeps nested >> valid C++03
    }

    // Generics: std::vector<Elem(*)>, std::map<...>, etc.
    if (jtype->is_generic()) {
        py::str outer = base_cpp(jtype->name);
        py::str joined;
        bool first = true;
        for (const JTypeRef& a : jtype->args) {
            py::str ac = cpp_type(a, py::str("element"));
            if (is_pointer(a) && !ac.endswith(py::str("*"))) {
                ac += py::str("*");
            }
            if (!first) {
                joined += py::str(", ");
            }
            first = false;
            joined += ac;
        }
        return outer + py::str("<") + joined + py::str(">");
    }

    const py::str& name = jtype->name;
    if (enums.contains(name) && !typed_enums.contains(name)) {
        return py::str("int");  // constant-only enum variable/field/param/return -> int
    }
    // String is passed as const ref (cheap), but RETURNED by value: methods often
    // build a new string (concat), and returning that as const& would dangle. Uses
    // JavaString (a std::string subclass with Java's methods) so s.equals()/s.trim()/...
    // compile on the parameter too.
    if (name == py::str("String") && context == py::str("param")) {
        return py::str("const JavaString&");
    }

    py::str cpp = base_cpp(name);
    // raw (non-generic) container -> give it a concrete element type so it's valid C++
    if (typemap::RAW_CONTAINER_DEFAULT.contains(cpp)) {
        cpp += typemap::RAW_CONTAINER_DEFAULT.at(cpp);
    }
    // Add the ownership pointer for reference types -- except in 'element'
    // context, where the array/generic caller is responsible for the star.
    if (context != py::str("element") && ownership(jtype) == py::str("pointer") && !cpp.endswith(py::str("*"))) {
        cpp += py::str("*");
    }
    return cpp;
}

// convenience wrappers mirroring the call sites emit will use
py::str TypeResolver::field_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("field")); }

py::str TypeResolver::param_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("param")); }

py::str TypeResolver::return_type(const JTypeRef& jtype) const { return cpp_type(jtype, py::str("return")); }

}  // namespace codebreaker::java::types
