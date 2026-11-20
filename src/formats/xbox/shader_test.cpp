#include "formats/xbox/shader.hpp"

#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/xbox/effect.hpp"

namespace {

using namespace cb;
namespace shf = cb::formats::xbox::shader;
namespace fxf = cb::formats::xbox::effect;
namespace d3dx = cb::formats::xbox::d3dx;

Bytes effect_file(const char* name) {
    return fs::read_file(testing::data_path(std::string("decompile/testdata/xbox_effect/files/") + name));
}

// The two shaders of the SDK sample effect `sprite` (compiled by the SDK's fxc), read through the effect that holds them.
struct Sprite {
    Bytes vertex, pixel;
};
Sprite sprite_shaders() {
    const fxf::Effect fx = fxf::read(effect_file("fxo_sprite.fxo"));
    Sprite s;
    s.vertex = fx.objects.at(3);
    s.pixel = fx.objects.at(4);
    return s;
}

}  // namespace

// The expected values are what the SDK's listing and the recorded reader output gave for this effect, read through by hand.
CB_TEST("formats.xbox.shader.pixel_shader_of_the_sprite_effect", [] {
    const Sprite sp = sprite_shaders();
    CB_CHECK(shf::is_container(sp.pixel));
    const shf::Shader s = shf::read(sp.pixel);
    CB_CHECK(s.stage == shf::Stage::Pixel);
    CB_CHECK_EQ(s.target, "ps_3_0");
    CB_CHECK_EQ(s.words.size(), 21u);
    CB_CHECK_EQ(s.constants.size(), 2u);
    const shf::Constant& brightness = s.constants[0];
    CB_CHECK_EQ(brightness.name, "Brightness");
    CB_CHECK(brightness.set == shf::RegisterSet::Float4);
    CB_CHECK_EQ(brightness.first_register, 0);
    CB_CHECK_EQ(brightness.count, 1);
    CB_CHECK(brightness.cls == d3dx::ParamClass::Scalar);
    CB_CHECK(brightness.type == d3dx::ParamType::Float);
    CB_CHECK_EQ(brightness.defaults.size(), 4u);
    CB_CHECK_EQ(brightness.defaults[0], 2.0f);
    const shf::Constant& sampler = s.constants[1];
    CB_CHECK_EQ(sampler.name, "DiffuseSampler");
    CB_CHECK(sampler.set == shf::RegisterSet::Sampler);
    CB_CHECK(sampler.cls == d3dx::ParamClass::Object);
    CB_CHECK(sampler.type == d3dx::ParamType::Sampler2D);
    CB_CHECK(sampler.defaults.empty());   // no default in the table

    // the compiler's own constants: c255 = (0.1, 0.2, 0.3, 0) among them, the pixel shader's registers counted from 256
    CB_CHECK_EQ(s.literals.size(), 4u);
    CB_CHECK_EQ(s.literals.at(255)[0], 0.1f);
    CB_CHECK_EQ(s.literals.at(255)[2], 0.3f);
    CB_CHECK(s.loop_literals.empty());

    // inputs (the interpolators) and the pixel shader's control words
    CB_CHECK_EQ(s.pixel_inputs.size(), 2u);
    CB_CHECK_EQ(s.pixel_inputs[0].reg, 0u);
    CB_CHECK_EQ(s.pixel_inputs[0].usage, "TEXCOORD");
    CB_CHECK_EQ(s.pixel_inputs[0].mask, 3u);
    CB_CHECK_EQ(s.pixel_inputs[1].usage, "COLOR");
    CB_CHECK_EQ(s.pixel_inputs[1].mask, 15u);
    CB_CHECK_EQ(s.interpolator_control, 65539u);
    CB_CHECK_EQ(s.output_mask, 1u);
    CB_CHECK(s.vertex_inputs.empty());
    CB_CHECK(s.outputs.empty());

    const auto at = s.constant_at(shf::RegisterSet::Float4, 0);
    CB_CHECK(at.constant == &brightness);
    CB_CHECK(s.constant_at(shf::RegisterSet::Float4, 1).constant == nullptr);
    CB_CHECK(s.constant_at(shf::RegisterSet::Sampler, 0).constant == &sampler);
});

