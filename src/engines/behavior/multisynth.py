#!/usr/bin/env python3
r"""multisynth.py -- multi-statement program synthesis (Frontier B), validated by the binary.

The single-expression synth (`synth.py`) proves `return <expr>;` for functions whose return
reduces to one shallow closed form. A large slice of the reconstructable residue is deeper:
pure functions of their inputs whose body is a *straight-line program* -- a hash mixer
(`x ^= x >> 16; x *= 0x45d9f3b; ...`), a masked byte-swap, a multi-step bit merge. No single
expression in synth's 2-level grammar reproduces them, so they floor.

This engine searches the straight-line-program space the faithful way -- it does NOT read the
disassembly. It uses **bottom-up enumerative synthesis over observational-equivalence classes**
(the standard PBE technique):

  1. CANDIDACY reuses synth's gate: returns cleanly, no side effects outside its own frame,
     return is a function of <=2 integer inputs, no pointer args. (Side-effecting / pointer /
     conditional functions are struct's / piecewise's territory -- we decline, never fake.)
  2. SAMPLE the oracle (the real bytes) on a fixed diverse input vector.
  3. ENUMERATE programs bottom-up in order of cost: start from {inputs, constants}; combine
     known values with unary/binary operators; key every sub-program by its OUTPUT VECTOR over
     the samples and keep only the cheapest program per distinct vector (observational
     equivalence). Stop when a program's vector equals the observed-output vector. Because the
     frontier is bounded and processed cheapest-first, the simplest multi-op program wins.
  4. PRUNE coincidences on fresh random inputs; emit MULTI-STATEMENT C (shared sub-programs
     become temporaries); CONFIRM natively with gcc when present -- the spec's final authority.

    wt behavior multisynth <module> [func] [--budget 1500] [--trials 40]
"""
import os
import random
import sys
import time

from src.engines.behavior import synth

MASK = synth.MASK


# --- operators ---------------------------------------------------------------
_SHIFTS = (1, 2, 3, 4, 5, 7, 8, 11, 13, 16, 24)

def _unops():
    """(label_fmt, c_fmt, fn) unary operators. c_fmt takes the operand C text."""
    yield ("~", "~%s", lambda a: (~a) & MASK)
    yield ("-", "-%s", lambda a: (-a) & MASK)
    yield ("bswap", "__builtin_bswap32(%s)", synth._bswap)
    for k in _SHIFTS:
        yield (">>%d" % k, "(%%s >> %d)" % k, (lambda k: lambda a: a >> k)(k))
        yield ("<<%d" % k, "(%%s << %d)" % k, (lambda k: lambda a: (a << k) & MASK)(k))

def _binops():
    """(label, c_fmt, fn, commutative) binary operators; c_fmt takes (left, right) C text."""
    yield ("+", "(%s + %s)", lambda a, b: (a + b) & MASK, True)
    yield ("-", "(%s - %s)", lambda a, b: (a - b) & MASK, False)
    yield ("*", "(%s * %s)", lambda a, b: (a * b) & MASK, True)
    yield ("&", "(%s & %s)", lambda a, b: a & b, True)
    yield ("|", "(%s | %s)", lambda a, b: a | b, True)
    yield ("^", "(%s ^ %s)", lambda a, b: a ^ b, True)


# --- program nodes -----------------------------------------------------------
# A node is a tuple: ('x',) | ('y',) | ('c', k) | ('u', cfmt, child) | ('b', cfmt, l, r).
# Children are shared node objects (the kept representative for their vector), so the winning
# node is a DAG and common sub-programs collapse to one temporary on emit.
def _render_flat(node):
    """Fully-inlined single C expression (for the candidate label)."""
    t = node[0]
    if t == "x": return "x"
    if t == "y": return "y"
    if t == "c": return "0x%xU" % node[1]
    if t == "u": return node[1] % _render_flat(node[2])
    return node[1] % (_render_flat(node[2]), _render_flat(node[3]))


def _emit_multi(node, func_va, params):
    """Linearize the winning DAG into multi-statement C: one `unsigned tN = ...;` per internal
    node, shared nodes reused (CSE), terminals inlined, ending in `return t<last>;`."""
    temps, lines, memo = [0], [], {}
    def walk(n):
        t = n[0]
        if t in ("x", "y"): return n[0]
        if t == "c": return "0x%xU" % n[1]
        key = id(n)
        if key in memo: return memo[key]
        if t == "u":
            rhs = n[1] % walk(n[2])
        else:
            rhs = n[1] % (walk(n[2]), walk(n[3]))
        name = "t%d" % temps[0]; temps[0] += 1
        lines.append("    unsigned %s = %s;" % (name, rhs))
        memo[key] = name
        return name
    last = walk(node)
    body = "\n".join(lines + ["    return %s;" % last])
    return "unsigned sub_%x(%s)\n{\n%s\n}\n" % (func_va, params or "void", body)


