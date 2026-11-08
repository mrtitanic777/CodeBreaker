// codebreaker/formats/xbox/effect.cpp -- typed C++ from codebreaker.formats.xbox.effect (see effect.hpp).
#include "codebreaker/formats/xbox/effect.hpp"

#include <cmath>
#include <cstring>

namespace codebreaker::formats::xbox::effect {

namespace {

py::list<py::str> make_types() {
    py::list<py::str> t;
    for (const char* s : {"void", "bool", "int", "float", "string", "texture", "texture1D", "texture2D",
                          "texture3D", "textureCUBE", "sampler", "sampler1D", "sampler2D", "sampler3D",
                          "samplerCUBE", "pixelshader", "vertexshader"})
        t.append(py::str(s));
    return t;
}

py::list<py::str> make_classes() {
    py::list<py::str> t;
    for (const char* s : {"scalar", "vector", "matrix_rows", "matrix_columns", "object", "struct"}) t.append(py::str(s));
    return t;
}

py::dict<int64_t, py::str> make_states() {
    py::list<py::str> names;
    for (const char* s : {"ZEnable", "FillMode", "ZWriteEnable", "AlphaTestEnable", "SrcBlend", "DestBlend",
                          "CullMode", "ZFunc", "AlphaRef", "AlphaFunc", "AlphaBlendEnable", "StencilEnable",
                          "StencilFail", "StencilZFail", "StencilPass", "StencilFunc", "StencilRef", "StencilMask",
                          "StencilWriteMask"})
        names.append(py::str(s));
    for (int64_t i = 0; i < 16; ++i) names.append(py::percent(py::str("Wrap%d"), i));
    for (const char* s : {"ClipPlaneEnable", "PointSize", "PointSize_Min", "PointSize_Max", "PointSpriteEnable",
                          "MultiSampleAntiAlias", "MultiSampleMask", "ColorWriteEnable", "BlendOp",
                          "ScissorTestEnable", "SlopeScaleDepthBias", "MinTessellationLevel",
                          "MaxTessellationLevel", "TwoSidedStencilMode", "CCW_StencilFail", "CCW_StencilZFail",
                          "CCW_StencilPass", "CCW_StencilFunc"})
        names.append(py::str(s));
    py::dict<int64_t, py::str> d;
    int64_t i = 0;
    for (const py::str& n : names) d.set(i++, n);
    const std::pair<int64_t, const char*> more[] = {
        {56, "ColorWriteEnable1"}, {57, "ColorWriteEnable2"}, {58, "ColorWriteEnable3"}, {59, "BlendFactor"},
        {60, "DepthBias"}, {61, "SeparateAlphaBlendEnable"}, {62, "SrcBlendAlpha"}, {63, "DestBlendAlpha"},
        {64, "BlendOpAlpha"}, {88, "LastPixel"}, {91, "FVF"}, {92, "VertexShader"}, {93, "PixelShader"},
        {110, "Texture"}, {111, "AddressU"}, {112, "AddressV"}, {113, "AddressW"}, {114, "BorderColor"},
        {115, "MagFilter"}, {116, "MinFilter"}, {117, "MipFilter"}, {118, "MipMapLodBias"}, {119, "MaxMipLevel"},
        {120, "MaxAnisotropy"}};
    for (const auto& kv : more) d.set(kv.first, py::str(kv.second));
    return d;
}

using IntNames = py::dict<int64_t, py::str>;

IntNames names_of(std::initializer_list<std::pair<int64_t, const char*>> xs) {
    IntNames d;
    for (const auto& kv : xs) d.set(kv.first, py::str(kv.second));
    return d;
}

py::dict<py::str, IntNames> make_values() {
    // 14/15: the Xbox's constant alpha
    const IntNames blend = names_of({{0, "Zero"}, {1, "One"}, {4, "SrcColor"}, {5, "InvSrcColor"}, {6, "SrcAlpha"},
                                     {7, "InvSrcAlpha"}, {8, "DestColor"}, {9, "InvDestColor"}, {10, "DestAlpha"},
                                     {11, "InvDestAlpha"}, {12, "BlendFactor"}, {13, "InvBlendFactor"},
                                     {14, "BlendFactor"}, {15, "InvBlendFactor"}, {16, "SrcAlphaSat"}});
    const IntNames cmp = names_of({{0, "Never"}, {1, "Less"}, {2, "Equal"}, {3, "LessEqual"}, {4, "Greater"},
                                   {5, "NotEqual"}, {6, "GreaterEqual"}, {7, "Always"}});
    const IntNames stencil = names_of({{0, "Keep"}, {1, "Zero"}, {2, "Replace"}, {3, "IncrSat"}, {4, "DecrSat"},
                                       {5, "Invert"}, {6, "Incr"}, {7, "Decr"}});
    const IntNames address = names_of({{0, "Wrap"}, {1, "Mirror"}, {2, "Clamp"}, {3, "MirrorOnce"}, {4, "Border"},
                                       {5, "MirrorOnce"}, {6, "Border"}, {7, "MirrorOnce"}});
    const IntNames filter = names_of({{0, "Point"}, {1, "Linear"}, {2, "None"}, {4, "Anisotropic"}});
    py::dict<py::str, IntNames> v;
    v.set(py::str("SrcBlend"), blend);
    v.set(py::str("DestBlend"), blend);
    v.set(py::str("SrcBlendAlpha"), blend);
    v.set(py::str("DestBlendAlpha"), blend);
    v.set(py::str("BlendOp"), names_of({{0, "Add"}, {1, "Subtract"}, {2, "Min"}, {3, "Max"}, {4, "RevSubtract"}}));
    v.set(py::str("CullMode"), names_of({{0, "None"}, {2, "CW"}, {6, "CCW"}}));
    v.set(py::str("FillMode"), names_of({{0, "Solid"}, {1, "Point"}, {0x25, "WireFrame"}}));
    v.set(py::str("ZFunc"), cmp);
    v.set(py::str("AlphaFunc"), cmp);
    v.set(py::str("StencilFunc"), cmp);
    v.set(py::str("CCW_StencilFunc"), cmp);
    v.set(py::str("StencilFail"), stencil);
    v.set(py::str("StencilZFail"), stencil);
    v.set(py::str("StencilPass"), stencil);
    v.set(py::str("CCW_StencilFail"), stencil);
    v.set(py::str("CCW_StencilZFail"), stencil);
    v.set(py::str("CCW_StencilPass"), stencil);
    v.set(py::str("AddressU"), address);
    v.set(py::str("AddressV"), address);
    v.set(py::str("AddressW"), address);
    v.set(py::str("MagFilter"), filter);
    v.set(py::str("MinFilter"), filter);
    v.set(py::str("MipFilter"), filter);
    return v;
}

// repr() of a list of already-formatted items
py::str list_repr(const std::vector<py::str>& items) {
    py::str out("[");
    for (size_t i = 0; i < items.size(); ++i) {
        if (i) out += py::str(", ");
        out += items[i];
    }
    return out + py::str("]");
}

// len() of a value that is a list (0 for None and for an id: callers know those are not lists)
int64_t list_len(const Value& v) {
    return std::visit(
        [](const auto& x) -> int64_t {
            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, std::monostate> ||
                          std::is_same_v<std::decay_t<decltype(x)>, int64_t>)
                return 0;
            else
                return x.size();
        },
        v.data);
}

}  // namespace

