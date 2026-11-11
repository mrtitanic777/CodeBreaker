// codebreaker/java/emit/naming.hpp -- SHIM over cb/java/emit/naming.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer. (The port's mutable RESERVED_MEMBER_ set had no consumer and is not carried.)
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/java/emit/naming.hpp"
#include "py.hpp"

namespace codebreaker::java::emit::naming {

// What field_name() reads of a field: the Python takes any object with `.name` and `.is_static`.
struct Field {
    py::str name;
    bool is_static = false;
};

inline py::str static_field_name(const py::str& name) {
    return cb::compat::to_py(cb::java::emit::naming::static_field_name(cb::compat::from_py(name)));
}
inline py::str instance_field_name(const py::str& name) {
    return cb::compat::to_py(cb::java::emit::naming::instance_field_name(cb::compat::from_py(name)));
}
inline py::str field_name(const Field& field) {
    return cb::compat::to_py(cb::java::emit::naming::field_name(cb::compat::from_py(field.name), field.is_static));
}
inline py::str guard_macro(const py::str& class_name) {
    return cb::compat::to_py(cb::java::emit::naming::guard_macro(cb::compat::from_py(class_name)));
}
inline py::str enum_member(const py::str& name) {
    return cb::compat::to_py(cb::java::emit::naming::enum_member(cb::compat::from_py(name)));
}

}  // namespace codebreaker::java::emit::naming
