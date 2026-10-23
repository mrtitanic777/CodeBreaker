"""z80.py -- a Z80, to run functions on (Unicorn has none).

    from codebreaker.emulate import z80
    cpu = z80.CPU(bus)                          # bus: a Bus, or a 64 KB bytearray
    cpu.step()                                  # one instruction
    z80.run(bus, entry, a=.., bc=.., ...)       # call a function: run to its RET; the run record

Every instruction of the table in decompile/z80.py, the undocumented ones included (IXH/IXL,
SLL, the DDCB forms that copy their result to a register, OUT (C),0), read from that table: each
pattern ("LD (IX+d),n", "ADC HL,DE", ...) is compiled once into a function. Flags as the CPU
sets them -- S Z H P/V N C, and the two undocumented bits (5 and 3, copied from the result, or
from the operand where the CPU takes them from there: CP, BIT, the block instructions) -- so
that a rebuilt program and its original leave the same F. R counts instruction fetches (its low
seven bits); interrupts are not raised (a function is run on its own).

What a function touches outside memory is the machine's business: the Z80 has its own I/O
ports (IN, OUT, and the block forms), which go to the Bus's `inp`/`outp` and are logged with the
memory ranges a Bus hands to callbacks -- the effects the judge compares.
"""
import re

from codebreaker.decompile import z80 as table

MAX_STEPS = 200000
FS, FZ, FY, FH, FX, FP, FN, FC = 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
RETURN = 0xFFFF   # the return address run() pushes: reaching it ends the run


class Stop(Exception):
    pass


class Bus(object):
    """64 KB of memory and 256 ports (the Z80 puts the full 16-bit BC on the bus; most machines
    decode the low byte). io ranges of memory go to callbacks: map(lo, hi, read, write)."""

    def __init__(self, memory=None, inp=None, outp=None):
        self.mem = memory if memory is not None else bytearray(0x10000)
        self.io = []
        self.log = []                                  # (kind, address or port, value)
        self._inp, self._outp = inp, outp

    def map(self, lo, hi, read=None, write=None):
        self.io.append((lo, hi, read, write))

    def __getitem__(self, a):
        for lo, hi, read, _w in self.io:
            if lo <= a <= hi:
                v = read(a) & 0xFF if read else 0
                self.log.append(("r", a, v))
                return v
        return self.mem[a]

    def __setitem__(self, a, v):
        for lo, hi, _r, write in self.io:
            if lo <= a <= hi:
                self.log.append(("w", a, v & 0xFF))
                if write:
                    write(a, v & 0xFF)
                return
        self.mem[a] = v & 0xFF

    def inp(self, port):
        v = (self._inp(port) if self._inp else 0xFF) & 0xFF
        self.log.append(("in", port & 0xFF, v))
        return v

    def outp(self, port, v):
        self.log.append(("out", port & 0xFF, v & 0xFF))
        if self._outp:
            self._outp(port, v & 0xFF)


def _parity(v):
    v ^= v >> 4
    v ^= v >> 2
    v ^= v >> 1
    return 0 if v & 1 else FP


_SZ = [(v & (FS | FY | FX)) | (FZ if v == 0 else 0) for v in range(256)]
_SZP = [_SZ[v] | _parity(v) for v in range(256)]


