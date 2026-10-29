#!/usr/bin/env python3
"""Tests for the program database and the incremental engine (codebreaker/engine): provenance,
staleness, partial recompute, early cutoff, hand-given facts, side by side, the size rules.
Run:  python tests/test_engine.py     (exit code 0 = all passed)

Every test is on steps of its own (kinds named t.*) over a made-up binary in a folder of its own:
no toolchain, no corpus, seconds."""
import os
import shutil
import sys
import tempfile
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.engine import engine, facts   # noqa: E402

RESULTS = []
MADE = {}                       # kind -> [key, ...] as made, in order
LOCK = threading.Lock()


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def made(kind, key):
    with LOCK:
        MADE.setdefault(kind, []).append(key)


# A little pipeline: t.words (the binary's text split) -> t.word <i> -> t.upper <i> -> t.joined.
# t.length <i> depends on t.word <i> but gives the same value for words of the same length.
VERSION = {"t.upper": "1"}


@engine.step("t.words", reads=("binary",), version="1")
def _words(ctx, key):
    made("t.words", key)
    with open(ctx.need("binary")["path"]) as f:
        return f.read().split()


@engine.step("t.word", reads=("t.words",), version="1")
def _word(ctx, key):
    made("t.word", key)
    words = ctx.need("t.words")
    return words[int(key)] if int(key) < len(words) else None


@engine.step("t.upper", reads=("t.word",), version=None)
def _upper(ctx, key):
    made("t.upper", key)
    time.sleep(0.05)
    return (ctx.need("t.word", key) or "").upper()


engine.STEPS["t.upper"].version = lambda: VERSION["t.upper"]       # (a version the test can change)


@engine.step("t.length", reads=("t.word",), version="1")
def _length(ctx, key):
    made("t.length", key)
    return len(ctx.need("t.word", key) or "")


@engine.step("t.total", reads=("t.length", "t.words"), version="1")
def _total(ctx, key):
    made("t.total", key)
    n = len(ctx.need("t.words"))
    return sum(ctx.need_many("t.length", [str(i) for i in range(n)]).values())


@engine.step("t.joined", reads=("t.upper", "t.words"), version="1")
def _joined(ctx, key):
    made("t.joined", key)
    n = len(ctx.need("t.words"))
    return " ".join(ctx.need_many("t.upper", [str(i) for i in range(n)]).values())


@engine.step("t.sneaky", reads=("t.word",), version="1")
def _sneaky(ctx, key):
    return ctx.need("t.words")                                   # (not declared)


@engine.step("t.batch", reads=("t.word",), batch=True, version="1")
def _batch(ctxs, hint):
    made("t.batch", tuple(sorted(ctxs)))
    out = {}
    for k, ctx in ctxs.items():
        w = ctx.need("t.word", k)
        out[k] = engine.Unkept("?") if w == "skip" else "%s:%s" % (hint, w)
    return out


class Place(object):
    """A binary and a folder for its database, gone afterwards."""

    def __init__(self, text):
        self.dir = tempfile.mkdtemp(prefix="cbengine")
        self.binary = os.path.join(self.dir, "thing.bin")
        self.write(text)
        self.db = engine.open(self.binary, folder=os.path.join(self.dir, "db"))
        MADE.clear()

    def write(self, text):
        with open(self.binary, "w") as f:
            f.write(text)

    def close(self):
        self.db.close()
        shutil.rmtree(self.dir, ignore_errors=True)


def test_made_once_then_kept():
    p = Place("alpha beta gamma")
    try:
        assert engine.get(p.db, "t.joined") == "ALPHA BETA GAMMA"
        assert MADE["t.upper"] and sorted(MADE["t.upper"]) == ["0", "1", "2"]
        MADE.clear()
        assert engine.get(p.db, "t.joined") == "ALPHA BETA GAMMA"
        assert MADE == {}, MADE                                     # nothing made again
        assert engine.last_run(p.db)["t.joined"] == [1, 0]
    finally:
        p.close()


