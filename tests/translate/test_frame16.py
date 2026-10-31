# -*- coding: utf-8 -*-
"""The 16-bit frame and shape passes on a function's L2 tree (decompile/frame16.py, shape16.py;
CB_FRAME16=1): locals laid out as the original's frame, statements in the form C 8 writes them."""

from codebreaker.decompile import frame16, shape16
from codebreaker.translate import l2
from codebreaker.translate.backends import c


def lit(v):
    return {'node': 'Lit', 'type': {'kind': 'i16'}, 'value': v}


def name(n, kind='i16'):
    return {'node': 'Name', 'type': {'kind': kind}, 'sym': {'kind': 'local', 'name': n}}


def assign(target, value, at):
    return {'node': 'ExprStmt', 'origin': {'at': at},
            'expr': {'node': 'Assign', 'target': target, 'value': value, 'type': target['type']}}


def local(n, kind='i16', at=None):
    s = {'node': 'Local', 'sym': {'kind': 'local', 'name': n}, 'type': {'kind': kind}}
    if at is not None:
        s['at'] = at
    return s


def function(body):
    doc = {'node': 'MethodDecl', 'name': 'f', 'kind': 'function', 'ret': {'kind': 'i16'}, 'params': [],
           'sym': {'kind': 'function', 'name': 'f', 'type': {'kind': 'fn', 'conv': 'cdecl', 'distance': 'far',
                                                             'params': [], 'result': {'at': 'none'}}},
           'mods': ['__cdecl16far'], 'body': body}
    assert not list(l2.validate(doc) or ())
    return doc


# push bp / mov bp, sp / sub sp, 4 / mov word ptr [bp-2], 0 / inc word ptr [bp-2] / mov ax, [bp-2] /
# mov sp, bp / pop bp / ret
FLAG = bytes.fromhex('55 8bec 83ec04 c746fe0000 ff46fe 8b46fe 8be5 5d c3'.replace(' ', ''))


def flag_doc():
    return function([
        local('bVar1', 'bool', 'none'),
        assign(name('bVar1', 'bool'), lit(0), [6]),
        assign(name('bVar1', 'bool'), lit(1), [11]),
        {'node': 'Return', 'expr': name('bVar1', 'bool'), 'origin': {'at': [14]}}])


def test_a_flag_kept_in_a_word_is_a_word():
    doc, said = frame16.lay_out(flag_doc(), FLAG, 0, len(FLAG))
    assert doc['body'][0]['type'] == {'kind': 'i16'}               # (the decompiler's bool: 2 bytes at [bp-2])
    assert said == ['bVar1: 2 bytes at [bp-2]']


def test_inc_is_written_as_plus_plus():
    doc, said = shape16.reshape(flag_doc(), FLAG)
    assert said == ['bVar1++']
    assert doc['body'][2]['expr']['node'] == 'Unary' and doc['body'][2]['expr']['postfix']
    assert doc['body'][1]['expr']['node'] == 'Assign'               # (the mov stays an assignment)
    text = c.function(doc)
    assert 'bVar1++;' in text and 'bVar1 = 0;' in text


def test_an_inc_of_a_register_stays_an_assignment():
    # inc ax / ret: the decompiler's `x = in_AX + 1` -- x++ would read x, not AX
    code = bytes.fromhex('40c3')
    doc = function([local('x'), assign(name('x'), {'node': 'Binary', 'op': '+', 'left': name('in_AX'),
                                                   'right': lit(1), 'type': {'kind': 'i16'}}, [0])])
    got, said = shape16.reshape(doc, code)
    assert said == [] and got == doc
    # dec cx / ret: `b = p == 0` (the flag the dec set) is no b--; `x = x - 1` is x--
    code = bytes.fromhex('49c3')
    doc = function([local('b', 'bool'), local('x'),
                    assign(name('b', 'bool'), {'node': 'Binary', 'op': '==', 'left': name('p'), 'right': lit(0),
                                               'type': {'kind': 'bool'}}, [0]),
                    assign(name('x'), {'node': 'Binary', 'op': '+', 'left': name('x'), 'right': lit(-1),
                                       'type': {'kind': 'i16'}}, [0])])
    got, said = shape16.reshape(doc, code)
    assert said == ['x--']


def test_an_inc_of_a_place_is_not_a_larger_step():
    # inc word ptr [bp-2] said to be `x = x + 3` (one of the statement's instructions): left
    doc = function([local('x'), assign(name('x'), {'node': 'Binary', 'op': '+', 'left': name('x'), 'right': lit(3),
                                                   'type': {'kind': 'i16'}}, [11])])
    got, said = shape16.reshape(doc, FLAG)
    assert said == []


