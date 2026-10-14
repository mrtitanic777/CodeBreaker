r"""grammars.py -- enumerative expression synthesis: candidate texts for a function's shape.

The step that removes the human from the loop. Instead of a person reading a gap,
analyzing the idiom, and hand-writing a rule, a GRAMMAR enumerates candidate C for the
function's shape and the compiler-oracle (refine.check / mocklink.verify) proves each
byte-exact. MSC 6.0's codegen is deterministic and its idiom vocabulary is finite, so a
bounded search terminates on the exact source -- no analysis required. Whatever verifies
is, by construction, a correct rule; it can be banked into templates.py automatically.

    from codebreaker.invert import grammars
    for grammar in grammars.GRAMMARS: for text in grammar(None): ...   (run.py compiles them)

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
