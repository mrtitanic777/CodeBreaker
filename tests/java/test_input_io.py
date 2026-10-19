# -*- coding: utf-8 -*-
"""Beside the JVM: FIXED BUG keyname-stub -- Keyboard.getKeyName returned "", so GameSettings printed
"Forward: " for "Forward: W"; and FIXED BUG null-reader -- `new InputStreamReader(null)` did not throw, so
GuiMainMenu's constructor read on from a missing splashes.txt and crashed where Java caught the
NullPointerException and kept its "missingno" splash."""

import os

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify import java

SOURCE = '''import java.io.BufferedReader;
import java.io.InputStreamReader;
import org.lwjgl.input.Keyboard;

public class Inputs {
	public static String binding(int var0) {
		return "Forward: " + Keyboard.getKeyName(var0 & 255);
	}

	public static int index(int var0) {
		String var1 = Keyboard.getKeyName(var0 & 255);
		return var1 == null ? -1 : Keyboard.getKeyIndex(var1);
	}

	public static String splash(int var0) {
		String var1 = "missingno";
		try {
			BufferedReader var2 = new BufferedReader(new InputStreamReader(Inputs.class.getResourceAsStream("/title/none" + var0 + ".txt")));
			var1 = var2.readLine();
		} catch (Exception var3) {
		}
		return var1;
	}
}
'''


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]
                         and any('lwjgl-' in os.path.basename(j) for j in probe.libraries())),
                    reason='needs g++, a JDK and LWJGL in java_libs')
def test_inputs_beside_the_jvm(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Inputs.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Inputs.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'binding', 'index', 'splash'} and r['judged'] == 3, (r['methods'], r['why'])
    assert r['differs'] == [], r['differs'][:6]
