#!/usr/bin/env python3
"""lift40 - a Microsoft C 4.0-aware lifter (16-bit MZ -> readable C).

Model (SIGNATURES.md SIG-CG-*): `push bp; mov bp,sp; mov ax,N; call __chkstk`
prologue; cdecl right-to-left calls, caller `add sp` cleanup; [bp+k] args /
[bp-k] locals; absolute [imm16] globals. Recursively structures forward `if` /
`if-else`; datarec resolves DS-offset immediates to string literals. Loops and
irreducible flow are emitted as `/* ... */` raw asm so nothing is faked.

Usage: python lift40.py <unpacked.exe> <hexoff> <size> [chkstk_hexoff] [--names triage.txt]
"""
import sys, struct, re, collections
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
MD = Cs(CS_ARCH_X86, CS_MODE_16); MD.detail = True
GPR = ("ax", "bx", "cx", "dx", "si", "di")
JCC = {"je","jne","jz","jnz","jg","jl","jge","jle","ja","jb","jae","jbe","jc","jnc","js","jns"}
# fall-through (branch NOT taken) condition, i.e. the C `if` guarding the then-block
INV = {"je":"!=","jz":"!=","jne":"==","jnz":"==","jg":"<=","jle":">","jl":">=","jge":"<",
       "ja":"<=","jbe":">","jb":">=","jae":"<","jc":">=","jnc":"<","js":">=","jns":"<"}
TAKEN = {"je":"==","jz":"==","jne":"!=","jnz":"!=","jg":">","jle":"<=","jl":"<","jge":">=",
         "ja":">","jbe":"<=","jb":"<","jae":">=","jc":"<","jnc":">=","js":"<","jns":">="}


def load(path):
    b = open(path, 'rb').read()
    hdr = struct.unpack('<H', b[8:10])[0] * 16
    nrel = struct.unpack('<H', b[6:8])[0]; reloff = struct.unpack('<H', b[24:26])[0]
    img = b[hdr:]
    words = []
    for i in range(nrel):
        o, s = struct.unpack('<HH', b[reloff+i*4:reloff+i*4+4])
        if s*16+o+2 <= len(img):
            words.append(struct.unpack('<H', img[s*16+o:s*16+o+2])[0])
    dgroup = (collections.Counter(words).most_common(1)[0][0] * 16) if words else 0
    return img, dgroup


def names_from_triage(path):
    nm = {}
    if path:
        for l in open(path):
            m = re.match(r"(0x[0-9a-f]+)\t(?:LIB (\S+)|APP)", l)
            if m and m.group(2):
                nm[int(m.group(1), 16)] = m.group(2)
    return nm


def cstr(img, base, off):
    """datarec: DS-offset -> C string literal if it points at a clean string."""
    p = base + off
    if p < 0 or p >= len(img):
        return None
    if p != base and img[p-1] != 0:      # must be a string boundary, not mid-string
        return None
    end = img.find(b"\x00", p, p + 200)
    if end < 0:
        return None
    s = img[p:end]
    if len(s) >= 3 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
        return '"' + s.decode('latin1').replace('\\', '\\\\').replace('"', '\\"')\
            .replace('\n', '\\n').replace('\t', '\\t').replace('\r', '\\r') + '"'
    return None


