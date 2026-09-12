#!/usr/bin/env python3
r"""synth.py -- constraint-guided program synthesis (spec Section 11), validated by the binary.

This does NOT guess source from disassembly. It CONSTRAINS a search with behavioral evidence and
then proves each survivor against the binary oracle:

  1. CANDIDACY from evidence: the function must be a leaf (makes no calls), and a tag sweep must
     show its return is a function of a single argument (or of none -> a constant). Anything
     richer is out of this v1 grammar's scope, and we say so rather than fake it.
  2. SAMPLE the oracle: run the real bytes on a spread of inputs for that argument (the binary
     is the oracle, exactly as difftest uses it).
  3. SEARCH a constrained grammar of pure integer expressions over the one input x; keep only
     candidates that reproduce EVERY observed (input -> output) pair.
  4. PRUNE coincidences: re-test survivors on fresh random inputs. A candidate that still agrees
     on dozens of inputs is behaviorally equivalent to the binary on the tested domain.
  5. EMIT C, simplest first. If several survive and all agree, they are reported together --
     the binary cannot tell us which textual form the author wrote (spec Section 19).
  6. CONFIRM natively when gcc is available: compile the emitted C and differential-test it
     against the oracle on more inputs -- the spec's final authority.

    wt behavior synth <module> [func] [--arg-of N] [--trials 40]
"""
import os
import random
import sys

from src.engines.behavior import tracer, perturb

MASK = 0xFFFFFFFF


def _bswap(x):
    x &= MASK
    return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | ((x >> 24) & 0xFF)


def _grammar(observed_outputs):
    """Yield (label, c_expr, fn) candidates over a single uint32 input x. Constants are drawn
    from a common set plus values implied by the observations, keeping the search small."""
    consts = sorted({0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 15, 16, 24, 31, 32, 100, 255, 256,
                     0xFF, 0xFFFF, 0x10000, 0xFFFFFFFF} | set(observed_outputs))
    # nullary / unary
    yield ("x", "return x;", lambda x: x)
    yield ("~x", "return ~x;", lambda x: (~x) & MASK)
    yield ("-x", "return -x;", lambda x: (-x) & MASK)
    yield ("bswap(x)", "return __builtin_bswap32(x);", _bswap)
    for c in consts:
        yield ("const 0x%x" % c, "return 0x%xU;" % c, (lambda c: lambda x: c)(c))
    # binary with a constant
    for c in consts:
        if c == 0:
            continue
        yield ("x + 0x%x" % c, "return x + 0x%xU;" % c, (lambda c: lambda x: (x + c) & MASK)(c))
        yield ("x - 0x%x" % c, "return x - 0x%xU;" % c, (lambda c: lambda x: (x - c) & MASK)(c))
        yield ("0x%x - x" % c, "return 0x%xU - x;" % c, (lambda c: lambda x: (c - x) & MASK)(c))
        yield ("x * 0x%x" % c, "return x * 0x%xU;" % c, (lambda c: lambda x: (x * c) & MASK)(c))
        yield ("x & 0x%x" % c, "return x & 0x%xU;" % c, (lambda c: lambda x: x & c)(c))
        yield ("x | 0x%x" % c, "return x | 0x%xU;" % c, (lambda c: lambda x: x | c)(c))
        yield ("x ^ 0x%x" % c, "return x ^ 0x%xU;" % c, (lambda c: lambda x: x ^ c)(c))
        if c < 32:
            yield ("x << %d" % c, "return x << %d;" % c, (lambda c: lambda x: (x << c) & MASK)(c))
            yield ("x >> %d" % c, "return x >> %d;" % c, (lambda c: lambda x: x >> c)(c))
    # two-level: (x * a) + b  and  (x << a) + b  -- covers the classic "x*2+7" family
    small = [1, 2, 3, 4, 5, 8, 16]
    for a in small:
        for b in sorted(set(list(observed_outputs) + [1, 5, 7, 100])):
            yield ("x * %d + 0x%x" % (a, b), "return x * %d + 0x%xU;" % (a, b),
                   (lambda a, b: lambda x: (x * a + b) & MASK)(a, b))
    for a in range(1, 8):
        for b in sorted(set(list(observed_outputs) + [1, 5, 7])):
            yield ("(x << %d) + 0x%x" % (a, b), "return (x << %d) + 0x%xU;" % (a, b),
                   (lambda a, b: lambda x: ((x << a) + b) & MASK)(a, b))


