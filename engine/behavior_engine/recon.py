#!/usr/bin/env python3
r"""recon.py -- whole-module reconstruction orchestration (spec Section 5/15/20).

The capstone that ties the engine together: reconstruct many functions in DEPENDENCY ORDER
(leaves first, then their callers), routing each to the right reconstructor and keeping the
verified results as a growing body of recovered source.

For each function it tries, in order of cleanliness:
    synth   -- pure integer leaf (1-2 args)                -> an expression / branch-free C
    struct  -- takes a pointer argument                    -> a struct + accessor body
    cfg     -- return gated by a scalar argument           -> an `if`
and records the first that succeeds (and whether the binary VERIFIED it). Functions that fit
none are honestly recorded as "unresolved" -- the evidence is kept; the reconstruction isn't
faked.

Dependency order comes from the observed call graph, so when later slices synthesize callers in
terms of already-recovered callees, the callees are ready first.

    wt behavior recon <module> [--limit N] [--trace N]
"""
import os
import sys

from engine.behavior_engine import tracer, graph, synth, corpus, globals as globmod
from engine.behavior_engine import struct as structmod
from engine.behavior_engine import cfg as cfgmod


def _topo_order(module, limit):
    """Functions leaf-first by the observed direct-call graph (callees before callers)."""
    g = graph.build(module)
    known = {int(k, 16) for k, v in g["nodes"].items() if v.get("kind") == "function"}
    succ = {}
    for e in g["edges"]:
        if e["kind"] in ("call_direct", "call_indirect") and e["to"].startswith("0x"):
            t = int(e["to"], 16)                     # API-name targets (externals) aren't hex; skip
            if t in known:
                succ.setdefault(int(e["from"], 16), set()).add(t)
    # order by number of intra-module callees (0 = leaf) ascending, then address
    traced = [int(k, 16) for k, v in g["nodes"].items()
              if v.get("kind") == "function" and "leaf" in v]
    traced.sort(key=lambda va: (len(succ.get(va, ())), va))
    return traced[:limit] if limit else traced


def reconstruct_module(module, limit=20, trace_n=0):
    if trace_n:
        graph.trace_sample(module, trace_n)
    order = _topo_order(module, limit)
    results = []
    for va in order:
        entry = {"func_va": va, "method": None, "verified": None, "summary": None, "c": None}
        try:
            s = synth.synthesize(module, func=hex(va), trials=20)
        except Exception as e:
            s = {"candidate": False, "reason": "synth error: %r" % e}
        if s.get("candidate") and s.get("candidates"):
            entry.update(method="synth", verified=bool((s.get("confirm") or {}).get("equivalent", True)),
                         summary=s["candidates"][0]["expr"], c=s.get("emitted_c"))
            results.append(entry); continue
        try:
            st = structmod.reconstruct(module, func=hex(va))
        except Exception:
            st = {"reconstructed": False}
        if st.get("reconstructed") and (st.get("out_fields") or st.get("in_fields")):
            entry.update(method="struct", verified=st.get("verified"),
                         summary="%d in / %d out fields via arg%d" %
                         (len(st["in_fields"]), len(st["out_fields"]), st["ptr_arg"]),
                         c=st.get("struct_c"))
            results.append(entry); continue
        try:
            cf = cfgmod.reconstruct_cfg(module, func=hex(va))
        except Exception:
            cf = {"reconstructed": False}
        if cf.get("reconstructed"):
            entry.update(method="cfg", verified=cf.get("verified"),
                         summary=cf.get("predicate"), c=cf.get("emitted_c"))
            results.append(entry); continue
        try:
            gl = globmod.reconstruct_globals(module, func=hex(va))
        except Exception:
            gl = {"reconstructed": False}
        if gl.get("reconstructed") and gl.get("writes"):
            entry.update(method="globals", verified=gl.get("verified"),
                         summary="%d global write(s)" % len(gl["writes"]), c=gl.get("c"))
            results.append(entry); continue
        entry["summary"] = (s.get("reason") or "no v1 reconstructor applies")
        results.append(entry)

    report = {"schema": "cb.behavior.recon/1", "module": os.path.basename(module),
              "attempted": len(results), "results": results}
    fp = corpus.record_derived("recon", {"module": report["module"], "func_va": 0, **report})
    report["_path"] = fp
    return report


# --------------------------------------------------------------------------
from engine.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    limit = int(_opt(argv, "--limit", 20))
    trace_n = int(_opt(argv, "--trace", 0))

    rep = reconstruct_module(module, limit=limit, trace_n=trace_n)
    by = {}
    for r in rep["results"]:
        by[r["method"] or "unresolved"] = by.get(r["method"] or "unresolved", 0) + 1
    print("module reconstruction: %s  (%d functions attempted, dependency order)" %
          (rep["module"], rep["attempted"]))
    print("  by method: %s" % ", ".join("%s=%d" % kv for kv in sorted(by.items())))
    ver = sum(1 for r in rep["results"] if r["verified"])
    print("  verified : %d\n" % ver)
    for r in rep["results"]:
        if not r["method"]:
            continue
        flag = "VERIFIED" if r["verified"] else "partial" if r["verified"] is False else "-"
        print("  0x%-9x %-7s %-8s %s" % (r["func_va"], r["method"], flag, r["summary"]))
    un = [r for r in rep["results"] if not r["method"]]
    print("\n  %d unresolved (evidence kept, no v1 reconstructor)" % len(un))
    print("  stored: %s" % rep["_path"])


if __name__ == "__main__":
    main()
