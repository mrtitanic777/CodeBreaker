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
from engine.foundation import cores, layout                  # noqa: E402
from engine.format_loaders.pe import open_pe                 # noqa: E402
from engine.format_loaders.pe_seg import PeSeg               # noqa: E402
from engine.format_loaders.pe_trace import PeCarve           # noqa: E402
from engine.ir_engine import il                              # noqa: E402
import certify_fn, clean_graph                               # noqa: E402
CLEAN = "--clean" in sys.argv       # test the DEREGISTERIZED clean lift instead of the register-machine C
GRAPH = "--graph" in sys.argv       # WHOLE-GRAPH memory: allow NON-leaf mem fns (compile the closure in)
MANIFEST = sys.argv[sys.argv.index("--manifest") + 1] if "--manifest" in sys.argv else None
if __name__ == "__main__":          # standalone run: register the win95 target (wt sets it normally)
    cores.set_target(os.path.join(os.path.dirname(os.path.dirname(HERE)), "targets", "win95"))

I686 = os.environ.get("I686_GCC", r"C:\msys64\mingw32\bin\gcc.exe")
os.environ["PATH"] = os.path.dirname(I686) + os.pathsep + os.environ.get("PATH", "")
HOSTCC = os.environ.get("HOST_GCC", I686)                    # 32-bit CRT exe (WOW64-runnable); i686 mingw
INC = os.path.join(os.path.dirname(HERE), "include")
BUFSZ, KCASES, MAXPTR, BUF0 = 2048, 6, 6, 0x00500000   # MAXPTR raised: register inputs also get buffers
MAXARGS = 8                                            # harness calln supports up to this many cdecl args
DEFAULT_MODS = ["mfc40.dll", "comctl32.dll", "shell32.dll", "oleaut32.dll", "ole32.dll",
                "user32.dll", "gdi32.dll", "kernel32.dll", "mapi32.dll", "msjt3032.dll"]
_ENTRY = re.compile(r"\b(edx|esi|edi|ebx|ecx)\b")
_BLOCK = re.compile(
    r"/\* \[AUTO\]\s+\w+\s+\w+\s+seg1:0x([0-9a-f]+)[^\n]*\*/\s*"
    r"(?:struct[^{;]*\{[^}]*\}\s*;\s*)*"                      # optional struct typedef(s)
    r"([A-Za-z_][\w ]*?\**)\s*(\w+)\(([^)]*)\)\s*\{"
    r"((?:(?!/\* \[AUTO\]).)*?)\n\}", re.S)


def _leading_regs_named(cc, name):
    """The lifted function's leading REGISTER params, in signature order (il emits recovered register
    inputs as the leading params, named after the register). Parses `<ret> NAME(<params>)`."""
    m = re.search(r"\b" + re.escape(name) + r"\s*\(", cc)
    if not m:
        return []
    i, depth, seg = m.end(), 1, []
    while i < len(cc) and depth:
        ch = cc[i]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                break
        seg.append(ch); i += 1
    regs = []
    for p in "".join(seg).split(","):
        nm = p.replace("*", " ").split()[-1] if p.strip() else ""
        if nm in ("eax", "ecx", "edx", "ebx", "esi", "edi", "ebp"):
            regs.append(nm)
        else:
            break
    return regs


def _entry_regs(body, params=""):
    """Registers READ before written at entry (genuine register inputs), from a lifted body. edx/esi/
    edi/ebx/ecx (never eax=return). ecx alone => a thiscall/fastcall register arg we CAN recover;
    any other => non-standard entry we don't handle yet."""
    regs = set()
    for em in _ENTRY.finditer(body):
        r_, pre = em.group(1), body[:em.start()]
        if (r_ + " =") not in pre and (r_ + "=") not in pre and (r_ + " =") not in params:
            regs.add(r_)
    return regs


def _select(txt):
    """{off: (stack_arity, [stack_ptr_pos...], name, block_src, has_reg)} for buffer functions.
    In CLEAN mode a fn with register inputs is ALLOWED (has_reg=True) -- build() recovers them via il
    liveness (matching package_ff), maps a buffer for each leading register param, and the emulator
    sets the real registers. The pointer/field info here is for the STACK args only."""
    out = {}
    for m in _BLOCK.finditer(txt):
        off, name, params, body = int(m.group(1), 16), m.group(3), m.group(4), m.group(5)
        if ("sub_" in body and not GRAPH) or "goto *" in body or "goto seg" in body or re.search(r"\*\(u\d+\*\)\(0x", body):
            continue                                         # --graph: allow calls (closure compiled in)
        if re.search(r"\b(cs|ds|es|fs|gs|esp|ebp)\b", body):
            continue
        entry = _entry_regs(body, params)                    # register inputs at entry
        if entry and not CLEAN:
            continue                                         # register recovery only in CLEAN mode
        argpos = [int(x) for x in re.findall(r"\barg(\d+)\b", params)]
        ptr = [int(x) for x in re.findall(r"\*\s*(?:const\s*)?arg(\d+)", params)]
        if (not ptr and not entry) or len(ptr) > MAXPTR or any(p not in argpos for p in ptr):
            continue                                         # need >=1 buffer (a stack ptr OR a register)
        flds = [int(x, 16) for x in re.findall(r"->f([0-9a-f]+)", body)]
        if flds and max(flds) > BUFSZ - 8:
            continue
        blk = m.group(0)
        out[off] = (len(argpos), [argpos.index(p) for p in ptr], name, blk[blk.find("*/") + 2:], bool(entry))
    return out


