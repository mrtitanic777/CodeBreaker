#!/usr/bin/env python3
"""Tests for the knowledge base (codebreaker/knowledge): a kind made under a stamp and made again
when it changes, learned and given pieces kept through that, names that follow a body, an older
function store's bodies carried over, the authored files read through it, the emulator's
stand-ins as data, and the size rule leaving it alone.
Run:  python tests/test_knowledge.py     (exit code 0 = all passed)

Each test is on a knowledge base in a folder of its own (CB_KNOWLEDGE_DB): no corpus, seconds."""
import os
import shutil
import sqlite3
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker import knowledge   # noqa: E402
from codebreaker.engine import facts   # noqa: E402
from codebreaker.knowledge import sources, store   # noqa: E402
from codebreaker.store import functions as F   # noqa: E402

RESULTS = []


def check(fn):
    d = tempfile.mkdtemp()
    old = os.environ.get("CB_KNOWLEDGE_DB")
    os.environ["CB_KNOWLEDGE_DB"] = os.path.join(d, "db", "knowledge.sqlite")
    try:
        fn(d)
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))
    finally:
        if old is None:
            os.environ.pop("CB_KNOWLEDGE_DB", None)
        else:
            os.environ["CB_KNOWLEDGE_DB"] = old
        for kb in list(store._OPEN.values()):
            if kb.path.startswith(os.path.abspath(d)):
                kb.close()
                store._OPEN.pop(kb.path, None)
        shutil.rmtree(d, ignore_errors=True)


def test_a_kind_is_made_again_only_when_its_stamp_changes(d):
    kb = store.open()
    made = []

    def make(n):
        def m():
            made.append(n)
            return {"b": n, "a": n + 1}
        return m
    assert kb.ensure("win32", "t.kind", "s1", make(1), "test") == {"b": 1, "a": 2}
    assert list(kb.table("win32", "t.kind")) == ["b", "a"]          # the order it was made in
    kb.put("win32", "t.kind", "learned", 7, "found by hand")
    kb._ensured.clear()
    kb._tables.clear()
    kb.ensure("win32", "t.kind", "s1", make(5), "test")
    assert made == [1]                                              # the same stamp: not made again
    kb.ensure("win32", "t.kind", "s2", make(5), "test")
    assert made == [1, 5]
    assert kb.table("win32", "t.kind") == {"b": 5, "a": 6, "learned": 7}    # what was learned stays
    again = store.KnowledgeBase(kb.path)                            # another process
    assert again.get("win32", "t.kind", "learned") == 7 and again.about("win32", "t.kind", "b")[1] == store.AUTHORED
    again.close()


def test_values_keep_their_shape(d):
    kb = store.open()
    v = {"structure": (8, [(0, 4, "left", "int4")], 4), "nothing": None}
    kb.put("win32", "t.shape", "x", v)
    kb._tables.clear()
    assert store.KnowledgeBase(kb.path).get("win32", "t.shape", "x") == v
    assert isinstance(store.KnowledgeBase(kb.path).get("win32", "t.shape", "x")["structure"], tuple)


def test_a_name_follows_the_body(d):
    kb = store.open()
    kb.add_module("m1", "a.dll", "pe")
    kb.add_module("m2", "b.dll", "pe")
    kb.set_bodies("m1", {"10": ("aaaa", "StrCopy", "export"), "20": ("bbbb", None, None)})
    kb.set_bodies("m2", {"70": ("aaaa", None, None), "80": ("bbbb", None, None)})
    assert knowledge.name("aaaa") == ("StrCopy", "the same body in a.dll")
    assert knowledge.name("bbbb") is None
    knowledge.name_body("bbbb", "ParseLine")
    assert knowledge.name("bbbb") == ("ParseLine", "named by hand")
    assert kb.names_from_bodies("m2") == {"70": ("StrCopy", "the same body in a.dll"), "80": ("ParseLine", "named by hand")}


