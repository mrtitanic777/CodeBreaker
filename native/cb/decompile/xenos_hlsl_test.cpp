#include "cb/decompile/xenos_hlsl.hpp"

#include <cmath>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace x = cb::decompile::xenos_hlsl;
using std::string;
using std::string_view;

// DIVERGES from the Python module (the old harness: 7,872 generated shaders, 442 real and damaged containers, 6,000 calls of
// the small functions; the text of every translation that completes is the original's, and the checked-in SDK shaders'
// are its output):
//  - a shader whose stage has the other kind of input table (Python: ValueError from tuple unpacking) cannot be built: the
//    types keep vertex inputs and pixel inputs apart;
//  - microcode that executes slots past its end is a FormatError (Python: ValueError from unpacking a short slice);
//  - a matrix constant with no rows or columns is Unsupported (Python: ZeroDivisionError);
//  - `number_text` of a double past the float range is 1e38, as an infinity is (Python: OverflowError).

namespace {

using Words = std::vector<std::uint32_t>;

// 48-bit control-flow instructions (two to three words), then the 3-word slots they address.
Words program(std::vector<std::uint64_t> cfs, const std::vector<std::array<std::uint32_t, 3>>& slots) {
    if (cfs.size() % 2) cfs.push_back(0);
    Words w;
    for (std::size_t i = 0; i < cfs.size(); i += 2) {
        const std::uint64_t a = cfs[i], b = cfs[i + 1];
        w.push_back(static_cast<std::uint32_t>(a & 0xFFFFFFFF));
        w.push_back(static_cast<std::uint32_t>((a >> 32) | ((b & 0xFFFF) << 16)));
        w.push_back(static_cast<std::uint32_t>(((b >> 16) & 0xFFFF) | ((b >> 32) << 16)));
    }
    for (const auto& s : slots) w.insert(w.end(), s.begin(), s.end());
    return w;
}

std::uint64_t cf_exec(unsigned op, unsigned address, unsigned count, unsigned sequence) {
    return address | (std::uint64_t{count} << 12) | (std::uint64_t{sequence} << 16) | (std::uint64_t{op} << 44);
}

constexpr std::uint32_t kNopScalar = 50u << 26;   // "retain previous": reads nothing, writes nothing

x::Constant constant(string name, x::RegisterSet set, std::uint32_t reg, x::ConstantClass cls = x::ConstantClass::Vector) {
    x::Constant c;
    c.name = std::move(name);
    c.set = set;
    c.first_register = reg;
    c.count = 1;
    c.cls = cls;
    c.rows = 1;
    c.columns = 4;
    c.elements = 1;
    return c;
}

x::Shader shader(x::Stage stage, Words words) {
    x::Shader s;
    s.stage = stage;
    s.words = std::move(words);
    return s;
}

x::Constant read_constant(const cb::Json& j) {
    x::Constant c;
    c.name = j["name"].get<string>();
    const string set = j["set"].get<string>();
    c.set = set == "bool" ? x::RegisterSet::Bool : set == "int4" ? x::RegisterSet::Int4 : set == "float4" ? x::RegisterSet::Float4 : x::RegisterSet::Sampler;
    c.first_register = j["register"].get<std::uint32_t>();
    c.count = j["count"].get<std::uint32_t>();
    const string cls = j["cls"].get<string>();
    c.cls = cls == "scalar" ? x::ConstantClass::Scalar : cls == "vector" ? x::ConstantClass::Vector
          : cls == "matrix_rows" ? x::ConstantClass::MatrixRows : cls == "matrix_columns" ? x::ConstantClass::MatrixColumns : x::ConstantClass::Other;
    c.is_bool = j["type"].get<string>() == "bool";
    c.rows = j["rows"].get<std::uint32_t>();
    c.columns = j["columns"].get<std::uint32_t>();
    c.elements = j["elements"].get<std::uint32_t>();
    if (!j["default"].is_null()) c.defaults = j["default"].get<std::vector<float>>();
    return c;
}

}  // namespace

