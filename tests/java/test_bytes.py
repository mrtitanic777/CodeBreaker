# -*- coding: utf-8 -*-
"""Java's byte (signed char here): FIXED BUG byte-fields-uninitialised -- every constructor zeroes byte
fields, as Java does (Packet24MobSpawn.type, Packet34EntityTeleport.yaw were garbage on a fresh packet) --
and a byte in string concatenation prints its number, not a character (NBTTagByte.toString())."""

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify.lanes import java

SOURCE = '''public class Packet {
	public int entityId;
	public byte type;
	public byte yaw;

	public Packet() {
	}

	public String describe() {
		return "type " + this.type + ", yaw " + this.yaw;
	}

	public static String show(int var0) {
		byte var1 = (byte)var0;
		return "" + var1;
	}
}
'''


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Packet.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out


def test_byte_fields_are_zeroed_by_the_constructor(tree):
    _src, out = tree
    text = (out / 'Packet.cpp').read_text(encoding='utf-8')
    ctor = text[text.index('Packet::Packet()'):text.index('{', text.index('Packet::Packet()'))]
    assert 'type(0)' in ctor and 'yaw(0)' in ctor and 'entityId(0)' in ctor


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_bytes_beside_the_jvm(tree, tmp_path):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Packet.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'describe', 'show'} and r['differs'] == [], r['differs'][:4]
