"""cache.py -- never do the same work twice: results kept by what they were made from.

    from codebreaker.base import cache
    k = cache.key(cache.tool(compiler), flags, header, text)     # everything the result depends on
    data = cache.get("objects", k)                               # bytes, or None
    cache.put("objects", k, data)
    answer = cache.value("checks", k)                            # any Python value, or None
    cache.keep("checks", k, answer)
    p = cache.path("port-objects", k)                            # a file of its own, for a tool to write
    cache.hold()                                                 # back under the size cap (after such files)

    python -m codebreaker cache [--stats]     what is kept, kind by kind, and the cap
    python -m codebreaker cache --prune       old generations dropped, the cap held
    python -m codebreaker cache --clear       everything kept, gone

A compiler given the same source and flags gives the same object file; the decompiler given the
same bytes gives the same C. Both take most of the time of every run, and most of what a run is
given is what the run before was given. So the result is filed under a hash of everything it
depends on -- the tool itself (its name, size and date), its options, its input -- and looked up
before the tool is started. Nothing is ever stale: a different input is a different name.

But a name no one asks for again is never asked for again, and kept for ever it is only weight
(the judge's fingerprints once filled 10 GB, mostly under revisions of the comparer long gone). So
the cache looks after its own size, by two rules:

  GENERATIONS  every kind of result is made by some version of the code, and the version is in
               its keys already (the comparer's REV, the decompiler's build and languages). The
               kind says which, once, in GENERATIONS below; its results are kept apart by it, and
               when the kind is first used in a process the results of every other generation
               go: the code of now can never ask for them. Going is one file deleted, not a
               look through millions of names.

  THE CAP      all of state's caches together are held under the setting `cache_max_gb`
               (default 2). As results are written, past the cap the ones used longest ago go
               first, from any kind, until four fifths of it is used.

SHARED       the kinds in SHARED below -- what an assembler or a compiler made of its inputs --
               are kept apart from any one state folder, in the build cache (the setting
               build_cache, default the main checkout's state/build_cache), so a fresh state
               or another worktree reuses them. Their keys name every input by its bytes
               (tool_bytes, files_bytes), never by a path or a date; their generations are
               never dropped (another checkout may run other code), only the cap
               (build_cache_max_gb, default 6) holds them.

Each kind is one sqlite file, state/cache/<kind>/<generation>.sqlite, its results rows in it (many
small results are one file, not millions); a kind whose results are files a tool writes itself
(path()) has a folder, state/cache/<kind>/<generation>/, instead. Deleting state/cache costs time
and nothing else. CB_NO_CACHE=1 turns every lookup into a miss and keeps nothing.
"""
import hashlib
import importlib
import os
import pickle
import shutil
import sqlite3
import sys
import threading
import time
import uuid

# The version of the code that makes each kind: the module, and the name in it (a value, or a
# function giving one), of what in the kind's keys changes with the code. A kind not named here
# has one generation; its results go only by the cap.
GENERATIONS = {
    "fingerprints": ("codebreaker.verify.compare", "REV"),
    "masm": ("codebreaker.build.masm", "CACHE_VER"),
    "decompiled": ("codebreaker.decompile.decompiler", "generation"),
    "decompiled_fn": ("codebreaker.decompile.decompiler", "generation"),
    "gate16_tables": ("codebreaker.decompile.decompiler", "generation"),
    "compilable16": ("codebreaker.decompile.c_text", "generation"),
}
DEFAULT_MAX_GB = 2
# The kinds every state folder and checkout shares (settings.build_cache): results that are a pure
# function of the inputs their keys name, all of them by content.
SHARED = frozenset(("wasm16", "objects", "never_set", "masm", "assembled16", "omf_publics"))
DEFAULT_SHARED_MAX_GB = 6
CHECK_EVERY = 32 << 20          # bytes a process writes between looks at the cap
TOUCH = 3600                    # a result's time of use is renewed at most this often (seconds)
DROPPED = "_dropped"            # where what is going waits for its deletion, out of the way

_HITS = {}
_OPEN = {}                      # kind -> (process id, connection or False, generation)
_FOLDERS = {}                   # kind -> its generation folder, for path() kinds
_LOCK = threading.RLock()
_WRITTEN = [0, False]           # bytes written since the cap was looked at; looked at yet?


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


_BYTES = {}                     # (path, size, mtime) -> digest of the file's bytes


def tool_bytes(path):
    """What identifies a program by its bytes (for a key the shared build cache files under: a
    copy of the same tool elsewhere, or touched, is the same tool; a rebuilt one is not)."""
    try:
        st = os.stat(path)
    except OSError:
        return "%s:missing" % os.path.basename(path or "")
    k = (os.path.normcase(os.path.abspath(path)), st.st_size, st.st_mtime_ns)
    got = _BYTES.get(k)
    if got is None:
        h = hashlib.sha1()
        with open(path, "rb") as f:
            for block in iter(lambda: f.read(1 << 20), b""):
                h.update(block)
        got = _BYTES[k] = "%s:%s" % (os.path.basename(path).lower(), h.hexdigest())
    return got


