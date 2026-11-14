#include "cb/decompile/xbox_effect.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/xenos_hlsl.hpp"

namespace {

using namespace cb;
namespace xe = cb::decompile::xbox_effect;

const char* const kFiles = "decompile/testdata/xbox_effect/files/";

Bytes effect_file(const std::string& name) { return fs::read_file(testing::data_path(kFiles + name)); }

}  // namespace

// The SDK sample effect `sprite` (tests/data/xbox_effects/sprite.fx; fxc's output beside it is the input here): the .fx text
// it comes back as, read through against the source: the parameters with their defaults, the sampler and its states, the two
// shaders, and the technique with its render states.
CB_TEST("decompile.xbox_effect.sprite_effect_comes_back_as_its_fx_source", [] {
    const std::string fx = xe::to_fx(effect_file("fxo_sprite.fxo"), "sprite");
    const auto lines = text::split(fx, '\n');
    CB_CHECK_EQ(lines[0], "// sprite: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect) from its microcode.");
    CB_CHECK_EQ(lines[1], "// The shaders read as the compiled programs did: registers r0..rN, the constants by their names.");
    CB_CHECK_EQ(lines[2], "");
    const auto has = [&](std::string_view line) { return std::find(lines.begin(), lines.end(), line) != lines.end(); };
    CB_CHECK(has("float4x4 WorldViewProj;"));            // all zero: no initializer
    CB_CHECK(has("float4 Tint = float4(1.0, 0.5, 0.25, 1.0);"));
    CB_CHECK(has("float Brightness = 2.0;"));
    CB_CHECK(has("texture2D Diffuse;") || has("texture Diffuse;"));
    CB_CHECK(fx.find("sampler DiffuseSampler = sampler_state\n{\n    Texture = <Diffuse>;\n    MinFilter = Linear;\n    MagFilter = Linear;\n    AddressU = Clamp;\n    AddressV = Wrap;\n};\n") != std::string::npos);
    CB_CHECK(fx.find("VertexShader = compile vs_3_0 VS_Probe_P0();") != std::string::npos);
    CB_CHECK(fx.find("PixelShader = compile ps_3_0 PS_Probe_P0();") != std::string::npos);
    CB_CHECK(fx.find("technique Probe\n{\n    pass P0\n    {\n        AlphaBlendEnable = true;\n        SrcBlend = SrcAlpha;\n        DestBlend = InvSrcAlpha;\n        CullMode = None;\n") != std::string::npos);
    CB_CHECK(fx.ends_with("    }\n}\n"));
});

CB_TEST("decompile.xbox_effect.default_name_and_damaged_input", [] {
    const std::string fx = xe::to_fx(effect_file("fxo_sprite.fxo"));
    CB_CHECK(fx.starts_with("// Effect: an Xbox 360 effect"));
    CB_CHECK_THROWS_AS(FormatError, (void)xe::to_fx(as_bytes("not an effect")));
    CB_CHECK_THROWS_AS(FormatError, (void)xe::to_fx(Bytes()));
});

CB_TEST("decompile.xbox_effect.untranslated_shaders_name_the_shader_and_instruction", [] {
    // `branch` has a control-flow instruction the translator does not do (a call, made by the harness)
    // (the corpus below holds the cases; this one names what the message must say)
    const Bytes data = effect_file("mut_fxo_branch.fxo_3.bin");
    try {
        (void)xe::to_fx(data, "branch");
        CB_CHECK(false);
    } catch (const decompile::xenos_hlsl::Unsupported& e) {
        CB_CHECK(e.shader().find("VS_Branch_P0") != std::string::npos || e.shader().find("PS_Branch_P0") != std::string::npos);
    }
});

