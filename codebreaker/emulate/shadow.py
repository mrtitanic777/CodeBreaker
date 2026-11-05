"""shadow.py -- which values of a run are made from its inputs, and from which: a run's symbolic
shadow, kept up beside the emulator instruction by instruction.

    sh = shadow.Shadow16(lifter, segbase, inputs)     # a 16-bit run in Unicorn (windows16)
    sh = shadow.Shadow32(lifter, inputs)              # a flat 32-bit run in Unicorn (emulator.trace_function)
    sh = shadow.ShadowLE(lifter, inputs)              # a driver's flat ring-0 run in Unicorn (driver.run)
    sh = shadow.ShadowPPC(lifter, inputs)             # a PowerPC run in CodeBreaker's interpreter (powerpc.run)
    ... the runner calls sh.enter(), sh.step() before each instruction, sh.after_call() ...
    sh.path        [(site, taken, condition, kind)]  the conditions the run's path went by: each
                   a condition (emulate/symbolic.py) and whether it held -- a jump's ("jump": taken or not), one
                   inside an instruction's p-code ("inner": a string instruction's count), or an
                   address or selector held at its value ("pin")
    sh.trail       [(site, taken)]                   every conditional jump, in order
    sh.vars        {name: (kind, where, bits, value)} the inputs the conditions are over
    sh.lost        why the conditions may not be all there is (empty: they are)

The run itself is the emulator's: nothing here changes it. Before each instruction runs, its
p-code (emulate/pcode.py, the decompiler's own specification) is evaluated twice over at once --
concretely, from the registers and memory as they are at that moment, and symbolically
(emulate/symbolic.py, for the solver) for the values that come from an input. Only the symbolic
half is kept: which register bytes and memory bytes hold an expression over the inputs. A
conditional jump whose condition is such an expression is a condition of the path; its
direction is the one the emulator then takes (the next instruction says which).

What is particular to a processor and an emulator is a View (the platform's cpu slot: its
register names as the specification has them, its word, its byte order; and how to read the
emulator's registers and memory without disturbing it): UnicornX86 for both x86 runners,
PowerPCView for the interpreter. Everything else -- the p-code's evaluation, the inputs, the
conditions -- is the same for all of them.

The inputs are the function's argument words (a0, a1 ... : in memory where they were pushed, or
in the registers they are passed in), any byte of memory the `inputs(address)` test names (the
data segment, the scratch memory pointers go to) not yet written by the run (m<address>), and
what a call the environment does not model returns (ret). An address made from an input is held
at the value it had (a "pin" in the path: the pointer as given), and so is a selector; a value
made from an input that leaves through something not followed (a call the environment answers
from its arguments, an interrupt, an operation without a meaning here) makes the path's
conditions possibly short of all there are, and says so in `lost`."""
from codebreaker.emulate import symbolic as S

_CAP_PATH = 400                     # conditions kept per run; past it the run is followed no further
_FLAGS = {"CF": 0, "PF": 2, "AF": 4, "ZF": 6, "SF": 7, "TF": 8, "IF": 9, "DF": 10, "OF": 11}


def _mask(size):
    return (1 << (8 * size)) - 1


def _sx(v, size):
    bits = 8 * size
    return v - (1 << bits) if v >> (bits - 1) & 1 else v


def _bv(c, s, size):
    return s if s is not None else S.const(c, 8 * size)


def _join(entries, conc, size):
    """A value of `size` bytes from its bytes: each (expr, k) -- byte k of expr -- or None for a
    concrete byte (of `conc`). All of one expression in order: that expression."""
    first = entries[0]
    if first is not None and all(e is not None and e[0] is first[0] and e[1] == first[1] + k
                                 for k, e in enumerate(entries)):
        return S.extract(first[0], 8 * first[1], 8 * size)
    out = None
    for k in range(size):
        e = entries[k]
        byte = S.extract(e[0], 8 * e[1], 8) if e is not None else S.const((conc >> (8 * k)) & 0xFF, 8)
        out = byte if out is None else S.concat(byte, out)
    return out


class _Stop(Exception):
    pass


