"""m68k.py -- the Motorola 68000's instructions: a disassembler, an assembler.

    from codebreaker.decompile import m68k
    m68k.decode(code, at, pc)       Insn(mnemonic, size, operands, length, target, flow) or None
    m68k.text(insn, labels)         "MOVE.L D0,(A1)+" / "BNE.S loop" / "LEA $10(A0),A1"
    m68k.assemble(line, pc, labels) the bytes of one instruction line

Every instruction of the 68000 (not the 68010's and later additions), with the addressing modes
each one allows -- an opcode word whose fields name a mode the instruction does not take is not
an instruction, so data is not read as code. Where the CPU ignores bits (the brief extension
word's 8-10, an immediate byte's high byte), only the encoding an assembler writes is taken, so
that what is written as an instruction assembles back to the same bytes.

Syntax is Motorola's, as vasm, asm68k and SNASM68K take it: `MOVE.W #$1234,(A0)+`,
`LEA $10(A0),A1`, `MOVE.B 2(A0,D1.W),D2`, `JSR (sub).L`, `($FF8000).W`, `label(PC)`,
`MOVEM.L D0-D7/A0-A6,-(SP)` (A7 is written SP), `BRA.S` / `BRA.W` for the two branch sizes.

The assembler is the disassembler run backwards: every one of the 65,536 opcode words is
decoded once into its shape (the text with the values that live in extension words left out),
and a line is assembled by finding the opcode word with its shape, then writing its extension
words.
"""
import re

SIZES = {0: "B", 1: "W", 2: "L"}
CC = ["T", "F", "HI", "LS", "CC", "CS", "NE", "EQ", "VC", "VS", "PL", "MI", "GE", "LT", "GT", "LE"]
BCC = ["RA", "SR"] + CC[2:]     # line 6: 0 BRA, 1 BSR, then the conditions

# addressing-mode categories (68000 programmer's manual, table 2-4)
DATA, MEMORY, CONTROL, ALTERABLE = 1, 2, 4, 8


def _category(mode, reg):
    """The categories (mode, reg) belongs to, or None if it is not a mode."""
    if mode == 0:
        return DATA | ALTERABLE
    if mode == 1:
        return ALTERABLE
    if mode in (2, 5, 6):
        return DATA | MEMORY | CONTROL | ALTERABLE
    if mode in (3, 4):
        return DATA | MEMORY | ALTERABLE
    if reg in (0, 1):
        return DATA | MEMORY | CONTROL | ALTERABLE
    if reg in (2, 3):
        return DATA | MEMORY | CONTROL
    if reg == 4:
        return DATA | MEMORY
    return None


def _ok(mode, reg, need, no_an=False, no_imm=False):
    c = _category(mode, reg)
    if c is None or (c & need) != need:
        return False
    if no_an and mode == 1:
        return False
    if no_imm and mode == 7 and reg == 4:
        return False
    return True


class Op(object):
    """One operand. kind: D A ind post pre disp index absw absl pcdisp pcindex imm quick label regs ccr sr usp."""
    __slots__ = ("kind", "reg", "value", "xreg", "xsize", "size")

    def __init__(self, kind, reg=0, value=0, xreg=None, xsize="W", size=None):
        self.kind, self.reg, self.value, self.xreg, self.xsize, self.size = kind, reg, value, xreg, xsize, size

    def key(self):
        """The operand's part of a shape: registers and in-opcode values, not extension words."""
        k = self.kind
        if k in ("D", "A", "ind", "post", "pre", "disp"):
            return "%s%d" % (k, self.reg)
        if k == "index":                      # (the index register and its size are the extension word's)
            return "index%d" % self.reg
        if k == "pcindex":
            return "pcindex"
        if k == "quick":
            return "#%d" % self.value
        if k == "label8":
            return "d8:%d" % self.value
        return k

    def __eq__(self, other):
        return self.key() == other.key()


class Insn(object):
    __slots__ = ("mnemonic", "size", "operands", "length", "target", "flow", "pc", "bytes", "word")

    def __init__(self, mnemonic, size, operands, length, pc, raw, word):
        self.mnemonic, self.size, self.operands, self.length = mnemonic, size, operands, length
        self.pc, self.bytes, self.word = pc, raw, word
        self.target, self.flow = None, ""

    def shape(self):
        return "%s%s %s" % (self.mnemonic, "." + self.size if self.size else "",
                            ",".join(o.key() for o in self.operands))

    def __repr__(self):
        return "Insn(%s)" % text(self)


