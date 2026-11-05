#!/usr/bin/env python3
"""Tests for the function store kept in the program databases (codebreaker/store/functions.py):
a module's rows go to its binary's database, the index answers across modules, a store from before
the databases is carried over (once, then only what was written there since) with its verdicts
marked carried, and the databases' cap leaves the verdicts.
Run:  python tests/test_store.py     (exit code 0 = all passed)

Every test is on a store in a folder of its own with made-up modules: no toolchain, no corpus, seconds."""
import json
import os
import shutil
import sqlite3
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.engine import facts   # noqa: E402
from codebreaker.store import functions as F   # noqa: E402

RESULTS = []
SHA_A, SHA_B = "a" * 64, "b" * 64

# the store's own file as it was before the databases (every module's rows in it)
OLD_SCHEMA = """
CREATE TABLE module(sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL);
CREATE TABLE function(id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,
    UNIQUE(module, addr));
CREATE TABLE candidate(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,
    name TEXT, source TEXT, problem TEXT, ts REAL, UNIQUE(function, engine));
CREATE TABLE verdict(candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,
    limits TEXT, detail TEXT, ts REAL, PRIMARY KEY (candidate, profile));
CREATE TABLE profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL);
CREATE TABLE text(module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,
    PRIMARY KEY (module, engine, kind));
CREATE TABLE identified(module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, library TEXT, ts REAL,
    PRIMARY KEY (module, addr));
CREATE TABLE built(module TEXT NOT NULL, addr TEXT NOT NULL, profile TEXT NOT NULL, sig TEXT,
    PRIMARY KEY (module, addr, profile));
CREATE TABLE variant(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, compiler TEXT NOT NULL, source TEXT,
    parent INTEGER, knobs TEXT, distance INTEGER, total INTEGER, verdict TEXT, behaves TEXT, ts REAL,
    UNIQUE(function, compiler, knobs));
"""


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _rows(addrs, verdicts):
    return [dict(addr=a, sig="s" + a, name="f_" + a, source="int f_%s(void) { return 0; }" % a, verdict=v,
                 limits="low-coverage" if v == "weak" else "")
            for a, v in zip(addrs, verdicts)]


def test_a_modules_rows_are_in_its_database():
    d = tempfile.mkdtemp(prefix="cbstore")
    try:
        fs = F.open(os.path.join(d, "functions.sqlite"))
        fs.add_module(SHA_A, "a.dll", "C:/a.dll", "pe")
        fs.add_profile("p1", "{}")
        fs.add_functions(SHA_A, ["00401000", "00401010", "00401020"])
        fs.record_many(SHA_A, "decompiler", "1", "p1", _rows(["00401000", "00401010"], ["equivalent", "divergent"]))
        fs.set_identified(SHA_A, {"00401020": ("_memset", "libc")})
        fs.put_text(SHA_A, "decompiler", "header", "typedef int int4;")
        assert fs.database_path(SHA_A) == os.path.join(d, "db", SHA_A + ".sqlite")
        assert os.path.isfile(fs.database_path(SHA_A))
        names = {r[0] for r in fs.db.execute("SELECT name FROM sqlite_master WHERE type='table'")}
        assert "verdict" not in names and "function" not in names, names     # (the index holds no module's rows)
        db = fs.module_db(SHA_A)
        assert db.execute("SELECT COUNT(*) FROM verdict WHERE carried=0").fetchone()[0] == 2
        assert fs.tally("p1", SHA_A) == dict(dict.fromkeys(F.ORDER, 0), verified=1, unverified=1, **{"no-source": 1})
        assert fs.scores("p1", SHA_A)[F.IDENTIFIED] == 1 and fs.get_text(SHA_A, "decompiler", "header")
        assert fs.modules("p1") == [(SHA_A, "a.dll")] and fs.modules("p2") == []
        assert fs.module_rows("pe") == [(SHA_A, "a.dll", "C:/a.dll", "pe")] and fs.module_path("a.dll") == "C:/a.dll"
        assert fs.latest_profile("pe") == "p1" and fs.stamps()[SHA_A][:2] == (3, 2)
        assert fs.recent()[0][:3] == ("a.dll", "pe", 2) and fs.verdicts_since(time.time() - 60) == 2
        assert [c["verdicts"][0]["verdict"] for c in fs.stored(SHA_A, "00401010")] == ["divergent"]
        fs.close()
        o = F.overview(os.path.join(d, "functions.sqlite"))
        assert o[0]["latest"]["verified"] == 1 and o[0]["latest"]["no-source"] == 1 and o[0]["identified"] == 1, o
        assert F.latest(SHA_A, os.path.join(d, "functions.sqlite")) == {"00401000": "verified", "00401010": "unverified"}
        ro = F.open(os.path.join(d, "functions.sqlite"), readonly=True)
        try:
            ro.module_db(SHA_A).execute("DELETE FROM verdict")
            raise AssertionError("a read-only store wrote")
        except sqlite3.OperationalError:
            pass
        assert ro.best(SHA_A, "p1")["00401000"]["class"] == "verified" and ro.addresses(SHA_B) == []
        ro.close()
        assert F.open(os.path.join(d, "none.sqlite"), readonly=True) is None
        assert F.databases_folder(os.path.join(d, "x.sqlite"), True) == os.path.join(d, "x.db")
    finally:
        shutil.rmtree(d, ignore_errors=True)


