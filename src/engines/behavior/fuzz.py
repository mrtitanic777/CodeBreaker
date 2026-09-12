#!/usr/bin/env python3
r"""fuzz.py -- coverage-guided input discovery on the Unicorn oracle (dynamic cluster).

explore.py drives a PURPOSEFUL value ladder (one arg at a time, each value aimed at a
known latent role). This is its complement: a coverage-guided search that mutates the
whole argument vector to DISCOVER paths the ladder never reaches -- the branches behind
input combinations no single-arg probe trips.

Doctrine fit: fuzzing is a DISCOVERY front-end, not a source of truth. The randomness is
SEEDED (default 0), so a session is fully reproducible, and the only thing kept is the
recorded trace (deterministic evidence), never the RNG. A discovered input is a witness:
"these args reach this coverage" -- an observation, exactly like every other trace.

Coverage = executed basic-block heads inside the function + resolved indirect-call
targets + external API dependencies + the exit kind. An input is kept in the corpus only
if it reveals NEW coverage; kept inputs are then mutated further (an evolutionary,
coverage-feedback loop). The discovered input corpus feeds `fracture` (which reads the
resolved indirect targets) and `exectree` (which unions the observed edges).

    wt behavior fuzz <module> [func] [--arity N] [--budget N] [--seed S] [--insns N]
"""
import random
import sys

from src.engines.behavior import tracer, corpus
from src.cli.common import opt as _opt

# Values that trip the common latent roles (zero-guard, sign, pointer-into-scratch, bounds).
_INTERESTING = [0, 1, 0xFFFFFFFF, 0x7FFFFFFF, 0x80000000, 2, 16, 0x100, 0x10000,
                0x204000, 0x204100, 0xDEAD0000]

# Per-arg pointer buffers inside the tracer's scratch window (0x200000..0x210000). Pointing an
# arg here and fuzzing the BUFFER's bytes is what reaches the field-compares in pointer/struct
# functions (RECT/POINT/... -- the majority of interesting Win32 APIs), which scalar-only arg
# mutation never trips (the lesson from fuzzing user32!SubtractRect).
_BUFBASE, _BUFSTRIDE, _BUFLEN = 0x204000, 0x400, 64


def _ptr(i):
    return _BUFBASE + i * _BUFSTRIDE


def _seed_buf(rng):
    """A 64-byte buffer of small ints + interesting dwords, so a struct's field compares vary."""
    b = bytearray(_BUFLEN)
    for off in range(0, _BUFLEN, 4):
        v = rng.choice([0, 1, 2, 0x10, 0xFFFFFFFF, rng.getrandbits(32), rng.randrange(256)])
        b[off:off + 4] = int(v & 0xFFFFFFFF).to_bytes(4, "little")
    return bytes(b)


def _coverage(rec):
    """The coverage signature of a trace: every executed block head (reaching new code
    ANYWHERE is coverage -- a function's real body often spans beyond its carved range,
    e.g. jump-table arms in a shared handler region), resolved indirect-call targets,
    API deps, and the exit kind. A set of short tags."""
    cov = set()
    for va in rec.get("path", []):
        cov.add("b:%x" % va)
    for c in rec.get("calls", []):
        rt = c.get("resolved_target")
        if c.get("indirect") and rt is not None:
            cov.add("i:%x->%x" % (c["pc"], rt))
    for api in rec.get("api_deps", []):
        cov.add("a:%s" % api)
    cov.add("x:%s" % rec.get("exit"))
    return cov


def _mutate(inp, rng):
    """Yield input variants of {'args':[...], 'mem':{addr:bytes}} -- deterministic given the
    seeded rng. Three mutation kinds: (a) a scalar arg set to an interesting value / bit flip /
    random dword; (b) POINTERIZE an arg -- aim it at a fresh scratch buffer and seed the buffer;
    (c) mutate the bytes of an already-pointed buffer (reach the struct-field compares)."""
    args, mem = inp["args"], inp["mem"]
    n = len(args)
    for i in range(n):
        for v in _INTERESTING:                            # (a) scalar arg mutations
            m = list(args); m[i] = v; yield {"args": m, "mem": dict(mem)}
        m = list(args); m[i] ^= (1 << rng.randrange(32)); yield {"args": m, "mem": dict(mem)}
        m = list(args); m[i] = rng.getrandbits(32); yield {"args": m, "mem": dict(mem)}
        p = _ptr(i)                                       # (b) pointerize arg i
        m2 = list(args); m2[i] = p
        mm = dict(mem); mm[p] = _seed_buf(rng)
        yield {"args": m2, "mem": mm}
    for addr, buf in mem.items():                         # (c) mutate a pointed buffer's fields
        for _ in range(2):
            b = bytearray(buf); off = rng.randrange(0, len(b), 4)
            b[off:off + 4] = int(rng.choice(_INTERESTING) & 0xFFFFFFFF).to_bytes(4, "little")
            mm = dict(mem); mm[addr] = bytes(b)
            yield {"args": list(args), "mem": mm}


