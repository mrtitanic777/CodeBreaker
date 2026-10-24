"""mgfx.py -- MonoGame's compiled effect (.mgfxo, version 10: MonoGame 3.8.1), read far enough to add parameters.

    from codebreaker.xna import mgfx
    data = mgfx.add_parameters(data, parameters)    the effect with every parameter in `parameters` (dicts: name,
                                                    semantic, type, cls, rows, columns, elements, members, value --
                                                    formats/xnb.py writes them) it lacks appended, with its default

The effect compiler keeps only the parameters a shader reads. An XNA game looks its effect's parameters up by name
and sets them whether a shader reads them or not (a stock effect's ShaderIndex, a value only one technique uses), so
the port gives a compiled effect back every parameter the original declared; one no shader reads holds what the
game sets and does nothing. They go at the end of the list, since constant buffers and samplers name parameters by
their index.

The layout is MonoGame's Effect.ReadEffect and Shader's constructor: "MGFX", version, profile, key; the constant
buffers; the shaders; the parameters (class, type, name, semantic, annotations, rows, columns, elements, members,
then the values of a numeric one); the techniques; "MGFX" again.
"""
import struct

SIGNATURE = b"MGFX"
CLASSES = {"scalar": 0, "vector": 1, "matrix_rows": 2, "matrix_columns": 2, "object": 3, "struct": 4}
TYPES = {"void": 0, "bool": 1, "int": 2, "float": 3, "string": 4, "texture": 5, "texture1D": 6, "texture2D": 7,
         "texture3D": 8, "textureCUBE": 9}


class _Reader:
    def __init__(self, data, at):
        self.d, self.at = data, at

    def take(self, fmt):
        v = struct.unpack_from("<" + fmt, self.d, self.at)
        self.at += struct.calcsize("<" + fmt)
        return v[0] if len(v) == 1 else v

    def string(self):
        n = shift = 0
        while True:
            b = self.d[self.at]
            self.at += 1
            n |= (b & 0x7F) << shift
            shift += 7
            if not b & 0x80:
                break
        s = self.d[self.at:self.at + n].decode("utf-8")
        self.at += n
        return s

    def annotations(self):
        if self.take("i"):
            raise ValueError("MGFX annotations are not read")       # MonoGame's compiler writes none


def _string(s):
    raw = s.encode("utf-8")
    n, out = len(raw), bytearray()
    while True:
        b = n & 0x7F
        n >>= 7
        out.append(b | (0x80 if n else 0))
        if not n:
            return bytes(out) + raw


def _skip_parameters(r, names=None):
    for _ in range(r.take("i")):
        _cls, typ = r.take("B"), r.take("B")
        name = r.string()
        if names is not None:
            names.add(name)
        r.string()
        r.annotations()
        rows, cols = r.take("B"), r.take("B")
        elements_at = r.at
        n_elements = r.take("i")
        r.at = elements_at
        _skip_parameters(r)
        members_at = r.at
        n_members = r.take("i")
        r.at = members_at
        _skip_parameters(r)
        if not n_elements and not n_members and typ in (1, 2, 3):
            r.at += 4 * rows * cols


class _P:
    def __init__(self, d):
        self.__dict__.update(d)
        self.members = [_P(m) for m in d.get("members") or []]


def _parameter(p, element=False):
    """One parameter in MGFX's form (its value: the default, as floats -- MojoShader keeps every number a float)."""
    cls = CLASSES.get(p.cls, 3)
    typ = TYPES.get(p.type, 0)
    if p.type.startswith("sampler"):
        typ = TYPES["texture"]                  # what MonoGame calls the texture a sampler reads
    out = bytearray(struct.pack("<BB", cls, typ))
    # an array's elements are nameless (MonoGame indexes a collection by its names)
    out += _string("" if element else p.name) + _string("" if element else p.semantic or "") + struct.pack("<i", 0)
    rows, cols = (p.rows or 0), (p.columns or 0)
    if p.cls == "vector":
        rows, cols = 1, p.columns
    out += struct.pack("<BB", rows, cols)
    if p.elements:
        out += struct.pack("<i", len(p.members))
        for m in p.members:
            out += _parameter(m, element=True)
        out += struct.pack("<i", 0)
        return bytes(out)
    if p.cls == "struct":
        out += struct.pack("<i", 0) + struct.pack("<i", len(p.members))
        for m in p.members:
            out += _parameter(m)
        return bytes(out)
    out += struct.pack("<ii", 0, 0)
    if typ in (1, 2, 3):
        values = list(p.value or [])
        n = rows * cols
        values = (values + [0] * n)[:n]
        if p.cls == "matrix_columns":           # the Xbox's effect keeps a column at a time; MonoGame a row
            values = [values[c * rows + r] for r in range(rows) for c in range(cols)]
        out += struct.pack("<%df" % n, *[float(v) for v in values])
    return bytes(out)


def add_parameters(data, parameters):
    """`data` (an .mgfxo) with each of `parameters` it lacks (by name) appended to its parameter list."""
    if data[:4] != SIGNATURE:
        raise ValueError("not a MonoGame effect")
    r = _Reader(data, 10)                       # signature, version, profile, effect key
    for _ in range(r.take("i")):                # constant buffers
        r.string()
        r.take("h")
        n = r.take("i")
        r.at += 6 * n
    for _ in range(r.take("i")):                # shaders
        r.take("?")
        n = r.take("i")
        r.at += n
        for _ in range(r.take("B")):
            r.at += 3
            if r.take("?"):
                r.at += 3 + 4 + 1 + 4 + 4 + 4
            r.string()
            r.take("B")
        n = r.take("B")
        r.at += n
        for _ in range(r.take("B")):
            r.string()
            r.at += 4
    count_at = r.at
    names = set()
    _skip_parameters(r, names)
    end = r.at
    parameters = [p if hasattr(p, "name") else _P(p) for p in parameters]
    extra = [p for p in parameters if p.name and p.name not in names and
             not p.type.startswith("sampler") and p.type not in ("vertexshader", "pixelshader", "string")]
    if not extra:
        return data
    count = struct.unpack_from("<i", data, count_at)[0]
    added = b"".join(_parameter(p) for p in extra)
    return data[:count_at] + struct.pack("<i", count + len(extra)) + data[count_at + 4:end] + added + data[end:]
