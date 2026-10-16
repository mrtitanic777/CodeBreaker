"""hooks_lattice.py -- answers a pass of the Lattice-lineage compilers is given at a place (profile hooks with "call").

cmp_test: the code generator of MS C 1.04's own executables stood between the two shipped ones. When it
compared two longs it loaded both words of the left one first only if neither operand was reached through
a pointer (type flag 2000h); the shipped MC2.EXE 1.04 does so for every long - the test in the function that
generates comparisons (image 0x4D79). There is no room to patch a longer test in, so the pass is told the
outcome at 0x4D7F: go on at 0x4D85 (load) or 0x4DA1 (do not).
"""


def cmp_test(rd, bp, ss, ds):
    n, m = rd(ss, bp + 0x1C), rd(ss, bp + 0x1E)              # the two operand nodes
    eager = rd(ss, bp + 6) == 4 and not (rd(ds, n + 2) & 0x2000) and not (rd(ds, m + 2) & 0x2000)
    return 0x4D85 if eager else 0x4DA1
