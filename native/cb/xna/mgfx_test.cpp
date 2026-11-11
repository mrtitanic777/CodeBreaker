// Tests of cb::xna::mgfx. The expected bytes come from an encoder written here from MonoGame's layout (not from
// mgfx.cpp), and from three effects mgfxc really compiled (testdata/effect_*.mgfxo) whose patched forms were checked
// against the layout by the reader.
#include "cb/xna/mgfx.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdio>
#include <map>
#include <cstring>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

// A comparison of containers (a failure message cannot show them): the two are equal or the check fails.
#define CB_CHECK_SAME(a, ...) CB_CHECK((a) == (__VA_ARGS__))

namespace {

using cb::Bytes;
using cb::ByteView;
using namespace cb::xna::mgfx;

// ---- an independent encoder of the MGFX pieces -------------------------------------------------------------

void put(Bytes& out, const Bytes& more) { out.insert(out.end(), more.begin(), more.end()); }
void put_i32(Bytes& out, std::int32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(static_cast<std::uint32_t>(v) >> (8 * i)));
}
void put_f32(Bytes& out, float f) {
    std::uint32_t u;
    std::memcpy(&u, &f, 4);
    put_i32(out, static_cast<std::int32_t>(u));
}

Bytes vs(const std::string& s) {   // a string: 7-bit varint length, bytes
    Bytes out;
    std::size_t n = s.size();
    do {
        std::uint8_t b = n & 0x7F;
        n >>= 7;
        out.push_back(static_cast<std::uint8_t>(b | (n ? 0x80 : 0)));
    } while (n);
    out.insert(out.end(), s.begin(), s.end());
    return out;
}

constexpr std::uint8_t kScalar = 0, kVector = 1, kMatrix = 2, kObject = 3, kStruct = 4;
constexpr std::uint8_t kBool = 1, kInt = 2, kFloat = 3, kTexture = 5, kTexture2D = 7;

Bytes enc_param(std::uint8_t cls, std::uint8_t typ, const std::string& name, const std::string& sem, std::uint8_t rows,
                std::uint8_t cols, const std::vector<Bytes>& elements = {}, const std::vector<Bytes>& members = {},
                const std::vector<float>& values = {}) {
    Bytes out{cls, typ};
    put(out, vs(name));
    put(out, vs(sem));
    put_i32(out, 0);
    out.push_back(rows);
    out.push_back(cols);
    put_i32(out, static_cast<std::int32_t>(elements.size()));
    for (const Bytes& e : elements) put(out, e);
    put_i32(out, static_cast<std::int32_t>(members.size()));
    for (const Bytes& m : members) put(out, m);
    if (elements.empty() && members.empty() && (typ == 1 || typ == 2 || typ == 3))
        for (float v : values) put_f32(out, v);
    return out;
}

Bytes plist(const std::vector<Bytes>& ps) {
    Bytes out;
    put_i32(out, static_cast<std::int32_t>(ps.size()));
    for (const Bytes& p : ps) put(out, p);
    return out;
}

struct Buffer {
    std::string name;
    std::int16_t size;
    std::int32_t n;
};

Bytes shader(bool vertex, const std::string& code, const std::vector<std::pair<std::string, bool>>& samplers,
             const std::vector<std::uint8_t>& buffers, const std::vector<std::string>& attributes) {
    Bytes out{static_cast<std::uint8_t>(vertex)};
    put_i32(out, static_cast<std::int32_t>(code.size()));
    out.insert(out.end(), code.begin(), code.end());
    out.push_back(static_cast<std::uint8_t>(samplers.size()));
    for (const auto& [name, state] : samplers) {
        out.insert(out.end(), {'a', 'b', 'c', static_cast<std::uint8_t>(state)});
        if (state) out.insert(out.end(), 3 + 4 + 1 + 4 + 4 + 4, std::uint8_t{0});
        put(out, vs(name));
        out.push_back(5);
    }
    out.push_back(static_cast<std::uint8_t>(buffers.size()));
    out.insert(out.end(), buffers.begin(), buffers.end());
    out.push_back(static_cast<std::uint8_t>(attributes.size()));
    for (const std::string& a : attributes) {
        put(out, vs(a));
        out.insert(out.end(), 4, std::uint8_t{0});
    }
    return out;
}

