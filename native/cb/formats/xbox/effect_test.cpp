#include "cb/formats/xbox/effect.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
namespace fxf = cb::formats::xbox::effect;
namespace d3dx = cb::formats::xbox::d3dx;

Bytes effect_file(const char* name) {
    return fs::read_file(testing::data_path(std::string("decompile/testdata/xbox_effect/files/") + name));
}

const fxf::Parameter& param(const fxf::Effect& fx, std::string_view name) {
    const fxf::Parameter* p = fx.parameter(name);
    CB_ASSERT(p != nullptr, "the effect has no such parameter");
    return *p;
}

}  // namespace

// The expected values are what the SDK compiled (tests/data/xbox_effects/sprite.fx beside the .fxo) and the recorded reader output
// read from it, checked against the .fx source.
CB_TEST("formats.xbox.effect.sprite_effect_little_endian", [] {
    const fxf::Effect fx = fxf::read(effect_file("fxo_sprite.fxo"));
    CB_CHECK(!fx.big_endian);
    CB_CHECK_EQ(fx.parameters.size(), 5u);

    const fxf::Parameter& wvp = param(fx, "WorldViewProj");
    CB_CHECK(wvp.type == d3dx::ParamType::Float);
    CB_CHECK(wvp.cls == d3dx::ParamClass::MatrixColumns);
    CB_CHECK_EQ(wvp.rows, 4u);
    CB_CHECK_EQ(wvp.columns, 4u);
    CB_CHECK_EQ(wvp.floats().size(), 16u);
    CB_CHECK_EQ(wvp.size(), 64u);

    const fxf::Parameter& tint = param(fx, "Tint");
    CB_CHECK(tint.cls == d3dx::ParamClass::Vector);
    CB_CHECK_EQ(tint.columns, 4u);   // a vector: columns, then rows
    CB_CHECK_EQ(tint.rows, 1u);
    CB_CHECK_EQ(tint.floats()[0], 1.0f);
    CB_CHECK_EQ(tint.floats()[1], 0.5f);
    CB_CHECK_EQ(tint.floats()[2], 0.25f);
    CB_CHECK_EQ(param(fx, "Brightness").floats()[0], 2.0f);
    CB_CHECK(param(fx, "Brightness").ints().empty());

    const fxf::Parameter& diffuse = param(fx, "Diffuse");   // a texture: its object id
    CB_CHECK(d3dx::is_texture_type(diffuse.type));
    CB_CHECK(diffuse.object().has_value());
    CB_CHECK_EQ(diffuse.object()->id, 1u);
    CB_CHECK(fx.object(*diffuse.object()) != nullptr);
    CB_CHECK(fx.object(*diffuse.object())->empty());

    const fxf::Parameter& sampler = param(fx, "DiffuseSampler");
    CB_CHECK(d3dx::is_sampler_type(sampler.type));
    CB_CHECK_EQ(sampler.states.size(), 5u);
    CB_CHECK_EQ(sampler.states[0].name, "Texture");
    CB_CHECK(!sampler.states[0].text());   // an object has no text
    CB_CHECK_EQ(*sampler.states[1].text(), "Linear");   // MinFilter
    CB_CHECK_EQ(*sampler.states[2].text(), "Linear");   // MagFilter
    CB_CHECK_EQ(*sampler.states[3].text(), "Clamp");    // AddressU
    CB_CHECK_EQ(*sampler.states[4].text(), "Wrap");     // AddressV

    CB_CHECK_EQ(fx.techniques.size(), 1u);
    const fxf::Technique& t = fx.techniques[0];
    CB_CHECK_EQ(t.name, "Probe");
    CB_CHECK_EQ(t.passes.size(), 1u);
    const fxf::Pass& pass = t.passes[0];
    CB_CHECK_EQ(pass.name, "P0");
    CB_CHECK_EQ(pass.states.size(), 6u);
    CB_CHECK_EQ(pass.states[0].name, "AlphaBlendEnable");
    CB_CHECK_EQ(*pass.states[0].text(), "true");
    CB_CHECK_EQ(*pass.state("SrcBlend")->text(), "SrcAlpha");
    CB_CHECK_EQ(*pass.state("DestBlend")->text(), "InvSrcAlpha");
    CB_CHECK_EQ(*pass.state("CullMode")->text(), "None");
    CB_CHECK(!pass.state("VertexShader")->text());
    CB_CHECK_EQ(pass.state("VertexShader")->parameter.object()->id, 3u);
    CB_CHECK_EQ(pass.state("PixelShader")->parameter.object()->id, 4u);
    CB_CHECK(pass.state("ZEnable") == nullptr);

    // the shaders are the objects the passes name
    CB_CHECK_EQ(fx.objects.size(), 3u);
    CB_CHECK_EQ(fx.object({3})->size(), 488u);
    CB_CHECK_EQ(fx.object({4})->size(), 432u);
    CB_CHECK(fx.object({99}) == nullptr);
    CB_CHECK_EQ(fx.string({99}), "");
});

