"""m68k.py -- a 68000, to run functions on.

    from codebreaker.emulate import m68k
    bus = m68k.Bus(size=0x1000000)          # 16 MB, big-endian; map(lo, hi, read, write) hands ranges to callbacks
    m68k.run(bus, entry, d=[...], a=[...])  call a function as JSR would: run to its RTS; the run record

Every instruction decompile/m68k.py decodes, read from it (each decoded instruction is cached by
address). Flags as the 68000 sets them -- X N Z V C, with X following C where the manual says,
ADDX/SUBX/NEGX/ABCD/SBCD/NBCD leaving Z set only if it was and the result is zero, DIVU/DIVS
overflow leaving the destination alone and setting V -- and addresses 24 bits wide. A byte pushed
or popped through A7 moves it by two. Exceptions (TRAP, CHK, a division by zero, ILLEGAL, line
A/F) end the run with the reason: a function is run on its own, without the program's handlers.
"""
from codebreaker.decompile import m68k as table

MAX_STEPS = 200000
FX, FN, FZ, FV, FC = 0x10, 0x08, 0x04, 0x02, 0x01
RETURN = 0x00FFFFF0          # the return address run() pushes (even, outside a cartridge's code): reaching it ends the run
MASK = {"B": 0xFF, "W": 0xFFFF, "L": 0xFFFFFFFF}
SIGN = {"B": 0x80, "W": 0x8000, "L": 0x80000000}
BYTES = {"B": 1, "W": 2, "L": 4}


class Stop(Exception):
    pass


class Bus(object):
    """Memory, big-endian, 24-bit addresses; ranges handed to callbacks (each access logged)."""

    def __init__(self, memory=None, size=0x1000000):
        self.mem = memory if memory is not None else bytearray(size)
        self.io = []
        self.log = []

    def map(self, lo, hi, read=None, write=None):
        self.io.append((lo, hi, read, write))

    def _io(self, a):
        for lo, hi, read, write in self.io:
            if lo <= a <= hi:
                return read, write
        return None

    def read(self, a, n):
        a &= 0xFFFFFF
        io = self._io(a) if self.io else None
        if io:
            read = io[0]
            v = read(a, n) if read else 0
            self.log.append(("r%d" % n, a, v))
            return v
        if a + n > len(self.mem):
            return 0
        return int.from_bytes(self.mem[a:a + n], "big")

    def write(self, a, n, v):
        a &= 0xFFFFFF
        io = self._io(a) if self.io else None
        if io:
            self.log.append(("w%d" % n, a, v))
            if io[1]:
                io[1](a, n, v)
            return
        if a + n <= len(self.mem):
            self.mem[a:a + n] = (v & ((1 << (8 * n)) - 1)).to_bytes(n, "big")


def _sx(v, size):
    v &= MASK[size]
    return v - (MASK[size] + 1) if v & SIGN[size] else v


