"""m6502.py -- a 6502, to run functions on (Unicorn has none).

    from codebreaker.emulate import m6502
    cpu = m6502.CPU(memory)                     # memory: a 64 KB bytearray (or a Bus)
    cpu.step()                                  # one instruction
    m6502.run(memory, entry, regs)              # call a function: run to its RTS; the run record

The NMOS 6502 as documented, every one of its 151 instructions: the flags each sets (N V Z C,
with V from signed overflow), the carry as "no borrow" in subtraction, JSR pushing the address
of its own last byte and RTS adding one to what it pops, BRK pushing the address two past
itself with B set, PHP pushing B and bit 5 set, PLP and RTI ignoring both, the indirect JMP
that fetches the high byte from the start of the same page when the pointer ends a page
($xxFF), and zero-page indexing and pointers that wrap within page zero. `decimal` (default
off: the NES's 2A03 has no decimal mode) makes ADC and SBC follow D as a stock 6502 does.

What a function touches outside its own RAM is the machine's business: a Bus routes
reads and writes of chosen ranges (the NES's PPU and APU registers) to callbacks, and the run
record lists them in order -- the effects the judge compares.
"""
from codebreaker.decompile.m6502 import OPCODES

MAX_STEPS = 200000
_N, _V, _U, _B, _D, _I, _Z, _C = 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01


class Stop(Exception):
    pass


class Bus(object):
    """64 KB of memory, with ranges handed to callbacks: io[(lo, hi)] = (read(addr), write(addr, v))."""

    def __init__(self, memory=None):
        self.mem = memory if memory is not None else bytearray(0x10000)
        self.io = []
        self.log = []                                  # (kind, address, value) of every I/O access

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


