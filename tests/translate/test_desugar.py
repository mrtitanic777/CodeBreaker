# -*- coding: utf-8 -*-
"""The lowering pass (translate/desugar.py): the forms it lowers and the one fault it had -- a loop with a label was lowered
without its own body being lowered first, so a for-each inside it was left for the back end to decline. The same cases are
tested on the C++ module (native/cb/translate/desugar_test.cpp), which writes the same documents."""

import json
import os

from codebreaker.translate import desugar, ir, l2

I32 = ir.I32
ARRAY = ir.Type('array', elem=I32)


def _each(var, xs, body):
    x = ir.Sym('local', var, 'p.C.m', type=I32)
    xs_sym = ir.Sym('local', xs, 'p.C.m', type=ARRAY)
    return ir.ForEach(var=ir.Local(sym=x, type=I32, init=None, mods=[]),
                      iterable=ir.Name(sym=xs_sym, type=ARRAY), body=body)


def _unit(stmts):
    method = ir.MethodDecl(sym=ir.Sym('method', 'm', 'p.C'), name='m', kind='method', params=[], mods=[],
                           body=ir.Block(stmts=stmts))
    cls = ir.ClassDecl(name='C', qname='p.C', kind='class', members=[method], mods=[])
    return ir.Unit(lang='java', types=[cls]), method


def _kinds(root):
    return [type(n).__name__ for n in ir.walk(root)]


def test_a_foreach_over_an_array_becomes_a_counting_loop():
    unit, method = _unit([_each('x', 'xs', ir.Block(stmts=[]))])
    assert desugar.run([unit]) == 1
    assert 'ForEach' not in _kinds(unit) and _kinds(unit).count('For') == 1
    wrap = method.body.stmts[0]
    assert isinstance(wrap, ir.Block) and isinstance(wrap.stmts[0], ir.Local) and wrap.stmts[0].sym.name == 'j2x_a1'


def test_a_labelled_loop_lowers_the_loop_inside_it_too():
    inner = _each('y', 'ys', ir.Block(stmts=[ir.Continue()]))
    outer = _each('x', 'xs', ir.Block(stmts=[inner]))
    unit, method = _unit([ir.Labeled(label='outer', body=outer)])
    assert desugar.run([unit]) == 2
    assert 'ForEach' not in _kinds(unit)
    # the label goes on the loop, not on its set-up
    wrap = method.body.stmts[0]
    assert isinstance(wrap, ir.Block) and isinstance(wrap.stmts[0], ir.Local)
    assert isinstance(wrap.stmts[1], ir.Labeled) and wrap.stmts[1].label == 'outer'
    assert isinstance(wrap.stmts[1].body, ir.For)


def test_the_modern_java_program_is_lowered_as_the_cpp_module_lowers_it():
    here = os.path.dirname(os.path.abspath(__file__))
    data = os.path.join(here, '..', '..', 'native', 'cb', 'translate', 'testdata')
    with open(os.path.join(data, 'l2_java_units.json'), encoding='utf-8') as f:
        units = [l2.from_json(d) for d in json.load(f)]
    with open(os.path.join(data, 'l2_java_units.desugared.json'), encoding='utf-8') as f:
        want = json.load(f)
    assert desugar.run(units) == want['lowered']
    assert [l2.to_json(u) for u in units] == want['units']
    assert desugar.run(units) == 0