def mode(cpu):
    """The lift mode (emulate/pcode.py) for a platform's cpu slot (codebreaker/platforms: its
    SLEIGH language, bits, byte order), or None where there is no lift: x86 16-bit (its segments
    read from the run), x86 32-bit flat, PowerPC big-endian."""
    lang = str(cpu)
    if lang.startswith("x86:LE:16"):
        return "x86-16-real"
    if lang.startswith("x86:LE:32"):
        return "x86-32-flat"
    if lang.startswith("PowerPC:BE"):
        return "ppc-be-64"
    return None


class UnicornX86(object):
    """The view of an x86 run in Unicorn (emulate/windows16.py, emulate/emulator.py): the
    specification's register names are Unicorn's, its flags bits of EFLAGS; memory little-endian.
    `bits`: 16 (a jump's target an offset in its segment) or 32 (flat)."""
    endian = "little"
    addresses = 0xFFFFFFFF                  # (what of an address reaches memory)

    def __init__(self, bits=16):
        from unicorn import x86_const as X
        self.X = X
        self.mask = 0xFFFF if bits == 16 else 0xFFFFFFFF
        self._ids = {}

    def known(self, name):
        return name in _FLAGS or getattr(self.X, "UC_X86_REG_" + name, None) is not None

    def reg(self, uc, name):
        bit = _FLAGS.get(name)
        if bit is not None:
            return (uc.reg_read(self.X.UC_X86_REG_EFLAGS) >> bit) & 1
        r = self._ids.get(name)
        if r is None:
            r = self._ids[name] = getattr(self.X, "UC_X86_REG_" + name)
        return uc.reg_read(r)

    def code(self, uc, address):
        try:
            return bytes(uc.mem_read(address, 16))
        except Exception:
            return bytes(uc.mem_read(address, (address | 0xFFF) + 1 - address))

    def mem(self, uc, at, size):
        return bytes(uc.mem_read(at, size))


class PowerPCView(object):
    """The view of a run in CodeBreaker's PowerPC interpreter (emulate/powerpc.py): r0-r31, LR,
    CTR, the condition register's eight fields, XER's bits, f0-f31 as the interpreter keeps them;
    memory big-endian, read without making pages (a page the run has not touched reads as the
    interpreter would give it)."""
    endian = "big"
    mask = 0xFFFFFFFF
    addresses = 0xFFFFFFFF                  # (64-bit registers, 32-bit addresses: the interpreter's M32)

    def __init__(self):
        from codebreaker.emulate import powerpc
        self.page = powerpc.PAGE

    def known(self, name):
        return name in ("LR", "CTR", "XER", "xer_so", "xer_ov", "xer_ca") or \
            (name[:1] in "rf" and name[1:].isdigit()) or (name[:2] == "cr" and name[2:].isdigit())

    def reg(self, m, name):
        c = name[0]
        if c == "r":
            return m.r[int(name[1:])]
        if c == "f":
            return m.f[int(name[1:])]
        if name[:2] == "cr":
            return (m.cr >> (28 - 4 * int(name[2:]))) & 0xF
        if name == "LR":
            return m.lr
        if name == "CTR":
            return m.ctr
        if name == "xer_ca":
            return m.ca
        if name == "XER":
            return m.ca << 29
        return 0                                             # (SO, OV: the interpreter never sets them)

    def mem(self, m, at, size):
        at &= 0xFFFFFFFF
        off = at & (self.page - 1)
        if off + size <= self.page:                          # (within one page: one slice)
            page = m.pages.get(at >> 12)
            if page is None:
                page = m.shared.get(at >> 12)
            return bytes(page[off:off + size]) if page is not None else bytes(size)
        return b"".join(self.mem(m, at + i, 1) for i in range(size))

    def code(self, m, address):
        return self.mem(m, address, 4)


