"""A latent bug found while moving exact/aim to C++ (native/cb/exact):

  aim._norm       int(text, 0) raises ValueError for a decimal with a leading zero ("08"); the number is read by its own base
(The twin bug of exact/standin, a four-bit fix-up location, is native/cb/exact's standin_test: a_fixup_location_is_four_bits.)
"""
from codebreaker.exact import aim


class _Insn(object):
    def __init__(self, mnemonic, op_str):
        self.mnemonic, self.op_str = mnemonic, op_str


def test_aim_reads_a_decimal_with_a_leading_zero():
    assert aim._norm(_Insn("mov", "ax, word ptr [bx + 08]"), 0) == "mov ax, word ptr [bx+8]"
    assert aim._norm(_Insn("mov", "ax, 0x20"), 0) == "mov ax, 0x20"
    assert aim._norm(_Insn("mov", "ax, 0x80"), 0) == "mov ax, N"
