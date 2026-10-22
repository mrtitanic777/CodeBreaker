# -*- coding: utf-8 -*-
r"""
controller_nav.py -- inject console CONTROLLER MENU NAVIGATION into every converted Minecraft port.

THE BUG THIS ENDS (permanently): on the console the mouse is a virtual cursor driven by the pad. Menu
screens (GuiMainMenu / GuiOptions / GuiNewLevel / ...) are lists of GuiButtons; without extra glue the
D-pad does NOTHING (the cursor is frozen at screen-center over the first button) -- the menu is unusable.
The proven hand-ports (MinecraftAlpha) fixed this with a per-project overlay: a GuiScreen method that reads
the button rectangles, moves a selection with the D-pad, and snaps the virtual cursor onto it so the game's
OWN hover-highlight + A-as-click work unchanged. But it was a hand overlay -- a fresh convert (Indev, or a
regenerated Alpha) shipped WITHOUT it and the whole UI locked up. This pass migrates that glue into the
GENERATOR so EVERY edition gets working menu nav from one place and it can never be forgotten again.

DIVISION OF LABOUR (correct-by-construction):
  * The ALGORITHM (2D-spatial nav + cursor snap) lives ONCE in the reference: Input::menuNavSelect /
    Input::menuSnapCursor (Input.reference.cpp) -- pure geometry, no game types, edition-agnostic.
  * This pass injects a THIN adapter into the converted GuiScreen that feeds it the live button geometry.
    The adapter uses only fields that are UNIFORM across every edition (GuiButton xPosition/yPosition/
    width/height/enabled; GuiScreen controlList/width/height; Minecraft displayWidth/displayHeight), and the
    `(GuiButton*)controlList[i]` cast the game already uses -- so ONE injected body compiles for all.

WHAT IT INJECTS (keyed on the GuiScreen structure, not names/signatures -> covers all editions):
  1. GuiScreen.h  : `int navSelected;` field + `void handleControllerMenuNav();` decl (after controlList).
  2. GuiScreen.cpp: ctor member-init `, navSelected(-1)`; `#include "Input.h"/"GuiButton.h"`; the adapter body.
  3. Minecraft.cpp: a per-frame `currentScreen->handleControllerMenuNav();` right before the existing
     `currentScreen->updateScreen();` (the once-per-frame screen tick present in every edition).

Container GUIs (inventory/chest) that need a FREE analog cursor + Pointer icon are a SEPARATE feature
(g_guiCursorMode + EntityRenderer) intentionally NOT done here -- button menus are the fix. Discrete nav is
a no-op on a screen with no buttons, so this is safe on every screen.

Runs as a post-emit pass over the converted tree (xbox360), BEFORE overlays (so a project overlay can still
win). Idempotent: each edit is skipped if `handleControllerMenuNav` is already present. Fail-loud: run()
reports which anchors it could NOT find so a structural drift surfaces instead of a silently dead UI. Also
exposed as `codebreaker java controller-nav <dir>`.
"""

import os
import re

# The injected adapter body. UNIFORM across editions: only GuiButton geometry fields + the controlList cast
# the game itself uses. Calls the edition-agnostic core in Input.reference (Input::menuNavSelect/SnapCursor).
_NAV_METHOD = (
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
    "}\n"
)

_CONTROLLIST_FIELD = re.compile(r'\n([ \t]*)JList<[^;\n]*>\s+controlList\s*;')
_CTOR_INITLIST = re.compile(r'(GuiScreen::GuiScreen\(\)\s*:[^{}]*?)(\n\s*\{)')
_FIRST_INCLUDE = re.compile(r'(#include [^\n]*\n)')
_UPDATESCREEN_CALL = re.compile(r'(\n[ \t]*)([A-Za-z_][\w>.\-]*)->updateScreen\(\);')
# CONTAINER cursor mode: displayGuiScreen sets g_guiCursorMode true for container GUIs (inventory/chest/...)
# so the left stick drives a FREE analog cursor there (vs discrete button nav on menus). var1 = the new screen.
_CURRENTSCREEN_ASSIGN = re.compile(r'(this->currentScreen = \(GuiScreen\s*\*\)(\w+);)')
# EntityRenderer draws the current screen; the Pointer.png cursor is drawn right after, in container mode.
_DRAWSCREEN_CALL = re.compile(r'(\n[ \t]*)(this->mc->currentScreen->drawScreen\([^;]*\);)')


