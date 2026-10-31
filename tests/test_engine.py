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


# --- a lane on the lanes' steps (engine/lanes.py): a made-up lane, judged in this process ---------
class _FakeModule(object):
    """What a lane's prepare gives: the functions' C, their keys, the header, the module's signature."""

    def __init__(self, words):
        self.order = ["f%d" % i for i in range(len(words))]
        self.texts = {"f%d" % i: "int f%d(void) { return %s; }" % (i, w) for i, w in enumerate(words)}
        self.key = {"f%d" % i: "%04x" % i for i in range(len(words))}
        self.all_keys = sorted(self.key.values()) + ["ffff"]            # (one the decompiler gave nothing for)
        self.header = "typedef int int4;"
        self.sig = "whole:" + " ".join(words)


def _fake_lane():
    """A lane module (verify/lanes' four calls) kept in sys.modules: prepare reads the binary's words,
    build 'compiles' each function to the digits of its body (so a comment leaves its code alone),
    judge says equivalent for an even key, divergent for an odd one, and notes each call."""
    import re
    import types
    from codebreaker.build import functions as builder
    lane = types.ModuleType("cb_test_fake_lane")
    lane.__file__ = os.path.abspath(__file__)
    lane.KIND, lane.PER_FUNCTION, lane.LIBRARY = "tfake", True, False
    lane.PROFILE = ["p1", "{}"]
    lane.JUDGED = []

    def profile(path):
        return tuple(lane.PROFILE)

    def prepare(path, work):
        with open(path) as f:
            return _FakeModule(f.read().split())

    def build(m, path, work):
        with open(os.path.join(work, "m.dll"), "w") as f:
            f.write("built")
        builder.OBJECTS[work] = {n: "".join(re.findall(r"\d", t.split("{", 1)[1].split("/*")[0]))
                                 for n, t in m.texts.items()}
        items = {n: (m.key[n], os.path.join(work, "m.dll")) for n in m.order if "x" not in m.texts[n]}
        return "context", items, {n: "it does not compile" for n in m.order if "x" in m.texts[n]}

    def judge(context, item):
        with LOCK:
            lane.JUDGED.append(item[0])
        assert context == "context" and os.path.isfile(item[1])
        return ("equivalent" if int(item[0], 16) % 2 == 0 else "divergent"), "", ["coverage=100"], ""
    lane.profile, lane.prepare, lane.build, lane.judge = profile, prepare, build, judge
    sys.modules[lane.__name__] = lane
    return lane


def _declared():
    """The lanes' steps declared for the made-up lane (kinds lane.tfake.*)."""
    from codebreaker.engine import lanes
    lane = sys.modules.get("cb_test_fake_lane") or _fake_lane()

    class P(object):
        name, id, lane = "tfake", "tfake", "cb_test_fake_lane"
        packager = type("Packager", (), {"tools": None})()

        def lane_module(self):
            return sys.modules["cb_test_fake_lane"]
    if "lane.tfake.rows" not in engine.STEPS:
        lanes._declare(P())
    return lane


def test_a_lane_on_the_lanes_steps():
    lane = _declared()
    p = Place("2 7 4 9")
    try:
        lane.JUDGED[:] = []
        got = engine.get(p.db, "lane.tfake.rows")
        rows = {r["addr"]: r for r in got["rows"]}
        assert sorted(lane.JUDGED) == ["0000", "0001", "0002", "0003"]
        assert rows["0000"]["verdict"] == "equivalent" and rows["0001"]["verdict"] == "divergent"
        assert rows["ffff"]["verdict"] == "no-source" and got["profile"] == "p1" and got["header"]
        assert sorted(got["built"]) == ["0000", "0001", "0002", "0003"]
        lane.JUDGED[:] = []
        assert engine.get(p.db, "lane.tfake.rows") == got and lane.JUDGED == []     # nothing judged again
        # a comment added to one function by hand: its code is the same, so no verdict is made again --
        # but the rows carry its new C and signature
        f = engine.get(p.db, "lane.tfake.function", "0002")
        engine.put(p.db, "lane.tfake.function", "0002", [f[0], f[1] + " /* by hand */"])
        again = engine.get(p.db, "lane.tfake.rows")
        assert lane.JUDGED == [] and engine.last_run(p.db)["lane.tfake.build"] == [0, 1]
        mine = {r["addr"]: r for r in again["rows"]}["0002"]
        assert mine["source"].endswith("/* by hand */") and mine["sig"] != rows["0002"]["sig"]
        # ... a change to its code: that function judged again (and only it); one that does not compile is not run
        engine.put(p.db, "lane.tfake.function", "0002", [f[0], f[1].replace("4", "6")])
        engine.put(p.db, "lane.tfake.function", "0003", ["f3", "int f3(void) { return x; }"])
        again = {r["addr"]: r for r in engine.get(p.db, "lane.tfake.rows")["rows"]}
        assert lane.JUDGED == ["0002"], lane.JUDGED
        assert again["0003"]["verdict"] == "no-compile" and again["0003"]["reason"] == "it does not compile"
        assert again["0003"]["problem"] == "compile"
        engine.forget(p.db, "lane.tfake.function", "0002")
        engine.forget(p.db, "lane.tfake.function", "0003")
        lane.JUDGED[:] = []
        assert engine.get(p.db, "lane.tfake.rows")["rows"] == got["rows"] and sorted(lane.JUDGED) == ["0002", "0003"]
        # the profile changed and the build's files gone: built again there, every function judged again
        lane.PROFILE[0] = "p2"
        lane.JUDGED[:] = []
        shutil.rmtree(os.path.join(os.path.splitext(p.db.path)[0] + ".work", "lane"), ignore_errors=True)
        got2 = engine.get(p.db, "lane.tfake.rows")
        assert got2["profile"] == "p2" and sorted(lane.JUDGED) == ["0000", "0001", "0002", "0003"]
        assert [r["verdict"] for r in got2["rows"]] == [r["verdict"] for r in got["rows"]]
    finally:
        lane.PROFILE[0] = "p1"
        p.close()


def main():
    for fn in (test_made_once_then_kept, test_provenance_is_recorded,
               test_a_given_fact_recomputes_only_what_depends_on_it, test_the_same_value_stops_the_change,
               test_a_new_step_version_remakes_its_facts_and_drops_the_old, test_a_changed_binary_is_a_new_database,
               test_stale_facts_are_made_side_by_side, test_a_step_reads_only_what_it_declares,
               test_batch_steps_and_unkept_values, test_files_are_facts_too, test_values_and_blobs,
               test_old_versions_and_the_cap, test_a_lane_on_the_lanes_steps):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