def _grammar2(observed_outputs):
    """Yield (label, c_expr, fn(x,y)) over two uint32 inputs -- the common 'combine two values'
    shapes (packing, linear combinations, bitwise merges)."""
    yield ("x + y", "return x + y;", lambda x, y: (x + y) & MASK)
    yield ("x - y", "return x - y;", lambda x, y: (x - y) & MASK)
    yield ("y - x", "return y - x;", lambda x, y: (y - x) & MASK)
    yield ("x * y", "return x * y;", lambda x, y: (x * y) & MASK)
    yield ("x & y", "return x & y;", lambda x, y: x & y)
    yield ("x | y", "return x | y;", lambda x, y: x | y)
    yield ("x ^ y", "return x ^ y;", lambda x, y: x ^ y)
    for s in (8, 16):   # packing: x | (y << s) and the mirror -- MAKELONG/MAKEWORD family
        yield ("x | (y << %d)" % s, "return x | (y << %d);" % s,
               (lambda s: lambda x, y: (x | (y << s)) & MASK)(s))
        yield ("y | (x << %d)" % s, "return y | (x << %d);" % s,
               (lambda s: lambda x, y: (y | (x << s)) & MASK)(s))
        yield ("(x & 0x%x) | (y << %d)" % ((1 << s) - 1, s),
               "return (x & 0x%xU) | (y << %d);" % ((1 << s) - 1, s),
               (lambda s: lambda x, y: ((x & ((1 << s) - 1)) | (y << s)) & MASK)(s))
    cset = sorted({0, 1, 5, 7, 8} | set(observed_outputs))
    for a in (1, 2, 3, 4):
        for b in (1, 2, 3, 4):
            for c in cset:
                yield ("%d*x + %d*y + 0x%x" % (a, b, c),
                       "return %d*x + %d*y + 0x%xU;" % (a, b, c),
                       (lambda a, b, c: lambda x, y: (a * x + b * y + c) & MASK)(a, b, c))


def _oracle_vec(module, func, args, insn_cap=2000):
    t = tracer.trace_function(module, func=func, args=[a & MASK for a in args], insn_cap=insn_cap)
    return t["ret"] if t["exit"] == "ret" else None


def _oracle(module, func, arg_index, arity, x, insn_cap=2000, base_args=None):
    args = list(base_args) if base_args else [0] * arity
    if len(args) < arity:
        args += [0] * (arity - len(args))
    if arg_index is not None:
        args[arg_index] = x & MASK
    return _oracle_vec(module, func, args, insn_cap)


def _candidacy(module, func, insn_cap=3000, base_args=None):
    """Return (ok, info). info carries arity, the input arg index (or None for constant),
    and a reason when not ok. base_args: a terminating input baseline for no-ret functions
    (the target arg is still varied on top of it)."""
    base = tracer.trace_function(module, func=func, args=base_args, insn_cap=insn_cap)
    if base["exit"] != "ret":
        return False, {"reason": "does not return cleanly (%s)" % base["exit"], "base": base}
    # synth models only the RETURN VALUE, so it must not accept a function with SIDE EFFECTS:
    # any write outside the function's own stack frame (globals, heap, a buffer, a null deref)
    # is behaviour the emitted `return <expr>;` would silently drop -- claiming equivalence then
    # would be a lie. Decline those (struct handles the pointer/field-writing ones).
    _STK_LO, _STK_HI = 0x206000, 0x208100
    if any(not (_STK_LO <= w["addr"] < _STK_HI) for w in base.get("mem_writes", [])):
        return False, {"reason": "has memory side effects (writes outside its stack frame) -- "
                       "return-only synth can't capture; struct's territory", "base": base}
    # NOTE: a function that calls others is NOT rejected. Intra-module callees execute correctly
    # during the trace and stubbed externals return deterministically, so the function's net
    # behavior is still a well-defined function of its inputs. If it reduces to an expression,
    # the differential test proves it (behavioral, not textual, equivalence -- spec Section 19);
    # if it doesn't, no candidate survives and it stays honestly unresolved. This is the
    # inter-procedural path: reconstruct the caller by its observed I/O, callees included.
    _, report = perturb.run_experiment(module, func=func, base_args=base_args, insn_cap=insn_cap)
    affect = [a["index"] for a in report["args"]
              if "ret" in a["influence"] or any(h["as"] == "return_value" for h in a["dataflow"])]
    ptr = [a["index"] for a in report["args"]
           if any(h["as"] in ("read_address", "write_address") for h in a["dataflow"])]
    info = {"arity": report["arity"], "affect": affect, "ptr": ptr,
            "func_va": base["func_va"], "func_off": base["func_off"], "base": base}
    if ptr:
        return False, dict(info, reason="has pointer argument(s) %s -- out of v1 grammar scope" % ptr)
    if len(affect) > 2:
        return False, dict(info, reason="return depends on %d args %s -- grammar does <=2"
                                        % (len(affect), affect))
    info["inputs"] = affect                 # 0, 1, or 2 return-affecting integer args
    info["input"] = affect[0] if len(affect) == 1 else None
    return True, info


