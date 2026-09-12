#!/usr/bin/env python3
r"""ne_sweep95.py -- corpus-wide 16-bit (Win16 NE) DEEP reconstruction sweep.

The NE counterpart to behav_sweep95. Runs recon16 -- the 16-bit reconstruction cascade on the
hardened tracer16 oracle (interproc16 static caller recovery + the validated-delegation template +
the far-pointer / pure-leaf synth) -- over EVERY function (exports *and* internal helpers) of EVERY
NE module, and emits readable, ordinal-named C per module (<module>.recon16.c).

This is the DEEP pass. The fast lane (emit_all95) runs recon16 capped at 80 functions/module just to
bank a fast reconstruction; here the per-module limit is removed so the whole module is covered -- the
untapped 16-bit recovery the ledger flagged (KRNL386, GDI/USER, the shell, the bundled apps: 360
modules only ever shallow-sampled). It is SLOW (per-function tracer16 emulation), so it RESUMES across
runs (skips modules already emitted) and runs module-level PARALLEL (independent per module, like the
PE deep lane) for a near-linear speedup with identical output.

    wt --target win95 ne_sweep95 <OUTDIR> [--limit N] [--mods a.dll,b.dll] [--jobs 8]

--limit caps functions/module (default: 0 = uncapped, the deep pass); --jobs sets parallel workers.
"""
import os
import sys
import collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import target

# 16-bit core first (best structural yield + the modules that define Win16), then the rest by name.
PRIORITY = ["krnl386.exe", "gdi.exe", "user.exe", "keyboard.drv", "mouse.drv", "system.drv",
            "shell.dll", "commdlg.dll", "mmsystem.dll", "ole2.dll", "ole2disp.dll", "storage.dll",
            "toolhelp.dll", "ver.dll", "lzexpand.dll", "comm.drv", "sound.drv"]


def _ne_modules():
    """Every NE module path under the corpus, priority-ordered."""
    try:
        allp = {os.path.basename(p).lower(): p for p in target.all_modules()}
    except Exception:
        allp = {}
    ne = []
    for name, p in allp.items():
        try:
            if target.format_of(p) == "NE":
                ne.append(name)
        except Exception:
            pass
    ne = sorted(set(ne))
    pset = set(PRIORITY)
    return [m for m in PRIORITY if m in set(ne)] + [m for m in ne if m not in pset]


def _emit_module(outdir, limit, m):
    """Reconstruct ONE NE module and write <stem>.recon16.c. The shared unit of work (sequential
    loop AND each parallel worker). Returns (status, n_funcs, n_reconstructed). RESUME-safe."""
    from src.engines.behavior import recon16
    stem = m.replace(".", "_")
    outp = os.path.join(outdir, stem + ".recon16.c")
    if os.path.exists(outp):
        return ("skip", 0, 0)
    try:
        path = target.resolve(m)
        if target.format_of(path) != "NE":
            return ("notne", 0, 0)
    except Exception:
        return ("resolveerr", 0, 0)
    try:
        rs = recon16.reconstruct_module(path, limit=(limit or None))
    except Exception as e:
        return ("ERR:" + str(e)[:48], 0, 0)
    rec = [r for r in rs if r.get("reconstructed")]
    tmp = outp + ".part"                                      # write-then-rename: a crash mid-write
    with open(tmp, "w", encoding="utf-8") as f:               # never leaves a half file resume skips
        f.write("/* %s -- 16-bit (Win16 NE) deep reconstruction (recon16 on tracer16):\n"
                "   readable C where recoverable, validated against the binary oracle. */\n\n" % m)
        for r in rs:
            if not r.get("reconstructed"):
                continue
            c = r.get("emitted_c") or r.get("source") or ""
            if not c.strip():
                continue
            label = r.get("label") or ("sub_%x_%x" % (r.get("seg", 0), r.get("offset", 0)))
            meth = r.get("method", "")
            f.write("/* %s  [%s] */\n%s\n\n" % (label, meth, c.rstrip()))
    os.replace(tmp, outp)
    return ("ok", len(rs), len(rec))


