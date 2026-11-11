#include "cb/decompile/xenos_hlsl.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <cmath>
#include <format>
#include <limits>
#include <map>
#include <set>
#include <span>

#include "cb/core/text.hpp"

namespace cb::decompile::xenos_hlsl {

std::string_view stage_name(Stage stage) { return stage == Stage::Vertex ? "vertex" : "pixel"; }

Shader::Found Shader::constant_at(RegisterSet set, std::int64_t reg) const {
    for (const Constant& c : constants) {
        if (c.set == set && static_cast<std::int64_t>(c.first_register) <= reg &&
            reg < static_cast<std::int64_t>(c.first_register) + c.count) {
            return {&c, static_cast<std::uint32_t>(reg - c.first_register)};
        }
    }
    return {};
}

namespace {

constexpr std::string_view kComponents = "xyzw";

std::uint32_t bits(std::uint64_t v, unsigned low, unsigned n) { return static_cast<std::uint32_t>((v >> low) & ((1ull << n) - 1)); }

// Sign-extend the low n bits.
std::int32_t sign_extend(std::uint32_t v, unsigned n) {
    return (v & (1u << (n - 1))) ? static_cast<std::int32_t>(v) - (1 << n) : static_cast<std::int32_t>(v);
}

bool all_of_set(std::string_view s, std::string_view allowed) {
    return !s.empty() && s.find_first_not_of(allowed) == std::string_view::npos;
}

// a + b, with `a + -x` written `a - x`.
std::string plus(const std::string& a, const std::string& b) {
    if (b.starts_with('-') && !b.starts_with("-(")) return std::format("{} - {}", a, b.substr(1));
    return std::format("{} + {}", a, b);
}

std::string minus(const std::string& a, const std::string& b) {
    if (b.starts_with('-') && !b.starts_with("-(")) return std::format("{} + {}", a, b.substr(1));
    const bool needs_parentheses = (b.find(' ') != std::string::npos) && !b.starts_with('(');
    return std::format("{} - {}", a, needs_parentheses ? "(" + b + ")" : b);
}

// `r1.xxxx` -> `r1.x`, `(0.5).xxxx` -> `0.5`: one value repeated; else nullopt.
std::optional<std::string> uniform(const std::string& v) {
    if (v.size() < 6 || v[v.size() - 5] != '.') return std::nullopt;
    const char c = v.back();
    if (kComponents.find(c) == std::string_view::npos) return std::nullopt;
    if (v[v.size() - 2] != c || v[v.size() - 3] != c || v[v.size() - 4] != c) return std::nullopt;
    const std::string base = v.substr(0, v.size() - 5);
    if (base.size() >= 3 && base.front() == '(' && base.back() == ')' && all_of_set(std::string_view(base).substr(1, base.size() - 2), "0123456789.e+-")) {
        return base.substr(1, base.size() - 2);
    }
    return base + "." + c;
}

// The 48-bit control-flow instruction `index`: two of them are packed into three dwords.
std::uint64_t control_flow(std::span<const std::uint32_t> words, std::size_t index) {
    const std::size_t pair = index / 2;
    const std::uint64_t d0 = words[pair * 3], d1 = words[pair * 3 + 1], d2 = words[pair * 3 + 2];
    if (index % 2 == 0) return d0 | ((d1 & 0xFFFF) << 32);
    return (d1 >> 16) | ((d2 & 0xFFFF) << 16) | ((d2 >> 16) << 32);
}

constexpr bool is_exec_op(unsigned op) { return (op >= 1 && op <= 6) || op == 13 || op == 14; }
constexpr bool is_scalar_two(unsigned sop) { return sop == 0 || sop == 2 || sop == 4 || sop == 5 || sop == 6 || sop == 25; }
constexpr bool is_vector_side_effect(unsigned vop) { return (vop >= 20 && vop <= 27) || vop == 29; }

// One lane of an operand: a number, or a component of a named value (a vector's 'x', a matrix's '_m03', or the whole
// scalar when `comp` is empty).
struct Lane {
    bool number = false;
    float value = 0;
    std::string base;
    std::string comp;
};

Lane number_lane(float v) {
    Lane l;
    l.number = true;
    l.value = v;
    return l;
}
Lane named_lane(std::string base, std::string comp) {
    Lane l;
    l.base = std::move(base);
    l.comp = std::move(comp);
    return l;
}

std::string lane_text(const Lane& t) {
    if (t.number) return number_text(t.value);
    if (t.comp.empty()) return t.base;
    return t.base + "." + t.comp;
}

std::string join_lanes(const std::vector<Lane>& lanes) {
    std::string out;
    for (std::size_t i = 0; i < lanes.size(); ++i) {
        if (i) out += ", ";
        out += lane_text(lanes[i]);
    }
    return out;
}

// Lane tokens -> one HLSL expression.
std::string lanes_text(const std::vector<Lane>& lanes) {
    const std::size_t n = lanes.size();
    const bool one_base = n > 0 && std::ranges::all_of(lanes, [&](const Lane& l) { return !l.number && l.base == lanes[0].base; });
    if (one_base) {
        const std::string& base = lanes[0].base;
        const bool none_comp = std::ranges::all_of(lanes, [](const Lane& l) { return l.comp.empty(); });
        const bool all_comp = std::ranges::none_of(lanes, [](const Lane& l) { return l.comp.empty(); });
        std::string comps;
        for (const Lane& l : lanes) comps += l.comp;
        if (none_comp) return n == 1 ? base : std::format("((float{}){})", n, base);
        if (all_comp && std::ranges::all_of(lanes, [](const Lane& l) { return l.comp.starts_with("_m"); })) return base + "." + comps;
        if (all_comp && std::ranges::all_of(lanes, [](const Lane& l) { return l.comp.size() == 1 && kComponents.find(l.comp[0]) != std::string_view::npos; })) {
            return base + "." + comps;
        }
    }
    if (std::ranges::all_of(lanes, [](const Lane& l) { return l.number; })) {
        if (n == 1) return number_text(lanes[0].value);
        if (n > 0 && std::ranges::all_of(lanes, [&](const Lane& l) { return l.value == lanes[0].value; })) {
            return std::format("({}).{}", number_text(lanes[0].value), std::string(n, 'x'));
        }
        return std::format("float{}({})", n, join_lanes(lanes));
    }
    return n == 1 ? lane_text(lanes[0]) : std::format("float{}({})", n, join_lanes(lanes));
}

// The 3 dwords of an ALU or fetch instruction.
using Word3 = std::array<std::uint32_t, 3>;

std::string capitalize(std::string s) {
    for (std::size_t i = 0; i < s.size(); ++i) s[i] = static_cast<char>(i == 0 ? std::toupper(static_cast<unsigned char>(s[i])) : std::tolower(static_cast<unsigned char>(s[i])));
    return s;
}

// A vertex input or output field's name: the usage, capitalised, with the HLSL spelling of the compound words.
std::string field_name(std::string_view usage, std::uint32_t index, bool vertex_input) {
    std::string name = text::replace_all(capitalize(std::string(usage)), "Texcoord", "TexCoord");
    if (vertex_input) {
        name = text::replace_all(name, "Blendweight", "BlendWeight");
        name = text::replace_all(name, "Blendindices", "BlendIndices");
    }
    return std::format("{}{}", name, index);
}

std::string widen(const std::string& value, std::size_t n) {
    if (n == 1) return value;
    const bool simple = all_of_set(value, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.[]");
    const bool numeric = all_of_set(value, "0123456789.e+-");
    return simple && !numeric ? std::format("{}.{}", value, std::string(n, 'x')) : std::format("((float{})({}))", n, value);
}

// Does the text read register r<reg> (as a whole word)?
bool reads_register(std::string_view s, std::uint32_t reg) {
    const std::string name = std::format("r{}", reg);
    const auto is_word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };
    for (std::size_t at = s.find(name); at != std::string_view::npos; at = s.find(name, at + 1)) {
        const bool before = at == 0 || !is_word(s[at - 1]);
        const bool after = at + name.size() == s.size() || !is_word(s[at + name.size()]);
        if (before && after) return true;
    }
    return false;
}

std::string modify(std::string s, bool absolute, bool negate) {
    if (absolute) s = std::format("abs({})", s);
    if (negate) s = (std::isalnum(static_cast<unsigned char>(s[0])) || s[0] == '(' || s[0] == '_') ? "-" + s : std::format("-({})", s);
    return s;
}

std::string pad(const std::string& coord, std::string_view kind) {
    const int n = kind == "1D" ? 1 : kind == "2D" ? 2 : 3;
    std::string out = coord;
    for (int i = n; i < 3; ++i) out += ", 0.0";
    return out;
}

using LoopRange = std::pair<std::size_t, std::size_t>;   // a loop's start and end control-flow slot

class Translator {
public:
    Translator(const Shader& shader, std::string_view name) : s_(shader), name_(name), vertex_(shader.stage == Stage::Vertex) {}

