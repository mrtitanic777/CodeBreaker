# -*- coding: utf-8 -*-
"""The C++03 back end: method bodies written from the typed IR, compiled into the transpiler's own
declarations, and judged beside the JVM. FIXED BUG tree-npe-catch: a try whose catch (Exception) a
NullPointerException reaches crashed (GameSettings.saveOptions); its dereferences are null-checked
(j2xNN) so the handler catches Java's exception."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.translate.backends import cpp03
from codebreaker.verify.lanes import java

SOURCE = '''import java.util.ArrayList;
import java.util.List;

public class Calc {
	public static int total;
	public static int[] table;
	private int[] cells = new int[8];

	static {
		table = new int[4];
		table[1] = 5;
		total = table[1] * 2;
	}
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

	public int guarded(int var1) {
		Calc var2 = var1 > 0 ? null : this;
		try {
			return var2.cells.length + var1;
		} catch (Exception var3) {
			return -7;
		}
	}

	public int cleanup(int var1) {
		int var2 = 0;
		try {
			var2 = 100 / (var1 & 3);
		} catch (ArithmeticException var3) {
			var2 = -1;
		} finally {
			total += 1;
		}
		return var2 + total;
	}

	public int search(int var1) {
		int var2 = 0;
		label9:
		for (int var3 = 0; var3 < 5; ++var3) {
			for (int var4 = 0; var4 < 5; ++var4) {
				if (var3 * var4 == (var1 & 15)) {
					var2 = var3 * 10 + var4;
					break label9;
				}
				if (var4 > var3) {
					continue label9;
				}
			}
		}
		return var2;
	}

	public static int sum(int... var0) {
		int var1 = 0;
		for (int var2 = 0; var2 < var0.length; ++var2) {
			var1 += var0[var2];
		}
		return var1;
	}

	public static int sums(int var0) {
		return sum() + sum(var0) + sum(var0, 2, 3) + sum(new int[]{var0, var0});
	}

	public static int tabled(int var0) {
		return table[var0 & 3] + total;
	}

	public static int made(int var0) {
		Base var1 = new Base(var0, 3);
		return var1.size + var1.scale;
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


BASE = '''public class Base {
	public int size;
	public int scale = 2;

	public Base(int var1) {
		this.size = var1 * this.scale;
	}

	public Base(int var1, int var2) {
		this(var1);
		this.scale += var2;
	}
}
'''

SUB = '''public class Sub extends Base {
	public int extra = 7;

	public Sub(int var1) {
		super(var1 + 1);
		this.extra += this.size;
	}

	public static int build(int var0) {
		Sub var1 = new Sub(var0);
		return var1.extra * 3 + var1.size;
	}
}
'''


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Calc.java').write_text(SOURCE, encoding='utf-8')
    (src / 'Base.java').write_text(BASE, encoding='utf-8')
    (src / 'Sub.java').write_text(SUB, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    s = transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out, s


@needs_tools
def test_bodies_come_from_the_tree(tree):
    _src, out, s = tree
    written, declined = s['tree_bodies']
    assert written >= 10 and not declined, (written, declined)
    sub = (out / 'Sub.cpp').read_text(encoding='utf-8')
    assert 'Sub::Sub(int var1)' in sub and ': Base(var1 + 1)' in sub    # super(args) as the base initialiser
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
    # guarded: a NullPointerException caught by catch (Exception)
    assert set(r['methods']) >= {'fill', 'mix', 'wrap', 'label', 'count', 'guarded', 'cleanup', 'search',
                                 'sums', 'tabled', 'made'}, (r['methods'], r['why'])
    sub = probe.run(staged, str(out), parse.parse_file(str(src / 'Sub.java')), str(tmp_path / 'work2'),
                    java.compiler(), javac, jvm)
    assert 'build' in sub['methods'] and sub['differs'] == [], (sub['why'], sub['differs'][:4])
    assert r['differs'] == [], r['differs'][:6]


def test_null_of_value_types():
    assert cpp03.null_of('Block*') == 'NULL'
    assert cpp03.null_of('const JavaString&') == 'JavaString()'
    assert cpp03.null_of('JList<JObject>') == 'JList<JObject>(0LL)'
    assert cpp03.null_of('JArray<int >') == 'JArray<int >()'
