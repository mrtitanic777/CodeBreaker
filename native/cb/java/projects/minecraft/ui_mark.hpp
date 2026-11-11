// cb/java/projects/minecraft/ui_mark.hpp -- tag every UI function with a greppable `// J2X-UI[Class::method]`
// marker and write a catalogue.
//
// After a UI merge (a new UI layered over the converted menus), you need to find EVERY UI function fast -- which
// screens/menus/buttons exist, where each method lives, what got layered where. This tool scans the converted C++,
// identifies the UI classes (Gui*, *Screen, *Menu, and anything extending a GUI base), and inserts
// `// J2X-UI[Class::method]` at each UI method definition, so `grep -rn "J2X-UI" .` enumerates the whole UI
// surface. It also writes UI_INDEX.md: every UI class -> its methods with file:line.
//
// Idempotent: a definition already carrying a J2X-UI marker is left alone, so it can be re-run after each merge
// without doubling markers. A dry run reports what it WOULD mark without editing.
#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::projects::minecraft::ui_mark {

// Name-based heuristic: Minecraft UI classes are Gui* or end in Screen/Menu.
bool is_ui_class_name(std::string_view name);

// The UI class names of a tree: `extra` (classes the caller knows), the name-pattern matches, and every class that
// extends a UI base (Gui, GuiScreen, GuiButton, ...) or another UI class, transitively. Scans the `.h`/`.hpp` files
// for `class X : public Base {`.
std::set<std::string> discover_ui_classes(const std::filesystem::path& src_dir,
                                          const std::vector<std::string>& extra = {});

// One marked definition.
struct Mark {
    std::string file;      // the file's name
    std::size_t line = 0;  // 1-based
    std::string method;    // "drawScreen", "~GuiScreen"
    auto operator<=>(const Mark&) const = default;
};

struct Result {
    std::set<std::string> classes;                  // every UI class found
    std::map<std::string, std::vector<Mark>> marked;  // class -> its marked definitions (in file order)
    std::size_t skipped = 0;                        // definitions that already carried a marker
    std::optional<std::filesystem::path> index;     // the catalogue written (run() only, and not on a dry run)
};

// Insert `// J2X-UI[Class::method]` at each UI method definition of the `.cpp` files under `src_dir`: a line with
// `Class::method(` (not a call: the class name starts a token, and the line is not a `;`-terminated prototype) that
// opens a body within the next few lines. A dry run only reports.
Result mark(const std::filesystem::path& src_dir, bool dry_run = false,
            const std::vector<std::string>& extra_classes = {});

// The text of UI_INDEX.md.
std::string render_index(const Result& result);

// Mark the tree (unless dry_run) and write UI_INDEX.md into `index_dir` (default: `src_dir`).
// An `index_dir` that does not exist is created; a `src_dir` that does not exist, with no `index_dir`, is a NotFoundError
// (there is nowhere to write the index).
Result run(const std::filesystem::path& src_dir, bool dry_run = false, const std::vector<std::string>& extra_classes = {},
           const std::optional<std::filesystem::path>& index_dir = std::nullopt);

}  // namespace cb::java::projects::minecraft::ui_mark