CB_TEST("formats.xbox.shader.vertex_shader_of_the_sprite_effect", [] {
    const Sprite sp = sprite_shaders();
    const shf::Shader s = shf::read(sp.vertex);
    CB_CHECK(s.stage == shf::Stage::Vertex);
    CB_CHECK_EQ(s.target, "vs_3_0");
    CB_CHECK_EQ(s.words.size(), 39u);
    CB_CHECK_EQ(s.constants.size(), 2u);
    CB_CHECK_EQ(s.constants[0].name, "Tint");
    CB_CHECK_EQ(s.constants[0].first_register, 4);
    CB_CHECK(s.constants[0].cls == d3dx::ParamClass::Vector);
    CB_CHECK_EQ(s.constants[0].columns, 4);
    CB_CHECK_EQ(s.constants[0].defaults[1], 0.5f);
    CB_CHECK_EQ(s.constants[1].name, "WorldViewProj");
    CB_CHECK_EQ(s.constants[1].count, 4);
    CB_CHECK(s.constants[1].cls == d3dx::ParamClass::MatrixColumns);
    CB_CHECK_EQ(s.constants[1].defaults.size(), 16u);
    CB_CHECK(s.literals.empty());

    CB_CHECK_EQ(s.vertex_inputs.size(), 3u);
    CB_CHECK_EQ(s.vertex_inputs[0].slot, 3u);
    CB_CHECK_EQ(s.vertex_inputs[0].usage, "POSITION");
    CB_CHECK_EQ(s.vertex_inputs[1].usage, "TEXCOORD");
    CB_CHECK_EQ(s.vertex_inputs[2].usage, "COLOR");
    CB_CHECK_EQ(s.outputs.size(), 2u);
    CB_CHECK_EQ(s.outputs[0].usage, "TEXCOORD");
    CB_CHECK_EQ(s.outputs[0].mask, 3u);
    CB_CHECK_EQ(s.outputs[1].usage, "COLOR");
    CB_CHECK_EQ(s.outputs[1].mask, 15u);
    CB_CHECK_EQ(s.output_writers.size(), 2u);
    CB_CHECK_EQ(s.output_writers[0], 10u);
    CB_CHECK_EQ(s.output_writers[1], 11u);
});

CB_TEST("formats.xbox.shader.loops_are_read_from_the_literal_records", [] {
    // the effect `literal_loop`: its pixel shader has two loop constants of the compiler's making (`defi i30, 3`, `defi i31, 10`)
    const fxf::Effect fx = fxf::read(effect_file("fxo_literal_loop.fxo"));
    const shf::Shader pixel = shf::read(fx.objects.at(2));
    CB_CHECK(pixel.stage == shf::Stage::Pixel);
    CB_CHECK_EQ(pixel.loop_literals.size(), 2u);
    CB_CHECK_EQ(pixel.loop_literals.at(30), 3u);
    CB_CHECK_EQ(pixel.loop_literals.at(31), 10u);
    CB_CHECK_EQ(pixel.literals.size(), 4u);
    const shf::Shader vertex = shf::read(fx.objects.at(1));
    CB_CHECK(vertex.loop_literals.empty());
});

