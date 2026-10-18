# -*- coding: utf-8 -*-
"""J2X-BUGFIX float-int-cast-saturate: Java's (int)/(long) of a float or double saturates (NaN -> 0) where
C++'s is undefined out of range. Every (int)x / (long long)x in an emitted class becomes j2xToInt(x) /
j2xToLong(x) (expr.java_int_casts, JavaRT.h). Found by the behavioural judge: MathHelper.sin(1000000F)."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.java.emit.statements.expr import java_int_casts
from codebreaker.verify import java


@pytest.mark.parametrize('cpp, want', [
    ('return s_SIN_TABLE[(int)(var0 * 10430.378F) & 0xffff];',
     'return s_SIN_TABLE[j2xToInt((var0 * 10430.378F)) & 0xffff];'),
    ('int a = (int)ceilf((double)(var1 + 2));', 'int a = j2xToInt(ceilf((double)(var1 + 2)));'),
    ('x = (int)-v->y[i] + (int)this->a.b(c);', 'x = j2xToInt(-v->y[i]) + j2xToInt(this->a.b(c));'),
    ('long long z = (long long)(float)q;', 'long long z = j2xToLong((float)q);'),
    ('int w = (int)Math::floor(d) * 2;', 'int w = j2xToInt(Math::floor(d)) * 2;'),
    ('printf("(int)x"); // (int)y', 'printf("(int)x"); // (int)y'),          # literals and comments as they are
    ('char c = (char)i;', 'char c = (char)i;'),                              # other casts as they are
])
def test_casts_are_rewritten(cpp, want):
    assert java_int_casts(cpp) == want


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_out_of_range_casts_saturate_like_java(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Casts.java').write_text('''public class Casts {
	public static int toInt(float var0) {
		return (int)(var0 * 10430.378F);
	}

	public static int toIntD(double var0) {
		return (int)(var0 * 1.0E10D);
	}

	public static long toLong(double var0) {
		return (long)(var0 * 1.0E19D);
	}
}
''', encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    assert 'j2xToInt(' in (out / 'Casts.cpp').read_text(encoding='utf-8')
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Casts.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert r['judged'] == 3 and r['differs'] == [], r['differs']         # 1000000F, -1000.75, ... saturate alike
