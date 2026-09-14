#!/usr/bin/env python3
r"""verify95.py -- step 2.2: the verification oracle. A reconstructed body is CORRECT only when it
BEHAVES like the original, proven on the Unicorn oracle -- not merely when it compiles. This is the
definition of "done" for the full reconstruction and the throughput gate for the whole program.

For a function it runs the ORIGINAL and the REBUILT under the same oracle with identical inputs and
compares a codegen-INDEPENDENT behavioral fingerprint: (return value, clean-vs-fault exit, and the
sequence of imported API calls). Instruction path and count are ignored -- MSC-vs-OpenWatcom codegen
differs there without any behavioral difference. PE/DOS bodies run on the behavior-engine tracer;
NE (16-bit) bodies on ne_dynoracle. Divergence on identical inputs is a real reconstruction defect.

    wt --target win95 verify95 <retail> <rebuilt> [--arity N] [--max N]   # pair-verify two modules
    wt --target win95 verify95 --ledger                                   # batch: verify every body
                                                                          # whose module is rebuilt,
                                                                          # update body_ledger.csv

No fabrication: a fingerprint records only what the oracle observed; a trace error is recorded as
itself, never scored as a pass.
"""
import os
import sys
import csv

from src.engines.behavior import tracer
from src.targets.win95 import ne_dynoracle, origintriage as OT

# compiler/CRT-runtime imports the toolchain emits (not the body's own logic) -- ignored so
# MSC-vs-OpenWatcom startup noise is not scored as a behavioral divergence.
_TOOLCHAIN = {"KERNEL.GETVERSION", "KERNEL.GETWINFLAGS", "KERNEL.LOCALINIT", "KERNEL.GETVERSION16",
              "KERNEL32.GETVERSION", "KERNEL32.GETVERSIONEXA", "KERNEL32.GETCURRENTTHREADID"}


def _exit_norm(e):
    if e is None:
        return "none"
    e = str(e)
    return "ret" if e == "ret" else ("fault" if e.startswith("exception") else e.split(":")[0])


def _fp_pe(path, func, args):
    try:
        r = tracer.trace_function(path, func=func, args=args, stub_externals=True)
    except Exception as e:
        return {"err": type(e).__name__}
    deps = tuple(d for d in r.get("api_deps", []) if str(d).upper() not in _TOOLCHAIN)
    return {"ret": r.get("ret"), "exit": _exit_norm(r.get("exit")), "deps": deps,
            "truncated": bool(r.get("truncated"))}


def _fp_ne(path, ordn, args):
    try:
        r = ne_dynoracle.probe(path, ordinal=ordn, args=args)
    except Exception as e:
        return {"err": type(e).__name__}
    deps = tuple(h.get("api") for h in r.get("imports_hit", []) if h.get("api") not in _TOOLCHAIN)
    return {"ret": r.get("ret"), "exit": _exit_norm(r.get("exit")), "deps": deps, "truncated": False}


def _verdict(a, b):
    """Behavioral verdict for two fingerprints (codegen-independent):
      error        -- a side failed to trace (no evidence)
      inconclusive -- a side's trace truncated (hit a cap / ran into unfollowable code, e.g. a
                      32->16 thunk) so behavior was not fully observed -- NOT counted as verified
      equivalent   -- clean on both sides and (ret, exit, imported-call sequence) match
      divergent    -- clean on both sides but behavior differs (a real reconstruction defect)."""
    if "err" in a or "err" in b:
        return "error"
    if a.get("truncated") or b.get("truncated"):
        return "inconclusive"
    if a["ret"] == b["ret"] and a["exit"] == b["exit"] and a["deps"] == b["deps"]:
        return "equivalent"
    return "divergent"


def _fmt(path):
    lane, _exe = OT._detect(path)
    return lane


def verify_pair(retail, rebuilt, arity=0, maxn=64):
    """Per-export behavioral verdicts for retail vs rebuilt. Returns [(name, verdict, fp_r, fp_b)]."""
    lane = _fmt(retail)
    from src.targets.win95 import autobuild95 as A
    out = []
    if lane == "ne":
        import struct
        for ordn, nm in ne_dynoracle_exports(retail)[:maxn]:
            fr = _fp_ne(retail, ordn, [0] * arity)
            fb = _fp_ne(rebuilt, ordn, [0] * arity)
            out.append((nm or ("@%d" % ordn), _verdict(fr, fb), fr, fb))
    else:
        pr, pb = A.PE(retail), A.PE(rebuilt)
        rbase, bbase = A._imagebase(pr), A._imagebase(pb)   # tracer wants a VA (imagebase+rva)
        raddr, baddr = pr.export_addrs(), pb.export_addrs()
        for nm in list(raddr)[:maxn]:
            if nm not in baddr:
                continue
            fr = _fp_pe(retail, rbase + raddr[nm], [0] * arity)
            fb = _fp_pe(rebuilt, bbase + baddr[nm], [0] * arity)
            out.append((nm, _verdict(fr, fb), fr, fb))
    return out


