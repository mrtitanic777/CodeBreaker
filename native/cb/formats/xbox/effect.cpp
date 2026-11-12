#include "cb/formats/xbox/effect.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <cmath>
#include <format>
#include <map>
#include <span>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::effect {

namespace {

constexpr std::string_view kEffectReader = "xbox effect";
constexpr std::uint32_t kNoIndex = 0xFFFFFFFF;
constexpr std::uint32_t kMaxElements = 1u << 16;   // no real effect has an array of more
constexpr std::uint64_t kMaxValues = 1u << 20;     // the numbers of one parameter
constexpr int kMaxNesting = 64;                    // nor a struct inside a struct this deep

// The Xbox 360's effect state numbering: operation -> the state's name in an .fx file. Found by compiling an effect
// setting each state with the SDK's fxc (53-55 and 65-87 are the Xbox's own states, none of which an effect for the PC
// has).
const std::map<std::uint32_t, std::string>& state_names() {
    static const std::map<std::uint32_t, std::string> names = [] {
        std::map<std::uint32_t, std::string> m;
        std::uint32_t n = 0;
        for (const char* s : {"ZEnable", "FillMode", "ZWriteEnable", "AlphaTestEnable", "SrcBlend", "DestBlend", "CullMode",
                              "ZFunc", "AlphaRef", "AlphaFunc", "AlphaBlendEnable", "StencilEnable", "StencilFail",
                              "StencilZFail", "StencilPass", "StencilFunc", "StencilRef", "StencilMask", "StencilWriteMask"}) {
            m[n++] = s;
        }
        for (int i = 0; i < 16; ++i) m[n++] = std::format("Wrap{}", i);
        for (const char* s : {"ClipPlaneEnable", "PointSize", "PointSize_Min", "PointSize_Max", "PointSpriteEnable",
                              "MultiSampleAntiAlias", "MultiSampleMask", "ColorWriteEnable", "BlendOp", "ScissorTestEnable",
                              "SlopeScaleDepthBias", "MinTessellationLevel", "MaxTessellationLevel", "TwoSidedStencilMode",
                              "CCW_StencilFail", "CCW_StencilZFail", "CCW_StencilPass", "CCW_StencilFunc"}) {
            m[n++] = s;
        }
        m[56] = "ColorWriteEnable1"; m[57] = "ColorWriteEnable2"; m[58] = "ColorWriteEnable3"; m[59] = "BlendFactor";
        m[60] = "DepthBias"; m[61] = "SeparateAlphaBlendEnable"; m[62] = "SrcBlendAlpha"; m[63] = "DestBlendAlpha";
        m[64] = "BlendOpAlpha"; m[88] = "LastPixel"; m[91] = "FVF"; m[92] = "VertexShader"; m[93] = "PixelShader";
        m[110] = "Texture"; m[111] = "AddressU"; m[112] = "AddressV"; m[113] = "AddressW"; m[114] = "BorderColor";
        m[115] = "MagFilter"; m[116] = "MinFilter"; m[117] = "MipFilter"; m[118] = "MipMapLodBias"; m[119] = "MaxMipLevel";
        m[120] = "MaxAnisotropy";
        return m;
    }();
    return names;
}

using EnumNames = std::map<std::int64_t, std::string_view>;

// The Xbox 360's values of the enumerated states (the SDK's d3d9types.h), by their .fx names.
const EnumNames* enum_values(std::string_view state) {
    static const EnumNames blend = {{0, "Zero"}, {1, "One"}, {4, "SrcColor"}, {5, "InvSrcColor"}, {6, "SrcAlpha"},
                                    {7, "InvSrcAlpha"}, {8, "DestColor"}, {9, "InvDestColor"}, {10, "DestAlpha"},
                                    {11, "InvDestAlpha"}, {12, "BlendFactor"}, {13, "InvBlendFactor"},
                                    {14, "BlendFactor"}, {15, "InvBlendFactor"}, {16, "SrcAlphaSat"}};   // 14/15: the Xbox's constant alpha
    static const EnumNames cmp = {{0, "Never"}, {1, "Less"}, {2, "Equal"}, {3, "LessEqual"}, {4, "Greater"},
                                  {5, "NotEqual"}, {6, "GreaterEqual"}, {7, "Always"}};
    static const EnumNames stencil = {{0, "Keep"}, {1, "Zero"}, {2, "Replace"}, {3, "IncrSat"}, {4, "DecrSat"},
                                      {5, "Invert"}, {6, "Incr"}, {7, "Decr"}};
    static const EnumNames address = {{0, "Wrap"}, {1, "Mirror"}, {2, "Clamp"}, {3, "MirrorOnce"}, {4, "Border"},
                                      {5, "MirrorOnce"}, {6, "Border"}, {7, "MirrorOnce"}};
    static const EnumNames filter = {{0, "Point"}, {1, "Linear"}, {2, "None"}, {4, "Anisotropic"}};
    static const EnumNames blend_op = {{0, "Add"}, {1, "Subtract"}, {2, "Min"}, {3, "Max"}, {4, "RevSubtract"}};
    static const EnumNames cull = {{0, "None"}, {2, "CW"}, {6, "CCW"}};
    static const EnumNames fill = {{0, "Solid"}, {1, "Point"}, {0x25, "WireFrame"}};
    static const std::map<std::string_view, const EnumNames*> table = {
        {"SrcBlend", &blend}, {"DestBlend", &blend}, {"SrcBlendAlpha", &blend}, {"DestBlendAlpha", &blend},
        {"BlendOp", &blend_op}, {"CullMode", &cull}, {"FillMode", &fill},
        {"ZFunc", &cmp}, {"AlphaFunc", &cmp}, {"StencilFunc", &cmp}, {"CCW_StencilFunc", &cmp},
        {"StencilFail", &stencil}, {"StencilZFail", &stencil}, {"StencilPass", &stencil}, {"CCW_StencilFail", &stencil},
        {"CCW_StencilZFail", &stencil}, {"CCW_StencilPass", &stencil},
        {"AddressU", &address}, {"AddressV", &address}, {"AddressW", &address},
        {"MagFilter", &filter}, {"MinFilter", &filter}, {"MipFilter", &filter}};
    const auto it = table.find(state);
    return it == table.end() ? nullptr : it->second;
}

// A float the shortest way that reads back as the same float, with a ".0" when it would read as an integer.
std::string float_text(float v) {
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, v);
    std::string s(buf, r.ptr);
    if (s.find_first_of(".en") == std::string::npos) s += ".0";   // (inf and nan carry an i or an n)
    return s;
}

