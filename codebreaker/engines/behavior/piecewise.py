#!/usr/bin/env python3
r"""piecewise.py -- multi-block behavioral synthesis: an N-way guarded expression.

The gap between synth and cfg. `synth` reconstructs a straight-line expression but no
branch; `cfg` reconstructs a guard but only CONSTANT arms (`return 0xC;`). A function
whose return is different EXPRESSIONS in different input regions -- the commonest
`human`-route shape (clamp-and-scale, sign-split, bounded transform, char classes) -- is
reconstructed by neither. This recovers it by GREEDILY PEELING: partition the observed
I/O with a recovered guard, fit each region with an expression, recursing on the
remainder -- producing an N-way `if/else` chain (nested guards also express ranges, e.g.
`if (x<0x5b){ if(x<0x41) return x; else return x+0x20; } else return x;` = CharLower).

Faithful-only (the project rule for this lane): the oracle is sampled on REAL inputs; no
gate-busting / forced branches feed synthesis. Every candidate must reproduce EVERY
observation AND survive fresh random inputs; gcc confirms when present. Scope: one
return-affecting integer argument, no side effects, guards from {==0, (int)<0, x<T for
fixed + data-derived T, ==C}, arms in a cheap expr grammar (const/identity/+c/^c/-c/
*small/shift), recursion depth <= 4. Anything richer is declined, not faked.

    wt behavior piecewise <module> [func] [--arg-of N] [--trials 60]
"""
import os
import random
import sys
from collections import Counter

from codebreaker.engines.behavior import tracer, synth, cfg
from codebreaker.cli.common import opt as _opt

MASK = 0xFFFFFFFF


def _guard_candidates(obs):
    """(desc, predicate_fn, c_condition) guards to try as the domain split."""
    cands = [("x == 0", lambda x: (x & MASK) == 0, "x == 0"),
             ("(int)x < 0", lambda x: cfg._s32(x) < 0, "(int)x < 0")]
    # fixed bounds + thresholds DERIVED from the sampled x-values (so a boundary like x<100 or a
    # char class 'A'..'Z' is available even though it isn't a round number). Region-specific.
    fixedT = (2, 3, 4, 5, 6, 8, 10, 16, 24, 32, 64, 128, 256, 0x100, 0x400, 0x1000,
              0x8000, 0x10000, 0x7FFFFFFF, 0x80000000)
    dataT = sorted({x & MASK for x in obs if 0 < (x & MASK) <= 0x10000})[:256]
    seenT = set()
    for T in list(fixedT) + dataT:
        if T in seenT:
            continue
        seenT.add(T)
        cands.append(("x < 0x%x" % T, (lambda T: lambda x: (x & MASK) < T)(T), "x < 0x%xU" % T))
    cnt = Counter(obs.values())                          # equality to a value only one input produces
    for x, o in obs.items():
        if cnt[o] == 1:
            cands.append(("x == 0x%x" % x, (lambda X: lambda z: (z & MASK) == X)(x), "x == 0x%xU" % x))
    return cands


def _fit(part):
    """Grammar expressions (simplest first) reproducing every (x->out) in a partition."""
    outs = set(part.values())
    surv = [(l, c, fn) for (l, c, fn) in synth._grammar(outs)
            if all((fn(x) & MASK) == part[x] for x in part)]
    surv.sort(key=lambda t: len(t[1]))
    return surv


