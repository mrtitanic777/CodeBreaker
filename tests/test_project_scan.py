# -*- coding: utf-8 -*-
"""Tests for the one-folder project resolver (j2ctool.project_scan)."""

import os
from j2ctool import project_scan as ps


def _make_tree(tmp_path):
    """A decompiled-Minecraft-shaped tree under <base>."""
    base = tmp_path / 'minecraft'
    (base / 'jars').mkdir(parents=True)
    (base / 'jars' / 'deobfuscated.jar').write_bytes(b'PK\x03\x04')
    (base / 'jars' / 'deobfuscated-source.jar').write_bytes(b'PK\x03\x04')   # must NOT be chosen
    res = base / 'game' / 'resources' / 'newsound' / 'random'
    res.mkdir(parents=True)
    (res / 'click1.ogg').write_bytes(b'OggS')
    src = base / 'src' / 'net' / 'minecraft'
    src.mkdir(parents=True)
    (src / 'World.java').write_text('public class World {}', encoding='utf-8')
    lang = base / 'game' / 'resources' / 'lang'
    lang.mkdir(parents=True)
    (lang / 'en_US.lang').write_text('achievement.x=X', encoding='utf-8')
    return str(base)


def test_resolve_from_base(tmp_path):
    base = _make_tree(tmp_path)
    r = ps.resolve(base)
    assert os.path.basename(r['base']) == 'minecraft'
    assert r['jar'].endswith('deobfuscated.jar') and 'source' not in os.path.basename(r['jar'])
    assert os.path.basename(r['resources']) == 'resources'
    assert r['java_src'].endswith('src')                 # the ROOT, not a deep package
    assert r['lang'].endswith('en_US.lang')


def test_resolve_from_src(tmp_path):
    base = _make_tree(tmp_path)
    r = ps.resolve(os.path.join(base, 'src'))
    assert r['jar'].endswith('deobfuscated.jar')
    assert r['java_src'].endswith('src')


def test_resolve_from_deep_package(tmp_path):
    base = _make_tree(tmp_path)
    r = ps.resolve(os.path.join(base, 'src', 'net', 'minecraft'))
    # still finds the base + src root by walking up
    assert os.path.basename(r['base']) == 'minecraft'
    assert r['java_src'].endswith('src')
    assert r['jar'].endswith('deobfuscated.jar')


def test_prefers_deobfuscated_over_other_jar(tmp_path):
    base = _make_tree(tmp_path)
    # add a stray jar in the base; deobfuscated.jar under jars/ still wins
    (tmp_path / 'minecraft' / 'other.jar').write_bytes(b'PK\x03\x04')
    r = ps.resolve(base)
    assert os.path.basename(r['jar']) == 'deobfuscated.jar'
