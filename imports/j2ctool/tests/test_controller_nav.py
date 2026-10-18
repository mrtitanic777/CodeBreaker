# -*- coding: utf-8 -*-
"""Regressions for controller_nav.py -- the pass that injects console D-pad menu navigation into the
converted GuiScreen (+ the per-frame call), so no edition ships a dead menu. Keys on structure (the
controlList field, the member-init-list ctor, the per-frame `->updateScreen();`), not names, so it must
cover Indev/Alpha/Beta whose surrounding code differs."""

import os
import tempfile

from j2ctool import controller_nav as cn


_GUISCREEN_H = (
    '#ifndef GUISCREEN_H\n#define GUISCREEN_H\n'
    'class GuiScreen : public Gui {\npublic:\n'
    '\tGuiScreen();\n\tMinecraft* mc;\n\tint width;\n\tint height;\n'
    '\tJList<JObject> controlList;\n\tbool allowUserInput;\n'
    '\tvirtual void updateScreen();\n};\n#endif\n'
)
_GUISCREEN_CPP = (
    '#include "stdafx.h"\n#include "GuiScreen.h"\n#include "GuiButton.h"\n\n'
    'GuiScreen::GuiScreen()\n\t: mc(NULL)\n\t, width(0)\n\t, height(0)\n\t, allowUserInput(0)\n{\n}\n\n'
    'void GuiScreen::updateScreen()\n{\n}\n'
)
_MINECRAFT_CPP = (
    '#include "Minecraft.h"\n#include "GuiContainer.h"\n'
    'void Minecraft::runTick() {\n'
    '\tif(this->currentScreen != NULL) {\n'
    '\t\tthis->currentScreen->updateScreen();\n'
    '\t}\n}\n'
    'void Minecraft::displayGuiScreen(GuiScreen* var1) {\n'
    '\tthis->currentScreen = (GuiScreen*)var1;\n'
    '\tif(var1 != NULL) { var1->setWorldAndResolution(this, 0, 0); }\n}\n'
)


def test_guiscreen_h_gets_field_and_decl():
    new, ok = cn.patch_guiscreen_h(_GUISCREEN_H)
    assert ok
    assert 'int navSelected;' in new
    assert 'void handleControllerMenuNav();' in new
    # inserted right after the controlList field, before allowUserInput
    assert new.index('navSelected') > new.index('controlList')
    assert new.index('navSelected') < new.index('allowUserInput')


def test_guiscreen_cpp_gets_ctor_init_and_body():
    new, ok = cn.patch_guiscreen_cpp(_GUISCREEN_CPP)
    assert ok
    assert ', navSelected(-1)' in new                      # ctor member-init
    assert 'void GuiScreen::handleControllerMenuNav() {' in new
    assert 'Input::menuNavSelect(' in new and 'Input::menuSnapCursor(' in new
    assert '#include "Input.h"' in new                     # adapter needs the core's header
    # the ctor init lands INSIDE the init list (before the body brace)
    assert new.index(', navSelected(-1)') < new.index('\n{')


def test_callsite_injected_before_updatescreen():
    new, ok = cn.patch_minecraft_callsite(_MINECRAFT_CPP)
    assert ok
    assert 'this->currentScreen->handleControllerMenuNav();' in new
    # nav call comes immediately before the updateScreen call, same receiver
    assert new.index('handleControllerMenuNav') < new.index('->updateScreen')


def test_idempotent_reapply():
    # applying twice must not double-inject (post-emit pass re-runs every convert)
    once, _ = cn.patch_guiscreen_cpp(_GUISCREEN_CPP)
    twice, ok = cn.patch_guiscreen_cpp(once)
    assert ok and twice == once
    assert twice.count('handleControllerMenuNav() {') == 1


def test_fail_loud_on_missing_anchor():
    # a GuiScreen.cpp with no member-init-list ctor -> not injected, reported (not silently skipped)
    bad = _GUISCREEN_CPP.replace('GuiScreen::GuiScreen()\n\t: mc(NULL)\n\t, width(0)\n\t, height(0)\n\t, allowUserInput(0)\n{',
                                 'GuiScreen::GuiScreen() {')
    _new, ok = cn.patch_guiscreen_cpp(bad)
    assert not ok


_MINECRAFT_DGS = (
    '#include "Minecraft.h"\n#include "GuiContainer.h"\n'
    'void Minecraft::displayGuiScreen(GuiScreen* var1) {\n'
    '\tthis->currentScreen = (GuiScreen*)var1;\n'
    '\tif(var1 != NULL) { var1->setWorldAndResolution(this, 0, 0); }\n}\n'
)
_ENTITYRENDERER = (
    '#include "EntityRenderer.h"\n#include "Tessellator.h"\n#include "RenderEngine.h"\n#include "Input.h"\n'
    'void EntityRenderer::updateCameraAndRender(float var1) {\n'
    '\td3d9::glClear(0);\n'
    '\tthis->mc->currentScreen->drawScreen(var5, var6, var1);\n}\n'
)