// The effect's bytes with the endianness of the file.
class Reader {
public:
    Reader(ByteView data, std::uint64_t base, bool big) : data_(data), base_(base), big_(big) {}
    ByteView data() const { return data_; }
    std::uint64_t base() const { return base_; }
    std::uint32_t u32(std::uint64_t at) const {
        io::ByteReader r(data_, kEffectReader);
        r.seek(base_ + at);
        return big_ ? r.u32_be() : r.u32();
    }
    // A length-prefixed string at `at` (relative to the data base), up to its NUL.
    std::string name(std::uint64_t at) const {
        const std::uint32_t n = u32(at);
        return text::from_latin1(until_nul(io::checked_range(data_, base_ + at + 4, n)));
    }
    // `n` bytes at `at` (relative to the base).
    ByteView bytes(std::uint64_t at, std::uint64_t n) const { return io::checked_range(data_, base_ + at, n); }
    // The same, but a blob whose last bytes are not there is what is there: an effect cut inside the padding after its last
    // object (or an object that the file does not finish) still gives the rest. What a blob is made of is checked by whoever
    // reads it (a shader is read by formats/xbox/shader, which throws for one that is cut short).
    ByteView blob(std::uint64_t at, std::uint64_t n) const {
        const std::uint64_t from = base_ + at;
        if (from > data_.size()) throw TruncatedError(kEffectReader, from, n, 0);
        return data_.subspan(static_cast<std::size_t>(from), static_cast<std::size_t>(std::min<std::uint64_t>(n, data_.size() - from)));
    }

private:
    static ByteView until_nul(ByteView v) {
        const auto nul = std::find(v.begin(), v.end(), std::uint8_t{0});
        return v.first(static_cast<std::size_t>(nul - v.begin()));
    }
    ByteView data_;
    std::uint64_t base_;
    bool big_;
};

