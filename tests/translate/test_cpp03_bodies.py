# -*- coding: utf-8 -*-
"""Phase 1's C++03 back end: method bodies written from the typed IR (CB_TREE_BODIES=1), compiled into
the transpiler's own declarations, and judged beside the JVM."""

import os

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.translate.backends import cpp03
from codebreaker.verify import java

SOURCE = '''import java.util.ArrayList;
import java.util.List;

public class Calc {
	public static int total;
	private int[] cells = new int[8];
	private List items = new ArrayList();
	public String name = "calc";

	public Calc() {
	}

	public int fill(int var1) {
		int var2 = 0;
		for (int var3 = 0; var3 < this.cells.length; ++var3) {
			this.cells[var3] = var1 * var3 - (var3 >>> 1);
			var2 += this.cells[var3];
		}
		total += var2;
		return var2 > 100 ? var2 % 7 : -var2;
	}

	public static long mix(long var0, int var2) {
		long var3 = var0 ^ (long)var2 << 3;
		while (var3 > 1000L) {
			var3 >>>= 2;
		}
		return var3 >>> 1;
	}

	public static float wrap(float var0) {
		return var0 % 3.5F + (float)Math.floor((double)var0);
	}

	public String label(int var1) {
		String var2 = var1 < 0 ? null : this.name + ":" + var1;
		return var2 == null ? "none" : var2 + (char)(65 + (var1 & 7));
	}

	public int count(int var1) {
		this.items.add(this.name);
		switch (var1 & 3) {
			case 0:
				return this.items.size();
			case 1:
				this.items.clear();
			default:
				return -1;
		}
	}
}
'''

needs_tools = pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Calc.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    old = os.environ.get('CB_TREE_BODIES')
    os.environ['CB_TREE_BODIES'] = '1'
    try:
        s = transpile.convert(str(src), str(out), platform=None, write_report=False)
    finally:
        if old is None:
            os.environ.pop('CB_TREE_BODIES')
        else:
            os.environ['CB_TREE_BODIES'] = old
    return src, out, s


@needs_tools
def test_bodies_come_from_the_tree(tree):
    _src, out, s = tree
    written, declined = s['tree_bodies']
    assert written >= 4, (written, declined)
    text = (out / 'Calc.cpp').read_text(encoding='utf-8')
    assert 'static_cast<int>(static_cast<unsigned int>(' in text   # >>> as an unsigned shift, wrapping
    assert 'fmod(' in text                            # float % as fmod


@needs_tools
def test_tree_bodies_beside_the_jvm(tree, tmp_path):
    src, out, _s = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Calc.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'fill', 'mix', 'wrap', 'label', 'count'}, (r['methods'], r['why'])
    assert r['differs'] == [], r['differs'][:6]


def test_null_of_value_types():
    assert cpp03.null_of('Block*') == 'NULL'
    assert cpp03.null_of('const JavaString&') == 'JavaString()'
    assert cpp03.null_of('JList<JObject>') == 'JList<JObject>(0LL)'
    assert cpp03.null_of('JArray<int >') == 'JArray<int >()'