class CPU(object):
    def __init__(self, memory):
        self.m = memory
        self.a = self.f = self.b = self.c = self.d = self.e = self.h = self.l = 0
        self.a_ = self.f_ = self.b_ = self.c_ = self.d_ = self.e_ = self.h_ = self.l_ = 0
        self.ix = self.iy = 0xFFFF
        self.sp, self.pc, self.i, self.r = 0xFFFF, 0, 0, 0
        self.iff1 = self.iff2 = False
        self.im = 0
        self.halted = False
        self.steps = 0
        self._decoded = {}

    # ---- memory
    def rd(self, a):
        return self.m[a & 0xFFFF]

    def wr(self, a, v):
        self.m[a & 0xFFFF] = v & 0xFF

    def rd16(self, a):
        return self.rd(a) | self.rd(a + 1) << 8

    def wr16(self, a, v):
        self.wr(a, v)
        self.wr(a + 1, v >> 8)

    def push(self, v):
        self.sp = (self.sp - 2) & 0xFFFF
        self.wr16(self.sp, v)

    def pop(self):
        v = self.rd16(self.sp)
        self.sp = (self.sp + 2) & 0xFFFF
        return v

    # ---- register pairs and halves
    def get(self, name):
        if name in _PAIRS:
            hi, lo = _PAIRS[name]
            return getattr(self, hi) << 8 | getattr(self, lo)
        if name in ("IXH", "IXL", "IYH", "IYL"):
            v = self.ix if name[1] == "X" else self.iy
            return v >> 8 if name[2] == "H" else v & 0xFF
        return getattr(self, _FIELD[name])

    def set(self, name, v):
        if name in _PAIRS:
            hi, lo = _PAIRS[name]
            setattr(self, hi, (v >> 8) & 0xFF)
            setattr(self, lo, v & 0xFF)
        elif name in ("IXH", "IXL", "IYH", "IYL"):
            field = "ix" if name[1] == "X" else "iy"
            old = getattr(self, field)
            setattr(self, field, ((v & 0xFF) << 8 | (old & 0xFF)) if name[2] == "H" else ((old & 0xFF00) | (v & 0xFF)))
        else:
            setattr(self, _FIELD[name], v & (0xFFFF if name in ("SP", "IX", "IY") else 0xFF))

    def cond(self, cc):
        f = self.f
        return {"NZ": not f & FZ, "Z": bool(f & FZ), "NC": not f & FC, "C": bool(f & FC),
                "PO": not f & FP, "PE": bool(f & FP), "P": not f & FS, "M": bool(f & FS)}[cc]

    # ---- one instruction
    def step(self):
        if self.halted:
            raise Stop("HALT")
        pc = self.pc
        insn = self._decoded.get(pc)
        if insn is None or insn[1] != self._bytes_at(pc, len(insn[1])):
            code = self._bytes_at(pc, 4)
            d = table.decode(code, 0, pc)
            if d is None:
                raise Stop("undecodable at %04X" % pc)
            insn = (d, d.bytes)
            self._decoded[pc] = insn
        d = insn[0]
        self.r = (self.r & 0x80) | ((self.r + (2 if d.bytes[:1] in (b"\xDD", b"\xFD", b"\xCB", b"\xED") else 1)) & 0x7F)
        self.pc = (pc + d.size) & 0xFFFF
        _handler(d.pattern)(self, d)
        self.steps += 1

    def _bytes_at(self, pc, n):
        return bytes(self.rd(pc + i) for i in range(n))


_PAIRS = {"AF": ("a", "f"), "BC": ("b", "c"), "DE": ("d", "e"), "HL": ("h", "l")}
_FIELD = {"A": "a", "F": "f", "B": "b", "C": "c", "D": "d", "E": "e", "H": "h", "L": "l", "SP": "sp",
          "IX": "ix", "IY": "iy", "I": "i", "R": "r"}
_REG8 = {"A", "B", "C", "D", "E", "H", "L", "IXH", "IXL", "IYH", "IYL", "I", "R"}
_REG16 = {"AF", "BC", "DE", "HL", "SP", "IX", "IY"}


# ---- operands: (read, write) for each written form ------------------------------------------------

def _operand(form, k):
    """(read(cpu, d), write(cpu, d, v), consumes a value?) for an operand as the table writes it;
    `k` is the index of its value in d.values."""
    if form in _REG8 or form in _REG16:
        return (lambda c, d: c.get(form)), (lambda c, d, v: c.set(form, v)), False
    if form in ("n", "nn"):
        return (lambda c, d: d.values[k]), None, True
    if form in ("(BC)", "(DE)", "(HL)"):
        pair = form[1:3]
        return (lambda c, d: c.rd(c.get(pair))), (lambda c, d, v: c.wr(c.get(pair), v)), False
    if form in ("(nn)",):
        return (lambda c, d: c.rd(d.values[k])), (lambda c, d, v: c.wr(d.values[k], v)), True
    m = re.match(r"\((I[XY])\+d\)$", form)
    if m:
        reg = m.group(1)
        addr = lambda c, d: (c.get(reg) + d.values[k]) & 0xFFFF
        return (lambda c, d: c.rd(addr(c, d))), (lambda c, d, v: c.wr(addr(c, d), v)), True
    if form.isdigit():
        return (lambda c, d: int(form)), None, False
    # (C) (n) (SP) (IX) AF' and the like: read by their instruction's own handler
    return None, None, form == "(n)"


