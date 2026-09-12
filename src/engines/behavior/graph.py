#!/usr/bin/env python3
r"""graph.py -- the execution/dependency graph (spec Section 6), built from observed evidence.

This aggregates the RAW traces in the corpus into one queryable graph of what actually ran:
functions that executed, the calls they made, and the memory regions they touched. It is an
OBSERVED graph, not a static import table -- an edge exists because a trace recorded it.

    nodes:  function <module>@<va>  (+ its hypothesis-derived role summary, if any)
            target  <va>            (a call destination: another function, or an external addr)
    edges:  from-func --calls-->  target   (direct calls; kind=call_direct)
            from-func --writes--> region   (coarse memory region the function wrote)
            from-func --reads-->  region

Indirect calls (register/memory targets Unicorn couldn't resolve to an immediate) are counted
per function rather than drawn as edges -- we record the uncertainty instead of inventing a
destination. Everything here is DERIVED and regenerable from raw/.

    wt behavior graph <module> [--trace N] [--summary] [--callers 0xVA] [--callees 0xVA]
"""
import json
import os

from src.engines.behavior import corpus, tracer, hypothesis


def _raw_traces(module):
    """Yield the latest stored trace dict per function for a module."""
    d = os.path.join(corpus.raw_dir(), os.path.basename(module))
    if not os.path.isdir(d):
        return
    for f in sorted(x for x in os.listdir(d) if x.endswith(".json")):
        with open(os.path.join(d, f), encoding="utf-8") as fh:
            yield json.load(fh)


def _region(addr):
    """Coarse label for a memory address -- stack vs the mapped scratch/heap vs code vs low."""
    if 0x200000 <= addr < 0x210000:
        return "scratch:0x%x000" % (addr >> 12)
    if addr >= 0xBF000000:
        return "code/module"
    if addr < 0x1000:
        return "nullpage"
    return "other:0x%x000" % (addr >> 12)


def trace_sample(module, n, insn_cap=3000):
    """Populate the corpus with traces of the n LARGEST functions (most likely to contain calls
    and branches; the tiny ones at the start are import thunks that just run away), so the graph
    has data. Returns how many produced a clean (non-truncated) trace."""
    from src.engines.behavior import env
    sig = env.resolver()
    biggest = sorted(tracer.list_functions(module), key=lambda x: -x[2])[:n]
    ok = 0
    for off, va, size in biggest:
        # stub externals so calls into the API resolve to NAMED dependencies, not null pointers
        rec = tracer.trace_function(module, func=hex(va), insn_cap=insn_cap,
                                    stub_externals=True, sig_resolver=sig)
        corpus.record(rec)
        if not rec["truncated"]:
            ok += 1
    return ok