const py::list<py::str> TYPES = make_types();
const py::list<py::str> CLASSES = make_classes();
const py::set<py::str> OBJECT_TYPES = {py::str("string"), py::str("texture"), py::str("texture1D"),
                                       py::str("texture2D"), py::str("texture3D"), py::str("textureCUBE"),
                                       py::str("pixelshader"), py::str("vertexshader")};
const py::set<py::str> SAMPLER_TYPES = {py::str("sampler"), py::str("sampler1D"), py::str("sampler2D"),
                                        py::str("sampler3D"), py::str("samplerCUBE")};
const py::dict<int64_t, py::str> STATES = make_states();
const py::dict<py::str, py::dict<int64_t, py::str>> VALUES = make_values();

py::str repr_value(const Value& v) {
    struct Visitor {
        py::str operator()(std::monostate) const { return py::str("None"); }
        py::str operator()(const py::list<int64_t>& xs) const {
            std::vector<py::str> out;
            for (int64_t x : xs) out.push_back(py::repr(x));
            return list_repr(out);
        }
        py::str operator()(const py::list<double>& xs) const {
            std::vector<py::str> out;
            for (double x : xs) out.push_back(py::repr(x));
            return list_repr(out);
        }
        py::str operator()(int64_t x) const { return py::repr(x); }
        py::str operator()(const py::list<State>& xs) const {
            std::vector<py::str> out;
            for (const State& x : xs) out.push_back(x.__repr__());
            return list_repr(out);
        }
        py::str operator()(const py::list<Value>& xs) const {
            std::vector<py::str> out;
            for (const Value& x : xs) out.push_back(repr_value(x));
            return list_repr(out);
        }
    };
    return std::visit(Visitor{}, v.data);
}

