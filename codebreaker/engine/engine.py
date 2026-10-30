"""engine.py -- the incremental engine: a fact is made once, and made again only when what it was made from changed.

    from codebreaker.engine import engine
    db = engine.open(path)                          # the binary's database; its bytes are the fact `binary`
    image = engine.get(db, "ne.image")              # the fact, current: kept, or made again
    texts = engine.get_many(db, "function.c", keys) # {key: value}, the stale ones made side by side
    engine.put(db, "function.c", "1:7cd4", text)    # a fact given by hand (an edit): kept until forgotten
    engine.forget(db, "function.c", "1:7cd4")       # ... and made by its step again
    engine.last_run(db)                             # {kind: [kept, made]} of the last get

A step makes one kind of fact. It is a function, declared with what it reads and makes:

    @engine.step("function.object", reads=("function.source", "ne.header"), platform="win16",
                 uses=(functions,), stamp=lambda: cache.tool(P.compiler.tool()))
    def function_object(ctx, key):
        src = ctx.need("function.source", key)      # another fact: made first if it is stale
        ...
        return value                                # anything engine/facts.py can keep

Its VERSION is a digest of its code -- its own source, and that of each module and helper
function named in `uses` (a helper not named is a change the engine cannot see: name every one)
-- and of what `stamp()` gives (the tools it runs, the settings it reads). The
facts it reads are recorded as it reads them (ctx.need), each with the hash it had: what a fact
depends on is what its step actually asked for, not a list kept by hand. `reads` is the promise
(asking for a kind not in it is an error) and what `facts` shows.

When a fact is asked for it is CURRENT if its step's version is the version of now and every
fact it was made from is current and has the hash it had then; that is decided once per run, and
recursively, so what is stale is found from the bottom up. A current fact is returned as kept.
A stale one is made again -- and when it comes out the same as before (the same hash) nothing
made from it is stale: a change stops where it stops making a difference (a function's C
changed by a comment compiles to the same object, and nothing past the object is made again).

Side by side: get_many() finds which of the facts asked for are stale, then makes those at once
on threads (jobs/pool.threads) -- a step's work is mostly a program it waits on, a compiler or an
emulator in processes of its own. How many at once is the machine's processors, held down by the
setting `low_load` (a number, or true for the machine's budget, base/parallel.workers()). A step
declared `batch=True` is given all its stale keys in one call instead, as fn({key: ctx}, hint)
-> {key: value} (the gate shares them out over its own processes).

Some facts are not made from facts: a step declared `always=True` is run once per run (a file's
hash: steps.file) and its value compared, so what depends on a file is stale when the file is.
A step may return `Unkept(value)`: the value is used this run and not kept (a verdict that could
not be reached is not a verdict).

What a step reads of the knowledge base (knowledge/: a prototype, the bytes of arguments an
import removes, a body's name, the library signatures) is recorded as it runs, wherever in the
code it is asked (knowledge/reads.py), and each piece is an input of the fact like any other:
("kb", "platform:kind:key", the piece's hash). A changed piece makes stale exactly the facts that
read it; a fact that read none is untouched by any change to the base.

The facts of a step of another version are dropped from a database the first time the step is
used on it in a process (facts.py: versions, and the cap on all databases).
"""
import hashlib
import io
import inspect
import os
import threading
import time

from codebreaker.engine import facts
from codebreaker.knowledge import reads

STEPS = {}                      # kind -> Step
def _step_modules():
    """{platform -> the module whose steps it has} (imported when asked): each platform's
    descriptor says (codebreaker/platforms: `steps`), filed under the descriptor's name (win16)."""
    from codebreaker import platforms
    return platforms.step_modules()


class Unkept(object):
    """A step's value used this run and not kept."""

    def __init__(self, value):
        self.value = value


class Step(object):
    def __init__(self, kind, fn, reads, platform, uses, stamp, batch, always, version):
        self.kind, self.fn, self.reads, self.platform = kind, fn, tuple(reads), platform
        self.uses, self.stamp, self.batch, self.always = tuple(uses), stamp, batch, always
        self.fixed_version = version
        self.doc = (inspect.getdoc(fn) or "").split("\n\n")[0]
        self.code = None                     # the digest of the code, worked out once per process

    def version(self):
        """A digest of the code that makes this step's facts: the step's own source, each module
        (its file) and function (its source) in `uses` -- read once, the first time it is asked
        in a process: the code that runs is the code loaded then, however long the process lives
        (the service) -- and what stamp() gives, now."""
        if self.fixed_version is not None:
            return str(self.fixed_version)
        if self.code is None:
            h = hashlib.sha1(self.kind.encode())
            for code in (self.fn,) + self.uses:
                if inspect.ismodule(code):
                    path = getattr(code, "__file__", None)
                    if path:
                        with io.open(path, "rb") as f:
                            h.update(f.read())
                else:
                    h.update(inspect.getsource(code).encode("utf-8"))
            self.code = h
        h = self.code.copy()
        if self.stamp:
            h.update(inspect.getsource(self.stamp).encode("utf-8"))
            h.update(repr(self.stamp()).encode("utf-8", "replace"))
        return h.hexdigest()[:16]


