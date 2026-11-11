#include "cb/java/console/xact.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

// ---- text forms the goldens use ---------------------------------------------------------------------------------

// A list as Python shows it: ['a', 'b'].
std::string list_text(const std::vector<std::string>& v) {
    std::string out = "[";
    for (std::size_t i = 0; i < v.size(); ++i) out += (i ? ", '" : "'") + v[i] + "'";
    return out + "]";
}

std::string xwb_text(const xact::Xwb& w) {
    std::string out = std::format("xwb {} magic={} version={} header_version={} name='{}' flags={} entry_count={}", w.file, w.magic,
                                  w.version, w.header_version, w.name, w.flags, w.entry_count);
    for (std::size_t i = 0; i < xact::kSegmentCount; ++i)
        out += std::format("\n  segment {} {} {}", xact::segment_name(static_cast<xact::SegmentId>(i)), w.segments[i].offset, w.segments[i].length);
    for (const xact::WaveEntry& e : w.entries)
        out += std::format("\n  [{}] {} {}ch {}Hz block={} {}bit play={}+{}{}", e.index, xact::codec_name(e.format.codec), e.format.channels,
                           e.format.sample_rate, e.format.block_align, e.format.bits, e.play_offset, e.play_length,
                           e.name ? " name=" + *e.name : std::string());
    return out;
}

xact::SoundDemand demand() {
    return {{"mob.pig", "random bow", "random.click", "random.door.close", "step.grass"}, {"grass", "stone"}};
}

// The files below `root` as "== path\ncontent" blocks (a folder's files by name, then its folders), with `from` written `to`.
void tree_text(const stdfs::path& root, const stdfs::path& dir, const std::string& from, std::vector<std::string>& rows) {
    std::vector<std::string> files, dirs;
    for (const auto& e : stdfs::directory_iterator(dir))
        (e.is_directory() ? dirs : files).push_back(fs::to_text(e.path().filename()));
    std::sort(files.begin(), files.end());
    std::sort(dirs.begin(), dirs.end());
    for (const std::string& f : files)
        rows.push_back("== " + fs::to_text((dir / f).lexically_relative(root)) + "\n" +
                       text::replace_all(test::lf(fs::read_text(dir / f)), from, "<RES>"));
    for (const std::string& d : dirs) tree_text(root, dir / d, from, rows);
}

// The Python (which listed a cue's events as the code mentioned them, repeats included) repeated "step.grass"; the new
// module lists each event once.
std::string without_repeated_events(std::string golden) { return text::replace_all(golden, "step.grass, step.grass", "step.grass"); }

// ---- banks -----------------------------------------------------------------------------------------------------

CB_TEST("java.console.xact.pcm_bank_matches_the_original_byte_for_byte", [] {
    fs::ScopedTempDir tmp("cbxact");
    const std::vector<stdfs::path> wavs = {test::data("xact/wav/step_grass.wav"), test::data("xact/wav/random_click.wav"),
                                           test::data("xact/wav/dig_stone.wav")};
    const stdfs::path out = tmp.path() / "sounds.golden.xwb";
    const std::vector<xact::PcmEntry> entries = xact::build_pcm_xwb(wavs, out);
    CB_CHECK(equal_bytes(fs::read_file(out), fs::read_file(test::data("xact/sounds.golden.xwb"))));   // the very bytes the Python wrote

    std::string got = "build_pcm_xwb:";
    for (const auto& e : entries) got += std::format(" {}:{}:{}:{}", e.name, e.mini, e.offset, e.length);
    got += "\n" + xwb_text(xact::read_xwb(out)) + "\n" + xact::dump(out) + "\n";
    const std::string first_three = test::golden("xact.xwb.golden.txt");
    CB_CHECK(first_three.starts_with(got));   // the golden goes on with the handmade banks

    // the handmade banks: one with entry names, one cut in the middle of its second entry, one with 70 entries
    std::string rest;
    for (const char* name : {"named.xwb", "cut.xwb"}) rest += xwb_text(xact::read_xwb(test::data(std::string("xact/") + name))) + "\n" +
                                                               (std::string(name) == "named.xwb" ? xact::dump(test::data("xact/named.xwb")) + "\n" : "");
    rest += xact::dump(test::data("xact/big.xwb")) + "\n";
    CB_CHECK_EQ(got + rest, first_three);
});

