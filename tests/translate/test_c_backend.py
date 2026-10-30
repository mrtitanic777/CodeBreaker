# -*- coding: utf-8 -*-
"""The hub's C back end (translate/backends/c.py): L2 written back as the decompiler's C. Each rule
here is one of PrintC's that the golden corpus showed (tools/l2c.py holds the whole corpus to it)."""

from codebreaker.translate import l2
from codebreaker.translate.backends import c
from codebreaker.decompile import c_text


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
    # (behind a cast too, `(int4)*(T *)(...)`: GDI's sub_35_0669 -- once read as a product, through DS)
    read = {'node': 'Cast', 'type': {'kind': 'i32'},
            'x': {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': at}}}
    doc = sixteen([local('axStack_a8', buf), local('iVar1', {'kind': 'i16'}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': read})])
    assert '  y = (int4)*(xunknown2 *)MK_FP(codebreaker_ss(),(int2)axStack_a8 + iVar1);' in c.function(doc, c.MOVED)


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


def param(n, t):
    return {'node': 'Param', 'sym': {'kind': 'param', 'name': n}, 'type': t}


def test_moved_passes_that_read_the_module():
    """c_text's passes after the pointer ones, done on the tree with what the module says of the
    function (facts: c_text.module_facts16): the model word and __loadds before the name, `void`
    written xunknown2, `_a1` as the four bytes from a1 and read through as a far pointer, an
    argument loaded as a far pointer read through with its segment, and the module's flags."""
    word = {'kind': 'i16'}
    deref = {'node': 'Unary', 'op': '*', 'x': name('_a1')}
    through = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': name('a3', 'i16')}}
    doc = sixteen([stmt({'node': 'Assign', 'target': name('x'), 'value': deref}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': name('_a1')}),
                   stmt({'node': 'Assign', 'target': name('z'), 'value': through})],
                  [param('a1', word), param('a2', word), param('a3', word), param('a4', word)])
    facts = {'distance': 'far', 'model': '__stdcall16far', 'widths': {True: {'a1': 1}, False: {}},
             'loads': {10}, 'export': False, 'loadds': True}
    text = c.function(doc, c.MOVED, facts=facts)
    assert text.startswith('xunknown2 __stdcall16far __loadds f(a1,a2,a3,a4)\nint2 a1;')
    assert '  x = *(*(char __far * *)&a1);' in text
    assert '  y = (*(uint4 *)&a1);' in text
    assert '  z = *(int2 __far *)CODEBREAKER_FAR(a4, a3);' in text
    assert c.flags(doc, facts) == {'novalue'}
    # without the module's facts (the C the decompiler is run for at first): none of them
    assert c.function(doc, c.MOVED).startswith('void f(')


def fcall(fn, *args):
    return {'node': 'Call', 'target': {'node': 'Name', 'sym': {'kind': 'function', 'name': fn}}, 'args': list(args)}


def test_moved_own_segment_calls():
    """c_text._own_segment_calls on the tree: MSMOUS31's Disable, `call word ptr [18h]`, as the
    decompiler prints it -- `(*MK_FP(0x1000,...))()`, segment 1's made-up selector -- is a near
    call (made a far call to 1000h it faulted at Setup's restart); any other MK_FP of the
    function's own selector reads through CS as it runs. Another segment's selector stays."""
    word = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr({'kind': 'class', 'name': 'xunknown2'}, 'near'),
                                              'x': lit(0x18, text='0x18')}}
    call = {'node': 'Call', 'target': {'node': 'Unary', 'op': '*', 'x': fcall('MK_FP', lit(0x1000, text='0x1000'), word)},
            'args': []}
    read = {'node': 'Unary', 'op': '*', 'x': fcall('MK_FP', lit(0x1000, text='0x1000'),
                                                   {'node': 'Cast', 'type': ptr({'kind': 'i16'}, 'near'),
                                                    'x': lit(0x40, text='0x40')})}
    doc = sixteen([stmt({'node': 'Assign', 'target': name('i'), 'value': call}), stmt(read)])
    text = c.function(doc, c.MOVED, facts={'selector': 0x1000})
    assert '  i = ((*(code __near *)*(xunknown2 *)0x18))();' in text, text
    assert '  *((int2 __far *)MK_FP(codebreaker_cs(), 0x40));' in text, text      # (and _far_addresses)
    other = c.function(doc, c.MOVED, facts={'selector': 0x1040})        # (segment 9's: 1040h)
    assert other.count('MK_FP(0x1000') == 2, other


def test_moved_interrupts():
    """c_text._interrupts on the tree: a call of int21 is its helper with the registers set for it
    (in_BX, passed on, is none), an interrupt's result is the register (`int_EBX_12` is int_BX)
    and has no local of its own; the interrupts taken go to the facts, for the header."""
    word = {'kind': 'i16'}
    call = fcall('int21', lit(0x3d00, text='0x3d00'), name('in_BX'), name('x'))
    doc = sixteen([local('x', word), local('int_EBX_12', word),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': call}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': name('int_EBX_12')})])
    facts = {}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '  y = int21_ax_cx(0x3d00,x);' in text, text
    assert '  y = int_BX;' in text and 'int_EBX_12' not in text, text
    assert facts['interrupts'] == {(0x21, ('ax', 'cx'))}


def test_moved_selector_structs():
    """c_text._selector_structs on the tree: a segment read through a selector at two constant
    offsets is a structure it holds, SEG_PTR(seg_<function>_<selector>, selector)->f_<offset>;
    its declaration goes to the facts. A global selector's is the module's."""
    def at(sel, kind, off):
        return {'node': 'Unary', 'op': '*', 'x': fcall('MK_FP', sel, {'node': 'Cast', 'type': ptr({'kind': kind}, 'near'),
                                                                        'x': lit(off, text='0x%x' % off)})}
    doc = sixteen([stmt({'node': 'Assign', 'target': name('a'), 'value': at(name('xVar2'), 'i16', 0x3e)}),
                   stmt({'node': 'Assign', 'target': name('b'), 'value': at(name('xVar2'), 'u8', 0x40)})])
    facts = {'selector_structs': True}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '  a = SEG_PTR(seg_f_xVar2, xVar2)->f_3e;' in text, text
    assert '  b = SEG_PTR(seg_f_xVar2, xVar2)->f_40;' in text, text
    assert facts['selector_decls'] == ['typedef _Packed struct seg_f_xVar2 { uint1 _pad_0[62]; int2 f_3e; uint1 f_40; } seg_f_xVar2;']
    assert 'MK_FP' in c.function(doc, c.MOVED, facts={})          # (without structures: as it was)
    # a global selector's: the module's, its fields reported; where the functions disagree the
    # module gives this one its own (c_text._shared_selector_structs) and its uses are written so
    glob = sixteen([stmt({'node': 'Assign', 'target': name('a'), 'value': at(name('xRam10000002'), 'i16', 0x3e)}),
                    stmt({'node': 'Assign', 'target': name('b'), 'value': at(name('xRam10000002'), 'u8', 0x40)})])
    facts = {'selector_structs': True}
    assert '  a = SEG_PTR(seg_xRam10000002, xRam10000002)->f_3e;' in c.function(glob, c.MOVED, facts=facts)
    assert facts['selector_shared'] == {'seg_xRam10000002': {'f': {0x3e: 'int2', 0x40: 'uint1'}}}
    other = {'seg_xRam10000002': {'f': {0x3e: 'int2', 0x40: 'uint1'}, 'g': {0x3e: 'uint1'}}}
    decls, renames = c_text._shared_selector_structs(other)
    assert renames == {'f': {'seg_xRam10000002': 'seg_f_xRam10000002'}, 'g': {'seg_xRam10000002': 'seg_g_xRam10000002'}}
    text = c.function(glob, c.MOVED, facts={'selector_structs': True, 'selector_renames': renames['f']})
    assert '  a = SEG_PTR(seg_f_xRam10000002, xRam10000002)->f_3e;' in text, text


def test_moved_old_style_definition():
    """c_text._open_arguments on the tree (16-bit, with the module's facts): the definition in the
    older style, the names in the brackets and each declared before the `{` (a far pointer
    __far); none, `f()`. A pointer result with a fastcall word keeps its heading as it was
    (c_text's parser lost that heading)."""
    word = {'kind': 'i16'}
    body = [stmt({'node': 'Assign', 'target': name('x'), 'value': name('a1')})]
    doc = sixteen(body, [param('a1', word), param('a2', ptr(word, 'far'))])
    text = c.function(doc, c.MOVED, facts={'distance': 'far', 'model': '__stdcall16far'})
    assert text.startswith('xunknown2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 __far *a2;\n{\n  x = a1;'), text
    assert c.function(sixteen(body), c.MOVED, facts={'distance': 'far'}).startswith('xunknown2 __far f()\n{\n')
    lost = dict(doc, ret=ptr(word, 'near'), mods=['__fastcall16near'])
    assert '(int2 a1,int2 __far *a2)' in c.function(lost, c.MOVED, facts={'distance': 'near'})


def test_moved_entry_sp_and_stack_arguments():
    """c_text._entry_sp and _stack_arguments on the tree: SP as the function was entered is
    codebreaker_entry_sp(); `&stack0xfff0` the address there; a read of one of its own locals
    through that address (`*(int2 *)(&stack0xfffa + 2)`, entry SP - 4) is the local; the caller's
    words it reads, in_stack_0000000c and the piece uStack000e, are those places on the stack."""
    word = {'kind': 'i16'}

    def amp(n):
        return {'node': 'Unary', 'op': '&', 'x': name(n)}
    read = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(word, 'near'),
                                              'x': binary('+', amp('stack0xfffa'), lit(2, text='2'))}}
    doc = sixteen([local('iStack_4', word), local('in_stack_0000000c', word),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': read}),
                   stmt({'node': 'Assign', 'target': name('b'), 'value': name('register0x00000010')}),
                   stmt({'node': 'Assign', 'target': name('c'), 'value': amp('stack0xfff0')}),
                   stmt({'node': 'Assign', 'target': name('d'),
                         'value': binary('+', name('in_stack_0000000c'), name('uStack000e'))})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  a = iStack_4;' in text, text
    assert '  b = codebreaker_entry_sp();' in text, text
    assert '  c = ((uint1 *)codebreaker_entry_sp() + (int2)0xfff0);' in text, text
    assert '  d = codebreaker_stack(int2, 0xc) + codebreaker_stack(uint2, 0xe);' in text, text
    assert 'int2 in_stack_0000000c;' not in text, text


def test_decompile16_counts_calls_in_l2():
    """decompile16's rounds read each function's calls and arguments off its L2 document: the
    places not known as functions yet (func_0x...), how many arguments each call passes, and
    what the function itself declares (a far pointer as its facts say)."""
    from codebreaker.decompile import decompiler as dc
    word = {'kind': 'i16'}
    doc = sixteen([stmt(fcall('func_0x10000123', name('a'))), stmt(fcall('sub_1_0010', name('a'), name('b'))),
                   stmt(fcall('sub_1_0010'))],
                  [param('a1', word), param('a2', ptr(word, 'near'))])
    assert dc._placeholders('', doc) == {0x10000123}
    assert dc._call_arguments('', 'sub_1_0010', doc) == [2, 0]
    assert dc._calls('', doc, 'sub_1_0010') and not dc._calls('', doc, 'sub_1_0020')
    assert dc._own_arguments('', 'f', {'l2': doc, 'pointers': {'a2': 4}}) == (2, ['2', '4p'])
    # (without a document: the text, as before)
    assert dc._call_arguments('x = sub_1_0010(a,(b,c));', 'sub_1_0010') == [2]


def test_moved_far_addresses():
    """c_text._far_addresses on the tree: MK_FP(seg,(T *)off) is a far pointer to T, `p + n` as the
    pointer p is; a far pointer's own halves put back together are its own arithmetic, and passed
    as two halves the pointer passed; the variable that held the high half goes."""
    word = {'kind': 'i16'}
    p = typed('p', ptr(word, 'far'))
    q = typed('q', ptr(word, 'near'))
    high = {'node': 'Cast', 'type': {'kind': 'class', 'name': 'xunknown2'},
            'x': binary('>>', {'node': 'Cast', 'type': {'kind': 'u32'}, 'x': p}, lit(16, text='0x10'))}
    cast = {'node': 'Cast', 'type': ptr(word, 'near'), 'x': lit(0x40, text='0x40')}
    low = {'node': 'Cast', 'type': word, 'x': {'node': 'Cast', 'type': ptr({'kind': 'void'}, 'near'), 'x': p}}
    doc = sixteen([local('p', ptr(word, 'far')), local('q', ptr(word, 'near')), local('xVar1', {'kind': 'class', 'name': 'xunknown2'}),
                   stmt({'node': 'Assign', 'target': name('xVar1'), 'value': high}),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': {'node': 'Unary', 'op': '*', 'x': fcall('MK_FP', name('s'), cast)}}),
                   stmt({'node': 'Assign', 'target': name('b'), 'value': {'node': 'Unary', 'op': '*',
                                                                          'x': fcall('MK_FP', name('s'), binary('+', q, lit(2, text='2')))}}),
                   stmt(fcall('g', name('a'), low, name('xVar1')))])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  a = *((int2 __far *)MK_FP(s, 0x40));' in text, text
    assert '  b = *((int2 __far *)MK_FP(s, q + 2));' in text, text
    assert '  g(a, p);' in text, text
    assert 'xVar1' not in text, text


def test_moved_far_casts():
    """c_text._far_casts on the tree: a far pointer read as another type stays far, `*(int2 __far
    *)p`; its halves put back together, CONCAT22(<high word>,p), are `(int4)p`."""
    word = {'kind': 'i16'}
    p = typed('p', ptr({'kind': 'void'}, 'far'))
    read = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': p}}
    high = {'node': 'Cast', 'type': word,
            'x': binary('>>', {'node': 'Cast', 'type': {'kind': 'u32'}, 'x': p}, lit(16, text='0x10'))}
    doc = sixteen([local('p', ptr({'kind': 'void'}, 'far')),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': read}),
                   stmt({'node': 'Assign', 'target': name('b'), 'value': fcall('CONCAT22', high, p)})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  a = *(int2 __far *)p;' in text, text
    assert '  b = (int4)p;' in text, text


def test_moved_far_locals():
    """c_text._far_locals on the tree: a four-byte local read through is a far pointer to what it
    is set from (`T __far *x`, its casts far); `_p` beside a near p, p never set, is the far
    pointer and p its offset; a pointer set from one typed otherwise is cast to its own type.
    (`x->f` is no read through `*x` as the text is written.)"""
    wide = {'kind': 'class', 'name': 'xunknown4'}
    char, word = {'kind': 'char'}, {'kind': 'i16'}
    x, p, w = typed('x', wide), typed('p', ptr(char, 'near')), typed('_p', wide)
    q, r = typed('q', ptr(word, 'near')), typed('r', ptr({'kind': 'i32'}, 'near'))
    concat = fcall('CONCAT22', name('s'), name('o'))
    doc = sixteen([local('x', wide), local('y', wide), local('p', ptr(char, 'near')),
                   local('q', ptr(word, 'near')), local('r', ptr({'kind': 'i32'}, 'near')),
                   stmt({'node': 'Assign', 'target': x, 'value': {'node': 'Cast', 'type': ptr(char, 'near'), 'x': concat}}),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': {'node': 'Unary', 'op': '*', 'x': x}}),
                   stmt({'node': 'Assign', 'target': name('b'),
                         'value': select({'node': 'Unary', 'op': '*', 'x': typed('y', wide)}, 'f_0')}),
                   stmt({'node': 'Assign', 'target': w, 'value': {'node': 'Cast', 'type': ptr(char, 'near'), 'x': concat}}),
                   stmt({'node': 'Assign', 'target': name('a'), 'value': binary('+', p, lit(1))}),
                   stmt({'node': 'Assign', 'target': q, 'value': r})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  char __far *x;' in text and '  xunknown4 y;' in text, text
    assert '  x = (char __far *)CONCAT22(s,o);' in text, text
    assert '  char __far *_p;' in text and 'char *p;' not in text, text
    assert '  _p = (char __far *)CONCAT22(s,o);' in text, text
    assert '  a = ((char *)(uint2)(uint4)_p) + 1;' in text, text
    assert '  q = (int2 *)r;' in text, text


def test_moved_far_concat_casts():
    """c_text._far_concat_casts on the tree: `(T *)CONCAT22(seg,off)` wherever it stands is far."""
    concat = {'node': 'Cast', 'type': ptr({'kind': 'class', 'name': 'xunknown1'}, 'near'),
              'x': fcall('CONCAT22', name('a4'), name('a3'))}
    doc = sixteen([stmt(fcall('g', concat))])
    assert '  g((xunknown1 __far *)CONCAT22(a4,a3));' in c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  g((xunknown1 *)CONCAT22(a4,a3));' in c.function(doc, c.MOVED)

def test_moved_buffer_fields():
    """c_text._buffer_fields on the tree: words below one whose address is passed on are one array
    of them, laid out to the saved BP; a local never set inside a buffer passed on is read where it
    is, the buffer made to reach it; two buffers are one packed structure, as the frame had them."""
    word = {'kind': 'i16'}
    arr = lambda n: {'kind': 'array', 'elem': word, 'count': n}
    addr = lambda n: {'node': 'Unary', 'op': '&', 'x': typed(n, word)}
    doc = sixteen([local('iStack_10', word), local('iStack_e', word),
                   stmt(fcall('g', addr('iStack_10'))),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': typed('iStack_e', word)})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  int2 aiStack_10 [7];' in text and 'iStack_e' not in text, text
    assert '  g(&aiStack_10[0]);' in text and '  y = aiStack_10[1];' in text, text
    doc = sixteen([local('aiStack_a', arr(2)), local('iStack_6', word),
                   stmt(fcall('g', typed('aiStack_a', arr(2)))),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': typed('iStack_6', word)})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  int2 aiStack_a [4];' in text and 'int2 iStack_6' not in text, text
    assert '  y = (*(int2 *)((uint1 *)aiStack_a + 0x4));' in text, text
    doc = sixteen([local('aiStack_20', arr(2)), local('aiStack_a', arr(2)),
                   stmt(fcall('g', typed('aiStack_20', arr(2)), typed('aiStack_a', arr(2))))])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  _Packed struct {\n  int2 aiStack_20 [11];\n  int2 aiStack_a [4];\n  } codebreaker_frame;' in text, text
    assert '  g(codebreaker_frame.aiStack_20,codebreaker_frame.aiStack_a);' in text, text

def test_moved_far_reads():
    """c_text._far_reads on the tree: a far place read through a near cast is read far; behind two
    casts after a product (c_text's parser read a product there) it is left as it was."""
    word = {'kind': 'i16'}
    read = lambda: {'node': 'Unary', 'op': '*',
                    'x': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': fcall('MK_FP', name('s'), name('o'))}}
    two = {'node': 'Cast', 'type': {'kind': 'i32'}, 'x': {'node': 'Cast', 'type': word, 'x': read()}}
    doc = sixteen([stmt({'node': 'Assign', 'target': name('a'), 'value': read()}),
                   stmt({'node': 'Assign', 'target': name('b'), 'value': binary('*', name('b'), two)})])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  a = *(int2 __far *)((uint1 __far *)MK_FP(s, o));' in text, text
    assert '  b = b * (int4)(int2)*(int2 *)((uint1 __far *)MK_FP(s, o));' in text, text

def test_moved_near_to_far():
    """c_text._near_to_far on the tree: a number made a far pointer is an offset in DS,
    `(T __far *)(T *)(n)`; a four-byte variable's value is far already."""
    word = {'kind': 'i16'}
    p = typed('p', ptr(word))
    sets = lambda v: stmt({'node': 'Assign', 'target': p,
                           'value': {'node': 'Cast', 'type': ptr(word, 'near'), 'x': binary('+', v, lit(10))}})
    doc = sixteen([local('p', ptr(word)), local('w', {'kind': 'i32'}), sets(name('n')), sets(typed('w', {'kind': 'i32'}))])
    text = c.function(doc, c.MOVED, facts={'distance': 'far'})
    assert '  p = (int2 __far *)(int2 *)(n + 10);' in text, text
    assert '  p = (int2 __far *)(w + 10);' in text, text

def test_moved_stack_segment_stores():
    """c_text._stack_segment_stores on the tree: SS the original stores into its frame (`mov
    [bp-0Ch], ss`, a fact of the module: c_text._ss_store_places) is put back in the buffer that
    holds the place, before the first statement that passes the buffer on."""
    from codebreaker.decompile import c_text
    assert c_text._ss_store_places(b"\x90\x8c\x56\xf4\xc3", 0, 5) == [0xe]
    buf = {'kind': 'array', 'elem': {'kind': 'class', 'name': 'xunknown2'}, 'count': 4}
    doc = sixteen([local('axStack_10', buf), stmt({'node': 'Assign', 'target': name('a'), 'value': lit(1)}),
                   stmt(fcall('g', typed('axStack_10', buf)))])
    text = c.function(doc, c.MOVED, facts={'distance': 'far', 'ss_stores': [0xc]})
    assert '  a = 1;\n  (*(int2 *)((uint1 *)axStack_10 + 0x4)) = codebreaker_ss();\n  g(axStack_10);' in text, text

def test_moved_callee_pops():
    """c_text._callee_pops on the tree: in a function whose calls through pointers the callee
    cleans up after (a fact of the module), a cast to `code *` is `code_callee_pops *`."""
    fn = {'kind': 'fnptr', 'space': 'far', 'fn': {'kind': 'fn'}}
    call = {'node': 'Call', 'target': {'node': 'Unary', 'op': '*',
                                       'x': {'node': 'Cast', 'type': fn, 'x': name('p')}}, 'args': []}
    doc = sixteen([stmt(call)])
    assert '  (*(code_callee_pops *)p)();' in c.function(doc, c.MOVED, facts={'distance': 'far', 'callee_pops': True})
    assert '  (*(code *)p)();' in c.function(doc, c.MOVED, facts={'distance': 'far'})


def test_moved_caller_frame_slots():
    """c_text._caller_frame_slots on the tree: KEYBOARD's sub_9_0A62 clears two of its caller's
    locals through BP as the caller left it. Written once, each function reports what the module
    decides from (facts["frame"]); the module's plan (c.caller_frame_plan) has the helper take
    each place as a far pointer, in the older style, and each caller pass its own local's address
    there -- one it never declared declared first."""
    word = {'kind': 'class', 'name': 'xunknown2'}
    slot = lambda less: {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(word, 'near'),
                                                           'x': binary('+', name('unaff_BP', 'i16'), lit(-less, 'i16'))}}
    helper = dict(sixteen([local('unaff_BP', {'kind': 'i16'}),
                           stmt({'node': 'Assign', 'target': slot(4), 'value': lit(0, 'i16')}),
                           stmt({'node': 'Assign', 'target': slot(6), 'value': lit(0, 'i16')})]), name='h')
    caller = dict(sixteen([local('puStack_6', ptr({'kind': 'u8'}, 'near')), stmt(fcall('h'))]), name='g')
    near, far = {'distance': 'near', 'model': '__cdecl16near'}, {'distance': 'far', 'model': '__stdcall16far'}
    reports = {}
    for doc, facts in ((helper, dict(near)), (caller, dict(far))):
        c.function(doc, c.MOVED, facts=facts)
        reports[doc['name']] = facts['frame']
    assert reports['h']['slots'] == {4: 'xunknown2', 6: 'xunknown2'} and reports['g']['slots'] is None
    plan = c.caller_frame_plan(reports, {'h': False, 'g': True})
    assert plan['h'] == {'frame_slots': {4: 'xunknown2', 6: 'xunknown2'}}
    assert plan['g']['frame_declare'] == [('xunknown2', 'xStack_8')]
    text = c.function(helper, c.MOVED, facts=dict(near, **plan['h']))
    assert text.startswith('xunknown2 __cdecl16near h(frame_4,frame_6)\nxunknown2 __far *frame_4;\n'
                           'xunknown2 __far *frame_6;\n{\n'), text
    assert '*frame_4 = 0;' in text and 'unaff_BP' not in text
    text = c.function(caller, c.MOVED, facts=dict(far, **plan['g']))
    assert '{\n  xunknown2 xStack_8;\n  uint1 *puStack_6;\n' in text, text
    assert 'h((xunknown2 __far *)((uint1 __far *)MK_FP(codebreaker_ss(), &puStack_6)),' \
           '(xunknown2 __far *)((uint1 __far *)MK_FP(codebreaker_ss(), &xStack_8)));' in text
    # (a caller with no frame of its own: nothing is changed)
    assert c.caller_frame_plan(reports, {'g': False}) == {}


def test_moved_selector_imports():
    """c_text's import values on the tree: the made-up number an import used for its value is
    shown as (`mov es, __0040H`) is the import, FP_SEG(&__0040H) -- FP_OFF for one used for its
    offset -- and the imports so used are found for the header. Another number stays."""
    doc = sixteen([stmt({'node': 'Assign', 'target': name('x'), 'value': lit(0xd008, 'u16', text='0xd008')}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': lit(0xd010, 'u16', text='0xd010')}),
                   stmt({'node': 'Assign', 'target': name('z'), 'value': lit(0xd018, 'u16', text='0xd018')})])
    facts = {'distance': 'far', 'selector_values': {0xd008: ('__0040H', 'segment'), 0xd010: ('__WINFLAGS', 'offset')}}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '  x = FP_SEG(&__0040H);\n  y = FP_OFF(&__WINFLAGS);\n  z = 0xd018;' in text, text
    assert facts['selector_imports'] == {'__0040H': 'segment', '__WINFLAGS': 'offset'}


def test_moved_segment_values():
    """c_text's own segments as values on the tree: where a function's code holds the loader's
    fix-up for a segment's selector, the made-up selector the decompiler shows is DS for the data
    segment, a far function's selector for a code segment (c.segment_plan decides, for the module),
    and a far pointer passed whole -- an offset, then that selector -- is the function there
    (USER's sub_1_3514 passing the callback 1:3452 to an enumeration)."""
    functions = [(0x10000, 'f'), (0x13452, 'cb'), (0x20000, 'g')]
    where = {0x10000: (1, 0), 0x13452: (1, 0x3452), 0x20000: (2, 0)}
    sites = {1: [(0x10, 1), (0x12, 2)]}                  # (two fix-ups in f: segment 1's selector, segment 2's)
    plan = c.segment_plan(functions, {'g'}, where, sites, {1: b'', 2: b''}, 2, set(), 0x1000)
    assert set(plan) == {'f'}
    seg1 = '((uint2)((uint4)(void __far *)&f >> 16))'
    assert plan['f']['segment_values'] == {'0x1000': seg1, '0x1008': 'codebreaker_ds()'}
    doc = sixteen([stmt(fcall('EnumWindows', lit(0x3452, 'u16', text='0x3452'), lit(0x1000, 'u16', text='0x1000'))),
                   stmt({'node': 'Assign', 'target': name('x'), 'value': lit(0x1008, 'u16', text='0x1008')}),
                   stmt({'node': 'Assign', 'target': name('y'), 'value': lit(0x1010, 'u16', text='0x1010')})])
    text = c.function(doc, c.MOVED, facts=dict({'distance': 'far'}, **plan['f']))
    assert '  EnumWindows(((uint2)(uint4)(void __far *)&cb),%s);' % seg1 in text, text
    assert '  x = codebreaker_ds();\n  y = 0x1010;' in text, text


def test_moved_code_segment_reads():
    """c_text's _cs_reads and _cs_places on the tree (KEYBOARD's GetKeyNameText): tables beside
    its code read through CS by a pointer set to the first (0x261) and a number set to the second
    (0x290); a word table at 1C7h of places it then reads; a step (`+ 1`), a count (`iVar8 =
    0x2f`) and an index (`uStack_c = 0`, set from arithmetic too) are no places; a far pointer
    passed as (offset, CS) is read there too. Given "code_places", each is written as the member."""
    char, word = {'kind': 'char'}, {'kind': 'class', 'name': 'xunknown2'}
    cs, cs_call = name('unaff_CS', 'i16'), {'node': 'Call', 'target': name('codebreaker_cs'), 'args': []}
    at = lambda seg, off, t=char: {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(t, 'far'),
                                                                     'x': fcall('MK_FP', seg, off)}}
    put = lambda v, x: stmt({'node': 'Assign', 'target': name(v), 'value': x})
    p11, p12 = typed('pcVar11', ptr(char, 'near')), typed('pcVar12', ptr(char, 'near'))
    body = [local('pcVar11', ptr(char, 'near')), local('pcVar12', ptr(char, 'near')), local('iVar10', {'kind': 'i16'}),
            local('iVar8', {'kind': 'i16'}), local('uStack_c', {'kind': 'u16'}), local('unaff_CS', word),
            put('pcVar11', {'node': 'Cast', 'type': ptr(char, 'near'), 'x': lit(0x261, 'i16', '0x261')}),
            put('iVar10', lit(0x290, 'i16', '0x290')), put('iVar8', lit(0x2f, 'i16', '0x2f')), put('pcVar12', p11),
            put('uStack_c', binary('*', binary('+', name('a5', 'i16'), lit(-1, 'i16')), lit(2, 'i16'))),
            put('uStack_c', lit(0, 'i16')),
            put('pcVar11', binary('+', p11, lit(1, 'i16'))),
            put('iVar8', at(cs, binary('+', binary('+', p11, name('iVar10', 'i16')), p12))),
            put('pcVar11', {'node': 'Cast', 'type': ptr(char, 'near'),
                            'x': at(cs_call, binary('+', binary('*', name('a5', 'i16'), lit(2, 'i16')),
                                                    lit(0x1c7, 'i16', '0x1c7')), word)}),
            put('iVar8', at(cs, p11)),
            stmt(at(cs, binary('+', name('uStack_c', 'u16'), lit(0x30, 'i16', '0x30')))),
            stmt(fcall('KERNEL_128', lit(0x5a, 'i16', '0x5a'), cs, lit(0x3f, 'i16', '0x3f')))]
    doc = sixteen(body, [param('a5', {'kind': 'i16'})])
    facts = {'distance': 'far'}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '  xunknown2 unaff_CS = codebreaker_cs();' in text, text
    rep = facts['code_reads']
    assert rep['reads'] == 5 and not rep['unsafe'], rep
    inside = lambda v: 0 < v < 0x305
    assert sorted(v for v in rep['places'].values() if inside(v)) == [0x30, 0x5a, 0x1c7, 0x261, 0x290], rep['places']
    assert {v for kind, (_k, v) in rep['events'] if inside(v)} == {0x1c7} and rep['events'][0][0] == 'pointers'
    k = next(k for k, v in rep['places'].items() if v == 0x261)
    again = c.function(doc, c.MOVED, facts={'distance': 'far', 'code_places': {k: '(uint2)codebreaker_code_9.at_0261'}})
    assert '  pcVar11 = (char *)(uint2)codebreaker_code_9.at_0261;' in again, again
    assert c_text._string_literal(b"type\0subtype\0") == '"type\\000subtype"'


def test_names_used_for_the_header():
    """What the header is written from, read off the tree as the function is written (c_text read
    every text): the globals named by address -- `_pcRam..`, written pcRam.. by a pass, among them
    -- those called through, the calls, MK_FP."""
    fn = {'kind': 'fnptr', 'space': 'far', 'fn': {'kind': 'fn'}}
    g = {'node': 'Name', 'sym': {'kind': 'global', 'name': '_pcRam1000001a'}, 'type': fn}
    doc = sixteen([stmt({'node': 'Call', 'target': {'node': 'Unary', 'op': '*', 'x': g}, 'args': [lit(1, 'i16')]}),
                   stmt({'node': 'Assign', 'target': name('iRam10080012', 'i16'), 'value': fcall('KERNEL_3', lit(2, 'i16'))}),
                   stmt(fcall('MK_FP', lit(0, 'i16'), lit(1, 'i16')))])
    facts = {'distance': 'far'}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '(*pcRam1000001a)(1);' in text, text
    got = facts['names']
    assert got['globals'] == {('pc', '1000001a'), ('i', '10080012')} and got['called_through'] == {'pcRam1000001a'}
    assert {'KERNEL_3', 'MK_FP'} <= got['called'] and got['mk_fp'] and not got['segments']


def test_prototypes_where_the_calls_agree():
    """c_text.prototypes from what the back end wrote: g takes a word and every call passes one, so
    it is declared with its parameter; h is passed a long where it takes a word, so its list is
    left open; a heading in the newer style (`(void)`, a blank line before its `{`) is none
    c_text's pattern read, and gets no prototype."""
    word = {'kind': 'i16'}
    far = {'distance': 'far', 'model': '__stdcall16far'}
    g = dict(sixteen([stmt({'node': 'Assign', 'target': name('x'), 'value': name('a1', 'i16')})], [param('a1', word)]), name='g')
    h = dict(g, name='h')
    f = dict(sixteen([stmt(fcall('g', lit(1, 'i16'))), stmt(fcall('h', lit(0x12345, 'i32', '0x12345')))]), name='f')
    lost = dict(sixteen([]), name='k', mods=['__fastcall16far'])
    heads = {}
    for doc in (f, g, h, lost):
        facts = dict(far)
        c.function(doc, c.MOVED, facts=facts)
        heads[doc['name']] = facts['heading']
    assert heads['g']['declaration'] == 'xunknown2 __stdcall16far g();'
    assert ('g', ['1']) in heads['f']['calls']
    assert c.prototypes(heads, ['f', 'g', 'h', 'k']) == {'f': 'xunknown2 __stdcall16far f(void);',
                                                          'g': 'xunknown2 __stdcall16far g(int2 a1);'}
    assert c.argument_words('(uint4)x', {}) == 2 and c.argument_words('&x', {}) == 1
    # (c_text._arguments_at's frozen case)
    assert c.arguments_at("*MK_FP(iVar2,piVar1 + 1)", 6) == (["iVar2", "piVar1 + 1"], 24)


def test_moved_32bit_heading():
    """compilable32's heading rules on the tree, for the module's C (given facts): a `void`
    result is `int` (EAX is there whatever the core saw), and `__thiscall` -- OpenWatcom has no
    word for it -- is left out and reported, for the header's #pragma. Without the module's facts
    (the C the decompiler is read for) the heading is as the core printed it."""
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'}, 'mods': ['__thiscall'],
           'origin': {'module': '', 'va': 0x401000}, 'params': [param('self', ptr({'kind': 'i32'}))],
           'body': [stmt({'node': 'Assign', 'target': name('x'), 'value': lit(1)})]}
    facts = {}
    text = c.function(doc, c.MOVED, facts=facts)
    assert text.startswith('int f(int4 *self)'), text
    assert facts['registers'] == 'ecx'
    assert c.function(doc, c.MOVED).startswith('void __thiscall f(int4 *self)')
    assert c.function(dict(doc, mods=['__fastcall']), c.MOVED, facts=facts).startswith('int f(') and facts['registers'] == 'edx'
    # an offset of a recovered structure that holds no field (compilable32's _struct_gaps): the
    # byte there, counted from the structure's start as a byte pointer
    rec = {'kind': 'class', 'name': 's_f_p'}
    gap = {'node': 'Select', 'obj': {'node': 'Unary', 'op': '*', 'x': typed('p', ptr(rec))},
           'sym': {'kind': 'field', 'name': 'field_0x4'}, 'type': {'kind': 'u8'}}
    flat = dict(doc, mods=[], body=[stmt({'node': 'Assign', 'target': name('x'), 'value': gap})])
    assert '  x = (*((uint1 *)(p) + 0x4));' in c.function(flat, c.MOVED, facts={})
    assert '  x = p->field_0x4;' in c.function(flat, c.MOVED)
    # a function's address stored in a pointer the core typed loosely (compilable32's _code_pointers)
    store = dict(doc, mods=[], body=[stmt({'node': 'Assign', 'target': name('a3'), 'value': name('sub_004583')}),
                                     stmt({'node': 'Assign', 'target': name('b'), 'value': name('other')})])
    text = c.function(store, c.MOVED, facts={'functions': {'sub_004583', 'f'}})
    assert '  a3 = (code *)sub_004583;\n  b = other;' in text, text
    # a pointer compared with a number is compared as one (compilable32's _compare_as_numbers)
    cmp = dict(doc, mods=[], body=[local('pcVar2', ptr({'kind': 'char'})),
                                   {'node': 'If', 'cond': binary('<', typed('pcVar2', ptr({'kind': 'char'})),
                                                                 {'node': 'Cast', 'type': {'kind': 'u32'}, 'x': name('a1')}),
                                    'then': [stmt({'node': 'Assign', 'target': name('x'), 'value': lit(1)})]}])
    assert '  if ((uint4)pcVar2 < (uint4)a1) {' in c.function(cmp, c.MOVED, facts={})
    assert '  if (pcVar2 < (uint4)a1) {' in c.function(cmp, c.MOVED)
    sixteen_doc = sixteen([], [])
    assert c.function(sixteen_doc, c.MOVED, facts={'distance': 'far'}).startswith('xunknown2 __far f()')


def test_moved_keep_frame():
    """compilable32's _keep_frame on the tree: where a frame local's address escapes, the locals
    are the fields of one frame structure at their offsets (filler between, the rest of the frame
    to the return address after the last), each use `_f.x`, the other declarations' lines left
    empty as c_text left them; without an escape, or without the module's facts, as printed."""
    w2, w1, i4 = {'kind': 'class', 'name': 'xunknown2'}, {'kind': 'class', 'name': 'xunknown1'}, {'kind': 'i32'}

    def addr(n, t):
        return {'node': 'Cast', 'type': i4, 'x': {'node': 'Unary', 'op': '&', 'x': typed(n, t)}}
    body = [local('iVar1', i4), local('xStack_c', w2), local('xStack_a', w1), local('xStack_8', w1),
            local('xStack_7', w1),
            stmt({'node': 'Assign', 'target': typed('xStack_c', w2), 'value': name('_cRam00405080')}),
            stmt({'node': 'Assign', 'target': typed('xStack_7', w1), 'value': lit(0)}),
            stmt({'node': 'Assign', 'target': name('iVar1'),
                  'value': fcall('lstrcmpiA', addr('xStack_8', w1), addr('xStack_c', w2))}),
            {'node': 'Return', 'expr': name('iVar1')}]
    doc = {'node': 'MethodDecl', 'name': 'sub_004419', 'kind': 'function', 'ret': i4, 'mods': ['__cdecl'],
           'origin': {'module': '', 'va': 0x404419}, 'params': [], 'body': body}
    text = c.function(doc, c.MOVED, facts={})
    assert ('{\n  int4 iVar1;\n  struct { xunknown2 xStack_c; xunknown1 xStack_a; uint1 _gap_8[1]; xunknown1 xStack_8; '
            'xunknown1 xStack_7; uint1 _rest[6]; } _f;   /* locals at their offsets in the original\'s frame */\n\n\n\n'
            '  \n  _f.xStack_c = _cRam00405080;\n  _f.xStack_7 = 0;\n'
            '  iVar1 = lstrcmpiA((int4)&_f.xStack_8,(int4)&_f.xStack_c);\n') in text, text
    assert '  xStack_c = _cRam00405080;' in c.function(doc, c.MOVED)
    # (no address taken, no array, no entry SP: the locals stay where the compiler puts them)
    plain = dict(doc, body=body[:-2] + [{'node': 'Return', 'expr': name('iVar1')}])
    assert '  xunknown2 xStack_c;\n' in c.function(plain, c.MOVED, facts={})
    # (a type of unknown size: left as it is; the structures recovered say theirs)
    rec = {'kind': 'class', 'name': 's_f_p'}
    odd = dict(doc, body=[local('xStack_c', rec), stmt(fcall('g', addr('xStack_c', rec)))])
    assert '  s_f_p xStack_c;' in c.function(odd, c.MOVED, facts={})
    assert 'struct { s_f_p xStack_c; uint1 _rest[4]; } _f;' in \
        c.function(odd, c.MOVED, facts={'struct_sizes': {'s_f_p': 8}})


def test_moved_32bit_globals():
    """compilable32's rules over the module's globals on the tree, each function given its part of
    what the module decided (module_names32): a global named as a number but read through is read
    through as a pointer there, `*(int4 *)iRam..` (not after a cast the parser took for a product,
    nor one the module calls through); one the function uses at another width is its alias; a
    pointer global compared with a number cast to an integer is cast the same."""
    i4 = {'kind': 'i32'}
    deref = {'node': 'Unary', 'op': '*', 'x': name('iRam0040c000')}
    body = [stmt({'node': 'Assign', 'target': name('a'), 'value': deref}),
            stmt({'node': 'Assign', 'target': name('b'),
                  'value': {'node': 'Index', 'arr': name('uRam0040c010'), 'index': lit(2)}}),
            stmt({'node': 'Assign', 'target': name('d'),
                  'value': {'node': 'Cast', 'type': i4, 'x': {'node': 'Unary', 'op': '*', 'x': name('iRam0040c000')}}}),
            stmt({'node': 'Assign', 'target': name('e'), 'value': name('iRam0040c020')}),
            stmt(binary('!=', {'node': 'Cast', 'type': {'kind': 'u32'}, 'x': name('pcVar3')}, name('pcRam004094b0'))),
            stmt({'node': 'Call', 'target': {'node': 'Unary', 'op': '*', 'x': name('xRam0040c030')}, 'args': []})]
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': i4,
           'origin': {'module': '', 'va': 0x401000}, 'params': [], 'body': body}
    named = c.module_names32(doc)
    assert named['called_through'] == {'xRam0040c030'}
    assert {'iRam0040c000', 'pcRam004094b0'} <= named['names']
    facts = {'called_through': named['called_through'], 'aliases': {'iRam0040c020': 'iRam0040c020_2'},
             'pointer_globals': {'pcRam004094b0'}}
    text = c.function(doc, c.MOVED, facts=facts)
    assert '  a = *(int4 *)iRam0040c000;\n' in text
    assert '  b = ((uint4 *)uRam0040c010)[2];\n' in text
    assert '  d = (int4)*iRam0040c000;\n' in text          # (`(int4) * iRam..` to c_text's parser)
    assert '  e = iRam0040c020_2;\n' in text
    assert '  (uint4)pcVar3 != (uint4)pcRam004094b0;\n' in text
    assert '  (*xRam0040c030)();\n' in text
    assert '  a = *iRam0040c000;\n' in c.function(doc, c.MOVED)


def test_moved_fit_calls():
    """compilable32's _fit_calls on the tree: a call passing another number of arguments than the
    function's definition takes (takes32, from its heading: none for a __cdecl one) is made
    through a pointer with the list left open; the others as they are."""
    assert c.takes32('int4 CopyLZFile(int4 a1,int4 a2)') == 2
    assert c.takes32('int4 f(void)') == 0
    assert c.takes32('int4 __cdecl LZDone(void)') is None
    body = [stmt(fcall('sub_001135', name('a'))), stmt(fcall('sub_001135', name('a'), name('b'))),
            stmt(fcall('LZDone', name('a')))]
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'i32'},
           'origin': {'module': '', 'va': 0x401000}, 'params': [], 'body': body}
    text = c.function(doc, c.MOVED, facts={'takes': {'sub_001135': 2}})
    assert '  ((int4 (*)())sub_001135)(a);\n  sub_001135(a,b);\n  LZDone(a);\n' in text, text
    assert '  sub_001135(a);\n' in c.function(doc, c.MOVED)


def test_moved_powerpc_passes():
    """compilable_powerpc's passes on the tree (the facts' lane): an address widened to 64 bits is
    the 32-bit one; a piece is PIECE(x, off, T), from the end of its storage; a slot the caller
    keeps for a register argument is that argument, its declaration and the store of it into its
    own slot gone; a function returning nothing returns uint8 (r3 is there) and says so; the
    definition is in the older style."""
    u8, w1 = {'kind': 'u64'}, {'kind': 'class', 'name': 'xunknown1'}
    home = typed('xStack00000010', w1)
    body = [local('iVar1', {'kind': 'i32'}), local('xStack00000010', w1),
            stmt({'node': 'Assign', 'target': typed('_xStack00000010', w1), 'value': name('param_1')}),
            stmt(fcall('sub_93012470', {'node': 'Unary', 'op': '&', 'x': home})),
            stmt({'node': 'Assign', 'target': name('iVar1'), 'value': select(name('x'), '_4_4_')}),
            stmt({'node': 'Assign', 'target': name('iVar1'), 'value': lit(0xffffffff913e16f8, 'u64', '0xffffffff913e16f8')})]
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'void'},
           'origin': {'module': '', 'va': 0x913E5000}, 'params': [param('param_1', u8), param('param_2', u8)],
           'body': body}
    facts = {'lane': 'powerpc'}
    text = c.function(doc, c.MOVED, facts=facts)
    assert text.startswith('uint8 f(param_1,param_2)\nuint8 param_1;\nuint8 param_2;\n{\n  int4 iVar1;\n  \n'), text
    assert 'xStack00000010' not in text and '  sub_93012470(&param_1);\n' in text, text
    assert '  iVar1 = PIECE(x, 4, uint4);\n' in text and '  iVar1 = 0x913e16f8;\n' in text, text
    assert facts['novalue']
    assert c.function(doc, c.MOVED).startswith('void f(uint8 param_1,uint8 param_2)')
    # how many arguments each callee is called with, from the trees (c_text.call_counts: where every
    # call agrees; `p->name(...)` is a call of name)
    member = {'node': 'Call', 'target': select({'node': 'Unary', 'op': '*', 'x': name('p')}, 'import1_ord_16'),
              'args': [lit(1), lit(2)]}
    calls = dict(doc, body=[stmt(fcall('import0_ord_5', lit(1))), stmt(fcall('import0_ord_5', lit(1), lit(2))),
                            stmt(fcall('import0_ord_7')), stmt(member)])
    arity = c.module_names32(calls)['arity']
    assert arity == [('import0_ord_5', 1), ('import0_ord_5', 2), ('import0_ord_7', 0), ('import1_ord_16', 2)]
    seen = {}
    for callee, k in arity:
        seen.setdefault(callee, set()).add(k)
    assert c_text.call_counts(seen, {'import0_ord_5', 'import0_ord_7', 'import1_ord_16'}) == \
        {'import0_ord_7': 0, 'import1_ord_16': 2}


def test_moved_struct_fields():
    """struct_text on the tree: `*(T *)(p + N)` through an argument known to be a structure is
    `((S *)p)->f_N`, declared at that offset with the type most of the module's accesses there
    use; an access of another type at the same place is left alone, and so is `*(T *)p->f` (a
    field's deref, which the old pattern over the text took for `*(T *)p` -- COMCTL32's
    `((struct_7 *)a1)->f_0->f_1c`)."""
    from codebreaker.analyze import image, structures
    from codebreaker.decompile import struct_text
    from codebreaker.verify.authored import match
    i4, u1 = {'kind': 'i32'}, {'kind': 'u8'}
    code = {'kind': 'class', 'name': 'code'}

    def at(t, off=None, p='a1'):
        x = name(p) if off is None else binary('+', name(p), lit(off))
        return {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(t, 'flat'), 'x': x}}
    rec = {'kind': 'ptr', 'space': 'flat', 'to': {'kind': 'class', 'name': 's_f_a1'}}
    deref_field = {'node': 'Unary', 'op': '*', 'x': {'node': 'Cast', 'type': ptr(i4, 'flat'),
                                                     'x': {'node': 'Select', 'obj': {'node': 'Unary', 'op': '*', 'x': typed('a1', rec)},
                                                           'sym': {'kind': 'field', 'name': 'f_1c'}}}}
    call = {'node': 'Call', 'target': {'node': 'Unary', 'op': '*', 'x': at(ptr(code, 'flat'), 0x10)}, 'args': [name('a2')]}
    f = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': i4, 'origin': {'module': '', 'va': 0x1000},
         'params': [param('a1', i4), param('a2', i4)],
         'body': [stmt({'node': 'Assign', 'target': at(i4, 8), 'value': binary('+', at(i4), name('a2'))}),
                  {'node': 'Return', 'expr': binary('+', binary('+', at(u1, 8), at(i4, 8)), call)},
                  stmt({'node': 'Assign', 'target': name('x'), 'value': deref_field})]}
    g = {'node': 'MethodDecl', 'name': 'g', 'kind': 'function', 'ret': i4, 'origin': {'module': '', 'va': 0x2000},
         'params': [param('a1', i4)], 'body': [{'node': 'Return', 'expr': at(i4, 8)}]}
    assert c.parameter_names(f) == ['a1', 'a2']
    found = [{"name": "struct_1", "roots": [("arg", 0x1000, 0)]}]
    real = (image.load, structures.recover)
    image.load, structures.recover = (lambda path: None), (lambda img: found)
    try:
        planned = struct_text.plan("x.dll", {'f': f, 'g': g}, {0x1000: "f", 0x2000: "g"},
                                   {'f': 'int4 f(int4 a1,int4 a2)', 'g': 'int4 g(int4 a1)'})
    finally:
        image.load, structures.recover = real
    assert set(planned) == {'f'}                         # (g's argument is not known to be one)
    facts = {'struct_fields': planned['f']}
    text = c.function(f, c.MOVED, facts=facts)
    assert "((struct_1 *)a1)->f_8 = ((struct_1 *)a1)->f_0 + a2;" in text, text
    assert "*(uint1 *)(a1 + 8) + ((struct_1 *)a1)->f_8 + (*((struct_1 *)a1)->f_10)(a2)" in text, text
    assert "x = *(int4 *)a1->f_1c;" in text, text
    assert facts['structs_used'] == {'struct_1'}
    header = "typedef int int4;\n" + struct_text.declarations(planned, facts['structs_used'])
    assert "typedef struct struct_1 {\n  int4 f_0;\n  uint1 _gap_4[4];\n  int4 f_8;\n  uint1 _gap_c[4];\n  code *f_10;\n} struct_1;" in header, header
    near = match.needed_header(header, text)
    assert "int4 f_8;" in near and "struct_1" not in match.needed_header(header, c.function(g, c.MOVED, facts={}))
    assert c.arguments_at("*MK_FP(iVar2,piVar1 + 1)", 6) == (["iVar2", "piVar1 + 1"], 24)
    assert c_text._string_literal(b"\x04\x00\x22\x00") is None