// codebreaker/java/projects/minecraft/keybind_mark.hpp -- C++ port of codebreaker.java.projects.minecraft.keybind_mark.
//
// keybind_mark.py -- find every HARDCODED keyboard key-press handler in the converted C++ and tag it with a
// greppable `// J2X-KEYBIND[KEY -> action]` marker + write KEYBIND_INDEX.md.
//
// Minecraft drives a lot of actions off physical keyboard keys (F1 hide-GUI, F2 screenshot, F3 debug overlay,
// F5 third-person, F8 cinematic camera, F11 fullscreen, ESC pause, number keys = hotbar, shift = sneak...).
// There is NO keyboard on the console, so every `Keyboard::getEventKey() == KEY_x` / `Keyboard::isKeyDown(
// KEY_x)` site is DEAD -- the action is unreachable until it is re-routed. This tool catalogues them all so
// each can be deliberately turned into EITHER a controller button press OR a menu (GameSettings) toggle -- the
// user's choice per key -- instead of hand-wiring them one at a time (e.g. the Show-FPS -> debug-panel merge).
//
// It does NOT rewrite the branch logic (the route decision is deferred): it only annotates + indexes, exactly
// like ui_mark. Idempotent -- a line already carrying a J2X-KEYBIND marker is left alone. Runs automatically at
// the end of every xbox360 conversion, and is also exposed as `codebreaker java keybind-mark`.
#pragma once

#include "py.hpp"

namespace codebreaker::java::projects::minecraft::keybind_mark {

// Hardcoded, non-rebindable keys and what they do in Beta. (The rebindable movement/inventory/hotbar binds
// go through GameSettings.keyBindings + the controller config already; these function keys do not.)
extern const py::dict<py::str, py::str> KEY_ACTIONS;

// `Keyboard::getEventKey() == Keyboard::KEY_x`  OR  `Keyboard::isKeyDown(Keyboard::KEY_x)`
extern const py::re::Pattern _KEY_SITE;
extern const py::str _MARKER;

// One key-press site: (file name, line number, the keys on the line, the label written into the marker)
using Site = std::tuple<py::str, int64_t, py::list<py::str>, py::str>;

// The dict mark() returns ({'sites': [...], 'skipped': int}); run() adds 'index' (the path written, or None).
struct Result {
    py::list<Site> sites;
    int64_t skipped = 0;
    std::optional<py::str> index;
};

// Every KEY_x referenced on a line (a line may test two, e.g. F3+S).
py::list<py::str> _keys_on_line(const py::str& line);

py::str _action(const py::str& key);

// Append `// J2X-KEYBIND[KEY_x -> action | ROUTE: controller button OR menu toggle]` to each keyboard
// key-press line. Returns {'sites': [(file,line,keys,action)], 'skipped': int}.
Result mark(const py::str& src_dir, bool dry_run = false);

py::str render_index(const Result& result);

// Mark the tree (unless dry_run) and write KEYBIND_INDEX.md. Returns the result dict + index path.
Result run(const py::str& src_dir, bool dry_run = false, const std::optional<py::str>& index_dir = std::nullopt);

}  // namespace codebreaker::java::projects::minecraft::keybind_mark