def patch_guiscreen_h(text):
    """Add `int navSelected;` + the method decl after the controlList field. Returns (new, ok)."""
    if 'handleControllerMenuNav' in text:
        return text, True
    m = _CONTROLLIST_FIELD.search(text)
    if not m:
        return text, False
    ind = m.group(1)
    add = ('\n%s// CONSOLE (controller menu nav): D-pad-selected button index (-1 = none yet). CodeBreaker-injected.'
           '\n%sint navSelected;'
           '\n%svoid handleControllerMenuNav();' % (ind, ind, ind))
    return text[:m.end()] + add + text[m.end():], True


def patch_guiscreen_cpp(text):
    """Add the ctor init, the two includes, and the adapter body. Returns (new, ok)."""
    if 'handleControllerMenuNav' in text:
        return text, True
    new, nsub = _CTOR_INITLIST.subn(r'\1\n\t, navSelected(-1)\2', text, count=1)
    if nsub == 0:
        return text, False                      # no member-init-list ctor found -> fail loud
    for inc in ('#include "Input.h"', '#include "GuiButton.h"'):
        if inc not in new:
            new = _FIRST_INCLUDE.sub(lambda mm, i=inc: mm.group(1) + i + '\n', new, count=1)
    new = new.rstrip('\n') + '\n' + _NAV_METHOD
    return new, True


def patch_minecraft_callsite(text):
    """Inject `<recv>->handleControllerMenuNav();` before the per-frame `<recv>->updateScreen();`."""
    if 'handleControllerMenuNav' in text:
        return text, True

    def rep(m):
        return '%s%s->handleControllerMenuNav();%s%s->updateScreen();' % (
            m.group(1), m.group(2), m.group(1), m.group(2))

    new, nsub = _UPDATESCREEN_CALL.subn(rep, text, count=1)
    return (new, True) if nsub else (text, False)


def patch_minecraft_displayguiscreen(text, has_container):
    """In displayGuiScreen (the ONE screen-transition choke point), make the controller's input mode a pure
    function of game state -- the single source of truth that ties nav / A-select / B-back / pause / in-game
    together instead of per-button hotfixes:

      * GRAB the mouse (in-game pad mode: sticks=move/look, face=jump/etc) IFF no screen is up AND a world
        exists (var1 == NULL && theWorld != NULL). ANY screen up, or no world (main menu), stays UNGRABBED so
        the pad drives GUI nav. This self-corrects every path: e.g. B->keyTyped(ESCAPE)->displayGuiScreen(NULL)
        re-creates the main menu (var1 becomes non-NULL) so it ungrabs, and the game's own setIngameFocus()
        (which re-enters displayGuiScreen) can't leave the menu grabbed. Fixes "B doesn't go back" and every
        menu-nav-goes-dead-after-a-transition case at once.
      * (container editions) set g_guiCursorMode for the free analog cursor in inventory/chest GUIs.

    `var1` here is the FINAL screen (after the null->GuiMainMenu / GuiGameOver fixups run above), so the test
    is exact. theWorld + mouseHelper are standard Minecraft fields present in every edition."""
    if 'CONSOLE: pad grabbed' in text:                     # idempotent marker (survives with/without container)
        return text, True

    def rep(m):
        var = m.group(2)
        block = m.group(1)
        if has_container:
            block += ('\n\t{ extern bool g_guiCursorMode; g_guiCursorMode = (%s != NULL && '
                      'dynamic_cast<GuiContainer*>(%s) != NULL); }'
                      '   // CONSOLE: analog cursor in container GUIs (CodeBreaker-injected)' % (var, var))
        block += ('\n\tMouse::setGrabbed(%s == NULL && this->theWorld != NULL);'
                  '   // CONSOLE: pad grabbed (in-game mode) IFF no screen up AND a world exists -- any menu'
                  ' -> GUI nav mode. Sets g_grabbed DIRECTLY (NOT MouseHelper::grab/ungrab, which use AWT'
                  ' windowComponent/robot -- NULL/broken on console -- and never call setGrabbed anyway, which'
                  ' is why g_grabbed was stuck false). Single source of truth for controller input mode.' % var)
        return block

    new, nsub = _CURRENTSCREEN_ASSIGN.subn(rep, text, count=1)
    return (new, True) if nsub else (text, False)


