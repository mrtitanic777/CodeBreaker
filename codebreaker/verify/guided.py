"""guided.py -- inputs aimed at the branches no input has taken yet.

A function verified on inputs that ran under half of its instructions is "verified (weak)": the
rest was never compared. The fixed inputs (verify/win32.py _vectors, _confirm_vectors) and the
directed ones (constants and globals read off the code) stop at the first test they do not
happen to satisfy. This generator watches each conditional jump of the ORIGINAL function -- which
way it went, and what the comparison before it compared -- and makes inputs for the side not
taken yet:

  * input-to-state: when a value the comparison saw is also in the input (an argument, the value
    imported calls return, a word of the memory handed in), that place gets the value it was
    compared with, one either side of it, or for a zero test the opposite of zero;
  * otherwise the compared-with value is tried in each argument, and a few fixed changes
    (interesting integers, what imported calls return) are tried on the inputs kept so far.

An input is kept when the original then takes a branch direction, or runs an instruction, that
no input before it did. Kept inputs are run on both sides by compare.compare like any other.

    guided_inputs(run, seeds, budget=96, keep=16) -> [input]

`run(input)` traces the original and returns {"branches": [(jump, taken, compare)], "reach":
set of addresses, "args_at": where the first argument is on the stack, "stack": (low, high) of
the stack} or None when it cannot be observed (compare: see emulate/emulator.py _branch_hook). An input is a dict with "args" (a list
of words) and optionally "mem_init" [(address, bytes)] and "stub_ret". Nothing here knows the
processor: verify/win32.py and verify/powerpc.py supply `run`."""
import random

INTERESTING = (0, 1, 2, 0xFFFFFFFF, 0x7FFFFFFF, 0x80000000, 0xFF, 0x100, 0xFFFF, 0x10000, 0x40, 0x8000)


def _edges(obs):
    return {(a, bool(t)) for a, t, _c in obs.get("branches") or ()}


def _copy(v):
    out = dict(v)
    out["args"] = list(v.get("args") or [])
    if v.get("mem_init") is not None:
        out["mem_init"] = list(v["mem_init"])
    return out


def _flips(cmp):
    """(operand, value seen, value wanted): changes to one operand that would send a comparison
    the other way."""
    mn, size, a, b = cmp[:4]
    mask = (1 << (8 * size)) - 1
    if mn in ("test", "and", "or") and a == b:                 # a zero test: zero or not
        return [(0, a, 1 if a == 0 else 0)]
    if mn in ("test", "and"):                                  # a flag test: the bits, set or clear
        return [(0, a, (a | b) & mask), (0, a, (a & ~b) & mask), (1, b, (b | a) & mask)]
    if mn in ("dec", "inc", "add", "xor", "or"):
        return [(0, a, (b if mn != "inc" else -b) & mask), (0, a, 0)]
    out = []
    for k, seen, other in ((0, a, b), (1, b, a)):
        for want in (other, other + 1, other - 1):
            want &= mask
            if want != seen:
                out.append((k, seen, want))
    return out


class _Machine(object):
    def __init__(self, byteorder, word):
        self.order, self.word = byteorder, word
        self.wmask = (1 << (8 * word)) - 1


