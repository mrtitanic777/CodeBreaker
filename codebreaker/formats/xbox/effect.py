"""effect.py -- an Xbox 360 Direct3D effect (an XNA Effect's content, or the SDK's fxc /Tfx_2_0 output), as data.

    from codebreaker.formats.xbox import effect
    fx = effect.read(data)              Effect: .parameters, .techniques, .shaders {object id: container bytes}
    fx.parameter("World")               a Parameter: name, semantic, type, cls, rows, columns, elements,
                                        members, value (numbers, a string, a texture's object id, or a
                                        sampler's states), annotations
    pass_.states                        [State]: name ("AlphaBlendEnable", "VertexShader", ...), index, value

XNA writes an effect as 0xBCF00BCF, the offset of the Direct3D effect in it, and the effect -- the
D3DX9 effect layout (fx_2_0: 0xFEFF0901, then the offset of the parameter and technique lists in the
data that follows), which Wine's d3dx9 effect.c reads too. The Xbox 360's XNA content pipeline writes
it big-endian; the SDK's fxc writes it little-endian. Its shaders are Xbox shader containers
(formats/xbox/shader.py), and the numbering of its states is the Xbox's own (STATES below: the SDK's
fxc compiled one state at a time and read back -- the PC's table numbers them differently).
"""
import struct

XNA_MAGIC = 0xBCF00BCF
D3DX_MAGIC = 0xFEFF0901

# D3DXPARAMETER_TYPE and D3DXPARAMETER_CLASS
TYPES = ["void", "bool", "int", "float", "string", "texture", "texture1D", "texture2D", "texture3D",
         "textureCUBE", "sampler", "sampler1D", "sampler2D", "sampler3D", "samplerCUBE", "pixelshader",
         "vertexshader"]
CLASSES = ["scalar", "vector", "matrix_rows", "matrix_columns", "object", "struct"]
OBJECT_TYPES = {"string", "texture", "texture1D", "texture2D", "texture3D", "textureCUBE", "pixelshader",
                "vertexshader"}
SAMPLER_TYPES = {"sampler", "sampler1D", "sampler2D", "sampler3D", "samplerCUBE"}

# The Xbox 360's effect state numbering: operation -> the state's name in an .fx file. Found by compiling an
# effect setting each state with the SDK's fxc (53-55 and 65-87 are the Xbox's own states, none of which an
# effect for the PC has).
STATES = dict(enumerate([
    "ZEnable", "FillMode", "ZWriteEnable", "AlphaTestEnable", "SrcBlend", "DestBlend", "CullMode", "ZFunc",
    "AlphaRef", "AlphaFunc", "AlphaBlendEnable", "StencilEnable", "StencilFail", "StencilZFail", "StencilPass",
    "StencilFunc", "StencilRef", "StencilMask", "StencilWriteMask"] +
    ["Wrap%d" % i for i in range(16)] + [
    "ClipPlaneEnable", "PointSize", "PointSize_Min", "PointSize_Max", "PointSpriteEnable", "MultiSampleAntiAlias",
    "MultiSampleMask", "ColorWriteEnable", "BlendOp", "ScissorTestEnable", "SlopeScaleDepthBias",
    "MinTessellationLevel", "MaxTessellationLevel", "TwoSidedStencilMode", "CCW_StencilFail", "CCW_StencilZFail",
    "CCW_StencilPass", "CCW_StencilFunc"]))
STATES.update({56: "ColorWriteEnable1", 57: "ColorWriteEnable2", 58: "ColorWriteEnable3", 59: "BlendFactor",
               60: "DepthBias", 61: "SeparateAlphaBlendEnable", 62: "SrcBlendAlpha", 63: "DestBlendAlpha",
               64: "BlendOpAlpha", 88: "LastPixel", 91: "FVF", 92: "VertexShader", 93: "PixelShader",
               110: "Texture", 111: "AddressU", 112: "AddressV", 113: "AddressW", 114: "BorderColor",
               115: "MagFilter", 116: "MinFilter", 117: "MipFilter", 118: "MipMapLodBias", 119: "MaxMipLevel",
               120: "MaxAnisotropy"})

