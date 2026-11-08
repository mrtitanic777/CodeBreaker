// codebreaker/decompile/xbox_effect.cpp -- typed C++ port of codebreaker.decompile.xbox_effect (see xbox_effect.hpp).

#include "codebreaker/decompile/xbox_effect.hpp"

#include <array>
#include <cmath>
#include <map>
#include <variant>
#include <vector>

namespace codebreaker::decompile::xbox_effect {

namespace shformat = codebreaker::formats::xbox::shader;

const py::set<py::str> PASS_STATES = {
    py::str("AlphaBlendEnable"), py::str("SrcBlend"), py::str("DestBlend"), py::str("BlendOp"),
    py::str("SeparateAlphaBlendEnable"), py::str("SrcBlendAlpha"), py::str("DestBlendAlpha"), py::str("BlendOpAlpha"),
    py::str("BlendFactor"), py::str("ColorWriteEnable"), py::str("ColorWriteEnable1"), py::str("ColorWriteEnable2"),
    py::str("ColorWriteEnable3"), py::str("ZEnable"), py::str("ZWriteEnable"), py::str("ZFunc"), py::str("DepthBias"),
    py::str("SlopeScaleDepthBias"), py::str("StencilEnable"), py::str("StencilFail"), py::str("StencilZFail"),
    py::str("StencilPass"), py::str("StencilFunc"), py::str("StencilRef"), py::str("StencilMask"),
    py::str("StencilWriteMask"), py::str("TwoSidedStencilMode"), py::str("CCW_StencilFail"),
    py::str("CCW_StencilZFail"), py::str("CCW_StencilPass"), py::str("CCW_StencilFunc"), py::str("CullMode"),
    py::str("FillMode"), py::str("MultiSampleAntiAlias"), py::str("ScissorTestEnable")};
const py::set<py::str> SAMPLER_STATES = {
    py::str("AddressU"), py::str("AddressV"), py::str("AddressW"), py::str("BorderColor"), py::str("MagFilter"),
    py::str("MinFilter"), py::str("MipFilter"), py::str("MipMapLodBias"), py::str("MaxMipLevel"), py::str("MaxAnisotropy")};

namespace {

using fxformat::Effect;
using fxformat::Parameter;
using fxformat::State;
using fxformat::Value;

// One number of a parameter's value: the effect reader gives floats for a float parameter, ints for the rest.
using Num = std::variant<int64_t, double>;

// The numbers of a value that is a list of them; TypeError for any other kind of value (see the header).
std::vector<Num> numbers_of(const Value& v) {
    std::vector<Num> out;
    if (const auto* is = std::get_if<py::list<int64_t>>(&v.data)) {
        for (int64_t x : *is) out.push_back(x);
    } else if (const auto* ds = std::get_if<py::list<double>>(&v.data)) {
        for (double x : *ds) out.push_back(x);
    } else if (const auto* ss = std::get_if<py::list<State>>(&v.data)) {
        if (!ss->empty()) throw py::TypeError("a list of states is not a list of numbers");
    } else if (const auto* vs = std::get_if<py::list<Value>>(&v.data)) {
        if (!vs->empty()) throw py::TypeError("a list of lists is not a list of numbers");
    } else {
        throw py::TypeError("a parameter's value is not a list of numbers");
    }
    return out;
}

// the `if v` of a list value: a non-empty list
bool is_nonempty_list(const Value& v) {
    if (const auto* is = std::get_if<py::list<int64_t>>(&v.data)) return !is->empty();
    if (const auto* ds = std::get_if<py::list<double>>(&v.data)) return !ds->empty();
    if (const auto* ss = std::get_if<py::list<State>>(&v.data)) return !ss->empty();
    if (const auto* vs = std::get_if<py::list<Value>>(&v.data)) return !vs->empty();
    return false;
}

bool is_list(const Value& v) {
    return std::holds_alternative<py::list<int64_t>>(v.data) || std::holds_alternative<py::list<double>>(v.data) ||
           std::holds_alternative<py::list<State>>(v.data) || std::holds_alternative<py::list<Value>>(v.data);
}

bool is_none(const Value& v) { return std::holds_alternative<std::monostate>(v.data); }

// any(values): some number is not zero
bool any_nonzero(const Value& v) {
    for (const Num& n : numbers_of(v)) {
        if (const auto* i = std::get_if<int64_t>(&n)) {
            if (*i != 0) return true;
        } else if (std::get<double>(n) != 0.0) {
            return true;
        }
    }
    return false;
}

// Python's `a == b` for two parameter values (a list of ints equals the list of the same floats; lists of
// states or of lists only when both are empty, as the objects they hold are never the same ones here).
bool values_equal(const Value& a, const Value& b) {
    if (is_none(a) || is_none(b)) return is_none(a) && is_none(b);
    const int64_t* ia = std::get_if<int64_t>(&a.data);
    const int64_t* ib = std::get_if<int64_t>(&b.data);
    if (ia || ib) return ia && ib && *ia == *ib;
    if (!is_list(a) || !is_list(b)) return false;
    std::vector<Num> na, nb;
    try {
        na = numbers_of(a);
        nb = numbers_of(b);
    } catch (const py::TypeError&) {
        return false;
    }
    if (na.size() != nb.size()) return false;
    for (size_t i = 0; i < na.size(); i++) {
        double x = std::holds_alternative<int64_t>(na[i]) ? static_cast<double>(std::get<int64_t>(na[i])) : std::get<double>(na[i]);
        double y = std::holds_alternative<int64_t>(nb[i]) ? static_cast<double>(std::get<int64_t>(nb[i])) : std::get<double>(nb[i]);
        if (x != y) return false;
    }
    return true;
}

// the `.split(b"\0", 1)[0].decode("latin-1")` of bytes
py::str up_to_nul(const py::bytes& b) {
    int64_t nul = b.find(0);
    return (nul < 0 ? b : b.slice_to(nul)).decode("latin-1");
}

// "".join(c if c.isalnum() or c == "_" else "_" for c in name)
py::str sanitized(const py::str& name) {
    std::u32string out;
    for (char32_t c : name.u32()) {
        out.push_back(py::str(std::u32string(1, c)).isalnum() || c == U'_' ? c : U'_');
    }
    return py::str(out);
}

}  // namespace

std::optional<py::str> _type_name(const Parameter& p) {
    py::str base;
    if (p.type == py::str("float")) base = py::str("float");
    else if (p.type == py::str("int")) base = py::str("int");
    else if (p.type == py::str("bool")) base = py::str("bool");
    else return std::nullopt;
    if (p.cls == py::str("scalar")) {
        return base;
    }
    if (p.cls == py::str("vector")) {
        return py::percent("%s%d", base, p.columns);
    }
    if (p.cls == py::str("matrix_rows") || p.cls == py::str("matrix_columns")) {
        // declared without row_major (MonoGame's compiler takes no such parameter): the shaders read a matrix's
        // elements by row and column (_m01), which mean the same whichever way it is stored
        return py::percent("%s%dx%d", base, p.rows, p.columns);
    }
    return std::nullopt;
}

py::str _literal(const Parameter& p, const Value& values_in) {
    // def one(v)
    auto one = [&p](const Num& v) -> py::str {
        if (p.type == py::str("bool")) {
            bool truth = std::holds_alternative<int64_t>(v) ? std::get<int64_t>(v) != 0 : std::get<double>(v) != 0.0;
            return truth ? py::str("true") : py::str("false");
        }
        if (p.type == py::str("int")) {
            if (const auto* i = std::get_if<int64_t>(&v)) return py::str(std::to_string(*i));
            double d = std::get<double>(v);
            if (std::isnan(d)) throw py::ValueError("cannot convert float NaN to integer");
            if (std::isinf(d)) throw py::OverflowError("cannot convert float infinity to integer");
            return py::str(std::to_string(static_cast<int64_t>(d)));
        }
        double d = std::holds_alternative<int64_t>(v) ? static_cast<double>(std::get<int64_t>(v)) : std::get<double>(v);
        return xenos_hlsl::_num(d);
    };
    std::vector<Num> values = numbers_of(values_in);
    auto at = [&values](int64_t i) -> const Num& {
        int64_t n = static_cast<int64_t>(values.size());
        if (i < -n || i >= n) throw py::IndexError("list index out of range");
        return values[static_cast<size_t>(i < 0 ? i + n : i)];
    };
    int64_t n = p.rows * p.columns;
    if (p.cls == py::str("scalar")) {
        return one(at(0));
    }
    std::optional<py::str> t = _type_name(p);
    if (p.cls == py::str("matrix_columns")) {            // stored a column at a time; the initializer is by rows
        std::vector<Num> by_rows;
        for (int64_t r = 0; r < p.rows; r++) {
            for (int64_t c = 0; c < p.columns; c++) {
                by_rows.push_back(at(c * p.rows + r));
            }
        }
        values = by_rows;
    }
    py::list<py::str> parts;
    for (int64_t i = 0; i < n && i < static_cast<int64_t>(values.size()); i++) {
        parts.append(one(values[static_cast<size_t>(i)]));
    }
    return py::percent("%s(%s)", t ? *t : py::str("None"), py::join(py::str(", "), parts));
}

py::str _string_annotations(const Effect& fx, const Parameter& p) {
    py::list<py::str> parts;
    for (const Parameter& a : p.annotations) {
        if (a.type == py::str("string") && !is_none(a.value)) {
            const int64_t* id = std::get_if<int64_t>(&a.value.data);
            if (id == nullptr) throw py::TypeError("a string annotation's value is not an object id");
            parts.append(py::percent("string %s = \"%s\";", a.name, fx.string(*id).replace(py::str("\""), py::str("'"))));
        } else if (_type_name(a) && is_list(a.value)) {
            parts.append(py::percent("%s %s = %s;", *_type_name(a), a.name, _literal(a, a.value)));
        }
    }
    return py::truthy(parts) ? py::percent(" <%s>", py::join(py::str(" "), parts)) : py::str("");
}

std::optional<py::str> _texture_of(const Effect& fx, const State& state) {
    if (state.parameter && state.parameter->reference && py::truthy(*state.parameter->reference)) {
        return *state.parameter->reference;
    }
    Value state_value = state.value();
    for (const Parameter& p : fx.parameters) {
        if (p.type.startswith(py::str("texture")) && values_equal(p.value, state_value)) {
            return p.name;
        }
    }
    return std::nullopt;
}

std::optional<std::tuple<py::list<int64_t>, py::str>> _array_selection(const Effect& fx, const State& state) {
    if (!state.parameter || !state.parameter->expression || !py::truthy(*state.parameter->expression)) {
        return std::nullopt;
    }
    const py::bytes& e = *state.parameter->expression;
    int64_t n = py::unpack_from1<int64_t>(fx.big_endian ? ">I" : "<I", e, 0);
    std::optional<Parameter> array = fx.parameter(up_to_nul(e.slice(4, 4 + n)));
    if (!array || array->elements == 0) {
        return std::nullopt;
    }
    py::bytes rest = e.slice_from(4 + n);
    py::list<Parameter> named;
    for (const Parameter& p : fx.parameters) {
        if (p.type == py::str("int") && rest.contains(p.name.encode() + py::bytes(std::string(1, '\0')))) {
            named.append(p);
        }
    }
    std::optional<Parameter> selector, table;
    for (const Parameter& p : named) {
        if (p.elements == 0 && p.cls == py::str("scalar")) {
            selector = p;
            break;
        }
    }
    for (const Parameter& p : named) {
        if (p.elements != 0) {
            table = p;
            break;
        }
    }
    if (!selector) {
        return std::nullopt;
    }
    // the shader array's values: object ids
    const py::list<Value>* array_values = std::get_if<py::list<Value>>(&array->value.data);
    auto array_at = [&](int64_t i) -> int64_t {
        if (array_values == nullptr) throw py::TypeError("a shader array's value is not a list");
        int64_t len = py::len(*array_values);
        if (i < -len || i >= len) throw py::IndexError("list index out of range");
        const int64_t* id = std::get_if<int64_t>(&(*array_values)[i].data);
        if (id == nullptr) throw py::TypeError("a shader array's element is not an object id");
        return *id;
    };
    py::list<int64_t> indices;
    if (table) {
        const py::list<Value>* rows = std::get_if<py::list<Value>>(&table->value.data);
        if (rows == nullptr) throw py::TypeError("an index table's value is not a list");
        for (const Value& v : *rows) {
            const py::list<int64_t>* first = std::get_if<py::list<int64_t>>(&v.data);
            if (first == nullptr) throw py::TypeError("an index table's element is not a list of ints");
            if (first->empty()) throw py::IndexError("list index out of range");
            indices.append((*first)[0]);
        }
    } else {
        for (int64_t i = 0; i < array->elements; i++) indices.append(i);
    }
    py::list<int64_t> ids;
    for (int64_t i : indices) {
        ids.append(array_at(i));
    }
    return std::make_tuple(ids, selector->name);
}

py::str to_fx(const py::bytes& data, const py::str& name) {
    Effect fx = fxformat::read(data);
    py::list<py::str> out;
    out.append(py::percent("// %s: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect.py) from its microcode.", name));
    out.append(py::str("// The shaders read as the compiled programs did: registers r0..rN, the constants by their names."));
    out.append(py::str(""));
    // the shaders, by the passes using them: each pass as one or more variants (vertex, pixel function names)
    py::dict<py::str, xenos_hlsl::Function> functions;
    py::dict<py::bytes, py::str> names;

    auto function = [&](const py::bytes& blob, const py::str& fname_in) -> py::str {
        if (!names.contains(blob)) {
            py::str fname = sanitized(fname_in);
            names.set(blob, fname);
            functions.set(fname, xenos_hlsl::translate(shformat::read(blob), fname));
        }
        return names.at(blob);
    };

    using Variant = std::tuple<std::optional<py::str>, std::optional<py::str>>;
    using Key = std::tuple<py::str, int64_t>;
    py::dict<Key, py::list<Variant>> variants;
    py::set<py::str> selectors;
    const std::array<std::pair<const char*, const char*>, 2> kinds = {{{"VertexShader", "VS"}, {"PixelShader", "PS"}}};
    for (const fxformat::Technique& t : fx.techniques) {
        int64_t i = -1;
        for (const fxformat::Pass& p : t.passes) {
            i++;
            std::array<std::optional<py::list<py::str>>, 2> choices;
            for (size_t k = 0; k < kinds.size(); k++) {
                py::str kind(kinds[k].first), prefix(kinds[k].second);
                std::optional<State> s = p.state(kind);
                if (!s) {
                    continue;
                }
                auto chosen = _array_selection(fx, *s);
                if (chosen) {
                    const py::list<int64_t>& ids = std::get<0>(*chosen);
                    selectors.add(std::get<1>(*chosen));
                    py::str array = up_to_nul(s->parameter->expression->slice_from(4));
                    std::optional<Parameter> array_param = fx.parameter(array);
                    if (!array_param) throw py::AttributeError("'NoneType' object has no attribute 'value'");
                    const py::list<Value>* array_values = std::get_if<py::list<Value>>(&array_param->value.data);
                    if (array_values == nullptr) throw py::TypeError("a shader array's value is not a list");
                    py::dict<int64_t, int64_t> element;
                    int64_t kk = 0;
                    for (const Value& v : *array_values) {
                        const int64_t* oid = std::get_if<int64_t>(&v.data);
                        if (oid == nullptr) throw py::TypeError("a shader array's element is not an object id");
                        element.set(*oid, kk++);
                    }
                    py::list<py::str> picked;
                    for (int64_t oid : ids) {
                        py::bytes blob = fx.objects.at(oid);
                        picked.append(function(blob, py::percent("%s_%d", array, element.at(oid))));
                    }
                    choices[k] = picked;
                } else {
                    Value sv = s->value();
                    if (is_list(sv)) throw py::TypeError("unhashable type: 'list'");
                    const int64_t* oid = std::get_if<int64_t>(&sv.data);
                    if (oid != nullptr && fx.objects.contains(*oid) && py::truthy(fx.objects.at(*oid))) {
                        py::str pname = py::truthy(p.name) ? p.name : py::percent("P%d", i);
                        choices[k] = py::list<py::str>{function(fx.objects.at(*oid), py::percent("%s_%s_%s", prefix, t.name, pname))};
                    }
                }
            }
            int64_t count = 1;
            bool any_choice = false;
            for (const auto& c : choices) {
                if (c) {
                    int64_t len = py::len(*c);
                    count = any_choice ? py::max(count, len) : len;
                    any_choice = true;
                }
            }
            py::list<Variant> made;
            for (int64_t v = 0; v < count; v++) {
                std::array<std::optional<py::str>, 2> picked;
                for (size_t k = 0; k < 2; k++) {
                    if (choices[k]) {
                        const py::list<py::str>& c = *choices[k];
                        int64_t at = py::len(c) > 1 ? v : 0;
                        if (at >= py::len(c)) throw py::IndexError("list index out of range");
                        picked[k] = c[at];
                    }
                }
                made.append(Variant{picked[0], picked[1]});
            }
            variants.set(Key{t.name, i}, made);
        }
    }
    // the parameters
    for (const Parameter& p : fx.parameters) {
        if (fxformat::SAMPLER_TYPES.contains(p.type) || p.type == py::str("vertexshader") || p.type == py::str("pixelshader")) {
            continue;
        }
        py::str semantic = py::truthy(p.semantic) ? py::percent(" : %s", p.semantic) : py::str("");
        py::str array = p.elements ? py::percent("[%d]", p.elements) : py::str("");
        py::str annotations = _string_annotations(fx, p);
        if (p.type.startswith(py::str("texture"))) {
            out.append(py::percent("%s %s%s%s;", p.type, p.name, semantic, annotations));
            continue;
        }
        if (p.type == py::str("string")) {
            py::str text("");
            if (!is_none(p.value)) {
                const int64_t* id = std::get_if<int64_t>(&p.value.data);
                if (id == nullptr) throw py::TypeError("a string parameter's value is not an object id");
                text = fx.string(*id);
            }
            out.append(py::percent("string %s%s = \"%s\";", p.name, annotations, text));
            continue;
        }
        std::optional<py::str> t = _type_name(p);
        if (!t) {
            out.append(py::percent("// %s: a %s %s parameter, not written", p.name, p.cls, p.type));
            continue;
        }
        py::str init("");
        if (p.elements) {
            bool some = false;
            for (const Parameter& m : p.members) {
                // any(any(v) for v in values if v)
                if (is_nonempty_list(m.value) && any_nonzero(m.value)) {
                    some = true;
                    break;
                }
            }
            if (some) {
                py::list<py::str> items;
                for (const Parameter& m : p.members) {
                    items.append(_literal(m, m.value));
                }
                init = py::percent(" = { %s }", py::join(py::str(", "), items));
            }
        } else if (is_nonempty_list(p.value) && any_nonzero(p.value)) {
            init = py::percent(" = %s", _literal(p, p.value));
        }
        out.append(py::percent("%s %s%s%s%s%s;", *t, p.name, array, semantic, annotations, init));
    }
    out.append(py::str(""));
    // the samplers
    for (const Parameter& p : fx.parameters) {
        if (!fxformat::SAMPLER_TYPES.contains(p.type) || p.elements) {
            continue;
        }
        py::str kind;
        if (p.type == py::str("sampler")) kind = py::str("sampler");
        else if (p.type == py::str("sampler1D")) kind = py::str("sampler1D");
        else if (p.type == py::str("sampler2D")) kind = py::str("sampler2D");
        else if (p.type == py::str("sampler3D")) kind = py::str("sampler3D");
        else if (p.type == py::str("samplerCUBE")) kind = py::str("samplerCUBE");
        else throw py::KeyError(p.type);
        py::list<py::str> states;
        if (const auto* ss = std::get_if<py::list<State>>(&p.value.data)) {
            for (const State& s : *ss) {
                std::optional<py::str> text = s.text();
                if (s.name == py::str("Texture")) {
                    std::optional<py::str> tex = _texture_of(fx, s);
                    if (tex && py::truthy(*tex)) {
                        states.append(py::percent("Texture = <%s>;", *tex));
                    }
                } else if (SAMPLER_STATES.contains(s.name) && text) {
                    states.append(py::percent("%s = %s;", s.name, *text));
                }
            }
        } else if (is_list(p.value) && is_nonempty_list(p.value)) {
            throw py::AttributeError("a sampler's state has no name");     // (a list of numbers or lists, not of States)
        } else if (const int64_t* id = std::get_if<int64_t>(&p.value.data)) {
            if (*id != 0) throw py::TypeError("'int' object is not iterable");
        }
        py::str body = py::truthy(states)
                           ? py::percent(" = sampler_state\n{\n    %s\n}", py::join(py::str("\n    "), states))
                           : py::str("");
        out.append(py::percent("%s %s%s;\n", kind, p.name, body));
    }
    bool needs_helpers = false;
    for (const py::str& k : functions.keys()) {
        if (functions.at(k).needs_helpers) needs_helpers = true;
    }
    if (needs_helpers) {
        out.append(xenos_hlsl::HELPERS);
    }
    for (const py::str& k : functions.keys()) {
        out.append(functions.at(k).text);
    }
    // the techniques (one per selector value when a pass chooses its shaders from arrays)
    for (const fxformat::Technique& t : fx.techniques) {
        int64_t count = 1;
        if (!t.passes.empty()) {
            count = 0;
            for (int64_t i = 0; i < py::len(t.passes); i++) {
                count = py::max(count, py::len(variants.at(Key{t.name, i})));
            }
        }
        for (int64_t v = 0; v < count; v++) {
            if (v == 0 && count > 1) {
                out.append(py::percent("// %s__<n>: the shaders %s n chooses", t.name,
                                       py::join(py::str(" / "), py::sorted(selectors))));
            }
            out.append(py::percent("technique %s\n{", v == 0 ? t.name : py::percent("%s__%d", t.name, v)));
            int64_t i = -1;
            for (const fxformat::Pass& p : t.passes) {
                i++;
                const py::list<Variant>& vs_list = variants.at(Key{t.name, i});
                const Variant& pick = vs_list[py::min(v, py::len(vs_list) - 1)];
                const std::optional<py::str>& vs = std::get<0>(pick);
                const std::optional<py::str>& ps = std::get<1>(pick);
                out.append(py::percent("    pass %s\n    {", py::truthy(p.name) ? p.name : py::percent("P%d", i)));
                for (const State& s : p.states) {
                    std::optional<py::str> text = s.text();
                    if (s.name == py::str("VertexShader")) {
                        if (vs && py::truthy(*vs)) {
                            out.append(py::percent("        VertexShader = compile vs_3_0 %s();", *vs));
                        }
                    } else if (s.name == py::str("PixelShader")) {
                        if (ps && py::truthy(*ps)) {
                            out.append(py::percent("        PixelShader = compile ps_3_0 %s();", *ps));
                        }
                    } else if (!text) {
                        continue;
                    } else if (PASS_STATES.contains(s.name)) {
                        out.append(py::percent("        %s = %s;", s.name, *text));
                    } else {
                        out.append(py::percent("        // %s = %s;   (the Xbox's; the PC has no such state)", s.name, *text));
                    }
                }
                out.append(py::str("    }"));
            }
            out.append(py::str("}\n"));
        }
    }
    return py::join(py::str("\n"), out);
}

}  // namespace codebreaker::decompile::xbox_effect
