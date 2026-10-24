"""xenos_hlsl.py -- an Xbox 360 shader's microcode as HLSL (Shader Model 3), the language it was written in.

    from codebreaker.decompile import xenos_hlsl
    f = xenos_hlsl.translate(shader, "PS_Main")     a Function: .text (the HLSL: input and output structs and the
                                                    function), .samplers (the sampler names it reads)

The shader is a formats/xbox/shader.Shader. What comes out reads as the compiled program did: one statement per
operation, on registers r0..rN, with the constant table's names for the constants (`World._m00_m10_m20_m30` for
a column of a matrix, `Bones[a0 / 3]` for an array read through the address register), the compiler's literal
constants written in place, and the inputs and outputs by their semantics. It compiles for the PC as vs_3_0 or
ps_3_0 (MonoGame's mgfxc).

The operations are the microcode's (decompile/xenos.py decodes it; native/x360's GPU translator, from which the
operation semantics here are taken, runs it on the console's terms): a vector and a scalar operation per ALU
instruction, reading every operand before writing; the predicate p0; kills; the address register a0; vertex
fetches by the input table's semantic (the vertex declaration on the PC converts formats, as the console's
runtime patched the fetch for it); texture fetches by the sampler bound to the fetch constant. Control flow:
conditional execs (on a bool constant or the predicate), loops (a `for` over the loop constant's count, with the
loop counter aL) and forward jumps (a `skip_to` index: the blocks a taken jump passes over do not run; out of a loop,
a break); calls and backward jumps stop with Unsupported, naming the instruction.
"""
import re
import struct

CH = "xyzw"


class Unsupported(Exception):
    pass


class Function:
    def __init__(self, text, samplers, inputs, outputs):
        self.text, self.samplers, self.inputs, self.outputs = text, samplers, inputs, outputs


def _bits(v, low, n):
    return (v >> low) & ((1 << n) - 1)


def _sext(v, n):
    return v - (1 << n) if v & (1 << (n - 1)) else v


def _num(f):
    """A float as HLSL writes it, shortest round-trip form."""
    if f != f:
        return "(0.0 / 0.0)"
    if f in (float("inf"), float("-inf")):
        return "1e38" if f > 0 else "-1e38"
    f32 = struct.unpack("<f", struct.pack("<f", f))[0]
    for digits in range(1, 10):
        s = "%.*g" % (digits, f32)
        if struct.unpack("<f", struct.pack("<f", float(s)))[0] == f32:
            break
    if "e" not in s and "." not in s:
        s += ".0"
    return s


def _plus(a, b):
    """a + b, with `a + -x` written `a - x`."""
    if b.startswith("-") and not b.startswith("-("):
        return "%s - %s" % (a, b[1:])
    return "%s + %s" % (a, b)


def _minus(a, b):
    if b.startswith("-") and not b.startswith("-("):
        return "%s + %s" % (a, b[1:])
    return "%s - %s" % (a, b if not (" " in b and not b.startswith("(")) else "(%s)" % b)


def _uniform(v):
    """`r1.xxxx` -> `r1.x`, `(0.5).xxxx` -> `0.5`: one value repeated; else None."""
    m = re.fullmatch(r"(.+)\.([xyzw])\2{3}", v)
    if not m:
        return None
    base = m.group(1)
    if base.startswith("(") and base.endswith(")") and re.fullmatch(r"\(-?[\d.e+-]+\)", base):
        return base[1:-1]
    return "%s.%s" % (base, m.group(2))


def control_flow(words, index):
    pair = index // 2
    d0, d1, d2 = words[pair * 3], words[pair * 3 + 1], words[pair * 3 + 2]
    if index % 2 == 0:
        return d0 | ((d1 & 0xFFFF) << 32)
    return (d1 >> 16) | ((d2 & 0xFFFF) << 16) | ((d2 >> 16) << 32)


EXEC_OPS = {1, 2, 3, 4, 5, 6, 13, 14}


def _lanes_text(lanes, n=None):
    """Lane tokens -> one HLSL expression. A token is (base, component) -- a component of a vector ('x'), of a
    matrix ('_m03'), or None for a whole scalar -- or ('#', number)."""
    if n is None:
        n = len(lanes)
    lanes = lanes[:n]
    bases = {b for b, _ in lanes}
    if len(bases) == 1 and "#" not in bases:
        base = lanes[0][0]
        comps = [c for _, c in lanes]
        if all(c is None for c in comps):
            return base if n == 1 else "((float%d)%s)" % (n, base)
        if all(c is not None and c.startswith("_m") for c in comps):
            return "%s.%s" % (base, "".join(comps))
        if all(c in CH for c in comps if c is not None) and None not in comps:
            return "%s.%s" % (base, "".join(comps))
    if all(b == "#" for b, _ in lanes):
        values = [c for _, c in lanes]
        if n == 1:
            return _num(values[0])
        if len(set(values)) == 1:
            return "(%s).%s" % (_num(values[0]), "x" * n)
        return "float%d(%s)" % (n, ", ".join(_num(v) for v in values))
    parts = [_lane_text(t) for t in lanes]
    return parts[0] if n == 1 else "float%d(%s)" % (n, ", ".join(parts))


def _lane_text(t):
    b, c = t
    if b == "#":
        return _num(c)
    if c is None:
        return b
    return "%s.%s" % (b, c)


