"""facts.py -- the program database: one SQLite file per binary, every fact about it with where it came from.

    from codebreaker.engine import facts
    db = facts.open_for(path)                  # state/db/<sha>.sqlite, the binary's own
    row = db.row("function.c", "1:7cd4")       # a Row, or None
    db.write("function.c", "1:7cd4", value, step="function.c", version=v, inputs=[(kind, key, hash), ...])
    db.rows("function.gate")                   # every fact of a kind
    facts.hold()                               # every database back under the setting db_max_gb

A fact is (kind, key, value): `kind` says what it is ("function.object"), `key` which one within
the binary (a function's address "1:7cd4", or "" for one of the whole binary). With it is kept
how it was made: the step that made it, that step's version (a digest of its code), and the
hash of every fact it was made from. A value is any Python value; it is kept as

    bytes        as they are                    (tag B)
    text         UTF-8                          (tag S)
    JSON         what comes back from JSON the same as it went in: numbers, text, lists, dicts
                 with text keys                  (tag J)
    a pickle     anything else                  (tag P)

and its hash is the SHA-1 of the tag and those bytes: two facts are the same exactly when their
hashes are. A small value is kept in its row; a large one (past BLOB_AT bytes) once in the blob
table, by its hash, however many facts hold it.

The size of it all is looked after the way base/cache.py looks after the cache's:

  VERSIONS     a fact made by a version of a step that is not the version of now can never be
               asked for again: the first time a step is used on a database in a process, its
               facts of other versions go (engine.py calls drop_other_versions), and the blobs
               no fact holds go with them.

  THE CAP      all the databases together are held under the setting `db_max_gb` (default 4):
               past it, the databases opened longest ago go first, whole (a database is the
               binary's knowledge; deleting it costs the time to make it again and nothing
               else), never one open in this process.

Facts a person gives -- a function's C edited by hand -- are made by the step "given" and are
never dropped by either rule.
"""
import hashlib
import json
import os
import pickle
import shutil
import sqlite3
import threading
import time

BLOB_AT = 4096                  # values larger than this are kept once, in the blob table
DEFAULT_MAX_GB = 4
GIVEN = "given"                 # the step of a fact a person (or the engine's caller) set

_SCHEMA = """
CREATE TABLE IF NOT EXISTS fact(
    kind TEXT NOT NULL, key TEXT NOT NULL, hash TEXT NOT NULL, tag TEXT NOT NULL,
    value BLOB, blob TEXT, size INTEGER, step TEXT NOT NULL, version TEXT, inputs TEXT,
    made REAL, took REAL,
    PRIMARY KEY (kind, key));
CREATE INDEX IF NOT EXISTS fact_step ON fact(step, version);
CREATE TABLE IF NOT EXISTS blob(hash TEXT PRIMARY KEY, data BLOB);
CREATE TABLE IF NOT EXISTS about(name TEXT PRIMARY KEY, value TEXT);
"""

_OPEN = {}                      # path -> Database, of this process
_LOCK = threading.RLock()


def encode(value):
    """-> (tag, bytes) of a value as it is kept."""
    if isinstance(value, (bytes, bytearray)):
        return "B", bytes(value)
    if isinstance(value, str):
        return "S", value.encode("utf-8", "surrogatepass")
    try:
        text = json.dumps(value, sort_keys=True, separators=(",", ":"))
        if json.loads(text) == value:
            return "J", text.encode("utf-8")
    except (TypeError, ValueError):
        pass
    return "P", pickle.dumps(value, protocol=4)


def decode(tag, data):
    data = bytes(data)
    if tag == "B":
        return data
    if tag == "S":
        return data.decode("utf-8", "surrogatepass")
    if tag == "J":
        return json.loads(data.decode("utf-8"))
    return pickle.loads(data)


def digest(tag, data):
    h = hashlib.sha1(tag.encode("ascii"))
    h.update(data)
    return h.hexdigest()


class Row(object):
    """One fact as it is kept: its value is read only when asked for."""
    __slots__ = ("db", "kind", "key", "hash", "tag", "size", "step", "version", "inputs", "made", "took",
                 "_value", "_blob")

    def __init__(self, db, kind, key, hash, tag, value, blob, size, step, version, inputs, made, took):
        self.db, self.kind, self.key, self.hash, self.tag = db, kind, key, hash, tag
        self._value, self._blob, self.size = value, blob, size
        self.step, self.version, self.made, self.took = step, version, made, took
        self.inputs = [tuple(i) for i in json.loads(inputs)] if inputs else []

    def data(self):
        """The value's bytes, as kept."""
        if self._value is not None:
            return bytes(self._value)
        return self.db.blob(self._blob)

    def value(self):
        return decode(self.tag, self.data())

    @property
    def given(self):
        return self.step == GIVEN


