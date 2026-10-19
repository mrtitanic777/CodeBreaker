"""The PowerPC SLEIGH specification checked against a real processor's emulator, instruction by
instruction -- tools/sleigh_verify.py's check, for the specification the PowerPC lane decompiles
with (languages/ppc_64_isa_altivec_be.sla, PowerPC:BE:64:A2ALT-32addr).

    python tools/sleigh_verify_ppc.py [--per-form 3] [--states 8] [--report FILE] [LIBRARY_OR_IMAGE ...]

Takes the instructions real PowerPC code uses -- by default the Xbox 360 SDK's C runtime and
Direct3D libraries (settings.json: powerpc_lib names libcmt.lib; its folder holds the others),
whose code is compiler output and hand-written runtime alike; or the code of the libraries
(.lib), objects (.obj) and images (.exe/.dll/.xex's PE) given. Every distinct form (mnemonic and
operand kinds), up to --per-form encodings each, runs from --states random machine states in
Unicorn (64-bit PowerPC, the 970 model: the PowerPC the Xbox 360's is closest to) and through
the specification's p-code (native/decompiler/build/bin/pcodecheck, mode ppc-be-64), and what
each left is compared: r0-r31, LR, CTR, the condition register's eight fields, XER's SO OV CA,
f0-f31, the bytes of memory written, and where execution goes next.

Unicorn's 64-bit PowerPC cannot have its machine state register set (a write, mtmsr and mtmsrd
all leave it 0): it runs in 32-bit mode with floating point off, and reads XER's SO OV CA back as
0 (they are set by running mtxer, and read by running mfxer). So it is the reference for what
both modes do alike, which with registers holding sign-extended 32-bit values -- the state of
code built for 32-bit addresses -- is nearly everything: the registers are 64 bits in either
mode, and CA comes out the same from such values. What the modes do differently is not compared:
the high word of an effective address (both sides cut it to 32 bits: the language truncates
`ram` to 4 bytes) -- of the register an update form writes it to, and of where a branch goes --
and, where a result does not fit 32 bits, the LT GT EQ of a record form and OV.

What Unicorn will not run in that mode -- the 64-bit instructions (ld, std, rldicl, extsw ...) and
floating point -- is run instead by CodeBreaker's own PowerPC interpreter (emulate/powerpc.py: 64-bit
mode, 32-bit addresses), the one the PowerPC lane verifies with. It is no processor, so a
disagreement there says one of the two is wrong, not which; and it keeps no SO or OV, so those
are not compared (the SO bit of each condition field neither). The report marks those forms "(interp)".

What the architecture leaves undefined is not compared: the high word of mulhw/mulhwu/divw/divwu's
result (and the whole result of a division by zero or an overflowing one), and the LT GT EQ bits
of their record forms then. Memory reads as a fixed pattern on the Unicorn side; the bytes Unicorn
read are handed to the p-code side, which reads zero anywhere else. A case is 'untested' when the
p-code uses an operation the emulator has no meaning for (sync, trap, cache ops ...), when Unicorn
cannot run it (a privileged or Xbox-only instruction: the VMX128 vector set is no 970's), or when
Capstone cannot decode it.

The report: per form, agree / disagree / untested, and for each disagreement what differed."""
import argparse
import collections
import os
import random
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

IP = 0x82000000                              # where a case's instruction is put (an Xbox 360 image's code)
PRE = 0x7F000000                             # mtxer r0, mfxer r0: XER's bits into and out of Unicorn
NOP = b"\x60\x00\x00\x00"                    # after it: the p-code side decodes what follows
PAGE = 0x1000
M64 = (1 << 64) - 1
GPRS = ["r%d" % i for i in range(32)]
FPRS = ["f%d" % i for i in range(32)]
CRS = ["cr%d" % i for i in range(8)]
XER = {"xer_so": 31, "xer_ov": 30, "xer_ca": 29}
UNDEFINED_HIGH = ("mulhw", "mulhwu", "divw", "divwu")
# primary opcode 31: the XO-form arithmetic (its OE bit records overflow) ...
ARITHMETIC = {266, 10, 138, 234, 202, 40, 8, 136, 232, 200, 104, 235, 233, 491, 459, 489, 457}
# ... and the X forms whose result is rA (logical, shifts, extends, counts)
WRITES_RA = {28, 60, 444, 412, 316, 476, 124, 284, 24, 536, 792, 824, 826, 827, 26, 954, 922, 986, 27, 539, 794, 58}
DOUBLES = [0.0, -0.0, 1.0, -1.0, 1.5, -2.25, 3.0e10, -7.5e-5, 0.1, 255.0, 65536.0, 1e300, -1e-300]