def _fits_cheap(part):
    """A fast fit over the shapes a piecewise ARM actually takes (const/identity/+c/^c/
    *small/shift) -- avoids running synth's full grammar for every guard trial, so we can
    afford ALL exact data-derived boundaries. Returns ('return <expr>;', fn) or None."""
    xs = list(part)
    outs = [part[x] & MASK for x in xs]
    xm = [x & MASK for x in xs]
    if len(set(outs)) == 1:
        c = outs[0]; return ("return 0x%xU;" % c, (lambda c: lambda x: c)(c))
    if all(x == o for x, o in zip(xm, outs)):
        return ("return x;", lambda x: x & MASK)
    cs = {(o - x) & MASK for x, o in zip(xm, outs)}
    if len(cs) == 1:
        c = cs.pop(); return ("return x + 0x%xU;" % c, (lambda c: lambda x: (x + c) & MASK)(c))
    cs = {o ^ x for x, o in zip(xm, outs)}
    if len(cs) == 1:
        c = cs.pop(); return ("return x ^ 0x%xU;" % c, (lambda c: lambda x: (x ^ c) & MASK)(c))
    cs = {(x - o) & MASK for x, o in zip(xm, outs)}
    if len(cs) == 1:
        c = cs.pop(); return ("return x - 0x%xU;" % c, (lambda c: lambda x: (x - c) & MASK)(c))
    for k in range(1, 32):
        if all(((x << k) & MASK) == o for x, o in zip(xm, outs)):
            return ("return x << %d;" % k, (lambda k: lambda x: (x << k) & MASK)(k))
        if all((x >> k) == o for x, o in zip(xm, outs)):
            return ("return x >> %d;" % k, (lambda k: lambda x: x >> k)(k))
    for m in (2, 3, 4, 5, 6, 8, 10, 16):
        if all(((x * m) & MASK) == o for x, o in zip(xm, outs)):
            return ("return x * %d;" % m, (lambda m: lambda x: (x * m) & MASK)(m))
    return None


def _region(obs, depth=0, maxdepth=4):
    """Reconstruct one input region as a C STATEMENT by GREEDILY PEELING: a leaf
    `return <expr>;` if one grammar expression fits the whole region; else find the guard
    that peels off the LARGEST sub-region a single expression fits, emit
    `if (guard) return <that>; else <recurse on the remainder>`, and recurse only on the
    remainder. This yields N-way if/else chains (and ranges via nested guards) in
    polynomial time -- unlike splitting on every guard and recursing both sides. Returns
    (cstmt, fn, size) or None; fn is a Python predictor over the region (for pruning)."""
    whole = _fits_cheap(obs)
    if whole:                                             # leaf: one expression fits the whole region
        return (whole[0], whole[1], len(whole[0]))
    if depth >= maxdepth:
        return None
    best = None                                           # (peeled_count, gc, gfn, leaf, remainder, on_true)
    for _gdesc, gfn, gc in _guard_candidates(obs):
        A = {x: o for x, o in obs.items() if gfn(x)}
        B = {x: o for x, o in obs.items() if not gfn(x)}
        if len(A) < 2 or len(B) < 2:                      # guard must split into two usable regions
            continue
        fa = _fits_cheap(A)
        if fa and (best is None or len(A) > best[0]):
            best = (len(A), gc, gfn, fa, B, True)
        fb = _fits_cheap(B)
        if fb and (best is None or len(B) > best[0]):
            best = (len(B), gc, gfn, fb, A, False)
    if best is None:
        return None
    _n, gc, gfn, leaf, rem, on_true = best
    rr = _region(rem, depth + 1, maxdepth)                # recurse only on the remainder
    if rr is None:
        return None
    lc, lfn = leaf[0], leaf[1]
    rc, rfn, _ = rr
    if on_true:                                           # if (g) <leaf> else <remainder>
        fn = (lambda g, lf, rf: lambda x: (lf(x) if g(x) else rf(x)) & MASK)(gfn, lfn, rfn)
        cstmt = "if (%s) { %s } else { %s }" % (gc, lc, rc)
    else:                                                 # leaf is on the false side -> swap arms
        fn = (lambda g, lf, rf: lambda x: (rf(x) if g(x) else lf(x)) & MASK)(gfn, lfn, rfn)
        cstmt = "if (%s) { %s } else { %s }" % (gc, rc, lc)
    return (cstmt, fn, len(cstmt))