def _consts(observed_outputs, sample_inputs):
    """A DELIBERATELY SMALL constant pool: every extra terminal multiplies the per-node binary
    fan-out, so we keep the operands that actually recur in straight-line integer code -- small
    ints, byte/word masks, a few famous multiplicative-hash constants -- plus only the handful
    of observed output/input values that look like masks or small ints (not arbitrary results)."""
    base = {0, 1, 2, 3, 4, 5, 7, 8, 10, 16, 24, 31, 32, 255, 256,
            0xFF, 0xFF00, 0xFFFF, 0xFFFF0000, 0x10000, 0xFFFFFFFF}
    base |= {0x9E3779B9, 0x45D9F3B, 0x811C9DC5, 0x01000193}   # golden ratio, Murmur, FNV
    for v in set(observed_outputs) | set(sample_inputs):
        v &= MASK
        if v < 0x10000 or v in (0xFFFFFFFF, 0xFFFF0000) or (v & (v + 1)) == 0:  # small or mask-like
            base.add(v)
    return sorted(base)


def _search(sample_vecs, target, budget, time_budget, seed):
    """Bottom-up enumerative synthesis over observational-equivalence classes, expanded in
    order of program SIZE (smallest first) so the simplest multi-op program wins. sample_vecs:
    {'x': (..), 'y': (..)} terminal vectors (len = #samples); target: observed-output tuple.
    Returns the winning node (a DAG, sharing kept sub-programs) or None.

    Branching is bounded by restricting binary operators to `subexpr OP terminal` (one side is
    always an input or a constant). This captures chain-shaped straight-line programs -- hash
    mixers (`x = (x ^ (x>>16)) * k`), accumulators (`h = h*31 + c`), masked byte-swaps -- which
    is the shape of the reconstructable residue, while keeping each expansion to |terminals|
    combinations instead of |kept|^2. `budget` caps kept vectors; `time_budget` the wall clock."""
    import heapq
    n = len(target)
    kept = {}           # vector -> node (cheapest/first representative; for dedup)
    pq = []             # heap of (size, tiebreak, vector) -- nodes awaiting expansion
    operands = []       # (vec, node) -- the binary partner set (terminals + input primitives)
    t0 = time.time()
    tb = [0]

    def add(vec, node, sz, push):
        if vec in kept:
            return vec == target
        kept[vec] = node
        if push:
            tb[0] += 1
            heapq.heappush(pq, (sz, tb[0], vec))
        return vec == target

    unops, binops = list(_unops()), list(_binops())
    seeded = []         # input-dependent seeded nodes (for the post-seed lookahead sweep)
    # inputs (expandable + operands)
    inputs = [("x", sample_vecs["x"])] + ([("y", sample_vecs["y"])] if "y" in sample_vecs else [])
    for name, vec in inputs:
        if add(vec, (name,), 0, True):
            return kept[vec]
        operands.append((vec, (name,))); seeded.append((vec, (name,)))
    # constants: operands only (a constant never needs expanding -- it spawns only constants)
    for c in _consts(set(target), sample_vecs["x"] + sample_vecs.get("y", ())):
        cv = tuple([c]) * n
        if cv not in kept:
            kept[cv] = ("c", c)
            operands.append((cv, ("c", c)))
    # unary-of-input PRIMITIVES (x>>k, x<<k, ~x, -x, bswap): precomputed as ready operands AND
    # pushed for expansion, so a two-derived-subexpr body (x^(x>>16), (x<<3)+(x>>3)) forms in one
    # binary step instead of waiting to pop a deep node.
    for name, vec in inputs:
        innode = (name,)
        for _lbl, cfmt, fn in unops:
            nv = tuple(fn(a) for a in vec)
            node = ("u", cfmt, innode)
            if add(nv, node, 1, True):
                return kept[target]
            operands.append((nv, node)); seeded.append((nv, node))
    if target in kept:
        return kept[target]

    def finish(vvec, vnode):
        """1-ply lookahead: is `target` one operator away from vvec? Returns the finishing node
        or None. This decouples DETECTION from pop-order -- the moment a penultimate node (e.g.
        x^(x>>16) for a mixer) is created, we complete it to target without waiting to pop it,
        so the budget cap no longer blocks deep-but-narrow programs."""
        for _l, cf, fn in unops:
            if tuple(fn(a) for a in vvec) == target:
                return ("u", cf, vnode)
        for ovec, onode in operands:
            for _l, cf, fn, comm in binops:
                if tuple(fn(a, b) for a, b in zip(vvec, ovec)) == target:
                    return ("b", cf, vnode, onode)
                if not comm and tuple(fn(a, b) for a, b in zip(ovec, vvec)) == target:
                    return ("b", cf, onode, vnode)
        return None

    # post-seed sweep: target may be one op from a primitive (e.g. (x>>k) * C)
    for vec, node in seeded:
        r = finish(vec, node)
        if r is not None:
            return r

    while pq:
        if len(kept) >= budget or (time.time() - t0) > time_budget:
            break
        sz, _, v = heapq.heappop(pq)
        node = kept[v]
        new = []
        for _lbl, cfmt, fn in unops:                 # unary(v) -- free chaining
            new.append((tuple(fn(a) for a in v), ("u", cfmt, node)))
        for ovec, onode in operands:                 # binary(v, operand) and the mirror
            for _lbl, cfmt, fn, comm in binops:
                new.append((tuple(fn(a, b) for a, b in zip(v, ovec)), ("b", cfmt, node, onode)))
                if not comm:
                    new.append((tuple(fn(a, b) for a, b in zip(ovec, v)), ("b", cfmt, onode, node)))
        for nv, nnode in new:
            if nv == target:
                return nnode
            if nv not in kept:
                r = finish(nv, nnode)               # detect target 1 op beyond this new node
                if r is not None:
                    return r
                add(nv, nnode, sz + 1, True)
    return None