def _emulate(unicorn, va0, code, fva, end, args, pidxs, bufs, reg_order=()):
    from unicorn.x86_const import (UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX,
                                   UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    RMAP = {"eax": UC_X86_REG_EAX, "ecx": UC_X86_REG_ECX, "edx": UC_X86_REG_EDX, "ebx": UC_X86_REG_EBX,
            "esi": UC_X86_REG_ESI, "edi": UC_X86_REG_EDI, "ebp": UC_X86_REG_EBP}
    uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    base = va0 & ~0xFFF
    uc.mem_map(base, max((len(code) + (va0 - base) + 0xFFF) & ~0xFFF, 0x1000)); uc.mem_write(va0, bytes(code))
    uc.mem_map(0x200000, 0x10000)
    a = list(args)
    for j, pi in enumerate(pidxs):
        addr = BUF0 + j * 0x1000
        uc.mem_map(addr, 0x1000); uc.mem_write(addr, bytes(bufs[j])); a[pi] = addr
    # the leading len(reg_order) candidate args are recovered register inputs -- set the REAL registers
    # (in il's param order), NOT the stack; the rest are stdcall stack args (pushed as usual).
    nreg = len(reg_order)
    for i, rn in enumerate(reg_order):
        uc.reg_write(RMAP[rn], int(a[i] & 0xffffffff))
    esp, RET = 0x208000, 0xDEAD0000
    for v in reversed(a[nreg:]):
        esp -= 4; uc.mem_write(esp, int(v & 0xffffffff).to_bytes(4, "little"))
    esp -= 4; uc.mem_write(esp, RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    try:
        uc.emu_start(fva, RET, count=300000)
    except unicorn.UcError:
        return None, None
    outs = [bytes(uc.mem_read(BUF0 + j * 0x1000, len(bufs[j]))) for j in range(len(pidxs))]
    return uc.reg_read(UC_X86_REG_EAX), outs


def _closure_threaded(module, off, an):
    """--graph: the WHOLE intra-module closure of `off`, each fn lifted with il liveness (reg_inputs=
    None) and its recovered register args THREADED across every call-site -- EXACTLY what package_ff
    --clean emits. Returns ({o: body_named_sub_<o>}, top_reg_order) or None if any closure fn is
    defective/unliftable. This lets the oracle verify NON-LEAF fns (with cross-call register threading),
    not just leaves -- the top is oracle-run end-to-end against the real callees (whole .text mapped)."""
    if an is None:
        return None
    funcs, defects, calls = an
    clo = clean_graph._closure(off, funcs, calls)
    if not clo or any(defects.get(o) for o in clo):
        return None
    raw, regp = {}, {}
    for o in sorted(clo):
        ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % o)
        if not os.path.isfile(ap):
            return None
        try:
            cb = il.lift_asm_function(ap, "sub_%x" % o, clean=True, reg_inputs=None)
        except Exception:
            return None
        body = cb.split("{", 1)[-1]
        if ("not recovered" in body or "goto *" in body or "__lifted" in body
                or re.search(r"\*\(u\d+\*\)\(0x", body) or re.search(r"\b(cs|ds|es|fs|gs)\b", body)):
            return None
        raw[o], regp[o] = cb, clean_graph._leading_regs(cb, o)
    for o in list(raw):                                      # thread each callee's reg args into calls
        b = raw[o]
        for c in clo:
            if regp.get(c):
                b = clean_graph._thread(b, c, regp[c], o == c)
        raw[o] = b
    return raw, tuple(regp[off])


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
        an = certify_fn._analyze(module) if GRAPH else None
        for off, (stack_arity, stack_ptr, name, blksrc, has_reg) in sorted(sel.items()):
            if off not in sg.funcs or arities.get(off) is None:
                continue
            f = sg.funcs[off]
            reg_order = ()
            if CLEAN:
                if an is not None:                       # --graph: whole closure, il liveness + threading
                    res = _closure_threaded(module, off, an)   # (LEAF and NON-LEAF, EXACTLY package_ff)
                    if res is None:
                        continue                         # defective / won't lift to self-contained clean C
                    _bodies, reg_order = res
                    blksrc = re.sub(r"\bsub_%x\b" % off, name,     # top sub_<off> -> `name`
                                    "\n".join(_bodies[o] for o in sorted(_bodies)))
                else:                                    # no --graph: LEAF only (single self-contained fn)
                    ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % off)
                    if not os.path.isfile(ap):
                        continue
                    try:
                        cc = il.lift_asm_function(ap, name, clean=True, reg_inputs=None)
                    except Exception:
                        continue
                    cbody = cc.split("{", 1)[-1]
                    if ("not recovered" in cbody or "goto *" in cbody or "__lifted" in cbody
                            or "sub_" in cbody or re.search(r"\*\(u\d+\*\)\(0x", cbody)):  # abs global
                        continue
                    reg_order = tuple(_leading_regs_named(cc, name))
                    blksrc = cc
            elif has_reg:
                continue                                 # register recovery needs the clean lift
            # candidate layout: recovered registers are the LEADING params (each a mapped buffer), then
            # the stdcall stack args. pidxs index into the candidate arg vector.
            nreg = len(reg_order)
            arity = nreg + stack_arity
            pidxs = list(range(nreg)) + [nreg + sp for sp in stack_ptr]
            if len(pidxs) > MAXPTR or arity > MAXARGS:
                continue                                 # too many buffers/args for the harness
            cases, ok = [], True
            for _ in range(KCASES):
                args = [rng.getrandbits(32) for _ in range(arity)]
                bufs = [bytes(rng.getrandbits(8) for _ in range(BUFSZ)) for _ in pidxs]
                r, bo = _emulate(unicorn, va0, code, va0 + off, f["end"], args, pidxs, bufs, reg_order)
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
          "const unsigned*args;const unsigned char*bin;const unsigned char*bout;const unsigned*ret;"
          "const int*retbuf;const unsigned*retoff;}MT;",
          "#define BUFSZ %d" % BUFSZ, "#define MAXPTR %d" % MAXPTR]
    for i, (mod, exp, arity, pidxs, cases) in enumerate(tests):
        args = [v & 0xffffffff for (a, b, r, bo) in cases for v in a]
        bin_ = [x for (a, b, r, bo) in cases for buf in b for x in buf]
        bout = [x for (a, b, r, bo) in cases for buf in bo for x in buf]
        ret = [r for (a, b, r, bo) in cases]
        # POINTER-IDENTITY return: if the emulated return lands inside buffer j (mapped at BUF0+j*0x1000),
        # it is a returned POINTER (e.g. a thiscall returning `this`/`this+k`). Its raw value differs from
        # the candidate's VirtualAlloc'd buffer, so record (buffer idx, offset) and let the harness rebuild
        # the expected pointer from ITS buffer -- otherwise a correct return-`this` looks like a mismatch.
        rbuf, roff = [], []
        for (a, b, r, bo) in cases:
            if BUF0 <= r < BUF0 + len(pidxs) * 0x1000:
                rbuf.append((r - BUF0) >> 12); roff.append((r - BUF0) & 0xFFF)
            else:
                rbuf.append(-1); roff.append(r & 0xffffffff)
        th.append("static const int P%d[]={%s};" % (i, ",".join(str(x) for x in pidxs)))
        th.append("static const unsigned A%d[]={%s};" % (i, ",".join("0x%xu" % v for v in args)))
        th.append("static const unsigned char BI%d[]={%s};" % (i, ",".join(str(v) for v in bin_)))
        th.append("static const unsigned char BO%d[]={%s};" % (i, ",".join(str(v) for v in bout)))
        th.append("static const unsigned R%d[]={%s};" % (i, ",".join("0x%xu" % v for v in ret)))
        th.append("static const int RB%d[]={%s};" % (i, ",".join(str(x) for x in rbuf)))
        th.append("static const unsigned RO%d[]={%s};" % (i, ",".join("0x%xu" % v for v in roff)))
    th.append("static const MT MT_[]={")
    for i, (mod, exp, arity, pidxs, cases) in enumerate(tests):
        th.append('  {"%s","%s",%d,%d,P%d,%d,A%d,BI%d,BO%d,R%d,RB%d,RO%d},'
                  % (mod, exp, arity, len(pidxs), i, len(cases), i, i, i, i, i, i))
    th += ["};", "static const int NMT=%d;" % len(tests)]
    open(os.path.join(stage, "memtests.h"), "w").write("\n".join(th))   # per-run (parallel-safe), not HERE
    r = subprocess.run([HOSTCC, "-m32", "-w", "-I", stage, "-I", HERE, "-o", os.path.join(stage, "MEMTEST.EXE"),
                        os.path.join(HERE, "memtest_harness.c")], capture_output=True, text=True)
    if r.returncode:
        sys.exit("MEMTEST.EXE build failed:\n" + r.stderr[-1500:])
    out = subprocess.run([os.path.join(stage, "MEMTEST.EXE")], capture_output=True, text=True, cwd=stage)
    print(out.stdout.strip() or out.stderr.strip())
    if MANIFEST:                                        # record the memory-VERIFIED functions (no XX line = passed)
        failed = set(re.findall(r"XX\s+\S+:(\S+)", out.stdout))
        with open(MANIFEST, "a", encoding="utf-8") as mf:
            for (mod, exp, arity, pidxs, cases) in tests:
                if exp not in failed:
                    mf.write("%s\t%x\n" % (mod, int(exp.rsplit("_", 1)[1], 16)))
    return len(tests)


def main():
    mods = [a for a in sys.argv[1:] if not a.startswith("-")] or DEFAULT_MODS
    difftest._GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")
    stage = tempfile.mkdtemp(prefix="memdt_")
    build(mods, stage)


if __name__ == "__main__":
    main()
