"""ppctrace — a lightweight single-step PowerPC (Xenon) interpreter + tracer.

The static side of this toolchain recompiles PPC to C++; this is the dynamic
complement: it *executes* a function one instruction at a time over a flat guest
memory, recording a register trace. Useful for debugging a reconstruction,
diffing two builds' behaviour, or understanding a routine without a console.

Integer core only (GPRs, CR, LR, CTR, XER-CA), big-endian, 32-bit effective
addresses: the common arithmetic/logical/rotate/compare/branch/load-store
vocabulary. Floating-point and VMX/VMX128 instructions are traced as
`unsupported` and skipped (they do not affect the integer state a trainer or a
control-flow diff cares about). Instruction decode reuses Capstone (PPC BE).
"""

import math
import struct
import sys

try:
    import capstone
    from capstone import ppc as _ppc
except Exception:
    capstone = None
    _ppc = None

MASK32 = 0xFFFFFFFF


def _s32(v):
    v &= MASK32
    return v - (1 << 32) if v & 0x80000000 else v


class Mem:
    """Sparse big-endian guest memory (page dict)."""
    PAGE = 0x1000

    def __init__(self):
        self.pages = {}

    def _pg(self, addr):
        return self.pages.setdefault(addr & ~(self.PAGE - 1), bytearray(self.PAGE))

    def read(self, addr, n):
        out = bytearray()
        for i in range(n):
            a = (addr + i) & MASK32
            out.append(self._pg(a)[a & (self.PAGE - 1)])
        return bytes(out)

    def write(self, addr, data):
        for i, b in enumerate(data):
            a = (addr + i) & MASK32
            self._pg(a)[a & (self.PAGE - 1)] = b

    def load(self, addr, buf, base=None):
        self.write(base if base is not None else addr, buf)

    def r8(self, a): return self.read(a, 1)[0]
    def r16(self, a): return struct.unpack(">H", self.read(a, 2))[0]
    def r32(self, a): return struct.unpack(">I", self.read(a, 4))[0]
    def w8(self, a, v): self.write(a, bytes([v & 0xFF]))
    def w16(self, a, v): self.write(a, struct.pack(">H", v & 0xFFFF))
    def w32(self, a, v): self.write(a, struct.pack(">I", v & MASK32))


class Cpu:
    def __init__(self, mem=None):
        self.gpr = [0] * 32
        self.fpr = [0.0] * 32            # floating-point registers (doubles)
        self.lr = 0
        self.ctr = 0
        self.cr = 0                      # 8 x 4-bit fields, field 0 = bits 28..31
        self.ca = 0                      # XER carry
        self.pc = 0
        self.mem = mem or Mem()
        self.steps = 0

    # -- CR field 0 (from a signed compare of a to b) --
    def _setcr0(self, val):
        s = _s32(val)
        f = 0
        if s < 0:
            f |= 8
        elif s > 0:
            f |= 4
        else:
            f |= 2
        self.cr = (self.cr & ~(0xF << 28)) | (f << 28)

    def _cmp(self, field, a, b, signed=True):
        a2 = _s32(a) if signed else (a & MASK32)
        b2 = _s32(b) if signed else (b & MASK32)
        f = 8 if a2 < b2 else (4 if a2 > b2 else 2)
        sh = 28 - field * 4
        self.cr = (self.cr & ~(0xF << sh)) | (f << sh)

    def _crbit(self, bi):
        return (self.cr >> (31 - bi)) & 1


def _reg_index(name):
    # capstone gives 'r3','r0',... ; also 'sp'/'r1'
    n = name.lower()
    if n in ("sp",):
        return 1
    if n in ("rtoc",):
        return 2
    if n.startswith("r"):
        try:
            return int(n[1:])
        except ValueError:
            return None
    return None