class CPU(object):
    def __init__(self, bus):
        self.bus = bus
        self.d = [0] * 8
        self.a = [0] * 8
        self.pc = 0
        self.sr = 0x2700
        self.usp = 0
        self.steps = 0
        self._cache = {}

    # ---- flags
    def flag(self, f):
        return 1 if self.sr & f else 0

    def setf(self, x=None, n=None, z=None, v=None, c=None):
        sr = self.sr
        for bit, val in ((FX, x), (FN, n), (FZ, z), (FV, v), (FC, c)):
            if val is not None:
                sr = (sr | bit) if val else (sr & ~bit)
        self.sr = sr

    def nz(self, r, size):
        self.setf(n=r & SIGN[size], z=(r & MASK[size]) == 0)

    def logic(self, r, size):
        self.setf(n=r & SIGN[size], z=(r & MASK[size]) == 0, v=0, c=0)

    def cond(self, cc):
        n, z, v, c = self.flag(FN), self.flag(FZ), self.flag(FV), self.flag(FC)
        return {"T": True, "F": False, "HI": not c and not z, "LS": c or z, "CC": not c, "CS": c, "NE": not z,
                "EQ": z, "VC": not v, "VS": v, "PL": not n, "MI": n, "GE": n == v, "LT": n != v,
                "GT": not z and n == v, "LE": z or n != v, "RA": True}[cc]

    # ---- memory
    def rd(self, a, size):
        return self.bus.read(a, BYTES[size])

    def wr(self, a, size, v):
        self.bus.write(a, BYTES[size], v & MASK[size])

    def push(self, v, size="L"):
        self.a[7] = (self.a[7] - BYTES[size]) & 0xFFFFFFFF
        self.wr(self.a[7], size, v)

    def pop(self, size="L"):
        v = self.rd(self.a[7], size)
        self.a[7] = (self.a[7] + BYTES[size]) & 0xFFFFFFFF
        return v

    # ---- operands
    def address(self, o, size):
        """The address an operand names (applying -(An) / (An)+), or None for a register or immediate."""
        k = o.kind
        step = BYTES[size] if not (o.reg == 7 and size == "B") else 2
        if k == "ind":
            return self.a[o.reg]
        if k == "post":
            a = self.a[o.reg]
            self.a[o.reg] = (a + step) & 0xFFFFFFFF
            return a
        if k == "pre":
            self.a[o.reg] = (self.a[o.reg] - step) & 0xFFFFFFFF
            return self.a[o.reg]
        if k == "disp":
            return (self.a[o.reg] + o.value) & 0xFFFFFFFF
        if k in ("index", "pcindex"):
            xk, xn = o.xreg
            x = self.d[xn] if xk == "D" else self.a[xn]
            x = _sx(x, "W") if o.xsize == "W" else _sx(x, "L")
            base = self.a[o.reg] + o.value if k == "index" else o.value
            return (base + x) & 0xFFFFFFFF
        if k in ("absw", "absl", "pcdisp"):
            return o.value & 0xFFFFFFFF
        return None

    def get(self, o, size, at=None):
        k = o.kind
        if k == "D":
            return self.d[o.reg] & MASK[size]
        if k == "A":
            return self.a[o.reg] & MASK[size]
        if k in ("imm", "quick"):
            return o.value & MASK[size]
        if k == "sr":
            return self.sr
        if k == "ccr":
            return self.sr & 0x1F
        return self.rd(self.address(o, size) if at is None else at, size)

    def put(self, o, size, v, at=None):
        k = o.kind
        if k == "D":
            m = MASK[size]
            self.d[o.reg] = (self.d[o.reg] & ~m & 0xFFFFFFFF) | (v & m)
        elif k == "A":
            self.a[o.reg] = _sx(v, size) & 0xFFFFFFFF if size == "W" else v & 0xFFFFFFFF
        elif k == "sr":
            self.sr = v & 0xA71F
        elif k == "ccr":
            self.sr = (self.sr & 0xFF00) | (v & 0x1F)
        else:
            self.wr(self.address(o, size) if at is None else at, size, v)

    def modify(self, o, size, fn):
        """Read-modify-write one operand (its address worked out once)."""
        at = None if o.kind in ("D", "A") else self.address(o, size)
        r = fn(self.get(o, size, at))
        if r is not None:
            self.put(o, size, r, at)

    # ---- one instruction
    def step(self):
        pc = self.pc
        insn = self._cache.get(pc)
        if insn is None:
            code = bytes(self.bus.read(pc + i, 1) for i in range(10))
            insn = table.decode(code, 0, pc)
            if insn is None:
                raise Stop("no instruction at %06X" % pc)
            self._cache[pc] = insn
        self.pc = (pc + insn.length) & 0xFFFFFFFF
        _execute(self, insn)
        self.steps += 1


def _add(cpu, a, b, size, x=0, keep_z=False):
    m = MASK[size]
    r = (a + b + x) & m
    s = SIGN[size]
    c = (a + b + x) > m
    v = bool((~(a ^ b) & (a ^ r)) & s)
    z = (r == 0) and (cpu.flag(FZ) if keep_z else True)
    cpu.setf(x=c, n=r & s, z=z, v=v, c=c)
    return r


def _sub(cpu, a, b, size, x=0, keep_z=False, set_x=True):
    """a - b - x."""
    m = MASK[size]
    r = (a - b - x) & m
    s = SIGN[size]
    c = (b + x) > a
    v = bool(((a ^ b) & (a ^ r)) & s)
    z = (r == 0) and (cpu.flag(FZ) if keep_z else True)
    if set_x:
        cpu.setf(x=c, n=r & s, z=z, v=v, c=c)
    else:
        cpu.setf(n=r & s, z=z, v=v, c=c)
    return r


def _bcd_add(cpu, a, b):
    x = cpu.flag(FX)
    lo = (a & 0xF) + (b & 0xF) + x
    hi = (a & 0xF0) + (b & 0xF0)
    r = hi + lo
    if lo > 9:
        r += 6
    c = r > 0x99
    if c:
        r += 0x60
    r &= 0xFF
    cpu.setf(x=c, c=c, z=(r == 0) and cpu.flag(FZ))
    return r


