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

from engine.behavior_engine import tracer, perturb

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


def _oracle(module, func, arg_index, arity, x, insn_cap=2000):
    args = [0] * arity
    if arg_index is not None:
        args[arg_index] = x & MASK
    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap)
    return t["ret"] if t["exit"] == "ret" else None


def _candidacy(module, func, insn_cap=3000):
    """Return (ok, info). info carries arity, the input arg index (or None for constant),
    and a reason when not ok."""
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    if base["exit"] != "ret":
        return False, {"reason": "does not return cleanly (%s)" % base["exit"], "base": base}
    if base.get("calls"):
        return False, {"reason": "calls other functions (not a leaf) -- out of v1 grammar scope",
                       "base": base}
    _, report = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
    affect = [a["index"] for a in report["args"]
              if "ret" in a["influence"] or any(h["as"] == "return_value" for h in a["dataflow"])]
    ptr = [a["index"] for a in report["args"]
           if any(h["as"] in ("read_address", "write_address") for h in a["dataflow"])]
    info = {"arity": report["arity"], "affect": affect, "ptr": ptr,
            "func_va": base["func_va"], "func_off": base["func_off"], "base": base}
    if ptr:
        return False, dict(info, reason="has pointer argument(s) %s -- out of v1 grammar scope" % ptr)
    if len(affect) > 1:
        return False, dict(info, reason="return depends on multiple args %s -- v1 does one" % affect)
    info["input"] = affect[0] if affect else None
    return True, info


def synthesize(module, func=None, arg_of=None, trials=40, insn_cap=2000):
    ok, info = _candidacy(module, func, insn_cap)
    result = {"schema": "cb.behavior.synth/1", "module": os.path.basename(info["base"]["module"]),
              "func_va": info["base"]["func_va"], "func_off": info["base"]["func_off"],
              "candidate": bool(ok)}
    if not ok:
        result["reason"] = info["reason"]
        return result
    arity = info["arity"]
    xi = arg_of if arg_of is not None else info["input"]
    # 1. sample the oracle on a diverse, fixed input set
    seeds = [0, 1, 2, 3, 4, 7, 8, 10, 100, 255, 256, 0x100, 0x1234, 0xFFFF,
             0x10000, 0x12345678, 0xABCDEF01, 0xFF00FF00, 0x80000000, 0xFFFFFFFF]
    obs = {}
    for x in seeds:
        r = _oracle(module, func, xi, arity, x, insn_cap)
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
    ex_obs = {x: _oracle(module, func, xi, arity, x, insn_cap) for x in extra}
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
                                         module, func, insn_cap)
    return result


def _gcc_confirm(csrc, fname, xi, arity, module, func, insn_cap):
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
        o = _oracle(module, func, xi, arity, x, insn_cap)
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
def _opt(a, name, d=None):
    return a[a.index(name) + 1] if name in a and a.index(name) + 1 < len(a) else d


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
    print("  input arg : %s   arity: %d   tested on %d inputs" %
          (("arg%d" % res["input_arg"]) if res["input_arg"] is not None else "(none/constant)",
           res["arity"], res["tested_inputs"]))
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
