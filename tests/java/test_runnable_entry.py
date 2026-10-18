# -*- coding: utf-8 -*-
"""Regression: the entry point for a Runnable game shell with no static main (Indev's Minecraft is
`final ... implements Runnable`, started on a thread). detect_runnable_game_class must find the
CONCRETE, TOP-LEVEL Runnable and generate a construct-and-run() entry -- and must NOT fire for
Alpha/Beta (abstract Minecraft + MinecraftImpl), which keep the Beta-specific Main.reference.cpp."""

from codebreaker.java import parse, runtime
from codebreaker.java.console.platform_refs import detect_runnable_game_class, generate_runnable_entry_point


def _c(src):
    return parse.parse_source(src)


def test_detects_concrete_toplevel_runnable():
    game = _c('public final class Game implements Runnable {\n'
              ' public Game(int w, int h, boolean f) {}\n public void run() {}\n}')
    other = _c('public class Helper { public int x; }')
    got = detect_runnable_game_class([game, other])
    assert got is not None and got.name == 'Game'


def test_abstract_runnable_is_ignored_beta_guard():
    # Beta's Minecraft is abstract (MinecraftImpl is the concrete subclass) -> None, so Main.reference.cpp is kept.
    absm = _c('public abstract class Minecraft implements Runnable { public void run() {} }')
    impl = _c('public class MinecraftImpl extends Minecraft { public void run() {} }')
    assert detect_runnable_game_class([absm, impl]) is None


def test_ui_runnable_with_superclass_is_ignored():
    # Indev's GuiLoadLevel implements Runnable but extends GuiScreen -> not the shell.
    ui = _c('public class GuiLoadLevel extends GuiScreen implements Runnable { public void run() {} }')
    assert detect_runnable_game_class([ui]) is None


def test_ambiguous_returns_none():
    a = _c('public class A implements Runnable { public void run() {} }')
    b = _c('public class B implements Runnable { public void run() {} }')
    assert detect_runnable_game_class([a, b]) is None


def test_entry_point_constructs_and_runs_with_default_args():
    game = _c('public final class Game implements Runnable {\n'
              ' public Game(Object canvas, int w, int h, boolean f) {}\n public void run() {}\n}')
    entry = generate_runnable_entry_point(game)
    assert 'new Game(NULL, SCREEN_WIDTH, SCREEN_HEIGHT, false)' in entry   # ref->NULL, ints->size, bool->false
    assert '_game->run();' in entry
    assert 'MinecraftImpl' not in entry
    assert 'j2x_initAll();' in entry and 'Display::create();' in entry


def test_glcontext_stub_has_gl3_caps():
    gl = runtime.generate_files(platform_id='xbox360')['GLContext.h']
    assert 'OpenGL30' in gl and 'OpenGL32' in gl and 'GL_ARB_compatibility' in gl