CB_TEST("java.console.xact.mini_formats", [] {
    const xact::MiniFormat m = xact::decode_miniformat(xact::pack_pcm_miniformat(2, 44100, 16));
    CB_CHECK(m.codec == xact::Codec::Pcm);
    CB_CHECK_EQ(m.channels, 2u);
    CB_CHECK_EQ(m.sample_rate, 44100u);
    CB_CHECK_EQ(m.block_align, 4u);
    CB_CHECK_EQ(m.bits, 16u);
    CB_CHECK_EQ(xact::decode_miniformat(xact::pack_pcm_miniformat(1, 11025, 8)).bits, 8u);
    // every field at its limit, and the four codecs
    const xact::MiniFormat top = xact::decode_miniformat(0xFFFFFFFF);
    CB_CHECK(top.codec == xact::Codec::Wma);
    CB_CHECK_EQ(top.channels, 7u);
    CB_CHECK_EQ(top.sample_rate, 0x3FFFFu);
    CB_CHECK_EQ(top.block_align, 255u);
    CB_CHECK_EQ(top.bits, 16u);
    CB_CHECK_EQ(std::string(xact::codec_name(xact::decode_miniformat(1).codec)), "XMA");
    CB_CHECK_EQ(std::string(xact::codec_name(xact::decode_miniformat(2).codec)), "ADPCM");
    CB_CHECK_EQ(std::string(xact::codec_name(xact::decode_miniformat(3).codec)), "WMA");
    // PCM parameters that do not fit their fields lose the excess instead of spilling into the next field
    CB_CHECK_EQ(xact::decode_miniformat(xact::pack_pcm_miniformat(9, 0x40000 + 5, 16)).channels, 1u);
    CB_CHECK_EQ(xact::decode_miniformat(xact::pack_pcm_miniformat(9, 0x40000 + 5, 16)).sample_rate, 5u);
});

CB_TEST("java.console.xact.xwb_errors", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_xwb(as_bytes("RIFFxxxxxxxxxxxx"), "a.xwb"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_xwb({}, "empty.xwb"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_xwb(as_bytes("DN"), "tiny.xwb"));
    // a header cut short, before the bank data it points at
    Bytes header = fs::read_file(test::data("xact/named.xwb"));
    header.resize(30);
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)xact::parse_xwb(header, "cut.xwb"));
    header.resize(100);   // the bank data begins at 52 but is cut at 100 (its name table is not all there)
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)xact::parse_xwb(header, "cut.xwb"));
    // a PC bank (little-endian magic) is accepted by name, read big-endian as the format has always been here
    Bytes pc = fs::read_file(test::data("xact/named.xwb"));
    pc[0] = 'W'; pc[1] = 'B'; pc[2] = 'N'; pc[3] = 'D';
    CB_CHECK_EQ(xact::parse_xwb(pc, "pc.xwb").magic, "WBND");
    // a huge entry count and a tiny file: the list ends where the file does
    Bytes many = fs::read_file(test::data("xact/sounds.golden.xwb"));
    many[52 + 4] = 0xFF; many[52 + 5] = 0xFF; many[52 + 6] = 0xFF; many[52 + 7] = 0xFF;
    const xact::Xwb w = xact::parse_xwb(many, "many.xwb");
    CB_CHECK_EQ(w.entry_count, 0xFFFFFFFFu);
    CB_CHECK(w.entries.size() < 1000);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)xact::dump("sounds.wav"));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)xact::dump(test::data("xact/missing.xwb")));
});

