#!/usr/bin/env python3
r"""rc.py -- the RECOMPILE driver (rung 1: make the derived C compile).

The byte-exact ASM ships in the ISO; il.py also lifts it to readable "register machine" C
(src/generated/, ~225,000 fns) that does NOT compile as-is. This project makes that C
compile -- WITHOUT touching il.py -- via two layers:

  1. a PRELUDE  (include/win95c.h)  declaring the shared vocabulary once
     (register-globals, byte/word lanes, x86 intrinsics, flags, fnptr).
  2. a thin ADAPTER (adapt(), below) applying mechanical source rewrites the prelude
     can't express (self-OR idiom, computed-goto pointer cast, ...).

--classify compiles every generated module (GCC, gnu89: computed goto + old-style calls
OK) and buckets the results, so the remaining blockers ARE the worklist. Byte-exactness is
a later, separate concern (that stays matchsearch + MSVC 2.0).

    python rc.py --classify [--limit N] [-j 8]
"""
import glob
import os
import re
import subprocess
import sys
import tempfile
from collections import Counter
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))                # engine/recompile95/tools
INC = os.path.join(os.path.dirname(HERE), "include")             # engine/recompile95/include
_ENGINE = os.path.dirname(os.path.dirname(HERE))                 # the shared core: engine/
GEN = os.path.join(os.environ.get("WT_DATA_ROOT") or os.path.join(   # win95 corpus (wt sets
    os.path.dirname(os.path.dirname(_ENGINE)), "win95"),             # WT_DATA_ROOT to it)
    "src", "generated")


def _count_args(s):
    """Number of top-level, comma-separated arguments in a call's arg string (paren/bracket
    aware). '' -> 0."""
    s = s.strip()
    if not s:
        return 0
    depth = 0
    n = 1
    for c in s:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif c == "," and depth == 0:
            n += 1
    return n


def _call_consensus(src):
    """{sub_X: arity} where EVERY call site of sub_X passes the same arg count -- the call-graph
    signal for functions whose own `ret` doesn't fix the arity (cdecl / caller-cleaned). Only a
    unanimous count is trusted (returns that count); disagreeing call sites -> no entry."""
    seen = {}
    pat = re.compile(r"\bsub_([0-9a-f]+(?:__\d+)?)\(")
    i = 0
    while True:
        m = pat.search(src, i)
        if not m:
            break
        depth, j = 1, m.end()
        while j < len(src) and depth:
            depth += (src[j] == "(") - (src[j] == ")")
            j += 1
        na = _count_args(src[m.end():j - 1])
        seen.setdefault("sub_" + m.group(1), set()).add(na)
        i = j
    return {s: next(iter(v)) for s, v in seen.items() if len(v) == 1}


def _drop_casts(src, def_arity):
    """Replace the blanket fn-ptr cast with a per-call decision: a `sub_X(args)` whose arg
    count EQUALS its callee's known arity is emitted DIRECT (type-checks against the real
    prototype); a mismatch (or unknown callee) keeps the `((u32(*)())sub_X)(args)` cast (the
    honest marker). Defs are RCDEF_-marked here, so `\\bsub_` matches only CALL sites."""
    pat = re.compile(r"\bsub_([0-9a-f]+(?:__\d+)?)\(")
    out = []
    i = 0
    dropped = kept = 0
    while True:
        m = pat.search(src, i)
        if not m:
            out.append(src[i:])
            break
        out.append(src[i:m.start()])
        sub = "sub_" + m.group(1)
        depth, j = 1, m.end()
        while j < len(src) and depth:
            depth += (src[j] == "(") - (src[j] == ")")
            j += 1
        args = src[m.end():j - 1]
        ar = def_arity.get(sub)
        if ar is not None and ar == _count_args(args):
            out.append("%s(%s)" % (sub, args))                    # direct -- cast dropped
            dropped += 1
        else:
            out.append("((u32(*)())%s)(%s)" % (sub, args))        # cast kept (marker)
            kept += 1
        i = j
    return "".join(out), dropped, kept


