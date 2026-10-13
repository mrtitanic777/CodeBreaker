#!/usr/bin/env python3
"""Tests for the verification pipeline and its results database.
Run:  python tests/test_verification.py     (exit code 0 = all passed)

The database tests are self-contained. The pipeline tests run real verification on two small
retail modules and are skipped when the corpus, OpenWatcom or the decompilers are absent."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus
from codebreaker.base import period_tools as toolchain                      # noqa: E402
from codebreaker.verify import pipeline
from codebreaker.verify import store  # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def _db():
    return store.open(os.path.join(tempfile.mkdtemp(), "results.sqlite"))


def test_store_roundtrip_and_reports():
    db = _db()
    assert db.get("pe", "sha", "1000") is None
    db.put_many([("pe", "a.dll", "sha", "1000", "f", "sig1", "divergent", "deps-differ", "b1"),
                 ("pe", "a.dll", "sha", "2000", "g", "sig2", "equivalent", "", "b1"),
                 ("ne", "n.dll", "nsha", "1:0010", "h", "sig3", "consistent", "", "b1")])
    row = db.get("pe", "sha", "1000")
    assert row["verdict"] == "divergent" and row["sig"] == "sig1" and row["prev_verdict"] is None
    assert set(db.get_many("pe", "sha")) == {"1000", "2000"}
    assert db.summary("pe") == {"divergent": 1, "equivalent": 1}
    assert db.summary("ne") == {"consistent": 1}
    assert db.reasons("pe") == {"deps-differ": 1}
    assert db.by_module("pe") == [("a.dll", 2, 1)]


def test_store_records_verdict_changes():
    db = _db()
    db.put("pe", "a.dll", "sha", "1000", "f", "sig1", "divergent", "ret-differs", "build1")
    assert db.flips("pe") == []
    db.put("pe", "a.dll", "sha", "1000", "f", "sig1", "divergent", "ret-differs", "build2")   # same verdict
    assert db.flips("pe") == []
    db.put("pe", "a.dll", "sha", "1000", "f", "sig2", "equivalent", "", "build3")
    assert db.flips("pe") == [("a.dll", "1000", "f", "divergent", "equivalent")]
    assert db.flips("pe", "build3") and db.flips("pe", "build2") == []
    # the change stays on record when a later build confirms the new verdict
    db.put("pe", "a.dll", "sha", "1000", "f", "sig2", "equivalent", "", "build4")
    assert db.flips("pe", "build3") == [("a.dll", "1000", "f", "divergent", "equivalent")]


def test_reason_for_divergence():
    a = {"ret": 1, "exit": "ret", "deps": ("K.x",)}
    assert pipeline.why_divergent([a], [dict(a, exit="fault")]) == "exit-differs"
    assert pipeline.why_divergent([a], [dict(a, deps=())]) == "deps-differ"
    assert pipeline.why_divergent([a], [dict(a, ret=2)]) == "ret-differs"
    assert pipeline.why_divergent([{"err": "X"}, a], [a, dict(a, ret=2)]) == "ret-differs"
    assert pipeline.why_inconclusive([{"err": "ValueError"}], [a]) == "retail: ValueError"


def _need(lane):
    if not (corpus.retail("lz32.dll") and toolchain.decompiler(lane) and toolchain.ow_tool("wlink")):
        raise Skip("needs the retail corpus, OpenWatcom and the decompilers")


def test_reconstruct_is_incremental():
    """One path for every decompiler: results land in the function store, a second run judges
    nothing again, and a later decompiler is asked only about what an earlier one left."""
    _need("pe")
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    p = corpus.retail("lz32.dll")
    log = []
    sha, digest = reconstruct.run(p, engines=("native",), fs=fs, log=log.append)
    first = fs.candidates(sha, digest, "native")
    t = fs.tally(digest, sha)
    # lz32 is nearly all 32->16 thunk stubs, which pass their argument in CL; few hold up.
    assert t["verified"] + t["weak"] >= 5, t
    assert sum(t.values()) == len(pipeline.functions("pe", p)), t
    assert all(r["reason"] for r in first if r["verdict"] == "divergent")
    assert all(r["source"] for r in first if r["verdict"] not in ("no-synth",))
    log[:] = []
    reconstruct.run(p, engines=("native",), fs=fs, log=log.append)
    assert "judged 0 now" in log[0], log
    assert fs.tally(digest, sha) == t
    # every function gets the best of its candidates, and a class
    best = fs.best(sha, digest)
    assert all(r["class"] in F.ORDER for r in best.values())


def test_function_store():
    from codebreaker.store import functions as F
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    fs.add_module("m1", "A.DLL", "x", "pe")
    fs.add_functions("m1", ["10", "20", "30"])
    fs.record("m1", "10", "core", "1", "s1", "f10", "int f10(){}", "p1", "equivalent", limits="coverage=90")
    fs.record("m1", "10", "native", "1", "s2", "f10", "int f10(void){}", "p1", "divergent", reason="ret-differs")
    fs.record("m1", "20", "native", "1", "s3", "f20", "int f20(void){}", "p1", "equivalent", limits="low-coverage,coverage=10")
    best = fs.best("m1", "p1", prefer=("core", "native"))
    assert best["10"]["engine"] == "core" and best["10"]["class"] == "verified"
    assert best["20"]["class"] == "weak" and best["20"]["weak"] == ["low-coverage"]
    assert fs.tally("p1", "m1") == {"verified": 1, "weak": 1, "unverified": 0, "not-judged": 0,
                                    "no-compile": 0, "no-source": 1}
    # another verifier profile sees nothing judged yet, and the old verdicts are still there
    assert fs.tally("p2", "m1")["no-source"] == 3
    fs.record("m1", "10", "core", "1", "s1", "f10", "int f10(){}", "p2", "divergent", reason="writes-differ")
    assert fs.best("m1", "p1")["10"]["class"] == "verified" and fs.best("m1", "p2")["10"]["class"] == "unverified"
    # a changed source invalidates what was judged for the old one
    fs.record("m1", "10", "core", "2", "s9", "f10", "int f10(){return 1;}", "p2", "equivalent")
    assert "10" not in fs.judged("m1", "p1", "core")
    assert [n for _s, n in fs.modules("p1")] == ["a.dll"]


def test_ne_pipeline():
    _need("ne")
    p = corpus.retail("msmixmgr.dll")
    if not p:
        raise Skip("msmixmgr.dll not in the corpus")
    db = _db()
    first = pipeline.verify("ne", p, store=db, bank=False)
    assert pipeline.tally(first)["consistent"] >= 8, dict(pipeline.tally(first))   # 10 today
    second = pipeline.verify("ne", p, store=db, bank=False)
    assert all(r["cached"] for r in second.values() if r["verdict"] != "no-synth")


def main():
    for fn in (test_store_roundtrip_and_reports, test_store_records_verdict_changes,
               test_reason_for_divergence, test_function_store, test_reconstruct_is_incremental,
               test_ne_pipeline):
        try:
            fn()
            RESULTS.append((fn.__name__, "PASS", ""))
        except Skip as s:
            RESULTS.append((fn.__name__, "SKIP", str(s)))
        except Exception as e:
            RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, str(e)[:500])))
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
