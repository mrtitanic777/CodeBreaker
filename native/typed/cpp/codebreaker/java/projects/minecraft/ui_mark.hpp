// codebreaker/java/projects/minecraft/ui_mark.hpp -- C++ port of codebreaker.java.projects.minecraft.ui_mark.
//
// ui_mark.py -- tag every UI function with a greppable // J2X-UI[Class::method] marker + catalogue.
//
// After a UI merge (a new UI layered over the converted menus), you need to find EVERY UI function fast
// -- which screens/menus/buttons exist, where each method lives, what got layered where. This tool scans
// the converted C++, identifies the UI classes (Gui*/*Screen/*Menu + anything extending a GUI base), and
// inserts `// J2X-UI[Class::method]` at each UI method definition, so `grep -rn "J2X-UI" .` enumerates
// the entire UI surface. It also writes UI_INDEX.md: every UI class -> its methods with file:line.
//
// Idempotent: a definition already carrying a J2X-UI marker is left alone, so you can re-run after each
// merge without doubling markers. `--dry-run` reports what it WOULD mark without editing.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::ui_mark {

// UI base classes: a class extending one of these is a UI class even if its name doesn't match.
extern const py::set<py::str> UI_BASES;

extern const py::re::Pattern _CLASS_DECL;
extern const py::re::Pattern _EXTENDS;

// One marked definition: (file name, line number, method name)
using Mark = std::tuple<py::str, int64_t, py::str>;

// The dict mark() returns ({'classes': set, 'marked': {class: [(file,line,method)]}, 'skipped': int});
// run() adds 'index' (the path written, or None).
struct Result {
    py::set<py::str> classes;
    py::dict<py::str, py::list<Mark>> marked;
    int64_t skipped = 0;
    std::optional<py::str> index;
};

// Name-based heuristic: Minecraft UI classes are Gui* or end in Screen/Menu.
bool is_ui_class_name(const py::str& name);

// Return the set of UI class names: name-pattern matches PLUS classes that extend a UI base
// (transitively within this tree). Scans .h files for `class X : public Base`.
py::set<py::str> discover_ui_classes(const py::str& src_dir, const std::optional<py::list<py::str>>& extra = std::nullopt);

// a method DEFINITION line: `RetType Class::method(...)` (not a call: `Class::` sits after a return
// type / at statement start, and the line is not a `;`-terminated prototype).
py::re::Pattern _def_regex(const py::str& cls);

// Insert `// J2X-UI[Class::method]` at each UI method definition.
Result mark(const py::str& src_dir, bool dry_run = false, const std::optional<py::list<py::str>>& extra_classes = std::nullopt);

py::str render_index(const Result& result);

// Mark the tree (unless dry_run) and write UI_INDEX.md. Returns the result dict + index path.
Result run(const py::str& src_dir, bool dry_run = false, const std::optional<py::list<py::str>>& extra_classes = std::nullopt,
           const std::optional<py::str>& index_dir = std::nullopt);

}  // namespace codebreaker::java::projects::minecraft::ui_mark
