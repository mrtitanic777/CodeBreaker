# -*- coding: utf-8 -*-
"""Tests for the achievement conversion analyzer (codebreaker.java.projects.minecraft.achievements)."""

import os
from codebreaker.java.projects.minecraft import achievements as ach


_SAMPLE = (
    '#include "AchievementList.h"\n'
    'void AchievementList::initStatics() {\n'
    '    s_open = (new Achievement(0, "openInventory", 0, 0, Item::s_book, (Achievement)NULL))->registerAchievement();\n'
    '    s_wood = (new Achievement(1, "mineWood", 2, 1, Block::s_wood, s_open))->registerAchievement();\n'
    '    s_rail = (new Achievement(2, "onARail", 2, 3, Block::s_rail, s_wood))->setSpecial()->registerAchievement();\n'
    '}\n'
)


def _write(tmp_path):
    p = tmp_path / 'AchievementList.cpp'
    p.write_text(_SAMPLE, encoding='utf-8')
    return str(tmp_path)


def test_scan_finds_all_and_flags_special(tmp_path):
    a = ach.scan_achievements(_write(tmp_path))
    keys = [x['key'] for x in a]
    assert keys == ['openInventory', 'mineWood', 'onARail']
    special = {x['key']: x['hidden'] for x in a}
    assert special['onARail'] is True
    assert special['openInventory'] is False
    # parent captured for the child
    byk = {x['key']: x for x in a}
    assert byk['mineWood']['parent'] == 's_open'
    assert byk['openInventory']['parent'] is None       # (Achievement)NULL -> no parent


def test_humanize_names():
    assert ach._humanize('openInventory') == 'Open Inventory'
    assert ach._humanize('onARail') == 'On A Rail'
    assert ach._humanize('build_work_bench') == 'Build work bench'


def test_gamerscore_sums_exactly_to_budget(tmp_path):
    a = ach.scan_achievements(_write(tmp_path))
    m = ach.map_to_platform(a, profile='retail')
    assert m['total_score'] == 1000
    assert all(e['gamerscore'] % 5 == 0 for e in m['entries'])
    # arcade budget
    m2 = ach.map_to_platform(a, profile='arcade')
    assert m2['total_score'] == 200
    # 1-based platform ids
    assert [e['platform_id'] for e in m['entries']] == [1, 2, 3]


def test_arcade_count_limit_warns(tmp_path):
    many = [{'id': i, 'key': 'a%d' % i, 'name': 'A%d' % i, 'desc': 'd', 'hidden': False, 'parent': None}
            for i in range(20)]
    m = ach.map_to_platform(many, profile='arcade')
    assert any('exceeds' in w for w in m['warnings'])


def test_lang_names_override(tmp_path):
    lang = tmp_path / 'en_US.lang'
    lang.write_text('achievement.openInventory=Taking Inventory\n'
                    'achievement.openInventory.desc=Open your inventory\n', encoding='utf-8')
    a = ach.scan_achievements(_write(tmp_path), lang_file=str(lang))
    byk = {x['key']: x for x in a}
    assert byk['openInventory']['name'] == 'Taking Inventory'
    assert byk['openInventory']['desc'] == 'Open your inventory'


def test_run_emits_all_artifacts(tmp_path):
    code = _write(tmp_path)
    out = tmp_path / 'out'
    res = ach.run(code, str(out))
    assert os.path.isfile(res['report'])
    assert os.path.isfile(res['xlast'])
    h = open(os.path.join(str(out), 'Achievements.h'), encoding='utf-8').read()
    assert '#define ACH_OPEN_INVENTORY' in h
    assert 'Achievement_Unlock' in h
    cpp = open(os.path.join(str(out), 'Achievements.cpp'), encoding='utf-8').read()
    assert 'XUserWriteAchievements' in cpp               # online delivery -> real write
    xlast = open(res['xlast'], encoding='utf-8').read()
    assert 'hidden="true"' in xlast                       # onARail special -> hidden


def test_local_delivery_is_stub(tmp_path):
    res = ach.run(_write(tmp_path), str(tmp_path / 'out2'), delivery='local')
    cpp = open(os.path.join(str(tmp_path / 'out2'), 'Achievements.cpp'), encoding='utf-8').read()
    assert 'J2X-STUB[achievements-local]' in cpp
    assert 'XUserWriteAchievements' not in cpp


