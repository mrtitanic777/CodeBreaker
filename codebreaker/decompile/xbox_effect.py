"""xbox_effect.py -- an Xbox 360 effect (an XNA Effect's content) as an .fx file: its HLSL source.

    from codebreaker.decompile import xbox_effect
    text = xbox_effect.to_fx(data, "Bloom")     the effect as .fx text (raises xenos_hlsl.Unsupported, naming
                                                the shader and instruction, for what is not translated yet)

The parameters are declared as the effect declares them (type, array size, semantic, default value), the
samplers with their states and textures, each shader as HLSL (decompile/xenos_hlsl.py: one function per shader,
named for the technique and pass that use it first), and the techniques with their passes' render states. It
compiles for the PC as fx with vs_3_0 / ps_3_0 shaders: MonoGame's mgfxc builds it (xna/port.py).

A pass that picks its shader from an array by an expression -- XNA's stock effects (BasicEffect and the rest, which
games copy) write `VertexShader = (VSArray[VSIndices[ShaderIndex]])` -- becomes one technique per value of the
selecting parameter: the technique itself for 0, <technique>__<n> for n; the PC runtime makes the one the game's
ShaderIndex names current (XnaPort.Effects.SetShaderIndex), as MonoGame's own stock effects do.
"""
import struct

from codebreaker.decompile import xenos_hlsl
from codebreaker.formats.xbox import effect as fxformat
from codebreaker.formats.xbox import shader as shformat

# the render states MonoGame's effect compiler takes; the rest are written as comments
PASS_STATES = {"AlphaBlendEnable", "SrcBlend", "DestBlend", "BlendOp", "SeparateAlphaBlendEnable", "SrcBlendAlpha",
               "DestBlendAlpha", "BlendOpAlpha", "BlendFactor", "ColorWriteEnable", "ColorWriteEnable1",
               "ColorWriteEnable2", "ColorWriteEnable3", "ZEnable", "ZWriteEnable", "ZFunc", "DepthBias",
               "SlopeScaleDepthBias", "StencilEnable", "StencilFail", "StencilZFail", "StencilPass", "StencilFunc",
               "StencilRef", "StencilMask", "StencilWriteMask", "TwoSidedStencilMode", "CCW_StencilFail",
               "CCW_StencilZFail", "CCW_StencilPass", "CCW_StencilFunc", "CullMode", "FillMode",
               "MultiSampleAntiAlias", "ScissorTestEnable"}
SAMPLER_STATES = {"AddressU", "AddressV", "AddressW", "BorderColor", "MagFilter", "MinFilter", "MipFilter",
                  "MipMapLodBias", "MaxMipLevel", "MaxAnisotropy"}


def _type_name(p):
    base = {"float": "float", "int": "int", "bool": "bool"}.get(p.type)
    if base is None:
        return None
    if p.cls == "scalar":
        return base
    if p.cls == "vector":
        return "%s%d" % (base, p.columns)
    if p.cls in ("matrix_rows", "matrix_columns"):
        # declared without row_major (MonoGame's compiler takes no such parameter): the shaders read a matrix's
        # elements by row and column (_m01), which mean the same whichever way it is stored
        return "%s%dx%d" % (base, p.rows, p.columns)
    return None


def _literal(p, values):
    """Values of a numeric parameter as an HLSL initializer."""
    def one(v):
        if p.type == "bool":
            return "true" if v else "false"
        if p.type == "int":
            return str(int(v))
        return xenos_hlsl._num(v)
    n = p.rows * p.columns
    if p.cls == "scalar":
        return one(values[0])
    t = _type_name(p)
    if p.cls == "matrix_columns":                 # stored a column at a time; the initializer is by rows
        values = [values[c * p.rows + r] for r in range(p.rows) for c in range(p.columns)]
    return "%s(%s)" % (t, ", ".join(one(v) for v in values[:n]))


def _string_annotations(fx, p):
    parts = []
    for a in p.annotations:
        if a.type == "string" and a.value is not None:
            parts.append('string %s = "%s";' % (a.name, fx.string(a.value).replace('"', "'")))
        elif _type_name(a) and isinstance(a.value, list):
            parts.append("%s %s = %s;" % (_type_name(a), a.name, _literal(a, a.value)))
    return " <%s>" % " ".join(parts) if parts else ""


def _texture_of(fx, state):
    """The texture parameter a sampler's Texture state names."""
    ref = getattr(state.parameter, "reference", None)
    if ref:
        return ref
    for p in fx.parameters:
        if p.type.startswith("texture") and p.value == state.value:
            return p.name
    return None


def _array_selection(fx, state):
    """A shader state choosing from a shader array by an expression: -> ([object id per selector value], selector
    name), or None. The expression's preshader names the array (first), the selecting int and the int array
    indexing the shader array, if there is one; that is the stock effects' form, and the only one taken."""
    e = getattr(state.parameter, "expression", None)
    if not e:
        return None
    n = struct.unpack_from(">I" if fx.big_endian else "<I", e, 0)[0]
    array = fx.parameter(e[4:4 + n].split(b"\0", 1)[0].decode("latin-1"))
    if array is None or not array.elements:
        return None
    rest = e[4 + n:]
    named = [p for p in fx.parameters if p.type == "int" and (p.name.encode() + b"\0") in rest]
    selector = next((p for p in named if not p.elements and p.cls == "scalar"), None)
    table = next((p for p in named if p.elements), None)
    if selector is None:
        return None
    indices = [v[0] for v in table.value] if table else list(range(array.elements))
    return [array.value[i] for i in indices], selector.name