// An effect: header, buffers, shaders, `params` (the list as written), `tail` (what follows the parameters).
Bytes effect(const std::vector<Buffer>& buffers, const std::vector<Bytes>& shaders, const Bytes& params, const Bytes& tail) {
    Bytes out{'M', 'G', 'F', 'X', 10, 1, 1, 2, 3, 4};
    put_i32(out, static_cast<std::int32_t>(buffers.size()));
    for (const Buffer& b : buffers) {
        put(out, vs(b.name));
        out.push_back(static_cast<std::uint8_t>(b.size & 0xFF));
        out.push_back(static_cast<std::uint8_t>(b.size >> 8));
        put_i32(out, b.n);
        if (b.n > 0) out.insert(out.end(), 6 * static_cast<std::size_t>(b.n), std::uint8_t{0});
    }
    put_i32(out, static_cast<std::int32_t>(shaders.size()));
    for (const Bytes& s : shaders) put(out, s);
    put(out, params);
    put(out, tail);
    return out;
}

const Bytes kTail = [] {
    Bytes t{2, 0, 0, 0};
    const std::string text = "technique-bytes-here";
    t.insert(t.end(), text.begin(), text.end());
    t.insert(t.end(), {0, 'M', 'G', 'F', 'X'});
    return t;
}();

std::vector<float> iota(int n) {
    std::vector<float> v;
    for (int i = 0; i < n; ++i) v.push_back(static_cast<float>(i));
    return v;
}

// The effect the tests patch: two buffers, two shaders (one with samplers carrying state), five parameters of
// every kind (scalar, object, array, struct, matrix).
struct Fixture {
    std::vector<Bytes> existing;
    Bytes effect_bytes;
    Fixture() {
        existing = {
            enc_param(kScalar, kBool, "Flag", "", 1, 1, {}, {}, {1.0f}),
            enc_param(kObject, kTexture, "Diffuse", "", 0, 0),
            enc_param(kScalar, kInt, "Indices", "", 1, 1,
                      {enc_param(kScalar, kInt, "", "", 1, 1, {}, {}, {5.0f}), enc_param(kScalar, kInt, "", "", 1, 1, {}, {}, {6.0f})}),
            enc_param(kStruct, 0, "Light", "", 0, 0, {},
                      {enc_param(kVector, kFloat, "Dir", "", 1, 3, {}, {}, {0, 1, 0}), enc_param(kScalar, kFloat, "Power", "", 1, 1, {}, {}, {2})}),
            enc_param(kMatrix, kFloat, "Bones", "", 4, 3, {}, {}, iota(12)),
        };
        effect_bytes = effect({{"Globals", 64, 3}, {"PerObject", 128, 0}},
                              {shader(true, "vertexcode", {{"tex", true}, {"lm", false}}, {0, 1}, {"POSITION", "TEXCOORD"}),
                               shader(false, "", {}, {}, {})},
                              plist(existing), kTail);
    }
};

// What add_parameters must give: the same effect with `more` appended to the list.
Bytes with_more(const Fixture& f, const std::vector<Bytes>& more) {
    std::vector<Bytes> all = f.existing;
    all.insert(all.end(), more.begin(), more.end());
    return effect({{"Globals", 64, 3}, {"PerObject", 128, 0}},
                  {shader(true, "vertexcode", {{"tex", true}, {"lm", false}}, {0, 1}, {"POSITION", "TEXCOORD"}), shader(false, "", {}, {}, {})},
                  plist(all), kTail);
}

