// codebreaker/decompile/xenos_hlsl.cpp -- typed C++ port of codebreaker.decompile.xenos_hlsl.

#include "codebreaker/decompile/xenos_hlsl.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#include "py_os.hpp"

namespace codebreaker::decompile::xenos_hlsl {

const py::str DOC = R"DOC(xenos_hlsl.py -- an Xbox 360 shader's microcode as HLSL (Shader Model 3), the language it was written in.

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
)DOC";

const py::str CH = py::str("xyzw");

// "..." % (args) -- Python's %-formatting
template <class... A> static py::str fmt(const char* f, const A&... a) { return py::percent(py::str(f), a...); }

static const py::str EMPTY = py::str("");

// ---------------------------------------------------------------------------------------------- helpers

int64_t _bits(int64_t v, int64_t low, int64_t n) {
    return (v >> low) & ((static_cast<int64_t>(1) << n) - 1);
}

int64_t _sext(int64_t v, int64_t n) {
    return (v & (static_cast<int64_t>(1) << (n - 1))) ? v - (static_cast<int64_t>(1) << n) : v;
}

// struct.pack("<f", f) then back: the float32 value of f, OverflowError past the float range as Python's pack
static double to_f32_(double f) {
    float y = static_cast<float>(f);
    if (std::isinf(y) && !std::isinf(f)) throw py::OverflowError("float too large to pack with f format");
    return static_cast<double>(y);
}

py::str _num(double f) {
    if (f != f) return py::str("(0.0 / 0.0)");
    if (std::isinf(f)) return py::str(f > 0 ? "1e38" : "-1e38");
    double f32 = to_f32_(f);
    std::string s;
    for (int digits = 1; digits < 10; ++digits) {
        char buf[64];
        std::snprintf(buf, sizeof buf, "%.*g", digits, f32);
        s = buf;
        if (to_f32_(std::strtod(buf, nullptr)) == f32) break;
    }
    if (s.find('e') == std::string::npos && s.find('.') == std::string::npos) s += ".0";
    return py::str(s);
}

py::str _plus(const py::str& a, const py::str& b) {
    if (b.startswith(py::str("-")) && !b.startswith(py::str("-("))) return fmt("%s - %s", a, b.slice_from(1));
    return fmt("%s + %s", a, b);
}

py::str _minus(const py::str& a, const py::str& b) {
    if (b.startswith(py::str("-")) && !b.startswith(py::str("-("))) return fmt("%s + %s", a, b.slice_from(1));
    return fmt("%s - %s", a, !(b.contains(py::str(" ")) && !b.startswith(py::str("("))) ? b : fmt("(%s)", b));
}

std::optional<py::str> _uniform(const py::str& v) {
    auto m = py::re::fullmatch(py::str("(.+)\\.([xyzw])\\2{3}"), v);
    if (!m) return std::nullopt;
    py::str base = m->group(1);
    if (base.startswith(py::str("(")) && base.endswith(py::str(")")) &&
        py::re::fullmatch(py::str("\\(-?[\\d.e+-]+\\)"), base))
        return base.slice(1, -1);
    return fmt("%s.%s", base, m->group(2));
}

int64_t control_flow(const py::list<int64_t>& words, int64_t index) {
    int64_t pair = py::floordiv(index, 2);
    int64_t d0 = words[pair * 3], d1 = words[pair * 3 + 1], d2 = words[pair * 3 + 2];
    if (py::mod(index, 2) == 0) return d0 | ((d1 & 0xFFFF) << 32);
    return (d1 >> 16) | ((d2 & 0xFFFF) << 16) | ((d2 >> 16) << 32);
}

const py::set<int64_t> EXEC_OPS = {1, 2, 3, 4, 5, 6, 13, 14};

py::str _lane_text(const Lane& t) {
    if (t.base == py::str("#")) return _num(t.value);
    if (!t.comp) return t.base;
    return fmt("%s.%s", t.base, *t.comp);
}

py::str _lanes_text(const py::list<Lane>& lanes_in, std::optional<int64_t> n_opt) {
    int64_t n = n_opt ? *n_opt : py::len(lanes_in);
    py::list<Lane> lanes = lanes_in.slice(0, n);
    py::set<py::str> bases;
    for (const Lane& l : lanes) bases.add(l.base);
    if (py::len(bases) == 1 && !bases.contains(py::str("#"))) {
        py::str base = lanes[0].base;
        bool all_none = true, all_matrix = true, all_xyzw = true, none_present = false;
        for (const Lane& l : lanes) {
            if (l.comp) {
                all_none = false;
                if (!l.comp->startswith(py::str("_m"))) all_matrix = false;
                if (!CH.contains(*l.comp)) all_xyzw = false;
            } else {
                all_matrix = false;
                none_present = true;
            }
        }
        if (all_none) return n == 1 ? base : fmt("((float%d)%s)", n, base);
        if (all_matrix) {
            py::str joined = EMPTY;
            for (const Lane& l : lanes) joined += *l.comp;
            return fmt("%s.%s", base, joined);
        }
        if (all_xyzw && !none_present) {
            py::str joined = EMPTY;
            for (const Lane& l : lanes) joined += *l.comp;
            return fmt("%s.%s", base, joined);
        }
    }
    bool all_numbers = true;
    for (const Lane& l : lanes) all_numbers = all_numbers && l.base == py::str("#");
    if (all_numbers) {
        py::list<double> values;
        for (const Lane& l : lanes) values.append(l.value);
        if (n == 1) return _num(values[0]);
        // (len(set(values)) == 1: a float set; NaN lanes count as one when their bits are the same)
        bool same = !values.empty();
        for (int64_t i = 1; i < py::len(values); ++i) {
            double a = values[0], b = values[i];
            bool eq = (a == b) || (a != a && b != b && std::memcmp(&a, &b, sizeof a) == 0);
            same = same && eq;
        }
        if (same) return fmt("(%s).%s", _num(values[0]), py::str(std::u32string(static_cast<size_t>(n), U'x')));
        py::list<py::str> parts;
        for (double v : values) parts.append(_num(v));
        return fmt("float%d(%s)", n, py::join(py::str(", "), parts));
    }
    py::list<py::str> parts;
    for (const Lane& t : lanes) parts.append(_lane_text(t));
    if (n == 1) return parts[0];
    return fmt("float%d(%s)", n, py::join(py::str(", "), parts));
}

const py::set<int64_t> SCALAR_TWO = {0, 2, 4, 5, 6, 25};
const py::set<int64_t> VECTOR_SIDE_EFFECTS = {20, 21, 22, 23, 24, 25, 26, 27, 29};

// ---------------------------------------------------------------------------------------------- Translator_

static py::str xs_(int64_t n) { return py::str(std::u32string(static_cast<size_t>(n), U'x')); }