def adapt(src, link=False, arities=None):
    """Mechanical source rewrites that make the readable lift compile (rung 1).
    Each rule is a whole-class fix, keyed off a pattern il.py emits corpus-wide.
    link=True (rung 2): name each definition `sub_<offset>` to MATCH the `sub_HEX(...)`
    call sites, so intra-module calls resolve at link time (offset collisions get a
    __N suffix). link=False (rung 1): unique `NAME__f<ctr>` names, calls stay undefined.
    arities (rung 2c): {func_offset: nargs} from sigrecover -- set each def's real arity so
    matching calls drop the fn-ptr cast and use a true prototype (see _drop_casts)."""
    arities = arities or {}
    # STRUCT-ARG ELIMINATION: il.py types some args as `struct MOD_OFF_argK *` (from field
    # recovery) but ALSO uses them as integers (arg & 0xff), which is illegal, AND the struct
    # TYPE names collide across offset-dupes (redefinition). Both vanish if the args are just
    # u32: rewrite field access `x->fHEX` to a correctly-offset deref, drop the struct defs,
    # and turn every `struct ..._argK *` into u32. (Fields are named f<hex-byte-offset>.)
    src = re.sub(r'(\w+)->f([0-9a-f]+)\b', r'(*(u32*)((char*)(\1)+0x\2))', src)
    src = re.sub(r'struct \w+_arg\w+ \{[^}]*\};\n?', '', src)
    src = re.sub(r'struct \w+_arg\w+ \*', 'u32 ', src)
    # generic `void *argN` params/locals are also used as integers (arg & 0xff) -> u32.
    # (the `(void*)` cast the goto rule adds has no identifier after *, so it's untouched.)
    src = re.sub(r'\bvoid \*(\w+)', r'u32 \1', src)
    # self-OR idiom: x86 `or reg,reg` (flag-set no-op) -> just the value. Fixes ptr|ptr.
    src = re.sub(r'\(([A-Za-z_]\w*(?:->\w+|\.\w+)?) \| \1\)', r'(\1)', src)
    # computed goto: target must be a pointer -> cast through void*.
    src = src.replace('goto *', 'goto *(void*)')
    # address-of a literal: the lift emits `= &0xffffffff` / `&0x94` (a lea/seg artifact).
    # Can't take the address of a constant -> drop the & (hex or decimal). Bitwise `x & 0xff`
    # has a space before the operand, so it's unaffected.
    src = re.sub(r'&(0x[0-9a-fA-F]+|\d+)(?=[;,)\]])', r'\1', src)
    # unresolved jumps: `goto seg4:0x0000;`, `goto MFC250.1206;`, `goto seg;` are not valid
    # labels. The lift's real local labels are `L_<hex>`; drop any goto that isn't one of
    # those (or a computed `goto *`). Rung 1 = compile; the control edge is recovered later.
    src = re.sub(r'goto (?!\*|L_)[A-Za-z_][\w.:]*\s*;', ';', src)
    # jump pseudo-op emitted as assignment to an address literal: `0xADDR = __jmpd(...)`.
    # Can't assign to a constant -> drop the LHS, keep the call.
    src = re.sub(r'\b0x[0-9a-fA-F]+ = (?=__jmpd)', '', src)
    # numbered flag temps (__cf0, __ovf_add_1, ...) are both ASSIGNED and read by the lift.
    # Declare each as a global so both work (function-like helpers __carry(a,b) are the
    # prelude's job and don't match the no-paren lookahead).
    flags = sorted(set(re.findall(
        r'\b__(?:cf|zf|sf|of|pf|af|ovf|carry|borrow|aux)\w*\b(?!\s*\()', src)))
    decls = "".join("static u32 %s;\n" % f for f in flags)
    # UNIQUE-ify function DEFINITIONS: il.py names functions by offset, but offsets recur
    # across segments AND some modules emit degenerate offsets (every fn = seg1:0x00000),
    # so seg/offset can't disambiguate. Split on each [AUTO] header and rename that block's
    # function `NAME(` -> `NAME__f<counter>(` with a global counter -> guaranteed unique.
    # Safe: calls use the separate `sub_HEX` name, so renaming definitions breaks nothing.
    consensus = _call_consensus(src) if link else {}     # call-graph arity fallback
    parts = re.split(r'(/\* \[AUTO\] (\w+)\b[^\n]*?\*/)', src)
    def_arity = {}                                        # sub_<off> -> its (real) parameter count
    if len(parts) > 1:
        out = [parts[0]]
        ctr = 0
        seen = {}
        i = 1
        while i + 2 <= len(parts) - 1:
            comment, name, body = parts[i], parts[i + 1], parts[i + 2]
            if link:
                mo = re.search(r'seg\d+:0x([0-9a-f]+)', comment)
                # def headers are zero-padded (0x02942) but il.py emits CALLS in natural hex
                # (sub_2942); strip the padding so def names match the call sites and link.
                off = (mo.group(1).lstrip("0") or "0") if mo else "x"
                seen[off] = seen.get(off, 0) + 1
                sub = "sub_%s" % off if seen[off] == 1 else "sub_%s__%d" % (off, seen[off])
                new = "RCDEF_" + sub                      # mark defs so the cast skips them
                body = re.sub(r'\b' + re.escape(name) + r'\(', new + "(", body, count=1)
                # RUNG-2c: give the def its REAL arity. Prefer sigrecover's ground truth (ret N,
                # first def at an offset); else keep the lift's own param count. Params follow the
                # lift's stack-slot naming (arg2..); any argK the body uses beyond the arity is
                # declared a local so it still compiles. Return type -> u32.
                hdr = re.search(r'[A-Za-z_][\w ]*\**' + re.escape(new) + r'\s*\(([^)]*)\)', body)
                lp = (hdr.group(1).strip() if hdr else "")
                lift_idx = sorted(int(x) for x in re.findall(r'\barg(\d+)\b', lp))
                lift_n = len(lift_idx)
                aoff = None
                if off != "x" and seen[off] == 1:
                    try:
                        aoff = int(off, 16)
                    except ValueError:
                        aoff = None
                N = arities.get(aoff)                     # ret N (stdcall ground truth) first
                if N is None:
                    N = consensus.get(sub)                # else the call-graph consensus
                if N is None:
                    N = lift_n                            # last resort -> trust the lift
                base = lift_idx[0] if lift_idx else 2     # the lift's stack-slot base (arg1/arg2/..)
                pset = set(range(base, base + N))
                params = ", ".join("u32 arg%d" % k for k in sorted(pset)) if N > 0 else "void"
                body = re.sub(r'[A-Za-z_][\w ]*\**' + re.escape(new) + r'\s*\([^)]*\)',
                              "u32 %s(%s)" % (new, params), body, count=1)
                # any argK the body uses that is NOT a parameter -> declare a local (compiles;
                # correct value is a rung-3 concern). Covers both N<lift and odd-base cases.
                extra = sorted(k for k in set(int(x) for x in re.findall(r'\barg(\d+)\b', body))
                               if k not in pset)
                if extra:
                    body = re.sub(r'\)\s*\{', ")\n{\n" + "".join("    u32 arg%d;\n" % k for k in extra),
                                  body, count=1)
                def_arity[sub] = N
            else:
                ctr += 1
                new = "%s__f%d" % (name, ctr)
                body = re.sub(r'\b' + re.escape(name) + r'\(', new + "(", body, count=1)
            out += [comment, body]
            i += 3
        src = "".join(out)
    linkdecls = ""
    if link:
        # RUNG-2c cast-dropping: a call whose arg count == its callee's arity is emitted DIRECT
        # (type-checks against the real prototype below); a mismatch or unknown callee keeps the
        # `((u32(*)())sub_X)(...)` cast as the honest marker. Defs are still RCDEF_-marked here, so
        # `\bsub_` matches only CALL sites.
        src, _drop, _keep = _drop_casts(src, def_arity)
        # Forward-declare EVERY referenced sub_ -- call sites AND bare VALUE uses (function-pointer
        # cast target, `goto *`, pointer table). The old scan matched only `sub_X(`, so a sub used
        # purely as a value was left undeclared -> the dominant rung-2 failure (`'sub_XXXX'
        # undeclared`). Widening `\(` to `\b` catches those; in-module defs keep getting a matching
        # forward proto (real arity from def_arity), so a call-before-def still resolves cleanly.
        called = sorted(set(re.findall(r'\bsub_([0-9a-f]+(?:__\d+)?)\b', src)))
        src = src.replace("RCDEF_sub_", "sub_")

        def _proto(c):                                    # real prototype where the arity is known
            n = def_arity.get("sub_" + c)
            args = ("void" if n == 0 else ", ".join(["u32"] * n)) if n is not None else ""
            return "extern u32 sub_%s(%s);\n" % (c, args)
        linkdecls = "".join(_proto(c) for c in called)
    decls = linkdecls + decls
    return '#include "win95c.h"\n' + decls + src


