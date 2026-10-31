#!/usr/bin/env python3
"""Tests for the one verdict vocabulary (codebreaker/verify/verdicts.py): every older spelling read as
the word now, the classes as they always were, a store written before read in today's words, the
module writer's lookup by L2 provenance, and the producers speaking it.
Run:  python tests/test_verdicts.py     (exit code 0 = all passed)

No toolchain, no corpus, seconds."""
import os
import shutil
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.verify import verdicts as V   # noqa: E402

RESULTS = []
SHA = "c" * 64


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        import traceback
        RESULTS.append((fn.__name__, "FAIL", "%s: %s\n%s" % (type(e).__name__, e, traceback.format_exc())))


def _old_classify(verdict, limits=""):
    """store/functions.classify as it was before the vocabulary (2026-10-30), to hold the new one to."""
    weak_codes = ("fault-only", "low-coverage", "truncated", "unobserved", "writes-capped", "stub-callee",
                  "argc-unknown", "calls-capped", "methods-partial")
    codes = [c for c in (limits or "").split(",") if c]
    weak = [c for c in codes if c in weak_codes]
    if verdict == "equivalent":
        return ("weak" if weak else "verified"), weak
    if verdict == "consistent":
        return "weak", weak or ["fault-only"]
    return {"divergent": "unverified", "inconclusive": "not-judged", "no-compile": "no-compile"}.get(
        verdict, "no-source"), []


def test_the_classes_are_as_they_were():
    for v in ("equivalent", "consistent", "divergent", "inconclusive", "no-compile", "no-synth", "no-source",
              None, "", "something else"):
        for lim in ("", "low-coverage", "coverage=40,truncated", "paths=3,stub-callee,fault-only", "layout-differs"):
            assert V.classify(v, lim) == _old_classify(v, lim), (v, lim)


def test_every_older_spelling_is_read_as_now():
    assert [V.verdict(w) for w in ("same", "alike", "differs", "different", "not run", "not-run", "untested",
                                   "does not compile", "no-synth")] == [
        V.EQUIVALENT, V.EQUIVALENT, V.DIVERGENT, V.DIVERGENT, V.INCONCLUSIVE, V.INCONCLUSIVE, V.INCONCLUSIVE,
        V.NO_COMPILE, V.NO_SOURCE]
    assert all(V.verdict(w) == w for w in V.VERDICTS) and V.verdict(None) is None
    assert [V.klass(w) for w in ("VERIFIED (weak)", "not judged", "runtime library", "run-time library",
                                 "DOES NOT COMPILE", "verified")] == [
        V.WEAK, V.NOT_JUDGED, V.IDENTIFIED, V.IDENTIFIED, V.NO_COMPILE, V.VERIFIED]
    assert [V.match(w) for w in ("does not compile", "same length, different", "different length",
                                 "run-time library", "identical")] == [
        V.NO_COMPILE, V.SAME_SIZE, V.DIFFERENT, V.IDENTIFIED, V.IDENTICAL]
    assert V.pair(True) == V.EQUIVALENT and V.pair(False) == V.DIVERGENT and V.pair(True, ran=False) == V.INCONCLUSIVE


def test_each_class_has_one_title_and_one_label():
    for c in V.SCORED:
        assert c in V.TITLE and c in V.LABEL and V.klass(V.TITLE[c]) == c and V.klass(V.LABEL[c]) == c, c
    for v in V.VERDICTS:
        assert v in V.SAID and v in V.CLASS
    for m in V.MATCHES:
        assert m in V.MATCH_LABEL and V.match(V.MATCH_LABEL[m]) == m, m
    from codebreaker.store import functions as F
    from codebreaker.ui import terminal
    assert F.ORDER == V.ORDER and F.TITLE is V.TITLE and F.WEAK is V.WEAK_LIMITS and F.classify is V.classify
    assert terminal.TITLE is V.LABEL and set(terminal.VERDICTS) == set(V.SCORED)
    assert set(terminal.COLOR) == set(V.SCORED) == set(terminal.MARK)


