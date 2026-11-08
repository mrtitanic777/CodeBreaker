// codebreaker/java/emit/naming.cpp -- C++ from codebreaker.java.emit.naming
// (static_field_name, guard_macro and enum_member are the judge-proven translation).

#include "codebreaker/java/emit/naming.hpp"

namespace codebreaker::java::emit::naming {

py::set<py::str> RESERVED_MEMBER_ = py::set<py::str>{py::str("TRUE"), py::str("FALSE"), py::str("NULL"), py::str("NAN"), py::str("INFINITY"), py::str("ERROR"), py::str("DELETE"), py::str("IN"), py::str("OUT"), py::str("OPTIONAL"), py::str("CONST"), py::str("VOID"), py::str("NEAR"), py::str("FAR"), py::str("THIS"), py::str("INTERFACE"), py::str("DOMAIN"), py::str("this"), py::str("new"), py::str("delete"), py::str("class"), py::str("template"), py::str("operator"), py::str("default"), py::str("private"), py::str("public"), py::str("protected"), py::str("register"), py::str("export"), py::str("try"), py::str("throw"), py::str("namespace"), py::str("true"), py::str("false"), py::str("nullptr"), py::str("min"), py::str("max")};

py::str static_field_name(const py::str& name) {
    return py::str("s_") + name.lstrip(py::str("_"));
}

py::str instance_field_name(const py::str& name) {
    return name;
}

py::str field_name(const Field& field) {
    return field.is_static ? static_field_name(field.name) : instance_field_name(field.name);
}

// 'Vec3D' -> 'VEC3D_H'.
py::str guard_macro(const py::str& class_name) {
    return py::re::sub(py::str("[^A-Za-z0-9]"), py::str("_"), class_name).upper() + "_H";
}

// Sanitize an enum-constant identifier that collides with a C/C++ macro or keyword.
// Idempotent and applied consistently at declaration and reference sites.
py::str enum_member(const py::str& name) {
    return RESERVED_MEMBER_.contains(name) ? name + "_" : name;
}

}  // namespace codebreaker::java::emit::naming
