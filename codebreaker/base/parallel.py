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

    _di_parallel = cores.get('parallel')
    tmap = _di_parallel.tmap
    resolve_jobs = _di_parallel.resolve_jobs
    jobs = resolve_jobs(argv)                 # -j/--jobs N, or WIN95RE_JOBS, or all cores
    results = tmap(work_one, items, jobs=jobs, label="carve")
"""
import os
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed


def cpu_count():
    return os.cpu_count() or 4


def workers():
    """How many things run side by side, everywhere: the one number every part of the workbench
    asks for (compiles, judging, sweeps). settings.json "workers", or CB_JOBS; otherwise half the
    processors, at most 6 -- so that two parts running at once do not take the whole machine."""
    from codebreaker.base import settings
    n = os.environ.get("CB_JOBS") or settings.get("workers")
    try:
        n = int(n)
    except (TypeError, ValueError):
        n = 0
    return n if n > 0 else max(1, min(6, cpu_count() // 2))


# ---------------------------------------------------------------------------
# Long sweeps: one isolated subprocess per item.
#
# A process pool is the wrong tool for a multi-hour oracle sweep on Windows: one worker stuck
# inside an in-process emulator has no wall-clock limit, so the pool waits forever (this froze a
# corpus sweep for hours), and a dead worker takes the pool down with a BrokenPipe. Running each
# item as its own subprocess with a timeout bounds the damage to that item, and a state file makes
# the sweep resumable. Failures are recorded SEPARATELY from completions, so a timed-out item is
# retried on the next run instead of being silently counted as done.
# ---------------------------------------------------------------------------
def throttle(cores=None, below_normal=True):
    """Lower this process's priority and/or pin it to the first `cores` CPUs, so a background
    sweep leaves the machine usable. Child processes inherit both. Windows only; a no-op (False)
    elsewhere or on failure."""
    if os.name != "nt":
        return False
    try:
        import ctypes
        k = ctypes.windll.kernel32
        # OpenProcess on our own pid: the GetCurrentProcess pseudo-handle overflows a c_void_p.
        h = k.OpenProcess(0x1F0FFF, False, os.getpid())
        if not h:
            return False
        if below_normal:
            k.SetPriorityClass(h, 0x00004000)                # BELOW_NORMAL_PRIORITY_CLASS
        if cores:
            k.SetProcessAffinityMask(h, (1 << min(cores, cpu_count())) - 1)
        k.CloseHandle(h)
        return True
    except Exception:
        return False


def available_gb():
    """the memory the system can still give out without paging, in GB (None where not known)"""
    if os.name == "nt":
        try:
            import ctypes

            class MS(ctypes.Structure):
                _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                            ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                            ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                            ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                            ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]
            m = MS()
            m.dwLength = ctypes.sizeof(MS)
            if ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(m)):
                return min(m.ullAvailPhys, m.ullAvailPageFile) / 2 ** 30   # (RAM free, and commit left)
        except Exception:
            return None
    try:
        with open("/proc/meminfo") as f:
            for l in f:
                if l.startswith("MemAvailable:"):
                    return int(l.split()[1]) / 2 ** 20
    except OSError:
        pass
    return None


def min_free_gb():
    """settings.json "min_free_gb" (or CB_MIN_FREE_GB): below this much free memory no new heavy job starts"""
    from codebreaker.base import settings
    v = os.environ.get("CB_MIN_FREE_GB") or settings.get("min_free_gb")
    try:
        return float(v)
    except (TypeError, ValueError):
        return 6.0


def wait_for_memory(label="", poll=2.0, limit=600):
    """before a heavy job: wait while the system has less than min_free_gb free - the machine first, the work
    after. Returns the seconds waited."""
    import time
    need, waited = min_free_gb(), 0.0
    while True:
        a = available_gb()
        if a is None or a >= need or waited >= limit:
            return waited
        if waited == 0:
            import sys
            print("(waiting: %.1f GB free, %.1f wanted%s)" % (a, need, " - " + label if label else ""),
                  file=sys.stderr, flush=True)
        time.sleep(poll)
        waited += poll


def _load_state(path):
    import json
    try:
        with open(path) as f:
            st = json.load(f)
        st.setdefault("done", {}); st.setdefault("failed", {})
        return st
    except (OSError, ValueError):
        return {"done": {}, "failed": {}}


def _save_state(path, st):
    import json
    tmp = "%s.tmp.%d" % (path, os.getpid())
    with open(tmp, "w") as f:
        json.dump(st, f)
    os.replace(tmp, path)


def _last_json(text):
    import json
    for ln in reversed((text or "").strip().splitlines()):
        if ln.startswith("{"):
            try:
                return json.loads(ln)
            except ValueError:
                return None
    return None


def isolated_map(argv_for, items, key=str, jobs=4, timeout=1200, state_path=None,
                 max_tries=2, retry_failed=False, on_result=None, cwd=None, env=None):
    """Run one subprocess per item and collect each one's result.

    argv_for(item) -> the command line. The child reports by printing a JSON object as its last
    stdout line; an object carrying an "error" key, a timeout, a crash or no JSON at all counts
    as a FAILURE. With `state_path` the sweep is resumable: completed items are skipped, failed
    items are retried until they have been tried `max_tries` times (`retry_failed` retries them
    regardless). on_result(item, result, ok) is called in the parent as each item finishes.

    Returns {"done": {key: result}, "failed": {key: {"error", "tries"}}, "ran": n, "skipped": n}.
    """
    import subprocess
    st = _load_state(state_path) if state_path else {"done": {}, "failed": {}}
    todo, skipped = [], 0
    for it in items:
        k = key(it)
        if k in st["done"]:
            skipped += 1
        elif (not retry_failed and k in st["failed"]
              and st["failed"][k].get("tries", 0) >= max_tries):
            skipped += 1
        else:
            todo.append(it)
    lock = threading.Lock()

    def run(it):
        try:
            r = subprocess.run(argv_for(it), capture_output=True, text=True, timeout=timeout,
                               cwd=cwd, env=env)
            res = _last_json(r.stdout)
            if res is None:
                res = {"error": "no-output", "rc": r.returncode,
                       "stderr": (r.stderr or "").strip()[-300:]}
        except subprocess.TimeoutExpired:
            res = {"error": "timeout", "timeout": timeout}
        except Exception as e:
            res = {"error": type(e).__name__, "detail": str(e)[:200]}
        return res

    def record(it, res):
        k = key(it)
        ok = "error" not in res
        with lock:
            if ok:
                st["done"][k] = res
                st["failed"].pop(k, None)
            else:
                prev = st["failed"].get(k, {}).get("tries", 0)
                st["failed"][k] = dict(res, tries=prev + 1)
            if state_path:
                _save_state(state_path, st)
        if on_result:
            on_result(it, res, ok)

    if todo:
        with ThreadPoolExecutor(max_workers=max(1, min(jobs, len(todo)))) as ex:
            futs = {ex.submit(run, it): it for it in todo}
            for fut in as_completed(futs):
                record(futs[fut], fut.result())
    return {"done": st["done"], "failed": st["failed"], "ran": len(todo), "skipped": skipped}
