#!/usr/bin/env python3
r"""synth_search.py -- enumerative expression synthesis: the auto-rule-generator.

The step that removes the human from the loop. Instead of a person reading a gap,
analyzing the idiom, and hand-writing a rule, a GRAMMAR enumerates candidate C for the
function's shape and the compiler-oracle (refine.check / mocklink.verify) proves each
byte-exact. MSC 6.0's codegen is deterministic and its idiom vocabulary is finite, so a
bounded search terminates on the exact source -- no analysis required. Whatever verifies
is, by construction, a correct rule; it can be banked into templates.py automatically.

    from engine.inverse_engine import synth_search
    c, grammar, tried = synth_search.solve(target_bytes, verify=mocklink.verify)

This is the "superoptimization" / "genetic search" layer of the roadmap. The offline
precomputed [asm -> C] table is the same idea cached: run the grammars once over all
holes, index by masked bytes, then decompilation is an O(1) lookup.
"""
import itertools


# ---- grammars: each yields candidate C strings for a function SHAPE -------------
# A grammar encodes the finite parameter space of one MSC-6 idiom class. The search is
# the cross-product of its holes; the oracle rejects all but the byte-exact instantiation.

def g_thunk(ctx):
    """Pure forwarder: return SIB(args). Holes: convention, arg count/order."""
    n = (ctx or {}).get("nargs", 3)
    for conv in ("pascal ", ""):
        params = ", ".join("int a%d" % i for i in range(n))
        for order in (range(n), range(n - 1, -1, -1)):
            fwd = ", ".join("a%d" % i for i in order)
            yield "extern int far SIB();\nint far %sF(%s){ return SIB(%s); }" % (conv, params, fwd)


def g_bufread(ctx):
    """Sibling fills a stack buffer, function reads it back and stores to *out.
    Holes: buffer size, read type/expr, return-flow, out-pointer width. This ONE grammar
    covers both seg11:0x7f4 (2-byte WORD) and seg11:0x830 (4-byte long) hand-fixes."""
    word_reads = ["b[0]|(b[1]<<8)", "*(unsigned*)b"]
    # 2-byte WORD variant
    for bufsz, rd, flow in itertools.product((2, 4), word_reads, ("if", "early")):
        if flow == "if":
            body = "unsigned char b[%d]; if (SIB(p,(char far*)b,2)){ *out=%s; return 0xffff; } return 0;" % (bufsz, rd)
        else:
            body = "unsigned char b[%d]; if (!SIB(p,(char far*)b,2)) return 0; *out=%s; return 0xffff;" % (bufsz, rd)
        yield "extern int far SIB();\nint far F(int p, unsigned *out){ %s }" % body
    # 4-byte long variant: declare `long v`, then build it AFTER the buffer is filled
    build = "v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0];"
    for bufsz, flow in itertools.product((4, 6), ("if", "early")):
        if flow == "if":
            body = "unsigned char b[%d]; long v; if (SIB(p,(char far*)b,4)){ %s *out=v; return 0xffff; } return 0;" % (bufsz, build)
        else:
            body = "unsigned char b[%d]; long v; if (!SIB(p,(char far*)b,4)) return 0; %s *out=v; return 0xffff;" % (bufsz, build)
        yield "extern int far SIB();\nint far F(int p, long *out){ %s }" % body


GRAMMARS = [g_thunk, g_bufread]


def verify_any(target, cand):
    """A COMPLETE oracle: try leaf (refine), then pascal siblings (mocklink), then cdecl
    siblings (own stubs). The synthesizer is only as good as its judge -- an incomplete
    verifier rejects correct candidates (mocklink is pascal-only, so it fails cdecl-sibling
    functions like seg11:0x7f4 even when the C is exact)."""
    import re
    from engine.sdk import refine, mocklink
    from engine.inverse_engine import msc6_oracle as ora
    from engine.inverse_engine import oracle_router as router
    from engine.inverse_engine.msc6fn import _mask
    target = bytes(target)
    if router.is_msvc152(target):        # 1.52c frame -> its own oracle + recipe set
        return router.byte_exact(target, cand)
    sibs = re.findall(r"extern int far (\w+)\(\);", cand)
    if not sibs:
        return refine.check(target, cand)
    r = mocklink.verify(target, cand)                       # pascal siblings (near + far)
    if r.get("exact"):
        return r
    body = cand                                             # cdecl siblings: own stubs
    for sname in sibs:
        body = body.replace("extern int far %s();\n" % sname, "")
    stubs = "".join("int far %s(){ return 0; }\n" % sname for sname in sibs)

    def _norm(b):
        return _mask(b).rstrip(b"\x90\x00")
    a = _norm(target)
    for rcp in mocklink.RECIPES:
        try:
            rc = mocklink._first_func(ora.mc6_compile(body + "\n" + stubs, rcp))
        except Exception:
            continue
        if _norm(rc) == a:
            return {"exact": True, "recipe": "cdecl-siblings " + rcp}
    return {"exact": False}


