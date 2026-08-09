#!/usr/bin/env python3
r"""vmdifftest.py -- build a VM differential-test ISO for the Lifted C.

difftest.py proves the Lifted C runs on the host (Unicorn oracle, pure-leaf subset). This packs
a self-contained ISO you run in a 32-bit Windows VM (win95), where BOTH sides execute natively --
covering functions the host emulator can't:
  * LIFT.DLL   -- the pure-leaf Lifted C, recompiled to a real 32-bit PE DLL (i686 gcc).
  * R<k>.DLL   -- the real win95 DLLs (8.3-safe copies), loaded and called by RVA.
  * HARNESS.EXE-- freestanding (-nostdlib, win9x-safe): SELF = LIFT.DLL vs an embedded reference
                  (each function's Unicorn output); LIVE = the real DLL vs that reference. Writes
                  RESULTS.TXT. SELF runs anywhere (incl. WOW64); LIVE needs the win95 VM.

    wt recompile vmdifftest [OUT.iso] [--modules gdi32.dll,user32.dll,...]
Requires: unicorn, the i686 mingw toolchain (C:\msys64\mingw32), and isowrite.py.
"""
import os, re, sys, random, subprocess, shutil, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import difftest, sigrecover                                  # noqa: E402
from engine.foundation import cores                          # noqa: E402
from engine.format_loaders.pe import open_pe                 # noqa: E402
from engine.format_loaders.pe_seg import PeSeg               # noqa: E402
from engine.format_loaders.pe_trace import PeCarve           # noqa: E402

I686 = os.environ.get("I686_GCC", r"C:\msys64\mingw32\bin\gcc.exe")
os.environ["PATH"] = os.path.dirname(I686) + os.pathsep + os.environ.get("PATH", "")
INC = os.path.join(os.path.dirname(HERE), "include")
KCASES = 8
DEFAULT_MODS = ["gdi32.dll", "user32.dll", "shell32.dll", "oleaut32.dll", "advapi32.dll",
                "comdlg32.dll", "comctl32.dll", "kernel32.dll"]


def _i686_links(csrc):
    d = tempfile.mkdtemp()
    open(os.path.join(d, "x.c"), "w").write('#include "win95c.h"\n' + csrc)
    r = subprocess.run([I686, "-x", "c", "-std=gnu89", "-w", "-m32", "-shared",
                        "-o", os.path.join(d, "x.dll"), os.path.join(d, "x.c"),
                        os.path.join(INC, "win95rt.c"), "-I", INC], capture_output=True, text=True)
    return r.returncode == 0


def _select(module, rng):
    """Yield (rva, expname, arity, [(args,out)]) for standalone-callable proven pure-leaf fns."""
    import unicorn
    pe = open_pe(cores.get("target").resolve(module))
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(cores.get("target").resolve(module)))
    arities = sigrecover.arities(module)
    blocks = difftest._blocks(module)
    mod3 = module.replace(".", "_")
    for off, f in sorted(sg.funcs.items()):
        if (f["end"] - off) < 8 or not difftest._is_pure(blocks.get(off, ""), off, arities):
            continue
        b = blocks.get(off, "").split("*/", 1)[-1]           # code only (drop the [AUTO] comment)
        if re.search(r"\b(esp|ebp|cs|ds|es|fs|gs)\b", b) or "->" in b or "struct " in b or "[" in b:
            continue                                          # must touch only args + scratch regs
        n = arities.get(off) or 0
        if n > 4:
            continue
        cases, ok = [], True
        for _ in range(KCASES):
            args = [rng.getrandbits(32) for _ in range(n)]
            o = difftest._emulate(unicorn, va0, code, va0 + off, f["end"], args)
            if o is None:
                ok = False; break
            cases.append((args, o & 0xffffffff))
        if not ok:
            continue
        expname = "f_%s_%05x" % (mod3, off)
        body = blocks[off].split("*/", 1)[1]
        dn = re.search(r"\b([A-Za-z_]\w*)\s*\(", body).group(1)
        c = re.sub(r"\b" + re.escape(dn) + r"\b", expname, body)
        if not _i686_links(c):
            continue
        yield (pe.image_base, va0, off, expname, n, cases, "__declspec(dllexport) " + c)


