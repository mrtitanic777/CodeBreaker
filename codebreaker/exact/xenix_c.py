"""xenix_c.py -- the C compiler of XENIX for the 8086 (cpp, c0, c1, c2, as; 1982-83), as a toolchain.

The profile (toolchains/xenixc.json) names this module ("plugin"); what cannot be data is here. The
compiler that survives is the Altos ACS 8600 XENIX 2.3 diskette's; the programs Microsoft built
with it (LINK 2.40/2.44, LIB 2.20/2.40, 1983-84) were made by another revision. The variant
"microsoft" is that revision, as far as their bytes tell:

  - c1 without its stack probe (the limit word it keeps: 32767)
  - c1's assembly revised before c2 reads it (rules U D M B: where a value is made)
  - c2: at an unconditional jump its two `mov reg,reg` passes give up (and one of them also at the
    note that ends a function); a jump or call through a pointer counts 6 more to its reckoning of
    how far a jump goes; a constant is not taken from a register a variable lives in; a call through
    a pointer uses every register
  - c2's block moves: a loop's block is left where it is, a block a conditional jump leads to over
    one other changes places with it (hooks at two places of codemove)

Run under the XENIX emulator (emu/xenix.py). The object is a Version 7 a.out (exact/aout.py).
"""
import hashlib
import os
import pickle
import re
import shutil
import struct
import tempfile

from codebreaker.base import settings
from codebreaker.exact import toolchain as tcm

CACHE_VERSION = "x1"


# ---- c1's assembly, revised (Microsoft's c1)
PAIRS = {("si", "di"), ("dx", "si")}
OPS = "add|sub|and|or|xor|sal|shl|shr|sar|neg|not|inc|dec"


def rule_u(t):
    """an unsigned word made a long: this c1 loads the low register of the pair and clears the high one;
    Microsoft's made the long in AX:DX and moved it to the pair if it was wanted in one"""
    L = t.split("\n")
    k = 0
    while k < len(L):
        m = re.fullmatch(r"sub\t(\w\w),(\w\w)", L[k])
        if m and m.group(1) == m.group(2):
            hi = m.group(1)
            lo = {h: l for l, h in PAIRS}.get(hi)
            j = k
            while lo and j > 0 and (re.fullmatch(r"(?:mov|%s)\t%s(?:,.*)?" % (OPS, lo), L[j - 1]) or (j < k and re.fullmatch(r"mov\tcx,.*", L[j - 1]))):
                j -= 1
            if lo and j > 0 and j == k or (lo and j < k and not L[j].startswith("mov\t")):
                j = j - 1 if lo and j > 0 and re.fullmatch(r"(?:L\d+:)+mov\t%s,.*" % lo, L[j - 1]) else k
            m2 = re.fullmatch(r"((?:L\d+:)*)mov\t(\w\w),(.*)", L[j]) if lo and j < k else None
            direct = (m2 and m2.group(3) == "ax" and j + 1 == k and k + 3 < len(L) and re.fullmatch(r"mov\t\S*[(_]\S*,%s" % lo, L[k + 1])
                      and re.fullmatch(r"mov\t\S*[(_]\S*,%s" % hi, L[k + 2]) and re.match(r"jmp\t", L[k + 3]))
            if m2 and m2.group(2) == lo and "(%s)" % lo not in m2.group(3) and not direct:
                pushed = L[k + 1:k + 3] == ["push\t%s" % lo, "push\t%s" % hi]
                if j + 1 == k and L[k + 1:k + 3] == ["mov\tax,%s" % lo, "mov\tdx,%s" % hi]:
                    new = ([] if m2.group(3) == "ax" and not m2.group(1) else ["%smov\tax,%s" % (m2.group(1), m2.group(3))]) + ["sub\tdx,dx"]
                    L[j:k + 3] = new
                    k = j + len(new) - 1
                else:
                    if j + 1 == k:
                        new = ["%smov\tax,%s" % (m2.group(1), m2.group(3))]
                    else:
                        new = ["%smov\t%s,%s" % (m2.group(1), hi, m2.group(3))] + [x.replace(lo, hi) for x in L[j + 1:k]] + ([] if pushed else ["mov\tax,%s" % hi])
                    new.append("sub\tdx,dx")
                    if pushed:
                        new += ["push\t%s" % ("ax" if j + 1 == k else hi), "push\tdx"]
                        end = k + 3
                    else:
                        new += ["mov\t%s,dx" % hi, "mov\t%s,ax" % lo]
                        end = k + 1
                    L[j:end] = new
                    k = j + len(new) - 1
        k += 1
    return "\n".join(L)