def _operands(pattern):
    m, _, rest = pattern.partition(" ")
    forms = rest.split(",") if rest else []
    out, k = [], 0
    for f in forms:
        r, w, takes = _operand(f, k) if f not in table.CC or m not in ("JP", "JR", "CALL", "RET") else (None, None, False)
        out.append((f, r, w))
        k += takes
    return m, out


# ---- arithmetic with flags ---------------------------------------------------------------------------

def _add8(c, a, b, carry=0):
    r = a + b + carry
    f = _SZ[r & 0xFF] | ((a ^ b ^ r) & FH) | (FC if r > 0xFF else 0)
    f |= FP if (~(a ^ b) & (a ^ r) & 0x80) else 0
    c.f = f
    return r & 0xFF


def _sub8(c, a, b, carry=0, keep=True):
    r = a - b - carry
    f = _SZ[r & 0xFF] | FN | ((a ^ b ^ r) & FH) | (FC if r < 0 else 0)
    f |= FP if ((a ^ b) & (a ^ r) & 0x80) else 0
    c.f = f
    return r & 0xFF


def _alu(c, op, v):
    a = c.a
    if op == "ADD":
        c.a = _add8(c, a, v)
    elif op == "ADC":
        c.a = _add8(c, a, v, c.f & FC)
    elif op == "SUB":
        c.a = _sub8(c, a, v)
    elif op == "SBC":
        c.a = _sub8(c, a, v, c.f & FC)
    elif op == "AND":
        c.a = a & v
        c.f = _SZP[c.a] | FH
    elif op == "XOR":
        c.a = a ^ v
        c.f = _SZP[c.a]
    elif op == "OR":
        c.a = a | v
        c.f = _SZP[c.a]
    else:  # CP: the flags of A - v, bits 5 and 3 from v
        _sub8(c, a, v)
        c.f = (c.f & ~(FY | FX)) | (v & (FY | FX))


def _rot(c, op, v):
    cy = c.f & FC
    if op == "RLC":
        r = ((v << 1) | (v >> 7)) & 0xFF
        co = v >> 7
    elif op == "RRC":
        r = ((v >> 1) | (v << 7)) & 0xFF
        co = v & 1
    elif op == "RL":
        r = ((v << 1) | cy) & 0xFF
        co = v >> 7
    elif op == "RR":
        r = (v >> 1) | (cy << 7)
        co = v & 1
    elif op == "SLA":
        r = (v << 1) & 0xFF
        co = v >> 7
    elif op == "SRA":
        r = (v >> 1) | (v & 0x80)
        co = v & 1
    elif op == "SLL":
        r = ((v << 1) | 1) & 0xFF
        co = v >> 7
    else:  # SRL
        r = v >> 1
        co = v & 1
    c.f = _SZP[r] | (FC if co else 0)
    return r


def _inc8(c, v):
    r = (v + 1) & 0xFF
    c.f = (c.f & FC) | _SZ[r] | (FH if (v & 0x0F) == 0x0F else 0) | (FP if v == 0x7F else 0)
    return r


def _dec8(c, v):
    r = (v - 1) & 0xFF
    c.f = (c.f & FC) | _SZ[r] | FN | (FH if (v & 0x0F) == 0 else 0) | (FP if v == 0x80 else 0)
    return r


def _add16(c, a, b):
    r = a + b
    c.f = (c.f & (FS | FZ | FP)) | (((a ^ b ^ r) >> 8) & FH) | ((r >> 8) & (FY | FX)) | (FC if r > 0xFFFF else 0)
    return r & 0xFFFF


def _adc16(c, a, b):
    cy = c.f & FC
    r = a + b + cy
    f = (FS if r & 0x8000 else 0) | (FZ if (r & 0xFFFF) == 0 else 0) | (((a ^ b ^ r) >> 8) & FH)
    f |= ((r >> 8) & (FY | FX)) | (FC if r > 0xFFFF else 0)
    f |= FP if (~(a ^ b) & (a ^ r) & 0x8000) else 0
    c.f = f
    return r & 0xFFFF


def _sbc16(c, a, b):
    cy = c.f & FC
    r = a - b - cy
    f = FN | (FS if r & 0x8000 else 0) | (FZ if (r & 0xFFFF) == 0 else 0) | (((a ^ b ^ r) >> 8) & FH)
    f |= ((r >> 8) & (FY | FX)) | (FC if r < 0 else 0)
    f |= FP if ((a ^ b) & (a ^ r) & 0x8000) else 0
    c.f = f
    return r & 0xFFFF