def test_a_store_written_before_reads_in_todays_words():
    from codebreaker.store import functions as F
    d = tempfile.mkdtemp(prefix="cbverdicts")
    try:
        fs = F.open(os.path.join(d, "functions.sqlite"))
        fs.add_module(SHA, "c.dll", "C:/c.dll", "pe")
        fs.add_profile("p1", "{}")
        fs.add_functions(SHA, ["00401000", "00401010", "00401020"])
        fs.record_many(SHA, "decompiler", "1", "p1", [
            dict(addr="00401000", sig="s0", name="f0", source="int f0(void){return 0;}", verdict="equivalent"),
            dict(addr="00401010", sig="s1", name="f1", source=None, verdict="no-synth"),     # (an older producer)
            dict(addr="00401020", sig="s2", name="f2", source="int f2(void){return 1;}", verdict="same")])
        db = fs.module_db(SHA)
        assert sorted(r[0] for r in db.execute("SELECT verdict FROM verdict")) == ["equivalent", "equivalent",
                                                                                  "no-source"]   # (normalised on write)
        db.execute("UPDATE verdict SET verdict='no-synth' WHERE sig='s1'")                  # (as a store from before has it)
        db.commit()
        rows = {r["addr"]: r for r in fs.candidates(SHA, "p1")}
        assert rows["00401010"]["verdict"] == V.NO_SOURCE and rows["00401020"]["verdict"] == V.EQUIVALENT
        assert fs.stored(SHA, "00401010")[0]["verdicts"][0]["verdict"] == V.NO_SOURCE
        assert fs.tally("p1", SHA) == dict(dict.fromkeys(F.ORDER, 0), verified=2, **{"no-source": 1})
        fs.record_variant(SHA, "00401000", "msc6", "int f0(void){return 0;}", 0, 10, "does not compile",
                          behaves="same")
        v = fs.variants(SHA, "msc6")[0]
        assert (v["verdict"], v["behaves"]) == (V.NO_COMPILE, V.EQUIVALENT)
    finally:
        shutil.rmtree(d, ignore_errors=True)


def test_the_module_writer_reads_a_verdict_by_provenance():
    rows = {"00401000": {"addr": "00401000", "class": V.VERIFIED}, "00401010": {"addr": "00401010", "class": V.WEAK}}
    ix = V.Index(SHA, rows)
    from codebreaker.translate import l2
    node = l2.function("RenamedLater", "int f(void){return 0;}", origin=ix.origin("00401010"))
    assert ix.at(node["origin"])["class"] == V.WEAK                  # (by module + address, not by the name)
    assert ix.at({"module": "d" * 64, "va": "00401010"}) is None and ix.at(None) is None
    assert V.of_row(SHA, {"addr": "00401000", "verdict": "no-synth"}).verdict == V.NO_SOURCE


def test_the_producers_speak_it():
    from codebreaker.verify import compare, lane_common, reconstruct
    from codebreaker.verify.lanes import win16, win32
    assert reconstruct.GOOD is V.GOOD and reconstruct.NOT_RUN is V.NOT_RUN
    assert win32.GOOD is V.GOOD and win16.NOT_RUN is V.NOT_RUN
    assert lane_common.judged(lambda info: (_ for _ in ()).throw(ValueError("x")))[0] == V.INCONCLUSIVE
    from codebreaker.verify.authored import match
    assert match._ORDER == V.MATCHES
    assert match.compare((b"\x90\xc3", bytes(2)), (b"\x90\xc3", bytes(2)))[0] == V.IDENTICAL
    assert match.compare((b"\x90\xc3", bytes(2)), (b"\x91\xc3", bytes(2)))[0] == V.SAME_SIZE
    assert compare.__name__


def main():
    for fn in (test_the_classes_are_as_they_were, test_every_older_spelling_is_read_as_now,
               test_each_class_has_one_title_and_one_label, test_a_store_written_before_reads_in_todays_words,
               test_the_module_writer_reads_a_verdict_by_provenance, test_the_producers_speak_it):
        check(fn)
    for name, status, why in RESULTS:
        print("%-60s %s %s" % (name, status, why))
    bad = sum(1 for r in RESULTS if r[1] != "PASS")
    print("%d passed, %d failed" % (len(RESULTS) - bad, bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
