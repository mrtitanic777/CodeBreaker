# -*- coding: utf-8 -*-
"""javac's typed tree lowered into the IR: names resolved by symbol, implicit constructors and super()
marked, and modern Java (lambdas, for-each, arrow switches, try-with-resources) as IR, not Unsupported."""

import pytest

from codebreaker.translate import ir
from codebreaker.translate.frontends import java as fj
from codebreaker.translate.frontends.java import lower as L
from codebreaker.verify.lanes import java

SOURCE = '''import java.io.*;
import java.util.*;

public class Shapes {
	enum Kind { SQUARE, ROUND }

	static int made;
	private final int[] sizes = new int[4];
	String label = "s";

	public Shapes(int n) {
		made += n;
	}

	public Shapes() {
		this(1);
	}

	int area(int i) {
		return sizes.length + sizes[i] * Shapes.made;
	}

	String describe(Kind k, List<String> names) {
		StringBuilder sb = new StringBuilder();
		for (String n : names) sb.append(n);
		Comparator<String> c = (a, b) -> a.length() - b.length();
		String s = switch (k) { case SQUARE -> "square"; case ROUND -> "round"; };
		return label + s + made + 'x' + c.compare("a", "bb");
	}

	int read(String path) throws IOException {
		try (Reader r = new FileReader(path)) {
			return r.read();
		} catch (FileNotFoundException | SecurityException e) {
			return -1;
		} finally {
			made--;
		}
	}

	Runnable later() {
		return new Runnable() {
			public void run() { made++; }
		};
	}
}

class Empty {
}
'''

needs_jdk = pytest.mark.skipif(not java.jdk()[0], reason='needs a JDK')


@pytest.fixture(scope='module')
def unit(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Shapes.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('out')
    units = fj.dump(str(src), str(out))
    assert not [d for d in fj.diagnostics(str(out)) if d['kind'] == 'ERROR']
    return L.lower(units['Shapes'], 'Shapes')


def _cls(unit, name):
    return next(c for c in unit.types if c.name == name)


def _method(cls, name, nparams=None):
    return next(m for m in cls.members if isinstance(m, ir.MethodDecl) and m.name == name
                and (nparams is None or len(m.params) == nparams))


@needs_jdk
def test_nothing_unsupported(unit):
    assert ir.unsupported(unit) == []


@needs_jdk
def test_types(unit):
    assert L.parse_type('java.util.Map<K,java.util.List<V>>') == ir.Type(
        'class', 'java.util.Map', [ir.Type('typevar', 'K'), ir.Type('class', 'java.util.List', [ir.Type('typevar', 'V')])])
    assert L.parse_type('int[][]') == ir.array_of(ir.I32, 2)
    assert L.parse_type('char') == ir.Type('u16') and L.parse_type('<nulltype>') == ir.NULL


@needs_jdk
def test_names_are_resolved_by_symbol(unit):
    area = _method(_cls(unit, 'Shapes'), 'area')
    nodes = list(ir.walk(area.body))
    length = next(n for n in nodes if isinstance(n, ir.Length))
    assert isinstance(length.arr, ir.Select) and isinstance(length.arr.obj, ir.This)   # sizes.length: this->sizes
    statics = [n for n in nodes if isinstance(n, ir.Select) and n.sym.name == 'made']
    assert statics and all(isinstance(s.obj, ir.TypeName) and s.sym.static for s in statics)
    idx = next(n for n in nodes if isinstance(n, ir.Name))
    assert idx.sym.kind == 'param' and idx.type == ir.I32


@needs_jdk
def test_constructors(unit):
    shapes = _cls(unit, 'Shapes')
    one = _method(shapes, '<init>', 1)
    assert one.body.stmts[0].kind == 'super:implicit'                # javac's own super()
    zero = _method(shapes, '<init>', 0)
    assert zero.body.stmts[0].kind == 'this' and len(zero.body.stmts[0].args) == 1
    empty = _method(_cls(unit, 'Empty'), '<init>')
    assert 'IMPLICIT' in empty.mods                                  # javac's default constructor


@needs_jdk
def test_modern_java_lowers(unit):
    d = _method(_cls(unit, 'Shapes'), 'describe')
    nodes = list(ir.walk(d.body))
    assert any(isinstance(n, ir.ForEach) for n in nodes)
    lam = next(n for n in nodes if isinstance(n, ir.Lambda))
    assert [p.sym.name for p in lam.params] == ['a', 'b'] and isinstance(lam.body, ir.Binary)
    concat = next(n for n in nodes if isinstance(n, ir.Concat))
    assert len(concat.parts) == 5 and concat.parts[3].type == ir.Type('u16')
    r = _method(_cls(unit, 'Shapes'), 'read')
    t = next(n for n in ir.walk(r.body) if isinstance(n, ir.Try))
    assert len(t.resources) == 1 and t.final is not None and len(t.catches[0].types) == 2


@needs_jdk
def test_enums_and_anonymous_classes(unit):
    shapes = _cls(unit, 'Shapes')
    kind = next(m for m in shapes.members if isinstance(m, ir.ClassDecl) and m.name == 'Kind')
    assert kind.kind == 'enum' and [c.sym.name for c in kind.members if isinstance(c, ir.EnumConst)] == ['SQUARE', 'ROUND']
    new = next(n for n in ir.walk(_method(shapes, 'later').body) if isinstance(n, ir.New))
    assert new.body is not None and any(isinstance(m, ir.MethodDecl) and m.name == 'run' for m in new.body.members)