class Shadow(object):
    """The shadow of one run. `lifter`: emulate/pcode.py's for the processor; `view`: how the
    emulator's registers and memory are read (UnicornX86, PowerPCView); `inputs(address)`: whether
    a byte of memory is one of the run's inputs until written; `span`: (low, high) addresses of the
    function's own code (its jumps are the path's); `segbase(machine, selector)`: the base a
    selector names (x86; a flat program's are all 0). `result`: [(register, byte of it, byte of
    `ret`)] -- where an unmodelled call's result lands; `clobbered`: the registers such a call
    sets (anything else it leaves as it was); `ret_bits`: how wide `ret` is."""

    def __init__(self, lifter, view, inputs, span=None, segbase=None, result=(), clobbered=(), ret_bits=32):
        self.lifter, self.view, self.inputs, self.span = lifter, view, inputs, span
        self._segbase = segbase or (lambda m, sel: 0)
        self.result, self.clobbered, self.ret_bits = tuple(result), tuple(clobbered), ret_bits
        self.little = view.endian == "little"
        self.mask = view.mask
        self.bases = {}                     # selector -> base, as read this run (until a call)
        self.reg = {}                       # register byte -> (expr, k)
        self.mem = {}                       # byte of memory -> (expr, k)
        self.touched = set()                # bytes the run wrote (no longer inputs)
        self.vars = {}
        self.path, self.trail = [], []
        self.lost = set()
        self.pins = 0
        self.steps = 0
        self.off = False
        self.pending = None                 # (site, target, fallthrough, entry of path or None)
        self.lifted = {}
        self._sym = {}
        # register bytes -> (register name, shift): byte k of a register, from its low end on a
        # little-endian processor, its high end on a big-endian one
        self.places = {}
        for name, (off, size) in lifter.regs.items():
            if not view.known(name):
                continue
            for k in range(size):
                self.places[off + k] = (name, 8 * (k if self.little else size - 1 - k))
        self.named = {n: o for n, (o, _s) in lifter.regs.items()}

    def _bytes(self, at, size):
        """The addresses of a value's bytes, its least significant first."""
        return range(at, at + size) if self.little else range(at + size - 1, at - 1, -1)

    def segbase(self, m, sel):
        """The base `sel` names (read once, until the environment next answers a call: an
        allocation makes descriptors)."""
        got = self.bases.get(sel)
        if got is None:
            got = self.bases[sel] = self._segbase(m, sel)
        return got

    # --- inputs ---------------------------------------------------------------------------------
    def var(self, name, bits, kind, where, value):
        if name not in self.vars:
            self.vars[name] = (kind, where, bits, value)
            self._sym[name] = S.var(name, bits)
        return self._sym[name]

    def enter(self, places, bits=16):
        """The run is about to start: argument k, `bits` wide, is (where, value) -- where a
        memory address, or a register's name; or (where, value, k) to name it argument k (an
        input named `k` itself, of kind "reg", when k is a string: "ecx")."""
        for n, item in enumerate(places):
            at, value = item[0], item[1]
            k = item[2] if len(item) > 2 else n
            v = self.var("a%d" % k, bits, "arg", k, value) if isinstance(k, int) else \
                self.var(k, bits, "reg", k, value)
            if isinstance(at, str):
                off, size = self.lifter.regs[at]
                for j, b in enumerate(self._bytes(off, size)):
                    if j < bits // 8:
                        self.reg[b] = (v, j)
            else:
                for j, b in enumerate(self._bytes(at, bits // 8)):
                    self.mem[b] = (v, j)

    def forget(self, names):
        """Registers the environment set (not an instruction): no longer made from anything."""
        if not self.off:
            self._safely(self._forget_regs, names)

    def _forget_regs(self, names):
        for n in names:
            off = self.named.get(n)
            if off is None:
                continue
            size = self.lifter.regs[n][1]
            for b in range(off, off + size):
                self.reg.pop(b, None)

    # (what the runner calls: nothing here may change the run -- whatever goes wrong in the
    # shadow ends the shadow, never the run: a selector no descriptor names, read for its base,
    # was an exception in Unicorn's hook, and Unicorn stopped LZCOPY there)
    def after_call(self, *a, **k):
        self._safely(self._after_call, *a, **k)

    def step(self, m, address, cs_base=0):
        """Before the instruction at `address` (its segment based at `cs_base`) runs."""
        if not self.off:
            self._safely(self._step, m, address, cs_base)

    def _safely(self, f, *a, **k):
        try:
            f(*a, **k)
        except Exception as e:
            self.lost.add("shadow: %s" % type(e).__name__)
            self.off = True

    def _after_call(self, m, symbolic_result, value, args_at=None, nargs=0, why=None, word=2):
        """A call the environment answered (an imported function, or a callee not run): the
        `clobbered` registers were set, the result ones to its result -- `ret`, an input, when
        `symbolic_result`. Arguments made from the inputs (`nargs` words of `word` bytes from
        `args_at`), when the result is made from them (`why`): the path may be short of what it
        depends on."""
        self.pending = None
        self.bases.clear()
        if self.off:
            return
        if why and args_at is not None:
            if any(b in self.mem for b in range(args_at, args_at + word * nargs)):
                self.lost.add(why)
        self._forget_regs(self.clobbered)
        if symbolic_result:
            r = self.var("ret", self.ret_bits, "ret", None, value & ((1 << self.ret_bits) - 1))
            for name, k, j in self.result:
                off, size = self.lifter.regs[name]
                self.reg[off + k if self.little else off + size - 1 - k] = (r, j)

    def written(self, at, size):
        """Memory the environment wrote (not an instruction): no longer an input, nor made from one."""
        for b in range(at, at + size):
            self.mem.pop(b, None)
            self.touched.add(b)

    # --- one instruction ------------------------------------------------------------------------
    def _step(self, m, address, cs_base):
        pend = self.pending
        if pend is not None:
            self.pending = None
            site, target, fall, entry, guess = pend
            if address in (target, fall):
                went = address == target
                self.trail.append((site, went))
                if entry is not None:
                    if went != guess:                        # (the specification and the processor
                        self.lost.add("disagree")            # differ here: the processor's way kept)
                    self.path.append((site, went, entry, "jump"))
        self.steps += 1
        try:
            raw = self.view.code(m, address)
        except Exception:
            return
        off = address - cs_base
        ins = self.lifted.get((raw, off))
        if ins is None:
            ins = self.lifter.lift(raw, off)
            if ins is None:
                self._unknown("undecoded")
                return
            self.lifted[(raw, off)] = ins
        if not ins.full and not (self.reg and any(b in self.reg for b in ins.reads)):
            if self.reg:
                for b in ins.writes:
                    self.reg.pop(b, None)
            return
        try:
            self._run(m, address, cs_base, ins)
        except _Stop:
            pass
        if len(self.path) >= _CAP_PATH:
            self.lost.add("path-capped")
            self.off = True

    def _unknown(self, why):
        """An instruction not followed: what it writes is no longer known to be made from anything."""
        self.lost.add(why)
        self.reg.clear()

    def _run(self, uc, address, cs_base, ins):
        local_reg, local_mem, uniq, conc = {}, {}, {}, {}
        regs = self.reg
        view, order, little = self.view, self._bytes, self.little
        amask = view.addresses

        def concrete_reg(b):
            place = self.places.get(b)
            if place is None:
                return 0
            r, shift = place
            if r not in conc:
                conc[r] = view.reg(uc, r)
            return (conc[r] >> shift) & 0xFF

        def read(vn):
            space, off, size = vn
            if space == "c":
                return off & _mask(size), None
            if space == "u":
                got = uniq.get(off)
                if got is not None and got[0] == size:
                    return got[1], got[2]
                if got is not None and got[0] > size:        # (the low part of a wider one)
                    return got[1] & _mask(size), (S.extract(got[2], 0, 8 * size) if got[2] is not None else None)
                return 0, None
            if space == "r":
                c, ents, any_s = 0, [], False
                for k, b in enumerate(order(off, size)):
                    if b in local_reg:
                        cb, e = local_reg[b]
                    else:
                        cb, e = concrete_reg(b), regs.get(b)
                    c |= cb << (8 * k)
                    ents.append(e)
                    any_s = any_s or e is not None
                return c, (_join(ents, c, size) if any_s else None)
            return load(off, size, None)                     # (ram named directly)

        def write(vn, c, s):
            space, off, size = vn
            c &= _mask(size)
            if space == "u":
                uniq[off] = (size, c, s)
            elif space == "r":
                for k, b in enumerate(order(off, size)):
                    local_reg[b] = ((c >> (8 * k)) & 0xFF, (s, k) if s is not None else None)
            elif space == "m":
                store(off, size, c, s)

        def pin(c, s, bits):
            if s is not None:
                self.path.append((address, True, S.compare("eq", s, S.const(c, S.bits(s))), "pin"))
                self.pins += 1

        def load(at, size, _s):
            try:
                data = view.mem(uc, at, size)
            except Exception:
                data = bytes(size)
            c = int.from_bytes(data, view.endian)
            ents, any_s = [], False
            for k, b in enumerate(order(at, size)):
                if b in local_mem:
                    cb, e = local_mem[b]
                    c = (c & ~(0xFF << (8 * k))) | (cb << (8 * k))
                else:
                    e = self.mem.get(b)
                    if e is None and b not in self.touched and self.inputs(b):
                        e = (self.var("m%x" % b, 8, "mem", b, data[b - at]), 0)
                        self.mem[b] = e
                ents.append(e)
                any_s = any_s or e is not None
            return c, (_join(ents, c, size) if any_s else None)

        def store(at, size, c, s):
            for k, b in enumerate(order(at, size)):
                local_mem[b] = ((c >> (8 * k)) & 0xFF, (s, k) if s is not None else None)

        ops = ins.ops
        i, n = 0, len(ops)
        done = False
        while i < n and not done:
            opc, out, args = ops[i]
            i += 1
            if opc == "COPY":
                c, s = read(args[0])
                write(out, c, s)
            elif opc == "LOAD":
                ac, asym = read(args[1])
                pin(ac, asym, 8 * args[1][2])
                c, s = load(ac & amask, out[2], None)
                write(out, c, s)
            elif opc == "STORE":
                ac, asym = read(args[1])
                pin(ac, asym, 8 * args[1][2])
                c, s = read(args[2])
                store(ac & amask, args[2][2], c & _mask(args[2][2]), s)
            elif opc == "CALLOTHER":
                name = args[0]
                if name == "segment":
                    sc, ss = read(args[1])
                    oc, os_ = read(args[2])
                    pin(sc, ss, 8 * args[1][2])
                    base = self.segbase(uc, sc)
                    s = None
                    if self.mask == 0xFFFF:                  # (real or protected 16-bit: base + 16-bit offset)
                        c = base + (oc & 0xFFFF)
                        if os_ is not None:
                            off16 = S.extract(os_, 0, 16)
                            bits = 8 * out[2]
                            s = S.binary("add", S.zext(off16, bits), S.const(base, bits)) if bits > 16 else off16
                    else:                                    # (flat: the offset is the address)
                        c = base + oc
                        if os_ is not None:
                            bits = 8 * out[2]
                            s = os_ if S.bits(os_) == bits else S.zext(os_, bits) if S.bits(os_) < bits \
                                else S.extract(os_, 0, bits)
                            if base:
                                s = S.binary("add", s, S.const(base, bits))
                    write(out, c, s)
                else:
                    if any(isinstance(a, tuple) and read(a)[1] is not None for a in args[1:]):
                        self.lost.add("userop:" + name)
                    if out is not None:
                        write(out, 0, None)
            elif opc == "CBRANCH":
                cc, cs = read(args[1])
                taken = cc != 0
                tgt = args[0]
                if tgt[0] == "c":                            # (a branch inside the instruction's p-code)
                    if cs is not None:
                        self.path.append((address, taken, S.nonzero(cs), "inner"))
                    if taken:
                        i = i - 1 + _sx(tgt[1], tgt[2])
                else:
                    target, fall = cs_base + (tgt[1] & self.mask), address + ins.length
                    if target in (fall, address):            # (a string instruction's count run out, or
                        if cs is not None:                   # its compare: its loop is the instruction's own)
                            self.path.append((address, taken, S.nonzero(cs), "inner"))
                    else:                                    # (the instruction's jump: resolved next step)
                        self.pending = (address, target, fall, S.nonzero(cs) if cs is not None else None, taken)
                    if taken:
                        done = True
            elif opc == "BRANCH":
                tgt = args[0]
                if tgt[0] == "c":
                    i = i - 1 + _sx(tgt[1], tgt[2])
                else:
                    done = True
            elif opc in ("BRANCHIND", "CALLIND", "RETURN"):
                c, s = read(args[0])
                pin(c, s, 8 * args[0][2])
                done = True
            elif opc == "CALL":
                done = True
            else:
                vals = [read(a) for a in args]
                c, s = _arith(opc, out[2] if out else 0, vals, args, self)
                if out is not None:
                    write(out, c, s)
        # what the instruction left: kept where it is made from an input, forgotten elsewhere
        for b, (_cb, e) in local_reg.items():
            if e is None:
                regs.pop(b, None)
            else:
                regs[b] = e
        for b, (_cb, e) in local_mem.items():
            self.touched.add(b)
            if e is None:
                self.mem.pop(b, None)
            else:
                self.mem[b] = e


class PyShadow16(Shadow):
    """The shadow of one 16-bit run (emulate/windows16.py trace_function16, `shadow`). `lifter`:
    emulate/pcode.py's for x86-16-real. `segbase(uc, selector)`: the base a selector names.
    `inputs(address)`: whether a linear byte is one of the run's inputs until written. `span`:
    (low, high) linear addresses of the function's own code (its jumps are the path's). What an
    unmodelled Windows call returns lands in DX:AX (and the high half of EAX)."""

    def __init__(self, lifter, segbase, inputs, span=None):
        Shadow.__init__(self, lifter, UnicornX86(16), inputs, span, segbase,
                        result=(("EAX", 0, 0), ("EAX", 1, 1), ("EAX", 2, 2), ("EAX", 3, 3), ("EDX", 0, 2), ("EDX", 1, 3)),
                        clobbered=("EAX", "EDX", "ESP", "EIP", "CS"))

    def enter(self, arg_places, bits=16):
        """The run is about to start: argument word k sits at linear address arg_places[k]."""
        Shadow.enter(self, arg_places, bits)


class PyShadow32(Shadow):
    """The shadow of one flat 32-bit run (emulate/emulator.py trace_function, `shadow`): every
    segment based at 0; an import or callback the tracer answers sets EAX to `ret` and moves ESP
    and EIP, nothing else. Arguments: dwords on the stack, or ECX / EDX (enter)."""

    def __init__(self, lifter, inputs, span=None):
        Shadow.__init__(self, lifter, UnicornX86(32), inputs, span, None,
                        result=(("EAX", 0, 0), ("EAX", 1, 1), ("EAX", 2, 2), ("EAX", 3, 3)),
                        clobbered=("EAX", "ESP", "EIP"))


class PyShadowLE(PyShadow32):
    """The shadow of one run of a driver's flat ring-0 code (emulate/driver.py run, `shadow`): a
    service call (or a call out through a pointer) the emulator answers sets EAX to `ret` and the
    carry flag to `carry`, as the input says. Inputs: the registers a procedure is entered with,
    the carry and zero flags, its stack words, the scratch memory its pointers lead into."""

    def _after_call(self, m, symbolic_result, value, *a, carry=0, **k):
        Shadow._after_call(self, m, symbolic_result, value, *a, **k)
        if symbolic_result and not self.off:
            off = self.lifter.regs["CF"][0]
            self.reg[off] = (self.var("carry", 8, "carry", None, carry & 1), 0)


class ShadowPPC(Shadow):
    """The shadow of one PowerPC run (emulate/powerpc.py run, `shadow`): a call out of the code
    given returns `ret` (64 bits) in r3 and changes nothing else. Arguments: r3-r10 (enter)."""

    def __init__(self, lifter, inputs, span=None):
        Shadow.__init__(self, lifter, PowerPCView(), inputs, span, None,
                        result=tuple(("r3", k, k) for k in range(8)), clobbered=("r3",), ret_bits=64)


def _arith(opc, osize, vals, args, sh):
    """One p-code operation of values (concrete, symbolic or None): -> (concrete, symbolic or None)."""
    (ac, a_s) = vals[0]
    asize = args[0][2]
    m = _mask(osize) if osize else 0
    sym = any(s is not None for _c, s in vals)
    if len(vals) > 1:
        bc, b_s = vals[1]
        bsize = args[1][2]
    # --- the concrete half
    if opc == "INT_ADD":
        c = (ac + bc) & m
    elif opc == "INT_SUB":
        c = (ac - bc) & m
    elif opc == "INT_AND":
        c = ac & bc
    elif opc == "INT_OR":
        c = ac | bc
    elif opc == "INT_XOR":
        c = ac ^ bc
    elif opc == "INT_MULT":
        c = (ac * bc) & m
    elif opc == "INT_DIV":
        c = (ac // bc) & m if bc else 0
    elif opc == "INT_REM":
        c = (ac % bc) & m if bc else 0
    elif opc == "INT_SDIV":
        x, y = _sx(ac, asize), _sx(bc, bsize)
        c = (abs(x) // abs(y) * (1 if (x < 0) == (y < 0) else -1)) & m if y else 0
    elif opc == "INT_SREM":
        x, y = _sx(ac, asize), _sx(bc, bsize)
        c = ((abs(x) % abs(y)) * (1 if x >= 0 else -1)) & m if y else 0
    elif opc == "INT_EQUAL":
        c = int(ac == bc)
    elif opc == "INT_NOTEQUAL":
        c = int(ac != bc)
    elif opc == "INT_LESS":
        c = int(ac < bc)
    elif opc == "INT_LESSEQUAL":
        c = int(ac <= bc)
    elif opc == "INT_SLESS":
        c = int(_sx(ac, asize) < _sx(bc, bsize))
    elif opc == "INT_SLESSEQUAL":
        c = int(_sx(ac, asize) <= _sx(bc, bsize))
    elif opc == "INT_ZEXT":
        c = ac
    elif opc == "INT_SEXT":
        c = _sx(ac, asize) & m
    elif opc == "INT_CARRY":
        c = int(ac + bc > _mask(asize))
    elif opc == "INT_SCARRY":
        r = _sx(ac, asize) + _sx(bc, asize)
        c = int(not -(1 << (8 * asize - 1)) <= r < (1 << (8 * asize - 1)))
    elif opc == "INT_SBORROW":
        r = _sx(ac, asize) - _sx(bc, asize)
        c = int(not -(1 << (8 * asize - 1)) <= r < (1 << (8 * asize - 1)))
    elif opc == "INT_2COMP":
        c = (-ac) & m
    elif opc == "INT_NEGATE":
        c = (~ac) & m
    elif opc == "INT_LEFT":
        c = (ac << bc) & m if bc < 8 * osize else 0
    elif opc == "INT_RIGHT":
        c = (ac >> bc) & m if bc < 8 * asize else 0
    elif opc == "INT_SRIGHT":
        c = (_sx(ac, asize) >> min(bc, 8 * asize)) & m
    elif opc == "BOOL_NEGATE":
        c = int(not ac)
    elif opc == "BOOL_AND":
        c = int(bool(ac) and bool(bc))
    elif opc == "BOOL_OR":
        c = int(bool(ac) or bool(bc))
    elif opc == "BOOL_XOR":
        c = int(bool(ac) != bool(bc))
    elif opc == "PIECE":
        c = ((ac << (8 * bsize)) | bc) & m
    elif opc == "SUBPIECE":
        c = (ac >> (8 * bc)) & m
    elif opc == "POPCOUNT":
        c = bin(ac).count("1") & m
    elif opc == "LZCOUNT":
        c = (8 * asize - ac.bit_length()) & m
    else:                                                    # (floating point and the rest: not followed)
        if sym:
            sh.lost.add("op:" + opc)
        return 0, None
    if not sym:
        return c, None
    # --- the symbolic half
    A = _bv(ac, a_s, asize)
    if len(vals) > 1:
        B = _bv(bc, b_s, bsize)
    if opc in _BINARY:
        s = S.binary(_BINARY[opc], A, B)
    elif opc in _COMPARE:
        how, flip = _COMPARE[opc]
        c2 = S.compare(how, A, B)
        s = S.to_bool(S.negate(c2) if flip else c2)
    elif opc == "INT_ZEXT":
        s = S.zext(A, 8 * osize)
    elif opc == "INT_SEXT":
        s = S.sext(A, 8 * osize)
    elif opc == "INT_CARRY":
        s = S.to_bool(S.compare("ult", S.binary("add", A, B), A))
    elif opc in ("INT_SCARRY", "INT_SBORROW"):
        r = S.binary("add" if opc == "INT_SCARRY" else "sub", A, B)
        x = S.binary("and", S.binary("xor", A, r), S.binary("xor", B, r)) if opc == "INT_SCARRY" else \
            S.binary("and", S.binary("xor", A, B), S.binary("xor", A, r))
        s = S.to_bool(S.nonzero(S.extract(x, 8 * asize - 1, 1)))
    elif opc == "INT_2COMP":
        s = S.unary("neg", A)
    elif opc == "INT_NEGATE":
        s = S.unary("not", A)
    elif opc in ("INT_LEFT", "INT_RIGHT", "INT_SRIGHT"):
        width = 8 * asize
        kind = {"INT_LEFT": "shl", "INT_RIGHT": "lshr", "INT_SRIGHT": "ashr"}[opc]
        if bsize > asize:
            amount = S.extract(B, 0, width)
            big = S.compare("ule", S.const(width, 8 * bsize), B)
        else:
            amount, big = S.zext(B, width), None
        s = S.binary(kind, A, amount)
        if big is not None:
            over = S.binary("ashr", A, S.const(width - 1, width)) if kind == "ashr" else S.const(0, width)
            s = ("ite", width, big, over, s)
    elif opc == "BOOL_NEGATE":
        s = S.to_bool(S.negate(S.nonzero(A)))
    elif opc in ("BOOL_AND", "BOOL_OR", "BOOL_XOR"):
        s = S.to_bool(S.logic({"BOOL_AND": "band", "BOOL_OR": "bor", "BOOL_XOR": "bxor"}[opc],
                              S.nonzero(A), S.nonzero(B)))
    elif opc == "PIECE":
        s = S.concat(A, B)
    elif opc == "SUBPIECE":
        lo = 8 * bc
        width = min(8 * asize - lo, 8 * osize)
        s = S.zext(S.extract(A, lo, width), 8 * osize)
    elif opc == "POPCOUNT":
        s = ("popcount", 8 * osize, A)
    else:
        sh.lost.add("op:" + opc)
        return c, None
    if S.bits(s) != 8 * osize:                               # (an output narrower or wider than made)
        s = S.extract(s, 0, 8 * osize) if S.bits(s) > 8 * osize else S.zext(s, 8 * osize)
    return c, s


_BINARY = {"INT_ADD": "add", "INT_SUB": "sub", "INT_AND": "and", "INT_OR": "or", "INT_XOR": "xor",
           "INT_MULT": "mul", "INT_DIV": "udiv", "INT_REM": "urem", "INT_SDIV": "sdiv", "INT_SREM": "srem"}
_COMPARE = {"INT_EQUAL": ("eq", False), "INT_NOTEQUAL": ("eq", True), "INT_LESS": ("ult", False),
            "INT_LESSEQUAL": ("ule", False), "INT_SLESS": ("slt", False), "INT_SLESSEQUAL": ("sle", False)}


# --- the x86 shadows: in C++ (emulate/native_shadow.py, CB_NATIVE_SHADOW) or the Python above.
# `inputs`: [(low, high)] -- the memory bytes that are inputs of the run until written.
def _inputs_test(inputs):
    ranges = [(int(lo), int(hi)) for lo, hi in inputs]
    return lambda b: any(lo <= b < hi for lo, hi in ranges)


def real_mode(_uc, sel):
    """A selector's base in real mode (Shadow16's `segbase` for a real-mode run)."""
    return sel * 16


def Shadow16(lifter, segbase, inputs, span=None):
    """A 16-bit run's (windows16.trace_function16): `segbase(uc, selector)` -- windows16.selector_base
    (the GDT's), or real_mode."""
    from codebreaker.emulate import native_shadow, windows16
    if native_shadow.enabled() and segbase in (windows16.selector_base, real_mode):
        return native_shadow.Shadow(16, lifter, inputs, span, real_mode=segbase is real_mode)
    return PyShadow16(lifter, segbase, _inputs_test(inputs), span)


def Shadow32(lifter, inputs, span=None):
    """A flat 32-bit run's (emulator.trace_function)."""
    from codebreaker.emulate import native_shadow
    if native_shadow.enabled():
        return native_shadow.Shadow(32, lifter, inputs, span)
    return PyShadow32(lifter, _inputs_test(inputs), span)


def ShadowLE(lifter, inputs, span=None):
    """A driver's flat ring-0 run's (driver.run)."""
    from codebreaker.emulate import native_shadow
    if native_shadow.enabled():
        return native_shadow.Shadow(33, lifter, inputs, span)
    return PyShadowLE(lifter, _inputs_test(inputs), span)
