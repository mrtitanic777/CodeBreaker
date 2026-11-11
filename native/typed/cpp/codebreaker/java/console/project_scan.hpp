// codebreaker/java/console/project_scan.hpp -- SHIM over cb/java/console/project_scan.hpp (docs/CPP_DESIGN.md 6.3);
// deleted with its last py:: consumer (run).
#pragma once

#include <optional>

#include "codebreaker/java/console/_shim.hpp"
#include "cb/java/console/project_scan.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::project_scan {

// What resolve() returns: the dict of possibly-None values {base, jar, resources, java_src, lang}.
struct Resolved {
    py::str base;
    std::optional<py::str> jar;
    std::optional<py::str> resources;
    std::optional<py::str> java_src;
    std::optional<py::str> lang;
};
template <class F> void for_each_field(const Resolved& r, F&& f) {
    f("base", r.base);
    f("jar", r.jar);
    f("resources", r.resources);
    f("java_src", r.java_src);
    f("lang", r.lang);
}

inline py::str find_base(const py::str& project_dir) {
    return shim::native(cb::java::console::project_scan::find_base(shim::path_of(project_dir)));
}

inline std::optional<py::str> find_jar(const py::str& base) {
    return shim::native(cb::java::console::project_scan::find_jar(shim::path_of(base)));
}

inline std::optional<py::str> find_resources(const py::str& base) {
    return shim::native(cb::java::console::project_scan::find_resources(shim::path_of(base)));
}

inline std::optional<py::str> find_java_src(const py::str& project_dir, const py::str& base) {
    return shim::native(cb::java::console::project_scan::find_java_src(shim::path_of(project_dir), shim::path_of(base)));
}

inline std::optional<py::str> find_lang(const py::str& base) {
    return shim::native(cb::java::console::project_scan::find_lang(shim::path_of(base)));
}

inline Resolved resolve(const py::str& project_dir) {
    const auto r = cb::java::console::project_scan::resolve(shim::path_of(project_dir));
    Resolved out;
    out.base = shim::native(r.base);
    out.jar = shim::native(r.jar);
    out.resources = shim::native(r.resources);
    out.java_src = shim::native(r.java_src);
    out.lang = shim::native(r.lang);
    return out;
}

}  // namespace codebreaker::java::console::project_scan