// "".join(CH[c] for c in lanes)
static py::str comps_of_(const py::list<int64_t>& lanes) {
    py::str out = EMPTY;
    for (int64_t c : lanes) out += CH[c];
    return out;
}

// a, b, c = w with w a tuple of the words found: ValueError as Python's unpacking
static Word3 unpack3_(const py::list<int64_t>& w) {
    if (py::len(w) < 3) throw py::ValueError(fmt("not enough values to unpack (expected 3, got %d)", py::len(w)));
    return Word3{w[0], w[1], w[2]};
}

// usage.capitalize().replace("Texcoord", "TexCoord")
static py::str usage_field_(const py::str& usage) {
    return usage.capitalize().replace(py::str("Texcoord"), py::str("TexCoord"));
}

Translator_::Translator_(const shader::Shader& shader, const py::str& name_)
    : s(shader), name(name_), vertex(shader.stage == py::str("vertex")), words(shader.words) {}

void Translator_::line(const py::str& text) {
    py::str pad = EMPTY;
    for (int64_t i = 0; i < indent; ++i) pad += py::str("    ");
    lines.append(pad + text);
}

void Translator_::fail(const py::str& what) {
    throw Unsupported(fmt("%s shader %s, slot %d: %s", s.stage, name, slot, what));
}

// ---- operands

py::str Translator_::temp(int64_t reg, bool relative) {
    if (relative) fail(py::str("a temporary register addressed through the loop counter"));
    used_regs.add(reg);
    return fmt("r%d", reg);
}

py::list<Lane> Translator_::constant_lanes(int64_t index, bool relative, bool to_a0) {
    py::str index_register = py::str(to_a0 ? "a0" : "aL");
    if (!relative && s.literals.contains(index)) {
        py::list<Lane> out;
        auto [x, y, z, w] = s.literals.at(index);
        for (double v : {x, y, z, w}) out.append(number_lane(v));
        return out;
    }
    auto [cc, k] = s.constant_at(py::str("float4"), index);
    if (!cc) {
        if (relative) fail(fmt("c[a0 + %d] outside every constant", index));
        fail(fmt("c%d is in no constant of the table", index));
    }
    const shader::Constant& c = *cc;
    py::str nm = c.name;
    if (c.cls == py::str("matrix_columns") || c.cls == py::str("matrix_rows")) {
        // a register of a matrix is a column (column-major, HLSL's default) or a row of it
        int64_t per = c.cls == py::str("matrix_columns") ? c.columns : c.rows;
        if (relative) {
            // through a0, which the program set to element * registers per element
            uses_a0 = true;
            py::str element = fmt("%s / %d", index_register, per) +
                              (py::floordiv(k, per) ? fmt(" + %d", py::floordiv(k, per)) : EMPTY);
            nm = fmt("%s[%s]", nm, element);
            k = py::mod(k, per);
        } else if (c.elements > 1) {
            nm = fmt("%s[%d]", nm, py::floordiv(k, per));
            k = py::mod(k, per);
        }
        py::list<Lane> lanes;
        for (int64_t lane = 0; lane < 4; ++lane) {
            if (c.cls == py::str("matrix_columns"))
                lanes.append(lane < c.rows ? Lane{nm, fmt("_m%d%d", lane, k), 0.0} : number_lane(0.0));
            else
                lanes.append(lane < c.columns ? Lane{nm, fmt("_m%d%d", k, lane), 0.0} : number_lane(0.0));
        }
        return lanes;
    }
    if (relative) {
        uses_a0 = true;
        nm = index != c.register_ ? fmt("%s[%s + %d]", nm, index_register, index - c.register_)
                                  : fmt("%s[%s]", nm, index_register);
    } else if (c.elements > 1) {
        nm = fmt("%s[%d]", nm, k);
    }
    if (c.type == py::str("bool")) {
        nm = !nm.endswith(py::str("]")) ? fmt("(float)%s", nm) : fmt("(float)(%s)", nm);
        return py::list<Lane>{Lane{nm, std::nullopt, 0.0}, number_lane(0.0), number_lane(0.0), number_lane(0.0)};
    }
    int64_t width = c.cls == py::str("vector") ? c.columns : 1;
    if (width == 1)
        return py::list<Lane>{Lane{nm, std::nullopt, 0.0}, number_lane(0.0), number_lane(0.0), number_lane(0.0)};
    py::list<Lane> out;
    for (int64_t i = 0; i < 4; ++i) out.append(i < width ? Lane{nm, CH[i], 0.0} : number_lane(0.0));
    return out;
}

std::tuple<py::list<Lane>, bool, bool> Translator_::alu_lanes(const Word3& w, int64_t i) {
    int64_t w0 = w[0], w1 = w[1], w2 = w[2];
    int64_t reg = _bits(w2, 16 - 8 * (i - 1), 8);
    int64_t is_temp = _bits(w2, 31 - (i - 1), 1);
    int64_t sw = _bits(w1, 16 - 8 * (i - 1), 8);
    bool negate = _bits(w1, 26 - (i - 1), 1) != 0;
    py::list<Lane> raw;
    bool absolute;
    if (is_temp) {
        py::str base = temp(reg & 0x3F, (reg & 0x40) != 0);
        for (int64_t c = 0; c < 4; ++c) raw.append(Lane{base, CH[c], 0.0});
        absolute = (reg & 0x80) != 0;
    } else {
        raw = constant_lanes(reg, constant_relative(w, i), _bits(w1, 29, 1) != 0);
        absolute = _bits(w0, 7, 1) != 0;
    }
    py::list<Lane> lanes;
    for (int64_t c = 0; c < 4; ++c) lanes.append(raw[((sw >> (2 * c)) + c) & 3]);
    return {lanes, absolute, negate};
}

bool Translator_::constant_relative(const Word3& w, int64_t i) {
    int64_t w1 = w[1], w2 = w[2];
    int64_t t1 = _bits(w2, 31, 1), t2 = _bits(w2, 30, 1);
    int64_t c0 = _bits(w1, 31, 1), c1 = _bits(w1, 30, 1);
    if (i == 1) return c0 != 0;
    if (i == 2) return (t1 ? c0 : c1) != 0;
    return ((t1 && t2) ? c0 : c1) != 0;
}

py::str Translator_::modify(const py::str& text_in, bool absolute, bool negate) {
    py::str text = text_in;
    if (absolute) text = fmt("abs(%s)", text);
    if (negate) text = (text[0].isalnum() || py::str("(_").contains(text[0])) ? py::str("-") + text : fmt("-(%s)", text);
    return text;
}