CB_TEST("formats.xbox.effect.xna_content_is_big_endian_and_may_carry_its_header", [] {
    const Bytes with_header = effect_file("xnb_game_AlphaToBlack.bin");   // 0xBCF00BCF, 0x10, then the effect
    CB_CHECK_EQ(with_header[0], 0xBC);
    const Bytes raw(with_header.begin() + 16, with_header.end());
    const fxf::Effect fx = fxf::read(raw);
    CB_CHECK_EQ(fxf::read(with_header).objects.size(), 1u);   // the header is taken off
    CB_CHECK(fx.big_endian);
    CB_CHECK_EQ(fx.parameters.size(), 1u);
    CB_CHECK_EQ(fx.parameters[0].name, "ColorSampler");
    CB_CHECK(fx.parameters[0].states.empty());
    CB_CHECK_EQ(fx.techniques[0].name, "Technique1");
    CB_CHECK_EQ(fx.techniques[0].passes[0].name, "Pass1");
    CB_CHECK_EQ(fx.techniques[0].passes[0].states[0].name, "PixelShader");
    CB_CHECK_EQ(fx.objects.size(), 1u);

    // the same effect with XNA's header in front (0xBCF00BCF and the offset of the Direct3D effect), either way round
    for (const bool big : {true, false}) {
        Bytes wrapped;
        const auto word = [&](std::uint32_t v) {
            for (int i = 0; i < 4; ++i) wrapped.push_back(static_cast<std::uint8_t>(v >> (big ? 24 - 8 * i : 8 * i)));
        };
        word(fxf::kXnaMagic);
        word(8);
        wrapped.insert(wrapped.end(), raw.begin(), raw.end());
        const fxf::Effect again = fxf::read(wrapped);
        CB_CHECK_EQ(again.parameters.size(), 1u);
        CB_CHECK_EQ(again.objects.at(1).size(), fx.objects.at(1).size());
    }
});

CB_TEST("formats.xbox.effect.arrays_hold_their_elements", [] {
    // the SDK's BasicEffect, as XNA compiled it for the console: arrays of shaders chosen by an expression
    const fxf::Effect fx = fxf::read(effect_file("bw_BasicClipEffect360.bin"));
    const fxf::Parameter& vs = param(fx, "VSArray");
    CB_CHECK(vs.type == d3dx::ParamType::VertexShader);
    CB_CHECK_EQ(vs.elements, 20u);
    CB_CHECK_EQ(vs.members.size(), 20u);
    for (std::size_t i = 0; i < vs.members.size(); ++i) {
        CB_CHECK(vs.members[i].object().has_value());
        CB_CHECK(fx.object(*vs.members[i].object()) != nullptr);
        CB_CHECK_EQ(vs.members[i].elements, 0u);   // an element is not itself an array
    }
    const fxf::Parameter& indices = param(fx, "VSIndices");
    CB_CHECK_EQ(indices.elements, 32u);
    CB_CHECK_EQ(indices.members.size(), 32u);
    CB_CHECK_EQ(indices.members[1].ints()[0], 1);
    CB_CHECK_EQ(indices.members[8].ints()[0], 8);
    CB_CHECK_EQ(indices.members[9].ints()[0], 8);   // two selector values share a shader
    CB_CHECK_EQ(param(fx, "ShaderIndex").ints()[0], 0);
    CB_CHECK_EQ(indices.size(), 32u * 4u);

    // a pass whose shader is an expression: the preshader is kept, not evaluated
    const fxf::State* s = fx.techniques[0].passes[0].state("VertexShader");
    CB_CHECK(s != nullptr);
    CB_CHECK(s->parameter.expression.has_value());
    CB_CHECK(!s->parameter.expression->empty());
});