Parameter param(const std::string& name, ParamType type, ParamClass cls, std::uint32_t rows, std::uint32_t cols,
                std::vector<double> value = {}, std::optional<std::string> semantic = std::string()) {
    Parameter p;
    p.name = name;
    p.type = type;
    p.cls = cls;
    p.rows = rows;
    p.columns = cols;
    p.value = std::move(value);
    p.semantic = std::move(semantic);
    return p;
}

Bytes add(const Bytes& data, const std::vector<Parameter>& ps) { return add_parameters(data, ps); }

std::filesystem::path data(const char* name) { return cb::testing::data_path(std::string("xna/testdata/") + name); }

}  // namespace

CB_TEST("xna.mgfx.scalar_vector_matrix_defaults", [] {
    const Fixture f;
    const Bytes got = add(f.effect_bytes,
                          {param("ShaderIndex", ParamType::Int, ParamClass::Scalar, 1, 1, {2}),
                           param("Tint", ParamType::Float, ParamClass::Vector, 0, 4, {1, 0.5, 0.25, 1}, "COLOR0"),
                           param("Mat", ParamType::Float, ParamClass::MatrixRows, 2, 3, {1, 2, 3, 4, 5, 6})});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kScalar, kInt, "ShaderIndex", "", 1, 1, {}, {}, {2}),
                                   enc_param(kVector, kFloat, "Tint", "COLOR0", 1, 4, {}, {}, {1, 0.5f, 0.25f, 1}),
                                   enc_param(kMatrix, kFloat, "Mat", "", 2, 3, {}, {}, {1, 2, 3, 4, 5, 6})}));
});

CB_TEST("xna.mgfx.matrix_columns_are_written_a_row_at_a_time", [] {
    const Fixture f;
    // the Xbox's effect keeps a column at a time: 2 rows x 3 columns {1..6} is the columns (1,2) (3,4) (5,6)
    const Bytes got = add(f.effect_bytes, {param("M", ParamType::Float, ParamClass::MatrixColumns, 2, 3, {1, 2, 3, 4, 5, 6})});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kMatrix, kFloat, "M", "", 2, 3, {}, {}, {1, 3, 5, 2, 4, 6})}));
});

CB_TEST("xna.mgfx.default_is_padded_and_cut_to_the_size", [] {
    const Fixture f;
    const Bytes got = add(f.effect_bytes, {param("Short", ParamType::Float, ParamClass::Vector, 0, 4, {1, 2}),
                                           param("Long", ParamType::Float, ParamClass::Vector, 0, 2, {1, 2, 3, 4, 5}),
                                           param("None", ParamType::Float, ParamClass::Vector, 0, 3)});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kVector, kFloat, "Short", "", 1, 4, {}, {}, {1, 2, 0, 0}),
                                   enc_param(kVector, kFloat, "Long", "", 1, 2, {}, {}, {1, 2}),
                                   enc_param(kVector, kFloat, "None", "", 1, 3, {}, {}, {0, 0, 0})}));
});

CB_TEST("xna.mgfx.numbers_are_floats_of_32_bits", [] {
    const Fixture f;
    const Bytes got = add(f.effect_bytes, {param("Frac", ParamType::Float, ParamClass::Vector, 0, 3, {0.1, 1.0 / 3, 9007199254740993.0}),
                                           param("Inf", ParamType::Float, ParamClass::Vector, 0, 1, {HUGE_VAL})});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kVector, kFloat, "Frac", "", 1, 3, {}, {}, {0.1f, static_cast<float>(1.0 / 3), 9007199254740992.0f}),
                                   enc_param(kVector, kFloat, "Inf", "", 1, 1, {}, {}, {HUGE_VALF})}));
});