def test_an_older_stores_bodies_are_carried_over(d):
    path = os.path.join(d, "functions.sqlite")
    db = sqlite3.connect(path)
    db.executescript("CREATE TABLE module(sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL);"
                     "CREATE TABLE body(module TEXT, addr TEXT, hash TEXT, name TEXT, source TEXT, PRIMARY KEY (module, addr));"
                     "CREATE TABLE body_name(hash TEXT PRIMARY KEY, name TEXT, source TEXT, ts REAL);")
    db.execute("INSERT INTO module VALUES('m1', 'a.dll', '', 'pe', 0)")
    db.execute("INSERT INTO body VALUES('m1', '10', 'cccc', NULL, NULL)")
    db.execute("INSERT INTO body_name VALUES('cccc', 'GivenLongAgo', 'named by hand', 1.0)")
    db.commit()
    db.close()
    fs = F.open(path)
    assert fs.knowledge.path == os.path.abspath(os.path.join(d, "db", "knowledge.sqlite"))
    assert fs.names_from_bodies("m1") == {"10": ("GivenLongAgo", "named by hand")}
    assert fs.knowledge.name("cccc") == ("GivenLongAgo", "named by hand")
    fs.close()


def test_authored_files_are_read_through_it(d):
    sigs = knowledge.signatures("pe")
    assert sigs and all(len(s) == 6 and isinstance(s[5], str) for s in sigs[:50])     # the library's label
    assert knowledge.table("win32", "argument_bytes.header").get("FindWindowA") == 8
    assert knowledge.table("windows", "ordinal_names")
    kb = store.open()
    stamp = kb.stamp("win32", "signature")
    assert stamp == sources.file_stamp(sources.AUTHORED[("win32", "signature")][0])
    assert kb.about("win32", "signature", "000000")[0].replace("\\", "/") == "codebreaker/symbols/runtime_signatures.json"


def test_the_stand_ins_are_data(d):
    from codebreaker.emulate import driver, windows16

    class Ctx(object):
        def alloc_heap(self):
            return 0x1234
    stub = windows16.default_stub
    assert stub({"api": "KERNEL.@15"}, None, None, Ctx()) == {"ax": 0x1234, "pop": 6}       # GlobalAlloc by number
    assert stub({"api": "KERNEL.GLOBALLOCK", "args_stack": [0x47]}, None, None, Ctx()) == {"ax": 0, "dx": 0x47, "pop": 2}
    assert stub({"api": "KERNEL._LLSEEK", "args_stack": [5, 6]}, None, None, Ctx()) == {"ax": 6, "dx": 0, "pop": 8}
    assert stub({"api": "USER.NONESUCH"}, None, None, Ctx()) == {"ax": 0}
    assert driver.answers_in("Get_Cur_Thread_Handle") == "edi" and driver.answers_in("Nonesuch") is None
    assert knowledge.api_model("win16", "GLOBALALLOC") == {"ax": "heap", "pop": 6}


def test_the_size_rule_leaves_it_alone(d):
    kb = store.open()
    kb.put("win32", "t.x", "k", "x" * 100000)
    other = facts.open_at(os.path.join(d, "db", "a.sqlite"))
    other.write("t", "", b"y" * 1000, step="t", version="1")
    found = [os.path.basename(p) for p, _s, _t in facts.databases(os.path.join(d, "db"))]
    assert found == ["a.sqlite"], found
    other.conn.close()
    facts._OPEN.pop(other.path, None)


def test_platform_names(d):
    assert [knowledge.platform(p) for p in ("ne", "pe", "le", "powerpc", 16, "32", "win16")] == \
        ["win16", "win32", "vxd", "xbox360", "win16", "win32", "win16"]


def main():
    for fn in (test_a_kind_is_made_again_only_when_its_stamp_changes, test_values_keep_their_shape,
               test_a_name_follows_the_body, test_an_older_stores_bodies_are_carried_over,
               test_authored_files_are_read_through_it, test_the_stand_ins_are_data, test_the_size_rule_leaves_it_alone,
               test_platform_names):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
