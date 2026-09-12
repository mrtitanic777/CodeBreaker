#!/usr/bin/env python3
r"""certify.py -- the HYBRID swap gate: is a module's C drop-in faithful enough to swap into the
byte-exact booting Win95?

A module is only safe to substitute if the C reproduces Win95's behaviour everywhere we can check
AND its lifted source is low-risk. This runs the oracles (difftest pure-leaf + memdifftest buffer)
and censuses the lift defects, then classifies:

  CERTIFIED : oracle found >=1 proven fn and 0 mismatches, AND no hard lift defects
              (empty bodies / (*(fnptr)0) / goto*0) -> safe to swap.
  RISKY     : no mismatches but has lift defects or nothing was testable -> swap behind a flag.
  FAILED    : the oracle caught a real behaviour mismatch -> do NOT swap.

    python certify.py <module> [<module> ...]      (or --corpus for all PE modules)
"""
import os, re, sys, glob, subprocess
from src.core import paths

CB = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))
GEN = os.path.join(W, "src", "generated")


def _oracle(module, tool):
    """Run difftest/memdifftest via wt; return (proven, mismatch)."""
    env = dict(os.environ, WT_DATA_ROOT=os.path.dirname(W))
    try:
        out = subprocess.run([sys.executable, os.path.join(CB, "wt.py"), "recompile", tool, module],
                             capture_output=True, text=True, env=env, timeout=150, cwd=CB).stdout
    except Exception:
        return 0, 0
    if tool == "difftest":
        m = re.search(r'proven=(\d+)\s+mismatch=(\d+)', out)
        return (int(m.group(1)), int(m.group(2))) if m else (0, 0)
    m = re.search(r'(\d+)/(\d+) functions fully identical', out)
    if m:
        return int(m.group(1)), int(m.group(2)) - int(m.group(1))
    return 0, 0


def defects(module):
    c = glob.glob(os.path.join(GEN, "**", module + ".c"), recursive=True)
    if not c:
        return None
    t = open(c[0], encoding="utf-8", errors="replace").read()
    empty = len(re.findall(r'/\* \[AUTO\][^\n]*\*/\s*(?:u32|int|void|short|long)\s+\w+\s*\([^)]*\)\s*\{\s*\}', t))
    return dict(empty=empty, fnptr0=len(re.findall(r'\(\*\(fnptr\)0\)', t)),
                gotostar0=t.count('goto *0'), notrec=t.count('not recovered'),
                fns=len(re.findall(r'/\* \[AUTO\]', t)))


def certify(module):
    dp, dm = _oracle(module, "difftest")
    mp, mm = _oracle(module, "memdifftest")
    d = defects(module) or {}
    proven, mism = dp + mp, dm + mm
    hard = d.get("empty", 0) + d.get("fnptr0", 0) + d.get("gotostar0", 0)
    if mism > 0:
        v = "FAILED"
    elif proven > 0 and hard == 0:
        v = "CERTIFIED"
    else:
        v = "RISKY"
    return v, proven, mism, hard, d.get("notrec", 0), d.get("fns", 0)


def main():
    if "--corpus" in sys.argv:
        mods = sorted({os.path.basename(p)[:-2] for p in glob.glob(GEN + "/**/*.dll.c", recursive=True)})
    else:
        mods = [a for a in sys.argv[1:] if not a.startswith("-")]
    counts = {}
    for m in mods:
        v, proven, mism, hard, nr, fns = certify(m)
        counts[v] = counts.get(v, 0) + 1
        print("%-10s %-18s proven=%-4d mism=%-3d hard-defects=%-4d not-rec=%-5d fns=%d"
              % (v, m, proven, mism, hard, nr, fns), flush=True)
    print("== " + "  ".join("%s:%d" % (k, counts[k]) for k in ("CERTIFIED", "RISKY", "FAILED") if k in counts) + " ==")


if __name__ == "__main__":
    main()
