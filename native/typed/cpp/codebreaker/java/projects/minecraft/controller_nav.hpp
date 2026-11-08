// codebreaker/java/projects/minecraft/controller_nav.hpp -- C++ port of
// codebreaker.java.projects.minecraft.controller_nav: inject console CONTROLLER MENU NAVIGATION into every
// converted Minecraft port (the GuiScreen adapter, the per-frame call, grab/cursor mode, the Pointer.png
// cursor, console-safe MouseHelper grab/ungrab).
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::controller_nav {

// The injected adapter body. UNIFORM across editions: only GuiButton geometry fields + the controlList cast
// the game itself uses. Calls the edition-agnostic core in Input.reference (Input::menuNavSelect/SnapCursor).
extern const py::str _NAV_METHOD;

extern const py::re::Pattern _CONTROLLIST_FIELD;
extern const py::re::Pattern _CTOR_INITLIST;
extern const py::re::Pattern _FIRST_INCLUDE;
extern const py::re::Pattern _UPDATESCREEN_CALL;
// CONTAINER cursor mode: displayGuiScreen sets g_guiCursorMode true for container GUIs (inventory/chest/...)
// so the left stick drives a FREE analog cursor there (vs discrete button nav on menus). var1 = the new screen.
extern const py::re::Pattern _CURRENTSCREEN_ASSIGN;
// EntityRenderer draws the current screen; the Pointer.png cursor is drawn right after, in container mode.
extern const py::re::Pattern _DRAWSCREEN_CALL;
extern const py::re::Pattern _GRAB_FN;
extern const py::re::Pattern _UNGRAB_FN;
extern const py::str _GRAB_SAFE;
extern const py::str _UNGRAB_SAFE;

// Every patch_* returns (new text, ok).
using Patched = std::tuple<py::str, bool>;

// Add `int navSelected;` + the method decl after the controlList field.
Patched patch_guiscreen_h(const py::str& text);

// Add the ctor init, the two includes, and the adapter body.
Patched patch_guiscreen_cpp(const py::str& text);

// Inject `<recv>->handleControllerMenuNav();` before the per-frame `<recv>->updateScreen();`.
Patched patch_minecraft_callsite(const py::str& text);

// In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure
// function of game state (grab the mouse iff no screen is up AND a world exists; container editions set
// g_guiCursorMode for the free analog cursor).
Patched patch_minecraft_displayguiscreen(const py::str& text, bool has_container);

// Draw the Pointer.png crosshair cursor right after the current screen renders, ONLY in container mode.
// GL namespace auto-detected.
Patched patch_entityrenderer_pointer(const py::str& text);

// Replace MouseHelper::grab/ungrabMouseCursor with console-safe bodies.
Patched patch_mousehelper_grab(const py::str& text);

// The dict run() returns: which edits applied, and `missing` (fail-loud reasons).
struct Result {
    bool guiscreen_h = false;
    bool guiscreen_cpp = false;
    bool callsite = false;
    bool focusmode = false;
    bool mousehelper = false;
    bool pointer = false;
    py::list<py::str> missing;
};

// Inject the controller menu-nav glue across the converted tree. `missing` is fail-loud: a required anchor
// was not found (structural drift) so the UI would ship dead.
Result run(const py::str& out_dir);

}  // namespace codebreaker::java::projects::minecraft::controller_nav