py::str Translator_::vector_operand(const Word3& w, int64_t i, const py::list<int64_t>& lanes_wanted) {
    auto [lanes, absolute, negate] = alu_lanes(w, i);
    py::list<Lane> picked;
    for (int64_t c : lanes_wanted) picked.append(lanes[c]);
    bool all_numbers = true;
    for (const Lane& l : picked) all_numbers = all_numbers && l.base == py::str("#");
    if (negate && all_numbers) {
        py::list<Lane> n2;
        for (const Lane& l : picked) n2.append(number_lane(-l.value));
        picked = n2;
        negate = false;
    }
    if (absolute && all_numbers) {
        py::list<Lane> n2;
        for (const Lane& l : picked) n2.append(number_lane(std::fabs(l.value)));
        picked = n2;
        absolute = false;
    }
    return modify(_lanes_text(picked), absolute, negate);
}

py::list<Lane> Translator_::_raw_lanes(const Word3& w, int64_t i) {
    int64_t w1 = w[1], w2 = w[2];
    int64_t reg = _bits(w2, 16 - 8 * (i - 1), 8);
    if (_bits(w2, 31 - (i - 1), 1)) {
        py::str base = temp(reg & 0x3F, (reg & 0x40) != 0);
        py::list<Lane> out;
        for (int64_t c = 0; c < 4; ++c) out.append(Lane{base, CH[c], 0.0});
        return out;
    }
    return constant_lanes(reg, constant_relative(w, i), _bits(w1, 29, 1) != 0);
}

py::str Translator_::scalar_operand(const Word3& w, int64_t lane) {
    auto [unused, absolute, negate] = alu_lanes(w, 3);
    (void)unused;
    Lane t = _raw_lanes(w, 3)[lane];
    if (t.base == py::str("#")) {
        double v = absolute ? std::fabs(t.value) : t.value;
        return _num(negate ? -v : v);
    }
    return modify(_lane_text(t), absolute, negate);
}

// ---- ALU

py::str Translator_::export_target(int64_t index) {
    if (vertex) {
        if (index == 62) {
            outputs.set(py::str("Position"), py::str("POSITION0"));
            return py::str("output.Position");
        }
        if (index == 63) {
            outputs.set(py::str("PointSize"), py::str("PSIZE0"));
            return py::str("output.PointSize");
        }
        for (const shader::Interpolator& o : s.outputs) {
            auto [reg, usage, uindex, mask] = o;
            (void)mask;
            if (reg == index) {
                py::str field = fmt("%s%d", usage_field_(usage), uindex);
                outputs.set(field, fmt("%s%d", usage, uindex));
                return py::str("output.") + field;
            }
        }
        fail(fmt("export to o%d, which the output table does not name", index));
    }
    if (index < 4) {
        outputs.set(fmt("Color%d", index), fmt("COLOR%d", index));
        return fmt("output.Color%d", index);
    }
    if (index == 61) {
        outputs.set(py::str("Depth"), py::str("DEPTH0"));
        return py::str("output.Depth");
    }
    fail(fmt("export to register %d", index));
}

void Translator_::emit_alu(const py::list<int64_t>& wl) {
    Word3 w = unpack3_(wl);
    int64_t w0 = w[0], w1 = w[1], w2 = w[2];
    int64_t vop = _bits(w2, 24, 5), sop = _bits(w0, 26, 6);
    int64_t vmask = _bits(w0, 16, 4), smask = _bits(w0, 20, 4);
    bool exports = _bits(w0, 15, 1) != 0;
    int64_t vdest = _bits(w0, 0, 6), sdest = _bits(w0, 8, 6);
    bool vsat = _bits(w0, 24, 1) != 0, ssat = _bits(w0, 25, 1) != 0;
    if (_bits(w0, 6, 1) || (_bits(w0, 14, 1) && !exports))
        fail(py::str("a destination addressed through the loop counter"));
    bool predicated = _bits(w1, 28, 1) != 0;
    if (predicated) {
        uses_p0 = true;
        line(fmt("if (%sp0)", _bits(w1, 27, 1) ? EMPTY : py::str("!")));
        line(py::str("{"));
        indent += 1;
    }

    // Every operand is read before anything is written; the vector result is written before the scalar one.
    // The scalar result goes through a temporary only when the vector op writes a register it reads.
    auto [stext, side] = scalar_expression(w, sop);
    py::list<int64_t> vlanes, slanes;
    for (int64_t c = 0; c < 4; ++c)
        if ((vmask >> c) & 1) vlanes.append(c);
    for (int64_t c = 0; c < 4; ++c)
        if ((smask >> c) & 1) slanes.append(c);
    bool writes_scalar = !slanes.empty() || uses_ps_anywhere;
    py::str svalue = stext;
    if (uses_ps_anywhere) {
        if (stext != py::str("ps")) line(fmt("ps = %s;", stext));
        svalue = py::str("ps");
    } else if (writes_scalar && !exports && !vlanes.empty() && reads_register(stext, vdest)) {
        line(fmt("s = %s;", stext));
        s_declared_ = true;
        svalue = py::str("s");
    }
    if (ssat) svalue = fmt("saturate(%s)", svalue);
    // the scalar op's side effects (p0, a0, a kill) do not change what the vector op reads -- unless it writes
    // a0 and the vector op reads a constant through it
    bool a0_found = false;
    for (const py::str& l : side) a0_found = a0_found || l.contains(py::str("a0 ="));
    bool a0_late = a0_found && _bits(w1, 29, 1) != 0;
    if (!a0_late) {
        for (const py::str& l : side) line(l);
    }
    if (exports) {
        py::str target = export_target(vdest);
        py::list<int64_t> both, only_v, only_s;
        for (int64_t c : vlanes) (slanes.contains(c) ? both : only_v).append(c);
        for (int64_t c : slanes)
            if (!vlanes.contains(c)) only_s.append(c);
        if (!only_v.empty() || VECTOR_SIDE_EFFECTS.contains(vop))
            emit_vector(w, vop, only_v, !only_v.empty() ? std::optional<py::str>(target) : std::nullopt, vsat);
        if (!only_s.empty())
            line(fmt("%s.%s = %s;", target, comps_of_(only_s), widen(svalue, py::len(only_s))));
        if (!both.empty()) line(fmt("%s.%s = 1.0;", target, comps_of_(both)));
        if (!vmask && !smask && _bits(w0, 14, 1)) line(fmt("%s = 0.0;", target));
    } else {
        if (!vlanes.empty() || VECTOR_SIDE_EFFECTS.contains(vop))
            emit_vector(w, vop, vlanes, !vlanes.empty() ? std::optional<py::str>(temp(vdest)) : std::nullopt, vsat);
        if (!slanes.empty()) {
            py::str t = temp(sdest);
            line(fmt("%s.%s = %s;", t, comps_of_(slanes), widen(svalue, py::len(slanes))));
        }
    }
    if (a0_late) {
        for (const py::str& l : side) line(l);
    }
    if (predicated) {
        indent -= 1;
        line(py::str("}"));
    }
}