def step(kind, reads=(), platform="", uses=(), stamp=None, batch=False, always=False, version=None):
    """Declare the function below as the step that makes facts of `kind` (see the module's text).
    `version` fixes the version instead of the code's digest (tests)."""
    def declare(fn):
        STEPS[kind] = Step(kind, fn, reads, platform, uses, stamp, batch, always, version)
        return fn
    return declare


def steps(platform=None):
    """{kind: Step} of a platform's steps (its module imported), or of every step known."""
    if platform:
        load(platform)
        return {k: s for k, s in STEPS.items() if s.platform in (platform, "")}
    for p in _step_modules():
        load(p)
    return dict(STEPS)


def load(platform):
    import importlib
    from codebreaker import platforms
    p = platforms.named(platform)
    if p is not None and p.steps:
        importlib.import_module(p.steps)


def jobs():
    """How many facts are made at once: the machine's processors, or fewer by the setting
    low_load (a number; or true: the machine's budget, base/parallel.workers())."""
    from codebreaker.base import parallel, settings
    n = os.environ.get("CB_LOW_LOAD") or settings.get("low_load")
    if n in (None, "", False, 0, "0", "false", "False"):
        return max(1, os.cpu_count() or 4)
    if n is True or str(n).lower() in ("true", "yes", "on"):
        return parallel.workers()
    try:
        return max(1, int(n))
    except (TypeError, ValueError):
        return parallel.workers()


# --- the facts every platform has -------------------------------------------------------------
@step("binary", platform="")
def _binary(ctx, key):
    """The binary itself: its path (the hash is of its bytes). Given when the database is opened."""
    raise LookupError("the binary is given when its database is opened (engine.open)")


@step("file", platform="", always=True)
def _file(ctx, key):
    """A file a step reads that is not a fact (a list kept in the source tree): its SHA-1."""
    try:
        with io.open(key, "rb") as f:
            return hashlib.sha1(f.read()).hexdigest()
    except OSError:
        return "missing"