def patch_entityrenderer_pointer(text):
    """Draw the Pointer.png crosshair cursor right after the current screen renders, ONLY in container mode
    (g_guiCursorMode). Cursor coords are computed from Mouse::getX/getY + the screen scale so this is
    edition-independent (no dependency on the game's local mouseX/Y locals). GL namespace auto-detected."""
    if 'Pointer.png' in text:
        return text, True
    ns = 'd3d9' if 'd3d9::gl' in text else 'GL11'
    _body = [
        '// CONSOLE (controller): draw the virtual cursor (Pointer.png) ONLY in container GUIs. CodeBreaker-injected.',
        '{ extern bool g_guiCursorMode;',
        'if (g_guiCursorMode && this->mc->currentScreen != NULL) {',
        '\tint _gw = this->mc->currentScreen->width, _gh = this->mc->currentScreen->height;',
        '\tint _mx = _gw > 0 ? Mouse::getX() * _gw / this->mc->displayWidth : 0;',
        '\tint _my = _gh > 0 ? _gh - Mouse::getY() * _gh / this->mc->displayHeight - 1 : 0;',
        '\t@NS@::glBindTexture(@NS@::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/Pointer.png"));',
        '\t@NS@::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);',
        '\t@NS@::glEnable(@NS@::GL_BLEND);',
        '\t@NS@::glBlendFunc(@NS@::GL_SRC_ALPHA, @NS@::GL_ONE_MINUS_SRC_ALPHA);',
        '\t@NS@::glDisable(@NS@::GL_DEPTH_TEST);',
        '\tTessellator* _t = Tessellator::s_instance;',
        '\tint _cs = 16, _px = _mx - _cs / 2, _py = _my - _cs / 2;',
        '\t_t->startDrawingQuads();',
        '\t_t->addVertexWithUV((float)_px, (float)(_py + _cs), 0.0F, 0.0F, 1.0F);',
        '\t_t->addVertexWithUV((float)(_px + _cs), (float)(_py + _cs), 0.0F, 1.0F, 1.0F);',
        '\t_t->addVertexWithUV((float)(_px + _cs), (float)_py, 0.0F, 1.0F, 0.0F);',
        '\t_t->addVertexWithUV((float)_px, (float)_py, 0.0F, 0.0F, 0.0F);',
        '\t_t->draw();',
        '\t@NS@::glEnable(@NS@::GL_DEPTH_TEST);',
        '}',
        '}',
    ]

    def rep(m):
        ind = m.group(1)                                  # leading newline+indent of the drawScreen line
        block = ''.join(ind + line for line in _body).replace('@NS@', ns)
        return m.group(1) + m.group(2) + block

    new, nsub = _DRAWSCREEN_CALL.subn(rep, text, count=1)
    return (new, True) if nsub else (text, False)


# MouseHelper::grab/ungrabMouseCursor -- the whole function up to its column-0 closing brace (inner try/catch
# and if/else braces are indented, so `\n}` matches only the function end).
_GRAB_FN = re.compile(r'void MouseHelper::grabMouseCursor\(\)\s*\{[\s\S]*?\n\}')
_UNGRAB_FN = re.compile(r'void MouseHelper::ungrabMouseCursor\(\)\s*\{[\s\S]*?\n\}')
_GRAB_SAFE = ('void MouseHelper::grabMouseCursor()\n{\n\tMouse::setGrabbed(true);'
              '   // CONSOLE grab (CodeBreaker): set the pad mode flag only -- NO AWT\n'
              '\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}')
_UNGRAB_SAFE = ('void MouseHelper::ungrabMouseCursor()\n{\n\tMouse::setGrabbed(false);'
                '   // CONSOLE ungrab (CodeBreaker): set the pad mode flag only -- NO AWT\n'
                '\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}')


