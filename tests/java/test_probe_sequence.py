# -*- coding: utf-8 -*-
"""The probe's cases share one program: FIXED BUG probe-skipped-state -- a case that threw in Java was left
out of the C++ run, but it had already changed static state before it threw (an Item's constructor
registers it in Item.itemsList), so a later case found the slot filled in Java and empty in C++, and C++
crashed (ItemSword.hitBlock -> ItemStack.getMaxDamage). The original is now run again on exactly the cases
C++ runs, and that is what C++ is compared with. And FIXED BUG discarded-new-escapes: a bare `new Reg(x);`
became `delete new Reg(x);`, freeing an object its constructor had just put in Reg.list."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify.lanes import java

SOURCE = '''public class Reg {
	public static Reg[] list = new Reg[8];
	public int id;

	public Reg(int var1) {
		list[var1 & 7] = this;
		this.id = var1;
		if(var1 < 0) {
			throw new IllegalArgumentException("negative");
		}
	}

	public static int make(int var0) {
		new Reg(var0);
		return var0;
	}

	public static int peek(int var0) {
		return list[var0 & 7].id;
	}
}
'''


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_cases_left_out_do_not_leave_state_behind(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Reg.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path / 'cpp'
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Reg.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'make', 'peek'} and r['judged'] == 2, r
    assert r['differs'] == [], r['differs'][:4]