# The Xbox 360's values of the enumerated states (the SDK's d3d9types.h), by their .fx names.
_BLEND = {0: "Zero", 1: "One", 4: "SrcColor", 5: "InvSrcColor", 6: "SrcAlpha", 7: "InvSrcAlpha", 8: "DestColor",
          9: "InvDestColor", 10: "DestAlpha", 11: "InvDestAlpha", 12: "BlendFactor", 13: "InvBlendFactor",
          14: "BlendFactor", 15: "InvBlendFactor", 16: "SrcAlphaSat"}   # 14/15: the Xbox's constant alpha
_CMP = {0: "Never", 1: "Less", 2: "Equal", 3: "LessEqual", 4: "Greater", 5: "NotEqual", 6: "GreaterEqual", 7: "Always"}
_STENCIL = {0: "Keep", 1: "Zero", 2: "Replace", 3: "IncrSat", 4: "DecrSat", 5: "Invert", 6: "Incr", 7: "Decr"}
_ADDRESS = {0: "Wrap", 1: "Mirror", 2: "Clamp", 3: "MirrorOnce", 4: "Border", 5: "MirrorOnce", 6: "Border", 7: "MirrorOnce"}
_FILTER = {0: "Point", 1: "Linear", 2: "None", 4: "Anisotropic"}
VALUES = {"SrcBlend": _BLEND, "DestBlend": _BLEND, "SrcBlendAlpha": _BLEND, "DestBlendAlpha": _BLEND,
          "BlendOp": {0: "Add", 1: "Subtract", 2: "Min", 3: "Max", 4: "RevSubtract"},
          "CullMode": {0: "None", 2: "CW", 6: "CCW"}, "FillMode": {0: "Solid", 1: "Point", 0x25: "WireFrame"},
          "ZFunc": _CMP, "AlphaFunc": _CMP, "StencilFunc": _CMP, "CCW_StencilFunc": _CMP,
          "StencilFail": _STENCIL, "StencilZFail": _STENCIL, "StencilPass": _STENCIL, "CCW_StencilFail": _STENCIL,
          "CCW_StencilZFail": _STENCIL, "CCW_StencilPass": _STENCIL,
          "AddressU": _ADDRESS, "AddressV": _ADDRESS, "AddressW": _ADDRESS,
          "MagFilter": _FILTER, "MinFilter": _FILTER, "MipFilter": _FILTER}


class Parameter:
    def __init__(self):
        self.name = self.semantic = ""
        self.type = self.cls = ""
        self.rows = self.columns = self.elements = 0
        self.members = []                   # a struct's members, or an array's elements
        self.value = None
        self.flags = 0
        self.annotations = []

    @property
    def size(self):
        """Bytes of value data (an object or sampler has none here)."""
        if self.elements:
            return sum(m.size for m in self.members)
        if self.cls == "struct":
            return sum(m.size for m in self.members)
        if self.cls in ("scalar", "vector", "matrix_rows", "matrix_columns"):
            return 4 * self.rows * self.columns
        return 0

    def __repr__(self):
        return "<%s %s %s>" % (self.type, self.cls, self.name)


class State:
    def __init__(self, operation, index, parameter):
        self.operation, self.index, self.parameter = operation, index, parameter
        self.name = STATES.get(operation, "state_%d" % operation)

    @property
    def value(self):
        return self.parameter.value

    @property
    def text(self):
        """The value as an .fx file writes it: an enumerated state by its name, a flag as true/false, else the
        number (None for an object: a shader, a texture)."""
        v = self.parameter.value
        if not isinstance(v, list) or len(v) != 1:
            return None
        v = v[0]
        if self.name in VALUES:
            return VALUES[self.name].get(v, str(v))
        if self.parameter.type == "bool" or self.name.endswith("Enable") or self.name == "TwoSidedStencilMode":
            return "true" if v else "false"
        return repr(v) if isinstance(v, float) else str(v)

    def __repr__(self):
        return "<%s[%d] = %r>" % (self.name, self.index, self.value)


class Pass:
    def __init__(self, name, annotations, states):
        self.name, self.annotations, self.states = name, annotations, states

    def state(self, name):
        return next((s for s in self.states if s.name == name), None)