CB_TEST("decompile.xenos_hlsl.number_text", [] {
    CB_CHECK_EQ(x::number_text(0.5), "0.5");
    CB_CHECK_EQ(x::number_text(3.0), "3.0");
    CB_CHECK_EQ(x::number_text(-2.5), "-2.5");
    CB_CHECK_EQ(x::number_text(-0.0), "-0.0");
    CB_CHECK_EQ(x::number_text(255.0), "255.0");
    CB_CHECK_EQ(x::number_text(0.0625), "0.0625");
    CB_CHECK_EQ(x::number_text(0.001), "0.001");
    CB_CHECK_EQ(x::number_text(0.1), "0.1");                      // a float32 that reads back as itself
    CB_CHECK_EQ(x::number_text(1.0 / 3.0), "0.33333334");
    CB_CHECK_EQ(x::number_text(1e-5), "1e-05");
    CB_CHECK_EQ(x::number_text(100000.0), "1e+05");
    CB_CHECK_EQ(x::number_text(12000.0), "1.2e+04");
    CB_CHECK_EQ(x::number_text(16777216.0), "16777216.0");
    CB_CHECK_EQ(x::number_text(1234567.0), "1234567.0");
    CB_CHECK_EQ(x::number_text(3.4e38), "3.4e+38");
    CB_CHECK_EQ(x::number_text(std::numeric_limits<double>::quiet_NaN()), "(0.0 / 0.0)");
    CB_CHECK_EQ(x::number_text(std::numeric_limits<double>::infinity()), "1e38");
    CB_CHECK_EQ(x::number_text(-std::numeric_limits<double>::infinity()), "-1e38");
    CB_CHECK_EQ(x::number_text(1e300), "1e38");   // (past the float range)
});

CB_TEST("decompile.xenos_hlsl.add_of_two_registers", [] {
    const std::array<std::uint32_t, 3> alu = {kNopScalar | (0xFu << 16) | 1, 0, (1u << 31) | (1u << 30) | (2u << 16) | (3u << 8)};
    const x::Function f = x::translate(shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 0)}, {alu})), "S_A");
    CB_CHECK_EQ(f.text,
                "struct S_A_Input\n{\n    float4 Unused : TEXCOORD15;\n};\n\nstruct S_A_Output\n{\n};\n\n"
                "S_A_Output S_A(S_A_Input input)\n{\n    S_A_Output output = (S_A_Output)0;\n    float4 r1 = 0.0, r2 = 0.0, r3 = 0.0;\n"
                "    r1.xyzw = r2.xyzw + r3.xyzw;\n    return output;\n}\n");
    CB_CHECK(f.samplers.empty());
    CB_CHECK(f.inputs.empty());
    CB_CHECK(!f.needs_helpers);
});

CB_TEST("decompile.xenos_hlsl.pixel_export_with_an_interpolator_and_a_constant", [] {
    const std::array<std::uint32_t, 3> alu = {kNopScalar | (0xFu << 16) | (1u << 15), 0, (1u << 24) | (1u << 31) | (1u << 16)};
    x::Shader s = shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 0)}, {alu}));
    s.constants.push_back(constant("Tint", x::RegisterSet::Float4, 0));
    s.pixel_inputs.push_back({1, "TEXCOORD", 0, 3});
    const x::Function f = x::translate(s, "S_B");
    CB_CHECK_EQ(f.text,
                "struct S_B_Input\n{\n    float4 TexCoord0 : TEXCOORD0;\n};\n\nstruct S_B_Output\n{\n    float4 Color0 : COLOR0;\n};\n\n"
                "S_B_Output S_B(S_B_Input input)\n{\n    S_B_Output output = (S_B_Output)0;\n    float4 r1 = 0.0;\n"
                "    r1.xy = input.TexCoord0.xy;\n    output.Color0.xyzw = r1.xyzw * Tint.xyzw;\n    return output;\n}\n");
    CB_CHECK_EQ(f.inputs.size(), 1u);
    CB_CHECK_EQ(*f.inputs.find("TexCoord0"), "TEXCOORD0");
    CB_CHECK_EQ(*f.outputs.find("Color0"), "COLOR0");
});

CB_TEST("decompile.xenos_hlsl.vertex_position_export", [] {
    const std::array<std::uint32_t, 3> alu = {kNopScalar | (0xFu << 16) | (1u << 15) | 62, 0, (2u << 24) | (1u << 31) | (1u << 30)};
    const x::Function f = x::translate(shader(x::Stage::Vertex, program({cf_exec(2, 1, 1, 0)}, {alu})), "S_C");
    CB_CHECK(f.text.find("    float4 Position0 : POSITION0;\n") != string::npos);   // (no vertex fetch: a placeholder input)
    CB_CHECK(f.text.find("    float4 Position : POSITION0;\n") != string::npos);
    CB_CHECK(f.text.find("    output.Position.xyzw = r0.xyzw;\n") != string::npos);   // max(r0, r0) is r0
    CB_CHECK_EQ(*f.outputs.find("Position"), "POSITION0");
});