    Function run();
    bool needs_helpers() const { return cube_needed_ || cube_fetch_needed_; }

private:
    // ---- output
    void line(const std::string& text) { lines_.push_back(std::string(static_cast<std::size_t>(indent_) * 4, ' ') + text); }
    [[noreturn]] void fail(const std::string& what) const { throw Unsupported(s_.stage, name_, slot_, what); }

    // ---- operands
    std::string temp(std::uint32_t reg, bool relative = false) {
        if (relative) fail("a temporary register addressed through the loop counter");
        used_regs_.insert(reg);
        return std::format("r{}", reg);
    }
    std::array<Lane, 4> constant_lanes(std::uint32_t index, bool relative, bool to_a0);
    struct AluOperand {
        std::array<Lane, 4> lanes;
        bool absolute;
        bool negate;
    };
    AluOperand alu_lanes(const Word3& w, int i);
    static bool constant_relative(const Word3& w, int i);
    std::string vector_operand(const Word3& w, int i, std::span<const int> lanes_wanted);
    std::string scalar_operand(const Word3& w, unsigned lane);
    std::array<Lane, 4> raw_lanes(const Word3& w, int i);

    // ---- ALU
    std::string export_target(std::uint32_t index);
    void emit_alu(const Word3& w);
    void emit_vector(const Word3& w, unsigned vop, const std::vector<int>& lanes, const std::optional<std::string>& dest, bool saturate);
    std::pair<std::string, std::vector<std::string>> scalar_expression(const Word3& w, unsigned sop);

    // ---- fetches
    void fetch_destination(std::uint32_t w0, std::uint32_t w1, const std::array<Lane, 4>& value_lanes);
    void emit_fetch(const Word3& w);
    void emit_vertex_fetch(const Word3& w);
    const Constant& sampler_for(std::uint32_t tf);
    void emit_texture_fetch(const Word3& w);

    // ---- control flow
    void emit_range(std::size_t lo, std::size_t hi, const std::vector<LoopRange>& loops);
    void emit_loop(std::size_t start, std::size_t end, const std::vector<LoopRange>& loops);
    struct LoopConstant {
        std::string count, first, step;
    };
    LoopConstant loop_constant(std::uint32_t k);
    std::string bool_name(std::int64_t b);
    void reset_body();
    void emit_exec(std::uint64_t cf);

    Function assemble();

