#!/usr/bin/env python3
r"""fracture.py -- fractured-branch reverse-engineering (dynamic cluster, system #7).

A "fractured branch" is a control transfer whose destination static analysis cannot
read: an indirect `call reg`/`call [mem]` (a function pointer / vtable slot) or an
indirect `jmp [table + idx*4]` (a switch dispatch / the eax-index thunks). The
disassembler declines these -- the edge is data-dependent, resolved only at runtime.

This reconstructs them from EVIDENCE. It drives the coverage-guided fuzzer to reach the
different arms, re-traces each discovered input, and reads where each indirect transfer
actually LANDED (the tracer resolves an indirect CALL's target directly; an indirect
JMP's target is the next instruction on the executed path). The result is the branch
reassembled as a table: per fracture site, the set of observed targets, each with a
witness input that reaches it -- an honest, observed reconstruction (exhaustive only if
the driving inputs covered the whole index space, which is reported, never assumed).

    wt behavior fracture <module> [func] [--budget N] [--seed S] [--insns N]
"""
import sys

from codebreaker.engines.behavior import tracer, corpus, fuzz
from codebreaker.cli.common import opt as _opt


def _sites(rec):
    """(pc, kind, target) for every indirect transfer observed in a trace.
    kind: 'call*' (indirect call) or 'jmp*' (indirect jmp -> next path VA is the landing)."""
    out = []
    for c in rec.get("calls", []):
        if c.get("indirect") and c.get("resolved_target") is not None:
            out.append((c["pc"], "call*", c["resolved_target"]))
    path, mn = rec.get("path", []), rec.get("mnemonics", {})
    for k in range(len(path) - 1):
        m = mn.get("0x%x" % path[k], "")
        if m.startswith("jmp") and "[" in m:                 # indirect jmp: landed at path[k+1]
            out.append((path[k], "jmp*", path[k + 1]))
    return out


def fracture(module, func=None, budget=400, seed=0, insn_cap=4000):
    """Reconstruct the fractured branches of a function. Returns a report; persists it
    under derived/fracture."""
    sess = fuzz.fuzz(module, func=func, budget=budget, seed=seed, insn_cap=insn_cap)
    lo, hi = sess["func_va"], None
    agg = {}                                                 # pc -> {kind, mnemonic, targets:{tgt:witness}}
    for c in sess["corpus"]:
        args = c["args"]
        mem = [(int(k, 16), bytes.fromhex(v)) for k, v in c["mem"].items()]
        try:
            rec = tracer.trace_function(module, func=func, args=args, mem_init=mem, insn_cap=insn_cap)
        except Exception:
            continue
        mn = rec.get("mnemonics", {})
        for pc, kind, tgt in _sites(rec):
            e = agg.setdefault(pc, {"kind": kind, "mnemonic": mn.get("0x%x" % pc, "?"), "targets": {}})
            e["targets"].setdefault(tgt, {"args": args, "mem": c["mem"]})
    sites = []
    for pc, e in sorted(agg.items()):
        tgts = sorted(e["targets"])
        sites.append({
            "pc": pc, "kind": e["kind"], "mnemonic": e["mnemonic"],
            "n_targets": len(tgts),
            "fractured": len(tgts) > 1,                      # >1 observed target = a real dispatch
            "targets": [{"target": t, "witness": e["targets"][t]} for t in tgts],
        })
    report = {
        "schema": "cb.behavior.fracture/1", "module": sess["module"],
        "func_va": sess["func_va"], "func_off": sess["func_off"],
        "seed": seed, "driven_inputs": len(sess["corpus"]),
        "sites": sites,
        "n_fractured": sum(1 for s in sites if s["fractured"]),
        "n_resolved_ptr": sum(1 for s in sites if not s["fractured"]),
    }
    corpus.record_derived("fracture", report)
    return report


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(main.__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")
                 and argv[argv.index(a) - 1] not in ("--budget", "--seed", "--insns")), None)
    budget = int(_opt(argv, "--budget", 400))
    seed = int(_opt(argv, "--seed", 0))
    insns = int(_opt(argv, "--insns", 4000))
    r = fracture(module, func=func, budget=budget, seed=seed, insn_cap=insns)
    print("fracture %s  func VA 0x%x  (%d driving inputs)" % (r["module"], r["func_va"], r["driven_inputs"]))
    print("  indirect sites: %d  (%d fractured/multi-target dispatch, %d single-target pointer)"
          % (len(r["sites"]), r["n_fractured"], r["n_resolved_ptr"]))
    for s in r["sites"]:
        tag = "DISPATCH" if s["fractured"] else "ptr"
        print("  [%s] 0x%x  %-28s -> %d target(s)" % (tag, s["pc"], s["mnemonic"], s["n_targets"]))
        for t in s["targets"][:6]:
            w = t["witness"]
            print("       -> 0x%-8x  witness args=[%s]%s" %
                  (t["target"], ", ".join("0x%x" % a for a in w["args"]),
                   ("  +ptr" if w["mem"] else "")))
    if not r["sites"]:
        print("  (no indirect transfers observed -- this function's control flow is fully static)")


if __name__ == "__main__":
    main()