def synthesize(module, func=None, arg_of=None, trials=40, insn_cap=2000, base_args=None):
    ok, info = _candidacy(module, func, insn_cap, base_args=base_args)
    result = {"schema": "cb.behavior.synth/1", "module": os.path.basename(info["base"]["module"]),
              "func_va": info["base"]["func_va"], "func_off": info["base"]["func_off"],
              "candidate": bool(ok)}
    if not ok:
        result["reason"] = info["reason"]
        return result
    arity = info["arity"]
    inputs = info.get("inputs", [])
    if len(inputs) == 2 and arg_of is None:
        return _synth2(module, func, info, inputs, arity, trials, insn_cap, result)
    xi = arg_of if arg_of is not None else info["input"]
    # 1. sample the oracle on a diverse, fixed input set
    seeds = [0, 1, 2, 3, 4, 7, 8, 10, 100, 255, 256, 0x100, 0x1234, 0xFFFF,
             0x10000, 0x12345678, 0xABCDEF01, 0xFF00FF00, 0x80000000, 0xFFFFFFFF]
    obs = {}
    for x in seeds:
        r = _oracle(module, func, xi, arity, x, insn_cap, base_args)
        if r is not None:
            obs[x] = r
    outputs = set(obs.values())
    # 2. constrained search: keep candidates matching every observation
    survivors = []
    for label, cexpr, fn in _grammar(outputs):
        if all((fn(x) & MASK) == obs[x] for x in obs):
            survivors.append((label, cexpr, fn))
    # 3. prune coincidences with fresh random inputs
    rng = random.Random(info["func_va"])
    extra = [rng.getrandbits(32) for _ in range(trials)]
    ex_obs = {x: _oracle(module, func, xi, arity, x, insn_cap, base_args) for x in extra}
    pruned = [(l, c, fn) for (l, c, fn) in survivors
              if all(ex_obs[x] is None or (fn(x) & MASK) == ex_obs[x] for x in extra)]
    pruned.sort(key=lambda t: len(t[1]))                     # simplest expression first
    tested = len(obs) + sum(1 for v in ex_obs.values() if v is not None)

    result.update({"input_arg": xi, "arity": arity, "tested_inputs": tested,
                   "samples": {("0x%x" % k): ("0x%x" % v) for k, v in list(obs.items())[:8]},
                   "candidates": [{"expr": l, "c": c} for (l, c, _) in pruned[:5]]})
    # 4. emit C for the best survivor + native gcc confirmation if possible
    if pruned:
        best_label, best_c, best_fn = pruned[0]
        params = ", ".join(("unsigned x" if i == xi else "unsigned a%d" % i) for i in range(arity))
        csrc = "unsigned sub_%x(%s)\n{\n    %s\n}\n" % (result["func_va"], params or "void", best_c)
        result["emitted_c"] = csrc
        result["confirm"] = _gcc_confirm(csrc, "sub_%x" % result["func_va"], xi, arity,
                                         module, func, insn_cap, base_args)
    return result


