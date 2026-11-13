// Tests of the XACT readers (cb/xna/xact.hpp), against what the translated Python's xwb / xsb / xma made of the same files (testdata/xact/):
//   Wave Bank / Music Wave Bank   the first 64 KB of Arcadecraft's two wave banks (every entry's format is in the header), their entry tables
//   Sound Bank.xsb                Arcadecraft's sound bank whole: 106 cues over two wave banks
//   test_bank_be / _le            wave banks written for the test (PCM, XMA and WMA streams) in each byte order, and what decode_bank wrote of
//                                 them with no ffmpeg (the PCM stream as a WAV, the others raw)
//   python_wraps.json             a raw XMA2 stream wrapped in RIFF-WAVE(XMA2), three ways
// The XMA decode itself is ffmpeg's and is not run here.
#include "cb/xna/xact.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace xact = cb::xna::xact;

namespace {

namespace stdfs = std::filesystem;

stdfs::path xat_data(std::string_view name) { return testing::data_path("xna/testdata/xact/" + std::string(name)); }
Json xat_golden(const std::string& name) { return json::load(xat_data("python_banks.json")).at(name); }

Json xat_entries(const xact::WaveBank& bank) {
    Json out = Json::object();
    out["bank_name"] = bank.name;
    out["version"] = bank.version;
    out["big"] = bank.big_endian;
    out["entry_count"] = bank.entries.size();
    Json entries = Json::array();
    for (const xact::WaveEntry& e : bank.entries) {
        entries.push_back({{"index", e.index}, {"codec_name", std::string(xact::codec_name(e.codec))}, {"channels", e.channels}, {"rate", e.rate},
                           {"align", e.align}, {"offset", e.offset}, {"length", e.length}});
    }
    out["entries"] = entries;
    return out;
}

}  // namespace

CB_TEST("xna.xact.the_entries_of_real_wave_banks_match_the_python", [] {
    for (const char* name : {"wave_bank_head.xwb", "music_wave_bank_head.xwb"}) {
        const xact::WaveBank bank = xact::read_wave_bank(fs::read_file(xat_data(name)));
        Json want = xat_golden(name);
        CB_CHECK_EQ(xat_entries(bank).dump(), want.dump());
        CB_CHECK(bank.big_endian);   // the console's
    }
});

CB_TEST("xna.xact.test_banks_in_both_byte_orders", [] {
    for (const char* name : {"test_bank_be.xwb", "test_bank_le.xwb"}) {
        const xact::WaveBank bank = xact::read_wave_bank(fs::read_file(xat_data(name)));
        Json want = xat_golden(name);
        want.erase("decoded");
        CB_CHECK_EQ(xat_entries(bank).dump(), want.dump());
        CB_CHECK_EQ(bank.name, "TestBank");
        CB_CHECK_EQ(bank.entries.size(), std::size_t{3});
        CB_CHECK(bank.stream(bank.entries[1]).size() == 64 && bank.stream(bank.entries[1])[5] == 5);   // the XMA stream is what was put there
    }
});

CB_TEST("xna.xact.decode_bank_without_ffmpeg_keeps_what_it_cannot_decode_raw", [] {
    for (const char* side : {"be", "le"}) {
        fs::ScopedTempDir dir{"cb-xact"};
        std::vector<std::string> said;
        const std::size_t written = xact::decode_bank(xat_data(std::string("test_bank_") + side + ".xwb"), dir.path(), std::nullopt, [&](std::string_view line) { said.emplace_back(line); });
        CB_CHECK_EQ(written, std::size_t{1});   // the PCM stream, as a WAV
        CB_CHECK_EQ(said.size(), std::size_t{1});
        CB_CHECK(said[0].starts_with("  stream 1: XMA decode failed"));
        for (const char* file : {"stream_000.wav", "stream_001.xma", "stream_002.wma"}) {
            const stdfs::path want = xat_data(std::string("decoded_") + side + "_" + file);
            CB_CHECK(fs::read_file(dir.path() / file) == fs::read_file(want));
        }
    }
});

