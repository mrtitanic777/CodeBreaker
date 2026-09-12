#!/usr/bin/env python3
"""
oracle.py — the codegen oracle / flag-pinner (byte-exact stage F).

Two jobs, both deterministic and local:

1. CATALOG — compile a battery of small C functions across a matrix of compiler
   flag sets (memory model x optimization x stack-check), extract each
   function's exact bytes (objextract), and report the FINGERPRINTS: which flags
   actually change the emitted code, and how. This is the codegen-RE knowledge
   base the permuter is steered by.

2. PIN — given a function's shipped bytes, find the flag set(s) whose output
   matches (relocation sites masked). This is how we discover the exact flags
   the retail build was compiled with: match one known function, and the flags are
   pinned for the whole module. Self-tested here by compiling under a hidden
   flag set and recovering it.

Each compile is an independent DOSBox run, so the matrix sweep fans out across
cores (parallel.py).

    python oracle.py catalog             # build + print the fingerprint report
    python oracle.py pin --selftest      # recover a hidden flag set (proof)
"""
import argparse
from src.core import cores
import sys

cc16 = cores.get('cc16')
from src.formats import objextract
parallel = cores.get('parallel')

# --- the flag matrix (edit freely; each combo is one compile) ---------------
MODELS = ["/AS", "/AM", "/AL"]          # small / medium / large memory model
OPTS = ["/Od", "/Ox"]                   # no opt / max opt
STACK = ["", "/Gs"]                     # default stack checks / disabled
BASE = "/c"                             # compile only


def matrix():
    out = []
    for m in MODELS:
        for o in OPTS:
            for s in STACK:
                out.append(" ".join(x for x in (m, o, s, BASE) if x))
    return out


# --- the battery: small functions chosen to exercise flag-sensitive codegen -
BATTERY = r"""
int  add2(int a, int b)      { return a + b; }
long mul3(long x)            { return x * 3; }
int  deref(int *p)           { return *p; }
int  loop(int n)             { int i, s = 0; for (i = 0; i < n; i++) s += i; return s; }
int  big(void)               { char buf[600]; int i; for (i=0;i<600;i++) buf[i]=(char)i; return buf[3]; }
"""


def _compile_one(flags):
    """Compile the whole battery under one flag set -> {func: (bytes, mask)}."""
    try:
        r = cc16.compile_c(BATTERY, flags=flags)
        if "obj" not in r:
            return (flags, None)
        return (flags, objextract.Obj(r["obj"]).functions())
    except Exception as e:
        return (flags, None)


def build_catalog(jobs):
    flags = matrix()
    results = parallel.tmap(_compile_one, flags, jobs=jobs, label="oracle compile")
    cat = {f: fns for f, fns in results if fns is not None}
    failed = [f for f, fns in results if fns is None]
    return cat, failed


def _match(a, am, b, bm):
    """True if byte strings a,b are equal ignoring masked (relocated) positions."""
    if len(a) != len(b):
        return False
    skip = am | bm
    return all(a[i] == b[i] for i in range(len(a)) if i not in skip)


def _funcnames(cat):
    names = set()
    for fns in cat.values():
        names.update(fns)
    return sorted(names)


def cmd_catalog(jobs):
    cat, failed = build_catalog(jobs)
    print("codegen oracle: %d flag sets compiled%s\n"
          % (len(cat), (", %d failed" % len(failed)) if failed else ""))
    for fn in _funcnames(cat):
        # group flag sets by identical (masked) output
        groups = []                       # [(repr_bytes, repr_mask, [flags])]
        for flags in matrix():
            if flags not in cat or fn not in cat[flags]:
                continue
            b, m = cat[flags][fn]
            for g in groups:
                if _match(b, m, g[0], g[1]):
                    g[2].append(flags); break
            else:
                groups.append((b, m, [flags]))
        print("%-8s %d distinct encoding(s) over %d flag sets:"
              % (fn.lstrip("_"), len(groups), sum(len(g[2]) for g in groups)))
        for b, m, fl in sorted(groups, key=lambda g: -len(g[2])):
            chk = "chkstk" if _calls_helper(b) else ""
            print("   %3dB %-6s  %s" % (len(b), chk, ", ".join(fl)))
        # which axis matters?
        axes = []
        if _axis_varies(cat, fn, MODELS): axes.append("model")
        if _axis_varies(cat, fn, OPTS): axes.append("opt")
        if _axis_varies(cat, fn, STACK): axes.append("stack")
        print("   sensitive to: %s\n" % (", ".join(axes) or "nothing"))


def _calls_helper(b):
    return b.count(0x9A) > 0             # far call opcode (runtime helper stub)


def _axis_varies(cat, fn, tokens):
    """Does toggling this axis ever change fn's bytes (other axes fixed)?"""
    seen = {}
    for flags, fns in cat.items():
        if fn not in fns:
            continue
        tok = next((t for t in tokens if t and t in flags.split()), "(none)")
        rest = tuple(x for x in flags.split() if x not in tokens)
        seen.setdefault(rest, {})[tok] = fns[fn][0]
    for rest, byt in seen.items():
        if len({v for v in byt.values()}) > 1:
            return True
    return False


def cmd_pin(jobs, selftest, hidden="/AL /Ox /Gs /c", target=None):
    cat, _ = build_catalog(jobs)
    if selftest:
        r = cc16.compile_c(BATTERY, flags=hidden)
        fns = objextract.Obj(r["obj"]).functions()
        print("pin selftest: hidden flags = %r\n" % hidden)
        ok = True
        for fn in ("_big", "_loop", "_mul3"):
            tb, tm = fns[fn]
            hits = [f for f in matrix()
                    if fn in cat.get(f, {}) and _match(tb, tm, *cat[f][fn])]
            got = hidden.replace(" /c", "").strip() in \
                [h.replace(" /c", "").strip() for h in hits]
            print("  %-6s -> matched %d flag set(s): %s  [%s]"
                  % (fn.lstrip("_"), len(hits),
                     ", ".join(h.replace(" /c", "") for h in hits) or "none",
                     "recovered" if got else "MISS"))
            ok = ok and got
        print("\nSELFTEST:", "PASS - hidden flags recovered" if ok else "FAIL")
        sys.exit(0 if ok else 1)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("mode", nargs="?", default="catalog", choices=["catalog", "pin"])
    ap.add_argument("--selftest", action="store_true")
    jobs = parallel.resolve_jobs(sys.argv, default=6)   # cap DOSBox fan-out modestly
    args = ap.parse_args()
    jobs = min(jobs, 6)
    if args.mode == "pin":
        cmd_pin(jobs, args.selftest)
    else:
        cmd_catalog(jobs)


if __name__ == "__main__":
    main()