def synthesize_piecewise(module, func=None, arg_of=None, trials=60, insn_cap=2500):
    try:
        ok, info = synth._candidacy(module, func, insn_cap)
    except Exception as e:                                # a bad selector / trace fault is a clean
        return {"schema": "cb.behavior.piecewise/1", "module": os.path.basename(module),
                "func_va": 0, "reconstructed": False,     #   decline, not a crash
                "reason": "could not analyze (%s)" % str(e)[:80]}
    res = {"schema": "cb.behavior.piecewise/1", "module": os.path.basename(info["base"]["module"]),
           "func_va": info["base"]["func_va"], "func_off": info["base"]["func_off"], "reconstructed": False}
    if not ok:
        res["reason"] = info["reason"]; return res
    inputs = info.get("inputs", [])
    xi = arg_of if arg_of is not None else (inputs[0] if len(inputs) == 1 else None)
    if xi is None:
        res["reason"] = "piecewise v1 needs exactly one return-affecting integer arg (got %s)" % inputs
        return res
    arity = info["arity"]
    cb = lambda x: synth._oracle(module, func, xi, arity, x, insn_cap)

    # dense small sweep (catches low + char/byte-class thresholds like 'A'..'Z') + a wide spread
    xs = sorted(set(list(range(0, 160)) + [0x7F, 0x80, 0xFF, 0x100, 0x1FF, 0x400, 0x1000, 0x1234,
                 0xFFFF, 0x10000, 0x12345678, 0xABCDEF01, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF]))
    obs = {x: cb(x) for x in xs}
    obs = {x: v for x, v in obs.items() if v is not None}
    if len(obs) < 8:
        res["reason"] = "too few clean oracle returns to synthesize"; return res

    # if a SINGLE expression already fits, this is synth's job, not piecewise -- say so.
    if _fit(obs):
        res["reason"] = "return is a single straight-line expression -- use `wt behavior synth`"
        return res

    reg = _region(obs, 0, maxdepth=3)                    # recursive N-way reconstruction
    if reg is None:
        res["reason"] = "no guarded split (v1 grammar, depth<=3) reproduces the behavior"
        return res
    body, pf, _comp = reg

    # prune coincidences on fresh random inputs (the WHOLE piecewise function)
    rng = random.Random(res["func_va"])
    ok_prune = True
    tested = len(obs)
    for _ in range(trials):
        x = rng.getrandbits(32)
        o = cb(x)
        if o is None:
            continue
        tested += 1
        if pf(x) != o:
            ok_prune = False; break
    if not ok_prune:
        res["reason"] = "candidate piecewise form failed on fresh inputs (guard/arm not exact)"
        return res

    params = ", ".join(("unsigned x" if i == xi else "unsigned a%d" % i) for i in range(arity))
    csrc = "unsigned sub_%x(%s)\n{\n    %s\n}\n" % (res["func_va"], params or "void", body)
    res.update({"reconstructed": True, "input_arg": xi, "arity": arity, "tested_inputs": tested,
                "branches": body.count("return"), "ways": body.count("if (") + 1,
                "samples": {("0x%x" % k): ("0x%x" % v) for k, v in list(obs.items())[:8]},
                "emitted_c": csrc,
                "confirm": synth._gcc_confirm(csrc, "sub_%x" % res["func_va"], xi, arity,
                                              module, func, insn_cap)})
    return res


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")), None)
    arg_of = _opt(argv, "--arg-of"); arg_of = int(arg_of) if arg_of is not None else None
    trials = int(_opt(argv, "--trials", 60))
    res = synthesize_piecewise(module, func=func, arg_of=arg_of, trials=trials)
    print("piecewise synthesis: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    print("  input arg%d   arity %d   tested on %d inputs   (%d-way, %d branches)"
          % (res["input_arg"], res["arity"], res["tested_inputs"], res["ways"], res["branches"]))
    print("\n  %s" % res["emitted_c"].replace("\n", "\n  ").rstrip())
    conf = res.get("confirm", {})
    if conf.get("compiled"):
        print("\n  native gcc differential test vs the binary: %s"
              % ("EQUIVALENT" if conf.get("equivalent") else "MISMATCH (%d/%d)" % (conf["agree"], conf["total"])))
    elif conf.get("available") is False:
        print("\n  (gcc not found -- validated against the binary oracle in-process only)")


if __name__ == "__main__":
    main()