class _Reader(object):
    def __init__(self, code, at):
        self.code, self.at = code, at

    def word(self):
        if self.at + 2 > len(self.code):
            raise IndexError
        v = self.code[self.at] << 8 | self.code[self.at + 1]
        self.at += 2
        return v

    def long(self):
        return self.word() << 16 | self.word()


def _s16(v):
    return v - 0x10000 if v & 0x8000 else v


def _s8(v):
    return v - 0x100 if v & 0x80 else v


def _ea(r, mode, reg, size, pc_of_ext):
    """The operand of effective address (mode, reg), reading its extension words; None if invalid."""
    if mode == 0:
        return Op("D", reg)
    if mode == 1:
        return Op("A", reg)
    if mode == 2:
        return Op("ind", reg)
    if mode == 3:
        return Op("post", reg)
    if mode == 4:
        return Op("pre", reg)
    if mode == 5:
        return Op("disp", reg, _s16(r.word()))
    if mode == 6 or (mode == 7 and reg == 3):
        ext_at = r.at
        ext = r.word()
        if ext & 0x0700:                     # the 68000 has only the brief format, bits 8-10 zero
            return None
        xreg = ("A" if ext & 0x8000 else "D", (ext >> 12) & 7)
        xsize = "L" if ext & 0x0800 else "W"
        if mode == 6:
            return Op("index", reg, _s8(ext & 0xFF), xreg, xsize)
        return Op("pcindex", 0, (pc_of_ext(ext_at) + _s8(ext & 0xFF)) & 0xFFFFFFFF, xreg, xsize)
    if reg == 0:
        return Op("absw", 0, _s16(r.word()) & 0xFFFFFFFF)
    if reg == 1:
        return Op("absl", 0, r.long())
    if reg == 2:
        ext_at = r.at
        return Op("pcdisp", 0, (pc_of_ext(ext_at) + _s16(r.word())) & 0xFFFFFFFF)
    if reg == 4:
        if size == "B":
            v = r.word()
            if v & 0xFF00:
                return None
            return Op("imm", 0, v, size="B")
        if size == "W":
            return Op("imm", 0, r.word(), size="W")
        return Op("imm", 0, r.long(), size="L")
    return None


def decode(code, at, pc):
    """The instruction at code[at:] (running at address `pc`), or None if it is not one (or code
    ends inside it)."""
    try:
        return _decode(code, at, pc)
    except IndexError:
        return None