def _bcd_sub(cpu, a, b):
    """a - b - X in BCD."""
    x = cpu.flag(FX)
    lo = (a & 0xF) - (b & 0xF) - x
    r = (a & 0xF0) - (b & 0xF0) + lo
    if lo < 0:
        r -= 6
    c = r < 0
    if c:
        r -= 0x60
    r &= 0xFF
    cpu.setf(x=c, c=c, z=(r == 0) and cpu.flag(FZ))
    return r


def _shift(cpu, name, v, count, size):
    m, s, bits = MASK[size], SIGN[size], BYTES[size] * 8
    kind, left = name[:-1], name.endswith("L")
    x = cpu.flag(FX)
    c, vflag = 0, 0
    if count == 0:
        cpu.setf(n=v & s, z=(v & m) == 0, v=0, c=x if kind == "ROX" else 0)
        return v
    for _ in range(count):
        if kind in ("AS", "LS"):
            if left:
                c = 1 if v & s else 0
                nv = (v << 1) & m
                if kind == "AS" and ((nv ^ v) & s):
                    vflag = 1
            else:
                c = v & 1
                nv = (v >> 1) | (v & s if kind == "AS" else 0)
            v, x = nv, c
        elif kind == "RO":
            if left:
                c = 1 if v & s else 0
                v = ((v << 1) | c) & m
            else:
                c = v & 1
                v = (v >> 1) | (s if c else 0)
        else:  # ROX
            if left:
                c = 1 if v & s else 0
                v = ((v << 1) | x) & m
            else:
                c = v & 1
                v = (v >> 1) | (s if x else 0)
            x = c
    if kind == "RO":
        cpu.setf(n=v & s, z=v == 0, v=0, c=c)
    else:
        cpu.setf(x=x, n=v & s, z=v == 0, v=vflag, c=c)
    return v


def _execute(cpu, insn):
    mn, size, ops = insn.mnemonic, insn.size or "W", insn.operands
    f = _HANDLERS.get(mn)
    if f is None:
        for prefix, handler in _PREFIXED:
            if mn.startswith(prefix) and mn not in _NOT_PREFIXED:
                return handler(cpu, insn, mn[len(prefix):], size, ops)
        raise Stop("not run: %s" % mn)
    return f(cpu, insn, size, ops)


def _move(cpu, insn, size, ops):
    src, dst = ops
    if dst.kind in ("sr", "ccr", "usp") or src.kind in ("sr", "usp"):
        if dst.kind == "usp":
            cpu.usp = cpu.a[src.reg]
        elif src.kind == "usp":
            cpu.a[dst.reg] = cpu.usp
        else:
            cpu.put(dst, "W", cpu.get(src, "W"))
        return
    v = cpu.get(src, size)
    cpu.put(dst, size, v)
    cpu.logic(v, size)


def _movea(cpu, insn, size, ops):
    cpu.put(ops[1], size, cpu.get(ops[0], size))


def _moveq(cpu, insn, size, ops):
    v = ops[0].value & 0xFFFFFFFF
    cpu.d[ops[1].reg] = v
    cpu.logic(v, "L")


def _movem(cpu, insn, size, ops):
    n = BYTES[size]
    if ops[0].kind == "regs":                         # registers to memory
        mask, dst = ops[0].value, ops[1]
        regs = [cpu.d[i] for i in range(8)] + [cpu.a[i] for i in range(8)]
        if dst.kind == "pre":
            addr = cpu.a[dst.reg]
            for i in range(15, -1, -1):              # the mask is reversed: bit 0 is A7
                if mask & (1 << (15 - i)):
                    addr -= n
                    cpu.wr(addr, size, regs[i])
            cpu.a[dst.reg] = addr & 0xFFFFFFFF
        else:
            addr = cpu.address(dst, size)
            for i in range(16):
                if mask & (1 << i):
                    cpu.wr(addr, size, regs[i])
                    addr += n
        return
    src, mask = ops[0], ops[1].value
    addr = cpu.a[src.reg] if src.kind == "post" else cpu.address(src, size)
    for i in range(16):
        if mask & (1 << i):
            v = cpu.rd(addr, size)
            if size == "W":
                v = _sx(v, "W") & 0xFFFFFFFF
            if i < 8:
                cpu.d[i] = v
            else:
                cpu.a[i - 8] = v
            addr += n
    if src.kind == "post":
        cpu.a[src.reg] = addr & 0xFFFFFFFF