def test_displayguiscreen_injects_cursor_and_grab_invariant():
    new, ok = cn.patch_minecraft_displayguiscreen(_MINECRAFT_DGS, True)
    assert ok
    # container-cursor half (var1 must appear -- regression for the octal-escape bug that dropped it)
    assert 'g_guiCursorMode = (var1 != NULL && dynamic_cast<GuiContainer*>(var1) != NULL);' in new
    # grab-mode invariant: pad grabbed IFF no screen up AND a world exists -- set DIRECTLY (no AWT MouseHelper)
    assert 'Mouse::setGrabbed(var1 == NULL && this->theWorld != NULL);' in new
    assert 'mouseHelper->grabMouseCursor' not in new       # must NOT route through the broken AWT path


def test_grab_invariant_applies_without_container():
    # editions with no GuiContainer still get the grab-mode invariant (menus must be navigable) but NO cursor
    new, ok = cn.patch_minecraft_displayguiscreen(_MINECRAFT_DGS, False)
    assert ok
    assert 'Mouse::setGrabbed(var1 == NULL && this->theWorld != NULL);' in new
    assert 'g_guiCursorMode' not in new


def test_pointer_draw_injected_after_drawscreen():
    new, ok = cn.patch_entityrenderer_pointer(_ENTITYRENDERER)
    assert ok
    assert 'Pointer.png' in new and 'g_guiCursorMode' in new
    # GL namespace auto-detected from the file (d3d9 here, not GL11)
    assert 'd3d9::glBindTexture' in new and 'GL11::glBindTexture' not in new
    # drawn AFTER the screen renders
    assert new.index('Pointer.png') > new.index('->drawScreen(')


_MOUSEHELPER = (
    '#include "MouseHelper.h"\n#include "Input.h"\n'
    'void MouseHelper::grabMouseCursor()\n{\n'
    '\ttry {\n\tMouse::setNativeCursor(this->cursor);\n\t} catch (LWJGLException* var2) {\n'
    '\tvar2->printStackTrace();\n\t}\n\tthis->ungrabMouseCursor();\n\tthis->deltaX = 0;\n\tthis->deltaY = 0;\n}\n\n'
    'void MouseHelper::ungrabMouseCursor()\n{\n'
    '\tPoint* var1 = MouseInfo::getPointerInfo().getLocation();\n'
    '\tPoint* var2 = this->windowComponent->getLocationOnScreen();\n'
    '\tthis->robot->mouseMove(this->mouseX, this->mouseY);\n'
    '\tthis->mouseX = var2->x + this->windowComponent->getWidth() / 2;\n'
    '\tif(this->mouseInt == 0) {\n\tthis->deltaX = var1->x - this->mouseX;\n\t} else {\n\t--this->mouseInt;\n\t}\n}\n'
)


def test_mousehelper_grab_made_console_safe():
    new, ok = cn.patch_mousehelper_grab(_MOUSEHELPER)
    assert ok
    # both bodies replaced: set the flag, no AWT
    assert 'Mouse::setGrabbed(true);' in new and 'Mouse::setGrabbed(false);' in new
    assert 'MouseInfo::getPointerInfo' not in new          # AWT gone
    assert 'windowComponent->getLocationOnScreen' not in new
    assert 'robot->mouseMove' not in new
    # idempotent
    twice, ok2 = cn.patch_mousehelper_grab(new)
    assert ok2 and twice == new


def test_displayguiscreen_idempotent():
    once, _ = cn.patch_minecraft_displayguiscreen(_MINECRAFT_DGS, True)
    twice, ok = cn.patch_minecraft_displayguiscreen(once, True)
    assert ok and twice == once


def test_run_over_tree_reports_success_and_missing():
    d = tempfile.mkdtemp()
    for name, src in (('GuiScreen.h', _GUISCREEN_H), ('GuiScreen.cpp', _GUISCREEN_CPP),
                      ('Minecraft.cpp', _MINECRAFT_CPP), ('MouseHelper.cpp', _MOUSEHELPER)):
        with open(os.path.join(d, name), 'w', encoding='utf-8') as f:
            f.write(src)
    res = cn.run(d)
    assert res['guiscreen_h'] and res['guiscreen_cpp'] and res['callsite'] and res['focusmode'] and res['mousehelper']
    assert res['missing'] == []
    # and the files on disk now carry the glue
    assert 'handleControllerMenuNav' in open(os.path.join(d, 'GuiScreen.cpp'), encoding='utf-8').read()

    # a tree missing Minecraft.cpp -> callsite reported missing (fail-loud), not a silent dead UI
    d2 = tempfile.mkdtemp()
    with open(os.path.join(d2, 'GuiScreen.h'), 'w', encoding='utf-8') as f:
        f.write(_GUISCREEN_H)
    res2 = cn.run(d2)
    assert any('Minecraft.cpp' in m for m in res2['missing'])