def test_provenance_is_recorded():
    p = Place("alpha beta")
    try:
        engine.get(p.db, "t.joined")
        w = engine.why(p.db, "t.upper", "1")
        assert w["step"] == "t.upper" and w["version"] == VERSION["t.upper"]
        assert [(k, key) for k, key, _h, _s in w["inputs"]] == [("t.word", "1")]
        assert all(same for *_x, same in w["inputs"])
        assert w["inputs"][0][2] == p.db.row("t.word", "1").hash
        j = engine.why(p.db, "t.joined")
        assert {(k, key) for k, key, _h, _s in j["inputs"]} == {("t.words", ""), ("t.upper", "0"), ("t.upper", "1")}
        assert p.db.row("t.words").inputs[0][:2] == ("binary", "")
    finally:
        p.close()


def test_a_given_fact_recomputes_only_what_depends_on_it():
    p = Place("alpha beta gamma delta")
    try:
        engine.get(p.db, "t.joined")
        engine.get(p.db, "t.total")
        MADE.clear()
        engine.put(p.db, "t.word", "2", "GAMMAS")                   # one function's C edited by hand
        assert engine.get(p.db, "t.joined") == "ALPHA BETA GAMMAS DELTA"
        assert MADE.get("t.upper") == ["2"], MADE                   # only that one, and the join
        assert MADE.get("t.joined") == [""] and "t.word" not in MADE and "t.words" not in MADE
        assert engine.get(p.db, "t.total") == 5 + 4 + 6 + 5
        assert MADE.get("t.length") == ["2"]
        MADE.clear()
        engine.forget(p.db, "t.word", "2")                          # back to what its step makes
        assert engine.get(p.db, "t.joined") == "ALPHA BETA GAMMA DELTA"
        assert MADE.get("t.word") == ["2"] and MADE.get("t.upper") == ["2"]
    finally:
        p.close()


def test_the_same_value_stops_the_change():
    p = Place("alpha beta")
    try:
        engine.get(p.db, "t.total")
        MADE.clear()
        engine.put(p.db, "t.word", "1", "gamm")                     # another word, the same length
        assert engine.get(p.db, "t.total") == 9
        assert MADE.get("t.length") == ["1"] and "t.total" not in MADE, MADE   # cut off at t.length
    finally:
        p.close()


def test_a_new_step_version_remakes_its_facts_and_drops_the_old():
    p = Place("alpha beta")
    try:
        engine.get(p.db, "t.joined")
        MADE.clear()
        VERSION["t.upper"] = "2"
        p.db.cleaned.clear()                                        # (a new process)
        try:
            assert engine.get(p.db, "t.joined") == "ALPHA BETA"
            assert sorted(MADE.get("t.upper", [])) == ["0", "1"]
            assert "t.joined" not in MADE                           # the same values: the join holds
            assert all(r.version == "2" for r in p.db.rows("t.upper"))
        finally:
            VERSION["t.upper"] = "1"
    finally:
        p.close()


def test_a_changed_binary_is_a_new_database():
    p = Place("alpha beta")
    try:
        engine.get(p.db, "t.joined")
        p.write("alpha beta gamma")
        db2 = engine.open(p.binary, folder=os.path.join(p.dir, "db"))
        assert db2.path != p.db.path
        assert engine.get(db2, "t.joined") == "ALPHA BETA GAMMA"
        db2.close()
    finally:
        p.close()


def test_stale_facts_are_made_side_by_side():
    p = Place(" ".join("w%d" % i for i in range(24)))
    try:
        old = os.environ.get("CB_LOW_LOAD")
        os.environ["CB_LOW_LOAD"] = "8"
        try:
            t = time.time()
            engine.get(p.db, "t.joined")
            took = time.time() - t
        finally:
            if old is None:
                os.environ.pop("CB_LOW_LOAD", None)
            else:
                os.environ["CB_LOW_LOAD"] = old
        assert len(MADE["t.upper"]) == 24
        assert took < 24 * 0.05 * 0.6, "took %.2fs: not side by side" % took    # (one at a time: 1.2s)
        os.environ["CB_LOW_LOAD"] = "1"
        try:
            assert engine.jobs() == 1
        finally:
            os.environ.pop("CB_LOW_LOAD", None)
            if old is not None:
                os.environ["CB_LOW_LOAD"] = old
    finally:
        p.close()


def test_a_step_reads_only_what_it_declares():
    p = Place("alpha")
    try:
        try:
            engine.get(p.db, "t.sneaky", "0")
        except LookupError as e:
            assert "does not declare" in str(e)
        else:
            raise AssertionError("an undeclared read went through")
    finally:
        p.close()