def _movep(cpu, insn, size, ops):
    n = BYTES[size]
    if ops[0].kind == "disp":
        addr = cpu.address(ops[0], size)
        v = 0
        for i in range(n):
            v = (v << 8) | cpu.rd(addr + 2 * i, "B")
        cpu.put(ops[1], size, v)
    else:
        v = cpu.get(ops[0], size)
        addr = cpu.address(ops[1], size)
        for i in range(n):
            cpu.wr(addr + 2 * i, "B", v >> (8 * (n - 1 - i)))


def _lea(cpu, insn, size, ops):
    cpu.a[ops[1].reg] = cpu.address(ops[0], "L")


def _pea(cpu, insn, size, ops):
    cpu.push(cpu.address(ops[0], "L"))


def _clr(cpu, insn, size, ops):
    cpu.put(ops[0], size, 0)
    cpu.setf(n=0, z=1, v=0, c=0)


def _tst(cpu, insn, size, ops):
    cpu.logic(cpu.get(ops[0], size), size)


def _tas(cpu, insn, size, ops):
    def f(v):
        cpu.logic(v, "B")
        return v | 0x80
    cpu.modify(ops[0], "B", f)


def _neg(cpu, insn, size, ops):
    cpu.modify(ops[0], size, lambda v: _sub(cpu, 0, v, size))


def _negx(cpu, insn, size, ops):
    cpu.modify(ops[0], size, lambda v: _sub(cpu, 0, v, size, cpu.flag(FX), keep_z=True))


def _not(cpu, insn, size, ops):
    def f(v):
        r = ~v & MASK[size]
        cpu.logic(r, size)
        return r
    cpu.modify(ops[0], size, f)


def _ext(cpu, insn, size, ops):
    reg = ops[0].reg
    if size == "W":
        r = _sx(cpu.d[reg], "B") & 0xFFFF
        cpu.d[reg] = (cpu.d[reg] & 0xFFFF0000) | r
    else:
        r = _sx(cpu.d[reg], "W") & 0xFFFFFFFF
        cpu.d[reg] = r
    cpu.logic(r, size)


def _swap(cpu, insn, size, ops):
    v = cpu.d[ops[0].reg]
    r = ((v << 16) | (v >> 16)) & 0xFFFFFFFF
    cpu.d[ops[0].reg] = r
    cpu.logic(r, "L")


def _exg(cpu, insn, size, ops):
    a, b = ops
    va, vb = cpu.get(a, "L"), cpu.get(b, "L")
    cpu.put(a, "L", vb)
    cpu.put(b, "L", va)


def _arith(name):
    def handler(cpu, insn, size, ops):
        src, dst = ops
        if dst.kind in ("ccr", "sr"):                 # ANDI/ORI/EORI to CCR/SR
            v = cpu.get(src, size)
            cur = cpu.get(dst, "W")
            r = cur & v if name == "AND" else cur | v if name == "OR" else cur ^ v
            cpu.put(dst, "W", r)
            return
        b = cpu.get(src, size)

        def f(a):
            if name == "ADD":
                return _add(cpu, a, b, size)
            if name == "SUB":
                return _sub(cpu, a, b, size)
            if name == "CMP":
                _sub(cpu, a, b, size, set_x=False)
                return None
            r = a & b if name == "AND" else a | b if name == "OR" else a ^ b
            cpu.logic(r, size)
            return r
        cpu.modify(dst, size, f)
    return handler


def _addr_arith(name):
    def handler(cpu, insn, size, ops):
        v = _sx(cpu.get(ops[0], size), size)
        a = cpu.a[ops[1].reg]
        if name == "CMPA":
            _sub(cpu, a, v & 0xFFFFFFFF, "L", set_x=False)
            return
        cpu.a[ops[1].reg] = (a + v if name == "ADDA" else a - v) & 0xFFFFFFFF
    return handler


def _quick(name):
    def handler(cpu, insn, size, ops):
        q, dst = ops[0].value, ops[1]
        if dst.kind == "A":                            # to an address register: all 32 bits, no flags
            cpu.a[dst.reg] = (cpu.a[dst.reg] + (q if name == "ADDQ" else -q)) & 0xFFFFFFFF
            return
        cpu.modify(dst, size, lambda a: _add(cpu, a, q, size) if name == "ADDQ" else _sub(cpu, a, q, size))
    return handler