def _decode(code, at, pc):
    r = _Reader(code, at)
    w = r.word()
    line = w >> 12
    mode, reg = (w >> 3) & 7, w & 7
    hi = (w >> 9) & 7
    ext_pc = lambda ext_at: pc + (ext_at - at)          # the PC a PC-relative extension word is relative to
    ops = []

    def ea(m, rg, size, need, **kw):
        if not _ok(m, rg, need, **kw):
            raise _Not
        o = _ea(r, m, rg, size, ext_pc)
        if o is None:
            raise _Not
        return o

    def done(mn, size, operands, flow="", target=None):
        insn = Insn(mn, size, operands, r.at - at, pc, bytes(code[at:r.at]), w)
        insn.flow, insn.target = flow, target
        return insn

    try:
        if line == 0:
            if w in (0x003C, 0x007C, 0x023C, 0x027C, 0x0A3C, 0x0A7C):
                mn = {0x00: "ORI", 0x02: "ANDI", 0x0A: "EORI"}[w >> 8]
                to_sr = w & 0x40
                v = r.word()
                if not to_sr and v & 0xFF00:
                    raise _Not
                return done(mn, "W" if to_sr else "B", [Op("imm", 0, v, size="W" if to_sr else "B"),
                                                       Op("sr" if to_sr else "ccr")])
            if w & 0x0100:
                if mode == 1:                          # MOVEP
                    opm = (w >> 6) & 3
                    size = "L" if opm & 1 else "W"
                    d = Op("disp", reg, _s16(r.word()))
                    pair = [d, Op("D", hi)] if opm < 2 else [Op("D", hi), d]
                    return done("MOVEP", size, pair)
                t = (w >> 6) & 3
                mn = ["BTST", "BCHG", "BCLR", "BSET"][t]
                need = DATA if t == 0 else DATA | ALTERABLE
                dst = ea(mode, reg, "B", need)
                return done(mn, "L" if mode == 0 else "B", [Op("D", hi), dst])
            top = (w >> 9) & 7
            if top == 4:                               # static bit operations
                t = (w >> 6) & 3
                mn = ["BTST", "BCHG", "BCLR", "BSET"][t]
                v = r.word()
                if v & 0xFF00:
                    raise _Not
                need = DATA if t == 0 else DATA | ALTERABLE
                dst = ea(mode, reg, "B", need, no_imm=True)
                return done(mn, "L" if mode == 0 else "B", [Op("imm", 0, v, size="B"), dst])
            names = {0: "ORI", 1: "ANDI", 2: "SUBI", 3: "ADDI", 5: "EORI", 6: "CMPI"}
            if top in names and (w >> 6) & 3 != 3:
                size = SIZES[(w >> 6) & 3]
                imm = _ea(r, 7, 4, size, ext_pc)
                if imm is None:
                    raise _Not
                dst = ea(mode, reg, size, DATA | ALTERABLE)
                return done(names[top], size, [imm, dst])
            raise _Not
        if line in (1, 2, 3):
            size = {1: "B", 3: "W", 2: "L"}[line]
            src = ea(mode, reg, size, 0, no_an=size == "B")
            dmode, dreg = (w >> 6) & 7, hi
            if dmode == 1:
                if size == "B":
                    raise _Not
                return done("MOVEA", size, [src, Op("A", dreg)])
            dst = ea(dmode, dreg, size, DATA | ALTERABLE)
            return done("MOVE", size, [src, dst])
        if line == 4:
            return _line4(w, r, mode, reg, hi, ea, done, ext_pc)
        if line == 5:
            ss = (w >> 6) & 3
            if ss == 3:
                cond = CC[(w >> 8) & 0xF]
                if mode == 1:
                    disp_at = r.at
                    t = (ext_pc(disp_at) + _s16(r.word())) & 0xFFFFFFFF
                    return done("DB" + cond, "", [Op("D", reg), Op("label", 0, t)], "branch", t)
                dst = ea(mode, reg, "B", DATA | ALTERABLE)
                return done("S" + cond, "", [dst])
            q = hi or 8
            size = SIZES[ss]
            if mode == 1 and size == "B":
                raise _Not
            dst = ea(mode, reg, size, ALTERABLE)
            return done("SUBQ" if w & 0x0100 else "ADDQ", size, [Op("quick", 0, q), dst])
        if line == 6:
            c = (w >> 8) & 0xF
            d8 = w & 0xFF
            mn = "B" + BCC[c]
            if d8 == 0:
                disp_at = r.at
                t = (ext_pc(disp_at) + _s16(r.word())) & 0xFFFFFFFF
                op = Op("label", 0, t)
                size = "W"
            else:
                t = (pc + 2 + _s8(d8)) & 0xFFFFFFFF
                op = Op("label8", 0, d8)
                op.size = t
                size = "S"
            flow = "jump" if c == 0 else "call" if c == 1 else "branch"
            return done(mn, size, [op], flow, t)
        if line == 7:
            if w & 0x0100:
                raise _Not
            return done("MOVEQ", "", [Op("quick", 0, _s8(w & 0xFF)), Op("D", hi)])
        if line in (8, 9, 0xB, 0xC, 0xD):
            return _arith(w, line, r, mode, reg, hi, ea, done)
        if line == 0xE:
            ss = (w >> 6) & 3
            names = ["AS", "LS", "ROX", "RO"]
            if ss == 3:
                if hi & 4:
                    raise _Not
                dst = ea(mode, reg, "W", MEMORY | ALTERABLE)
                return done(names[hi & 3] + ("L" if w & 0x0100 else "R"), "W", [dst])
            t = (w >> 3) & 3
            count = Op("D", hi) if w & 0x20 else Op("quick", 0, hi or 8)
            return done(names[t] + ("L" if w & 0x0100 else "R"), SIZES[ss], [count, Op("D", reg)])
        return None                                    # line A and line F: no 68000 instruction
    except _Not:
        return None


class _Not(Exception):
    pass


