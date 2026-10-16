"""x86.py -- 16-bit code disassembled, as the period's compilers wrote it.

Microsoft's languages (C 3.00-5.x, Pascal, FORTRAN) compiled floating point for the 8087 emulator
(-FPi): each ESC instruction an interrupt the emulator takes - INT 34h-3Bh stand for the ESC opcodes
D8h-DFh (the next byte is the instruction's modrm), INT 3Ch for an ESC with a segment override (the
next byte the segment and the opcode), INT 3Dh for FWAIT. A plain disassembler reads the bytes after
the interrupt as instructions of their own and loses its way; here they are read as what they
stand for.

    for i in x86.disasm(img, at, end, emulator=True): i.address i.size i.bytes i.mnemonic i.op_str
"""
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

_CS = Cs(CS_ARCH_X86, CS_MODE_16)


class Insn(object):
    __slots__ = ("address", "size", "bytes", "mnemonic", "op_str")

    def __init__(self, a, b, m, o):
        self.address, self.bytes, self.mnemonic, self.op_str = a, bytes(b), m, o
        self.size = len(b)


def disasm(img, at, end, emulator=True):
    a = at
    while a < end:
        b = img[a:a + 2]
        if emulator and b[:1] == b"\xcd" and len(b) == 2:
            if 0x34 <= b[1] <= 0x3B:
                x = next(_CS.disasm(bytes([0xD8 + b[1] - 0x34]) + bytes(img[a + 2:a + 16]), a + 1), None)
                if x:
                    m = x.mnemonic if x.mnemonic.startswith("f") else "f" + x.mnemonic
                    yield Insn(a, img[a:a + 1 + x.size], m, x.op_str)
                    a += 1 + x.size
                    continue
            elif b[1] == 0x3D:
                yield Insn(a, b, "fwait", "")
                a += 2
                continue
            elif b[1] == 0x3C and a + 2 < len(img):
                sg = {0x00: "es", 0x40: "cs", 0x80: "ss", 0xC0: "ds"}[img[a + 2] & 0xC0]
                x = next(_CS.disasm(bytes([0xD8 | (img[a + 2] & 7)]) + bytes(img[a + 3:a + 16]), a + 2), None)
                if x:
                    yield Insn(a, img[a:a + 2 + x.size], x.mnemonic, sg + ": " + x.op_str)
                    a += 2 + x.size
                    continue
        x = next(_CS.disasm(bytes(img[a:min(a + 16, end)]), a), None)
        if x is None:
            yield Insn(a, img[a:a + 1], "db", "0x%02x" % img[a])
            a += 1
            continue
        yield Insn(x.address, x.bytes, x.mnemonic, x.op_str)
        a += x.size


def tables(insns, img, lo, hi, base=0):
    """{table address: its length} - the jump tables of a switch kept in the code (jmp cs:[bx+T]): the words
    at T, as long as each is a place in [lo, hi)"""
    import re
    out = {}
    for i in insns:
        if i.mnemonic == "jmp" and "cs:[" in i.op_str:
            m = re.search(r"\+ 0x([0-9a-f]+)\]", i.op_str)
            if not m:
                continue
            t = base + int(m.group(1), 16)
            k = t
            while lo <= k < hi and k + 2 <= hi and lo <= base + (img[k] | img[k + 1] << 8) < hi:
                k += 2
            if k > t:
                out[t] = k - t
    return out


def body(img, lo, hi, emulator=True, base=0):
    """the instructions of a function's code [lo, hi), its switch tables stepped over (they are data)"""
    first = list(disasm(img, lo, hi, emulator))
    tb = tables(first, img, lo, hi, base)
    if not tb:
        return first
    out = []
    a = lo
    for t in sorted(tb) + [hi]:
        if t > a:
            out += list(disasm(img, a, t, emulator))
        a = max(a, t + tb.get(t, 0))
    return out
