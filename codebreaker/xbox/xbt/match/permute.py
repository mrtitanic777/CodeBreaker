#!/usr/bin/env python3
"""
permute.py — the permuter: deterministic matching-decompilation search (stage F).

Given a reconstructed function whose bytes don't yet match the shipped bytes,
mechanically try semantics-preserving variations of the C until the `segdiff`
verdict is byte-exact. This is the decomp-scene "matching decompilation"
technique made deterministic: no LLM, no randomness — a bounded, ordered search
over idiom choices, scored by the byte-distance we can now compute.

A candidate is a C TEMPLATE with holes written `$name`, plus a `choices` map
giving each hole a few equivalent forms — e.g. `$mul` -> ["x * 2", "x << 1",
"x + x"], `$loop` -> ["for (...)", "while (...)"]. These are exactly the
ambiguities the lifter/Ghidra can't resolve on their own; the compiler's bytes
resolve them. The search:
  * full product when the combo count is small (<= MAX_PRODUCT)
  * else coordinate descent: sweep one hole at a time, keep the best, repeat
Every candidate is one compile, so the sweep fans out across cores (parallel).

    python permute.py --selftest        # recover a hidden idiom set by search
"""
import argparse
import itertools
import json
import string
import sys

import parallel
import segdiff                       # default backend: 16-bit x86 (Win95 path)

MAX_PRODUCT = 200
BIG = 10 ** 9


def _use_backend(arch):
    """Swap the compile+diff backend. permute runs on a THREAD pool (parallel.
    tmap), so rebinding this module global is seen by the worker `_score` calls —
    no pickling/spawn concern. Returns (default_flags, func_prefix)."""
    global segdiff
    if arch == "ppc":
        import segdiff_ppc
        segdiff = segdiff_ppc
        return "/c /O2", ""          # XDK PowerPC: symbols have no leading _
    import segdiff as _sd
    segdiff = _sd
    return "/AM /Od /c", "_"         # MSVC 1.52c: C symbols get a leading _


def expansions(template, choices):
    keys = sorted(choices)
    for combo in itertools.product(*[choices[k] for k in keys]):
        sub = dict(zip(keys, combo))
        yield sub, string.Template(template).safe_substitute(sub)


def _distance(res):
    if res is None:
        return BIG
    if not res["length_match"]:
        return 10 ** 6 + abs(res["our_len"] - res["shipped_len"])
    return res["n_diff"]


def _score(job):
    """job = (csrc, func, target_bytes, target_mask, flags) -> (dist, res)."""
    csrc, func, tgt, tmask, flags = job
    try:
        g = segdiff.compile_group(csrc, flags=flags)
        if func not in g:
            return (BIG, None)
        b, m = g[func]
        res = segdiff.diff(b, m, tgt, tmask)
        return (_distance(res), res)
    except Exception:
        return (BIG, None)


def search(template, choices, func, target, tmask, flags="/AM /Od /c", jobs=6):
    """Return (best_sub, best_csrc, best_dist, best_res, n_tried)."""
    keys = sorted(choices)
    total = 1
    for k in keys:
        total *= len(choices[k])

    def score_batch(cands):                 # cands: [(sub, csrc)]
        jobs_ = [(csrc, func, target, tmask, flags) for _sub, csrc in cands]
        scored = parallel.tmap(_score, jobs_, jobs=min(jobs, 6), label="permute")
        return [(d, r, sub, csrc) for (d, r), (sub, csrc) in zip(scored, cands)]

    tried = 0
    if total <= MAX_PRODUCT:                # full product
        cands = list(expansions(template, choices))
        tried = len(cands)
        best = min(score_batch(cands), key=lambda x: x[0])
    else:                                   # coordinate descent
        cur = {k: choices[k][0] for k in keys}
        best = None
        improved = True
        while improved:
            improved = False
            for k in keys:
                cands = []
                for opt in choices[k]:
                    sub = dict(cur); sub[k] = opt
                    cands.append((sub, string.Template(template).safe_substitute(sub)))
                tried += len(cands)
                d, r, sub, csrc = min(score_batch(cands), key=lambda x: x[0])
                if best is None or d < best[0]:
                    best = (d, r, sub, csrc); cur = sub; improved = True
            if best and best[0] == 0:
                break
    d, r, sub, csrc = best
    return sub, csrc, d, r, tried


# ---------------------------------------------------------------------------
# selftest: hide an idiom set, then recover it by search (no retail anchor)
# ---------------------------------------------------------------------------
TEMPLATE = r"""
int f(int x, int y)
{
    int t = $A;
    return $B;
}
"""
CHOICES = {"A": ["x + y", "y + x"],
           "B": ["t * 2", "t << 1", "t + t", "t * 3"]}
SECRET = {"A": "y + x", "B": "t * 3"}


TEMPLATE_PPC = r"""
int f(int x, int y)
{
    int t = $A;
    return $B;
}
"""
CHOICES_PPC = {"A": ["x + y", "y + x"],
               "B": ["t * 2", "t << 1", "t + t", "t * 3"]}
SECRET_PPC = {"A": "y + x", "B": "t * 3"}