// A type definition at `at`; the offset after it. `parent` is set for the elements of an array: they take their type from
// the array's definition and read nothing.
std::uint64_t read_typedef(const Reader& r, std::uint64_t at, Parameter& p, const Parameter* parent, int depth) {
    if (depth > kMaxNesting) throw FormatError(kEffectReader, r.base() + at, "type definitions nested too deep");
    if (parent != nullptr) {
        p.type = parent->type;
        p.cls = parent->cls;
        p.name = parent->name;
        p.semantic = parent->semantic;
        p.rows = parent->rows;
        p.columns = parent->columns;
        p.members = parent->members;
        return at;
    }
    p.type = d3dx::type_of(r.u32(at));
    const std::uint32_t cls = r.u32(at + 4);
    if (cls >= 6) throw FormatError(kEffectReader, r.base() + at + 4, std::format("parameter class {} is not one the effect framework defines", cls));
    p.cls = d3dx::class_of(cls);
    p.name = r.name(r.u32(at + 8));
    p.semantic = r.name(r.u32(at + 12));
    p.elements = r.u32(at + 16);
    at += 20;
    switch (p.cls) {
        case d3dx::ParamClass::Vector:   // a vector is written as columns, then rows
            p.columns = r.u32(at);
            p.rows = r.u32(at + 4);
            at += 8;
            break;
        case d3dx::ParamClass::Scalar:
        case d3dx::ParamClass::MatrixRows:
        case d3dx::ParamClass::MatrixColumns:
            p.rows = r.u32(at);
            p.columns = r.u32(at + 4);
            at += 8;
            break;
        case d3dx::ParamClass::Struct: {
            const std::uint32_t count = r.u32(at);
            at += 4;
            for (std::uint32_t i = 0; i < count; ++i) {
                Parameter m;
                at = read_typedef(r, at, m, nullptr, depth + 1);
                p.members.push_back(std::move(m));
            }
            break;
        }
        default:
            break;
    }
    if (p.elements != 0) {
        if (p.elements > kMaxElements) throw FormatError(kEffectReader, r.base() + at, std::format("an array of {} elements", p.elements));
        std::vector<Parameter> element_members = std::move(p.members);
        p.members.clear();
        for (std::uint32_t i = 0; i < p.elements; ++i) {
            Parameter e;
            read_typedef(r, at, e, &p, depth);
            e.members = element_members;
            p.members.push_back(std::move(e));
        }
    }
    return at;
}

std::uint64_t read_state(const Reader& r, std::uint64_t ptr, State& out);

// Fill p's value from the value data at `at` (numbers) and the stream at `ptr` (objects, samplers); the stream's offset
// after, and the bytes of value data used.
struct ValueEnd {
    std::uint64_t ptr;
    std::uint64_t used;
};

