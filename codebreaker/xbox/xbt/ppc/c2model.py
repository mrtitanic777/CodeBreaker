"""c2 codegen predictor — how the MSVC Xbox 360 back end (`c2.dll`) allocates
registers and orders the stores of a constant store-run.

Matching decompilation needs to know what byte sequence the compiler will emit
for a given source shape *without* running it. This encodes the two settled,
holdout-verified models of that behaviour for the common case (a run of stores
whose values are single-word literals):

  ALLOC — which register each value-producer gets.
  ORDER — the order the stores come out in when the head slots are contested.

Both are reverse-engineered black-box (probe grids graded against the real
compiler's own `/FAsc` listing), reproduced here as a predictor our `oracle` /
`permute` can consult. It also encodes the *refusal boundaries*: the shapes the
reference port itself will not predict (mixed producer kinds, a multiply, more
than three producers, or a wide literal beside another producer) — for those,
predict() returns None and the caller must fall back to compiling.

Knowledge ported from c2-rs docs/ALLOC.md and docs/ORDER.md.
"""

# Free volatile pool, handed out highest-first; r12 is never allocated.
_POOL = [11, 10, 9, 8, 7, 6, 5, 4]


def _producers(store_values):
    """value -> (use_count, first_source_index), in first-appearance order."""
    prod = {}
    for i, v in enumerate(store_values):
        if v not in prod:
            prod[v] = [0, i]
        prod[v][0] += 1
    return prod


def refuse_reason(store_values, kinds=None):
    """Return why this run is unpredictable, or None if ALLOC/ORDER apply.
    `kinds` maps value -> 'const' | 'reg' | 'mul' (default: all constant)."""
    prod = _producers(store_values)
    if len(prod) > 3:
        return "more than three producers (register reuse is open)"
    if kinds:
        ks = {kinds.get(v, "const") for v in prod}
        if "mul" in ks:
            return "a multiply producer (its own regime)"
        if "reg" in ks and "const" in ks:
            return "mixed register-derived and constant producers"
        if ks == {"reg"} and len(prod) > 1:
            return "multiple register-derived producers (allocation open)"
    return None


def allocate(store_values, kinds=None):
    """ALLOC: {value: 'rN'} for a constant store-run, or None if it is a shape
    the model refuses. Sort producers by use-count descending; break a count-1
    tie in source order and a count>=2 tie in REVERSE source order (the sign
    genuinely flips with the count); assign r11, r10, r9, ... in that order."""
    if refuse_reason(store_values, kinds) is not None:
        return None
    prod = _producers(store_values)
    items = [(v, c, f) for v, (c, f) in prod.items()]
    items.sort(key=lambda it: (-it[1], it[2] if it[1] == 1 else -it[2]))
    if len(items) > len(_POOL):
        return None
    return {v: "r%d" % _POOL[i] for i, (v, _c, _f) in enumerate(items)}


def producer_rank(store_values):
    """Rank producers by (use-count descending, first-use source index
    ascending). Returns {value: rank}. Drives ORDER's store placement."""
    prod = _producers(store_values)
    items = sorted(prod.items(), key=lambda kv: (-kv[1][0], kv[1][1]))
    return {v: r for r, (v, _cf) in enumerate(items)}


def store_order(store_values, unproduced=None):
    """ORDER: the source indices of the stores in the order c2 emits them.

    `unproduced` is the set of source indices whose store needs no producer
    (e.g. a formal already live); such stores are never blocked. A store whose
    producer has rank j may not occupy a position < u + j, where u is the
    leading run of unproduced stores (capped at 2). Walk the source in order and
    emit the earliest still-allowed store for each output position."""
    unproduced = set(unproduced or ())
    rank = producer_rank([v for i, v in enumerate(store_values) if i not in unproduced])
    u = 0
    for i in range(len(store_values)):
        if i in unproduced and u < 2:
            u += 1
        else:
            break
    remaining = list(range(len(store_values)))
    out = []
    for pos in range(len(store_values)):
        for k, src in enumerate(remaining):
            if src in unproduced:
                floor = 0
            else:
                floor = u + rank[store_values[src]]
            if pos >= floor:
                out.append(src)
                del remaining[k]
                break
    return out


def predict(store_values, kinds=None, unproduced=None):
    """Full prediction for a constant store-run: the emit order of the stores
    and the register each value lands in — or None if the run is a refused
    shape. Returns {"order": [src...], "regs": {value: 'rN'}}."""
    regs = allocate(store_values, kinds)
    if regs is None:
        return None
    return {"order": store_order(store_values, unproduced), "regs": regs}


DESCRIPTION = "predict c2's register allocation + store order for a constant store-run (matching decomp)"


def main():
    import sys
    a = sys.argv[1:]
    if a and a[0] == "--selftest":
        # The killer cells from ALLOC.md §1.1 and ORDER.md / ALLOC §6.
        alloc_cells = [
            ([1, 2, 3, 1], {1: "r11", 2: "r10", 3: "r9"}),
            ([1, 2, 3, 2, 1], {1: "r10", 2: "r11", 3: "r9"}),
            ([1, 2, 1, 2], {1: "r10", 2: "r11"}),
            ([1, 1, 2, 2, 2], {1: "r10", 2: "r11"}),
        ]
        order_cells = [
            ([1, 2, 2], [1, 0, 2]),
            ([1, 2, 1, 2], [0, 1, 2, 3]),
            ([1, 1, 2, 2, 2], [2, 0, 1, 3, 4]),
        ]
        bad = 0
        for sv, want in alloc_cells:
            got = allocate(sv)
            if got != want:
                bad += 1
                print("  ALLOC FAIL %s: %s != %s" % (sv, got, want))
        for sv, want in order_cells:
            got = store_order(sv)
            if got != want:
                bad += 1
                print("  ORDER FAIL %s: %s != %s" % (sv, got, want))
        print("c2model self-test: %s (%d ALLOC + %d ORDER cells)" % (
            "OK" if not bad else "%d FAILED" % bad, len(alloc_cells), len(order_cells)))
        sys.exit(1 if bad else 0)
    print("usage: c2model --selftest   (verify the ALLOC/ORDER models)\n"
          "  library: allocate(store_values) / store_order(...) / predict(...)")


if __name__ == "__main__":
    main()
