# -*- coding: utf-8 -*-
"""Tests for asset extraction (j2ctool.assets) and sound source-matching (j2ctool.xact.match_sources)."""

import os
import io
import zipfile

from codebreaker.java.console import assets, xact


def _make_jar(path, entries):
    with zipfile.ZipFile(path, 'w') as z:
        for name, data in entries.items():
            z.writestr(name, data)


def test_extract_mirrors_jar_paths(tmp_path):
    jar = str(tmp_path / 'game.jar')
    _make_jar(jar, {
        'gui/gui.png': b'\x89PNG-gui',
        'mob/char.png': b'\x89PNG-char',
        'terrain.png': b'\x89PNG-terrain',
        'lang/en_US.lang': b'key=value',
        'net/minecraft/Foo.class': b'CAFEBABE',      # not an asset -> skipped
    })
    content = str(tmp_path / 'Content')
    written = assets.extract(jar, content)
    # every asset, exact path preserved; the .class is not extracted
    assert set(written) == {'gui/gui.png', 'mob/char.png', 'terrain.png', 'lang/en_US.lang'}
    assert os.path.isfile(os.path.join(content, 'gui', 'gui.png'))
    assert os.path.isfile(os.path.join(content, 'lang', 'en_US.lang'))
    assert not os.path.exists(os.path.join(content, 'net', 'minecraft', 'Foo.class'))
    with open(os.path.join(content, 'gui', 'gui.png'), 'rb') as f:
        assert f.read() == b'\x89PNG-gui'


def _palette_png_bytes(with_trns=True):
    """A tiny 2x2 palettized (indexed) PNG, optionally with palette transparency."""
    from PIL import Image
    pal = Image.new('P', (2, 2))
    pal.putpalette([255, 0, 0,  0, 255, 0,  0, 0, 255,  0, 0, 0] + [0] * (256 * 3 - 12))
    pal.putpixel((0, 0), 0); pal.putpixel((1, 0), 1)
    pal.putpixel((0, 1), 2); pal.putpixel((1, 1), 3)
    buf = io.BytesIO()
    if with_trns:
        pal.info['transparency'] = 3        # index 3 fully transparent
        pal.save(buf, format='PNG', transparency=3)
    else:
        pal.save(buf, format='PNG')
    return buf.getvalue()


def test_png_needs_rgba_detects_indexed():
    idx = _palette_png_bytes()
    assert assets.png_needs_rgba(idx) is True
    rgba_bytes, changed = assets.normalize_png_to_rgba(idx)
    assert changed is True
    assert assets._png_colortype(rgba_bytes) == 6      # now RGBA


def test_normalize_preserves_palette_transparency():
    from PIL import Image
    idx = _palette_png_bytes(with_trns=True)
    rgba_bytes, _ = assets.normalize_png_to_rgba(idx)
    img = Image.open(io.BytesIO(rgba_bytes)).convert('RGBA')
    # the pixel that used palette index 3 (marked transparent) must have alpha 0
    assert img.getpixel((1, 1))[3] == 0
    # an opaque palette pixel keeps alpha 255 and its color
    assert img.getpixel((0, 0)) == (255, 0, 0, 255)


def _rgba_png_bytes():
    from PIL import Image
    buf = io.BytesIO()
    Image.new('RGBA', (2, 2), (10, 20, 30, 255)).save(buf, format='PNG')
    return buf.getvalue()


def test_extract_normalizes_indexed_png(tmp_path):
    jar = str(tmp_path / 'game.jar')
    rgba = _rgba_png_bytes()
    _make_jar(jar, {'gui/background.png': _palette_png_bytes(), 'terrain.png': rgba})
    content = str(tmp_path / 'Content')
    converted = []
    assets.extract(jar, content, converted=converted)
    assert converted == ['gui/background.png']            # only the indexed one converted
    with open(os.path.join(content, 'gui', 'background.png'), 'rb') as f:
        assert assets._png_colortype(f.read()) == 6
    with open(os.path.join(content, 'terrain.png'), 'rb') as f:
        assert f.read() == rgba                           # already-RGBA copied byte-for-byte