CB_TEST("java.console.xact.sound_banks_match_the_original", [] {
    std::string got;
    for (const char* n : {"sounds.xsb", "pc.xsb", "empty.xsb"}) {
        const xact::Xsb s = xact::read_xsb(test::data(std::string("xact/") + n));
        got += std::format("xsb {} magic={} name='{}' cues={}\n", s.file, s.magic, s.name, list_text(s.cue_names));
        got += xact::dump(test::data(std::string("xact/") + n)) + "\n";
    }
    CB_CHECK_EQ(got, test::golden("xact.xsb.golden.txt"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_xsb(as_bytes("KBDX0123456789abcdef"), "bad.xsb"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_xsb({}, "empty.xsb"));
    // names are printable runs of three or more characters; the bank-name repeats are not cues
    using namespace std::string_literals;
    const std::string table = "SDBK"s + std::string(12, '\0') + "bank\0\x01xy\0abc\0bank\0\x7f"s + "def\0long name here\0"s;
    const xact::Xsb s = xact::parse_xsb(as_bytes(table), "x.xsb");
    CB_CHECK_EQ(s.name, "bank");
    CB_CHECK_EQ(list_text(s.cue_names), "['abc', 'def', 'long name here']");
});

// ---- the demand --------------------------------------------------------------------------------------------------

CB_TEST("java.console.xact.cue_names", [] {
    CB_CHECK_EQ(xact::cue_name("step.grass"), "step_grass");
    CB_CHECK_EQ(xact::cue_name("random.door.close"), "random_door_close");
    CB_CHECK_EQ(xact::cue_name("random bow"), "random_bow");
    CB_CHECK_EQ(xact::cue_name("  a.. b\t.c  "), "a_b_c");        // runs of dots and white space are one underscore; the ends are trimmed
    CB_CHECK_EQ(xact::cue_name("calm1"), "calm1");
    CB_CHECK_EQ(xact::cue_name(".x"), "_x");
    CB_CHECK_EQ(xact::cue_name(""), "");
    // the table is the same whatever order the source was scanned in, and each event is listed once
    xact::SoundDemand shuffled = demand();
    std::reverse(shuffled.sounds.begin(), shuffled.sounds.end());
    shuffled.sounds.push_back("step.grass");
    std::reverse(shuffled.step_sounds.begin(), shuffled.step_sounds.end());
    CB_CHECK(xact::needed_cues(shuffled) == xact::needed_cues(demand()));
    CB_CHECK_EQ(xact::needed_cues(demand()).at("step_grass").size(), std::size_t{1});   // "step.grass" asked for twice: once
    CB_CHECK(xact::needed_cues({}).empty());
});

CB_TEST("java.console.xact.coverage_and_sources_match_the_original", [] {
    std::string got = "needed:\n";
    const xact::Needed needed = xact::needed_cues(demand());
    for (const auto& [cue, events] : needed) got += std::format("  {} <- {}\n", cue, text::join(events, ", "));
    const xact::CueCoverage cov = xact::cue_coverage(demand(), xact::read_xsb(test::data("xact/sounds.xsb")));
    got += std::format("coverage authored={} missing={} unused={}\n", list_text(cov.authored), list_text(cov.missing), list_text(cov.unused));
    got += xact::render_coverage(cov) + "\n";
    const stdfs::path res = test::data("xact/res");
    got += "ogg_keys:\n";
    for (const char* p : {"newsound/step/grass1.ogg", "random/door_close.ogg", "Sound/x.ogg", "sound/a_b/c_d2.ogg", "streaming/x9.OGG", "music.ogg",
                          "sound.ogg", "plain", "a/b/c12", "x_.ogg", "_a.ogg", "resources/z.ogg", "newsound/newsound/q1.ogg", "12.ogg"})
        got += std::format("  {} -> {}\n", p, list_text(xact::ogg_keys(p)));
    got += "index:\n";
    for (const auto& [key, files] : xact::scan_ogg_sources(res)) got += std::format("  {} -> {}\n", key, list_text(files));
    const xact::Match m = xact::match_sources(demand(), res);
    got += "matched:\n";
    for (const auto& [cue, source] : m.matched) got += std::format("  {} event={} files={}\n", cue, source.event, list_text(source.files));
    got += "missing:\n";
    for (const auto& [cue, events] : m.missing) got += std::format("  {} <- {}\n", cue, list_text(events));
    got += std::format("index_keys={}\n", m.index_keys);
    CB_CHECK_EQ(got, without_repeated_events(test::golden("xact.sources.golden.txt")));
});

CB_TEST("java.console.xact.dig_reuses_the_step_samples", [] {
    fs::ScopedTempDir tmp("cbxact");
    fs::write_text(tmp.path() / "newsound" / "step" / "stone1.ogg", "");
    fs::write_text(tmp.path() / "newsound" / "dig" / "grass1.ogg", "");   // a real dig sample is preferred
    const xact::Match m = xact::match_sources({{}, {"stone", "grass", "wood"}}, tmp.path());
    CB_CHECK_EQ(m.matched.at("dig_stone").files.front(), "newsound/step/stone1.ogg");
    CB_CHECK_EQ(m.matched.at("dig_grass").files.front(), "newsound/dig/grass1.ogg");
    CB_CHECK_EQ(m.matched.at("dig_stone").event, "dig.stone");
    CB_CHECK(m.missing.contains("dig_wood") && m.missing.contains("step_wood") && m.missing.contains("step_grass"));
    CB_CHECK(!xact::match_sources({{}, {"x"}}, tmp.path() / "nowhere").matched.size());   // no resources: nothing matched, nothing thrown
});

// ---- authoring --------------------------------------------------------------------------------------------------

CB_TEST("java.console.xact.authored_files_match_the_original", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path res = test::data("xact/res");
    const stdfs::path ff = tmp.path() / "ff";
    fs::make_dirs(ff);
    const xact::Match m = xact::match_sources(demand(), res);
    xact::write_ffmpeg_script(m, ff, res);
    xact::write_build_script(ff);
    xact::write_build_script(ff, "music", "music_banks.bat");
    xact::write_xap(ff / "cue_list.xap", {"step_grass", "random_click", "dig_stone"});
    xact::XapOptions music;
    music.bank = "music";
    music.sound_category = "Music";
    music.streaming = true;
    xact::write_xap(ff / "music_like.xap", {"calm3", "calm1"}, music);
    // the wave folder decides which cues are authored: only those whose .wav is there, unless none is
    fs::write_text(ff / "gated" / "wav" / "a.wav", "");
    fs::write_text(ff / "gated" / "wav" / "c.wav", "");
    CB_CHECK_EQ(list_text(xact::write_xap(ff / "gated" / "g.xap", {"c", "b", "a"})), "['a', 'c']");
    CB_CHECK_EQ(list_text(xact::write_xap(ff / "gated" / "none_present.xap", {"x", "y"})), "['x', 'y']");
    xact::XapOptions dotted;
    dotted.wave_dir = "other";
    dotted.cue_namer = [](std::string_view c) { return text::replace_all(c, "_", "."); };
    xact::write_xap(ff / "gated" / "dotted.xap", {"step_grass"}, dotted);

    std::vector<std::string> rows;
    tree_text(ff, ff, res.u8string(), rows);   // the script has the folder as it was handed in
    CB_CHECK_EQ(text::join(rows, "\n") + "\n", test::golden("xact.files.golden.txt"));
    // written with the Windows line ends the XDK tools write; the bat files with one CRLF per line
    const std::string bat = fs::read_text(ff / "build_banks.bat");
    CB_CHECK(bat.starts_with("@echo off\r\nrem Build the XACT"));
    CB_CHECK(bat.find("\r\r") == std::string::npos);
    CB_CHECK(fs::read_text(ff / "cue_list.xap").find("\r\n") != std::string::npos);
});

CB_TEST("java.console.xact.music_and_build_match_the_original", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path res = test::data("xact/res");
    const std::string res_text = res.u8string();
    const stdfs::path mdir = tmp.path() / "music_out";
    fs::make_dirs(mdir);
    const std::vector<std::string> found = xact::build_music(mdir, res);
    const stdfs::path none = tmp.path() / "nomusic";
    fs::make_dirs(none);
    const std::vector<std::string> found_none = xact::build_music(none, test::data("xact/wav"));
    CB_CHECK(stdfs::is_empty(none));   // no track found: nothing is written
    const stdfs::path bdir = tmp.path() / "build_out";
    xact::BuildOptions options;
    options.resources_dir = res;
    const xact::BuildResult built = xact::build(demand(), bdir, options);

    std::vector<std::string> rows = {std::format("music found={} none={}", list_text(found), list_text(found_none)),
                                     std::format("build cues={}", list_text(built.cues)),
                                     std::format("build xap={}", fs::to_text(built.xap.filename())), "-- music_out"};
    std::vector<std::string> a, b;
    tree_text(mdir, mdir, res_text, a);
    tree_text(bdir, bdir, res_text, b);
    rows.push_back(text::join(a, "\n"));
    rows.push_back("-- build_out");
    rows.push_back(text::join(b, "\n"));
    CB_CHECK_EQ(text::join(rows, "\n") + "\n", without_repeated_events(test::golden("xact.build.golden.txt")));
});

