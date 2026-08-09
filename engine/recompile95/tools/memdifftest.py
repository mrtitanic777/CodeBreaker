#!/usr/bin/env python3
r"""memdifftest.py -- differential-test MEMORY functions of the Lifted C.

difftest.py verifies scalar-in/scalar-out functions; this verifies functions that read/write a
caller BUFFER (struct *argN). For each such call-free function it:
  * ORACLE   -- emulates win95's real bytes in Unicorn with a buffer mapped per pointer arg, and
                captures the return value AND every buffer's after-state.
  * CANDIDATE-- compiles the function's Lifted C into a 32-bit DLL (LIFTM.DLL).
  * TESTER   -- memtest_harness.c (built to a 32-bit CRT exe, runs under WOW64) marshals matching
                buffers, calls the candidate, and compares return + buffer bytes to the oracle.
An 'ok' means the recompiled Lifted C reproduces Windows 95's behaviour, memory side-effects and all.

    wt recompile memdifftest [module.dll ...]
Requires: unicorn, the i686 mingw toolchain (C:\msys64\mingw32), a working host gcc for the tester.
"""
import os, re, sys, glob, random, subprocess, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import difftest, sigrecover                                  # noqa: E402
from engine.foundation import cores                          # noqa: E402
from engine.format_loaders.pe import open_pe                 # noqa: E402
from engine.format_loaders.pe_seg import PeSeg               # noqa: E402
from engine.format_loaders.pe_trace import PeCarve           # noqa: E402

I686 = os.environ.get("I686_GCC", r"C:\msys64\mingw32\bin\gcc.exe")
os.environ["PATH"] = os.path.dirname(I686) + os.pathsep + os.environ.get("PATH", "")
HOSTCC = os.environ.get("HOST_GCC", I686)                    # 32-bit CRT exe (WOW64-runnable); i686 mingw
INC = os.path.join(os.path.dirname(HERE), "include")
BUFSZ, KCASES, MAXPTR, BUF0 = 512, 6, 3, 0x00500000
DEFAULT_MODS = ["mfc40.dll", "comctl32.dll", "shell32.dll", "oleaut32.dll", "ole32.dll",
                "user32.dll", "gdi32.dll", "kernel32.dll", "mapi32.dll", "msjt3032.dll"]
_ENTRY = re.compile(r"\b(edx|esi|edi|ebx|ecx)\b")
_BLOCK = re.compile(
    r"/\* \[AUTO\]\s+\w+\s+\w+\s+seg1:0x([0-9a-f]+)[^\n]*\*/\s*"
    r"(?:struct[^{;]*\{[^}]*\}\s*;\s*)*"                      # optional struct typedef(s)
    r"([A-Za-z_][\w ]*?\**)\s*(\w+)\(([^)]*)\)\s*\{"
    r"((?:(?!/\* \[AUTO\]).)*?)\n\}", re.S)


def _select(txt):
    """{off: (arity, [ptr_arg_index...], name, block_src)} for call-free struct*-buffer functions."""
    out = {}
    for m in _BLOCK.finditer(txt):
        off, name, params, body = int(m.group(1), 16), m.group(3), m.group(4), m.group(5)
        if "sub_" in body or "goto *" in body or "goto seg" in body or "*(u32*)(0x" in body:
            continue
        if re.search(r"\b(cs|ds|es|fs|gs|esp|ebp)\b", body):
            continue
        argpos = [int(x) for x in re.findall(r"\barg(\d+)\b", params)]
        ptr = [int(x) for x in re.findall(r"\*\s*(?:const\s*)?arg(\d+)", params)]
        if not ptr or len(ptr) > MAXPTR or any(p not in argpos for p in ptr):
            continue
        flds = [int(x, 16) for x in re.findall(r"->f([0-9a-f]+)", body)]
        if flds and max(flds) > BUFSZ - 8:
            continue
        bad = False                                          # reject entry-register reads (non-std entry)
        for em in _ENTRY.finditer(body):
            r_, pre = em.group(1), body[:em.start()]
            if (r_ + " =") not in pre and (r_ + "=") not in pre and (r_ + " =") not in params:
                bad = True; break
        if bad:
            continue
        blk = m.group(0)
        out[off] = (len(argpos), [argpos.index(p) for p in ptr], name, blk[blk.find("*/") + 2:])
    return out