CB_TEST("xna.mgfx.textures_arrays_structs", [] {
    const Fixture f;
    Parameter tex = param("Tex3", ParamType::Texture3D, ParamClass::Object, 0, 0, {}, "TEXCOORD0");
    Parameter arr = param("Arr", ParamType::Int, ParamClass::Scalar, 1, 1);
    arr.elements = 2;
    arr.members = {param("Arr", ParamType::Int, ParamClass::Scalar, 1, 1, {5}), param("Arr", ParamType::Int, ParamClass::Scalar, 1, 1, {6})};
    Parameter st = param("Struct", ParamType::Void, ParamClass::Struct, 0, 0);
    st.members = {param("A", ParamType::Float, ParamClass::Vector, 0, 3, {1, 2, 3}), param("B", ParamType::Float, ParamClass::Scalar, 1, 1, {2})};
    const Bytes got = add(f.effect_bytes, {tex, arr, st});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kObject, 8, "Tex3", "TEXCOORD0", 0, 0),
                                   // an array's elements are nameless
                                   enc_param(kScalar, kInt, "Arr", "", 1, 1, {enc_param(kScalar, kInt, "", "", 1, 1, {}, {}, {5}), enc_param(kScalar, kInt, "", "", 1, 1, {}, {}, {6})}),
                                   enc_param(kStruct, 0, "Struct", "", 0, 0, {},
                                             {enc_param(kVector, kFloat, "A", "", 1, 3, {}, {}, {1, 2, 3}), enc_param(kScalar, kFloat, "B", "", 1, 1, {}, {}, {2})})}));
});

CB_TEST("xna.mgfx.what_is_not_added", [] {
    const Fixture f;
    // a parameter the effect has, one without a name, a sampler (its texture is the parameter), shaders and strings
    const Bytes got = add(f.effect_bytes, {param("Flag", ParamType::Float, ParamClass::Scalar, 1, 1, {9}),
                                           param("", ParamType::Float, ParamClass::Scalar, 1, 1, {1}),
                                           param("S", ParamType::Sampler, ParamClass::Object, 0, 0),
                                           param("VS", ParamType::VertexShader, ParamClass::Object, 0, 0),
                                           param("PS", ParamType::PixelShader, ParamClass::Object, 0, 0),
                                           param("Str", ParamType::String, ParamClass::Object, 0, 0)});
    CB_CHECK_SAME(got, f.effect_bytes);
});

CB_TEST("xna.mgfx.applied_twice_it_changes_nothing", [] {
    const Fixture f;
    const std::vector<Parameter> ps = {param("A", ParamType::Float, ParamClass::Scalar, 1, 1, {1}), param("B", ParamType::Bool, ParamClass::Scalar, 1, 1, {1})};
    const Bytes once = add(f.effect_bytes, ps);
    CB_CHECK_NE(once, f.effect_bytes);
    CB_CHECK_SAME(add(once, ps), once);
    CB_CHECK_SAME(add(f.effect_bytes, {}), f.effect_bytes);
});

CB_TEST("xna.mgfx.unknown_class_and_type_are_object_and_void", [] {
    CB_CHECK(parse_class("weird") == ParamClass::Object);
    CB_CHECK(parse_class("") == ParamClass::Object);
    CB_CHECK(parse_class("matrix_columns") == ParamClass::MatrixColumns);
    CB_CHECK(parse_type("foo") == ParamType::Void);
    CB_CHECK(parse_type("samplerCUBE") == ParamType::Sampler);
    CB_CHECK(parse_type("sampler2D") == ParamType::Sampler);
    CB_CHECK(parse_type("textureCUBE") == ParamType::TextureCube);
    const Fixture f;
    // an object of an unknown type: class 3 (object), type 0 (void), no default
    const Bytes got = add(f.effect_bytes, {param("U", parse_type("foo"), parse_class("weird"), 2, 2, {1, 2, 3, 4})});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kObject, 0, "U", "", 2, 2)}));
});