def _selftest_ppc():
    print("permuter selftest (--arch ppc, XDK PowerPC compiler)\n")
    _use_backend("ppc")
    import ppc_cc
    if not ppc_cc.available():
        print("XDK compiler not found — set: XboxTranspiler config xdk <path>")
        return False
    flags = "/c /O2"
    secret_src = string.Template(TEMPLATE_PPC).safe_substitute(SECRET_PPC)
    g = segdiff.compile_group(secret_src, flags=flags)
    tgt, tmask = g["f"]
    print("hidden idioms: %s -> target %d PowerPC bytes" % (SECRET_PPC, len(tgt)))
    sub, csrc, dist, res, tried = search(TEMPLATE_PPC, CHOICES_PPC, "f", tgt,
                                         tmask, flags)
    print("tried %d candidates; best distance = %d" % (tried, dist))
    print("recovered idioms: %s" % sub)
    ok = dist == 0 and res is not None and res["exact"]
    print("\nSELFTEST:", "PASS - byte-exact PowerPC found by search" if ok else "FAIL")
    return ok


def _selftest():
    print("permuter selftest\n")
    flags = "/AM /Od /c"
    # build the target from the hidden idiom set
    secret_src = string.Template(TEMPLATE).safe_substitute(SECRET)
    g = segdiff.compile_group(secret_src, flags=flags)
    tgt, tmask = g["_f"]
    print("hidden idioms: %s -> target %d bytes" % (SECRET, len(tgt)))

    sub, csrc, dist, res, tried = search(TEMPLATE, CHOICES, "_f", tgt, tmask, flags)
    print("tried %d candidates; best distance = %d" % (tried, dist))
    print("recovered idioms: %s" % sub)
    ok = dist == 0 and res is not None and res["exact"]
    # the value of B must match the secret's codegen (A may be a no-op tie)
    print("B recovered exactly:", sub.get("B") == SECRET["B"] or "byte-equivalent")
    print("\nSELFTEST:", "PASS - byte-exact form found by search" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("template", nargs="?", help="C template file with $holes")
    ap.add_argument("--choices", help="JSON {hole: [forms...]}")
    ap.add_argument("--func", help="function to match (e.g. _Foo)")
    ap.add_argument("--arch", choices=("x86", "ppc"), default="x86",
                    help="x86 = Win95 16-bit (default); ppc = Xbox 360 PowerPC")
    ap.add_argument("--module"); ap.add_argument("--seg", type=int)
    ap.add_argument("--off"); ap.add_argument("--flags", default=None)
    # PowerPC target anchor (shipped side comes from the XEX, not an NE segment)
    ap.add_argument("--xex"); ap.add_argument("--vaddr", type=lambda x: int(x, 0))
    ap.add_argument("--size", type=lambda x: int(x, 0))
    ap.add_argument("--selftest", action="store_true")
    jobs = parallel.resolve_jobs(sys.argv, default=6)
    args = ap.parse_args()

    default_flags, _pfx = _use_backend(args.arch)
    flags = args.flags or default_flags

    if args.selftest:
        sys.exit(0 if (_selftest_ppc() if args.arch == "ppc" else _selftest()) else 1)

    tmpl = args.template and open(args.template).read()
    choices = args.choices and json.load(open(args.choices))

    if args.arch == "ppc":
        if not (tmpl and choices and args.func and args.xex and args.vaddr):
            ap.error("ppc: need template + --choices --func --xex --vaddr (or --selftest)")
        probe = string.Template(tmpl).safe_substitute(
            {k: v[0] for k, v in choices.items()})
        g = segdiff.compile_group(probe, flags=flags)
        size = args.size or len(g[args.func][0])
        tgt = segdiff.shipped_xex_function(args.xex, args.vaddr, size)
        tmask = {}
        sub, csrc, dist, res, tried = search(tmpl, choices, args.func, tgt, tmask,
                                             flags, jobs)
        print("tried %d; best distance %d\n%s" % (tried, dist, segdiff.render(res)))
        print("\nwinning idioms: %s" % sub)
        if res and res["exact"]:
            print("\n--- byte-exact C ---\n%s" % csrc)
        sys.exit(0 if res and res["exact"] else 1)

    if not (tmpl and choices and args.func and args.module
            and args.seg is not None and args.off):
        ap.error("need template + --choices --func --module --seg --off (or --selftest)")
    import target
    off = int(args.off, 0)
    seg_bytes, seg_mask = segdiff.shipped_ne_segment(target.resolve(args.module), args.seg)
    # size unknown up front; use the first candidate's length as the window
    probe = string.Template(tmpl).safe_substitute(
        {k: v[0] for k, v in choices.items()})
    g = segdiff.compile_group(probe, flags=flags)
    size = len(g[args.func][0])
    tgt = seg_bytes[off:off + size]
    tmask = {m - off for m in seg_mask if off <= m < off + size}
    sub, csrc, dist, res, tried = search(tmpl, choices, args.func, tgt, tmask,
                                         flags, jobs)
    print("tried %d; best distance %d\n%s" % (tried, dist, segdiff.render(res)))
    print("\nwinning idioms: %s" % sub)
    if res and res["exact"]:
        print("\n--- byte-exact C ---\n%s" % csrc)
    sys.exit(0 if res and res["exact"] else 1)


if __name__ == "__main__":
    main()