def _extended(name):
    def handler(cpu, insn, size, ops):
        src, dst = ops
        b = cpu.get(src, size)
        cpu.modify(dst, size, lambda a: (_add if name == "ADDX" else _sub)(cpu, a, b, size, cpu.flag(FX), keep_z=True))
    return handler


def _cmpm(cpu, insn, size, ops):
    b = cpu.get(ops[0], size)
    a = cpu.get(ops[1], size)
    _sub(cpu, a, b, size, set_x=False)


def _mul(signed):
    def handler(cpu, insn, size, ops):
        b = cpu.get(ops[0], "W")
        a = cpu.d[ops[1].reg] & 0xFFFF
        if signed:
            r = _sx(a, "W") * _sx(b, "W")
        else:
            r = a * b
        r &= 0xFFFFFFFF
        cpu.d[ops[1].reg] = r
        cpu.logic(r, "L")
    return handler


def _div(signed):
    def handler(cpu, insn, size, ops):
        b = cpu.get(ops[0], "W")
        a = cpu.d[ops[1].reg]
        if b == 0:
            raise Stop("division by zero")
        if signed:
            sa, sb = _sx(a, "L"), _sx(b, "W")
            q = abs(sa) // abs(sb) * (1 if (sa < 0) == (sb < 0) else -1)
            rem = sa - q * sb
            if not -0x8000 <= q <= 0x7FFF:
                cpu.setf(v=1, c=0)
                return
        else:
            q, rem = a // b, a % b
            if q > 0xFFFF:
                cpu.setf(v=1, c=0)
                return
        cpu.d[ops[1].reg] = ((rem & 0xFFFF) << 16) | (q & 0xFFFF)
        cpu.logic(q & 0xFFFF, "W")
    return handler


def _bcd(name):
    def handler(cpu, insn, size, ops):
        if name == "NBCD":
            cpu.modify(ops[0], "B", lambda v: _bcd_sub(cpu, 0, v))
            return
        b = cpu.get(ops[0], "B")
        cpu.modify(ops[1], "B", lambda a: _bcd_add(cpu, a, b) if name == "ABCD" else _bcd_sub(cpu, a, b))
    return handler


def _shift_handler(cpu, insn, size, ops):
    name = insn.mnemonic
    if len(ops) == 1:                                  # memory: one bit, word
        cpu.modify(ops[0], "W", lambda v: _shift(cpu, name, v, 1, "W"))
        return
    count = ops[0].value if ops[0].kind == "quick" else cpu.d[ops[0].reg] & 63
    cpu.modify(ops[1], size, lambda v: _shift(cpu, name, v, count, size))


def _bit(name):
    def handler(cpu, insn, size, ops):
        dst = ops[1]
        width = 32 if dst.kind == "D" else 8
        n = cpu.get(ops[0], "L") % width
        sz = "L" if dst.kind == "D" else "B"

        def f(v):
            cpu.setf(z=not (v >> n) & 1)
            if name == "BTST":
                return None
            return v ^ (1 << n) if name == "BCHG" else v & ~(1 << n) if name == "BCLR" else v | (1 << n)
        cpu.modify(dst, sz, f)
    return handler


def _branch(cpu, insn, cc, size, ops):
    if cc == "SR":
        cpu.push(cpu.pc)
        cpu.pc = insn.target
    elif cpu.cond(cc):
        cpu.pc = insn.target


def _dbcc(cpu, insn, cc, size, ops):
    if cpu.cond(cc):
        return
    reg = ops[0].reg
    v = (cpu.d[reg] - 1) & 0xFFFF
    cpu.d[reg] = (cpu.d[reg] & 0xFFFF0000) | v
    if v != 0xFFFF:
        cpu.pc = insn.target


def _scc(cpu, insn, cc, size, ops):
    cpu.put(ops[0], "B", 0xFF if cpu.cond(cc) else 0)


def _jmp(cpu, insn, size, ops):
    cpu.pc = cpu.address(ops[0], "L")


def _jsr(cpu, insn, size, ops):
    t = cpu.address(ops[0], "L")
    cpu.push(cpu.pc)
    cpu.pc = t


def _rts(cpu, insn, size, ops):
    cpu.pc = cpu.pop()


def _rtr(cpu, insn, size, ops):
    cpu.sr = (cpu.sr & 0xFF00) | (cpu.pop("W") & 0x1F)
    cpu.pc = cpu.pop()


