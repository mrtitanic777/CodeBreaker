"""functions.py -- the function store: one place that knows every function and what was made of it.

Everything CodeBreaker does to a function ends up here, and everything that reports reads from
here: the module writer, the scoreboard, the tools that explain a failure.

    module       one original binary (by content hash)
    function     one function of a module (by address)
    candidate    one decompiler's reconstruction of a function: its source, or why it has none
    verdict      what the verifier found for a candidate, under one set of verifier settings
    text         something a decompiler's functions share, such as its header of types
    identified   a function recognised as a runtime library's (its name there, and the library)

Bodies and the names given to them -- the name store -- are the knowledge base's
(knowledge/store.py: body, body_name); the store's set_bodies, bodies, name_body,
names_from_bodies and shared_bodies are kept for their callers and go there. A store made
before carries its bodies over the first time it is opened.

A verdict is kept per verifier profile (a digest of every setting a verdict depends on), so
changing the verifier adds new verdicts beside the old ones instead of erasing them: nothing
has to be re-judged to see what a change did, and old results stay comparable.

    from codebreaker.store import functions
    fs = functions.open()                          # state/functions.sqlite
    fs.add_module(sha, "comctl32.dll", path, "pe")
    fs.record(sha, "bfee83ad", "core", rev, sig, "sub_0183AD", source, profile,
              "equivalent", limits="coverage=100")
    fs.best(sha, profile)                          # {address: the best candidate's row}
    fs.tally(profile, sha)                         # {class: count}
"""
import os
import sqlite3
import time

_SCHEMA = """
CREATE TABLE IF NOT EXISTS module(
    sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS function(
    id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,
    UNIQUE(module, addr));
CREATE TABLE IF NOT EXISTS candidate(
    id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,
    name TEXT, source TEXT, problem TEXT, ts REAL,
    UNIQUE(function, engine));
CREATE TABLE IF NOT EXISTS verdict(
    candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,
    limits TEXT, detail TEXT, ts REAL,
    PRIMARY KEY (candidate, profile));
CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS text(
    module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,
    PRIMARY KEY (module, engine, kind));
CREATE INDEX IF NOT EXISTS function_module ON function(module);
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

# How good a result is, best first. A candidate's class is what the module writer prints.
ORDER = ("verified", "weak", "unverified", "not-judged", "no-compile", "no-source")
TITLE = {"verified": "VERIFIED", "weak": "VERIFIED (weak)", "unverified": "UNVERIFIED",
         "not-judged": "NOT JUDGED", "no-compile": "DOES NOT COMPILE", "no-source": "NO SOURCE"}
# limits that make a match weaker than it looks
WEAK = {"fault-only": "the two sides only ever fail alike; neither returned normally",
        "low-coverage": "under half of the original's instructions ran",
        "truncated": "a run was cut off at the step limit",
        "unobserved": "some inputs could not be run on one side",
        "writes-capped": "too many memory writes to compare them all",
        "stub-callee": "a function it calls was a stand-in, not that function's reconstruction",
        "argc-unknown": "an API call was compared by name only",
        "calls-capped": "it makes more calls than are compared",
        "methods-partial": "only some of the class's methods could be run beside the original (java)"}


# A function recognised as a runtime library's is counted on its own, not as a failure of
# reconstruction -- unless it was also verified, which is the stronger statement and stays.
IDENTIFIED = "identified"
SCORED = ORDER[:2] + (IDENTIFIED,) + ORDER[2:]
TITLE[IDENTIFIED] = "IDENTIFIED (runtime library)"


def classify(verdict, limits=""):
    """(class, weak reasons) for one verdict row."""
    codes = [c for c in (limits or "").split(",") if c]
    weak = [c for c in codes if c in WEAK]
    if verdict == "equivalent":
        return ("weak" if weak else "verified"), weak
    if verdict == "consistent":
        return "weak", weak or ["fault-only"]
    return {"divergent": "unverified", "inconclusive": "not-judged", "no-compile": "no-compile"}.get(
        verdict, "no-source"), []


def overview(path=None):
    """The store at a glance, quickly (seconds, where the coverage ledger takes many minutes): per
    module {sha, name, kind, path, functions, identified, latest: {class: count}}, by each function's
    LATEST verdict under any judge -- what it last came to, not what the judge in force says (a
    change to the judge leaves every module unjudged until it is measured again)."""
    import sqlite3
    db = sqlite3.connect("file:%s?mode=ro" % (path or default_path()), uri=True, timeout=30.0)
    try:
        mods = {sha: {"sha": sha, "name": name, "kind": kind, "path": p, "functions": 0, "identified": 0,
                      "latest": dict.fromkeys(ORDER, 0)}
                for sha, name, p, kind in db.execute("SELECT sha, name, path, kind FROM module")}
        for sha, n in db.execute("SELECT module, COUNT(*) FROM function GROUP BY module"):
            if sha in mods:
                mods[sha]["functions"] = n
        for sha, n in db.execute("SELECT module, COUNT(*) FROM identified GROUP BY module"):
            if sha in mods:
                mods[sha]["identified"] = n
        for sha, verdict, limits, n in db.execute("""
                WITH last AS (
                  SELECT f.module AS module, v.verdict AS verdict, v.limits AS limits,
                         ROW_NUMBER() OVER (PARTITION BY f.id ORDER BY v.ts DESC) AS k
                  FROM function f JOIN candidate c ON c.function = f.id JOIN verdict v ON v.candidate = c.id)
                SELECT module, verdict, limits, COUNT(*) FROM last WHERE k = 1 GROUP BY module, verdict, limits"""):
            if sha in mods:
                cls = classify(verdict, limits)[0]
                mods[sha]["latest"][cls] += n
        for m in mods.values():                             # (functions with no verdict at all)
            m["latest"]["no-source"] += max(0, m["functions"] - sum(m["latest"].values()))
        return sorted(mods.values(), key=lambda m: m["name"] or "")
    finally:
        db.close()


def latest(module_sha, path=None):
    """{addr: class} by each function's latest verdict under any judge (see overview)."""
    import sqlite3
    db = sqlite3.connect("file:%s?mode=ro" % (path or default_path()), uri=True, timeout=30.0)
    try:
        return {addr: classify(verdict, limits)[0] for addr, verdict, limits in db.execute("""
            SELECT addr, verdict, limits FROM (
              SELECT f.addr AS addr, v.verdict AS verdict, v.limits AS limits,
                     ROW_NUMBER() OVER (PARTITION BY f.id ORDER BY v.ts DESC) AS k
              FROM function f JOIN candidate c ON c.function = f.id JOIN verdict v ON v.candidate = c.id
              WHERE f.module = ?) WHERE k = 1""", (module_sha,))}
    finally:
        db.close()


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_FUNCTIONS_DB") or settings.state("functions.sqlite")


