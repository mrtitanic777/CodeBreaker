// codebreaker/java/console/project_layout.hpp -- SHIM over cb/java/console/project_layout.hpp (docs/CPP_DESIGN.md 6.3);
// deleted with its last py:: consumer (run).
//
// HERE (the folder the Python module lives in) is where the template is looked for when the host sets it with
// set_here(); otherwise the Java lane's data folder is searched for (JavaData::locate).
#pragma once

#include <cstdint>
#include <optional>

#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "cb/java/console/project_layout.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::project_layout {

// The folder of the (Python) package's console module, where templates/xbox360.vcxproj.in sits.
inline py::str& HERE_() {
    static py::str here;
    return here;
}

// Point HERE at the folder project_layout.py lives in.
inline void set_here(const py::str& here) { HERE_() = here; }

// A stable per-name GUID (so re-running yields the same .sln/.vcxproj binding).
inline py::str project_guid(const py::str& name) {
    return cb::compat::to_py(cb::java::console::project_layout::project_guid(name.utf8()));
}

inline py::str write_solution(const py::str& root, const py::str& name, const py::str& guid) {
    return cb::compat::guard([&] {
        return shim::native(cb::java::console::project_layout::write_solution(shim::path_of(root), name.utf8(), guid.utf8()));
    });
}

// What layout() returns (the Python's dict with these keys).
struct Layout {
    py::str name;
    py::str root;
    py::str proj_dir;
    py::str sln;
    py::str vcxproj;
    py::str guid;
    int64_t moved = 0;
};

// Make `root` a solution root. `name` defaults to the root folder's basename. If sources are dumped flat in `root`,
// they are moved down into `root/<name>/`. Returns a summary.
inline Layout layout(const py::str& root, const std::optional<py::str>& name = std::nullopt, bool regen = true) {
    return cb::compat::guard([&] {
        namespace nw = cb::java::console::project_layout;
        nw::Options options;
        if (name && name->size() > 0) options.name = name->utf8();
        options.regenerate = regen;
        // the template: next to the console module when the host said where that is, else in the Java lane's data folder
        const auto template_file = HERE_().size() > 0 ? shim::path_of(HERE_()) / "templates" / "xbox360.vcxproj.in"
                                                      : nw::template_path(cb::java::JavaData::locate());
        const nw::Layout made = nw::layout(shim::path_of(root), template_file, vcxproj::active_rules_(), options);
        if (made.refreshed) py::print(cb::compat::to_py(made.refreshed->summary()));
        Layout out;
        out.name = cb::compat::to_py(made.name);
        out.root = shim::native(made.root);
        out.proj_dir = shim::native(made.proj_dir);
        out.sln = shim::native(made.sln);
        out.vcxproj = shim::native(made.project_file);
        out.guid = cb::compat::to_py(made.guid);
        out.moved = static_cast<int64_t>(made.moved);
        return out;
    });
}

}  // namespace codebreaker::java::console::project_layout
