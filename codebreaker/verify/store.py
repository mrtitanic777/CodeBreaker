#!/usr/bin/env python3
r"""store.py -- the per-function results database for behavioural verification.

One row per function body: what it was verified as, why, and under which decompiler build. It
replaces the scattered progress records (CSV tallies, "swept" lists, per-run counters) with one
queryable place, and it is what makes re-verification INCREMENTAL: each row carries a signature of
everything the verdict depended on (the synthesised source, the bodies it links against, the oracle
and pipeline revisions). A later run that finds the same signature reuses the verdict instead of
compiling and tracing again, so after a decompiler fix only the bodies whose output actually
changed are re-run.

    from codebreaker.verify import store
    db = store.open()                         # state/results.sqlite
    db.get("pe", mod_sha, "bff82941")         # -> row dict or None
    db.put("pe", module, mod_sha, key, fname, sig, verdict, reason, build)
    db.summary("pe")                          # {verdict: count}
    db.reasons("pe")                          # {reason: count} for the bodies that did not verify
    db.flips("pe", build)                     # bodies whose verdict changed under `build`

`put` keeps the previous verdict when it changes, so a decompiler build's effect on the corpus can
be read straight off the table (which bodies it fixed, which it broke).
"""
import os
import sqlite3
import time

_SCHEMA = """
CREATE TABLE IF NOT EXISTS body(
    lane TEXT NOT NULL, mod_sha TEXT NOT NULL, key TEXT NOT NULL,
    module TEXT, fname TEXT, sig TEXT, verdict TEXT, reason TEXT, build TEXT, ts REAL,
    prev_verdict TEXT, changed_build TEXT,
    PRIMARY KEY (lane, mod_sha, key));
CREATE INDEX IF NOT EXISTS body_verdict ON body(lane, verdict);
CREATE INDEX IF NOT EXISTS body_module ON body(lane, module);
CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, lane TEXT, settings TEXT, ts REAL);
"""
_COLS = ("lane", "mod_sha", "key", "module", "fname", "sig", "verdict", "reason", "build", "ts",
         "prev_verdict", "changed_build", "limits", "profile", "detail")
# Columns added after the first version of the table; older databases gain them on open.
_ADDED = (("limits", "TEXT"), ("profile", "TEXT"), ("detail", "TEXT"))


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_RESULTS_DB") or settings.state("results.sqlite")


