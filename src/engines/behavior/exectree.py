#!/usr/bin/env python3
r"""exectree.py -- derived execution-tree discovery (dynamic cluster, system #8).

The whole-program view the per-function tools feed. It DERIVES a reachable-execution
graph from the raw trace evidence (append-only corpus): every traced function is a
node, and every observed call is an edge -- direct calls, external/API dependencies,
AND the indirect edges `fracture` resolved that static disassembly could not provide.
"Derived" in the corpus's sense: regenerable at any time from raw/ without re-running
the software, so it is stored under derived/exectree and freely rebuilt.

Two modes:
  * AGGREGATE (default) -- union all recorded traces of a module into the graph, then
    print the reachable tree from the entry functions. Pure evidence, instant.
  * DISCOVER (--discover ENTRY) -- actively grow the tree: fuzz the entry, follow each
    observed INTERNAL call target, fuzz/trace that too, to a depth bound. This is the
    discovery loop -- the tree expands into code reached only through resolved edges.

    wt behavior exectree <module> [--discover FUNC] [--depth N] [--budget N] [--seed S]
"""
import bisect
import json
import os
import sys

from src.core import cores
from src.engines.behavior import corpus, fuzz, tracer
from src.cli.common import opt as _opt


def _funcmap(module):
    """sorted (start_va list, {start_va: name}) for the module -- to resolve any executed
    VA to its enclosing function."""
    from src.formats.pe_trace import PeCarve
    path = cores.get("target").resolve(module)
    c = PeCarve(path)
    starts = sorted(c.funcs)
    names = {s: (c.funcs[s].get("name") or "sub_%x" % s) for s in starts}
    return starts, names


def _enclosing(starts, va):
    """The start VA of the function containing `va`, or None."""
    i = bisect.bisect_right(starts, va) - 1
    return starts[i] if i >= 0 else None


def _iter_raw(module):
    """Every recorded trace for the module, from the append-only raw log."""
    log = os.path.join(corpus.raw_dir(), "traces.jsonl")
    base = os.path.basename(module)
    if not os.path.exists(log):
        return
    with open(log, encoding="utf-8") as fh:
        for line in fh:
            try:
                rec = json.loads(line)
            except Exception:
                continue
            if rec.get("module") == base:
                yield rec


def _edges_from(rec, starts, names):
    """Observed edges out of the traced function in one trace: (dst_key, kind)."""
    out = []
    for c in rec.get("calls", []):
        rt = c.get("resolved_target")
        if c.get("external") and c.get("api"):
            out.append(("api:%s" % c["api"], "indirect-api" if c.get("indirect") else "api"))
        elif rt is not None:                              # resolved internal target (incl. indirect)
            f = _enclosing(starts, rt)
            if f is not None:
                out.append(("fn:%x" % f, "indirect" if c.get("indirect") else "direct"))
        elif c.get("target") is not None:                 # static direct target
            f = _enclosing(starts, c["target"])
            if f is not None:
                out.append(("fn:%x" % f, "direct"))
    for api in rec.get("api_deps", []):                   # stubbed imports (jmp/call thunks)
        out.append(("api:%s" % api, "api"))
    return out


def build(module):
    """Aggregate all recorded traces of a module into an execution graph (nodes + edges)."""
    starts, names = _funcmap(module)
    nodes = {}                                            # key -> {label, kind, entry}
    edges = {}                                            # (src,dst) -> set(kinds)
    entries = set()
    for rec in _iter_raw(module):
        src = "fn:%x" % (_enclosing(starts, rec["func_va"]) or rec["func_va"])
        entries.add(src)
        nodes.setdefault(src, {"label": names.get(rec["func_va"], "0x%x" % rec["func_va"]),
                               "kind": "fn", "va": rec["func_va"]})
        for dst, kind in _edges_from(rec, starts, names):
            if dst.startswith("fn:"):
                va = int(dst[3:], 16)
                nodes.setdefault(dst, {"label": names.get(va, dst), "kind": "fn", "va": va})
            else:
                nodes.setdefault(dst, {"label": dst[4:], "kind": "api", "va": None})
            edges.setdefault((src, dst), set()).add(kind)
    return starts, names, nodes, edges, entries