class Tracer:
    def __init__(self, base_vaddr, code):
        if capstone is None:
            raise RuntimeError("capstone not available (pip install capstone)")
        self.base = base_vaddr
        self.code = code
        self.md = capstone.Cs(capstone.CS_ARCH_PPC,
                              capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
        self.md.detail = True
        self._cache = {}

    def _decode(self, pc):
        ins = self._cache.get(pc)
        if ins is None:
            off = pc - self.base
            if 0 <= off <= len(self.code) - 4:
                ins = next(iter(self.md.disasm(bytes(self.code[off:off + 4]), pc, 1)), None)
            self._cache[pc] = ins
        return ins

    def run(self, cpu, max_steps=100000, trace=None):
        """Execute from cpu.pc until an blr returns to 0 (or the call stack
        empties), an unsupported control transfer, or max_steps. `trace` is an
        optional list that receives (pc, text, {reg: newval}) per step."""
        ret_sentinel = cpu.lr
        while cpu.steps < max_steps:
            ins = self._decode(cpu.pc)
            if ins is None:
                # VMX/VMX128 vector ops don't decode under base-PPC capstone but
                # are fixed-width and never branch, and don't touch the integer
                # state this core models — skip past them (as documented) rather
                # than halting. Halt only on genuinely non-decodable data.
                off = cpu.pc - self.base
                raw = self.code[off:off + 4] if 0 <= off <= len(self.code) - 4 else b""
                if len(raw) == 4 and (raw[0] >> 2) in (4, 5, 6):
                    cpu.pc = (cpu.pc + 4) & MASK32
                    cpu.steps += 1
                    continue
                break
            before = list(cpu.gpr)
            old_pc = cpu.pc
            stop = self._exec(cpu, ins)
            cpu.steps += 1
            if trace is not None:
                changed = {"r%d" % i: cpu.gpr[i] for i in range(32) if cpu.gpr[i] != before[i]}
                trace.append((ins.address, "%s %s" % (ins.mnemonic, ins.op_str), changed))
            if stop == "return":
                return "return"
            if cpu.pc == old_pc:                    # no branch taken -> advance
                cpu.pc = old_pc + 4
        return "halt"

    # ---- the interpreter core ----
    def _exec(self, cpu, ins):
        m = ins.mnemonic
        ops = ins.operands
        g = cpu.gpr

        def reg(o):
            return _reg_index(ins.reg_name(o.reg))

        def rval(o):
            i = reg(o)
            return g[i] if i is not None else 0

        # branches first (they set pc)
        if m in ("b", "ba"):
            cpu.pc = ops[0].imm & MASK32
            return
        if m in ("bl", "bla"):
            cpu.lr = (ins.address + 4) & MASK32
            cpu.pc = ops[0].imm & MASK32
            return
        if m == "blr":
            cpu.pc = cpu.lr & MASK32
            return "return"
        if m in ("bctr", "bctrl"):
            if m == "bctrl":
                cpu.lr = (ins.address + 4) & MASK32
            cpu.pc = cpu.ctr & MASK32
            return
        if m.startswith("b") and ops and ops[-1].type == _ppc.PPC_OP_IMM:
            # conditional branch bc / beq/bne/blt/bgt/bge/ble... capstone gives target imm
            if _branch_taken(cpu, m):
                cpu.pc = ops[-1].imm & MASK32
            return

        # integer ops writing rD = f(rA, rB/imm)
        rc = ins.mnemonic.endswith(".")
        base = m[:-1] if rc else m

        def wr(val):
            i = reg(ops[0])
            if i is not None:
                g[i] = val & MASK32
                if rc or base in ("cmpwi", "cmpw"):
                    pass
                if rc:
                    cpu._setcr0(g[i])

        if base in ("add", "addc"):
            wr(rval(ops[1]) + rval(ops[2])); return
        if base in ("addi", "addic"):
            wr(rval(ops[1]) + (ops[2].imm & MASK32)); return
        if base == "addis":
            wr(rval(ops[1]) + ((ops[2].imm << 16) & MASK32)); return
        if base in ("subf", "sub"):
            wr(rval(ops[2]) - rval(ops[1]) if base == "subf" else rval(ops[1]) - rval(ops[2])); return
        if base == "subfic":
            wr((ops[2].imm & MASK32) - rval(ops[1])); return
        if base == "neg":
            wr((-_s32(rval(ops[1]))) & MASK32); return
        if base == "and":
            wr(rval(ops[1]) & rval(ops[2])); return
        if base == "or":
            wr(rval(ops[1]) | rval(ops[2])); return
        if base == "xor":
            wr(rval(ops[1]) ^ rval(ops[2])); return
        if base == "nand":
            wr(~(rval(ops[1]) & rval(ops[2]))); return
        if base == "nor":
            wr(~(rval(ops[1]) | rval(ops[2]))); return
        if base == "andi":
            i = reg(ops[0]); g[i] = rval(ops[1]) & (ops[2].imm & MASK32); cpu._setcr0(g[i]); return
        if base == "ori":
            wr(rval(ops[1]) | (ops[2].imm & MASK32)); return
        if base == "oris":
            wr(rval(ops[1]) | ((ops[2].imm << 16) & MASK32)); return
        if base == "xori":
            wr(rval(ops[1]) ^ (ops[2].imm & MASK32)); return
        if base in ("li",):
            wr(ops[1].imm & MASK32); return
        if base in ("lis",):
            wr((ops[1].imm << 16) & MASK32); return
        if base in ("mr", "mr."):
            wr(rval(ops[1])); return
        if base == "mullw":
            wr((_s32(rval(ops[1])) * _s32(rval(ops[2]))) & MASK32); return
        if base == "mulli":
            wr((_s32(rval(ops[1])) * ops[2].imm) & MASK32); return
        if base == "divw":
            b = _s32(rval(ops[2]))
            wr((int(_s32(rval(ops[1])) / b) if b else 0) & MASK32); return
        if base == "divwu":
            b = rval(ops[2]) & MASK32
            wr(((rval(ops[1]) & MASK32) // b if b else 0) & MASK32); return
        if base == "slw":
            wr(rval(ops[1]) << (rval(ops[2]) & 31)); return
        if base == "srw":
            wr((rval(ops[1]) & MASK32) >> (rval(ops[2]) & 31)); return
        if base == "srawi":
            wr(_s32(rval(ops[1])) >> (ops[2].imm & 31)); return
        if base == "slwi":
            wr(rval(ops[1]) << (ops[2].imm & 31)); return
        if base == "srwi":
            wr((rval(ops[1]) & MASK32) >> (ops[2].imm & 31)); return
        if base == "rlwinm":
            wr(_rlwinm(rval(ops[1]), ops[2].imm, ops[3].imm, ops[4].imm)); return
        if base == "extsb":
            wr(_s32((rval(ops[1]) & 0xFF) | (0xFFFFFF00 if rval(ops[1]) & 0x80 else 0))); return
        if base == "extsh":
            v = rval(ops[1]) & 0xFFFF
            wr(v | 0xFFFF0000 if v & 0x8000 else v); return
        if base == "cntlzw":
            wr(32 - (rval(ops[1]) & MASK32).bit_length()); return

        # compares
        if base == "cmpw":
            fld = _cr_field(ins, cpu); cpu._cmp(fld, rval(ops[-2]), rval(ops[-1]), True); return
        if base == "cmpwi":
            fld = _cr_field(ins, cpu); cpu._cmp(fld, rval(ops[-2]), ops[-1].imm, True); return
        if base == "cmplw":
            fld = _cr_field(ins, cpu); cpu._cmp(fld, rval(ops[-2]), rval(ops[-1]), False); return
        if base == "cmplwi":
            fld = _cr_field(ins, cpu); cpu._cmp(fld, rval(ops[-2]), ops[-1].imm & MASK32, False); return

        # SPR moves
        if base == "mflr":
            wr(cpu.lr); return
        if base == "mtlr":
            cpu.lr = rval(ops[0]); return
        if base == "mfctr":
            wr(cpu.ctr); return
        if base == "mtctr":
            cpu.ctr = rval(ops[0]); return

        # loads / stores  (op0 = reg, op1 = mem{base,disp})
        if base in ("lwz", "lbz", "lhz", "lha", "lwzx", "lbzx", "lhzx"):
            ea = _ea(ins, cpu, ops)
            i = reg(ops[0])
            if "lwz" in base:
                g[i] = cpu.mem.r32(ea)
            elif "lbz" in base:
                g[i] = cpu.mem.r8(ea)
            elif "lha" in base:
                v = cpu.mem.r16(ea); g[i] = v | 0xFFFF0000 if v & 0x8000 else v
            else:
                g[i] = cpu.mem.r16(ea)
            return
        if base in ("stw", "stb", "sth", "stwx", "stbx", "sthx"):
            ea = _ea(ins, cpu, ops)
            v = rval(ops[0])
            if "stw" in base:
                cpu.mem.w32(ea, v)
            elif "stb" in base:
                cpu.mem.w8(ea, v)
            else:
                cpu.mem.w16(ea, v)
            return
        if base in ("nop", "ori") and m == "nop":
            return

        # floating-point core (FPRs hold doubles; single ops just round on store)
        fp = cpu.fpr

        def fi(o):
            nm = ins.reg_name(o.reg) or ""
            return int(nm[1:]) if nm.lower().startswith("f") and nm[1:].isdigit() else None

        def fv(o):
            i = fi(o)
            return fp[i] if i is not None else 0.0

        def fwr(val):
            i = fi(ops[0])
            if i is not None:
                fp[i] = float(val)

        if base in ("lfs", "lfsx", "lfd", "lfdx"):          # FP loads (start l)
            ea = _ea(ins, cpu, ops)
            fp[fi(ops[0])] = (struct.unpack(">f", cpu.mem.read(ea, 4))[0] if "lfs" in base
                              else struct.unpack(">d", cpu.mem.read(ea, 8))[0])
            return
        if base in ("stfs", "stfsx", "stfd", "stfdx"):      # FP stores (start s)
            ea = _ea(ins, cpu, ops)
            v = fv(ops[0])
            if "stfs" in base:
                try:
                    packed = struct.pack(">f", v)
                except OverflowError:                       # double out of float range
                    packed = struct.pack(">f", math.copysign(float("inf"), v))
                cpu.mem.write(ea, packed)
            else:
                cpu.mem.write(ea, struct.pack(">d", v))
            return
        if base and base[0] == "f":
            if base in ("fadd", "fadds"):
                fwr(fv(ops[1]) + fv(ops[2])); return
            if base in ("fsub", "fsubs"):
                fwr(fv(ops[1]) - fv(ops[2])); return
            if base in ("fmul", "fmuls"):
                fwr(fv(ops[1]) * fv(ops[2])); return
            if base in ("fdiv", "fdivs"):
                d = fv(ops[2]); fwr(fv(ops[1]) / d if d else 0.0); return
            if base in ("fmadd", "fmadds"):
                fwr(fv(ops[1]) * fv(ops[2]) + fv(ops[3])); return
            if base in ("fmsub", "fmsubs"):
                fwr(fv(ops[1]) * fv(ops[2]) - fv(ops[3])); return
            if base in ("fnmadd", "fnmadds"):
                fwr(-(fv(ops[1]) * fv(ops[2]) + fv(ops[3]))); return
            if base in ("fnmsub", "fnmsubs"):
                fwr(-(fv(ops[1]) * fv(ops[2]) - fv(ops[3]))); return
            if base == "fmr":
                fwr(fv(ops[1])); return
            if base == "fneg":
                fwr(-fv(ops[1])); return
            if base == "fabs":
                fwr(abs(fv(ops[1]))); return
            if base == "fnabs":
                fwr(-abs(fv(ops[1]))); return
            if base == "fcmpu":
                x, y = fv(ops[-2]), fv(ops[-1])
                f = 8 if x < y else (4 if x > y else 2)
                cpu.cr = (cpu.cr & ~(0xF << 28)) | (f << 28)
                return
            return                      # other FP: skip
        return  # unsupported: skip (integer state untouched)


def _ea(ins, cpu, ops):
    for o in ops:
        if o.type == _ppc.PPC_OP_MEM:
            b = _reg_index(ins.reg_name(o.mem.base)) if o.mem.base else None
            base = cpu.gpr[b] if b is not None else 0
            return (base + o.mem.disp) & MASK32
    # indexed form: rA,rB
    if len(ops) >= 3:
        a = _reg_index(ins.reg_name(ops[1].reg)) or 0
        bb = _reg_index(ins.reg_name(ops[2].reg)) or 0
        return (cpu.gpr[a] + cpu.gpr[bb]) & MASK32
    return 0


def _cr_field(ins, cpu):
    # cmpw crN,rA,rB -> op0 may be the CR field; default 0
    if ins.operands and ins.operands[0].type == _ppc.PPC_OP_REG:
        nm = ins.reg_name(ins.operands[0].reg) or ""
        if nm.lower().startswith("cr"):
            try:
                return int(nm[2:])
            except ValueError:
                return 0
    return 0


def _branch_taken(cpu, mnem):
    m = mnem.rstrip("+-")
    cond = {"beq": (2, 1), "bne": (2, 0), "blt": (0, 1), "bge": (0, 0),
            "bgt": (1, 1), "ble": (1, 0)}
    if m in cond:
        bit, want = cond[m]
        return ((cpu.cr >> (31 - bit)) & 1) == want
    if m in ("bdnz", "bdnz+", "bdnz-"):
        cpu.ctr = (cpu.ctr - 1) & MASK32
        return cpu.ctr != 0
    if m in ("bdz",):
        cpu.ctr = (cpu.ctr - 1) & MASK32
        return cpu.ctr == 0
    return True                                      # unknown cond: fall through-safe


def _rlwinm(rs, sh, mb, me):
    rs &= MASK32
    rot = ((rs << sh) | (rs >> (32 - sh))) & MASK32 if sh else rs
    if mb <= me:
        mask = 0
        for b in range(mb, me + 1):
            mask |= 1 << (31 - b)
    else:
        mask = MASK32
        for b in range(me + 1, mb):
            mask &= ~(1 << (31 - b))
    return rot & mask


DESCRIPTION = "single-step PowerPC interpreter/tracer (integer core) — dynamic analysis"


def _load_image(path):
    """Return (base_vaddr, image_bytes). Accepts a XEX (decoded basefile) or a
    raw PE/basefile (mapped at 0x82000000 by convention)."""
    data = open(path, "rb").read()
    if data[:4] == b"XEX2":
        import xex
        x = xex.open_xex(path)
        import xexinject
        load, _ = xexinject._va_off(x, 0)
        return load, x.basefile()
    return 0x82000000, data


def selftest():
    import ppcasm
    base = 0x82000000
    lines = ["li r3, 0", "li r4, 1", "add r3, r3, r4", "addi r4, r4, 1",
             "cmpwi r4, 11", "blt 0x82000008", "mulli r5, r3, 2",
             "stw r5, 0x100(r1)", "lwz r6, 0x100(r1)", "blr"]
    code = b"".join(struct.pack(">I", ppcasm.assemble_one(l, base + 4 * i))
                     for i, l in enumerate(lines))
    cpu = Cpu()
    cpu.gpr[1] = 0x70000000
    cpu.pc, cpu.lr = base, 0
    Tracer(base, code).run(cpu, max_steps=1000)
    if not (cpu.gpr[3] == 55 and cpu.gpr[5] == 110 and cpu.gpr[6] == 110):
        return False
    # floating-point: lfd x3; fmul; fadd; stfd  ->  2.5*4.0 + 1.5 = 11.5
    fprog = [0xC8230000, 0xC8430008, 0xC8630010, 0xFC0100B2, 0xFC00182A, 0xD8030018, 0x4E800020]
    fcode = b"".join(struct.pack(">I", w) for w in fprog)
    c = Cpu()
    c.gpr[3] = 0x70000000
    c.mem.write(0x70000000, struct.pack(">ddd", 2.5, 4.0, 1.5))
    c.pc, c.lr = 0x82002000, 0
    Tracer(0x82002000, fcode).run(c, max_steps=100)
    if abs(struct.unpack(">d", c.mem.read(0x70000018, 8))[0] - 11.5) >= 1e-9:
        return False
    # robustness: stfs of an out-of-float-range double must not throw (it saturates
    # to +/-inf) — real code stores garbage doubles read from uninitialized memory.
    c2 = Cpu()
    c2.gpr[1] = 0x70000000
    c2.fpr[5] = 1e300
    c2.pc, c2.lr = 0x82003000, 0
    stfs = struct.pack(">I", (52 << 26) | (5 << 21) | (1 << 16) | 0x40)   # stfs f5,0x40(r1)
    try:
        Tracer(0x82003000, stfs).run(c2, max_steps=2)
    except OverflowError:
        return False
    if not math.isinf(struct.unpack(">f", c2.mem.read(0x70000040, 4))[0]):
        return False
    # vector ops (VMX128, primary opcode 4/5/6) are skipped, not halted on
    vprog = b"".join(struct.pack(">I", w) for w in [
        ppcasm.assemble_one("li r3, 7", 0x82004000), (4 << 26) | 0x123,   # vector
        ppcasm.assemble_one("addi r3, r3, 1", 0x82004008),
        ppcasm.assemble_one("blr", 0x8200400C)])
    c3 = Cpu()
    c3.pc, c3.lr = 0x82004000, 0
    Tracer(0x82004000, vprog).run(c3, max_steps=20)
    if c3.gpr[3] != 8:                          # 7, skip vector, +1 -> 8
        return False
    return True


def main():
    a = sys.argv[1:]
    if a and a[0] == "--selftest":
        print("ppctrace self-test:", "OK (sum 1..10 = 55)" if selftest() else "FAILED")
        sys.exit(0 if selftest() else 1)
    if not a or a[0] in ("-h", "--help"):
        print("usage: ppctrace <xex|raw> --vaddr 0xVA [--rN V ...] [--steps N] [--trace]\n"
              "       ppctrace --selftest\n"
              "  runs the integer core from <vaddr>; prints final GPRs (--trace: each step)")
        return
    if capstone is None:
        print("capstone (PPC) not available: pip install capstone"); return
    path = a[0]
    va = int(a[a.index("--vaddr") + 1], 0) if "--vaddr" in a else None
    steps = int(a[a.index("--steps") + 1], 0) if "--steps" in a else 100000
    do_trace = "--trace" in a
    base, image = _load_image(path)
    cpu = Cpu()
    cpu.mem.write(base, image)
    cpu.gpr[1] = 0x70000000
    for i, tok in enumerate(a):
        if tok.startswith("--r") and tok[3:].isdigit():
            cpu.gpr[int(tok[3:])] = int(a[i + 1], 0) & MASK32
    cpu.pc = va if va is not None else base
    cpu.lr = 0
    tr = [] if do_trace else None
    res = Tracer(base, image).run(cpu, max_steps=steps, trace=tr)
    if do_trace:
        for pc, txt, ch in tr:
            chg = "  ".join("%s=0x%X" % (k, v) for k, v in ch.items())
            print("  0x%08X  %-24s %s" % (pc, txt, chg))
    print("[%s after %d steps]  r3=0x%08X r4=0x%08X r5=0x%08X" % (
        res, cpu.steps, cpu.gpr[3], cpu.gpr[4], cpu.gpr[5]))
    print("  LR=0x%08X CTR=0x%08X CR0=%X" % (cpu.lr, cpu.ctr, (cpu.cr >> 28) & 0xF))


if __name__ == "__main__":
    main()
