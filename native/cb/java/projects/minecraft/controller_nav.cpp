#include "cb/java/projects/minecraft/controller_nav.hpp"

#include <array>
#include <functional>
#include <optional>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::projects::minecraft::controller_nav {

namespace stdfs = std::filesystem;

namespace {

// The injected adapter body (starts with a blank line). UNIFORM across editions: only GuiButton geometry fields
// and the controlList cast the game itself uses; it calls the edition-agnostic core in Input.reference
// (Input::menuNavSelect / menuSnapCursor).
constexpr std::string_view kNavMethod =
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
    "}\n";

// The GuiScreen field the declaration goes after: `JList<...> controlList;` on its own indented line.
const Regex& controllist_field() {
    static const Regex re(R"(\n([ \t]*)JList<[^;\n]*>\s+controlList\s*;)");
    return re;
}
// GuiScreen's constructor with a member-init list, up to the brace that opens its body.
const Regex& ctor_initlist() {
    static const Regex re(R"((GuiScreen::GuiScreen\(\)\s*:[^{}]*?)(\n\s*\{))");
    return re;
}
const Regex& first_include() {
    static const Regex re(R"((#include [^\n]*\n))");
    return re;
}
// The once-per-frame screen tick: `<receiver>->updateScreen();` on its own line.
const Regex& updatescreen_call() {
    static const Regex re(R"((\n[ \t]*)([A-Za-z_][\w>.-]*)->updateScreen\(\);)");
    return re;
}
// CONTAINER cursor mode: displayGuiScreen sets g_guiCursorMode true for container GUIs (inventory/chest/...), so
// the left stick drives a FREE analog cursor there (vs discrete button nav on menus). group 2 = the new screen.
const Regex& currentscreen_assign() {
    static const Regex re(R"((this->currentScreen = \(GuiScreen\s*\*\)(\w+);))");
    return re;
}
// EntityRenderer draws the current screen; the Pointer.png cursor is drawn right after it, in container mode.
const Regex& drawscreen_call() {
    static const Regex re(R"((\n[ \t]*)(this->mc->currentScreen->drawScreen\([^;]*\);))");
    return re;
}
// MouseHelper::grab/ungrabMouseCursor -- the whole function up to its column-0 closing brace (inner try/catch and
// if/else braces are indented, so `\n}` matches only the function end).
const Regex& grab_fn() {
    static const Regex re(R"(void MouseHelper::grabMouseCursor\(\)\s*\{.*?\n\})", RegexFlags::DotAll);
    return re;
}
const Regex& ungrab_fn() {
    static const Regex re(R"(void MouseHelper::ungrabMouseCursor\(\)\s*\{.*?\n\})", RegexFlags::DotAll);
    return re;
}

constexpr std::string_view kGrabSafe =
    "void MouseHelper::grabMouseCursor()\n{\n\tMouse::setGrabbed(true);"
    "   // CONSOLE grab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
    "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}";
constexpr std::string_view kUngrabSafe =
    "void MouseHelper::ungrabMouseCursor()\n{\n\tMouse::setGrabbed(false);"
    "   // CONSOLE ungrab (CodeBreaker): set the pad mode flag only -- NO AWT\n"
    "\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}";

bool has(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }

// Replace the first match with what `make` returns for it; nullopt if nothing matches.
std::optional<std::string> replace_first(const Regex& re, std::string_view text,
                                         const std::function<std::string(const RegexMatch&)>& make) {
    const std::optional<RegexMatch> m = re.search(text);
    if (!m) return std::nullopt;
    std::string out(text.substr(0, m->start()));
    out += make(*m);
    out.append(text.substr(m->end()));
    return out;
}

Patched unchanged(std::string_view text, bool ok) { return {std::string(text), ok}; }

}  // namespace

