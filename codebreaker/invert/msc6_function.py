r"""msc6_function.py -- whole-function MSC 6.0a inverse for the long-global-free idiom (LZEXPAND's
Cleanup: `if (gpX != 0L) { GlobalFree((WORD)(gpX>>16)); gpX = 0L; }` blocks), and the reloc mask the
models compare recompiled bytes under: the DGROUP offsets and far-call targets the linker fills.

Both are C++ (native/invert/msc6_small.cc, msc6_helpers.cc; docs/NATIVE.md); this keeps the API.

    python -m codebreaker invert --tool msc6-function     # the Cleanup self-test against the real compiler
"""
from codebreaker.invert import native_invert


def _mask(code):
    """The bytes with the relocated operands zeroed: a far call's target, a near call's rel16, the
    imm16 of mov ax,[m] / mov [m],ax / push imm16, and a direct [imm16] operand's displacement."""
    return native_invert.mask(code)


def invert_cleanup(code, syms):
    """The Cleanup function's C.  syms: {lo-word offset: name} for each long global."""
    return native_invert.small(0, code, syms)


def _selftest():
    from codebreaker.invert import compilers as ora
    # shipped LZEXPAND Cleanup bytes + its DGROUP long-global map (lo-word offset -> name)
    code = bytes.fromhex(
        "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a344"
        "00a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
        "009affff00002bc0a35600a354004d4d8be51f5d4dcb90")
    syms = {0x44: "gpA", 0x48: "gpB", 0x54: "gpC"}
    src = invert_cleanup(code, syms)
    print("=== recovered C ===\n" + src + "\n")
    rc = ora.mc6_compile(src, "/AM /G2 /Gsw /Ox")
    ok = _mask(rc) == _mask(code) and len(rc) == len(code)
    print("recompiled %d bytes vs shipped %d; masked-equal: %s" % (len(rc), len(code), ok))
    print("*** WHOLE-FUNCTION VERIFIED byte-exact (relocs masked) ***" if ok
          else "*** mismatch:\n  shipped %s\n  ours    %s" % (_mask(code).hex(), _mask(rc).hex()))


if __name__ == "__main__":
    _selftest()