class Technique:
    def __init__(self, name, annotations, passes):
        self.name, self.annotations, self.passes = name, annotations, passes


class Effect:
    def __init__(self):
        self.parameters, self.techniques = [], []
        self.objects = {}                   # object id -> bytes (strings, shaders)
        self.big_endian = True

    def parameter(self, name):
        return next((p for p in self.parameters if p.name == name), None)

    def string(self, object_id):
        data = self.objects.get(object_id, b"")
        return data.split(b"\0", 1)[0].decode("latin-1")


class _Reader:
    def __init__(self, data, base, big):
        self.data, self.base, self.e = data, base, ">" if big else "<"

    def u32(self, at):
        return struct.unpack_from(self.e + "I", self.data, self.base + at)[0]

    def name(self, at):
        """A length-prefixed string at `at` (relative to the data base)."""
        n = self.u32(at)
        return self.data[self.base + at + 4:self.base + at + 4 + n].split(b"\0", 1)[0].decode("latin-1")


def _typedef(r, at, parent=None):
    """A type definition at `at`; -> (Parameter, offset after it)."""
    p = Parameter()
    if parent is None:
        p.type = TYPES[r.u32(at)] if r.u32(at) < len(TYPES) else "type_%d" % r.u32(at)
        p.cls = CLASSES[r.u32(at + 4)]
        p.name = r.name(r.u32(at + 8))
        p.semantic = r.name(r.u32(at + 12))
        p.elements = r.u32(at + 16)
        at += 20
        if p.cls == "vector":
            p.columns, p.rows = r.u32(at), r.u32(at + 4)
            at += 8
        elif p.cls in ("scalar", "matrix_rows", "matrix_columns"):
            p.rows, p.columns = r.u32(at), r.u32(at + 4)
            at += 8
        elif p.cls == "struct":
            count = r.u32(at)
            at += 4
            p.members = []
            for _ in range(count):
                m, at = _typedef(r, at)
                p.members.append(m)
    else:
        p.type, p.cls, p.name, p.semantic = parent.type, parent.cls, parent.name, parent.semantic
        p.rows, p.columns = parent.rows, parent.columns
        p.members = [_copy(m) for m in parent.members]
        return p, at
    if p.elements:
        element_members = p.members
        p.members = []
        for _ in range(p.elements):
            e, _ = _typedef(r, at, parent=p)
            e.members = [_copy(m) for m in element_members]
            p.members.append(e)
    return p, at


def _copy(p):
    q = Parameter()
    q.__dict__.update(p.__dict__)
    q.members = [_copy(m) for m in p.members]
    return q


def _value(r, p, at, ptr, numbers):
    """Fill p's value from the value data at `at` (numbers) and the stream at ptr (objects, samplers);
    -> (ptr after, bytes of value data used)."""
    if p.elements or p.cls == "struct":
        used = 0
        for m in p.members:
            ptr, n = _value(r, m, at + used, ptr, numbers)
            used += n
        if p.elements:
            p.value = [m.value for m in p.members]
        return ptr, used
    if p.cls in ("scalar", "vector", "matrix_rows", "matrix_columns"):
        n = p.rows * p.columns
        raw = [r.u32(at + 4 * i) for i in range(n)]
        if p.type == "float":
            p.value = [struct.unpack("<f", struct.pack("<I", w))[0] for w in raw]
        else:
            p.value = [w if w < 0x80000000 else w - (1 << 32) for w in raw]
        return ptr, 4 * n
    if p.type in OBJECT_TYPES:
        p.value = r.u32(ptr)                 # the object's id
        return ptr + 4, 0
    if p.type in SAMPLER_TYPES:
        count = r.u32(ptr)
        ptr += 4
        states = []
        for _ in range(count):
            s, ptr = _state(r, ptr)
            states.append(s)
        p.value = states
        return ptr, 0
    return ptr, 0


def _state(r, ptr):
    operation, index = r.u32(ptr), r.u32(ptr + 4)
    p, _ = _typedef(r, r.u32(ptr + 8))
    _value(r, p, r.u32(ptr + 12), r.u32(ptr + 12), True)
    if p.type in OBJECT_TYPES:              # an object's value is its id, at the value offset
        p.value = r.u32(r.u32(ptr + 12))
    return State(operation, index, p), ptr + 16


