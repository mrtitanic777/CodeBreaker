"""Two latent bugs found while moving exact/aim and exact/standin to C++ (native/cb/exact):

  aim._norm       int(text, 0) raises ValueError for a decimal with a leading zero ("08"); the number is read by its own base
  standin.fixups  the location of a fix-up is four bits; it was masked to three, so a 32-bit offset (9) read as a low byte (1)
"""
from codebreaker.exact import aim, standin


class _Insn(object):
    def __init__(self, mnemonic, op_str):
        self.mnemonic, self.op_str = mnemonic, op_str


def test_aim_reads_a_decimal_with_a_leading_zero():
    assert aim._norm(_Insn("mov", "ax, word ptr [bx + 08]"), 0) == "mov ax, word ptr [bx+8]"
    assert aim._norm(_Insn("mov", "ax, 0x20"), 0) == "mov ax, 0x20"
    assert aim._norm(_Insn("mov", "ax, 0x80"), 0) == "mov ax, N"


def test_standin_keeps_a_four_bit_location():
    # M=0, location 9 (a 32-bit offset), offset 4; fix data: frame method 5, target method 2 (external), P=1; external 1
    record = bytes([0x80 | (9 << 2), 0x04, (5 << 4) | 4 | 2, 0x01])
    fixes = standin.fixups(record, {})
    assert fixes[0]["loc"] == 9
    assert standin.fixrec([dict(fixes[0], at=4)], 0)[3] == 0x80 | (9 << 2)
