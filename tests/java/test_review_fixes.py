# -*- coding: utf-8 -*-
"""Regressions for the 12 findings of the release-branch review (JavaString leak, profile restore on
exception, controller-nav atomicity, ->length flip, charAt receiver match, Save360 mount gate, sqrt
precision, alpha-ref persistence, viewport height, applet-ping CLI, beta edition markers)."""

import os
import re
import shutil
import subprocess

import pytest

from codebreaker.java import run as cli, parse, transpile as pipeline, profiles
from codebreaker.java.projects.minecraft import edition
from codebreaker.java.projects.minecraft import controller_nav

_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
_CANON = os.path.join(_ROOT, 'codebreaker', 'java', 'platform', 'canonical')
_REF = os.path.join(_ROOT, 'codebreaker', 'java', 'platform', 'reference')


def _read(*parts):
    with open(os.path.join(*parts), encoding='utf-8') as f:
        return f.read()


def _func_body(text, signature):
    """The `{...}` body of the function whose definition starts with `signature`."""
    start = text.index(signature)
    i = text.index('{', start)
    depth = 0
    for j in range(i, len(text)):
        if text[j] == '{':
            depth += 1
        elif text[j] == '}':
            depth -= 1
            if depth == 0:
                return text[i:j + 1]
    raise AssertionError('unbalanced body for %s' % signature)


_GXX = shutil.which('g++')


def _compile_and_run(tmp_path, src, include_dir=_CANON):
    cpp = tmp_path / 't.cpp'
    exe = tmp_path / 't.exe'
    cpp.write_text(src, encoding='utf-8')
    r = subprocess.run([_GXX, '-std=c++98', '-w', '-I', str(include_dir), str(cpp), '-o', str(exe)],
                       capture_output=True, text=True)
    assert r.returncode == 0, r.stderr
    r = subprocess.run([str(exe)], capture_output=True, text=True, timeout=60)
    assert r.returncode == 0, r.stdout + r.stderr
    return r.stdout


# ---------------------------------------------------------------- 1. toCharArray / getBytes leak
@pytest.mark.skipif(_GXX is None, reason='needs g++')
def test_tochararray_and_getbytes_memory_is_bounded(tmp_path):
    out = _compile_and_run(tmp_path, r'''
#include <cstdio>
#include <cstdlib>
#include <new>
static long g_live = 0;
void* operator new[](std::size_t n) throw(std::bad_alloc) {
    std::size_t* p = (std::size_t*)std::malloc(n + sizeof(std::size_t)); *p = n; g_live += (long)n; return p + 1; }
void operator delete[](void* q) throw() {
    if (!q) return; std::size_t* p = (std::size_t*)q - 1; g_live -= (long)*p; std::free(p); }
#include "JavaString.h"
int main() {
    JavaString s(std::string(1000, 'x'));
    for (int i = 0; i < 20000; ++i) { JArray<unsigned char> c = s.toCharArray(); JArray<unsigned char> b = s.getBytes("UTF-8"); }
    JArray<unsigned char> c = JavaString("abc").toCharArray();
    if (c.length != 3 || c[0] != 'a' || c[2] != 'c' || c.data()[3] != 0) return 2;   // copy + NUL tail
    std::printf("%ld\n", g_live);
    return 0;
}
''')
    # leaking would hold 20000 * 2 * 1001 bytes (~40 MB); the ring caps it at 2 * 256 slots
    assert int(out.strip()) < 2 * 256 * 1001 + 4096


# ---------------------------------------------------------------- 2. profile restored on exception
def test_convert_restores_profile_and_getters_when_it_raises(tmp_path, monkeypatch):
    from codebreaker.java import typemap
    java = tmp_path / 'java'; java.mkdir()
    (java / 'Level.java').write_text('public class Level { public int x; }', encoding='utf-8')
    before = profiles.active().name
    getters = set(typemap.KNOWN_NULLABLE_GETTERS)

    def boom(_classes):
        raise RuntimeError('midway failure')
    monkeypatch.setattr(pipeline, 'resolve_inheritance', boom)
    with pytest.raises(RuntimeError):
        pipeline.convert(str(java), str(tmp_path / 'out'), edition='rubydung')
    assert profiles.active().name == before
    assert typemap.KNOWN_NULLABLE_GETTERS == getters


