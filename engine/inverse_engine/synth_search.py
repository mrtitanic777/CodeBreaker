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
    long_read = ("long v; v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0];", "v")
    # 2-byte WORD variant
    for bufsz, rd, flow in itertools.product((2, 4), word_reads, ("if", "early")):
        if flow == "if":
            body = "unsigned char b[%d]; if (SIB(p,(char far*)b,2)){ *out=%s; return 0xffff; } return 0;" % (bufsz, rd)
        else:
            body = "unsigned char b[%d]; if (!SIB(p,(char far*)b,2)) return 0; *out=%s; return 0xffff;" % (bufsz, rd)
        yield "extern int far SIB();\nint far F(int p, unsigned *out){ %s }" % body
    # 4-byte long variant
    for bufsz, flow in itertools.product((4, 6), ("if", "early")):
        pre, expr = long_read
        if flow == "if":
            body = "unsigned char b[%d]; %s if (SIB(p,(char far*)b,4)){ *out=%s; return 0xffff; } return 0;" % (bufsz, pre, expr)
            body = "unsigned char b[%d]; if (SIB(p,(char far*)b,4)){ %s *out=%s; return 0xffff; } return 0;" % (bufsz, pre, expr)
        else:
            body = "unsigned char b[%d]; %s if (!SIB(p,(char far*)b,4)) return 0; v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0]; *out=v; return 0xffff;" % (bufsz, pre)
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
    from engine.inverse_engine.msc6fn import _mask
    target = bytes(target)
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
