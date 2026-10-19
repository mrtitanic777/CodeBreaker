# -*- coding: utf-8 -*-
"""The Java front end's first step: javac's typed tree, every node with what javac resolved."""

import pytest

from codebreaker.translate.frontends import java as fj
from codebreaker.verify import java

SOURCE = '''import java.util.List;

public class Demo {
	static int total;

	public int add(int a, long b) {
		return (int)(a + b) >>> 1;
	}

	public String join(List<String> xs) {
		StringBuilder sb = new StringBuilder();
		for (String x : xs) sb.append(x).append(',');
		Runnable r = () -> total++;
		r.run();
		return sb.toString() + total;
	}
}
'''

needs_jdk = pytest.mark.skipif(not java.jdk()[0], reason='needs a JDK')


def _walk(n):
    if isinstance(n, dict):
        if 'k' in n:                                   # a node (not a $sym record)
            yield n
        for v in n.values():
            yield from _walk(v)
    elif isinstance(n, list):
        for v in n:
            yield from _walk(v)


@pytest.fixture(scope='module')
def unit(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Demo.java').write_text(SOURCE, encoding='utf-8')
    out = tmp_path_factory.mktemp('out')
    units = fj.dump(str(src), str(out))
    return units, fj.diagnostics(str(out))


@needs_jdk
def test_every_unit_dumped_without_errors(unit):
    units, diags = unit
    assert list(units) == ['Demo'] and not [d for d in diags if d['kind'] == 'ERROR']


@needs_jdk
def test_types_and_symbols_are_resolved(unit):
    units, _d = unit
    nodes = list(_walk(units['Demo']))
    shift = next(n for n in nodes if n['k'] == 'UNSIGNED_RIGHT_SHIFT')
    assert shift['$type'] == 'int' and shift['l'] == 7
    plus = next(n for n in nodes if n['k'] == 'PLUS' and n.get('$type') == 'long')
    assert plus['rightOperand']['$sym'] == {'kind': 'PARAMETER', 'name': 'b', 'owner': 'Demo.add', 'type': 'long'}
    cast = next(n for n in nodes if n['k'] == 'TYPE_CAST')
    assert cast['$type'] == 'int' and cast['type']['k'] == 'PRIMITIVE_TYPE'   # javac's own `type` child kept
    append = next(n for n in nodes if n['k'] == 'METHOD_INVOCATION' and n['$sym']['name'] == 'append'
                  and n['arguments'][0]['k'] == 'CHAR_LITERAL')
    assert append['$sym']['params'] == ['char'] and append['$sym']['owner'] == 'java.lang.StringBuilder'
    lam = next(n for n in nodes if n['k'] == 'LAMBDA_EXPRESSION')
    assert lam['$type'] == 'java.lang.Runnable'
    concat = next(n for n in nodes if n['k'] == 'PLUS' and n.get('$type') == 'java.lang.String')
    assert concat['rightOperand']['$sym']['static'] is True


@needs_jdk
def test_the_dumper_is_cached(tmp_path):
    assert fj.tool() == fj.tool()