ValueEnd read_value(const Reader& r, Parameter& p, std::uint64_t at, std::uint64_t ptr) {
    if (p.elements != 0 || p.cls == d3dx::ParamClass::Struct) {
        std::uint64_t used = 0;
        for (Parameter& m : p.members) {
            const ValueEnd e = read_value(r, m, at + used, ptr);
            ptr = e.ptr;
            used += e.used;
        }
        return {ptr, used};
    }
    if (d3dx::is_numeric_class(p.cls)) {
        const std::uint64_t n = std::uint64_t{p.rows} * p.columns;
        if (n > kMaxValues) throw FormatError(kEffectReader, r.base() + at, std::format("a parameter of {} values", n));
        (void)r.bytes(at, 4 * n);   // (a table that lies past the end is damage, however many values it claims)
        if (p.type == d3dx::ParamType::Float) {
            std::vector<float> v;
            for (std::uint64_t i = 0; i < n; ++i) v.push_back(std::bit_cast<float>(r.u32(at + 4 * i)));
            p.value = std::move(v);
        } else {
            std::vector<std::int32_t> v;
            for (std::uint64_t i = 0; i < n; ++i) v.push_back(static_cast<std::int32_t>(r.u32(at + 4 * i)));
            p.value = std::move(v);
        }
        return {ptr, 4 * n};
    }
    if (d3dx::is_object_type(p.type)) {
        p.value = ObjectId{r.u32(ptr)};   // the object's id
        return {ptr + 4, 0};
    }
    if (d3dx::is_sampler_type(p.type)) {
        const std::uint32_t count = r.u32(ptr);
        ptr += 4;
        for (std::uint32_t i = 0; i < count; ++i) {
            State s;
            ptr = read_state(r, ptr, s);
            p.states.push_back(std::move(s));
        }
        return {ptr, 0};
    }
    return {ptr, 0};
}

std::uint64_t read_state(const Reader& r, std::uint64_t ptr, State& out) {
    out.operation = r.u32(ptr);
    out.index = r.u32(ptr + 4);
    out.name = state_name(out.operation);
    read_typedef(r, r.u32(ptr + 8), out.parameter, nullptr, 0);
    const std::uint32_t value_at = r.u32(ptr + 12);
    read_value(r, out.parameter, value_at, value_at);
    return ptr + 16;
}

std::uint64_t read_annotations(const Reader& r, std::uint64_t ptr, std::uint32_t count, std::vector<Parameter>& out) {
    for (std::uint32_t i = 0; i < count; ++i) {
        Parameter p;
        read_typedef(r, r.u32(ptr), p, nullptr, 0);
        const std::uint32_t value_at = r.u32(ptr + 4);
        read_value(r, p, value_at, value_at);
        out.push_back(std::move(p));
        ptr += 8;
    }
    return ptr;
}

std::optional<std::string> text_of_blob(ByteView blob) {
    const auto nul = std::find(blob.begin(), blob.end(), std::uint8_t{0});
    return text::from_latin1(blob.first(static_cast<std::size_t>(nul - blob.begin())));
}

std::uint32_t be32(ByteView d, std::size_t at) {
    return (std::uint32_t{d[at]} << 24) | (std::uint32_t{d[at + 1]} << 16) | (std::uint32_t{d[at + 2]} << 8) | d[at + 3];
}
std::uint32_t le32(ByteView d, std::size_t at) {
    return (std::uint32_t{d[at + 3]} << 24) | (std::uint32_t{d[at + 2]} << 16) | (std::uint32_t{d[at + 1]} << 8) | d[at];
}

}  // namespace

// ---- the records (out of line: a Parameter holds States, which hold a Parameter)
Parameter::Parameter() = default;
Parameter::Parameter(const Parameter&) = default;
Parameter::Parameter(Parameter&&) noexcept = default;
Parameter& Parameter::operator=(const Parameter&) = default;
Parameter& Parameter::operator=(Parameter&&) noexcept = default;
Parameter::~Parameter() = default;

std::size_t Parameter::size() const {
    if (elements != 0 || cls == d3dx::ParamClass::Struct) {
        std::size_t total = 0;
        for (const Parameter& m : members) total += m.size();
        return total;
    }
    if (d3dx::is_numeric_class(cls)) return std::size_t{4} * rows * columns;
    return 0;
}

std::span<const float> Parameter::floats() const {
    const auto* v = std::get_if<std::vector<float>>(&value);
    return v ? std::span<const float>(*v) : std::span<const float>();
}