py::str Translator_::widen(const py::str& value, int64_t n) {
    if (n == 1) return value;
    bool simple = py::re::fullmatch(py::str("[\\w.\\[\\]]+"), value).has_value();
    return (simple && !py::re::fullmatch(py::str("-?[\\d.e+-]+"), value)) ? fmt("%s.%s", value, xs_(n))
                                                                          : fmt("((float%d)(%s))", n, value);
}

bool Translator_::reads_register(const py::str& text, int64_t reg) {
    return py::re::search(fmt("\\br%d\\b", reg), text).has_value();
}

void Translator_::emit_vector(const Word3& w, int64_t vop, const py::list<int64_t>& lanes,
                              const std::optional<py::str>& dest, bool saturate) {
    py::str comps = comps_of_(lanes);
    int64_t n = py::len(lanes);
    auto op = [&](int64_t i) { return vector_operand(w, i, lanes); };
    auto opl = [&](int64_t i, const py::list<int64_t>& ls) { return vector_operand(w, i, ls); };
    const py::list<int64_t> all4 = {0, 1, 2, 3};
    py::str expr;
    if (vop == 0) {
        py::str a = op(1);
        expr = _plus(a, op(2));
    } else if (vop == 1) {
        py::str a = op(1);
        expr = fmt("%s * %s", a, op(2));
    } else if (vop == 2 || vop == 3) {
        py::str a = op(1), b = op(2);
        expr = a == b ? a : fmt("%s(%s, %s)", py::str(vop == 2 ? "max" : "min"), a, b);
    } else if (vop >= 4 && vop <= 7) {
        py::str cmp = py::str(vop == 4 ? "==" : vop == 5 ? ">" : vop == 6 ? ">=" : "!=");
        if (n == 1) {
            py::str a = op(1);
            py::str b = op(2);
            expr = fmt("(%s %s %s)", a, cmp, b);
        } else {
            py::str a = op(1);
            py::str b = op(2);
            expr = fmt("(float%d)(%s %s %s)", n, a, cmp, b);
        }
        if (n == 1) expr = py::str("(float)") + expr;
    } else if (vop == 8) {
        expr = fmt("frac(%s)", op(1));
    } else if (vop == 9) {
        expr = fmt("trunc(%s)", op(1));
    } else if (vop == 10) {
        expr = fmt("floor(%s)", op(1));
    } else if (vop == 11) {
        py::str a = op(1);
        py::str b = op(2);
        py::str prod = fmt("%s * %s", a, b);
        expr = _plus(prod, op(3));
    } else if (vop == 12 || vop == 13 || vop == 14) {
        py::str cmp = py::str(vop == 12 ? "==" : vop == 13 ? ">=" : ">");
        py::str zero = py::str("0.0");
        py::str a = op(1);
        py::str b = op(2);
        py::str c = op(3);
        expr = fmt("(%s %s %s) ? %s : %s", a, cmp, zero, b, c);
    } else if (vop == 15) {
        py::str a = opl(1, all4);
        expr = fmt("dot(%s, %s)", a, opl(2, all4));
    } else if (vop == 16) {
        const py::list<int64_t> xyz = {0, 1, 2};
        py::str a = opl(1, xyz);
        expr = fmt("dot(%s, %s)", a, opl(2, xyz));
    } else if (vop == 17) {
        const py::list<int64_t> xy = {0, 1}, x = {0};
        py::str a = opl(1, xy);
        py::str b = opl(2, xy);
        expr = fmt("dot(%s, %s) + %s", a, b, opl(3, x));
    } else if (vop == 18) {
        cube_needed = true;
        py::str a = opl(1, all4);
        expr = fmt("xe_cube(%s, %s)", a, opl(2, all4));
        if (n != 4) expr = fmt("(%s).%s", expr, comps);
    } else if (vop == 19) {
        py::str v = opl(1, all4);
        if (v.replace(py::str("."), EMPTY).replace(py::str("_"), EMPTY).isalnum()) {
            expr = fmt("max(max(%s.x, %s.y), max(%s.z, %s.w))", v, v, v, v);
        } else {
            line(fmt("float4 m4 = %s;", v));
            expr = py::str("max(max(m4.x, m4.y), max(m4.z, m4.w))");
        }
    } else if (vop >= 20 && vop <= 23) {
        py::str cmp = py::str(vop == 20 ? "==" : vop == 21 ? "!=" : vop == 22 ? ">" : ">=");
        uses_p0 = true;
        const py::list<int64_t> w3 = {3}, w0 = {0};
        py::str a = opl(1, w3);
        py::str b = opl(2, w3);
        line(fmt("p0 = %s == 0.0 && %s %s 0.0;", a, b, cmp));
        py::str ax = opl(1, w0);
        py::str bx = opl(2, w0);
        expr = fmt("(%s == 0.0 && %s %s 0.0) ? 0.0 : %s + 1.0", ax, bx, cmp, ax);
    } else if (vop >= 24 && vop <= 27) {
        py::str cmp = py::str(vop == 24 ? "==" : vop == 25 ? ">" : vop == 26 ? ">=" : "!=");
        py::str a = opl(1, all4);
        py::str b = opl(2, all4);
        std::optional<py::str> a1 = _uniform(a), b1 = _uniform(b);
        if (a1 && b1)
            line(fmt("if (%s %s %s) discard;", *a1, cmp, *b1));
        else
            line(fmt("if (any(%s %s %s)) discard;", a, cmp, b));
        expr = py::str("0.0");
    } else if (vop == 28) {
        py::str a = opl(1, all4);
        py::str b = opl(2, all4);
        expr = fmt("float4(1.0, %s.y * %s.y, %s.z, %s.w)", a, b, a, b);
        if (n != 4) expr = fmt("(%s).%s", expr, comps);
    } else if (vop == 29) {
        uses_a0 = true;
        const py::list<int64_t> w3 = {3};
        line(fmt("a0 = (int)floor(%s + 0.5);", opl(1, w3)));
        py::str a = op(1);
        py::str b = op(2);
        expr = a == b ? a : fmt("max(%s, %s)", a, b);
    } else {
        fail(fmt("vector opcode %d", vop));
    }
    if (!dest || lanes.empty()) return;
    static const py::set<int64_t> WIDENED = {15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27};
    if (WIDENED.contains(vop) && n > 1)
        expr = !expr.startswith(py::str("dot(")) ? fmt("(%s).%s", expr, xs_(n)) : fmt("%s.%s", expr, xs_(n));
    if (saturate) expr = fmt("saturate(%s)", expr);
    line(fmt("%s.%s = %s;", *dest, comps, expr));
}

