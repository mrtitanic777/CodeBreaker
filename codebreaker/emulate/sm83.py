"""sm83.py -- the Game Boy's CPU, to run functions on.

    from codebreaker.emulate import sm83
    sm83.run(bus, entry, a=.., b=.., ...)       call a function: run to its RET; the run record

Every instruction of decompile/sm83.py's table, each pattern compiled once into a function.
Flags as the SM83 keeps them: Z N H C in bits 7-4 of F, bits 3-0 always zero; 16-bit ADD sets H
and C from bits 11 and 15; ADD SP,e and LD HL,SP+e set H and C from the low byte's unsigned add
and clear Z; DAA works from N H C as the Z80's does but leaves H clear. The I/O registers at
$FF00-$FF7F and $FFFF are the machine's: a Bus hands them to callbacks and logs each access.
"""

from codebreaker.decompile import sm83 as table
from codebreaker.emulate.z80 import Bus, Stop  # noqa: F401 (sm83.Bus: the same 64 KB bus with mapped ranges)

MAX_STEPS = 200000
FZ, FN, FH, FC = 0x80, 0x40, 0x20, 0x10
RETURN = 0xFFFE   # pushed by run(): reaching it ends the run (an address no program returns to: high RAM's end)


class CPU(object):
    def __init__(self, memory):
        self.m = memory
        self.a = self.f = self.b = self.c = self.d = self.e = self.h = self.l = 0
        self.sp, self.pc = 0xFFFE, 0x0100
        self.ime = False
        self.halted = False
        self.steps = 0
        self._decoded = {}

    def rd(self, a):
        return self.m[a & 0xFFFF]

    def wr(self, a, v):
        self.m[a & 0xFFFF] = v & 0xFF

    def push(self, v):
        self.sp = (self.sp - 2) & 0xFFFF
        self.wr(self.sp, v)
        self.wr(self.sp + 1, v >> 8)

    def pop(self):
        v = self.rd(self.sp) | self.rd(self.sp + 1) << 8
        self.sp = (self.sp + 2) & 0xFFFF
        return v

    def get(self, name):
        if name in _PAIRS:
            hi, lo = _PAIRS[name]
            return getattr(self, hi) << 8 | getattr(self, lo)
        return getattr(self, name.lower())

    def set(self, name, v):
        if name in _PAIRS:
            hi, lo = _PAIRS[name]
            setattr(self, hi, (v >> 8) & 0xFF)
            setattr(self, lo, v & (0xF0 if lo == "f" else 0xFF))
        elif name == "SP":
            self.sp = v & 0xFFFF
        else:
            setattr(self, name.lower(), v & 0xFF)

    def cond(self, cc):
        return {"NZ": not self.f & FZ, "Z": bool(self.f & FZ), "NC": not self.f & FC, "C": bool(self.f & FC)}[cc]

    def step(self):
        if self.halted:
            raise Stop("HALT")
        pc = self.pc
        d = self._decoded.get(pc)
        if d is None or d.bytes != bytes(self.rd(pc + i) for i in range(d.size)):
            d = table.decode(bytes(self.rd(pc + i) for i in range(3)), 0, pc)
            if d is None:
                raise Stop("no instruction at %04X" % pc)
            self._decoded[pc] = d
        self.pc = (pc + d.size) & 0xFFFF
        _handler(d.pattern)(self, d)
        self.steps += 1


_PAIRS = {"AF": ("a", "f"), "BC": ("b", "c"), "DE": ("d", "e"), "HL": ("h", "l")}
_REG8 = {"A", "B", "C", "D", "E", "H", "L"}


def _operand(form, k):
    if form in _REG8:
        return (lambda c, d: c.get(form)), (lambda c, d, v: c.set(form, v)), False
    if form in ("BC", "DE", "HL", "SP", "AF"):
        return (lambda c, d: c.get(form)), (lambda c, d, v: c.set(form, v)), False
    if form in ("n", "nn", "s"):
        return (lambda c, d: d.values[k]), None, True
    if form in ("(BC)", "(DE)", "(HL)"):
        pair = form[1:3]
        return (lambda c, d: c.rd(c.get(pair))), (lambda c, d, v: c.wr(c.get(pair), v)), False
    if form in ("(nn)", "(h)"):
        return (lambda c, d: c.rd(d.values[k])), (lambda c, d, v: c.wr(d.values[k], v)), True
    if form == "(C)":
        return (lambda c, d: c.rd(0xFF00 + c.c)), (lambda c, d, v: c.wr(0xFF00 + c.c, v)), False
    if form in ("(HL+)", "(HL-)"):
        step = 1 if form[3] == "+" else -1

        def r(c, d):
            hl = c.get("HL")
            c.set("HL", hl + step)
            return c.rd(hl)

        def w(c, d, v):
            hl = c.get("HL")
            c.set("HL", hl + step)
            c.wr(hl, v)
        return r, w, False
    if form.isdigit():
        return (lambda c, d: int(form)), None, False
    return None, None, False


