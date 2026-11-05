r"""msvc2.py - INVERSE of msvc2model: MSVC 2.0 (win95 C2.EXE) bytes -> byte-exact C.

The 32-bit sibling of early_c.py. Recognises the codegen templates msvc2model emits and recovers
the source expression, then SELF-VERIFIES: a candidate is accepted only if msvc2model.emit(node)
reproduces the input bytes exactly (relocation slots -- call targets, global addresses -- masked),
so every returned C is byte-exact by construction -- never a plausible-but-wrong guess.
Out-of-class byte sequences are cleanly DECLINED (return None), exactly like the 16-bit engine.

Scope = the modeled forward class: single-`return` __stdcall leaves over int args/consts, chains,
mixed precedence, comparisons, deref / index, globals, stores, extern & indirect calls, the
frameless (/Oy) and __fastcall forms, and straight-line statement lists (msvc2_registers.interp).

The inverse is C++ (native/invert/msvc2.cc; docs/NATIVE.md); this keeps its API and the round-trip
self-test.

    wt inverse msvc2inv                 # round-trip battery: bytes -> C -> (oracle) -> bytes
"""
import sys

from codebreaker.invert import msvc2_forward as M, native_invert


def invert(code, mask=None):
    """bytes -> {nargs, expr, c, node} byte-exact, or None if out of the modeled class.  `mask`:
    the byte positions to ignore (default: each candidate's own relocation slots)."""
    return native_invert.msvc2_invert(code, mask)


def _match(pred, code, mask):
    if pred is None or len(pred) != len(code):
        return False
    return all(pred[i] == code[i] for i in range(len(code)) if i not in mask)


def selftest(show=False):
    """Round-trip: oracle-compile each modeled shape -> bytes -> invert -> C -> re-check by
    msvc2model.emit (== the oracle bytes we already validate). True bytes->C->bytes."""
    cc32 = M._oracle()
    ok = tot = 0
    for name, node in M.battery():
        tot += 1
        r = cc32.compile_c(M.to_c(node), flags="/c /O1 /Oy-")
        fn = next((v for k, v in cc32.Obj(r["obj"]).functions().items() if k.startswith("_f")), None) if "obj" in r else None
        if not fn:
            print("  %-8s ORACLE-FAIL" % name)
            continue
        real = bytes(fn[0])
        mask = set(fn[1])
        inv = invert(real, mask)
        good = inv is not None and _match(M.emit(inv["node"]), real, mask)   # re-emit the full node
        ok += good
        if show or not good:
            print("  %-8s %s  %s" % (name, "OK " if good else "MISS",
                                     inv["c"] if inv else "(declined)"))
    print("\nmsvc2inv round-trip: %d/%d bytes->C->byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    return ok == tot


def main():
    sys.exit(0 if selftest(show="--show" in sys.argv[1:]) else 1)


if __name__ == "__main__":
    main()
