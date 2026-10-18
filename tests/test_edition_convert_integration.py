# -*- coding: utf-8 -*-
"""Light end-to-end: convert() activates the edition profile for the run and restores the default
afterward, with no SDK/decompile required."""

import pytest

from j2ctool import pipeline, profiles


@pytest.fixture(autouse=True)
def _restore_default():
    yield
    profiles.use('minecraft_xbox360')


def _write_corpus(d, files):
    for name, src in files.items():
        (d / name).write_text(src, encoding='utf-8')
    return str(d)


def test_explicit_edition_activates_then_restores(tmp_path):
    java = tmp_path / 'java'; java.mkdir()
    out = tmp_path / 'out'
    _write_corpus(java, {'Level.java': 'public class Level { public int x; }'})
    assert profiles.active().name == 'minecraft_xbox360'
    s = pipeline.convert(java, out, platform=None, edition='rubydung')
    assert s['parsed'] == 1
    # scoped to the conversion: active profile restored to the default afterward
    assert profiles.active().name == 'minecraft_xbox360'


def test_autodetect_beta(tmp_path):
    java = tmp_path / 'java'; java.mkdir()
    out = tmp_path / 'out'
    _write_corpus(java, {
        'World.java': 'package net.minecraft.src;\npublic class World { public int x; }',
        'BiomeGenBase.java': 'package net.minecraft.src;\npublic class BiomeGenBase { public int id; }',
    })
    logs = []
    pipeline.convert(java, out, platform=None, progress_cb=logs.append)
    assert any('active target profile: minecraft_beta' in m for m in logs)
    assert profiles.active().name == 'minecraft_xbox360'


def test_no_edition_inconclusive_keeps_default(tmp_path):
    java = tmp_path / 'java'; java.mkdir()
    out = tmp_path / 'out'
    _write_corpus(java, {'Foo.java': 'public class Foo { public int x; }'})
    logs = []
    pipeline.convert(java, out, platform=None, progress_cb=logs.append)
    # nothing conclusive -> no switch happened, default never left
    assert not any('active target profile' in m for m in logs)
    assert profiles.active().name == 'minecraft_xbox360'


def test_version_hint_selects_edition(tmp_path):
    java = tmp_path / 'java'; java.mkdir()
    out = tmp_path / 'out'
    _write_corpus(java, {'Foo.java': 'public class Foo { public int x; }'})
    logs = []
    pipeline.convert(java, out, platform=None, mc_version='a1.2.6', progress_cb=logs.append)
    assert any('active target profile: minecraft_alpha' in m for m in logs)