def _line4(w, r, mode, reg, hi, ea, done, ext_pc):
    if w == 0x4AFC:
        return done("ILLEGAL", "", [])
    if (w & 0xFFF0) == 0x4E40:
        return done("TRAP", "", [Op("quick", 0, w & 0xF)], "call")
    if (w & 0xFFF8) == 0x4E50:
        return done("LINK", "", [Op("A", reg), Op("imm", 0, r.word(), size="W")])
    if (w & 0xFFF8) == 0x4E58:
        return done("UNLK", "", [Op("A", reg)])
    if (w & 0xFFF8) == 0x4E60:
        return done("MOVE", "L", [Op("A", reg), Op("usp")])
    if (w & 0xFFF8) == 0x4E68:
        return done("MOVE", "L", [Op("usp"), Op("A", reg)])
    simple = {0x4E70: "RESET", 0x4E71: "NOP", 0x4E73: "RTE", 0x4E75: "RTS", 0x4E76: "TRAPV", 0x4E77: "RTR"}
    if w in simple:
        return done(simple[w], "", [], "ret" if w in (0x4E73, 0x4E75, 0x4E77) else "")
    if w == 0x4E72:
        return done("STOP", "", [Op("imm", 0, r.word(), size="W")], "halt")
    if (w & 0xFFC0) == 0x4E80:
        dst = ea(mode, reg, "L", CONTROL)
        return done("JSR", "", [dst], "call", _absolute(dst))
    if (w & 0xFFC0) == 0x4EC0:
        dst = ea(mode, reg, "L", CONTROL)
        t = _absolute(dst)
        return done("JMP", "", [dst], "jump" if t is not None else "indirect", t)
    if (w & 0xF1C0) == 0x41C0:
        return done("LEA", "", [ea(mode, reg, "L", CONTROL), Op("A", hi)])
    if (w & 0xF1C0) == 0x4180:
        return done("CHK", "W", [ea(mode, reg, "W", DATA), Op("D", hi)])
    if (w & 0xFFF8) == 0x4840:
        return done("SWAP", "", [Op("D", reg)])
    if (w & 0xFFF8) == 0x4880:
        return done("EXT", "W", [Op("D", reg)])
    if (w & 0xFFF8) == 0x48C0:
        return done("EXT", "L", [Op("D", reg)])
    if (w & 0xFFC0) == 0x4840:
        return done("PEA", "", [ea(mode, reg, "L", CONTROL)])
    if (w & 0xFB80) == 0x4880:                         # MOVEM
        size = "L" if w & 0x40 else "W"
        mask = r.word()
        if w & 0x0400:                                  # memory to registers
            if not (_ok(mode, reg, CONTROL) or mode == 3):
                raise _Not
            src = ea(mode, reg, size, 0) if mode == 3 else ea(mode, reg, size, CONTROL)
            return done("MOVEM", size, [src, Op("regs", 0, mask)])
        if not (_ok(mode, reg, CONTROL | ALTERABLE) or mode == 4):
            raise _Not
        dst = ea(mode, reg, size, 0) if mode == 4 else ea(mode, reg, size, CONTROL | ALTERABLE)
        return done("MOVEM", size, [Op("regs", 0, mask, xsize="pre" if mode == 4 else ""), dst])
    if (w & 0xFFC0) == 0x4800:
        return done("NBCD", "", [ea(mode, reg, "B", DATA | ALTERABLE)])
    if (w & 0xFFC0) == 0x40C0:
        return done("MOVE", "W", [Op("sr"), ea(mode, reg, "W", DATA | ALTERABLE)])
    if (w & 0xFFC0) == 0x44C0:
        return done("MOVE", "W", [ea(mode, reg, "W", DATA), Op("ccr")])
    if (w & 0xFFC0) == 0x46C0:
        return done("MOVE", "W", [ea(mode, reg, "W", DATA), Op("sr")])
    if (w & 0xFFC0) == 0x4AC0:
        return done("TAS", "", [ea(mode, reg, "B", DATA | ALTERABLE)])
    ss = (w >> 6) & 3
    if ss != 3:
        names = {0x40: "NEGX", 0x42: "CLR", 0x44: "NEG", 0x46: "NOT", 0x4A: "TST"}
        mn = names.get(w >> 8)
        if mn:
            size = SIZES[ss]
            return done(mn, size, [ea(mode, reg, size, DATA | ALTERABLE)])
    raise _Not