class Store(object):
    def __init__(self, path=None):
        self.path = path or default_path()
        # Several sweep workers write concurrently: WAL lets readers proceed, and a generous busy
        # timeout makes a writer wait for the lock instead of failing.
        self.db = sqlite3.connect(self.path, timeout=60.0)
        self.db.execute("PRAGMA journal_mode=WAL")
        self.db.execute("PRAGMA synchronous=NORMAL")
        self.db.executescript(_SCHEMA)
        have = {r[1] for r in self.db.execute("PRAGMA table_info(body)")}
        for col, typ in _ADDED:
            if col not in have:
                try:
                    self.db.execute("ALTER TABLE body ADD COLUMN %s %s" % (col, typ))
                except sqlite3.OperationalError:
                    pass                          # another worker added it first
        self.db.commit()

    def close(self):
        self.db.close()

    def get(self, lane, mod_sha, key):
        row = self.db.execute("SELECT %s FROM body WHERE lane=? AND mod_sha=? AND key=?"
                              % ",".join(_COLS), (lane, mod_sha, key)).fetchone()
        return dict(zip(_COLS, row)) if row else None

    def get_many(self, lane, mod_sha):
        """{key: row} for every recorded body of one module (one query instead of one per body)."""
        cur = self.db.execute("SELECT %s FROM body WHERE lane=? AND mod_sha=?" % ",".join(_COLS),
                              (lane, mod_sha))
        return {r[2]: dict(zip(_COLS, r)) for r in cur}

    def put_many(self, rows):
        """rows: iterable of (lane, module, mod_sha, key, fname, sig, verdict, reason, build
        [, limits, profile]). One transaction. A changed verdict moves the old one to prev_verdict."""
        now = time.time()
        with self.db:
            for row in rows:
                lane, module, mod_sha, key, fname, sig, verdict, reason, build = row[:9]
                limits = row[9] if len(row) > 9 else ""
                profile = row[10] if len(row) > 10 else ""
                detail = row[11] if len(row) > 11 else ""
                old = self.db.execute("SELECT verdict, prev_verdict, changed_build FROM body "
                                      "WHERE lane=? AND mod_sha=? AND key=?",
                                      (lane, mod_sha, key)).fetchone()
                prev, cb = (old[1], old[2]) if old else (None, None)
                if old and old[0] != verdict:
                    prev, cb = old[0], build
                self.db.execute(
                    "INSERT OR REPLACE INTO body(%s) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)" % ",".join(_COLS),
                    (lane, mod_sha, key, module, fname, sig, verdict, reason or "", build, now, prev, cb,
                     limits or "", profile or "", detail or ""))

    def put_profile(self, digest, lane, settings):
        """Record what a profile digest stands for (the settings a verdict was reached under)."""
        with self.db:
            self.db.execute("INSERT OR IGNORE INTO profile(digest, lane, settings, ts) VALUES(?,?,?,?)",
                            (digest, lane, settings, time.time()))

    def limits(self, lane, verdicts=("equivalent", "consistent")):
        """{limitation code: bodies carrying it} among the verified bodies, plus '' = none."""
        out = {}
        q = "SELECT limits FROM body WHERE lane=? AND verdict IN (%s)" % ",".join("?" * len(verdicts))
        for (lim,) in self.db.execute(q, (lane,) + tuple(verdicts)):
            codes = [c for c in (lim or "").split(",") if c and not c.startswith("coverage=")]
            for code in codes or [""]:
                out[code] = out.get(code, 0) + 1

    def coverage(self, lane, verdicts=("equivalent", "consistent")):
        """[coverage percent] of the verified bodies that recorded one."""
        out = []
        q = "SELECT limits FROM body WHERE lane=? AND verdict IN (%s)" % ",".join("?" * len(verdicts))
        for (lim,) in self.db.execute(q, (lane,) + tuple(verdicts)):
            for code in (lim or "").split(","):
                if code.startswith("coverage="):
                    out.append(int(code[9:]))
        return out
        return out

    def put(self, lane, module, mod_sha, key, fname, sig, verdict, reason="", build=""):
        self.put_many([(lane, module, mod_sha, key, fname, sig, verdict, reason, build)])

    # ---- reporting -----------------------------------------------------------------------
    def summary(self, lane, module=None):
        q, a = "SELECT verdict, COUNT(*) FROM body WHERE lane=?", [lane]
        if module:
            q += " AND module=?"; a.append(module)
        return dict(self.db.execute(q + " GROUP BY verdict", a).fetchall())

    def reasons(self, lane, verdict="divergent"):
        return dict(self.db.execute("SELECT reason, COUNT(*) FROM body WHERE lane=? AND verdict=? "
                                    "GROUP BY reason ORDER BY 2 DESC", (lane, verdict)).fetchall())

    def by_module(self, lane):
        """[(module, total, verified)] where verified = equivalent + consistent."""
        return self.db.execute(
            "SELECT module, COUNT(*), SUM(verdict IN ('equivalent','consistent')) FROM body "
            "WHERE lane=? GROUP BY module ORDER BY 2 DESC", (lane,)).fetchall()

    def flips(self, lane, build=None):
        """[(module, key, fname, prev_verdict, verdict)] for bodies whose verdict changed (under
        `build`, or under any build when None)."""
        q, a = ("SELECT module, key, fname, prev_verdict, verdict FROM body "
                "WHERE lane=? AND prev_verdict IS NOT NULL"), [lane]
        if build:
            q += " AND changed_build=?"; a.append(build)
        return self.db.execute(q + " ORDER BY module, key", a).fetchall()


def open(path=None):                                    # noqa: A001 (mirrors sqlite3/shelve naming)
    return Store(path)
