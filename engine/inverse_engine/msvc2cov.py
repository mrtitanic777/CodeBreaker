#!/usr/bin/env python3
r"""msvc2cov.py -- REAL-CORPUS coverage of the MSVC-2.0 inverse (msvc2inv).

Runs msvc2inv over actual shipped Windows 95 functions (their real linked bytes, via the match
engine) and reports the honest byte-exact recovery rate -- the fraction of real win95 that the
compiler-inverse turns back into byte-exact C. A function counts only if invert() returns C whose
model bytes equal the shipped bytes with relocation slots masked (byte-exact by construction).

    wt --target win95 <...>  # (run from the win95 data root, or with WT_DATA_ROOT set)
    python -m engine.inverse_engine.msvc2cov [limit] [module]
"""
import os
import sys
from collections import Counter


def _setup():
    """Point the engine at the win95 target + data root (matchbatch resolves relative to cwd)."""
    from engine.foundation import cores
    here = os.path.dirname(os.path.abspath(__file__))
    cb = os.path.dirname(os.path.dirname(here))
    if cores._active_dir is None:
        cores.set_target(os.path.join(cb, "engine", "targets", "win95"))
    roots = ([os.path.join(os.environ["WT_DATA_ROOT"], "win95")] if os.environ.get("WT_DATA_ROOT") else [])
    roots += [os.path.join(cb, "binaries", "win95"), os.path.join(os.path.dirname(cb), "win95"), os.getcwd()]
    for r in roots:
        if os.path.isdir(os.path.join(r, "files")) or os.path.isdir(os.path.join(r, "src")) \
                or os.path.isdir(os.path.join(r, "reconstructed")):
            os.chdir(r)
            break


def run(limit=2000, module=None, show=0):
    _setup()
    from engine.match_engine import matchbatch, matchsearch
    from engine.inverse_engine import msvc2inv
    tot = inv = 0
    shapes = Counter()
    bylen = Counter()
    for mod, seg, off in matchsearch.iter_corpus(module, limit):
        ap = matchbatch._asm_for(mod, seg, off)
        if not ap:
            continue
        try:
            sb = matchbatch.shipped_bytes(ap)
        except Exception:
            continue
        if not sb:
            continue
        tot += 1
        r = msvc2inv.invert(sb)
        if r:
            inv += 1
            shapes[r["expr"][0] if r.get("expr") else "seq"] += 1
            bylen[len(sb) // 8 * 8] += 1
            if show and inv <= show:
                print("  %-16s (%2dB) -> %s" % ("%s:%s:%s" % (mod, seg, off), len(sb), r["c"]))
    print("\n" + "=" * 64)
    print("MSVC-2.0 INVERSE -- real-win95 coverage")
    print("=" * 64)
    print("  real functions sampled:            %d" % tot)
    print("  byte-exact recovered by msvc2inv:  %d  (%.1f%%)" % (inv, 100 * inv / max(1, tot)))
    print("  recovered-shape breakdown:  " + ", ".join("%s=%d" % kv for kv in shapes.most_common()))
    return inv, tot


_CORE = ["kernel32.dll", "gdi32.dll", "user32.dll"]


def _leaf_bytes(md, carve, s, end):
    """Bytes of the function at `s` up to and including its first ret (or a bare
    tail-jmp thunk), or None if it is not a short straight-line leaf."""
    base, data = carve._sec_of(s)
    if base is None:
        return None
    buf = bytes(data[s - base:min(end, base + len(data)) - base])
    out = b""
    for ins in md.disasm(buf, s):
        out += ins.bytes
        if ins.mnemonic in ("ret", "retn"):
            return out
        if ins.mnemonic == "jmp" and out == ins.bytes:      # tail-call thunk: return g()
            return out
        if len(out) > 80:
            return None
    return None


_BR = ("je", "jne", "jz", "jnz", "jmp", "jg", "jl", "jle", "jge", "ja", "jb", "jae", "jbe", "js", "jns")


def _func_bytes(md, carve, s, end):
    """Full function bytes: extend to the first `ret` that no earlier forward branch jumps
    past (so control-flow / multi-return functions aren't truncated at their first ret).
    None if it runs longer than the modeled class can plausibly reach."""
    base, data = carve._sec_of(s)
    if base is None:
        return None
    buf = bytes(data[s - base:min(end, base + len(data)) - base])
    out, maxtgt = b"", s
    for ins in md.disasm(buf, s):
        out += ins.bytes
        if ins.mnemonic in _BR and ins.op_str.startswith("0x"):
            t = int(ins.op_str, 16)
            if t > ins.address:
                maxtgt = max(maxtgt, t)
        if ins.mnemonic in ("ret", "retn") and ins.address >= maxtgt:
            return out
        if ins.mnemonic == "jmp" and out == ins.bytes:          # bare tail-call thunk
            return out
        if len(out) > 200:
            return None
    return None


def run_direct(modules=None, show=0):
    """Coverage by carving the SHIPPED PE modules directly (no pre-generated corpus needed):
    isolate each leaf function and count msvc2inv byte-exact recoveries. This is the honest
    self-contained baseline for the modeled class."""
    _setup()
    import capstone
    from engine.format_loaders.pe_trace import PeCarve
    from engine.inverse_engine import msvc2inv
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    tot = inv = 0
    shapes = Counter()
    shown = 0
    for mod in (modules or _CORE):
        try:
            carve = PeCarve(mod)
        except Exception as e:
            print("  SKIP %s (%r)" % (mod, e))
            continue
        mtot = minv = 0
        for s, f in sorted(carve.funcs.items()):
            b = _func_bytes(md, carve, s, f["end"])
            if not b or len(b) < 3:
                continue
            mtot += 1
            r = msvc2inv.invert(b)
            if r:
                minv += 1
                shapes[r["expr"][0] if r.get("expr") else "seq"] += 1
                if show and shown < show:
                    print("  %-22s %s" % ((f.get("name") or "sub_%x" % s)[:22], r["c"]))
                    shown += 1
        print("  %-13s leaf-ish %4d  byte-exact C %3d  (%.1f%%)" % (
            mod, mtot, minv, 100.0 * minv / max(1, mtot)))
        tot += mtot
        inv += minv
    print("\n" + "=" * 64)
    print("MSVC-2.0 INVERSE -- real-win95 direct coverage")
    print("=" * 64)
    print("  leaf-ish functions sampled:        %d" % tot)
    print("  byte-exact recovered by msvc2inv:  %d  (%.1f%%)" % (inv, 100 * inv / max(1, tot)))
    print("  recovered-shape breakdown:  " + ", ".join("%s=%d" % kv for kv in shapes.most_common()))
    return inv, tot


def main():
    a = sys.argv[1:]
    limit = int(a[0]) if a and a[0].isdigit() else 2000
    module = next((x for x in a if not x.isdigit() and not x.startswith("-")), None)
    show = 20 if "--show" in a else 0
    if "--corpus" in a:                     # the old pre-generated-corpus path
        run(limit=limit, module=module, show=show)
    else:                                   # default: self-contained direct carve
        run_direct(modules=[module] if module else None, show=show)


if __name__ == "__main__":
    main()
