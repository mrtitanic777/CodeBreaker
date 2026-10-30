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
