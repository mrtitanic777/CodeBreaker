// codebreaker/java/console/project_layout.hpp -- typed C++ port of codebreaker.java.console.project_layout.
//
// project_layout.py -- give a converted tree the standard IDE solution layout.
//
// The converter writes source files flat into a directory. A Visual Studio solution wants a
// *solution root* that holds a `<Name>.sln`, with the actual project (sources + `.vcxproj`) one level
// down in a same-named subfolder:
//
//     <root>/<Name>.sln
//     <root>/<Name>/<Name>.vcxproj   + all .cpp/.h + Content\ + ...
//
// That is exactly how the reference Alpha port is arranged. Without it, pointing the converter at the
// repo root dumps hundreds of files flat there -- and pointing it a second time one level down creates a
// duplicate copy (the "duplicate directories" bug). This module makes the layout deterministic:
//
//     layout(root)                    // root basename is the project name
//       -> ensures <root>/<Name>/ holds the sources (moves a flat dump down if needed)
//       -> writes <Name>.vcxproj from a template if absent, then refreshes its file list
//       -> (re)writes <root>/<Name>.sln bound to the project's GUID
//
// Idempotent: running it again on an already-structured root is a no-op beyond refreshing the file list.
// The .sln/.vcxproj are Visual-Studio project *configuration* (build settings, not SDK headers/symbols);
// the platform SDK gate is unchanged.
//
// Notes on the port:
//   * HERE (the folder of project_layout.py, from __file__): C++ has no __file__, so HERE is a variable
//     that starts as the checkout's codebreaker/java/console (the Python package folder the template sits
//     in, found from where this .cpp lives); the host sets it with set_here(), which also moves
//     _TEMPLATE (= HERE/templates/xbox360.vcxproj.in);
//   * layout() returns the dict of fixed keys as the struct Layout;
//   * `from codebreaker.java.console import vcxproj; vcxproj.run(proj_dir, name=name)` is the typed
//     port's vcxproj::run (java/console/vcxproj.hpp).
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::project_layout {

extern py::str HERE;
extern py::str _TEMPLATE;
// Point HERE (and so _TEMPLATE) at the folder project_layout.py lives in.
void set_here(const py::str& here);

// things that stay at the solution root (never moved down into the project subfolder)
extern const std::tuple<py::str, py::str, py::str> _ROOT_KEEP_EXT;

extern const py::str _SLN;
extern const py::list<py::str> _CONFIGS;

// A stable per-name GUID (so re-running yields the same .sln/.vcxproj binding).
py::str project_guid(const py::str& name);

std::optional<py::str> _existing_guid(const py::str& vcx_path);

py::str _sln_text(const py::str& name, const py::str& guid);

py::str write_solution(const py::str& root, const py::str& name, const py::str& guid);

void _write_vcxproj_from_template(const py::str& vcx_path, const py::str& guid);

bool _has_flat_sources(const py::str& d);

// Move a flat dump at `root` down into `proj_dir` (everything except the solution files, the
// project subfolder itself, and the .sln). Returns count moved.
int64_t _move_flat_into(const py::str& root, const py::str& proj_dir, const py::str& name);

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

// Make `root` a solution root. `name` defaults to the root folder's basename. If sources are
// dumped flat in `root`, they are moved down into `root/<name>/`. Returns a summary.
Layout layout(const py::str& root, const std::optional<py::str>& name = std::nullopt, bool regen = true);

}  // namespace codebreaker::java::console::project_layout
