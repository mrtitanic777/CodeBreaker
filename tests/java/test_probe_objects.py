# -*- coding: utf-8 -*-
"""The behavioural judge beyond primitives: Strings in and out, objects made as arguments (by constructor
or static factory) and compared as results by their fields, a void method compared by its object
afterwards -- and the bug it found with them, J2X-BUGFIX java-number-text (Vec3D.toString())."""

import pytest

from codebreaker.java import probe, transpile
from codebreaker.verify.lanes import java

SOURCES = {
    'Point.java': '''package geo;

public class Point {
	public double x;
	public double y;
	String label = "p";

	private Point(double var1, double var3) {
		this.x = var1;
		this.y = var3;
	}

	public static Point of(double var0, double var2) {
		return new Point(var0, var2);
	}

	public Point add(Point var1) {
		return new Point(this.x + var1.x, this.y + var1.y);
	}

	public void scale(double var1) {
		this.x *= var1;
		this.y *= var1;
	}

	public String toString() {
		return "(" + this.x + ", " + this.y + ")";
	}

	public String named(String var1) {
		return var1.toUpperCase() + ":" + this.x;
	}

	public static int length(String var0) {
		return var0.length();
	}
}
''',
}


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for name, text in SOURCES.items():
        (src / name).write_text(text, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out


def test_what_can_be_called(tree):
    src, out = tree
    staged = probe.stage_sources(str(src), str(out.parent / 'staged'))
    cls = probe.lookup(staged, 'Point')
    assert probe.maker(cls) == ('of', ['double', 'double'])              # private ctor: the static factory
    header = (out / 'Point.h').read_text(encoding='utf-8')
    found = {m[0]: m for m in probe.probeable(cls, header, staged, str(out))}
    assert set(found) >= {'of', 'add', 'scale', 'toString', 'named', 'length'}
    assert found['add'][1] == [('obj', 'Point', ('of', ['double', 'double']))]   # an object argument
    assert found['scale'][2][0] == 'self'                               # void: the object afterwards
    assert found['length'][1] == ['String']


def test_cases_never_list_every_combination():
    many = probe.cases(['double'] * 8)                                   # 12^8 combinations
    assert len(many) <= probe._MAX_CASES + 8 * 12 and len(set(many)) == len(many)


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_strings_objects_and_number_text_beside_the_jvm(tree, tmp_path):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), probe.lookup(staged, 'Point'), str(tmp_path / 'work'), java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'add', 'scale', 'toString', 'named', 'length'}
    # toString prints doubles: "(0.0, 1000000.0)" in Java -- C++ streams gave "(0, 1e+06)"
    assert r['differs'] == [], r['differs'][:4]
    assert r['judged'] >= 6