    const Shader& s_;
    std::string name_;
    bool vertex_;
    std::vector<std::string> lines_;
    int indent_ = 1;
    std::vector<std::string> samplers_;
    std::set<std::uint32_t> used_regs_;
    bool uses_p0_ = false, uses_a0_ = false, uses_al_ = false, uses_lod_ = false;
    bool uses_ps_anywhere_ = false;
    bool s_declared_ = false, t_declared_ = false;
    bool cube_needed_ = false, cube_fetch_needed_ = false;
    OrderedMap<std::string, std::string> inputs_, outputs_;   // field name -> semantic
    std::uint32_t slot_ = 0;
    bool jumps_ = false;
    std::map<std::size_t, std::size_t> loop_end_;
    std::vector<std::uint64_t> program_;
};

// The four lanes of float constant c[index] (+ a0 when relative).
std::array<Lane, 4> Translator::constant_lanes(std::uint32_t index, bool relative, bool to_a0) {
    const char* index_register = to_a0 ? "a0" : "aL";
    if (!relative) {
        if (const auto it = s_.literals.find(index); it != s_.literals.end()) {
            return {number_lane(it->second[0]), number_lane(it->second[1]), number_lane(it->second[2]), number_lane(it->second[3])};
        }
    }
    const Shader::Found found = s_.constant_at(RegisterSet::Float4, index);
    if (!found.constant) {
        if (relative) fail(std::format("c[a0 + {}] outside every constant", index));
        fail(std::format("c{} is in no constant of the table", index));
    }
    const Constant& c = *found.constant;
    std::string name = c.name;
    std::uint32_t k = found.offset;
    const std::array<Lane, 4> zeros{number_lane(0), number_lane(0), number_lane(0), number_lane(0)};
    if (c.cls == ConstantClass::MatrixColumns || c.cls == ConstantClass::MatrixRows) {
        // a register of a matrix is a column (column-major, HLSL's default) or a row of it
        const bool columns = c.cls == ConstantClass::MatrixColumns;
        const std::uint32_t per = columns ? c.columns : c.rows;
        if (per == 0 && (relative || c.elements > 1)) fail(std::format("the constant {} has no {}", c.name, columns ? "columns" : "rows"));
        if (relative) {
            // through a0, which the program set to element * registers per element
            uses_a0_ = true;
            std::string element = std::format("{} / {}", index_register, per);
            if (k / per) element += std::format(" + {}", k / per);
            name = std::format("{}[{}]", name, element);
            k %= per;
        } else if (c.elements > 1) {
            name = std::format("{}[{}]", name, k / per);
            k %= per;
        }
        std::array<Lane, 4> lanes = zeros;
        for (std::uint32_t lane = 0; lane < 4; ++lane) {
            if (columns && lane < c.rows) lanes[lane] = named_lane(name, std::format("_m{}{}", lane, k));
            if (!columns && lane < c.columns) lanes[lane] = named_lane(name, std::format("_m{}{}", k, lane));
        }
        return lanes;
    }
    if (relative) {
        uses_a0_ = true;
        name = index != c.first_register ? std::format("{}[{} + {}]", name, index_register, index - c.first_register)
                                         : std::format("{}[{}]", name, index_register);
    } else if (c.elements > 1) {
        name = std::format("{}[{}]", name, k);
    }
    if (c.is_bool) {
        name = name.ends_with(']') ? std::format("(float)({})", name) : std::format("(float){}", name);
        return {named_lane(name, ""), number_lane(0), number_lane(0), number_lane(0)};
    }
    const std::uint32_t width = c.cls == ConstantClass::Vector ? c.columns : 1;
    if (width == 1) return {named_lane(name, ""), number_lane(0), number_lane(0), number_lane(0)};
    std::array<Lane, 4> lanes = zeros;
    for (std::uint32_t i = 0; i < 4 && i < width; ++i) lanes[i] = named_lane(name, std::string(1, kComponents[i]));
    return lanes;
}

// Operand i (1-3) of an ALU instruction: its four lanes after the swizzle, with abs / negate.
Translator::AluOperand Translator::alu_lanes(const Word3& w, int i) {
    const auto [w0, w1, w2] = w;
    const std::uint32_t reg = bits(w2, 16 - 8 * (i - 1), 8);
    const bool is_temp = bits(w2, 31 - (i - 1), 1) != 0;
    const std::uint32_t sw = bits(w1, 16 - 8 * (i - 1), 8);
    const bool negate = bits(w1, 26 - (i - 1), 1) != 0;
    std::array<Lane, 4> raw;
    bool absolute;
    if (is_temp) {
        const std::string base = temp(reg & 0x3F, (reg & 0x40) != 0);
        for (unsigned c = 0; c < 4; ++c) raw[c] = named_lane(base, std::string(1, kComponents[c]));
        absolute = (reg & 0x80) != 0;
    } else {
        raw = constant_lanes(reg, constant_relative(w, i), bits(w1, 29, 1) != 0);
        absolute = bits(w0, 7, 1) != 0;
    }
    std::array<Lane, 4> lanes;
    for (unsigned c = 0; c < 4; ++c) lanes[c] = raw[((sw >> (2 * c)) + c) & 3];
    return {lanes, absolute, negate};
}

bool Translator::constant_relative(const Word3& w, int i) {
    const std::uint32_t t1 = bits(w[2], 31, 1), t2 = bits(w[2], 30, 1);
    const std::uint32_t c0 = bits(w[1], 31, 1), c1 = bits(w[1], 30, 1);
    if (i == 1) return c0 != 0;
    if (i == 2) return (t1 ? c0 : c1) != 0;
    return ((t1 && t2) ? c0 : c1) != 0;
}

// Operand i's lanes `lanes_wanted` as one expression.
std::string Translator::vector_operand(const Word3& w, int i, std::span<const int> lanes_wanted) {
    AluOperand operand = alu_lanes(w, i);
    std::vector<Lane> picked;
    for (int c : lanes_wanted) picked.push_back(operand.lanes[static_cast<std::size_t>(c)]);
    const bool all_numbers = std::ranges::all_of(picked, [](const Lane& l) { return l.number; });
    if (operand.negate && all_numbers) {
        for (Lane& l : picked) l.value = -l.value;
        operand.negate = false;
    }
    if (operand.absolute && all_numbers) {
        for (Lane& l : picked) l.value = std::fabs(l.value);
        operand.absolute = false;
    }
    return modify(lanes_text(picked), operand.absolute, operand.negate);
}

// A scalar operand: lane `lane` (absolute, not through the swizzle) of operand 3's register (03c 2.2).
std::string Translator::scalar_operand(const Word3& w, unsigned lane) {
    const AluOperand operand = alu_lanes(w, 3);
    const Lane t = raw_lanes(w, 3)[lane];
    if (t.number) {
        const float v = operand.absolute ? std::fabs(t.value) : t.value;
        return number_text(operand.negate ? -v : v);
    }
    return modify(lane_text(t), operand.absolute, operand.negate);
}

std::array<Lane, 4> Translator::raw_lanes(const Word3& w, int i) {
    const std::uint32_t reg = bits(w[2], 16 - 8 * (i - 1), 8);
    if (bits(w[2], 31 - (i - 1), 1)) {
        const std::string base = temp(reg & 0x3F, (reg & 0x40) != 0);
        std::array<Lane, 4> out;
        for (unsigned c = 0; c < 4; ++c) out[c] = named_lane(base, std::string(1, kComponents[c]));
        return out;
    }
    return constant_lanes(reg, constant_relative(w, i), bits(w[1], 29, 1) != 0);
}

std::string Translator::export_target(std::uint32_t index) {
    if (vertex_) {
        if (index == 62) {
            outputs_.insert_or_assign("Position", "POSITION0");
            return "output.Position";
        }
        if (index == 63) {
            outputs_.insert_or_assign("PointSize", "PSIZE0");
            return "output.PointSize";
        }
        for (const Interpolator& o : s_.outputs) {
            if (o.reg == index) {
                const std::string field = field_name(o.usage, o.index, false);
                outputs_.insert_or_assign(field, std::format("{}{}", o.usage, o.index));
                return "output." + field;
            }
        }
        fail(std::format("export to o{}, which the output table does not name", index));
    }
    if (index < 4) {
        outputs_.insert_or_assign(std::format("Color{}", index), std::format("COLOR{}", index));
        return std::format("output.Color{}", index);
    }
    if (index == 61) {
        outputs_.insert_or_assign("Depth", "DEPTH0");
        return "output.Depth";
    }
    fail(std::format("export to register {}", index));
}

std::string components_of(std::span<const int> lanes) {
    std::string out;
    for (int c : lanes) out += kComponents[static_cast<std::size_t>(c)];
    return out;
}

void Translator::emit_alu(const Word3& w) {
    const auto [w0, w1, w2] = w;
    const unsigned vop = bits(w2, 24, 5), sop = bits(w0, 26, 6);
    const std::uint32_t vmask = bits(w0, 16, 4), smask = bits(w0, 20, 4);
    const bool exports = bits(w0, 15, 1) != 0;
    const std::uint32_t vdest = bits(w0, 0, 6), sdest = bits(w0, 8, 6);
    const bool vsat = bits(w0, 24, 1) != 0, ssat = bits(w0, 25, 1) != 0;
    if (bits(w0, 6, 1) || (bits(w0, 14, 1) && !exports)) fail("a destination addressed through the loop counter");
    const bool predicated = bits(w1, 28, 1) != 0;
    if (predicated) {
        uses_p0_ = true;
        line(std::format("if ({}p0)", bits(w1, 27, 1) ? "" : "!"));
        line("{");
        ++indent_;
    }

    // Every operand is read before anything is written; the vector result is written before the scalar one. The scalar
    // result goes through a temporary only when the vector op writes a register it reads.
    const auto [stext, side] = scalar_expression(w, sop);
    std::vector<int> vlanes, slanes;
    for (int c = 0; c < 4; ++c) {
        if (vmask >> c & 1) vlanes.push_back(c);
        if (smask >> c & 1) slanes.push_back(c);
    }
    const bool writes_scalar = !slanes.empty() || uses_ps_anywhere_;
    std::string svalue = stext;
    if (uses_ps_anywhere_) {
        if (stext != "ps") line(std::format("ps = {};", stext));
        svalue = "ps";
    } else if (writes_scalar && !exports && !vlanes.empty() && reads_register(stext, vdest)) {
        line(std::format("s = {};", stext));
        s_declared_ = true;
        svalue = "s";
    }
    if (ssat) svalue = std::format("saturate({})", svalue);
    // the scalar op's side effects (p0, a0, a kill) do not change what the vector op reads -- unless it writes a0 and the
    // vector op reads a constant through it
    const bool a0_late = std::ranges::any_of(side, [](const std::string& l) { return l.find("a0 =") != std::string::npos; }) && bits(w1, 29, 1) != 0;
    if (!a0_late) {
        for (const std::string& l : side) line(l);
    }
    if (exports) {
        const std::string target = export_target(vdest);
        std::vector<int> both, only_v, only_s;
        for (int c : vlanes) (std::ranges::find(slanes, c) != slanes.end() ? both : only_v).push_back(c);
        for (int c : slanes) {
            if (std::ranges::find(vlanes, c) == vlanes.end()) only_s.push_back(c);
        }
        if (!only_v.empty() || is_vector_side_effect(vop)) {
            emit_vector(w, vop, only_v, only_v.empty() ? std::nullopt : std::optional<std::string>(target), vsat);
        }
        if (!only_s.empty()) line(std::format("{}.{} = {};", target, components_of(only_s), widen(svalue, only_s.size())));
        if (!both.empty()) line(std::format("{}.{} = 1.0;", target, components_of(both)));
        if (!vmask && !smask && bits(w0, 14, 1)) line(std::format("{} = 0.0;", target));
    } else {
        if (!vlanes.empty() || is_vector_side_effect(vop)) {
            emit_vector(w, vop, vlanes, vlanes.empty() ? std::nullopt : std::optional<std::string>(temp(vdest)), vsat);
        }
        if (!slanes.empty()) line(std::format("{}.{} = {};", temp(sdest), components_of(slanes), widen(svalue, slanes.size())));
    }
    if (a0_late) {
        for (const std::string& l : side) line(l);
    }
    if (predicated) {
        --indent_;
        line("}");
    }
}

// The vector operation's lanes `lanes` into `dest` (a register or an export).
void Translator::emit_vector(const Word3& w, unsigned vop, const std::vector<int>& lanes, const std::optional<std::string>& dest, bool saturate) {
    static constexpr std::array<int, 4> k4 = {0, 1, 2, 3};
    static constexpr std::array<int, 3> k3 = {0, 1, 2};
    static constexpr std::array<int, 2> k2 = {0, 1};
    static constexpr std::array<int, 1> k0 = {0};
    static constexpr std::array<int, 1> k_w = {3};
    const std::string comps = components_of(lanes);
    const std::size_t n = lanes.size();
    // operands are read left to right, as the instruction lists them
    const auto op = [&](int i) { return vector_operand(w, i, lanes); };
    const auto op_of = [&](int i, std::span<const int> ls) { return vector_operand(w, i, ls); };
    std::string expr;
    switch (vop) {
        case 0: {
            const std::string a = op(1), b = op(2);
            expr = plus(a, b);
            break;
        }
        case 1: {
            const std::string a = op(1), b = op(2);
            expr = std::format("{} * {}", a, b);
            break;
        }
        case 2:
        case 3: {
            const std::string a = op(1), b = op(2);
            expr = a == b ? a : std::format("{}({}, {})", vop == 2 ? "max" : "min", a, b);
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7: {
            const std::string_view cmp = vop == 4 ? "==" : vop == 5 ? ">" : vop == 6 ? ">=" : "!=";
            const std::string a = op(1), b = op(2);
            expr = n == 1 ? std::format("(float)({} {} {})", a, cmp, b) : std::format("(float{})({} {} {})", n, a, cmp, b);
            break;
        }
        case 8: expr = std::format("frac({})", op(1)); break;
        case 9: expr = std::format("trunc({})", op(1)); break;
        case 10: expr = std::format("floor({})", op(1)); break;
        case 11: {
            const std::string a = op(1), b = op(2), c = op(3);
            expr = plus(std::format("{} * {}", a, b), c);
            break;
        }
        case 12:
        case 13:
        case 14: {
            const std::string_view cmp = vop == 12 ? "==" : vop == 13 ? ">=" : ">";
            const std::string a = op(1), b = op(2), c = op(3);
            expr = std::format("({} {} 0.0) ? {} : {}", a, cmp, b, c);
            break;
        }
        case 15: {
            const std::string a = op_of(1, k4), b = op_of(2, k4);
            expr = std::format("dot({}, {})", a, b);
            break;
        }
        case 16: {
            const std::string a = op_of(1, k3), b = op_of(2, k3);
            expr = std::format("dot({}, {})", a, b);
            break;
        }
        case 17: {
            const std::string a = op_of(1, k2), b = op_of(2, k2), c = op_of(3, k0);
            expr = std::format("dot({}, {}) + {}", a, b, c);
            break;
        }
        case 18: {
            cube_needed_ = true;
            const std::string a = op_of(1, k4), b = op_of(2, k4);
            expr = std::format("xe_cube({}, {})", a, b);
            if (n != 4) expr = std::format("({}).{}", expr, comps);
            break;
        }
        case 19: {
            const std::string v = op_of(1, k4);
            std::string flat = text::replace_all(text::replace_all(v, ".", ""), "_", "");
            if (all_of_set(flat, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")) {
                expr = std::format("max(max({0}.x, {0}.y), max({0}.z, {0}.w))", v);
            } else {
                line(std::format("float4 m4 = {};", v));
                expr = "max(max(m4.x, m4.y), max(m4.z, m4.w))";
            }
            break;
        }
        case 20:
        case 21:
        case 22:
        case 23: {
            const std::string_view cmp = vop == 20 ? "==" : vop == 21 ? "!=" : vop == 22 ? ">" : ">=";
            uses_p0_ = true;
            const std::string a = op_of(1, k_w), b = op_of(2, k_w);
            line(std::format("p0 = {} == 0.0 && {} {} 0.0;", a, b, cmp));
            const std::string ax = op_of(1, k0), bx = op_of(2, k0);
            expr = std::format("({} == 0.0 && {} {} 0.0) ? 0.0 : {} + 1.0", ax, bx, cmp, ax);
            break;
        }
        case 24:
        case 25:
        case 26:
        case 27: {
            const std::string_view cmp = vop == 24 ? "==" : vop == 25 ? ">" : vop == 26 ? ">=" : "!=";
            const std::string a = op_of(1, k4), b = op_of(2, k4);
            const std::optional<std::string> a1 = uniform(a), b1 = uniform(b);
            if (a1 && b1) line(std::format("if ({} {} {}) discard;", *a1, cmp, *b1));
            else line(std::format("if (any({} {} {})) discard;", a, cmp, b));
            expr = "0.0";
            break;
        }
        case 28: {
            const std::string a = op_of(1, k4), b = op_of(2, k4);
            expr = std::format("float4(1.0, {}.y * {}.y, {}.z, {}.w)", a, b, a, b);
            if (n != 4) expr = std::format("({}).{}", expr, comps);
            break;
        }
        case 29: {
            uses_a0_ = true;
            line(std::format("a0 = (int)floor({} + 0.5);", op_of(1, k_w)));
            const std::string a = op(1), b = op(2);
            expr = a == b ? a : std::format("max({}, {})", a, b);
            break;
        }
        default: fail(std::format("vector opcode {}", vop));
    }
    if (!dest || lanes.empty()) return;
    const bool reduces = (vop >= 15 && vop <= 17) || (vop >= 19 && vop <= 27);
    if (reduces && n > 1) {
        expr = expr.starts_with("dot(") ? std::format("{}.{}", expr, std::string(n, 'x')) : std::format("({}).{}", expr, std::string(n, 'x'));
    }
    if (saturate) expr = std::format("saturate({})", expr);
    line(std::format("{}.{} = {};", *dest, comps, expr));
}

// The scalar op's result as an expression, and the statements its side effects need.
std::pair<std::string, std::vector<std::string>> Translator::scalar_expression(const Word3& w, unsigned sop) {
    const std::uint32_t w1 = w[1];
    const std::uint32_t sw = bits(w1, 0, 8);
    const unsigned a_lane = ((sw >> 6) + 3) & 3, b_lane = sw & 3;
    std::vector<std::string> side;
    std::string a, b;   // b stays empty for the ops that take one operand
    if (is_scalar_two(sop)) {
        a = scalar_operand(w, a_lane);
        b = scalar_operand(w, b_lane);
    } else if (sop >= 42 && sop <= 47) {
        // mulsc/addsc/subsc: a constant lane and a temp register lane (03c 2.4)
        const std::uint32_t w0 = w[0], w2 = w[2];
        const std::array<Lane, 4> c_lanes = constant_lanes(bits(w2, 0, 8), constant_relative(w, 3), bits(w1, 29, 1) != 0);
        const std::uint32_t t_reg = (sop & 1) | (bits(w2, 29, 1) << 1) | (bits(w1, 0, 8) & 0x3C);
        const bool neg = bits(w1, 24, 1) != 0, ab = bits(w0, 7, 1) != 0;
        const Lane& ca = c_lanes[a_lane];
        if (ca.number) {
            const float v = ab ? std::fabs(ca.value) : ca.value;
            a = number_text(neg ? -v : v);
        } else {
            a = modify(lane_text(ca), ab, neg);
        }
        b = modify(std::format("{}.{}", temp(t_reg), kComponents[b_lane]), ab, neg);
    } else if (sop != 33 && sop != 50) {
        a = scalar_operand(w, a_lane);
    }
    const std::string prev = "ps";
    if (sop == 1 || sop == 3 || sop == 4 || sop == 26 || (sop == 50 && bits(w[0], 20, 4))) uses_ps_anywhere_ = true;
    const auto result = [&](std::string text) { return std::pair<std::string, std::vector<std::string>>(std::move(text), side); };
    switch (sop) {
        case 0:
        case 44:
        case 45: return result(plus(a, b));
        case 1: return result(std::format("{} + {}", a, prev));
        case 2:
        case 42:
        case 43: return result(std::format("{} * {}", a, b));
        case 3: return result(std::format("{} * {}", a, prev));
        case 4:
            return result(std::format("({0} == -1e38 || {1} == -1e38 || isinf({0}) || isnan({0})) ? -1e38 : {2} * {0}", prev, b, a));
        case 5: return result(a == b ? a : std::format("max({}, {})", a, b));
        case 6: return result(a == b ? a : std::format("min({}, {})", a, b));
        case 7:
        case 8:
        case 9:
        case 10: return result(std::format("({} {} 0.0) ? 1.0 : 0.0", a, sop == 7 ? "==" : sop == 8 ? ">" : sop == 9 ? ">=" : "!="));
        case 11: return result(std::format("frac({})", a));
        case 12: return result(std::format("trunc({})", a));
        case 13: return result(std::format("floor({})", a));
        case 14: return result(std::format("exp2({})", a));
        case 15: return result(std::format("max(log2({}), -1e38)", a));
        case 16: return result(std::format("log2({})", a));
        case 17:
        case 18:
        case 19: return result(std::format("1.0 / {}", a));
        case 20:
        case 21:
        case 22: return result(std::format("rsqrt({})", a));
        case 23:
        case 24:
            uses_a0_ = true;
            side.push_back(std::format("a0 = (int){}({});", sop == 23 ? "round" : "floor", a));
            return result(a);   // (b is never given for these two: the second operand is not read)
        case 25: return result(minus(a, b));
        case 26: return result(std::format("{} - {}", a, prev));
        case 27:
        case 28:
        case 29:
        case 30: {
            uses_p0_ = true;
            const std::string cond = std::format("{} {} 0.0", a, sop == 27 ? "==" : sop == 28 ? "!=" : sop == 29 ? ">" : ">=");
            side.push_back(std::format("p0 = {};", cond));
            return result(std::format("({}) ? 0.0 : 1.0", cond));
        }
        case 31:
            uses_p0_ = true;
            side.push_back(std::format("p0 = {} == 1.0;", a));
            return result(std::format("({0} == 1.0) ? 0.0 : ({0} == 0.0 ? 1.0 : {0})", a));
        case 32:
            uses_p0_ = true;
            side.push_back(std::format("p0 = {} - 1.0 <= 0.0;", a));
            return result(std::format("({0} - 1.0 <= 0.0) ? 0.0 : {0} - 1.0", a));
        case 33:
            uses_p0_ = true;
            side.push_back("p0 = false;");
            return result("1e38");
        case 34:
            uses_p0_ = true;
            side.push_back(std::format("p0 = {} == 0.0;", a));
            return result(a);
        case 35:
        case 36:
        case 37:
        case 38:
        case 39: {
            const std::string_view cmp = sop == 35 ? "== 0.0" : sop == 36 ? "> 0.0" : sop == 37 ? ">= 0.0" : sop == 38 ? "!= 0.0" : "== 1.0";
            side.push_back(std::format("if ({} {}) discard;", a, cmp));
            return result("0.0");
        }
        case 40: return result(std::format("sqrt({})", a));
        case 46:
        case 47: return result(minus(a, b));
        case 48: return result(std::format("sin({})", a));
        case 49: return result(std::format("cos({})", a));
        case 50: return result(prev);
        default: break;
    }
    fail(std::format("scalar opcode {}", sop));
}

// The fetch's destination swizzle: 0-3 a fetched lane, 4 zero, 5 one, 7 left as it is.
void Translator::fetch_destination(std::uint32_t w0, std::uint32_t w1, const std::array<Lane, 4>& value_lanes) {
    const std::string dest = temp(bits(w0, 12, 6), bits(w0, 18, 1) != 0);
    const std::uint32_t sw = bits(w1, 0, 12);
    std::string comps;
    std::vector<Lane> parts;
    for (unsigned c = 0; c < 4; ++c) {
        const unsigned sel = (sw >> (3 * c)) & 7;
        if (sel == 7) continue;
        if (sel == 6) fail("fetch destination swizzle 6");
        comps += kComponents[c];
        parts.push_back(sel < 4 ? value_lanes[sel] : number_lane(sel == 4 ? 0.0f : 1.0f));
    }
    if (!comps.empty()) line(std::format("{}.{} = {};", dest, comps, lanes_text(parts)));
}

void Translator::emit_fetch(const Word3& w) {
    const auto [w0, w1, w2] = w;
    const unsigned opcode = bits(w0, 0, 5);
    const bool predicated = bits(w1, 31, 1) != 0;
    if (predicated) {
        uses_p0_ = true;
        line(std::format("if ({}p0)", bits(w2, 31, 1) ? "" : "!"));
        line("{");
        ++indent_;
    }
    if (opcode == 0) {
        emit_vertex_fetch(w);
    } else if (opcode == 1) {
        emit_texture_fetch(w);
    } else if (opcode == 24) {   // setTexLOD: the level of detail later fetches may use
        uses_lod_ = true;
        line(std::format("lod = {}.{};", temp(bits(w0, 5, 6), bits(w0, 11, 1) != 0), kComponents[bits(w0, 26, 6) & 3]));
    } else {
        fail(std::format("fetch instruction {}", opcode));
    }
    if (predicated) {
        --indent_;
        line("}");
    }
}

void Translator::emit_vertex_fetch(const Word3& w) {
    const auto it = std::ranges::find_if(s_.vertex_inputs, [&](const VertexInput& e) { return e.slot == slot_; });
    if (it == s_.vertex_inputs.end()) fail("a vertex fetch the input table does not name");
    const std::string field = field_name(it->usage, it->index, true);
    inputs_.insert_or_assign(field, std::format("{}{}", it->usage, it->index));
    std::array<Lane, 4> lanes;
    for (unsigned c = 0; c < 4; ++c) lanes[c] = named_lane("input." + field, std::string(1, kComponents[c]));
    fetch_destination(w[0], w[1], lanes);
}

const Constant& Translator::sampler_for(std::uint32_t tf) {
    const Shader::Found found = s_.constant_at(RegisterSet::Sampler, tf);
    if (!found.constant) fail(std::format("texture fetch constant {} is bound to no sampler", tf));
    if (std::ranges::find(samplers_, found.constant->name) == samplers_.end()) samplers_.push_back(found.constant->name);
    return *found.constant;
}

void Translator::emit_texture_fetch(const Word3& w) {
    const auto [w0, w1, w2] = w;
    const std::uint32_t tf = bits(w0, 20, 5);
    const std::uint32_t dim = bits(w2, 14, 2);
    const std::string src = temp(bits(w0, 5, 6), bits(w0, 11, 1) != 0);
    const std::uint32_t sw = bits(w0, 26, 6);
    const std::string sampler = sampler_for(tf).name;
    if (bits(w0, 25, 1)) fail("unnormalized texture coordinates");
    if (bits(w2, 16, 10)) fail("texture fetch offsets");
    const double bias = sign_extend(bits(w2, 2, 7), 7) / 16.0;
    const bool computed_lod = bits(w1, 28, 1) && !vertex_;
    const bool register_lod = bits(w1, 29, 1) != 0;
    const auto swizzle = [&](unsigned count) {
        std::string out = src + ".";
        for (unsigned i = 0; i < count; ++i) out += kComponents[(sw >> (2 * i)) & 3];
        return out;
    };
    std::string coord, kind;
    if (dim == 1) {
        coord = swizzle(2);
        kind = "2D";
    } else if (dim == 0) {
        coord = swizzle(1);
        kind = "1D";
    } else if (dim == 2) {
        coord = swizzle(3);
        kind = "3D";
    } else {
        // cube: the face coordinates the cube instruction made (+1.5) and the face, back to a direction
        cube_fetch_needed_ = true;
        coord = std::format("xe_cube_direction({})", swizzle(3));
        kind = "CUBE";
    }
    const std::string level = register_lod ? (bias != 0 ? std::format("lod + {}", number_text(bias)) : "lod") : number_text(bias);
    std::string call;
    if (computed_lod && !register_lod) {
        call = bias == 0 ? std::format("tex{}({}, {})", kind, sampler, coord)
                         : std::format("tex{}bias({}, float4({}, {}))", kind, sampler, pad(coord, kind), number_text(bias));
    } else if (computed_lod) {
        call = std::format("tex{}bias({}, float4({}, {}))", kind, sampler, pad(coord, kind), level);
    } else {
        call = std::format("tex{}lod({}, float4({}, {}))", kind, sampler, pad(coord, kind), level);
    }
    line(std::format("t = {};", call));
    t_declared_ = true;
    std::array<Lane, 4> lanes;
    for (unsigned c = 0; c < 4; ++c) lanes[c] = named_lane("t", std::string(1, kComponents[c]));
    fetch_destination(w0, w1, lanes);
}

// ---- control flow

// The control flow, then the body: the CF instructions fill the first slots, up to the first exec's address; the program
// ends at an unconditional exece.
Function Translator::run() {
    const std::span<const std::uint32_t> words = s_.words;
    program_.clear();
    std::optional<std::size_t> first_exec;
    std::size_t index = 0;
    while ((index / 2) * 3 + 2 < words.size()) {
        if (first_exec && index >= *first_exec * 2) break;
        const std::uint64_t cf = control_flow(words, index);
        const unsigned op = bits(cf, 44, 4);
        if (is_exec_op(op)) {
            const std::size_t address = bits(cf, 0, 12);
            first_exec = first_exec ? std::min(*first_exec, address) : address;
        }
        program_.push_back(cf);
        ++index;
        if (op == 2) break;
    }
    // loops: a loop start and the loop end jumping back to the instruction after it, a `for` over the loop constant's
    // count; jumps: forward ones become `skip_to` (the blocks a taken jump passes over do not run), out of a loop a break
    // as well; others stop
    jumps_ = false;
    loop_end_.clear();
    for (std::size_t i = 0; i < program_.size(); ++i) {
        const std::uint64_t cf = program_[i];
        const unsigned op = bits(cf, 44, 4);
        if (op == 7) {
            std::optional<std::size_t> end;
            for (std::size_t j = i + 1; j < program_.size() && !end; ++j) {
                if (bits(program_[j], 44, 4) == 8 && bits(program_[j], 0, 13) == i + 1) end = j;
            }
            if (!end) {
                slot_ = static_cast<std::uint32_t>(i);
                fail("a loop without its end");
            }
            loop_end_[i] = *end;
        } else if (op == 11) {
            if (bits(cf, 0, 13) <= i) {
                slot_ = static_cast<std::uint32_t>(i);
                fail("a backward jump in the control flow");
            }
            jumps_ = true;
        } else if (op == 9 || op == 10) {
            slot_ = static_cast<std::uint32_t>(i);
            fail(std::format("control flow instruction {} (calls are not translated yet)", op));
        } else if ((op == 4 || op == 6 || op == 14) && i + 1 < program_.size()) {
            slot_ = static_cast<std::uint32_t>(i);
            fail("a conditional end of the program before its last instruction");
        }
    }
    // one pass first to learn whether the previous scalar result is read anywhere
    uses_ps_anywhere_ = false;
    for (int pass = 0; pass < 2; ++pass) {
        const bool keep = uses_ps_anywhere_;
        reset_body();
        uses_ps_anywhere_ = keep;
        emit_range(0, program_.size(), {});
    }
    return assemble();
}

// CF instructions lo..hi-1; `loops`: the (start, end) of each loop around them, outermost first.
void Translator::emit_range(std::size_t lo, std::size_t hi, const std::vector<LoopRange>& loops) {
    std::size_t i = lo;
    while (i < hi) {
        const std::uint64_t cf = program_[i];
        const unsigned op = bits(cf, 44, 4);
        if (is_exec_op(op)) {
            if (jumps_) {
                line(std::format("if (skip_to <= {})", i));
                line("{");
                ++indent_;
            }
            emit_exec(cf);
            if (jumps_) {
                --indent_;
                line("}");
            }
        } else if (op == 11) {
            const std::size_t target = bits(cf, 0, 13);
            std::string cond;
            if (bits(cf, 13, 1)) {
                cond = std::format("skip_to <= {}", i);
            } else if (bits(cf, 14, 1)) {
                uses_p0_ = true;
                cond = std::format("skip_to <= {} && {}p0", i, bits(cf, 42, 1) ? "" : "!");
            } else {
                cond = std::format("skip_to <= {} && {}{}", i, bits(cf, 42, 1) ? "" : "!", bool_name(bits(cf, 34, 8)));
            }
            const bool leaves = !loops.empty() && target > loops.back().second;
            line(std::format("if ({}) {{ skip_to = {};{} }}", cond, target, leaves ? " break;" : ""));
        } else if (op == 7) {
            const std::size_t end = loop_end_.at(i);
            emit_loop(i, end, loops);
            i = end + 1;
            continue;
        }
        ++i;
    }
}

void Translator::emit_loop(std::size_t start, std::size_t end, const std::vector<LoopRange>& loops) {
    const std::uint64_t cf = program_[start], cf_end = program_[end];
    const LoopConstant lc = loop_constant(bits(cf, 16, 5));
    const std::size_t depth = loops.size();
    const std::string it = std::format("i{}", depth);
    uses_al_ = true;
    if (jumps_) {
        line(std::format("if (skip_to <= {})", start));
        line("{");
        ++indent_;
    }
    line(std::format("[loop] for (int {0} = 0; {0} < {1}; {0}++)", it, lc.count));
    line("{");
    ++indent_;
    if (!bits(cf, 13, 1)) {   // the loop sets aL (or keeps the outer one's)
        std::string value;
        if (lc.step == "0") value = lc.first;
        else if (lc.first != "0") value = std::format("{} + {} * {}", lc.first, it, lc.step);
        else if (lc.step != "1") value = std::format("{} * {}", it, lc.step);
        else value = it;
        line(std::format("aL = {};", value));
    }
    if (jumps_) line(std::format("skip_to = {};", start + 1));
    std::vector<LoopRange> inner = loops;
    inner.emplace_back(start, end);
    emit_range(start + 1, end, inner);
    if (bits(cf_end, 21, 1)) {   // the loop end's predicated break
        uses_p0_ = true;
        line(std::format("if ({}p0) break;", bits(cf_end, 42, 1) ? "" : "!"));
    }
    --indent_;
    line("}");
    if (!loops.empty() && !bits(program_[loops.back().first], 13, 1)) {
        const LoopConstant outer = loop_constant(bits(program_[loops.back().first], 16, 5));
        line(std::format("aL = {} + i{} * {};", outer.first, depth - 1, outer.step));
    }
    if (jumps_) {
        --indent_;
        line("}");
    }
}

// Loop constant k -> (count, aL's first value, its step) as expressions: an int parameter's own value is the count (start
// and step its defaults' second and third), an int4's are its x, y and z.
Translator::LoopConstant Translator::loop_constant(std::uint32_t k) {
    if (const auto it = s_.loop_literals.find(k); it != s_.loop_literals.end()) {   // one the compiler made: numbers
        const std::uint32_t v = it->second;
        const std::int32_t step = static_cast<std::int32_t>((v >> 16) & 0xFF);
        return {std::to_string(v & 0xFF), std::to_string((v >> 8) & 0xFF), std::to_string((step & 0x80) ? step - 256 : step)};
    }
    const std::int64_t reg = static_cast<std::int64_t>(k) - (vertex_ ? 0 : 16);
    const Shader::Found found = s_.constant_at(RegisterSet::Int4, reg);
    if (!found.constant) fail(std::format("loop constant i{} is in no constant of the table", k));
    const Constant& c = *found.constant;
    const std::string name = c.elements <= 1 ? c.name : std::format("{}[{}]", c.name, found.offset);
    if (c.cls == ConstantClass::Scalar) {
        // the default value's floats; a constant with none counts as {0, 0, 1, 0}
        const std::vector<float> values = c.defaults.empty() ? std::vector<float>{0, 0, 1, 0} : c.defaults;
        std::vector<int> d;
        for (std::size_t i = 4ull * found.offset; i < 4ull * found.offset + 4 && i < values.size(); ++i) {
            d.push_back(std::fabs(values[i]) < 1e6f && values[i] == std::trunc(values[i]) ? static_cast<int>(values[i]) : 0);
        }
        return {name, d.size() > 1 ? std::to_string(d[1]) : "0", d.size() > 2 ? std::to_string(d[2]) : "1"};
    }
    return {name + ".x", name + ".y", name + ".z"};
}

std::string Translator::bool_name(std::int64_t b) {
    if (!vertex_) b -= 128;   // a pixel shader's bool constants follow the vertex shader's
    const Shader::Found found = s_.constant_at(RegisterSet::Bool, b);
    if (!found.constant) fail(std::format("bool constant b{} is in no constant of the table", b));
    const Constant& c = *found.constant;
    return c.elements <= 1 ? c.name : std::format("{}[{}]", c.name, found.offset);
}

void Translator::reset_body() {
    lines_.clear();
    indent_ = 1;
    samplers_.clear();
    used_regs_.clear();
    inputs_.clear();
    outputs_.clear();
    uses_a0_ = uses_p0_ = uses_al_ = uses_lod_ = false;
    s_declared_ = t_declared_ = false;
    cube_needed_ = cube_fetch_needed_ = false;
}

void Translator::emit_exec(std::uint64_t cf) {
    const unsigned op = bits(cf, 44, 4);
    const std::uint32_t address = bits(cf, 0, 12), count = bits(cf, 12, 3), sequence = bits(cf, 16, 12);
    const bool condition = bits(cf, 42, 1) != 0;
    bool guarded = false;
    if (op == 3 || op == 4 || op == 13 || op == 14) {   // on a bool constant
        line(std::format("if ({}{})", condition ? "" : "!", bool_name(bits(cf, 34, 8))));
        guarded = true;
    } else if (op == 5 || op == 6) {   // on the predicate
        uses_p0_ = true;
        line(std::format("if ({}p0)", condition ? "" : "!"));
        guarded = true;
    }
    if (guarded) {
        line("{");
        ++indent_;
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        slot_ = address + i;
        const std::size_t at = static_cast<std::size_t>(slot_) * 3;
        if (at + 3 > s_.words.size()) throw FormatError("xenos_hlsl", at * 4, std::format("an exec at slot {} reads past the microcode", slot_));
        const Word3 w = {s_.words[at], s_.words[at + 1], s_.words[at + 2]};
        if ((sequence >> (2 * i)) & 1) emit_fetch(w);
        else emit_alu(w);
    }
    if (guarded) {
        --indent_;
        line("}");
    }
}

Function Translator::assemble() {
    const std::string& n = name_;
    std::string out;
    std::map<std::uint32_t, std::pair<std::string, std::uint32_t>> pixel_inputs;   // register -> field, mask
    if (!vertex_) {
        for (const Interpolator& in : s_.pixel_inputs) {
            std::string field = capitalize(in.usage);
            field = std::format("{}{}", text::replace_all(field, "Texcoord", "TexCoord"), in.index);
            pixel_inputs[in.reg] = {field, in.mask};
            inputs_.insert_or_assign(field, std::format("{}{}", in.usage, in.index));
        }
    }
    out += std::format("struct {}_Input\n{{\n", n);
    for (const auto& [field, semantic] : inputs_) out += std::format("    float4 {} : {};\n", field, semantic);
    if (inputs_.empty()) out += vertex_ ? "    float4 Position0 : POSITION0;\n" : "    float4 Unused : TEXCOORD15;\n";
    out += "};\n\n";
    out += std::format("struct {}_Output\n{{\n", n);
    if (vertex_ && !outputs_.contains("Position")) {
        OrderedMap<std::string, std::string> with_position;
        with_position.insert_or_assign("Position", "POSITION0");
        for (const auto& [field, semantic] : outputs_) with_position.insert_or_assign(field, semantic);
        outputs_ = std::move(with_position);
    }
    for (const auto& [field, semantic] : outputs_) {
        out += std::format("    float{} {} : {};\n", field == "Depth" || field == "PointSize" ? "" : "4", field, semantic);
    }
    out += "};\n\n";
    out += std::format("{0}_Output {0}({0}_Input input)\n{{\n", n);
    out += std::format("    {0}_Output output = ({0}_Output)0;\n", n);
    std::set<std::uint32_t> regs = used_regs_;
    for (const auto& [reg, field] : pixel_inputs) regs.insert(reg);
    if (!regs.empty()) {
        std::string list;
        for (std::uint32_t r : regs) list += std::format("{}r{} = 0.0", list.empty() ? "" : ", ", r);
        out += std::format("    float4 {};\n", list);
    }
    if (uses_ps_anywhere_) out += "    float ps = 0.0;\n";
    if (uses_p0_) out += "    bool p0 = false;\n";
    if (uses_a0_) out += "    int a0 = 0;\n";
    if (uses_al_) out += "    int aL = 0;\n";
    if (uses_lod_) out += "    float lod = 0.0;\n";
    if (t_declared_) out += "    float4 t;\n";
    if (s_declared_) out += "    float s;\n";
    if (jumps_) out += "    int skip_to = 0;\n";
    for (const auto& [reg, field_mask] : pixel_inputs) {
        std::string comps;
        for (unsigned c = 0; c < 4; ++c) {
            if (field_mask.second >> c & 1) comps += kComponents[c];
        }
        if (comps.empty()) comps = "xyzw";
        out += std::format("    r{0}.{2} = input.{1}.{2};\n", reg, field_mask.first, comps);
    }
    for (const std::string& l : lines_) out += l + "\n";
    out += "    return output;\n}\n";
    Function f;
    f.text = std::move(out);
    f.samplers = samplers_;
    f.inputs = inputs_;
    f.outputs = outputs_;
    f.needs_helpers = needs_helpers();
    return f;
}

}  // namespace

std::string number_text(double f) {
    if (std::isnan(f)) return "(0.0 / 0.0)";
    if (std::isinf(f)) return f > 0 ? "1e38" : "-1e38";
    const float f32 = static_cast<float>(f);
    if (std::isinf(f32)) return f > 0 ? "1e38" : "-1e38";   // (past the float range)
    // the fewest digits that read back as the same float, in the style of printf's %g (1e-05, 1.5, 100000 as 1e+05)
    std::array<char, 64> buf{};
    std::string s;
    for (int digits = 1; digits < 10; ++digits) {
        const auto r = std::to_chars(buf.data(), buf.data() + buf.size(), f32, std::chars_format::general, digits);
        s.assign(buf.data(), r.ptr);
        float back = 0;
        std::from_chars(s.data(), s.data() + s.size(), back);
        if (back == f32) break;
    }
    if (s.find('e') == std::string::npos && s.find('.') == std::string::npos) s += ".0";
    return s;
}

Function translate(const Shader& shader, std::string_view name) {
    Translator t(shader, name);
    return t.run();
}

const std::string_view kHelpers = R"HLSL(// The Xbox 360 GPU's cube instruction: the face coordinates (+1.5), the major axis times two, the face index.
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

}  // namespace cb::decompile::xenos_hlsl