def _emulate(unicorn, va0, code, fva, end, args, pidxs, bufs):
    from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX
    uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    base = va0 & ~0xFFF
    uc.mem_map(base, max((len(code) + (va0 - base) + 0xFFF) & ~0xFFF, 0x1000)); uc.mem_write(va0, bytes(code))
    uc.mem_map(0x200000, 0x10000)
    a = list(args)
    for j, pi in enumerate(pidxs):
        addr = BUF0 + j * 0x1000
        uc.mem_map(addr, 0x1000); uc.mem_write(addr, bytes(bufs[j])); a[pi] = addr
    esp, RET = 0x208000, 0xDEAD0000
    for v in reversed(a):
        esp -= 4; uc.mem_write(esp, int(v & 0xffffffff).to_bytes(4, "little"))
    esp -= 4; uc.mem_write(esp, RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    try:
        uc.emu_start(fva, RET, count=300000)
    except unicorn.UcError:
        return None, None
    outs = [bytes(uc.mem_read(BUF0 + j * 0x1000, len(bufs[j]))) for j in range(len(pidxs))]
    return uc.reg_read(UC_X86_REG_EAX), outs


def build(modules, stage):
    import unicorn
    rng = random.Random(7)
    lift = ['#include "win95c.h"\n',
            "int __stdcall DllMainCRTStartup(void*h,unsigned r,void*x){return 1;}\n",
            "__declspec(dllexport) void __zero(void){eax=ebx=ecx=edx=esi=edi=CF=ZF=SF=OF=PF=AF=0;}\n"]
    tests = []
    for module in modules:
        try:
            pe = open_pe(cores.get("target").resolve(module))
        except Exception:
            continue
        va0, code, secname = pe.code_sections()[0]
        sg = PeSeg(module, section=secname, carve=PeCarve(cores.get("target").resolve(module)))
        arities = sigrecover.arities(module)
        c = glob.glob(os.path.join(difftest._GEN, "**", module + ".c"), recursive=True)
        if not c:
            continue
        sel = _select(open(c[0], encoding="utf-8", errors="replace").read())
        mod3 = module.replace(".", "_")
        for off, (arity, pidxs, name, blksrc) in sorted(sel.items()):
            if off not in sg.funcs or arities.get(off) is None:
                continue
            f = sg.funcs[off]
            cases, ok = [], True
            for _ in range(KCASES):
                args = [rng.getrandbits(32) for _ in range(arity)]
                bufs = [bytes(rng.getrandbits(8) for _ in range(BUFSZ)) for _ in pidxs]
                r, bo = _emulate(unicorn, va0, code, va0 + off, f["end"], args, pidxs, bufs)
                if r is None:
                    ok = False; break
                cases.append((args, bufs, r & 0xffffffff, bo))
            if not ok or not cases:
                continue
            expname = "m_%s_%05x" % (mod3, off)
            src = re.sub(r"\b" + re.escape(name) + r"\b", expname, blksrc)
            d = tempfile.mkdtemp()
            open(os.path.join(d, "x.c"), "w").write('#include "win95c.h"\n' + src)
            if subprocess.run([I686, "-x", "c", "-std=gnu89", "-w", "-m32", "-shared", "-o",
                               os.path.join(d, "x.dll"), os.path.join(d, "x.c"),
                               os.path.join(INC, "win95rt.c"), "-I", INC], capture_output=True).returncode:
                continue
            src = re.sub(r"(int|void|u32|struct[^\n]*\*)\s+" + re.escape(expname) + r"\s*\(",
                         lambda mm: "__declspec(dllexport) " + mm.group(0), src, count=1)
            lift.append(src + "\n")
            tests.append((module, expname, arity, pidxs, cases))
    print("selected %d memory functions (1-%d buffers each)" % (len(tests), MAXPTR))
    if not tests:
        return 0

    open(os.path.join(stage, "LIFTM.C"), "w").write("".join(lift))
    r = subprocess.run([I686, "-x", "c", "-std=gnu89", "-w", "-m32", "-mstackrealign", "-shared",
                        "-nostdlib", "-Wl,-e,_DllMainCRTStartup@12", "-o", os.path.join(stage, "LIFTM.DLL"),
                        os.path.join(stage, "LIFTM.C"), os.path.join(INC, "win95rt.c"), "-I", INC, "-lgcc"],
                       capture_output=True, text=True)
    if r.returncode:
        sys.exit("LIFTM.DLL build failed:\n" + r.stderr[-1500:])

    th = ["typedef struct{const char*mod;const char*name;int arity;int nptr;const int*pidx;int nc;"
          "const unsigned*args;const unsigned char*bin;const unsigned char*bout;const unsigned*ret;}MT;",
          "#define BUFSZ %d" % BUFSZ, "#define MAXPTR %d" % MAXPTR]
    for i, (mod, exp, arity, pidxs, cases) in enumerate(tests):
        args = [v & 0xffffffff for (a, b, r, bo) in cases for v in a]
        bin_ = [x for (a, b, r, bo) in cases for buf in b for x in buf]
        bout = [x for (a, b, r, bo) in cases for buf in bo for x in buf]
        ret = [r for (a, b, r, bo) in cases]
        th.append("static const int P%d[]={%s};" % (i, ",".join(str(x) for x in pidxs)))
        th.append("static const unsigned A%d[]={%s};" % (i, ",".join("0x%xu" % v for v in args)))
        th.append("static const unsigned char BI%d[]={%s};" % (i, ",".join(str(v) for v in bin_)))
        th.append("static const unsigned char BO%d[]={%s};" % (i, ",".join(str(v) for v in bout)))
        th.append("static const unsigned R%d[]={%s};" % (i, ",".join("0x%xu" % v for v in ret)))
    th.append("static const MT MT_[]={")
    for i, (mod, exp, arity, pidxs, cases) in enumerate(tests):
        th.append('  {"%s","%s",%d,%d,P%d,%d,A%d,BI%d,BO%d,R%d},'
                  % (mod, exp, arity, len(pidxs), i, len(cases), i, i, i, i))
    th += ["};", "static const int NMT=%d;" % len(tests)]
    open(os.path.join(HERE, "memtests.h"), "w").write("\n".join(th))
    try:
        r = subprocess.run([HOSTCC, "-m32", "-w", "-I", HERE, "-o", os.path.join(stage, "MEMTEST.EXE"),
                            os.path.join(HERE, "memtest_harness.c")], capture_output=True, text=True)
    finally:
        os.remove(os.path.join(HERE, "memtests.h"))
    if r.returncode:
        sys.exit("MEMTEST.EXE build failed:\n" + r.stderr[-1500:])
    out = subprocess.run([os.path.join(stage, "MEMTEST.EXE")], capture_output=True, text=True, cwd=stage)
    print(out.stdout.strip() or out.stderr.strip())
    return len(tests)


def main():
    mods = [a for a in sys.argv[1:] if not a.startswith("-")] or DEFAULT_MODS
    difftest._GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")
    stage = tempfile.mkdtemp(prefix="memdt_")
    build(mods, stage)


if __name__ == "__main__":
    main()
