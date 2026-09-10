#!/usr/bin/env python3
r"""matchdecomp.py -- the matching-decompilation worklist + driver (byte-exact stage F).

The conductor (`conduct`) drives every function to byte-exactness, but a byte MASM
cannot reproduce becomes a `db <hex>` fallback: byte-exact, yet not *live* code.
Those `db` bytes are exactly the functions that need MATCHING DECOMPILATION -- write
the C, compile it with the era's real compiler (`cc16` = MS C 3.0 for win1.0 / MSVC
1.52c for win3.1 under DOSBox; `cc32` = the era-correct MSVC, 2.0 for win95 / 5.0
for 98/nt4), and let `permute`+
`oracle` search source/flags until `segdiff` says the emitted bytes match the shipped
bytes. This module is the layer ABOVE those: it reads the conduct ledgers, finds and
ranks the candidates, and reports how much of the image is still `db`-fallback rather
than recovered code -- the honest "how far from a byte-identical, fully-live rebuild"
metric.

    wt --target <t> matchdecomp status              # aggregate: %-live, db-bytes left
    wt --target <t> matchdecomp scan  [--top N]     # worklist: functions by db count
    wt --target <t> matchdecomp scan --module FOO   # one module's candidates
    wt --target <t> matchdecomp ready               # is the byte-match pipeline wired?
    wt --target <t> matchdecomp candidate FOO       # our C for FOO's top target (seed)

Every number is read from reports/conduct/*.json (written by `conduct`); nothing is
recomputed or estimated. A function with db==0 is fully live and needs no work.
"""
import glob
import json
import os
import re
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
from codebreaker.foundation import layout

LEDGERS = os.path.join(layout.runtime_dir(), "conduct")


def _load():
    """Yield (module, func_key, rec) for every function in every conduct ledger."""
    for p in sorted(glob.glob(os.path.join(LEDGERS, "*.json"))):
        mod = os.path.basename(p)[:-5]
        try:
            led = json.load(open(p))
        except (OSError, ValueError):
            continue
        if isinstance(led, dict):
            for key, rec in led.items():
                if isinstance(rec, dict) and "db" in rec:
                    yield mod, key, rec


def _candidates():
    """Functions with any db-fallback bytes -- the matching-decompilation worklist."""
    return [(m, k, r) for (m, k, r) in _load() if r.get("db", 0) > 0]


def _asm_file(module, seg, off):
    """The reconstructed .asm for a function, by seg:off (matches decompile.py's
    sec<seg>_<offhex> naming; tolerant of zero-padding / obj vs sec)."""
    d = layout.asm_module_dir(module)
    for f in glob.glob(os.path.join(d, "*.asm")):
        m = re.match(r"(?:sec|obj)(\d+)_([0-9a-fA-F]+)", os.path.basename(f))
        if m and int(m.group(1)) == seg and int(m.group(2), 16) == off:
            return f
    return None


def cmd_candidate(module, topn):
    """Bridge the worklist to a starting C candidate: for a module's top PARTIAL
    functions (live code + db bytes), print our decompiler's readable C -- the seed
    a human/permuter refines into a byte-exact template (add $holes for the idioms
    the compiler's bytes resolve), then drives with `permute`."""
    import sys as _s
    _s.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from codebreaker.ir_engine import il
    p = os.path.join(LEDGERS, module + ".json")
    if not os.path.exists(p):
        print("no conduct ledger for %s (run `conduct %s` first)" % (module, module)); return
    led = json.load(open(p))
    cands = [(k, r) for k, r in led.items()
             if isinstance(r, dict) and r.get("db", 0) > 0 and r.get("live", 0) > 0]
    cands.sort(key=lambda kr: -kr[1]["db"])
    if not cands:
        print("no PARTIAL candidates in %s (nothing to matching-decompile)." % module); return
    tgt = os.environ.get("WT_TARGET", "?")
    for k, r in cands[:topn]:
        seg, off = r.get("seg", 1), r.get("off", 0)
        f = _asm_file(module, seg, off)
        print("=" * 72)
        print("candidate %-10s seg%d:0x%x  size=%d  db=%d  live=%d/%d  (%.0f%% live)"
              % (k, seg, off, r.get("size", 0), r["db"], r["live"], r["total"],
                 100 * r["live"] / max(1, r["total"])))
        if f:
            try:
                print(il.lift_asm_function(f, "fn_%x" % off))
            except Exception as e:
                print("  (lift error: %s)" % str(e)[:80])
        else:
            print("  (asm not found for this candidate)")
    print("\n" + "=" * 72)
    print("next: refine a candidate into a template with $holes for ambiguous idioms")
    print("      (e.g. $mul -> ['x*2','x<<1']), then let the period compiler pick:")
    print("        wt --target %s oracle pin <module>     # find the build flags" % tgt)
    print("        wt --target %s permute ...              # search source -> byte-exact" % tgt)


