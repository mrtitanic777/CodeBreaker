// Tests of the XNB converter (cb/xna/xnb.hpp). testdata/xnb/ holds an .xnb of each kind the converter handles (made by a script that
// writes them as XNA's content pipeline lays them out: a texture in both byte orders, DXT1 in both word orders, a format that is not
// converted, a sprite font, PCM sounds for the PC and for the console, a type the converter only lists, two real Xbox 360 effects taken from
// games' content, junk and an empty file) and the recorded result for each: its record (python_records.json), the
// pixels of its PNG (*.pixels: width, height, the RGBA bytes in hex), its WAV, its .fx and .fx.json, its font JSON.
//
// Compressed content (LZX frames) is tested on a real game's files by the xna command's proof on Arcadecraft (see run_test / the report).
//
// The text of an error is not compared (the message is the reader's own), only that the record names the file and carries an "error"; JSON files are written with a two-space indent.
#include "cb/xna/xnb.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/java/rgba_png.hpp"

using namespace cb;
namespace xnb = cb::xna::xnb;

namespace {

namespace stdfs = std::filesystem;

stdfs::path xt_data(std::string_view name) { return testing::data_path("xna/testdata/xnb/" + std::string(name)); }

// The pixels of a PNG as the goldens write them: "width height\nhex".
// An effect's HLSL as the goldens have it: they name the module that wrote them by its source file.
std::string xt_fx(const std::string& text) { return text::replace_all(text, "decompile/xbox_effect.py)", "decompile/xbox_effect)"); }

std::string xt_pixels(const stdfs::path& png) {
    const java::rgba_png::Image image = java::rgba_png::decode(fs::read_file(png));
    std::string hex;
    for (const std::uint8_t b : image.rgba) hex += std::format("{:02x}", b);
    return std::format("{} {}\n{}\n", image.width, image.height, hex);
}

// A record as JSON, the text of an error left out.
Json xt_record(const std::string& text) {
    Json record = json::parse(text);
    if (record.contains("error")) record["error"] = "<error>";
    return record;
}

struct XtConverted {
    fs::ScopedTempDir dir{"cb-xnb"};
    Json record;
    stdfs::path out() const { return dir.path(); }
};

// Convert one sample, and check its record against the recorded one.
XtConverted xt_convert(const std::string& name, const std::string& file = {}) {
    XtConverted c;
    const std::string input = file.empty() ? name + ".xnb" : file;
    c.record = xt_record(xnb::convert(xt_data(input), c.dir.path()));
    Json recorded = json::load(xt_data("python_records.json"));
    Json want = recorded.at(name);
    if (want.contains("error")) want["error"] = "<error>";
    CB_CHECK_EQ(c.record.dump(), want.dump());
    return c;
}

}  // namespace

CB_TEST("xna.xnb.textures_in_both_byte_orders", [] {
    for (const char* name : {"texture_w", "texture_x"}) {
        const XtConverted c = xt_convert(name);
        CB_CHECK_EQ(xt_pixels(c.out() / (std::string(name) + ".png")), fs::read_text(xt_data(std::string(name) + ".pixels")));
    }
    // and the pixels are the ones the sample was made of (red, translucent blue-green, two near-black ones), in either order
    const std::string pixels = fs::read_text(xt_data("texture_x.pixels"));
    CB_CHECK(pixels.starts_with("2 2\nff0000ff0080ff40010203" "04fafbfcfd"));
});

CB_TEST("xna.xnb.dxt1_in_both_word_orders", [] {
    for (const char* name : {"dxt1_w", "dxt1_x"}) {
        const XtConverted c = xt_convert(name);
        CB_CHECK_EQ(xt_pixels(c.out() / (std::string(name) + ".png")), fs::read_text(xt_data(std::string(name) + ".pixels")));
    }
});

CB_TEST("xna.xnb.a_surface_format_that_is_not_converted_is_reported", [] {
    const XtConverted c = xt_convert("texture_bgr565_w");
    CB_CHECK(c.record.contains("error"));
    CB_CHECK(!stdfs::exists(c.out() / "texture_bgr565_w.png"));
});

CB_TEST("xna.xnb.a_sprite_font", [] {
    const XtConverted c = xt_convert("font_w");
    CB_CHECK_EQ(xt_pixels(c.out() / "font_w.png"), fs::read_text(xt_data("font_w.pixels")));
    CB_CHECK(json::load(c.out() / "font_w.json") == json::load(xt_data("font_w.json")));
});

CB_TEST("xna.xnb.pcm_sounds_become_wav_files", [] {
    for (const char* name : {"sound_w", "sound_x"}) {
        const XtConverted c = xt_convert(name);
        CB_CHECK(fs::read_file(c.out() / (std::string(name) + ".wav")) == fs::read_file(xt_data(std::string(name) + ".wav")));
    }
});

CB_TEST("xna.xnb.an_xma_sound_needs_ffmpeg", [] {
    // the PCM sample with its format tag changed to XMA2 (0x0166): with no ffmpeg it is reported, not written
    Bytes raw = fs::read_file(xt_data("sound_w.xnb"));
    const xnb::Xnb x = xnb::read(raw);
    CB_CHECK_EQ(x.readers.size(), std::size_t{1});
    const xnb::SoundEffect sound = xnb::read_sound_effect(x.data, x.offset, x.platform);
    CB_CHECK_EQ(sound.tag, 1u);
    xnb::SoundEffect xma = sound;
    xma.tag = 0x166;
    fs::ScopedTempDir dir{"cb-xnb"};
    CB_CHECK_THROWS_AS(ToolError, xnb::write_wav(xma, dir.path() / "x.wav", {}));
});

