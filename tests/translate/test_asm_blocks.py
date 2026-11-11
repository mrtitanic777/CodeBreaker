# -*- coding: utf-8 -*-
"""The _asm blocks pass on a function's L2 tree (decompile/asm_blocks.py, CB_ASM_BLOCKS): two faults found
while the pass was rewritten in C++ (native/cb/decompile/asm_blocks)."""

from codebreaker.decompile import asm_blocks

# push bp / mov bp,sp / sub sp,4 / mov ax,4400h / mov bx,5 / int 21h / jb +3 / mov ax,[bp-2] / mov sp,bp / pop bp / ret
CODE = bytes.fromhex('55 8bec 83ec04 b80044 bb0500 cd21 7203 8b46fe 8be5 5d c3'.replace(' ', ''))


def _call_int21(at):
    return {'node': 'ExprStmt', 'origin': {'at': at},
            'expr': {'node': 'Call', 'target': {'node': 'Name', 'sym': {'name': 'int21'}}, 'args': []}}


def test_a_decimal_with_a_leading_zero_in_an_operand_is_a_place():
    # capstone writes decimals plainly, but int("08", 0) is an error in Python: one such operand ended the whole pass
    assert asm_blocks._named('mov ax, word ptr [bp - 08]', {-8: 'local'}) == 'mov ax, word ptr local'
    assert asm_blocks._named('mov ax, word ptr [bp + 0x10]', {16: 'far'}) == 'mov ax, word ptr far'


def test_a_window_that_leaves_no_instruction_places_no_block():
    # the two ends moved past each other: the pass wrote an empty block ("0020-0017") or raised IndexError
    doc = {'node': 'Function', 'body': [_call_int21([12])]}
    got, said = asm_blocks.place(doc, CODE, 0, len(CODE), window=(20, -20))
    assert said == ['the window leaves no instruction in the block']
    assert got == doc
    got, said = asm_blocks.place(doc, CODE, 0, len(CODE), window=(0, -40))
    assert said == ['the window leaves no instruction in the block'] and got == doc