def files_bytes(*folders):
    """The same for folders of files a tool reads (a compiler's headers): every file's name below
    the folder and its bytes."""
    parts = []
    for top in folders:
        for where, dirs, files in os.walk(top):
            dirs.sort()
            for f in sorted(files):
                p = os.path.join(where, f)
                parts.append(os.path.relpath(p, top).replace("\\", "/").lower() + "=" + tool_bytes(p))
    return key(parts)


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


def root(kind=None):
    """state/cache, where every kind is kept -- or, for a SHARED kind, the shared build cache."""
    from codebreaker.base import settings
    if kind in SHARED:
        return settings.build_cache()
    return settings.state("cache")


def roots():
    """[(folder, cap in bytes)]: state's caches, then the shared build cache."""
    from codebreaker.base import settings
    try:
        gb = float(settings.get("build_cache_max_gb") or DEFAULT_SHARED_MAX_GB)
    except (TypeError, ValueError):
        gb = DEFAULT_SHARED_MAX_GB
    return [(settings.state("cache"), max_bytes()), (settings.build_cache(), int(gb * (1 << 30)))]


def max_bytes():
    """The cap on all of state's caches together: the setting cache_max_gb (default 2)."""
    from codebreaker.base import settings
    try:
        gb = float(settings.get("cache_max_gb") or DEFAULT_MAX_GB)
    except (TypeError, ValueError):
        gb = DEFAULT_MAX_GB
    return int(gb * (1 << 30))


# --- generations ------------------------------------------------------------------------------
def generation(kind):
    """The name of the generation of `kind` the code of now makes ("g" and a short digest), or
    None when what names it could not be read (then nothing of the kind is dropped)."""
    where = GENERATIONS.get(kind)
    if where is None:
        return "g0"
    try:
        v = getattr(importlib.import_module(where[0]), where[1])
        v = v() if callable(v) else v
    except Exception:
        return None
    return "g" + key(repr(v))[:12]


def _drop_others(path, gen):
    """Every generation of a kind but `gen` out of its folder: moved aside at once (another
    process opening the kind finds them gone), then deleted."""
    try:
        names = os.listdir(path)
    except OSError:
        return
    for n in names:
        if n.split(".")[0] == gen:
            continue
        aside = os.path.join(root(), DROPPED, uuid.uuid4().hex)
        try:
            os.makedirs(os.path.dirname(aside), exist_ok=True)
            os.replace(os.path.join(path, n), aside)
        except OSError:
            continue                             # (in use by a process of other code: next time)
        _remove(aside)


def _remove(path):
    if os.path.isdir(path):
        shutil.rmtree(path, ignore_errors=True)
    else:
        try:
            os.remove(path)
        except OSError:
            pass


def _connect(path):
    db = sqlite3.connect(path, timeout=10.0, check_same_thread=False)
    db.execute("PRAGMA auto_vacuum=INCREMENTAL")         # (a new file only: rows deleted give the space back)
    db.execute("PRAGMA journal_mode=WAL")                # (workers read while one writes)
    db.execute("PRAGMA synchronous=NORMAL")
    db.execute("CREATE TABLE IF NOT EXISTS entry(k TEXT PRIMARY KEY, v BLOB, used INTEGER)")
    db.execute("CREATE INDEX IF NOT EXISTS entry_used ON entry(used)")
    db.commit()
    return db


def _db(kind):
    """The connection to `kind`'s file of this generation, opened once per process (older
    generations dropped as it is), or None when it cannot be used."""
    had = _OPEN.get(kind)
    if had is not None and had[0] == os.getpid():
        return had[1] or None
    with _LOCK:
        had = _OPEN.get(kind)
        if had is not None and had[0] == os.getpid():
            return had[1] or None
        db, gen = False, generation(kind)
        try:
            path = os.path.join(root(kind), kind)
            if gen is not None and kind not in SHARED:   # (a shared kind: another checkout's generation stays)
                _drop_others(path, gen)
            os.makedirs(path, exist_ok=True)
            db = _connect(os.path.join(path, (gen or "g0") + ".sqlite"))
        except (OSError, sqlite3.Error):
            db = False
        _OPEN[kind] = (os.getpid(), db, gen)
        return db or None


def _count(kind, found):
    hit, miss = _HITS.get(kind, (0, 0))
    _HITS[kind] = (hit + 1, miss) if found else (hit, miss + 1)