def synthesize(module, func=None, arg_of=None, budget=20000, trials=40,
               time_budget=1.5, insn_cap=2000):
    ok, info = synth._candidacy(module, func, insn_cap)
    result = {"schema": "cb.behavior.multisynth/1",
              "module": os.path.basename(info["base"]["module"]),
              "func_va": info["base"]["func_va"], "func_off": info["base"]["func_off"],
              "candidate": bool(ok)}
    if not ok:
        result["reason"] = info["reason"]
        return result
    arity = info["arity"]
    inputs = info.get("inputs", [])
    if not inputs:
        result["reason"] = "return is constant (no input dependence) -- synth v1 territory"
        return result

    # sample the oracle on a fixed diverse input vector over the 1 or 2 return-affecting args
    if len(inputs) == 1:
        xi = arg_of if arg_of is not None else inputs[0]
        seeds = [0, 1, 2, 3, 7, 8, 15, 100, 255, 256, 0x1234, 0xFFFF, 0x10000,
                 0x12345678, 0xABCDEF01, 0xFF00FF00, 0xDEADBEEF, 0x80000000, 0x7FFFFFFF, 0xFFFFFFFF]
        obs = [(x, synth._oracle(module, func, xi, arity, x, insn_cap)) for x in seeds]
        obs = [(x, r) for x, r in obs if r is not None]
        xs = tuple(x for x, _ in obs); target = tuple(r for _, r in obs)
        sample_vecs = {"x": xs}
        params = ", ".join(("unsigned x" if k == xi else "unsigned a%d" % k) for k in range(arity))
        result["input_arg"] = xi
    else:
        ix, iy = inputs[:2]
        pairs = [(0, 0), (1, 0), (0, 1), (1, 1), (2, 3), (3, 2), (5, 7), (255, 1), (1, 255),
                 (0x100, 0x10), (0x1234, 0x5678), (0x12345678, 0x9ABCDEF0), (0xFFFF, 0xFFFF),
                 (0xFF00, 0x00FF), (0xDEAD, 0xBEEF)]
        def mk(x, y):
            v = [0] * arity; v[ix] = x; v[iy] = y; return v
        obs = [((x, y), synth._oracle_vec(module, func, mk(x, y), insn_cap)) for (x, y) in pairs]
        obs = [(p, r) for p, r in obs if r is not None]
        xs = tuple(p[0] for p, _ in obs); ys = tuple(p[1] for p, _ in obs)
        target = tuple(r for _, r in obs)
        sample_vecs = {"x": xs, "y": ys}
        params = ", ".join(("unsigned x" if k == ix else "unsigned y" if k == iy
                            else "unsigned a%d" % k) for k in range(arity))
        result["input_args"] = [ix, iy]

    if len(target) < 6:
        result["reason"] = "too few clean oracle samples (%d) to constrain a search" % len(target)
        return result

    node = _search(sample_vecs, target, budget, time_budget, info["func_va"])
    result["tested_inputs"] = len(target)
    result["samples"] = {("0x%x" % (obs[i][0] if len(inputs) == 1 else 0)): ("0x%x" % target[i])
                         for i in range(min(6, len(target)))}
    if node is None:
        result["reason"] = "no multi-statement program within budget reproduces the samples"
        return result

    # prune coincidences on fresh random inputs (same oracle), then emit + gcc-confirm
    rng = random.Random(info["func_va"] ^ 0x5A5A)
    ok_prune = True
    if len(inputs) == 1:
        xi = result["input_arg"]
        for _ in range(trials):
            x = rng.getrandbits(32)
            o = synth._oracle(module, func, xi, arity, x, insn_cap)
            if o is None:
                continue
            if _eval(node, {"x": x}) != o:
                ok_prune = False; break
    else:
        ix, iy = result["input_args"]
        for _ in range(trials):
            x, y = rng.getrandbits(32), rng.getrandbits(32)
            o = synth._oracle_vec(module, func, mk(x, y), insn_cap)
            if o is None:
                continue
            if _eval(node, {"x": x, "y": y}) != o:
                ok_prune = False; break
    if not ok_prune:
        result["reason"] = "candidate program matched samples but failed fresh-input pruning " \
                           "(coincidence) -- declined rather than faked"
        return result

    result["expr"] = _render_flat(node)
    csrc = _emit_multi(node, result["func_va"], params)
    result["emitted_c"] = csrc
    if len(inputs) == 1:
        result["confirm"] = synth._gcc_confirm(csrc, "sub_%x" % result["func_va"],
                                               result["input_arg"], arity, module, func, insn_cap)
    else:
        result["confirm"] = {"available": None, "note": "2-var gcc confirm not wired"}
    return result