Patched patch_guiscreen_h(std::string_view text) {
    if (has(text, "handleControllerMenuNav")) return unchanged(text, true);
    const std::optional<RegexMatch> m = controllist_field().search(text);
    if (!m) return unchanged(text, false);
    const std::string ind(m->group(1));
    std::string out(text.substr(0, m->end()));
    out += "\n" + ind + "// CONSOLE (controller menu nav): D-pad-selected button index (-1 = none yet). CodeBreaker-injected.";
    out += "\n" + ind + "int navSelected;";
    out += "\n" + ind + "void handleControllerMenuNav();";
    out.append(text.substr(m->end()));
    return {std::move(out), true};
}

Patched patch_guiscreen_cpp(std::string_view text) {
    if (has(text, "handleControllerMenuNav")) return unchanged(text, true);
    std::optional<std::string> result = replace_first(ctor_initlist(), text, [](const RegexMatch& m) {
        return std::string(m[1]) + "\n\t, navSelected(-1)" + std::string(m[2]);
    });
    if (!result) return unchanged(text, false);   // no member-init-list ctor found -> fail loud
    for (const char* include : {"#include \"Input.h\"", "#include \"GuiButton.h\""}) {
        if (has(*result, include)) continue;
        if (auto with = replace_first(first_include(), *result,
                                      [&](const RegexMatch& m) { return std::string(m[1]) + include + "\n"; })) {
            result = std::move(with);
        }
    }
    // the body goes after the file's last line (only trailing newlines are dropped; the start is as it was)
    std::size_t end = result->size();
    while (end > 0 && (*result)[end - 1] == '\n') --end;
    std::string out = result->substr(0, end);
    out += "\n";
    out += kNavMethod;
    return {std::move(out), true};
}

Patched patch_minecraft_callsite(std::string_view text) {
    if (has(text, "handleControllerMenuNav")) return unchanged(text, true);
    std::optional<std::string> out = replace_first(updatescreen_call(), text, [](const RegexMatch& m) {
        return std::string(m[1]) + std::string(m[2]) + "->handleControllerMenuNav();" + std::string(m[1]) +
               std::string(m[2]) + "->updateScreen();";
    });
    if (!out) return unchanged(text, false);
    return {std::move(*out), true};
}

// In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure function of
// game state -- the single source of truth that ties nav / A-select / B-back / pause / in-game together instead of
// per-button hotfixes:
//
//   * GRAB the mouse (in-game pad mode: sticks = move/look, face = jump/etc) IFF no screen is up AND a world
//     exists (var == NULL && theWorld != NULL). ANY screen up, or no world (main menu), stays UNGRABBED so the pad
//     drives GUI nav. This self-corrects every path: e.g. B->keyTyped(ESCAPE)->displayGuiScreen(NULL) re-creates the
//     main menu (var becomes non-NULL) so it ungrabs, and the game's own setIngameFocus() (which re-enters
//     displayGuiScreen) can't leave the menu grabbed. Fixes "B doesn't go back" and every
//     menu-nav-goes-dead-after-a-transition case at once.
//   * (container editions) set g_guiCursorMode for the free analog cursor in inventory/chest GUIs.
//
// `var` here is the FINAL screen (after the null->GuiMainMenu / GuiGameOver fixups run above), so the test is
// exact. theWorld + mouseHelper are standard Minecraft fields present in every edition.
Patched patch_minecraft_displayguiscreen(std::string_view text, bool has_container) {
    if (has(text, "CONSOLE: pad grabbed")) return unchanged(text, true);   // the idempotence marker
    std::optional<std::string> out = replace_first(currentscreen_assign(), text, [has_container](const RegexMatch& m) {
        const std::string var(m[2]);
        std::string block(m[1]);
        if (has_container) {
            block += "\n\t{ extern bool g_guiCursorMode; g_guiCursorMode = (" + var + " != NULL && dynamic_cast<GuiContainer*>(" +
                     var + ") != NULL); }   // CONSOLE: analog cursor in container GUIs (CodeBreaker-injected)";
        }
        block += "\n\tMouse::setGrabbed(" + var +
                 " == NULL && this->theWorld != NULL);"
                 "   // CONSOLE: pad grabbed (in-game mode) IFF no screen up AND a world exists -- any menu"
                 " -> GUI nav mode. Sets g_grabbed DIRECTLY (NOT MouseHelper::grab/ungrab, which use AWT"
                 " windowComponent/robot -- NULL/broken on console -- and never call setGrabbed anyway, which"
                 " is why g_grabbed was stuck false). Single source of truth for controller input mode.";
        return block;
    });
    if (!out) return unchanged(text, false);
    return {std::move(*out), true};
}

