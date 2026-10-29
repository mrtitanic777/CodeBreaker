"""pool.py -- do one thing to many items at once: the one way CodeBreaker runs work side by side.

    from codebreaker.jobs import pool
    results = pool.run(work, items, context=(module_path, dll))     # {key: result}, worker processes
    texts = pool.threads(compile_one, names, jobs=4)                # [result], in the items' order
    for item, result in pool.as_done(build_one, items):            # each as it finishes
        ...
    out = pool.isolated(argv_for, items, state_path=p)              # a subprocess each, resumable

How many run at once is the machine's budget (base/parallel.py workers(): settings.json
"workers") unless `jobs` says. Threads are for work that waits on another program -- a
compiler, an emulator, a DOSBox -- and processes for work that computes in Python or runs
native code that may crash.

`work(context, item)` runs in a worker process and returns that item's result; `key(item)`
(default: the item's first element, or the item) names it in the result. It must be a
module-level function.

Native code under test can take a whole process down -- an emulator fault is not a Python
exception. A dead worker takes its pool with it, so each worker notes the item it is on; after
a crash the items that were in progress are each retried alone (four of five were innocent),
the one that kills its process again gets `crashed(item)` as its result, and everything else
that had not finished is run again. No item is silently dropped.

Workers run below normal priority on a bounded number of cores, so the machine stays usable.
"""
import os
import tempfile


def _chunk(work, context, items, key, mark):
    """One worker's share. -> {key: result}."""
    try:
        from codebreaker.base import parallel
        # (on as many cores as the machine's budget: the workers are no more than that)
        parallel.throttle(cores=int(os.environ.get("CB_CORES") or 0) or parallel.workers(), below_normal=True)
    except Exception:
        pass
    note = os.path.join(mark, "%d.cur" % os.getpid()) if mark else None
    out = {}
    for n, item in enumerate(items):
        if note:
            with open(note, "w") as f:
                f.write(str(n))
        out[key(item)] = work(context, item)
    if note:
        with open(note, "w") as f:
            f.write("")
    return out


def _first(item):
    return item[0] if isinstance(item, (tuple, list)) else item


def processes_safe():
    """Can worker processes be started from here? A worker process on Windows starts by running
    the calling script again, as __mp_main__: a script with no `if __name__ == "__main__":`
    guard would run whatever it does -- a whole module build -- once in every worker before
    failing. Safe: run as `python -m ...`, `python -c ...` or interactively, or a script that
    has the guard."""
    import sys
    main = sys.modules.get("__main__")
    if main is None or getattr(getattr(main, "__spec__", None), "name", None) or not getattr(main, "__file__", None):
        return True
    try:
        with open(main.__file__, encoding="utf-8", errors="replace") as f:
            import re
            return bool(re.search(r"""__name__\s*==\s*['"]__main__['"]""", f.read()))
    except OSError:
        return False


def default_jobs():
    from codebreaker.base import parallel
    return parallel.workers()


def run(work, items, context=None, key=_first, jobs=None, crashed=None, small=24, init=None, initargs=(),
        progress=None, size=None):
    """-> {key(item): work(context, item)} for every item. A worker is given `size` items at a time
    (default: a sixth of its share, at least eight); init(*initargs) is run first in each worker
    process; progress(done, total) is told as each share comes back."""
    items = list(items)
    jobs = jobs or default_jobs()
    crashed = crashed or (lambda item: None)
    if jobs <= 1 or len(items) < small or not processes_safe():
        return _chunk(work, context, items, key, None)
    from concurrent.futures import ProcessPoolExecutor
    size = size or max(8, -(-len(items) // (jobs * 6)))
    out, todo = {}, items
    for _round in range(12):
        if not todo:
            break
        with tempfile.TemporaryDirectory(prefix="cbjobs") as mark:
            chunks = [todo[i:i + size] for i in range(0, len(todo), size)]
            with ProcessPoolExecutor(max_workers=jobs, initializer=init, initargs=initargs) as ex:
                futs = [(ex.submit(_chunk, work, context, c, key, _sub(mark, i)), c) for i, c in enumerate(chunks)]
                for fut, _c in futs:
                    try:
                        out.update(fut.result())
                    except ImportError:
                        raise                               # a broken setup is not a crashed item
                    except Exception:
                        pass
                    if progress:
                        progress(len(out), len(items))
            suspects = []
            for i, c in enumerate(chunks):                  # which item was each dead worker on
                d = _sub(mark, i, make=False)
                for name in (os.listdir(d) if os.path.isdir(d) else ()):
                    with open(os.path.join(d, name)) as f:
                        cur = f.read().strip()
                    if cur and key(c[int(cur)]) not in out:
                        suspects.append(c[int(cur)])
        todo = [it for it in todo if key(it) not in out]
        if not todo:
            break
        if not suspects:                                    # died outside any item: go one at a time
            size = 1
            continue
        for item in suspects:
            try:
                with ProcessPoolExecutor(max_workers=1, initializer=init, initargs=initargs) as ex:
                    out.update(ex.submit(_chunk, work, context, [item], key, None).result())
            except ImportError:
                raise
            except Exception:
                out[key(item)] = crashed(item)
        todo = [it for it in todo if key(it) not in out]
    for item in todo:
        out[key(item)] = crashed(item)
    return out


def _sub(mark, i, make=True):
    d = os.path.join(mark, str(i))
    if make:
        os.makedirs(d, exist_ok=True)
    return d


def _jobs(jobs):
    return max(1, jobs) if jobs is not None else default_jobs()


def threads(work, items, jobs=None):
    """-> [work(item) for item in items], `jobs` at a time on threads of this process. An item's
    exception is raised here, once every item has ended."""
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(max_workers=_jobs(jobs)) as ex:
        return list(ex.map(work, items))


def as_done(work, items, jobs=None, processes=False, failed=None):
    """Yield (item, work(item)) for each item as it finishes, `jobs` at a time: on threads, or with
    `processes` in worker processes (`work` then a module-level function). failed(item, error),
    when given, is the result of an item whose work raised; otherwise the error is raised here."""
    from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor, as_completed
    with (ProcessPoolExecutor if processes else ThreadPoolExecutor)(max_workers=_jobs(jobs)) as ex:
        futs = {ex.submit(work, it): it for it in items}
        for fut in as_completed(futs):
            try:
                r = fut.result()
            except Exception as e:
                if failed is None:
                    raise
                r = failed(futs[fut], e)
            yield futs[fut], r


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


def isolated(argv_for, items, key=str, jobs=4, timeout=1200, state_path=None,
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

    def run_one(it):
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

    if todo:
        for it, res in as_done(run_one, todo, jobs=min(jobs, len(todo))):
            k = key(it)
            ok = "error" not in res
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
    return {"done": st["done"], "failed": st["failed"], "ran": len(todo), "skipped": skipped}