def _tree_lines(nodes, edges, roots, max_depth=6):
    """Reachable-tree text from the roots (DFS, cycle-guarded)."""
    adj = {}
    for (s, d), kinds in edges.items():
        adj.setdefault(s, []).append((d, kinds))
    lines, seen = [], set()

    def walk(n, depth, prefix):
        lbl = nodes.get(n, {}).get("label", n)
        tag = "" if nodes.get(n, {}).get("kind") == "fn" else "  [API]"
        lines.append("%s%s%s" % (prefix, lbl, tag))
        if n in seen or depth >= max_depth:
            if n in seen and adj.get(n):
                lines[-1] += "  (*)"
            return
        seen.add(n)
        for d, kinds in sorted(adj.get(n, []), key=lambda x: nodes.get(x[0], {}).get("label", "")):
            mark = "~>" if ("indirect" in kinds or "indirect-api" in kinds) else "->"
            walk(d, depth + 1, prefix + "  " + mark + " ")

    for r in sorted(roots, key=lambda x: nodes.get(x, {}).get("label", "")):
        walk(r, 0, "")
    return lines


def discover(module, entry, depth=2, budget=200, seed=0, insn_cap=4000):
    """Grow the tree from `entry`: fuzz it, follow each observed internal call target,
    fuzz/trace that too, to `depth`. Records traces raw; the graph is rebuilt after."""
    starts, names = _funcmap(module)
    frontier = [(str(entry), 0)]
    visited = set()
    while frontier:
        func, d = frontier.pop(0)
        if func in visited or d > depth:
            continue
        visited.add(func)
        try:
            sess = fuzz.fuzz(module, func=func, budget=budget, seed=seed, insn_cap=insn_cap)
        except Exception:
            continue
        if d < depth:                                     # follow this function's internal call targets
            for rec in _iter_raw(module):
                if rec.get("func_off") != sess["func_off"]:
                    continue
                for dst, _kind in _edges_from(rec, starts, names):
                    if dst.startswith("fn:"):
                        sel = "0x%x" % int(dst[3:], 16)
                        if sel not in visited:
                            frontier.append((sel, d + 1))
    return build(module)


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(main.__doc__); return
    module = argv[0]
    disc = _opt(argv, "--discover")
    depth = int(_opt(argv, "--depth", 2))
    budget = int(_opt(argv, "--budget", 200))
    seed = int(_opt(argv, "--seed", 0))
    if disc is not None:
        starts, names, nodes, edges, entries = discover(module, disc, depth=depth, budget=budget, seed=seed)
    else:
        starts, names, nodes, edges, entries = build(module)
    if not nodes:
        print("no recorded traces for %s -- run `wt behavior fuzz/trace %s <func>` first, "
              "or use --discover FUNC" % (module, module)); return
    fn_nodes = [k for k, v in nodes.items() if v["kind"] == "fn"]
    api_nodes = [k for k, v in nodes.items() if v["kind"] == "api"]
    indirect = sum(1 for kinds in edges.values() if ("indirect" in kinds or "indirect-api" in kinds))
    # roots = traced entries with no incoming edge from another node
    incoming = {d for (_s, d) in edges}
    roots = [e for e in entries if e not in incoming] or sorted(entries)
    graph = {"schema": "cb.behavior.exectree/1", "module": os.path.basename(module),
             "functions": len(fn_nodes), "apis": len(api_nodes), "edges": len(edges),
             "indirect_edges": indirect,
             "nodes": {k: v for k, v in nodes.items()},
             "edge_list": [{"src": s, "dst": d, "kinds": sorted(k)} for (s, d), k in edges.items()]}
    corpus.record_derived("exectree", {"module": os.path.basename(module),
                                        "func_va": nodes[roots[0]]["va"] if roots and nodes[roots[0]]["va"] else 0,
                                        **graph})
    print("execution tree: %s  (%d functions, %d APIs, %d edges, %d indirect/resolved)"
          % (graph["module"], graph["functions"], graph["apis"], graph["edges"], graph["indirect_edges"]))
    print("  roots: %s\n" % ", ".join(nodes[r]["label"] for r in roots[:6]))
    for ln in _tree_lines(nodes, edges, roots)[:40]:
        print("  " + ln)


if __name__ == "__main__":
    main()
