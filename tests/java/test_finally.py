# -*- coding: utf-8 -*-
"""FIXED BUG finally-skipped-on-return: a `finally` body runs before every return/break/continue that
leaves its try (the back end's finally_with_jumps). World.updatingLighting()'s depth counter
never came back down, so lighting stopped after 50 calls; ThreadDownloadImage never disconnected on
its early returns."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify import java


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