def _rte(cpu, insn, size, ops):
    cpu.sr = cpu.pop("W") & 0xA71F
    cpu.pc = cpu.pop()


def _link(cpu, insn, size, ops):
    reg = ops[0].reg
    cpu.push(cpu.a[reg])
    cpu.a[reg] = cpu.a[7]
    cpu.a[7] = (cpu.a[7] + _sx(ops[1].value, "W")) & 0xFFFFFFFF


def _unlk(cpu, insn, size, ops):
    reg = ops[0].reg
    cpu.a[7] = cpu.a[reg]
    cpu.a[reg] = cpu.pop()


def _chk(cpu, insn, size, ops):
    bound = _sx(cpu.get(ops[0], "W"), "W")
    v = _sx(cpu.d[ops[1].reg], "W")
    if v < 0 or v > bound:
        cpu.setf(n=v < 0)
        raise Stop("CHK exception")


def _stop_run(why):
    def handler(cpu, insn, size, ops):
        raise Stop(why if why else insn.mnemonic)
    return handler


def _trapv(cpu, insn, size, ops):
    if cpu.flag(FV):
        raise Stop("TRAPV exception")


_HANDLERS = {
    "MOVE": _move, "MOVEA": _movea, "MOVEQ": _moveq, "MOVEM": _movem, "MOVEP": _movep, "LEA": _lea, "PEA": _pea,
    "CLR": _clr, "TST": _tst, "TAS": _tas, "NEG": _neg, "NEGX": _negx, "NOT": _not, "EXT": _ext, "SWAP": _swap,
    "EXG": _exg, "ADD": _arith("ADD"), "ADDI": _arith("ADD"), "SUB": _arith("SUB"), "SUBI": _arith("SUB"),
    "CMP": _arith("CMP"), "CMPI": _arith("CMP"), "AND": _arith("AND"), "ANDI": _arith("AND"),
    "OR": _arith("OR"), "ORI": _arith("OR"), "EOR": _arith("EOR"), "EORI": _arith("EOR"),
    "ADDA": _addr_arith("ADDA"), "SUBA": _addr_arith("SUBA"), "CMPA": _addr_arith("CMPA"),
    "ADDQ": _quick("ADDQ"), "SUBQ": _quick("SUBQ"), "ADDX": _extended("ADDX"), "SUBX": _extended("SUBX"),
    "CMPM": _cmpm, "MULU": _mul(False), "MULS": _mul(True), "DIVU": _div(False), "DIVS": _div(True),
    "ABCD": _bcd("ABCD"), "SBCD": _bcd("SBCD"), "NBCD": _bcd("NBCD"),
    "ASL": _shift_handler, "ASR": _shift_handler, "LSL": _shift_handler, "LSR": _shift_handler,
    "ROL": _shift_handler, "ROR": _shift_handler, "ROXL": _shift_handler, "ROXR": _shift_handler,
    "BTST": _bit("BTST"), "BCHG": _bit("BCHG"), "BCLR": _bit("BCLR"), "BSET": _bit("BSET"),
    "JMP": _jmp, "JSR": _jsr, "RTS": _rts, "RTR": _rtr, "RTE": _rte, "LINK": _link, "UNLK": _unlk, "CHK": _chk,
    "NOP": lambda cpu, insn, size, ops: None, "RESET": lambda cpu, insn, size, ops: None,
    "TRAP": _stop_run("TRAP"), "ILLEGAL": _stop_run("ILLEGAL"), "STOP": _stop_run("STOP"), "TRAPV": _trapv,
}
# conditional families, by prefix: B<cc> (BRA, BSR too), DB<cc>, S<cc>
_PREFIXED = [("DB", _dbcc), ("B", _branch), ("S", _scc)]
_NOT_PREFIXED = set(_HANDLERS)


def run(bus, entry, steps=MAX_STEPS, d=None, a=None, sp=0x00FFFF00, sr=0x2700):
    """Call the function at `entry` as JSR would and run it until it returns.
    -> {end, d: [8], a: [8], sr, steps, io}"""
    cpu = CPU(bus)
    cpu.d = [(v & 0xFFFFFFFF) for v in (d or [0] * 8)]
    cpu.a = [(v & 0xFFFFFFFF) for v in (a or [0] * 8)]
    cpu.a[7] = sp
    cpu.sr = sr
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
    return {"end": end, "d": list(cpu.d), "a": list(cpu.a), "sr": cpu.sr, "steps": cpu.steps,
            "io": list(getattr(bus, "log", []))}