CB_TEST("xna.mgfx.names_in_other_scripts_are_kept_as_bytes", [] {
    const Fixture f;
    const Bytes got = add(f.effect_bytes, {param("Größe", ParamType::Float, ParamClass::Vector, 0, 2, {1, 2}, "日本")});
    CB_CHECK_SAME(got, with_more(f, {enc_param(kVector, kFloat, "Größe", "日本", 1, 2, {}, {}, {1, 2})}));
    const std::vector<std::string> names = parameter_names(got);
    CB_CHECK_EQ(names.back(), std::string("Größe"));
    // a name past the 7-bit length boundary
    const std::string wide(20000, 'L');
    CB_CHECK_EQ(parameter_names(add(f.effect_bytes, {param(wide, ParamType::Float, ParamClass::Scalar, 1, 1, {1})})).back(), wide);
});

CB_TEST("xna.mgfx.parameter_names_lists_the_effects_own", [] {
    const Fixture f;
    const std::vector<std::string> names = parameter_names(f.effect_bytes);
    CB_CHECK_SAME(names, (std::vector<std::string>{"Flag", "Diffuse", "Indices", "Light", "Bones"}));
    CB_CHECK(parameter_names(effect({}, {}, plist({}), kTail)).empty());
});

// ---- real effects --------------------------------------------------------------------------------------------

CB_TEST("xna.mgfx.real_effects_match_the_checked_result", [] {
    // The extras are parsed from a .fx.json as the port reads it; the three results were produced by the reference
    // (Python) implementation and read back with the layout: every effect keeps its own parameters, the extras follow
    // in order (the sampler "Skin" is not added), and nothing else moved.
    const std::vector<Parameter> extras = parse_parameters(cb::fs::read_text(data("extras.fx.json")));
    CB_CHECK_EQ(extras.size(), std::size_t{7});
    for (const char* which : {"a", "b", "c"}) {
        const Bytes original = cb::fs::read_file(data((std::string("effect_") + which + ".mgfxo").c_str()));
        const Bytes expected = cb::fs::read_file(data((std::string("effect_") + which + ".patched.mgfxo").c_str()));
        const Bytes got = add_parameters(original, extras);
        CB_CHECK_SAME(got, expected);
        CB_CHECK_SAME(parameter_names(original), std::vector<std::string>{"ColorSampler"});
        CB_CHECK_SAME(parameter_names(got), (std::vector<std::string>{"ColorSampler", "ShaderIndex", "Tint", "WorldViewProj", "Diffuse", "Indices", "Light"}));
        CB_CHECK_SAME(add_parameters(got, extras), got);   // once only
        // everything before and after the parameter list is as it was
        CB_CHECK_EQ(got.size(), original.size() + (expected.size() - original.size()));
        CB_CHECK(std::equal(original.begin(), original.begin() + 10, got.begin()));
        CB_CHECK(std::equal(original.end() - 9, original.end(), got.end() - 9));   // the techniques' tail
    }
});

CB_TEST("xna.mgfx.parse_parameters_reads_the_fx_json", [] {
    const std::vector<Parameter> ps = parse_parameters(cb::fs::read_text(data("extras.fx.json")));
    CB_CHECK_EQ(ps[0].name, std::string("ShaderIndex"));
    CB_CHECK(ps[0].type == ParamType::Int && ps[0].cls == ParamClass::Scalar);
    CB_CHECK(ps[0].semantic.has_value() && ps[0].semantic->empty());
    CB_CHECK(ps[1].cls == ParamClass::Vector && ps[1].columns == 4 && ps[1].semantic == std::string("COLOR0"));
    CB_CHECK_SAME(ps[1].value, (std::vector<double>{1, 0.5, 0.25, 1}));
    CB_CHECK(ps[2].cls == ParamClass::MatrixColumns && ps[2].value.size() == 16);
    CB_CHECK(!ps[3].semantic.has_value() && ps[3].members.empty() && ps[3].value.empty());   // null keys
    CB_CHECK(ps[4].type == ParamType::Sampler);
    CB_CHECK(ps[5].elements == 2 && ps[5].members.size() == 2 && ps[5].members[1].value == std::vector<double>{6});
    CB_CHECK(ps[6].cls == ParamClass::Struct && ps[6].members.size() == 2);
    CB_CHECK_THROWS_AS(cb::ParseError, parse_parameters("[{"));
    CB_CHECK_THROWS_AS(cb::FormatError, parse_parameters("{}"));
    CB_CHECK_THROWS_AS(cb::FormatError, parse_parameters("[1]"));
    CB_CHECK_THROWS_AS(cb::FormatError, parse_parameters(R"([{"name": 3}])"));
    CB_CHECK_THROWS_AS(cb::FormatError, parse_parameters(R"([{"name": "x", "value": ["a"]}])"));
});