def rule_d(t):
    """a ++ or -- put off until its byte is fetched: Microsoft's c1 does it when the byte is the value wanted"""
    t = re.sub(r"(movb\tdx,[^\n]+\n)((?:inc|dec)\t(?:\w\w|[*#]?-?\d+\.\(bp\)|_\w+)\n)(and\tdx,#255\.\n|mov\t(?:si|di),dx\n)", r"\1\3\2", t)

    def through_di(m):
        n = re.findall(r"\|NREG (\d+)", t[:m.start()])
        if not n or n[-1] != "3":
            return m.group(0)
        return "mov\tdi,%s\nmovb\tax,(di)\ncbw\n%s\t%s\nadd\tax," % (m.group(1), m.group(2), m.group(1))
    return re.sub(r"mov\tax,(\*-?\d+\.\(bp\))\nmov\tbx,ax\nmovb\tax,\(bx\)\ncbw\n(inc|dec)\t\1\nadd\tax,", through_di, t)


def rule_m(t):
    """op (R),M with M in memory (no 8086 instruction): Microsoft's c1 brought M to AX first"""
    mem = r"(?:[*#]?-?\d+\.)?(?:\+?_\w+)?\((?:bp|bx|si|di)\)|(?:\d+\.\+)?_\w+"
    return re.sub(r"(^|\n)((?:L\d+:)*)(add|sub|and|or|xor)\t(\((?:bx|si|di)\)),(%s)(?=\n)" % mem,
                  lambda m: "%s%smov\tax,%s\n%s\t%s,ax" % (m.group(1), m.group(2), m.group(5), m.group(3), m.group(4)), t)


def rule_b(t):
    """a char made an int OR'ed with a constant: Microsoft's c1 did it on the word"""
    return re.sub(r"(^|\n)((?:L\d+:)*)orb\t(ax|dx|bx|cx),(#\d+\.)(?=\n)", r"\1\2or\t\3,\4", t)


RULES = {"U": rule_u, "D": rule_d, "M": rule_m, "B": rule_b}


# ---- c2's block moves (Microsoft's c2), from outside: hooks at codemove
JBR, CBR, JMP, LABEL = 1, 2, 3, 4


class _List(object):
    def __init__(self, uc, ds):
        self.uc, self.b = uc, ds * 16

    def w(self, a):
        return struct.unpack("<H", bytes(self.uc.mem_read(self.b + a, 2)))[0]

    def setw(self, a, v):
        self.uc.mem_write(self.b + a, struct.pack("<H", v & 0xFFFF))

    def op(self, p):
        return bytes(self.uc.mem_read(self.b + p, 1))[0]

    def sub(self, p):
        return bytes(self.uc.mem_read(self.b + p + 1, 1))[0]

    def forw(self, p):
        return self.w(p + 2)

    def back(self, p):
        return self.w(p + 4)

    def ref(self, p):
        return self.w(p + 6)

    def link(self, a, b):
        self.setw(a + 2, b)
        self.setw(b + 4, a)

    def unlink(self, p):
        self.link(self.back(p), self.forw(p))


def _one_block(l, p1):
    L = l.forw(p1)
    if l.op(L) != LABEL:
        return None
    t = l.ref(p1)
    while l.op(l.back(t)) == LABEL:
        t = l.back(t)
    bend = l.back(t)
    if l.op(bend) not in (JBR, JMP):
        return None
    p = l.forw(L)
    while p != bend:
        if p == 0 or l.op(p) in (JBR, JMP, LABEL):
            return None
        p = l.forw(p)
    return L, bend, t


def _swap(l, p1):
    L, bend, t = _one_block(l, p1)
    T = l.ref(p1)
    if l.w(L + 0xC) != 1:
        return False
    q = l.back(p1)
    n = 0
    while not (l.op(q) == CBR and l.ref(q) == L):
        if q == 0 or l.op(q) in (JBR, JMP, LABEL) or n > 64:
            return False
        q = l.back(q)
        n += 1
    x = l.forw(q)
    if x == p1:
        return False
    b0 = l.forw(L)
    l.link(l.back(p1), t)
    l.link(q, b0)
    l.link(bend, L)
    l.link(L, x)
    rev = bytes(l.uc.mem_read(l.b + 0x106 + l.sub(q), 1))[0]
    l.uc.mem_write(l.b + q + 1, bytes([rev]))
    c = l.w(T + 0xC) - 1
    l.setw(T + 0xC, c)
    if c <= 0:
        l.unlink(T)
    l.setw(0x800, l.w(0x800) + 1)
    return q


