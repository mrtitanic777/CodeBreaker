# -*- coding: utf-8 -*-
"""Tests for the skeleton language emitters (j2x.scaffold)."""

import os
import pytest
from j2x import scaffold


_JAVA = (
    'public class Player extends Entity {\n'
    '    public int health;\n'
    '    private String name;\n'
    '    public Player(int health, String name) { this.health = health; }\n'
    '    public int getHealth() { return this.health; }\n'
    '    public void damage(int amount) { this.health -= amount; }\n'
    '    public static Player create(String name) { return new Player(20, name); }\n'
    '}\n'
)


def _src(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Player.java').write_text(_JAVA, encoding='utf-8')
    return str(src)


def test_c_skeleton(tmp_path):
    out = str(tmp_path / 'c')
    r = scaffold.run(_src(tmp_path), out, 'c')
    assert r['classes'] == 1 and 'Player.h' in r['written']
    h = open(os.path.join(out, 'Player.h'), encoding='utf-8').read()
    assert 'typedef struct Player' in h
    assert 'int health;' in h and 'const char* name;' in h
    assert 'int Player_getHealth(Player* self);' in h
    assert 'Player* Player_create(const char* name);' in h        # static -> no self


def test_csharp_skeleton(tmp_path):
    out = str(tmp_path / 'cs')
    scaffold.run(_src(tmp_path), out, 'csharp')
    cs = open(os.path.join(out, 'Player.cs'), encoding='utf-8').read()
    assert 'public class Player : Entity' in cs                    # base class carried
    assert 'public static Player create(string name)' in cs
    assert 'NotImplementedException' in cs


def test_rust_skeleton(tmp_path):
    out = str(tmp_path / 'rs')
    scaffold.run(_src(tmp_path), out, 'rust')
    rs = open(os.path.join(out, 'Player.rs'), encoding='utf-8').read()
    assert 'pub struct Player' in rs
    assert 'pub health: i32,' in rs
    assert 'pub fn get_health(&self) -> i32' in rs                 # name: type order, snake_case
    assert 'pub fn create(name: String) -> Player' in rs          # static -> no &self
    assert 'pub fn new(health: i32, name: String)' in rs


def test_readme_written(tmp_path):
    out = str(tmp_path / 'c')
    scaffold.run(_src(tmp_path), out, 'c')
    assert os.path.isfile(os.path.join(out, 'SKELETON_C.md'))


def test_unknown_language_raises(tmp_path):
    with pytest.raises(ValueError):
        scaffold.run(_src(tmp_path), str(tmp_path / 'x'), 'haskell')
