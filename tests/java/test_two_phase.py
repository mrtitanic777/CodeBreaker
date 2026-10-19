# -*- coding: utf-8 -*-
"""Two-phase construction: FIXED BUG virtual-call-in-constructor -- Java dispatches a virtual call made in
a constructor to the subclass's override (Block's constructor reads isOpaqueCube() into opaqueCubeLookup);
C++ reached the base's version, so every transparent block came out opaque. A class like that now gets a
field-zeroing C() and a j2xInit(...) per Java constructor, and `new C(args)` is `(new C())->j2xInit(args)`."""

import pytest

from codebreaker.java import parse, probe, transpile, twophase
from codebreaker.verify import java

BLOCK = '''public class Block {
	public static final boolean[] opaque = new boolean[16];
	public final int blockID;
	public int light = 3;

	protected Block(int var1) {
		this.blockID = var1;
		opaque[var1] = this.isOpaqueCube();
		this.light = this.lightValue() + this.light;
	}

	protected Block(int var1, int var2) {
		this(var1);
		this.light += var2;
	}

	public boolean isOpaqueCube() {
		return true;
	}

	public int lightValue() {
		return 0;
	}

	public static boolean opaqueAt(int var0) {
		return opaque[var0 & 15];
	}

	public static int glassLight(int var0) {
		return new BlockGlass(var0 & 15).light;
	}

	public static boolean glassOpaque(int var0) {
		new BlockGlass(var0 & 15);
		return opaque[var0 & 15];
	}
}
'''

GLASS = '''public class BlockGlass extends Block {
	public int extra = 5;

	public BlockGlass(int var1) {
		super(var1, 2);
		this.extra += var1;
	}

	public boolean isOpaqueCube() {
		return false;
	}

	public int lightValue() {
		return this.extra + 10;
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


@pytest.fixture(scope='module')
def tree(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for name, text in (('Block', BLOCK), ('BlockGlass', GLASS), ('Plain', PLAIN)):
        (src / (name + '.java')).write_text(text, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out


def test_the_set_is_the_hierarchy_with_the_virtual_call(tree):
    src, _out = tree
    classes = [parse.parse_file(str(src / n)) for n in ('Block.java', 'BlockGlass.java', 'Plain.java')]
    assert twophase.compute(classes) == {'Block', 'BlockGlass'}


def test_constructors_become_j2xinit(tree):
    _src, out = tree
    h = (out / 'BlockGlass.h').read_text(encoding='utf-8')
    assert 'BlockGlass();' in h and 'BlockGlass* j2xInit(int var1);' in h
    cpp = (out / 'BlockGlass.cpp').read_text(encoding='utf-8')
    assert 'Block::j2xInit(var1, 2);' in cpp and 'return this;' in cpp
    block = (out / 'Block.cpp').read_text(encoding='utf-8')
    assert 'Block::j2xInit(var1);' in block                       # this(var1)
    assert '(new BlockGlass())->j2xInit(' in block
    assert 'Plain::Plain(int var1)' in (out / 'Plain.cpp').read_text(encoding='utf-8')


def test_rewrite_news_is_balanced_and_idempotent():
    text = 'x = new Block(f(a, b), (c)); y = new Block[4]; z = new BlockX(1);'
    once, n = twophase.rewrite_news(text, {'Block'})
    assert n == 1 and once == 'x = (new Block())->j2xInit(f(a, b), (c)); y = new Block[4]; z = new BlockX(1);'
    assert twophase.rewrite_news(once, {'Block'}) == (once, 0)


def test_rewrite_stack_temporary():
    text, n = twophase.rewrite_stack('{ Block _j2xleak0(3); _j2xleak0.go(); }', {'Block'})
    assert n == 1 and text == '{ Block _j2xleak0; _j2xleak0.j2xInit(3); _j2xleak0.go(); }'


@pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')
def test_block_opacity_beside_the_jvm(tree, tmp_path):
    src, out = tree
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Block.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= {'glassLight', 'glassOpaque'} and r['differs'] == [], r['differs'][:4]