def build(out_iso, modules):
    rng = random.Random(20260808)
    stage = tempfile.mkdtemp(prefix="vmdt_")
    lift = ['#include "win95c.h"\n',
            '__declspec(dllexport) void __zero(void){eax=ebx=ecx=edx=esi=edi=CF=ZF=SF=OF=PF=AF=0;}\n']
    tests, used = [], []
    for m in modules:
        try:
            sel = list(_select(m, rng))
        except Exception:
            continue
        if not sel:
            continue
        used.append(m)
        for imgbase, va0, off, expname, n, cases, csrc in sel:
            lift.append(csrc + "\n")
            tests.append((m, (va0 - imgbase) + off, expname, n, cases))
    short = {m: "R%d.DLL" % i for i, m in enumerate(used)}
    print("selected %d proven pure-leaf functions across %d modules" % (len(tests), len(used)))

    open(os.path.join(stage, "LIFT.C"), "w").write("".join(lift))
    _run([I686, "-x", "c", "-std=gnu89", "-w", "-m32", "-mstackrealign", "-shared", "-o",
          os.path.join(stage, "LIFT.DLL"), os.path.join(stage, "LIFT.C"),
          os.path.join(INC, "win95rt.c"), "-I", INC], "LIFT.DLL")
    os.remove(os.path.join(stage, "LIFT.C"))

    th = ["typedef struct{const char*mod;const char*realdll;unsigned rva;const char*expname;"
          "int arity;int ncase;const unsigned*cases;}TEST;"]
    for i, (m, rva, exp, n, cs) in enumerate(tests):
        flat = [v & 0xffffffff for a, o in cs for v in list(a) + [o]]
        th.append("static const unsigned K%d[]={%s};" % (i, ",".join("0x%xu" % v for v in flat)))
    th.append("static const TEST TESTS[]={")
    for i, (m, rva, exp, n, cs) in enumerate(tests):
        th.append('  {"%s","%s",0x%xu,"%s",%d,%d,K%d},' % (m, short[m], rva, exp, n, len(cs), i))
    th += ["};", "static const int NTEST=%d;" % len(tests)]
    open(os.path.join(HERE, "tests.h"), "w").write("\n".join(th))
    _run([I686, "-std=gnu99", "-w", "-m32", "-nostdlib", "-mstackrealign", "-O1", "-Wl,-e,_run",
          "-o", os.path.join(stage, "HARNESS.EXE"), os.path.join(HERE, "vmtest_harness.c"),
          "-I", HERE, "-lkernel32"], "HARNESS.EXE")
    os.remove(os.path.join(HERE, "tests.h"))

    for m in used:
        shutil.copy(cores.get("target").resolve(m), os.path.join(stage, short[m]))
    open(os.path.join(stage, "MODULES.TXT"), "w").write(
        "".join("%s = %s\r\n" % (short[m], m) for m in used))
    open(os.path.join(stage, "README.TXT"), "w").write(
        "Lifted C differential harness -- run HARNESS.EXE in a 32-bit Windows (win95 VM).\r\n"
        "It writes RESULTS.TXT: SELF = our recompiled Lifted C vs the emulator reference "
        "(runs anywhere); LIVE = the real win95 DLL vs the reference (needs the win95 VM).\r\n"
        "%d functions, %d DLLs. R<k>.DLL -> module map in MODULES.TXT.\r\n" % (len(tests), len(used)))

    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(HERE)), "targets", "win95"))
    import isowrite
    isowrite.write_iso(stage, out_iso, volid="LIFTEDC")
    print("ISO:", out_iso, os.path.getsize(out_iso), "bytes")


def _run(cmd, label):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit("%s build FAILED:\n%s" % (label, r.stderr[-1500:]))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("-")]
    out = args[0] if args else os.path.join(os.getcwd(), "LIFTEDC_DIFFTEST.iso")
    mods = DEFAULT_MODS
    if "--modules" in sys.argv:
        mods = sys.argv[sys.argv.index("--modules") + 1].split(",")
    build(out, mods)


if __name__ == "__main__":
    main()