def build(module):
    """Build the observed graph for a module from its corpus traces. Persist + return it."""
    mod = os.path.basename(module)
    # known function VAs (to classify a call target as intra-module vs external)
    known = {va for _, va, _ in tracer.list_functions(module)}
    nodes, edges = {}, []
    for t in _raw_traces(module):
        fva = t["func_va"]
        hyps = hypothesis.load(mod, fva)["hypotheses"]
        roles = [h["statement"] for h in hyps if h["confidence"] >= 0.66
                 and h["subject"].get("kind") == "arg"]
        nodes["0x%x" % fva] = {
            "kind": "function", "va": fva, "off": t["func_off"],
            "leaf": len(t.get("calls", [])) == 0,
            "indirect_calls": sum(1 for c in t.get("calls", []) if c["target"] is None),
            "roles": roles, "exit": t["exit"],
        }
        for c in t.get("calls", []):
            # a stubbed external call carries its resolved API name -> use that as the node, so
            # the graph shows real dependencies (KERNEL32.dll!GlobalAlloc), not bare addresses.
            if c.get("api"):
                tnode = c["api"]
                nodes.setdefault(tnode, {"kind": "external", "api": c["api"]})
            else:
                # prefer the static direct target; else the dynamically resolved indirect target
                tgt = c["target"] if c["target"] is not None else c.get("resolved_target")
                if tgt is None:
                    continue
                tnode = "0x%x" % tgt
                if tnode not in nodes:
                    nodes[tnode] = {"kind": "function" if tgt in known else "external", "va": tgt,
                                    "external": bool(c.get("external"))}
            edges.append({"from": "0x%x" % fva, "to": tnode,
                          "kind": "call_indirect" if c.get("indirect") else "call_direct",
                          "pc": c["pc"]})
        for m in t.get("mem_writes", [])[:200]:
            edges.append({"from": "0x%x" % fva, "to": _region(m["addr"]), "kind": "writes"})
        for m in t.get("mem_reads", [])[:200]:
            edges.append({"from": "0x%x" % fva, "to": _region(m["addr"]), "kind": "reads"})
    # dedupe region edges (keep call edges distinct by pc)
    seen, uniq = set(), []
    for e in edges:
        key = (e["from"], e["to"], e["kind"], e.get("pc"))
        if key in seen:
            continue
        seen.add(key); uniq.append(e)
    g = {"schema": "cb.behavior.graph/1", "module": mod,
         "nodes": nodes, "edges": uniq}
    d = os.path.join(corpus.derived_dir(), "graph")
    os.makedirs(d, exist_ok=True)
    fp = os.path.join(d, "%s.json" % mod)
    with open(fp, "w", encoding="utf-8") as fh:
        json.dump(g, fh, indent=1)
    g["_path"] = fp
    return g


_CALL = ("call_direct", "call_indirect")


def callers(g, va):
    t = "0x%x" % (int(va, 0) if isinstance(va, str) else va)
    return sorted({e["from"] for e in g["edges"] if e["kind"] in _CALL and e["to"] == t})


def callees(g, va):
    f = "0x%x" % (int(va, 0) if isinstance(va, str) else va)
    return sorted({e["to"] for e in g["edges"] if e["kind"] in _CALL and e["from"] == f})


# --------------------------------------------------------------------------
import sys  # noqa: E402


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    if "--trace" in argv:
        n = int(_opt(argv, "--trace", 50))
        ok = trace_sample(module, n)
        print("traced %d functions (%d clean) into the corpus" % (n, ok))
    g = build(module)
    fcount = sum(1 for v in g["nodes"].values() if v["kind"] == "function")
    ecount = sum(1 for v in g["nodes"].values() if v["kind"] == "external")
    calls = sum(1 for e in g["edges"] if e["kind"] in ("call_direct", "call_indirect"))
    indirect = sum(1 for e in g["edges"] if e["kind"] == "call_indirect")
    print("graph %s: %d function nodes, %d external targets, %d call edges (%d indirect)  (%s)" %
          (g["module"], fcount, ecount, calls, indirect, g["_path"]))

    if "--callers" in argv:
        va = _opt(argv, "--callers")
        print("callers of %s: %s" % (va, ", ".join(callers(g, va)) or "(none observed)"))
    if "--callees" in argv:
        va = _opt(argv, "--callees")
        print("callees of %s: %s" % (va, ", ".join(callees(g, va)) or "(none observed)"))
    if "--summary" in argv or ("--callers" not in argv and "--callees" not in argv):
        leaves = [k for k, v in g["nodes"].items() if v.get("leaf")]
        callers_ct = {}
        for e in g["edges"]:
            if e["kind"] in ("call_direct", "call_indirect"):
                callers_ct[e["to"]] = callers_ct.get(e["to"], 0) + 1
        hot = sorted(callers_ct.items(), key=lambda kv: -kv[1])[:8]
        print("  leaf functions : %d" % len(leaves))
        if hot:
            print("  most-called targets (observed):")
            for tgt, c in hot:
                kind = g["nodes"].get(tgt, {}).get("kind", "?")
                print("     %s  <- %d call site(s)   [%s]" % (tgt, c, kind))


if __name__ == "__main__":
    main()
