"""functions.py -- the function store: one place that knows every function and what was made of it.

Everything CodeBreaker does to a function ends up here, and everything that reports reads from
here: the module writer, the scoreboard, the tools that explain a failure.

    module       one original binary (by content hash)
    function     one function of a module (by address)
    candidate    one decompiler's reconstruction of a function: its source, or why it has none
    verdict      what the verifier found for a candidate, under one set of verifier settings
    text         something a decompiler's functions share, such as its header of types
    identified   a function recognised as a runtime library's (its name there, and the library)

Where it is kept. A module's functions, candidates, verdicts, texts, identified functions, built
signatures and byte-match variants are tables of that binary's program database
(state/db/<sha>.sqlite, engine/facts.py: the same file the engine keeps the binary's facts in) --
the verdict table IS the database's. The store's own file (state/functions.sqlite, CB_FUNCTIONS_DB)
is the index of them: which modules there are (sha, name, path, kind), the verifier profiles, and
a summary per module (functions, verdicts, the latest class of each function) kept as each module
is written, so the questions asked across the corpus (overview, the ledger, progress) open one file.
A store given a path of its own keeps its databases in the folder `db` beside it (a store not named
functions.sqlite: in <its name>.db/), as the knowledge base beside it is found.

Carried over: a store from before the databases held every module's rows in its own file. The
first time such a store is opened they are copied into the modules' databases, each verdict marked
`carried` (1) -- and again for rows an older checkout wrote there since (by their time). The old
tables are then left as they were and not read.

Bodies and the names given to them -- the name store -- are the knowledge base's
(knowledge/store.py: body, body_name); the store's set_bodies, bodies, name_body,
names_from_bodies and shared_bodies are kept for their callers and go there. A store made
before carries its bodies over the first time it is opened.

A verdict is kept per verifier profile (a digest of every setting a verdict depends on), so
changing the verifier adds new verdicts beside the old ones instead of erasing them: nothing
has to be re-judged to see what a change did, and old results stay comparable.

    from codebreaker.store import functions
    fs = functions.open()                          # state/functions.sqlite and state/db/
    fs.add_module(sha, "comctl32.dll", path, "pe")
    fs.record(sha, "bfee83ad", "core", rev, sig, "sub_0183AD", source, profile,
              "equivalent", limits="coverage=100")
    fs.best(sha, profile)                          # {address: the best candidate's row}
    fs.tally(profile, sha)                         # {class: count}
    fs.module_rows()                               # [(sha, name, path, kind)] (the index)
"""
import json
import os
import sqlite3
import threading
import time

from codebreaker.verify import verdicts as V

# the index: the store's own file
_INDEX = """
CREATE TABLE IF NOT EXISTS module(
    sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS summary(
    module TEXT PRIMARY KEY, functions INTEGER, identified INTEGER, judged INTEGER, last REAL,
    last_profile TEXT, profiles TEXT, latest TEXT);
CREATE TABLE IF NOT EXISTS store_about(name TEXT PRIMARY KEY, value TEXT);
"""

# a module's tables, in its program database
_MODULE = """
CREATE TABLE IF NOT EXISTS function(
    id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,
    UNIQUE(module, addr));
CREATE TABLE IF NOT EXISTS candidate(
    id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,
    name TEXT, source TEXT, problem TEXT, ts REAL,
    UNIQUE(function, engine));
CREATE TABLE IF NOT EXISTS verdict(
    candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,
    limits TEXT, detail TEXT, ts REAL, carried INTEGER NOT NULL DEFAULT 0,
    PRIMARY KEY (candidate, profile));
CREATE TABLE IF NOT EXISTS text(
    module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,
    PRIMARY KEY (module, engine, kind));
CREATE INDEX IF NOT EXISTS candidate_function ON candidate(function);
CREATE TABLE IF NOT EXISTS identified(
    module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, library TEXT, ts REAL,
    PRIMARY KEY (module, addr));
CREATE TABLE IF NOT EXISTS built(
    module TEXT NOT NULL, addr TEXT NOT NULL, profile TEXT NOT NULL, sig TEXT,
    PRIMARY KEY (module, addr, profile));
CREATE TABLE IF NOT EXISTS variant(
    id INTEGER PRIMARY KEY, function INTEGER NOT NULL, compiler TEXT NOT NULL, source TEXT,
    parent INTEGER, knobs TEXT, distance INTEGER, total INTEGER, verdict TEXT, behaves TEXT, ts REAL,
    UNIQUE(function, compiler, knobs));
CREATE INDEX IF NOT EXISTS variant_function ON variant(function);
"""

