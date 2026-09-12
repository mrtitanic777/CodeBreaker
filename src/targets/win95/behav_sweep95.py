#!/usr/bin/env python3
r"""behav_sweep95.py -- corpus-wide BEHAVIORAL reconstruction sweep (the deep readable-C lane).

Runs the fused static+dynamic reconstruct (compose: synth/struct/effect/globals/gptr/piecewise/
multisynth) per function and emits readable, oracle-validated C per module (<module>.fused.c).
This is the deep lane for the real C-logic functions (the 90.5% C-logic bulk) -- distinct from
the fast wrapper lane (emit_all95). It is SLOW (per-function Unicorn tracing), so it is bounded
per module (--per) and RESUMES across runs (skips modules already emitted); run it repeatedly to
chew through the corpus in batches.

    wt --target win95 behav_sweep95 <OUTDIR> [--per 60] [--mods a.dll,b.dll]
"""
import os
import sys
import collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import target

# C-logic-heavy modules first (best readable-C yield), then the rest of the corpus by ledger.
PRIORITY = ["shell32.dll", "ole32.dll", "oleaut32.dll", "comctl32.dll", "mapi32.dll",
            "comdlg32.dll", "winmm.dll", "rpcrt4.dll", "shlwapi.dll", "wininet.dll",
            "msvfw32.dll", "rasapi32.dll", "crtdll.dll", "advapi32.dll", "mpr.dll",
            "kernel32.dll", "gdi32.dll", "user32.dll"]


def _modules():
    from src.core import layout
    allm = sorted(os.path.basename(d) for d in layout.asm_module_dirs())
    allset = set(allm)
    ordered = [m for m in PRIORITY if m in allset] + [m for m in allm if m not in set(PRIORITY)]
    return ordered


def run(outdir, per, mods=None):
    os.makedirs(outdir, exist_ok=True)
    import compose
    mods = mods or _modules()
    tot = collections.Counter()
    done = 0
    for m in mods:
        stem = m.replace(".", "_")
        outp = os.path.join(outdir, stem + ".fused.c")
        if os.path.exists(outp):
            continue
        try:
            if target.format_of(target.resolve(m)) != "PE":
                continue
        except Exception:
            continue
        try:
            rs = compose.reconstruct_module(m, per, fuse=True, behav_limit=per)
        except Exception as e:
            print("  SKIP %-14s (%s)" % (m, str(e)[:40]), flush=True); continue
        readable = [r for r in rs if r.status != "floor"]
        with open(outp, "w", encoding="utf-8") as f:
            f.write("/* %s -- behavioral reconstruction (compose): readable C where recoverable,\n"
                    "   validated against the binary oracle. */\n\n" % m)
            for r in rs:
                if r.source:
                    f.write("/* sub_%x [%s %s] */\n%s\n\n" % (r.rva, r.status, r.verify, r.source.rstrip()))
        meth = collections.Counter(r.status for r in rs)
        tot["funcs"] += len(rs); tot["readable"] += len(readable); tot["mods"] += 1
        done += 1
        print("  %-14s %d/%d readable  %s" % (m, len(readable), len(rs), dict(meth)), flush=True)
    print("=" * 60)
    print("BEHAVIORAL SWEEP (this run): %d modules, %d funcs, %d readable (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"],
           100.0 * tot["readable"] / (tot["funcs"] or 1)))
    print("  (resume: re-run to continue; skips the %d modules already emitted)" % done)


from src.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if sys.argv[1:] and sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    outdir = argv[0] if argv else os.path.join(os.getcwd(), "src", "reconstructed", "behavioral")
    per = int(_opt(sys.argv[1:], "--per") or 60)
    mods = _opt(sys.argv[1:], "--mods")
    mods = mods.split(",") if mods else None
    print("behavioral reconstruction sweep (per=%d) -> %s" % (per, outdir), flush=True)
    run(outdir, per, mods)


if __name__ == "__main__":
    main()
