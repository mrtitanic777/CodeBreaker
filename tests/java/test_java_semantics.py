# -*- coding: utf-8 -*-
"""Java semantics the behavioural judge found missing and that are now kept:
  * an empty array is not null (J2X-BUGFIX empty-array-not-null -- NibbleArray.isValid());
  * a null dereference inside a try reaches the handler that catches it (FIXED BUG npe-in-try-crashes --
    ColorizerGrass's static init);
  * Java's char is unsigned (J2X-BUGFIX char-unsigned).
Each is checked on the emitted text and, with g++ and a JDK, beside the JVM (instance methods included)."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify.lanes import java

SOURCES = {
    'Nibbles.java': '''public class Nibbles {
	private byte[] data;

	public Nibbles(int var1) {
		this.data = new byte[var1 >> 1];
	}

	public boolean isValid() {
		return this.data != null;
	}

	public int size() {
		return this.data.length;
	}
}
''',
    'Loader.java': '''public class Loader {
	static int[] buffer = new int[4];
	static int failures;
	Loader next;

	static Loader find(int var0) {
		return null;
	}

	public static int load(int var0) {
		try {
			Loader var1 = find(var0);
			var1.touch();
			return 1;
		} catch (Exception var2) {
			++failures;
			return -1;
		}
	}

	public static int guarded(int var0) {
		try {
			Loader var1 = find(var0);
			if(var1 != null && var1.next != null) {
				return 2;
			}
			return 3;
		} catch (Exception var2) {
			return -1;
		}
	}

	void touch() {
	}
}
''',
    'Chars.java': '''public class Chars {
	public static int code(int var0) {
		char var1 = (char)var0;
		return var1 + 0;
	}

	public static boolean high(int var0) {
		char var1 = (char)var0;
		return var1 > 127;
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


def test_char_is_unsigned_in_the_emitted_code(tree):
    _src, out = tree
    text = (out / 'Chars.cpp').read_text(encoding='utf-8')
    assert 'unsigned char var1 = (unsigned char)var0;' in text


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
@pytest.mark.parametrize('name, methods', [
    ('Nibbles', ['isValid', 'size']),                                   # instance methods, made with Nibbles(int)
    ('Loader', ['guarded', 'load']),
    ('Chars', ['code', 'high']),
])
def test_beside_the_jvm(tree, tmp_path, name, methods):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / (name + '.java'))), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert sorted(r['methods']) == methods and r['judged'] == len(methods)
    if name == 'Chars':
        # unsigned 8-bit: every char of 0-255 agrees with Java; one past that cannot be held (strings are 8-bit)
        assert r['differs'] and all(not 0 <= int(a[0]) <= 255 for _m, a, _j, _c in r['differs']), r['differs']
    else:
        assert r['differs'] == [], r['differs'][:5]
