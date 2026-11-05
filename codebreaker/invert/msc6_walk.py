r"""msc6_walk.py -- generalized whole-function MSC 6.0a inverse: a symbol-map-driven statement walker
(returns, if-blocks recursed, far calls, long/word global stores), declining at the first unmodeled
byte.  C++ (native/invert/msc6_small.cc; docs/NATIVE.md); this keeps the API and the self-test.
"""
import sys
from codebreaker.invert import native_invert
from codebreaker.invert.msc6_function import _mask


def invert(code, syms, callee="G"):
    """(C text, {"stmts", "globals"}) or (None, why).  syms: {offset: (name, 'long'|'word')}."""
    return native_invert.small(1, code, syms, callee)


if __name__ == "__main__":
    # regression: Cleanup must invert + verify through the generalized walker
    from codebreaker.invert import compilers as ora
    code = bytes.fromhex(
        "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a344"
        "00a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
        "009affff00002bc0a35600a354004d4d8be51f5d4dcb90")
    syms = {0x44: ("gpA", 'long'), 0x48: ("gpB", 'long'), 0x54: ("gpC", 'long')}
    src, meta = invert(code, syms, callee="GlobalFree")
    if src is None:
        print("DECLINED:", meta); sys.exit(1)
    print(src)
    rc = ora.mc6_compile(src.replace("void far F", "void far Cleanup"), "/AM /G2 /Gsw /Ox")
    print("\n// generalized walker: masked-equal = %s (%d stmts)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), meta["stmts"]))
