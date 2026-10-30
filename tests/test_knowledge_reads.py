#!/usr/bin/env python3
"""Tests for the knowledge base's pieces as inputs of the program databases' facts
(knowledge/reads.py, engine/engine.py): a step that reads a prototype, an argument size or a
body's name is made again when that piece changes, and only such a step.
Run:  python tests/test_knowledge_reads.py     (exit code 0 = all passed)

Each test is on a knowledge base and a made-up binary in a folder of its own (CB_KNOWLEDGE_DB),
with steps of its own (kinds t.kb.*): no headers, no corpus, seconds."""
import os
import shutil
import sys
import tempfile
import threading

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker import knowledge   # noqa: E402
from codebreaker.engine import engine   # noqa: E402
from codebreaker.knowledge import reads, store   # noqa: E402
from codebreaker.symbols import api_arguments, prototypes   # noqa: E402

RESULTS = []
MADE = {}
LOCK = threading.Lock()


def made(kind, key):
    with LOCK:
        MADE.setdefault(kind, []).append(key)


def taken():
    with LOCK:
        out = {k: sorted(v) for k, v in MADE.items()}
        MADE.clear()
    return out


# --- the steps: each reads the base through the code the real steps use -------------------------
@engine.step("t.kb.params", version="1")
def _params(ctx, key):
    """How many parameters the headers give a function (symbols/prototypes.of)."""
    made("t.kb.params", key)
    p = prototypes.of(key, 32)
    return len(p["parameters"]) if p else None


@engine.step("t.kb.argc", version="1")
def _argc(ctx, key):
    """An import's argument count for the oracle's stub (api_arguments.argc: the headers' table)."""
    made("t.kb.argc", key)
    return api_arguments.argc("USER32", key)


@engine.step("t.kb.total", reads=("t.kb.params",), version="1")
def _total(ctx, key):
    """Made from other facts only: stale when they change, not when the base does."""
    made("t.kb.total", key)
    return sum(v or 0 for v in ctx.need_many("t.kb.params", key.split(",")).values())


@engine.step("t.kb.none", reads=("binary",), version="1")
def _none(ctx, key):
    made("t.kb.none", key)
    return ctx.need("binary")["size"]


@engine.step("t.kb.name", version="1")
def _name(ctx, key):
    """A body's name, from the name store."""
    made("t.kb.name", key)
    got = knowledge.name(key)
    return got[0] if got else None


def _worker_read(context, item):
    from codebreaker import knowledge as kb
    return kb.known("win32", "t.number", item)


@engine.step("t.kb.pooled", version="1")
def _pooled(ctx, key):
    """Reads made in jobs/pool's workers (threads, and pool.run's) are the step's."""
    from codebreaker.jobs import pool
    made("t.kb.pooled", key)
    a = pool.threads(lambda k: knowledge.known("win32", "t.number", k), [key + "1"], jobs=2)
    b = pool.run(_worker_read, [key + "2"], jobs=1)
    c = pool.run(_worker_read, [key + "3", key + "4"], jobs=2, small=1, below_normal=False)   # (processes)
    return [a[0], b[key + "2"], c[key + "3"]]


@engine.step("t.kb.judged", version="1", batch=True)
def _judged(ctxs, hint):
    """A batch step whose per-key work is handed out (as lane.K.verdict): each key's reads are its own."""
    from codebreaker.jobs import pool
    for k in ctxs:
        made("t.kb.judged", k)
    got = pool.run(reads.Carry(_worker_read), list(ctxs), jobs=1)
    return {k: reads.unwrap(got[k], ctxs[k].knowledge) for k in ctxs}


