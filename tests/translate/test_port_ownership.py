# -*- coding: utf-8 -*-
"""The no-GC ownership cleanups on bodies the back end writes (ported from test_beta_fixes, whose
versions rendered the old text translator's bodies). RubyDung's leaks on the 360:
  * a builder returning a container of fresh heap objects the caller iterates and drops
    (Level.getCubes -> new AABBs, Entity.move): an RAII element-deleter after the call -- only when the
    elements are used transiently;
  * an owned scalar built inline as an argument (`sink.take(bb.expand(1))`): hoisted and RAII-deleted --
    only when the receiver provably just reads it;
  * an owned scalar member reassigned (Entity.setPos -> new AABB): delete-before-reassign -- only for
    owned members, once."""

import re

import pytest

from codebreaker.java import transpile

SOURCES = {
    'AABB.java': '''public class AABB {
	public float x0;

	public AABB(float var1) {
		this.x0 = var1;
	}

	public AABB expand(float var1) {
		return new AABB(this.x0 + var1);
	}

	public float clipYCollide(AABB var1, float var2) {
		return var2 + var1.x0;
	}
}
''',
    'Level.java': '''import java.util.ArrayList;

public class Level {
	public ArrayList<AABB> getCubes(AABB var1) {
		ArrayList<AABB> var2 = new ArrayList<AABB>();
		var2.add(new AABB(var1.x0));
		return var2;
	}
}
''',
    'Reader.java': '''public class Reader {
	public int take(AABB var1) {
		return (int)var1.x0;
	}
}
''',
    'Keeper.java': '''public class Keeper {
	public AABB kept;

	public int keep(AABB var1) {
		this.kept = var1;
		return 0;
	}
}
''',
    'Entity.java': '''import java.util.ArrayList;

public class Entity {
	public Level level;
	public AABB bb = new AABB(0.0F);
	public AABB sel;
	public Reader reader = new Reader();
	public Keeper keeper = new Keeper();

	public Entity(Level var1) {
		this.level = var1;
	}

	public float transientUse() {
		ArrayList<AABB> var1 = this.level.getCubes(this.bb);
		float var2 = 0.0F;
		for (int var3 = 0; var3 < var1.size(); ++var3) {
			var2 = ((AABB)var1.get(var3)).clipYCollide(this.bb, var2);
		}
		return var2;
	}

	public void retained() {
		ArrayList<AABB> var1 = this.level.getCubes(this.bb);
		this.sel = (AABB)var1.get(0);
	}

	public int readArg() {
		return this.reader.take(this.bb.expand(1.0F));
	}

	public int keepArg() {
		return this.keeper.keep(this.bb.expand(1.0F));
	}

	public int captured() {
		AABB var1 = this.bb.expand(1.0F);
		return this.reader.take(var1);
	}

	public void setPos(float var1) {
		this.bb = new AABB(var1);
	}

	public void rebind(Level var1) {
		this.level = var1;
	}
}
''',
}


@pytest.fixture(scope='module')
def entity(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for name, text in SOURCES.items():
        (src / name).write_text(text, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return (out / 'Entity.cpp').read_text(encoding='utf-8')


def _body(text, name):
    m = re.search(r'\n\w[^\n]*\bEntity::%s\(.*?\n}\n' % name, text, re.S)
    assert m, name
    return m.group(0)


def test_transient_elements_are_freed(entity):
    assert 'J2X_ScopedFreeElems<' in _body(entity, 'transientUse'), _body(entity, 'transientUse')


def test_retained_elements_are_not(entity):
    assert 'J2X_ScopedFreeElems' not in _body(entity, 'retained')


def test_inline_owned_argument_is_hoisted_when_only_read(entity):
    body = _body(entity, 'readArg')
    assert 'J2X_ScopedDelete<AABB >' in body and '= this->bb->expand(1.0F);' in body, body


def test_inline_owned_argument_kept_when_retained_or_captured(entity):
    assert 'J2X_ScopedDelete' not in _body(entity, 'keepArg')
    assert 'J2X_ScopedDelete' not in _body(entity, 'captured')


def test_owned_member_reassign_deletes_once(entity):
    body = _body(entity, 'setPos')
    assert body.count('delete this->bb;') == 1 and 'this->bb = new AABB(var1);' in body, body
    assert 'delete this->level;' not in _body(entity, 'rebind')      # borrowed: set from a parameter