std::tuple<py::str, py::list<py::str>> Translator_::scalar_expression(const Word3& w, int64_t sop) {
    int64_t w1 = w[1];
    int64_t sw = _bits(w1, 0, 8);
    int64_t a_lane = ((sw >> 6) + 3) & 3, b_lane = sw & 3;
    py::list<py::str> side;
    std::optional<py::str> a_opt, b_opt;
    if (SCALAR_TWO.contains(sop)) {
        a_opt = scalar_operand(w, a_lane);
        b_opt = scalar_operand(w, b_lane);
    } else if (sop >= 42 && sop <= 47) {
        // mulsc/addsc/subsc: a constant lane and a temp register lane
        int64_t w0 = w[0], w2 = w[2];
        py::list<Lane> c_lanes =
            constant_lanes(_bits(w2, 0, 8), constant_relative(w, 3), _bits(w1, 29, 1) != 0);
        int64_t t_reg = (sop & 1) | (_bits(w2, 29, 1) << 1) | (_bits(w1, 0, 8) & 0x3C);
        bool neg = _bits(w1, 24, 1) != 0, ab = _bits(w0, 7, 1) != 0;
        Lane ca = c_lanes[a_lane];
        if (ca.base == py::str("#"))
            a_opt = _num((neg ? -1 : 1) * (ab ? std::fabs(ca.value) : ca.value));
        else
            a_opt = modify(_lane_text(ca), ab, neg);
        py::str tr = temp(t_reg);
        b_opt = modify(fmt("%s.%s", tr, CH[b_lane]), ab, neg);
    } else if (sop == 33 || sop == 50) {
        // (no operands)
    } else {
        a_opt = scalar_operand(w, a_lane);
    }
    // (a is None only for sop 33 and 50, which do not read it; b is None unless the op takes two)
    py::str a = a_opt ? *a_opt : EMPTY, b = b_opt ? *b_opt : EMPTY;
    py::str prev = py::str("ps");
    if (sop == 1 || sop == 3 || sop == 4 || sop == 26 || (sop == 50 && _bits(w[0], 20, 4))) uses_ps_anywhere = true;
    if (sop == 0 || sop == 44 || sop == 45) return {_plus(a, b), side};
    if (sop == 1) return {fmt("%s + %s", a, prev), side};
    if (sop == 2 || sop == 42 || sop == 43) return {fmt("%s * %s", a, b), side};
    if (sop == 3) return {fmt("%s * %s", a, prev), side};
    if (sop == 4)
        return {fmt("(%s == -1e38 || %s == -1e38 || isinf(%s) || isnan(%s)) ? -1e38 : %s * %s", prev, b, prev, prev, a,
                    prev),
                side};
    if (sop == 5) return {a == b ? a : fmt("max(%s, %s)", a, b), side};
    if (sop == 6) return {a == b ? a : fmt("min(%s, %s)", a, b), side};
    if (sop >= 7 && sop <= 10)
        return {fmt("(%s %s 0.0) ? 1.0 : 0.0", a, py::str(sop == 7 ? "==" : sop == 8 ? ">" : sop == 9 ? ">=" : "!=")),
                side};
    if (sop == 11) return {fmt("frac(%s)", a), side};
    if (sop == 12) return {fmt("trunc(%s)", a), side};
    if (sop == 13) return {fmt("floor(%s)", a), side};
    if (sop == 14) return {fmt("exp2(%s)", a), side};
    if (sop == 15) return {fmt("max(log2(%s), -1e38)", a), side};
    if (sop == 16) return {fmt("log2(%s)", a), side};
    if (sop >= 17 && sop <= 19) return {fmt("1.0 / %s", a), side};
    if (sop >= 20 && sop <= 22) return {fmt("rsqrt(%s)", a), side};
    if (sop == 23 || sop == 24) {
        uses_a0 = true;
        side.append(fmt("a0 = (int)%s(%s);", py::str(sop == 23 ? "round" : "floor"), a));
        // (`a if sop == 24 or b is None else max(a, b)`: b is None here)
        return {a, side};
    }
    if (sop == 25) return {_minus(a, b), side};
    if (sop == 26) return {fmt("%s - %s", a, prev), side};
    if (sop >= 27 && sop <= 30) {
        uses_p0 = true;
        py::str cond = fmt("%s %s 0.0", a, py::str(sop == 27 ? "==" : sop == 28 ? "!=" : sop == 29 ? ">" : ">="));
        side.append(fmt("p0 = %s;", cond));
        return {fmt("(%s) ? 0.0 : 1.0", cond), side};
    }
    if (sop == 31) {
        uses_p0 = true;
        side.append(fmt("p0 = %s == 1.0;", a));
        return {fmt("(%s == 1.0) ? 0.0 : (%s == 0.0 ? 1.0 : %s)", a, a, a), side};
    }
    if (sop == 32) {
        uses_p0 = true;
        side.append(fmt("p0 = %s - 1.0 <= 0.0;", a));
        return {fmt("(%s - 1.0 <= 0.0) ? 0.0 : %s - 1.0", a, a), side};
    }
    if (sop == 33) {
        uses_p0 = true;
        side.append(py::str("p0 = false;"));
        return {py::str("1e38"), side};
    }
    if (sop == 34) {
        uses_p0 = true;
        side.append(fmt("p0 = %s == 0.0;", a));
        return {a, side};
    }
    if (sop >= 35 && sop <= 39) {
        const char* f = sop == 35 ? "%s == 0.0" : sop == 36 ? "%s > 0.0" : sop == 37 ? "%s >= 0.0"
                        : sop == 38 ? "%s != 0.0" : "%s == 1.0";
        py::str cond = fmt(f, a);
        side.append(fmt("if (%s) discard;", cond));
        return {py::str("0.0"), side};
    }
    if (sop == 40) return {fmt("sqrt(%s)", a), side};
    if (sop == 46 || sop == 47) return {_minus(a, b), side};
    if (sop == 48) return {fmt("sin(%s)", a), side};
    if (sop == 49) return {fmt("cos(%s)", a), side};
    if (sop == 50) return {prev, side};
    fail(fmt("scalar opcode %d", sop));
}

// ---- fetches

void Translator_::fetch_destination(int64_t w0, int64_t w1, const py::list<Lane>& value_lanes) {
    py::str dest = temp(_bits(w0, 12, 6), _bits(w0, 18, 1) != 0);
    int64_t sw = _bits(w1, 0, 12);
    py::str comps = EMPTY;
    py::list<Lane> parts;
    for (int64_t c = 0; c < 4; ++c) {
        int64_t sel = (sw >> (3 * c)) & 7;
        if (sel == 7) continue;
        if (sel == 6) fail(py::str("fetch destination swizzle 6"));
        comps += CH[c];
        parts.append(sel < 4 ? value_lanes[sel] : number_lane(sel == 4 ? 0.0 : 1.0));
    }
    if (!comps.empty()) line(fmt("%s.%s = %s;", dest, comps, _lanes_text(parts)));
}

