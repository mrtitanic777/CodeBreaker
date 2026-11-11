// cb/java/projects/minecraft/controller_nav.hpp -- inject console CONTROLLER MENU NAVIGATION into every converted
// Minecraft port: the GuiScreen adapter, the per-frame call, grab/cursor mode, the Pointer.png cursor, and
// console-safe MouseHelper grab/ungrab.
//
// THE BUG THIS ENDS (permanently): on the console the mouse is a virtual cursor driven by the pad. Menu screens
// (GuiMainMenu / GuiOptions / GuiNewLevel / ...) are lists of GuiButtons; without extra glue the D-pad does
// NOTHING (the cursor is frozen at screen-centre over the first button) -- the menu is unusable. The proven
// hand-ports (MinecraftAlpha) fixed this with a per-project overlay: a GuiScreen method that reads the button
// rectangles, moves a selection with the D-pad, and snaps the virtual cursor onto it, so the game's OWN
// hover-highlight + A-as-click work unchanged. But it was a hand overlay -- a fresh convert (Indev, or a
// regenerated Alpha) shipped WITHOUT it and the whole UI locked up. This pass migrates that glue into the
// GENERATOR, so EVERY edition gets working menu nav from one place and it can never be forgotten again.
//
// DIVISION OF LABOUR (correct by construction):
//   * The ALGORITHM (2-D spatial nav + cursor snap) lives ONCE in the reference: Input::menuNavSelect /
//     Input::menuSnapCursor (Input.reference.cpp) -- pure geometry, no game types, edition-agnostic.
//   * This pass injects a THIN adapter into the converted GuiScreen that feeds it the live button geometry. The
//     adapter uses only fields that are UNIFORM across every edition (GuiButton xPosition/yPosition/width/height/
//     enabled; GuiScreen controlList/width/height; Minecraft displayWidth/displayHeight), and the
//     `(GuiButton*)controlList[i]` cast the game already uses -- so ONE injected body compiles for all.
//
// WHAT IT INJECTS (keyed on the GuiScreen structure, not names/signatures -> covers all editions):
//   1. GuiScreen.h  : `int navSelected;` field + `void handleControllerMenuNav();` declaration (after controlList).
//   2. GuiScreen.cpp: ctor member-init `, navSelected(-1)`; `#include "Input.h"/"GuiButton.h"`; the adapter body.
//   3. Minecraft.cpp: a per-frame `currentScreen->handleControllerMenuNav();` right before the existing
//      `currentScreen->updateScreen();` (the once-per-frame screen tick present in every edition).
//
// Container GUIs (inventory/chest) that need a FREE analog cursor + Pointer icon are handled in displayGuiScreen
// (g_guiCursorMode) and EntityRenderer (the Pointer.png draw); button menus get discrete nav, a no-op on a screen
// with no buttons, so it is safe on every screen.
//
// Runs as a post-emit pass over the converted tree (xbox360), BEFORE overlays (so a project overlay can still
// win). Idempotent: each edit is skipped if its marker is already present. Fail-loud: run() reports which anchors
// it could NOT find, so a structural drift surfaces instead of a silently dead UI. Also exposed as
// `codebreaker java controller-nav <dir>`.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::projects::minecraft::controller_nav {

// The text after an edit, and whether the anchor it needs was found (or the edit was already there). When `ok`
// is false the text is unchanged.
struct Patched {
    std::string text;
    bool ok = false;
};

// Add `int navSelected;` + the method declaration after the `controlList` field.
Patched patch_guiscreen_h(std::string_view text);

// Add the constructor init, the two includes, and the adapter body.
Patched patch_guiscreen_cpp(std::string_view text);

// Inject `<recv>->handleControllerMenuNav();` before the per-frame `<recv>->updateScreen();`.
Patched patch_minecraft_callsite(std::string_view text);

// In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure function of
// game state: grab the mouse iff no screen is up AND a world exists (var == NULL && theWorld != NULL); editions
// with container GUIs also set g_guiCursorMode so the left stick drives a free analog cursor there.
Patched patch_minecraft_displayguiscreen(std::string_view text, bool has_container);

// Draw the Pointer.png crosshair cursor right after the current screen renders, ONLY in container mode. The GL
// namespace (`d3d9` or `GL11`) is detected from the text.
Patched patch_entityrenderer_pointer(std::string_view text);

// Replace MouseHelper::grabMouseCursor / ungrabMouseCursor with console-safe bodies (no AWT).
Patched patch_mousehelper_grab(std::string_view text);

// Which edits run() applied, and why it could not apply the others.
struct Result {
    bool guiscreen_h = false;
    bool guiscreen_cpp = false;
    bool callsite = false;
    bool focusmode = false;
    bool mousehelper = false;
    bool pointer = false;
    std::vector<std::string> missing;   // one reason per anchor not found (structural drift)
};

// Inject the controller menu-nav glue across the converted tree in `out_dir` (the files are named as the converter
// names them: GuiScreen.h, GuiScreen.cpp, Minecraft.cpp, MouseHelper.cpp, EntityRenderer.cpp).
Result run(const std::filesystem::path& out_dir);

}  // namespace cb::java::projects::minecraft::controller_nav