int64_t Parameter::size() const {
    if (elements) {
        int64_t n = 0;
        for (const Parameter& m : members) n += m.size();
        return n;
    }
    if (cls == "struct") {
        int64_t n = 0;
        for (const Parameter& m : members) n += m.size();
        return n;
    }
    if (cls == "scalar" || cls == "vector" || cls == "matrix_rows" || cls == "matrix_columns") return 4 * rows * columns;
    return 0;
}

py::str Parameter::__repr__() const { return py::percent(py::str("<%s %s %s>"), type, cls, name); }

State::State(int64_t operation_, int64_t index_, std::shared_ptr<Parameter> parameter_)
    : operation(operation_), index(index_), parameter(std::move(parameter_)) {
    name = STATES.get(operation, py::percent(py::str("state_%d"), operation));
}

Value State::value() const { return parameter->value; }

std::optional<py::str> State::text() const {
    const Value& pv = parameter->value;
    // v is Python's `v[0]` of a one-item list value: its kind decides what the tests below do with it.
    enum class Kind { Int, Float, Object, List, None } kind;
    int64_t i = 0;
    double d = 0;
    py::str shown;     // str(v) / repr(v) for the kinds that are not numbers
    bool truth = false;
    if (auto* xs = std::get_if<py::list<int64_t>>(&pv.data)) {
        if (xs->size() != 1) return std::nullopt;
        kind = Kind::Int;
        i = (*xs)[0];
    } else if (auto* fs = std::get_if<py::list<double>>(&pv.data)) {
        if (fs->size() != 1) return std::nullopt;
        kind = Kind::Float;
        d = (*fs)[0];
    } else if (auto* ss = std::get_if<py::list<State>>(&pv.data)) {
        if (ss->size() != 1) return std::nullopt;
        kind = Kind::Object;
        shown = (*ss)[0].__repr__();
        truth = true;
    } else if (auto* as = std::get_if<py::list<Value>>(&pv.data)) {
        if (as->size() != 1) return std::nullopt;
        const Value& e = (*as)[0];
        shown = repr_value(e);
        if (auto* ei = std::get_if<int64_t>(&e.data)) { kind = Kind::Int; i = *ei; }
        else if (std::holds_alternative<std::monostate>(e.data)) { kind = Kind::None; }
        else {
            kind = Kind::List;
            truth = list_len(e) != 0;
        }
    } else {
        return std::nullopt;   // None, or an object id: not a list
    }
    const py::str& n = name;
    if (VALUES.contains(n)) {
        const py::dict<int64_t, py::str>& table = VALUES.at(n);
        switch (kind) {
            case Kind::Int: return table.get(i, py::to_str(i));
            case Kind::Float:
                // dict.get(float): an integral float finds the int key that equals it
                if (std::isfinite(d) && d == std::floor(d) && std::fabs(d) < 9.2e18) {
                    int64_t k = static_cast<int64_t>(d);
                    return table.get(k, py::to_str(d));
                }
                return py::to_str(d);
            case Kind::Object: return shown;
            case Kind::None: return py::str("None");
            case Kind::List: throw py::TypeError("unhashable type: 'list'");
        }
    }
    if (parameter->type == "bool" || n.endswith(py::str("Enable")) || n == "TwoSidedStencilMode") {
        bool t = false;
        switch (kind) {
            case Kind::Int: t = i != 0; break;
            case Kind::Float: t = d != 0.0; break;
            case Kind::Object: case Kind::List: t = truth; break;
            case Kind::None: t = false; break;
        }
        return py::str(t ? "true" : "false");
    }
    switch (kind) {
        case Kind::Float: return py::repr(d);
        case Kind::Int: return py::to_str(i);
        case Kind::None: return py::str("None");
        default: return shown;
    }
}

py::str State::__repr__() const {
    return py::percent(py::str("<%s[%d] = %s>"), name, index, repr_value(parameter->value));
}