def _old_store(path):
    """A store from before the databases: modules A and B, a verdict each under two profiles, a
    variant with a parent, identified functions, a text, built signatures, a hand-named body."""
    db = sqlite3.connect(path)
    db.executescript(OLD_SCHEMA)
    t = time.time() - 1000
    for sha, name in ((SHA_A, "a.dll"), (SHA_B, "b.exe")):
        db.execute("INSERT INTO module VALUES(?,?,?,?,?)", (sha, name, "C:/" + name, "pe", t))
        for i, addr in enumerate(("00401000", "00401010", "00401020")):
            f = db.execute("INSERT INTO function(module, addr, name, size) VALUES(?,?,?,?)", (sha, addr, None, None)).lastrowid
            if i == 2:
                continue
            c = db.execute("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES"
                           "(?,?,?,?,?,?,?,?)", (f, "decompiler", "9", "s", "f%d" % i, "int f(void){return %d;}" % i, "", t)).lastrowid
            db.execute("INSERT INTO verdict VALUES(?,?,?,?,?,?,?,?)", (c, "p1", "s", "equivalent" if i == 0 else "divergent",
                                                                        "" if i == 0 else "ret-differs", "", "", t))
            db.execute("INSERT INTO verdict VALUES(?,?,?,?,?,?,?,?)", (c, "p0", "s", "inconclusive", "", "", "", t - 10))
            db.execute("INSERT INTO built VALUES(?,?,?,?)", (sha, addr, "p1", "obj%d" % i))
        db.execute("INSERT INTO identified VALUES(?,?,?,?,?)", (sha, "00401020", "_strlen", "libc", t))
        db.execute("INSERT INTO text VALUES(?,?,?,?,?)", (sha, "decompiler", "header", "/* %s */" % name, t))
    f = db.execute("SELECT id FROM function WHERE module=? AND addr='00401000'", (SHA_A,)).fetchone()[0]
    v1 = db.execute("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, behaves, ts) "
                    "VALUES(?,?,?,?,?,?,?,?,?,?)", (f, "msc6", "x", None, "", 4, 10, "different", "equivalent", t)).lastrowid
    db.execute("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, behaves, ts) "
               "VALUES(?,?,?,?,?,?,?,?,?,?)", (f, "msc6", "y", v1, "loop", 0, 10, "identical", "equivalent", t))
    db.execute("INSERT INTO profile VALUES('p1','{}',?)", (t,))
    db.commit()
    db.close()


