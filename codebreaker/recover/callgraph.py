#!/usr/bin/env python3
r"""callgraph.py -- build a module's static call graph (who calls whom).

The substrate for navigation and for bottom-up reconstruction: statically disassemble every carved
function, collect its direct `call rel32` targets (internal, to another carved function) and its
import calls, and derive fan-in / fan-out, leaves vs. orchestrators, strongly-connected components
(recursion cycles), and a reverse-topological order -- the order in which interproc v3 should
reconstruct so a caller's callees are already modeled.
"""
import sys
from codebreaker.emulate import trace32 as tracer


def build(module):
    """{off: {'calls': set(callee_off), 'napis': int}} over the module's carved functions."""
    import capstone
    _, va0, code, _s, funcs, _sha, imports = tracer._load(module)
    starts = sorted(funcs)
    ends = {o: funcs[o]["end"] for o in starts}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    g = {}
    for o in starts:
        calls, napis = set(), 0
        for ins in md.disasm(code[o:ends[o]], va0 + o):
            if ins.mnemonic == "call":
                if ins.op_str.startswith("0x"):
                    try:
                        t = int(ins.op_str, 16) - va0
                    except ValueError:
                        continue
                    if t in funcs:
                        calls.add(t)
                elif "[" in ins.op_str and "0x" in ins.op_str:
                    napis += 1                                # call [iat] -> an import (approx)
        g[o] = {"calls": calls, "napis": napis}
    return g


def _sccs(g):
    """Tarjan SCCs -> list of components (each a list of offsets); size>1 or self-loop = a cycle."""
    idx = {}; low = {}; onst = {}; st = []; out = []; c = [0]
    sys.setrecursionlimit(max(10000, len(g) * 4 + 1000))

    def dfs(v):
        idx[v] = low[v] = c[0]; c[0] += 1; st.append(v); onst[v] = True
        for w in g[v]["calls"]:
            if w not in g:
                continue
            if w not in idx:
                dfs(w); low[v] = min(low[v], low[w])
            elif onst.get(w):
                low[v] = min(low[v], idx[w])
        if low[v] == idx[v]:
            comp = []
            while True:
                w = st.pop(); onst[w] = False; comp.append(w)
                if w == v:
                    break
            out.append(comp)

    for v in g:
        if v not in idx:
            dfs(v)
    return out


def analyze(module):
    g = build(module)
    fan_in = {o: 0 for o in g}
    for o in g:
        for c in g[o]["calls"]:
            if c in fan_in:
                fan_in[c] += 1
    leaves = [o for o in g if not g[o]["calls"]]
    orchestr = sorted(g, key=lambda o: -len(g[o]["calls"]))[:10]
    sccs = _sccs(g)
    cycles = [c for c in sccs if len(c) > 1 or (c[0] in g[c[0]]["calls"])]
    # reverse-topological order (callees before callers) by condensing SCCs
    order = []
    seen = set()
    for comp in sccs:                                        # Tarjan yields reverse-topo already
        for o in comp:
            if o not in seen:
                order.append(o); seen.add(o)
    return {"n": len(g), "edges": sum(len(v["calls"]) for v in g.values()),
            "leaves": len(leaves), "cycles": len(cycles),
            "top_orchestrators": [(o, len(g[o]["calls"])) for o in orchestr],
            "recon_order": order, "graph": g, "fan_in": fan_in}
