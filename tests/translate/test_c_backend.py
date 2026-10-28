# -*- coding: utf-8 -*-
"""The hub's C back end (translate/backends/c.py): L2 written back as the decompiler's C. Each rule
here is one of PrintC's that the golden corpus showed (tools/l2c.py holds the whole corpus to it)."""

from codebreaker.translate import l2
from codebreaker.translate.backends import c


def lit(v, kind='i32', text=None):
    d = {'node': 'Lit', 'type': {'kind': kind}, 'value': v}
    if text:
        d['text'] = text
    return d


def name(n, kind='i32'):
    return {'node': 'Name', 'type': {'kind': kind}, 'sym': {'kind': 'local', 'name': n}}


def binary(op, a, b):
    return {'node': 'Binary', 'op': op, 'left': a, 'right': b}


def joined(op, a, b):
    """A condition the structure joined, as PrintL2 marks one."""
    return binary(op, {'node': 'Paren', 'x': a}, {'node': 'Paren', 'x': b})


def test_numbers_in_their_natural_base():
    # PrintC::push_integer: up to 10 in decimal; past it, whichever base ends in a run of 0s/9s or 0s/Fs
    assert [c.expr(lit(v)) for v in (0, 9, 10, 11, 500, 1000, 0x400, 0x104, 0xfff, -0x400)] == \
        ['0', '9', '10', '0xb', '500', '1000', '0x400', '0x104', '0xfff', '-0x400']
    assert c.expr({'node': 'Lit', 'type': {'kind': 'ptr', 'space': 'flat', 'to': {'kind': 'i16'}}, 'value': 0}) == '0x0'
    assert c.expr(lit(0xff0f, 'u32', text='0xff0fU')) == '0xff0fU'   # the spelling, where L2 has it


def test_brackets_as_printc_places_them():
    a, b, d = name('a'), name('b'), name('d')
    assert c.expr(binary('-', binary('+', a, b), d)) == '(a + b) - d'       # same precedence, other operator
    assert c.expr(binary('+', binary('+', a, b), d)) == 'a + b + d'         # the same associative one
    assert c.expr(binary('-', a, binary('-', b, d))) == 'a - (b - d)'
    assert c.expr(binary('&&', binary('<', a, b), binary('==', b, d))) == 'a < b && b == d'
    cast = {'node': 'Cast', 'type': {'kind': 'u16'}, 'x': binary('<', a, b)}
    assert c.expr(cast) == '(uint2)(a < b)'
    deref = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': {'kind': 'ptr', 'space': 'flat',
                                                                        'to': {'kind': 'i16'}}, 'x': lit(70)}}
    assert c.expr(deref) == '*(int2 *)0x46'


def test_joined_conditions():
    a, b, d = (binary('!=', name(n), lit(0)) for n in 'abd')
    assert c.condition(a) == '(a != 0)'
    assert c.condition(joined('&&', a, b)) == '((a != 0) && (b != 0))'
    assert c.condition(joined('&&', joined('&&', a, b), joined('||', a, d))) == \
        '(((a != 0) && (b != 0)) && ((a != 0 || (d != 0))))'
    assert c.condition(joined('&&', a, b), True) == '(a != 0 && (b != 0))'      # a loop's: comma_separate


def function(body, mods=None):
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'i16'}, 'params': [],
           'sym': {'kind': 'function', 'name': 'f', 'type': {'kind': 'fn', 'conv': 'cdecl', 'distance': 'far',
                                                             'params': [], 'result': {'at': 'none'}}},
           'body': body}
    if mods:
        doc['mods'] = mods
    assert not list(l2.validate(doc) or ())
    return c.function(doc)


def test_statements():
    cond = binary('==', name('x'), lit(0))
    text = function([
        {'node': 'Local', 'sym': {'kind': 'local', 'name': 'x'}, 'type': {'kind': 'i16'}},
        {'node': 'If', 'cond': cond, 'then': {'node': 'Goto', 'label': 'LAB_1'}},                 # if-goto
        {'node': 'If', 'cond': cond, 'then': [{'node': 'Return', 'expr': lit(1)}],
         'orelse': [{'node': 'If', 'cond': cond, 'then': [{'node': 'Return', 'expr': lit(2)}]}]},  # else if
        {'node': 'If', 'cond': cond, 'then': [], 'orelse': [{'node': 'Block', 'stmts': []}]},       # else { }
        {'node': 'Return', 'expr': lit(0)}], mods=['__stdcall16far'])
    assert text.splitlines()[0] == 'int2 __stdcall16far f(void)'
    assert '  int2 x;' in text
    assert '  if (x == 0) goto LAB_1;' in text
    assert '  else if (x == 0) {' in text
    assert '  else {\n  }' in text