std::span<const std::int32_t> Parameter::ints() const {
    const auto* v = std::get_if<std::vector<std::int32_t>>(&value);
    return v ? std::span<const std::int32_t>(*v) : std::span<const std::int32_t>();
}

std::optional<ObjectId> Parameter::object() const {
    if (const auto* id = std::get_if<ObjectId>(&value)) return *id;
    return std::nullopt;
}

std::string state_name(std::uint32_t operation) {
    const auto it = state_names().find(operation);
    return it == state_names().end() ? std::format("state_{}", operation) : it->second;
}

std::optional<std::string> State::text() const {
    double number = 0;
    bool is_float = false;
    if (const auto* i = std::get_if<std::vector<std::int32_t>>(&parameter.value); i && i->size() == 1) {
        number = (*i)[0];
    } else if (const auto* f = std::get_if<std::vector<float>>(&parameter.value); f && f->size() == 1) {
        number = (*f)[0];
        is_float = true;
    } else {
        return std::nullopt;
    }
    if (const EnumNames* values = enum_values(name)) {
        if (number == std::floor(number)) {
            const auto it = values->find(static_cast<std::int64_t>(number));
            if (it != values->end()) return std::string(it->second);
        }
        return is_float ? float_text(static_cast<float>(number)) : std::to_string(static_cast<std::int64_t>(number));
    }
    if (parameter.type == d3dx::ParamType::Bool || name.ends_with("Enable") || name == "TwoSidedStencilMode") {
        return number != 0 ? "true" : "false";
    }
    return is_float ? float_text(static_cast<float>(number)) : std::to_string(static_cast<std::int64_t>(number));
}

const State* Pass::state(std::string_view wanted) const {
    for (const State& s : states) {
        if (s.name == wanted) return &s;
    }
    return nullptr;
}

const Parameter* Effect::parameter(std::string_view wanted) const {
    for (const Parameter& p : parameters) {
        if (p.name == wanted) return &p;
    }
    return nullptr;
}

const Bytes* Effect::object(ObjectId id) const {
    const auto it = objects.find(id.id);
    return it == objects.end() ? nullptr : &it->second;
}

std::string Effect::string(ObjectId id) const {
    const Bytes* b = object(id);
    return b ? text_of_blob(*b).value_or("") : std::string();
}

