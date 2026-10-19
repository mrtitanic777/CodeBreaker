# -*- coding: utf-8 -*-
"""The java kind's behavioural judge (codebreaker/java/probe.py): a class's static methods run in the JVM
and natively on the same inputs, results compared exactly."""

import os

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify import java

SOURCES = {
    'Calc.java': '''package demo;

public class Calc {
	private static float[] TABLE = new float[256];

	public static int clamp(int var0, int var1) {
		return var0 < 0 ? 0 : (var0 > var1 ? var1 : var0);
	}

	public static float wave(float var0) {
		return TABLE[(int)(var0 * 40.743664F) & 255];
	}

	static double half(double var0) {
		return var0 / 2.0D;
	}

	public static long mix(long var0, int var2) {
		return var0 * 31L + (long)var2;
	}

	public static int widen(int var0) {
		return (char)var0;
	}

	public int notStatic(int var1) {
		return var1;
	}

	static {
		for(int var0 = 0; var0 < 256; ++var0) {
			TABLE[var0] = (float)Math.sin((double)var0 * Math.PI * 2.0D / 256.0D);
		}
	}
}
''',
}


def test_cases_are_deterministic_and_cover_every_value():
    a, b = probe.cases(['int', 'float']), probe.cases(['int', 'float'])
    assert a == b and 0 < len(a) <= probe._MAX_CASES + 40
    assert set(v for v, _ in a) == set(probe._INPUTS['int'][1])
    assert set(v for _, v in a) == set(probe._INPUTS['float'][1])


def test_floats_compare_by_bits_and_nan_matches_nan():
    assert probe.same('float', '3f800000', '3f800000')
    assert not probe.same('float', '3f800000', '3f800001')                       # one ulp is a difference
    assert probe.same('float', '7fc00000', 'ffc00001')                          # NaN == NaN
    assert not probe.same('int', '1', '2')


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_methods_run_beside_the_jvm(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    for name, text in SOURCES.items():
        (src / name).write_text(text, encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    assert os.path.isfile(os.path.join(staged, 'demo', 'Calc.java'))           # laid out by package
    cls = parse.parse_file(str(src / 'Calc.java'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), cls, str(tmp_path / 'work'), java.compiler(), javac, jvm)
    # primitive in and out: the static ones, and notStatic on a Calc made with its implicit constructor
    assert sorted(r['methods']) == ['clamp', 'half', 'mix', 'notStatic', 'wave', 'widen']
    assert r['judged'] == 6 and r['total'] == 6 and r['cases'] > 50
    differs = set(m for m, _a, _j, _c in r['differs'])
    # wave reads a table built with Math.PI: equal only because PI is the double Java uses
    assert 'wave' not in differs and 'clamp' not in differs and 'mix' not in differs and 'half' not in differs
    # Java's char is 16 bits and unsigned; here it is unsigned char: (char)255 now agrees, (char)65535 cannot
    widen = [a for m, a, _j, _c in r['differs'] if m == 'widen']
    assert ('255',) not in widen and ('65535',) in widen


def test_a_missing_class_is_read_from_every_kind_of_linker_complaint():
    err = ("a.o: undefined reference to `vtable for MaterialLogic'\n"
           "b.o: undefined reference to `Block::foo()'\n"
           "c.o: undefined reference to `typeinfo for Item'\n")
    assert probe._missing(err) == {'MaterialLogic', 'Block', 'Item'}


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_a_result_java_itself_does_not_repeat_is_left_out(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Dice.java').write_text('''public class Dice {
	public static int roll(int var0) {
		return (int)(System.nanoTime() % 1000L) + var0 * 0;
	}

	public static int twice(int var0) {
		return var0 * 2;
	}
}
''', encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Dice.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert r['unsteady'] > 0 and r['differs'] == []                     # roll is left out, twice agrees
    assert r['judged'] == 1