def classify_one(path):
    try:
        src = open(path, encoding="utf-8", errors="replace").read()
    except Exception as e:
        return {"mod": os.path.basename(path), "ok": False, "errs": 1, "cats": ["read:%s" % e]}
    fd, tmp = tempfile.mkstemp(suffix=".c", dir=tempfile.gettempdir())
    os.close(fd)
    open(tmp, "w", encoding="utf-8").write(adapt(src))
    try:
        r = subprocess.run(["gcc", "-std=gnu89", "-fsyntax-only", "-w", "-I", INC, tmp],
                           capture_output=True, text=True, timeout=120)
    finally:
        try: os.remove(tmp)
        except OSError: pass
    errs = [l for l in r.stderr.splitlines() if " error:" in l]
    cats = []
    for l in errs:
        m = re.search(r"error: (.*)", l)
        if m:
            c = re.sub(r"'[^']*'", "'X'", m.group(1))
            c = re.sub(r"\bat [^ ]+", "", c)[:60]
            cats.append(c.strip())
    return {"mod": os.path.basename(path)[:-2], "ok": r.returncode == 0,
            "errs": len(errs), "cats": cats, "kind": _kind(path)}


def _kind(path):
    """Rough format bucket by extension (proxy for PE / NE-16bit / VxD-LE)."""
    ext = re.search(r"\.(\w+)\.c$", os.path.basename(path))
    ext = ext.group(1) if ext else "?"
    if ext in ("vxd", "386"):
        return "VxD/LE"
    if ext in ("drv",):
        return "NE/drv"
    if ext in ("dll", "exe", "cpl", "ocx", "scr", "sys"):
        return "PE-ish"
    return "other"


