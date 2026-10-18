# -*- coding: utf-8 -*-
"""FIXED BUG finally-skipped-on-return: a `finally` body runs before every return/break/continue that
leaves its try (statements/controlflow.py _inline_finally). World.updatingLighting()'s depth counter
never came back down, so lighting stopped after 50 calls; ThreadDownloadImage never disconnected on
its early returns."""

import pytest

from codebreaker.java import ir, parse, probe, transpile
from codebreaker.java.emit.statements.controlflow import _inline_finally
from codebreaker.verify import java


def _lines(text):
    return text.strip('\n').split('\n')


def test_return_inside_try_runs_the_finally_first():
    out = _inline_finally(_lines('''
++depth;
try {
	if(a) {
		return true;
	}
	return false;
} finally {
	--depth;
}'''), ir.JType('boolean'))
    text = '\n'.join(l.strip() for l in out)
    assert text.count('--depth;') == 3                                   # both returns + the finally itself
    assert 'boolean j2xRet1 = true;\n--depth;\nreturn j2xRet1;' in text
    assert 'boolean j2xRet2 = false;\n--depth;\nreturn j2xRet2;' in text


def test_break_and_continue_that_leave_the_try():
    out = '\n'.join(l.strip() for l in _inline_finally(_lines('''
while(true) {
	try {
		for(int i = 0; i < 3; ++i) {
			break;
		}
		if(a) {
			continue;
		}
		break;
	} finally {
		--n;
	}
}'''), None))
    assert 'for(int i = 0; i < 3; ++i) {\nbreak;\n}' in out              # its loop is inside the try: as it was
    assert '{\n--n;\ncontinue;\n}' in out and '{\n--n;\nbreak;\n}' in out


def test_left_alone_where_java_does_not_run_the_finally_there():
    void = _inline_finally(_lines('''
try {
	Runnable r = new Runnable() {
		public void run() {
			return;
		}
	};
	return;
} finally {
	close();
}'''), None)
    text = '\n'.join(l.strip() for l in void)
    assert 'public void run() {\nreturn;\n}' in text                     # the anonymous class's own return
    assert '{\nclose();\nreturn;\n}' in text                             # the method's return
    jumping = _lines('try {\n\treturn 1;\n} finally {\n\treturn 2;\n}')
    assert _inline_finally(jumping, ir.JType('int')) == jumping          # a finally that jumps itself


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_the_counter_comes_back_down_beside_the_jvm(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Lighting.java').write_text('''public class Lighting {
	static int depth;

	static boolean step(int var0) {
		++depth;
		try {
			if(var0 > 0) {
				return true;
			}
			return false;
		} finally {
			--depth;
		}
	}

	public static int run(int var0) {
		for(int var1 = 0; var1 < var0; ++var1) {
			step(var1);
		}
		return depth;
	}
}
''', encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Lighting.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert 'run' in r['methods'] and r['cases'] > 0
    assert r['differs'] == [], r['differs']                              # depth is 0 after any number of calls