Pass::Pass(py::str name_, py::list<Parameter> annotations_, py::list<State> states_)
    : name(std::move(name_)), annotations(std::move(annotations_)), states(std::move(states_)) {}

std::optional<State> Pass::state(const py::str& name_) const {
    for (const State& s : states)
        if (s.name == name_) return s;
    return std::nullopt;
}

Technique::Technique(py::str name_, py::list<Parameter> annotations_, py::list<Pass> passes_)
    : name(std::move(name_)), annotations(std::move(annotations_)), passes(std::move(passes_)) {}

std::optional<Parameter> Effect::parameter(const py::str& name) const {
    for (const Parameter& p : parameters)
        if (p.name == name) return p;
    return std::nullopt;
}

py::str Effect::string(int64_t object_id) const {
    py::bytes data = objects.get(object_id, py::bytes(""));
    int64_t nul = data.find(py::bytes("\0", 1));
    return (nul < 0 ? data : data.slice(0, nul)).decode("latin-1");
}

Reader::Reader(py::bytes data_, int64_t base_, bool big_) : data(std::move(data_)), base(base_), big(big_) {}

int64_t Reader::u32(int64_t at) const { return py::unpack_from1<int64_t>(big ? ">I" : "<I", data, base + at); }

py::str Reader::name(int64_t at) const {
    int64_t n = u32(at);
    py::bytes s = data.slice(base + at + 4, base + at + 4 + n);
    int64_t nul = s.find(py::bytes("\0", 1));
    return (nul < 0 ? s : s.slice(0, nul)).decode("latin-1");
}

std::tuple<Parameter, int64_t> _typedef(const Reader& r, int64_t at, const Parameter* parent) {
    Parameter p;
    if (parent == nullptr) {
        int64_t type_id = r.u32(at);
        p.type = type_id < py::len(TYPES) ? TYPES[type_id] : py::percent(py::str("type_%d"), r.u32(at));
        p.cls = CLASSES[r.u32(at + 4)];
        p.name = r.name(r.u32(at + 8));
        p.semantic = r.name(r.u32(at + 12));
        p.elements = r.u32(at + 16);
        at += 20;
        if (p.cls == "vector") {
            p.columns = r.u32(at);
            p.rows = r.u32(at + 4);
            at += 8;
        } else if (p.cls == "scalar" || p.cls == "matrix_rows" || p.cls == "matrix_columns") {
            p.rows = r.u32(at);
            p.columns = r.u32(at + 4);
            at += 8;
        } else if (p.cls == "struct") {
            int64_t count = r.u32(at);
            at += 4;
            p.members = py::list<Parameter>();
            for (int64_t k = 0; k < count; ++k) {
                auto [m, next] = _typedef(r, at);
                at = next;
                p.members.append(m);
            }
        }
    } else {
        p.type = parent->type;
        p.cls = parent->cls;
        p.name = parent->name;
        p.semantic = parent->semantic;
        p.rows = parent->rows;
        p.columns = parent->columns;
        py::list<Parameter> copies;
        for (const Parameter& m : parent->members) copies.append(_copy(m));
        p.members = copies;
        return {p, at};
    }
    if (p.elements) {
        py::list<Parameter> element_members = p.members;
        p.members = py::list<Parameter>();
        for (int64_t k = 0; k < p.elements; ++k) {
            Parameter e = std::get<0>(_typedef(r, at, &p));
            py::list<Parameter> copies;
            for (const Parameter& m : element_members) copies.append(_copy(m));
            e.members = copies;
            p.members.append(e);
        }
    }
    return {p, at};
}

Parameter _copy(const Parameter& p) {
    Parameter q = p;   // (q.__dict__.update(p.__dict__): the lists and the value are shared)
    py::list<Parameter> copies;
    for (const Parameter& m : p.members) copies.append(_copy(m));
    q.members = copies;
    return q;
}