def c2_hooks(mode="RS"):
    from unicorn.x86_const import UC_X86_REG_DI, UC_X86_REG_SI, UC_X86_REG_BP, UC_X86_REG_DX, UC_X86_REG_IP
    pending = []

    def at_ce3(uc, cs, ds):
        l = _List(uc, ds)
        p1 = uc.reg_read(UC_X86_REG_DI)
        bp = uc.reg_read(UC_X86_REG_BP)
        p3 = l.w(bp - 6)
        if p3 == p1 or l.ref(p3) != l.ref(p1) or not _one_block(l, p1):
            return
        if "S" in mode:
            q = _swap(l, p1)
            if q:
                uc.reg_write(UC_X86_REG_DX, q)
                uc.reg_write(UC_X86_REG_IP, 0xE5A)
                return
        pending.append(p1)
        l.setw(bp + 4, p3)
        uc.reg_write(UC_X86_REG_DI, p3)
        uc.reg_write(UC_X86_REG_SI, l.back(uc.reg_read(UC_X86_REG_SI)))
        uc.reg_write(UC_X86_REG_IP, 0xD4F)

    def at_e5a(uc, cs, ds):
        if pending:
            uc.reg_write(UC_X86_REG_DX, pending.pop())
    return {0xCE3: at_ce3, 0xE5A: at_e5a}


# ---- the passes, changed as Microsoft's
def _c1(b):
    q = 16 + struct.unpack_from("<H", b, 2)[0] + 0x472
    if struct.unpack_from("<H", b, q)[0] != 100:
        raise RuntimeError("xenixc: not the c1 this was written for")
    struct.pack_into("<H", b, q, 0x7FFF)
    return b


def _c2(c2):
    C2FIX = [(0x2F82, "7417", "74e7"), (0x332A, "803c017509837ef6ff7418e9ccfe", "803c017405803c3f7504e9cdfe")]
    for at, old, new in C2FIX:
        old, new = bytes.fromhex(old), bytes.fromhex(new)
        if c2[16 + at:16 + at + len(old)] != old:
            raise RuntimeError("xenixc: not the c2 this was written for")
        c2[16 + at:16 + at + len(new)] = new
    tsz = struct.unpack_from("<H", c2, 2)[0]
    if not (struct.unpack_from("<H", c2, 16 + 0x1885)[0] == 0x17BD and struct.unpack_from("<H", c2, 16 + 0x18E1)[0] == 0x17BD):
        raise RuntimeError("xenixc: not the c2 this was written for")
    # a jump or call through a pointer counts 6 more (the figure for jumps and calls, 17BD, through a stub)
    stub = bytes.fromhex("8b5c0a" "803f40" "7504" "8346fa06") + bytes([0xE9]) + struct.pack("<h", 0x17BD - (tsz + 15))
    c2[16 + tsz:16 + tsz] = stub
    struct.pack_into("<H", c2, 2, tsz + len(stub))
    struct.pack_into("<H", c2, 16 + 0x1885, tsz)
    struct.pack_into("<H", c2, 16 + 0x18E1, tsz)
    # a constant is not taken from SI or DI while a variable lives there (1DDE)
    at4 = tsz + len(stub)
    s4 = b""

    def J(op, to):
        return bytes([op]) + struct.pack("<h", to - (at4 + len(s4) + 3))
    s4 = bytes.fromhex("807f0164" "7503")
    s4 += J(0xE9, 0x1E3B)
    s4 += bytes.fromhex("803e8c092a" "752b" "83ff02" "7c26" "83ff03" "7f21" "8bc7" "48" "39060208" "7f18" "b82800" "50" "b88c09" "50")
    s4 += J(0xE8, 0x410E)
    s4 += bytes.fromhex("83c404" "85c0" "7506" "bfffff")
    s4 += J(0xE9, 0x1E24)
    s4 += J(0xE9, 0x1E64)
    if c2[16 + 0x1E35:16 + 0x1E3B] != bytes.fromhex("807f01647529"):
        raise RuntimeError("xenixc: not the c2 this was written for")
    c2[16 + at4:16 + at4] = s4
    stub += s4
    struct.pack_into("<H", c2, 2, at4 + len(s4))
    c2[16 + 0x1E35:16 + 0x1E3B] = bytes([0xE9]) + struct.pack("<h", at4 - 0x1E38) + bytes([0x90] * 3)
    # a call through a pointer uses every register (2788, and the fold of a load into its push, 2436)
    at5 = tsz + len(stub)
    s5 = bytes.fromhex("c6440fff" "8b5c0a" "803f40" "7403")
    s5 += bytes([0xE9]) + struct.pack("<h", 0x2A6E - (at5 + len(s5) + 3))
    s5 += bytes.fromhex("c6440eff")
    s5 += bytes([0xE9]) + struct.pack("<h", 0x2C74 - (at5 + len(s5) + 3))
    at6 = at5 + len(s5)
    s6 = bytes.fromhex("803c2f" "7403")
    s6 += bytes([0xE9]) + struct.pack("<h", 0x243B - (at6 + len(s6) + 3))
    s6 += bytes.fromhex("8b5c0a" "803f40" "7503")
    s6 += bytes([0xE9]) + struct.pack("<h", 0x2487 - (at6 + len(s6) + 3))
    s6 += bytes([0xE9]) + struct.pack("<h", 0x2453 - (at6 + len(s6) + 3))
    if c2[16 + 0x2436:16 + 0x243B] != bytes.fromhex("803c2f7418") or struct.unpack_from("<H", c2, 16 + 0x2C52)[0] != 0x2A6A:
        raise RuntimeError("xenixc: not the c2 this was written for")
    c2[16 + 0x2436:16 + 0x243B] = bytes([0xE9]) + struct.pack("<h", at6 - 0x2439) + bytes([0x90] * 2)
    s5 += s6
    c2[16 + at5:16 + at5] = s5
    struct.pack_into("<H", c2, 2, at5 + len(s5))
    struct.pack_into("<H", c2, 16 + 0x2C52, at5)
    return c2


