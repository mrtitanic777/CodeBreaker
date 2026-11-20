#include "decompile/xbox_effect.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <optional>
#include <set>

#include "core/error.hpp"
#include "core/ordered_map.hpp"
#include "core/text.hpp"
#include "decompile/xenos_hlsl.hpp"
#include "formats/xbox/d3dx.hpp"
#include "formats/xbox/effect.hpp"
#include "formats/xbox/shader.hpp"

namespace cb::decompile::xbox_effect {

namespace {

namespace d3dx = formats::xbox::d3dx;
namespace fxf = formats::xbox::effect;
namespace shf = formats::xbox::shader;
namespace hlsl = decompile::xenos_hlsl;

// The render states MonoGame's effect compiler takes; the rest are written as comments.
bool fxe_is_pass_state(std::string_view name) {
    static const std::set<std::string_view> states = {
        "AlphaBlendEnable", "SrcBlend", "DestBlend", "BlendOp", "SeparateAlphaBlendEnable", "SrcBlendAlpha",
        "DestBlendAlpha", "BlendOpAlpha", "BlendFactor", "ColorWriteEnable", "ColorWriteEnable1", "ColorWriteEnable2",
        "ColorWriteEnable3", "ZEnable", "ZWriteEnable", "ZFunc", "DepthBias", "SlopeScaleDepthBias", "StencilEnable",
        "StencilFail", "StencilZFail", "StencilPass", "StencilFunc", "StencilRef", "StencilMask", "StencilWriteMask",
        "TwoSidedStencilMode", "CCW_StencilFail", "CCW_StencilZFail", "CCW_StencilPass", "CCW_StencilFunc", "CullMode",
        "FillMode", "MultiSampleAntiAlias", "ScissorTestEnable"};
    return states.contains(name);
}

bool fxe_is_sampler_state(std::string_view name) {
    static const std::set<std::string_view> states = {"AddressU", "AddressV", "AddressW", "BorderColor", "MagFilter",
                                                       "MinFilter", "MipFilter", "MipMapLodBias", "MaxMipLevel", "MaxAnisotropy"};
    return states.contains(name);
}

// "float" / "float4" / "float4x4" / "int" ... for a numeric parameter; nothing for any other.
std::optional<std::string> fxe_type_name(const fxf::Parameter& p) {
    std::string_view base;
    switch (p.type) {
        case d3dx::ParamType::Float: base = "float"; break;
        case d3dx::ParamType::Int: base = "int"; break;
        case d3dx::ParamType::Bool: base = "bool"; break;
        default: return std::nullopt;
    }
    switch (p.cls) {
        case d3dx::ParamClass::Scalar: return std::string(base);
        case d3dx::ParamClass::Vector: return std::format("{}{}", base, p.columns);
        // declared without row_major (MonoGame's compiler takes no such parameter): the shaders read a matrix's elements by
        // row and column (_m01), which mean the same whichever way it is stored
        case d3dx::ParamClass::MatrixRows:
        case d3dx::ParamClass::MatrixColumns: return std::format("{}{}x{}", base, p.rows, p.columns);
        default: return std::nullopt;
    }
}

// The numbers of a numeric parameter, as written.
std::vector<double> fxe_numbers(const fxf::Parameter& p) {
    std::vector<double> out;
    for (float f : p.floats()) out.push_back(f);
    for (std::int32_t i : p.ints()) out.push_back(i);
    return out;
}

bool fxe_any_nonzero(const std::vector<double>& v) {
    return std::any_of(v.begin(), v.end(), [](double x) { return x != 0; });
}

// The values of a numeric parameter as an HLSL initializer.
std::string fxe_literal(const fxf::Parameter& p, std::vector<double> values) {
    const auto one = [&](double v) {
        if (p.type == d3dx::ParamType::Bool) return std::string(v != 0 ? "true" : "false");
        if (p.type == d3dx::ParamType::Int) return std::to_string(static_cast<std::int64_t>(v));
        return hlsl::number_text(v);
    };
    const std::size_t n = std::size_t{p.rows} * p.columns;
    if (p.cls == d3dx::ParamClass::Scalar) {
        if (values.empty()) throw FormatError("xbox effect", 0, std::format("the parameter {} has no value", p.name));
        return one(values[0]);
    }
    if (values.size() < n) throw FormatError("xbox effect", 0, std::format("the parameter {} has {} values, not {}", p.name, values.size(), n));
    if (p.cls == d3dx::ParamClass::MatrixColumns) {   // stored a column at a time; the initializer is by rows
        std::vector<double> by_rows;
        for (std::size_t r = 0; r < p.rows; ++r) {
            for (std::size_t c = 0; c < p.columns; ++c) by_rows.push_back(values[c * p.rows + r]);
        }
        values = std::move(by_rows);
    }
    std::string parts;
    for (std::size_t i = 0; i < n; ++i) parts += (i ? ", " : "") + one(values[i]);
    return std::format("{}({})", fxe_type_name(p).value_or("float"), parts);
}

// ` <string name = "..."; float4 other = float4(...);>` for the annotations that can be written, else "".
std::string fxe_string_annotations(const fxf::Effect& fx, const fxf::Parameter& p) {
    std::vector<std::string> parts;
    for (const fxf::Parameter& a : p.annotations) {
        if (a.type == d3dx::ParamType::String && a.object()) {
            parts.push_back(std::format("string {} = \"{}\";", a.name, text::replace_all(fx.string(*a.object()), "\"", "'")));
        } else if (const auto t = fxe_type_name(a);
                   t && (std::holds_alternative<std::vector<float>>(a.value) || std::holds_alternative<std::vector<std::int32_t>>(a.value))) {
            parts.push_back(std::format("{} {} = {};", *t, a.name, fxe_literal(a, fxe_numbers(a))));
        }
    }
    return parts.empty() ? std::string() : std::format(" <{}>", text::join(parts, " "));
}

// The texture parameter a sampler's Texture state names.
std::optional<std::string> fxe_texture_of(const fxf::Effect& fx, const fxf::State& state) {
    if (state.parameter.reference && !state.parameter.reference->empty()) return state.parameter.reference;
    const std::optional<fxf::ObjectId> wanted = state.parameter.object();
    if (!wanted) return std::nullopt;
    for (const fxf::Parameter& p : fx.parameters) {
        if (d3dx::is_texture_type(p.type) && p.object() == wanted) return p.name;
    }
    return std::nullopt;
}

// A shader state choosing from a shader array by an expression: the object id per selector value, and the selector's name.
// The expression's preshader names the array (first), the selecting int and the int array indexing the shader array, if
// there is one; that is the stock effects' form, and the only one taken.
struct FxeSelection {
    std::vector<fxf::ObjectId> ids;
    std::string selector;
    std::string array;
};

std::optional<FxeSelection> fxe_array_selection(const fxf::Effect& fx, const fxf::State& state) {
    const std::optional<Bytes>& e = state.parameter.expression;
    if (!e || e->size() < 4) return std::nullopt;
    const ByteView v(*e);
    const std::uint32_t n = fx.big_endian ? (std::uint32_t{v[0]} << 24) | (std::uint32_t{v[1]} << 16) | (std::uint32_t{v[2]} << 8) | v[3]
                                          : (std::uint32_t{v[3]} << 24) | (std::uint32_t{v[2]} << 16) | (std::uint32_t{v[1]} << 8) | v[0];
    // (a length past the end of the expression names what is there: the preshader's own count is not trusted)
    const std::size_t name_end = std::min<std::size_t>(std::size_t{4} + n, v.size());
    const ByteView name_bytes = v.subspan(4, name_end - 4);
    const auto nul = std::find(name_bytes.begin(), name_bytes.end(), std::uint8_t{0});
    const std::string array_name = text::from_latin1(name_bytes.first(static_cast<std::size_t>(nul - name_bytes.begin())));
    const fxf::Parameter* array = fx.parameter(array_name);
    if (array == nullptr || array->elements == 0) return std::nullopt;
    const ByteView rest = v.subspan(name_end);
    const auto in_rest = [&](const std::string& name) {
        std::string needle = name;
        needle.push_back('\0');
        return std::search(rest.begin(), rest.end(), needle.begin(), needle.end(),
                           [](std::uint8_t a, char b) { return static_cast<char>(a) == b; }) != rest.end();
    };
    const fxf::Parameter* selector = nullptr;
    const fxf::Parameter* table = nullptr;
    for (const fxf::Parameter& p : fx.parameters) {
        if (p.type != d3dx::ParamType::Int || !in_rest(p.name)) continue;
        if (selector == nullptr && p.elements == 0 && p.cls == d3dx::ParamClass::Scalar) selector = &p;
        if (table == nullptr && p.elements != 0) table = &p;
    }
    if (selector == nullptr) return std::nullopt;
    std::vector<std::size_t> indices;
    if (table != nullptr) {
        for (const fxf::Parameter& m : table->members) {
            if (m.ints().empty() || m.ints()[0] < 0) return std::nullopt;
            indices.push_back(static_cast<std::size_t>(m.ints()[0]));
        }
    } else {
        for (std::size_t i = 0; i < array->elements; ++i) indices.push_back(i);
    }
    FxeSelection sel;
    sel.selector = selector->name;
    sel.array = array_name;
    for (std::size_t i : indices) {
        if (i >= array->members.size() || !array->members[i].object()) return std::nullopt;   // not the stock effects' form
        sel.ids.push_back(*array->members[i].object());
    }
    return sel;
}

// The part of the shader container the translation reads.
hlsl::Shader fxe_translatable(const shf::Shader& s) {
    hlsl::Shader n;
    n.stage = s.stage == shf::Stage::Vertex ? hlsl::Stage::Vertex : hlsl::Stage::Pixel;
    for (const shf::Constant& c : s.constants) {
        hlsl::Constant k;
        k.name = c.name;
        switch (c.set) {
            case shf::RegisterSet::Bool: k.set = hlsl::RegisterSet::Bool; break;
            case shf::RegisterSet::Int4: k.set = hlsl::RegisterSet::Int4; break;
            case shf::RegisterSet::Float4: k.set = hlsl::RegisterSet::Float4; break;
            default: k.set = hlsl::RegisterSet::Sampler; break;
        }
        k.first_register = c.first_register;
        k.count = c.count;
        switch (c.cls) {
            case d3dx::ParamClass::Scalar: k.cls = hlsl::ConstantClass::Scalar; break;
            case d3dx::ParamClass::Vector: k.cls = hlsl::ConstantClass::Vector; break;
            case d3dx::ParamClass::MatrixRows: k.cls = hlsl::ConstantClass::MatrixRows; break;
            case d3dx::ParamClass::MatrixColumns: k.cls = hlsl::ConstantClass::MatrixColumns; break;
            default: k.cls = hlsl::ConstantClass::Other; break;
        }
        k.is_bool = c.type == d3dx::ParamType::Bool;
        k.rows = c.rows;
        k.columns = c.columns;
        k.elements = c.elements;
        k.defaults = c.defaults;
        n.constants.push_back(std::move(k));
    }
    for (const shf::VertexInput& v : s.vertex_inputs) n.vertex_inputs.push_back({v.slot, v.usage, v.index});
    for (const shf::Interpolator& i : s.pixel_inputs) n.pixel_inputs.push_back({i.reg, i.usage, i.index, i.mask});
    for (const shf::Interpolator& o : s.outputs) n.outputs.push_back({o.reg, o.usage, o.index, o.mask});
    for (const auto& [reg, v] : s.literals) n.literals[reg] = v;
    for (const auto& [reg, v] : s.loop_literals) n.loop_literals[reg] = v;
    n.words = s.words;
    return n;
}

// What a technique's pass uses for each of its two shaders: one name, or one per value of a selecting parameter.
struct FxeVariant {
    std::optional<std::string> vs, ps;
};

// The shaders of the effect as HLSL functions, one per distinct shader, named for the technique and pass using it first.
class FxeFunctions {
public:
    const std::string& name_of(const Bytes& blob, std::string wanted) {
        if (const auto it = names_.find(blob); it != names_.end()) return it->second;
        std::string name = text::sanitize_identifier(wanted);
        // two shaders that would be named alike are two functions: the later one is numbered
        std::string unique = name;
        for (int n = 2; functions_.contains(unique); ++n) unique = std::format("{}_{}", name, n);
        functions_.insert_or_assign(unique, hlsl::translate(fxe_translatable(shf::read(blob)), unique));
        return names_.emplace(blob, std::move(unique)).first->second;
    }
    const OrderedMap<std::string, hlsl::Function>& all() const { return functions_; }

private:
    std::map<Bytes, std::string> names_;
    OrderedMap<std::string, hlsl::Function> functions_;
};

}  // namespace

std::string to_fx(ByteView data, std::string_view name) {
    const fxf::Effect fx = fxf::read(data);
    std::vector<std::string> out = {
        std::format("// {}: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect) from its microcode.", name),
        "// The shaders read as the compiled programs did: registers r0..rN, the constants by their names.", ""};

    // the shaders, by the passes using them: each pass as one or more variants (vertex, pixel function names)
    FxeFunctions functions;
    std::vector<std::vector<std::vector<FxeVariant>>> variants(fx.techniques.size());   // [technique][pass] -> per selector value
    std::set<std::string> selectors;
    for (std::size_t ti = 0; ti < fx.techniques.size(); ++ti) {
        const fxf::Technique& t = fx.techniques[ti];
        for (std::size_t i = 0; i < t.passes.size(); ++i) {
            const fxf::Pass& p = t.passes[i];
            std::map<std::string_view, std::vector<std::string>> choices;
            for (const auto& [kind, prefix] : {std::pair<std::string_view, std::string_view>{"VertexShader", "VS"},
                                               std::pair<std::string_view, std::string_view>{"PixelShader", "PS"}}) {
                const fxf::State* s = p.state(kind);
                if (s == nullptr) continue;
                if (const std::optional<FxeSelection> chosen = fxe_array_selection(fx, *s)) {
                    selectors.insert(chosen->selector);
                    const fxf::Parameter* array = fx.parameter(chosen->array);
                    std::map<std::uint32_t, std::size_t> element;   // an id the array holds twice names its last element
                    for (std::size_t k = 0; k < array->members.size(); ++k) {
                        if (const auto id = array->members[k].object()) element[id->id] = k;
                    }
                    std::vector<std::string> names;
                    for (const fxf::ObjectId id : chosen->ids) {
                        const Bytes* blob = fx.object(id);
                        if (blob == nullptr) throw FormatError("xbox effect", 0, std::format("the shader array {} names an object the effect lacks", chosen->array));
                        names.push_back(functions.name_of(*blob, std::format("{}_{}", chosen->array, element.at(id.id))));
                    }
                    choices[kind] = std::move(names);
                } else if (const auto id = s->parameter.object(); id && fx.object(*id) && !fx.object(*id)->empty()) {
                    choices[kind] = {functions.name_of(*fx.object(*id), std::format("{}_{}_{}", prefix, t.name,
                                                                                    p.name.empty() ? std::format("P{}", i) : p.name))};
                }
            }
            std::size_t count = 1;
            for (const auto& [kind, names] : choices) count = std::max(count, names.size());
            std::vector<FxeVariant> per_value;
            for (std::size_t v = 0; v < count; ++v) {
                FxeVariant var;
                if (const auto it = choices.find("VertexShader"); it != choices.end()) var.vs = it->second[it->second.size() > 1 ? v : 0];
                if (const auto it = choices.find("PixelShader"); it != choices.end()) var.ps = it->second[it->second.size() > 1 ? v : 0];
                per_value.push_back(std::move(var));
            }
            variants[ti].push_back(std::move(per_value));
        }
    }

    // the parameters
    for (const fxf::Parameter& p : fx.parameters) {
        if (d3dx::is_sampler_type(p.type) || p.type == d3dx::ParamType::VertexShader || p.type == d3dx::ParamType::PixelShader) continue;
        const std::string semantic = p.semantic.empty() ? std::string() : " : " + p.semantic;
        const std::string array = p.elements != 0 ? std::format("[{}]", p.elements) : std::string();
        const std::string annotations = fxe_string_annotations(fx, p);
        if (d3dx::is_texture_type(p.type)) {
            out.push_back(std::format("{} {}{}{};", d3dx::type_name(p.type), p.name, semantic, annotations));
            continue;
        }
        if (p.type == d3dx::ParamType::String) {
            out.push_back(std::format("string {}{} = \"{}\";", p.name, annotations, p.object() ? fx.string(*p.object()) : std::string()));
            continue;
        }
        const std::optional<std::string> t = fxe_type_name(p);
        if (!t) {
            out.push_back(std::format("// {}: a {} {} parameter, not written", p.name, d3dx::class_name(p.cls), d3dx::type_name(p.type)));
            continue;
        }
        std::string init;
        if (p.elements != 0) {
            const bool any = std::any_of(p.members.begin(), p.members.end(), [](const fxf::Parameter& m) { return fxe_any_nonzero(fxe_numbers(m)); });
            if (any) {
                std::vector<std::string> literals;
                for (const fxf::Parameter& m : p.members) literals.push_back(fxe_literal(m, fxe_numbers(m)));
                init = std::format(" = {{ {} }}", text::join(literals, ", "));
            }
        } else if (const std::vector<double> values = fxe_numbers(p); fxe_any_nonzero(values)) {
            init = " = " + fxe_literal(p, values);
        }
        out.push_back(std::format("{} {}{}{}{}{};", *t, p.name, array, semantic, annotations, init));
    }
    out.push_back("");

    // the samplers
    for (const fxf::Parameter& p : fx.parameters) {
        if (!d3dx::is_sampler_type(p.type) || p.elements != 0) continue;
        std::vector<std::string> states;
        for (const fxf::State& s : p.states) {
            if (s.name == "Texture") {
                if (const auto tex = fxe_texture_of(fx, s)) states.push_back(std::format("Texture = <{}>;", *tex));
            } else if (fxe_is_sampler_state(s.name)) {
                if (const auto text = s.text()) states.push_back(std::format("{} = {};", s.name, *text));
            }
        }
        const std::string body = states.empty() ? std::string() : std::format(" = sampler_state\n{{\n    {}\n}}", text::join(states, "\n    "));
        out.push_back(std::format("{} {}{};\n", d3dx::type_name(p.type), p.name, body));
    }
    if (std::any_of(functions.all().begin(), functions.all().end(), [](const auto& kv) { return kv.second.needs_helpers; })) {
        out.emplace_back(hlsl::kHelpers);
    }
    for (const auto& [fname, f] : functions.all()) out.push_back(f.text);

    // the techniques (one per selector value when a pass chooses its shaders from arrays)
    for (std::size_t ti = 0; ti < fx.techniques.size(); ++ti) {
        const fxf::Technique& t = fx.techniques[ti];
        std::size_t count = 1;
        if (!t.passes.empty()) {
            count = 0;
            for (const auto& pass_variants : variants[ti]) count = std::max(count, pass_variants.size());
        }
        for (std::size_t v = 0; v < count; ++v) {
            if (v == 0 && count > 1) {
                out.push_back(std::format("// {}__<n>: the shaders {} n chooses", t.name, text::join(selectors, " / ")));
            }
            out.push_back(std::format("technique {}\n{{", v == 0 ? t.name : std::format("{}__{}", t.name, v)));
            for (std::size_t i = 0; i < t.passes.size(); ++i) {
                const fxf::Pass& p = t.passes[i];
                const auto& pass_variants = variants[ti][i];
                const FxeVariant& var = pass_variants[std::min(v, pass_variants.size() - 1)];
                out.push_back(std::format("    pass {}\n    {{", p.name.empty() ? std::format("P{}", i) : p.name));
                for (const fxf::State& s : p.states) {
                    if (s.name == "VertexShader") {
                        if (var.vs) out.push_back(std::format("        VertexShader = compile vs_3_0 {}();", *var.vs));
                    } else if (s.name == "PixelShader") {
                        if (var.ps) out.push_back(std::format("        PixelShader = compile ps_3_0 {}();", *var.ps));
                    } else if (const auto text = s.text()) {
                        if (fxe_is_pass_state(s.name)) {
                            out.push_back(std::format("        {} = {};", s.name, *text));
                        } else {
                            out.push_back(std::format("        // {} = {};   (the Xbox's; the PC has no such state)", s.name, *text));
                        }
                    }
                }
                out.push_back("    }");
            }
            out.push_back("}\n");
        }
    }
    return text::join(out, "\n");
}

}  // namespace cb::decompile::xbox_effect