CB_TEST("java.console.xact.clean_intermediates", [] {
    fs::ScopedTempDir tmp("cbxact");
    for (const char* f : {"sounds.xwb", "sounds.xsb", "x.bat", "y.xap", "z.h", "g.xgs", "n.md", "t.txt", "keep.dat", "wav/a.wav"})
        fs::write_text(tmp.path() / "built" / f, "x");
    for (const char* f : {"y.xap", "t.txt", "wav/a.wav"}) fs::write_text(tmp.path() / "unbuilt" / f, "x");
    xact::clean_audio_intermediates(tmp.path() / "built");
    xact::clean_audio_intermediates(tmp.path() / "unbuilt");
    auto left = [&](const char* d) {
        std::vector<std::string> out;
        for (const auto& e : stdfs::recursive_directory_iterator(tmp.path() / d))
            if (e.is_regular_file()) out.push_back(fs::to_text(e.path().lexically_relative(tmp.path() / d)));
        std::sort(out.begin(), out.end());
        return list_text(out);
    };
    CB_CHECK_EQ("banks built: " + left("built") + "\nno banks: " + left("unbuilt") + "\n", test::golden("xact.clean.golden.txt"));
    xact::clean_audio_intermediates(tmp.path() / "nowhere");   // nothing there: nothing to do
});