# How good a result is, best first. A candidate's class is what the module writer prints. The
# words -- the judge's, the classes', their titles, the limits that make a match weak -- are the
# one vocabulary's (verify/verdicts.py); they are named here too, as the store's readers know them.
ORDER, TITLE, WEAK = V.ORDER, V.TITLE, V.WEAK_LIMITS
# A function recognised as a runtime library's is counted on its own, not as a failure of
# reconstruction -- unless it was also verified, which is the stronger statement and stays.
IDENTIFIED, SCORED = V.IDENTIFIED, V.SCORED
classify = V.classify


def _judged(row):
    """A row read from the verdict table, its word as the vocabulary says it now (a store written
    before keeps meaning what it meant: no-synth is no-source)."""
    if row.get("verdict") is not None:
        row["verdict"] = V.verdict(row["verdict"])
    return row


def overview(path=None):
    """The store at a glance, quickly (one file: the index's summaries, where the coverage ledger
    takes many minutes): per module {sha, name, kind, path, functions, identified, latest: {class:
    count}}, by each function's LATEST verdict under any judge -- what it last came to, not what the
    judge in force says (a change to the judge leaves every module unjudged until it is measured
    again)."""
    fs = open(path, readonly=True)
    if fs is None:
        return []
    try:
        out = []
        summaries = fs.summaries()
        for sha, name, p, kind in fs.module_rows():
            s = summaries.get(sha) or {}
            latest = dict.fromkeys(ORDER, 0)
            latest.update(s.get("latest") or {})
            n = s.get("functions") or 0
            latest["no-source"] += max(0, n - sum(latest.values()))      # (functions with no verdict at all)
            out.append({"sha": sha, "name": name, "kind": kind, "path": p, "functions": n,
                        "identified": s.get("identified") or 0, "latest": latest})
        return sorted(out, key=lambda m: m["name"] or "")
    finally:
        fs.close()


def latest(module_sha, path=None):
    """{addr: class} by each function's latest verdict under any judge (see overview)."""
    fs = open(path, readonly=True)
    if fs is None:
        return {}
    try:
        return fs.latest(module_sha)
    finally:
        fs.close()


_LATEST = """
    SELECT addr, verdict, limits FROM (
      SELECT f.addr AS addr, v.verdict AS verdict, v.limits AS limits,
             ROW_NUMBER() OVER (PARTITION BY f.id ORDER BY v.ts DESC) AS k
      FROM function f JOIN candidate c ON c.function = f.id JOIN verdict v ON v.candidate = c.id
      WHERE f.module = ?) WHERE k = 1"""


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_FUNCTIONS_DB") or settings.state("functions.sqlite")


def databases_folder(path=None, own=None):
    """Where a store keeps its modules' databases: the engine's (state/db) for the store of the
    state folder; for a store given a path of its own, the folder `db` beside it (or <name>.db/
    for a store not called functions.sqlite)."""
    from codebreaker.engine import facts
    if own is None:
        own = bool(path or os.environ.get("CB_FUNCTIONS_DB"))
    path = path or default_path()
    if not own:
        return facts.root()
    if os.path.basename(path).lower() == "functions.sqlite":
        return os.path.join(os.path.dirname(os.path.abspath(path)), "db")
    return os.path.splitext(os.path.abspath(path))[0] + ".db"


def _ro(path):
    return sqlite3.connect("file:%s?mode=ro" % path.replace("\\", "/").replace("?", "%3f").replace("#", "%23"),
                           uri=True, timeout=60.0, check_same_thread=False)


class _Module(object):
    """One module's tables, in its database: the connection and the lock its writes go under."""

    def __init__(self, conn, lock, owned):
        self.conn, self.lock, self.owned = conn, lock, owned

    def rows(self, q, args=()):
        with self.lock:
            return self.conn.execute(q, args).fetchall()

    def one(self, q, args=()):
        with self.lock:
            return self.conn.execute(q, args).fetchone()


class _Empty(object):
    """A module with no database (read-only, and never written): nothing in it."""

    def rows(self, q, args=()):
        return []

    def one(self, q, args=()):
        return None