class Database(object):
    """The facts of one binary (see the module's text)."""

    def __init__(self, path):
        self.path = path
        self.lock = threading.RLock()
        os.makedirs(os.path.dirname(path), exist_ok=True)
        self.conn = sqlite3.connect(path, timeout=30.0, check_same_thread=False)
        self.conn.execute("PRAGMA auto_vacuum=INCREMENTAL")
        self.conn.execute("PRAGMA journal_mode=WAL")
        self.conn.execute("PRAGMA synchronous=NORMAL")
        self.conn.executescript(_SCHEMA)
        self.conn.commit()
        self.cleaned = set()                 # steps whose other versions' facts are gone, this process
        try:                                 # (when it was last opened: the cap goes by it)
            os.utime(path, None)
        except OSError:
            pass

    # --- reading ---------------------------------------------------------------------------
    def row(self, kind, key=""):
        with self.lock:
            r = self.conn.execute("SELECT kind, key, hash, tag, value, blob, size, step, version, inputs, made, took "
                                  "FROM fact WHERE kind=? AND key=?", (kind, key)).fetchone()
        return Row(self, *r) if r else None

    def rows(self, kind=None):
        """Every fact (of one kind), in key order."""
        q = "SELECT kind, key, hash, tag, value, blob, size, step, version, inputs, made, took FROM fact"
        with self.lock:
            got = self.conn.execute(q + (" WHERE kind=? ORDER BY key" if kind else " ORDER BY kind, key"),
                                    (kind,) if kind else ()).fetchall()
        return [Row(self, *r) for r in got]

    def kinds(self):
        """[(kind, facts, bytes, steps)]"""
        with self.lock:
            return self.conn.execute("SELECT kind, COUNT(*), SUM(size), GROUP_CONCAT(DISTINCT step) "
                                     "FROM fact GROUP BY kind ORDER BY kind").fetchall()

    def blob(self, h):
        with self.lock:
            r = self.conn.execute("SELECT data FROM blob WHERE hash=?", (h,)).fetchone()
        if r is None:
            raise KeyError("blob %s is missing from %s" % (h, self.path))
        return bytes(r[0])

    def about(self, name, value=None):
        """A note about the database itself (the binary's path, its name): read, or with
        `value` written."""
        with self.lock:
            if value is None:
                r = self.conn.execute("SELECT value FROM about WHERE name=?", (name,)).fetchone()
                return r[0] if r else None
            self.conn.execute("INSERT OR REPLACE INTO about(name, value) VALUES(?,?)", (name, value))
            self.conn.commit()
            return value

    # --- writing ---------------------------------------------------------------------------
    def write(self, kind, key, value, step, version="", inputs=(), took=0.0, encoded=None):
        """Keep a fact (in place of any before it). -> its hash"""
        tag, data = encoded or encode(value)
        h = digest(tag, data)
        inline, blob = (data, None) if len(data) <= BLOB_AT else (None, h)
        with self.lock:
            if blob:
                self.conn.execute("INSERT OR IGNORE INTO blob(hash, data) VALUES(?,?)", (h, sqlite3.Binary(data)))
            self.conn.execute("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, "
                              "inputs, made, took) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)",
                              (kind, key, h, tag, sqlite3.Binary(inline) if inline is not None else None, blob,
                               len(data), step, version, json.dumps([list(i) for i in inputs]), time.time(), took))
            self.conn.commit()
        return h

    def remake(self, kind, key, version, inputs):
        """A fact found the same as it was when made again: only how it was made is new."""
        with self.lock:
            self.conn.execute("UPDATE fact SET version=?, inputs=?, made=? WHERE kind=? AND key=?",
                              (version, json.dumps([list(i) for i in inputs]), time.time(), kind, key))
            self.conn.commit()

    def forget(self, kind, key=None):
        """A fact (or every fact of a kind) gone. -> how many"""
        with self.lock:
            if key is None:
                n = self.conn.execute("DELETE FROM fact WHERE kind=?", (kind,)).rowcount
            else:
                n = self.conn.execute("DELETE FROM fact WHERE kind=? AND key=?", (kind, key)).rowcount
            self.conn.commit()
        return n

    def drop_other_versions(self, step, version):
        """The facts `step` made in other versions of its code, gone (once per process). -> how many"""
        if step in self.cleaned or step == GIVEN:
            return 0
        with self.lock:
            n = self.conn.execute("DELETE FROM fact WHERE step=? AND version<>?", (step, version)).rowcount
            self.conn.commit()
            self.cleaned.add(step)
        if n:
            self.collect()
        return n

    def collect(self):
        """The blobs no fact holds, gone, and their space given back."""
        with self.lock:
            self.conn.execute("DELETE FROM blob WHERE hash NOT IN (SELECT blob FROM fact WHERE blob IS NOT NULL)")
            self.conn.commit()
            try:
                self.conn.execute("PRAGMA incremental_vacuum").fetchall()
                self.conn.execute("PRAGMA wal_checkpoint(TRUNCATE)").fetchall()
            except sqlite3.Error:
                pass

    def close(self):
        with _LOCK:
            _OPEN.pop(self.path, None)
        with self.lock:
            try:
                self.conn.close()
            except sqlite3.Error:
                pass


