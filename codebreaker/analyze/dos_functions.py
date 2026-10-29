"""dos_functions.py -- where the functions of a real-mode program are, and what is code at all.

    from codebreaker.formats import dos
    from codebreaker.analyze import dos_functions
    a = dos_functions.discover(dos.read(path))
    a.functions        {linear address: Function}  .name .distance ("far"|"near") .removes .evidence .sure
    a.interrupts       [Interrupt]  every `int` reached: .address .number .ah .al .function
    a.tables           {address: number of entries}  jump tables read
    a.census()         bytes of the program by what is known about them

    python -m codebreaker dos-functions <program> [--all]

A real-mode program has no table of its functions, and code and data share its segments, so
function starts are established from what the program itself does, each with its evidence:

  entry      the entry point; a device driver's strategy and interrupt routines
  called     the target of a near or far call that is itself reached from a known function
  vector     a handler the program installs: int 21h AH=25h with a constant DS:DX, or a pair of
             stores into the interrupt table at 0000:4n
  table      a target of `call [table + index]`; the targets of `jmp [table + index]` are
             followed as part of the function that jumps
  variable   the value the file gives a pointer variable that the program calls or jumps
             through (`call [variable]`), where that decodes cleanly
  pointer    the target of a far pointer in data (a relocated segment preceded by an offset)
             into a segment known to hold code, where that decodes cleanly to a return
  prologue   `push bp / mov bp, sp` or `enter n, 0` in bytes nothing reaches, decoding cleanly
             to a return
  follows    what stands directly behind a return or jump of code already found (routines
             written in assembly lie back to back and have no prologue), where that decodes
             cleanly to a return and does not read as text

The first five follow the program's own control flow; the last three are guesses checked only
for looking like code (`sure` is False for them and for what only they call), and they are kept
apart in the census. Everything not reached stays "not classified": it may be data, or code
reached only through a computed pointer.

Whether a function is far or near, and how many bytes of arguments it removes, is read from
its own return instructions, not from how it was found.
"""
import bisect
import struct

from capstone import x86_const as X

from codebreaker.base import corpus
from codebreaker.formats import dos
from codebreaker.symbols import dos_interrupts
from codebreaker import platforms

