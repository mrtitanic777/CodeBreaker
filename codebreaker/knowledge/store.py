"""store.py -- the knowledge base's database: one SQLite file, every platform's knowledge keyed by platform.

    from codebreaker.knowledge import store
    kb = store.open()                                 # state/db/knowledge.sqlite (CB_KNOWLEDGE_DB)
    kb.get("win32", "prototype", "GetDlgItem")        # a value, or None
    kb.put("win32", "argument_bytes.code", "USER32.FindWindowA", 8, source="user32.dll's code")
    kb.table("win16", "prototype")                    # {key: value} of a kind, read once per process
    kb.ensure("win32", "prototype", stamp, make, source)   # a whole kind made again when its stamp changes

A piece of knowledge is (platform, kind, key, value), kept the way the program database keeps a
fact (engine/facts.py: a value is bytes, text, JSON or a pickle, with the SHA-1 of it), and with
where it came from:

    source     what it was read or worked out from: a file of the repository, the headers, a
               module's code, a person
    origin     authored   loaded from a file people write (ordinal_names.json, driver_services.json,
                          ...): the file is the knowledge, the rows its copy, made again when the
                          file changes
               derived    worked out from inputs outside the repository (the compiler's headers):
                          made again when their stamp changes
               learned    found once by a step and kept (the bytes of arguments an export removes,
                          read from its code): written once, seen by every later ask
               given      given by hand (a body named); never dropped

A whole kind that is authored or derived is made in one go by ensure(): the stamp it was made
under (the file's hash; the header folders' sizes and times) is kept in `loaded`, and when it
differs the kind's authored/derived rows are made again -- learned and given rows stay.

Bodies (the name store) are kept here too: `body` -- a function's body digest in a module, with
the name the module itself gives it -- and `body_name` -- a name given to a body, which holds in
every module with that body (symbols/names.py says how a body is digested).

The file lives beside the program databases (state/db/) but is not one of them: the size rule
that deletes those (facts.hold) leaves it alone -- what was learned and given is not made again.
"""
import os
import sqlite3
import threading
import time

from codebreaker.engine.facts import decode, digest, encode

AUTHORED, DERIVED, LEARNED, GIVEN = "authored", "derived", "learned", "given"

_SCHEMA = """
CREATE TABLE IF NOT EXISTS knowledge(
    platform TEXT NOT NULL, kind TEXT NOT NULL, key TEXT NOT NULL, seq INTEGER,
    hash TEXT NOT NULL, tag TEXT NOT NULL, value BLOB, source TEXT, origin TEXT NOT NULL, made REAL,
    PRIMARY KEY (platform, kind, key));
CREATE TABLE IF NOT EXISTS loaded(
    platform TEXT NOT NULL, kind TEXT NOT NULL, stamp TEXT, source TEXT, made REAL,
    PRIMARY KEY (platform, kind));
CREATE TABLE IF NOT EXISTS module(sha TEXT PRIMARY KEY, name TEXT, platform TEXT);
CREATE TABLE IF NOT EXISTS body(
    module TEXT NOT NULL, addr TEXT NOT NULL, hash TEXT NOT NULL, name TEXT, source TEXT,
    PRIMARY KEY (module, addr));
CREATE INDEX IF NOT EXISTS body_hash ON body(hash);
CREATE TABLE IF NOT EXISTS body_name(hash TEXT PRIMARY KEY, name TEXT, source TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS carried(what TEXT PRIMARY KEY, mark TEXT);
"""

_OPEN = {}                      # path -> KnowledgeBase, of this process
_LOCK = threading.RLock()


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_KNOWLEDGE_DB") or settings.state("db", "knowledge.sqlite")


def beside(store_path):
    """The knowledge base of the state folder a function store is in (a test's own store in a
    folder of its own has a knowledge base of its own)."""
    return os.path.join(os.path.dirname(os.path.abspath(store_path)), "db", "knowledge.sqlite")


