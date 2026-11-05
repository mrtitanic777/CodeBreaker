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
               else), never one open in this process. A database that holds the function
               store's tables of its binary (its verdicts, store/functions.py) is not deleted:
               its facts go, and the store's rows stay.

Facts a person gives -- a function's C edited by hand -- are made by the step "given" and are
never dropped by either rule.

The fact store is C++ (native/engine: cbengine.dll, through engine/native_engine.py), on SQLite's C
API: Database below is its. The Python store it replaced is gone (the milestone of 2026-11-04);
what stays here is what the C++ calls back or callers read: a value's encoding, Row, where the
databases are.
"""
import json
import os
import pickle
import threading

BLOB_AT = 4096                  # values larger than this are kept once, in the blob table
DEFAULT_MAX_GB = 4
GIVEN = "given"                 # the step of a fact a person (or the engine's caller) set

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


def Database(path):
    """The facts of one binary (see the module's text): the C++ store's."""
    from codebreaker.engine import native_engine
    return native_engine.Database(path)


# --- where the databases are ------------------------------------------------------------------
def root():
    """state/db, where every binary's database is."""
    from codebreaker.base import settings
    return settings.state("db")


def sha(path):
    """The binary's SHA-256 (kept by path, size and time, in the C++)."""
    from codebreaker.engine import native_engine
    return native_engine.sha(path)


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


def databases(folder=None):
    """[(path, bytes, last opened)] of every database, the one opened longest ago first. (Not the
    knowledge base beside them, knowledge.sqlite: it holds what was learned and given by hand,
    which nothing would make again; the cap never deletes it.)"""
    from codebreaker.engine import native_engine
    return native_engine.databases(folder or root())


def hold(cap=None, folder=None):
    """All the databases back under `cap` bytes (default the setting db_max_gb): past it, those
    opened longest ago go, whole, with their work folders, until four fifths of the cap is used;
    never one open in this process (and of one holding the function store's verdicts only its
    facts). -> [path gone or emptied of facts]"""
    from codebreaker.engine import native_engine
    return native_engine.hold(max_bytes() if cap is None else cap, folder or root())