Effect read(ByteView data) {
    // XNA's header: the magic and the offset of the Direct3D effect
    if (data.size() >= 8 && be32(data, 0) == kXnaMagic) {
        data = io::checked_range(data, be32(data, 4), data.size() - std::min<std::size_t>(data.size(), be32(data, 4)));
    } else if (data.size() >= 8 && le32(data, 0) == kXnaMagic) {
        data = io::checked_range(data, le32(data, 4), data.size() - std::min<std::size_t>(data.size(), le32(data, 4)));
    }
    if (data.size() < 8) throw TruncatedError(kEffectReader, 0, 8, data.size());
    bool big;
    if (be32(data, 0) == kD3dxMagic) {
        big = true;
    } else if (le32(data, 0) == kD3dxMagic) {
        big = false;
    } else {
        throw FormatError(kEffectReader, 0, "not a Direct3D effect (no 0xFEFF0901)");
    }
    const Reader r(data, 8, big);
    Effect fx;
    fx.big_endian = big;
    std::uint64_t ptr = big ? be32(data, 4) : le32(data, 4);
    const std::uint32_t n_params = r.u32(ptr), n_tech = r.u32(ptr + 4);
    ptr += 16;   // and the shader and object counts
    for (std::uint32_t i = 0; i < n_params; ++i) {
        Parameter p;
        read_typedef(r, r.u32(ptr), p, nullptr, 0);
        const std::uint32_t value_at = r.u32(ptr + 4), n_annotations = r.u32(ptr + 12);
        p.flags = r.u32(ptr + 8);
        ptr += 16;
        ptr = read_annotations(r, ptr, n_annotations, p.annotations);
        if (d3dx::is_object_type(p.type) && p.elements == 0 && !d3dx::is_sampler_type(p.type)) {
            p.value = ObjectId{r.u32(value_at)};
        } else {
            read_value(r, p, value_at, value_at);
        }
        fx.parameters.push_back(std::move(p));
    }
    for (std::uint32_t i = 0; i < n_tech; ++i) {
        Technique t;
        t.name = r.name(r.u32(ptr));
        const std::uint32_t n_annotations = r.u32(ptr + 4), n_passes = r.u32(ptr + 8);
        ptr += 12;
        ptr = read_annotations(r, ptr, n_annotations, t.annotations);
        for (std::uint32_t k = 0; k < n_passes; ++k) {
            Pass pass;
            pass.name = r.name(r.u32(ptr));
            const std::uint32_t n_pa = r.u32(ptr + 4), n_states = r.u32(ptr + 8);
            ptr += 12;
            ptr = read_annotations(r, ptr, n_pa, pass.annotations);
            for (std::uint32_t s = 0; s < n_states; ++s) {
                State state;
                ptr = read_state(r, ptr, state);
                pass.states.push_back(std::move(state));
            }
            t.passes.push_back(std::move(pass));
        }
        fx.techniques.push_back(std::move(t));
    }
    const std::uint32_t n_strings = r.u32(ptr), n_resources = r.u32(ptr + 4);
    ptr += 8;
    for (std::uint32_t i = 0; i < n_strings; ++i) {
        const std::uint32_t id = r.u32(ptr), size = r.u32(ptr + 4);
        const ByteView blob = r.blob(ptr + 8, size);
        fx.objects[id] = Bytes(blob.begin(), blob.end());
        ptr += 8 + ((std::uint64_t{size} + 3) & ~std::uint64_t{3});
    }
    for (std::uint32_t i = 0; i < n_resources; ++i) {
        const std::uint32_t technique = r.u32(ptr), index = r.u32(ptr + 4), element = r.u32(ptr + 8),
                            state_index = r.u32(ptr + 12), usage = r.u32(ptr + 16);
        ptr += 20;
        State* state = nullptr;
        if (technique == kNoIndex) {   // a sampler parameter's state
            if (index >= fx.parameters.size()) throw FormatError(kEffectReader, r.base() + ptr, "a resource names a parameter the effect does not have");
            Parameter* p = &fx.parameters[index];
            if (element != kNoIndex && p->elements != 0) {
                if (element >= p->members.size()) throw FormatError(kEffectReader, r.base() + ptr, "a resource names an array element the parameter does not have");
                p = &p->members[element];
            }
            if (state_index >= p->states.size()) throw FormatError(kEffectReader, r.base() + ptr, "a resource names a sampler state the parameter does not have");
            state = &p->states[state_index];
        } else {
            if (technique >= fx.techniques.size() || index >= fx.techniques[technique].passes.size() ||
                state_index >= fx.techniques[technique].passes[index].states.size()) {
                throw FormatError(kEffectReader, r.base() + ptr, "a resource names a state the effect does not have");
            }
            state = &fx.techniques[technique].passes[index].states[state_index];
        }
        const std::uint32_t size = r.u32(ptr);
        const ByteView blob = r.blob(ptr + 4, size);
        ptr += 4 + ((std::uint64_t{size} + 3) & ~std::uint64_t{3});
        if (usage == 0) {   // the object itself: a shader
            const std::optional<ObjectId> id = state->parameter.object();
            if (!id) throw FormatError(kEffectReader, r.base() + ptr, "a resource gives an object to a state whose value is not one");
            fx.objects[id->id] = Bytes(blob.begin(), blob.end());
        } else if (usage == 1) {   // the name of a parameter holding it
            state->parameter.reference = text_of_blob(blob);
        } else {   // an expression (a preshader): kept, not evaluated
            state->parameter.expression = Bytes(blob.begin(), blob.end());
        }
    }
    return fx;
}

}  // namespace cb::formats::xbox::effect