class FunctionStore(object):
    def __init__(self, path=None):
        self.own_path = bool(path or os.environ.get("CB_FUNCTIONS_DB"))
        self.path = path or default_path()
        self.db = sqlite3.connect(self.path, timeout=60.0)     # several workers may write
        self.db.execute("PRAGMA journal_mode=WAL")
        self.db.execute("PRAGMA synchronous=NORMAL")
        self.db.executescript(_SCHEMA)
        self.db.commit()

    def close(self):
        self.db.close()

    # ---- writing -------------------------------------------------------------------------
    def add_module(self, sha, name, path="", kind=""):
        with self.db:
            self.db.execute("INSERT INTO module(sha, name, path, kind, ts) VALUES(?,?,?,?,?) "
                            "ON CONFLICT(sha) DO UPDATE SET name=excluded.name, path=excluded.path, "
                            "kind=excluded.kind", (sha, name.lower(), path, kind, time.time()))

    def add_profile(self, digest, settings):
        with self.db:
            self.db.execute("INSERT OR IGNORE INTO profile(digest, settings, ts) VALUES(?,?,?)",
                            (digest, settings, time.time()))

    def _function(self, module, addr, name=None, size=None):
        row = self.db.execute("SELECT id FROM function WHERE module=? AND addr=?", (module, addr)).fetchone()
        if row:
            return row[0]
        return self.db.execute("INSERT INTO function(module, addr, name, size) VALUES(?,?,?,?)",
                               (module, addr, name, size)).lastrowid

    def add_functions(self, module, addrs):
        """Make sure every address of a module is known, reconstructed or not."""
        with self.db:
            for addr in addrs:
                self._function(module, addr)

    def record_many(self, module, engine, rev, profile, rows):
        """rows: dicts with addr, name, source, sig, verdict and optionally reason, limits, detail,
        problem. One transaction. A candidate is replaced by a newer one from the same engine;
        its verdicts under other profiles stay as long as the source did not change."""
        now = time.time()
        with self.db:
            for r in rows:
                f = self._function(module, r["addr"], r.get("name"))
                old = self.db.execute("SELECT id, sig FROM candidate WHERE function=? AND engine=?",
                                      (f, engine)).fetchone()
                if old:
                    cid = old[0]
                    if old[1] != r.get("sig"):
                        self.db.execute("DELETE FROM verdict WHERE candidate=? AND profile<>?", (cid, profile))
                    self.db.execute("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? "
                                    "WHERE id=?", (rev, r.get("sig"), r.get("name"), r.get("source"),
                                                   r.get("problem"), now, cid))
                else:
                    cid = self.db.execute(
                        "INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) "
                        "VALUES(?,?,?,?,?,?,?,?)", (f, engine, rev, r.get("sig"), r.get("name"),
                                                    r.get("source"), r.get("problem"), now)).lastrowid
                self.db.execute(
                    "INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts) "
                    "VALUES(?,?,?,?,?,?,?,?)", (cid, profile, r.get("sig"), r["verdict"], r.get("reason") or "",
                                                r.get("limits") or "", r.get("detail") or "", now))

    def put_text(self, module, engine, kind, body):
        """A module-level text an engine's functions need, e.g. kind "header": its types and
        declarations."""
        with self.db:
            self.db.execute("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)",
                            (module, engine, kind, body, time.time()))

    def get_text(self, module, engine, kind):
        row = self.db.execute("SELECT body FROM text WHERE module=? AND engine=? AND kind=?",
                              (module, engine, kind)).fetchone()
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
        return [dict(zip(self._ROW, r)) for r in self.db.execute(q, args)]

    def built(self, module, profile):
        """{addr: signature of the compiled code a function's verdict under `profile` was reached
        for} -- its own object code and that of everything it reaches (verify/reconstruct.py)."""
        return dict(self.db.execute("SELECT addr, sig FROM built WHERE module=? AND profile=?", (module, profile)))

    def set_built(self, module, profile, sigs):
        with self.db:
            self.db.executemany("INSERT OR REPLACE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)",
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

    def addresses(self, module):
        return [r[0] for r in self.db.execute("SELECT addr FROM function WHERE module=? ORDER BY addr", (module,))]

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
        with self.db:
            self.db.execute("DELETE FROM identified WHERE module=?", (module,))
            self.db.executemany("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)",
                                [(module, a, n, lib, now) for a, (n, lib) in found.items()])

    def identified(self, module):
        """{addr: (name, library)}"""
        return {a: (n, lib) for a, n, lib in
                self.db.execute("SELECT addr, name, library FROM identified WHERE module=?", (module,))}

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
        with self.db:
            f = self._function(module, addr)
            self.db.execute(
                "INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, "
                "behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?) ON CONFLICT(function, compiler, knobs) DO UPDATE SET "
                "source=excluded.source, parent=excluded.parent, distance=excluded.distance, "
                "total=excluded.total, verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts",
                (f, compiler, source, parent, knobs, distance, total, verdict, behaves, now))
            return self.db.execute("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?",
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
        return [dict(zip(self._VROW, r)) for r in self.db.execute(q, args)]

    def best_variant(self, module, compiler):
        """{addr: the closest variant row} -- the one with the smallest byte distance that still
        behaves alike (or, if none behaves, the smallest distance), per function."""
        out = {}
        for r in self.variants(module, compiler):
            ok = r["behaves"] in ("equivalent", "consistent", "")
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
        ident = sum(1 for r in best.values() if r["verdict"] == "identical")
        same = sum(1 for r in best.values() if r["verdict"] == "same size")
        return len(best), ident, same, len(best) - ident - same

    def modules(self, profile=None):
        """[(sha, name)] of modules, limited to those judged under `profile` when given."""
        if profile is None:
            return self.db.execute("SELECT sha, name FROM module ORDER BY name").fetchall()
        return self.db.execute(
            "SELECT DISTINCT m.sha, m.name FROM module m JOIN function f ON f.module=m.sha "
            "JOIN candidate c ON c.function=f.id JOIN verdict v ON v.candidate=c.id "
            "WHERE v.profile=? ORDER BY m.name", (profile,)).fetchall()


def open(path=None):                                    # noqa: A001 (as sqlite3.connect / shelve.open)
    return FunctionStore(path)
