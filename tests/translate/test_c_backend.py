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


def ptr(to, space='far'):
    return {'kind': 'ptr', 'space': space, 'to': to}


def test_moved_passes_far_and_near_calls():
    """c_text's _far and _near_calls done on the tree (step 3): a four-byte pointer is written
    __far where it is declared and in the cast assigned to it; a call through a word is near."""
    word = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr({'kind': 'class', 'name': 'xunknown2'}, 'near'),
                                              'x': lit(0x18)}}
    call = {'node': 'Call', 'target': {'node': 'Unary', 'op': '*',
                                       'x': {'node': 'Cast', 'type': {'kind': 'fnptr', 'space': 'far',
                                                                      'fn': {'kind': 'fn'}}, 'x': word}}, 'args': []}
    p = {'node': 'Name', 'sym': {'kind': 'local', 'name': 'p'}, 'type': ptr({'kind': 'i16'})}
    q = {'node': 'Name', 'sym': {'kind': 'param', 'name': 'q'}, 'type': ptr({'kind': 'void'})}
    set_p = {'node': 'Assign', 'target': p,
             'value': {'node': 'Cast', 'type': ptr({'kind': 'i16'}, 'near'), 'x': q}}
    set_data = {'node': 'Assign', 'target': p,
                'value': {'node': 'Cast', 'type': ptr({'kind': 'i16'}, 'near'), 'x': lit(0x40)}}
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'},
           'origin': {'module': '', 'segment': 1, 'offset': 0},
           'params': [{'node': 'Param', 'sym': {'kind': 'param', 'name': 'q'}, 'type': ptr({'kind': 'void'})}],
           'body': [{'node': 'Local', 'sym': {'kind': 'local', 'name': 'p'}, 'type': ptr({'kind': 'i16'})},
                    {'node': 'ExprStmt', 'expr': set_p}, {'node': 'ExprStmt', 'expr': set_data},
                    {'node': 'ExprStmt', 'expr': call}]}
    plain, moved = c.function(doc), c.function(doc, c.MOVED)
    assert '__far' not in plain and '__near' not in plain           # (as the decompiler prints it)
    assert 'void __far *q' in moved and '  int2 __far *p;' in moved
    assert 'p = (int2 __far *)q;' in moved
    assert 'p = (int2 *)0x40;' in moved                             # (a data address: DS, made far by C)
    assert '(*(code __near *)*(xunknown2 *)0x18)();' in moved
    assert 'void *q' in c.function(doc, c.MOVED, far={'p'})         # (the notes' far set, when given, rules)
    flat = dict(doc, origin={'module': '', 'va': 0x401000})
    assert '__far' not in c.function(flat, c.MOVED)                  # (32-bit: no segments, nothing moves)


def test_moved_far_from_stack():
    """c_text's _far_from_stack on the tree: a far pointer set from a near one into the frame is
    made with SS (USER's WM_WINDOWPOSCHANGED)."""
    def name(n, t, kind='local'):
        return {'node': 'Name', 'sym': {'kind': kind, 'name': n}, 'type': t}
    near, far = ptr({'kind': 'i16'}, 'near'), ptr({'kind': 'i16'})
    buf = {'kind': 'array', 'elem': {'kind': 'i16'}, 'count': 7}
    sets = lambda v, x: {'node': 'ExprStmt', 'expr': {'node': 'Assign', 'target': name(v, near), 'value': x}}
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'},
           'origin': {'module': '', 'segment': 1, 'offset': 0}, 'params': [],
           'body': [{'node': 'Local', 'sym': {'kind': 'local', 'name': 'aiStack_10'}, 'type': buf},
                    {'node': 'Local', 'sym': {'kind': 'local', 'name': 'piVar1'}, 'type': near},
                    {'node': 'Local', 'sym': {'kind': 'local', 'name': 'piVar2'}, 'type': far},
                    sets('piVar1', name('aiStack_10', buf)),
                    {'node': 'ExprStmt', 'expr': {'node': 'Assign', 'target': name('piVar2', far),
                                                  'value': name('piVar1', near)}}]}
    assert 'piVar2 = MK_FP(codebreaker_ss(), piVar1);' in c.function(doc, c.MOVED)
    assert 'piVar2 = piVar1;' in c.function(doc, ('far', 'near_calls'))


def test_comments_where_printc_writes_them():
    """PrintC's comments come with L2: the function's warnings above it (MethodDecl.comments, a
    line break in one written as the core writes it, the comment's fill after it), the others as
    Comment statements, each on a line of its own at column 20 (line_commentindent)."""
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'}, 'params': [],
           'comments': ['WARNING: Instruction at (ram,0x10e8036d) overlaps instruction at (ram,0x10e8036c)\n',
                        'WARNING: Treating indirect jump as call'],
           'body': [{'node': 'Comment', 'text': 'WARNING: Could not recover jumptable at 0x00011a6a. Too many branches'},
                    {'node': 'Return'}]}
    assert not list(l2.validate(doc) or ())
    assert c.function(doc) == (
        '/* WARNING: Instruction at (ram,0x10e8036d) overlaps instruction at (ram,0x10e8036c)\n'
        '    */\n'
        '/* WARNING: Treating indirect jump as call */\n'
        '\n'
        'void f(void)\n'
        '\n'
        '{\n'
        '                    /* WARNING: Could not recover jumptable at 0x00011a6a. Too many branches */\n'
        '  return;\n'
        '}\n')


def test_a_long_line_broken_as_printc_breaks_it():
    """EmitPrettyPrint's rule, not C's: lines of 100, broken at the last place the groups allow --
    here between two arguments, the rest under the bracket (USER's 1:59E6)."""
    args = ['iStack_4', 'iStack_1e', 'iStack_1c', 'iStack_1a', 'iStack_18', 'iStack_16', 'iStack_14',
            'iStack_12', 'iStack_10', 'a5']
    call = {'node': 'Call', 'target': {'node': 'Name', 'sym': {'kind': 'function', 'name': 'sub_1_65AE'}},
            'args': [name(a) for a in args]}
    stmt = {'node': 'ExprStmt', 'expr': {'node': 'Assign', 'target': name('iVar3'), 'value': call}}
    nest = [stmt]
    for _ in range(3):
        nest = [{'node': 'If', 'cond': binary('==', name('x'), lit(0)), 'then': nest}]
    text = function(nest)
    assert '        iVar3 = sub_1_65AE(iStack_4,iStack_1e,iStack_1c,iStack_1a,iStack_18,iStack_16,iStack_14,\n' \
           '                           iStack_12,iStack_10,a5);\n' in text
    assert c.expr(stmt['expr']) == 'iVar3 = sub_1_65AE(%s)' % ','.join(args)     # (on its own: one line)