# --- the place ------------------------------------------------------------------------------------
class Place(object):
    """A knowledge base, a binary and its database, each in a folder gone afterwards; the headers'
    table seeded by hand (as prototypes.table() keeps it) instead of read from the headers."""

    def __init__(self):
        self.dir = tempfile.mkdtemp(prefix="cbkbreads")
        self.old = os.environ.get("CB_KNOWLEDGE_DB")
        os.environ["CB_KNOWLEDGE_DB"] = os.path.join(self.dir, "kb", "knowledge.sqlite")
        self.kb = store.open()
        self.kb.put_many("win32", "prototype", {
            "FindWindowA": {"returns": "HWND", "marker": "WINAPI", "parameters": [["LPCSTR", "c"], ["LPCSTR", "w"]]},
            "GetDlgItem": {"returns": "HWND", "marker": "WINAPI", "parameters": [["HWND", "h"], ["int", "id"]]}},
            "a test", store.DERIVED)
        self.saved = (prototypes._table, api_arguments._TABLE)
        t32 = self.kb.table("win32", "prototype")
        prototypes._table = {"32": t32, "16": {}, "spelling": {n.upper(): n for n in t32}}
        api_arguments._TABLE = None
        self.binary = os.path.join(self.dir, "thing.bin")
        with open(self.binary, "w") as f:
            f.write("a made-up binary")
        self.db = engine.open(self.binary, folder=os.path.join(self.dir, "db"))
        reads._FRESH.clear()
        taken()

    def close(self):
        self.db.close()
        self.kb.close()
        store._OPEN.pop(self.kb.path, None)
        prototypes._table, api_arguments._TABLE = self.saved
        reads._FRESH.clear()
        if self.old is None:
            os.environ.pop("CB_KNOWLEDGE_DB", None)
        else:
            os.environ["CB_KNOWLEDGE_DB"] = self.old
        shutil.rmtree(self.dir, ignore_errors=True)


def check(fn):
    p = Place()
    try:
        fn(p)
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        import traceback
        RESULTS.append((fn.__name__, "FAIL", "%s: %s\n%s" % (type(e).__name__, e, traceback.format_exc())))
    finally:
        p.close()


def _all(p):
    return {"FindWindowA": engine.get(p.db, "t.kb.params", "FindWindowA"),
            "GetDlgItem": engine.get(p.db, "t.kb.params", "GetDlgItem"),
            "total": engine.get(p.db, "t.kb.total", "FindWindowA,GetDlgItem"),
            "none": engine.get(p.db, "t.kb.none")}


# --- the tests ------------------------------------------------------------------------------------
def test_a_changed_prototype_remakes_only_what_read_it(p):
    assert _all(p) == {"FindWindowA": 2, "GetDlgItem": 2, "total": 4, "none": 16}
    assert taken() == {"t.kb.params": ["FindWindowA", "GetDlgItem"], "t.kb.total": ["FindWindowA,GetDlgItem"],
                       "t.kb.none": [""]}
    w = engine.why(p.db, "t.kb.params", "FindWindowA")
    assert [i[:2] for i in w["inputs"]] == [("kb", "win32:prototype:FindWindowA")], w["inputs"]
    assert w["inputs"][0][3] is True
    assert _all(p)["total"] == 4 and taken() == {}                   # nothing changed: all kept
    p.kb.put("win32", "prototype", "FindWindowA",                    # the headers say three now
             {"returns": "HWND", "marker": "WINAPI", "parameters": [["LPCSTR", "c"], ["LPCSTR", "w"], ["int", "x"]]},
             "a test", store.DERIVED)
    assert engine.why(p.db, "t.kb.params", "FindWindowA")["inputs"][0][3] is False
    assert _all(p) == {"FindWindowA": 3, "GetDlgItem": 2, "total": 5, "none": 16}
    assert taken() == {"t.kb.params": ["FindWindowA"], "t.kb.total": ["FindWindowA,GetDlgItem"]}
    p.kb.put("win32", "prototype", "GetDlgItem", p.kb.get("win32", "prototype", "GetDlgItem"), "again", store.DERIVED)
    assert _all(p)["total"] == 5 and taken() == {}                   # the same value again: nothing stale


def test_an_absent_piece_that_appears_is_a_change(p):
    assert engine.get(p.db, "t.kb.params", "SetWindowTextA") is None
    assert engine.why(p.db, "t.kb.params", "SetWindowTextA")["inputs"][0][2] == reads.ABSENT
    taken()
    assert engine.get(p.db, "t.kb.params", "SetWindowTextA") is None and taken() == {}
    p.kb.put("win32", "prototype", "SetWindowTextA",
             {"returns": "BOOL", "marker": "WINAPI", "parameters": [["HWND", "h"], ["LPCSTR", "s"]]}, "a test")
    prototypes._table["spelling"]["SETWINDOWTEXTA"] = "SetWindowTextA"
    assert engine.get(p.db, "t.kb.params", "SetWindowTextA") == 2
    assert taken() == {"t.kb.params": ["SetWindowTextA"]}