# --- a run ------------------------------------------------------------------------------------
class Run(object):
    """One asking of the engine: each fact decided current or made again at most once in it."""

    def __init__(self, db, log=None):
        self.db, self.log = db, log
        self.done = {}                       # (kind, key) -> (hash, value or _UNREAD)
        self.locks = {}
        self.lock = threading.RLock()
        self.versions = {}
        self.counts = {}                     # kind -> [kept, made]

    def version(self, kind):
        with self.lock:
            v = self.versions.get(kind)
            if v is None:
                s = _step_of(kind)
                v = self.versions[kind] = s.version()
                self.db.drop_other_versions(kind, v)
        return v

    def _lock_for(self, kind, key):
        with self.lock:
            return self.locks.setdefault((kind, key), threading.RLock())

    def _count(self, kind, made):
        with self.lock:
            c = self.counts.setdefault(kind, [0, 0])
            c[1 if made else 0] += 1

    def hash_of(self, kind, key):
        """The fact's hash, once it is current (made again first if it is stale)."""
        return self._current(kind, key)[0]

    def value(self, kind, key):
        h, v = self._current(kind, key)
        if v is _UNREAD:
            with self._lock_for(kind, key):
                h, v = self.done[(kind, key)]
                if v is _UNREAD:
                    v = self.db.row(kind, key).value()
                    self.done[(kind, key)] = (h, v)
        return v

    def _current(self, kind, key):
        got = self.done.get((kind, key))
        if got is not None:
            return got
        if kind == reads.KIND:                   # a piece of the knowledge base: its hash now, once a run
            h = reads.digest(key)
            with self.lock:
                got = self.done.setdefault((kind, key), (h, h))
            return got
        with self._lock_for(kind, key):
            got = self.done.get((kind, key))
            if got is not None:
                return got
            row = self.db.row(kind, key)
            if row is not None and self._still(row):
                self.done[(kind, key)] = got = (row.hash, _UNREAD)
                self._count(kind, False)
                return got
            return self._make(kind, [key], {key: row})[key]

    def _still(self, row):
        """Is a kept fact current? (its step's version, and each input's hash as it was)"""
        if row.given:
            return True
        s = _step_of(row.kind)
        if s.always or row.version != self.version(row.kind):
            return False
        for kind, key, h in row.inputs:
            try:
                if self.hash_of(kind, key) != h:
                    return False
            except LookupError:
                return False
        return True

    def stale(self, kind, keys):
        """The keys of `kind` whose facts are not current (each decided once: current ones are
        then known current for the run)."""
        out = []
        for key in keys:
            if (kind, key) in self.done:
                continue
            with self._lock_for(kind, key):
                if (kind, key) in self.done:
                    continue
                row = self.db.row(kind, key)
                if row is not None and self._still(row):
                    self.done[(kind, key)] = (row.hash, _UNREAD)
                    self._count(kind, False)
                else:
                    out.append(key)
        return out

    def _make(self, kind, keys, rows=None, hint=None):
        """Make the facts of `kind` at `keys`. -> {key: (hash, value)}"""
        s = _step_of(kind)
        version = self.version(kind)
        rows = rows or {}
        ctxs = {k: Context(self, s, k, hint) for k in keys}
        t = time.time()
        if s.batch:
            with reads.recording() as r:         # (what the knowledge base was asked: inputs too)
                values = s.fn(ctxs, hint)
            for c in ctxs.values():
                c.knowledge(r)
            missing = [k for k in keys if k not in values]
            if missing:
                raise LookupError("step %s made no %s for %s" % (kind, kind, ", ".join(missing[:4])))
        else:
            values = {}
            for k in keys:
                with reads.recording() as r:
                    values[k] = s.fn(ctxs[k], k)
                ctxs[k].knowledge(r)
        took = (time.time() - t) / max(1, len(keys))
        out = {}
        for k in keys:
            v = values[k]
            if isinstance(v, Unkept):
                self.db.forget(kind, k)                   # (what was kept before is no longer it)
                out[k] = (None, v.value)
                self.done[(kind, k)] = out[k]
                self._count(kind, True)
                continue
            enc = facts.encode(v)
            h = facts.digest(*enc)
            row = rows.get(k, _NO) if k in rows else self.db.row(kind, k)
            if row is not None and row is not _NO and row.hash == h and not row.given:
                self.db.remake(kind, k, version, ctxs[k].inputs)      # (the same as before: what
            else:                                                     # was made from it holds)
                self.db.write(kind, k, v, step=kind, version=version, inputs=ctxs[k].inputs,
                              took=took, encoded=enc)
            out[k] = self.done[(kind, k)] = (h, v)
            self._count(kind, True)
        if self.log and keys and (len(keys) > 1 or time.time() - t > 1.0):
            self.log("  made %d %s (%.1fs)" % (len(keys), kind, time.time() - t))
        return out

    def many(self, kind, keys, hint=None):
        """{key: value} for every key, the stale ones made side by side."""
        keys = list(dict.fromkeys(keys))
        todo = self.stale(kind, keys)
        if todo:
            s = _step_of(kind)
            locks = [self._lock_for(kind, k) for k in todo]
            if s.batch:
                for lk in locks:
                    lk.acquire()
                try:
                    todo = [k for k in todo if (kind, k) not in self.done]
                    if todo:
                        self._make(kind, todo, hint=hint)
                finally:
                    for lk in locks:
                        lk.release()
            else:
                from codebreaker.jobs import pool

                def one(k):
                    with self._lock_for(kind, k):
                        if (kind, k) not in self.done:
                            self._make(kind, [k], hint=hint)
                n = min(jobs(), len(todo))
                if n <= 1:
                    for k in todo:
                        one(k)
                else:
                    t = time.time()
                    log, self.log = self.log, None           # (one line for the lot, not one each)
                    try:
                        pool.threads(one, todo, jobs=n)
                    finally:
                        self.log = log
                    if self.log:
                        self.log("  made %d %s, %d at once (%.1fs)" % (len(todo), kind, n, time.time() - t))
        return {k: self.value(kind, k) for k in keys}


_UNREAD = object()
_NO = object()