def solve(target, verify, ctx=None, grammars=None):
    """Search the grammars for a candidate that the oracle proves byte-exact. Returns
    (c, grammar_name, n_tried) -- (None, None, n) if the space is exhausted."""
    tried = 0
    for grammar in (grammars or GRAMMARS):
        for cand in grammar(ctx):
            tried += 1
            try:
                if verify(bytes(target), cand).get("exact"):
                    return cand, grammar.__name__, tried
            except Exception:
                continue
    return None, None, tried


def bank_template(c, name):
    """A verified candidate IS a correct rule. In the full loop this appends a generalized
    Template to templates.py (holes re-parameterized). Stubbed here as the auto-commit hook."""
    return {"template": name, "c": c}


# ---- SPEED DIAL: directed verification + parallel workers ----------------------
import re as _re
from concurrent.futures import ThreadPoolExecutor, as_completed
from capstone import Cs as _Cs, CS_ARCH_X86 as _ARCH, CS_MODE_16 as _M16
_MDS = _Cs(_ARCH, _M16)


def sibling_conv(fb):
    """Read the target's sibling call semantics to pick the convention WITHOUT compiling:
    `push cs ; call near` then `add sp,N` -> cdecl sibling (caller cleans); no add sp ->
    pascal. This skips the wasted half of the recipe search (e.g. 8 pascal compiles on a
    cdecl function). Returns 'cdecl' | 'pascal' | None (no near sibling)."""
    ins = list(_MDS.disasm(bytes(fb), 0))
    for i, inst in enumerate(ins):
        if inst.mnemonic == "call" and inst.op_str.startswith("0x") and i > 0 \
                and ins[i - 1].mnemonic == "push" and ins[i - 1].op_str == "cs":
            nx = ins[i + 1] if i + 1 < len(ins) else None
            return "cdecl" if (nx and nx.mnemonic == "add" and nx.op_str.startswith("sp")) else "pascal"
    return None


def _srcs(cand, conv):
    """(compilable source, is_sibling) forms for a candidate, DIRECTED by the target's
    sibling convention -- only the matching stub kind, not both."""
    sibs = _re.findall(r"extern int far (\w+)\(\);", cand)
    if not sibs:
        yield cand, False
        return
    body = cand
    for s in sibs:
        body = body.replace("extern int far %s();\n" % s, "")
    if conv in ("cdecl", None):
        yield body + "\n" + "".join("int far %s(){ return 0; }\n" % s for s in sibs), True
    if conv in ("pascal", None):
        yield body + "\n" + "".join("int far pascal %s(){ return 0; }\n" % s for s in sibs), True


def parallel_solve(target, grammars=None, workers=6, ctx=None, prune=None):
    """Directed + parallel synthesis: enumerate (candidate x recipe) tasks pruned to the
    target's convention (and by `prune(cand)`, the pre-flight constraint filter), compile+
    verify them concurrently across `workers` DOSBox processes (each independent), return
    the first byte-exact candidate. ~10x the serial solve()."""
    from engine.inverse_engine import oracle_router as router
    from engine.inverse_engine.msc6fn import _mask
    from engine.sdk.mocklink import _first_func
    target = bytes(target)
    a = _mask(target).rstrip(b"\x90\x00")
    ora, RECIPES = router.pick(target)               # route to the compiler that built this frame
    _compile_batch = getattr(ora, "compile_batch", None) or getattr(ora, "mc6_compile_batch")
    conv = sibling_conv(target)
    tasks = []
    for grammar in (grammars or GRAMMARS):
        for cand in grammar(ctx):
            if prune is not None and not prune(cand):        # constraint-engine prune
                continue
            for src, is_sib in _srcs(cand, conv):
                for rcp in RECIPES:
                    tasks.append((cand, src, rcp, is_sib))

    import math

    def _run_batch(batch):
        # ONE DOSBox run for the whole batch (spawn paid once), then check each result
        try:
            results = _compile_batch([(src, rcp) for (_c, src, rcp, _s) in batch])
        except Exception:
            return None
        for (cand, _src, _rcp, is_sib), rc in zip(batch, results):
            if rc is None:
                continue
            b = _first_func(rc) if is_sib else rc
            if _mask(b).rstrip(b"\x90\x00") == a:
                return cand
        return None

    if not tasks:
        return None, conv, 0
    bsize = max(1, math.ceil(len(tasks) / workers))     # ~`workers` batches, run in parallel
    batches = [tasks[i:i + bsize] for i in range(0, len(tasks), bsize)]
    with ThreadPoolExecutor(max_workers=workers) as ex:
        futs = [ex.submit(_run_batch, b) for b in batches]
        for fut in as_completed(futs):
            r = fut.result()
            if r:
                for f in futs:
                    f.cancel()
                return r, conv, len(tasks)
    return None, conv, len(tasks)