def to_fx(data, name="Effect"):
    fx = fxformat.read(data)
    out = ["// %s: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect.py) from its microcode." % name,
           "// The shaders read as the compiled programs did: registers r0..rN, the constants by their names.", ""]
    # the shaders, by the passes using them: each pass as one or more variants (vertex, pixel function names)
    functions, names = {}, {}

    def function(blob, fname):
        if blob not in names:
            fname = "".join(c if c.isalnum() or c == "_" else "_" for c in fname)
            names[blob] = fname
            functions[fname] = xenos_hlsl.translate(shformat.read(blob), fname)
        return names[blob]

    variants, selectors = {}, set()
    for t in fx.techniques:
        for i, p in enumerate(t.passes):
            choices = {}
            for kind, prefix in (("VertexShader", "VS"), ("PixelShader", "PS")):
                s = p.state(kind)
                if s is None:
                    continue
                chosen = _array_selection(fx, s)
                if chosen:
                    ids, selector = chosen
                    selectors.add(selector)
                    array = getattr(s.parameter, "expression")[4:].split(b"\0", 1)[0].decode("latin-1")
                    element = {oid: k for k, oid in enumerate(fx.parameter(array).value)}
                    choices[kind] = [function(fx.objects[oid], "%s_%d" % (array, element[oid])) for oid in ids]
                elif fx.objects.get(s.value):
                    choices[kind] = [function(fx.objects[s.value], "%s_%s_%s" % (prefix, t.name, p.name or "P%d" % i))]
            count = max([len(v) for v in choices.values()] or [1])
            variants[(t.name, i)] = [tuple((choices[k][v if len(choices[k]) > 1 else 0] if k in choices else None)
                                           for k in ("VertexShader", "PixelShader")) for v in range(count)]
    # the parameters
    for p in fx.parameters:
        if p.type in fxformat.SAMPLER_TYPES or p.type in ("vertexshader", "pixelshader"):
            continue
        semantic = " : %s" % p.semantic if p.semantic else ""
        array = "[%d]" % p.elements if p.elements else ""
        annotations = _string_annotations(fx, p)
        if p.type.startswith("texture"):
            out.append("%s %s%s%s;" % (p.type, p.name, semantic, annotations))
            continue
        if p.type == "string":
            out.append('string %s%s = "%s";' % (p.name, annotations, fx.string(p.value) if p.value is not None else ""))
            continue
        t = _type_name(p)
        if t is None:
            out.append("// %s: a %s %s parameter, not written" % (p.name, p.cls, p.type))
            continue
        init = ""
        if p.elements:
            values = [m.value for m in p.members]
            if any(any(v) for v in values if v):
                init = " = { %s }" % ", ".join(_literal(m, m.value) for m in p.members)
        elif p.value and any(p.value):
            init = " = %s" % _literal(p, p.value)
        out.append("%s %s%s%s%s%s;" % (t, p.name, array, semantic, annotations, init))
    out.append("")
    # the samplers
    for p in fx.parameters:
        if p.type not in fxformat.SAMPLER_TYPES or p.elements:
            continue
        kind = {"sampler": "sampler", "sampler1D": "sampler1D", "sampler2D": "sampler2D", "sampler3D": "sampler3D",
                "samplerCUBE": "samplerCUBE"}[p.type]
        states = []
        for s in p.value or []:
            if s.name == "Texture":
                tex = _texture_of(fx, s)
                if tex:
                    states.append("Texture = <%s>;" % tex)
            elif s.name in SAMPLER_STATES and s.text is not None:
                states.append("%s = %s;" % (s.name, s.text))
        body = " = sampler_state\n{\n    %s\n}" % "\n    ".join(states) if states else ""
        out.append("%s %s%s;\n" % (kind, p.name, body))
    if any(getattr(f, "needs_helpers", False) for f in functions.values()):
        out.append(xenos_hlsl.HELPERS)
    for f in functions.values():
        out.append(f.text)
    # the techniques (one per selector value when a pass chooses its shaders from arrays)
    for t in fx.techniques:
        count = max(len(variants[(t.name, i)]) for i in range(len(t.passes))) if t.passes else 1
        for v in range(count):
            if v == 0 and count > 1:
                out.append("// %s__<n>: the shaders %s n chooses" % (t.name, " / ".join(sorted(selectors))))
            out.append("technique %s\n{" % (t.name if v == 0 else "%s__%d" % (t.name, v)))
            for i, p in enumerate(t.passes):
                vs, ps = variants[(t.name, i)][min(v, len(variants[(t.name, i)]) - 1)]
                out.append("    pass %s\n    {" % (p.name or "P%d" % i))
                for s in p.states:
                    if s.name == "VertexShader":
                        if vs:
                            out.append("        VertexShader = compile vs_3_0 %s();" % vs)
                    elif s.name == "PixelShader":
                        if ps:
                            out.append("        PixelShader = compile ps_3_0 %s();" % ps)
                    elif s.text is None:
                        continue
                    elif s.name in PASS_STATES:
                        out.append("        %s = %s;" % (s.name, s.text))
                    else:
                        out.append("        // %s = %s;   (the Xbox's; the PC has no such state)" % (s.name, s.text))
                out.append("    }")
            out.append("}\n")
    return "\n".join(out)
