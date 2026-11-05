r"""msc6_registers.py -- register-value-tracking inverse for MSC 6.0a (the piece register
variables need, beyond the template walker). A small abstract interpreter tracks each
register's symbolic C-expression through the instruction stream, recovers the enregistered
locals (SI/DI) and the value-numbering idiom (`push ax; mov si,ax` = one value both passed
AND kept live in a var), and emits C; and the counted for-loop with a register counter.
Verified byte-exact via the oracle (relocs masked). Declines at the first unmodeled instruction.

Both are C++ (native/invert/msc6_registers.cc; docs/NATIVE.md); this keeps the API and the self-test.

    wt inverse msc6reg     # runs the non-loop register-var self-test
"""
from codebreaker.invert import native_invert
from codebreaker.invert.msc6_function import _mask


def invert_reg(code, imports=None):
    """code:bytes (a whole function) -> (c_source, meta) or (None, reason).  imports: {call index: name}."""
    return native_invert.msc6_registers(0, code, imports)


def invert_loop(code, imports=None):
    """Recover the MSC 6.0a counted-for-loop with a register counter:
        di = param ; si = 0 ; or di,di; jle skip ; L: BODY ; inc si; cmp si,di; jl L; skip: return si
    -> `for (i = 0; i < a; i++) BODY; return i;`  Declines otherwise."""
    return native_invert.msc6_registers(1, code, imports)


def _selftest():
    from codebreaker.invert import compilers as ora
    code = bytes.fromhex("1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200")
    src, meta = invert_reg(code, imports={0: "G", 1: "H"})
    if src is None:
        print("DECLINED:", meta)
        return
    print(src)
    rc = ora.mc6_compile(src, "/AM /G2 /Gsw /Ox")
    print("\n// register-var tracker: masked-equal = %s (%d vs %d B)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), len(rc), len(code)))


if __name__ == "__main__":
    _selftest()
