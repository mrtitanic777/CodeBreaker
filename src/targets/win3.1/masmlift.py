#!/usr/bin/env python3
"""
masmlift.py — assemble reconstructed 16-bit ASM with the period MASM (stage F).

Much of KERNEL/USER/GDI was hand-written assembly, and assembly can only be
proven byte-exact by RE-ASSEMBLING it and diffing the bytes — translating it to
C never reproduces the original encoding. MASM is a 16-bit DOS program (no NTVDM
on x64), so like `cc16` drives CL, this drives MASM.EXE under the vendored
DOSBox-X.

Crucially, MASM emits an OMF `.OBJ` exactly like the C compiler, so the whole
downstream is REUSED: `objextract` pulls the code bytes + relocation mask and
`segdiff` renders the byte-exact verdict. This module only has to run the
assembler — the acceptance test for hand-asm is then identical to the C path.

    python masmlift.py --selftest            # assemble a tiny proc, extract it
    python masmlift.py foo.asm               # assemble foo.asm, dump OBJ segments
"""
import argparse
import os
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.abspath(__file__))
DBX = r"dosbox-x"
MASMROOT = r"reference/build16"      # has BIN/INC/LIB

CONF = r"""[sdl]
autolock=false
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=6.22
[autoexec]
mount c "{masm}"
mount d "{work}"
set PATH=C:\BIN
set INCLUDE=C:\INC
set LIB=C:\LIB
set TMP=D:\
d:
{cmd}
exit
"""


def assemble(asm_src, stem="t", timeout=180, masmroot=None):
    """Assemble asm_src with MASM under DOSBox-X. Returns {obj, lst, log, work}.

    masmroot selects the assembler: the default (MASM 4.00, 16-bit, proven
    byte-exact for the NE core) or an override such as the vendored MASM 5.10A
    (32-bit `.386`/USE32) that the PE path in asmrecon32 needs."""
    masm = masmroot or MASMROOT
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    if not os.path.exists(os.path.join(masm, "BIN", "MASM.EXE")):
        sys.exit("MASM.EXE not found under %s\\BIN" % masm)
    work = tempfile.mkdtemp(prefix="masm_")
    with open(os.path.join(work, stem + ".ASM"), "w", newline="\r\n") as f:
        f.write(asm_src)
    cmd = "MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG 2>&1" % (stem, stem, stem, stem)
    conf = os.path.join(work, "masm.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(CONF.format(masm=masm, work=work, cmd=cmd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    # Run DOSBox as a Popen and poll a cancel check so a Cancel press kills the
    # assembler immediately instead of waiting out the run (fast power-off).
    try:
        from src.core import progress as _pr
        aborted = _pr.cancelled
    except Exception:
        aborted = lambda: False
    proc = subprocess.Popen([DBX, "-conf", conf, "-nolog"], env=env,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    waited = 0.0
    while proc.poll() is None:
        if aborted():
            proc.kill()
            proc.wait(timeout=5)
            return {"work": work, "aborted": True}
        time.sleep(0.05)
        waited += 0.05
        if waited > timeout:
            proc.kill()
            break
    out = {"work": work}
    for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
        p = os.path.join(work, stem + ext)
        if os.path.exists(p):
            out[key] = open(p, "rb").read() if ext == ".OBJ" else \
                open(p, encoding="latin1").read()
    return out


def assemble_many(items, timeout=None, masmroot=None):
    """Assemble MANY (stem, asm_src) pairs in ONE DOSBox session — one boot, then
    MASM run per file. Returns {"work": dir, "results": {stem: {obj,lst,log}}} (or
    {"aborted": True} on Cancel).

    The DOSBox BOOT is the dominant cost (~0.5-2 s); a single reconstruction pass
    over a whole chunk of functions therefore costs one boot instead of one boot
    PER function. Stems must be unique and DOS 8.3-safe (callers use F0..FN)."""
    masm = masmroot or MASMROOT
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    if not os.path.exists(os.path.join(masm, "BIN", "MASM.EXE")):
        sys.exit("MASM.EXE not found under %s\\BIN" % masm)
    if timeout is None:
        timeout = 120 + 3 * len(items)
    work = tempfile.mkdtemp(prefix="masm_")
    for stem, src in items:
        with open(os.path.join(work, stem + ".ASM"), "w", newline="\r\n") as f:
            f.write(src)
    cmd = "\n".join("MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG 2>&1"
                    % (s, s, s, s) for s, _ in items)
    conf = os.path.join(work, "masm.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(CONF.format(masm=masm, work=work, cmd=cmd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        from src.core import progress as _pr
        aborted = _pr.cancelled
    except Exception:
        aborted = lambda: False
    proc = subprocess.Popen([DBX, "-conf", conf, "-nolog"], env=env,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    waited = 0.0
    while proc.poll() is None:
        if aborted():
            proc.kill(); proc.wait(timeout=5)
            return {"work": work, "aborted": True}
        time.sleep(0.05)
        waited += 0.05
        if waited > timeout:
            proc.kill()
            break
    results = {}
    for stem, _src in items:
        d = {}
        for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
            p = os.path.join(work, stem + ext)
            if os.path.exists(p):
                d[key] = open(p, "rb").read() if ext == ".OBJ" else \
                    open(p, encoding="latin1").read()
        results[stem] = d
    return {"work": work, "results": results}


def functions(asm_src, flags=None):
    """Assemble and return {name: (bytes, mask)} via objextract (same as cc16)."""
    from src.formats import objextract
    r = assemble(asm_src)
    if "obj" not in r:
        raise RuntimeError("MASM produced no OBJ:\n" + r.get("log", "")[:600])
    return objextract.Obj(r["obj"]).functions()


SELF_ASM = """\
_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT
        public  _addasm
_addasm proc near
        push    bp
        mov     bp, sp
        mov     ax, [bp+4]
        add     ax, [bp+6]
        pop     bp
        ret
_addasm endp
_TEXT   ends
        end
"""


def _selftest():
    from src.formats import objextract
    r = assemble(SELF_ASM)
    if "obj" not in r:
        print("no OBJ; MASM log:\n" + r.get("log", "")[:800])
        return False
    fns = objextract.Obj(r["obj"]).functions()
    print("functions:", {k: len(v[0]) for k, v in fns.items()})
    # MASM upper-cases public symbols by default (_ADDASM); match case-insensitively
    key = next((k for k in fns if k.lower() == "_addasm"), None)
    b = fns.get(key, (b"", set()))[0] if key else b""
    print("_addasm bytes:", b.hex())
    # 55 8b ec  8b 46 04  03 46 06  5d c3  = push bp;mov bp,sp;mov ax,[bp+4];add;pop;ret
    ok = b.startswith(bytes.fromhex("558bec")) and bytes.fromhex("034606") in b \
        and b.endswith(b"\xc3")
    print("\nSELFTEST:", "PASS - MASM assembled byte-exact proc" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("asm", nargs="?", help="path to a .ASM file")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()
    if args.selftest:
        sys.exit(0 if _selftest() else 1)
    if not args.asm:
        ap.error("give a .ASM file or --selftest")
    from src.formats import objextract
    r = assemble(open(args.asm).read())
    if "obj" not in r:
        sys.exit("no OBJ produced; MASM log:\n" + r.get("log", "")[:1000])
    obj = objextract.Obj(r["obj"])
    for name, cls in obj.segs:
        b = obj.seg_bytes.get(name, b"")
        print("  %-12s class=%-8s %6d bytes" % (name, cls, len(b)))
    print("# work dir: %s" % r["work"])


if __name__ == "__main__":
    main()