def _absolute(op):
    if op.kind in ("absw", "absl", "pcdisp"):
        return op.value
    return None


def _arith(w, line, r, mode, reg, hi, ea, done):
    opm = (w >> 6) & 7
    ss = opm & 3
    if line in (8, 0xC) and ss == 3:                   # DIVU DIVS / MULU MULS
        mn = ("DIV" if line == 8 else "MUL") + ("S" if w & 0x0100 else "U")
        return done(mn, "W", [ea(mode, reg, "W", DATA), Op("D", hi)])
    if line in (9, 0xB, 0xD) and ss == 3:              # SUBA CMPA ADDA
        size = "L" if w & 0x0100 else "W"
        mn = {9: "SUBA", 0xB: "CMPA", 0xD: "ADDA"}[line]
        return done(mn, size, [ea(mode, reg, size, 0), Op("A", hi)])
    size = SIZES[ss]
    to_ea = bool(w & 0x0100)
    if to_ea and mode in (0, 1):
        if line in (8, 0xC) and ss == 0:               # SBCD ABCD
            mn = "SBCD" if line == 8 else "ABCD"
            pair = [Op("pre", reg), Op("pre", hi)] if mode else [Op("D", reg), Op("D", hi)]
            return done(mn, "", pair)
        if line == 0xC:                                # EXG
            kinds = {(1, 0): ("D", "D"), (1, 1): ("A", "A"), (2, 1): ("D", "A")}.get((ss, mode))
            if not kinds:
                raise _Not
            return done("EXG", "", [Op(kinds[0], hi), Op(kinds[1], reg)])
        if line in (9, 0xD):                           # SUBX ADDX
            mn = "SUBX" if line == 9 else "ADDX"
            pair = [Op("pre", reg), Op("pre", hi)] if mode else [Op("D", reg), Op("D", hi)]
            return done(mn, size, pair)
        if line == 0xB and mode == 1:                  # CMPM
            return done("CMPM", size, [Op("post", reg), Op("post", hi)])
        if line == 0xB:                                # EOR Dn,Dn
            return done("EOR", size, [Op("D", hi), Op("D", reg)])
        raise _Not
    mn = {8: "OR", 9: "SUB", 0xB: "CMP" if not to_ea else "EOR", 0xC: "AND", 0xD: "ADD"}[line]
    if to_ea:
        need = DATA | ALTERABLE if mn == "EOR" else MEMORY | ALTERABLE
        return done(mn, size, [Op("D", hi), ea(mode, reg, size, need)])
    no_an = mn in ("OR", "AND") or size == "B"
    return done(mn, size, [ea(mode, reg, size, 0, no_an=no_an), Op("D", hi)])


# ---- text ------------------------------------------------------------------------------------------

def _reg(kind, n):
    return "SP" if kind == "A" and n == 7 else "%s%d" % (kind, n)


def _hex(v, digits=0):
    if v < 0:
        return "-$%X" % -v
    return "$%0*X" % (digits, v)


def _regs(mask, predec):
    """A MOVEM register list: D0-D7/A0-A6 (the mask is reversed for -(An))."""
    if predec:
        mask = int("{:016b}".format(mask)[::-1], 2)
    names = ["D%d" % i for i in range(8)] + ["A%d" % i for i in range(8)]
    parts, i = [], 0
    while i < 16:
        if mask & (1 << i):
            j = i
            while j + 1 < 16 and mask & (1 << (j + 1)) and (j + 1) // 8 == i // 8:
                j += 1
            parts.append(names[i] if i == j else "%s-%s" % (names[i], names[j]))
            i = j + 1
        else:
            i += 1
    return "/".join(p.replace("A7", "SP") for p in parts) or "0"


