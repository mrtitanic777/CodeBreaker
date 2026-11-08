// codebreaker/java/projects/minecraft/controller_nav.cpp -- C++ port of codebreaker.java.projects.minecraft.controller_nav
// (see the header).
//
// controller_nav.py -- inject console CONTROLLER MENU NAVIGATION into every converted Minecraft port.
//
// THE BUG THIS ENDS (permanently): on the console the mouse is a virtual cursor driven by the pad. Menu
// screens (GuiMainMenu / GuiOptions / GuiNewLevel / ...) are lists of GuiButtons; without extra glue the
// D-pad does NOTHING (the cursor is frozen at screen-center over the first button) -- the menu is unusable.
// The proven hand-ports (MinecraftAlpha) fixed this with a per-project overlay: a GuiScreen method that reads
// the button rectangles, moves a selection with the D-pad, and snaps the virtual cursor onto it so the game's
// OWN hover-highlight + A-as-click work unchanged. But it was a hand overlay -- a fresh convert (Indev, or a
// regenerated Alpha) shipped WITHOUT it and the whole UI locked up. This pass migrates that glue into the
// GENERATOR so EVERY edition gets working menu nav from one place and it can never be forgotten again.
//
// DIVISION OF LABOUR (correct-by-construction):
//   * The ALGORITHM (2D-spatial nav + cursor snap) lives ONCE in the reference: Input::menuNavSelect /
//     Input::menuSnapCursor (Input.reference.cpp) -- pure geometry, no game types, edition-agnostic.
//   * This pass injects a THIN adapter into the converted GuiScreen that feeds it the live button geometry.
//     The adapter uses only fields that are UNIFORM across every edition (GuiButton xPosition/yPosition/
//     width/height/enabled; GuiScreen controlList/width/height; Minecraft displayWidth/displayHeight), and the
//     `(GuiButton*)controlList[i]` cast the game already uses -- so ONE injected body compiles for all.
//
// WHAT IT INJECTS (keyed on the GuiScreen structure, not names/signatures -> covers all editions):
//   1. GuiScreen.h  : `int navSelected;` field + `void handleControllerMenuNav();` decl (after controlList).
//   2. GuiScreen.cpp: ctor member-init `, navSelected(-1)`; `#include "Input.h"/"GuiButton.h"`; the adapter body.
//   3. Minecraft.cpp: a per-frame `currentScreen->handleControllerMenuNav();` right before the existing
//      `currentScreen->updateScreen();` (the once-per-frame screen tick present in every edition).
//
// Container GUIs (inventory/chest) that need a FREE analog cursor + Pointer icon are a SEPARATE feature
// (g_guiCursorMode + EntityRenderer) intentionally NOT done here -- button menus are the fix. Discrete nav is
// a no-op on a screen with no buttons, so this is safe on every screen.
//
// Runs as a post-emit pass over the converted tree (xbox360), BEFORE overlays (so a project overlay can still
// win). Idempotent: each edit is skipped if `handleControllerMenuNav` is already present. Fail-loud: run()
// reports which anchors it could NOT find so a structural drift surfaces instead of a silently dead UI. Also
// exposed as `codebreaker java controller-nav <dir>`.

#include "codebreaker/java/projects/minecraft/controller_nav.hpp"

#include <functional>
#include <optional>