def test_a_function_without_a_frame_is_left():
    code = bytes.fromhex('b80100c3')                                # mov ax, 1 / ret
    doc = function([{'node': 'Return', 'expr': lit(1), 'origin': {'at': [0]}}])
    got, said = frame16.lay_out(doc, code, 0, len(code))
    assert got == doc and said == ['no frame']


# push bp / mov bp, sp / sub sp, 12h / lea ax, [bp-10h] / push ax / call $+3 / add sp, 2 /
# mov ax, [bp-10h] / add ax, [bp-0Ch] / mov word ptr [bp-12h], 5 / mov sp, bp / pop bp / ret
BUFFER = bytes.fromhex('55 8bec 83ec12 8d46f0 50 e80000 83c402 8b46f0 0346f4 c746ee0500 8be5 5d c3'
                       .replace(' ', ''))


def test_the_pieces_of_a_buffer_are_one_array():
    doc = function([
        local('iVar1', 'i16', 'none'),
        local('xStack_12', 'i16', {'stack': -0x12}),
        local('xStack_e', 'i16', {'stack': -0xe}),
        assign(name('iVar1'), lit(5), [22]),
        {'node': 'Return', 'expr': {'node': 'Binary', 'op': '+', 'left': name('xStack_12'),
                                    'right': name('xStack_e'), 'type': {'kind': 'i16'}}, 'origin': {'at': [16, 19]}}])
    got, said = frame16.lay_out(doc, BUFFER, 0, len(BUFFER))
    decls = [s for s in got['body'] if s['node'] == 'Local']
    assert [d['sym']['name'] for d in decls] == ['local_10', 'iVar1']   # (nearest the frame pointer first)
    assert decls[0]['type'] == {'kind': 'array', 'elem': {'kind': 'u8'}, 'count': 16}
    assert sorted(said) == ["xStack_12: local_10's bytes 0..", "xStack_e: local_10's bytes 4.."]
    names = {n['sym']['name'] for n in frame16._walk(got['body']) if n.get('node') == 'Name'}
    assert 'xStack_12' not in names and 'xStack_e' not in names
    assert not list(l2.validate(got) or ())
    text = c.function(got)
    assert 'uint1 local_10 [16];' in text
    assert 'return *(int2 *)local_10 + *(int2 *)(local_10 + 4);' in text


def test_the_back_end_writes_postfix_increments():
    deref = {'node': 'Unary', 'op': '*', 'x': name('p')}
    assert c.expr({'node': 'Unary', 'op': '++', 'postfix': True, 'x': deref}) == '(*p)++'
    assert c.expr({'node': 'Unary', 'op': '--', 'postfix': True, 'x': name('x')}) == 'x--'


def test_an_if_return_is_turned_round_where_the_original_has_the_other_first():
    cond = {'node': 'Binary', 'op': '==', 'left': name('x'), 'right': lit(0), 'type': {'kind': 'bool'}}
    doc = function([
        local('x'),
        {'node': 'If', 'cond': cond, 'then': [{'node': 'Return', 'expr': lit(1), 'origin': {'at': [9]}}]},
        {'node': 'Return', 'expr': lit(2), 'origin': {'at': [5]}}])
    got, said = shape16.reshape(doc, bytes(16))
    assert said == ['an if-return turned round']
    assert got['body'][1]['then'][0]['expr']['value'] == 2 and got['body'][2]['expr']['value'] == 1
    assert got['body'][1]['cond']['op'] == '!'
    # (in the order the original has them: unchanged)
    got, said = shape16.reshape(function(doc['body'][:1] + [dict(doc['body'][1], then=[
        {'node': 'Return', 'expr': lit(1), 'origin': {'at': [3]}}]), doc['body'][2]]), bytes(16))
    assert said == []


HELD = 'a buffer left as it was: a far pointer in it, an array in the frame, or a local over it ([bp-16])'


def _select(obj, field, kind='u16'):
    return {'node': 'Select', 'type': {'kind': kind}, 'obj': obj, 'sym': {'kind': 'field', 'name': field}}


