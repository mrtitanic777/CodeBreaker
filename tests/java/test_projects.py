# -*- coding: utf-8 -*-
"""The project layer (codebreaker/java/projects; docs/TRANSLATOR_DESIGN.md phase 3): a program that is not
Minecraft converts with no Minecraft code loaded; Minecraft is found by its marks and brings its own."""

import os
import subprocess
import sys
import textwrap

import pytest

from codebreaker.java import parse, projects
from codebreaker.translate.backends import cpp03
from codebreaker.verify.lanes import java

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

PLAIN = '''public class Ledger {
	private int total;

	public int add(int var1) {
		this.total += var1;
		return this.total;
	}

	public String label() {
		return "total=" + this.total;
	}
}
'''


@pytest.mark.skipif(not java.jdk()[0], reason='needs a JDK')
def test_a_plain_program_loads_no_minecraft_code(tmp_path):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Ledger.java').write_text(PLAIN, encoding='utf-8')
    script = textwrap.dedent('''
        import sys
        from codebreaker.java import transpile, profiles
        transpile.convert(sys.argv[1], sys.argv[2], platform=None, write_report=False)
        loaded = sorted(m for m in sys.modules if 'minecraft' in m.lower())
        print('LOADED', loaded)
        print('PROFILE', profiles.active().name)
    ''')
    env = dict(os.environ, PYTHONPATH=ROOT)
    r = subprocess.run([sys.executable, '-c', script, str(src), str(tmp_path / 'out')],
                       capture_output=True, text=True, env=env, cwd=ROOT, timeout=600)
    assert r.returncode == 0, r.stderr[-2000:]
    assert 'LOADED []' in r.stdout, r.stdout
    assert 'PROFILE generic' in r.stdout, r.stdout
    assert (tmp_path / 'out' / 'Ledger.cpp').is_file()


def test_minecraft_is_found_by_its_marks_and_options():
    alpha = [parse.parse_source('public class Minecraft {}\n')]
    assert projects.choose(alpha).name == 'minecraft'
    rd = [parse.parse_source('package com.mojang.rubydung;\npublic class RubyDung {}\n')]
    assert projects.choose(rd).name == 'minecraft'
    plain = [parse.parse_source(PLAIN)]
    assert projects.choose(plain) is projects.GENERIC
    assert projects.choose(plain, edition='beta').name == 'minecraft'          # an option it claims
    assert projects.choose(plain, project='generic') is projects.GENERIC
    with pytest.raises(ValueError):
        projects.GENERIC.profile_for(edition='beta')


def test_minecraft_profiles_load_with_the_project():
    from codebreaker.java import profiles
    assert profiles.get('minecraft_alpha').name == 'minecraft_alpha'
    mc = projects.load('minecraft')
    assert mc.profile_for(edition='indev') == 'minecraft_indev'
    assert mc.profile_for(classes=[]) == 'minecraft_xbox360'                  # era unknown: the bundle


def test_the_font_glyph_is_minecrafts():
    mc = projects.load('minecraft')
    assert cpp03._cstring(u'~⌂', mc.char_map) == r'"~\177"'               # its font's tile 127
    assert cpp03._cstring(u'~⌂') == '"~?"'                                 # anyone else's: Latin-1