CB_TEST("decompile.xenos_hlsl.texture_fetch_names_its_sampler", [] {
    const std::array<std::uint32_t, 3> fetch = {1u | (1u << 5), (1u << 28) | 0x688, 1u << 14};
    x::Shader s = shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 1)}, {fetch}));
    s.constants.push_back(constant("Tex", x::RegisterSet::Sampler, 0));
    const x::Function f = x::translate(s, "S_D");
    CB_CHECK(f.text.find("    float4 t;\n    t = tex2D(Tex, r1.xx);\n    r0.xyzw = t.xyzw;\n") != string::npos);
    CB_CHECK_EQ(f.samplers.size(), 1u);
    CB_CHECK_EQ(f.samplers[0], "Tex");
    // a fetch constant no sampler is bound to
    x::Shader unbound = shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 1)}, {fetch}));
    CB_CHECK_THROWS_AS(x::Unsupported, (void)x::translate(unbound, "S"));
});

CB_TEST("decompile.xenos_hlsl.scalar_reciprocal", [] {
    const std::array<std::uint32_t, 3> alu = {(17u << 26) | (0x1u << 20), 0, (1u << 29) | 1u};
    const x::Function f = x::translate(shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 0)}, {alu})), "S_H");
    CB_CHECK(f.text.find("    r0.x = 1.0 / r1.w;\n") != string::npos);
});

CB_TEST("decompile.xenos_hlsl.what_it_refuses", [] {
    const std::array<std::uint32_t, 3> alu = {kNopScalar | (0xFu << 16) | 1, 0, (1u << 31) | (1u << 30) | (2u << 16) | (3u << 8)};
    // a call
    try {
        (void)x::translate(shader(x::Stage::Pixel, program({cf_exec(9, 1, 1, 0)}, {alu})), "S_F");
        CB_CHECK(false);
    } catch (const x::Unsupported& e) {
        CB_CHECK_EQ(e.slot(), 0u);
        CB_CHECK_EQ(e.shader(), "S_F");
        CB_CHECK(e.what_failed().find("control flow instruction 9") != string::npos);
    }
    // a constant that is in no table
    const std::array<std::uint32_t, 3> reads_c4 = {kNopScalar | (0x3u << 16) | 1, 0, (11u << 24) | (1u << 31) | (1u << 30) | (2u << 16) | (3u << 8) | 4};
    CB_CHECK_THROWS_AS(x::Unsupported, (void)x::translate(shader(x::Stage::Pixel, program({cf_exec(2, 1, 1, 0)}, {reads_c4})), "S_E"));
    // an exec past the end of the microcode
    CB_CHECK_THROWS_AS(cb::FormatError, (void)x::translate(shader(x::Stage::Pixel, program({cf_exec(2, 5, 1, 0)}, {alu})), "S_G"));
    // no microcode at all translates to an empty body
    const x::Function empty = x::translate(shader(x::Stage::Pixel, {}), "E");
    CB_CHECK(empty.text.find("return output;") != string::npos);
});