def _piece_doc(extra=()):
    return function([
        local('xStack_12', 'i16', {'stack': -0x12}),
        local('xStack_e', 'i16', {'stack': -0xe})] + list(extra) + [
        {'node': 'Return', 'expr': {'node': 'Binary', 'op': '+', 'left': _select(name('xStack_12'), '_1_1_', 'u8'),
                                    'right': name('xStack_e'), 'type': {'kind': 'i16'}}, 'origin': {'at': [16, 19]}}])


def test_a_piece_of_a_buffer_piece_is_read_at_its_place():
    # x._1_1_ of a piece: the byte at its place + 1 (written `->_1_1_` of a cast it did not compile,
    # SUWIN 3:00F6)
    got, said = frame16.lay_out(_piece_doc(), BUFFER, 0, len(BUFFER))
    names = {n['sym']['name'] for n in frame16._walk(got['body']) if n.get('node') == 'Name'}
    assert names == {'local_10'}
    assert not any(n.get('node') == 'Select' for n in frame16._walk(got['body']))
    text = c.function(got)
    assert '*(uint1 *)(local_10 + 1)' in text and '_1_1_' not in text


def test_a_buffer_holding_a_far_pointer_is_left():
    # `_x`, the decompiler's x joined with the word above it (a far pointer's halves), or a piece
    # used as a far pointer: inside a cast its __far is lost and two bytes move for four (SUWIN
    # 2:410C passed SETUPX.2 a pointer's offset and 0) -- the buffer is left as it was
    far = {'kind': 'ptr', 'space': 'far', 'to': {'kind': 'char', 'signed': True}}
    for target in ({'node': 'Name', 'type': far, 'sym': {'kind': 'local', 'name': '_xStack_12'}},
                   {'node': 'Name', 'type': far, 'sym': {'kind': 'local', 'name': 'xStack_e'}}):
        doc = _piece_doc([{'node': 'ExprStmt', 'origin': {'at': [16]},
                           'expr': {'node': 'Assign', 'target': target, 'value': lit(0), 'type': far}}])
        got, said = frame16.lay_out(doc, BUFFER, 0, len(BUFFER))
        assert said == [HELD]
        assert [s['sym']['name'] for s in got['body'] if s['node'] == 'Local'] == ['xStack_e', 'xStack_12']
    # (and one with an array below it: the back end makes such an array reach up over what it reads,
    # backends/c._buffer_fields -- SUWIN 4:503E's axStack_52 over the pieces at [bp-30h])
    doc = _piece_doc()
    doc['body'].insert(0, {'node': 'Local', 'sym': {'kind': 'local', 'name': 'axStack_14'}, 'at': {'stack': -0x14},
                           'type': {'kind': 'array', 'elem': {'kind': 'u8'}, 'count': 2}})
    got, said = frame16.lay_out(doc, BUFFER, 0, len(BUFFER))
    assert said[0] == HELD



def test_a_buffer_with_an_array_among_its_pieces_is_left():
    # (the back end reads the words a callee fills above an array whose address is passed on as its
    # fields, backends/c._buffer_fields: cut out as a piece of its own, VERX's VERQUERYVALUE lost them)
    arr = {'kind': 'array', 'elem': {'kind': 'char', 'signed': True}, 'count': 4}
    doc = function([
        local('xStack_12', 'i16', {'stack': -0x12}),
        {'node': 'Local', 'sym': {'kind': 'local', 'name': 'acStack_e'}, 'at': {'stack': -0xe}, 'type': arr},
        {'node': 'Return', 'origin': {'at': [16, 19]}, 'expr': {
            'node': 'Index', 'type': {'kind': 'char', 'signed': True}, 'index': lit(1),
            'arr': {'node': 'Name', 'type': arr, 'sym': {'kind': 'local', 'name': 'acStack_e'}}}}])
    got, said = frame16.lay_out(doc, BUFFER, 0, len(BUFFER))
    assert said == [HELD]
    assert 'local_10' not in c.function(got)


def test_an_unknown_local_in_a_place_of_its_own_keeps_its_type():
    # only a bool takes its place's width: an xunknown local's type is its uses' (a pointer made
    # an int2 was dereferenced, SUWIN 9:0924)
    doc = flag_doc()
    doc['body'][0]['type'] = {'kind': 'class', 'name': 'xunknown2'}
    for s in doc['body'][1:3]:
        s['expr']['target']['type'] = s['expr']['type'] = {'kind': 'class', 'name': 'xunknown2'}
    got, said = frame16.lay_out(doc, FLAG, 0, len(FLAG))
    assert got['body'][0]['type'] == {'kind': 'class', 'name': 'xunknown2'} and said == []
