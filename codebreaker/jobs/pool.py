"""pool.py -- do one thing to many items across worker processes, and survive a worker dying.

    from codebreaker.jobs import pool
    results = pool.run(work, items, context=(module_path, dll))     # {key: result}

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
        parallel.throttle(cores=int(os.environ.get("CB_CORES", "6")), below_normal=True)
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


def default_jobs():
    return int(os.environ.get("CB_JOBS", "5"))


def run(work, items, context=None, key=_first, jobs=None, crashed=None, small=24):
    """-> {key(item): work(context, item)} for every item."""
    items = list(items)
    jobs = jobs or default_jobs()
    crashed = crashed or (lambda item: None)
    if jobs <= 1 or len(items) < small:
        return _chunk(work, context, items, key, None)
    from concurrent.futures import ProcessPoolExecutor
    size = max(8, -(-len(items) // (jobs * 6)))
    out, todo = {}, items
    for _round in range(12):
        if not todo:
            break
        with tempfile.TemporaryDirectory(prefix="cbjobs") as mark:
            chunks = [todo[i:i + size] for i in range(0, len(todo), size)]
            with ProcessPoolExecutor(max_workers=jobs) as ex:
                futs = [(ex.submit(_chunk, work, context, c, key, _sub(mark, i)), c) for i, c in enumerate(chunks)]
                for fut, _c in futs:
                    try:
                        out.update(fut.result())
                    except ImportError:
                        raise                               # a broken setup is not a crashed item
                    except Exception:
                        pass
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
                with ProcessPoolExecutor(max_workers=1) as ex:
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