# --- where the databases are ------------------------------------------------------------------
def root():
    """state/db, where every binary's database is."""
    from codebreaker.base import settings
    return settings.state("db")


def sha(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def open_at(path):
    """The database in the file `path` (one connection per process)."""
    path = os.path.abspath(path)
    with _LOCK:
        db = _OPEN.get(path)
        if db is None:
            db = _OPEN[path] = Database(path)
        return db


def open_for(binary, folder=None):
    """The database of the binary at `binary` (its SHA-256 names it): state/db/<sha>.sqlite."""
    s = sha(binary)
    db = open_at(os.path.join(folder or root(), s + ".sqlite"))
    db.sha = s
    if db.about("name") is None:
        db.about("name", os.path.basename(binary))
    db.about("path", os.path.abspath(binary))
    return db


def work(db, *parts):
    """A folder for the files a step writes as it works (a build's objects): beside the database,
    <sha>.work/. Not facts: deleting it costs nothing but the next run's time."""
    p = os.path.join(os.path.splitext(db.path)[0] + ".work", *parts)
    os.makedirs(p, exist_ok=True)
    return p


def max_bytes():
    from codebreaker.base import settings
    try:
        gb = float(settings.get("db_max_gb") or DEFAULT_MAX_GB)
    except (TypeError, ValueError):
        gb = DEFAULT_MAX_GB
    return int(gb * (1 << 30))


def _size(p):
    total = 0
    for suffix in ("", "-wal", "-shm"):
        try:
            total += os.path.getsize(p + suffix)
        except OSError:
            pass
    return total


def databases(folder=None):
    """[(path, bytes, last opened)] of every database, the one opened longest ago first. (Not the
    knowledge base beside them, knowledge.sqlite: it holds what was learned and given by hand,
    which nothing would make again; the cap never deletes it.)"""
    folder = folder or root()
    out = []
    for n in os.listdir(folder):
        if n.endswith(".sqlite") and n != "knowledge.sqlite":
            p = os.path.join(folder, n)
            try:
                out.append((p, _size(p), os.path.getmtime(p)))
            except OSError:
                pass
    return sorted(out, key=lambda d: d[2])


def hold(cap=None, folder=None):
    """All the databases back under `cap` bytes (default the setting db_max_gb): past it, those
    opened longest ago go, whole, with their work folders, until four fifths of the cap is used;
    never one open in this process. -> [path gone]"""
    cap = max_bytes() if cap is None else cap
    dbs = databases(folder)
    total = sum(s for _p, s, _t in dbs)
    gone = []
    if total <= cap:
        return gone
    with _LOCK:
        mine = {os.path.normcase(p) for p in _OPEN}
    for p, size, _t in dbs:
        if total <= cap * 0.8:
            break
        if os.path.normcase(os.path.abspath(p)) in mine:
            continue
        for suffix in ("", "-wal", "-shm"):
            try:
                os.remove(p + suffix)
            except OSError:
                pass
        shutil.rmtree(os.path.splitext(p)[0] + ".work", ignore_errors=True)
        total -= size
        gone.append(p)
    return gone
