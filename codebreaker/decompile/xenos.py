"""xenos.py -- the Xbox 360 GPU's (Xenos) shader microcode, as instructions.

    python -m codebreaker.decompile.xenos <hex dword> ...     disassemble microcode words
    python -m codebreaker.decompile.xenos --sample            the shader the decode is checked on

    from codebreaker.decompile import xenos
    xenos.disassemble(dwords)       ["CF: alloc / exece", "muls r0.z, r0.yyxx", ...]

The console's second instruction set: what the SDK's shader compiler (fxc) makes of HLSL. An ALU
instruction is three words (96 bits) that issue a vector and a scalar operation together; control
flow (CF) instructions are 48 bits, two to three words; fetch instructions read vertices and
textures. The encodings are the ones docs/XBOX360_TOOLCHAIN.md's study recovered from the SDK's
microcode assembler.

What is checked (tests/test_xenos.py), against the shader compiler's own listing of the same
microcode (`fxc /T ps_3_0 /XuFormatRaw /Fh`): the ALU word's fields -- vector and scalar opcode,
destination, write masks, the export bit, saturate, each source's register, select, negate and
swizzle -- and the CF opcodes. What is not yet: the layout of a whole program (here the leading
CF words are one pair and every later group of three words is an ALU instruction -- true of the
simple shaders it is checked on; a real program interleaves them through CF exec ranges), the
fetch words (recognised, not decoded), and the swizzle's spelling (relative, so `r0.xy` reads
`r0.yyxx`: the same channels, written out in full).
"""
import sys

VECTOR = {0x00: "add", 0x01: "mul", 0x02: "max", 0x03: "max", 0x04: "seq", 0x05: "sgt", 0x06: "sge",
          0x07: "sne", 0x08: "frc", 0x09: "trunc", 0x0a: "floor", 0x0b: "mad", 0x0c: "cndeq",
          0x0d: "cndge", 0x0e: "cndgt", 0x0f: "dp4", 0x10: "dp3", 0x11: "dp2add", 0x12: "cube",
          0x13: "max4", 0x14: "setp_eq_push", 0x15: "setp_ne_push", 0x16: "setp_gt_push",
          0x17: "setp_ge_push", 0x18: "kille", 0x19: "killgt", 0x1a: "killge", 0x1b: "killne",
          0x1c: "dst", 0x1d: "mova"}
# how many sources each vector opcode reads
VECTOR_SOURCES = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 2, 2, 3, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0]
SCALAR = {0x00: "adds", 0x01: "adds_prev", 0x02: "muls", 0x03: "muls_prev", 0x04: "muls_prev2",
          0x05: "maxs", 0x06: "mins", 0x07: "seqs", 0x08: "sgts", 0x09: "sges", 0x0a: "snes",
          0x0b: "frcs", 0x0c: "truncs", 0x0d: "floors", 0x0e: "exp", 0x0f: "logc", 0x10: "log",
          0x11: "rcpc", 0x12: "rcpf", 0x13: "rcp", 0x14: "rsqc", 0x15: "rsqf", 0x16: "rsq",
          0x17: "mova", 0x18: "movaf", 0x1a: "subs_prev", 0x1b: "setp_eq", 0x1c: "setp_ne",
          0x1d: "setp_gt", 0x1e: "setp_ge", 0x1f: "setp_inv", 0x20: "setp_pop", 0x21: "setp_clr",
          0x22: "setp_rst", 0x23: "kills_eq", 0x24: "kills_gt", 0x25: "kills_ge", 0x26: "kills_ne",
          0x27: "kills_one", 0x28: "sqrt", 0x2a: "mulsc0", 0x2b: "mulsc1", 0x2c: "addsc0",
          0x2d: "addsc1", 0x2e: "subsc0", 0x2f: "subsc1", 0x30: "sin", 0x31: "cos"}
CONTROL = {0: "nop", 1: "exec", 2: "exece", 3: "cexec", 4: "cexece", 5: "cexecp", 6: "cexecpe",
           7: "loop", 8: "endloop", 9: "call", 0xa: "ret", 0xb: "jmp", 0xc: "alloc"}
_CHANNELS = "xyzw"

