#!/usr/bin/env python3
"""cmpp.py - a Pascal procedure compiled by the original compiler, compared with the executable.

  python cmpp.py M.EXE 0x00001 m100/p00001.pas [--proc NAME] [--asm] [--cod]

The file is a small complete Pascal program: the declarations the procedure needs, stubs for the
procedures it calls, and the procedure itself, named P<address> (P00001 for the one at image
0x00001) unless --proc says otherwise. It is compiled with the MS-Pascal compiler of 1981
(tools/pcc.py) and the procedure's bytes are compared with the executable's. Not compared, because
they depend on where things are placed: the bytes a linker fills in (addresses of variables and
constants, far call targets) and the displacement of a near call to another procedure.
Prints IDENTICAL, or the instructions that differ. --asm prints the original procedure and
compiles nothing; --cod prints the compiler's own listing of the compiled procedure.
"""
import os, sys, re, json, struct
from codebreaker import platforms
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
MD = platforms.decoder(16)

HDRLEN = 3          # what follows a procedure's call of the entry helper: a byte and a word (build.py sets 2 for MASM 3.00)

def walk(b, base=0, skip_hdr=True):
    """-> [(offset, size, text, mnemonic)]; the db/dw after the entry call are data"""
    out = []; p = 0; n = len(b)
    while p < n:
        if skip_hdr and p == 3 and b[0] == 0xE8:
            if HDRLEN == 2:                             # MS-Pascal 3.x: only the frame size behind the call
                out.append((3, 2, "dw 0x%04x" % struct.unpack_from("<H", b, 3)[0], "hdr")); p = 5; continue
            out.append((3, 3, "db 0x%02x ; dw 0x%04x" % (b[3], struct.unpack_from("<H", b, 4)[0]), "hdr")); p = 6; continue
        i = next(MD.disasm(b[p:p + 16], base + p), None)
        if i is None: out.append((p, 1, "db 0x%02x" % b[p], "db")); p += 1
        else: out.append((p, i.size, "%s %s" % (i.mnemonic, i.op_str), i.mnemonic)); p += i.size
    return out

def loose_bytes(b, fx=()):
    """offsets not compared: fixups, near call displacements to outside, far pointers"""
    m = set(fx); n = len(b)
    for off, sz, t, mn in walk(b):
        if mn == "hdr": continue                        # (the entry helper's data, not code: its first byte is no opcode)
        if b[off] == 0xE8 and sz == 3:
            tgt = (off + 3 + struct.unpack_from("<h", b, off + 1)[0])
            if not (0 <= tgt < n): m.update((off + 1, off + 2))
        elif b[off] in (0x9A, 0xEA) and sz == 5: m.update(range(off + 1, off + 5))
    return m

def main():
    a_ = [x for x in sys.argv[1:] if not x.startswith("--")]
    opt = lambda k, d: (sys.argv[sys.argv.index(k) + 1] if k in sys.argv else d)
    if "--proc" in sys.argv: a_ = [x for x in a_ if x != opt("--proc", None)]
    exe, a = a_[0], int(a_[1], 16)
    import pcc
    img = pcc.image(exe)
    L = json.load(open(os.path.join(HERE, "..", "ledger", exe.upper() + ".json")))
    size = L["0x%05x" % a]["size"]; want = img[a:a + size]
    if "--asm" in sys.argv:
        print("== %s 0x%05x  %d bytes" % (exe, a, size))
        for off, sz, t, mn in walk(want, a): print("%05x %-14s %s" % (a + off, want[off:off + sz].hex()[:14], t))
        return
    r = pcc.compile_pas(a_[2])
    name = opt("--proc", "P%05X" % a).upper()
    if r.code is None or name not in r.procs:
        print("NO CODE for %s" % name)
        txt = r.lst if "Error" in r.console or "rror" in r.lst else r.console
        bad = [l for l in r.lst.split("\n") if re.search(r"\^|Error|Warning", l)]
        print("\n".join(bad[:30]) if bad else r.console[-600:]); sys.exit(2)
    o, n = r.procs[name]; got = r.code[o:o + n]; fx = {k - o for k in r.fix if o <= k < o + n}
    if "--cod" in sys.argv:
        on = False
        for line in r.cod.split("\n"):
            m = re.match(r"\s*Procedure/Function : (\S+)", line)
            if m: on = m.group(1).upper() == name
            if on: print(line)
    mg = loose_bytes(got, fx); mw = loose_bytes(want)
    if n == size and all(k in mg or got[k] == want[k] for k in range(n)):
        print("IDENTICAL  %s 0x%05x  %d bytes (%d not compared: link-time addresses, calls to other procedures)" % (exe, a, n, len(mg))); return
    print("DIFFERS  compiled %d bytes, original %d" % (n, size))
    A = walk(want, a); B = walk(got, a); NUM = re.compile(r"0x[0-9a-f]+|\b\d+\b")
    nz = lambda t: NUM.sub("N", t)
    i = j = 0; shown = 0
    while i < len(A) and j < len(B):
        (oa, sa, ta, ma), (ob, sb, tb, mb) = A[i], B[j]
        lo = any((ob + k) in mg for k in range(sb))
        same = (nz(ta) == nz(tb)) if lo else (want[oa:oa + sa] == got[ob:ob + sb])
        if same: i += 1; j += 1; continue
        if ma == mb and sa == sb and shown < 12:
            print("  %05x  %-34s | %s" % (a + oa, ta, tb)); shown += 1; i += 1; j += 1; continue
        print("  out of step at original %05x / compiled +%d:" % (a + oa, ob))
        for k in range(14):
            l = "%05x %s" % (a + A[i + k][0], A[i + k][2]) if i + k < len(A) else ""
            rr = ("%s%s" % ("*" if any((B[j + k][0] + q) in mg for q in range(B[j + k][1])) else " ", B[j + k][2])) if j + k < len(B) else ""
            print("    %-40s | %s" % (l, rr))
        sys.exit(1)
    sys.exit(1)

if __name__ == "__main__":
    main()