class FunctionStore(object):
    def __init__(self, path=None, readonly=False):
        self.own_path = bool(path or os.environ.get("CB_FUNCTIONS_DB"))
        self.path = path or default_path()
        self.readonly = readonly
        self.folder = databases_folder(self.path, self.own_path)
        self._modules = {}
        self._lock = threading.RLock()
        if readonly:
            self.db = _ro(self.path)
            return
        os.makedirs(os.path.dirname(os.path.abspath(self.path)), exist_ok=True)
        self.db = sqlite3.connect(self.path, timeout=60.0)     # several workers may write
        self.db.execute("PRAGMA journal_mode=WAL")
        self.db.execute("PRAGMA synchronous=NORMAL")
        self.db.executescript(_INDEX)
        self.db.commit()
        self.carry()

    def close(self):
        for m in self._modules.values():
            if getattr(m, "owned", False):
                try:
                    m.conn.close()
                except sqlite3.Error:
                    pass
        self._modules.clear()
        self.db.close()

    # ---- a module's database ---------------------------------------------------------------
    def database_path(self, module):
        return os.path.join(self.folder, module + ".sqlite")

    def _m(self, module):
        """The module's tables (its database opened, and its tables made, the first time)."""
        got = self._modules.get(module)
        if got is not None:
            return got
        with self._lock:
            got = self._modules.get(module)
            if got is not None:
                return got
            p = self.database_path(module)
            if self.readonly:
                got = _Module(_ro(p), threading.RLock(), True) if os.path.isfile(p) else _Empty()
            else:
                from codebreaker.engine import facts
                d = facts.open_at(p)                       # (the engine's connection to it, shared)
                with d.lock:
                    d.conn.executescript(_MODULE)
                    if "carried" not in [r[1] for r in d.conn.execute("PRAGMA table_info(verdict)")]:
                        d.conn.execute("ALTER TABLE verdict ADD COLUMN carried INTEGER NOT NULL DEFAULT 0")
                    d.conn.commit()
                got = _Module(d.conn, d.lock, False)
            self._modules[module] = got
            return got

    def module_db(self, module):
        """The sqlite3 connection of a module's tables (for reading by hand, and the tests)."""
        return getattr(self._m(module), "conn", None)

    class _Write(object):
        def __init__(self, m):
            self.m = m

        def __enter__(self):
            self.m.lock.acquire()
            self.m.conn.__enter__()
            return self.m.conn

        def __exit__(self, *exc):
            try:
                return self.m.conn.__exit__(*exc)
            finally:
                self.m.lock.release()

    def _write(self, module):
        if self.readonly:
            raise sqlite3.OperationalError("attempt to write a readonly database")
        return self._Write(self._m(module))

    # ---- the summary of a module, in the index ------------------------------------------------
    def _summarize(self, module):
        """The module's summary made again from its tables (after each write to them)."""
        m = self._m(module)
        functions = m.one("SELECT COUNT(*) FROM function WHERE module=?", (module,))[0]
        identified = m.one("SELECT COUNT(*) FROM identified WHERE module=?", (module,))[0]
        q = "FROM verdict v JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function WHERE f.module=?"
        judged, last = m.one("SELECT COUNT(*), MAX(v.ts) " + q, (module,))
        row = m.one("SELECT v.profile " + q + " ORDER BY v.ts DESC LIMIT 1", (module,))
        profiles = sorted(r[0] for r in m.rows("SELECT DISTINCT v.profile " + q, (module,)))
        latest = {}
        for _addr, verdict, limits in m.rows(_LATEST, (module,)):
            cls = classify(verdict, limits)[0]
            latest[cls] = latest.get(cls, 0) + 1
        with self.db:
            self.db.execute("INSERT OR REPLACE INTO summary(module, functions, identified, judged, last, last_profile, "
                            "profiles, latest) VALUES(?,?,?,?,?,?,?,?)",
                            (module, functions, identified, judged, last, row[0] if row else None,
                             json.dumps(profiles), json.dumps(latest, sort_keys=True)))

    def summaries(self):
        """{sha: {functions, identified, judged, last, last_profile, profiles, latest}}"""
        out = {}
        for sha, n, ident, judged, last, lp, profiles, latest in self.db.execute(
                "SELECT module, functions, identified, judged, last, last_profile, profiles, latest FROM summary"):
            out[sha] = {"functions": n or 0, "identified": ident or 0, "judged": judged or 0, "last": last,
                        "last_profile": lp, "profiles": json.loads(profiles or "[]"),
                        "latest": json.loads(latest or "{}")}
        return out

    # ---- writing -------------------------------------------------------------------------
    def add_module(self, sha, name, path="", kind=""):
        with self.db:
            self.db.execute("INSERT INTO module(sha, name, path, kind, ts) VALUES(?,?,?,?,?) "
                            "ON CONFLICT(sha) DO UPDATE SET name=excluded.name, path=excluded.path, "
                            "kind=excluded.kind", (sha, name.lower(), path, kind, time.time()))
        self._m(sha)

    def add_profile(self, digest, settings):
        with self.db:
            self.db.execute("INSERT OR IGNORE INTO profile(digest, settings, ts) VALUES(?,?,?)",
                            (digest, settings, time.time()))

    @staticmethod
    def _function(conn, module, addr, name=None, size=None):
        row = conn.execute("SELECT id FROM function WHERE module=? AND addr=?", (module, addr)).fetchone()
        if row:
            return row[0]
        return conn.execute("INSERT INTO function(module, addr, name, size) VALUES(?,?,?,?)",
                            (module, addr, name, size)).lastrowid

    def add_functions(self, module, addrs):
        """Make sure every address of a module is known, reconstructed or not."""
        with self._write(module) as db:
            for addr in addrs:
                self._function(db, module, addr)
        self._summarize(module)

    def record_many(self, module, engine, rev, profile, rows):
        """rows: dicts with addr, name, source, sig, verdict and optionally reason, limits, detail,
        problem. One transaction. A candidate is replaced by a newer one from the same engine;
        its verdicts under other profiles stay as long as the source did not change."""
        now = time.time()
        with self._write(module) as db:
            for r in rows:
                f = self._function(db, module, r["addr"], r.get("name"))
                old = db.execute("SELECT id, sig FROM candidate WHERE function=? AND engine=?", (f, engine)).fetchone()
                if old:
                    cid = old[0]
                    if old[1] != r.get("sig"):
                        db.execute("DELETE FROM verdict WHERE candidate=? AND profile<>?", (cid, profile))
                    db.execute("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? "
                               "WHERE id=?", (rev, r.get("sig"), r.get("name"), r.get("source"),
                                              r.get("problem"), now, cid))
                else:
                    cid = db.execute(
                        "INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) "
                        "VALUES(?,?,?,?,?,?,?,?)", (f, engine, rev, r.get("sig"), r.get("name"),
                                                    r.get("source"), r.get("problem"), now)).lastrowid
                db.execute(
                    "INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, carried) "
                    "VALUES(?,?,?,?,?,?,?,?,0)", (cid, profile, r.get("sig"), V.verdict(r["verdict"]), r.get("reason") or "",
                                                  r.get("limits") or "", r.get("detail") or "", now))
        self._summarize(module)

    def put_text(self, module, engine, kind, body):
        """A module-level text an engine's functions need, e.g. kind "header": its types and
        declarations."""
        with self._write(module) as db:
            db.execute("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)",
                       (module, engine, kind, body, time.time()))

    def get_text(self, module, engine, kind):
        row = self._m(module).one("SELECT body FROM text WHERE module=? AND engine=? AND kind=?",
                                  (module, engine, kind))
        return row[0] if row else ""

    def record(self, module, addr, engine, rev, sig, name, source, profile, verdict, **more):
        self.record_many(module, engine, rev, profile,
                         [dict(addr=addr, sig=sig, name=name, source=source, verdict=verdict, **more)])

    # ---- reading -------------------------------------------------------------------------
    _ROW = ("addr", "engine", "rev", "sig", "name", "source", "problem", "verdict", "reason", "limits",
            "detail", "judged_sig")

    def candidates(self, module, profile, engine=None):
        """[row] for every candidate of a module, with its verdict under `profile` (verdict None
        when it has not been judged under that profile)."""
        q = ("SELECT f.addr, c.engine, c.rev, c.sig, c.name, c.source, c.problem, v.verdict, v.reason, "
             "v.limits, v.detail, v.sig FROM function f JOIN candidate c ON c.function=f.id "
             "LEFT JOIN verdict v ON v.candidate=c.id AND v.profile=? WHERE f.module=?")
        args = [profile, module]
        if engine:
            q += " AND c.engine=?"
            args.append(engine)
        return [_judged(dict(zip(self._ROW, r))) for r in self._m(module).rows(q, args)]

    def stored(self, module, addr):
        """[{engine, rev, name, source, problem, verdicts: [{profile, verdict, reason, limits, carried}]}]
        -- every candidate kept for one function, each with its verdicts under any profile, newest first."""
        m = self._m(module)
        out = []
        for cid, e, r, n, s, pr in m.rows("SELECT c.id, c.engine, c.rev, c.name, c.source, c.problem FROM candidate c "
                                          "JOIN function f ON f.id=c.function WHERE f.module=? AND f.addr=?",
                                          (module, addr)):
            out.append({"engine": e, "rev": r, "name": n, "source": s or "", "problem": pr or "", "verdicts": [
                {"profile": p, "verdict": V.verdict(v), "reason": why or "", "limits": lim or "", "carried": bool(car)}
                for p, v, why, lim, car in m.rows("SELECT profile, verdict, reason, limits, carried FROM verdict "
                                                  "WHERE candidate=? ORDER BY ts DESC", (cid,))]})
        return out

    def built(self, module, profile):
        """{addr: signature of the compiled code a function's verdict under `profile` was reached
        for} -- its own object code and that of everything it reaches (verify/reconstruct.py)."""
        return dict(self._m(module).rows("SELECT addr, sig FROM built WHERE module=? AND profile=?", (module, profile)))

    def set_built(self, module, profile, sigs):
        with self._write(module) as db:
            db.executemany("INSERT OR REPLACE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)",
                           [(module, addr, profile, sig) for addr, sig in sigs.items()])

    def judged(self, module, profile, engine):
        """{addr: row} of an engine's candidates that have a verdict under `profile` for the source
        they have now -- what does not need judging again."""
        return {r["addr"]: r for r in self.candidates(module, profile, engine)
                if r["verdict"] is not None and r["judged_sig"] == r["sig"]}

    def best(self, module, profile, prefer=()):
        """{addr: row} -- for each function the candidate with the best class under `profile`.
        Between equally good ones the engine earlier in `prefer` wins. Each row gains `class`
        and `weak`."""
        rank = {e: i for i, e in enumerate(prefer)}
        out = {}
        for r in self.candidates(module, profile):
            if r["verdict"] is None:
                continue
            r["class"], r["weak"] = classify(r["verdict"], r["limits"])
            if r["class"] != "no-source" and not r["source"]:
                r["class"] = "no-source"
            key = (ORDER.index(r["class"]), rank.get(r["engine"], len(rank)))
            if r["addr"] not in out or key < out[r["addr"]]["_key"]:
                r["_key"] = key
                out[r["addr"]] = r
        for r in out.values():
            del r["_key"]
        return out

    def latest(self, module):
        """{addr: class} by each function's latest verdict under any judge (see overview)."""
        return {addr: classify(verdict, limits)[0] for addr, verdict, limits in self._m(module).rows(_LATEST, (module,))}

    def addresses(self, module):
        return [r[0] for r in self._m(module).rows("SELECT addr FROM function WHERE module=? ORDER BY addr", (module,))]

    def tally(self, profile, module, prefer=()):
        """{class: count} over every known function of a module (unjudged ones are no-source)."""
        best = self.best(module, profile, prefer)
        out = dict.fromkeys(ORDER, 0)
        for addr in self.addresses(module):
            out[best[addr]["class"] if addr in best else "no-source"] += 1
        return out

    # ---- runtime-library functions -------------------------------------------------------
    def set_identified(self, module, found):
        """Replace what is known of a module's runtime-library functions: {addr: (name, library)}."""
        now = time.time()
        with self._write(module) as db:
            db.execute("DELETE FROM identified WHERE module=?", (module,))
            db.executemany("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)",
                           [(module, a, n, lib, now) for a, (n, lib) in found.items()])
        self._summarize(module)

    def identified(self, module):
        """{addr: (name, library)}"""
        return {a: (n, lib) for a, n, lib in
                self._m(module).rows("SELECT addr, name, library FROM identified WHERE module=?", (module,))}

    def scores(self, profile, module, prefer=()):
        """tally() with the identified functions in a class of their own: {class: count} over
        SCORED. A function both identified and verified (or weakly verified) counts as verified."""
        best, known = self.best(module, profile, prefer), self.identified(module)
        out = dict.fromkeys(SCORED, 0)
        for addr in self.addresses(module):
            cls = best[addr]["class"] if addr in best else "no-source"
            out[IDENTIFIED if addr in known and cls not in ("verified", "weak") else cls] += 1
        return out

    # ---- bodies and their names: the knowledge base's (knowledge/store.py) -------------------
    @property
    def knowledge(self):
        """The knowledge base the store's bodies are kept in: the state folder's (a store given a
        path of its own: the one beside it), with what this store held before carried over."""
        kb = self.__dict__.get("_knowledge")
        if kb is None:
            from codebreaker.knowledge import store as kstore
            kb = kstore.open(kstore.beside(self.path) if self.own_path else None)
            kb.carry_bodies(self.db, "function store " + os.path.normcase(os.path.abspath(self.path)))
            self._knowledge = kb
        return kb

    def set_bodies(self, module, rows):
        """Replace a module's bodies: {addr: (hash, name the module itself gives it or None, where
        that name is from)}."""
        row = self.db.execute("SELECT name, kind FROM module WHERE sha=?", (module,)).fetchone()
        if row:
            self.knowledge.add_module(module, row[0], row[1])
        self.knowledge.set_bodies(module, rows)

    def bodies(self, module):
        """{addr: (hash, own name, its source)}"""
        return self.knowledge.bodies(module)

    def name_body(self, digest, name, source="named by hand"):
        """Give a body a name. It then holds for every module containing that body."""
        self.knowledge.name_body(digest, name, source)

    def names_from_bodies(self, module):
        """{addr: (name, where it is from)} (knowledge/store.py names_from_bodies)"""
        return self.knowledge.names_from_bodies(module)

    def shared_bodies(self):
        return self.knowledge.shared_bodies()

    def modules_with_body(self, digest):
        """How many modules hold the body `digest`."""
        kb = self.knowledge
        with kb.lock:
            return kb.conn.execute("SELECT COUNT(DISTINCT module) FROM body WHERE hash=?", (digest,)).fetchone()[0]

    # ---- byte-match variants (the repair search) -----------------------------------------
    # The behavioural pipeline (candidate/verdict) judges whether a reconstruction MEANS the same.
    # The repair search is a different question: compiled with a compiler of the original's time,
    # how close are its BYTES to the original's? It explores many source variants of one function,
    # each a small change (a "knob": a declaration order, a loop form, an unused local, ...) from a
    # parent, and keeps those that shrink the byte distance while still behaving alike. Those live
    # here, apart from the behavioural tables, because the question and the compiler are different.
    #   distance  bytes that differ outside the places the linker fills in; 0 is byte-identical
    #   verdict   "identical" | "same size" | "different"
    #   behaves   the behavioural verdict of this exact source, so a variant is never kept if the
    #             change broke the meaning ("equivalent"/"consistent" keep; others are recorded,
    #             not used)
    def record_variant(self, module, addr, compiler, source, distance, total, verdict,
                       knobs="", parent=None, behaves=""):
        """Record (or update) one variant of a function for a period compiler. -> its id."""
        now = time.time()
        with self._write(module) as db:
            f = self._function(db, module, addr)
            db.execute(
                "INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, "
                "behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?) ON CONFLICT(function, compiler, knobs) DO UPDATE SET "
                "source=excluded.source, parent=excluded.parent, distance=excluded.distance, "
                "total=excluded.total, verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts",
                (f, compiler, source, parent, knobs, distance, total, V.match(verdict), V.verdict(behaves), now))
            return db.execute("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?",
                              (f, compiler, knobs)).fetchone()[0]

    _VROW = ("addr", "compiler", "source", "parent", "knobs", "distance", "total", "verdict", "behaves")

    def variants(self, module, compiler=None):
        """[row] of every recorded variant of a module's functions (optionally one compiler)."""
        q = ("SELECT f.addr, v.compiler, v.source, v.parent, v.knobs, v.distance, v.total, v.verdict, "
             "v.behaves FROM variant v JOIN function f ON f.id=v.function WHERE f.module=?")
        args = [module]
        if compiler:
            q += " AND v.compiler=?"
            args.append(compiler)
        out = [dict(zip(self._VROW, r)) for r in self._m(module).rows(q, args)]
        for r in out:                                   # (older spellings read as the words now)
            r["verdict"], r["behaves"] = V.match(r["verdict"]), V.verdict(r["behaves"])
        return out

    def best_variant(self, module, compiler):
        """{addr: the closest variant row} -- the one with the smallest byte distance that still
        behaves alike (or, if none behaves, the smallest distance), per function."""
        out = {}
        for r in self.variants(module, compiler):
            ok = r["behaves"] in V.GOOD + ("",)
            key = (0 if ok else 1, r["distance"] if r["distance"] is not None else 1 << 30)
            if r["addr"] not in out or key < out[r["addr"]]["_key"]:
                r["_key"] = key
                out[r["addr"]] = r
        for r in out.values():
            del r["_key"]
        return out

    def match_tally(self, module, compiler):
        """(functions with a variant, identical, same size, different) for one compiler."""
        best = self.best_variant(module, compiler)
        ident = sum(1 for r in best.values() if r["verdict"] == V.IDENTICAL)
        same = sum(1 for r in best.values() if r["verdict"] == V.SAME_SIZE)
        return len(best), ident, same, len(best) - ident - same

    # ---- across modules: the index ---------------------------------------------------------
    def modules(self, profile=None):
        """[(sha, name)] of modules, limited to those judged under `profile` when given."""
        rows = self.db.execute("SELECT sha, name FROM module ORDER BY name").fetchall()
        if profile is None:
            return rows
        judged = {sha for sha, s in self.summaries().items() if profile in s["profiles"]}
        return [r for r in rows if r[0] in judged]

    def module_rows(self, kind=None):
        """[(sha, name, path, kind)] of every module (of one kind)."""
        if kind is None:
            return self.db.execute("SELECT sha, name, path, kind FROM module ORDER BY name").fetchall()
        return self.db.execute("SELECT sha, name, path, kind FROM module WHERE kind=? ORDER BY name", (kind,)).fetchall()

    def module_path(self, name):
        """The path the module of this name was last added from (None if it is not known)."""
        row = self.db.execute("SELECT path FROM module WHERE name=? ORDER BY ts DESC LIMIT 1", (name,)).fetchone()
        return row[0] if row else None

    def stamps(self):
        """{sha: (functions, verdicts, the latest verdict's time)} -- what changes when a module's
        results change."""
        return {sha: (s["functions"], s["judged"], s["last"]) for sha, s in self.summaries().items()
                if s["functions"]}

    def latest_profile(self, kind):
        """The profile of the latest verdict on a module of this kind (None when there is none)."""
        row = self.db.execute("SELECT s.last_profile FROM summary s JOIN module m ON m.sha=s.module "
                              "WHERE m.kind=? AND s.last IS NOT NULL ORDER BY s.last DESC LIMIT 1", (kind,)).fetchone()
        return row[0] if row else None

    def recent(self, limit=8):
        """[(name, kind, verdicts, the latest verdict's time)] of the modules judged most lately."""
        return self.db.execute("SELECT m.name, m.kind, s.judged, s.last FROM summary s JOIN module m ON m.sha=s.module "
                               "WHERE s.last IS NOT NULL ORDER BY s.last DESC LIMIT ?", (limit,)).fetchall()

    def verdicts_since(self, t):
        """How many verdicts were reached after the time `t`, over every module."""
        n = 0
        for (sha,) in self.db.execute("SELECT module FROM summary WHERE last > ?", (t,)).fetchall():
            n += self._m(sha).one("SELECT COUNT(*) FROM verdict WHERE ts > ?", (t,))[0]
        return n

    # ---- the store from before the databases, carried over -----------------------------------
    def _about(self, name, value=None):
        if value is None:
            row = self.db.execute("SELECT value FROM store_about WHERE name=?", (name,)).fetchone()
            return row[0] if row else None
        with self.db:
            self.db.execute("INSERT OR REPLACE INTO store_about(name, value) VALUES(?,?)", (name, str(value)))
        return value

    def _old(self):
        """Does the store's own file hold the tables of the store before the databases?"""
        names = {r[0] for r in self.db.execute("SELECT name FROM sqlite_master WHERE type='table'")}
        return {"function", "candidate", "verdict"} <= names

    def carry(self, log=None):
        """The rows of the store from before the databases, copied into the modules' databases (once;
        then only rows written there since, by an older checkout). -> {modules, functions,
        candidates, verdicts, ...} carried this time."""
        if self.readonly or not self._old():
            return {}
        names = {r[0] for r in self.db.execute("SELECT name FROM sqlite_master WHERE type='table'")}
        since = float(self._about("carried_upto") or 0)
        newest = since
        for t in ("candidate", "verdict", "text", "identified", "variant"):
            if t in names:
                got = self.db.execute("SELECT MAX(ts) FROM %s" % t).fetchone()[0]
                newest = max(newest, got or 0)
        if self._about("carried") is not None and newest <= since:
            return {}
        q = {"function": "SELECT DISTINCT module FROM function WHERE id IN (SELECT function FROM candidate WHERE ts > ?)",
             "verdict": "SELECT DISTINCT f.module FROM verdict v JOIN candidate c ON c.id=v.candidate "
                        "JOIN function f ON f.id=c.function WHERE v.ts > ?",
             "text": "SELECT DISTINCT module FROM text WHERE ts > ?",
             "identified": "SELECT DISTINCT module FROM identified WHERE ts > ?",
             "variant": "SELECT DISTINCT f.module FROM variant v JOIN function f ON f.id=v.function WHERE v.ts > ?"}
        modules = set()
        if self._about("carried") is None:
            modules = {r[0] for r in self.db.execute("SELECT DISTINCT module FROM function")}
            modules |= {r[0] for r in self.db.execute("SELECT sha FROM module")}
        else:
            for t, sql in q.items():
                if t in names:
                    modules |= {r[0] for r in self.db.execute(sql, (since,))}
        say = log or (lambda s: None)
        if self._about("carried") is None and modules:
            import sys
            say = log or (lambda s: sys.stderr.write(s + "\n"))
            say("the function store: carrying %d modules' rows into their databases (%s), once"
                % (len(modules), self.folder))
        total = {}
        t0 = time.time()
        for i, sha in enumerate(sorted(modules)):
            for k, n in self._carry_module(sha, since, names).items():
                total[k] = total.get(k, 0) + n
            if i and i % 200 == 0:
                say("  %d of %d modules (%.0fs)" % (i, len(modules), time.time() - t0))
        total["modules"] = len(modules)
        self._about("carried_upto", repr(newest))
        if self._about("carried") is None:
            self._about("carried", json.dumps(dict(total, at=time.time())))
            say("the function store: carried %s (%.0fs)" % (", ".join("%d %s" % (v, k) for k, v in sorted(total.items())),
                                                           time.time() - t0))
        return total

    def _carry_module(self, sha, since, names):
        """One module's rows newer than `since`, from the old tables into its database. A row the
        database already has newer is left; a verdict carried is marked carried."""
        src = self.db
        out = {}
        m = self._m(sha)
        with self._write(sha) as db:
            fid = {}
            for oid, addr, name, size in src.execute("SELECT id, addr, name, size FROM function WHERE module=?", (sha,)):
                db.execute("INSERT OR IGNORE INTO function(module, addr, name, size) VALUES(?,?,?,?)", (sha, addr, name, size))
                fid[oid] = None
            new_id = dict(db.execute("SELECT addr, id FROM function WHERE module=?", (sha,)).fetchall())
            addr_of = dict(src.execute("SELECT id, addr FROM function WHERE module=?", (sha,)).fetchall())
            out["functions"] = len(addr_of)
            cid = {}
            n = 0
            for oc, f, engine, rev, sig, name, source, problem, ts in src.execute(
                    "SELECT c.id, c.function, c.engine, c.rev, c.sig, c.name, c.source, c.problem, c.ts FROM candidate c "
                    "JOIN function f ON f.id=c.function WHERE f.module=?", (sha,)):
                nf = new_id[addr_of[f]]
                have = db.execute("SELECT id, ts FROM candidate WHERE function=? AND engine=?", (nf, engine)).fetchone()
                if have is None:
                    cid[oc] = db.execute("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) "
                                         "VALUES(?,?,?,?,?,?,?,?)", (nf, engine, rev, sig, name, source, problem, ts)).lastrowid
                    n += 1
                else:
                    cid[oc] = have[0]
                    if (ts or 0) > (have[1] or 0) and (ts or 0) > since:
                        db.execute("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? WHERE id=?",
                                   (rev, sig, name, source, problem, ts, have[0]))
                        n += 1
            out["candidates"] = n
            n = 0
            for oc, profile, sig, verdict, reason, limits, detail, ts in src.execute(
                    "SELECT v.candidate, v.profile, v.sig, v.verdict, v.reason, v.limits, v.detail, v.ts FROM verdict v "
                    "JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function WHERE f.module=?", (sha,)):
                have = db.execute("SELECT ts FROM verdict WHERE candidate=? AND profile=?", (cid[oc], profile)).fetchone()
                if have is None or ((ts or 0) > (have[0] or 0) and (ts or 0) > since):
                    db.execute("INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, "
                               "carried) VALUES(?,?,?,?,?,?,?,?,1)", (cid[oc], profile, sig, verdict, reason, limits, detail, ts))
                    n += 1
            out["verdicts"] = n
            if "text" in names:
                rows = src.execute("SELECT engine, kind, body, ts FROM text WHERE module=?", (sha,)).fetchall()
                for engine, kind, body, ts in rows:
                    have = db.execute("SELECT ts FROM text WHERE module=? AND engine=? AND kind=?", (sha, engine, kind)).fetchone()
                    if have is None or (ts or 0) > (have[0] or 0):
                        db.execute("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)",
                                   (sha, engine, kind, body, ts))
                out["texts"] = len(rows)
            if "identified" in names:
                rows = src.execute("SELECT addr, name, library, ts FROM identified WHERE module=?", (sha,)).fetchall()
                newest = db.execute("SELECT MAX(ts) FROM identified WHERE module=?", (sha,)).fetchone()[0]
                if rows and (newest is None or max(r[3] or 0 for r in rows) > newest):
                    db.execute("DELETE FROM identified WHERE module=?", (sha,))
                    db.executemany("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)",
                                   [(sha,) + tuple(r) for r in rows])
                out["identified"] = len(rows)
            if "built" in names:
                rows = src.execute("SELECT addr, profile, sig FROM built WHERE module=?", (sha,)).fetchall()
                db.executemany("INSERT OR IGNORE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)",
                               [(sha,) + tuple(r) for r in rows])
                out["built"] = len(rows)
            if "variant" in names:
                rows = src.execute("SELECT v.id, v.function, v.compiler, v.source, v.parent, v.knobs, v.distance, v.total, "
                                   "v.verdict, v.behaves, v.ts FROM variant v JOIN function f ON f.id=v.function "
                                   "WHERE f.module=? ORDER BY v.id", (sha,)).fetchall()
                vid = {}
                for ov, f, compiler, source, parent, knobs, distance, total, verdict, behaves, ts in rows:
                    nf = new_id[addr_of[f]]
                    db.execute("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, "
                               "behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?) ON CONFLICT(function, compiler, knobs) DO UPDATE SET "
                               "source=excluded.source, distance=excluded.distance, total=excluded.total, "
                               "verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts WHERE excluded.ts > variant.ts",
                               (nf, compiler, source, None, knobs, distance, total, verdict, behaves, ts))
                    vid[ov] = (db.execute("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?",
                                          (nf, compiler, knobs)).fetchone()[0], parent)
                for ov, (nv, parent) in vid.items():                  # (a variant's parent, by its new number)
                    if parent in vid:
                        db.execute("UPDATE variant SET parent=? WHERE id=?", (vid[parent][0], nv))
                out["variants"] = len(rows)
        del m, fid
        self._summarize(sha)
        return out


def open(path=None, readonly=False):                    # noqa: A001 (as sqlite3.connect / shelve.open)
    """The function store (see the module's text). Read-only: None when there is no store yet."""
    if readonly and not os.path.isfile(path or default_path()):
        return None
    return FunctionStore(path, readonly=readonly)
