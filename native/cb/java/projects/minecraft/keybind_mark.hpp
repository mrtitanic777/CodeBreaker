// cb/java/projects/minecraft/keybind_mark.hpp -- find every HARDCODED keyboard key-press handler in the converted
// C++ and tag it with a greppable `// J2X-KEYBIND[KEY -> action]` marker, then write KEYBIND_INDEX.md.
//
// Minecraft drives a lot of actions off physical keyboard keys (F1 hide-GUI, F2 screenshot, F3 debug overlay, F5
// third-person, F8 cinematic camera, F11 fullscreen, ESC pause, number keys = hotbar, shift = sneak...). There is
// NO keyboard on the console, so every `Keyboard::getEventKey() == KEY_x` / `Keyboard::isKeyDown(KEY_x)` site is
// DEAD -- the action is unreachable until it is re-routed. This tool catalogues them all so each can be
// deliberately turned into EITHER a controller button press OR a menu (GameSettings) toggle -- the user's choice
// per key -- instead of hand-wiring them one at a time (e.g. the Show-FPS -> debug-panel merge).
//
// It does NOT rewrite the branch logic (the route decision is deferred): it only annotates and indexes, exactly
// like ui_mark. Idempotent -- a line already carrying a J2X-KEYBIND marker is left alone. It runs at the end of
// every xbox360 conversion, and is also exposed as `codebreaker java keybind-mark`.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::projects::minecraft::keybind_mark {

// What a hardcoded, non-rebindable key does in Beta ("F1" -> "hide GUI / HUD"). The rebindable movement/inventory/
// hotbar binds go through GameSettings.keyBindings and the controller config already; these keys do not. A key
// that is not known gets a "(unknown action -- inspect the branch)" answer.
std::string_view action_of(std::string_view key);

// Every KEY_x referenced on a line, in order (a line may test two, e.g. F3+S). Matches
// `Keyboard::getEventKey() == Keyboard::KEY_x` and `Keyboard::isKeyDown(Keyboard::KEY_x)`.
std::vector<std::string> keys_on_line(std::string_view line);

// One key-press site.
struct Site {
    std::string file;               // the file's name (the converted tree is flat)
    std::size_t line = 0;           // 1-based
    std::vector<std::string> keys;  // the keys tested on the line
    std::string label;              // what goes in the marker: "KEY_F3=toggle debug overlay (F3 panel) KEY_S=..."
};

struct Result {
    std::vector<Site> sites;                    // in file order (each folder's files by name)
    std::size_t skipped = 0;                    // sites that already carried a marker
    std::optional<std::filesystem::path> index; // the index file written (run() only, and not on a dry run)
};

// Append `// J2X-KEYBIND[KEY_x=action | keyboard is DEAD on console -- ROUTE TO: controller button OR menu toggle]`
// to each keyboard key-press line of the `.cpp` files under `src_dir`. A dry run only reports.
Result mark(const std::filesystem::path& src_dir, bool dry_run = false);

// The text of KEYBIND_INDEX.md: one row per site, ordered by file name and line.
std::string render_index(const Result& result);

// Mark the tree (unless dry_run) and write KEYBIND_INDEX.md into `index_dir` (default: `src_dir`).
// An `index_dir` that does not exist is created; a `src_dir` that does not exist, with no `index_dir`, is a NotFoundError
// (there is nowhere to write the index).
Result run(const std::filesystem::path& src_dir, bool dry_run = false,
           const std::optional<std::filesystem::path>& index_dir = std::nullopt);

}  // namespace cb::java::projects::minecraft::keybind_mark