std::tuple<int64_t, int64_t> _value(const Reader& r, Parameter& p, int64_t at, int64_t ptr, bool numbers) {
    if (p.elements || p.cls == "struct") {
        int64_t used = 0;
        for (int64_t k = 0; k < p.members.size(); ++k) {
            auto [next, n] = _value(r, p.members.vec()[static_cast<size_t>(k)], at + used, ptr, numbers);
            ptr = next;
            used += n;
        }
        if (p.elements) {
            py::list<Value> values;
            for (const Parameter& m : p.members) values.append(m.value);
            p.value = Value(values);
        }
        return {ptr, used};
    }
    if (p.cls == "scalar" || p.cls == "vector" || p.cls == "matrix_rows" || p.cls == "matrix_columns") {
        int64_t n = p.rows * p.columns;
        py::list<int64_t> raw;
        for (int64_t i = 0; i < n; ++i) raw.append(r.u32(at + 4 * i));
        if (p.type == "float") {
            py::list<double> floats;
            for (int64_t w : raw) {
                uint32_t bits = static_cast<uint32_t>(w);
                float f;
                std::memcpy(&f, &bits, sizeof f);   // struct.unpack("<f", struct.pack("<I", w))
                floats.append(static_cast<double>(f));
            }
            p.value = Value(floats);
        } else {
            py::list<int64_t> ints;
            for (int64_t w : raw) ints.append(w < 0x80000000 ? w : w - (int64_t(1) << 32));
            p.value = Value(ints);
        }
        return {ptr, 4 * n};
    }
    if (OBJECT_TYPES.contains(p.type)) {
        p.value = Value(r.u32(ptr));   // the object's id
        return {ptr + 4, 0};
    }
    if (SAMPLER_TYPES.contains(p.type)) {
        int64_t count = r.u32(ptr);
        ptr += 4;
        py::list<State> states;
        for (int64_t k = 0; k < count; ++k) {
            auto [s, next] = _state(r, ptr);
            ptr = next;
            states.append(s);
        }
        p.value = Value(states);
        return {ptr, 0};
    }
    return {ptr, 0};
}

std::tuple<State, int64_t> _state(const Reader& r, int64_t ptr) {
    int64_t operation = r.u32(ptr);
    int64_t index = r.u32(ptr + 4);
    auto p = std::make_shared<Parameter>(std::get<0>(_typedef(r, r.u32(ptr + 8))));
    _value(r, *p, r.u32(ptr + 12), r.u32(ptr + 12), true);
    if (OBJECT_TYPES.contains(p->type))   // an object's value is its id, at the value offset
        p->value = Value(r.u32(r.u32(ptr + 12)));
    return {State(operation, index, p), ptr + 16};
}

std::tuple<py::list<Parameter>, int64_t> _annotations(const Reader& r, int64_t ptr, int64_t count) {
    py::list<Parameter> out;
    for (int64_t k = 0; k < count; ++k) {
        Parameter p = std::get<0>(_typedef(r, r.u32(ptr)));
        int64_t value_at = r.u32(ptr + 4);
        _value(r, p, value_at, value_at, true);
        if (OBJECT_TYPES.contains(p.type)) p.value = Value(r.u32(value_at));
        out.append(p);
        ptr += 8;
    }
    return {out, ptr};
}