// The 16 shaders of the SDK's sample effects (tests/data/xbox_effects), read by the Python reader and translated by the
// Python original: the text is the original's, and was read through (registers, constants, predicates, a loop and forward
// jumps are among them).
CB_TEST("decompile.xenos_hlsl.sdk_shaders_translate_as_the_original_did", [] {
    const cb::Json all = cb::json::load(cb::testing::data_path("decompile/testdata/xenos_hlsl_sdk.json"));
    CB_CHECK_EQ(all.size(), 16u);
    for (const cb::Json& e : all) {
        const string name = e["name"].get<string>();
        x::Shader s;
        s.stage = e["stage"].get<string>() == "vertex" ? x::Stage::Vertex : x::Stage::Pixel;
        for (const cb::Json& c : e["constants"]) s.constants.push_back(read_constant(c));
        for (const cb::Json& v : e["vertex_inputs"]) s.vertex_inputs.push_back({v[0].get<std::uint32_t>(), v[1].get<string>(), v[2].get<std::uint32_t>()});
        for (const cb::Json& v : e["pixel_inputs"]) s.pixel_inputs.push_back({v[0].get<std::uint32_t>(), v[1].get<string>(), v[2].get<std::uint32_t>(), v[3].get<std::uint32_t>()});
        for (const cb::Json& v : e["outputs"]) s.outputs.push_back({v[0].get<std::uint32_t>(), v[1].get<string>(), v[2].get<std::uint32_t>(), v[3].get<std::uint32_t>()});
        for (const auto& [k, v] : e["literals"].items()) s.literals[static_cast<std::uint32_t>(std::stoul(k))] = {v[0].get<float>(), v[1].get<float>(), v[2].get<float>(), v[3].get<float>()};
        for (const auto& [k, v] : e["loop_literals"].items()) s.loop_literals[static_cast<std::uint32_t>(std::stoul(k))] = v.get<std::uint32_t>();
        s.words = e["words"].get<std::vector<std::uint32_t>>();
        const cb::Json& want = e["expected"];
        if (want.contains("unsupported")) {
            CB_CHECK_THROWS_AS(x::Unsupported, (void)x::translate(s, name));
            continue;
        }
        const x::Function f = x::translate(s, name);
        CB_CHECK_EQ(f.text, want["text"].get<string>());
        CB_CHECK_EQ(cb::text::join(f.samplers, ","), cb::text::join(want["samplers"].get<std::vector<string>>(), ","));
        CB_CHECK_EQ(f.needs_helpers, want["needs_helpers"].get<bool>());
        std::size_t i = 0;
        for (const auto& [field, semantic] : f.inputs) {
            CB_CHECK_EQ(field, want["inputs"][i][0].get<string>());
            CB_CHECK_EQ(semantic, want["inputs"][i][1].get<string>());
            ++i;
        }
        CB_CHECK_EQ(i, want["inputs"].size());
        i = 0;
        for (const auto& [field, semantic] : f.outputs) {
            CB_CHECK_EQ(field, want["outputs"][i][0].get<string>());
            CB_CHECK_EQ(semantic, want["outputs"][i][1].get<string>());
            ++i;
        }
        CB_CHECK_EQ(i, want["outputs"].size());
    }
});

CB_TEST("decompile.xenos_hlsl.helpers_text", [] {
    CB_CHECK(x::kHelpers.starts_with("// The Xbox 360 GPU's cube instruction"));
    CB_CHECK(x::kHelpers.find("float3 xe_cube_direction(float3 c)") != std::string_view::npos);
    CB_CHECK(x::kHelpers.ends_with("}\n"));
});

// The old harness of the Python module (state/work/typed-port/decompile.xenos_hlsl): 7,872 shaders built from the SDK's
// with generated microcode, edited constant tables and literals, each translated by the original; replayed when
// CB_XENOS_CORPUS names its folder (cases.txt and expected.txt, one case and one JSON answer per line). Not part of the
// normal run: the corpus is generated state, not source.
namespace {

struct Fields {
    std::vector<std::string_view> f;
    std::size_t i = 0;
    std::string_view next() { return f.at(i++); }
    long long num() { return std::stoll(string(next())); }
    string str() { return [&] { const std::string_view h = next().substr(1); string out; for (std::size_t k = 0; k + 1 < h.size(); k += 2) out += static_cast<char>(std::stoi(string(h.substr(k, 2)), nullptr, 16)); return out; }(); }
    double dbl() {
        const std::string_view h = next();
        std::uint64_t bits = std::stoull(string(h), nullptr, 16);
        double d;
        static_assert(sizeof d == sizeof bits);
        std::memcpy(&d, &bits, sizeof d);
        return d;
    }
};

}  // namespace

