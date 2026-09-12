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
    if not allm:
        # No per-module asm tree in this corpus -> enumerate the binaries directly (the behavioral
        # lane carves from the binary, so the asm tree is not required). target.all_modules() returns
        # every NE/PE/LE/LX module path under files/.
        try:
            allm = sorted({os.path.basename(p) for p in target.all_modules()})
        except Exception:
            allm = []
    allset = set(allm)
    ordered = [m for m in PRIORITY if m in allset] + [m for m in allm if m not in set(PRIORITY)]
    return ordered


def _emit_module(outdir, per, m):
    """Reconstruct ONE PE module and write <stem>.fused.c. The unit of work shared by the
    sequential loop AND each parallel worker. Returns (status, n_funcs, n_readable). RESUME-safe:
    a module whose .fused.c already exists returns ('skip',...) untouched."""
    import compose
    stem = m.replace(".", "_")
    outp = os.path.join(outdir, stem + ".fused.c")
    if os.path.exists(outp):
        return ("skip", 0, 0)
    try:
        if target.format_of(target.resolve(m)) != "PE":
            return ("notpe", 0, 0)
    except Exception:
        return ("resolveerr", 0, 0)
    try:
        rs = compose.reconstruct_module(m, per, fuse=True, behav_limit=per)
    except Exception as e:
        return ("ERR:" + str(e)[:48], 0, 0)
    readable = [r for r in rs if r.status != "floor"]
    tmp = outp + ".part"                                      # write-then-rename so a crash mid-write
    with open(tmp, "w", encoding="utf-8") as f:               # never leaves a half file that resume skips
        f.write("/* %s -- behavioral reconstruction (compose): readable C where recoverable,\n"
                "   validated against the binary oracle. */\n\n" % m)
        try:
            pre = compose.module_preamble(m)
        except Exception:
            pre = ""
        if pre:
            f.write(pre + "\n")
        for r in rs:
            if r.source:
                f.write(compose._fn_header(r) + "%s\n\n" % r.source.rstrip())
    os.replace(tmp, outp)
    return ("ok", len(rs), len(readable))


def run(outdir, per, mods=None, progress=None, jobs=1):
    os.makedirs(outdir, exist_ok=True)
    mods = mods or _modules()
    if jobs and jobs > 1:
        return run_parallel(outdir, per, mods, jobs=jobs, progress=progress)
    tot = collections.Counter()
    total = len(mods)
    for _i, m in enumerate(mods, 1):
        if progress:
            progress(_i, total, m, tot)
        st, nf, nr = _emit_module(outdir, per, m)
        if st == "ok":
            tot["funcs"] += nf; tot["readable"] += nr; tot["mods"] += 1
            print("  %-16s %d/%d readable" % (m, nr, nf), flush=True)
        elif st.startswith("ERR"):
            print("  SKIP %-14s (%s)" % (m, st), flush=True)
    print("=" * 60)
    print("BEHAVIORAL SWEEP (this run): %d modules, %d funcs, %d readable (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"],
           100.0 * tot["readable"] / (tot["funcs"] or 1)))


# ----------------------------------------------------------------- module-level PARALLELISM
# Modules are independent (each reconstructs its own binary and writes its own .fused.c), so the
# deep lane is embarrassingly parallel. A process pool (not threads -- Unicorn + the GIL) runs N
# modules at once for a ~linear Nx speedup with IDENTICAL output. Workers persist across modules so
# the heavy setup (import unicorn/capstone/engine, set target, build the .sym index) is paid once
# per worker, not per module.
_PW = {}


def _winit(corpus, target_dir, outdir, per):
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
    _PW["per"] = per


def _wtask(m):
    try:
        return (m,) + _emit_module(_PW["outdir"], _PW["per"], m)
    except Exception as e:
        return (m, "ERR:" + str(e)[:48], 0, 0)


def run_parallel(outdir, per, mods=None, jobs=4, progress=None):
    os.makedirs(outdir, exist_ok=True)
    mods = mods or _modules()
    from src.core import paths
    from concurrent.futures import ProcessPoolExecutor, as_completed
    cb = paths.repo_root()
    corpus = os.environ.get("WT_DATA_ROOT", os.getcwd())
    target_dir = os.path.join(cb, "src", "targets", "win95")
    # spawned workers must be able to import the package + the targets dir -> inherit via PYTHONPATH
    os.environ["PYTHONPATH"] = os.pathsep.join(
        [cb, target_dir] + ([os.environ["PYTHONPATH"]] if os.environ.get("PYTHONPATH") else []))
    tot = collections.Counter()
    total = len(mods)
    done = 0
    print("PARALLEL deep sweep: %d modules, %d workers" % (total, jobs), flush=True)
    with ProcessPoolExecutor(max_workers=jobs, initializer=_winit,
                             initargs=(corpus, target_dir, outdir, per)) as ex:
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
                print("  [%d/%d] %-16s %s %s" % (done, total, m, st,
                      ("%d/%d readable" % (nr, nf) if st == "ok" else "")), flush=True)
    print("=" * 60)
    print("PARALLEL SWEEP: %d modules, %d funcs, %d readable (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"], 100.0 * tot["readable"] / (tot["funcs"] or 1)))


from src.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if sys.argv[1:] and sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    outdir = argv[0] if argv else os.path.join(os.getcwd(), "src", "reconstructed", "behavioral")
    per = int(_opt(sys.argv[1:], "--per") or 60)
    jobs = int(_opt(sys.argv[1:], "--jobs") or 1)
    mods = _opt(sys.argv[1:], "--mods")
    mods = mods.split(",") if mods else None
    print("behavioral reconstruction sweep (per=%d, jobs=%d) -> %s" % (per, jobs, outdir), flush=True)
    run(outdir, per, mods, jobs=jobs)


if __name__ == "__main__":
    main()