def patch_mousehelper_grab(text):
    """Replace MouseHelper::grab/ungrabMouseCursor with console-safe bodies (just Mouse::setGrabbed + reset the
    look deltas). The Java versions deref windowComponent / MouseInfo / robot -- NULL/stub on console -- so any
    caller (setIngameFocus, keyTyped(ESCAPE)) faults; that's what black-screened the boot the moment the grab
    path was exercised. The pad-mode TRUTH is the displayGuiScreen invariant; these just have to be safe and
    consistent. MouseHelper.cpp already uses Mouse:: (setNativeCursor) so the namespace is in scope."""
    if 'CONSOLE grab (CodeBreaker)' in text or 'CONSOLE grab (j2ctool)' in text:   # trees converted before the rename
        return text, True
    new, n1 = _GRAB_FN.subn(lambda m: _GRAB_SAFE, text, count=1)
    new, n2 = _UNGRAB_FN.subn(lambda m: _UNGRAB_SAFE, new, count=1)
    return new, (n1 == 1 and n2 == 1)


def run(out_dir):
    """Inject the controller menu-nav glue across the converted tree. Returns a dict:
    {'guiscreen_h','guiscreen_cpp','callsite': bool applied, 'missing': [reasons]}. `missing` is fail-loud:
    a required anchor was not found (structural drift) so the UI would ship dead."""
    res = {'guiscreen_h': False, 'guiscreen_cpp': False, 'callsite': False,
           'focusmode': False, 'mousehelper': False, 'pointer': False, 'missing': []}

    def _stage(fname, fn, why):
        """Read + patch in memory. Returns (path, old, new) or None (reason recorded in `missing`)."""
        path = os.path.join(out_dir, fname)
        if not os.path.isfile(path):
            res['missing'].append('%s (%s: file absent)' % (fname, why))
            return None
        try:
            text = open(path, 'r', encoding='utf-8', errors='replace').read()
        except (IOError, OSError):
            res['missing'].append('%s (%s: unreadable)' % (fname, why))
            return None
        new, ok = fn(text)
        if not ok:
            res['missing'].append('%s (%s: anchor not found)' % (fname, why))
            return None
        return path, text, new

    def _commit(staged):
        path, text, new = staged
        if new != text:
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new)

    def _edit(fname, fn, key, why):
        staged = _stage(fname, fn, why)
        if staged is not None:
            _commit(staged)
            res[key] = True

    # The declaration (GuiScreen.h), definition (GuiScreen.cpp) and per-frame call (Minecraft.cpp) are ONE
    # unit: applying only some of them leaves a declared-but-undefined method (LNK2019) or a defined-but-
    # undeclared member (C2039). Stage all three in memory and write none unless every anchor matched.
    group = [('GuiScreen.h', patch_guiscreen_h, 'guiscreen_h', 'field+decl'),
             ('GuiScreen.cpp', patch_guiscreen_cpp, 'guiscreen_cpp', 'ctor+body'),
             ('Minecraft.cpp', patch_minecraft_callsite, 'callsite', 'per-frame call')]
    staged = [_stage(f, fn, why) for f, fn, _k, why in group]
    if all(st is not None for st in staged):
        for st, (_f, _fn, key, _why) in zip(staged, group):
            _commit(st)
            res[key] = True
    else:
        res['missing'].append('menu-nav glue NOT applied to any file (all-or-nothing: '
                              'GuiScreen.h/GuiScreen.cpp/Minecraft.cpp must all match)')

    # displayGuiScreen: the SINGLE source of truth for controller input mode (grab/ungrab) -- always applied,
    # this is what makes nav / A / B-back / pause / in-game coherent. The g_guiCursorMode analog-cursor half is
    # added only when the edition HAS GuiContainer (inventory/chest); its Pointer.png draw goes in EntityRenderer.
    has_container = os.path.isfile(os.path.join(out_dir, 'GuiContainer.h'))
    _edit('Minecraft.cpp', lambda t: patch_minecraft_displayguiscreen(t, has_container), 'focusmode',
          'grab/cursor mode')
    # MouseHelper grab/ungrab must be console-safe (no AWT) or setIngameFocus/keyTyped(ESCAPE) fault.
    _edit('MouseHelper.cpp', patch_mousehelper_grab, 'mousehelper', 'console-safe grab/ungrab')
    if has_container:
        _edit('EntityRenderer.cpp', patch_entityrenderer_pointer, 'pointer', 'Pointer.png cursor')
    return res