CB_TEST("java.console.xact.xap_details", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path xap = tmp.path() / "x.xap";
    // each cue's wave and sound index is its position in the sorted list, and repeats are authored once
    const std::vector<std::string> cues = xact::write_xap(xap, {"b", "a", "b", "c"});
    CB_CHECK_EQ(list_text(cues), "['a', 'b', 'c']");
    const std::string t = test::lf(fs::read_text(xap));
    for (int i = 0; i < 3; ++i) {
        CB_CHECK(t.find(std::format("Entry Index = {};", i)) != std::string::npos);
        CB_CHECK(t.find(std::format("            Index = {};", i)) != std::string::npos);
    }
    CB_CHECK(t.find("Entry Index = 3;") == std::string::npos);
    CB_CHECK(t.find("Streaming = 1;") == std::string::npos);                  // not streaming unless asked
    CB_CHECK(t.find("Xbox File = sounds.xwb;") != std::string::npos);
    CB_CHECK(t.ends_with("}\n"));
    CB_CHECK_EQ(list_text(xact::write_xap(xap, {})), "[]");                   // no cues: an empty bank, no failure
});

// ---- wave files -----------------------------------------------------------------------------------------------

Bytes make_wav(std::uint16_t tag, std::uint16_t channels, std::uint32_t rate, std::uint16_t bits, const Bytes& samples,
               bool with_fmt = true, std::uint32_t data_size_claim = 0xFFFFFFFF) {
    io::ByteWriter fmt;
    fmt.u16(tag);
    fmt.u16(channels);
    fmt.u32(rate);
    fmt.u32(rate * channels * bits / 8);
    fmt.u16(static_cast<std::uint16_t>(channels * bits / 8));
    fmt.u16(bits);
    io::ByteWriter w;
    w.bytes(as_bytes("RIFF"));
    w.u32(0);
    w.bytes(as_bytes("WAVE"));
    if (with_fmt) {
        w.bytes(as_bytes("fmt "));
        w.u32(static_cast<std::uint32_t>(fmt.size()));
        w.bytes(fmt.data());
    }
    w.bytes(as_bytes("LIST"));
    w.u32(3);                       // an odd-sized chunk is followed by a pad byte
    w.bytes(as_bytes("abc"));
    w.u8(0);
    w.bytes(as_bytes("data"));
    w.u32(data_size_claim == 0xFFFFFFFF ? static_cast<std::uint32_t>(samples.size()) : data_size_claim);
    w.bytes(samples);
    return w.take();
}