def operand_text(o, labels):
    k = o.kind
    if k in ("D", "A"):
        return _reg(k, o.reg)
    if k == "ind":
        return "(%s)" % _reg("A", o.reg)
    if k == "post":
        return "(%s)+" % _reg("A", o.reg)
    if k == "pre":
        return "-(%s)" % _reg("A", o.reg)
    if k == "disp":
        return "%s(%s)" % (_hex(o.value), _reg("A", o.reg))
    if k == "index":
        return "%s(%s,%s.%s)" % (_hex(o.value), _reg("A", o.reg), _reg(*o.xreg), o.xsize)
    if k == "absw":
        name = labels.get(o.value)
        return "(%s).W" % (name or _hex(o.value & 0xFFFF if o.value < 0x8000 else o.value, 4))
    if k == "absl":
        return "(%s).L" % (labels.get(o.value) or _hex(o.value, 8))
    if k == "pcdisp":
        return "%s(PC)" % (labels.get(o.value) or _hex(o.value))
    if k == "pcindex":
        return "%s(PC,%s.%s)" % (labels.get(o.value) or _hex(o.value), _reg(*o.xreg), o.xsize)
    if k == "imm":
        return "#%s" % ((labels.get(o.value) if o.size == "L" else None) or _hex(o.value))
    if k == "quick":
        return "#%d" % o.value
    if k == "label":
        return labels.get(o.value) or _hex(o.value)
    if k == "label8":
        return labels.get(o.size) or _hex(o.size)
    if k == "regs":
        return _regs(o.value, o.xsize == "pre")
    return {"ccr": "CCR", "sr": "SR", "usp": "USP"}[k]


def text(insn, labels=None):
    labels = labels or {}
    m = insn.mnemonic + ("." + insn.size if insn.size else "")
    if not insn.operands:
        return m
    return "%s %s" % (m, ",".join(operand_text(o, labels) for o in insn.operands))


# ---- the assembler -----------------------------------------------------------------------------------

_SHAPES = None


def _shapes():
    """{shape: opcode word} for every opcode word that decodes (the first with each shape)."""
    global _SHAPES
    if _SHAPES is None:
        _SHAPES = {}
        filler = bytes(8)
        for w in range(0x10000):
            insn = decode(bytes([w >> 8, w & 0xFF]) + filler, 0, 0)
            if insn is not None:
                _SHAPES.setdefault(insn.shape(), w)
    return _SHAPES


def _value(expr, labels, pc):
    from codebreaker.decompile.z80 import value
    return value(expr.replace("*", "$") if expr.strip() == "*" else expr, labels, pc)


def _parse_reg(s):
    s = s.strip().upper()
    if s == "SP":
        return ("A", 7)
    m = re.match(r"^([DA])([0-7])$", s)
    return (m.group(1), int(m.group(2))) if m else None


def _parse_regs(s):
    mask = 0
    for part in s.upper().split("/"):
        a, _, b = part.partition("-")
        ra, rb = _parse_reg(a), _parse_reg(b or a)
        if not ra or not rb:
            return None
        for i in range(ra[1] + (8 if ra[0] == "A" else 0), rb[1] + (8 if rb[0] == "A" else 0) + 1):
            mask |= 1 << i
    return mask


def _parse_operand(s, labels, pc, size):
    """-> (Op, [extension values as (bytes, value)]) for one operand's text."""
    t = s.strip()
    u = t.upper()
    if u in ("CCR", "SR", "USP"):
        return Op(u.lower())
    rr = _parse_reg(t)
    if rr:
        return Op(rr[0], rr[1])
    if t.startswith("#"):
        return Op("imm", 0, _value(t[1:], labels, pc), size=size)
    m = re.match(r"^\((\w+)\)(\+?)$", t)
    if m and _parse_reg(m.group(1)) and _parse_reg(m.group(1))[0] == "A":
        return Op("post" if m.group(2) else "ind", _parse_reg(m.group(1))[1])
    m = re.match(r"^-\((\w+)\)$", t)
    if m and _parse_reg(m.group(1)) and _parse_reg(m.group(1))[0] == "A":
        return Op("pre", _parse_reg(m.group(1))[1])
    m = re.match(r"^(.*)\((\w+)\s*,\s*(\w+)(?:\.([WL]))?\)$", t, re.I)
    if m:
        base = m.group(2).upper()
        x = _parse_reg(m.group(3))
        if x is None or (base != "PC" and (_parse_reg(base) is None or _parse_reg(base)[0] != "A")):
            raise ValueError("not an index operand: %r" % t)
        disp = _value(m.group(1), labels, pc) if m.group(1).strip() else 0
        xs = (m.group(4) or "W").upper()
        if base == "PC":
            return Op("pcindex", 0, disp, x, xs)
        return Op("index", _parse_reg(base)[1], disp, x, xs)
    m = re.match(r"^(.*)\((\w+)\)$", t)
    if m and m.group(1).strip() and (m.group(2).upper() == "PC" or (_parse_reg(m.group(2)) and _parse_reg(m.group(2))[0] == "A")):
        v = _value(m.group(1), labels, pc)
        if m.group(2).upper() == "PC":
            return Op("pcdisp", 0, v)
        return Op("disp", _parse_reg(m.group(2))[1], v)
    m = re.match(r"^\((.+)\)\.([WL])$", t, re.I) or re.match(r"^(.+)\.([WL])$", t, re.I)
    if m:
        v = _value(m.group(1), labels, pc)
        return Op("absw" if m.group(2).upper() == "W" else "absl", 0, v)
    if "/" in t or re.match(r"^([DA][0-7]|SP)-([DA][0-7]|SP)$", u):
        return Op("regs", 0, _parse_regs(t))
    return Op("expr", 0, _value(t, labels, pc))