_HANDLERS = {}


def _handler(pattern):
    h = _HANDLERS.get(pattern)
    if h is None:
        h = _HANDLERS[pattern] = _compile(pattern)
    return h


def _alu(c, op, v):
    a, cy = c.a, 1 if c.f & FC else 0
    if op in ("ADD", "ADC"):
        k = cy if op == "ADC" else 0
        r = a + v + k
        c.f = (FZ if r & 0xFF == 0 else 0) | (FH if (a & 0xF) + (v & 0xF) + k > 0xF else 0) | (FC if r > 0xFF else 0)
        c.a = r & 0xFF
    elif op in ("SUB", "SBC", "CP"):
        k = cy if op == "SBC" else 0
        r = a - v - k
        c.f = FN | (FZ if r & 0xFF == 0 else 0) | (FH if (a & 0xF) - (v & 0xF) - k < 0 else 0) | (FC if r < 0 else 0)
        if op != "CP":
            c.a = r & 0xFF
    elif op == "AND":
        c.a &= v
        c.f = (FZ if c.a == 0 else 0) | FH
    elif op == "XOR":
        c.a ^= v
        c.f = FZ if c.a == 0 else 0
    else:
        c.a |= v
        c.f = FZ if c.a == 0 else 0


def _rot(c, op, v):
    cy = 1 if c.f & FC else 0
    if op == "RLC":
        r, co = ((v << 1) | (v >> 7)) & 0xFF, v >> 7
    elif op == "RRC":
        r, co = ((v >> 1) | (v << 7)) & 0xFF, v & 1
    elif op == "RL":
        r, co = ((v << 1) | cy) & 0xFF, v >> 7
    elif op == "RR":
        r, co = (v >> 1) | (cy << 7), v & 1
    elif op == "SLA":
        r, co = (v << 1) & 0xFF, v >> 7
    elif op == "SRA":
        r, co = (v >> 1) | (v & 0x80), v & 1
    elif op == "SWAP":
        r, co = ((v << 4) | (v >> 4)) & 0xFF, 0
    else:
        r, co = v >> 1, v & 1
    c.f = (FZ if r == 0 else 0) | (FC if co else 0)
    return r


def _sp_plus(c, e):
    sp = c.sp
    c.f = (FH if (sp & 0xF) + (e & 0xF) > 0xF else 0) | (FC if (sp & 0xFF) + (e & 0xFF) > 0xFF else 0)
    return (sp + e) & 0xFFFF


