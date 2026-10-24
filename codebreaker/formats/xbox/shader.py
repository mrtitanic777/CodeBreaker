"""shader.py -- an Xbox 360 compiled shader (what the SDK's fxc writes for vs_3_0 / ps_3_0, and what an effect holds).

    from codebreaker.formats.xbox import shader
    s = shader.read(blob)
    s.stage                 "vertex" or "pixel"
    s.constants             [Constant]: name, set ("float4", "bool", "int4", "sampler"), register, count,
                            cls, type, rows, columns, elements, default (floats)
    s.literals              {register: (x, y, z, w)}: the compiler's own constants (`def c255, ...`)
    s.loop_literals         {loop constant: count | start << 8 | step << 16} (`defi i31, ...`)
    s.inputs                vertex: [(slot of the vertex fetch, usage, usage index)];
                            pixel: [(register, usage, usage index, mask)] -- the interpolators
    s.outputs               vertex: [(export register, usage, usage index, mask)]
    s.words                 the microcode, as dwords (decompile/xenos_hlsl.py reads it)

The container is big-endian: 0x102A1100 (pixel) or 0x102A1101 (vertex), the offset and size of the
physical part (the literal constants, then the microcode), the offset of its constant table -- the
Direct3D 9 CTAB, as on the PC -- the offset of the literal records, and the offset of the program
information, which begins with the literals' and the microcode's sizes. A vertex shader's information
lists, after five words, the inputs (a vertex fetch's slot in bits 0-11, the usage in 12-15, its index
in 16-19), the outputs (index 0-3, usage 4-7, export register 8-11, mask 12-15) and the instructions
writing each output; a pixel shader's, after its interpolator control and output mask, its inputs in the
outputs' form. The SDK's fxc listing (/Fc /Xudsrd) names the same tables, which is what they were read
against.
"""
import struct

PIXEL, VERTEX = 0x102A1100, 0x102A1101
USAGES = {0: "POSITION", 1: "BLENDWEIGHT", 2: "BLENDINDICES", 3: "NORMAL", 4: "PSIZE", 5: "TEXCOORD",
          6: "TANGENT", 7: "BINORMAL", 8: "TESSFACTOR", 9: "POSITIONT", 10: "COLOR", 11: "FOG", 12: "DEPTH",
          13: "SAMPLE"}
REGISTER_SETS = {0: "bool", 1: "int4", 2: "float4", 3: "sampler"}
CLASSES = {0: "scalar", 1: "vector", 2: "matrix_rows", 3: "matrix_columns", 4: "object", 5: "struct"}
TYPES = {0: "void", 1: "bool", 2: "int", 3: "float", 4: "string", 5: "texture", 6: "texture1D", 7: "texture2D",
         8: "texture3D", 9: "textureCUBE", 10: "sampler", 11: "sampler1D", 12: "sampler2D", 13: "sampler3D",
         14: "samplerCUBE"}


class Constant:
    def __init__(self, **kw):
        self.__dict__.update(kw)

    def __repr__(self):
        return "<%s %s %s%d x%d>" % (self.set, self.name, "c" if self.set == "float4" else self.set[0],
                                    self.register, self.count)


class Shader:
    def __init__(self):
        self.stage = ""
        self.constants, self.inputs, self.outputs = [], [], []
        self.output_writers = []
        self.literals = {}
        self.loop_literals = {}             # loop constant -> its value, for the ones the compiler made
        self.words = []
        self.target = ""

    def constant_at(self, register_set, register):
        """The constant whose registers include `register` -> (Constant, offset into it), or (None, 0)."""
        for c in self.constants:
            if c.set == register_set and c.register <= register < c.register + c.count:
                return c, register - c.register
        return None, 0


def _u32(b, at):
    return struct.unpack_from(">I", b, at)[0]


def _cstring(b, at):
    end = b.index(b"\0", at)
    return b[at:end].decode("latin-1")


