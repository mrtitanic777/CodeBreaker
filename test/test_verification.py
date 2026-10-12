#!/usr/bin/env python3
"""Tests for the verification pipeline and its results database.
Run:  python test/test_verification.py     (exit code 0 = all passed)

The database tests are self-contained. The pipeline tests run real verification on two small
retail modules and are skipped when the corpus, OpenWatcom or the decompilers are absent."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.core import corpus, toolchain                      # noqa: E402
from src.targets.win95.verification import pipeline, store  # noqa: E402

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


def test_pe_pipeline_is_incremental():
    _need("pe")
    db = _db()
    p = corpus.retail("lz32.dll")
    first = pipeline.verify("pe", p, store=db, bank=False)
    t = pipeline.tally(first)
    # lz32 is nearly all 32->16 thunk stubs. 21 of 36 "verified" until call arguments, memory
    # writes and the stack were compared; the stubs pass their argument in CL, which the
    # reconstruction turns into a pushed argument, so 6 hold up today.
    assert t["equivalent"] >= 5, dict(t)
    assert not any(r["cached"] for r in first.values())
    assert all(r["reason"] for r in first.values() if r["verdict"] == "divergent")
    second = pipeline.verify("pe", p, store=db, bank=False)
    assert {k: r["verdict"] for k, r in second.items()} == {k: r["verdict"] for k, r in first.items()}
    assert all(r["cached"] for r in second.values() if r["verdict"] != "no-synth")
    assert db.summary("pe") == dict(t) and db.flips("pe") == []
    # a subset of functions gives the same verdicts for those functions
    fns = pipeline.functions("pe", p)[:6]
    part = pipeline.verify("pe", p, fns, bank=False)
    assert set(part) <= set(first)


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
               test_reason_for_divergence, test_pe_pipeline_is_incremental, test_ne_pipeline):
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