// ---- damaged and unsupported input ---------------------------------------------------------------------------

CB_TEST("xna.mgfx.not_an_effect", [] {
    const std::vector<Parameter> one = {param("A", ParamType::Float, ParamClass::Scalar, 1, 1, {1})};
    CB_CHECK_THROWS_AS(cb::FormatError, add_parameters(Bytes{}, one));
    CB_CHECK_THROWS_AS(cb::FormatError, add_parameters(Bytes{'M', 'G', 'F'}, one));
    CB_CHECK_THROWS_AS(cb::FormatError, add_parameters(Bytes{'m', 'g', 'f', 'x', 10, 1}, one));
    CB_CHECK_THROWS_AS(cb::TruncatedError, add_parameters(Bytes{'M', 'G', 'F', 'X'}, one));
    CB_CHECK_THROWS_AS(cb::FormatError, parameter_names(Bytes{}));
});

CB_TEST("xna.mgfx.truncated_at_every_length", [] {
    const Fixture f;
    const std::size_t params_end = f.effect_bytes.size() - kTail.size();   // the reader needs nothing after the list
    const std::vector<Parameter> one = {param("A", ParamType::Float, ParamClass::Scalar, 1, 1, {1})};
    for (std::size_t n = 0; n < f.effect_bytes.size(); ++n) {
        const ByteView cut(f.effect_bytes.data(), n);
        if (n < params_end) {
            bool threw = false;
            try {
                (void)add_parameters(cut, one);
            } catch (const cb::FormatError&) {
                threw = true;
            }
            CB_CHECK(threw);   // (n bytes)
        } else {
            CB_CHECK_NOTHROW(add_parameters(cut, one));
        }
    }
    CB_CHECK_THROWS_AS(cb::TruncatedError, add_parameters(ByteView(f.effect_bytes.data(), params_end - 3), one));
});

CB_TEST("xna.mgfx.annotations_are_not_read", [] {
    const Fixture f;
    Bytes data = f.effect_bytes;
    // the first parameter's annotation count follows its class, type, name and semantic
    const std::string name = "Flag";
    const auto at = std::search(data.begin(), data.end(), name.begin(), name.end());
    CB_CHECK(at != data.end());
    data[static_cast<std::size_t>(at - data.begin()) + name.size() + 1] = 1;
    CB_CHECK_THROWS_AS(cb::UnsupportedError, parameter_names(data));
    CB_CHECK_THROWS_AS(cb::FormatError, add_parameters(data, {}));
});

CB_TEST("xna.mgfx.counts_that_lie", [] {
    const Fixture f;
    // a negative constant-buffer count of parameters, a shader size larger than the file, a string longer than the file
    Bytes negative = effect({{"G", 4, -1}}, {}, plist({}), kTail);
    CB_CHECK_THROWS_AS(cb::FormatError, parameter_names(negative));
    Bytes big_shader = effect({}, {shader(true, "code", {}, {}, {})}, plist({}), kTail);
    big_shader[10 + 4 + 4 + 1 + 3] = 0x7F;   // the code size's high byte
    CB_CHECK_THROWS_AS(cb::TruncatedError, parameter_names(big_shader));
    Bytes long_name = f.effect_bytes;
    const std::string flag = "Flag";
    const auto at = std::search(long_name.begin(), long_name.end(), flag.begin(), flag.end());
    long_name[static_cast<std::size_t>(at - long_name.begin()) - 1] = 0x7F;   // the length byte: 127 where 4 were
    CB_CHECK_THROWS_AS(cb::FormatError, parameter_names(long_name));
    // a length that never ends (a varint of more than 32 bits)
    Bytes varint = effect({}, {}, [] {
        Bytes p;
        put_i32(p, 1);
        p.insert(p.end(), {kScalar, kInt, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00});
        return p;
    }(), kTail);
    CB_CHECK_THROWS_AS(cb::FormatError, parameter_names(varint));
});