class KnowledgeBase(object):
    def __init__(self, path):
        self.path = path
        self.lock = threading.RLock()
        os.makedirs(os.path.dirname(path), exist_ok=True)
        self.conn = sqlite3.connect(path, timeout=60.0, check_same_thread=False)
        self.conn.execute("PRAGMA journal_mode=WAL")
        self.conn.execute("PRAGMA synchronous=NORMAL")
        self.conn.executescript(_SCHEMA)
        self.conn.commit()
        self._tables = {}                    # (platform, kind) -> {key: value}, read once
        self._ensured = {}                   # (platform, kind) -> stamp checked this process

    def close(self):
        with self.lock:
            self.conn.close()

    # ---- one piece --------------------------------------------------------------------------
    def get(self, platform, kind, key, default=None):
        t = self._tables.get((platform, kind))
        if t is not None:
            return t.get(key, default)
        with self.lock:
            row = self.conn.execute("SELECT tag, value FROM knowledge WHERE platform=? AND kind=? AND key=?",
                                    (platform, kind, key)).fetchone()
        return decode(row[0], row[1]) if row else default

    def has(self, platform, kind, key):
        t = self._tables.get((platform, kind))
        if t is not None:
            return key in t
        with self.lock:
            return self.conn.execute("SELECT 1 FROM knowledge WHERE platform=? AND kind=? AND key=?",
                                     (platform, kind, key)).fetchone() is not None

    def put(self, platform, kind, key, value, source="", origin=LEARNED):
        self.put_many(platform, kind, {key: value}, source, origin)

    def put_many(self, platform, kind, values, source="", origin=LEARNED):
        rows, now = [], time.time()
        for key, value in values.items():
            tag, data = encode(value)
            rows.append((platform, kind, key, None, digest(tag, data), tag, sqlite3.Binary(data), source, origin, now))
        with self.lock, self.conn:
            self.conn.executemany("INSERT OR REPLACE INTO knowledge(platform, kind, key, seq, hash, tag, value, source, "
                                  "origin, made) VALUES(?,?,?,?,?,?,?,?,?,?)", rows)
        t = self._tables.get((platform, kind))
        if t is not None:
            t.update(values)

    def forget(self, platform, kind, key):
        with self.lock, self.conn:
            self.conn.execute("DELETE FROM knowledge WHERE platform=? AND kind=? AND key=?", (platform, kind, key))
        self._tables.get((platform, kind), {}).pop(key, None)

    def about(self, platform, kind, key):
        """(source, origin, made, hash) of one piece, or None."""
        with self.lock:
            return self.conn.execute("SELECT source, origin, made, hash FROM knowledge WHERE platform=? AND kind=? "
                                     "AND key=?", (platform, kind, key)).fetchone()

    # ---- a whole kind ------------------------------------------------------------------------
    def table(self, platform, kind):
        """{key: value} of every piece of a kind, in the order the kind was made in; read from the
        file once per process (and kept up to date with what this process puts)."""
        t = self._tables.get((platform, kind))
        if t is None:
            with self.lock:
                rows = self.conn.execute("SELECT key, tag, value FROM knowledge WHERE platform=? AND kind=? "
                                         "ORDER BY seq IS NULL, seq, rowid", (platform, kind)).fetchall()
            t = self._tables[(platform, kind)] = {k: decode(tag, v) for k, tag, v in rows}
        return t

    def stamp(self, platform, kind):
        with self.lock:
            row = self.conn.execute("SELECT stamp FROM loaded WHERE platform=? AND kind=?", (platform, kind)).fetchone()
        return row[0] if row else None

    def ensure(self, platform, kind, stamp, make, source, origin=AUTHORED):
        """The kind as `make()` gives it ({key: value}, its order kept) under `stamp`: made again
        only when the stamp it was made under is another. -> table(platform, kind)"""
        if self._ensured.get((platform, kind)) == stamp:
            return self.table(platform, kind)
        if self.stamp(platform, kind) != stamp:
            values = make()
            rows, now = [], time.time()
            for seq, (key, value) in enumerate(values.items()):
                tag, data = encode(value)
                rows.append((platform, kind, key, seq, digest(tag, data), tag, sqlite3.Binary(data), source, origin, now))
            with self.lock, self.conn:
                self.conn.execute("DELETE FROM knowledge WHERE platform=? AND kind=? AND origin IN (?, ?)",
                                  (platform, kind, AUTHORED, DERIVED))
                self.conn.executemany("INSERT OR REPLACE INTO knowledge(platform, kind, key, seq, hash, tag, value, "
                                      "source, origin, made) VALUES(?,?,?,?,?,?,?,?,?,?)", rows)
                self.conn.execute("INSERT OR REPLACE INTO loaded(platform, kind, stamp, source, made) VALUES(?,?,?,?,?)",
                                  (platform, kind, stamp, source, now))
            self._tables.pop((platform, kind), None)
        self._ensured[(platform, kind)] = stamp
        return self.table(platform, kind)

    def kinds(self, platform=None):
        """[(platform, kind, pieces, bytes, {origin: count}, source of the last load)]"""
        with self.lock:
            rows = self.conn.execute(
                "SELECT platform, kind, origin, COUNT(*), SUM(LENGTH(value)) FROM knowledge %s GROUP BY platform, "
                "kind, origin ORDER BY platform, kind" % ("WHERE platform=?" if platform else ""),
                (platform,) if platform else ()).fetchall()
            loaded = {(p, k): s for p, k, s in self.conn.execute("SELECT platform, kind, source FROM loaded")}
        out = {}
        for p, k, origin, n, size in rows:
            e = out.setdefault((p, k), [p, k, 0, 0, {}, loaded.get((p, k), "")])
            e[2] += n
            e[3] += size or 0
            e[4][origin] = n
        return list(out.values())

    def carried(self, what, mark=None):
        """What was carried over from an older place, once: the mark kept for `what` (or set it)."""
        with self.lock, self.conn:
            if mark is not None:
                self.conn.execute("INSERT OR REPLACE INTO carried(what, mark) VALUES(?,?)", (what, mark))
                return mark
            row = self.conn.execute("SELECT mark FROM carried WHERE what=?", (what,)).fetchone()
        return row[0] if row else None

    # ---- bodies and their names (the name store) -----------------------------------------------
    def add_module(self, sha, name, platform=""):
        from codebreaker import knowledge
        platform = knowledge.platform(platform or "")
        with self.lock, self.conn:
            self.conn.execute("INSERT OR REPLACE INTO module(sha, name, platform) VALUES(?,?,?)", (sha, name, platform))

    def set_bodies(self, module, rows):
        """Replace a module's bodies: {addr: (hash, name the module itself gives it or None, where
        that name is from)}."""
        with self.lock, self.conn:
            self.conn.execute("DELETE FROM body WHERE module=?", (module,))
            self.conn.executemany("INSERT INTO body(module, addr, hash, name, source) VALUES(?,?,?,?,?)",
                                  [(module, a, h, n, s) for a, (h, n, s) in rows.items()])

    def bodies(self, module):
        """{addr: (hash, own name, its source)}"""
        with self.lock:
            return {a: (h, n, s) for a, h, n, s in
                    self.conn.execute("SELECT addr, hash, name, source FROM body WHERE module=?", (module,))}

    def name_body(self, body_id, name, source="named by hand"):
        """Give a body a name. It then holds for every module containing that body."""
        with self.lock, self.conn:
            self.conn.execute("INSERT OR REPLACE INTO body_name(hash, name, source, ts) VALUES(?,?,?,?)",
                              (body_id, name, source, time.time()))

    def name(self, body_id):
        """(name, where it is from) of a body: the name given to it, else the one every module that
        names it agrees on; or None."""
        with self.lock:
            row = self.conn.execute("SELECT name, source FROM body_name WHERE hash=?", (body_id,)).fetchone()
            if row:
                return row[0], row[1]
            found = self.conn.execute("SELECT DISTINCT b.name, m.name FROM body b LEFT JOIN module m ON m.sha=b.module "
                                      "WHERE b.hash=? AND b.name IS NOT NULL", (body_id,)).fetchall()
        names = {}
        for name, module in found:
            names.setdefault(name, module or "another module")
        if len(names) == 1:
            name, module = next(iter(names.items()))
            return name, "the same body in %s" % module
        return None

    def names_from_bodies(self, module):
        """{addr: (name, where it is from)} for the functions of a module whose body has a name
        somewhere: one given to the body, else the name another module gives the same body
        (when the modules that name it agree)."""
        out = {}
        with self.lock:
            given = self.conn.execute(
                "SELECT b.addr, b.hash, n.name, n.source FROM body b JOIN body_name n ON n.hash=b.hash "
                "WHERE b.module=?", (module,)).fetchall()
            others = self.conn.execute(
                "SELECT b.addr, o.name, m.name FROM body b JOIN body o ON o.hash=b.hash AND o.module<>b.module "
                "AND o.name IS NOT NULL LEFT JOIN module m ON m.sha=o.module WHERE b.module=? AND b.name IS NULL",
                (module,)).fetchall()
        for addr, _digest, name, source in given:
            out[addr] = (name, source)
        seen = {}
        for addr, name, other in others:
            seen.setdefault(addr, {}).setdefault(name, other or "another module")
        for addr, names in seen.items():
            if addr not in out and len(names) == 1:
                name, other = next(iter(names.items()))
                out[addr] = (name, "the same body in %s" % other)
        return out

    def shared_bodies(self):
        """(bodies recorded, distinct bodies, bodies found in two or more modules, functions that
        are a copy of a body in another module)"""
        with self.lock:
            total, distinct = self.conn.execute("SELECT COUNT(*), COUNT(DISTINCT hash) FROM body").fetchone()
            shared, copies = self.conn.execute(
                "SELECT COUNT(*), COALESCE(SUM(n), 0) FROM (SELECT COUNT(*) n FROM body GROUP BY hash "
                "HAVING COUNT(DISTINCT module) > 1)").fetchone()
        return total, distinct, shared, copies

    def carry_bodies(self, store_db, label):
        """Bodies and body names an older function store holds (before they were kept here), copied
        in once -- and again when names were added to it since (an older checkout still writing
        there): what was given by hand is never lost in the move."""
        try:
            n, newest = store_db.execute("SELECT COUNT(*), COALESCE(MAX(ts), 0) FROM body_name").fetchone()
            bodies = store_db.execute("SELECT COUNT(*) FROM body").fetchone()[0]
        except sqlite3.Error:
            return
        mark = "%d:%r:%d" % (n, newest, bodies)
        if self.carried(label) == mark:
            return
        names = store_db.execute("SELECT hash, name, source, ts FROM body_name").fetchall()
        rows = store_db.execute("SELECT module, addr, hash, name, source FROM body").fetchall()
        modules = store_db.execute("SELECT sha, name, kind FROM module WHERE sha IN (SELECT DISTINCT module FROM body)"
                                   ).fetchall()
        with self.lock, self.conn:
            self.conn.executemany("INSERT OR IGNORE INTO body_name(hash, name, source, ts) VALUES(?,?,?,?)", names)
            self.conn.executemany("INSERT OR IGNORE INTO body(module, addr, hash, name, source) VALUES(?,?,?,?,?)", rows)
            self.conn.executemany("INSERT OR IGNORE INTO module(sha, name, platform) VALUES(?,?,?)", modules)
        self.carried(label, mark)


def open(path=None):                                    # noqa: A001 (as sqlite3.connect / shelve.open)
    """The knowledge base in `path` (default: default_path()), one per process."""
    path = os.path.abspath(path or default_path())
    with _LOCK:
        kb = _OPEN.get(path)
        if kb is None:
            kb = _OPEN[path] = KnowledgeBase(path)
        return kb