def test_batch_steps_and_unkept_values():
    p = Place("alpha skip gamma")
    try:
        r = engine.Run(p.db)
        got = r.many("t.batch", ["0", "1", "2"], hint="h")
        assert got == {"0": "h:alpha", "1": "?", "2": "h:gamma"}
        assert MADE["t.batch"] == [("0", "1", "2")]                 # one call for the lot
        assert p.db.row("t.batch", "1") is None                     # not kept
        MADE.clear()
        r = engine.Run(p.db)
        r.many("t.batch", ["0", "1", "2"], hint="h")
        assert MADE["t.batch"] == [("1",)]                          # only the one not kept
    finally:
        p.close()


def test_files_are_facts_too():
    p = Place("alpha")
    try:
        side = os.path.join(p.dir, "list.txt")
        with open(side, "w") as f:
            f.write("one")

        @engine.step("t.listed", reads=("file",), version="1")
        def _listed(ctx, key):
            made("t.listed", key)
            ctx.file(side)
            with open(side) as f:
                return f.read()
        assert engine.get(p.db, "t.listed") == "one"
        MADE.clear()
        assert engine.get(p.db, "t.listed") == "one" and "t.listed" not in MADE
        with open(side, "w") as f:
            f.write("two")
        assert engine.get(p.db, "t.listed") == "two" and MADE["t.listed"] == [""]
    finally:
        p.close()


def test_values_and_blobs():
    p = Place("alpha")
    try:
        big = os.urandom(facts.BLOB_AT * 3)
        for v in (b"\x00\x01", "text", {"a": [1, 2]}, ("a", 1), {1: "x"}, big, None):
            h = p.db.write("t.value", "k", v, step="t.value")
            r = p.db.row("t.value", "k")
            assert r.value() == v and r.hash == h, repr(v)[:40]
        p.db.write("t.value", "k", big, step="t.value")
        assert p.db.write("t.value", "k2", big, step="t.value") == p.db.row("t.value", "k").hash
        assert p.db.conn.execute("SELECT COUNT(*) FROM blob").fetchone()[0] == 1    # kept once
        p.db.forget("t.value")
        p.db.collect()
        assert p.db.conn.execute("SELECT COUNT(*) FROM blob").fetchone()[0] == 0
    finally:
        p.close()


def test_old_versions_and_the_cap():
    d = tempfile.mkdtemp(prefix="cbengine")
    try:
        db = facts.open_at(os.path.join(d, "a.sqlite"))
        db.write("t.x", "1", "old", step="t.x", version="1")
        db.write("t.x", "2", "new", step="t.x", version="2")
        db.write("t.x", "3", "mine", step=facts.GIVEN)
        assert db.drop_other_versions("t.x", "2") == 1
        assert [r.key for r in db.rows("t.x")] == ["2", "3"]
        db.write("t.y", "1", os.urandom(600000), step="t.y")       # (the largest, and the oldest)
        db.close()
        for name in ("b", "c"):                                     # two more, opened later
            other = facts.open_at(os.path.join(d, name + ".sqlite"))
            other.write("t.x", "1", os.urandom(200000), step="t.x")
            other.close()
            time.sleep(0.05)
        os.utime(os.path.join(d, "a.sqlite"), (time.time() - 100, time.time() - 100))
        sizes = {os.path.basename(p): s for p, s, _t in facts.databases(d)}
        size = sum(sizes.values())
        gone = facts.hold(cap=int((size - sizes["a.sqlite"]) / 0.8) + 1, folder=d)
        assert [os.path.basename(g) for g in gone] == ["a.sqlite"], gone     # opened longest ago first
    finally:
        shutil.rmtree(d, ignore_errors=True)


def main():
    for fn in (test_made_once_then_kept, test_provenance_is_recorded,
               test_a_given_fact_recomputes_only_what_depends_on_it, test_the_same_value_stops_the_change,
               test_a_new_step_version_remakes_its_facts_and_drops_the_old, test_a_changed_binary_is_a_new_database,
               test_stale_facts_are_made_side_by_side, test_a_step_reads_only_what_it_declares,
               test_batch_steps_and_unkept_values, test_files_are_facts_too, test_values_and_blobs,
               test_old_versions_and_the_cap):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
