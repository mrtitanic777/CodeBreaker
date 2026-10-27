# -*- coding: utf-8 -*-
"""String and regex runtime beside the JVM. FIXED BUGs: regex-stub (Pattern/Matcher always failed, so
every J_JsonNumberNode threw), replaceall-noop (SoundPool's names kept their '/'), split-trailing (Java
drops trailing empty strings), string-compare (compareTo gave -1/0/1, hashCode summed signed bytes:
J_JsonStringNode), md5-stub (MD5String printed ""). Known limit: length() counts bytes, not UTF-16
chars, so MD5String's update(s.getBytes(), 0, s.length()) differs from Java past ASCII; md5 here hashes
ASCII text."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify.lanes import java

SOURCE = '''import java.math.BigInteger;
import java.security.MessageDigest;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Texts {
	private static final Pattern NUMBER = Pattern.compile("(-?)(0|([1-9]([0-9]*)))(\\\\.[0-9]+)?((e|E)(\\\\+|-)?[0-9]+)?");
	private static final Pattern CHUNK = Pattern.compile("c\\\\.(-?[0-9a-z]+)\\\\.(-?[0-9a-z]+)\\\\.dat");

	public static boolean isNumber(String var0) {
		return NUMBER.matcher(var0).matches();
	}

	public static String chunk(String var0) {
		Matcher var1 = CHUNK.matcher("c." + var0 + ".dat");
		return var1.matches() ? var1.group(1) + "|" + var1.group(2) : "none";
	}

	public static String dots(String var0) {
		return var0.replaceAll("/", ".") + var0.replaceAll("[aeiou]+", "<$0>");
	}

	public static int pieces(String var0) {
		return var0.split(":").length * 100 + var0.split(" ").length;
	}

	public static int compare(String var0, String var1) {
		return var0.compareTo(var1);
	}

	public static int hash(String var0) {
		return ("\\u00a7c" + var0).hashCode();
	}

	public static String md5(int var0) throws Exception {
		String var2 = "MD5String" + var0;
		MessageDigest var1 = MessageDigest.getInstance("MD5");
		var1.update(var2.getBytes(), 0, var2.length());
		return (new BigInteger(1, var1.digest())).toString(16);
	}
}
'''


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Texts.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_strings_beside_the_jvm(tree, tmp_path):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Texts.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    wanted = {'isNumber', 'chunk', 'dots', 'pieces', 'compare', 'hash', 'md5'}
    assert set(r['methods']) >= wanted and r['judged'] == len(r['methods']), (r['methods'], r['why'])
    assert r['differs'] == [], r['differs'][:6]