def run(outdir, limit=0, mods=None, progress=None, jobs=1):
    os.makedirs(outdir, exist_ok=True)
    mods = mods or _ne_modules()
    if jobs and jobs > 1:
        return run_parallel(outdir, limit, mods, jobs=jobs, progress=progress)
    tot = collections.Counter()
    total = len(mods)
    for _i, m in enumerate(mods, 1):
        if progress:
            progress(_i, total, m, tot)
        st, nf, nr = _emit_module(outdir, limit, m)
        if st == "ok":
            tot["funcs"] += nf; tot["readable"] += nr; tot["mods"] += 1
            print("  %-18s %d/%d reconstructed" % (m, nr, nf), flush=True)
        elif st.startswith("ERR"):
            print("  SKIP %-14s (%s)" % (m, st), flush=True)
    print("=" * 60)
    print("16-BIT DEEP SWEEP (this run): %d modules, %d funcs, %d reconstructed (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"],
           100.0 * tot["readable"] / (tot["funcs"] or 1)))


# ----------------------------------------------------------------- module-level PARALLELISM
_PW = {}


def _winit(corpus, target_dir, outdir, limit):
    import os as _os
    _os.environ["WT_DATA_ROOT"] = corpus
    try:
        _os.chdir(corpus)
    except Exception:
        pass
    try:
        from src.core import cores, config
        config.apply_config()
        cores.set_target(target_dir)
    except Exception:
        pass
    _PW["outdir"] = outdir
    _PW["limit"] = limit


def _wtask(m):
    try:
        return (m,) + _emit_module(_PW["outdir"], _PW["limit"], m)
    except Exception as e:
        return (m, "ERR:" + str(e)[:48], 0, 0)


def run_parallel(outdir, limit=0, mods=None, jobs=4, progress=None):
    os.makedirs(outdir, exist_ok=True)
    mods = mods or _ne_modules()
    from src.core import paths
    from concurrent.futures import ProcessPoolExecutor, as_completed
    cb = paths.repo_root()
    corpus = os.environ.get("WT_DATA_ROOT", os.getcwd())
    target_dir = os.path.join(cb, "src", "targets", "win95")
    os.environ["PYTHONPATH"] = os.pathsep.join(
        [cb, target_dir] + ([os.environ["PYTHONPATH"]] if os.environ.get("PYTHONPATH") else []))
    tot = collections.Counter()
    total = len(mods)
    done = 0
    print("PARALLEL 16-bit deep sweep: %d NE modules, %d workers" % (total, jobs), flush=True)
    with ProcessPoolExecutor(max_workers=jobs, initializer=_winit,
                             initargs=(corpus, target_dir, outdir, limit)) as ex:
        futs = {ex.submit(_wtask, m): m for m in mods}
        for fu in as_completed(futs):
            m = futs[fu]
            try:
                _m, st, nf, nr = fu.result()
            except Exception as e:
                st, nf, nr = "ERR:" + str(e)[:48], 0, 0
            done += 1
            if st == "ok":
                tot["funcs"] += nf; tot["readable"] += nr; tot["mods"] += 1
            if progress:
                progress(done, total, m, tot)
            if st == "ok" or st.startswith("ERR"):
                print("  [%d/%d] %-18s %s %s" % (done, total, m, st,
                      ("%d/%d reconstructed" % (nr, nf) if st == "ok" else "")), flush=True)
    print("=" * 60)
    print("16-BIT PARALLEL DEEP SWEEP: %d modules, %d funcs, %d reconstructed (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"], 100.0 * tot["readable"] / (tot["funcs"] or 1)))


from src.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if sys.argv[1:] and sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    outdir = argv[0] if argv else os.path.join(os.getcwd(), "src", "reconstructed", "ne_deep")
    limit = int(_opt(sys.argv[1:], "--limit") or 0)
    jobs = int(_opt(sys.argv[1:], "--jobs") or 1)
    mods = _opt(sys.argv[1:], "--mods")
    mods = mods.split(",") if mods else None
    print("16-bit NE deep reconstruction sweep (limit=%d, jobs=%d) -> %s" % (limit, jobs, outdir), flush=True)
    run(outdir, limit, mods, jobs=jobs)


if __name__ == "__main__":
    main()