def _eval(node, env):
    """Evaluate a program node on a concrete input env {'x':..,'y':..} (for pruning)."""
    t = node[0]
    if t == "x": return env["x"]
    if t == "y": return env["y"]
    if t == "c": return node[1]
    if t == "u":
        a = _eval(node[2], env)
        cf = node[1]
        if cf.startswith("~"): return (~a) & MASK
        if cf.startswith("-"): return (-a) & MASK
        if cf.startswith("__builtin_bswap"): return synth._bswap(a)
        if ">>" in cf: return a >> int(cf.split(">>")[1].split(")")[0])
        if "<<" in cf: return (a << int(cf.split("<<")[1].split(")")[0])) & MASK
    a = _eval(node[2], env); b = _eval(node[3], env); cf = node[1]
    if "+" in cf: return (a + b) & MASK
    if "-" in cf: return (a - b) & MASK
    if "*" in cf: return (a * b) & MASK
    if "&" in cf: return a & b
    if "|" in cf: return a | b
    if "^" in cf: return a ^ b
    raise ValueError("unknown node %r" % (node,))


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
    arg_of = _opt(argv, "--arg-of"); arg_of = int(arg_of) if arg_of is not None else None
    budget = int(_opt(argv, "--budget", 1500))
    trials = int(_opt(argv, "--trials", 40))

    res = synthesize(module, func=func, arg_of=arg_of, budget=budget, trials=trials)
    print("multisynth: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["candidate"]:
        print("  NOT a candidate: %s" % res["reason"]); return
    if "emitted_c" not in res:
        print("  %s" % res.get("reason", "no program found")); return
    print("  tested on %d oracle samples" % res["tested_inputs"])
    print("  flat expr : %s" % res["expr"])
    print("  emitted multi-statement C:\n    %s" % res["emitted_c"].replace("\n", "\n    ").rstrip())
    conf = res.get("confirm", {})
    if conf.get("compiled"):
        v = "EQUIVALENT" if conf.get("equivalent") else "MISMATCH (%d/%d)" % (conf["agree"], conf["total"])
        print("\n  native gcc differential test vs the binary: %s" % v)
    elif conf.get("available") is None:
        print("\n  (%s -- validated against the binary oracle in-process)" % conf.get("note", ""))
    else:
        print("\n  (gcc not found -- validated against the binary oracle in-process only)")


if __name__ == "__main__":
    main()