void Translator_::emit_fetch(const py::list<int64_t>& wl) {
    Word3 w = unpack3_(wl);
    int64_t w0 = w[0], w1 = w[1], w2 = w[2];
    int64_t opcode = _bits(w0, 0, 5);
    bool predicated = _bits(w1, 31, 1) != 0;
    if (predicated) {
        uses_p0 = true;
        line(fmt("if (%sp0)", _bits(w2, 31, 1) ? EMPTY : py::str("!")));
        line(py::str("{"));
        indent += 1;
    }
    if (opcode == 0) {
        emit_vertex_fetch(w);
    } else if (opcode == 1) {
        emit_texture_fetch(w);
    } else if (opcode == 24) {   // setTexLOD: the level of detail later fetches may use
        uses_lod = true;
        py::str t = temp(_bits(w0, 5, 6), _bits(w0, 11, 1) != 0);
        line(fmt("lod = %s.%s;", t, CH[_bits(w0, 26, 6) & 3]));
    } else {
        fail(fmt("fetch instruction %d", opcode));
    }
    if (predicated) {
        indent -= 1;
        line(py::str("}"));
    }
}

void Translator_::emit_vertex_fetch(const Word3& w) {
    int64_t w0 = w[0], w1 = w[1];
    std::optional<shader::VertexInput> entry;
    if (const auto* vin = std::get_if<py::list<shader::VertexInput>>(&s.inputs)) {
        for (const shader::VertexInput& e : *vin)
            if (std::get<0>(e) == slot) {
                entry = e;
                break;
            }
    } else {
        // a pixel shader's inputs are 4-tuples: `_, usage, index = entry` fails to unpack
        for (const shader::Interpolator& e : std::get<py::list<shader::Interpolator>>(s.inputs))
            if (std::get<0>(e) == slot) throw py::ValueError("too many values to unpack (expected 3, got 4)");
    }
    if (!entry) fail(py::str("a vertex fetch the input table does not name"));
    py::str usage = std::get<1>(*entry);
    int64_t index = std::get<2>(*entry);
    py::str field = fmt("%s%d",
                        usage_field_(usage).replace(py::str("Blendweight"), py::str("BlendWeight"))
                            .replace(py::str("Blendindices"), py::str("BlendIndices")),
                        index);
    inputs.set(field, fmt("%s%d", usage, index));
    py::list<Lane> value_lanes;
    for (int64_t c = 0; c < 4; ++c) value_lanes.append(Lane{py::str("input.") + field, CH[c], 0.0});
    fetch_destination(w0, w1, value_lanes);
}

shader::Constant Translator_::sampler_for(int64_t tf) {
    auto [c, unused] = s.constant_at(py::str("sampler"), tf);
    (void)unused;
    if (!c) fail(fmt("texture fetch constant %d is bound to no sampler", tf));
    if (!samplers.contains(c->name)) samplers.append(c->name);
    return *c;
}

void Translator_::emit_texture_fetch(const Word3& w) {
    int64_t w0 = w[0], w1 = w[1], w2 = w[2];
    int64_t tf = _bits(w0, 20, 5);
    int64_t dim = _bits(w2, 14, 2);
    py::str src = temp(_bits(w0, 5, 6), _bits(w0, 11, 1) != 0);
    int64_t sw = _bits(w0, 26, 6);
    py::str sampler = sampler_for(tf).name;
    if (_bits(w0, 25, 1)) fail(py::str("unnormalized texture coordinates"));
    if (_bits(w2, 16, 10)) fail(py::str("texture fetch offsets"));
    double bias = static_cast<double>(_sext(_bits(w2, 2, 7), 7)) / 16.0;
    bool computed_lod = _bits(w1, 28, 1) != 0 && !vertex;
    bool register_lod = _bits(w1, 29, 1) != 0;
    py::str coord, kind;
    if (dim == 1) {
        coord = fmt("%s.%s", src, CH[sw & 3] + CH[(sw >> 2) & 3]);
        kind = py::str("2D");
    } else if (dim == 0) {
        coord = fmt("%s.%s", src, CH[sw & 3]);
        kind = py::str("1D");
    } else if (dim == 2) {
        coord = fmt("%s.%s", src, CH[sw & 3] + CH[(sw >> 2) & 3] + CH[(sw >> 4) & 3]);
        kind = py::str("3D");
    } else {
        // cube: the face coordinates the cube instruction made (+1.5) and the face, back to a direction
        cube_fetch_needed = true;
        coord = fmt("xe_cube_direction(%s.%s)", src, CH[sw & 3] + CH[(sw >> 2) & 3] + CH[(sw >> 4) & 3]);
        kind = py::str("CUBE");
    }
    py::str level = register_lod ? (bias != 0.0 ? fmt("lod + %s", _num(bias)) : py::str("lod")) : _num(bias);
    py::str call;
    if (computed_lod && !register_lod) {
        call = bias == 0.0 ? fmt("tex%s(%s, %s)", kind, sampler, coord)
                           : fmt("tex%sbias(%s, float4(%s, %s))", kind, sampler, pad(coord, kind), _num(bias));
    } else if (computed_lod) {
        call = fmt("tex%sbias(%s, float4(%s, %s))", kind, sampler, pad(coord, kind), level);
    } else {
        call = fmt("tex%slod(%s, float4(%s, %s))", kind, sampler, pad(coord, kind), level);
    }
    line(fmt("t = %s;", call));
    t_declared_ = true;
    py::list<Lane> value_lanes;
    for (int64_t c = 0; c < 4; ++c) value_lanes.append(Lane{py::str("t"), CH[c], 0.0});
    fetch_destination(w0, w1, value_lanes);
}

py::str Translator_::pad(const py::str& coord, const py::str& kind) {
    int64_t n = kind == py::str("1D") ? 1 : kind == py::str("2D") ? 2 : 3;   // 1D 2D 3D CUBE
    py::str out = coord;
    for (int64_t i = 0; i < 3 - n; ++i) out += py::str(", 0.0");
    return out;
}

// ---- control flow