Patched patch_entityrenderer_pointer(std::string_view text) {
    if (has(text, "Pointer.png")) return unchanged(text, true);
    const std::string ns = has(text, "d3d9::gl") ? "d3d9" : "GL11";
    constexpr auto kBody = std::to_array<std::string_view>({
        "// CONSOLE (controller): draw the virtual cursor (Pointer.png) ONLY in container GUIs. CodeBreaker-injected.",
        "{ extern bool g_guiCursorMode;",
        "if (g_guiCursorMode && this->mc->currentScreen != NULL) {",
        "\tint _gw = this->mc->currentScreen->width, _gh = this->mc->currentScreen->height;",
        "\tint _mx = _gw > 0 ? Mouse::getX() * _gw / this->mc->displayWidth : 0;",
        "\tint _my = _gh > 0 ? _gh - Mouse::getY() * _gh / this->mc->displayHeight - 1 : 0;",
        "\t@NS@::glBindTexture(@NS@::GL_TEXTURE_2D, this->mc->renderEngine->getTexture(\"/gui/Pointer.png\"));",
        "\t@NS@::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);",
        "\t@NS@::glEnable(@NS@::GL_BLEND);",
        "\t@NS@::glBlendFunc(@NS@::GL_SRC_ALPHA, @NS@::GL_ONE_MINUS_SRC_ALPHA);",
        "\t@NS@::glDisable(@NS@::GL_DEPTH_TEST);",
        "\tTessellator* _t = Tessellator::s_instance;",
        "\tint _cs = 16, _px = _mx - _cs / 2, _py = _my - _cs / 2;",
        "\t_t->startDrawingQuads();",
        "\t_t->addVertexWithUV((float)_px, (float)(_py + _cs), 0.0F, 0.0F, 1.0F);",
        "\t_t->addVertexWithUV((float)(_px + _cs), (float)(_py + _cs), 0.0F, 1.0F, 1.0F);",
        "\t_t->addVertexWithUV((float)(_px + _cs), (float)_py, 0.0F, 1.0F, 0.0F);",
        "\t_t->addVertexWithUV((float)_px, (float)_py, 0.0F, 0.0F, 0.0F);",
        "\t_t->draw();",
        "\t@NS@::glEnable(@NS@::GL_DEPTH_TEST);",
        "}",
        "}",
    });
    std::optional<std::string> out = replace_first(drawscreen_call(), text, [&](const RegexMatch& m) {
        const std::string ind(m[1]);   // the newline and indent of the drawScreen line: every injected line gets them
        std::string block;
        for (const std::string_view line : kBody) block += ind + std::string(line);
        return std::string(m[1]) + std::string(m[2]) + text::replace_all(block, "@NS@", ns);
    });
    if (!out) return unchanged(text, false);
    return {std::move(*out), true};
}

// Replace MouseHelper::grab/ungrabMouseCursor with console-safe bodies (just Mouse::setGrabbed + reset the look
// deltas). The Java versions deref windowComponent / MouseInfo / robot -- NULL/stub on console -- so any caller
// (setIngameFocus, keyTyped(ESCAPE)) faults; that is what black-screened the boot the moment the grab path was
// exercised. The pad-mode TRUTH is the displayGuiScreen invariant; these just have to be safe and consistent.
// MouseHelper.cpp already uses Mouse:: (setNativeCursor) so the namespace is in scope.
Patched patch_mousehelper_grab(std::string_view text) {
    // (trees converted before the tool was renamed carry the old marker)
    if (has(text, "CONSOLE grab (CodeBreaker)") || has(text, "CONSOLE grab (j2ctool)")) return unchanged(text, true);
    const auto grabbed = replace_first(grab_fn(), text, [](const RegexMatch&) { return std::string(kGrabSafe); });
    if (!grabbed) return unchanged(text, false);
    const auto both = replace_first(ungrab_fn(), *grabbed, [](const RegexMatch&) { return std::string(kUngrabSafe); });
    if (!both) return unchanged(text, false);
    return {*both, true};
}