def pattern_byte(a):
    return ((a * 2654435761) >> 13) & 0xFF


def _coff_code(obj):
    """[code bytes] of a COFF object's code sections (an Xbox 360 object's headers are little-endian)"""
    if len(obj) < 20:
        return []
    machine, nsec = struct.unpack_from("<HH", obj, 0)
    if machine != 0x01F2:
        return []
    opt = struct.unpack_from("<H", obj, 16)[0]
    out = []
    for i in range(nsec):
        h = 20 + opt + 40 * i
        if h + 40 > len(obj):
            break
        rawsize, rawptr = struct.unpack_from("<II", obj, h + 16)
        chars = struct.unpack_from("<I", obj, h + 36)[0]
        if chars & 0x20 and rawptr and rawsize:
            out.append(obj[rawptr:rawptr + rawsize])
    return out


def _code(path):
    """[code bytes] of a library (every object member), an object, or a PE image"""
    with open(path, "rb") as f:
        data = f.read()
    if data.startswith(b"!<arch>\n"):
        out, at = [], 8
        while at + 60 <= len(data):
            size = int(data[at + 48:at + 58].strip() or 0)
            out += _coff_code(data[at + 60:at + 60 + size])
            at += 60 + size + (size & 1)
        return out
    if data[:2] == b"MZ":
        from codebreaker.formats.pe import open_pe
        return [bytes(raw) for _va, raw, _name in open_pe(path).code_sections()]
    return _coff_code(data)


def forms(modules, per_form, seed):
    """{form: [encoding bytes]} from the code of the given files"""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_64 | capstone.CS_MODE_BIG_ENDIAN)
    md.detail = True
    found = collections.OrderedDict()
    for path in modules:
        for raw in _code(path):
            for k in range(0, len(raw) - 3, 4):
                ins = next(md.disasm(raw[k:k + 4], IP), None)
                if ins is None:
                    continue
                key = (ins.mnemonic, tuple(op.type for op in ins.operands))
                lst = found.setdefault(key, [])
                b = bytes(ins.bytes)
                if b not in lst and len(lst) < per_form * 4:
                    lst.append(b)
    rng = random.Random(seed)
    return {k: rng.sample(v, min(per_form, len(v))) for k, v in found.items()}


def sext(v):
    """a 32-bit value as a 64-bit register holds it, sign-extended"""
    v &= 0xFFFFFFFF
    return v | 0xFFFFFFFF00000000 if v & 0x80000000 else v


def state(rng, mnemonic=""):
    regs = {}
    for r in GPRS:
        regs[r] = sext(rng.choice([
            lambda: rng.getrandbits(32),
            lambda: rng.randrange(0, 70),
            lambda: -rng.randrange(1, 70),
            lambda: rng.choice([0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0x7FFF, 0x8000, 0xFFFF]),
            lambda: 0x40000000 + rng.randrange(0, 0x100000) * 8,          # a pointer
            lambda: 0x82000000 + rng.randrange(0, 0x100000) * 4,          # ... into the image
        ])())
    regs["r1"] = 0x70000000 - rng.choice([0x100, 0x200, 0x1000])          # the stack
    regs["LR"] = sext(0x82000000 + rng.randrange(0, 0x10000) * 4)
    regs["CTR"] = sext(rng.choice([1, 2, rng.randrange(0, 50), 0x82000000 + rng.randrange(0, 0x10000) * 4]))
    if "ctr" in mnemonic:                    # (a branch to CTR: an address, its low bits 0)
        regs["CTR"] = sext(0x82000000 + rng.randrange(0, 0x10000) * 4)
    for c in CRS:
        regs[c] = rng.getrandbits(4)
    for x in XER:
        regs[x] = rng.getrandbits(1)
    for f in FPRS:
        regs[f] = struct.unpack(">Q", struct.pack(">d", rng.choice(DOUBLES) * rng.choice([1, 1, 3, -0.5])))[0]
    return regs