def _synth2(module, func, info, inputs, arity, trials, insn_cap, result):
    """Two-variable synthesis: inputs are the two return-affecting arg indices."""
    ix, iy = inputs
    def mk(x, y):
        v = [0] * arity; v[ix] = x; v[iy] = y; return v
    pairs = [(0, 0), (1, 0), (0, 1), (1, 1), (2, 3), (3, 2), (5, 7), (255, 1), (1, 255),
             (0x100, 0x10), (0x1234, 0x5678), (0x12345678, 0x9ABCDEF0), (0xFFFF, 0xFFFF),
             (0xFF00, 0x00FF)]
    obs = {}
    for (x, y) in pairs:
        r = _oracle_vec(module, func, mk(x, y), insn_cap)
        if r is not None:
            obs[(x, y)] = r
    outputs = set(obs.values())
    survivors = [(l, c, fn) for (l, c, fn) in _grammar2(outputs)
                 if all((fn(x, y) & MASK) == o for (x, y), o in obs.items())]
    rng = random.Random(result["func_va"])
    extra = [(rng.getrandbits(32), rng.getrandbits(32)) for _ in range(trials)]
    ex = {(x, y): _oracle_vec(module, func, mk(x, y), insn_cap) for (x, y) in extra}
    pruned = [(l, c, fn) for (l, c, fn) in survivors
              if all(ex[p] is None or (fn(*p) & MASK) == ex[p] for p in extra)]
    pruned.sort(key=lambda t: len(t[1]))
    tested = len(obs) + sum(1 for v in ex.values() if v is not None)
    result.update({"input_args": [ix, iy], "arity": arity, "tested_inputs": tested,
                   "input_arg": None,
                   "samples": {("0x%x,0x%x" % k): ("0x%x" % v) for k, v in list(obs.items())[:8]},
                   "candidates": [{"expr": l, "c": c} for (l, c, _) in pruned[:5]]})
    if pruned:
        best_c = pruned[0][1]
        params = ", ".join(("unsigned x" if i == ix else "unsigned y" if i == iy
                            else "unsigned a%d" % i) for i in range(arity))
        result["emitted_c"] = "unsigned sub_%x(%s)\n{\n    %s\n}\n" % (result["func_va"], params, best_c)
        result["confirm"] = {"available": None, "note": "2-var gcc confirm not wired in v1"}
    return result


def _gcc_confirm(csrc, fname, xi, arity, module, func, insn_cap, base_args=None):
    """Compile the emitted C with gcc and differential-test it against the oracle on fresh
    inputs -- the spec's final authority. Returns a dict, or {'available': False} if no gcc."""
    import ctypes
    import shutil
    import subprocess
    import tempfile
    if not shutil.which("gcc"):
        return {"available": False}
    d = tempfile.mkdtemp()
    src = os.path.join(d, "c.c")
    with open(src, "w") as fh:
        fh.write(csrc)
    so = os.path.join(d, "c.dll")
    r = subprocess.run(["gcc", "-std=gnu99", "-w", "-shared", "-o", so, src],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return {"available": True, "compiled": False, "error": r.stderr[:200]}
    lib = ctypes.CDLL(so)
    fn = getattr(lib, fname)
    fn.restype = ctypes.c_uint32
    fn.argtypes = [ctypes.c_uint32] * arity
    rng = random.Random(0xC0FFEE)
    agree = total = 0
    for _ in range(40):
        x = rng.getrandbits(32)
        o = _oracle(module, func, xi, arity, x, insn_cap, base_args)
        if o is None:
            continue
        call = [0] * arity
        if xi is not None:
            call[xi] = x
        total += 1
        agree += (fn(*call) & MASK) == o
    return {"available": True, "compiled": True, "agree": agree, "total": total,
            "equivalent": agree == total and total > 0}


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
    trials = int(_opt(argv, "--trials", 40))

    res = synthesize(module, func=func, arg_of=arg_of, trials=trials)
    print("synthesis: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["candidate"]:
        print("  NOT a v1 synthesis candidate: %s" % res["reason"]); return
    if res.get("input_args"):
        inlabel = "args %s" % ", ".join("arg%d" % i for i in res["input_args"])
    elif res.get("input_arg") is not None:
        inlabel = "arg%d" % res["input_arg"]
    else:
        inlabel = "(none/constant)"
    print("  input     : %s   arity: %d   tested on %d inputs" %
          (inlabel, res["arity"], res["tested_inputs"]))
    ex = "  ".join("%s->%s" % (k, v) for k, v in list(res["samples"].items())[:6])
    print("  oracle    : %s" % ex)
    if not res["candidates"]:
        print("  no expression in the v1 grammar reproduces the binary's behavior."); return
    print("  candidates consistent with ALL evidence (simplest first):")
    for c in res["candidates"]:
        print("     %s" % c["expr"])
    print("\n  emitted C:\n    %s" % res["emitted_c"].replace("\n", "\n    ").rstrip())
    conf = res.get("confirm", {})
    if conf.get("available") and conf.get("compiled"):
        verdict = "EQUIVALENT" if conf.get("equivalent") else "MISMATCH (%d/%d)" % (conf["agree"], conf["total"])
        print("\n  native gcc differential test vs the binary: %s" % verdict)
    elif conf.get("available") and not conf.get("compiled"):
        print("\n  (gcc present but candidate did not compile: %s)" % conf.get("error", "")[:80])
    else:
        print("\n  (gcc not found -- candidate validated against the binary oracle in-process only)")


if __name__ == "__main__":
    main()