namespace codebreaker::java::projects::minecraft::controller_nav {

const py::str _NAV_METHOD = py::str(
    "\n"
    "// CONSOLE (controller menu nav) -- CodeBreaker-injected (controller_nav.py). Feeds this screen's button\n"
    "// rectangles to the edition-agnostic Input::menuNavSelect core (Input.reference) and snaps the virtual\n"
    "// cursor onto the D-pad selection, so the game's OWN hover-highlight + A-as-click land on it. Migrated\n"
    "// from the per-project hand-port overlay so every edition gets working menu nav from one place.\n"
    "void GuiScreen::handleControllerMenuNav() {\n"
    "\tint n = (int)this->controlList.size();\n"
    "\tif (n <= 0) { this->navSelected = -1; return; }\n"
    "\tif (n > 128) n = 128;\n"
    "\tint bx[128], by[128], bw[128], bh[128]; unsigned char en[128];\n"
    "\tfor (int i = 0; i < n; ++i) {\n"
    "\t\tGuiButton* b = (GuiButton*)this->controlList[i];\n"
    "\t\tbx[i] = b ? b->xPosition : 0;  by[i] = b ? b->yPosition : 0;\n"
    "\t\tbw[i] = b ? b->width : 0;      bh[i] = b ? b->height : 0;\n"
    "\t\ten[i] = (b && b->enabled) ? 1 : 0;\n"
    "\t}\n"
    "\tthis->navSelected = Input::menuNavSelect(bx, by, bw, bh, en, n, this->navSelected);\n"
    "\tif (this->navSelected >= 0 && this->navSelected < n && this->mc != NULL) {\n"
    "\t\tGuiButton* b = (GuiButton*)this->controlList[this->navSelected];\n"
    "\t\tif (b != NULL) Input::menuSnapCursor(b->xPosition + b->width / 2, b->yPosition + b->height / 2,\n"
    "\t\t\tthis->width, this->height, this->mc->displayWidth, this->mc->displayHeight);\n"
    "\t}\n"
    "}\n");

const py::re::Pattern _CONTROLLIST_FIELD = py::re::compile(py::str(R"rx(\n([ \t]*)JList<[^;\n]*>\s+controlList\s*;)rx"));
const py::re::Pattern _CTOR_INITLIST = py::re::compile(py::str(R"rx((GuiScreen::GuiScreen\(\)\s*:[^{}]*?)(\n\s*\{))rx"));
const py::re::Pattern _FIRST_INCLUDE = py::re::compile(py::str(R"rx((#include [^\n]*\n))rx"));
const py::re::Pattern _UPDATESCREEN_CALL = py::re::compile(py::str(R"rx((\n[ \t]*)([A-Za-z_][\w>.\-]*)->updateScreen\(\);)rx"));
const py::re::Pattern _CURRENTSCREEN_ASSIGN = py::re::compile(py::str(R"rx((this->currentScreen = \(GuiScreen\s*\*\)(\w+);))rx"));
const py::re::Pattern _DRAWSCREEN_CALL = py::re::compile(py::str(R"rx((\n[ \t]*)(this->mc->currentScreen->drawScreen\([^;]*\);))rx"));

// MouseHelper::grab/ungrabMouseCursor -- the whole function up to its column-0 closing brace (inner try/catch
// and if/else braces are indented, so `\n}` matches only the function end).
const py::re::Pattern _GRAB_FN = py::re::compile(py::str(R"rx(void MouseHelper::grabMouseCursor\(\)\s*\{[\s\S]*?\n\})rx"));
const py::re::Pattern _UNGRAB_FN = py::re::compile(py::str(R"rx(void MouseHelper::ungrabMouseCursor\(\)\s*\{[\s\S]*?\n\})rx"));
const py::str _GRAB_SAFE = py::str(
    "void MouseHelper::grabMouseCursor()\n{\n\tMouse::setGrabbed(true);"
    "   // CONSOLE grab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
    "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}");
const py::str _UNGRAB_SAFE = py::str(
    "void MouseHelper::ungrabMouseCursor()\n{\n\tMouse::setGrabbed(false);"
    "   // CONSOLE ungrab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
    "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}");

Patched patch_guiscreen_h(const py::str& text) {
    if (text.contains(py::str("handleControllerMenuNav"))) {
        return {text, true};
    }
    std::optional<py::re::Match> m = _CONTROLLIST_FIELD.search(text);
    if (!m) {
        return {text, false};
    }
    py::str ind = m->group(1);
    py::str add = py::percent(
        py::str("\n%s// CONSOLE (controller menu nav): D-pad-selected button index (-1 = none yet). CodeBreaker-injected."
                "\n%sint navSelected;"
                "\n%svoid handleControllerMenuNav();"),
        ind, ind, ind);
    return {text.slice(0, m->end()) + add + text.slice(m->end(), text.size()), true};
}

Patched patch_guiscreen_cpp(const py::str& text) {
    if (text.contains(py::str("handleControllerMenuNav"))) {
        return {text, true};
    }
    auto [new_text, nsub] = _CTOR_INITLIST.subn(
        [](const py::re::Match& m) { return m.group(1) + py::str("\n\t, navSelected(-1)") + m.group(2); }, text, 1);
    py::str result = new_text;
    if (nsub == 0) {
        return {text, false};                      // no member-init-list ctor found -> fail loud
    }
    for (const char* inc : {"#include \"Input.h\"", "#include \"GuiButton.h\""}) {
        const py::str i = py::str(inc);
        if (!result.contains(i)) {
            result = std::get<0>(_FIRST_INCLUDE.subn(
                [&i](const py::re::Match& mm) { return mm.group(1) + i + py::str("\n"); }, result, 1));
        }
    }
    result = result.rstrip(py::str("\n")) + py::str("\n") + _NAV_METHOD;
    return {result, true};
}

Patched patch_minecraft_callsite(const py::str& text) {
    if (text.contains(py::str("handleControllerMenuNav"))) {
        return {text, true};
    }
    auto rep = [](const py::re::Match& m) {
        return py::percent(py::str("%s%s->handleControllerMenuNav();%s%s->updateScreen();"),
                           m.group(1), m.group(2), m.group(1), m.group(2));
    };
    auto [new_text, nsub] = _UPDATESCREEN_CALL.subn(rep, text, 1);
    if (nsub != 0) {
        return {new_text, true};
    }
    return {text, false};
}

Patched patch_minecraft_displayguiscreen(const py::str& text, bool has_container) {
    // In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure
    // function of game state -- the single source of truth that ties nav / A-select / B-back / pause / in-game
    // together instead of per-button hotfixes:
    //
    //   * GRAB the mouse (in-game pad mode: sticks=move/look, face=jump/etc) IFF no screen is up AND a world
    //     exists (var1 == NULL && theWorld != NULL). ANY screen up, or no world (main menu), stays UNGRABBED so
    //     the pad drives GUI nav. This self-corrects every path: e.g. B->keyTyped(ESCAPE)->displayGuiScreen(NULL)
    //     re-creates the main menu (var1 becomes non-NULL) so it ungrabs, and the game's own setIngameFocus()
    //     (which re-enters displayGuiScreen) can't leave the menu grabbed. Fixes "B doesn't go back" and every
    //     menu-nav-goes-dead-after-a-transition case at once.
    //   * (container editions) set g_guiCursorMode for the free analog cursor in inventory/chest GUIs.
    //
    // `var1` here is the FINAL screen (after the null->GuiMainMenu / GuiGameOver fixups run above), so the test
    // is exact. theWorld + mouseHelper are standard Minecraft fields present in every edition.
    if (text.contains(py::str("CONSOLE: pad grabbed"))) {                     // idempotent marker (survives with/without container)
        return {text, true};
    }

    auto rep = [has_container](const py::re::Match& m) {
        py::str var = m.group(2);
        py::str block = m.group(1);
        if (has_container) {
            block += py::percent(
                py::str("\n\t{ extern bool g_guiCursorMode; g_guiCursorMode = (%s != NULL && "
                        "dynamic_cast<GuiContainer*>(%s) != NULL); }"
                        "   // CONSOLE: analog cursor in container GUIs (CodeBreaker-injected)"),
                var, var);
        }
        block += py::percent(
            py::str("\n\tMouse::setGrabbed(%s == NULL && this->theWorld != NULL);"
                    "   // CONSOLE: pad grabbed (in-game mode) IFF no screen up AND a world exists -- any menu"
                    " -> GUI nav mode. Sets g_grabbed DIRECTLY (NOT MouseHelper::grab/ungrab, which use AWT"
                    " windowComponent/robot -- NULL/broken on console -- and never call setGrabbed anyway, which"
                    " is why g_grabbed was stuck false). Single source of truth for controller input mode."),
            var);
        return block;
    };

    auto [new_text, nsub] = _CURRENTSCREEN_ASSIGN.subn(rep, text, 1);
    if (nsub != 0) {
        return {new_text, true};
    }
    return {text, false};
}

Patched patch_entityrenderer_pointer(const py::str& text) {
    if (text.contains(py::str("Pointer.png"))) {
        return {text, true};
    }
    py::str ns = text.contains(py::str("d3d9::gl")) ? py::str("d3d9") : py::str("GL11");
    py::list<py::str> _body = {
        py::str("// CONSOLE (controller): draw the virtual cursor (Pointer.png) ONLY in container GUIs. CodeBreaker-injected."),
        py::str("{ extern bool g_guiCursorMode;"),
        py::str("if (g_guiCursorMode && this->mc->currentScreen != NULL) {"),
        py::str("\tint _gw = this->mc->currentScreen->width, _gh = this->mc->currentScreen->height;"),
        py::str("\tint _mx = _gw > 0 ? Mouse::getX() * _gw / this->mc->displayWidth : 0;"),
        py::str("\tint _my = _gh > 0 ? _gh - Mouse::getY() * _gh / this->mc->displayHeight - 1 : 0;"),
        py::str("\t@NS@::glBindTexture(@NS@::GL_TEXTURE_2D, this->mc->renderEngine->getTexture(\"/gui/Pointer.png\"));"),
        py::str("\t@NS@::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);"),
        py::str("\t@NS@::glEnable(@NS@::GL_BLEND);"),
        py::str("\t@NS@::glBlendFunc(@NS@::GL_SRC_ALPHA, @NS@::GL_ONE_MINUS_SRC_ALPHA);"),
        py::str("\t@NS@::glDisable(@NS@::GL_DEPTH_TEST);"),
        py::str("\tTessellator* _t = Tessellator::s_instance;"),
        py::str("\tint _cs = 16, _px = _mx - _cs / 2, _py = _my - _cs / 2;"),
        py::str("\t_t->startDrawingQuads();"),
        py::str("\t_t->addVertexWithUV((float)_px, (float)(_py + _cs), 0.0F, 0.0F, 1.0F);"),
        py::str("\t_t->addVertexWithUV((float)(_px + _cs), (float)(_py + _cs), 0.0F, 1.0F, 1.0F);"),
        py::str("\t_t->addVertexWithUV((float)(_px + _cs), (float)_py, 0.0F, 1.0F, 0.0F);"),
        py::str("\t_t->addVertexWithUV((float)_px, (float)_py, 0.0F, 0.0F, 0.0F);"),
        py::str("\t_t->draw();"),
        py::str("\t@NS@::glEnable(@NS@::GL_DEPTH_TEST);"),
        py::str("}"),
        py::str("}"),
    };

    auto rep = [&_body, &ns](const py::re::Match& m) {
        py::str ind = m.group(1);                                  // leading newline+indent of the drawScreen line
        py::str block;
        for (const py::str& line : _body) {
            block += ind + line;
        }
        block = block.replace(py::str("@NS@"), ns);
        return m.group(1) + m.group(2) + block;
    };

    auto [new_text, nsub] = _DRAWSCREEN_CALL.subn(rep, text, 1);
    if (nsub != 0) {
        return {new_text, true};
    }
    return {text, false};
}

Patched patch_mousehelper_grab(const py::str& text) {
    // Replace MouseHelper::grab/ungrabMouseCursor with console-safe bodies (just Mouse::setGrabbed + reset the
    // look deltas). The Java versions deref windowComponent / MouseInfo / robot -- NULL/stub on console -- so any
    // caller (setIngameFocus, keyTyped(ESCAPE)) faults; that's what black-screened the boot the moment the grab
    // path was exercised. The pad-mode TRUTH is the displayGuiScreen invariant; these just have to be safe and
    // consistent. MouseHelper.cpp already uses Mouse:: (setNativeCursor) so the namespace is in scope.
    if (text.contains(py::str("CONSOLE grab (CodeBreaker)")) || text.contains(py::str("CONSOLE grab (j2ctool)"))) {   // trees converted before the rename
        return {text, true};
    }
    auto [new_text, n1] = _GRAB_FN.subn([](const py::re::Match&) { return _GRAB_SAFE; }, text, 1);
    auto [new_text2, n2] = _UNGRAB_FN.subn([](const py::re::Match&) { return _UNGRAB_SAFE; }, new_text, 1);
    return {new_text2, n1 == 1 && n2 == 1};
}

Result run(const py::str& out_dir) {
    Result res;

    // Read + patch in memory. Returns (path, old, new) or nullopt (reason recorded in `missing`).
    auto _stage = [&res, &out_dir](const py::str& fname, const std::function<Patched(const py::str&)>& fn,
                                   const py::str& why) -> std::optional<std::tuple<py::str, py::str, py::str>> {
        py::str path = py::os::path::join(out_dir, fname);
        if (!py::os::path::isfile(path)) {
            res.missing.append(py::percent(py::str("%s (%s: file absent)"), fname, why));
            return std::nullopt;
        }
        py::str text;
        try {
            text = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
        } catch (const py::OSError&) {                 // (IOError, OSError)
            res.missing.append(py::percent(py::str("%s (%s: unreadable)"), fname, why));
            return std::nullopt;
        }
        auto [new_text, ok] = fn(text);
        if (!ok) {
            res.missing.append(py::percent(py::str("%s (%s: anchor not found)"), fname, why));
            return std::nullopt;
        }
        return std::make_tuple(path, text, new_text);
    };

    auto _commit = [](const std::tuple<py::str, py::str, py::str>& staged) {
        const py::str& path = std::get<0>(staged);
        const py::str& text = std::get<1>(staged);
        const py::str& new_text = std::get<2>(staged);
        if (new_text != text) {
            py::file fh = py::open(path, py::str("w"), py::str("utf-8"));
            fh.write(new_text);
            fh.close();
        }
    };

    auto _edit = [&](const py::str& fname, const std::function<Patched(const py::str&)>& fn, bool& key, const py::str& why) {
        auto staged = _stage(fname, fn, why);
        if (staged) {
            _commit(*staged);
            key = true;
        }
    };

    // The declaration (GuiScreen.h), definition (GuiScreen.cpp) and per-frame call (Minecraft.cpp) are ONE
    // unit: applying only some of them leaves a declared-but-undefined method (LNK2019) or a defined-but-
    // undeclared member (C2039). Stage all three in memory and write none unless every anchor matched.
    struct Group {
        const char* fname;
        std::function<Patched(const py::str&)> fn;
        bool* key;
        const char* why;
    };
    std::vector<Group> group = {{"GuiScreen.h", patch_guiscreen_h, &res.guiscreen_h, "field+decl"},
                                {"GuiScreen.cpp", patch_guiscreen_cpp, &res.guiscreen_cpp, "ctor+body"},
                                {"Minecraft.cpp", patch_minecraft_callsite, &res.callsite, "per-frame call"}};
    std::vector<std::optional<std::tuple<py::str, py::str, py::str>>> staged;
    for (const Group& g : group) {
        staged.push_back(_stage(py::str(g.fname), g.fn, py::str(g.why)));
    }
    bool all_staged = true;
    for (const auto& st : staged) {
        if (!st) {
            all_staged = false;
        }
    }
    if (all_staged) {
        for (size_t i = 0; i < group.size(); ++i) {
            _commit(*staged[i]);
            *group[i].key = true;
        }
    } else {
        res.missing.append(py::str("menu-nav glue NOT applied to any file (all-or-nothing: "
                                   "GuiScreen.h/GuiScreen.cpp/Minecraft.cpp must all match)"));
    }

    // displayGuiScreen: the SINGLE source of truth for controller input mode (grab/ungrab) -- always applied,
    // this is what makes nav / A / B-back / pause / in-game coherent. The g_guiCursorMode analog-cursor half is
    // added only when the edition HAS GuiContainer (inventory/chest); its Pointer.png draw goes in EntityRenderer.
    bool has_container = py::os::path::isfile(py::os::path::join(out_dir, py::str("GuiContainer.h")));
    _edit(py::str("Minecraft.cpp"),
          [has_container](const py::str& t) { return patch_minecraft_displayguiscreen(t, has_container); },
          res.focusmode, py::str("grab/cursor mode"));
    // MouseHelper grab/ungrab must be console-safe (no AWT) or setIngameFocus/keyTyped(ESCAPE) fault.
    _edit(py::str("MouseHelper.cpp"), patch_mousehelper_grab, res.mousehelper, py::str("console-safe grab/ungrab"));
    if (has_container) {
        _edit(py::str("EntityRenderer.cpp"), patch_entityrenderer_pointer, res.pointer, py::str("Pointer.png cursor"));
    }
    return res;
}

}  // namespace codebreaker::java::projects::minecraft::controller_nav
