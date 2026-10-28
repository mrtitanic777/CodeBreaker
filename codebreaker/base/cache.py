"""cache.py -- never do the same work twice: results kept by what they were made from.

    from codebreaker.base import cache
    k = cache.key(cache.tool(compiler), flags, header, text)     # everything the result depends on
    data = cache.get("objects", k)                               # bytes, or None
    cache.put("objects", k, data)
    answer = cache.value("checks", k)                            # any Python value, or None
    cache.keep("checks", k, answer)
    p = cache.path("port-objects", k)                            # a file of its own, for a tool to write
    cache.prune("port-objects", 3 << 30)                         # the longest unused gone first, under a cap

A compiler given the same source and flags gives the same object file; the decompiler given the
same bytes gives the same C. Both take most of the time of every run, and most of what a run is
given is what the run before was given. So the result is filed under a hash of everything it
depends on -- the tool itself (its name, size and date), its options, its input -- and looked up
before the tool is started. Nothing is ever stale: a different input is a different name.

The files are under state/cache/<kind>/. Deleting the folder costs time and nothing else.
CB_NO_CACHE=1 turns every lookup into a miss.
"""
import hashlib
import os
import pickle

_HITS = {}


def key(*parts):
    h = hashlib.sha1()
    for p in parts:
        if isinstance(p, (list, tuple)):
            p = "\x1f".join(str(x) for x in p)
        if not isinstance(p, bytes):
            p = str(p).encode("utf-8", "replace")
        h.update(b"%d:" % len(p))
        h.update(p)
    return h.hexdigest()


def tool(path):
    """What identifies a program: a newer build of it is a different tool."""
    try:
        st = os.stat(path)
        return "%s:%d:%d" % (os.path.basename(path).lower(), st.st_size, int(st.st_mtime))
    except OSError:
        return "%s:missing" % os.path.basename(path or "")


def folder(path):
    """The same for a folder of files a tool reads (a compiler's headers, a language's rules)."""
    parts = []
    for root, _dirs, files in os.walk(path):
        for f in sorted(files):
            try:
                st = os.stat(os.path.join(root, f))
                parts.append("%s:%d:%d" % (f, st.st_size, int(st.st_mtime)))
            except OSError:
                pass
    return key(parts)


def off():
    """Is caching turned off for this run (CB_NO_CACHE)?"""
    return bool(os.environ.get("CB_NO_CACHE"))


def path(kind, k):
    """Where the result filed under `k` is (its folder is made when it is written)."""
    from codebreaker.base import settings
    return os.path.join(settings.state("cache"), kind, k[:2], k)


def _count(kind, found):
    hit, miss = _HITS.get(kind, (0, 0))
    _HITS[kind] = (hit + 1, miss) if found else (hit, miss + 1)


def get(kind, k):
    if off():
        return None
    try:
        with open(path(kind, k), "rb") as f:
            data = f.read()
    except OSError:
        _count(kind, False)
        return None
    _count(kind, True)
    return data


def put(kind, k, data):
    if off():
        return
    p = path(kind, k)
    try:
        os.makedirs(os.path.dirname(p), exist_ok=True)
        tmp = "%s.%d.tmp" % (p, os.getpid())
        with open(tmp, "wb") as f:
            f.write(data)
        os.replace(tmp, p)                      # whole or not at all: two workers may write the same
    except OSError:
        pass


def value(kind, k):
    """A value kept by keep(), or None (none, or one another process is still writing)."""
    data = get(kind, k)
    if data is None:
        return None
    try:
        return pickle.loads(data)
    except (pickle.UnpicklingError, EOFError, ValueError, TypeError, AttributeError, ImportError):
        return None


def keep(kind, k, v):
    """Keep any value that pickles under `k`; one that does not is not kept."""
    try:
        data = pickle.dumps(v, protocol=pickle.HIGHEST_PROTOCOL)
    except (pickle.PicklingError, TypeError, AttributeError):
        return
    put(kind, k, data)


def prune(kind, cap):
    """Hold a kind's files under `cap` bytes: past four fifths of it, the ones longest unused (by
    their time) go first."""
    from codebreaker.base import settings
    files = []
    for d, _dirs, names in os.walk(os.path.join(settings.state("cache"), kind)):
        for n in names:
            try:
                st = os.stat(os.path.join(d, n))
                files.append((st.st_mtime, st.st_size, os.path.join(d, n)))
            except OSError:
                pass
    total = sum(s for _t, s, _p in files)
    for _t, s, p in sorted(files):
        if total <= cap * 0.8:
            break
        try:
            os.remove(p)
            total -= s
        except OSError:
            pass


def counts():
    """{kind: (found, not found)} in this process, for a run's summary."""
    return dict(_HITS)