def _annotations(r, ptr, count):
    out = []
    for _ in range(count):
        p, _ = _typedef(r, r.u32(ptr))
        value_at = r.u32(ptr + 4)
        _value(r, p, value_at, value_at, True)
        if p.type in OBJECT_TYPES:
            p.value = r.u32(value_at)
        out.append(p)
        ptr += 8
    return out, ptr


def read(data):
    """An effect's bytes (XNA's, with or without its header, or fxc's) -> Effect."""
    if len(data) >= 8 and struct.unpack_from(">I", data, 0)[0] == XNA_MAGIC:
        data = data[struct.unpack_from(">I", data, 4)[0]:]
    elif len(data) >= 8 and struct.unpack_from("<I", data, 0)[0] == XNA_MAGIC:
        data = data[struct.unpack_from("<I", data, 4)[0]:]
    if struct.unpack_from(">I", data, 0)[0] == D3DX_MAGIC:
        big = True
    elif struct.unpack_from("<I", data, 0)[0] == D3DX_MAGIC:
        big = False
    else:
        raise ValueError("not a Direct3D effect (no 0xFEFF0901)")
    r = _Reader(data, 8, big)
    start = struct.unpack_from((">" if big else "<") + "I", data, 4)[0]
    fx = Effect()
    fx.big_endian = big
    ptr = start
    n_params, n_tech = r.u32(ptr), r.u32(ptr + 4)
    ptr += 16                               # and the shader and object counts
    for _ in range(n_params):
        p, _ = _typedef(r, r.u32(ptr))
        value_at, p.flags, n_annotations = r.u32(ptr + 4), r.u32(ptr + 8), r.u32(ptr + 12)
        ptr += 16
        p.annotations, ptr = _annotations(r, ptr, n_annotations)
        if p.type in SAMPLER_TYPES and not p.elements:
            _value(r, p, value_at, value_at, True)
        elif p.type in OBJECT_TYPES and not p.elements:
            p.value = r.u32(value_at)
        else:
            _value(r, p, value_at, value_at, True)
        fx.parameters.append(p)
    for _ in range(n_tech):
        name = r.name(r.u32(ptr))
        n_annotations, n_passes = r.u32(ptr + 4), r.u32(ptr + 8)
        ptr += 12
        annotations, ptr = _annotations(r, ptr, n_annotations)
        passes = []
        for _ in range(n_passes):
            pname = r.name(r.u32(ptr))
            n_pa, n_states = r.u32(ptr + 4), r.u32(ptr + 8)
            ptr += 12
            pass_annotations, ptr = _annotations(r, ptr, n_pa)
            states = []
            for _ in range(n_states):
                s, ptr = _state(r, ptr)
                states.append(s)
            passes.append(Pass(pname, pass_annotations, states))
        fx.techniques.append(Technique(name, annotations, passes))
    n_strings, n_resources = r.u32(ptr), r.u32(ptr + 4)
    ptr += 8
    for _ in range(n_strings):
        oid, size = r.u32(ptr), r.u32(ptr + 4)
        fx.objects[oid] = data[8 + ptr + 8:8 + ptr + 8 + size]
        ptr += 8 + ((size + 3) & ~3)
    for _ in range(n_resources):
        technique, index, element, state_index, usage = (r.u32(ptr + 4 * i) for i in range(5))
        ptr += 20
        if technique == 0xFFFFFFFF:          # a sampler parameter's state
            p = fx.parameters[index]
            if element != 0xFFFFFFFF and p.elements:
                p = p.members[element]
            state = p.value[state_index]
        else:
            state = fx.techniques[technique].passes[index].states[state_index]
        size = r.u32(ptr)
        blob = data[8 + ptr + 4:8 + ptr + 4 + size]
        ptr += 4 + ((size + 3) & ~3)
        if usage == 0:                      # the object itself: a shader
            fx.objects[state.parameter.value] = blob
        elif usage == 1:                    # the name of a parameter holding it
            state.parameter.reference = blob.split(b"\0", 1)[0].decode("latin-1")
        else:                               # an expression (a preshader): kept, not evaluated
            state.parameter.expression = blob
    return fx