def interp_run(encoding, regs):
    """unicorn_run, by emulate/powerpc.py (see above): memory reads as the pattern"""
    from codebreaker.emulate import powerpc as E
    reads = {}

    class Patterned(E.Machine):
        def _page(self, a, write=True):
            key = a >> 12
            if key not in self.pages:
                self.pages[key] = bytearray(pattern_byte((key << 12) + i) for i in range(PAGE))
            return self.pages[key]

        def read(self, a, n):
            for k in range(n):
                x = (a + k) & 0xFFFFFFFF
                if x not in self.writes:
                    reads.setdefault(x, pattern_byte(x))
            return E.Machine.read(self, a, n)
    m = Patterned([])
    m.load(IP, encoding + NOP)
    m.writes = {}
    m.r = [regs["r%d" % i] for i in range(32)]
    m.f = [regs["f%d" % i] for i in range(32)]
    m.lr, m.ctr, m.ca = regs["LR"], regs["CTR"], regs["xer_ca"]
    m.cr = sum(regs["cr%d" % i] << (28 - 4 * i) for i in range(8))
    m.pc = IP
    try:
        E._step(m, struct.unpack(">I", encoding)[0])
    except Exception as e:                               # (Stop: an instruction it does not do)
        return "untested:interp %s" % (str(e) or type(e).__name__)[:40], None, None, reads, None
    out = {"r%d" % i: m.r[i] & M64 for i in range(32)}
    out.update({"f%d" % i: m.f[i] & M64 for i in range(32)})
    out["LR"], out["CTR"] = m.lr & M64, m.ctr & M64
    for i in range(8):
        out["cr%d" % i] = (m.cr >> (28 - 4 * i)) & 0xF
    out["xer_ca"] = m.ca
    out["xer_so"] = out["xer_ov"] = None                 # (not kept)
    return "interp", out, {a: new for a, (_old, new) in m.writes.items()}, reads, m.pc & M64