_PREFIX = (0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65, 0x66, 0x67, 0xF0, 0xF2, 0xF3)
_HALVES = {"ax": ("al", "ah"), "bx": ("bl", "bh"), "cx": ("cl", "ch"), "dx": ("dl", "dh")}
_HALF = {h: w for w, pair in _HALVES.items() for h in pair}
_WHOLE = ("si", "di", "bp", "ds", "es")
_VOLATILE = ("al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "si", "di", "bp", "es")
_ENDS_PROGRAM = {(0x20, None), (0x27, None), (0x21, 0x4C), (0x21, 0x00), (0x21, 0x31)}
# first bytes of instructions that real-mode programs hardly use and that text and tables decode to
_ODD = (0x00, 0x27, 0x2F, 0x37, 0x3F, 0x5C, 0x62, 0x63, 0x6C, 0x6D, 0x6E, 0x6F, 0xD6, 0xF1)


class Function(object):
    def __init__(self, address, name, distance, evidence, cs, sure=True):
        self.address, self.name, self.distance, self.evidence, self.cs = address, name, distance, evidence, cs
        self.sure = sure                # found by following the program from its entry points
        self.removes = -1               # bytes of arguments its return removes; -1 not established
        self.interrupt = False          # returns with iret
        self.size = 0                   # bytes of instructions reached from its start

    def __repr__(self):
        return "Function(%s at %#x, %s, %s)" % (self.name, self.address, self.distance, self.evidence)


class Interrupt(object):
    def __init__(self, address, number, ah, al, function):
        self.address, self.number, self.ah, self.al, self.function = address, number, ah, al, function


class _Ins(object):
    __slots__ = ("size", "flow", "targets", "far", "ret", "number", "table", "variable", "odd", "cs")


class Analysis(object):
    """What discover() found. See the module docstring."""
    def __init__(self, program):
        self.program = p = program
        self.functions, self.interrupts, self.tables, self.vectors = {}, [], {}, {}
        self._md = platforms.decoder(16, detail=True)
        self._cache = {}
        self._mark = bytearray(len(p.image))       # 1 instruction start, 2 inside one, 3 jump table
        self._guess = bytearray(len(p.image))      # reached only from a guessed function start
        self._labels = set()                       # addresses jumped to
        self._sites = {}                           # address of an `int` -> its number
        self._relocs = set(p.relocations)
        self._queue = []

    # reading instructions ----------------------------------------------------------------------
    def _decode(self, va, cs):
        """One instruction, as far as control flow needs it; None when the bytes are not one."""
        if va in self._cache:
            return self._cache[va]
        p = self.program
        raw = p.image[va - p.base:va - p.base + 16] if p.inside(va) else b""
        got = None if raw[:2] in (b"\x00\x00", b"\xFF\xFF", b"") else next(self._md.disasm(raw, va & 0xFFFF, 1), None)
        ins = None
        if got is not None and p.inside(va + got.size - 1):
            ins = _Ins()
            ins.size, ins.flow, ins.targets, ins.far, ins.ret, ins.number, ins.table, ins.cs = \
                got.size, "next", (), False, None, None, None, cs
            ins.variable = None
            k = 0
            while k < got.size - 1 and raw[k] in _PREFIX:
                k += 1
            op, nxt = raw[k], va + got.size
            ins.odd = op in _ODD or 0x64 in raw[:k] or 0x65 in raw[:k] or (op == 0x0F and raw[k + 1] < 0x10)

            def near(rel):                                    # stays inside the 64 KB of its segment
                t = nxt + rel
                return t if cs <= t < cs + 0x10000 else cs + ((t - cs) & 0xFFFF)
            if op == 0xE8:
                ins.flow, ins.targets = "call", (near(struct.unpack_from("<h", raw, k + 1)[0]),)
            elif op == 0xE9:
                ins.flow, ins.targets = "jump", (near(struct.unpack_from("<h", raw, k + 1)[0]),)
            elif op == 0xEB:
                ins.flow, ins.targets = "jump", (near(struct.unpack_from("<b", raw, k + 1)[0]),)
            elif 0x70 <= op <= 0x7F or 0xE0 <= op <= 0xE3:
                ins.flow, ins.targets = "branch", (near(struct.unpack_from("<b", raw, k + 1)[0]),)
            elif op == 0x0F and 0x80 <= raw[k + 1] <= 0x8F:
                ins.flow, ins.targets = "branch", (near(struct.unpack_from("<h", raw, k + 2)[0]),)
            elif op in (0x9A, 0xEA):
                off, seg = struct.unpack_from("<HH", raw, k + 1)
                ins.flow, ins.far, ins.targets = ("call" if op == 0x9A else "jump"), True, (seg * 16 + off,)
                if self._relocs and va + k + 3 not in self._relocs:
                    ins.targets = ()                          # an absolute segment: not this program
            elif op in (0xC3, 0xC2, 0xCB, 0xCA, 0xCF):
                ins.flow = "stop"
                ins.ret = ("iret" if op == 0xCF else "near" if op in (0xC3, 0xC2) else "far",
                           struct.unpack_from("<H", raw, k + 1)[0] if op in (0xC2, 0xCA) else 0)
            elif op in (0xCD, 0xCC, 0xCE):
                ins.number = raw[k + 1] if op == 0xCD else 3 if op == 0xCC else 4
            elif op == 0xF4:
                ins.flow = "stop"
            elif op == 0xFF and (raw[k + 1] >> 3) & 7 in (2, 3, 4, 5):
                kind = (raw[k + 1] >> 3) & 7
                ins.flow = "call" if kind in (2, 3) else "jump"
                if kind in (2, 4) and len(got.operands) == 1 and got.operands[0].type == X.X86_OP_MEM:
                    m = got.operands[0].mem
                    indexed = m.index or m.base in (X.X86_REG_BX, X.X86_REG_SI, X.X86_REG_DI)
                    if indexed and m.disp and m.segment in (0, X.X86_REG_CS, X.X86_REG_DS):
                        ins.table = (cs if m.segment == X.X86_REG_CS else p.ds * 16) + (m.disp & 0xFFFF)
                if len(got.operands) == 1 and got.operands[0].type == X.X86_OP_MEM:
                    m = got.operands[0].mem                   # through one pointer variable
                    if not m.index and not m.base and m.segment in (0, X.X86_REG_CS, X.X86_REG_DS):
                        ins.variable = (cs if m.segment == X.X86_REG_CS else p.ds * 16) + (m.disp & 0xFFFF)
                        ins.far = kind in (3, 5)
        self._cache[va] = ins
        return ins

    def _looks_like_code(self, va, cs, limit=1500, strict=False):
        """Does decoding from `va` run cleanly into a return, a known instruction or an indirect
        jump -- without meeting bytes that are no instruction, the middle of a known one, or a
        jump out of the program? `strict` also refuses instructions real-mode programs hardly
        use, a start that reads as text, and an end in an indirect jump."""
        base, seen, work, ended = self.program.base, set(), [va], False
        if strict and all(0x20 <= c < 0x7F for c in self.program.image[va - base:va - base + 8]):
            return False
        while work and len(seen) < limit:
            a = work.pop()
            while a not in seen:
                ins = self._decode(a, cs)
                if ins is None or self._mark[a - base] in (2, 3) or (strict and ins.odd):
                    return False
                seen.add(a)
                if self._mark[a - base] == 1:
                    ended = True
                    break
                if ins.flow == "stop" or (ins.number, None) in _ENDS_PROGRAM:
                    ended = ended or ins.ret is not None or ins.number is not None
                    break
                if ins.flow in ("jump", "branch") and not ins.far:
                    for t in ins.targets:
                        if not self.program.inside(t):
                            return False
                        work.append(t)
                if ins.flow == "jump":
                    ended = ended or (not ins.targets and not strict)
                    break
                a += ins.size
        return ended and len(seen) >= (2 if strict else 3)

    # following a function -----------------------------------------------------------------------
    def _add(self, va, distance, evidence, cs, name=None, sure=True):
        if va in self.functions or not self.program.inside(va) or self._mark[va - self.program.base] in (2, 3):
            return None
        f = Function(va, name or "sub_%05X" % (va - self.program.base), distance, evidence, cs, sure)
        self.functions[va] = f
        self._queue.append(f)
        return f

    def _table(self, at, cs):
        """The entries of a jump table at `at`: words that are offsets of code in segment `cs`,
        up to the first that is not, or to where the table would run into its own targets."""
        p, mark, out, stop = self.program, self._mark, [], at + 512
        while at + 2 <= stop and p.inside(at + 1) and mark[at - p.base] in (0, 3) and mark[at + 1 - p.base] in (0, 3):
            t = cs + p.word(at)
            known = t in self._labels or t in self.functions or (p.inside(t) and mark[t - p.base] == 1)
            if not p.inside(t) or t in (cs, at) or not (known or self._looks_like_code(t, cs)):
                break
            out.append(t)
            stop = min(stop, t) if t > at else stop
            at += 2
        return out

    def _follow(self, f):
        """Mark everything reached from one function's start; note its returns and its calls."""
        p, mark, guess = self.program, self._mark, not f.sure
        seen, work, returns = set(), [f.address], []
        while work:
            a = work.pop()
            regs = {}
            while a not in seen and p.inside(a):
                ins = self._decode(a, f.cs)
                i = a - p.base
                if ins is None or mark[i] in (2, 3):
                    break                                     # runs into data, or into the middle of code
                seen.add(a)
                if mark[i] == 0:
                    mark[i] = 1
                    for j in range(i + 1, i + ins.size):
                        mark[j] = 2
                    if guess:
                        self._guess[i:i + ins.size] = b"\x01" * ins.size
                elif not guess and self._guess[i]:
                    self._guess[i:i + ins.size] = bytes(ins.size)
                ends = False
                if ins.number is not None:
                    self._sites[a] = ins.number
                    ds, off = regs.get("ds"), self._word(regs, "dx")
                    if ins.number == 0x21 and regs.get("ah") == 0x25 and None not in (regs.get("al"), ds, off):
                        self.vectors.setdefault(ds * 16 + off, regs["al"])
                    ends = (ins.number, regs.get("ah") if ins.number == 0x21 else None) in _ENDS_PROGRAM
                vector = self._constants(a, ins, regs)
                if vector:
                    self.vectors.setdefault(*vector)
                if ins.ret:
                    returns.append(ins.ret)
                if ins.table is not None and ins.table not in self.tables and p.inside(ins.table):
                    targets = self._table(ins.table, f.cs)
                    if len(targets) >= 2:
                        self.tables[ins.table] = len(targets)
                        for j in range(ins.table - p.base, ins.table - p.base + 2 * len(targets)):
                            mark[j] = 3
                        if ins.flow == "jump":
                            self._labels.update(targets)
                            work.extend(targets)
                        else:
                            for t in targets:
                                self._add(t, "near", "table", f.cs, sure=f.sure)
                if ins.variable is not None and p.inside(ins.variable + (3 if ins.far else 1)) \
                        and mark[ins.variable - p.base] == 0:
                    at = ins.variable
                    if not ins.far:
                        t, cs = f.cs + p.word(at), f.cs
                    else:
                        cs = p.word(at + 2) * 16
                        t = cs + p.word(at) if not self._relocs or at + 2 in self._relocs else -1
                    if t != cs and p.inside(t) and (mark[t - p.base] == 1 or self._looks_like_code(t, cs)):
                        self._add(t, "far" if ins.far else "near", "variable", cs, sure=f.sure)
                if ins.flow == "call":
                    for t in ins.targets:
                        if t != a + ins.size:                 # `call $+3` only fetches the address
                            self._add(t, "far" if ins.far else "near", "called",
                                      self._far_cs(a, ins) if ins.far else f.cs, sure=f.sure)
                elif ins.flow in ("jump", "branch"):
                    for t in ins.targets:
                        if ins.far:
                            self._add(t, "far", "called", self._far_cs(a, ins), sure=f.sure)
                        elif p.inside(t) and t not in self.functions:
                            self._labels.add(t)
                            work.append(t)
                if ins.flow in ("stop", "jump") or ends:
                    break
                a += ins.size
        f.size = sum(self._cache[a].size for a in seen)
        far = sum(1 for k, _n in returns if k != "near")
        near = len(returns) - far
        if far != near:
            f.distance = "far" if far > near else "near"
        f.interrupt = any(k == "iret" for k, _n in returns) and not near
        removed = {n for k, n in returns if (k != "near") == (f.distance == "far")}
        f.removes = removed.pop() if len(removed) == 1 else -1

    def _far_cs(self, va, ins):
        """The segment base of a far call or jump's target: the segment word of the instruction."""
        raw = self.program.image[va - self.program.base:va - self.program.base + ins.size]
        return struct.unpack_from("<H", raw, ins.size - 2)[0] * 16

    # constants in registers, along a straight run of instructions -----------------------------------
    @staticmethod
    def _word(regs, reg):
        lo, hi = regs.get(_HALVES[reg][0]), regs.get(_HALVES[reg][1])
        return None if lo is None or hi is None else hi << 8 | lo

    def _constants(self, va, ins, regs):
        """Update `regs` ({register: constant}) across the instruction at `va`. Only the simple
        ways a program loads a service number or a pointer are understood; anything else that
        writes a register forgets it. -> (handler address, interrupt number) when the instruction
        completes a pair of stores into the interrupt table at 0000:4n, else None."""
        p = self.program
        raw = p.image[va - p.base:va - p.base + ins.size]
        got = next(self._md.disasm(raw, va & 0xFFFF, 1))
        name, ops, m = got.reg_name, got.operands, got.mnemonic

        def put(reg, value):
            if reg in _HALVES:
                lo, hi = _HALVES[reg]
                regs[lo], regs[hi] = (None, None) if value is None else (value & 0xFF, value >> 8 & 0xFF)
            elif reg in _WHOLE or reg in _HALF:
                regs[reg] = value

        def get(reg):
            if reg in _HALVES:
                return self._word(regs, reg)
            return ins.cs >> 4 if reg == "cs" else regs.get(reg)

        if m == "push" and len(ops) == 1:
            regs["pushed"] = get(name(ops[0].reg)) if ops[0].type == X.X86_OP_REG else \
                ops[0].imm & 0xFFFF if ops[0].type == X.X86_OP_IMM else None
            return None
        pushed = regs.pop("pushed", None)
        if m == "pop" and len(ops) == 1 and ops[0].type == X.X86_OP_REG:
            put(name(ops[0].reg), pushed)
            return None
        if ins.flow == "call" or ins.number is not None:
            for r in _VOLATILE:
                regs.pop(r, None)
            return None
        if len(ops) == 2 and ops[0].type == X.X86_OP_REG:
            dst, src = name(ops[0].reg), ops[1]
            if m == "mov" and src.type == X.X86_OP_IMM:
                return put(dst, src.imm & (0xFF if ops[0].size == 1 else 0xFFFF))
            if m == "mov" and src.type == X.X86_OP_REG:
                return put(dst, get(name(src.reg)))
            if m == "lea" and not src.mem.base and not src.mem.index:
                return put(dst, src.mem.disp & 0xFFFF)
            if m in ("xor", "sub") and src.type == X.X86_OP_REG and src.reg == ops[0].reg:
                return put(dst, 0)
        if m == "mov" and len(ops) == 2 and ops[0].type == X.X86_OP_MEM and ops[0].size == 2 \
                and not ops[0].mem.base and not ops[0].mem.index:
            seg = "es" if ops[0].mem.segment == X.X86_REG_ES else "ds" if not ops[0].mem.segment else None
            at = ops[0].mem.disp & 0xFFFF
            if seg and regs.get(seg) == 0 and at < 0x400:
                value = ops[1].imm & 0xFFFF if ops[1].type == X.X86_OP_IMM else get(name(ops[1].reg))
                half = regs.setdefault("table", {})
                half[at] = value
                if at % 4 == 2 and half.get(at - 2) is not None and value is not None:
                    return value * 16 + half[at - 2], at // 4
            return None
        for r in got.regs_access()[1]:
            r = name(r) or ""
            put(r[1:] if r[:1] == "e" and r[1:] in _HALVES else r, None)
        return None

    # the whole program ----------------------------------------------------------------------------
    def run(self):
        p = self.program
        for va, name, distance in p.entries:
            self._add(va, distance, "entry", p.segments.get(va, va >> 4 & 0xF000) * 16, name)
        self._drain()
        self._guesses()
        self._name_interrupts()
        return self

    def _drain(self):
        while self._queue:
            while self._queue:
                self._follow(self._queue.pop())
            for va, number in sorted(self.vectors.items()):      # handlers the code just read installs
                if va not in self.functions and self.program.inside(va):
                    cs = self._segment_of(va)
                    if self._looks_like_code(va, cs):
                        self._add(va, "far", "vector", cs, "int_%02X_handler_%05X" % (number, va - self.program.base))

    def _segment_of(self, va):
        """The segment base code at `va` runs with: that of the nearest function before it."""
        best = max((a for a in self.functions if a <= va), default=None)
        return self.functions[best].cs if best is not None and va - self.functions[best].cs < 0x10000 else va & ~0xF

    def _guesses(self):
        """Function starts nothing was seen to call. See `pointer` and `prologue` above."""
        p, tried = self.program, set()
        while True:
            before = len(self.functions)
            bases = {f.cs for f in self.functions.values()}
            for site in p.relocations:                            # far pointers in data
                i = site - p.base
                if i < 2 or self._mark[i] in (1, 2) or self._mark[i - 2] in (1, 2):
                    continue
                seg, off = p.word(site), p.word(site - 2)
                t = seg * 16 + off
                if seg * 16 in bases and p.inside(t) and t not in self.functions and self._mark[t - p.base] == 0 \
                        and self._looks_like_code(t, seg * 16):
                    self._add(t, "far", "pointer", seg * 16, sure=False)
            self._drain()
            for lo, n, _label in p.spans:                         # prologues in bytes nothing reaches
                i, end = lo - p.base, lo - p.base + n
                while True:
                    j = min((x for x in (p.image.find(b"\x55\x8B\xEC", i, end), p.image.find(b"\x55\x89\xE5", i, end),
                                         p.image.find(b"\xC8", i, end)) if x >= 0), default=-1)
                    if j < 0:
                        break
                    i = j + 1
                    va = p.base + j
                    if self._mark[j] or (p.image[j] == 0xC8 and (p.image[j + 3:j + 4] != b"\x00" or p.image[j + 1] & 1
                                                                 or p.image[j + 2] >= 0x10)):
                        continue                              # `enter`: an even frame under 4 KB, no nesting
                    cs = self._segment_of(va)
                    if self._looks_like_code(va, cs):
                        self._add(va, "near", "prologue", cs, sure=False)
                        self._drain()
            self._after_returns(tried)
            if len(self.functions) == before:
                break

    def _after_returns(self, tried):
        """`follows`: what stands directly behind a return or jump of code already found."""
        p, mark = self.program, self._mark
        while True:
            ends = sorted(va + ins.size for va, ins in self._cache.items()
                          if ins is not None and ins.flow in ("stop", "jump") and mark[va - p.base] == 1)
            found = False
            for va in ends:
                stop = va + 4
                while va < stop and p.inside(va) and mark[va - p.base] == 0 and p.image[va - p.base] in (0x90, 0xCC, 0):
                    va += 1                                   # padding between routines
                if va in tried or not p.inside(va) or mark[va - p.base] or va in self.functions:
                    continue
                tried.add(va)
                cs = self._segment_of(va)
                if self._looks_like_code(va, cs, strict=True):
                    self._add(va, "near", "follows", cs, sure=False)
                    self._drain()
                    found = True
            if not found:
                break

    def _name_interrupts(self):
        """The interrupt calls inside the functions found, with the service number where a
        constant was loaded in the same straight run of instructions. A run is cut at every
        address something jumps to, at every call and at every function start, so a number is
        only reported where no other path can bring a different one."""
        p, starts = self.program, sorted(self.functions)
        before = {}                                           # instruction -> the one that falls into it
        for va, ins in self._cache.items():
            if ins is not None and ins.flow == "next" and ins.number is None and self._mark[va - p.base] == 1:
                before[va + ins.size] = va
        for va in sorted(self._sites):
            run, a = [], va
            while a not in self._labels and a not in self.functions and a in before and len(run) < 64:
                a = before[a]
                run.append(a)
            regs = {}
            for b in reversed(run):
                self._constants(b, self._cache[b], regs)
            k = bisect.bisect_right(starts, va) - 1
            self.interrupts.append(Interrupt(va, self._sites[va], regs.get("ah"), regs.get("al"),
                                             self.functions[starts[k]] if k >= 0 else None))

    def census(self):
        """{"code": n, "guessed code": n, "jump tables": n, "not classified": n, "total": n} --
        bytes of the program's own spans. "guessed code" is reached only from `pointer` and
        `prologue` and `follows` starts."""
        p, out = self.program, {"code": 0, "guessed code": 0, "jump tables": 0, "not classified": 0, "total": 0}
        for lo, n, _label in p.spans:
            for i in range(lo - p.base, lo - p.base + n):
                m = self._mark[i]
                out["total"] += 1
                out["jump tables" if m == 3 else "not classified" if m == 0 else
                    "guessed code" if self._guess[i] else "code"] += 1
        return out


def discover(program):
    """Find the functions of a Program (formats.dos.read). -> Analysis"""
    return Analysis(program).run()


def main(argv):
    """python -m codebreaker dos-functions <program> [--all]

    The functions found in a real-mode program (a DOS .EXE or .COM, a device driver, IO.SYS):
    how many by which evidence, how much of the program is code, and the interrupt services it
    calls. --all lists every function."""
    if not argv:
        print(main.__doc__)
        return 2
    a = discover(dos.read(corpus.resolve(argv[0])))
    p = a.program
    stored = ", stored with %s (%s)" % (p.packer, "unpacked" if p.unpacked else "NOT unpacked") if p.packer else ""
    print("%s: %s%s, %d bytes loaded" % (p.name, p.kind, stored, sum(n for _a, n, _l in p.spans)))
    by = {}
    for f in a.functions.values():
        by[f.evidence] = by.get(f.evidence, 0) + 1
    print("%d functions: %s; %d of them rest on a guess" % (len(a.functions), ", ".join(
        "%d %s" % (n, k) for k, n in sorted(by.items())), sum(1 for f in a.functions.values() if not f.sure)))
    print("bytes: " + ", ".join("%s %d" % kv for kv in a.census().items()))
    named = [dos_interrupts.name(i.number, i.ah, i.al) for i in a.interrupts]
    print("%d interrupt calls, %d with the service named" % (len(named), sum(1 for n in named if n.exact)))
    for note in p.notes:
        print("note: " + note)
    if "--all" in argv:
        for va in sorted(a.functions):
            f = a.functions[va]
            print("  %05X %-26s %-4s removes %2d  %5d bytes  %s%s" % (va, f.name, f.distance, f.removes, f.size,
                                                                    f.evidence, "" if f.sure else " (guess)"))
    return 0