class CPU(object):
    def __init__(self, memory, decimal=False):
        self.m = memory
        self.a = self.x = self.y = 0
        self.s, self.p, self.pc = 0xFD, _U | _I, 0
        self.decimal = decimal
        self.steps = 0

    # ---- memory helpers
    def rd(self, a):
        return self.m[a & 0xFFFF]

    def wr(self, a, v):
        self.m[a & 0xFFFF] = v & 0xFF

    def rd16(self, a):
        return self.rd(a) | self.rd(a + 1) << 8

    def rd16_zp(self, a):                         # a pointer in page zero wraps within it
        return self.rd(a & 0xFF) | self.rd((a + 1) & 0xFF) << 8

    def push(self, v):
        self.wr(0x100 | self.s, v)
        self.s = (self.s - 1) & 0xFF

    def pull(self):
        self.s = (self.s + 1) & 0xFF
        return self.rd(0x100 | self.s)

    def flag(self, f, on):
        self.p = (self.p | f) if on else (self.p & ~f)

    def nz(self, v):
        v &= 0xFF
        self.flag(_Z, v == 0)
        self.flag(_N, v & 0x80)
        return v

    # ---- operands
    def address(self, mode):
        pc = self.pc
        if mode == "zp":
            return self.rd(pc + 1)
        if mode == "zpx":
            return (self.rd(pc + 1) + self.x) & 0xFF
        if mode == "zpy":
            return (self.rd(pc + 1) + self.y) & 0xFF
        if mode == "abs":
            return self.rd16(pc + 1)
        if mode == "abx":
            return (self.rd16(pc + 1) + self.x) & 0xFFFF
        if mode == "aby":
            return (self.rd16(pc + 1) + self.y) & 0xFFFF
        if mode == "izx":
            return self.rd16_zp(self.rd(pc + 1) + self.x)
        if mode == "izy":
            return (self.rd16_zp(self.rd(pc + 1)) + self.y) & 0xFFFF
        if mode == "ind":                         # the page-wrap of the original chip
            p = self.rd16(pc + 1)
            return self.rd(p) | self.rd((p & 0xFF00) | ((p + 1) & 0xFF)) << 8
        raise ValueError(mode)

    def load(self, mode):
        return self.rd(self.pc + 1) if mode == "imm" else self.a if mode == "acc" else self.rd(self.address(mode))

    def store_back(self, mode, addr, v):
        if mode == "acc":
            self.a = v & 0xFF
        else:
            self.wr(addr, v)

    # ---- arithmetic
    def adc(self, v):
        c = self.p & _C
        if self.decimal and self.p & _D:
            lo = (self.a & 0x0F) + (v & 0x0F) + c
            hi = (self.a >> 4) + (v >> 4) + (lo > 9)
            binary = self.a + v + c
            self.flag(_Z, binary & 0xFF == 0)
            self.flag(_N, hi & 0x08)
            self.flag(_V, ~(self.a ^ v) & (self.a ^ (hi << 4)) & 0x80)
            if lo > 9:
                lo += 6
            if hi > 9:
                hi += 6
            self.flag(_C, hi > 15)
            self.a = ((hi << 4) | (lo & 0x0F)) & 0xFF
            return
        r = self.a + v + c
        self.flag(_C, r > 0xFF)
        self.flag(_V, ~(self.a ^ v) & (self.a ^ r) & 0x80)
        self.a = self.nz(r)

    def sbc(self, v):
        if self.decimal and self.p & _D:
            borrow = 1 - (self.p & _C)
            r = self.a - v - borrow
            lo = (self.a & 0x0F) - (v & 0x0F) - borrow
            hi = (self.a >> 4) - (v >> 4) - (lo < 0)
            self.flag(_C, r >= 0)
            self.flag(_V, (self.a ^ v) & (self.a ^ r) & 0x80)
            self.nz(r)
            if lo < 0:
                lo -= 6
            if hi < 0:
                hi -= 6
            self.a = ((hi << 4) | (lo & 0x0F)) & 0xFF
            return
        self.adc(v ^ 0xFF)

    def compare(self, reg, v):
        self.flag(_C, reg >= v)
        self.nz(reg - v)

    # ---- one instruction
    def step(self):
        op = self.rd(self.pc)
        if op not in OPCODES:
            raise Stop("undocumented opcode %02X at %04X" % (op, self.pc))
        m, mode = OPCODES[op]
        size = {"imp": 1, "acc": 1, "imm": 2, "zp": 2, "zpx": 2, "zpy": 2, "izx": 2, "izy": 2, "rel": 2,
                "abs": 3, "abx": 3, "aby": 3, "ind": 3}[mode]
        nxt = (self.pc + size) & 0xFFFF
        self.steps += 1
        if m in ("LDA", "LDX", "LDY"):
            v = self.nz(self.load(mode))
            setattr(self, m[2].lower(), v)
        elif m in ("STA", "STX", "STY"):
            self.wr(self.address(mode), getattr(self, m[2].lower()))
        elif m == "ADC":
            self.adc(self.load(mode))
        elif m == "SBC":
            self.sbc(self.load(mode))
        elif m in ("AND", "ORA", "EOR"):
            v = self.load(mode)
            self.a = self.nz(self.a & v if m == "AND" else self.a | v if m == "ORA" else self.a ^ v)
        elif m in ("CMP", "CPX", "CPY"):
            self.compare({"CMP": self.a, "CPX": self.x, "CPY": self.y}[m], self.load(mode))
        elif m == "BIT":
            v = self.load(mode)
            self.flag(_Z, self.a & v == 0)
            self.flag(_N, v & 0x80)
            self.flag(_V, v & 0x40)
        elif m in ("ASL", "LSR", "ROL", "ROR"):
            addr = None if mode == "acc" else self.address(mode)
            v = self.a if mode == "acc" else self.rd(addr)
            c = self.p & _C
            if m == "ASL":
                out, r = v & 0x80, v << 1
            elif m == "LSR":
                out, r = v & 1, v >> 1
            elif m == "ROL":
                out, r = v & 0x80, (v << 1) | c
            else:
                out, r = v & 1, (v >> 1) | (c << 7)
            self.flag(_C, out)
            self.store_back(mode, addr, self.nz(r))
        elif m in ("INC", "DEC"):
            addr = self.address(mode)
            self.wr(addr, self.nz(self.rd(addr) + (1 if m == "INC" else -1)))
        elif m in ("INX", "DEX"):
            self.x = self.nz(self.x + (1 if m == "INX" else -1))
        elif m in ("INY", "DEY"):
            self.y = self.nz(self.y + (1 if m == "INY" else -1))
        elif m in ("TAX", "TAY", "TXA", "TYA", "TSX"):
            src = {"TAX": self.a, "TAY": self.a, "TXA": self.x, "TYA": self.y, "TSX": self.s}[m]
            setattr(self, m[2].lower() if m != "TSX" else "x", self.nz(src))
        elif m == "TXS":
            self.s = self.x                            # (no flags)
        elif m in ("CLC", "SEC", "CLI", "SEI", "CLD", "SED", "CLV"):
            self.flag({"C": _C, "I": _I, "D": _D, "V": _V}[m[2]], m[0] == "S")
        elif m == "PHA":
            self.push(self.a)
        elif m == "PHP":
            self.push(self.p | _B | _U)
        elif m == "PLA":
            self.a = self.nz(self.pull())
        elif m == "PLP":
            self.p = (self.pull() & ~_B) | _U
        elif m in ("BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS"):
            bit = {"BCC": _C, "BCS": _C, "BEQ": _Z, "BNE": _Z, "BMI": _N, "BPL": _N, "BVC": _V, "BVS": _V}[m]
            want = m in ("BCS", "BEQ", "BMI", "BVS")
            if bool(self.p & bit) == want:
                d = self.rd(self.pc + 1)
                nxt = (nxt + (d - 256 if d & 0x80 else d)) & 0xFFFF
        elif m == "JMP":
            nxt = self.address(mode)
        elif m == "JSR":
            ret = (self.pc + 2) & 0xFFFF                # the address of its own last byte
            self.push(ret >> 8)
            self.push(ret & 0xFF)
            nxt = self.address("abs")
        elif m == "RTS":
            lo = self.pull()
            nxt = ((self.pull() << 8 | lo) + 1) & 0xFFFF
        elif m == "RTI":
            self.p = (self.pull() & ~_B) | _U
            lo = self.pull()
            nxt = self.pull() << 8 | lo
        elif m == "BRK":
            ret = (self.pc + 2) & 0xFFFF
            self.push(ret >> 8)
            self.push(ret & 0xFF)
            self.push(self.p | _B | _U)
            self.flag(_I, True)
            nxt = self.rd16(0xFFFE)
        elif m == "NOP":
            pass
        else:
            raise Stop("no meaning for %s" % m)
        self.pc = nxt

    @property
    def flags(self):
        return {"N": self.p >> 7 & 1, "V": self.p >> 6 & 1, "B": self.p >> 4 & 1, "D": self.p >> 3 & 1,
                "I": self.p >> 2 & 1, "Z": self.p >> 1 & 1, "C": self.p & 1}