# ---- the handlers ----------------------------------------------------------------------------------

_HANDLERS = {}


def _handler(pattern):
    h = _HANDLERS.get(pattern)
    if h is None:
        h = _HANDLERS[pattern] = _compile(pattern)
    return h


def _compile(pattern):
    m, ops = _operands(pattern)
    forms = [o[0] for o in ops]
    rd = [o[1] for o in ops]
    wr = [o[2] for o in ops]

    if m == "NOP":
        return lambda c, d: None
    if m == "LD":
        if forms[0] in ("A",) and forms[1] in ("I", "R"):
            def ld_ir(c, d):
                c.a = c.get(forms[1])
                c.f = (c.f & FC) | _SZ[c.a] | (FP if c.iff2 else 0)
            return ld_ir
        if forms[0].startswith("(") and forms[0] == "(nn)" and forms[1] in _REG16:
            return lambda c, d: c.wr16(d.values[0], c.get(forms[1]))
        if forms[1] == "(nn)" and forms[0] in _REG16:
            return lambda c, d: c.set(forms[0], c.rd16(d.values[0]))
        if forms[0].endswith("+d)") and forms[1] == "n":
            # LD (IX+d),n: the displacement is value 0, the byte value 1
            reg = forms[0][1:3]
            return lambda c, d: c.wr((c.get(reg) + d.values[0]) & 0xFFFF, d.values[1])
        r1, w0 = rd[1], wr[0]
        return lambda c, d: w0(c, d, r1(c, d))
    if m in ("PUSH", "POP"):
        reg = forms[0]
        if m == "PUSH":
            return lambda c, d: c.push(c.get(reg))
        return lambda c, d: c.set(reg, c.pop())
    if m == "EX":
        if forms == ["AF", "AF'"]:
            def ex_af(c, d):
                c.a, c.a_ = c.a_, c.a
                c.f, c.f_ = c.f_, c.f
            return ex_af
        if forms == ["DE", "HL"]:
            def ex_de(c, d):
                de, hl = c.get("DE"), c.get("HL")
                c.set("DE", hl)
                c.set("HL", de)
            return ex_de
        reg = forms[1]   # EX (SP),HL/IX/IY

        def ex_sp(c, d):
            v = c.rd16(c.sp)
            c.wr16(c.sp, c.get(reg))
            c.set(reg, v)
        return ex_sp
    if m == "EXX":
        def exx(c, d):
            for r in "bcdehl":
                a, b = getattr(c, r), getattr(c, r + "_")
                setattr(c, r, b)
                setattr(c, r + "_", a)
        return exx
    if m in ("ADD", "ADC", "SUB", "SBC", "AND", "XOR", "OR", "CP"):
        if forms[0] in ("HL", "IX", "IY") and len(forms) == 2:
            dst, src = forms

            def arith16(c, d):
                a, b = c.get(dst), c.get(src)
                c.set(dst, _add16(c, a, b) if m == "ADD" else _adc16(c, a, b) if m == "ADC" else _sbc16(c, a, b))
            return arith16
        src = rd[-1]
        return lambda c, d: _alu(c, m, src(c, d))
    if m in ("INC", "DEC"):
        f = forms[0]
        if f in _REG16:
            delta = 1 if m == "INC" else -1
            return lambda c, d: c.set(f, (c.get(f) + delta) & 0xFFFF)
        r0, w0 = rd[0], wr[0]
        fn = _inc8 if m == "INC" else _dec8
        return lambda c, d: w0(c, d, fn(c, r0(c, d)))
    if m in ("RLCA", "RRCA", "RLA", "RRA"):
        op = m[:-1]

        def rot_a(c, d):
            keep = c.f & (FS | FZ | FP)
            c.a = _rot(c, op, c.a)
            c.f = keep | (c.f & FC) | (c.a & (FY | FX))
        return rot_a
    if m in table.ROT:
        r0, w0 = rd[0], wr[0]
        copy = forms[1] if len(forms) > 1 else None        # DDCB undocumented: result also into a register

        def rot(c, d):
            v = _rot(c, m, r0(c, d))
            w0(c, d, v)
            if copy:
                c.set(copy, v)
        return rot
    if m == "BIT":
        bit, r1 = int(forms[0]), rd[1]
        indexed = forms[1].endswith("+d)") or forms[1] == "(HL)"

        def bit_(c, d):
            v = r1(c, d)
            t = v & (1 << bit)
            f = (c.f & FC) | FH | (FZ | FP if not t else 0) | (FS if bit == 7 and t else 0)
            if not indexed:
                f |= v & (FY | FX)
            elif forms[1].endswith("+d)"):
                f |= ((c.get(forms[1][1:3]) + d.values[0]) >> 8) & (FY | FX)
            c.f = f
        return bit_
    if m in ("SET", "RES"):
        bit, r1, w1 = int(forms[0]), rd[1], wr[1]
        copy = forms[2] if len(forms) > 2 else None

        def setres(c, d):
            v = r1(c, d)
            v = v | (1 << bit) if m == "SET" else v & ~(1 << bit)
            w1(c, d, v)
            if copy:
                c.set(copy, v)
        return setres
    if m in ("JP", "JR", "CALL"):
        if forms and forms[0].startswith("("):              # JP (HL) / (IX) / (IY)
            reg = forms[0][1:-1]

            def jp_reg(c, d):
                c.pc = c.get(reg)
            return jp_reg
        cc = forms[0] if len(forms) == 2 else None

        def jump(c, d):
            if cc is None or c.cond(cc):
                if m == "CALL":
                    c.push(c.pc)
                c.pc = d.target
        return jump
    if m == "DJNZ":
        def djnz(c, d):
            c.b = (c.b - 1) & 0xFF
            if c.b:
                c.pc = d.target
        return djnz
    if m == "RET":
        cc = forms[0] if forms else None

        def ret(c, d):
            if cc is None or c.cond(cc):
                c.pc = c.pop()
        return ret
    if m in ("RETI", "RETN"):
        def reti(c, d):
            c.iff1 = c.iff2
            c.pc = c.pop()
        return reti
    if m == "RST":
        def rst(c, d):
            c.push(c.pc)
            c.pc = d.target
        return rst
    if m == "HALT":
        def halt(c, d):
            c.halted = True
        return halt
    if m in ("DI", "EI"):
        def di_ei(c, d):
            c.iff1 = c.iff2 = m == "EI"
        return di_ei
    if m == "IM":
        mode = int(forms[0])
        return lambda c, d: setattr(c, "im", mode)
    if m == "CPL":
        def cpl(c, d):
            c.a ^= 0xFF
            c.f = (c.f & (FS | FZ | FP | FC)) | FH | FN | (c.a & (FY | FX))
        return cpl
    if m == "NEG":
        def neg(c, d):
            c.a = _sub8(c, 0, c.a)
        return neg
    if m == "SCF":
        return lambda c, d: setattr(c, "f", (c.f & (FS | FZ | FP)) | FC | (c.a & (FY | FX)))
    if m == "CCF":
        return lambda c, d: setattr(c, "f", ((c.f & (FS | FZ | FP | FC)) | (FH if c.f & FC else 0) |
                                             (c.a & (FY | FX))) ^ FC)
    if m == "DAA":
        def daa(c, d):
            a, f = c.a, c.f
            adjust, carry = 0, f & FC
            if (f & FH) or (a & 0x0F) > 9:
                adjust |= 0x06
            if carry or a > 0x99:
                adjust |= 0x60
                carry = FC
            r = (a - adjust if f & FN else a + adjust) & 0xFF
            h = ((a ^ r) & FH)
            c.a = r
            c.f = _SZP[r] | (f & FN) | h | carry
        return daa
    if m in ("RLD", "RRD"):
        def rld(c, d):
            hl = c.get("HL")
            v = c.rd(hl)
            if m == "RLD":
                c.wr(hl, (v << 4) | (c.a & 0x0F))
                c.a = (c.a & 0xF0) | (v >> 4)
            else:
                c.wr(hl, (v >> 4) | ((c.a & 0x0F) << 4))
                c.a = (c.a & 0xF0) | (v & 0x0F)
            c.f = (c.f & FC) | _SZP[c.a]
        return rld
    if m == "IN":
        if forms == ["A", "(n)"]:
            return lambda c, d: setattr(c, "a", c.m.inp(d.values[0] | c.a << 8) if hasattr(c.m, "inp") else 0xFF)
        reg = forms[0] if forms[0] != "(C)" else None

        def in_c(c, d):
            v = c.m.inp(c.get("BC")) if hasattr(c.m, "inp") else 0xFF
            if reg:
                c.set(reg, v)
            c.f = (c.f & FC) | _SZP[v]
        return in_c
    if m == "OUT":
        if forms == ["(n)", "A"]:
            return lambda c, d: c.m.outp(d.values[0] | c.a << 8, c.a) if hasattr(c.m, "outp") else None
        src = forms[1]

        def out_c(c, d):
            if hasattr(c.m, "outp"):
                c.m.outp(c.get("BC"), 0 if src == "0" else c.get(src))
        return out_c
    if m in ("LDI", "LDD", "LDIR", "LDDR"):
        step = 1 if m[2] == "I" else -1
        repeat = m.endswith("R")

        def ldx(c, d):
            v = c.rd(c.get("HL"))
            c.wr(c.get("DE"), v)
            c.set("HL", (c.get("HL") + step) & 0xFFFF)
            c.set("DE", (c.get("DE") + step) & 0xFFFF)
            bc = (c.get("BC") - 1) & 0xFFFF
            c.set("BC", bc)
            n = (v + c.a) & 0xFF
            c.f = (c.f & (FS | FZ | FC)) | (FP if bc else 0) | (n & FX) | ((n << 4) & FY)
            if repeat and bc:
                c.pc = (c.pc - 2) & 0xFFFF
        return ldx
    if m in ("CPI", "CPD", "CPIR", "CPDR"):
        step = 1 if m[2] == "I" else -1
        repeat = m.endswith("R")

        def cpx(c, d):
            v = c.rd(c.get("HL"))
            r = (c.a - v) & 0xFF
            h = (c.a ^ v ^ r) & FH
            c.set("HL", (c.get("HL") + step) & 0xFFFF)
            bc = (c.get("BC") - 1) & 0xFFFF
            c.set("BC", bc)
            n = (r - (1 if h else 0)) & 0xFF
            c.f = (c.f & FC) | FN | (_SZ[r] & (FS | FZ)) | h | (FP if bc else 0) | (n & FX) | ((n << 4) & FY)
            if repeat and bc and r != 0:
                c.pc = (c.pc - 2) & 0xFFFF
        return cpx
    if m in ("INI", "IND", "INIR", "INDR", "OUTI", "OUTD", "OTIR", "OTDR"):
        step = 1 if m in ("INI", "INIR", "OUTI", "OTIR") else -1
        repeat = m.endswith("R") and m not in ("OUTD",)
        is_in = m.startswith("IN")

        def io_block(c, d):
            hl = c.get("HL")
            if is_in:
                v = c.m.inp(c.get("BC")) if hasattr(c.m, "inp") else 0xFF
                c.wr(hl, v)
                c.b = (c.b - 1) & 0xFF
            else:
                v = c.rd(hl)
                c.b = (c.b - 1) & 0xFF
                if hasattr(c.m, "outp"):
                    c.m.outp(c.get("BC"), v)
            c.set("HL", (hl + step) & 0xFFFF)
            c.f = _SZ[c.b] | (FN if v & 0x80 else 0) | (c.f & FC)
            if repeat and c.b:
                c.pc = (c.pc - 2) & 0xFFFF
        return io_block
    raise ValueError("no handler for %r" % pattern)


def run(memory, entry, steps=MAX_STEPS, sp=0xDFF0, **regs):
    """Call the function at `entry` as CALL would and run it until it returns.
    regs: a f b c d e h l (bytes), ix iy (words).
    -> {end: "return" | "stopped: <why>" | "limit", the registers, steps, io: [(kind, where, value)]}
    `memory` is a Bus or a 64 KB bytearray (changed in place)."""
    cpu = CPU(memory)
    for k, v in regs.items():
        setattr(cpu, k, v)
    cpu.sp = sp
    cpu.push(RETURN)
    cpu.pc = entry
    end = "limit"
    try:
        while cpu.steps < steps:
            if cpu.pc == RETURN:
                end = "return"
                break
            cpu.step()
    except Stop as e:
        end = "stopped: %s" % e
    return {"end": end, "a": cpu.a, "f": cpu.f, "bc": cpu.get("BC"), "de": cpu.get("DE"), "hl": cpu.get("HL"),
            "ix": cpu.ix, "iy": cpu.iy, "sp": cpu.sp, "steps": cpu.steps, "io": list(getattr(memory, "log", []))}