CB_TEST("java.console.xact.wav_reader", [] {
    const Bytes pcm = {1, 2, 3, 4, 5, 6, 7, 8};
    const xact::Wav w = xact::parse_wav(make_wav(1, 2, 44100, 16, pcm), "a.wav");
    CB_CHECK_EQ(w.channels, 2u);
    CB_CHECK_EQ(w.rate, 44100u);
    CB_CHECK_EQ(w.bits, 16u);
    CB_CHECK(w.data == pcm);
    CB_CHECK_EQ(xact::parse_wav(make_wav(0xFFFE, 6, 48000, 24, pcm)).channels, 6u);      // WAVE_FORMAT_EXTENSIBLE
    // a data chunk that claims more than the file holds gives what is there
    CB_CHECK(xact::parse_wav(make_wav(1, 1, 8000, 8, pcm, true, 1000)).data == pcm);
    // the real files of the tests, as the original read them
    const xact::Wav step = xact::parse_wav(fs::read_file(test::data("xact/wav/step_grass.wav")));
    CB_CHECK_EQ(step.rate, 22050u);
    CB_CHECK_EQ(step.data.size(), std::size_t{200});
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_wav(as_bytes("not a wave file at all"), "x"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_wav({}, "x"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xact::parse_wav(make_wav(1, 1, 8000, 8, pcm, false), "nofmt"));
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)xact::parse_wav(make_wav(3, 1, 8000, 32, pcm), "float"));   // IEEE float is not PCM
    Bytes cut = make_wav(1, 1, 8000, 8, pcm);
    cut.resize(12 + 8 + 10);   // the fmt chunk is cut short
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)xact::parse_wav(cut, "cut"));
});

CB_TEST("java.console.xact.pcm_bank_details", [] {
    fs::ScopedTempDir tmp("cbxact");
    const Bytes data(5, 0x11);
    fs::write_file(tmp.path() / "a.wav", make_wav(1, 1, 8000, 8, data));
    fs::write_file(tmp.path() / "b.wav", make_wav(1, 2, 16000, 16, Bytes(2049, 0x22)));
    const std::string long_name(80, 'n');
    const auto entries = xact::build_pcm_xwb({tmp.path() / "a.wav", tmp.path() / "b.wav"}, tmp.path() / "o.xwb", long_name);
    CB_CHECK_EQ(entries.size(), std::size_t{2});
    CB_CHECK_EQ(entries[0].offset, 0u);
    CB_CHECK_EQ(entries[1].offset, 2048u);                       // each wave starts on a 2048 boundary
    CB_CHECK_EQ(entries[1].length, 2049u);
    const xact::Xwb back = xact::read_xwb(tmp.path() / "o.xwb");
    CB_CHECK_EQ(back.name, std::string(64, 'n'));                // the name field is 64 bytes
    CB_CHECK_EQ(back.entries.size(), std::size_t{2});
    CB_CHECK_EQ(back.entries[1].format.channels, 2u);
    CB_CHECK_EQ(back.entries[1].format.block_align, 4u);
    CB_CHECK_EQ(back.segments[static_cast<std::size_t>(xact::SegmentId::EntryWaveData)].offset % 2048, 0u);
    CB_CHECK_EQ(back.segments[static_cast<std::size_t>(xact::SegmentId::EntryWaveData)].length, 2048u + 2049u);
    // the audio ends where the segment says
    const Bytes file = fs::read_file(tmp.path() / "o.xwb");
    const auto wave_data = back.segments[static_cast<std::size_t>(xact::SegmentId::EntryWaveData)];
    CB_CHECK_EQ(file.size(), std::size_t{wave_data.offset} + wave_data.length);
    CB_CHECK_EQ(file[wave_data.offset], 0x11);
    CB_CHECK_EQ(file[wave_data.offset + 2048], 0x22);
    // no waves: an empty bank that reads back as one
    xact::build_pcm_xwb({}, tmp.path() / "empty.xwb");
    CB_CHECK(xact::read_xwb(tmp.path() / "empty.xwb").entries.empty());
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)xact::build_pcm_xwb({tmp.path() / "missing.wav"}, tmp.path() / "x.xwb"));
});

// ---- finding and running -------------------------------------------------------------------------------------------

CB_TEST("java.console.xact.find_resources_dir", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path java = tmp.path() / "a" / "b" / "c";
    fs::make_dirs(java);
    CB_CHECK(!xact::find_resources_dir(java));
    fs::make_dirs(tmp.path() / "a" / "resources" / "textures");
    CB_CHECK(!xact::find_resources_dir(java));                                  // a resources folder needs a sound folder
    fs::make_dirs(tmp.path() / "a" / "resources" / "sound");
    CB_CHECK_PATH(*xact::find_resources_dir(java), tmp.path() / "a" / "resources");
    fs::make_dirs(tmp.path() / "a" / "b" / "game" / "resources" / "newsound");
    CB_CHECK_PATH(*xact::find_resources_dir(java), tmp.path() / "a" / "b" / "game" / "resources");   // nearer wins
    // five levels, counting the folder itself
    const stdfs::path deep = tmp.path() / "x" / "1" / "2" / "3" / "4" / "5";
    fs::make_dirs(deep);
    fs::make_dirs(tmp.path() / "x" / "resources" / "sound");
    CB_CHECK(!xact::find_resources_dir(deep));
    CB_CHECK(xact::find_resources_dir(deep.parent_path()).has_value());
});