def unicorn_run(encoding, regs):
    """-> (outcome, final registers, {address: byte written}, {address: byte read}, next address)"""
    import unicorn as U
    from unicorn import ppc_const as P
    uc = U.Uc(U.UC_ARCH_PPC, U.UC_MODE_PPC64 | U.UC_MODE_BIG_ENDIAN)
    uc.ctl_set_cpu_model(P.UC_CPU_PPC64_970FX_V3_1)

    def fill(page):
        uc.mem_map(page, PAGE)
        uc.mem_write(page, bytes(pattern_byte(a) for a in range(page, page + PAGE)))
    fill(IP & ~(PAGE - 1))
    uc.mem_write(IP, encoding + NOP)
    uc.mem_map(PRE, PAGE)
    uc.mem_write(PRE, bytes.fromhex("7c0103a6" "7c0102a6"))               # mtxer r0; mfxer r0
    uc.reg_write(P.UC_PPC_REG_0, sum(regs[x] << bit for x, bit in XER.items()))
    uc.emu_start(PRE, PRE + 4, count=1)
    for i in range(32):
        uc.reg_write(getattr(P, "UC_PPC_REG_%d" % i), regs["r%d" % i])
        uc.reg_write(getattr(P, "UC_PPC_REG_FPR%d" % i), regs["f%d" % i])
    uc.reg_write(P.UC_PPC_REG_LR, regs["LR"])
    uc.reg_write(P.UC_PPC_REG_CTR, regs["CTR"])
    uc.reg_write(P.UC_PPC_REG_CR, sum(regs["cr%d" % i] << (28 - 4 * i) for i in range(8)))
    reads, writes = {}, {}
    word = struct.unpack(">I", encoding)[0]
    reversed_store = word >> 26 == 31 and (word >> 1) & 0x3FF in (662, 918, 660)   # stwbrx sthbrx stdbrx

    def on_unmapped(uc_, access, addr, size, value, _ud):
        try:
            for page in {addr & ~(PAGE - 1), (addr + size - 1) & ~(PAGE - 1)}:
                fill(page)
        except U.UcError:
            return False
        return True

    def on_mem(uc_, access, addr, size, value, _ud):
        # (a load's bytes read from memory: it may read the instruction's own)
        if access == U.UC_MEM_READ:
            got = uc_.mem_read(addr, size)
            for k in range(size):
                if addr + k not in writes:
                    reads.setdefault(addr + k, got[k])
        else:
            for k in range(size):                        # (big-endian; a byte-reversed store's little)
                writes[addr + k] = (value >> (8 * (k if reversed_store else size - 1 - k))) & 0xFF
    interrupted = []
    uc.hook_add(U.UC_HOOK_MEM_UNMAPPED, on_unmapped)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, on_mem)
    uc.hook_add(U.UC_HOOK_INTR, lambda uc_, n, ud: (interrupted.append(n), uc_.emu_stop()))
    try:
        uc.emu_start(IP, IP + 8, count=1)
    except U.UcError as e:
        return "untested:unicorn %s" % e, None, None, reads, None
    if interrupted:
        return "untested:interrupt", None, None, reads, None
    out = {"r%d" % i: uc.reg_read(getattr(P, "UC_PPC_REG_%d" % i)) & M64 for i in range(32)}
    out.update({"f%d" % i: uc.reg_read(getattr(P, "UC_PPC_REG_FPR%d" % i)) & M64 for i in range(32)})
    out["LR"] = uc.reg_read(P.UC_PPC_REG_LR) & M64
    out["CTR"] = uc.reg_read(P.UC_PPC_REG_CTR) & M64
    cr = uc.reg_read(P.UC_PPC_REG_CR)
    for i in range(8):
        out["cr%d" % i] = (cr >> (28 - 4 * i)) & 0xF
    nxt = uc.reg_read(P.UC_PPC_REG_PC) & M64
    uc.emu_start(PRE + 4, PRE + 8, count=1)
    xer = uc.reg_read(P.UC_PPC_REG_0)
    for x, bit in XER.items():
        out[x] = (xer >> bit) & 1
    return "ok", out, writes, reads, nxt


def _nan(bits):
    return (bits >> 52) & 0x7FF == 0x7FF and bits & ((1 << 52) - 1)


