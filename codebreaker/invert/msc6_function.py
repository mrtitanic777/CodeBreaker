r"""msc6_function.py -- whole-function MSC 6.0a inverse (beyond the leaf templates in msc6inv).

Recovers a complete function's C from its bytes given a DGROUP symbol map (offset ->
(name,type)) and an import map (reloc-target -> name), by modeling the compound templates
that make up real functions. Verified byte-exact against the shipped bytes with the
data-offset and far-call operands MASKED (they are DGROUP/import relocations the linker
fills; the reconstruction re-links them, so they're compared masked -- same contract as
recon_engine/segdiff).

First whole-function target: LZEXPAND Cleanup -- three `if (gpX != 0L) {
GlobalFree((WORD)(gpX>>16)); gpX = 0L; }` blocks (long globals + far call + long store).

    wt inverse msc6fn        # runs the Cleanup end-to-end self-test
"""
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_MD = Cs(CS_ARCH_X86, CS_MODE_16)

PROLOGUE = bytes.fromhex("1e589045558bec1e8ed8")
EPI = bytes.fromhex("4d4d8be51f5d4d")


def _mask(code):
    """Zero the bytes that are DGROUP/import relocations: the imm16 offset of a direct
    memory operand ([addr]) and the 4-byte target of a far call (9a). Opcode-based so it
    needs no capstone detail mode."""
    b = bytearray(code)
    for ins in _MD.disasm(bytes(code), 0):
        op = ins.bytes; a = ins.address
        if op[0] == 0x9a:                                    # far call: 4 target bytes
            for k in range(1, 5):
                b[a + k] = 0
        elif op[0] == 0xe8:                                  # near CALL: rel16 = layout-assigned
            b[a + 1] = 0; b[a + 2] = 0                        # (intra-segment; masked like a reloc)
        elif op[0] in (0xa1, 0xa3):                          # mov ax,[imm16] / mov [imm16],ax
            b[a + 1] = 0; b[a + 2] = 0
        elif op[0] == 0x68:                                  # push imm16: a near-ptr offset
            b[a + 1] = 0; b[a + 2] = 0                        # (DGROUP addr; link-assigned)
        elif len(op) >= 4 and op[0] in (0xff, 0x0b, 0x83, 0xc6, 0xc7, 0x89, 0x8b) and op[1] in (0x36, 0x06, 0x3e, 0x26, 0x0e, 0x16, 0x1e):
            b[a + 2] = 0; b[a + 3] = 0                        # direct [imm16] disp (2 bytes at +2)
    return bytes(b)


def invert_cleanup(code, syms):
    """Recover the long-global-free idiom function (Cleanup class). syms: {lo_off:(name)}
    for each long global (lo word). Returns C source."""
    b = bytes(code)
    assert b.startswith(PROLOGUE)
    p = len(PROLOGUE)
    if b[p:p+2] == b"\x83\xec":
        p += 3
    lines = []
    used = []
    while True:
        # long-test: a1 <hi>; 0b06 <lo>; je <skip>
        if b[p:p+1] == b"\xa1" and b[p+3:p+5] == b"\x0b\x06" and b[p+7] == 0x74:
            hi = b[p+1] | (b[p+2] << 8); lo = b[p+5] | (b[p+6] << 8)
            name = syms.get(lo, "g_%x" % lo); used.append(name)
            # then-block: ff36 <hi>; 9a<reloc>; 2bc0; a3 <hi>; a3 <lo>
            q = p + 9
            assert b[q:q+2] == b"\xff\x36"                    # push [hi] = (WORD)(name>>16)
            q += 4
            assert b[q:q+1] == b"\x9a"; q += 5                # CALLF GlobalFree
            assert b[q:q+2] == b"\x2b\xc0"; q += 2            # sub ax,ax  (=0)
            assert b[q:q+1] == b"\xa3"; q += 3                # mov [hi],ax
            assert b[q:q+1] == b"\xa3"; q += 3                # mov [lo],ax  -> name = 0L
            lines += ["if (%s != 0L) {" % name,
                      "    GlobalFree((WORD)(%s >> 16));" % name,
                      "    %s = 0L;" % name, "}"]
            p = q
            continue
        break
    decls = "typedef unsigned int WORD;\nextern WORD far pascal GlobalFree(WORD);\n"
    decls += "".join("extern long %s;\n" % n for n in used)
    body = "".join("    %s\n" % ln for ln in lines)
    return "%s\nvoid far Cleanup(void)\n{\n%s}" % (decls, body)


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
