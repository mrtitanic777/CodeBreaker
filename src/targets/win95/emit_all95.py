#!/usr/bin/env python3
r"""emit_all95.py -- corpus-wide reconstructed-source emission.

Runs the fast static lanes across EVERY module and writes reconstructed .c per module:
  * 32-bit PE   -> recon32wrap (precision-gated validated-delegation wrappers)
  * 16-bit NE   -> recon16 (pure leaves + validated-delegation templates)
Tallies the corpus total. The behavioral lane (compose, per-function Unicorn tracing) is a
separate sustained sweep; this banks the fast-lane reconstructions across the whole corpus now.

    wt --target win95 emit_all95 <OUTDIR> [--limit-mods N]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import target
from src.core import layout


def run(outdir, limit_mods=None, progress=None):
    os.makedirs(outdir, exist_ok=True)
    mods = sorted(os.path.basename(d) for d in layout.asm_module_dirs())
    if not mods:
        # binary-only corpus (no per-module asm tree) -> enumerate the NE/PE/LE binaries directly
        try:
            mods = sorted({os.path.basename(p) for p in target.all_modules()})
        except Exception:
            mods = []
    if limit_mods:
        mods = mods[:limit_mods]
    import recon32wrap
    try:
        from src.engines.behavior import recon16   # behavior engine (qualified; not a targets sibling)
    except Exception:
        recon16 = None
    totals = {"pe": 0, "pe_wrappers": 0, "ne": 0, "ne_recon": 0, "files": 0, "err": 0}
    total = len(mods)
    for _i, m in enumerate(mods, 1):
        if progress:
            progress(_i, total, m, totals)
        stem = m.replace(".", "_")
        # resume: skip a module already emitted (lets a timed-out sweep continue)
        if os.path.exists(os.path.join(outdir, stem + ".wrappers.c")) or \
           os.path.exists(os.path.join(outdir, stem + ".recon16.c")):
            continue
        try:
            path = target.resolve(m)
            fmt = target.format_of(path)
        except Exception:
            totals["err"] += 1; continue
        try:
            if fmt == "PE":
                rs = recon32wrap.reconstruct_module(m)
                if rs:
                    stem = m.replace(".", "_")
                    with open(os.path.join(outdir, stem + ".wrappers.c"), "w", encoding="utf-8") as f:
                        f.write("/* %s -- reconstructed validated-delegation wrappers */\n\n" % m)
                        for r in rs:
                            f.write("/* %s */\n%s\n" % (r["name"], r["c"]))
                    totals["pe"] += 1; totals["pe_wrappers"] += len(rs); totals["files"] += 1
            elif fmt == "NE" and recon16 is not None:
                rs = recon16.reconstruct_module(path, limit=80)
                rec = [r for r in rs if r.get("reconstructed")]
                if rec:
                    stem = m.replace(".", "_")
                    with open(os.path.join(outdir, stem + ".recon16.c"), "w", encoding="utf-8") as f:
                        f.write("/* %s -- 16-bit reconstruction (recon16) */\n\n" % m)
                        try:
                            import enrich95
                            pre = enrich95.preamble16(m, path)
                        except Exception:
                            pre = ""
                        if pre:
                            f.write(pre + "\n")
                        for r in rec:
                            f.write("/* %s */\n%s\n" % (r.get("label", "?"), r.get("emitted_c", "")))
                    totals["ne"] += 1; totals["ne_recon"] += len(rec); totals["files"] += 1
        except Exception:
            totals["err"] += 1
        n = totals["pe"] + totals["ne"]
        if n and n % 50 == 0:
            print("  ...%d modules emitted (%d PE wrappers, %d NE funcs)" %
                  (n, totals["pe_wrappers"], totals["ne_recon"]), flush=True)
    print("=" * 66)
    print("CORPUS EMIT COMPLETE")
    print("  PE modules with wrappers : %d  (%d wrapper functions)" % (totals["pe"], totals["pe_wrappers"]))
    print("  NE modules reconstructed : %d  (%d 16-bit functions)" % (totals["ne"], totals["ne_recon"]))
    print("  .c files written         : %d  -> %s" % (totals["files"], outdir))
    print("  errors/skips             : %d" % totals["err"])
    print("=" * 66)


from src.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if sys.argv[1:] and sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    outdir = argv[0] if argv else os.path.join(os.getcwd(), "src", "reconstructed", "fastlane")
    lim = _opt(sys.argv[1:], "--limit-mods"); lim = int(lim) if lim else None
    print("emitting reconstructed source across the corpus -> %s" % outdir, flush=True)
    run(outdir, lim)


if __name__ == "__main__":
    main()