// The tables fxc's listing (/Fc /Xudsrd) gives for the SDK sample effect `skinned`: the vertex shader's inputs and exports, the constant
// table and the compiler's literals; the pixel shader's interpolators.
CB_TEST("formats.xbox.shader.the_tables_of_the_skinned_effect_are_what_fxc_lists", [] {
    const fxf::Effect fx = fxf::read(effect_file("fxo_skinned.fxo"));
    std::vector<shf::Shader> vertex, pixel;
    for (const auto& [id, bytes] : fx.objects) {
        if (!shf::is_container(bytes)) continue;
        shf::Shader s = shf::read(bytes);
        (s.stage == shf::Stage::Vertex ? vertex : pixel).push_back(std::move(s));
    }
    CB_CHECK_EQ(vertex.size(), 1u);
    CB_CHECK_EQ(pixel.size(), 1u);
    const shf::Shader& vs = vertex[0];
    const shf::Shader& ps = pixel[0];

    const std::vector<std::tuple<std::uint32_t, std::string, std::uint32_t>> inputs = {
        {5, "POSITION", 0}, {6, "NORMAL", 0}, {7, "TEXCOORD", 0}, {8, "TEXCOORD", 3}, {9, "COLOR", 1}, {10, "BLENDINDICES", 0}, {11, "BLENDWEIGHT", 0}};
    CB_CHECK_EQ(vs.vertex_inputs.size(), inputs.size());
    for (std::size_t i = 0; i < inputs.size() && i < vs.vertex_inputs.size(); ++i) {
        CB_CHECK_EQ(vs.vertex_inputs[i].slot, std::get<0>(inputs[i]));
        CB_CHECK_EQ(vs.vertex_inputs[i].usage, std::get<1>(inputs[i]));
        CB_CHECK_EQ(vs.vertex_inputs[i].index, std::get<2>(inputs[i]));
    }
    const std::vector<std::tuple<std::uint32_t, std::string, std::uint32_t, std::uint32_t>> exports = {
        {0, "TEXCOORD", 0, 3}, {1, "TEXCOORD", 1, 7}, {2, "TEXCOORD", 2, 1}, {3, "TEXCOORD", 5, 3}, {4, "COLOR", 0, 15}};
    CB_CHECK_EQ(vs.outputs.size(), exports.size());
    for (std::size_t i = 0; i < exports.size() && i < vs.outputs.size(); ++i) {
        CB_CHECK_EQ(vs.outputs[i].reg, std::get<0>(exports[i]));
        CB_CHECK_EQ(vs.outputs[i].usage, std::get<1>(exports[i]));
        CB_CHECK_EQ(vs.outputs[i].index, std::get<2>(exports[i]));
        CB_CHECK_EQ(vs.outputs[i].mask, std::get<3>(exports[i]));
    }
    std::vector<shf::Constant> table = vs.constants;
    std::sort(table.begin(), table.end(), [](const shf::Constant& a, const shf::Constant& b) { return a.first_register < b.first_register; });
    const std::vector<std::tuple<std::string, int, int>> constants = {{"World", 0, 4}, {"ViewProj", 4, 4}, {"Bones", 8, 60}, {"UseFog", 68, 1}};
    CB_CHECK_EQ(table.size(), constants.size());
    for (std::size_t i = 0; i < constants.size() && i < table.size(); ++i) {
        CB_CHECK_EQ(table[i].name, std::get<0>(constants[i]));
        CB_CHECK_EQ(static_cast<int>(table[i].first_register), std::get<1>(constants[i]));
        CB_CHECK_EQ(static_cast<int>(table[i].count), std::get<2>(constants[i]));
    }
    CB_CHECK_EQ(vs.literals.at(255)[0], 3.0f);
    CB_CHECK(std::abs(vs.literals.at(255)[2] - 0.01f) < 1e-7f);

    CB_CHECK_EQ(ps.pixel_inputs.size(), 5u);
    for (std::size_t i = 0; i < ps.pixel_inputs.size(); ++i) CB_CHECK_EQ(ps.pixel_inputs[i].reg, static_cast<std::uint32_t>(i));
    CB_CHECK_EQ(ps.pixel_inputs[3].usage, "TEXCOORD");
    CB_CHECK_EQ(ps.pixel_inputs[3].index, 5u);
});

CB_TEST("formats.xbox.shader.damaged_containers_are_format_errors", [] {
    const Sprite sp = sprite_shaders();
    CB_CHECK(!shf::is_container(ByteView()));
    CB_CHECK(!shf::is_container(as_bytes("MZ\x90\x00")));
    CB_CHECK_THROWS_AS(FormatError, (void)shf::read(as_bytes("MZ\x90\x00 and more")));   // not a shader
    CB_CHECK_THROWS_AS(TruncatedError, (void)shf::read(ByteView()));

    // cut at any length the container is damaged: an error, never a read past the end (the ASan build checks that too)
    for (const Bytes* blob : {&sp.vertex, &sp.pixel}) {
        for (std::size_t cut = 0; cut < blob->size(); ++cut) {
            bool threw = false;
            try {
                (void)shf::read(ByteView(blob->data(), cut));
            } catch (const FormatError&) {
                threw = true;
            }
            CB_CHECK(threw);
        }
    }

    // offsets that lie: the constant table, the literal records and the program information pointing past the end
    for (const std::size_t field : {16u, 20u, 24u}) {
        Bytes bad = sp.pixel;
        bad[field] = 0x7F;   // the offset's high byte: far past the end
        CB_CHECK_THROWS_AS(FormatError, (void)shf::read(bad));
    }
    // a vertex shader that lists more inputs than its program information holds
    Bytes bad = sp.vertex;
    const std::size_t info_at = (std::size_t{bad[24]} << 24) | (std::size_t{bad[25]} << 16) | (std::size_t{bad[26]} << 8) | bad[27];
    bad[info_at + 7 * 4 + 3] = 0xFF;   // the input count
    CB_CHECK_THROWS_AS(FormatError, (void)shf::read(bad));
});