def _places(v, obs, cmp, k, value, size, mach):
    """Places in input `v` to put a new value for operand `k` of comparison `cmp`: first where
    that operand was read from (an argument's stack slot -- also one past those given -- or any
    other memory the input can set), then every place in the input that holds the value it had."""
    found = []
    at = cmp[4 + k] if len(cmp) > 4 else None
    args_at = obs.get("args_at")
    stack = obs.get("stack") or (0, 0)
    if at is not None:
        w = mach.word
        if args_at is not None and args_at <= at < args_at + w * 32 and (at - args_at) % w + size <= w:
            byte = (at - args_at) % w
            found.append(("arg", ((at - args_at) // w, byte if mach.order == "little" else w - size - byte)))
        elif not stack[0] <= at < stack[1]:                    # (not the function's own frame)
            found.append(("addr", at))
    mask = (1 << (8 * size)) - 1
    for i, x in enumerate(v.get("args") or ()):
        if (x & mask) == value:
            found.append(("arg", (i, 0)))
    if v.get("stub_ret") is not None and (int(v["stub_ret"]) & mask) == value:
        found.append(("stub_ret", None))
    if value and size in (1, 2, 4):
        needle = value.to_bytes(size, mach.order)
        for j, (_addr, data) in enumerate(v.get("mem_init") or ()):
            pos = bytes(data).find(needle)
            hits = 0
            while pos >= 0 and hits < 2:
                found.append(("mem", (j, pos)))
                hits += 1
                pos = bytes(data).find(needle, pos + 1)
    out = []
    for f in found:
        if f not in out:
            out.append(f)
    return out[:6]


def _put(v, place, value, size, mach):
    out = _copy(v)
    mask = (1 << (8 * size)) - 1
    kind, where = place
    if kind == "arg":
        i, byte = where
        while len(out["args"]) <= i:
            out["args"].append(0)
        shift = 8 * byte
        out["args"][i] = (out["args"][i] & ~(mask << shift) & mach.wmask) | ((value & mask) << shift)
    elif kind == "stub_ret":
        out["stub_ret"] = (int(out.get("stub_ret") or 0) & ~mask & mach.wmask) | (value & mask)
    elif kind == "addr":
        out["mem_init"] = list(out.get("mem_init") or []) + [(where, (value & mask).to_bytes(size, mach.order))]
    else:
        j, at = where
        addr, data = out["mem_init"][j]
        data = bytearray(data)
        data[at:at + size] = (value & mask).to_bytes(size, mach.order)
        out["mem_init"][j] = (addr, bytes(data))
    return out


def guided_inputs(run, seeds, budget=96, keep=16, seed=0, byteorder="little", word=4):
    """Inputs (beyond `seeds`) that take branch directions the seeds did not. See the module text.
    `byteorder` and `word` (bytes in an argument) are the processor's."""
    rng = random.Random(seed)
    mach = _Machine(byteorder, word)
    seen_edges, seen_reach = set(), set()
    pool = []                                  # (input, its observation) -- seeds and kept inputs
    for v in seeds:
        o = run(v)
        if o is None:
            continue
        seen_edges |= _edges(o)
        seen_reach |= set(o.get("reach") or ())
        pool.append((v, o))
    if not pool:
        return []
    kept, tried, spent = [], set(), [0]

    def attempt(v):
        k = repr(sorted(v.items()))
        if k in tried or spent[0] >= budget:
            return False
        tried.add(k)
        spent[0] += 1
        o = run(v)
        if o is None:
            return False
        new_e = _edges(o) - seen_edges
        new_r = set(o.get("reach") or ()) - seen_reach
        if not new_e and not new_r:
            return False
        seen_edges.update(new_e)
        seen_reach.update(new_r)
        kept.append(v)
        pool.append((v, o))
        return True

    def frontier():
        """Jumps seen going one way only, each with the inputs (and comparisons) that reached it."""
        taken = {}
        for a, t in seen_edges:
            taken.setdefault(a, set()).add(t)
        out = {}
        for v, o in pool:
            for a, t, c in o.get("branches") or ():
                if len(taken.get(a, ())) == 1:
                    out.setdefault(a, []).append((v, o, c))
        return out

    progress = True
    while progress and spent[0] < budget and len(kept) < keep:
        progress = False
        for a, sites in sorted(frontier().items()):
            if spent[0] >= budget or len(kept) >= keep:
                break
            # the first few inputs that reached this jump, each with what it compared
            for v, o, c in sites[:3]:
                if c is None:
                    continue
                size = c[1]
                done = False
                for k, was, want in _flips(c):
                    places = _places(v, o, c, k, was, size, mach)
                    if not places:                         # not in the input as it is: try each argument
                        places = [("arg", (i, 0)) for i in range(len(v.get("args") or ()))]
                    for place in places:
                        if attempt(_put(v, place, want, size, mach)):
                            progress = done = True
                            break
                    if done or spent[0] >= budget:
                        break
                if done:
                    break
        if not progress and spent[0] < budget:
            # nothing above opened anything: fixed changes to inputs that are known to run
            for _ in range(min(12, budget - spent[0])):
                v, _o = pool[rng.randrange(len(pool))]
                w = _copy(v)
                r = rng.random()
                if r < 0.25:
                    w["stub_ret"] = rng.choice((0, 1, mach.wmask, 2))
                elif w["args"]:
                    i = rng.randrange(len(w["args"]))
                    w["args"][i] = rng.choice(INTERESTING) if r < 0.75 else (w["args"][i] ^ (1 << rng.randrange(8 * word)))
                if attempt(w):
                    progress = True
                if len(kept) >= keep:
                    break
    return kept