def _ctab(b, base):
    """The Direct3D constant table at `base` (big-endian here) -> ([Constant], target)."""
    n, info, target = _u32(b, base + 12), _u32(b, base + 16), _u32(b, base + 24)
    out = []
    for i in range(n):
        at = base + info + 20 * i
        name_at, = struct.unpack_from(">I", b, at)
        rset, reg, count, _ = struct.unpack_from(">4H", b, at + 4)
        type_at, default_at = struct.unpack_from(">II", b, at + 12)
        cls, typ, rows, cols, elements, members = struct.unpack_from(">6H", b, base + type_at)
        default = None
        if default_at:
            default = list(struct.unpack_from(">%df" % (4 * count), b, base + default_at))
        out.append(Constant(name=_cstring(b, base + name_at), set=REGISTER_SETS.get(rset, str(rset)),
                            register=reg, count=count, cls=CLASSES.get(cls, str(cls)), type=TYPES.get(typ, str(typ)),
                            rows=rows, columns=cols, elements=elements, members=members, default=default))
    return out, _cstring(b, base + target)


def _semantic(entry):
    return USAGES.get((entry >> 4) & 0xF, "USAGE%d" % ((entry >> 4) & 0xF)), entry & 0xF


def read(blob):
    """An Xbox shader container's bytes -> Shader."""
    magic = _u32(blob, 0)
    if magic not in (PIXEL, VERTEX):
        raise ValueError("not an Xbox 360 shader (magic %08X)" % magic)
    s = Shader()
    s.stage = "pixel" if magic == PIXEL else "vertex"
    physical, _size, ctab_at, literals_at, info_at = (_u32(blob, 4), _u32(blob, 8), _u32(blob, 16), _u32(blob, 20),
                                                       _u32(blob, 24))
    s.constants, s.target = _ctab(blob, ctab_at + 4)
    literal_bytes, microcode_bytes = _u32(blob, info_at), _u32(blob, info_at + 4)
    # the literal records: four words, the records' size, then the records -- a float one is register << 16 |
    # count, and two words (its floats are at the physical part's start, in order); a loop constant one is
    # 0x2320 + 4 * index << 16 | count, its values (count | start << 8 | step << 16) and a word. The SDK's fxc
    # listing (def / defi) shows the same constants.
    if literals_at:
        size = _u32(blob, literals_at + 16)
        at, end, pos = literals_at + 20, literals_at + 16 + size, physical
        base = 256 if s.stage == "pixel" else 0          # pixel shader registers follow the vertex shader's
        while at + 4 <= end:
            word = _u32(blob, at)
            register, count = word >> 16, word & 0xFFFF
            if word == 0:
                break
            if register >= 0x2320:
                for i in range(count):
                    s.loop_literals[(register - 0x2320) // 4 + i] = _u32(blob, at + 4 + 4 * i)
                at += 4 * (count + 2)
            else:
                for i in range(count // 4):
                    s.literals[register - base + i] = struct.unpack_from(">4f", blob, pos + 16 * i)
                pos += 4 * count
                at += 12
    start = physical + literal_bytes
    s.words = list(struct.unpack_from(">%dI" % (microcode_bytes // 4), blob, start))
    info = list(struct.unpack_from(">%dI" % ((physical - info_at) // 4), blob, info_at))
    if s.stage == "vertex":
        n_in, n_writers = info[7], info[8]
        entries = info[10:]
        for e in entries[:n_in]:
            usage = USAGES.get((e >> 12) & 0xF, "USAGE%d" % ((e >> 12) & 0xF))
            s.inputs.append((e & 0xFFF, usage, (e >> 16) & 0xF))
        outputs = entries[n_in:len(entries) - n_writers]
        writers = entries[len(entries) - n_writers:]
        for e in outputs:
            usage, index = _semantic(e)
            s.outputs.append(((e >> 8) & 0xF, usage, index, (e >> 12) & 0xF))
        s.output_writers = [w & 0xFFF for w in writers]
    else:
        s.interpolator_control, s.output_mask = info[6], info[7]
        for e in info[8:]:
            if e == 0:
                break
            usage, index = _semantic(e)
            s.inputs.append(((e >> 8) & 0xF, usage, index, (e >> 12) & 0xF))
    return s