CB_TEST("xna.xnb.xbox_effects_become_hlsl", [] {
    for (const char* name : {"effect0_x", "effect1_x"}) {
        const XtConverted c = xt_convert(name);
        CB_CHECK_EQ(fs::read_text(c.out() / (std::string(name) + ".fx")), xt_fx(fs::read_text(xt_data(std::string(name) + ".fx"))));
        CB_CHECK(json::load(c.out() / (std::string(name) + ".fx.json")) == json::load(xt_data(std::string(name) + ".fx.json")));
    }
});

CB_TEST("xna.xnb.an_effect_on_its_own", [] {
    for (const char* name : {"effect0_raw", "effect1_raw"}) {
        fs::ScopedTempDir dir{"cb-xnb"};
        const std::optional<std::string> record = xnb::convert_effect_file(xt_data(std::string(name) + ".fxo"), dir.path());
        CB_CHECK(record.has_value());
        const Json recorded = json::load(xt_data("python_records.json")).at(name);
        CB_CHECK_EQ(json::parse(*record).dump(), recorded.dump());
        CB_CHECK_EQ(fs::read_text(dir.path() / (std::string(name) + ".fx")), xt_fx(fs::read_text(xt_data(std::string(name) + ".fx"))));
    }
    fs::ScopedTempDir dir{"cb-xnb"};
    CB_CHECK(!xnb::convert_effect_file(xt_data("texture_w.xnb"), dir.path()).has_value());   // not an effect: left to the .xnb converter
});

CB_TEST("xna.xnb.other_types_are_listed_and_junk_is_reported", [] {
    CB_CHECK_EQ(xt_convert("other_w").record.value("kind", ""), "other");
    CB_CHECK(xt_convert("junk").record.contains("error"));
    CB_CHECK_EQ(xt_convert("empty").record.value("kind", ""), "empty");
});

CB_TEST("xna.xnb.reading_and_renaming_readers", [] {
    const Bytes raw = fs::read_file(xt_data("texture_x.xnb"));
    const xnb::Xnb x = xnb::read(raw);
    CB_CHECK_EQ(x.platform, 'x');
    CB_CHECK_EQ(x.version, std::uint8_t{5});
    CB_CHECK_EQ(x.readers.size(), std::size_t{1});
    CB_CHECK_EQ(x.readers[0], "Microsoft.Xna.Framework.Content.Texture2DReader");
    CB_CHECK_EQ(xnb::short_name(x.readers[0]), "Texture2D");
    CB_CHECK_EQ(xnb::short_name("Microsoft.Xna.Framework.Content.ListReader`1[[X, Y]]"), "List");
    CB_CHECK(!xnb::rename_readers(raw, [](const std::string& n) { return n; }).has_value());
    const auto renamed = xnb::rename_readers(raw, [](const std::string& n) { return text::replace_all(n, "Texture2DReader", "SomeOtherTextureReader"); });
    CB_CHECK(renamed.has_value());
    const xnb::Xnb y = xnb::read(*renamed);
    CB_CHECK_EQ(y.readers[0], "Microsoft.Xna.Framework.Content.SomeOtherTextureReader");
    CB_CHECK_EQ(y.platform, 'x');
    CB_CHECK(std::equal(x.data.begin() + static_cast<std::ptrdiff_t>(x.offset), x.data.end(), y.data.begin() + static_cast<std::ptrdiff_t>(y.offset), y.data.end()));   // the object as it was
});

CB_TEST("xna.xnb.damaged_files_are_format_errors", [] {
    CB_CHECK_THROWS_AS(FormatError, xnb::read(fs::read_file(xt_data("junk.xnb"))));
    Bytes cut = fs::read_file(xt_data("texture_w.xnb"));
    cut.resize(cut.size() - 8);   // the pixels end early
    CB_CHECK_THROWS_AS(FormatError, xnb::read_texture(xnb::read(cut).data, xnb::read(cut).offset));
});

// Real content of a game (Arcadecraft: textures and a font in LZX frames, an effect): the recorded result for each file.
CB_TEST("xna.xnb.real_compressed_content_matches_the_golden", [] {
    const Json recorded = json::load(testing::data_path("xna/testdata/xnb_real/python_records.json"));
    for (const auto& [file, entry] : recorded.items()) {
        const stdfs::path input = testing::data_path("xna/testdata/xnb_real/" + file);
        const std::string name = file.substr(0, file.size() - 4);
        fs::ScopedTempDir dir{"cb-xnb"};
        Json want = entry.at("record");
        want.erase("folder");
        const Json got = json::parse(xnb::convert(input, dir.path()));
        CB_CHECK_EQ(got.dump(), want.dump());
        if (entry.at("compressed").get<bool>()) CB_CHECK((fs::read_file(input)[5] & 0x80) != 0);   // the content really is compressed
        const stdfs::path pixels = testing::data_path("xna/testdata/xnb_real/" + name + ".pixels");
        if (stdfs::exists(pixels)) CB_CHECK_EQ(xt_pixels(dir.path() / (name + ".png")), fs::read_text(pixels));
        const stdfs::path fx = testing::data_path("xna/testdata/xnb_real/" + name + ".fx");
        if (stdfs::exists(fx)) {
            CB_CHECK_EQ(fs::read_text(dir.path() / (name + ".fx")), xt_fx(fs::read_text(fx)));
            CB_CHECK(json::load(dir.path() / (name + ".fx.json")) == json::load(testing::data_path("xna/testdata/xnb_real/" + name + ".fx.json")));
        }
        const stdfs::path font = testing::data_path("xna/testdata/xnb_real/" + name + ".json");
        if (stdfs::exists(font)) CB_CHECK(json::load(dir.path() / (name + ".json")) == json::load(font));
    }
});