def object_one(path, m32=False):
    """RUNG 2 probe: compile the module to an OBJECT FILE with intra-module calls named to
    resolve (link=True), then read nm to count defined functions vs undefined symbols (the
    cross-module imports still to be linked). m32=True targets i386 (RUNG 2d -- the ACTUAL
    Win95 architecture, via the -m32 prelude path in win95c.h), not the 64-bit host."""
    try:
        src = adapt(open(path, encoding="utf-8", errors="replace").read(), link=True)
    except Exception:
        return {"mod": os.path.basename(path)[:-2], "obj": False, "T": 0, "U": 0}
    fd, tmp = tempfile.mkstemp(suffix=".c", dir=tempfile.gettempdir())
    os.close(fd)
    obj = tmp[:-2] + ".o"
    open(tmp, "w", encoding="utf-8").write(src)
    T = U = 0
    ok = False
    try:
        cmd = ["gcc", "-std=gnu89", "-c", "-w"] + (["-m32"] if m32 else []) + ["-I", INC, "-o", obj, tmp]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=180)
        ok = r.returncode == 0 and os.path.exists(obj)
        if ok:
            n = subprocess.run(["nm", obj], capture_output=True, text=True)
            for l in n.stdout.splitlines():
                p = l.split()
                if len(p) >= 2 and p[-2] in ("T", "t"):
                    T += 1
                elif len(p) >= 2 and p[-2] == "U":
                    U += 1
    finally:
        for f in (tmp, obj):
            try: os.remove(f)
            except OSError: pass
    return {"mod": os.path.basename(path)[:-2], "obj": ok, "T": T, "U": U, "kind": _kind(path)}


def run_object(limit=None, jobs=8, m32=False):
    mods = sorted(glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True))
    mods = [m for m in mods if re.search(r"\.(dll|exe|cpl|ocx|scr|vxd|386|sys|drv)\.c$", m)]
    if limit:
        mods = mods[:limit]
    arch = "i386 (-m32, RUNG 2d)" if m32 else "host x86-64"
    print("RUNG-2 object build: %d modules (gcc -c %s, -j%d)..." % (len(mods), arch, jobs), flush=True)
    res = []
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for i, r in enumerate(ex.map(lambda p: object_one(p, m32=m32), mods)):
            res.append(r)
            if (i + 1) % 200 == 0:
                print("  ...%d/%d  .o=%d" % (i + 1, len(mods),
                      sum(x["obj"] for x in res)), flush=True)
    ok = sum(r["obj"] for r in res)
    T = sum(r["T"] for r in res)
    U = sum(r["U"] for r in res)
    print("\n" + "=" * 60)
    print("RUNG-2 OBJECT BUILD  (%d modules, %s)" % (len(res), arch))
    print("=" * 60)
    print("  produced a .o:        %d  (%.1f%%)" % (ok, 100 * ok / max(1, len(res))))
    print("  defined functions:    %d" % T)
    print("  undefined symbols:    %d   (= cross-module imports to resolve)" % U)
    print("  intra-module resolved: %.1f%% of all referenced symbols"
          % (100 * T / max(1, T + U)))
    return res