class XenixC(object):
    def __init__(self, profile, variant=None):
        self.p = profile
        self.name = profile["name"]
        self.variant = variant or profile.get("default_variant") or "microsoft"
        self.key = hashlib.sha1((repr(sorted(profile.items())) + self.variant + CACHE_VERSION).encode()).hexdigest()[:12]
        v = profile.get("variants", {}).get(self.variant, {})
        self.rules = v.get("rules", "")
        self.patch = v.get("patch", False)
        self.c2move = v.get("c2move", "")

    def compile(self, src, headers=None, include=(), switches=None, cache=True):
        src = src.replace(b"\r\n", b"\n")
        hs = sorted((headers or {}).items())
        incs = []
        for d in include:
            for f in sorted(os.listdir(d)):
                if f.lower().endswith(".h"):
                    incs.append((f, open(os.path.join(d, f), "rb").read()))
        key = hashlib.sha1(b"\0".join([src, repr(hs).encode(), repr(incs).encode(), self.key.encode()])).hexdigest()
        cf = settings.state("exact", "cache", key[:2], key)
        if cache and os.path.exists(cf):
            with open(cf, "rb") as f:
                return pickle.load(f)
        res = self._compile(src, hs + incs)
        os.makedirs(os.path.dirname(cf), exist_ok=True)
        with open(cf, "wb") as f:
            pickle.dump(res, f)
        return res

    def _compile(self, src, headers):
        from codebreaker.exact.emu import xenix
        root = tcm.kit_dir(self.p["kit"])
        w = tempfile.mkdtemp(prefix="xenixc_")
        log = ""
        try:
            os.makedirs(os.path.join(w, "tmp"))
            os.makedirs(os.path.join(w, "lib"))
            open(os.path.join(w, "x.c"), "wb").write(src)
            for f, b in headers:
                open(os.path.join(w, f), "wb").write(b.replace(b"\r\n", b"\n"))
            c1 = bytearray(open(os.path.join(root, "lib", "c1"), "rb").read())
            c2 = bytearray(open(os.path.join(root, "lib", "c2"), "rb").read())
            if self.patch:
                c1 = _c1(c1)
                c2 = _c2(c2)
            open(os.path.join(w, "lib", "c1"), "wb").write(c1)
            open(os.path.join(w, "lib", "c2"), "wb").write(c2)
            hooks = {"lib/c2": c2_hooks(self.c2move)} if self.c2move else {}
            steps = [("lib/cpp", ["x.c", "x.i"]), ("lib/c0", ["x.i", "t1", "t2"]), ("lib/c1", ["t1", "t2", "x.s"]),
                     ("lib/c2", ["x.s", "x.o.s"]), ("bin/as", ["-o", "x.o", "x.o.s"])]
            for prog, args in steps:
                r = xenix.run(root, w, prog, args, hooks=hooks.get(prog))
                log += r.console.decode("latin1")
                if r.status != 0 or r.error:
                    return None, log + "%s: exit %s %s\n(the pass that stopped: %s)\n" % (prog, r.status, r.error or "", prog)
                if prog == "lib/c1" and self.rules:
                    f = os.path.join(w, "x.s")
                    t = open(f, "rb").read().decode("latin1")
                    for n in self.rules:
                        t = RULES[n](t)
                    open(f, "wb").write(t.encode("latin1"))
            return open(os.path.join(w, "x.o"), "rb").read(), log
        finally:
            shutil.rmtree(w, ignore_errors=True)
