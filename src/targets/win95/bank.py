#!/usr/bin/env python3
r"""bank.py -- corpus-wide behavioural banking: verify every function of every module in a lane
against the retail binary and bank the ones that behave identically.

One command for both lanes:

    python -m src.targets.win95.bank pe                 # 32-bit lane, resume where it stopped
    python -m src.targets.win95.bank ne                 # 16-bit lane
    python -m src.targets.win95.bank pe --status        # progress, no work
    python -m src.targets.win95.bank pe --report        # verdicts, reasons, worst modules, what a build changed
    python -m src.targets.win95.bank pe --only kernel32.dll,user32.dll
    python -m src.targets.win95.bank pe --only @list.txt     # one module name per line
    python -m src.targets.win95.bank pe --retry-failed  # re-run units that timed out / crashed
    python -m src.targets.win95.bank pe --fresh         # forget this build's progress, start over

The unit of work is a CHUNK of one module's functions (--chunk N of them), run in its own
subprocess with a timeout (src.core.parallel.isolated_map). A module of any size is therefore a
series of bounded units: one hung or crashing chunk cannot stall the sweep or cost the rest of its
module, and a unit that fails is recorded as FAILED and retried later, never counted as done.
Progress is kept per DECOMPILER BUILD -- results from one build say nothing about another, so a
rebuilt decompiler starts a new sweep on its own. By default the sweep runs below normal priority
on six cores so the machine stays usable.
"""
import os
import sys
import csv
import json
import time

from src.core import corpus, parallel, paths, toolchain

LANES = ("pe", "ne")
GOOD = ("equivalent", "consistent")
CHUNK = {"pe": 400, "ne": 250}
_FIELDS = ["module", "chunk", "banked", "equivalent", "consistent", "divergent", "inconclusive",
           "no-synth", "no-compile", "secs"]


def state_dir():
    d = os.path.join(corpus.design_dir(), "bank")
    os.makedirs(d, exist_ok=True)
    return d


def _state_path(lane):
    return os.path.join(state_dir(), "%s.%s.state.json" % (lane, toolchain.decompiler_build_id()))


def _tally_path(lane):
    return os.path.join(state_dir(), "%s.%s.tally.csv" % (lane, toolchain.decompiler_build_id()))


def _scratch(lane, module, k):
    """Build scratch for one unit, outside the source tree."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bank", lane,
                        os.path.basename(module).lower(), "%d" % k)


# --- worker: one chunk of one module ---------------------------------------------------------
def verify_chunk(lane, path, k, size):
    """Verify + bank functions [k*size, (k+1)*size) of one module.
    Returns {module, chunk, total, banked, verdicts, secs}; `total` is the module's function count."""
    from src.targets.win95.verification import pipeline, store
    t0 = time.time()
    fns = pipeline.functions(lane, path)
    part = fns[k * size:(k + 1) * size]
    res = pipeline.verify(lane, path, part, outdir=_scratch(lane, path, k),
                          store=store.open(), bank=True) if part else {}
    cnt = pipeline.tally(res)
    return {"module": os.path.basename(path), "chunk": k, "total": len(fns),
            "banked": sum(cnt.get(v, 0) for v in GOOD), "verdicts": dict(cnt),
            "reused": sum(1 for r in res.values() if r.get("cached")),
            "secs": round(time.time() - t0, 1)}


def _worker(lane, path, k, size):
    try:
        res = verify_chunk(lane, path, k, size)
    except Exception as e:                       # report, never die silently: the parent records it
        res = {"module": os.path.basename(path), "chunk": k, "error": type(e).__name__,
               "detail": str(e)[:200]}
    print(json.dumps(res))


# --- parent ----------------------------------------------------------------------------------
def _key(unit):
    return "%s#%d" % (os.path.basename(unit[0]).lower(), unit[1])


