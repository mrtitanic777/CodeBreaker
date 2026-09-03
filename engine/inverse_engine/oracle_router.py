#!/usr/bin/env python3
r"""oracle_router.py -- route a reconstruction to the compiler that built it, by FRAME.

The Win95 NE corpus is a two-compiler world (see the corpus memory): MSC 6.0a lays a
push-ds;pop-ax prologue (1e58) with a dec bp;mov sp,bp teardown, MSVC 1.52c lays a mov-ax,ds
prologue (8cd8) with a lea sp,[bp-2] teardown. A candidate can only be proven byte-exact by
recompiling with the SAME compiler; the other emits a different frame and never matches,
however correct the body. This module picks the oracle + recipe set from a function's frame
bytes, so every verifier -- synth_search, auto_sweep, the pipeline -- checks against the
right toolchain instead of assuming 6.0a.

    from engine.inverse_engine import oracle_router as router
    if router.byte_exact(shipped_bytes, candidate_c)["exact"]: ...   # frame-routed
"""
from engine.inverse_engine import msc6_oracle as _msc6
from engine.inverse_engine import msvc152_oracle as _msvc152
from engine.inverse_engine.msc6fn import _mask

_EPI_LEA = bytes.fromhex("8d66fe1f5d4d")            # 1.52c lea-variant teardown (far frame)
_EPI_NEAR_LEA = bytes.fromhex("8d66fe1f5d")         # 1.52c lea teardown, near frame (no dec bp);
                                                    # a substring of _EPI_LEA, so it catches both
_PRO_MOVDS = bytes.fromhex("8cd8")                  # 1.52c mov-ax,ds prologue head
_PRO_NEAR = bytes.fromhex("558bec1e8ed8")           # 1.52c near-loadds prologue (no inc bp)

# recipe sets per compiler. 1.52c ships a lot of UNoptimized code (81ec frame + stack slots),
# so the no-/O forms lead; both sets keep the optimized variants for the modules built with them.
RECIPES_MSC6 = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /Gsw /Ox", "/AM /Gsw /Os")
RECIPES_MSVC152 = ("/AM /G2 /Gsw", "/AM /G2 /Gsw /Od", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw /Ox")


def is_msvc152(fb):
    """Frame test for MSVC 1.52c: the mov-ax,ds prologue, the near-loadds prologue, or a
    lea-based teardown (lea sp,[bp-2]) -- all of which 6.0a never emits. The near-lea epilogue
    is a substring of the far-lea one, so the single `in` check covers both frame widths."""
    fb = bytes(fb)
    return fb[:2] == _PRO_MOVDS or fb.startswith(_PRO_NEAR) or _EPI_NEAR_LEA in fb


def pick(fb):
    """(oracle_module, recipes) for a function, by frame signature."""
    if is_msvc152(fb):
        return _msvc152, RECIPES_MSVC152
    return _msc6, RECIPES_MSC6


def _compile_batch(ora, jobs):
    """Uniform batch entry: msc6_oracle exposes mc6_compile_batch, msvc152 exposes compile_batch."""
    fn = getattr(ora, "compile_batch", None) or getattr(ora, "mc6_compile_batch")
    return fn(jobs)


def _norm(b):
    return _mask(bytes(b)).rstrip(b"\x90\x00")


def byte_exact(fb, candidate_c):
    """Compile candidate_c with the frame-matched oracle across its recipe set; return
    {exact, oracle, recipe}. Never raises for a compile miss -- a recipe that produced no
    object is simply not a match."""
    fb = bytes(fb)
    ora, recipes = pick(fb)
    label = "msvc152" if ora is _msvc152 else "msc6"
    if not ora.available():
        return {"exact": False, "oracle": label, "reason": "oracle not configured"}
    tgt = _norm(fb)
    outs = _compile_batch(ora, [(candidate_c, rcp) for rcp in recipes])
    for rcp, rc in zip(recipes, outs):
        if rc is not None and _norm(rc) == tgt:
            return {"exact": True, "oracle": label, "recipe": rcp}
    return {"exact": False, "oracle": label}


def byte_exact_batch(items):
    """Verify many (fb, candidate_c) at once, GROUPED by oracle so each compiler runs one
    DOSBox batch. Returns a list of {exact, oracle, recipe} aligned with `items`."""
    items = list(items)
    results = [None] * len(items)
    for ora, recipes, label in ((_msc6, RECIPES_MSC6, "msc6"),
                                 (_msvc152, RECIPES_MSVC152, "msvc152")):
        idx = [k for k, (fb, _c) in enumerate(items) if pick(bytes(fb))[0] is ora]
        if not idx:
            continue
        if not ora.available():
            for k in idx:
                results[k] = {"exact": False, "oracle": label, "reason": "oracle not configured"}
            continue
        jobs = [(items[k][1], rcp) for k in idx for rcp in recipes]
        outs = _compile_batch(ora, jobs)
        for n, k in enumerate(idx):
            tgt = _norm(items[k][0]); hit = None
            for j, rcp in enumerate(recipes):
                rc = outs[n * len(recipes) + j]
                if rc is not None and _norm(rc) == tgt:
                    hit = rcp; break
            results[k] = {"exact": hit is not None, "oracle": label, "recipe": hit}
    return results


if __name__ == "__main__":
    # frame routing is pure/offline -- assert it without invoking a compiler
    msc6_frame = bytes.fromhex("1e589045558bec1e8ed8") + b"\x00" * 4 + bytes.fromhex("4d4d8be51f5d4dcb")
    msvc_frame = bytes.fromhex("8cd89045558bec1e8ed8") + b"\x00" * 4 + bytes.fromhex("8d66fe1f5d4dcb")
    assert not is_msvc152(msc6_frame) and pick(msc6_frame)[0] is _msc6
    assert is_msvc152(msvc_frame) and pick(msvc_frame)[0] is _msvc152
    print("oracle_router: frame routing OK (msc6 <- 1e58/std, msvc152 <- 8cd8/lea)")
    print("  msc6 configured   :", _msc6.available())
    print("  msvc152 configured:", _msvc152.available())
