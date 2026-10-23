"""ppc_asm.py -- an Xbox 360 PowerPC assembler: mnemonic -> big-endian 4-byte word.

For patching and hooking a title's code (formats/xex_edit.py): branches, the common D / X / XL / XFX
forms and the simplified mnemonics (nop, li, lis, mr, mtlr, blr, ...). Not a whole assembler -- an
instruction outside that set stops with AsmError. tests/test_xex_edit.py checks every encoding against
Capstone's PowerPC decoder.

PowerPC fields (bits numbered from the LSB): opcode<<26, RT/RS<<21, RA<<16, RB<<11, XO<<1, Rc<<0.
Big-endian in memory.
"""

import re
import struct

MASK32 = 0xFFFFFFFF


class AsmError(Exception):
    pass


def _reg(tok):
    t = tok.strip().lower()
    if t.startswith("r"):
        t = t[1:]
    if not t.isdigit() or not (0 <= int(t) <= 31):
        raise AsmError("bad register: %s" % tok)
    return int(t)


def _imm(tok, bits=16, signed=True):
    t = tok.strip()
    try:
        v = int(t, 0)
    except ValueError:
        raise AsmError("bad immediate: %s" % tok)
    if signed:
        lo, hi = -(1 << (bits - 1)), (1 << (bits - 1)) - 1
        if not (lo <= v <= hi) and not (0 <= v <= (1 << bits) - 1):
            raise AsmError("immediate out of range: %s" % tok)
    return v & ((1 << bits) - 1)


def _mem(tok):
    """Parse `D(rA)` -> (disp, rA)."""
    m = re.match(r"\s*(-?(?:0x)?[0-9a-fA-F]+)\s*\(\s*r?(\d+)\s*\)\s*$", tok)
    if not m:
        raise AsmError("bad memory operand: %s" % tok)
    return int(m.group(1), 0) & 0xFFFF, int(m.group(2))


def _dform(op, rt, ra, imm):
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | (imm & 0xFFFF)


def _xform(op, rt, ra, rb, xo, rc=0):
    return (((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) |
            ((rb & 0x1F) << 11) | ((xo & 0x3FF) << 1) | (rc & 1))


def _spr_enc(spr):
    return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F)


_SPR = {"xer": 1, "lr": 8, "ctr": 9}

# X-form 3-register ops: mnemonic -> (extended-opcode, has-Rc-dot-variant)
_XFORM3 = {
    "add": 266, "add.": 266, "subf": 40, "subf.": 40, "sub": 40,
    "and": 28, "and.": 28, "or": 444, "or.": 444, "xor": 316, "xor.": 316,
    "nand": 476, "nor": 124, "slw": 24, "srw": 536, "sraw": 792,
    "mullw": 235, "mullw.": 235, "divw": 491, "divwu": 459, "andc": 60,
}
# D-form ALU/logical: mnemonic -> (opcode, signed-imm)
_DFORM = {
    "addi": (14, True), "addis": (15, True), "subi": (14, True),
    "ori": (24, False), "oris": (25, False), "xori": (26, False),
    "xoris": (27, False), "andi.": (28, False), "andis.": (29, False),
    "mulli": (7, True), "cmpwi": (11, True), "cmplwi": (10, False),
}
# D-form load/store: mnemonic -> opcode
_MEMOP = {
    "lwz": 32, "lwzu": 33, "lbz": 34, "lbzu": 35, "lhz": 40, "lhzu": 41,
    "lha": 42, "stw": 36, "stwu": 37, "stb": 38, "stbu": 39, "sth": 44,
    "sthu": 45,
}
# conditional-branch simplified mnemonics -> (BO, BI)
_BCOND = {
    "beq": (12, 2), "bne": (4, 2), "blt": (12, 0), "bge": (4, 0),
    "bgt": (12, 1), "ble": (4, 1), "bdnz": (16, 0), "bdz": (18, 0),
    "bso": (12, 3), "bns": (4, 3),
}


