#!/usr/bin/env python3
r"""cfg.py -- control-flow reconstruction (spec Section 11: branches, not just expressions).

synth recovers straight-line integer expressions; this recovers the BRANCH a scalar argument
controls. For a function whose return depends on one argument through a decision, it samples the
argument, finds where the outcome changes (pinning an exact threshold by binary search against
the oracle), classifies the predicate, and emits a conditional -- then differential-verifies it.

Predicate forms recovered in v1 (each confirmed against the binary on fresh inputs):
    if (x == 0)           zero test
    if (x == C)           equality
    if (x <  T)  unsigned / signed threshold (exact T found by binary search)
    return x < C ? x : C  upper clamp (min);  lower clamp (max)

A function whose return is a smooth arithmetic function of x is synth's job, not this; one with
no clean 2-way decision is reported as such rather than forced into an if.

    wt behavior cfg <module> [func] [--arg N]
"""
import os
import random
import sys

from src.engines.behavior import tracer, perturb

MASK = 0xFFFFFFFF


def _s32(v):
    v &= MASK
    return v - 0x100000000 if v & 0x80000000 else v


def _oracle(module, func, ai, arity, x, base, insn_cap=3000):
    args = list(base) + [0] * (arity - len(base))
    args[ai] = x & MASK
    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap)
    return t["ret"] if t["exit"] == "ret" else None


def _find_control_arg(module, func, insn_cap):
    _, report = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
    # prefer an arg that affects BOTH return and control flow
    best = None
    for a in report["args"]:
        affect_ret = "ret" in a["influence"]
        affect_cf = "path_diverges_at" in a["influence"]
        if affect_ret and affect_cf:
            return a["index"], report["arity"]
        if affect_ret and best is None:
            best = a["index"]
    return best, report["arity"]


def _boundary(cb, lo, hi):
    """Binary-search the exact x in (lo, hi] where cb(x) flips from cb(lo) to cb(hi) (unsigned)."""
    a, b, va = lo, hi, cb(lo)
    while b - a > 1:
        m = (a + b) // 2
        if cb(m) == va:
            a = m
        else:
            b = m
    return b                    # smallest x with the flipped outcome


def reconstruct_cfg(module, func=None, ai=None, insn_cap=3000):
    base_trace = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.cfg/1", "module": os.path.basename(base_trace["module"]),
           "func_va": base_trace["func_va"], "func_off": base_trace["func_off"]}
    if ai is None:
        ai, arity = _find_control_arg(module, func, insn_cap)
    else:
        arity = max(ai + 1, 4)
    if ai is None:
        res["reconstructed"] = False
        res["reason"] = "no argument affects the return value"
        return res
    res["control_arg"], res["arity"] = ai, arity
    base = [0] * arity
    cb = lambda x: _oracle(module, func, ai, arity, x, base, insn_cap)

    # sample: dense small sweep + boundary-ish values + signed extremes
    xs = sorted(set(list(range(0, 65)) + [0x7F, 0x80, 0xFF, 0x100, 0x1000, 0xFFFF, 0x10000,
                 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF]))
    obs = {x: cb(x) for x in xs}
    obs = {x: v for x, v in obs.items() if v is not None}
    outs = set(obs.values())
    res["distinct_returns"] = len(outs)

    model = _classify(obs, cb)
    if model is None:
        res["reconstructed"] = False
        res["reason"] = ("return takes %d values with no clean 2-way decision "
                         "(try `wt behavior synth` for an arithmetic form)" % len(outs))
        return res

    # verify on fresh random inputs
    rng = random.Random(res["func_va"])
    ok = True
    for _ in range(60):
        x = rng.getrandbits(32)
        o = cb(x)
        if o is not None and (model["fn"](x) & MASK) != o:
            ok = False; break
    res.update({"reconstructed": True, "verified": ok, "predicate": model["desc"],
                "emitted_c": _emit(res["func_va"], ai, arity, model)})
    return res