Function Translator_::run() {
    // The control flow, then the body: the CF instructions fill the first slots, up to the first exec's
    // address; the program ends at an unconditional exece.
    py::list<int64_t> prog;
    std::optional<int64_t> first_exec;
    int64_t index = 0;
    while (py::floordiv(index, 2) * 3 + 2 < py::len(words)) {
        if (first_exec && index >= *first_exec * 2) break;
        int64_t cf = control_flow(words, index);
        int64_t op = _bits(cf, 44, 4);
        if (EXEC_OPS.contains(op)) {
            int64_t address = _bits(cf, 0, 12);
            first_exec = first_exec ? std::min(*first_exec, address) : address;
        }
        prog.append(cf);
        index += 1;
        if (op == 2) break;
    }
    // loops: a loop start and the loop end jumping back to the instruction after it, a `for` over the loop
    // constant's count; jumps: forward ones become `skip_to` (the blocks a taken jump passes over do not run),
    // out of a loop a break as well; others stop
    jumps = false;
    loop_end = py::dict<int64_t, int64_t>();
    for (int64_t i = 0; i < py::len(prog); ++i) {
        int64_t cf = prog[i];
        int64_t op = _bits(cf, 44, 4);
        if (op == 7) {
            std::optional<int64_t> j;
            for (int64_t jj = i + 1; jj < py::len(prog); ++jj)
                if (_bits(prog[jj], 44, 4) == 8 && _bits(prog[jj], 0, 13) == i + 1) {
                    j = jj;
                    break;
                }
            if (!j) {
                slot = i;
                fail(py::str("a loop without its end"));
            }
            loop_end.set(i, *j);
        } else if (op == 11) {
            if (_bits(cf, 0, 13) <= i) {
                slot = i;
                fail(py::str("a backward jump in the control flow"));
            }
            jumps = true;
        } else if (op == 9 || op == 10) {
            slot = i;
            fail(fmt("control flow instruction %d (calls are not translated yet)", op));
        } else if ((op == 4 || op == 6 || op == 14) && i + 1 < py::len(prog)) {
            slot = i;
            fail(py::str("a conditional end of the program before its last instruction"));
        }
    }
    program = prog;
    // one pass first to learn whether the previous scalar result is read anywhere
    uses_ps_anywhere = false;
    for (int pass_ = 0; pass_ < 2; ++pass_) {
        bool keep = uses_ps_anywhere;
        init_body_();
        uses_ps_anywhere = keep;
        emit_range(0, py::len(program), {});
    }
    return assemble();
}

void Translator_::emit_range(int64_t lo, int64_t hi, const std::vector<std::pair<int64_t, int64_t>>& loops) {
    int64_t i = lo;
    while (i < hi) {
        int64_t cf = program[i];
        int64_t op = _bits(cf, 44, 4);
        if (EXEC_OPS.contains(op)) {
            if (jumps) {
                line(fmt("if (skip_to <= %d)", i));
                line(py::str("{"));
                indent += 1;
            }
            emit_exec(cf);
            if (jumps) {
                indent -= 1;
                line(py::str("}"));
            }
        } else if (op == 11) {
            int64_t target = _bits(cf, 0, 13);
            py::str cond;
            if (_bits(cf, 13, 1)) {
                cond = fmt("skip_to <= %d", i);
            } else if (_bits(cf, 14, 1)) {
                uses_p0 = true;
                cond = fmt("skip_to <= %d && %sp0", i, _bits(cf, 42, 1) ? EMPTY : py::str("!"));
            } else {
                py::str bn = bool_name(_bits(cf, 34, 8));
                cond = fmt("skip_to <= %d && %s%s", i, _bits(cf, 42, 1) ? EMPTY : py::str("!"), bn);
            }
            bool leaves = !loops.empty() && target > loops.back().second;
            line(fmt("if (%s) { skip_to = %d;%s }", cond, target, leaves ? py::str(" break;") : EMPTY));
        } else if (op == 7) {
            int64_t end = loop_end.at(i);
            emit_loop(i, end, loops);
            i = end + 1;
            continue;
        }
        i += 1;
    }
}

void Translator_::emit_loop(int64_t start, int64_t end, const std::vector<std::pair<int64_t, int64_t>>& loops) {
    int64_t cf = program[start], cf_end = program[end];
    auto [count, first, step] = loop_constant(_bits(cf, 16, 5));
    int64_t depth = static_cast<int64_t>(loops.size());
    py::str it = fmt("i%d", depth);
    uses_aL = true;
    if (jumps) {
        line(fmt("if (skip_to <= %d)", start));
        line(py::str("{"));
        indent += 1;
    }
    line(fmt("[loop] for (int %s = 0; %s < %s; %s++)", it, it, count, it));
    line(py::str("{"));
    indent += 1;
    if (!_bits(cf, 13, 1)) {   // the loop sets aL (or keeps the outer one's)
        py::str value = step == py::str("0")
                            ? first
                            : (first != py::str("0") ? fmt("%s + %s * %s", first, it, step)
                                                     : (step != py::str("1") ? fmt("%s * %s", it, step) : it));
        line(fmt("aL = %s;", value));
    }
    if (jumps) line(fmt("skip_to = %d;", start + 1));
    std::vector<std::pair<int64_t, int64_t>> inner = loops;
    inner.emplace_back(start, end);
    emit_range(start + 1, end, inner);
    if (_bits(cf_end, 21, 1)) {   // the loop end's predicated break
        uses_p0 = true;
        line(fmt("if (%sp0) break;", _bits(cf_end, 42, 1) ? EMPTY : py::str("!")));
    }
    indent -= 1;
    line(py::str("}"));
    if (!loops.empty() && !_bits(program[loops.back().first], 13, 1)) {
        auto outer = loop_constant(_bits(program[loops.back().first], 16, 5));
        line(fmt("aL = %s + i%d * %s;", std::get<1>(outer), depth - 1, std::get<2>(outer)));
    }
    if (jumps) {
        indent -= 1;
        line(py::str("}"));
    }
}

std::tuple<py::str, py::str, py::str> Translator_::loop_constant(int64_t k) {
    if (s.loop_literals.contains(k)) {   // one the compiler made: its count, start and step are numbers
        int64_t v = s.loop_literals.at(k);
        int64_t step = (v >> 16) & 0xFF;
        return {fmt("%d", v & 0xFF), fmt("%d", (v >> 8) & 0xFF), fmt("%d", (step & 0x80) ? step - 256 : step)};
    }
    int64_t reg = k - (!vertex ? 16 : 0);
    auto [c, offset] = s.constant_at(py::str("int4"), reg);
    if (!c) fail(fmt("loop constant i%d is in no constant of the table", k));
    py::str nm = c->elements <= 1 ? c->name : fmt("%s[%d]", c->name, offset);
    if (c->cls == py::str("scalar")) {
        py::list<double> source = (c->default_ && !c->default_->empty()) ? *c->default_ : py::list<double>{0, 0, 1, 0};
        py::list<int64_t> d;
        for (double v : source.slice(4 * offset, 4 * offset + 4))
            d.append((std::fabs(v) < 1e6 && v == static_cast<double>(static_cast<int64_t>(v))) ? static_cast<int64_t>(v) : 0);
        return {nm, py::len(d) > 1 ? fmt("%d", d[1]) : py::str("0"), py::len(d) > 2 ? fmt("%d", d[2]) : py::str("1")};
    }
    return {fmt("%s.x", nm), fmt("%s.y", nm), fmt("%s.z", nm)};
}

