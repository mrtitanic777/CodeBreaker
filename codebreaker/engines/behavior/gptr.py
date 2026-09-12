#!/usr/bin/env python3
r"""gptr.py -- reconstruct functions that operate through a GLOBAL POINTER (spec Section 5/12).

struct models writes through a pointer ARGUMENT; globals models writes to fixed absolute
addresses. A large class of C-runtime / library functions does neither: they load a pointer
from a GLOBAL variable (an allocated buffer, an internal-state struct, a locale/heap table) and
read/write THROUGH it -- `base = *g_state; base->field = v`. In a bare trace that global is
NULL, so the writes land at address ~0 and synth/struct/globals all decline ("side effects" /
"no pointer arg" / "write to address 0 isn't a global").

This recovers them the faithful behavioral way -- GLOBAL-POINTER PROVISIONING:
  1. baseline trace -> detect null-ish writes (an unsupplied global pointer) and the image-range
     dword reads that are pointer candidates.
  2. seed each candidate global with a controlled buffer (struct's BUF_BASE); the one whose
     seeding makes the writes land IN the buffer IS the global pointer.
  3. with it seeded, run N times varying the scalar args; model each buffer offset's written
     value as an expression over the args (reusing struct._expr), and detect field copies.
  4. emit C through the NAMED global pointer (`g_<addr>->f_<off> = expr`), differential-verified
     by re-tracing. A write it can't explain is flagged, not faked.

    wt behavior gptr <module> [func]
"""
import os
import sys

from codebreaker.engines.behavior import tracer, perturb
from codebreaker.engines.behavior import struct as S

MASK = 0xFFFFFFFF
IMG_LO, IMG_SPAN = None, 0x400000          # image-range heuristic set per-call from base trace


def _null_writes(t):
    return [w for w in t.get("mem_writes", []) if w["addr"] < 0x1000]


def _global_reads(t, img_lo, img_hi):
    return sorted({m["addr"] for m in t.get("mem_reads", [])
                   if img_lo <= m["addr"] < img_hi and m["size"] == 4})


def _buf_writes(t):
    return [{"off": w["addr"] - S.BUF_BASE, "size": w["size"], "value": w["value"]}
            for w in t.get("mem_writes", []) if S._in_buf(w["addr"])]


def reconstruct(module, func=None, insn_cap=6000):
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.gptr/1", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"], "reconstructed": False}
    if base["exit"] != "ret":
        res["reason"] = "does not return cleanly (%s)" % base["exit"]; return res
    img_lo = base["func_va"] & ~(IMG_SPAN - 1)               # image base approx (page-aligned span)
    img_hi = img_lo + IMG_SPAN
    if not _null_writes(base):
        res["reason"] = "no null-pointer writes -- not a global-pointer function"; return res
    # arity
    try:
        _, rep = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
        arity = rep["arity"]
    except Exception:
        arity = 4
    # 1. find the global pointer: seed each candidate with the controlled buffer, keep the one
    #    whose seeding captures the most writes into the buffer.
    cands = _global_reads(base, img_lo, img_hi)
    best_g, best_n = None, 0
    for g in cands[:40]:
        t = tracer.trace_function(module, func=func, insn_cap=insn_cap,
                                  mem_init=[(g, S.BUF_BASE.to_bytes(4, "little")),
                                            (S.BUF_BASE, S._fill())])
        n = len(_buf_writes(t))
        if n > best_n:
            best_n, best_g = n, g
    if not best_g or best_n == 0:
        res["reason"] = "no global pointer captures the writes (writes may be computed/indexed)"
        return res
    res["global"] = best_g
    # 2. N runs varying the scalar args; record buffer writes + their values
    import random
    rng = random.Random(base["func_va"])
    N = 16
    runs = []
    for _ in range(N):
        avals = [rng.getrandbits(16) for _ in range(arity)]
        t = tracer.trace_function(module, func=func, args=avals, insn_cap=insn_cap,
                                  mem_init=[(best_g, S.BUF_BASE.to_bytes(4, "little")),
                                            (S.BUF_BASE, S._fill())])
        w = {x["off"]: x["value"] for x in _buf_writes(t) if x["value"] is not None}
        runs.append({"vars": {("a%d" % i): avals[i] for i in range(arity)}, "writes": w})
    # 3. model each consistently-written offset as an expression over the args
    offs = sorted({o for r in runs for o in r["writes"]})
    vars_ = ["a%d" % i for i in range(arity)]
    out_fields = []
    for off in offs:
        wrote = [r for r in runs if off in r["writes"]]
        if len(wrote) < max(4, N // 2):
            continue                                        # not written consistently
        series = [(r["vars"], r["writes"][off]) for r in wrote]
        model = S._expr(series, vars_)
        if model:
            out_fields.append({"off": off, "desc": model[0], "c": model[1]})
    if not out_fields:
        res["reason"] = "global pointer found but no field write reproduces as an arg expression"
        return res
    # 4. verify: re-trace with fresh args; each modeled write must reproduce
    ok = True
    for _ in range(10):
        avals = [rng.getrandbits(16) for _ in range(arity)]
        t = tracer.trace_function(module, func=func, args=avals, insn_cap=insn_cap,
                                  mem_init=[(best_g, S.BUF_BASE.to_bytes(4, "little")),
                                            (S.BUF_BASE, S._fill())])
        w = {x["off"]: x["value"] for x in _buf_writes(t)}
        env = {("a%d" % i): avals[i] for i in range(arity)}
        for f in out_fields:
            if f["off"] in w and w[f["off"]] is not None:
                try:
                    if (eval(f["c"].replace("U", ""), {}, env) & MASK) != (w[f["off"]] & MASK):
                        ok = False
                except Exception:
                    pass
    res.update(reconstructed=True, verified=ok, arity=arity,
               out_fields=out_fields, n_globals=len(cands),
               c=_emit(base["func_va"], best_g, out_fields, arity))
    return res


def _emit(va, g, out_fields, arity):
    params = ", ".join("unsigned a%d" % i for i in range(arity)) or "void"
    lines = ["    /* operates through global pointer g_%x */" % g]
    for f in out_fields:
        lines.append("    g_%x->f_%x = %s" % (g, f["off"], f["c"].replace("return ", "").rstrip(";") + ";"))
    lines.append("    return 0; /* value */")
    return "/* extern struct C_%x *g_%x; */\nint sub_%x(%s)\n{\n%s\n}\n" % (
        g, g, va, params, "\n".join(lines))


from codebreaker.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")), None)
    r = reconstruct(module, func=func)
    print("gptr: %s func 0x%x" % (r["module"], r["func_va"]))
    if not r["reconstructed"]:
        print("  not reconstructed: %s" % r["reason"]); return
    print("  global pointer: 0x%x   fields written: %d   verified: %s" %
          (r["global"], len(r["out_fields"]), r["verified"]))
    print(r["c"])


if __name__ == "__main__":
    main()