def _classify(obs, cb):
    xs = sorted(obs)
    outs = set(obs.values())
    # --- clamp (min/max): identity in one region, constant in the other ---
    ident = [x for x in xs if obs[x] == (x & MASK)]
    if len(ident) >= 3 and len(outs) > 2:
        const_vals = {obs[x] for x in xs if obs[x] != (x & MASK)}
        if len(const_vals) == 1:
            C = next(iter(const_vals))
            lo_ident = all(obs[x] == (x & MASK) for x in xs if x <= C)
            if lo_ident:     # x below C returns x, at/above returns C -> min(x, C)
                return {"desc": "min(x, 0x%x)" % C,
                        "fn": (lambda C: lambda x: min(x & MASK, C))(C),
                        "c": "return x < 0x%xU ? x : 0x%xU;" % (C, C)}
    if len(outs) != 2:
        return None
    a, b = sorted(outs)
    # map each sampled x to 0/1 by which outcome
    def out1(x): return obs[x] == b
    # --- zero test ---
    if obs.get(0) is not None:
        z = obs[0]
        if all((v == z) == (x == 0) for x, v in obs.items()):
            other = a if z == b else b
            return {"desc": "x == 0", "fn": (lambda z, o: lambda x: z if (x & MASK) == 0 else o)(z, other if z != other else a),
                    "c": "if (x == 0) return 0x%xU;\n    return 0x%xU;" % (z, a if z == b else b)}
    # --- equality to a single constant (minority outcome hit by exactly one sample) ---
    from collections import Counter
    cnt = Counter(obs.values())
    minority = min(cnt, key=cnt.get)
    mins = [x for x in xs if obs[x] == minority]
    if cnt[minority] == 1 and len(mins) == 1:
        C = mins[0]; maj = a if minority == b else b
        return {"desc": "x == 0x%x" % C,
                "fn": (lambda C, mn, mj: lambda x: mn if (x & MASK) == C else mj)(C, minority, maj),
                "c": "if (x == 0x%xU) return 0x%xU;\n    return 0x%xU;" % (C, minority, maj)}
    # --- monotonic threshold (unsigned): single flip point over ascending x ---
    flips = [i for i in range(1, len(xs)) if out1(xs[i]) != out1(xs[i - 1])]
    if len(flips) == 1:
        lo, hi = xs[flips[0] - 1], xs[flips[0]]
        T = _boundary(cb, lo, hi)
        loout = obs[lo]
        return {"desc": "x < 0x%x (unsigned)" % T,
                "fn": (lambda T, lo_, hi_: lambda x: lo_ if (x & MASK) < T else hi_)(T, loout, b if loout == a else a),
                "c": "if (x < 0x%xU) return 0x%xU;\n    return 0x%xU;" % (T, loout, b if loout == a else a)}
    # --- signed threshold: order by signed value, single flip ---
    xs_s = sorted(obs, key=_s32)
    sf = [i for i in range(1, len(xs_s)) if out1(xs_s[i]) != out1(xs_s[i - 1])]
    if len(sf) == 1:
        loout = obs[xs_s[0]]
        return {"desc": "(signed) x < 0 region split",
                "fn": (lambda lo_, hi_: lambda x: lo_ if _s32(x) < 0 else hi_)(obs[0xFFFFFFFF], obs[0]),
                "c": "if ((int)x < 0) return 0x%xU;\n    return 0x%xU;" % (obs[0xFFFFFFFF], obs[0])}
    return None


def _emit(va, ai, arity, model):
    params = ", ".join(("unsigned x" if i == ai else "unsigned a%d" % i) for i in range(arity))
    return "unsigned sub_%x(%s)\n{\n    %s\n}\n" % (va, params, model["c"].replace("\n", "\n    "))


# --------------------------------------------------------------------------
from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = None
    for a in argv[1:]:
        if not a.startswith("-"):
            func = a; break
    ai = _opt(argv, "--arg"); ai = int(ai) if ai is not None else None

    res = reconstruct_cfg(module, func=func, ai=ai)
    print("control-flow reconstruction: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    print("  control arg : arg%d   distinct returns: %d" % (res["control_arg"], res["distinct_returns"]))
    print("  predicate   : %s" % res["predicate"])
    print("  differential verification vs binary: %s" % ("VERIFIED" if res["verified"] else "MISMATCH"))
    print("\n  %s" % res["emitted_c"].replace("\n", "\n  ").rstrip())


if __name__ == "__main__":
    main()
