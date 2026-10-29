"""shadow.py -- which values of a run are made from its inputs, and from which: a run's symbolic
shadow, kept up beside Unicorn instruction by instruction.

    sh = shadow.Shadow16(lifter, segbase, inputs)     # inputs: which linear bytes are inputs
    ... the tracer calls sh.enter(), sh.step() before each instruction, sh.after_call() ...
    sh.path        [(site, taken, condition, kind)]  the conditions the run's path went by: each
                   a condition (emulate/symbolic.py) and whether it held -- a jump's ("jump": taken or not), one
                   inside an instruction's p-code ("inner": a string instruction's count), or an
                   address or selector held at its value ("pin")
    sh.trail       [(site, taken)]                   every conditional jump, in order
    sh.vars        {name: (kind, where, bits, value)} the inputs the conditions are over
    sh.lost        why the conditions may not be all there is (empty: they are)

The run itself is Unicorn's: nothing here changes it. Before each instruction runs, its p-code
(emulate/pcode.py, the decompiler's own specification) is evaluated twice over at once --
concretely, from the registers and memory as they are at that moment, and symbolically (emulate/symbolic.py,
for the solver) for the values that come from an input. Only the symbolic half is kept: which
register bytes and memory bytes hold an expression over the inputs. A conditional jump whose
condition is such an expression is a condition of the path; its direction is the one Unicorn
then takes (the next instruction says which).

The inputs are the function's argument words (a0, a1 ... in the order pushed), any byte of
memory the `inputs(address)` test names (the data segment, the scratch segment pointers go to)
not yet written by the run (m<address>), and what an unmodelled Windows call returns (ret, 32
bits, DX:AX). An address made from an input is held at the value it had (a "pin" in the path:
the pointer as given), and so is a selector; a value made from an input that leaves through
something not followed (a call the environment answers from its arguments, an interrupt, an
operation without a meaning here) makes the path's conditions possibly short of all there are,
and says so in `lost`."""
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


