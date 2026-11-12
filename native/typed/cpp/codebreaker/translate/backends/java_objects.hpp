// codebreaker/translate/backends/java_objects.hpp -- COMPAT SHIM over cb/translate/backends/java_objects.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/java_objects.{hpp,cpp}: the Java back end's idiomatic tier (C structs and C++ classes as
// Java objects). The old program and function writers (Objects, ObjWriter) extended the old java.hpp's Faithful and FnWriter, which are
// the new module's now; the old layer asks for the tier through java::program (the shim in java.hpp), so what is left here is the one
// pure function.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/translate/backends/java_objects.hpp"
#include "codebreaker/translate/backends/java.hpp"

namespace codebreaker::translate::backends::java_objects {

// operator+ -> plus, ...; Refused for an operator the table does not name
inline py::str method_name(const py::str& name) {
    try {
        return cb::compat::to_py(cb::translate::backends::java_objects::method_name(cb::compat::from_py(name)));
    } catch (const cb::translate::backends::java::Refused& r) {
        throw codebreaker::translate::backends::java::Refused(r.what());
    }
}

}  // namespace codebreaker::translate::backends::java_objects