namespace {

// One file's edit, read and patched in memory: the path, the text before and after.
struct Staged {
    stdfs::path path;
    std::string before;
    std::string after;
};

// Read + patch in memory. Records the reason in `missing` and returns nothing if the file is absent or unreadable
// or the anchor is not found.
std::optional<Staged> stage(const stdfs::path& out_dir, const char* fname, const std::function<Patched(std::string_view)>& patch,
                            const char* why, std::vector<std::string>& missing) {
    const stdfs::path path = out_dir / fname;
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) {
        missing.push_back(std::string(fname) + " (" + why + ": file absent)");
        return std::nullopt;
    }
    const std::optional<std::string> text = textutil::try_read_text(path);
    if (!text) {
        missing.push_back(std::string(fname) + " (" + why + ": unreadable)");
        return std::nullopt;
    }
    Patched patched = patch(*text);
    if (!patched.ok) {
        missing.push_back(std::string(fname) + " (" + why + ": anchor not found)");
        return std::nullopt;
    }
    return Staged{path, *text, std::move(patched.text)};
}

void commit(const Staged& s) {
    if (s.after != s.before) textutil::write_text(s.path, s.after);
}

}  // namespace

Result run(const stdfs::path& out_dir) {
    Result res;
    auto edit = [&](const char* fname, const std::function<Patched(std::string_view)>& patch, bool& applied, const char* why) {
        if (auto staged = stage(out_dir, fname, patch, why, res.missing)) {
            commit(*staged);
            applied = true;
        }
    };

    // The declaration (GuiScreen.h), definition (GuiScreen.cpp) and per-frame call (Minecraft.cpp) are ONE unit:
    // applying only some of them leaves a declared-but-undefined method (LNK2019) or a defined-but-undeclared
    // member (C2039). Stage all three in memory and write none unless every anchor matched.
    auto header = stage(out_dir, "GuiScreen.h", patch_guiscreen_h, "field+decl", res.missing);
    auto source = stage(out_dir, "GuiScreen.cpp", patch_guiscreen_cpp, "ctor+body", res.missing);
    auto callsite = stage(out_dir, "Minecraft.cpp", patch_minecraft_callsite, "per-frame call", res.missing);
    if (header && source && callsite) {
        commit(*header);
        commit(*source);
        commit(*callsite);
        res.guiscreen_h = res.guiscreen_cpp = res.callsite = true;
    } else {
        res.missing.emplace_back("menu-nav glue NOT applied to any file (all-or-nothing: "
                                 "GuiScreen.h/GuiScreen.cpp/Minecraft.cpp must all match)");
    }

    // displayGuiScreen: the SINGLE source of truth for controller input mode (grab/ungrab) -- always applied; this
    // is what makes nav / A / B-back / pause / in-game coherent. The g_guiCursorMode analog-cursor half is added
    // only when the edition HAS GuiContainer (inventory/chest); its Pointer.png draw goes in EntityRenderer.
    std::error_code ec;
    const bool has_container = stdfs::is_regular_file(out_dir / "GuiContainer.h", ec);
    edit("Minecraft.cpp", [has_container](std::string_view t) { return patch_minecraft_displayguiscreen(t, has_container); },
         res.focusmode, "grab/cursor mode");
    // MouseHelper grab/ungrab must be console-safe (no AWT) or setIngameFocus/keyTyped(ESCAPE) fault.
    edit("MouseHelper.cpp", patch_mousehelper_grab, res.mousehelper, "console-safe grab/ungrab");
    if (has_container) edit("EntityRenderer.cpp", patch_entityrenderer_pointer, res.pointer, "Pointer.png cursor");
    return res;
}

}  // namespace cb::java::projects::minecraft::controller_nav