def _split(s):
    out, depth, cur = [], 0, ""
    for c in s:
        depth += c == "("
        depth -= c == ")"
        if c == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur)
    return out


def assemble(line, pc=0, labels=None):
    """The bytes of one instruction (Motorola syntax); KeyError for an unknown label."""
    labels = {} if labels is None else labels
    line = line.strip()
    parts = line.split(None, 1)
    head, rest = (parts[0] if parts else ""), (parts[1] if len(parts) > 1 else "")
    mn, _, size = head.upper().partition(".")
    if size not in ("", "B", "W", "L", "S"):
        raise ValueError("not an instruction: %r" % line)
    ops = [_parse_operand(o, labels, pc, size or "W") for o in _split(rest)] if rest.strip() else []
    shapes = _shapes()
    for cand in _candidates(mn, size, ops, pc):
        key = "%s%s %s" % (cand[0], "." + cand[1] if cand[1] else "", ",".join(o.key() for o in cand[2]))
        if key in shapes:
            return _encode(shapes[key], cand[0], cand[1], cand[2], pc)
    raise ValueError("not an instruction: %r" % line)


def _candidates(mn, size, ops, pc):
    """The ways a parsed line can be read: branches by their size, values as absolute words or longs,
    quick values, and so on."""
    branch = mn in ["B" + c for c in BCC]
    if branch:
        if not ops or ops[0].kind != "expr":
            raise ValueError("a branch needs a target address")
        if size == "L":
            raise ValueError("a branch is .S or .W")
        target = ops[0].value
        d = _wrap(target - (pc + 2))
        if size in ("S", "B"):                       # a short branch as written: never made long
            if not -128 <= d <= 127 or d == 0:
                raise ValueError("short branch out of range")
            yield (mn, "S", [Op("label8", 0, d & 0xFF, size=target)])
            return
        if size == "" and -128 <= d <= 127 and d != 0:
            yield (mn, "S", [Op("label8", 0, d & 0xFF, size=target)])
            return
        yield (mn, "W", [Op("label", 0, target)])
        return
    if mn.startswith("DB"):
        if len(ops) < 2 or ops[1].kind != "expr":
            raise ValueError("%s needs a register and a target address" % mn)
        yield (mn, "", [ops[0], Op("label", 0, ops[1].value)])
        return
    if mn in ("JMP", "JSR"):
        o = ops[0]
        if o.kind == "expr":
            yield (mn, "", [Op("absl", 0, o.value)])
            return
    if mn in ("MOVEM",):
        ops = [Op("regs", 0, o.value if o.kind == "regs" else 1 << (o.reg + (8 if o.kind == "A" else 0)),
                  xsize="pre" if ops[-1].kind == "pre" and i == 0 else "")
               if o.kind in ("regs", "D", "A") and (i == 0 if ops[-1].kind not in ("D", "A", "regs") else i == 1)
               else o for i, o in enumerate(ops)]
        if ops[0].kind == "regs" and ops[1].kind == "pre":
            ops[0].value = int("{:016b}".format(ops[0].value)[::-1], 2)
        yield (mn, size, ops)
        return
    # quick forms take the value in the opcode word
    if mn in ("ADDQ", "SUBQ", "MOVEQ", "TRAP") or (mn[:2] in ("AS", "LS", "RO") and ops and ops[0].kind == "imm"):
        q = [Op("quick", 0, _s8(o.value & 0xFF) if mn == "MOVEQ" else o.value) if o.kind == "imm" and i == 0 else o
             for i, o in enumerate(ops)]
        yield (mn, "" if mn in ("MOVEQ", "TRAP") else size, q)
        return
    # a bare value is an absolute address: word if it fits, else long
    variants = [[]]
    for o in ops:
        if o.kind == "expr":
            v = o.value & 0xFFFFFFFF
            choices = [Op("absl", 0, v)]
            if v <= 0x7FFF or v >= 0xFFFF8000:
                choices.insert(0, Op("absw", 0, v))
            variants = [x + [c] for x in variants for c in choices]
        else:
            variants = [x + [o] for x in variants]
    for v in variants:
        yield (mn, size, v)
        if mn in ("MOVE", "ADD", "SUB", "CMP") and len(v) == 2 and v[1].kind == "A":
            yield (mn + "A", size, v)
        if mn in ("ADD", "SUB", "AND", "OR", "EOR", "CMP") and v and v[0].kind == "imm":
            yield (mn + "I", size, v)
        if mn in ("BTST", "BCHG", "BCLR", "BSET") and len(v) == 2:
            yield (mn, "L" if v[1].kind == "D" else "B", v)
        if mn in ("MOVE", "ORI", "ANDI", "EORI") and len(v) == 2 and v[1].kind == "ccr":
            yield (mn, "B" if mn != "MOVE" else "W", [v[0], v[1]])
        if mn in ("MOVE", "ORI", "ANDI", "EORI") and len(v) == 2 and "sr" in (v[0].kind, v[1].kind):
            yield (mn, "W", v)