CB_TEST("xna.mgfx.nested_too_deeply", [] {
    // an array holding an array holding ... (a hostile file must not exhaust the stack)
    Bytes inner = enc_param(kScalar, kInt, "x", "", 1, 1, {}, {}, {1});
    for (int i = 0; i < 100; ++i) inner = enc_param(kScalar, kInt, "x", "", 1, 1, {inner});
    CB_CHECK_THROWS_AS(cb::FormatError, parameter_names(effect({}, {}, plist({inner}), kTail)));
});

CB_TEST("xna.mgfx.parameters_that_do_not_fit", [] {
    const Fixture f;
    CB_CHECK_THROWS_AS(cb::UsageError, add(f.effect_bytes, {param("R", ParamType::Float, ParamClass::Scalar, 300, 1, {1})}));
    CB_CHECK_THROWS_AS(cb::UsageError, add(f.effect_bytes, {param("C", ParamType::Float, ParamClass::Scalar, 1, 300, {1})}));
    CB_CHECK_THROWS_AS(cb::UsageError, add(f.effect_bytes, {param("V", ParamType::Float, ParamClass::Vector, 0, 256, {1})}));
    CB_CHECK_NOTHROW(add(f.effect_bytes, {param("Ok", ParamType::Float, ParamClass::Scalar, 255, 1, {1})}));
    CB_CHECK_THROWS_AS(cb::UsageError, add(f.effect_bytes, {param("Big", ParamType::Float, ParamClass::Vector, 0, 1, {1e39})}));
    CB_CHECK_THROWS_AS(cb::UsageError, add(f.effect_bytes, {param("Neg", ParamType::Float, ParamClass::Vector, 0, 1, {-1e39})}));
    // the 32-bit float range ends at 3.4028235e38: that itself is fine
    CB_CHECK_NOTHROW(add(f.effect_bytes, {param("Max", ParamType::Float, ParamClass::Vector, 0, 1, {3.4028234663852886e38})}));
    // a vector's columns come from `columns`: a vector without any writes none
    CB_CHECK_SAME(add(f.effect_bytes, {param("V0", ParamType::Float, ParamClass::Vector, 0, 0, {1})}),
                with_more(f, {enc_param(kVector, kFloat, "V0", "", 1, 0)}));
});

// ---- the reference implementation's own cases (optional: needs the harness folder) ---------------------------