CB_TEST("decompile.xenos_hlsl.python_corpus", [] {
    const auto dir = cb::fs::get_env("CB_XENOS_CORPUS");
    if (!dir) return;
    const string cases = cb::fs::read_text(std::filesystem::path(*dir) / "cases.txt");
    const string answers = cb::fs::read_text(std::filesystem::path(*dir) / "expected.txt");
    const auto case_lines = cb::text::lines(cases);
    const auto answer_lines = cb::text::lines(answers);
    CB_CHECK_EQ(case_lines.size(), answer_lines.size());
    std::size_t checked = 0, differ = 0, arity = 0;
    std::vector<string> shown;
    for (std::size_t n = 0; n < case_lines.size() && n < answer_lines.size(); ++n) {
        if (!case_lines[n].starts_with("shd ")) continue;
        Fields in;
        in.f = cb::text::split(case_lines[n], ' ');
        in.next();
        const string name = in.str();
        x::Shader s;
        s.stage = in.next() == "vertex" ? x::Stage::Vertex : x::Stage::Pixel;
        const long long nconst = in.num();
        for (long long k = 0; k < nconst; ++k) {
            x::Constant c;
            c.name = in.str();
            const string set = in.str();
            c.set = set == "bool" ? x::RegisterSet::Bool : set == "int4" ? x::RegisterSet::Int4 : set == "float4" ? x::RegisterSet::Float4 : x::RegisterSet::Sampler;
            c.first_register = static_cast<std::uint32_t>(in.num());
            c.count = static_cast<std::uint32_t>(in.num());
            const string cls = in.str();
            c.cls = cls == "scalar" ? x::ConstantClass::Scalar : cls == "vector" ? x::ConstantClass::Vector
                  : cls == "matrix_rows" ? x::ConstantClass::MatrixRows : cls == "matrix_columns" ? x::ConstantClass::MatrixColumns : x::ConstantClass::Other;
            c.is_bool = in.str() == "bool";
            c.rows = static_cast<std::uint32_t>(in.num());
            c.columns = static_cast<std::uint32_t>(in.num());
            c.elements = static_cast<std::uint32_t>(in.num());
            in.num();   // members
            const std::string_view dl = in.next();
            if (dl != "-") {
                for (long long d = 0; d < std::stoll(string(dl)); ++d) c.defaults.push_back(static_cast<float>(in.dbl()));
            }
            s.constants.push_back(std::move(c));
        }
        const long long nlit = in.num();
        for (long long k = 0; k < nlit; ++k) {
            const auto reg = static_cast<std::uint32_t>(in.num());
            const float a = static_cast<float>(in.dbl()), b = static_cast<float>(in.dbl()), c = static_cast<float>(in.dbl()), d = static_cast<float>(in.dbl());
            s.literals[reg] = {a, b, c, d};
        }
        const long long nloop = in.num();
        for (long long k = 0; k < nloop; ++k) {
            const auto reg = static_cast<std::uint32_t>(in.num());
            s.loop_literals[reg] = static_cast<std::uint32_t>(in.num());
        }
        const bool pixel_kind = in.next() == "P";
        const long long nin = in.num();
        for (long long k = 0; k < nin; ++k) {
            const auto reg = static_cast<std::uint32_t>(in.num());
            string usage = in.str();
            const auto index = static_cast<std::uint32_t>(in.num());
            if (pixel_kind) s.pixel_inputs.push_back({reg, usage, index, static_cast<std::uint32_t>(in.num())});
            else s.vertex_inputs.push_back({reg, usage, index});
        }
        const long long nout = in.num();
        for (long long k = 0; k < nout; ++k) {
            const auto reg = static_cast<std::uint32_t>(in.num());
            string usage = in.str();
            const auto index = static_cast<std::uint32_t>(in.num());
            s.outputs.push_back({reg, usage, index, static_cast<std::uint32_t>(in.num())});
        }
        const long long nwords = in.num();
        for (long long k = 0; k < nwords; ++k) s.words.push_back(static_cast<std::uint32_t>(in.num()));
        // a Python ValueError was a shader whose stage has the other input table (not expressible here) or a short microcode
        const cb::Json want = cb::json::parse(answer_lines[n]);
        ++checked;
        string got;
        try {
            const x::Function f = x::translate(s, name);
            got = f.text;
            if (want.contains("text") && got == want["text"].get<string>()) continue;
        } catch (const x::Unsupported& e) {
            if (want.contains("error") && want["error"].get<string>() == "Unsupported") continue;
            if (want.contains("error") && want["error"].get<string>() == "ValueError" && string(e.what()).find("vertex fetch") != string::npos) {
                ++arity;   // (a vertex fetch in a pixel shader: the Python unpacked its 4-tuple input as a 3-tuple)
                continue;
            }
            got = string("<Unsupported> ") + e.what();
        } catch (const cb::Error& e) {
            // (FormatError is the Python's ValueError, any other Python error is a crash of its own)
            if (want.contains("error") && want["error"].get<string>() != "Unsupported") continue;
            got = string("<error ") + e.kind().data() + ">";
        }
        if (name.find("tuple") != string::npos) {   // (the other input table's arity: not expressible, see DIVERGES)
            ++arity;
            continue;
        }
        ++differ;
        if (shown.size() < 12) shown.push_back(std::format("case {} {}: want {}\n  got {}", n, name, want.dump().substr(0, 300), got.substr(0, 300)));
    }
    cb::testing::record_failure(__FILE__, __LINE__, std::format("corpus: {} shaders checked, {} differ, {} with the other input table's arity (not expressible)", checked, differ, arity));
    for (const string& s : shown) cb::testing::record_failure(__FILE__, __LINE__, s);
});
