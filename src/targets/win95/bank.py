#!/usr/bin/env python3
r"""bank.py -- corpus-wide behavioural banking: verify every function of every module in a lane
against the retail binary and bank the ones that behave identically.

One command for both lanes (it replaces the per-lane scratch runners):

    python -m src.targets.win95.bank pe                 # 32-bit lane, resume where it stopped
    python -m src.targets.win95.bank ne                 # 16-bit lane
    python -m src.targets.win95.bank pe --status        # progress, no work
    python -m src.targets.win95.bank pe --only kernel32.dll,user32.dll
    python -m src.targets.win95.bank pe --retry-failed  # re-run modules that timed out / crashed
    python -m src.targets.win95.bank pe --fresh         # forget this build's progress, start over

Each module runs in its own subprocess with a timeout (src.core.parallel.isolated_map), so one
hung or crashing module cannot stall the sweep, and a module that fails is recorded as FAILED and
retried later instead of being counted as done. Progress is kept per DECOMPILER BUILD: results
from one build say nothing about another, so a rebuilt decompiler starts a new sweep on its own.
By default the sweep runs below normal priority on six cores so the machine stays usable.
"""
import os
import sys
import csv
import json
import time

from src.core import corpus, parallel, paths, toolchain

LANES = ("pe", "ne")
GOOD = ("equivalent", "consistent")
_FIELDS = ["module", "banked", "equivalent", "consistent", "divergent", "inconclusive",
           "no-synth", "no-compile", "secs"]


def state_dir():
    d = os.path.join(corpus.design_dir(), "bank")
    os.makedirs(d, exist_ok=True)
    return d


def _state_path(lane):
    return os.path.join(state_dir(), "%s.%s.state.json" % (lane, toolchain.decompiler_build_id()))


def _tally_path(lane):
    return os.path.join(state_dir(), "%s.%s.tally.csv" % (lane, toolchain.decompiler_build_id()))


def _scratch(lane, module):
    """Build scratch for one module, outside the source tree."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bank", lane,
                        os.path.basename(module).lower())


# --- worker: one module ----------------------------------------------------------------------
def verify_module(lane, path):
    """Verify + bank every function of one module. Returns {module, banked, verdicts, secs}."""
    from collections import Counter
    t0 = time.time()
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)                 # several win95 tools still import siblings bare
    out = _scratch(lane, path)
    if lane == "pe":
        from src.targets.win95 import bodyverify
        cnt = bodyverify.verify_module_batched(path, outdir=out, bank=True, fast=True)
    else:
        from src.engines.behavior import interproc16
        from src.formats.ne import NEFile
        from src.targets.win95 import bodyverify16
        cnt = Counter()
        for seg, off in interproc16.all_functions(NEFile(path)):
            try:
                r = bodyverify16.verify_body16(path, seg, off,
                                               outdir=os.path.join(out, "%d_%x" % (seg, off)))
                cnt[r["verdict"]] += 1
            except Exception as e:
                cnt["error:" + type(e).__name__] += 1
    return {"module": os.path.basename(path), "banked": sum(cnt.get(v, 0) for v in GOOD),
            "verdicts": dict(cnt), "secs": round(time.time() - t0, 1)}


def _worker(lane, path):
    try:
        res = verify_module(lane, path)
    except Exception as e:                       # report, never die silently: the parent records it
        res = {"module": os.path.basename(path), "error": type(e).__name__, "detail": str(e)[:200]}
    print(json.dumps(res))


# --- parent ----------------------------------------------------------------------------------
def _append_tally(lane, res):
    p = _tally_path(lane)
    new = not os.path.exists(p)
    v = res.get("verdicts", {})
    with open(p, "a", newline="") as f:
        w = csv.writer(f)
        if new:
            w.writerow(_FIELDS)
        w.writerow([res.get("module"), res.get("banked", 0)] +
                   [v.get(k, 0) for k in _FIELDS[2:-1]] + [res.get("secs", 0)])


def status(lane):
    st = parallel._load_state(_state_path(lane))
    total = len(corpus.modules(lane))
    banked = sum(r.get("banked", 0) for r in st["done"].values())
    print("%s lane, decompiler build %s" % (lane.upper(), toolchain.decompiler_build_id()))
    print("  modules: %d done, %d failed, %d total" % (len(st["done"]), len(st["failed"]), total))
    print("  bodies banked: %d" % banked)
    for name, f in sorted(st["failed"].items()):
        print("  FAILED %-20s %s (tries=%s)" % (name, f.get("error"), f.get("tries")))
    return st


def sweep(lane, only=None, jobs=5, cores=6, timeout=2400, retry_failed=False, fresh=False):
    if not toolchain.decompiler(lane):
        raise SystemExit("decompiler for lane %r is not built (%s)" % (lane, toolchain.decompiler_dir()))
    mods = corpus.modules(lane)
    if only:
        want = {o.lower() for o in only}
        mods = [m for m in mods if os.path.basename(m).lower() in want]
    if fresh:
        for p in (_state_path(lane), _tally_path(lane)):
            if os.path.exists(p):
                os.remove(p)
    parallel.throttle(cores=cores, below_normal=True)
    key = lambda p: os.path.basename(p).lower()
    t0 = time.time()
    seen = [0]
    print("=== bank %s: %d modules, %d workers, decompiler build %s ===" % (
        lane, len(mods), jobs, toolchain.decompiler_build_id()), flush=True)

    def on_result(path, res, ok):
        seen[0] += 1
        if ok:
            _append_tally(lane, res)
            v = res.get("verdicts", {})
            detail = " ".join("%s=%s" % kv for kv in sorted(v.items())) or "-"
            print("[%d] %-20s +%-4d (%s)  %.0fs" % (seen[0], key(path), res.get("banked", 0),
                                                    detail, time.time() - t0), flush=True)
        else:
            print("[%d] %-20s FAILED %s  %.0fs" % (seen[0], key(path), res.get("error"),
                                                   time.time() - t0), flush=True)

    out = parallel.isolated_map(
        lambda p: [sys.executable, "-m", "src.targets.win95.bank", "--one", lane, p],
        mods, key=key, jobs=jobs, timeout=timeout, state_path=_state_path(lane),
        retry_failed=retry_failed, on_result=on_result, cwd=paths.repo_root())
    banked = sum(r.get("banked", 0) for r in out["done"].values())
    print("=== %s: %d modules done, %d failed, %d bodies banked (ran %d, skipped %d) in %.0fs ===" % (
        lane, len(out["done"]), len(out["failed"]), banked, out["ran"], out["skipped"],
        time.time() - t0), flush=True)
    return out


def main(argv):
    if len(argv) >= 3 and argv[0] == "--one":
        return _worker(argv[1], argv[2])
    if not argv or argv[0] not in LANES:
        print(__doc__)
        return
    lane = argv[0]

    def opt(flag, default=None):
        return argv[argv.index(flag) + 1] if flag in argv else default
    if "--status" in argv:
        status(lane)
        return
    only = opt("--only")
    sweep(lane, only=only.split(",") if only else None,
          jobs=int(opt("--jobs", 5)), cores=int(opt("--cores", 6)),
          timeout=int(opt("--timeout", 2400)),
          retry_failed="--retry-failed" in argv, fresh="--fresh" in argv)


if __name__ == "__main__":
    main(sys.argv[1:])