def test_argc_reads_go_through_the_same_records(p):
    """verify/lanes/win32's argc and c_text's argument sizes ask api_arguments: a changed size is
    a changed input."""
    knowledge._table("win32", "argument_bytes.header")               # (the authored table, into the test's base)
    p.kb.put("win32", "argument_bytes.header", "FindWindowA", 8, "a test")
    assert engine.get(p.db, "t.kb.argc", "FindWindowA") == 2
    assert engine.get(p.db, "t.kb.argc", "GetDlgItem") == api_arguments.argc("USER32", "GetDlgItem")
    w = engine.why(p.db, "t.kb.argc", "FindWindowA")
    assert ("kb", "win32:argument_bytes.header:FindWindowA") in [i[:2] for i in w["inputs"]], w["inputs"]
    taken()
    p.kb.put("win32", "argument_bytes.header", "FindWindowA", 12, "a test")
    assert engine.get(p.db, "t.kb.argc", "FindWindowA") == 3
    engine.get(p.db, "t.kb.argc", "GetDlgItem")
    assert taken() == {"t.kb.argc": ["FindWindowA"]}


def test_a_named_body_remakes_what_asked_its_name(p):
    assert engine.get(p.db, "t.kb.name", "body1") is None
    assert engine.get(p.db, "t.kb.name", "body2") is None
    taken()
    knowledge.name_body("body1", "LZCopy")
    assert engine.get(p.db, "t.kb.name", "body1") == "LZCopy"
    assert engine.get(p.db, "t.kb.name", "body2") is None
    assert taken() == {"t.kb.name": ["body1"]}


def test_reads_on_threads_and_in_workers_are_the_steps(p):
    assert engine.get(p.db, "t.kb.pooled", "n") == [None, None, None]
    keys = sorted(i[1] for i in engine.why(p.db, "t.kb.pooled", "n")["inputs"])
    assert keys == ["win32:t.number:n%d" % i for i in (1, 2, 3, 4)], keys
    taken()
    assert engine.get(p.db, "t.kb.pooled", "n") == [None, None, None] and taken() == {}
    knowledge.learn("win32", "t.number", "n2", 7)
    assert engine.get(p.db, "t.kb.pooled", "n") == [None, 7, None] and taken() == {"t.kb.pooled": ["n"]}
    knowledge.learn("win32", "t.number", "n4", 1)                   # (read in a worker process)
    assert engine.get(p.db, "t.kb.pooled", "n") == [None, 7, None] and taken() == {"t.kb.pooled": ["n"]}


def test_a_batch_steps_keys_keep_their_own_reads(p):
    assert engine.get_many(p.db, "t.kb.judged", ["a", "b"]) == {"a": None, "b": None}
    assert [i[1] for i in engine.why(p.db, "t.kb.judged", "a")["inputs"]] == ["win32:t.number:a"]
    taken()
    knowledge.learn("win32", "t.number", "b", 1)
    assert engine.get_many(p.db, "t.kb.judged", ["a", "b"]) == {"a": None, "b": 1}
    assert taken() == {"t.kb.judged": ["b"]}


def test_outside_a_step_nothing_is_recorded(p):
    assert not reads.active()
    prototypes.of("FindWindowA")                                     # (no recording: nothing to do)
    with reads.recording() as outer:
        prototypes.of("FindWindowA")
        with reads.recording() as inner:
            prototypes.of("GetDlgItem")
    assert outer.keys == {"win32:prototype:FindWindowA"} and inner.keys == {"win32:prototype:GetDlgItem"}


def main():
    for fn in (test_a_changed_prototype_remakes_only_what_read_it, test_an_absent_piece_that_appears_is_a_change,
               test_argc_reads_go_through_the_same_records, test_a_named_body_remakes_what_asked_its_name,
               test_reads_on_threads_and_in_workers_are_the_steps, test_a_batch_steps_keys_keep_their_own_reads,
               test_outside_a_step_nothing_is_recorded):
        check(fn)
    for name, status, why in RESULTS:
        print("%-60s %s %s" % (name, status, why))
    bad = sum(1 for r in RESULTS if r[1] != "PASS")
    print("%d passed, %d failed" % (len(RESULTS) - bad, bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
