#!/usr/bin/env python3
"""What a PowerPC program's calls show (analyze/ppc_calls.py): the register writes it counts, the
agreement it asks of call sites, and -- when this machine has Tetris Splash's recompiled copy
(Converted/TetrisSplash) -- the import argument counts it finds in the binary against the kernel's
documented signatures.

    python tests/test_ppc_calls.py"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.analyze import ppc_calls     # noqa: E402

GAME = os.path.join(ROOT, "Converted", "TetrisSplash", "original", "default.xex")
# the Xbox 360 kernel's own prototypes (xboxkrnl, as the XDK headers declare them)
DOCUMENTED = {"KeWaitForSingleObject": 5, "KeSetEvent": 3, "KeBugCheckEx": 5, "ExGetXConfigSetting": 5,
              "KfLowerIrql": 1, "ExFreePool": 1, "KeQueryPerformanceFrequency": 0}
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def test_register_writes():
    w = ppc_calls.gpr_writes
    assert w("addi", ["r4", "r1", "0x50"]) == {4}
    assert w("stw", ["r4", "0x10(r1)"]) == set()
    assert w("stwu", ["r1", "-0x60(r1)"]) == {1}
    assert w("lwzu", ["r5", "4(r6)"]) == {5, 6}
    assert w("lmw", ["r28", "0x10(r1)"]) == {28, 29, 30, 31}
    assert w("cmpwi", ["cr6", "r3", "0"]) == set() and w("mtctr", ["r11"]) == set()
    assert ppc_calls.fpr_writes("fadd", ["f1", "f2", "f3"]) == {1}
    assert ppc_calls.fpr_writes("stfd", ["f1", "8(r1)"]) == set()


def test_consensus():
    c = ppc_calls._consensus
    assert c([2, 2], stub=False) is None                       # too few sites
    assert c([2, 2, 3], stub=False) == 2                       # 67 % agree
    assert c([0, 0, 0], stub=False) is None                    # 0: maybe a pass-through
    assert c([0, 0, 0], stub=True) == 0                        # an import stub has no body
    assert c([8, 8, 8, 7], stub=False) is None                 # 8 needs 85 %
    assert c([5, 5, 5, 4], stub=True) is None and c([5] * 7 + [4], stub=True) == 5


def test_real_imports_match_their_prototypes():
    if not os.path.isfile(GAME):
        raise Skip("no Converted/TetrisSplash here")
    r = ppc_calls.analyse(GAME)
    found = {n: r.imports[n] for n in DOCUMENTED if n in r.imports}
    assert len(found) >= 4, found
    wrong = {n: (a, DOCUMENTED[n]) for n, a in found.items() if a != DOCUMENTED[n]}
    assert not wrong, wrong
    assert r.arity and r.clobber and r.functions > 1000


def main():
    for fn in (test_register_writes, test_consensus, test_real_imports_match_their_prototypes):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