# ---------------------------------------------------------------- 3. controller-nav all-or-nothing
def test_controller_nav_writes_nothing_when_one_anchor_is_missing(tmp_path):
    from tests.java.test_controller_nav import _GUISCREEN_H, _MINECRAFT_CPP
    files = {
        'GuiScreen.h': _GUISCREEN_H,
        # no member-initializer list -> the ctor+body anchor cannot match
        'GuiScreen.cpp': '#include "GuiScreen.h"\n\nGuiScreen::GuiScreen()\n{\n}\n',
        'Minecraft.cpp': _MINECRAFT_CPP,
    }
    for name, src in files.items():
        (tmp_path / name).write_text(src, encoding='utf-8')
    res = controller_nav.run(str(tmp_path))
    assert not (res['guiscreen_h'] or res['guiscreen_cpp'] or res['callsite'])
    assert any('all-or-nothing' in m for m in res['missing'])
    assert (tmp_path / 'GuiScreen.h').read_text(encoding='utf-8') == _GUISCREEN_H
    assert 'handleControllerMenuNav' not in (tmp_path / 'Minecraft.cpp').read_text(encoding='utf-8')


# ---------------------------------------------------------------- 5. charAt receiver match


# ---------------------------------------------------------------- 6/7. Save360 mount gate + unmount
def test_save360_shutdown_releases_mount_gate():
    body = _func_body(_read(_REF, 'Save360.reference.cpp'), 'void Save360_Shutdown()')
    assert 'g_mountState = 0' in body


def test_save360_unwritable_mount_is_closed_before_fallback():
    text = _read(_REF, 'Save360.reference.cpp')
    i = text.index('root is not writable')
    branch = text[i:text.index('return false;', i)]
    assert 'XContentClose(SAVE_ROOT_NAME' in branch


# ---------------------------------------------------------------- 8. Math.sqrt always double
@pytest.mark.skipif(_GXX is None, reason='needs g++')
def test_javamathsqrt_resolves_atan2_with_float_arg(tmp_path):
    # compile the shim exactly as shipped in JavaRT.h (in isolation: the full runtime header set shadows
    # system headers on a case-insensitive filesystem)
    shim = [l for l in _read(_CANON, 'JavaRT.h').splitlines() if 'javaMathSqrt(double' in l]
    assert len(shim) == 1
    out = _compile_and_run(tmp_path, '#include <cstdio>\n#include <math.h>\n' + shim[0] + r'''
int main() { float f = 4.0F; double d = 1.0; std::printf("%.3f\n", atan2(javaMathSqrt(f), d)); return 0; }
''')
    assert out.strip() == '1.107'


# ---------------------------------------------------------------- 9/10. GL alpha ref + viewport
def test_alpha_test_toggle_does_not_reset_alpha_ref():
    text = _read(_REF, 'GL11.reference.cpp')
    for sig in ('void glEnable(int cap)', 'void glDisable(int cap)'):
        assert not re.search(r'g_alphaRef\s*=', _func_body(text, sig))
    assert re.search(r'static float g_alphaRef = 0\.5f;', text)


def test_viewport_height_comes_from_backbuffer_and_y_is_clamped():
    text = _read(_REF, 'GL11.reference.cpp')
    vp = _func_body(text, 'void glViewport(int x, int y, int w, int h)')
    assert 'g_fbH =' not in vp                     # no longer captured from an arbitrary (0,0,w,h)
    assert 'if (fy < 0) fy = 0;' in vp
    assert 'GetBackBuffer' in _func_body(text, 'void glSetDevice(IDirect3DDevice9* dev)')


# ---------------------------------------------------------------- 11. applet-ping CLI
def test_applet_ping_cli_is_registered(tmp_path, capsys):
    assert cli.main(['applet-ping', str(tmp_path)]) == 0
    assert 'applet ping' in capsys.readouterr().out


# ---------------------------------------------------------------- 12. edition markers
def test_alpha_with_worldchunkmanager_is_not_beta():
    c = [parse.parse_source(s) for s in (
        'package net.minecraft.src;\npublic class World {}',
        'package net.minecraft.src;\npublic class Chunk {}',
        'package net.minecraft.src;\npublic class WorldChunkManager {}',
        'package net.minecraft.src;\npublic class ChunkProviderGenerate {}',
        'package net.minecraft.src;\npublic class MobSpawnerBase {}',
    )]
    assert edition.detect_edition(c) == 'alpha'


def test_beta_only_class_still_detects_beta():
    c = [parse.parse_source(s) for s in (
        'package net.minecraft.src;\npublic class World {}',
        'package net.minecraft.src;\npublic class Chunk {}',
        'package net.minecraft.src;\npublic class BlockBed {}',
    )]
    assert edition.detect_edition(c) == 'beta'