def cmd_status():
    fns = live = total = dbsum = exact = 0
    partial = partial_db = opaque = opaque_db = 0
    mods = set()
    for m, k, r in _load():
        mods.add(m)
        fns += 1
        live += r.get("live", 0)
        total += r.get("total", 0)
        db = r.get("db", 0)
        dbsum += db
        exact += 1 if r.get("status") == "exact" else 0
        if db > 0:
            if r.get("live", 0) > 0:                  # code with a few stubborn bytes
                partial += 1; partial_db += db
            else:                                     # all-db: opaque blob (likely data)
                opaque += 1; opaque_db += db
    if not fns:
        print("no conduct ledgers under %s -- run `conduct` first." % LEDGERS)
        return
    print("matching-decompilation status  (%s)" % os.environ.get("WT_TARGET", "?"))
    print("=" * 62)
    print("  modules with a ledger:   %d" % len(mods))
    print("  functions:               %d" % fns)
    print("  byte-exact:              %d  (%.2f%%)" % (exact, 100 * exact / fns))
    print("  live instrs / total:     %d / %d  (%.2f%% live)"
          % (live, total, 100 * live / max(1, total)))
    print("  --  remaining db-fallback work, split by kind:")
    print("  PARTIAL (live code + db):  %6d fns   %9d db-bytes  <- matching-decomp targets"
          % (partial, partial_db))
    print("  OPAQUE  (all db, no live): %6d fns   %9d db-bytes  <- likely data / unhandled"
          % (opaque, opaque_db))
    print("\n  next: `matchdecomp scan --partial --top 20` for the actionable worklist;")
    print("        drive each with permute/oracle (`matchdecomp ready` names this target's oracle:")
    print("        win1.0=MS C 3.0, win3.1=MSVC 1.52c [cc16]; win95=MSVC 2.0, 98/nt4=MSVC 5.0 [cc32]).")


def cmd_scan(top, only, partial):
    cands = _candidates()
    if partial:                                       # only functions with live code
        cands = [c for c in cands if c[2].get("live", 0) > 0]
    if only:
        cands = [c for c in cands if c[0] == only or c[0].startswith(only)]
    cands.sort(key=lambda c: (-c[2].get("db", 0), -c[2].get("size", 0)))
    if not cands:
        print("no db-fallback functions found -- nothing needs matching-decomp here.")
        return
    print("matching-decompilation worklist  (%d candidates; top %d by db-bytes)"
          % (len(cands), min(top, len(cands))))
    print("  %-22s %-10s %6s %6s %6s %8s" % ("module", "func", "db", "live", "total", "size"))
    print("  " + "-" * 66)
    for m, k, r in cands[:top]:
        print("  %-22s %-10s %6d %6d %6d %8d"
              % (m[:22], k, r.get("db", 0), r.get("live", 0),
                 r.get("total", 0), r.get("size", 0)))


def cmd_ready():
    """Report whether the matching-decompilation pipeline is wired for THIS target:
    the compiler oracle (cc16 for the 16-bit era, cc32/MSVC5.0 for 32-bit) plus the
    search/verify tools. Bridges the worklist (what needs matching) to capability."""
    target = os.environ.get("WT_TARGET", "?")
    tools = os.environ.get("WT_TOOLS_DIR", "")
    eng = os.path.dirname(os.path.abspath(__file__))

    def have(name):                                  # tool dir wins, else shared engine
        return any(os.path.exists(os.path.join(d, name + ".py")) for d in (tools, eng) if d)
    # the era-correct compiler oracle: 16-bit eras want cc16, 32-bit eras want cc32.
    need = "cc16" if target in ("win1.0", "win3.1") else "cc32"
    compiler = need if have(need) else None
    search = [t for t in ("oracle", "permute") if have(t)]
    verify = [t for t in ("objextract", "segdiff") if have(t)]
    print("matching-decompilation readiness  (%s)" % target)
    print("=" * 50)
    print("  era compiler (%s): %s" % (need, "present" if compiler else "MISSING"))
    if need == "cc32" and have("cc16"):
        print("  (cc16 also present -- for this era's 16-bit thunks only)")
    print("  search layer:     %s" % (", ".join(search) or "none"))
    print("  extract/verify:   %s" % (", ".join(verify) or "none"))
    ready = compiler and search and verify
    print("  --> %s" % ("READY: run permute/oracle against the `scan` worklist"
                         if ready else "BLOCKED: " + (
                             "%s (period compiler) not vendored" % need if not compiler
                             else "missing search/verify tools")))
    if need == "cc32" and compiler and target == "win95":
        print("  oracle: cc32 auto-selects the era-correct MSVC -- prefers 2.0/2.1/2.2 (the")
        print("          oraclebake-proven win95 match), then 4.2, then 5.0; all vendored under")
        print("          docs/reference/toolchain/. (Resolves to MSVC 2.0 here.)")
    if need == "cc16" and compiler and target == "win1.0":
        print("  note: cc16 uses MS C 3.0 (1985, era-correct) -- templates must be K&R,"
              " not ANSI prototypes.")


def main():
    a = sys.argv[1:]
    cmd = a[0] if a else "status"
    top = int(a[a.index("--top") + 1]) if "--top" in a else 25
    only = a[a.index("--module") + 1] if "--module" in a else None
    if cmd == "status":
        cmd_status()
    elif cmd == "scan":
        cmd_scan(top, only, "--partial" in a)
    elif cmd == "ready":
        cmd_ready()
    elif cmd == "candidate":                          # bridge worklist -> C to byte-match
        mod = a[1] if len(a) > 1 and not a[1].startswith("-") else only
        if not mod:
            print("usage: matchdecomp candidate <module> [--top N]"); sys.exit(2)
        cmd_candidate(mod, int(a[a.index("--top") + 1]) if "--top" in a else 1)
    else:
        print(__doc__); sys.exit(2)


if __name__ == "__main__":
    main()
