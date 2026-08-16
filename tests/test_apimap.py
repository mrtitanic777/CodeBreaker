# -*- coding: utf-8 -*-
"""Tests for the API-mapping registry (j2ctool.apimap)."""

from j2ctool import apimap


def _reg():
    return apimap.load(force=True)


def test_languages_and_dialects_loaded():
    reg = _reg()
    assert 'java' in reg.languages and 'cpp' in reg.languages
    assert 'opengl' in reg.languages and 'python' in reg.languages
    # cpp declares an ordered dialect list
    assert reg.dialect_order('cpp')[:3] == ['c++98', 'c++03', 'c++11']


def test_resolve_qualified_and_leaf():
    reg = _reg()
    for name in ('GL11.glVertex3f', 'glVertex3f', 'org.lwjgl.opengl.GL11.glVertex3f'):
        m = reg.resolve(name, 'opengl', 'direct3d9')
        assert m is not None, name
        assert m.target == 'j2c::gl::vertex3f'
        assert m.kind == 'shim'


def test_unmapped_returns_none():
    reg = _reg()
    assert reg.resolve('Zombie.tick', 'java', 'cpp') is None


def test_version_axis_selects_newest_at_or_below():
    reg = _reg()
    # null literal: NULL at C++03, nullptr at C++11+
    assert reg.resolve('null', 'java', 'cpp', 'c++03').target == 'NULL'
    assert reg.resolve('null', 'java', 'cpp', 'c++11').target == 'nullptr'
    assert reg.resolve('null', 'java', 'cpp', 'c++17').target == 'nullptr'
    # no dialect requested -> base target
    assert reg.resolve('null', 'java', 'cpp').target == 'NULL'


def test_version_axis_math_overload():
    reg = _reg()
    assert reg.resolve('Math.sqrt', 'java', 'cpp', 'c++03').target == 'sqrt'
    assert reg.resolve('Math.sqrt', 'java', 'cpp', 'c++11').target == 'std::sqrt'


def test_multi_language_python():
    reg = _reg()
    assert reg.resolve('len', 'python', 'cpp').target == 'std::size'
    assert reg.resolve('len', 'python', 'cpp', 'c++98').target == '.size()'
    # unsupported is registered, not just missing
    m = reg.resolve('list.append', 'python', 'c')
    assert m is not None and m.kind == 'unsupported' and not m.supported


def test_equiv_bridge_present():
    reg = _reg()
    # legacy per-class Java->Xbox360 intent map is queryable
    m = reg.resolve('GL11', 'java', 'xbox360')
    assert m is not None and 'IDirect3DDevice9' in m.target
    dead = reg.resolve('CodecJOrbis', 'java', 'xbox360')
    assert dead is not None and dead.kind == 'unsupported'


def test_reverse_index_includes_by_version():
    reg = _reg()
    assert reg.reverse('NULL', 'cpp', 'java') is not None
    assert reg.reverse('nullptr', 'cpp', 'java') is not None
    assert reg.reverse('j2c::gl::vertex3f', 'direct3d9', 'opengl') is not None


def test_coverage_classification():
    reg = _reg()
    cov = reg.coverage(['GL11.glVertex3f', 'glEnd', 'Foo.bar'], 'opengl', 'direct3d9')
    assert len(cov['mapped']) == 2
    assert cov['unmapped'] == ['Foo.bar']


def test_symbol_extraction():
    src = 'void f(){ GL11.glBegin(7); this.zombies.get(i); int v = GL11.GL_VIEWPORT; }'
    syms = apimap.extract_symbols(src)
    assert 'GL11.glBegin' in syms
    assert 'GL11.GL_VIEWPORT' in syms


def test_symbols_in_dedup_canonical():
    reg = _reg()
    rows = reg.symbols_in('java.nio')
    names = [n for n, _ in rows]
    # canonical qualified name kept, bare-leaf alias collapsed away
    assert 'Buffer.clear' in names
    assert 'clear' not in names


def test_render_coverage_md_smoke():
    md = apimap.render_coverage_md(['GL11.glBegin', 'Foo.bar'], 'opengl', 'direct3d9')
    assert '# API Mapping Coverage' in md
    assert 'glBegin' in md