py::str Translator_::bool_name(int64_t b) {
    if (!vertex) b -= 128;   // a pixel shader's bool constants follow the vertex shader's
    auto [c, k] = s.constant_at(py::str("bool"), b);
    if (!c) fail(fmt("bool constant b%d is in no constant of the table", b));
    return c->elements <= 1 ? c->name : fmt("%s[%d]", c->name, k);
}

void Translator_::init_body_() {
    lines = py::list<py::str>();
    indent = 1;
    samplers = py::list<py::str>();
    used_regs = py::set<int64_t>();
    inputs = py::dict<py::str, py::str>();
    outputs = py::dict<py::str, py::str>();
    uses_a0 = uses_p0 = uses_aL = uses_lod = false;
    s_declared_ = t_declared_ = false;
    cube_needed = cube_fetch_needed = false;
}

void Translator_::emit_exec(int64_t cf) {
    int64_t op = _bits(cf, 44, 4);
    int64_t address = _bits(cf, 0, 12), count = _bits(cf, 12, 3), sequence = _bits(cf, 16, 12);
    int64_t condition = _bits(cf, 42, 1);
    bool guarded = false;
    if (op == 3 || op == 4 || op == 13 || op == 14) {   // on a bool constant
        py::str bn = bool_name(_bits(cf, 34, 8));
        line(fmt("if (%s%s)", condition ? EMPTY : py::str("!"), bn));
        guarded = true;
    } else if (op == 5 || op == 6) {   // on the predicate
        uses_p0 = true;
        line(fmt("if (%sp0)", condition ? EMPTY : py::str("!")));
        guarded = true;
    }
    if (guarded) {
        line(py::str("{"));
        indent += 1;
    }
    for (int64_t i = 0; i < count; ++i) {
        slot = address + i;
        py::list<int64_t> w = words.slice(slot * 3, slot * 3 + 3);
        if ((sequence >> (2 * i)) & 1)
            emit_fetch(w);
        else
            emit_alu(w);
    }
    if (guarded) {
        indent -= 1;
        line(py::str("}"));
    }
}

// ---- the function

Function Translator_::assemble() {
    py::str n = name;
    py::list<py::str> out;
    std::map<int64_t, std::pair<py::str, int64_t>> pixel_inputs;
    if (!vertex) {
        auto add = [&](int64_t reg, const py::str& usage, int64_t index, int64_t mask) {
            py::str field = fmt("%s%d", usage_field_(usage), index);
            pixel_inputs[reg] = {field, mask};
            inputs.set(field, fmt("%s%d", usage, index));
        };
        if (const auto* pin = std::get_if<py::list<shader::Interpolator>>(&s.inputs)) {
            for (const shader::Interpolator& e : *pin) add(std::get<0>(e), std::get<1>(e), std::get<2>(e), std::get<3>(e));
        } else if (!std::get<py::list<shader::VertexInput>>(s.inputs).empty()) {
            // `for reg, usage, index, mask in self.s.inputs` over 3-tuples
            throw py::ValueError("not enough values to unpack (expected 4, got 3)");
        }
    }
    out.append(fmt("struct %s_Input\n{", n));
    for (const auto& kv : inputs.items()) out.append(fmt("    float4 %s : %s;", std::get<0>(kv), std::get<1>(kv)));
    if (inputs.empty())
        out.append(py::str(!vertex ? "    float4 Unused : TEXCOORD15;" : "    float4 Position0 : POSITION0;"));
    out.append(py::str("};\n"));
    out.append(fmt("struct %s_Output\n{", n));
    if (vertex && !outputs.contains(py::str("Position"))) {
        py::dict<py::str, py::str> o2;
        o2.set(py::str("Position"), py::str("POSITION0"));
        for (const auto& kv : outputs.items()) o2.set(std::get<0>(kv), std::get<1>(kv));
        outputs = o2;
    }
    for (const auto& kv : outputs.items()) {
        const py::str& field = std::get<0>(kv);
        out.append(fmt("    float%s %s : %s;",
                       (field == py::str("Depth") || field == py::str("PointSize")) ? EMPTY : py::str("4"), field,
                       std::get<1>(kv)));
    }
    out.append(py::str("};\n"));
    out.append(fmt("%s_Output %s(%s_Input input)\n{", n, n, n));
    out.append(fmt("    %s_Output output = (%s_Output)0;", n, n));
    py::set<int64_t> reg_set = used_regs;
    py::set<int64_t> regs_all = reg_set;
    for (const auto& kv : pixel_inputs) regs_all.add(kv.first);
    py::list<int64_t> regs = py::sorted(regs_all);
    if (!regs.empty()) {
        py::list<py::str> parts;
        for (int64_t r : regs) parts.append(fmt("r%d = 0.0", r));
        out.append(fmt("    float4 %s;", py::join(py::str(", "), parts)));
    }
    if (uses_ps_anywhere) out.append(py::str("    float ps = 0.0;"));
    if (uses_p0) out.append(py::str("    bool p0 = false;"));
    if (uses_a0) out.append(py::str("    int a0 = 0;"));
    if (uses_aL) out.append(py::str("    int aL = 0;"));
    if (uses_lod) out.append(py::str("    float lod = 0.0;"));
    if (t_declared_) out.append(py::str("    float4 t;"));
    if (s_declared_) out.append(py::str("    float s;"));
    if (jumps) out.append(py::str("    int skip_to = 0;"));
    for (const auto& kv : pixel_inputs) {
        int64_t reg = kv.first;
        const py::str& field = kv.second.first;
        int64_t mask = kv.second.second;
        py::str comps = EMPTY;
        for (int64_t c = 0; c < 4; ++c)
            if ((mask >> c) & 1) comps += CH[c];
        if (comps.empty()) comps = py::str("xyzw");
        out.append(fmt("    r%d.%s = input.%s.%s;", reg, comps, field, comps));
    }
    out.extend(lines);
    out.append(py::str("    return output;\n}\n"));
    Function f;
    f.text = py::join(py::str("\n"), out);
    f.samplers = samplers;
    f.inputs = inputs.copy();
    f.outputs = outputs.copy();
    return f;
}

const py::str HELPERS = R"HLSL(// The Xbox 360 GPU's cube instruction: the face coordinates (+1.5), the major axis times two, the face index.
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
)HLSL";

Function translate(const shader::Shader& shader, const py::str& name) {
    Translator_ t(shader, name);
    Function f = t.run();
    f.needs_helpers = t.cube_needed || t.cube_fetch_needed;
    return f;
}

}  // namespace codebreaker::decompile::xenos_hlsl
