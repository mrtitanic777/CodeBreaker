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
    # (a data address: DS and it -- left near, OpenWatcom made the constant far with segment 0)
    assert 'p = (int2 __far *)MK_FP(codebreaker_ds(), 0x40);' in moved
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


def sixteen(body, params=()):
    return {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'},
            'origin': {'module': '', 'segment': 1, 'offset': 0}, 'params': list(params), 'body': body}


def local(n, t):
    return {'node': 'Local', 'sym': {'kind': 'local', 'name': n}, 'type': t}


def typed(n, t):
    return {'node': 'Name', 'sym': {'kind': 'local', 'name': n}, 'type': t}


def test_moved_frame_through_ss():
    """c_text._frame_through_ss on the tree: a frame buffer reached through its address as a
    number is written with SS (KEYBOARD's NEWTABLEEX), given the room of the cast as printed."""
    buf = {'kind': 'array', 'elem': {'kind': 'class', 'name': 'xunknown1'}, 'count': 52}
    word = {'kind': 'class', 'name': 'xunknown2'}
    at = binary('+', {'node': 'Cast', 'type': {'kind': 'i16'}, 'x': typed('axStack_a8', buf)}, name('iVar1'))
    store = {'node': 'Assign', 'target': {'node': 'Unary', 'op': '*',
                                          'x': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': at}},
             'value': lit(0x5c)}
    doc = sixteen([local('axStack_a8', buf), local('iVar1', {'kind': 'i16'}), {'node': 'ExprStmt', 'expr': store}])
    assert '  *(xunknown2 *)MK_FP(codebreaker_ss(),(int2)axStack_a8 + iVar1) = 0x5c;' in c.function(doc, c.MOVED)
    assert '  *(xunknown2 *)((int2)axStack_a8 + iVar1) = 0x5c;' in c.function(doc, ('far',))


def select(obj, field):
    return {'node': 'Select', 'obj': obj, 'sym': {'kind': 'field', 'name': field}}


def stmt(e):
    return {'node': 'ExprStmt', 'expr': e}


def test_moved_pieces():
    """c_text._pieces on the tree: `x._2_2_` is the two bytes of x from byte 2 -- unsigned, but
    signed beside an ordering comparison; a piece of a piece has the inner one inside."""
    x = name('x')
    doc = sixteen([stmt({'node': 'Assign', 'target': name('a'), 'value': select(x, '_2_2_')}),
                   stmt(binary('<', select(x, '_0_1_'), name('a'))),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': select(select(x, '_0_4_'), '_2_2_')})])
    text = c.function(doc, c.MOVED)
    assert '  a = (*(uint2 *)((char *)&x + 2));' in text
    assert '  (*(int1 *)((char *)&x + 0)) < a;' in text
    assert '  a = (*(uint2 *)((char *)&(*(uint4 *)((char *)&x + 0)) + 2));' in text
    assert '  a = x._2_2_;' in c.function(doc, ('far',))


def test_moved_registers_null_and_globals():
    """c_text's _segment_registers, _null_is_zero, _global_pointer_steps and _one_name_for_globals
    on the tree."""
    word = {'kind': 'class', 'name': 'xunknown2'}
    null = {'node': 'Cast', 'type': ptr({'kind': 'i32'}, 'near'), 'x': lit(0, 'u16', '0x0')}
    doc = sixteen([local('unaff_SS', word), local('unaff_ES', word),
                   stmt(binary('==', name('x'), null)),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': binary('+', name('pRam00001234'), lit(2))}),
                   stmt({'node': 'Assign', 'target': name('y'),
                         'value': binary('+', {'node': 'Unary', 'op': '*', 'x': name('pRam00001234')}, lit(2))}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': name('_iRam0040c000')})])
    text = c.function(doc, c.MOVED)
    assert '  xunknown2 unaff_SS = codebreaker_ss();' in text and '  xunknown2 unaff_ES;' in text
    assert '  x == 0;' in text
    assert '  y = (uint1 *)pRam00001234 + 2;' in text
    assert '  y = *pRam00001234 + 2;' in text
    assert '  y = iRam0040c000;' in text


def test_a_module_as_one_tree_with_what_stayed_assembly(tmp_path):
    """Step 4: the 16-bit build's whole module as one L2 Module -- a function rebuilt from C as its
    L2 document, one kept as its instructions as a MachineFragment with the assembly the build
    wrote (a run assembled as one file: the text on the first, the next `within` it) -- and the C
    back end writing it: the C, a note in each fragment's place, the assembly beside."""
    from codebreaker.build import module as build_module

    class M:
        pass
    m = M()
    m.by_va = {0x10000: 'sub_1_0000', 0x10010: 'sub_1_0010', 0x10020: 'sub_1_0020', 0x20000: 'sub_2_0000'}
    m.where = {va: (va >> 16, va & 0xFFFF) for va in m.by_va}
    m.l2 = {'sub_1_0000': dict(sixteen([{'node': 'Return'}]), name='sub_1_0000')}
    (tmp_path / 'asm').mkdir()
    (tmp_path / 'asm' / 'run1.asm').write_text("CODE1 segment\n_sub_1_0010: ret\n_sub_1_0020: retf\nCODE1 ends\nend\n")
    assembled = {'sub_1_0010': 'written in assembly: no frame', 'sub_1_0020': 'written in assembly: no frame'}
    stems = {'sub_1_0010': 'asm/run1', 'sub_1_0020': 'asm/run1'}
    doc = build_module.module_document('test.dll', m, assembled, stems, str(tmp_path), {1: 0x30, 2: 0x8})
    assert not list(l2.validate(doc) or ())
    kinds = [(u['node'], u.get('name')) for u in doc['units']]
    assert kinds == [('MethodDecl', 'sub_1_0000'), ('MachineFragment', 'sub_1_0010'),
                     ('MachineFragment', 'sub_1_0020'), ('MethodDecl', 'sub_2_0000')]
    first, second = doc['units'][1], doc['units'][2]
    assert first['length'] == 0x10 and second['length'] == 0x10 and first['origin']['segment'] == 1
    assert first['asm'].startswith('CODE1 segment') and second['asm'] is None and second['within'] == 'sub_1_0010'
    text, asm = c.module(doc)
    assert 'void sub_1_0000(void)' in text and 'return;' in text
    assert '/* sub_1_0010 1:0010: kept as its instructions -- written in assembly: no frame */' in text
    assert '(assembled with sub_1_0010)' in text
    assert "/* sub_2_0000: not in L2 (its C is the decompiler's printed C) */" in text
    assert list(asm) == ['sub_1_0010'] and '_sub_1_0020: retf' in asm['sub_1_0010']


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
