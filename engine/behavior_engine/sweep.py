#!/usr/bin/env python3
r"""sweep.py -- reconstruction COVERAGE across a module (spec Section 16/17: the corpus as a
measured dataset).

Routes many functions through the reconstruction pipeline (synth -> struct -> cfg) and reports
how far the engine gets, broken down by outcome AND by the reason the rest are blocked. This is
the instrument that keeps deepening EVIDENCE-DRIVEN: it tells you which blocker is most common,
so the next capability you build is the one that actually moves the number -- not a guess.

Outcomes: synth | struct_verified | struct_partial | cfg_verified | unresolved.
Blocking reasons (for unresolved/partial): non-terminating (runaway loop under the current
environment), non-leaf (intra-module calls), multi-input-return, leaf-outside-grammar,
struct-gate/unmodeled, no-clean-return, opaque.

    wt behavior sweep <module> [--limit N]
"""
import collections
import os
import sys

from engine.behavior_engine import tracer, synth, corpus, globals as GL
from engine.behavior_engine import struct as S
from engine.behavior_engine import cfg as C


def classify(module, va, insn_cap=6000):
    """Return (outcome, reason|None) for one function."""
    try:
        base = tracer.trace_function(module, func=hex(va), insn_cap=insn_cap)
    except Exception as e:
        return "error", "trace:%r" % e
    try:
        s = synth.synthesize(module, func=hex(va), trials=16)
    except Exception:
        s = {"candidate": False, "reason": "synth-error"}
    if s.get("candidate") and s.get("candidates"):
        return "synth", None
    try:
        st = S.reconstruct(module, func=hex(va))
    except Exception:
        st = {"reconstructed": False}
    if st.get("reconstructed") and (st.get("out_fields") or st.get("in_fields")):
        return ("struct_verified", None) if st.get("verified") else \
               ("struct_partial", "struct-gate/unmodeled")
    try:
        cf = C.reconstruct_cfg(module, func=hex(va))
    except Exception:
        cf = {"reconstructed": False}
    if cf.get("reconstructed"):
        return "cfg_verified", None
    try:
        gl = GL.reconstruct_globals(module, func=hex(va))
    except Exception:
        gl = {"reconstructed": False}
    if gl.get("reconstructed") and gl.get("writes"):
        return ("globals_verified", None) if gl.get("verified") else ("globals_partial", "globals-unmodeled")
    # unresolved -> a coarse, actionable reason
    if base.get("truncated"):
        return "unresolved", "non-terminating"           # runaway loop under current environment
    if base.get("exit") != "ret":
        return "unresolved", "no-clean-return"
    if base.get("calls"):
        return "unresolved", "non-leaf-intramodule"
    r = (s.get("reason") or "").lower()
    if "multiple" in r or "does <=2" in r:
        return "unresolved", "multi-input-return"
    if "no expression" in r:
        return "unresolved", "leaf-outside-grammar"
    return "unresolved", "opaque"


def sweep(module, limit=30, insn_cap=6000):
    funcs = sorted(tracer.list_functions(module), key=lambda x: -x[2])[:limit]
    cat, reasons, examples = collections.Counter(), collections.Counter(), {}
    for off, va, size in funcs:
        c, why = classify(module, va, insn_cap)
        cat[c] += 1
        if why:
            reasons[why] += 1
            examples.setdefault(why, "0x%x" % va)
    report = {"schema": "cb.behavior.sweep/1", "module": os.path.basename(module),
              "func_va": 0, "attempted": sum(cat.values()),
              "outcomes": dict(cat), "reasons": dict(reasons), "examples": examples}
    corpus.record_derived("sweep", report)
    return report


from engine.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    limit = int(_opt(argv, "--limit", 30))
    rep = sweep(module, limit=limit)
    total = rep["attempted"]
    done = sum(rep["outcomes"].get(k, 0) for k in
               ("synth", "struct_verified", "cfg_verified", "globals_verified"))
    print("reconstruction sweep: %s  (%d functions)" % (rep["module"], total))
    print("  reconstructed & verified: %d  (%.0f%%)" % (done, 100.0 * done / max(total, 1)))
    for k, n in sorted(rep["outcomes"].items(), key=lambda kv: -kv[1]):
        print("    %-18s %d" % (k, n))
    if rep["reasons"]:
        print("  blocked by:")
        for k, n in sorted(rep["reasons"].items(), key=lambda kv: -kv[1]):
            print("    %-22s %d   e.g. 0x%s" % (k, n, rep["examples"][k].lstrip("0x")))


if __name__ == "__main__":
    main()
