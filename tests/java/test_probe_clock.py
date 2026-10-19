# -*- coding: utf-8 -*-
"""A method whose result depends on the clock is not compared: FIXED BUG probe-clock-noise -- Timer was
UNVERIFIED on every run because the time differs between the original's run and the C++ one, which two
runs of the original (the steadiness check) cannot see when each run is steady by itself."""

from codebreaker.java import parse, probe

SOURCE = '''public class Clocky {
	private long start;
	public int ticks;

	public Clocky() {
		this.start = System.currentTimeMillis();
	}

	private long now() {
		return System.nanoTime() / 1000000L;
	}

	public int elapsed() {
		return (int)(this.now() - this.start);
	}

	public static int pure(int var0) {
		return var0 * 2;
	}
}
'''

PLAIN = '''public class Plain {
	public int v;

	public Plain(int var1) {
		this.v = var1;
	}

	public int get() {
		return this.v;
	}
}
'''


def _cls(tmp_path, name, text):
    p = tmp_path / (name + '.java')
    p.write_text(text, encoding='utf-8')
    return parse.parse_file(str(p))


def test_clock_methods_and_constructor(tmp_path):
    clocked, in_ctor = probe.reads_clock(_cls(tmp_path, 'Clocky', SOURCE))
    assert clocked == {'now', 'elapsed'} and in_ctor


def test_only_the_static_pure_method_is_probed(tmp_path):
    cls = _cls(tmp_path, 'Clocky', SOURCE)
    header = 'class Clocky { public: Clocky(); virtual int elapsed(); static int pure(int var0); };'
    assert [m[0] for m in probe.probeable(cls, header)] == ['pure']


def test_a_class_without_the_clock(tmp_path):
    assert probe.reads_clock(_cls(tmp_path, 'Plain', PLAIN)) == (set(), False)


CLIP = '''public class Clip {
	public static String text() {
		try {
			return (String)java.awt.Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null).getTransferData(null);
		} catch (Exception var0) {
			return null;
		}
	}
}
'''


def test_the_clipboard_is_the_machine_not_the_code(tmp_path):
    # FIXED BUG probe-clipboard-noise: GuiScreen.getClipboardString differed whenever this PC's clipboard held text
    assert probe.reads_clock(_cls(tmp_path, 'Clip', CLIP)) == ({'text'}, False)