def _undefined(mnemonic, encoding, st, pc):
    """{register: mask of its bits not to compare} -- what the architecture leaves undefined, and
    what 64-bit mode does otherwise than the 32-bit mode the reference runs in"""
    word = struct.unpack(">I", encoding)[0]
    rd, ra, rb = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    out = {}
    primary = word >> 26
    # an update form's base register gets the effective address: its high word is the mode's
    if primary in (33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55) or (primary == 58 and word & 3 == 1) or \
            (primary == 62 and word & 3 == 1) or (primary == 31 and mnemonic.endswith("ux")):
        out["r%d" % ra] = 0xFFFFFFFF00000000
    # a record form, or an overflow-recording one, whose result does not fit 32 bits
    xo9, xo10 = (word >> 1) & 0x1FF, (word >> 1) & 0x3FF
    writes_ra = primary in (20, 21, 23, 24, 25, 26, 27, 28, 29, 30) or (primary == 31 and xo10 in WRITES_RA)
    v = pc.get("r%d" % (ra if writes_ra else rd), 0)
    record = primary in (13, 28, 29) or (word & 1 and primary in (31, 30, 20, 21, 23))   # (addic. andi. andis.)
    overflow = primary == 31 and xo9 in ARITHMETIC and word & 0x400
    if v != sext(v):
        if record:
            out["cr0"] = 0xE
        if overflow:
            out["xer_ov"] = out["xer_so"] = 1
            if record:
                out["cr0"] = 0xF
    if mnemonic.rstrip(".") in ("fctiw", "fctiwz"):
        out["f%d" % rd] = 0xFFFFFFFF00000000          # (the integer is the low word; the high one undefined)
    if mnemonic.rstrip(".") in ("fctiw", "fctiwz", "fctid", "fctidz"):
        x = struct.unpack(">d", struct.pack(">Q", st["f%d" % rb]))[0]
        if x != x or abs(x) >= (2.0 ** 31 if "w" in mnemonic else 2.0 ** 63):
            out["f%d" % rd] = M64                     # (out of range: the processor saturates, the
                                                      # p-code's conversion is the emulating host's)
    base = mnemonic.rstrip(".")
    if base.endswith("o"):
        base = base[:-1]
    if base not in UNDEFINED_HIGH:
        return out
    out["r%d" % rd] = 0xFFFFFFFF00000000
    if base.startswith("div"):
        a, b = st["r%d" % ra] & 0xFFFFFFFF, st["r%d" % rb] & 0xFFFFFFFF
        if b == 0 or (base == "divw" and a == 0x80000000 and b == 0xFFFFFFFF):
            out["r%d" % rd] = M64
    if mnemonic.endswith("."):
        out["cr0"] = 0xE                     # (LT GT EQ: of the undefined high word too)
    return out


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("modules", nargs="*")
    ap.add_argument("--per-form", type=int, default=3)
    ap.add_argument("--states", type=int, default=8)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--report", default=os.path.join(ROOT, "state", "sleigh_verify_ppc.txt"))
    a = ap.parse_args(argv)
    modules = a.modules
    if not modules:
        from codebreaker.base import settings
        lib = settings.get("powerpc_lib") or ""
        modules = [lib] + [os.path.join(os.path.dirname(lib), n) for n in ("d3d9.lib", "xapilib.lib")]
    modules = [m for m in modules if os.path.isfile(m)]
    if not modules:
        raise SystemExit("no PowerPC code to sample (settings.json: powerpc_lib, or name files)")
    tool = os.path.join(ROOT, "native", "decompiler", "build", "bin", "pcodecheck.exe")
    sla = os.path.join(ROOT, "native", "decompiler", "languages", "ppc_64_isa_altivec_be.sla")
    fs = forms(modules, a.per_form, a.seed)
    rng = random.Random(a.seed)
    cases = []                                   # (form, encoding, state, unicorn result)
    for form, encs in fs.items():
        for enc in encs:
            for _ in range(a.states):
                st = state(rng, form[0])
                u = unicorn_run(enc, st)
                if u[0] != "ok":
                    i = interp_run(enc, st)
                    u = i if i[0] == "interp" else (u[0] + "; " + i[0][9:],) + i[1:]
                cases.append((form, enc, st, u))
    names = GPRS + ["LR", "CTR"] + CRS + list(XER) + FPRS
    lines = []
    for k, (form, enc, st, u) in enumerate(cases):
        mem = dict(u[3])
        for addr in (u[2] or {}):                 # (what is written starts as what was there)
            mem.setdefault(addr, pattern_byte(addr))
        for i, b in enumerate(enc + NOP):
            mem[IP + i] = b
        toks = ["%s=%x" % (r, st[r]) for r in names] + ["IP=%x" % IP]
        toks += ["M%x=%02x" % (addr, b) for addr, b in sorted(mem.items())]
        lines.append("c%d %s" % (k, " ".join(toks)))
    p = subprocess.run([tool, sla, "ppc-be-64"], input="\n".join(lines) + "\n", capture_output=True, text=True)
    got = {}
    for ln in p.stdout.splitlines():
        parts = ln.split()
        if len(parts) >= 2:
            got[parts[0]] = parts[1:]
    by_form = collections.OrderedDict()
    for k, (form, enc, st, u) in enumerate(cases):
        rec = by_form.setdefault(form, {"agree": 0, "disagree": 0, "untested": 0, "why": collections.Counter(), "example": None})
        g = got.get("c%d" % k)
        if form[0] in ("mftb", "mftbu", "mfspr") and (struct.unpack(">I", enc)[0] >> 1) & 0x3FF == 371:
            rec["untested"] += 1
            rec["why"]["the time base: a clock"] += 1
            continue
        if u[0] not in ("ok", "interp"):
            rec["untested"] += 1
            rec["why"][u[0][:60]] += 1
            continue
        if not g or g[0] != "ok":
            rec["untested"] += 1
            rec["why"]["pcode " + (" ".join(g) if g else "no answer")[:60]] += 1
            continue
        pc, pw = {}, {}
        for t in g[1:]:
            key, _eq, val = t.partition("=")
            if key.startswith("W"):
                pw[int(key[1:], 16)] = int(val, 16)
            else:
                pc[key] = int(val, 16)
        undef = _undefined(form[0], enc, st, pc) if u[0] == "ok" else _undefined(form[0], enc, st, {})
        if u[0] == "interp":
            rec["interp"] = True
            undef.update({c: undef.get(c, 0) | 1 for c in CRS})
        diffs = []
        for r in names:
            if u[1][r] is None:
                continue
            if r[0] == "f" and _nan(pc.get(r, 0)) and _nan(u[1][r]):
                continue                             # (which NaN: the emulating host's, on the p-code side)
            keep = ~undef.get(r, 0) & M64
            if pc.get(r, 0) & keep != u[1][r] & keep:
                diffs.append("%s %x/%x" % (r, pc.get(r, 0), u[1][r]))
        for addr in sorted(set(u[2]) | set(pw)):
            if u[2].get(addr, pattern_byte(addr)) != pw.get(addr, pattern_byte(addr)):
                diffs.append("mem[%x] %x/%x" % (addr, pw.get(addr, -1), u[2].get(addr, -1)))
                break
        if pc.get("LIN", 0) & 0xFFFFFFFF != u[4] & 0xFFFFFFFF:      # (the mode's: see above)
            diffs.append("next %x/%x" % (pc.get("LIN", 0), u[4]))
        if diffs:
            rec["disagree"] += 1
            for d in diffs:
                rec["why"][d.split()[0]] += 1
            w = struct.unpack(">I", enc)[0]
            given = " ".join("r%d=%x" % (n, st["r%d" % n]) for n in sorted({(w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31}))
            rec["example"] = rec["example"] or "%s: %s  (from %s)" % (enc.hex(), ", ".join(diffs[:4]), given)
        else:
            rec["agree"] += 1
    total = collections.Counter()
    out = []
    for form, rec in sorted(by_form.items(), key=lambda kv: (-kv[1]["disagree"], kv[0][0])):
        status = "DISAGREE" if rec["disagree"] else "agree" if rec["agree"] else "untested"
        total[status] += 1
        name = "%s %s%s" % (form[0], "/".join(str(t) for t in form[1]), " (interp)" if rec.get("interp") else "")
        out.append("%-9s %-32s agree %3d  disagree %3d  untested %3d  %s%s" % (
            status, name[:32], rec["agree"], rec["disagree"], rec["untested"],
            ", ".join("%s x%d" % kv for kv in rec["why"].most_common(4)),
            ("   e.g. " + rec["example"]) if rec["example"] else ""))
    summary = "%d instruction forms from %d files, %d cases: %d agree, %d disagree, %d untested" % (
        len(by_form), len(modules), len(cases), total["agree"], total["DISAGREE"], total["untested"])
    with open(a.report, "w", encoding="utf-8") as f:
        f.write(summary + "\n\n" + "\n".join(out) + "\n")
    print(summary)
    print("report:", a.report)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