def fuzz(module, func=None, arity=None, budget=400, seed=0, insn_cap=4000,
         patches=None, stub_externals=None, stub_ret=0):
    """Coverage-guided fuzz of one function. Returns a session report; records each
    novel-coverage trace raw and the session under derived/fuzz.

    `patches` (list of (va, bytes)) are written over the code before every run -- the
    gate-busting hook (strategy 2): a 1-byte condition inversion on a handle-check jcc
    lets the fuzzer past a validation gate the bare oracle can't otherwise satisfy.
    `stub_externals`/`stub_ret` control import stubbing (a nonzero stub_ret makes an
    external validation call report 'valid', passing API-gate checks)."""
    rng = random.Random(seed)
    patches = list(patches or [])

    def _trace(args, mem):
        mem_init = list(mem) + patches                    # code patches applied after args' memory
        return tracer.trace_function(module, func=func, args=args, mem_init=mem_init,
                                     insn_cap=insn_cap, stub_externals=stub_externals, stub_ret=stub_ret)

    # baseline establishes the func range + arity, and is the first corpus seed.
    base = tracer.trace_function(module, func=func, arity=arity, insn_cap=insn_cap,
                                 mem_init=patches, stub_externals=stub_externals, stub_ret=stub_ret)
    corpus.record(base)
    lo, hi = base["func_va"], base["func_end_va"]
    n = base["input"]["arity"]
    global_cov = _coverage(base)
    seed_inp = {"args": list(base["input"]["args"]), "mem": {}}
    corpus_inputs = [seed_inp]
    queue = list(_mutate(seed_inp, rng))
    tried = 1
    novel = []                                            # inputs that each added coverage
    while queue and tried < budget:
        inp = queue.pop(0)
        tried += 1
        try:
            rec = _trace(inp["args"], list(inp["mem"].items()))
        except Exception:
            continue
        cov = _coverage(rec)
        new = cov - global_cov
        if new:
            global_cov |= cov
            corpus.record(rec)                            # keep the witness trace (raw evidence)
            corpus_inputs.append(inp)
            novel.append({"args": [int(a) & 0xFFFFFFFF for a in inp["args"]],
                          "ptrs": sorted("0x%x" % a for a in inp["mem"]),
                          "new_cov": len(new), "ret": rec.get("ret"), "exit": rec.get("exit")})
            queue.extend(_mutate(inp, rng))               # evolve from a productive input
    session = {
        "schema": "cb.behavior.fuzz/1", "module": base["module"],
        "func_va": base["func_va"], "func_off": base["func_off"], "arity": n,
        "seed": seed, "budget": budget, "traced": tried,
        "blocks": sum(1 for t in global_cov if t.startswith("b:")),
        "indirect_targets": sum(1 for t in global_cov if t.startswith("i:")),
        "api_deps": sorted(t[2:] for t in global_cov if t.startswith("a:")),
        "exits": sorted(t[2:] for t in global_cov if t.startswith("x:")),
        "corpus": [{"args": [int(a) & 0xFFFFFFFF for a in c["args"]],
                    "mem": {"0x%x" % a: b.hex() for a, b in c["mem"].items()}} for c in corpus_inputs],
        "novel": novel,
    }
    corpus.record_derived("fuzz", session)
    return session


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(main.__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")
                 and argv[argv.index(a) - 1] not in ("--arity", "--budget", "--seed", "--insns")), None)
    arity = _opt(argv, "--arity"); arity = int(arity) if arity is not None else None
    budget = int(_opt(argv, "--budget", 400))
    seed = int(_opt(argv, "--seed", 0))
    insns = int(_opt(argv, "--insns", 4000))
    s = fuzz(module, func=func, arity=arity, budget=budget, seed=seed, insn_cap=insns)
    print("fuzz %s  func VA 0x%x  (arity %d, seed %d)" % (s["module"], s["func_va"], s["arity"], s["seed"]))
    print("  traced %d inputs -> %d blocks, %d resolved indirect target(s), %d exit kind(s)"
          % (s["traced"], s["blocks"], s["indirect_targets"], len(s["exits"])))
    if s["api_deps"]:
        print("  API deps discovered: %s" % ", ".join(s["api_deps"][:12]))
    print("  coverage-growing inputs kept: %d" % len(s["corpus"]))
    for nv in s["novel"][:8]:
        ptrs = ("  ptr@%s" % ",".join(nv["ptrs"])) if nv.get("ptrs") else ""
        print("     args=[%s]%s +%d cov  ret=%s exit=%s"
              % (", ".join("0x%x" % a for a in nv["args"]), ptrs, nv["new_cov"], nv["ret"], nv["exit"]))
    print("  -> feeds `wt behavior fracture` (indirect targets) and `wt behavior exectree`")


if __name__ == "__main__":
    main()