# --- results ----------------------------------------------------------------------------------
def get(kind, k):
    if off():
        return None
    db = _db(kind)
    row = None
    if db:
        with _LOCK:
            try:
                row = db.execute("SELECT v, used FROM entry WHERE k=?", (k,)).fetchone()
                now = int(time.time())
                if row and row[1] < now - TOUCH:         # (used again: the last to go under the cap)
                    db.execute("UPDATE entry SET used=? WHERE k=?", (now, k))
                    db.commit()
            except sqlite3.Error:
                pass
    _count(kind, row is not None)
    return bytes(row[0]) if row else None


def put(kind, k, data):
    if off():
        return
    db = _db(kind)
    if not db:
        return
    with _LOCK:
        try:
            db.execute("INSERT OR REPLACE INTO entry(k, v, used) VALUES(?,?,?)",
                       (k, sqlite3.Binary(data), int(time.time())))
            db.commit()                          # (one that loses the lock is made again next time)
        except sqlite3.Error:
            return
        _WRITTEN[0] += len(data) + len(k)
        if _WRITTEN[0] < CHECK_EVERY and _WRITTEN[1]:
            return
        _WRITTEN[0], _WRITTEN[1] = 0, True
    hold()


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


def path(kind, k):
    """Where the result filed under `k` is, for a tool that writes its own file (its folder is
    made when it is written). Such files count toward the cap too; call hold() after writing."""
    gen = _FOLDERS.get(kind)
    if gen is None:
        gen = generation(kind)
        if gen is not None and kind not in SHARED:
            _drop_others(os.path.join(root(kind), kind), gen)
        gen = _FOLDERS[kind] = gen or "g0"
    return os.path.join(root(kind), kind, gen, k[:2], k)


def counts():
    """{kind: (found, not found)} in this process, for a run's summary."""
    return dict(_HITS)


# --- the cap ----------------------------------------------------------------------------------
def _stores(base=None):
    """{kind: [(path, is a database)]} of everything kept on disk now, of any generation."""
    out = {}
    base = base or root()
    if not os.path.isdir(base):
        return out
    for kind in sorted(os.listdir(base)):
        if kind == DROPPED or not os.path.isdir(os.path.join(base, kind)):
            continue
        out[kind] = [(os.path.join(base, kind, n), n.endswith(".sqlite"))
                     for n in sorted(os.listdir(os.path.join(base, kind)))
                     if not n.endswith(("-wal", "-shm", "-journal"))]
    return out


def _size(path):
    if os.path.isdir(path):
        total = 0
        for d, _dirs, names in os.walk(path):
            for n in names:
                try:
                    total += os.path.getsize(os.path.join(d, n))
                except OSError:
                    pass
        return total
    total = 0
    for suffix in ("", "-wal", "-shm"):
        try:
            total += os.path.getsize(path + suffix)
        except OSError:
            pass
    return total


def sizes(base=None):
    """{kind: bytes on disk}, every generation of it, and what is waiting to be deleted."""
    base = base or root()
    out = {kind: sum(_size(p) for p, _db in where) for kind, where in _stores(base).items()}
    if os.path.isdir(os.path.join(base, DROPPED)):
        out[DROPPED] = _size(os.path.join(base, DROPPED))
    return out


def _connection(p):
    """A connection to the file `p`: this process's own when it has one open, else a new one."""
    for pid, db, gen in list(_OPEN.values()):
        if pid == os.getpid() and db and os.path.normcase(db.execute("PRAGMA database_list").fetchone()[2]) \
                == os.path.normcase(os.path.abspath(p)):
            return db, False
    return sqlite3.connect(p, timeout=10.0, check_same_thread=False), True


def _shrink(p):
    """Give the space of deleted rows back to the disk: the free pages cut off the file, the
    write-ahead log folded in and emptied."""
    try:
        db, mine = _connection(p)
        db.execute("PRAGMA incremental_vacuum").fetchall()      # (a page a step: every step)
        db.execute("PRAGMA wal_checkpoint(TRUNCATE)").fetchall()
        if mine:
            db.close()
    except sqlite3.Error:
        pass


def _oldest(stores, n=4000):
    """[(used, size, database path or None, row id or file)] of the results used longest ago."""
    olds = []
    for where in stores.values():
        for p, is_db in where:
            if is_db:
                try:
                    db, mine = _connection(p)
                    olds += [(used, size, p, rowid) for rowid, used, size in db.execute(
                        "SELECT rowid, used, length(k) + length(v) FROM entry ORDER BY used LIMIT ?", (n,))]
                    if mine:
                        db.close()
                except sqlite3.Error:
                    pass
            elif os.path.isdir(p):
                for d, _dirs, names in os.walk(p):
                    for name in names:
                        try:
                            st = os.stat(os.path.join(d, name))
                            olds.append((int(st.st_mtime), st.st_size, None, os.path.join(d, name)))
                        except OSError:
                            pass
    olds.sort(key=lambda o: o[0])
    return olds