def assemble_one(line, addr=0, labels=None):
    """Assemble one instruction at virtual address `addr` -> u32 (or None for a
    blank/comment line). `labels` maps name->address for branch targets."""
    line = line.split("#", 1)[0].split(";", 1)[0].strip()
    if not line or line.endswith(":"):
        return None
    parts = line.replace(",", " ").split()
    mn = parts[0].lower()
    ops = parts[1:]
    labels = labels or {}

    def tgt(tok):
        if tok in labels:
            return labels[tok]
        return _imm(tok, 32, signed=True) if False else int(tok, 0)

    # ---- simplified / no-operand ------------------------------------------
    if mn == "nop":
        return 0x60000000
    if mn == "blr":
        return 0x4E800020
    if mn == "blrl":
        return 0x4E800021
    if mn == "bctr":
        return 0x4E800420
    if mn == "bctrl":
        return 0x4E800421
    if mn in ("trap", "tw"):
        return 0x7FE00008                      # tw 31,r0,r0 (unconditional trap)
    if mn == "mflr":
        e = _spr_enc(8)
        return (31 << 26) | (_reg(ops[0]) << 21) | (e << 11) | (339 << 1)
    if mn == "mtlr":
        rs = _reg(ops[0]); e = _spr_enc(8)
        return (31 << 26) | (rs << 21) | (e << 11) | (467 << 1)
    if mn == "mfctr":
        e = _spr_enc(9)
        return (31 << 26) | (_reg(ops[0]) << 21) | (e << 11) | (339 << 1)
    if mn == "mtctr":
        e = _spr_enc(9)
        return (31 << 26) | (_reg(ops[0]) << 21) | (e << 11) | (467 << 1)
    if mn in ("mfspr", "mtspr"):
        # mtspr spr, rS  /  mfspr rD, spr
        if mn == "mtspr":
            spr = _SPR.get(ops[0].lower(), None)
            spr = spr if spr is not None else _imm(ops[0], 10, False)
            rs = _reg(ops[1])
            return (31 << 26) | (rs << 21) | (_spr_enc(spr) << 11) | (467 << 1)
        else:
            rd = _reg(ops[0])
            spr = _SPR.get(ops[1].lower(), None)
            spr = spr if spr is not None else _imm(ops[1], 10, False)
            return (31 << 26) | (rd << 21) | (_spr_enc(spr) << 11) | (339 << 1)
    if mn == "li":
        return _dform(14, _reg(ops[0]), 0, _imm(ops[1]))
    if mn == "lis":
        return _dform(15, _reg(ops[0]), 0, _imm(ops[1]))
    if mn == "mr" or mn == "mr.":
        rs = _reg(ops[1])
        return _xform(31, rs, _reg(ops[0]), rs, 444, 1 if mn.endswith(".") else 0)
    if mn == "not":
        rs = _reg(ops[1])
        return _xform(31, rs, _reg(ops[0]), rs, 124)    # nor rA,rS,rS

    # ---- branches ---------------------------------------------------------
    if mn in ("b", "ba", "bl", "bla"):
        aa = 1 if "a" in mn[1:] else 0
        lk = 1 if mn.endswith("l") else 0
        dest = tgt(ops[0])
        li = dest if aa else (dest - addr)
        if li & 3:
            raise AsmError("branch target not 4-aligned")
        return (18 << 26) | (li & 0x03FFFFFC) | (aa << 1) | lk
    if mn in _BCOND or mn in ("bc", "bcl"):
        if mn in _BCOND:
            bo, bi = _BCOND[mn]
            dest = tgt(ops[0]); lk = 0
        else:
            bo = _imm(ops[0], 5, False); bi = _imm(ops[1], 5, False)
            dest = tgt(ops[2]); lk = 1 if mn.endswith("l") else 0
        bd = dest - addr
        if bd & 3:
            raise AsmError("branch target not 4-aligned")
        return (16 << 26) | (bo << 21) | (bi << 16) | (bd & 0xFFFC) | lk

    # ---- D-form -----------------------------------------------------------
    if mn in _DFORM:
        op, signed = _DFORM[mn]
        if mn == "subi":
            return _dform(14, _reg(ops[0]), _reg(ops[1]), (-_imm(ops[2], 16, False)) & 0xFFFF)
        if mn in ("cmpwi", "cmplwi"):
            return _dform(op, 0, _reg(ops[0]), _imm(ops[1], 16, signed))
        return _dform(op, _reg(ops[0]), _reg(ops[1]),
                      _imm(ops[2], 16, signed) if len(ops) > 2 else 0)
    if mn in _MEMOP:
        disp, ra = _mem(ops[1])
        return _dform(_MEMOP[mn], _reg(ops[0]), ra, disp)

    # ---- X-form 3-register ------------------------------------------------
    if mn in _XFORM3:
        xo = _XFORM3[mn]
        rc = 1 if mn.endswith(".") else 0
        if mn in ("sub", "subf", "subf."):
            # subf rD,rA,rB = rB - rA  (natural `sub rD,rB,rA` handled as subf)
            return _xform(31, _reg(ops[0]), _reg(ops[1]), _reg(ops[2]), xo, rc)
        return _xform(31, _reg(ops[0]), _reg(ops[1]), _reg(ops[2]), xo, rc)
    if mn in ("cmpw", "cmplw"):
        xo = 0 if mn == "cmpw" else 32
        return _xform(31, 0, _reg(ops[0]), _reg(ops[1]), xo)
    if mn in ("neg", "neg."):
        return _xform(31, _reg(ops[0]), _reg(ops[1]), 0, 104, 1 if mn.endswith(".") else 0)
    if mn in ("extsb", "extsh", "extsb.", "extsh."):
        xo = 954 if "b" in mn else 922
        return _xform(31, _reg(ops[1]), _reg(ops[0]), 0, xo, 1 if mn.endswith(".") else 0)

    raise AsmError("unsupported mnemonic: %s" % mn)


def assemble(text, base_addr=0):
    """Assemble multi-line asm -> big-endian bytes. Two passes for labels."""
    lines = text.splitlines()
    labels, addr = {}, base_addr
    for ln in lines:                                   # pass 1: labels
        s = ln.split("#", 1)[0].split(";", 1)[0].strip()
        if not s:
            continue
        if s.endswith(":"):
            labels[s[:-1].strip()] = addr
            continue
        addr += 4
    out, addr = bytearray(), base_addr
    for ln in lines:                                   # pass 2: encode
        w = assemble_one(ln, addr, labels)
        if w is None:
            continue
        out += struct.pack(">I", w & MASK32)
        addr += 4
    return bytes(out)