Effect read(const py::bytes& data_in) {
    py::bytes data = data_in;
    if (py::len(data) >= 8 && py::unpack_from1<int64_t>(">I", data, 0) == XNA_MAGIC)
        data = data.slice_from(py::unpack_from1<int64_t>(">I", data, 4));
    else if (py::len(data) >= 8 && py::unpack_from1<int64_t>("<I", data, 0) == XNA_MAGIC)
        data = data.slice_from(py::unpack_from1<int64_t>("<I", data, 4));
    bool big;
    if (py::unpack_from1<int64_t>(">I", data, 0) == D3DX_MAGIC) big = true;
    else if (py::unpack_from1<int64_t>("<I", data, 0) == D3DX_MAGIC) big = false;
    else throw py::ValueError("not a Direct3D effect (no 0xFEFF0901)");
    Reader r(data, 8, big);
    int64_t start = py::unpack_from1<int64_t>(big ? ">I" : "<I", data, 4);
    Effect fx;
    fx.big_endian = big;
    int64_t ptr = start;
    int64_t n_params = r.u32(ptr);
    int64_t n_tech = r.u32(ptr + 4);
    ptr += 16;   // and the shader and object counts
    for (int64_t k = 0; k < n_params; ++k) {
        Parameter p = std::get<0>(_typedef(r, r.u32(ptr)));
        int64_t value_at = r.u32(ptr + 4);
        p.flags = r.u32(ptr + 8);
        int64_t n_annotations = r.u32(ptr + 12);
        ptr += 16;
        std::tie(p.annotations, ptr) = _annotations(r, ptr, n_annotations);
        if (SAMPLER_TYPES.contains(p.type) && !p.elements) {
            _value(r, p, value_at, value_at, true);
        } else if (OBJECT_TYPES.contains(p.type) && !p.elements) {
            p.value = Value(r.u32(value_at));
        } else {
            _value(r, p, value_at, value_at, true);
        }
        fx.parameters.append(p);
    }
    for (int64_t k = 0; k < n_tech; ++k) {
        py::str name = r.name(r.u32(ptr));
        int64_t n_annotations = r.u32(ptr + 4);
        int64_t n_passes = r.u32(ptr + 8);
        ptr += 12;
        py::list<Parameter> annotations;
        std::tie(annotations, ptr) = _annotations(r, ptr, n_annotations);
        py::list<Pass> passes;
        for (int64_t j = 0; j < n_passes; ++j) {
            py::str pname = r.name(r.u32(ptr));
            int64_t n_pa = r.u32(ptr + 4);
            int64_t n_states = r.u32(ptr + 8);
            ptr += 12;
            py::list<Parameter> pass_annotations;
            std::tie(pass_annotations, ptr) = _annotations(r, ptr, n_pa);
            py::list<State> states;
            for (int64_t s = 0; s < n_states; ++s) {
                auto [st, next] = _state(r, ptr);
                ptr = next;
                states.append(st);
            }
            passes.append(Pass(pname, pass_annotations, states));
        }
        fx.techniques.append(Technique(name, annotations, passes));
    }
    int64_t n_strings = r.u32(ptr);
    int64_t n_resources = r.u32(ptr + 4);
    ptr += 8;
    for (int64_t k = 0; k < n_strings; ++k) {
        int64_t oid = r.u32(ptr);
        int64_t size = r.u32(ptr + 4);
        fx.objects.set(oid, data.slice(8 + ptr + 8, 8 + ptr + 8 + size));
        ptr += 8 + ((size + 3) & ~int64_t(3));
    }
    for (int64_t k = 0; k < n_resources; ++k) {
        int64_t technique = r.u32(ptr + 0), index = r.u32(ptr + 4), element = r.u32(ptr + 8),
                state_index = r.u32(ptr + 12), usage = r.u32(ptr + 16);
        ptr += 20;
        State* state = nullptr;
        // what Python raises when it uses the item it found as a State (after the blob is read)
        std::optional<py::str> not_a_state;
        if (technique == 0xFFFFFFFF) {   // a sampler parameter's state
            Parameter* p = &fx.parameters[index];
            if (element != 0xFFFFFFFF && p->elements) p = &p->members[element];
            const Value& v = p->value;
            if (auto* ss = std::get_if<py::list<State>>(&v.data)) {
                state = &(*ss)[state_index];
            } else if (std::holds_alternative<std::monostate>(v.data)) {
                throw py::TypeError("'NoneType' object is not subscriptable");
            } else if (std::holds_alternative<int64_t>(v.data)) {
                throw py::TypeError("'int' object is not subscriptable");
            } else {
                int64_t n = list_len(v);
                if (state_index >= n) throw py::IndexError("list index out of range");
                not_a_state = py::str("object has no attribute 'parameter'");
            }
        } else {
            state = &fx.techniques[technique].passes[index].states[state_index];
        }
        int64_t size = r.u32(ptr);
        py::bytes blob = data.slice(8 + ptr + 4, 8 + ptr + 4 + size);
        ptr += 4 + ((size + 3) & ~int64_t(3));
        if (not_a_state) throw py::AttributeError(not_a_state->utf8());
        if (usage == 0) {   // the object itself: a shader
            const Value& key = state->parameter->value;
            if (auto* id = std::get_if<int64_t>(&key.data)) fx.objects.set(*id, blob);
            else throw py::TypeError("effect object id is not an int (not representable in Effect.objects)");
        } else if (usage == 1) {   // the name of a parameter holding it
            int64_t nul = blob.find(py::bytes("\0", 1));
            state->parameter->reference = (nul < 0 ? blob : blob.slice(0, nul)).decode("latin-1");
        } else {   // an expression (a preshader): kept, not evaluated
            state->parameter->expression = blob;
        }
    }
    return fx;
}

}  // namespace codebreaker::formats::xbox::effect