CB_TEST("java.console.xact.run_build_skips_what_cannot_run", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path out = tmp.path() / "out";
    fs::make_dirs(out);
    fs::ScopedEnv no_tools("PATH", fs::to_text(tmp.path() / "nothing-here"));
    fs::ScopedEnv no_xedk("XEDK", std::nullopt);
    // nothing written yet
    xact::RunResult r = xact::run_build(out);
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.lines.size(), std::size_t{4});
    CB_CHECK_EQ(r.lines[0], "[audio] skip convert_audio.bat (not generated)");
    CB_CHECK_EQ(r.lines[3], "[audio] skip music_banks.bat (not generated)");
    // scripts there but no tools: skipped with the reason, and never a failure
    for (const char* f : {"convert_audio.bat", "music_convert.bat", "build_banks.bat", "music_banks.bat"}) fs::write_text(out / f, "@echo off\r\n");
    std::vector<std::string> logged;
    r = xact::run_build(out, std::nullopt, [&](std::string_view l) { logged.emplace_back(l); });
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.lines[0], "[audio] SKIP convert_audio.bat -- ffmpeg (add it to PATH) not found; the .bat is written, run it later.");
    CB_CHECK_EQ(r.lines[2], "[audio] SKIP build_banks.bat -- XACTBLD (set %XEDK% / install the Xbox 360 SDK) not found; the .bat is written, run it later.");
    CB_CHECK(logged == r.lines);                                                 // the log callback saw every line
    // only the named scripts are considered
    r = xact::run_build(out, std::nullopt, {}, std::vector<std::string>{"music_banks.bat"});
    CB_CHECK_EQ(r.lines.size(), std::size_t{1});
});

CB_TEST("java.console.xact.run_build_runs_the_scripts", [] {
    fs::ScopedTempDir tmp("cbxact");
    const stdfs::path out = tmp.path() / "out";
    fs::make_dirs(out);
    // a stand-in ffmpeg on PATH, and an XDK whose xactbld3.exe only has to exist (the script does not call it)
    fs::write_text(tmp.path() / "bin" / "ffmpeg.cmd", "@echo off\r\n");
    const std::string original_path = fs::get_env("PATH").value_or("");
    fs::ScopedEnv path("PATH", fs::to_text(tmp.path() / "bin") + ";" + original_path);
    fs::ScopedEnv no_xedk("XEDK", std::nullopt);
    fs::write_text(tmp.path() / "xdk" / "bin" / "win32" / "xactbld3.exe", "");
    fs::write_text(out / "convert_audio.bat", "@echo off\r\necho converted> converted.txt\r\n");        // runs in out_dir
    fs::write_text(out / "build_banks.bat", "@echo off\r\necho XEDK=%XEDK%\r\nexit /b 3\r\n");        // fails, and says what it saw
    const xact::RunResult r = xact::run_build(out, tmp.path() / "xdk", {}, std::vector<std::string>{"convert_audio.bat", "build_banks.bat"});
    CB_CHECK(!r.ok);                                                                                     // one script failed
    CB_CHECK_EQ(r.lines[0], "[audio] convert_audio.bat -> exit 0");
    CB_CHECK(stdfs::exists(out / "converted.txt"));
    CB_CHECK_EQ(r.lines[1], "[audio] build_banks.bat -> exit 3");
    CB_CHECK(r.lines[2].find("XEDK=" + fs::to_text(tmp.path() / "xdk")) != std::string::npos);        // the child got XEDK from sdk_root
    CB_CHECK(!fs::get_env("XEDK").has_value());                                                          // and our own environment did not
    // an XEDK the environment already has is not replaced
    fs::ScopedEnv own_xedk("XEDK", fs::to_text(tmp.path() / "xdk"));
    const xact::RunResult again = xact::run_build(out, tmp.path() / "other", {}, std::vector<std::string>{"build_banks.bat"});
    CB_CHECK(again.lines[1].find(fs::to_text(tmp.path() / "xdk")) != std::string::npos);
    CB_CHECK(again.lines[1].find("other") == std::string::npos);
});

}  // namespace
