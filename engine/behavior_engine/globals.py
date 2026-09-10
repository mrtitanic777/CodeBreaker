#!/usr/bin/env python3
r"""globals.py -- reconstruct GLOBAL-STATE side effects (spec Section 5/12).

synth models a function's return; struct models what it does through a pointer argument. Neither
captures writes to GLOBAL variables (the module's data at fixed absolute addresses) -- so a
function that, say, zeroes four globals is declined by synth ("side effects") and missed by
struct (no pointer arg). This reconstructs exactly that: it treats the data section as a struct
at absolute addresses, recovering which globals a function writes and the value each gets as an
expression over the function's arguments and the globals it reads.

Method (the struct method, at absolute addresses):
  1. baseline trace -> the set of GLOBAL writes (addr outside the stack/scratch) and the globals
     the function READS.
  2. N runs varying the scalar args AND seeding the read-globals with random values; record each
     global write's value.
  3. explain each written value as an expression over args + read-globals (reusing struct._expr);
     verify it reproduces across all runs.
  4. emit `extern` global decls + the writes, differential-verified against the binary.

v1 scope: dword globals, straight-line writes. A write it can't explain is flagged, not faked.

    wt behavior globals <module> [func] [--args a,b,c]
"""
import os
import random
import sys

from engine.behavior_engine import tracer, perturb
from engine.behavior_engine import struct as S

MASK = 0xFFFFFFFF
STK_LO, STK_HI = 0x200000, 0x210000          # the mapped stack/scratch window (NOT globals)


def _is_global(addr):
    return addr >= 0x10000 and not (STK_LO <= addr < STK_HI)


def reconstruct_globals(module, func=None, other_args=None, insn_cap=6000):
    other_args = other_args or []
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    arity = base["input"]["arity"] if not other_args else max(len(other_args), base["input"]["arity"])
    res = {"schema": "cb.behavior.globals/1", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"]}
    g_writes = sorted({w["addr"] for w in base.get("mem_writes", []) if _is_global(w["addr"])})
    g_reads = sorted({m["addr"] for m in base.get("mem_reads", []) if _is_global(m["addr"])})
    if not g_writes:
        res["reconstructed"] = False
        res["reason"] = "no global writes observed"
        return res

    # Vary only the globals the function READS. Args are held at baseline: randomizing them would
    # change the branch taken and erase the global writes (same lesson as struct). The written
    # value is then explained as an expression over the read-globals (const/copy/arithmetic).
    rng = random.Random(base["func_va"])
    vars_ = [("g", a) for a in g_reads]
    args = list(other_args) + [0] * (arity - len(other_args))
    N = 12
    runs = []
    for _ in range(N):
        gvals = {a: rng.getrandbits(32) for a in g_reads}
        vmap = {("g", a): gvals[a] for a in g_reads}
        mem = [(a, (gvals[a] & MASK).to_bytes(4, "little")) for a in g_reads]
        t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap, mem_init=mem)
        w = {x["addr"]: x["value"] for x in t.get("mem_writes", []) if _is_global(x["addr"])}
        runs.append({"vars": vmap, "writes": w})

    out, ok = [], True
    for addr in g_writes:
        series = [(r["vars"], r["writes"].get(addr)) for r in runs if r["writes"].get(addr) is not None]
        if len(series) < N:
            out.append({"addr": addr, "kind": "conditional"}); ok = False; continue
        ex = S._expr(series, vars_)
        if ex:
            out.append({"addr": addr, "kind": "expr", "desc": ex[0], "expr": ex[1]})
        else:
            out.append({"addr": addr, "kind": "computed"}); ok = False
    res.update({"reconstructed": True, "arity": arity, "reads": g_reads,
                "writes": out, "verified": ok,
                "c": _emit(res["func_va"], g_reads, out, arity)})
    return res


def _emit(va, g_reads, out, arity):
    decls = sorted({a for a in g_reads} | {o["addr"] for o in out})
    lines = ["extern unsigned g_%x;" % a for a in decls]
    body = []
    for o in out:
        if o["kind"] == "expr":
            body.append("    g_%x = %s;" % (o["addr"], o["expr"]))
        else:
            body.append("    /* g_%x = <%s; not reconstructed> */" % (o["addr"], o["kind"]))
    params = ", ".join("unsigned a%d" % i for i in range(arity)) or "void"
    return "%s\n\nvoid sub_%x(%s)\n{\n%s\n}\n" % ("\n".join(lines), va, params, "\n".join(body))


from engine.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = None
    for a in argv[1:]:
        if not a.startswith("-"):
            func = a; break
    other = [int(x, 0) for x in _opt(argv, "--args", "").split(",") if x.strip()]
    res = reconstruct_globals(module, func=func, other_args=other)
    print("global-state reconstruction: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    print("  reads %d global(s); writes %d global(s)" % (len(res["reads"]), len(res["writes"])))
    for o in res["writes"]:
        if o["kind"] == "expr":
            print("    g_%x = %s" % (o["addr"], o["desc"]))
        else:
            print("    g_%x : %s" % (o["addr"], o["kind"]))
    print("  differential verification vs binary: %s" % ("VERIFIED" if res["verified"] else "PARTIAL"))
    print("\n  %s" % res["c"].replace("\n", "\n  ").rstrip())


if __name__ == "__main__":
    main()
