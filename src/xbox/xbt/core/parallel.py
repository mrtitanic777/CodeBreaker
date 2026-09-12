#!/usr/bin/env python3
"""
parallel.py — shared concurrency for the workbench. Make the slow passes fast.

WHY THREADS (and why not GPU). The heavy work in this project is x86
disassembly (capstone — a ctypes C-extension that RELEASES the GIL on every
call) and running the period compiler/assembler (DOSBox subprocess — also
releases the GIL). Both are precisely what a thread pool speeds up on a
multicore CPU, with none of the pickling / spawn / freeze_support fragility a
process pool brings to a PyInstaller onefile EXE. So the default fan-out is
threads.

A GPU buys nothing here: disassembly, function carving, OMF parsing and the
byte-exact permuter are irregular, branch-heavy, pointer-chasing work with no
data-parallel kernel to offload. The real levers are CPU cores and running many
subprocesses at once — which is exactly what this module provides.

A process pool (`pmap`) is offered for genuinely CPU-bound *pure-Python* fan-out
where the GIL would otherwise bottleneck; it falls back to threads if the
workers can't be pickled (common in the frozen EXE), so callers never break.

    from parallel import tmap, resolve_jobs
    jobs = resolve_jobs(argv)                 # -j/--jobs N, or WIN95RE_JOBS, or all cores
    results = tmap(work_one, items, jobs=jobs, label="carve")
"""
import os
import sys
import threading
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed


def cpu_count():
    return os.cpu_count() or 4


def resolve_jobs(argv=None, default=None):
    """Pop `-j N` / `--jobs N` from argv (mutating it) or read $WIN95RE_JOBS;
    fall back to `default` or all cores. Always returns >= 1."""
    argv = sys.argv if argv is None else argv
    jobs = None
    for flag in ("-j", "--jobs"):
        while flag in argv:
            i = argv.index(flag)
            try:
                jobs = int(argv[i + 1]); del argv[i:i + 2]
            except (IndexError, ValueError):
                del argv[i:i + 1]
    if jobs is None:
        env = os.environ.get("WIN95RE_JOBS", "")
        jobs = int(env) if env.isdigit() else (default or cpu_count())
    return max(1, jobs)


def _bar(label, done, total):
    if not label or not sys.stderr.isatty():   # no \r spam when piped to a file
        return
    pct = (done * 100 // total) if total else 100
    sys.stderr.write("\r  %s: %d/%d (%d%%)   " % (label, done, total, pct))
    sys.stderr.flush()
    if done >= total:
        sys.stderr.write("\n")


def _prog():
    try:
        import progress
        return progress if progress.active() else None
    except Exception:
        return None


def tmap(fn, items, jobs=None, label=None):
    """Thread-pool map. Ordered results, live progress to stderr. Runs serially
    when jobs<=1. Safe in the frozen EXE (no pickling, shared memory)."""
    items = list(items)
    jobs = jobs or cpu_count()
    n = len(items)
    pr = _prog()
    if pr and label:
        pr.auto_op(label, n)
    if jobs <= 1 or n <= 1:
        out = []
        for i, it in enumerate(items):
            out.append(fn(it)); _bar(label, i + 1, n)
            if pr:
                pr.auto_bar(i + 1, n)
        return out
    results = [None] * n
    done = 0
    lock = threading.Lock()
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = {ex.submit(fn, it): i for i, it in enumerate(items)}
        for fut in as_completed(futs):
            results[futs[fut]] = fut.result()
            with lock:
                done += 1; _bar(label, done, n)
                if pr:
                    pr.auto_bar(done, n)
    return results


def pmap(fn, items, jobs=None, label=None):
    """Process-pool map for CPU-bound pure-Python fan-out. Falls back to `tmap`
    (threads) if the pool can't start or workers can't be pickled — so this is
    always safe to call, it just may not get true parallelism in the frozen EXE.
    `fn` must be a top-level importable function for the process path to engage."""
    items = list(items)
    jobs = jobs or cpu_count()
    n = len(items)
    if jobs <= 1 or n <= 1:
        return tmap(fn, items, jobs=1, label=label)
    pr = _prog()
    if pr and label:
        pr.auto_op(label, n)
    try:
        results = [None] * n
        done = 0
        with ProcessPoolExecutor(max_workers=jobs) as ex:
            futs = {ex.submit(fn, it): i for i, it in enumerate(items)}
            for fut in as_completed(futs):
                results[futs[fut]] = fut.result()
                done += 1; _bar(label, done, n)
                if pr:
                    pr.auto_bar(done, n)
        return results
    except Exception as e:                       # pickling / spawn / frozen quirks
        sys.stderr.write("\n  (process pool unavailable: %s; using threads)\n" % e)
        return tmap(fn, items, jobs=jobs, label=label)