CB_TEST("xna.xact.xma2_in_a_riff_container_matches_the_python", [] {
    const Json wraps = json::load(xat_data("python_wraps.json"));
    Bytes data(100);
    for (std::size_t i = 0; i < data.size(); ++i) data[i] = static_cast<std::uint8_t>(i);
    const auto hex = [](const Bytes& b) {
        std::string out;
        for (const std::uint8_t byte : b) out += std::format("{:02x}", byte);
        return out;
    };
    CB_CHECK_EQ(hex(xact::wrap_xma2(data, 2, 44100)), wraps.at(0).get<std::string>());
    CB_CHECK_EQ(hex(xact::wrap_xma2(data, 0, 0, 64)), wraps.at(1).get<std::string>());   // (no channels, no rate: stereo at 44.1 kHz)
    CB_CHECK_EQ(hex(xact::wrap_xma2(data, 5, 22050, 32)), wraps.at(2).get<std::string>());
});

CB_TEST("xna.xact.a_real_sound_bank_matches_the_python", [] {
    const xact::SoundBank bank = xact::read_sound_bank(fs::read_file(xat_data("sound_bank.xsb")));
    const Json want = xat_golden("sound_bank.xsb");
    CB_CHECK_EQ(bank.wave_banks.size(), std::size_t{2});
    CB_CHECK_EQ(bank.cues.size(), std::size_t{106});
    CB_CHECK(json::parse(xact::cues_json(bank)) == want);
});

CB_TEST("xna.xact.damaged_banks_are_format_errors", [] {
    CB_CHECK_THROWS_AS(FormatError, xact::read_wave_bank(Bytes{'n', 'o', 'p', 'e', 0, 0, 0, 0}));
    CB_CHECK_THROWS_AS(FormatError, xact::read_sound_bank(Bytes{'n', 'o', 'p', 'e'}));
    Bytes cut = fs::read_file(xat_data("test_bank_be.xwb"));
    cut.resize(40);   // the header is cut inside its segment table
    CB_CHECK_THROWS_AS(FormatError, xact::read_wave_bank(cut));
    Bytes soundbank = fs::read_file(xat_data("sound_bank.xsb"));
    soundbank.resize(60);
    CB_CHECK_THROWS_AS(FormatError, xact::read_sound_bank(soundbank));
});

CB_TEST("xna.xact.pcm_is_wrapped_as_wav", [] {
    const Bytes pcm = {0x12, 0x34, 0x56, 0x78};   // two big-endian samples
    const Bytes wav = xact::pcm_to_wav(pcm, 2, 44100);
    CB_CHECK_EQ(wav.size(), std::size_t{44 + 4});
    CB_CHECK(wav[44] == 0x34 && wav[45] == 0x12 && wav[46] == 0x78 && wav[47] == 0x56);   // swapped to the little-endian order of a WAV
    CB_CHECK_THROWS_AS(ToolError, xact::decode_to_wav(Bytes{1, 2, 3}, stdfs::temp_directory_path() / "cb-never.wav", std::nullopt));
});

// The banks of Minecraft's Xbox 360 builds under C:/Nexia360/Games (skipped where they are not): the 360 Edition's minecraft.xsb and resident.xwb,
// Indev's sounds and music banks; what the Python's xsb.read and xwb.XWB made of them.
CB_TEST("xna.xact.minecrafts_banks_match_the_python", [] {
    const Json python = json::load(xat_data("python_minecraft_banks.json"));
    const std::vector<std::pair<std::string, std::string>> files = {
        {"mc360_minecraft.xsb", "Minecraft Xbox 360 Edition/res/audio/minecraft.xsb"}, {"indev_sounds.xsb", "MinecraftIndev/Content/resources/sounds.xsb"},
        {"indev_music.xsb", "MinecraftIndev/Content/resources/music.xsb"},             {"mc360_resident.xwb", "Minecraft Xbox 360 Edition/res/audio/resident.xwb"},
        {"indev_sounds.xwb", "MinecraftIndev/Content/resources/sounds.xwb"},           {"indev_music.xwb", "MinecraftIndev/Content/resources/music.xwb"}};
    for (const auto& [name, relative] : files) {
        const auto file = testing::game_file(relative.c_str());
        if (!file) continue;
        if (name.ends_with(".xsb")) {
            CB_CHECK(json::parse(xact::cues_json(xact::read_sound_bank(fs::read_file(*file)))) == python.at(name));
        } else {
            CB_CHECK_EQ(xat_entries(xact::read_wave_bank(fs::read_file(*file))).dump(), python.at(name).dump());
        }
    }
});