def test_no_normalize_when_disabled(tmp_path):
    jar = str(tmp_path / 'game.jar')
    idx = _palette_png_bytes()
    _make_jar(jar, {'gui/background.png': idx})
    content = str(tmp_path / 'Content')
    converted = []
    assets.extract(jar, content, normalize_png=False, converted=converted)
    assert converted == []
    with open(os.path.join(content, 'gui', 'background.png'), 'rb') as f:
        assert f.read() == idx                            # untouched


def test_coverage_flags_missing_and_extra(tmp_path):
    jar = str(tmp_path / 'game.jar')
    _make_jar(jar, {'gui/gui.png': b'x', 'terrain.png': b'x', 'unused/spare.png': b'x'})
    code = tmp_path / 'src'
    code.mkdir()
    # code loads gui/gui.png (present) and misc/absent.png (missing from jar)
    (code / 'Renderer.cpp').write_text(
        'loadTexture(JavaString("/gui/gui.png"));\n'
        'loadTexture(JavaString("/misc/absent.png"));\n', encoding='utf-8')
    cov = assets.coverage(jar, str(code))
    assert 'gui/gui.png' in cov['present']
    assert cov['missing'] == ['misc/absent.png']
    assert 'unused/spare.png' in cov['extra']


def test_coverage_case_insensitive(tmp_path):
    jar = str(tmp_path / 'game.jar')
    _make_jar(jar, {'GUI/Gui.png': b'x'})
    code = tmp_path / 'src'
    code.mkdir()
    (code / 'R.java').write_text('load("/gui/gui.png");', encoding='utf-8')
    cov = assets.coverage(jar, str(code))
    assert cov['missing'] == []          # case drift tolerated on Windows FS


def _mc_source_tree(tmp_path):
    """A tiny Java corpus that references a spread of sound events + a StepSound family."""
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'A.java').write_text(
        'playSound("random.click", x, y);\n'
        'playSoundFX("mob.zombie.say", 1, 1);\n', encoding='utf-8')
    (src / 'BlockGrass.java').write_text('new StepSound("grass", 1.0F, 1.0F);\n', encoding='utf-8')
    return str(src)


def _resources_tree(tmp_path):
    """Mimic the real resources layout: newsound/step/grass1..2, newsound/random/click1,
    newsound/mob/zombie/say1, and a door_close underscore file."""
    res = tmp_path / 'resources'
    for rel in ('newsound/step/grass1.ogg', 'newsound/step/grass2.ogg',
                'newsound/random/click1.ogg', 'newsound/mob/zombie/say1.ogg',
                'newsound/random/door_close.ogg'):
        p = res / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(b'OggS')
    return str(res)


def test_match_sources_maps_events_and_step_family(tmp_path):
    java = _mc_source_tree(tmp_path)
    res = _resources_tree(tmp_path)
    m = xact.match_sources(java, res)
    matched = m['matched']
    assert matched['random_click']['files'] == ['newsound/random/click1.ogg']
    assert matched['mob_zombie_say']['files'] == ['newsound/mob/zombie/say1.ogg']
    # step family: both numbered variants collected
    assert matched['step_grass']['files'] == ['newsound/step/grass1.ogg', 'newsound/step/grass2.ogg']


def test_match_dig_reuses_step_samples(tmp_path):
    java = _mc_source_tree(tmp_path)
    res = _resources_tree(tmp_path)
    m = xact.match_sources(java, res)
    # dig.grass has no dedicated ogg -> falls back to the step.grass samples
    assert 'dig_grass' in m['matched']
    assert m['matched']['dig_grass']['files'] == ['newsound/step/grass1.ogg', 'newsound/step/grass2.ogg']
    assert 'dig_grass' not in m['missing']


def test_ogg_keys_underscore_variant():
    keys = xact._ogg_keys('newsound/random/door_close.ogg')
    assert 'random/door_close' in keys        # literal
    assert 'random/door/close' in keys        # so event random.door.close matches


def test_write_ffmpeg_script(tmp_path):
    java = _mc_source_tree(tmp_path)
    res = _resources_tree(tmp_path)
    m = xact.match_sources(java, res)
    out = str(tmp_path / 'audio')
    os.makedirs(out)
    bat = xact.write_ffmpeg_script(m, out, res)
    text = open(bat, encoding='utf-8').read()
    assert 'ffmpeg' in text
    assert 'random_click.wav' in text
    assert os.path.isdir(os.path.join(out, 'wav'))