def _obj_syms(o):
    """(defined, undefined) symbol-name sets of object `o` via nm."""
    d, u = set(), set()
    for l in subprocess.run(["nm", o], capture_output=True, text=True).stdout.splitlines():
        p = l.split()
        if len(p) >= 2:
            (u if p[-2] == "U" else d).add(p[-1])
    return d, u


def link_one(path, out_dir=None, keep=False):
    """RUNG 2d: compile the module to an i386 object and LINK it into a loadable PE module.
    The machine state resolves against win95rt.o (the runtime); every still-undefined symbol
    (cross-module sub_ callees, MinGW PE-runtime housekeeping) is satisfied by a generated stub,
    and the whole thing links freestanding (-nostdlib) into a PE32 .dll. Proves the readable C
    goes from 'compiles as objects' to 'links into a loadable module'. Returns a result dict."""
    name = os.path.basename(path)[:-2]
    work = out_dir or tempfile.mkdtemp(prefix="rclink_")
    os.makedirs(work, exist_ok=True)
    rt_c = os.path.join(INC, "win95rt.c")
    try:
        src = adapt(open(path, encoding="utf-8", errors="replace").read(), link=True)
        mc = os.path.join(work, "mod.c"); open(mc, "w", encoding="utf-8").write(src)
        mo = os.path.join(work, "mod.o"); ro = os.path.join(work, "win95rt.o")
        cc = ["gcc", "-std=gnu89", "-w", "-m32", "-fPIC", "-c", "-I", INC, "-o"]
        subprocess.run(cc + [mo, mc], capture_output=True, check=True)
        subprocess.run(cc + [ro, rt_c], capture_output=True, check=True)
        d1, u1 = _obj_syms(mo); d2, u2 = _obj_syms(ro)
        undef = (u1 | u2) - (d1 | d2)
        # Accept leading underscores: strip only the ONE COFF cdecl prefix, keeping intrinsic
        # helpers like ___dvh / ___dvl / ___xch / ___imulhi (COFF `_` + the C name `__dvh`), which
        # the old `_?` filter dropped -> a lone unstubbed symbol failed the whole link.
        stub_names = sorted({(s[1:] if s.startswith("_") else s) for s in undef
                             if re.match(r"_*[A-Za-z]\w*$", s) and s != "_GLOBAL_OFFSET_TABLE_"})
        sc = os.path.join(work, "stubs.c")
        open(sc, "w").write("typedef unsigned int u32;\n"
                            + "".join("u32 %s(){return 0;}\n" % n for n in stub_names))
        so = os.path.join(work, "stubs.o")
        subprocess.run(["gcc", "-std=gnu89", "-w", "-m32", "-fPIC", "-c", "-o", so, sc],
                       capture_output=True, check=True)
        out = os.path.join(work, name)          # `name` already carries the module ext (lz32.dll)
        r = subprocess.run(["gcc", "-m32", "-shared", "-nostdlib", "-nostartfiles", "-Wl,-e,0",
                            "-o", out, mo, ro, so], capture_output=True, text=True)
        ok = r.returncode == 0 and os.path.exists(out)
        return {"mod": name, "linked": ok, "size": os.path.getsize(out) if ok else 0,
                "stubs": len(stub_names), "xmod": [s for s in stub_names if s.startswith("sub_")],
                "err": "" if ok else r.stderr[:160], "out": out if (ok and keep) else None}
    except subprocess.CalledProcessError as e:
        return {"mod": name, "linked": False, "err": (e.stderr or b"")[:160] if isinstance(e.stderr, bytes) else (e.stderr or "")[:160]}
    finally:
        if not keep:
            for f in ("mod.c", "mod.o", "win95rt.o", "stubs.c", "stubs.o"):
                try: os.remove(os.path.join(work, f))
                except OSError: pass