// state/work/typed-port/xna.mgfx holds 4.6 MB of calls recorded from the Python (cases.txt, one JSON call per line,
// bytes as hex; expected.txt, the results). Point CB_XNA_MGFX_HARNESS at that folder to replay the add_parameters
// calls: every answer must be the same bytes, and every error must be an error here too -- except the cases listed as
// DIVERGES below.
//
// DIVERGES (old behaviour -> now):
//   * a name or semantic with invalid UTF-8 raised UnicodeDecodeError; here it is kept as the bytes it is.
//   * a string length that ran past the end was clamped (a shorter name, no error); here it is a TruncatedError.
//   * a negative size or count that moved the cursor backwards (an endless or wrong read) is a FormatError.
//   * a length varint of more than 5 bytes (a Python big integer) is a FormatError.
//   * a number that overflowed a 32-bit float raised OverflowError; here too (UsageError); inf and NaN pass. (The replay
//     writes Python's inf as the largest double, which is refused: 39 of the "only this raised" cases.)
//   * a skip that runs past the end of the data (a shader or constant-buffer size larger than the file) was not an
//     error until something read after it; here it is a TruncatedError at the skip (54 cases).
//   * a vector parameter without `columns` raised struct.error (None is not a byte); here it has 0 columns (31 cases).
CB_TEST("xna.mgfx.replays_the_reference_cases", [] {
    const auto dir = cb::fs::get_env("CB_XNA_MGFX_HARNESS");
    if (!dir) return;
    const std::string cases = cb::fs::read_text(std::filesystem::path(*dir) / "cases.txt");
    const std::string expected = cb::fs::read_text(std::filesystem::path(*dir) / "expected.txt");
    std::map<std::string, std::size_t> kinds;
    auto shape = [](std::string m) {   // a message without its numbers and the parameter's name
        for (char& c : m) if (c >= '0' && c <= '9') c = '#';
        return m.substr(0, m.find(':', m.find(':') + 1));
    };
    std::size_t at_c = 0, at_e = 0, compared = 0, both_error = 0, mismatched = 0, py_only_error = 0, cpp_only_error = 0;
    auto next_line = [](const std::string& s, std::size_t& at) {
        const std::size_t e = s.find('\n', at);
        std::string line = s.substr(at, e == std::string::npos ? std::string::npos : e - at);
        at = e == std::string::npos ? s.size() : e + 1;
        return line;
    };
    auto unhex = [](const std::string& h) {
        Bytes b;
        for (std::size_t i = 0; i + 1 < h.size(); i += 2) b.push_back(static_cast<std::uint8_t>(std::stoi(h.substr(i, 2), nullptr, 16)));
        return b;
    };
    auto hex_of = [](const Bytes& b) {
        static const char* d = "0123456789abcdef";
        std::string s;
        for (std::uint8_t c : b) s += {d[c >> 4], d[c & 15]};
        return s;
    };
    while (at_c < cases.size()) {
        std::string text = next_line(cases, at_c);
        for (std::size_t p; (p = text.find("Infinity")) != std::string::npos;) text.replace(p, 8, "1.7976931348623157e308");   // (Python writes inf so; the largest double stands in, which the module refuses)
        const nlohmann::json call = nlohmann::json::parse(text);
        const nlohmann::json want = nlohmann::json::parse(next_line(expected, at_e));
        if (call[0] != "add_parameters") continue;
        const Bytes data = unhex(call[1][0].get<std::string>());
        std::vector<Parameter> ps;
        for (const auto& j : call[1][1]) {
            nlohmann::json one = j;
            for (const char* k : {"rows", "columns", "elements"})   // a count the python could not write (negative)
                if (one.contains(k) && one[k].is_number() && one[k] < 0) one[k] = 4294967295.0;
            std::vector<Parameter> parsed = parse_parameters("[" + one.dump() + "]");
            ps.push_back(std::move(parsed[0]));
        }
        ++compared;
        try {
            const Bytes got = add_parameters(data, ps);
            if (want.is_string() && hex_of(got) == want.get<std::string>()) continue;
            if (want.is_object()) {
                ++py_only_error;
                ++kinds["python only: " + want["error"].get<std::string>()];
            } else {
                ++mismatched;   // the bytes differ
            }
        } catch (const cb::Error& e) {
            if (want.is_object()) {
                ++both_error;
            } else {
                ++cpp_only_error;
                ++kinds["here only: " + shape(e.what())];
            }
        }
    }
    std::fprintf(stderr, "replayed %zu calls: %zu errors on both sides, %zu only the python raised, %zu only this raised (see DIVERGES), %zu different bytes\n",
                 compared, both_error, py_only_error, cpp_only_error, mismatched);
    for (const auto& [kind, n] : kinds) std::fprintf(stderr, "  %6zu  %s\n", n, kind.c_str());
    CB_CHECK_EQ(mismatched, std::size_t{0});
});