# a pixel shader as fxc built it (/XuFormatRaw /Fh), and fxc's own listing of it
SAMPLE = [0x00000000, 0x2001c400, 0x22000000, 0x08400000, 0x00000061, 0xe2000000,
          0xc80f8000, 0x00404000, 0xe2000000, 0, 0, 0]
SAMPLE_LISTING = ["alloc colors", "exece", "muls r0.z, r0.xy", "mov oC0, r0.xyzx"]


def _mask(m):
    """A write mask (x=1, y=2, z=4, w=8) as its channels."""
    return "".join(c for i, c in enumerate(_CHANNELS) if m & (1 << i))


def _swizzle(sw):
    """Two bits per channel, relative to the channel itself (0 = the same channel)."""
    return "".join(_CHANNELS[(i + ((sw >> (2 * i)) & 3)) & 3] for i in range(4))


def _register(selected, number):
    """A source: a temporary register (rN) when selected, else a constant (cN); bit 7 takes its
    absolute value, bit 6 indexes it by the loop counter."""
    name = ("r%d" % (number & 0x3f)) if selected else ("c%d" % number)
    return name + ("_abs" if number & 0x80 else "") + ("[aL]" if number & 0x40 else "")


def alu(d0, d1, d2):
    """The instructions one 96-bit ALU word issues: [vector, scalar], each when its slot is used."""
    vop, sop = (d2 >> 24) & 0x1f, (d0 >> 26) & 0x3f
    vdst, sdst = d0 & 0x3f, (d0 >> 8) & 0x3f
    vmask, smask = (d0 >> 16) & 0xf, (d0 >> 20) & 0xf
    export, vsat, ssat = (d0 >> 15) & 1, (d0 >> 24) & 1, (d0 >> 25) & 1
    sources = []
    for rshift, sshift, selbit, negbit in ((16, 16, 31, 26), (8, 8, 30, 25), (0, 0, 29, 24)):
        sources.append(((d2 >> rshift) & 0xff, (d1 >> sshift) & 0xff, (d2 >> selbit) & 1, (d1 >> negbit) & 1))

    def source(i):
        number, sw, sel, neg = sources[i]
        return ("-" if neg else "") + _register(sel, number) + "." + _swizzle(sw)
    out = []
    if export or vmask:
        count = VECTOR_SOURCES[vop] if vop < len(VECTOR_SOURCES) else 3
        dst = "oC0" if export else "r%d.%s" % (vdst, _mask(vmask))
        # max of a source with itself is how the compiler writes a move, and how it lists it
        if vop in (2, 3) and count >= 2 and source(0) == source(1):
            out.append("mov%s %s, %s" % ("_sat" if vsat else "", dst, source(0)))
        else:
            out.append("%s%s %s, %s" % (VECTOR.get(vop, "vop%02x" % vop), "_sat" if vsat else "", dst,
                                        ", ".join(source(i) for i in range(count))))
    if smask:                                               # the scalar operation reads source C
        out.append("%s%s r%d.%s, %s" % (SCALAR.get(sop, "sop%02x" % sop), "_sat" if ssat else "", sdst,
                                        _mask(smask), source(2)))
    return out


def control(d0, d1, d2):
    """The two 48-bit CF instructions in three words: their opcodes."""
    v = d0 | (d1 << 32) | (d2 << 64)
    return [CONTROL.get((half >> 44) & 0xf, "cf%x" % ((half >> 44) & 0xf))
            for half in (v & ((1 << 48) - 1), v >> 48)]


def disassemble(dwords):
    """Lines for a simple shader: one CF pair, then ALU words up to three zero words."""
    out = []
    if len(dwords) >= 3:
        out.append("CF: " + " / ".join(control(*dwords[:3])))
    for i in range(3, len(dwords) - 2, 3):
        if not any(dwords[i:i + 3]):
            break
        out += alu(*dwords[i:i + 3])
    return out


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__.split("\n\n")[0])
        return 0
    words = SAMPLE if argv[0] == "--sample" else [int(x, 16) for x in argv]
    for line in disassemble(words):
        print(line)
    if argv[0] == "--sample":
        print("; the compiler's listing: " + " | ".join(SAMPLE_LISTING))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
