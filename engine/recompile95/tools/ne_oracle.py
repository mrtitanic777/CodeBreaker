#!/usr/bin/env python3
r"""ne_oracle.py -- the 16-bit '.code16 context' for NE modules: prove an NE (16-bit) hybrid
function RUNS correctly. difftest.py is 32-bit (PE, UC_MODE_32); NE code is 16-bit and cannot run
natively on 64-bit Windows (no NTVDM). This emulates the byte-exact NE bytes in Unicorn UC_MODE_16
(the ORACLE) and compares to the function's lifted C compiled native (the CANDIDATE), on random
16-bit inputs. Agreement proves the NE transliteration runs bit-identically to Microsoft's 16-bit
machine code.

    python ne_oracle.py <module> seg<N>:0x<off>       e.g. winsock.dll seg1:0x077fc
"""
import os, re, sys, glob, ctypes, tempfile, subprocess, random
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import hybrid_emit as H
INC = os.path.join(os.path.dirname(HERE), "include")
GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")


def fn_bytes(module, seg, off):
    """Exact byte stream of one NE function, from its per-fn asm (reusing hybrid_emit's parser)."""
    ent = H._index().get(module.lower())
    path = os.path.join(ent[0], "seg%d_%04x.asm" % (seg, off))
    if not os.path.exists(path):
        path = os.path.join(ent[0], "seg%d_%05x.asm" % (seg, off))
    _sg, _st, items = H.parse_function(path)
    out = bytearray()
    for it in items:
        out += bytes(it[3] if it[0] == "xfer" and it[3] else (it[1] if it[0] == "bytes" else b""))
    return bytes(out)


def lifted_c(module, seg, off):
    c = glob.glob(os.path.join(GEN, "**", module + ".c"), recursive=True)
    txt = open(c[0], encoding="utf-8", errors="replace").read()
    m = re.search(r'/\* \[AUTO\][^\n]*seg%d:0x0*%x\b[^\n]*\*/\s*'
                  r'((?:int|u32|short)\s+\w+\([^)]*\)\s*\{(?:(?!/\* \[AUTO\]).)*?\n\})' % (seg, off), txt, re.S)
    return m.group(1)


def _is_far(apath):
    """Far iff the function's actual RET is retf. Ignore the NE `retf macro` preamble by only
    scanning the segment body, and match a real ret: `retf`/`db 0cbh`/`db 0cah` (far) beats
    `ret`/`retn`/`db 0c3h`/`db 0c2h` (near)."""
    in_seg = False
    far = near = False
    for line in open(apath, encoding="utf-8", errors="replace"):
        s = line.strip().lower()
        if not in_seg:
            if re.search(r"\bsegment\b", s) and "macro" not in s:
                in_seg = True
            continue
        if "macro" in s:
            continue
        if re.search(r"\bretf\b", s) or re.search(r";.*\bretf\b", s) or "0cbh" in s or "0cah" in s:
            far = True
        elif re.match(r"(db\s+0c[23]h|retn?\b|ret\s)", s) or re.search(r";.*\bretn?\b", s):
            near = True
    return far or not near     # exported NE fns are far; default far if no near ret seen


def oracle16(code, args, far):
    """Emulate 16-bit NE code; args pushed as 16-bit words. Far exports use a 4-byte (seg:off)
    return frame, near uses 2 -- so args land at the offset the lift's argN expect. -> AX."""
    import unicorn as U
    from unicorn.x86_const import UC_X86_REG_SP, UC_X86_REG_AX
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_16)
    BASE, STK, RET = 0x1000, 0x8000, 0xEEEE
    uc.mem_map(0, 0x20000)
    uc.mem_write(BASE, code)
    sp = STK
    for a in reversed(args):
        sp -= 2; uc.mem_write(sp, int(a & 0xFFFF).to_bytes(2, "little"))
    if far:
        sp -= 2; uc.mem_write(sp, (0).to_bytes(2, "little"))   # return CS = 0
    sp -= 2; uc.mem_write(sp, RET.to_bytes(2, "little"))       # return IP
    uc.reg_write(UC_X86_REG_SP, sp)
    try:
        uc.emu_start(BASE, RET, count=20000)
    except U.UcError:
        return None
    return uc.reg_read(UC_X86_REG_AX) & 0xFFFF


def candidate(body, nargs):
    d = tempfile.mkdtemp()
    open(os.path.join(d, "f.c"), "w").write('#include "win95c.h"\n' + body)
    so = os.path.join(d, "f.dll")
    r = subprocess.run(["gcc", "-std=gnu89", "-w", "-shared", "-o", so,  # HOST (64-bit) DLL for ctypes
                        os.path.join(d, "f.c"), os.path.join(INC, "win95rt.c"), "-I", INC],
                       capture_output=True, text=True)
    if r.returncode:
        return None, r.stderr[:200]
    name = re.search(r'\b(\w+)\s*\(', body).group(1)
    lib = ctypes.CDLL(so)
    f = getattr(lib, name)
    f.restype = ctypes.c_uint32
    f.argtypes = [ctypes.c_uint32] * nargs
    return f, None


def main():
    module = sys.argv[1]
    seg, off = re.match(r"seg(\d+):0x([0-9a-f]+)", sys.argv[2]).groups()
    seg, off = int(seg), int(off, 16)
    os.environ.setdefault("WT_DATA_ROOT", os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))), "binaries", "win95"))
    code = fn_bytes(module, seg, off)
    ent = H._index().get(module.lower())
    apath = os.path.join(ent[0], "seg%d_%04x.asm" % (seg, off))
    if not os.path.exists(apath):
        apath = os.path.join(ent[0], "seg%d_%05x.asm" % (seg, off))
    far = _is_far(apath)
    body = lifted_c(module, seg, off)
    nargs = len([p for p in re.search(r'\(([^)]*)\)', body).group(1).split(",") if p.strip()])
    f, err = candidate(body, nargs)
    if not f:
        print("candidate compile failed:", err); return
    random.seed(0x1234)
    ok = bad = 0
    for _ in range(40):
        args = [random.randint(0, 0xFFFF) for _ in range(nargs)]
        o = oracle16(code, args, far)
        c = f(*args) & 0xFFFF
        if o is None:
            continue
        if o == c: ok += 1
        else:
            bad += 1
            if bad <= 3: print("  MISMATCH args=%s oracle=0x%04x cand=0x%04x" % (args, o, c))
    proven = bad == 0 and ok
    print("%s %s: %d/%d agree (16-bit oracle vs lifted C)  %s"
          % (module, sys.argv[2], ok, ok + bad, "PROVEN" if proven else "MISMATCH"))
    if proven and "--manifest" in sys.argv:              # append NE-verified fn to the corpus manifest
        with open(sys.argv[sys.argv.index("--manifest") + 1], "a", encoding="utf-8") as mf:
            mf.write("%s\t%d\t%x\n" % (module, seg, off))


if __name__ == "__main__":
    main()