def test_an_older_store_is_carried_over_once():
    d = tempfile.mkdtemp(prefix="cbstore")
    try:
        path = os.path.join(d, "functions.sqlite")
        _old_store(path)
        fs = F.open(path)
        carried = json.loads(fs._about("carried"))
        assert (carried["modules"], carried["functions"], carried["candidates"], carried["verdicts"],
                carried["variants"], carried["identified"]) == (2, 6, 4, 8, 2, 2), carried
        assert fs.tally("p1", SHA_A) == dict(dict.fromkeys(F.ORDER, 0), verified=1, unverified=1, **{"no-source": 1})
        assert fs.built(SHA_B, "p1") == {"00401000": "obj0", "00401010": "obj1"} and fs.identified(SHA_A)
        assert fs.get_text(SHA_B, "decompiler", "header") == "/* b.exe */"
        v = {r["knobs"]: r for r in fs.variants(SHA_A, "msc6")}
        ids = dict(fs.module_db(SHA_A).execute("SELECT knobs, id FROM variant").fetchall())
        assert v["loop"]["parent"] == ids[""] and fs.match_tally(SHA_A, "msc6") == (1, 1, 0, 0)
        db = fs.module_db(SHA_A)
        assert db.execute("SELECT COUNT(*), SUM(carried) FROM verdict").fetchone() == (4, 4)
        assert [x["carried"] for x in fs.stored(SHA_A, "00401000")[0]["verdicts"]] == [True, True]
        assert {m["name"]: m["latest"]["verified"] for m in F.overview(path)} == {"a.dll": 1, "b.exe": 1}
        # a new verdict is not carried; the carry-over does not run again ...
        fs.record_many(SHA_A, "decompiler", "10", "p2", _rows(["00401020"], ["equivalent"]))
        assert db.execute("SELECT COUNT(*) FROM verdict WHERE carried=0").fetchone()[0] == 1
        fs.close()
        assert F.open(path).carry() == {}
        # ... except for what an older checkout wrote in the old tables since
        old = sqlite3.connect(path)
        c = old.execute("SELECT c.id FROM candidate c JOIN function f ON f.id=c.function WHERE f.module=? AND f.addr="
                        "'00401010'", (SHA_B,)).fetchone()[0]
        old.execute("UPDATE verdict SET verdict='equivalent', reason='', ts=? WHERE candidate=? AND profile='p1'",
                    (time.time(), c))
        old.commit()
        old.close()
        fs = F.open(path)
        assert fs.tally("p1", SHA_B)["verified"] == 2 and fs.tally("p1", SHA_A)["verified"] == 1
        fs.close()
    finally:
        shutil.rmtree(d, ignore_errors=True)


def test_the_cap_leaves_the_verdicts():
    d = tempfile.mkdtemp(prefix="cbstore")
    try:
        fs = F.open(os.path.join(d, "functions.sqlite"))
        fs.add_module(SHA_A, "a.dll", "", "pe")
        fs.record_many(SHA_A, "decompiler", "1", "p1", _rows(["00401000"], ["equivalent"]))
        p = fs.database_path(SHA_A)
        fs.close()
        db = facts.open_at(p)
        db.write("t.big", "", os.urandom(200000), step="t.big")
        db.close()
        other = facts.open_at(os.path.join(d, "db", "c" * 64 + ".sqlite"))
        other.write("t.big", "", os.urandom(200000), step="t.big")
        other.close()
        os.utime(p, (time.time() - 100, time.time() - 100))
        gone = facts.hold(cap=1000, folder=os.path.join(d, "db"))
        assert len(gone) == 2 and os.path.isfile(p) and not os.path.isfile(os.path.join(d, "db", "c" * 64 + ".sqlite"))
        conn = sqlite3.connect(p)
        assert conn.execute("SELECT COUNT(*) FROM fact").fetchone()[0] == 0
        assert conn.execute("SELECT COUNT(*) FROM verdict").fetchone()[0] == 1
        conn.close()
    finally:
        shutil.rmtree(d, ignore_errors=True)


def main():
    for fn in (test_a_modules_rows_are_in_its_database, test_an_older_store_is_carried_over_once,
               test_the_cap_leaves_the_verdicts):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