class _Translator:
    def __init__(self, shader, name):
        self.s, self.name = shader, name
        self.vertex = shader.stage == "vertex"
        self.words = shader.words
        self.lines = []
        self.indent = 1
        self.samplers = []
        self.used_regs = set()
        self.uses_ps = self.uses_a0 = self.uses_p0 = False
        self.inputs, self.outputs = {}, {}     # field name -> semantic
        self.slot = 0

    def line(self, text):
        self.lines.append("    " * self.indent + text)

    def fail(self, what):
        raise Unsupported("%s shader %s, slot %d: %s" % (self.s.stage, self.name, self.slot, what))

    # ---- operands ------------------------------------------------------------------------------------------

    def temp(self, reg, relative=False):
        if relative:
            self.fail("a temporary register addressed through the loop counter")
        self.used_regs.add(reg)
        return "r%d" % reg

    def constant_lanes(self, index, relative, to_a0):
        """The four lanes of float constant c[index] (+ a0 when relative)."""
        index_register = "a0" if to_a0 else "aL"
        if not relative and index in self.s.literals:
            return [("#", v) for v in self.s.literals[index]]
        c, k = self.s.constant_at("float4", index)
        if c is None:
            if relative:
                self.fail("c[a0 + %d] outside every constant" % index)
            self.fail("c%d is in no constant of the table" % index)
        name = c.name
        if c.cls in ("matrix_columns", "matrix_rows"):
            # a register of a matrix is a column (column-major, HLSL's default) or a row of it
            per = c.columns if c.cls == "matrix_columns" else c.rows
            if relative:
                # through a0, which the program set to element * registers per element
                self.uses_a0 = True
                element = "%s / %d" % (index_register, per) + (" + %d" % (k // per) if k // per else "")
                name, k = "%s[%s]" % (name, element), k % per
            elif c.elements > 1:
                name, k = "%s[%d]" % (name, k // per), k % per
            lanes = []
            for lane in range(4):
                if c.cls == "matrix_columns":
                    lanes.append((name, "_m%d%d" % (lane, k)) if lane < c.rows else ("#", 0.0))
                else:
                    lanes.append((name, "_m%d%d" % (k, lane)) if lane < c.columns else ("#", 0.0))
            return lanes
        if relative:
            self.uses_a0 = True
            name = "%s[%s + %d]" % (name, index_register, index - c.register) if index != c.register \
                else "%s[%s]" % (name, index_register)
        elif c.elements > 1:
            name = "%s[%d]" % (name, k)
        if c.type == "bool":
            name = "(float)%s" % name if not name.endswith("]") else "(float)(%s)" % name
            return [(name, None)] + [("#", 0.0)] * 3
        width = c.columns if c.cls == "vector" else 1
        if width == 1:
            return [(name, None)] + [("#", 0.0)] * 3
        return [(name, CH[i]) if i < width else ("#", 0.0) for i in range(4)]

    def alu_lanes(self, w, i):
        """Operand i (1-3) of an ALU instruction: its four lanes after the swizzle, with abs / negate."""
        w0, w1, w2 = w
        reg = _bits(w2, 16 - 8 * (i - 1), 8)
        is_temp = _bits(w2, 31 - (i - 1), 1)
        sw = _bits(w1, 16 - 8 * (i - 1), 8)
        negate = _bits(w1, 26 - (i - 1), 1)
        if is_temp:
            base = self.temp(reg & 0x3F, bool(reg & 0x40))
            raw = [(base, CH[c]) for c in range(4)]
            absolute = bool(reg & 0x80)
        else:
            raw = self.constant_lanes(reg, self.constant_relative(w, i), bool(_bits(w1, 29, 1)))
            absolute = bool(_bits(w0, 7, 1))
        lanes = [raw[((sw >> (2 * c)) + c) & 3] for c in range(4)]
        return lanes, absolute, negate

    @staticmethod
    def constant_relative(w, i):
        w0, w1, w2 = w
        t1, t2 = _bits(w2, 31, 1), _bits(w2, 30, 1)
        c0, c1 = _bits(w1, 31, 1), _bits(w1, 30, 1)
        if i == 1:
            return c0
        if i == 2:
            return c0 if t1 else c1
        return c0 if (t1 and t2) else c1

    @staticmethod
    def modify(text, absolute, negate):
        if absolute:
            text = "abs(%s)" % text
        if negate:
            text = "-" + text if text[0].isalnum() or text[0] in "(_" else "-(%s)" % text
        return text

    def vector_operand(self, w, i, lanes_wanted):
        """Operand i's lanes `lanes_wanted` (a list of lane indices) as one expression."""
        lanes, absolute, negate = self.alu_lanes(w, i)
        picked = [lanes[c] for c in lanes_wanted]
        if negate and all(b == "#" for b, _ in picked):
            picked, negate = [("#", -v) for _, v in picked], False
        if absolute and all(b == "#" for b, _ in picked):
            picked, absolute = [("#", abs(v)) for _, v in picked], False
        return self.modify(_lanes_text(picked), absolute, negate)

    def scalar_operand(self, w, lane):
        """A scalar operand: lane `lane` (absolute, not through the swizzle) of operand 3's register (03c 2.2)."""
        _, absolute, negate = self.alu_lanes(w, 3)
        t = self._raw_lanes(w, 3)[lane]
        if t[0] == "#":
            v = abs(t[1]) if absolute else t[1]
            return _num(-v if negate else v)
        return self.modify(_lane_text(t), absolute, negate)

    def _raw_lanes(self, w, i):
        w0, w1, w2 = w
        reg = _bits(w2, 16 - 8 * (i - 1), 8)
        if _bits(w2, 31 - (i - 1), 1):
            base = self.temp(reg & 0x3F, bool(reg & 0x40))
            return [(base, CH[c]) for c in range(4)]
        return self.constant_lanes(reg, self.constant_relative(w, i), bool(_bits(w1, 29, 1)))

    # ---- ALU ------------------------------------------------------------------------------------------------

    def export_target(self, index):
        if self.vertex:
            if index == 62:
                self.outputs["Position"] = "POSITION0"
                return "output.Position"
            if index == 63:
                self.outputs["PointSize"] = "PSIZE0"
                return "output.PointSize"
            for reg, usage, uindex, mask in self.s.outputs:
                if reg == index:
                    field = "%s%d" % (usage.capitalize().replace("Texcoord", "TexCoord"), uindex)
                    self.outputs[field] = "%s%d" % (usage, uindex)
                    return "output." + field
            self.fail("export to o%d, which the output table does not name" % index)
        if index < 4:
            self.outputs["Color%d" % index] = "COLOR%d" % index
            return "output.Color%d" % index
        if index == 61:
            self.outputs["Depth"] = "DEPTH0"
            return "output.Depth"
        self.fail("export to register %d" % index)

    def emit_alu(self, w):
        w0, w1, w2 = w
        vop, sop = _bits(w2, 24, 5), _bits(w0, 26, 6)
        vmask, smask = _bits(w0, 16, 4), _bits(w0, 20, 4)
        exports = _bits(w0, 15, 1)
        vdest, sdest = _bits(w0, 0, 6), _bits(w0, 8, 6)
        vsat, ssat = _bits(w0, 24, 1), _bits(w0, 25, 1)
        if _bits(w0, 6, 1) or (_bits(w0, 14, 1) and not exports):
            self.fail("a destination addressed through the loop counter")
        predicated = _bits(w1, 28, 1)
        if predicated:
            self.uses_p0 = True
            self.line("if (%sp0)" % ("" if _bits(w1, 27, 1) else "!"))
            self.line("{")
            self.indent += 1

        # Every operand is read before anything is written; the vector result is written before the scalar one.
        # The scalar result goes through a temporary only when the vector op writes a register it reads.
        stext, side = self.scalar_expression(w, sop)
        vlanes = [c for c in range(4) if vmask >> c & 1]
        slanes = [c for c in range(4) if smask >> c & 1]
        writes_scalar = bool(slanes) or self.uses_ps_anywhere
        svalue = stext
        if self.uses_ps_anywhere:
            if stext != "ps":
                self.line("ps = %s;" % stext)
            svalue = "ps"
        elif writes_scalar and not exports and vlanes and self.reads_register(stext, vdest):
            self.line("s = %s;" % stext)
            self._s_declared = True
            svalue = "s"
        if ssat:
            svalue = "saturate(%s)" % svalue
        # the scalar op's side effects (p0, a0, a kill) do not change what the vector op reads -- unless it writes
        # a0 and the vector op reads a constant through it
        a0_late = any("a0 =" in l for l in side) and _bits(w1, 29, 1)
        if not a0_late:
            for l in side:
                self.line(l)
        if exports:
            target = self.export_target(vdest)
            both = [c for c in vlanes if c in slanes]
            only_v = [c for c in vlanes if c not in slanes]
            only_s = [c for c in slanes if c not in vlanes]
            if only_v or vop in VECTOR_SIDE_EFFECTS:
                self.emit_vector(w, vop, only_v, target if only_v else None, vsat)
            if only_s:
                self.line("%s.%s = %s;" % (target, "".join(CH[c] for c in only_s), self.widen(svalue, len(only_s))))
            if both:
                self.line("%s.%s = 1.0;" % (target, "".join(CH[c] for c in both)))
            if not vmask and not smask and _bits(w0, 14, 1):
                self.line("%s = 0.0;" % target)
        else:
            if vlanes or vop in VECTOR_SIDE_EFFECTS:
                self.emit_vector(w, vop, vlanes, self.temp(vdest) if vlanes else None, vsat)
            if slanes:
                self.line("%s.%s = %s;" % (self.temp(sdest), "".join(CH[c] for c in slanes),
                                           self.widen(svalue, len(slanes))))
        if a0_late:
            for l in side:
                self.line(l)
        if predicated:
            self.indent -= 1
            self.line("}")

    uses_ps_anywhere = False

    @staticmethod
    def widen(value, n):
        if n == 1:
            return value
        simple = re.fullmatch(r"[\w.\[\]]+", value) is not None
        return "%s.%s" % (value, "x" * n) if simple and not re.fullmatch(r"-?[\d.e+-]+", value) \
            else "((float%d)(%s))" % (n, value)

    @staticmethod
    def reads_register(text, reg):
        import re
        return re.search(r"\br%d\b" % reg, text) is not None

    def emit_vector(self, w, vop, lanes, dest, saturate):
        """The vector operation's lanes `lanes` into `dest` (a register or an export)."""
        comps = "".join(CH[c] for c in lanes)
        n = len(lanes)
        op = lambda i, ls=None: self.vector_operand(w, i, lanes if ls is None else ls)
        expr = None
        if vop == 0:
            expr = _plus(op(1), op(2))
        elif vop == 1:
            expr = "%s * %s" % (op(1), op(2))
        elif vop in (2, 3):
            a, b = op(1), op(2)
            expr = a if a == b else "%s(%s, %s)" % ("max" if vop == 2 else "min", a, b)
        elif vop in (4, 5, 6, 7):
            cmp = {4: "==", 5: ">", 6: ">=", 7: "!="}[vop]
            expr = "(%s %s %s)" % (op(1), cmp, op(2)) if n == 1 else "(float%d)(%s %s %s)" % (n, op(1), cmp, op(2))
            expr = "(float)" + expr if n == 1 else expr
        elif vop == 8:
            expr = "frac(%s)" % op(1)
        elif vop == 9:
            expr = "trunc(%s)" % op(1)
        elif vop == 10:
            expr = "floor(%s)" % op(1)
        elif vop == 11:
            expr = _plus("%s * %s" % (op(1), op(2)), op(3))
        elif vop in (12, 13, 14):
            cmp = {12: "==", 13: ">=", 14: ">"}[vop]
            zero = "0.0" if n == 1 else "0.0"
            expr = "(%s %s %s) ? %s : %s" % (op(1), cmp, zero, op(2), op(3))
        elif vop == 15:
            expr = "dot(%s, %s)" % (op(1, [0, 1, 2, 3]), op(2, [0, 1, 2, 3]))
        elif vop == 16:
            expr = "dot(%s, %s)" % (op(1, [0, 1, 2]), op(2, [0, 1, 2]))
        elif vop == 17:
            expr = "dot(%s, %s) + %s" % (op(1, [0, 1]), op(2, [0, 1]), op(3, [0]))
        elif vop == 18:
            self.cube_needed = True
            expr = "xe_cube(%s, %s)" % (op(1, [0, 1, 2, 3]), op(2, [0, 1, 2, 3]))
            if n != 4:
                expr = "(%s).%s" % (expr, comps)
        elif vop == 19:
            v = op(1, [0, 1, 2, 3])
            expr = "max(max(%s.x, %s.y), max(%s.z, %s.w))" % ((v,) * 4) if v.replace(".", "").replace("_", "").isalnum() \
                else None
            if expr is None:
                self.line("float4 m4 = %s;" % v)
                expr = "max(max(m4.x, m4.y), max(m4.z, m4.w))"
        elif vop in (20, 21, 22, 23):
            cmp = {20: "==", 21: "!=", 22: ">", 23: ">="}[vop]
            self.uses_p0 = True
            a, b = op(1, [3]), op(2, [3])
            self.line("p0 = %s == 0.0 && %s %s 0.0;" % (a, b, cmp))
            ax, bx = op(1, [0]), op(2, [0])
            expr = "(%s == 0.0 && %s %s 0.0) ? 0.0 : %s + 1.0" % (ax, bx, cmp, ax)
        elif vop in (24, 25, 26, 27):
            cmp = {24: "==", 25: ">", 26: ">=", 27: "!="}[vop]
            a, b = op(1, [0, 1, 2, 3]), op(2, [0, 1, 2, 3])
            a1, b1 = _uniform(a), _uniform(b)
            if a1 is not None and b1 is not None:
                self.line("if (%s %s %s) discard;" % (a1, cmp, b1))
            else:
                self.line("if (any(%s %s %s)) discard;" % (a, cmp, b))
            expr = "0.0"
        elif vop == 28:
            a, b = op(1, [0, 1, 2, 3]), op(2, [0, 1, 2, 3])
            expr = "float4(1.0, %s.y * %s.y, %s.z, %s.w)" % (a, b, a, b)
            if n != 4:
                expr = "(%s).%s" % (expr, comps)
        elif vop == 29:
            self.uses_a0 = True
            self.line("a0 = (int)floor(%s + 0.5);" % op(1, [3]))
            a, b = op(1), op(2)
            expr = a if a == b else "max(%s, %s)" % (a, b)
        else:
            self.fail("vector opcode %d" % vop)
        if dest is None or not lanes:
            return
        if vop in (15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27) and n > 1:
            expr = "(%s).%s" % (expr, "x" * n) if not expr.startswith("dot(") else "%s.%s" % (expr, "x" * n)
        if saturate:
            expr = "saturate(%s)" % expr
        self.line("%s.%s = %s;" % (dest, comps, expr))

    def scalar_expression(self, w, sop):
        """The scalar op's result as an expression, and statements its side effects need -> (text, [lines])."""
        w1 = w[1]
        sw = _bits(w1, 0, 8)
        a_lane, b_lane = ((sw >> 6) + 3) & 3, sw & 3
        side = []
        if sop in SCALAR_TWO:
            a, b = self.scalar_operand(w, a_lane), self.scalar_operand(w, b_lane)
        elif sop in (42, 43, 44, 45, 46, 47):
            # mulsc/addsc/subsc: a constant lane and a temp register lane (03c 2.4)
            w0, _, w2 = w
            c_lanes = self.constant_lanes(_bits(w2, 0, 8), self.constant_relative(w, 3), bool(_bits(w1, 29, 1)))
            t_reg = (sop & 1) | (_bits(w2, 29, 1) << 1) | (_bits(w1, 0, 8) & 0x3C)
            neg, ab = _bits(w1, 24, 1), _bits(w0, 7, 1)
            ca = c_lanes[a_lane]
            a = _num((-1 if neg else 1) * (abs(ca[1]) if ab else ca[1])) if ca[0] == "#" else \
                self.modify(_lane_text(ca), ab, neg)
            b = self.modify("%s.%s" % (self.temp(t_reg), CH[b_lane]), ab, neg)
        elif sop in (33, 50):
            a = b = None
        else:
            a = self.scalar_operand(w, a_lane)
            b = None
        prev = "ps"
        if sop in (1, 3, 4, 26) or (sop == 50 and _bits(w[0], 20, 4)):
            self.uses_ps_anywhere = True
        if sop == 0 or sop in (44, 45):
            return _plus(a, b), side
        if sop == 1:
            return "%s + %s" % (a, prev), side
        if sop == 2 or sop in (42, 43):
            return "%s * %s" % (a, b), side
        if sop == 3:
            return "%s * %s" % (a, prev), side
        if sop == 4:
            return "(%s == -1e38 || %s == -1e38 || isinf(%s) || isnan(%s)) ? -1e38 : %s * %s" % (prev, b, prev, prev, a, prev), side
        if sop == 5:
            return (a if a == b else "max(%s, %s)" % (a, b)), side
        if sop == 6:
            return (a if a == b else "min(%s, %s)" % (a, b)), side
        if sop in (7, 8, 9, 10):
            return "(%s %s 0.0) ? 1.0 : 0.0" % (a, {7: "==", 8: ">", 9: ">=", 10: "!="}[sop]), side
        if sop == 11:
            return "frac(%s)" % a, side
        if sop == 12:
            return "trunc(%s)" % a, side
        if sop == 13:
            return "floor(%s)" % a, side
        if sop == 14:
            return "exp2(%s)" % a, side
        if sop == 15:
            return "max(log2(%s), -1e38)" % a, side
        if sop == 16:
            return "log2(%s)" % a, side
        if sop in (17, 18, 19):
            return "1.0 / %s" % a, side
        if sop in (20, 21, 22):
            return "rsqrt(%s)" % a, side
        if sop in (23, 24):
            self.uses_a0 = True
            side.append("a0 = (int)%s(%s);" % ("round" if sop == 23 else "floor", a))
            return a if sop == 24 or b is None else "max(%s, %s)" % (a, b), side
        if sop == 25:
            return _minus(a, b), side
        if sop == 26:
            return "%s - %s" % (a, prev), side
        if sop in (27, 28, 29, 30):
            self.uses_p0 = True
            cond = "%s %s 0.0" % (a, {27: "==", 28: "!=", 29: ">", 30: ">="}[sop])
            side.append("p0 = %s;" % cond)
            return "(%s) ? 0.0 : 1.0" % cond, side
        if sop == 31:
            self.uses_p0 = True
            side.append("p0 = %s == 1.0;" % a)
            return "(%s == 1.0) ? 0.0 : (%s == 0.0 ? 1.0 : %s)" % (a, a, a), side
        if sop == 32:
            self.uses_p0 = True
            side.append("p0 = %s - 1.0 <= 0.0;" % a)
            return "(%s - 1.0 <= 0.0) ? 0.0 : %s - 1.0" % (a, a), side
        if sop == 33:
            self.uses_p0 = True
            side.append("p0 = false;")
            return "1e38", side
        if sop == 34:
            self.uses_p0 = True
            side.append("p0 = %s == 0.0;" % a)
            return a, side
        if sop in (35, 36, 37, 38, 39):
            cond = {35: "%s == 0.0", 36: "%s > 0.0", 37: "%s >= 0.0", 38: "%s != 0.0", 39: "%s == 1.0"}[sop] % a
            side.append("if (%s) discard;" % cond)
            return "0.0", side
        if sop == 40:
            return "sqrt(%s)" % a, side
        if sop in (46, 47):
            return _minus(a, b), side
        if sop == 48:
            return "sin(%s)" % a, side
        if sop == 49:
            return "cos(%s)" % a, side
        if sop == 50:
            return prev, side
        self.fail("scalar opcode %d" % sop)

    # ---- fetches ----------------------------------------------------------------------------------------------

    def fetch_destination(self, w0, w1, value_lanes):
        """The fetch's destination swizzle: 0-3 a fetched lane, 4 zero, 5 one, 7 left as it is."""
        dest = self.temp(_bits(w0, 12, 6), bool(_bits(w0, 18, 1)))
        sw = _bits(w1, 0, 12)
        comps, parts = "", []
        for c in range(4):
            sel = (sw >> (3 * c)) & 7
            if sel == 7:
                continue
            if sel == 6:
                self.fail("fetch destination swizzle 6")
            comps += CH[c]
            parts.append(value_lanes[sel] if sel < 4 else ("#", 0.0 if sel == 4 else 1.0))
        if comps:
            self.line("%s.%s = %s;" % (dest, comps, _lanes_text(parts)))

    def emit_fetch(self, w):
        w0, w1, w2 = w
        opcode = _bits(w0, 0, 5)
        predicated = _bits(w1, 31, 1)
        if predicated:
            self.uses_p0 = True
            self.line("if (%sp0)" % ("" if _bits(w2, 31, 1) else "!"))
            self.line("{")
            self.indent += 1
        if opcode == 0:
            self.emit_vertex_fetch(w)
        elif opcode == 1:
            self.emit_texture_fetch(w)
        elif opcode == 24:                  # setTexLOD: the level of detail later fetches may use
            self.uses_lod = True
            self.line("lod = %s.%s;" % (self.temp(_bits(w0, 5, 6), bool(_bits(w0, 11, 1))), CH[_bits(w0, 26, 6) & 3]))
        else:
            self.fail("fetch instruction %d" % opcode)
        if predicated:
            self.indent -= 1
            self.line("}")

    def emit_vertex_fetch(self, w):
        w0, w1, w2 = w
        entry = next((e for e in self.s.inputs if e[0] == self.slot), None)
        if entry is None:
            self.fail("a vertex fetch the input table does not name")
        _, usage, index = entry
        field = "%s%d" % (usage.capitalize().replace("Texcoord", "TexCoord").replace("Blendweight", "BlendWeight")
                          .replace("Blendindices", "BlendIndices"), index)
        self.inputs[field] = "%s%d" % (usage, index)
        self.fetch_destination(w0, w1, [("input." + field, CH[c]) for c in range(4)])

    def sampler_for(self, tf):
        c, _ = self.s.constant_at("sampler", tf)
        if c is None:
            self.fail("texture fetch constant %d is bound to no sampler" % tf)
        if c.name not in self.samplers:
            self.samplers.append(c.name)
        return c

    def emit_texture_fetch(self, w):
        w0, w1, w2 = w
        tf = _bits(w0, 20, 5)
        dim = _bits(w2, 14, 2)
        src = self.temp(_bits(w0, 5, 6), bool(_bits(w0, 11, 1)))
        sw = _bits(w0, 26, 6)
        sampler = self.sampler_for(tf).name
        if _bits(w0, 25, 1):
            self.fail("unnormalized texture coordinates")
        if _bits(w2, 16, 10):
            self.fail("texture fetch offsets")
        bias = _sext(_bits(w2, 2, 7), 7) / 16.0
        computed_lod = _bits(w1, 28, 1) and not self.vertex
        register_lod = bool(_bits(w1, 29, 1))
        if dim == 1:
            coord, kind = "%s.%s" % (src, CH[sw & 3] + CH[(sw >> 2) & 3]), "2D"
        elif dim == 0:
            coord, kind = "%s.%s" % (src, CH[sw & 3]), "1D"
        elif dim == 2:
            coord, kind = "%s.%s" % (src, CH[sw & 3] + CH[(sw >> 2) & 3] + CH[(sw >> 4) & 3]), "3D"
        else:
            # cube: the face coordinates the cube instruction made (+1.5) and the face, back to a direction
            self.cube_fetch_needed = True
            coord = "xe_cube_direction(%s.%s)" % (src, CH[sw & 3] + CH[(sw >> 2) & 3] + CH[(sw >> 4) & 3])
            kind = "CUBE"
        level = ("lod + %s" % _num(bias) if bias else "lod") if register_lod else _num(bias)
        if computed_lod and not register_lod:
            call = "tex%s(%s, %s)" % (kind, sampler, coord) if not bias else \
                "tex%sbias(%s, float4(%s, %s))" % (kind, sampler, self.pad(coord, kind), _num(bias))
        elif computed_lod:
            call = "tex%sbias(%s, float4(%s, %s))" % (kind, sampler, self.pad(coord, kind), level)
        else:
            call = "tex%slod(%s, float4(%s, %s))" % (kind, sampler, self.pad(coord, kind), level)
        self.line("t = %s;" % call)
        self._t_declared = True
        self.fetch_destination(w0, w1, [("t", CH[c]) for c in range(4)])

    @staticmethod
    def pad(coord, kind):
        n = {"1D": 1, "2D": 2, "3D": 3, "CUBE": 3}[kind]
        return coord + ", 0.0" * (3 - n)

    # ---- control flow ------------------------------------------------------------------------------------------

    def run(self):
        """The control flow, then the body: the CF instructions fill the first slots, up to the first exec's
        address; the program ends at an unconditional exece."""
        words = self.words
        program = []
        first_exec = None
        index = 0
        while (index // 2) * 3 + 2 < len(words):
            if first_exec is not None and index >= first_exec * 2:
                break
            cf = control_flow(words, index)
            op = _bits(cf, 44, 4)
            if op in EXEC_OPS:
                address = _bits(cf, 0, 12)
                first_exec = address if first_exec is None else min(first_exec, address)
            program.append(cf)
            index += 1
            if op == 2:
                break
        # loops: a loop start and the loop end jumping back to the instruction after it, a `for` over the loop
        # constant's count; jumps: forward ones become `skip_to` (the blocks a taken jump passes over do not run),
        # out of a loop a break as well; others stop
        self.jumps = False
        self.loop_end = {}
        for i, cf in enumerate(program):
            op = _bits(cf, 44, 4)
            if op == 7:
                j = next((j for j in range(i + 1, len(program)) if _bits(program[j], 44, 4) == 8 and
                          _bits(program[j], 0, 13) == i + 1), None)
                if j is None:
                    self.slot = i
                    self.fail("a loop without its end")
                self.loop_end[i] = j
            elif op == 11:
                if _bits(cf, 0, 13) <= i:
                    self.slot = i
                    self.fail("a backward jump in the control flow")
                self.jumps = True
            elif op in (9, 10):
                self.slot = i
                self.fail("control flow instruction %d (calls are not translated yet)" % op)
            elif op in (4, 6, 14) and i + 1 < len(program):
                self.slot = i
                self.fail("a conditional end of the program before its last instruction")
        self.program = program
        # one pass first to learn whether the previous scalar result is read anywhere
        self.uses_ps_anywhere = False
        for pass_ in range(2):
            keep = self.uses_ps_anywhere
            self.__init_body()
            self.uses_ps_anywhere = keep
            self.emit_range(0, len(program), [])
        return self.assemble()

    def emit_range(self, lo, hi, loops):
        """CF instructions lo..hi-1; `loops`: the (start, end) of each loop around them, outermost first."""
        program = self.program
        i = lo
        while i < hi:
            cf = program[i]
            op = _bits(cf, 44, 4)
            if op in EXEC_OPS:
                if self.jumps:
                    self.line("if (skip_to <= %d)" % i)
                    self.line("{")
                    self.indent += 1
                self.emit_exec(cf)
                if self.jumps:
                    self.indent -= 1
                    self.line("}")
            elif op == 11:
                target = _bits(cf, 0, 13)
                if _bits(cf, 13, 1):
                    cond = "skip_to <= %d" % i
                elif _bits(cf, 14, 1):
                    self.uses_p0 = True
                    cond = "skip_to <= %d && %sp0" % (i, "" if _bits(cf, 42, 1) else "!")
                else:
                    cond = "skip_to <= %d && %s%s" % (i, "" if _bits(cf, 42, 1) else "!", self.bool_name(_bits(cf, 34, 8)))
                leaves = loops and target > loops[-1][1]
                self.line("if (%s) { skip_to = %d;%s }" % (cond, target, " break;" if leaves else ""))
            elif op == 7:
                end = self.loop_end[i]
                self.emit_loop(i, end, loops)
                i = end + 1
                continue
            i += 1

    def emit_loop(self, start, end, loops):
        cf, cf_end = self.program[start], self.program[end]
        count, first, step = self.loop_constant(_bits(cf, 16, 5))
        depth = len(loops)
        it = "i%d" % depth
        self.uses_aL = True
        if self.jumps:
            self.line("if (skip_to <= %d)" % start)
            self.line("{")
            self.indent += 1
        self.line("[loop] for (int %s = 0; %s < %s; %s++)" % (it, it, count, it))
        self.line("{")
        self.indent += 1
        if not _bits(cf, 13, 1):            # the loop sets aL (or keeps the outer one's)
            self.line("aL = %s;" % (first if step == "0" else "%s + %s * %s" % (first, it, step) if first != "0"
                                    else "%s * %s" % (it, step) if step != "1" else it))
        if self.jumps:
            self.line("skip_to = %d;" % (start + 1))
        self.emit_range(start + 1, end, loops + [(start, end)])
        if _bits(cf_end, 21, 1):            # the loop end's predicated break
            self.uses_p0 = True
            self.line("if (%sp0) break;" % ("" if _bits(cf_end, 42, 1) else "!"))
        self.indent -= 1
        self.line("}")
        if loops and not _bits(self.program[loops[-1][0]], 13, 1):
            outer = self.loop_constant(_bits(self.program[loops[-1][0]], 16, 5))
            self.line("aL = %s + i%d * %s;" % (outer[1], depth - 1, outer[2]))
        if self.jumps:
            self.indent -= 1
            self.line("}")

    def loop_constant(self, k):
        """Loop constant k -> (count, aL's first value, its step) as expressions: an int parameter's own value is
        the count (start and step its defaults' second and third), an int4's are its x, y and z."""
        if k in self.s.loop_literals:       # one the compiler made: its count, start and step are numbers
            v = self.s.loop_literals[k]
            step = (v >> 16) & 0xFF
            return str(v & 0xFF), str((v >> 8) & 0xFF), str(step - 256 if step & 0x80 else step)
        register = k - (16 if not self.vertex else 0)
        c, offset = self.s.constant_at("int4", register)
        if c is None:
            self.fail("loop constant i%d is in no constant of the table" % k)
        name = c.name if c.elements <= 1 else "%s[%d]" % (c.name, offset)
        if c.cls == "scalar":
            d = [int(v) if abs(v) < 1e6 and v == int(v) else 0 for v in (c.default or [0, 0, 1, 0])[4 * offset:4 * offset + 4]]
            return name, str(d[1]) if len(d) > 1 else "0", str(d[2]) if len(d) > 2 else "1"
        return "%s.x" % name, "%s.y" % name, "%s.z" % name

    def bool_name(self, b):
        if not self.vertex:
            b -= 128                        # a pixel shader's bool constants follow the vertex shader's
        c, k = self.s.constant_at("bool", b)
        if c is None:
            self.fail("bool constant b%d is in no constant of the table" % b)
        return c.name if c.elements <= 1 else "%s[%d]" % (c.name, k)

    def __init_body(self):
        self.lines, self.indent = [], 1
        self.samplers, self.used_regs = [], set()
        self.inputs, self.outputs = {}, {}
        self.uses_a0 = self.uses_p0 = self.uses_aL = self.uses_lod = False
        self._s_declared = self._t_declared = False
        self.cube_needed = self.cube_fetch_needed = False

    def emit_exec(self, cf):
        op = _bits(cf, 44, 4)
        address, count, sequence = _bits(cf, 0, 12), _bits(cf, 12, 3), _bits(cf, 16, 12)
        condition = _bits(cf, 42, 1)
        guarded = False
        if op in (3, 4, 13, 14):            # on a bool constant
            self.line("if (%s%s)" % ("" if condition else "!", self.bool_name(_bits(cf, 34, 8))))
            guarded = True
        elif op in (5, 6):                  # on the predicate
            self.uses_p0 = True
            self.line("if (%sp0)" % ("" if condition else "!"))
            guarded = True
        if guarded:
            self.line("{")
            self.indent += 1
        for i in range(count):
            self.slot = address + i
            w = tuple(self.words[self.slot * 3:self.slot * 3 + 3])
            if (sequence >> (2 * i)) & 1:
                self.emit_fetch(w)
            else:
                self.emit_alu(w)
        if guarded:
            self.indent -= 1
            self.line("}")

    # ---- the function ------------------------------------------------------------------------------------------

    def assemble(self):
        n = self.name
        out = []
        pixel_inputs = {}
        if not self.vertex:
            for reg, usage, index, mask in self.s.inputs:
                field = "%s%d" % (usage.capitalize().replace("Texcoord", "TexCoord"), index)
                pixel_inputs[reg] = (field, mask)
                self.inputs[field] = "%s%d" % (usage, index)
        out.append("struct %s_Input\n{" % n)
        for field, semantic in self.inputs.items():
            out.append("    float4 %s : %s;" % (field, semantic))
        if not self.inputs:
            out.append("    float4 Unused : TEXCOORD15;" if not self.vertex else "    float4 Position0 : POSITION0;")
        out.append("};\n")
        out.append("struct %s_Output\n{" % n)
        if self.vertex and "Position" not in self.outputs:
            self.outputs = dict([("Position", "POSITION0")] + list(self.outputs.items()))
        for field, semantic in self.outputs.items():
            out.append("    float%s %s : %s;" % ("" if field in ("Depth", "PointSize") else "4", field, semantic))
        out.append("};\n")
        out.append("%s_Output %s(%s_Input input)\n{" % (n, n, n))
        out.append("    %s_Output output = (%s_Output)0;" % (n, n))
        regs = sorted(self.used_regs | set(pixel_inputs))
        if regs:
            out.append("    float4 %s;" % ", ".join("r%d = 0.0" % r for r in regs))
        if self.uses_ps_anywhere:
            out.append("    float ps = 0.0;")
        if self.uses_p0:
            out.append("    bool p0 = false;")
        if self.uses_a0:
            out.append("    int a0 = 0;")
        if self.uses_aL:
            out.append("    int aL = 0;")
        if self.uses_lod:
            out.append("    float lod = 0.0;")
        if self._t_declared:
            out.append("    float4 t;")
        if self._s_declared:
            out.append("    float s;")
        if self.jumps:
            out.append("    int skip_to = 0;")
        for reg, (field, mask) in sorted(pixel_inputs.items()):
            comps = "".join(CH[c] for c in range(4) if mask >> c & 1) or "xyzw"
            out.append("    r%d.%s = input.%s.%s;" % (reg, comps, field, comps))
        out.extend(self.lines)
        out.append("    return output;\n}\n")
        text = "\n".join(out)
        return Function(text, self.samplers, dict(self.inputs), dict(self.outputs))


SCALAR_TWO = {0, 2, 4, 5, 6, 25}
VECTOR_SIDE_EFFECTS = {20, 21, 22, 23, 24, 25, 26, 27, 29}

HELPERS = """// The Xbox 360 GPU's cube instruction: the face coordinates (+1.5), the major axis times two, the face index.
float4 xe_cube(float4 a, float4 b)
{
    float3 d = float3(a.z, a.y, b.x);
    float3 m = abs(d);
    float4 r;
    if (m.z >= m.x && m.z >= m.y) r = float4(d.z < 0 ? d.y : d.y, d.z < 0 ? -d.x : d.x, 2.0 * d.z, d.z < 0 ? 5.0 : 4.0);
    else if (m.y >= m.x) r = float4(d.y < 0 ? -d.z : d.z, d.y < 0 ? d.x : d.x, 2.0 * d.y, d.y < 0 ? 3.0 : 2.0);
    else r = float4(d.x < 0 ? d.y : d.y, d.x < 0 ? d.z : -d.z, 2.0 * d.x, d.x < 0 ? 1.0 : 0.0);
    return float4(-r.x, r.y, r.z, r.w);
}

float3 xe_cube_direction(float3 c)
{
    float s = c.x * 2.0 - 3.0, t = c.y * 2.0 - 3.0;
    float f = c.z;
    return f < 0.5 ? float3(1.0, -t, -s) : f < 1.5 ? float3(-1.0, -t, s) : f < 2.5 ? float3(s, 1.0, t) :
           f < 3.5 ? float3(s, -1.0, -t) : f < 4.5 ? float3(s, -t, 1.0) : float3(-s, -t, -1.0);
}
"""


def translate(shader, name):
    """formats/xbox/shader.Shader -> Function (raises Unsupported, naming the instruction, for what is not done)."""
    t = _Translator(shader, name)
    f = t.run()
    f.needs_helpers = t.cube_needed or t.cube_fetch_needed
    return f