class Shadow16(object):
    """The shadow of one 16-bit run (emulate/windows16.py trace_function16, `shadow`). `lifter`:
    emulate/pcode.py's for x86-16-real. `segbase(uc, selector)`: the base a selector names.
    `inputs(address)`: whether a linear byte is one of the run's inputs until written. `span`:
    (low, high) linear addresses of the function's own code (its jumps are the path's)."""

    def __init__(self, lifter, segbase, inputs, span=None):
        from unicorn import x86_const as X
        self.lifter, self._segbase, self.inputs, self.span = lifter, segbase, inputs, span
        self.bases = {}                     # selector -> base, as read this run (until a call)
        self.reg = {}                       # register byte -> (expr, k)
        self.mem = {}                       # linear byte -> (expr, k)
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
        # register bytes -> (Unicorn register, shift) or ("F", bit) for a flag
        self.places = {}
        for name, (off, size) in lifter.regs.items():
            if name in _FLAGS:
                self.places[off] = ("F", _FLAGS[name])
                continue
            uc_reg = getattr(X, "UC_X86_REG_" + name, None)
            if uc_reg is None:
                continue
            for k in range(size):
                self.places[off + k] = (uc_reg, 8 * k)
        self.flags_reg = X.UC_X86_REG_EFLAGS
        self.named = {n: o for n, (o, _s) in lifter.regs.items()}

    def segbase(self, uc, sel):
        """The base `sel` names (read once, until the environment next answers a call: an
        allocation makes descriptors)."""
        got = self.bases.get(sel)
        if got is None:
            got = self.bases[sel] = self._segbase(uc, sel)
        return got

    # --- inputs ---------------------------------------------------------------------------------
    def var(self, name, bits, kind, where, value):
        if name not in self.vars:
            self.vars[name] = (kind, where, bits, value)
            self._sym[name] = S.var(name, bits)
        return self._sym[name]

    def enter(self, arg_places):
        """The run is about to start: argument word k sits at linear address arg_places[k]."""
        for k, (at, value) in enumerate(arg_places):
            v = self.var("a%d" % k, 16, "arg", k, value)
            self.mem[at] = (v, 0)
            self.mem[at + 1] = (v, 1)

    def _forget_regs(self, names):
        for n in names:
            off = self.named.get(n)
            if off is None:
                continue
            size = self.lifter.regs[n][1]
            for b in range(off, off + size):
                self.reg.pop(b, None)

    # (what the tracer calls: nothing here may change the run -- whatever goes wrong in the
    # shadow ends the shadow, never the run: a selector no descriptor names, read for its base,
    # was an exception in Unicorn's hook, and Unicorn stopped LZCOPY there)
    def after_call(self, *a, **k):
        self._safely(self._after_call, *a, **k)

    def step(self, uc, address, cs_base):
        """Before the instruction at linear `address` (its segment based at `cs_base`) runs."""
        if not self.off:
            self._safely(self._step, uc, address, cs_base)

    def _safely(self, f, *a, **k):
        try:
            f(*a, **k)
        except Exception as e:
            self.lost.add("shadow: %s" % type(e).__name__)
            self.off = True

    def _after_call(self, uc, symbolic_result, value, args_at=None, nargs=0, why=None):
        """A call the environment answered (an imported function, or a callee not run): AX and DX
        now hold its result -- `ret`, an input, when `symbolic_result` -- and SP, CS, IP were set.
        Arguments made from the inputs, when the result is made from them (`why`): the path may be
        short of what it depends on."""
        self.pending = None
        self.bases.clear()
        if self.off:
            return
        if why and args_at is not None:
            if any(b in self.mem for b in range(args_at, args_at + 2 * nargs)):
                self.lost.add(why)
        self._forget_regs(("EAX", "EDX", "ESP", "EIP", "CS"))
        if symbolic_result:
            r = self.var("ret", 32, "ret", None, value & 0xFFFFFFFF)
            ax, dx = self.named["EAX"], self.named["EDX"]
            self.reg[ax], self.reg[ax + 1] = (r, 0), (r, 1)
            self.reg[ax + 2], self.reg[ax + 3] = (r, 2), (r, 3)
            self.reg[dx], self.reg[dx + 1] = (r, 2), (r, 3)

    def written(self, at, size):
        """Memory the environment wrote (not an instruction): no longer an input, nor made from one."""
        for b in range(at, at + size):
            self.mem.pop(b, None)
            self.touched.add(b)

    # --- one instruction ------------------------------------------------------------------------
    def _step(self, uc, address, cs_base):
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
            raw = bytes(uc.mem_read(address, 16))
        except Exception:
            try:
                raw = bytes(uc.mem_read(address, (address | 0xFFF) + 1 - address))
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
            self._run(uc, address, cs_base, ins)
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

        def concrete_reg(b):
            place = self.places.get(b)
            if place is None:
                return 0
            r, shift = place
            if r == "F":
                if "F" not in conc:
                    conc["F"] = uc.reg_read(self.flags_reg)
                return (conc["F"] >> shift) & 1
            if r not in conc:
                conc[r] = uc.reg_read(r)
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
                for k in range(size):
                    b = off + k
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
                for k in range(size):
                    local_reg[off + k] = ((c >> (8 * k)) & 0xFF, (s, k) if s is not None else None)
            elif space == "m":
                store(off, size, c, s)

        def pin(c, s, bits):
            if s is not None:
                self.path.append((address, True, S.compare("eq", s, S.const(c, S.bits(s))), "pin"))
                self.pins += 1

        def load(at, size, _s):
            try:
                data = bytes(uc.mem_read(at, size))
            except Exception:
                data = bytes(size)
            c = int.from_bytes(data, "little")
            ents, any_s = [], False
            for k in range(size):
                b = at + k
                if b in local_mem:
                    cb, e = local_mem[b]
                    c = (c & ~(0xFF << (8 * k))) | (cb << (8 * k))
                else:
                    e = self.mem.get(b)
                    if e is None and b not in self.touched and self.inputs(b):
                        e = (self.var("m%x" % b, 8, "mem", b, data[k]), 0)
                        self.mem[b] = e
                ents.append(e)
                any_s = any_s or e is not None
            return c, (_join(ents, c, size) if any_s else None)

        def store(at, size, c, s):
            for k in range(size):
                local_mem[at + k] = ((c >> (8 * k)) & 0xFF, (s, k) if s is not None else None)

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
                c, s = load(ac, out[2], None)
                write(out, c, s)
            elif opc == "STORE":
                ac, asym = read(args[1])
                pin(ac, asym, 8 * args[1][2])
                c, s = read(args[2])
                store(ac, args[2][2], c & _mask(args[2][2]), s)
            elif opc == "CALLOTHER":
                name = args[0]
                if name == "segment":
                    sc, ss = read(args[1])
                    oc, os_ = read(args[2])
                    pin(sc, ss, 8 * args[1][2])
                    base = self.segbase(uc, sc)
                    c = base + (oc & 0xFFFF)
                    s = None
                    if os_ is not None:
                        off16 = S.extract(os_, 0, 16)
                        bits = 8 * out[2]
                        s = S.binary("add", S.zext(off16, bits), S.const(base, bits)) if bits > 16 else off16
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
                    target, fall = cs_base + (tgt[1] & 0xFFFF), address + ins.length
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