// A pass whose shaders come from arrays (XNA's BasicEffect) is one technique per selector value.
CB_TEST("decompile.xbox_effect.stock_effect_becomes_one_technique_per_shader_index", [] {
    const std::string fx = xe::to_fx(effect_file("bw_BasicClipEffect360.bin"), "BasicEffect");
    CB_CHECK(fx.find("// BasicEffect__<n>: the shaders ShaderIndex n chooses") != std::string::npos);
    CB_CHECK(fx.find("technique BasicEffect\n{") != std::string::npos);
    CB_CHECK(fx.find("technique BasicEffect__1\n{") != std::string::npos);
    CB_CHECK(fx.find("technique BasicEffect__31\n{") != std::string::npos);
    CB_CHECK(fx.find("technique BasicEffect__32\n{") == std::string::npos);
    CB_CHECK(fx.find("VertexShader = compile vs_3_0 VSArray_0();") != std::string::npos);
    CB_CHECK(fx.find("PixelShader = compile ps_3_0 PSArray_0();") != std::string::npos);
    // a shader both selectors share is one function
    std::size_t defs = 0;
    for (std::size_t at = fx.find("VSArray_8("); at != std::string::npos; at = fx.find("VSArray_8(", at + 1)) ++defs;
    CB_CHECK(defs >= 2);   // the definition and a use
});

// An old harness: 44 effects (the SDK's fxc output, the Block
// World's, and the XNA effects of the converted games) each cut at a number of lengths and named a few ways, 484 cases. The
// outputs are the recorded ones, and what is expected to be the same is the .fx text (but for the file's own name in its first
// comment and the digits of a float state); a case refused in the recording is refused (a FormatError for a damaged file, the
// translator's Unsupported for what it does not do).
CB_TEST("decompile.xbox_effect.old_harness_cases", [] {
    const std::string dir = "decompile/testdata/xbox_effect/";
    const std::string cases = fs::read_text(testing::data_path(dir + "cases.txt"));
    const std::string expected = fs::read_text(testing::data_path(dir + "expected.txt"));
    const auto case_lines = text::lines(cases);
    const auto expected_lines = text::lines(expected);
    CB_CHECK_EQ(case_lines.size(), expected_lines.size());
    std::size_t ok = 0, refused = 0;
    std::string failures;   // what went wrong, case by case, so one run says all of it
    for (std::size_t i = 0; i < case_lines.size() && i < expected_lines.size(); ++i) {
        const auto fields = text::split(case_lines[i], '\t');
        CB_CHECK_EQ(fields.size(), 3u);
        const Bytes whole = effect_file(std::string(fields[0]));
        const long cut = std::stol(std::string(fields[2]));
        const ByteView data = cut < 0 ? ByteView(whole) : ByteView(whole).first(std::min<std::size_t>(static_cast<std::size_t>(cut), whole.size()));
        const Json want = json::parse(expected_lines[i]);
        const std::string label = std::string(fields[0]) + " cut " + std::string(fields[2]);
        if (want.contains("ok")) {
            std::string got;
            try {
                got = fields[1] == "@default" ? xe::to_fx(data) : xe::to_fx(data, fields[1]);
            } catch (const Error& e) {
                failures += label + ": " + e.what() + "\n";
                continue;
            }
            const std::string old = text::replace_all(want["ok"].get<std::string>(), "(decompile/xbox_effect.py)", "(decompile/xbox_effect)");
            if (got != old) {
                // report the first differing line
                const auto a = text::split(got, '\n'), b = text::split(old, '\n');
                std::string diff = label + ": differs";
                for (std::size_t k = 0; k < std::max(a.size(), b.size()); ++k) {
                    const std::string_view x = k < a.size() ? a[k] : "<end>", y = k < b.size() ? b[k] : "<end>";
                    if (x != y) {
                        diff += std::format(" at line {}: new [{}] old [{}]", k + 1, x, y);
                        break;
                    }
                }
                failures += diff + "\n";
            }
            ++ok;
        } else {
            bool refused_it = false;
            try {
                (void)(fields[1] == "@default" ? xe::to_fx(data) : xe::to_fx(data, fields[1]));
            } catch (const FormatError&) {   // (the translator's Unsupported is one)
                refused_it = true;
            }
            if (!refused_it) failures += label + ": the original refused it, this did not\n";
            ++refused;
        }
    }
    CB_CHECK_EQ(failures, "");
    CB_CHECK_EQ(ok, 111u);
    CB_CHECK_EQ(refused, 373u);
});