CB_TEST("formats.xbox.effect.state_names_and_texts", [] {
    CB_CHECK_EQ(fxf::state_name(0), "ZEnable");
    CB_CHECK_EQ(fxf::state_name(10), "AlphaBlendEnable");
    CB_CHECK_EQ(fxf::state_name(19), "Wrap0");
    CB_CHECK_EQ(fxf::state_name(34), "Wrap15");
    CB_CHECK_EQ(fxf::state_name(35), "ClipPlaneEnable");
    CB_CHECK_EQ(fxf::state_name(52), "CCW_StencilFunc");
    CB_CHECK_EQ(fxf::state_name(56), "ColorWriteEnable1");
    CB_CHECK_EQ(fxf::state_name(92), "VertexShader");
    CB_CHECK_EQ(fxf::state_name(93), "PixelShader");
    CB_CHECK_EQ(fxf::state_name(120), "MaxAnisotropy");
    CB_CHECK_EQ(fxf::state_name(55), "state_55");   // the Xbox's own states the SDK's fxc had no name for
    CB_CHECK_EQ(fxf::state_name(4000), "state_4000");

    const auto state_with = [](std::uint32_t op, d3dx::ParamType type, fxf::Value value) {
        fxf::State s;
        s.operation = op;
        s.name = fxf::state_name(op);
        s.parameter.type = type;
        s.parameter.value = std::move(value);
        return s;
    };
    using Ints = std::vector<std::int32_t>;
    using Floats = std::vector<float>;
    CB_CHECK_EQ(*state_with(4, d3dx::ParamType::Int, Ints{6}).text(), "SrcAlpha");
    CB_CHECK_EQ(*state_with(4, d3dx::ParamType::Int, Ints{12}).text(), "BlendFactor");
    CB_CHECK_EQ(*state_with(4, d3dx::ParamType::Int, Ints{14}).text(), "BlendFactor");   // the Xbox's constant alpha
    CB_CHECK_EQ(*state_with(4, d3dx::ParamType::Int, Ints{99}).text(), "99");              // not in the table: the number
    CB_CHECK_EQ(*state_with(1, d3dx::ParamType::Int, Ints{0x25}).text(), "WireFrame");
    CB_CHECK_EQ(*state_with(6, d3dx::ParamType::Int, Ints{2}).text(), "CW");
    CB_CHECK_EQ(*state_with(6, d3dx::ParamType::Int, Ints{6}).text(), "CCW");
    CB_CHECK_EQ(*state_with(7, d3dx::ParamType::Int, Ints{3}).text(), "LessEqual");
    CB_CHECK_EQ(*state_with(0, d3dx::ParamType::Int, Ints{1}).text(), "true");     // ZEnable: a flag
    CB_CHECK_EQ(*state_with(0, d3dx::ParamType::Int, Ints{0}).text(), "false");
    CB_CHECK_EQ(*state_with(48, d3dx::ParamType::Int, Ints{1}).text(), "true");    // TwoSidedStencilMode
    CB_CHECK_EQ(*state_with(16, d3dx::ParamType::Int, Ints{255}).text(), "255");   // StencilRef: a number
    CB_CHECK_EQ(*state_with(16, d3dx::ParamType::Int, Ints{-1}).text(), "-1");
    CB_CHECK_EQ(*state_with(60, d3dx::ParamType::Float, Floats{0.5f}).text(), "0.5");   // DepthBias
    CB_CHECK_EQ(*state_with(60, d3dx::ParamType::Float, Floats{1.0f}).text(), "1.0");
    CB_CHECK_EQ(*state_with(60, d3dx::ParamType::Float, Floats{0.1f}).text(), "0.1");   // the float's own shortest digits
    CB_CHECK_EQ(*state_with(60, d3dx::ParamType::Float, Floats{-3.0f}).text(), "-3.0");
    CB_CHECK_EQ(*state_with(0, d3dx::ParamType::Bool, Ints{1}).text(), "true");
    // anything that is not exactly one number has no text
    CB_CHECK(!state_with(4, d3dx::ParamType::Int, Ints{}).text());
    CB_CHECK(!state_with(4, d3dx::ParamType::Int, Ints{1, 2}).text());
    CB_CHECK(!state_with(92, d3dx::ParamType::VertexShader, fxf::ObjectId{3}).text());
    CB_CHECK(!state_with(92, d3dx::ParamType::VertexShader, std::monostate{}).text());
});

CB_TEST("formats.xbox.effect.damaged_effects_are_format_errors", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)fxf::read(ByteView()));
    CB_CHECK_THROWS_AS(FormatError, (void)fxf::read(as_bytes("not an effect at all")));
    const Bytes data = effect_file("fxo_sprite.fxo");
    // an effect cut anywhere is an error (a read past the end), never a read of what is not there -- but for the last object,
    // which gives what of it is there (the shader inside is checked when it is read): the final bytes may go without a throw
    for (std::size_t cut = 0; cut < data.size(); cut += 7) {
        bool threw = false;
        try {
            (void)fxf::read(ByteView(data.data(), cut));
        } catch (const FormatError&) {
            threw = true;
        }
        if (cut + 440 <= data.size()) CB_CHECK(threw);   // (the last object, a 432-byte shader, and its padding)
    }
    // an XNA header whose offset lies past the end
    Bytes wrapped = {0xBC, 0xF0, 0x0B, 0xCF, 0x7F, 0x00, 0x00, 0x00, 1, 2, 3, 4};
    CB_CHECK_THROWS_AS(FormatError, (void)fxf::read(wrapped));
});