class Context(object):
    """What a step is given: the facts it asks for, recorded as it asks."""

    def __init__(self, run, s, key, hint=None):
        self.run, self.step, self.key, self.hint = run, s, key, hint
        self.inputs = []
        self.db = run.db

    def _may(self, kind):
        if kind not in self.step.reads and kind not in ("binary", "file"):
            raise LookupError("step %s reads %s, which it does not declare (reads=%r)"
                              % (self.step.kind, kind, self.step.reads))

    def need(self, kind, key=""):
        """The fact's value (made first if stale); this fact is made from it."""
        self._may(kind)
        v = self.run.value(kind, key)
        self._record(kind, key)
        return v

    def need_many(self, kind, keys, hint=None):
        """{key: value} -- made side by side where stale; `hint` is handed to the step as how to
        make them (a build to run them in), not as something they are made from."""
        self._may(kind)
        got = self.run.many(kind, keys, hint=hint)
        for k in got:
            self._record(kind, k)
        return got

    def peek(self, kind, key=""):
        """The fact's value when it is current, else None -- nothing is made. A value seen is
        one this fact is made from."""
        self._may(kind)
        if not self.run.stale(kind, [key]) and self.run.done[(kind, key)][0] is not None:
            v = self.run.value(kind, key)
            self._record(kind, key)
            return v
        return None

    def file(self, path):
        """A file this fact is made from (its hash, through the step `file`)."""
        return self.need("file", os.path.abspath(path))

    def knowledge(self, keys):
        """Pieces of the knowledge base this fact is made from (knowledge/reads.py: "platform:kind:key"),
        each with its hash now. The engine adds what the step read as it ran; a step adds what
        its worker processes read (reads.Carry) for the one key it was for."""
        have = {(k, key) for k, key, _h in self.inputs}
        for read in keys:
            if (reads.KIND, read) not in have:
                have.add((reads.KIND, read))
                # (read now, not as the run first saw it: a piece the step itself learned -- the
                # bytes an export removes, read from its code -- is recorded as it now is)
                h = reads.digest(read)
                with self.run.lock:
                    self.run.done[(reads.KIND, read)] = (h, h)
                self.inputs.append((reads.KIND, read, h))

    def previous(self):
        """This fact's value as last kept (made by any version), or None. Not an input: a hint
        of where to start (the functions the gate ran last time)."""
        row = self.db.row(self.step.kind, self.key)
        try:
            return row.value() if row is not None else None
        except Exception:
            return None

    def work(self, *parts):
        """A folder for files this step writes as it works (not facts)."""
        return facts.work(self.db, *parts)

    def _record(self, kind, key):
        h = self.run.done[(kind, key)][0]
        if h is None:                          # (an unkept value: this fact cannot hold beyond the run)
            h = "unkept"
        self.inputs.append((kind, key, h))


def _step_of(kind):
    s = STEPS.get(kind)
    if s is None:
        for p in _step_modules():
            load(p)
        s = STEPS.get(kind)
    if s is None:
        raise LookupError("no step makes facts of the kind %r" % kind)
    return s


# --- asking -----------------------------------------------------------------------------------
_RUNS = {}


def open(path, folder=None):
    """The database of the binary at `path`, with the binary given as the fact `binary`."""
    db = facts.open_for(path, folder)
    row = db.row("binary", "")
    value = {"path": os.path.abspath(path), "size": os.path.getsize(path)}
    if row is None or row.hash != _binary_hash(db) or row.value() != value:
        _give_binary(db, value)
    return db


def _binary_hash(db):
    return "sha256:" + db.sha


def _give_binary(db, value):
    """The binary's fact: its value where it is, its hash its bytes' (moving it changes nothing)."""
    tag, data = facts.encode(value)
    with db.lock:
        db.conn.execute("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, inputs, "
                        "made, took) VALUES('binary','',?,?,?,NULL,?,?,'','[]',?,0)",
                        (_binary_hash(db), tag, data, len(data), facts.GIVEN, time.time()))
        db.conn.commit()


def get(db, kind, key="", log=None):
    """The fact, current (see the module's text)."""
    r = Run(db, log)
    _RUNS[db.path] = r
    return r.value(kind, key)


def get_many(db, kind, keys, log=None):
    r = Run(db, log)
    _RUNS[db.path] = r
    return r.many(kind, keys)


def last_run(db):
    """{kind: [kept, made]} of the last get on this database in this process."""
    r = _RUNS.get(db.path)
    return {k: list(v) for k, v in r.counts.items()} if r else {}


def put(db, kind, key, value):
    """A fact given by hand: what is made from it is stale when it differs from what was there."""
    return db.write(kind, key, value, step=facts.GIVEN)


def forget(db, kind, key=None):
    """Facts gone (one, or a kind): the next get makes them again."""
    return db.forget(kind, key)


def why(db, kind, key=""):
    """How a fact was made: {step, version, current version, made, took, inputs: [(kind, key, hash,
    same now?)]} -- read only; nothing is made."""
    row = db.row(kind, key)
    if row is None:
        return None
    out = {"kind": kind, "key": key, "hash": row.hash, "size": row.size, "step": row.step,
           "version": row.version, "made": row.made, "took": row.took, "inputs": []}
    try:
        out["version_now"] = _step_of(kind).version() if not row.given else row.version
    except LookupError:
        out["version_now"] = None
    for k, key2, h in row.inputs:
        if k == reads.KIND:
            out["inputs"].append((k, key2, h, reads.digest(key2) == h))
            continue
        now = db.row(k, key2)
        out["inputs"].append((k, key2, h, bool(now and now.hash == h)))
    return out