def _compile(pattern):
    m, _, rest = pattern.partition(" ")
    forms = rest.split(",") if rest else []
    ops, k = [], 0
    for f in forms:
        if f in table.CC and m in ("JP", "JR", "CALL", "RET"):
            ops.append((None, None))
            continue
        r, w, takes = _operand(f, k)
        ops.append((r, w))
        k += takes
    rd = [o[0] for o in ops]
    wr = [o[1] for o in ops]

    if m in ("NOP",):
        return lambda c, d: None
    if m == "STOP":
        def stop(c, d):
            raise Stop("STOP")
        return stop
    if pattern == "LD (nn),SP":
        def ld_sp(c, d):
            c.wr(d.values[0], c.sp)
            c.wr(d.values[0] + 1, c.sp >> 8)
        return ld_sp
    if pattern == "LD HL,SP+s":
        return lambda c, d: c.set("HL", _sp_plus(c, d.values[0]))
    if pattern == "ADD SP,s":
        return lambda c, d: setattr(c, "sp", _sp_plus(c, d.values[0]))
    if m in ("LD", "LDH"):
        r1, w0 = rd[1], wr[0]
        return lambda c, d: w0(c, d, r1(c, d))
    if m == "PUSH":
        return lambda c, d: c.push(c.get(forms[0]))
    if m == "POP":
        return lambda c, d: c.set(forms[0], c.pop())
    if m in ("ADD", "ADC", "SUB", "SBC", "AND", "XOR", "OR", "CP"):
        if pattern.startswith("ADD HL,"):
            src = forms[1]

            def add16(c, d):
                a, b = c.get("HL"), c.get(src)
                r = a + b
                c.f = (c.f & FZ) | (FH if (a & 0xFFF) + (b & 0xFFF) > 0xFFF else 0) | (FC if r > 0xFFFF else 0)
                c.set("HL", r)
            return add16
        src = rd[-1]
        return lambda c, d: _alu(c, m, src(c, d))
    if m in ("INC", "DEC"):
        f = forms[0]
        if f in ("BC", "DE", "HL", "SP"):
            delta = 1 if m == "INC" else -1
            return lambda c, d: c.set(f, (c.get(f) + delta) & 0xFFFF)
        r0, w0 = rd[0], wr[0]

        def incdec(c, d):
            v = r0(c, d)
            if m == "INC":
                r = (v + 1) & 0xFF
                c.f = (c.f & FC) | (FZ if r == 0 else 0) | (FH if v & 0xF == 0xF else 0)
            else:
                r = (v - 1) & 0xFF
                c.f = (c.f & FC) | FN | (FZ if r == 0 else 0) | (FH if v & 0xF == 0 else 0)
            w0(c, d, r)
        return incdec
    if m in ("RLCA", "RRCA", "RLA", "RRA"):
        op = m[:-1]

        def rot_a(c, d):
            c.a = _rot(c, op, c.a)
            c.f &= FC                       # Z is always cleared
        return rot_a
    if m in table.ROT:
        r0, w0 = rd[0], wr[0]
        return lambda c, d: w0(c, d, _rot(c, m, r0(c, d)))
    if m == "BIT":
        bit, r1 = int(forms[0]), rd[1]
        return lambda c, d: setattr(c, "f", (c.f & FC) | FH | (0 if r1(c, d) & (1 << bit) else FZ))
    if m in ("SET", "RES"):
        bit, r1, w1 = int(forms[0]), rd[1], wr[1]
        return lambda c, d: w1(c, d, r1(c, d) | (1 << bit) if m == "SET" else r1(c, d) & ~(1 << bit))
    if m in ("JP", "JR", "CALL"):
        if pattern == "JP HL":
            return lambda c, d: setattr(c, "pc", c.get("HL"))
        cc = forms[0] if len(forms) == 2 else None

        def jump(c, d):
            if cc is None or c.cond(cc):
                if m == "CALL":
                    c.push(c.pc)
                c.pc = d.target
        return jump
    if m == "RET":
        cc = forms[0] if forms else None

        def ret(c, d):
            if cc is None or c.cond(cc):
                c.pc = c.pop()
        return ret
    if m == "RETI":
        def reti(c, d):
            c.ime = True
            c.pc = c.pop()
        return reti
    if m == "RST":
        def rst(c, d):
            c.push(c.pc)
            c.pc = d.target
        return rst
    if m == "HALT":
        return lambda c, d: setattr(c, "halted", True)
    if m in ("DI", "EI"):
        return lambda c, d: setattr(c, "ime", m == "EI")
    if m == "CPL":
        def cpl(c, d):
            c.a ^= 0xFF
            c.f = (c.f & (FZ | FC)) | FN | FH
        return cpl
    if m == "SCF":
        return lambda c, d: setattr(c, "f", (c.f & FZ) | FC)
    if m == "CCF":
        return lambda c, d: setattr(c, "f", (c.f & FZ) | (0 if c.f & FC else FC))
    if m == "DAA":
        def daa(c, d):
            a, f = c.a, c.f
            carry = f & FC
            if f & FN:
                if f & FH:
                    a = (a - 0x06) & 0xFF
                if carry:
                    a = (a - 0x60) & 0xFF
            else:
                if (f & FH) or (a & 0x0F) > 9:
                    a += 0x06
                if carry or a > 0x9F:
                    a += 0x60
                    carry = FC
            c.a = a & 0xFF
            c.f = (FZ if c.a == 0 else 0) | (f & FN) | carry
        return daa
    raise ValueError("no handler for %r" % pattern)


def run(memory, entry, steps=MAX_STEPS, sp=0xDFF0, **regs):
    """Call the function at `entry` as CALL would and run it until it returns.
    -> {end, a, f, bc, de, hl, sp, steps, io}"""
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
            "sp": cpu.sp, "steps": cpu.steps, "io": list(getattr(memory, "log", []))}