RETURN = 0xFFF0                                     # where a called function's RTS lands


def run(memory, entry, a=0, x=0, y=0, p=_U | _I, steps=MAX_STEPS, decimal=False):
    """Call the function at `entry` as JSR would and run it until it returns.
    -> {end: "return" | "stopped: <why>" | "limit", a, x, y, p, s, steps, io: [(kind, address, value)]}
    `memory` is a Bus or a 64 KB bytearray (changed in place: what the function wrote is in it)."""
    cpu = CPU(memory, decimal)
    cpu.a, cpu.x, cpu.y, cpu.p = a & 0xFF, x & 0xFF, y & 0xFF, p
    ret = RETURN - 1
    cpu.push(ret >> 8)
    cpu.push(ret & 0xFF)
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
    return {"end": end, "a": cpu.a, "x": cpu.x, "y": cpu.y, "p": cpu.p, "s": cpu.s, "steps": cpu.steps,
            "io": list(getattr(memory, "log", []))}


def record(out):
    """run's record as the one run record (emulate/run.py): the registers (A X Y P S) its results,
    the bus's log its device accesses. What it wrote is in the memory it was given, which its
    lane reads (verify/lanes/image_lane.py)."""
    from codebreaker.emulate.run import Run
    return Run(out["end"], results={"A": out["a"], "X": out["x"], "Y": out["y"], "P": out["p"], "S": out["s"]}, io=out["io"], steps=out["steps"],
               truncated=out["end"] == "limit", endian="little", raw=out)


def recorded(memory, entry, **kw):
    """The platform's runner (codebreaker/platforms): run(...) as a Run."""
    return record(run(memory, entry, **kw))