def hold(cap=None):
    """Bring all of state's caches back under `cap` bytes (default the setting cache_max_gb), and
    the shared build cache under its own (build_cache_max_gb): past it, the results used longest
    ago go first, from any kind, until four fifths of the cap is used. -> bytes freed (as the
    results counted them)."""
    freed = 0
    for n, (base, its_cap) in enumerate(roots()):
        freed += _hold(base, its_cap if cap is None or n else cap)
    return freed


def _hold(base, cap):
    freed, shrunk = 0, False
    with _LOCK:
        for _round in range(16):
            try:
                total = sum(sizes(base).values())
            except OSError:
                return freed
            if total <= cap:
                return freed
            stores = _stores(base)
            if not shrunk:                       # (the logs folded in first: they may be most of it)
                for where in stores.values():
                    for p, is_db in where:
                        if is_db:
                            _shrink(p)
                shrunk = True
                continue
            need, got, gone = total - int(cap * 0.8), 0, {}
            for _used, size, p, what in _oldest(stores):
                if got >= need:
                    break
                gone.setdefault(p, []).append(what)
                got += size
            if not gone:
                return freed
            for p, whats in gone.items():
                if p is None:
                    for f in whats:
                        _remove(f)
                    continue
                try:
                    db, mine = _connection(p)
                    db.executemany("DELETE FROM entry WHERE rowid=?", [(r,) for r in whats])
                    db.commit()
                    if mine:
                        db.close()
                except sqlite3.Error:
                    continue
                _shrink(p)
            freed += got
        return freed


# --- seeing and emptying it -------------------------------------------------------------------
def prune():
    """Every kind back to its generation of now (the code is imported to say which) and the cap
    held. -> [(kind, what was dropped)]"""
    dropped = []
    for kind, where in _stores().items():
        gen = generation(kind)
        if gen is None:
            continue
        old = [p for p, _db in where if os.path.basename(p).split(".")[0] != gen]
        if old:
            _close(kind)
            _drop_others(os.path.join(root(), kind), gen)
            dropped.append((kind, [os.path.basename(p) for p in old]))
    _remove(os.path.join(root(), DROPPED))
    hold()
    return dropped


def _close(kind=None):
    for k in [kind] if kind else list(_OPEN):
        had = _OPEN.pop(k, None)
        if had and had[1]:
            try:
                had[1].close()
            except sqlite3.Error:
                pass
    if kind is None:
        _FOLDERS.clear()
    else:
        _FOLDERS.pop(kind, None)


def clear():
    """Everything kept, gone. -> bytes freed"""
    _close()
    total = sum(sizes().values())
    for kind in os.listdir(root()):
        _remove(os.path.join(root(), kind))
    return total


def _gb(n):
    return "%.2f GB" % (n / float(1 << 30)) if n >= 1 << 30 else "%.1f MB" % (n / float(1 << 20))


def stats():
    """[(kind, bytes, results, generations on disk, the generation of now)] -- state's kinds, then
    the shared build cache's (each named "shared:<kind>")."""
    out = []
    shared = root("objects")
    both = list(_stores().items()) + [("shared:" + k, w) for k, w in _stores(shared).items()]
    for kind, where in both:
        rows = 0
        for p, is_db in where:
            if is_db:
                try:
                    db, mine = _connection(p)
                    rows += db.execute("SELECT count(*) FROM entry").fetchone()[0]
                    if mine:
                        db.close()
                except sqlite3.Error:
                    pass
            else:
                rows += sum(len(names) for _d, _dirs, names in os.walk(p))
        out.append((kind, sum(_size(p) for p, _db in where), rows,
                    [os.path.basename(p).split(".")[0] for p, _db in where], generation(kind.split(":")[-1])))
    return out


def main(argv):
    """python -m codebreaker cache [--stats | --prune | --clear]"""
    if "--clear" in argv:
        print("cache cleared: %s freed" % _gb(clear()))
        return 0
    if "--prune" in argv:
        for kind, names in prune():
            print("  %-16s dropped old generation(s) %s" % (kind, ", ".join(names)))
    elif argv and argv[0] not in ("--stats",):
        print(__doc__.split("\n\n")[1])
        return 2
    total = 0
    print("%-16s %10s %10s  %s" % ("kind", "size", "results", "generations (* = of now)"))
    for kind, size, rows, gens, now in stats():
        total += size
        print("%-16s %10s %10d  %s" % (kind, _gb(size), rows,
                                       " ".join(g + ("*" if g == now else "") for g in gens)))
    print("%-16s %10s   (cap %s: setting cache_max_gb)" % ("all", _gb(total), _gb(max_bytes())))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