def run_link(limit=None, jobs=8):
    mods = sorted(glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True))
    mods = [m for m in mods if re.search(r"\.(dll|exe|cpl|ocx|drv|scr|sys)\.c$", m)]
    if limit:
        mods = mods[:limit]
    print("RUNG-2d link: %d modules -> loadable PE (gcc -m32 -shared -nostdlib, -j%d)..." % (len(mods), jobs), flush=True)
    res = []
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for i, r in enumerate(ex.map(link_one, mods)):
            res.append(r)
            if (i + 1) % 100 == 0:
                print("  ...%d/%d linked=%d" % (i + 1, len(mods), sum(x["linked"] for x in res)), flush=True)
    ok = sum(r["linked"] for r in res)
    print("\n" + "=" * 60)
    print("RUNG-2d LINK  (%d modules)" % len(res))
    print("  linked into a loadable PE module: %d  (%.1f%%)" % (ok, 100 * ok / max(1, len(res))))
    print("=" * 60)
    return res


def main():
    a = sys.argv[1:]
    if "--link" in a:
        lim = int(a[a.index("--limit") + 1]) if "--limit" in a else None
        jb = int(a[a.index("-j") + 1]) if "-j" in a else max(2, (os.cpu_count() or 4) - 2)
        if "--module" in a:                          # link one named module, keep the output
            name = a[a.index("--module") + 1]
            m = next((x for x in glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True)
                      if os.path.basename(x)[:-2] == name), None)
            if not m:
                sys.exit("no generated C for module: " + name)
            r = link_one(m, out_dir=os.path.join(os.path.dirname(GEN), "linked"), keep=True)
            print("RUNG-2d link %s: linked=%s size=%s stubs=%d cross-module=%s\n  %s" % (
                r["mod"], r.get("linked"), r.get("size"), r.get("stubs", 0), r.get("xmod"),
                r.get("out") or r.get("err")))
            return
        run_link(limit=lim, jobs=jb)
        return
    if "--object" in a:
        lim = int(a[a.index("--limit") + 1]) if "--limit" in a else None
        jb = int(a[a.index("-j") + 1]) if "-j" in a else max(2, (os.cpu_count() or 4) - 2)
        run_object(limit=lim, jobs=jb, m32="--m32" in a)   # --m32: RUNG 2d i386 target
        return
    limit = int(a[a.index("--limit") + 1]) if "--limit" in a else None
    # default to the machine (leave 2 cores for the OS); threads shell out to gcc, so
    # the GIL is released per-compile and these run as true parallel processes.
    jobs = int(a[a.index("-j") + 1]) if "-j" in a else max(2, (os.cpu_count() or 4) - 2)
    mods = sorted(glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True))
    mods = [m for m in mods if re.search(r"\.(dll|exe|cpl|ocx|drv|scr|vxd|386|sys)\.c$", m)]
    if limit:
        mods = mods[:limit]
    print("classifying %d modules (gcc gnu89, -j%d)..." % (len(mods), jobs), flush=True)
    res = []
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for i, r in enumerate(ex.map(classify_one, mods)):
            res.append(r)
            if (i + 1) % 100 == 0:
                print("  ...%d/%d  clean=%d" % (i + 1, len(mods),
                      sum(x["ok"] for x in res)), flush=True)
    ok = sum(r["ok"] for r in res)
    total_err = sum(r["errs"] for r in res)
    cats = Counter(c for r in res for c in r["cats"])
    print("\n" + "=" * 60)
    print("RECOMPILE rung-1 classify  (%d modules)" % len(res))
    print("=" * 60)
    print("  COMPILE CLEAN:   %d  (%.1f%%)" % (ok, 100 * ok / max(1, len(res))))
    print("  with errors:     %d   (%d total errors)" % (len(res) - ok, total_err))
    print("-" * 60)
    print("  compile-clean by module type (signal #3):")
    bykind = {}
    for r in res:
        k = r.get("kind", "?")
        bykind.setdefault(k, [0, 0])
        bykind[k][0] += r["ok"]; bykind[k][1] += 1
    for k, (o, t) in sorted(bykind.items(), key=lambda x: -x[1][1]):
        print("    %-8s %4d/%-4d  (%.1f%%)" % (k, o, t, 100 * o / max(1, t)))
    print("-" * 60)
    print("  remaining blocker classes (the worklist):")
    for c, n in cats.most_common(15):
        print("    %5d  %s" % (n, c))
    return res


if __name__ == "__main__":
    main()
