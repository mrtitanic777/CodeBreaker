# -*- coding: utf-8 -*-
"""Tests for the UI-marker tool (j2ctool.ui_mark) and debug markers (j2ctool.debug)."""

import os
from codebreaker.java import ui_mark, debug


def _write(p, text):
    with open(p, 'w', encoding='utf-8') as f:
        f.write(text)


def test_discover_by_name_and_inheritance(tmp_path):
    _write(str(tmp_path / 'GuiScreen.h'), 'class GuiScreen : public Gui {\npublic: void draw();\n};\n')
    _write(str(tmp_path / 'GuiMainMenu.h'), 'class GuiMainMenu : public GuiScreen {\n};\n')
    # extends a UI base but non-UI name -> still detected transitively
    _write(str(tmp_path / 'OptionsPane.h'), 'class OptionsPane : public GuiScreen {\n};\n')
    _write(str(tmp_path / 'World.h'), 'class World {\n};\n')   # not UI
    ui = ui_mark.discover_ui_classes(str(tmp_path))
    assert 'GuiScreen' in ui and 'GuiMainMenu' in ui
    assert 'OptionsPane' in ui          # transitive via GuiScreen
    assert 'World' not in ui


def test_mark_inserts_greppable_marker(tmp_path):
    _write(str(tmp_path / 'GuiMainMenu.h'), 'class GuiMainMenu : public GuiScreen {\n};\n')
    cpp = ('#include "GuiMainMenu.h"\n'
           'void GuiMainMenu::initGui()\n{\n    addButton();\n}\n'
           'void GuiMainMenu::drawScreen(int x, int y)\n{\n    blit();\n}\n'
           '// a call, not a def:\n'
           'void other() { GuiMainMenu::staticHelper(); }\n')
    _write(str(tmp_path / 'GuiMainMenu.cpp'), cpp)
    res = ui_mark.mark(str(tmp_path))
    text = open(str(tmp_path / 'GuiMainMenu.cpp'), encoding='utf-8').read()
    assert '// J2X-UI[GuiMainMenu::initGui]' in text
    assert '// J2X-UI[GuiMainMenu::drawScreen]' in text
    methods = {m for _f, _l, m in res['marked'].get('GuiMainMenu', [])}
    assert 'initGui' in methods and 'drawScreen' in methods


def test_mark_is_idempotent(tmp_path):
    _write(str(tmp_path / 'GuiChat.h'), 'class GuiChat : public GuiScreen {\n};\n')
    _write(str(tmp_path / 'GuiChat.cpp'),
           '#include "GuiChat.h"\nvoid GuiChat::draw()\n{\n}\n')
    ui_mark.mark(str(tmp_path))
    first = open(str(tmp_path / 'GuiChat.cpp'), encoding='utf-8').read()
    r2 = ui_mark.mark(str(tmp_path))
    second = open(str(tmp_path / 'GuiChat.cpp'), encoding='utf-8').read()
    assert first == second                              # no double markers
    assert first.count('J2X-UI[GuiChat::draw]') == 1
    assert r2['skipped'] >= 1


def test_dry_run_does_not_edit(tmp_path):
    _write(str(tmp_path / 'GuiChat.h'), 'class GuiChat : public GuiScreen {\n};\n')
    src = '#include "GuiChat.h"\nvoid GuiChat::draw()\n{\n}\n'
    _write(str(tmp_path / 'GuiChat.cpp'), src)
    res = ui_mark.mark(str(tmp_path), dry_run=True)
    assert open(str(tmp_path / 'GuiChat.cpp'), encoding='utf-8').read() == src
    assert res['marked'].get('GuiChat')                 # still reported


def test_run_writes_index(tmp_path):
    _write(str(tmp_path / 'GuiChat.h'), 'class GuiChat : public GuiScreen {\n};\n')
    _write(str(tmp_path / 'GuiChat.cpp'), '#include "GuiChat.h"\nvoid GuiChat::draw()\n{\n}\n')
    res = ui_mark.run(str(tmp_path))
    assert os.path.isfile(res['index'])
    idx = open(res['index'], encoding='utf-8').read()
    assert 'J2X-UI' in idx and 'GuiChat::draw' in idx


def test_debug_markers_scannable(tmp_path):
    debug.write_debug(str(tmp_path))
    markers = debug.scan_debug_markers(str(tmp_path))
    # a spread of facilities are catalogued
    for tag in ('once', 'leak-new', 'net', 'crash-hard', 'summary'):
        assert tag in markers


def test_debug_off_bakes_disabled(tmp_path):
    debug.write_debug(str(tmp_path), enabled_default=False)
    h = open(str(tmp_path / 'Debug.h'), encoding='utf-8').read()
    assert '#define DEBUG_ENABLED 0' in h