def ne_dynoracle_exports(path):
    from src.targets.win95 import autobuild95 as A
    ne = A.NE(path)
    return [(o, n) for o, n in ne.resident_names()] if ne.is_ne else []


def _print_pair(retail, rebuilt, verdicts):
    from collections import Counter
    c = Counter(v for _n, v, _a, _b in verdicts)
    _TAG = {"equivalent": "OK  ", "divergent": "DIFF", "inconclusive": "?   ", "error": "err "}
    print("=== verify %s  vs  %s ===" % (os.path.basename(retail), os.path.basename(rebuilt)))
    for nm, v, a, b in verdicts:
        note = ""
        if v == "divergent":
            note = "  ret %s/%s exit %s/%s deps %d/%d" % (a.get("ret"), b.get("ret"),
                    a.get("exit"), b.get("exit"), len(a.get("deps", ())), len(b.get("deps", ())))
        elif v == "inconclusive":
            note = "  (trace truncated -- unfollowable, e.g. 32->16 thunk)"
        print("  %-28s %s%s" % (nm, _TAG[v], note))
    print("  --- %d equivalent, %d divergent, %d inconclusive, %d error (of %d) ---" % (
        c["equivalent"], c["divergent"], c["inconclusive"], c["error"], len(verdicts)))
    return c["equivalent"], len(verdicts)


# --------------------------------------------------------------- ledger batch
def _design():
    from src.core import paths
    for c in (os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design"),
              r"C:\Users\mrtit\Documents\GitHub\Windows95\design"):
        if os.path.isdir(c):
            return c
    return None


def _rebuilt_index():
    """Map retail module name -> a rebuilt artifact path we can verify against (the from-source
    builds we've produced: base/<mod>/_build, base/<mod>/_ow, design/scaffold/<mod>)."""
    base = os.path.join(os.path.dirname(_design()), "base")
    idx = {}
    for root in (base,):
        if not os.path.isdir(root):
            continue
        for dp, _d, fs in os.walk(root):
            for fn in fs:
                if fn.lower().endswith((".dll", ".exe", ".drv", ".vxd", ".386")):
                    idx.setdefault(fn.lower(), os.path.join(dp, fn))
    return idx


def verify_ledger(maxmods=None):
    design = _design()
    ledger = os.path.join(design, "body_ledger.csv")
    rows = list(csv.DictReader(open(ledger)))
    rebuilt = _rebuilt_index()
    # group genuine bodies by their example module; verify modules that have a rebuilt artifact
    by_mod = {}
    for r in rows:
        if r["origin"] in ("genuine", "documented-api"):
            by_mod.setdefault(r["example_module"], []).append(r)
    verified_ids = set()
    done_mods = 0
    for mod, brs in by_mod.items():
        rb = rebuilt.get(mod.lower())
        if not rb:
            continue
        retail = OT._corpus_root()
        from src.targets.win95 import autobuild95 as A  # locate retail path
        rp = None
        for dp, _d, fs in os.walk(retail):
            if mod in fs:
                rp = os.path.join(dp, mod); break
        if not rp:
            continue
        try:
            verdicts = verify_pair(rp, rb, maxn=256)
        except Exception:
            continue
        okset = {nm for nm, v, _a, _b in verdicts if v == "equivalent"}
        for r in brs:
            if r["example_name"] in okset:
                verified_ids.add(r["body_id"])
        done_mods += 1
        if maxmods and done_mods >= maxmods:
            break
    # update ledger
    for r in rows:
        if r["body_id"] in verified_ids:
            r["verified"] = "1"
            r["state"] = "verified"
    with open(ledger, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=rows[0].keys())
        w.writeheader(); w.writerows(rows)
    return {"modules_verified": done_mods, "bodies_verified": len(verified_ids), "ledger": ledger}


def main(argv):
    if "--ledger" in argv:
        mm = int(argv[argv.index("--max") + 1]) if "--max" in argv else None
        r = verify_ledger(maxmods=mm)
        print("=== verify95 --ledger ===")
        print("  modules verified against a rebuild: %d" % r["modules_verified"])
        print("  bodies marked VERIFIED:             %d" % r["bodies_verified"])
        print("  ledger -> %s" % r["ledger"])
        return
    pos = [a for a in argv if not a.startswith("--")]
    if len(pos) < 2:
        print(__doc__); return
    arity = int(argv[argv.index("--arity") + 1]) if "--arity" in argv else 0
    maxn = int(argv[argv.index("--max") + 1]) if "--max" in argv else 64
    verdicts = verify_pair(pos[0], pos[1], arity=arity, maxn=maxn)
    _print_pair(pos[0], pos[1], verdicts)


if __name__ == "__main__":
    main(sys.argv[1:])