def _append_tally(lane, res):
    p = _tally_path(lane)
    new = not os.path.exists(p)
    v = res.get("verdicts", {})
    with open(p, "a", newline="") as f:
        w = csv.writer(f)
        if new:
            w.writerow(_FIELDS)
        w.writerow([res.get("module"), res.get("chunk", 0), res.get("banked", 0)] +
                   [v.get(k, 0) for k in _FIELDS[3:-1]] + [res.get("secs", 0)])


def _summary(st, size):
    """Per-module roll-up of unit results: {module: {units_done, units_total, banked, tried}}."""
    mods = {}
    for key, r in st["done"].items():
        name = key.rsplit("#", 1)[0]
        m = mods.setdefault(name, {"units_done": 0, "units_total": 1, "banked": 0, "tried": 0})
        m["units_done"] += 1
        m["units_total"] = max(1, -(-int(r.get("total", 0)) // size))
        m["banked"] += r.get("banked", 0)
        m["tried"] += sum(r.get("verdicts", {}).values())
    return mods


def status(lane, size=None):
    size = size or CHUNK[lane]
    st = parallel._load_state(_state_path(lane))
    mods = _summary(st, size)
    complete = sum(1 for m in mods.values() if m["units_done"] >= m["units_total"])
    print("%s lane, decompiler build %s" % (lane.upper(), toolchain.decompiler_build_id()))
    print("  modules: %d complete, %d partly done, %d in the lane" % (
        complete, len(mods) - complete, len(corpus.modules(lane))))
    print("  units: %d done, %d failed" % (len(st["done"]), len(st["failed"])))
    print("  bodies: %d banked of %d verified" % (sum(m["banked"] for m in mods.values()),
                                                 sum(m["tried"] for m in mods.values())))
    for name, f in sorted(st["failed"].items()):
        print("  FAILED %-24s %s (tries=%s)" % (name, f.get("error"), f.get("tries")))
    return st


def report(lane):
    """What the results database says about a lane: verdicts, why bodies did not verify, the
    modules with the most unverified bodies, and what the current decompiler build changed."""
    from src.targets.win95.verification import store
    db = store.open()
    s = db.summary(lane)
    total = sum(s.values())
    if not total:
        print("no %s results recorded yet (%s)" % (lane.upper(), db.path))
        return
    good = sum(s.get(v, 0) for v in GOOD)
    print("%s lane: %d bodies recorded, %d verified (%.1f%%)" % (lane.upper(), total, good,
                                                               100.0 * good / total))
    for v, n in sorted(s.items(), key=lambda kv: -kv[1]):
        print("  %-13s %7d  %5.1f%%" % (v, n, 100.0 * n / total))
    for verdict in ("divergent", "inconclusive", "no-compile"):
        rs = db.reasons(lane, verdict)
        if rs:
            print("why %s:" % verdict)
            for r, n in list(rs.items())[:6]:
                print("  %7d  %s" % (n, r or "(no reason recorded)"))
    print("modules with the most unverified bodies:")
    rows = sorted(db.by_module(lane), key=lambda r: -(r[1] - (r[2] or 0)))[:10]
    for module, n, ok in rows:
        print("  %-20s %6d bodies, %6d verified (%4.1f%%)" % (module, n, ok or 0, 100.0 * (ok or 0) / n))
    build = toolchain.decompiler_build_id()
    fl = db.flips(lane, build)
    if fl:
        gained = sum(1 for _m, _k, _f, a, b in fl if b in GOOD and a not in GOOD)
        lost = sum(1 for _m, _k, _f, a, b in fl if a in GOOD and b not in GOOD)
        print("decompiler build %s changed %d verdicts: %d newly verified, %d no longer verified"
              % (build, len(fl), gained, lost))
        for m, k, f, a, b in [x for x in fl if x[3] in GOOD and x[4] not in GOOD][:10]:
            print("  REGRESSED %s %s (%s): %s -> %s" % (m, f, k, a, b))


def sweep(lane, only=None, jobs=5, cores=6, timeout=1500, size=None, retry_failed=False, fresh=False):
    if not toolchain.decompiler(lane):
        raise SystemExit("decompiler for lane %r is not built (%s)" % (lane, toolchain.decompiler_dir()))
    size = size or CHUNK[lane]
    mods = corpus.modules(lane)
    if only:
        want = {o.lower() for o in only}
        mods = [m for m in mods if os.path.basename(m).lower() in want]
    if fresh:
        for p in (_state_path(lane), _tally_path(lane)):
            if os.path.exists(p):
                os.remove(p)
    parallel.throttle(cores=cores, below_normal=True)
    t0 = time.time()
    seen = [0]
    print("=== bank %s: %d modules, chunks of %d functions, %d workers, decompiler build %s ===" % (
        lane, len(mods), size, jobs, toolchain.decompiler_build_id()), flush=True)

    def on_result(unit, res, ok):
        seen[0] += 1
        if ok:
            _append_tally(lane, res)
            v = res.get("verdicts", {})
            detail = " ".join("%s=%s" % kv for kv in sorted(v.items())) or "-"
            print("[%d] %-24s +%-4d (%s)%s  %.0fs" % (seen[0], _key(unit), res.get("banked", 0),
                  detail, "  reused=%d" % res["reused"] if res.get("reused") else "",
                  time.time() - t0), flush=True)
        else:
            print("[%d] %-24s FAILED %s  %.0fs" % (seen[0], _key(unit), res.get("error"),
                                                   time.time() - t0), flush=True)

    def run(units):
        return parallel.isolated_map(
            lambda u: [sys.executable, "-m", "src.targets.win95.bank", "--one", lane, u[0],
                       str(u[1]), str(size)],
            units, key=_key, jobs=jobs, timeout=timeout, state_path=_state_path(lane),
            retry_failed=retry_failed, on_result=on_result, cwd=paths.repo_root())

    # Pass 1: the first chunk of every module, which also reports the module's function count.
    out = run([(m, 0) for m in mods])
    # Pass 2: the remaining chunks of the modules that have more than one.
    rest = []
    for m in mods:
        first = out["done"].get(_key((m, 0)))
        if first:
            rest += [(m, k) for k in range(1, -(-int(first.get("total", 0)) // size))]
    if rest:
        out = run(rest)
    mods_sum = _summary(out, size)
    print("=== %s: %d units done, %d failed; %d bodies banked of %d verified, in %.0fs ===" % (
        lane, len(out["done"]), len(out["failed"]), sum(m["banked"] for m in mods_sum.values()),
        sum(m["tried"] for m in mods_sum.values()), time.time() - t0), flush=True)
    return out


def main(argv):
    if len(argv) >= 3 and argv[0] == "--one":
        k = int(argv[3]) if len(argv) > 3 else 0
        size = int(argv[4]) if len(argv) > 4 else CHUNK[argv[1]]
        return _worker(argv[1], argv[2], k, size)
    if not argv or argv[0] not in LANES:
        print(__doc__)
        return
    lane = argv[0]

    def opt(flag, default=None):
        return argv[argv.index(flag) + 1] if flag in argv else default
    size = int(opt("--chunk", CHUNK[lane]))
    if "--status" in argv:
        status(lane, size)
        return
    if "--report" in argv:
        report(lane)
        return
    only = opt("--only")
    if only and only.startswith("@"):
        only = [ln.strip() for ln in open(only[1:]) if ln.strip()]
    elif only:
        only = only.split(",")
    sweep(lane, only=only, jobs=int(opt("--jobs", 5)), cores=int(opt("--cores", 6)),
          timeout=int(opt("--timeout", 1500)), size=size,
          retry_failed="--retry-failed" in argv, fresh="--fresh" in argv)


if __name__ == "__main__":
    main(sys.argv[1:])