def _encode(word, mn, size, ops, pc):
    out = bytearray([word >> 8, word & 0xFF])

    def put16(v):
        out.extend([(v >> 8) & 0xFF, v & 0xFF])

    def put32(v):
        put16((v >> 16) & 0xFFFF)
        put16(v & 0xFFFF)
    if mn == "MOVEM":
        put16(next(o.value for o in ops if o.kind == "regs"))
    for o in ops:
        k = o.kind
        if k == "imm":
            sz = o.size if mn not in ("LINK", "STOP") else "W"
            if mn in ("ORI", "ANDI", "EORI") and ops[-1].kind in ("ccr",):
                sz = "B"
            if mn in ("BTST", "BCHG", "BCLR", "BSET"):
                sz = "B"
            v = o.value
            if sz == "L":
                put32(v & 0xFFFFFFFF)
            else:
                put16(v & (0xFF if sz == "B" else 0xFFFF))
        elif k == "disp":
            if not -0x8000 <= o.value <= 0xFFFF:
                raise ValueError("displacement does not fit 16 bits")
            put16(o.value & 0xFFFF)
        elif k == "index":
            if not -0x80 <= o.value <= 0xFF:
                raise ValueError("displacement does not fit 8 bits")
            put16(_brief(o) | (o.value & 0xFF))
        elif k == "absw":
            put16(o.value & 0xFFFF)
        elif k == "absl":
            put32(o.value & 0xFFFFFFFF)
        elif k == "pcdisp":
            d = _wrap(o.value - (pc + len(out)))
            if not -0x8000 <= d <= 0x7FFF:
                raise ValueError("PC-relative operand out of range")
            put16(d & 0xFFFF)
        elif k == "pcindex":
            d = _wrap(o.value - (pc + len(out)))
            if not -0x80 <= d <= 0x7F:
                raise ValueError("PC-relative operand out of range")
            put16(_brief(o) | (d & 0xFF))
        elif k == "label":
            d = _wrap(o.value - (pc + len(out)))
            if not -0x8000 <= d <= 0x7FFF:
                raise ValueError("branch out of range")
            put16(d & 0xFFFF)
    return bytes(out)


def _wrap(v):
    """A difference of 32-bit addresses as a signed number (the address space wraps)."""
    return ((v + 0x80000000) & 0xFFFFFFFF) - 0x80000000


def _brief(o):
    return (0x8000 if o.xreg[0] == "A" else 0) | (o.xreg[1] << 12) | (0x0800 if o.xsize == "L" else 0)
