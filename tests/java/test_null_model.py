# -*- coding: utf-8 -*-
"""Null Strings, lists, maps and sets: FIXED BUG null-is-empty -- a String or List is held by value, and null used to
be the empty one, so "" == null (NBTTagString threw on every empty sign line, TileEntitySign could not be
saved), a never-set String printed "" where Java prints "null" (NBTTagString.toString), and a missing map
entry came back as an empty list that was not null (SoundPool went on to get() from it). Each now carries a
null state of its own: set by null and by never being given a value, cleared by any content."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify import java

SOURCE = '''import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Nulls {
	public String value;
	private Map byName = new HashMap();

	public Nulls() {
	}

	public Nulls(String var1) {
		this.value = var1;
		if(var1 == null) {
			throw new IllegalArgumentException("Empty string not allowed");
		}
	}

	public String toString() {
		return "" + this.value;
	}

	public static boolean emptyIsNotNull(int var0) {
		String var1 = var0 > 0 ? "" : null;
		return var1 == null;
	}

	public static int emptySaved(int var0) {
		String[] var1 = new String[]{"", "", "", ""};
		return new Nulls(var1[var0 & 3]).toString().length();
	}

	public static String unset(int var0) {
		Nulls var1 = new Nulls();
		return var1.toString() + var0;
	}

	public static boolean slotIsNull(int var0) {
		String[] var1 = new String[4];
		return var1[var0 & 3] == null;
	}

	public int lookup(int var1) {
		if(var1 > 0) {
			this.byName.put("a", new ArrayList());
		}
		List var2 = (List)this.byName.get("a");
		return var2 == null ? -1 : var2.size();
	}

	public static boolean nullMap(int var0) {
		Map var1 = var0 > 0 ? new HashMap() : null;
		return var1 == null;
	}

	public static boolean nullSet(int var0) {
		Set var1 = var0 > 0 ? new HashSet() : null;
		return var1 == null;
	}

	public int nested(int var1) {
		if(var1 > 0) {
			this.byName.put("m", new HashMap());
		}
		Map var2 = (Map)this.byName.get("m");
		return var2 == null ? -1 : var2.size();
	}

	public static boolean nullList(int var0) {
		List var1 = var0 > 0 ? new ArrayList() : null;
		return var1 == null;
	}
}
'''


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Nulls.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_nulls_beside_the_jvm(tree, tmp_path):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Nulls.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    wanted = {'emptyIsNotNull', 'emptySaved', 'unset', 'slotIsNull', 'lookup', 'nullList', 'nullMap', 'nullSet', 'nested'}
    assert set(r['methods']) >= wanted and r['differs'] == [], (r['methods'], r['differs'][:6])