def _icon_project(tmp_path):
    """A code tree + Content atlas so an item icon can be auto-wired."""
    from PIL import Image
    code = tmp_path / 'src'
    code.mkdir()
    (code / 'AchievementList.cpp').write_text(
        'void AchievementList::initStatics() {\n'
        '    s_open = (new Achievement(0, "openInventory", 0, 0, Item::s_book, (Achievement)NULL))->registerAchievement();\n'
        '}\n', encoding='utf-8')
    # s_book sits at items.png (col 1, row 0)
    (code / 'Item.cpp').write_text(
        'void Item::initStatics() {\n'
        '    Item::s_book = (new Item(84))->setIconCoord(1, 0)->setItemName("book");\n'
        '}\n', encoding='utf-8')
    content = tmp_path / 'Content' / 'gui'
    content.mkdir(parents=True)
    atlas = Image.new('RGBA', (256, 256), (0, 0, 0, 0))
    # paint the (1,0) sprite red so we can tell it was cropped
    for y in range(16):
        for x in range(16, 32):
            atlas.putpixel((x, y), (200, 30, 30, 255))
    atlas.save(str(content / 'items.png'))
    return str(code), str(tmp_path / 'Content')


def test_icon_resolution(tmp_path):
    code, _content = _icon_project(tmp_path)
    amap = ach.resolve_icon_atlas(code)
    assert amap['s_book'] == ('items', 1, 0)


def test_block_texture_resolver(tmp_path):
    code = tmp_path / 'src'
    code.mkdir()
    # base Block(id, tex, mat) with a literal 2nd int + a subclass that sets blockIndexInTexture
    (code / 'Block.cpp').write_text(
        'void Block::initStatics() {\n'
        '    Block::s_wood = (new BlockLog(17))->setBlockName("log");\n'
        '    Block::s_rail = (new BlockRail(66, 128, false))->setBlockName("rail");\n'
        '    Block::s_stone = (new Block(1, 1, Material::s_rock))->setBlockName("stone");\n'
        '}\n', encoding='utf-8')
    (code / 'BlockLog.cpp').write_text(
        'BlockLog::BlockLog(int var1)\n : Block(var1, Material::s_wood)\n{\n'
        '    this->blockIndexInTexture = 20;\n}\n', encoding='utf-8')
    amap = ach.resolve_icon_atlas(str(code))
    assert amap['s_wood'] == ('terrain', 20 % 16, 20 // 16)     # subclass ctor literal
    assert amap['s_rail'] == ('terrain', 128 % 16, 128 // 16)   # 2nd ctor int
    assert amap['s_stone'] == ('terrain', 1, 0)                 # base Block(id, tex, mat)


def test_title_id_normalize_and_suggest():
    assert ach.normalize_title_id('0xFFFE1234') == 'FFFE1234'
    assert ach.normalize_title_id('fffe1234') == 'FFFE1234'
    assert ach.normalize_title_id('4d53080f') == '4D53080F'
    assert ach.normalize_title_id('nothex') is None
    # suggestion is stable, 8 hex digits, homebrew prefix
    s = ach.suggest_title_id('My Game')
    assert len(s) == 8 and s.startswith('FFFE')
    assert ach.suggest_title_id('My Game') == s               # deterministic


def test_run_writes_title_id(tmp_path):
    res = ach.run(_write(tmp_path), str(tmp_path / 'o'), title_id='FFFE00AB')
    assert res['title_id'] == 'FFFE00AB' and res['title_suggested'] is False
    xlast = open(res['xlast'], encoding='utf-8').read()
    assert 'titleId="0xFFFE00AB"' in xlast
    # omitted -> conventional 00000000 placeholder (the user self-issues via Suggest / --title-id)
    res2 = ach.run(_write(tmp_path), str(tmp_path / 'o2'), title_name='Some Game')
    assert res2['title_id'] == '00000000' and res2['title_suggested'] is False
    x2 = open(res2['xlast'], encoding='utf-8').read()
    assert 'titleId="0x00000000"' in x2


def test_wire_icons_crops_and_writes_tiles(tmp_path):
    code, content = _icon_project(tmp_path)
    out = tmp_path / 'out'
    res = ach.run(code, str(out), content_dir=content)
    assert res['icons']['wired'] == ['openInventory']
    from PIL import Image
    # images now land UNDER the deploy Content dir (never the project root), refs relative to out_dir.
    un = Image.open(os.path.join(str(content), 'achievement_images', 'openInventory_unlocked.png'))
    assert un.size == (64, 64)
    assert un.getpixel((32, 32))[:3] == (200, 30, 30)     # the red sprite was cropped
    lk = Image.open(os.path.join(str(content), 'achievement_images', 'openInventory_locked.png'))
    assert sum(lk.getpixel((32, 32))[:3]) < sum(un.getpixel((32, 32))[:3])   # locked is darker
    xlast = open(res['xlast'], encoding='utf-8').read()
    assert 'achievement_images' in xlast and 'openInventory_unlocked.png' in xlast