class Lifter:
    def __init__(self, img, dgroup, names, chkstk):
        self.img, self.dgroup, self.names, self.chkstk = img, dgroup, names, chkstk
        self.faithful = True
        self.reg = {}         # transient GPR values
        self.pushes = []

    def opnd(self, s, as_ptr=False):
        s = s.strip()
        m = re.fullmatch(r"word ptr \[bp \+ (0x[0-9a-f]+|\d+)\]", s)
        if m:
            k = int(m.group(1), 0)
            return "a%d" % ((k-4)//2+1) if k >= 4 else "loc_%d" % k
        m = re.fullmatch(r"word ptr \[bp \- (0x[0-9a-f]+|\d+)\]", s)
        if m:
            return "v%d" % int(m.group(1), 0)
        m = re.fullmatch(r"(?:word|byte) ptr \[(0x[0-9a-f]+)\]", s)
        if m:
            return "g_%04x" % int(m.group(1), 16)
        m = re.fullmatch(r"0x[0-9a-f]+", s)
        if m:
            if as_ptr:
                lit = cstr(self.img, self.dgroup, int(s, 16))
                if lit:
                    return lit
            return s
        return s

    def name_call(self, ops):
        m = re.fullmatch(r"0x([0-9a-f]+)", ops.strip())
        if not m:
            return None, None
        t = int(m.group(1), 16)
        if t == self.chkstk:
            return t, "__chkstk"
        return t, self.names.get(t, "sub_%04x" % t).lstrip("_")

    def stmt_call(self, tgt, fname):
        args = list(reversed(self.pushes)); self.pushes = []
        # datarec: try string literal for immediate args
        pretty = []
        for a in args:
            if re.fullmatch(r"0x[0-9a-f]+", a):
                lit = cstr(self.img, self.dgroup, int(a, 16))
                pretty.append(lit or a)
            else:
                pretty.append(a)
        return "%s(%s);" % (fname, ", ".join(pretty))

    def lift_range(self, ins, lo, hi, depth=0, addr2idx=None):
        """Emit C statements for ins[lo:hi]. Returns list[str]."""
        out, i = [], lo
        if addr2idx is None:
            addr2idx = {ins[k].address: k for k in range(len(ins))}
        if depth > 60:
            self.faithful = False
            return ["/* [AUTO] nesting too deep - declined */"]
        steps = 0
        while i < hi:
            steps += 1
            if steps > 5000:
                self.faithful = False; out.append("/* [AUTO] runaway - declined */"); break
            a = ins[i]; mn, ops = a.mnemonic, a.op_str
            if mn == "mov" and ops == "sp, bp":
                break
            if mn in ("pop", "ret", "retf", "leave"):
                break
            if mn == "push":
                self.pushes.append(self.reg.get(ops, self.opnd(ops))); i += 1; continue
            if mn == "lea":
                m = re.fullmatch(r"(\w+), \[bp \+ (0x[0-9a-f]+|\d+)\]", ops)
                mn2 = re.fullmatch(r"(\w+), \[bp \- (0x[0-9a-f]+|\d+)\]", ops)
                if m:
                    k = int(m.group(2), 0)
                    self.reg[m.group(1)] = "&a%d" % ((k-4)//2+1) if k >= 4 else "&loc_%d" % k
                elif mn2:
                    self.reg[mn2.group(1)] = "&v%d" % int(mn2.group(2), 0)
                else:
                    self.faithful = False
                i += 1; continue
            if mn == "call":
                tgt, fname = self.name_call(ops)
                if fname:
                    if i+1 < hi and ins[i+1].mnemonic == "add" and ins[i+1].op_str.startswith("sp"):
                        i += 1
                    out.append(self.stmt_call(tgt, fname)); i += 1; continue
                self.faithful = False; out.append("/* call %s */" % ops); i += 1; continue
            if mn in ("cmp", "test") and i+1 < hi and ins[i+1].mnemonic in JCC:
                cc = ins[i+1].mnemonic
                mm = re.fullmatch(r"0x([0-9a-f]+)", ins[i+1].op_str.strip())
                tgt = int(mm.group(1), 16) if mm else None
                if mn == "cmp":
                    l, r = [self.opnd(x) for x in ops.split(",")]
                    cond_ft = "%s %s %s" % (l, INV[cc], r)     # fall-through guard
                    cond_tk = "%s %s %s" % (l, TAKEN[cc], r)   # taken guard
                else:
                    l, r = [x.strip() for x in ops.split(",")]
                    base = ("%s" if l == r else "(%s & %s)") % (self.opnd(l), self.opnd(r))
                    cond_ft, cond_tk = base + " == 0", base + " != 0"
                nxt = ins[i+2] if i+2 < hi else None
                jm = re.fullmatch(r"0x([0-9a-f]+)", nxt.op_str.strip()) if (nxt and nxt.mnemonic == "jmp") else None
                # diamond: cmp; jcc Lthen; jmp Lelse  (taken -> then)
                if jm and tgt in addr2idx:
                    Lelse = int(jm.group(1), 16)
                    ti, ei = addr2idx[tgt], addr2idx.get(Lelse, -1)
                    if ei > ti > i:
                        # then = [ti, ei) minus a trailing `jmp Lend`; else = [ei, Lend)
                        tk = ei - 1
                        je = re.fullmatch(r"0x([0-9a-f]+)", ins[tk].op_str.strip()) if ins[tk].mnemonic == "jmp" else None
                        ej = addr2idx.get(int(je.group(1), 16), -1) if je else -1
                        if ej > ei:
                            then = self.lift_range(ins, ti, tk, depth+1, addr2idx)
                            els = self.lift_range(ins, ei, ej, depth+1, addr2idx)
                            out.append("if (%s) {" % cond_tk)
                            out += ["    "+s for s in then] + ["} else {"] + ["    "+s for s in els] + ["}"]
                            i = ej; continue
                        then = self.lift_range(ins, ti, ei, depth+1, addr2idx)   # no else
                        out.append("if (%s) {" % cond_tk)
                        out += ["    "+s for s in then] + ["}"]
                        i = ei; continue
                if tgt in addr2idx and addr2idx[tgt] > i:      # simple forward if (fall-through = then)
                    tj = addr2idx[tgt]
                    then = self.lift_range(ins, i+2, tj, depth+1, addr2idx)
                    out.append("if (%s) {" % cond_ft)
                    out += ["    "+s for s in then] + ["}"]
                    i = tj; continue
                self.faithful = False; out.append("/* %s %s ; %s */" % (mn, ops, cc)); i += 1; continue
            if mn == "jmp":
                mm = re.fullmatch(r"0x([0-9a-f]+)", ops.strip())
                if mm and addr2idx.get(int(mm.group(1), 16), -1) <= i:
                    self.faithful = False; out.append("/* backward jmp %s (loop) */" % ops)
                    break
                self.faithful = False; out.append("/* jmp %s */" % ops); i += 1; continue
            if mn == "mov" and "," in ops:
                draw, sraw = [x.strip() for x in ops.split(",", 1)]
                if draw in ("ax", "sp", "bp") and sraw in ("bp", "sp"):
                    i += 1; continue
                if draw in GPR:
                    self.reg[draw] = self.reg.get(sraw, self.opnd(sraw)); i += 1; continue
                d = self.opnd(draw)
                s = self.reg.get(sraw, self.opnd(sraw, as_ptr=True))
                out.append("%s = %s;" % (d, s)); i += 1; continue
            if mn in ("nop", "cwd", "cbw"):
                i += 1; continue
            self.faithful = False; out.append("/* %s %s */" % (mn, ops)); i += 1
        return out


def lift(img, off, size, chkstk=None, names=None, dgroup=0):
    code = img[off:off+size]
    ins = list(MD.disasm(code, off))
    if not (len(ins) >= 2 and ins[0].mnemonic == "push" and ins[0].op_str == "bp"
            and ins[1].mnemonic == "mov" and ins[1].op_str == "bp, sp"):
        return None, "no C-4.0 prologue (not a standard function)"
    i = 2
    if i+1 < len(ins) and ins[i].mnemonic in ("mov", "xor") and ins[i].op_str.startswith("ax") \
       and ins[i+1].mnemonic == "call":
        i += 2
    while i < len(ins) and ins[i].mnemonic == "push" and ins[i].op_str in ("si", "di"):
        i += 1
    lf = Lifter(img, dgroup, names or {}, chkstk)
    body = lf.lift_range(ins, i, len(ins))
    maxk = max([0] + [int(m.group(1)) for b in body for m in re.finditer(r"\ba(\d+)\b", b)])
    params = ", ".join("int a%d" % k for k in range(1, maxk+1)) or "void"
    ret = "int" if any("return" in b for b in body) else "void"
    src = "%s f_%04x(%s)\n{\n%s}\n" % (ret, off, params, "".join("    "+b+"\n" for b in body))
    return src, ("faithful" if lf.faithful else "partial (has /* */ raw asm)")


def main():
    img, dgroup = load(sys.argv[1])
    off = int(sys.argv[2], 16); size = int(sys.argv[3], 0)
    chk = int(sys.argv[4], 16) if len(sys.argv) > 4 and sys.argv[4].startswith("0x") else None
    names = names_from_triage(sys.argv[sys.argv.index("--names")+1]) if "--names" in sys.argv else {}
    src, note = lift(img, off, size, chk, names, dgroup)
    print("// %s (DGROUP=%#x)\n%s" % (note, dgroup, src))


if __name__ == "__main__":
    main()
